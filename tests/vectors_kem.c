// SPDX-License-Identifier: MIT

// This tests the test vectors published by NIST ACVP

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>
#include <oqs/sha3.h>
#include "oqs/common.h"
#include "system_info.c"

#ifdef OQS_ENABLE_KEM_ML_KEM
/* macros for sanity checks for encaps and decaps key */
#define ML_KEM_POLYBYTES        384
#define ML_KEM_K_MAX            4
#define ML_KEM_N                256
#define ML_KEM_1024_PK_SIZE     1568
#define ML_KEM_Q                3329
#define SHA3_256_OP_LEN         32
#endif //OQS_ENABLE_KEM_ML_KEM

struct {
	const uint8_t *pos;
} prng_state = {
	.pos = 0
};

/* MLKEM-specific functions */
static inline bool is_ml_kem(const char *method_name) {
	return (0 == strcmp(method_name, OQS_KEM_alg_ml_kem_512))
	       || (0 == strcmp(method_name, OQS_KEM_alg_ml_kem_768))
	       || (0 == strcmp(method_name, OQS_KEM_alg_ml_kem_1024));
}

static void fprintBstr(FILE *fp, const char *S, const uint8_t *A, size_t L) {
	size_t i;
	fprintf(fp, "%s", S);
	for (i = 0; i < L; i++) {
		fprintf(fp, "%02X", A[i]);
	}
	if (L == 0) {
		fprintf(fp, "00");
	}
	fprintf(fp, "\n");
}

static uint8_t hexCharToDecimal(char c) {
	if (c >= '0' && c <= '9') {
		return (uint8_t) (c - '0');
	} else if (c >= 'a' && c <= 'f') {
		return (uint8_t) (c - 'a' + 10);
	} else if (c >= 'A' && c <= 'F') {
		return (uint8_t) (c - 'A' + 10);
	} else {
		fprintf(stderr, "Invalid hex character: %c\n", c);
		return 0;
	}
}

static void hexStringToByteArray(const char *hexString, uint8_t *byteArray) {
	size_t len = strlen(hexString);

	if (len % 2 != 0) {
		fprintf(stderr, "Hex string must have an even number of characters\n");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0, j = 0; i < len; i += 2, j++) {
		byteArray[j] = (uint8_t) ((hexCharToDecimal(hexString[i]) << 4) | hexCharToDecimal(hexString[i + 1]));
	}
}

#ifdef OQS_ENABLE_KEM_ML_KEM
/* barret reduction for mod(Q) */
int16_t barrett_reduce(int16_t a) {
	const int16_t v = ((1 << 26) + ML_KEM_Q / 2) / ML_KEM_Q;
	int32_t t = ((int32_t)v * a + (1 << 25)) >> 26;
	t *= ML_KEM_Q;
	a -= t;

	int16_t mask = a >> 15;
	a += (ML_KEM_Q & mask);
	return a;
}
/* fetch value of 'K' from ML-KEM version */
uint8_t get_ml_kem_k(const char *method) {
	if (0 == strcmp(method, OQS_KEM_alg_ml_kem_512)) {
		return 2;
	} else if (0 == strcmp(method, OQS_KEM_alg_ml_kem_768)) {
		return 3;
	} else if (0 == strcmp(method, OQS_KEM_alg_ml_kem_1024)) {
		return 4;
	} else {
		return 0;  // Default/error case
	}
}
/* sanity check for private/decaps key as specified in section 7.3 of FIPS-203 */
static inline bool sanityCheckSK(const uint8_t *sk, const char *method_name) {
	/* sanity checks */
	if ((NULL == sk) || (NULL == method_name) || (false == is_ml_kem(method_name))) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL or invalid method !\n", method_name);
		return false;
	}
	/* buffer to hold public key hash */
	uint8_t pkdig[SHA3_256_OP_LEN] = {0};
	/* fetch the value of k according to the ML-KEM algorithm as per FIPS-203
	K = 2 for ML-KEM-512, K = 3 for ML-KEM-768 & K = 4 for ML-KEM-1024 */
	uint8_t K = get_ml_kem_k(method_name);
	if (0 == K) {
		fprintf(stderr, "K value can be fetched only for ML-KEM !\n");
		return false;
	}
	/* calcualte hash of the public key(len = 384k+32) stored in private key at offset of 384k */
	OQS_SHA3_sha3_256(pkdig, sk + (ML_KEM_POLYBYTES * K), (ML_KEM_POLYBYTES * K) + 32);
	/* compare it with public key hash stored at 768k+32 offset */
	if (0 != memcmp(pkdig, sk + (ML_KEM_POLYBYTES * K * 2) + 32, SHA3_256_OP_LEN)) {
		return false;
	}
	return true;
}
/* sanity check for public/encaps key as specified in section 7.2 of FIPS-203 */
static inline bool sanityCheckPK(const uint8_t *pk, size_t pkLen, const char *method_name) {
	/* sanity checks */
	if ((NULL == pk) || (0 == pkLen) ||  (NULL == method_name) || (false == is_ml_kem(method_name))) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL or zero or invalid method !\n", method_name);
		return false;
	}
	unsigned int i, j;
	/* fetch the value of k according to the ML-KEM algorithm as per FIPS-203
	K = 2 for ML-KEM-512, K = 3 for ML-KEM-768 & K = 4 for ML-KEM-1024 */
	uint8_t K = get_ml_kem_k(method_name);
	if (0 == K) {
		fprintf(stderr, "K value can be fetched only for ML-KEM !\n");
		return false;
	}
	/* buffer to hold decoded value. max value used, so same buffer could be used for ML-KEM versions
	encaps key is of length 384K bytes(384K*8 bits). Grouped into 12-bit values, the buffer requires (384*K*8)/12 = 256*K entries of 12 bits */
	uint16_t buffd[ML_KEM_N * ML_KEM_K_MAX] = {0};
	/* buffer to hold encoded value */
	uint8_t buffe[ML_KEM_1024_PK_SIZE - 32] = {0};
	uint16_t *buff_dec;
	/* perform byte decoding as per Algo 6 of FIPS 203 */
	for (i = 0; i < K; i++) {
		buff_dec = &buffd[i * ML_KEM_N];
		const uint8_t *curr_pk = &pk[i * ML_KEM_POLYBYTES];
		for (j = 0; j < ML_KEM_N / 2; j++) {
			buff_dec[2 * j + 0] = ((curr_pk[3 * j + 0] >> 0) | ((uint16_t)curr_pk[3 * j + 1] << 8)) & 0xFFF;
			buff_dec[2 * j + 0] = (uint16_t)barrett_reduce((int16_t)buff_dec[2 * j]);
			buff_dec[2 * j + 1] = ((curr_pk[3 * j + 1] >> 4) | ((uint16_t)curr_pk[3 * j + 2] << 4)) & 0xFFF;
			buff_dec[2 * j + 1] = (uint16_t)barrett_reduce((int16_t)buff_dec[2 * j + 1]);
		}
	}
	/* perform byte encoding as per Algo 5 of FIPS 203 */
	for (i = 0; i < K; i++) {
		uint16_t t0, t1;
		buff_dec = &buffd[i * ML_KEM_N];
		uint8_t *buff_enc = &buffe[i * ML_KEM_POLYBYTES];
		for (j = 0; j < ML_KEM_N / 2; j++) {
			t0 = buff_dec[2 * j];
			t1 = buff_dec[2 * j + 1];
			buff_enc[3 * j + 0] = (uint8_t)(t0 >> 0);
			buff_enc[3 * j + 1] = (uint8_t)((t0 >> 8) | (t1 << 4));
			buff_enc[3 * j + 2] = (uint8_t)(t1 >> 4);
		}
	}
	/* compare the encoded value with original public key. discard value of `rho(32 bytes)` during comparision as its not encoded */
	if (0 != memcmp(buffe, pk, pkLen - 32)) {
		return false;
	}
	return true;
}
#endif //OQS_ENABLE_KEM_ML_KEM

static void MLKEM_randombytes_init(const uint8_t *entropy_input, const uint8_t *personalization_string) {
	(void) personalization_string;
	prng_state.pos = entropy_input;
}

static void MLKEM_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	memcpy(random_array, prng_state.pos, bytes_to_read);
	prng_state.pos += bytes_to_read;
}

static void MLKEM_randombytes_free(void) {
	prng_state.pos = 0;
}

static OQS_STATUS kem_kg_vector(const char *method_name,
                                uint8_t *prng_output_stream,
                                const uint8_t *kg_pk, const uint8_t *kg_sk) {

	uint8_t *entropy_input;
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

	void (*randombytes_init)(const uint8_t *, const uint8_t *) = NULL;
	void (*randombytes_free)(void) = NULL;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (is_ml_kem(method_name)) {
		OQS_randombytes_custom_algorithm(&MLKEM_randombytes);
		randombytes_init = &MLKEM_randombytes_init;
		randombytes_free = &MLKEM_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else {
		// Only ML-KEM supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	fh = stdout;

	public_key = OQS_MEM_malloc(kem->length_public_key);
	secret_key = OQS_MEM_malloc(kem->length_secret_key);

	if ((public_key == NULL) || (secret_key == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((prng_output_stream == NULL) || (kg_pk == NULL) || (kg_sk == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	rc = OQS_KEM_keypair(kem, public_key, secret_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "ek: ", public_key, kem->length_public_key);
	fprintBstr(fh, "dk: ", secret_key, kem->length_secret_key);

#ifdef OQS_ENABLE_KEM_ML_KEM
	if ((false == sanityCheckPK(public_key, kem->length_public_key, method_name)) || (false == sanityCheckSK(secret_key, method_name))) {
		fprintf(stderr, "[vectors_kem] %s ERROR: generated public key or private key are corrupted !\n", method_name);
		goto err;
	}
#endif //OQS_ENABLE_KEM_ML_KEM

	if (!memcmp(public_key, kg_pk, kem->length_public_key) && !memcmp(secret_key, kg_sk, kem->length_secret_key)) {
		ret = OQS_SUCCESS;
	} else {
		ret = OQS_ERROR;
		fprintf(stderr, "[vectors_kem] %s ERROR: public key or private key doesn't match!\n", method_name);
	}
	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
	}
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_MEM_insecure_free(public_key);
	OQS_KEM_free(kem);
	return ret;
}

static OQS_STATUS kem_vector_encdec_aft(const char *method_name,
                                        uint8_t *prng_output_stream,
                                        const uint8_t *encdec_pk,
                                        const uint8_t *encdec_k, const uint8_t *encdec_c) {

	uint8_t *entropy_input;
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *ss_encaps = NULL;
	uint8_t *ct_encaps = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

	void (*randombytes_init)(const uint8_t *, const uint8_t *) = NULL;
	void (*randombytes_free)(void) = NULL;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (is_ml_kem(method_name)) {
		OQS_randombytes_custom_algorithm(&MLKEM_randombytes);
		randombytes_init = &MLKEM_randombytes_init;
		randombytes_free = &MLKEM_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else {
		// Only ML-KEM supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	fh = stdout;

	ss_encaps = OQS_MEM_malloc(kem->length_shared_secret);
	ct_encaps = OQS_MEM_malloc(kem->length_ciphertext);
	if ((ss_encaps == NULL) || (ct_encaps == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((prng_output_stream == NULL) || (encdec_pk == NULL) || (encdec_k == NULL) || (encdec_c == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

#ifdef OQS_ENABLE_KEM_ML_KEM
	if (false == sanityCheckPK(encdec_pk, kem->length_public_key, method_name)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: passed encapsulation key is corrupted !\n", method_name);
		goto err;
	}
#endif //OQS_ENABLE_KEM_ML_KEM

	rc = OQS_KEM_encaps(kem, ct_encaps, ss_encaps, encdec_pk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
		goto err;
	}

	fprintBstr(fh, "c: ", ct_encaps, kem->length_ciphertext);
	fprintBstr(fh, "k: ", ss_encaps, kem->length_shared_secret);

	if (!memcmp(ct_encaps, encdec_c, kem->length_ciphertext) && !memcmp(ss_encaps, encdec_k, kem->length_shared_secret)) {
		ret = OQS_SUCCESS;
	} else {
		ret = OQS_ERROR;
		fprintf(stderr, "[vectors_kem] %s ERROR (AFT): ciphertext or shared secret doesn't match!\n", method_name);
	}

	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(ss_encaps, kem->length_shared_secret);
	}
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_MEM_insecure_free(ct_encaps);
	OQS_KEM_free(kem);
	return ret;
}

static OQS_STATUS kem_vector_encdec_val(const char *method_name,
                                        const uint8_t *encdec_sk, const uint8_t *encdec_c,
                                        const uint8_t *encdec_k) {
	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *ss_decaps = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	fh = stdout;

	ss_decaps = OQS_MEM_malloc(kem->length_shared_secret);

	if (ss_decaps == NULL) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((encdec_sk == NULL) || (encdec_k == NULL) || (encdec_c == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

#ifdef OQS_ENABLE_KEM_ML_KEM
	if (false == sanityCheckSK(encdec_sk, method_name)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: passed decapsulation key is corrupted !\n", method_name);
		goto err;
	}
#endif //OQS_ENABLE_KEM_ML_KEM

	rc = OQS_KEM_decaps(kem, ss_decaps, encdec_c, encdec_sk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
		goto err;
	}

	fprintBstr(fh, "k: ", ss_decaps, kem->length_shared_secret);

	if (!memcmp(ss_decaps, encdec_k, kem->length_shared_secret)) {
		ret = OQS_SUCCESS;
	} else {
		ret = OQS_ERROR;
		fprintf(stderr, "[vectors_kem] %s ERROR (AFT): shared secret doesn't match!\n", method_name);
	}

	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(ss_decaps, kem->length_shared_secret);
	}
	OQS_KEM_free(kem);
	return ret;
}

static OQS_STATUS kem_strcmp_vector(const char *method_name,
                                    uint8_t *seed, const uint8_t *ekExpected,
                                    const uint8_t *c, const uint8_t *kExpected) {

	uint8_t *entropy_input;
	OQS_KEM *kem = NULL;
	OQS_STATUS rc = OQS_ERROR, ret = OQS_ERROR;
	void (*randombytes_init)(const uint8_t *, const uint8_t *) = NULL;
	void (*randombytes_free)(void) = NULL;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	uint8_t *ek = OQS_MEM_malloc(kem->length_public_key);
	uint8_t *dk = OQS_MEM_malloc(kem->length_secret_key);
	uint8_t *k = OQS_MEM_malloc(kem->length_shared_secret);

	if ((ek == NULL) || (k == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((seed == NULL) || (ekExpected == NULL) || (c == NULL) || (kExpected == NULL)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	if (is_ml_kem(method_name)) {
		OQS_randombytes_custom_algorithm(&MLKEM_randombytes);
		randombytes_init = &MLKEM_randombytes_init;
		randombytes_free = &MLKEM_randombytes_free;
		entropy_input = seed;
	} else {
		// Only ML-KEM supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	rc = OQS_KEM_keypair(kem, ek, dk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_keypair failed!\n", method_name);
		goto err;
	}

	// check ek
	if (memcmp(ek, ekExpected, kem->length_public_key)) {
		rc = false;
		goto err;
	}

	// perform decapsulation
	rc = OQS_KEM_decaps(kem, k, c, dk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_encaps failed!\n", method_name);
		goto err;
	}

	// check k
	if (memcmp(k, kExpected, kem->length_shared_secret)) {
		rc = false;
		goto err;
	}

	ret = rc;

	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(ek, kem->length_public_key);
		OQS_MEM_secure_free(k, kem->length_shared_secret);
	}
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_KEM_free(kem);
	return ret;
}

static OQS_STATUS kem_modOverflow_vector(const char *method_name, const uint8_t *ek) {
	OQS_KEM *kem = NULL;
	OQS_STATUS ret = OQS_ERROR;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (ek == NULL) {
		fprintf(stderr, "[vectors_kem] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

#ifdef OQS_ENABLE_KEM_ML_KEM
	if (true == sanityCheckPK(ek, kem->length_public_key, method_name)) {
		fprintf(stderr, "[vectors_kem] %s ERROR: Modulus flow not detected !\n", method_name);
		goto err;
	}
	ret = 0;
#endif //OQS_ENABLE_KEM_ML_KEM

	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_KEM_free(kem);
	}
	return ret;
}

int main(int argc, char **argv) {
	OQS_STATUS rc = OQS_SUCCESS;

	OQS_init();

	if (argc != 4 && argc != 6 && argc != 7) {
		fprintf(stderr, "Usage: vectors_kem algname testname [testargs]\n");
		for (size_t i = 0; i < OQS_KEM_algs_length; i++) {
			if (i > 0) {
				fprintf(stderr, ", ");
			}
			fprintf(stderr, "%s", OQS_KEM_alg_identifier(i));
		}
		fprintf(stderr, "\n");
		printf("\n");
		print_system_info();
		OQS_destroy();

		return EXIT_FAILURE;
	}

	char *alg_name = argv[1];
	char *test_name = argv[2];
	char *prng_output_stream;
	char *kg_pk;
	char *kg_sk;
	char *encdec_aft_pk;
	char *encdec_aft_k;
	char *encdec_aft_c;

	char *encdec_val_sk;
	char *encdec_val_k;
	char *encdec_val_c;

	char *strcmp_seed;
	char *strcmp_ek;
	char *strcmp_c;
	char *strcmp_k;

	char *modOverflow_ek;

	uint8_t *prng_output_stream_bytes = NULL;
	uint8_t *kg_pk_bytes = NULL;
	uint8_t *kg_sk_bytes = NULL;

	uint8_t *encdec_aft_pk_bytes = NULL;
	uint8_t *encdec_aft_k_bytes = NULL;
	uint8_t *encdec_aft_c_bytes = NULL;

	uint8_t *encdec_val_sk_bytes = NULL;
	uint8_t *encdec_val_k_bytes = NULL;
	uint8_t *encdec_val_c_bytes = NULL;

	uint8_t *strcmp_seed_bytes = NULL;
	uint8_t *strcmp_ek_bytes = NULL;
	uint8_t *strcmp_c_bytes = NULL;
	uint8_t *strcmp_k_bytes = NULL;

	uint8_t *modOverflow_ek_bytes = NULL;

	OQS_KEM *kem = OQS_KEM_new(alg_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", alg_name);
		rc = OQS_ERROR;
		goto err;
	}

	if (!strcmp(test_name, "keyGen")) {
		prng_output_stream = argv[3]; // d || z : both should be 32 bytes each as per FIPS-203
		kg_pk = argv[4];
		kg_sk = argv[5];

		if (strlen(prng_output_stream) != 128 ||
		        strlen(kg_pk) != 2 * kem->length_public_key ||
		        strlen(kg_sk) != 2 * kem->length_secret_key) {
			rc = OQS_ERROR;
			goto err;
		}

		prng_output_stream_bytes = OQS_MEM_malloc(strlen(prng_output_stream) / 2);
		kg_pk_bytes = OQS_MEM_malloc(kem->length_public_key);
		kg_sk_bytes = OQS_MEM_malloc(kem->length_secret_key);

		if ((prng_output_stream_bytes == NULL) || (kg_pk_bytes == NULL) || (kg_sk_bytes == NULL)) {
			fprintf(stderr, "[vectors_kem] ERROR: OQS_MEM_malloc failed!\n");
			rc = OQS_ERROR;
			goto err;
		}

		hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
		hexStringToByteArray(kg_pk, kg_pk_bytes);
		hexStringToByteArray(kg_sk, kg_sk_bytes);


		rc = kem_kg_vector(alg_name, prng_output_stream_bytes, kg_pk_bytes, kg_sk_bytes);
	} else if (!strcmp(test_name, "encDecAFT")) {
		prng_output_stream = argv[3]; // m : should be 32 bytes as per FIPS-203
		encdec_aft_pk = argv[4];
		encdec_aft_k = argv[5];
		encdec_aft_c = argv[6];

		if (strlen(prng_output_stream) != 64 ||
		        strlen(encdec_aft_c) != 2 * kem->length_ciphertext ||
		        strlen(encdec_aft_k) != 2 * kem->length_shared_secret ||
		        strlen(encdec_aft_pk) != 2 * kem->length_public_key) {
			rc = OQS_ERROR;
			goto err;
		}

		prng_output_stream_bytes = OQS_MEM_malloc(strlen(prng_output_stream) / 2);
		encdec_aft_pk_bytes = OQS_MEM_malloc(kem->length_public_key);
		encdec_aft_k_bytes = OQS_MEM_malloc(kem->length_shared_secret);
		encdec_aft_c_bytes = OQS_MEM_malloc(kem->length_ciphertext);

		if ((prng_output_stream_bytes == NULL) || (encdec_aft_pk_bytes == NULL) || (encdec_aft_k_bytes == NULL) || (encdec_aft_c_bytes == NULL)) {
			fprintf(stderr, "[vectors_kem] ERROR: OQS_MEM_malloc failed!\n");
			rc = OQS_ERROR;
			goto err;
		}

		hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
		hexStringToByteArray(encdec_aft_pk, encdec_aft_pk_bytes);
		hexStringToByteArray(encdec_aft_k, encdec_aft_k_bytes);
		hexStringToByteArray(encdec_aft_c, encdec_aft_c_bytes);

		rc = kem_vector_encdec_aft(alg_name, prng_output_stream_bytes, encdec_aft_pk_bytes, encdec_aft_k_bytes, encdec_aft_c_bytes);
	} else if (!strcmp(test_name, "encDecVAL")) {
		encdec_val_sk = argv[3];
		encdec_val_k = argv[4];
		encdec_val_c = argv[5];

		if (strlen(encdec_val_c) != 2 * kem->length_ciphertext ||
		        strlen(encdec_val_k) != 2 * kem->length_shared_secret ||
		        strlen(encdec_val_sk) != 2 * kem->length_secret_key) {
			rc = OQS_ERROR;
			goto err;
		}

		encdec_val_sk_bytes = OQS_MEM_malloc(kem->length_secret_key);
		encdec_val_k_bytes = OQS_MEM_malloc(kem->length_shared_secret);
		encdec_val_c_bytes = OQS_MEM_malloc(kem->length_ciphertext);

		if ((encdec_val_sk_bytes == NULL) || (encdec_val_k_bytes == NULL) || (encdec_val_c_bytes == NULL)) {
			fprintf(stderr, "[vectors_kem] ERROR: OQS_MEM_malloc failed!\n");
			rc = OQS_ERROR;
			goto err;
		}

		hexStringToByteArray(encdec_val_sk, encdec_val_sk_bytes);
		hexStringToByteArray(encdec_val_k, encdec_val_k_bytes);
		hexStringToByteArray(encdec_val_c, encdec_val_c_bytes);

		rc = kem_vector_encdec_val(alg_name, encdec_val_sk_bytes, encdec_val_c_bytes, encdec_val_k_bytes);
	} else if (!strcmp(test_name, "strcmp")) {
		strcmp_seed = argv[3]; // d || z : both should be 32 bytes each as per FIPS-203
		strcmp_ek = argv[4];
		strcmp_c = argv[5];
		strcmp_k = argv[6];

		if (strlen(strcmp_seed) != 128 ||
		        strlen(strcmp_ek) != 2 * kem->length_public_key ||
		        strlen(strcmp_c) != 2 * kem->length_ciphertext ||
		        strlen(strcmp_k) != 2 * kem->length_shared_secret) {
			rc = OQS_ERROR;
			goto err;
		}

		size_t seedSize = strlen(strcmp_seed);
		strcmp_seed_bytes = OQS_MEM_malloc(seedSize / 2);
		strcmp_ek_bytes = OQS_MEM_malloc(kem->length_public_key);
		strcmp_c_bytes = OQS_MEM_malloc(kem->length_ciphertext);
		strcmp_k_bytes = OQS_MEM_malloc(kem->length_shared_secret);

		if ((strcmp_seed_bytes == NULL) || (strcmp_ek_bytes == NULL) ||
		        (strcmp_c_bytes == NULL) || (strcmp_k_bytes == NULL)) {
			fprintf(stderr, "[vectors_kem] ERROR: OQS_MEM_malloc failed!\n");
			rc = OQS_ERROR;
			goto err;
		}

		hexStringToByteArray(strcmp_seed, strcmp_seed_bytes);
		hexStringToByteArray(strcmp_ek, strcmp_ek_bytes);
		hexStringToByteArray(strcmp_c, strcmp_c_bytes);
		hexStringToByteArray(strcmp_k, strcmp_k_bytes);


		rc = kem_strcmp_vector(alg_name, strcmp_seed_bytes, strcmp_ek_bytes, strcmp_c_bytes, strcmp_k_bytes);
	} else if (!strcmp(test_name, "modOverflow")) {
		modOverflow_ek = argv[3]; // d || z : both should be 32 bytes each as per FIPS-203

		if (strlen(modOverflow_ek) != 2 * kem->length_public_key) {
			rc = OQS_ERROR;
			goto err;
		}

		modOverflow_ek_bytes = OQS_MEM_malloc(kem->length_public_key);

		if (modOverflow_ek_bytes == NULL) {
			fprintf(stderr, "[vectors_kem] ERROR: OQS_MEM_malloc failed!\n");
			rc = OQS_ERROR;
			goto err;
		}

		hexStringToByteArray(modOverflow_ek,  modOverflow_ek_bytes);


		rc = kem_modOverflow_vector(alg_name, modOverflow_ek_bytes);
	} else {
		printf("[vectors_kem] %s only keyGen supported!\n", alg_name);
	}

err:
	OQS_MEM_insecure_free(prng_output_stream_bytes);
	OQS_MEM_insecure_free(kg_pk_bytes);
	OQS_MEM_insecure_free(kg_sk_bytes);

	OQS_MEM_insecure_free(encdec_aft_c_bytes);
	OQS_MEM_insecure_free(encdec_aft_k_bytes);
	OQS_MEM_insecure_free(encdec_aft_pk_bytes);

	OQS_MEM_insecure_free(encdec_val_c_bytes);
	OQS_MEM_insecure_free(encdec_val_k_bytes);
	OQS_MEM_insecure_free(encdec_val_sk_bytes);

	OQS_MEM_insecure_free(strcmp_seed_bytes);
	OQS_MEM_insecure_free(strcmp_ek_bytes);
	OQS_MEM_insecure_free(strcmp_c_bytes);
	OQS_MEM_insecure_free(strcmp_k_bytes);

	OQS_MEM_insecure_free(modOverflow_ek_bytes);

	OQS_KEM_free(kem);

	OQS_destroy();

	if (rc != OQS_SUCCESS) {
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}


// int main(void)
// {
//  char *kemName = "ML-KEM-512";
//  char *type = "strcmp";
//  char *ek = "012d0964f73d10775357e83c3b7a2249cc1b37e1cd0868489a265573f0ab48a82de563ac11a25fe312be3799701920974c3ace4c45072e7c726a51b58e110ba74b3cf23208d006354565a03c7235ad47ae3ff35ef8d7b5fda9c844362371320208663fbd724b1a83c87f929f66e8ab55a53e2a9411c2c30ef66aa2a323c686e825f2ccb92ed792d317969515a5a4f95803577c273c745f94aec7fb060102607bb7555486808e509e8bfc70cf42409e4c6d439a2c96c05dfc6404d65c128f594fa6049f5ba53d27a712f459ccd391bd9506c611d90ab80120e5b4c9cea5b1cd6b1e7efc8b0b54020068b209648681a2afbc875b44478aaa1c70e06c75854a674fd86487b37a66d0078fa53dfdc3a50be51709c2bae9a03392446c777344a3664ac2177ab7412b33ea68b8b0015b9c540850594eeb6043e7b0d7d8912ab6750331072e165bfdd201c3598a6aca457b36147afc97979c4056da81e9ec0389d959cc863dc6564f03b460ea7375ebcb4b39968a4c95ad355a9b65d684325096f28ace3faaa060875f046081ab17b1ef24103eb7b3b838c6122cb44496a61a2b8875242d49a5ace3e362881632119554fec502ad322cc9a7263812b40db6ac1a6762ec3a16e575248cf3b214321557052ea7ab55f3f40dd8107fc275cbee181d153784186a9bc6558b77f0478bc54c47535a22fc8ecd273b84e9c5acbb8d8387af4113ac88777cb9d6622f156c10744f35e99341c11b9ab12a98f13ef5914a84d94848b45ea44802a04454f6459d5bfb6208c77dba176ef5807274b8918c9a7a5c8500f15567677a5d08a78b51d07b219958bfca57a3dbad20a25e565302e3302d582515d8e444ddd6060e889f0ecb80934716ee6670c13362ab845ab56a826e199ccd38a6bc2bceecc14202826c31a353c94b61b7087e0fa2abe971705c1c1c50568cada9c727116297538993f6aa4709646985b915382ef31984399ab4e46902b01b4e196ca7aa640e499c44eef03d515194bba95e77fb998afc33697256755062f0079ac97b1dedd7557ce5c805b6b838a87e6523398f55059b316bb277a425d056b0611203959deb8480f9bd1d3fd6a0d8b48a3a48b94069b9562f156a431ddf9f";
//  char *c = "";
//  char *k = "";

//  int argc = 6;
//  char *argv[6] = {"",kemName,type,ek,c,k};

//  return func(argc,argv);
// }
