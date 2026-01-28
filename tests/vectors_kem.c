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
	/* calculate hash of the public key(len = 384k+32) stored in private key at offset of 384k */
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
	/* compare the encoded value with original public key. discard value of `rho(32 bytes)` during comparison as its not encoded */
	if (0 != memcmp(buffe, pk, pkLen - 32)) {
		return false;
	}
	return true;
}
#endif //OQS_ENABLE_KEM_ML_KEM

static OQS_STATUS kem_kg_vector(const char *method_name,
                                uint8_t *prng_output_stream,
                                const uint8_t *kg_pk, const uint8_t *kg_sk) {

	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (!is_ml_kem(method_name)) {
		// Only ML-KEM supported
		goto err;
	}

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

	rc = OQS_KEM_keypair_derand(kem, public_key, secret_key, prng_output_stream);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_keypair_derand failed!\n", method_name);
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
	OQS_MEM_insecure_free(public_key);
	OQS_KEM_free(kem);
	return ret;
}

static OQS_STATUS kem_vector_encdec_aft(const char *method_name,
                                        uint8_t *prng_output_stream,
                                        const uint8_t *encdec_pk,
                                        const uint8_t *encdec_k, const uint8_t *encdec_c) {

	FILE *fh = NULL;
	OQS_KEM *kem = NULL;
	uint8_t *ss_encaps = NULL;
	uint8_t *ct_encaps = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

	kem = OQS_KEM_new(method_name);
	if (kem == NULL) {
		printf("[vectors_kem] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (!is_ml_kem(method_name)) {
		// Only ML-KEM supported
		goto err;
	}

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

	rc = OQS_KEM_encaps_derand(kem, ct_encaps, ss_encaps, encdec_pk, prng_output_stream);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_encaps_derand failed!\n", method_name);
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
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_decaps failed!\n", method_name);
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

	OQS_KEM *kem = NULL;
	OQS_STATUS rc = OQS_ERROR;

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

	if (!is_ml_kem(method_name)) {
		// Only ML-KEM supported
		goto err;
	}

	rc = OQS_KEM_keypair_derand(kem, ek, dk, seed);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_keypair_derand failed!\n", method_name);
		goto err;
	}

	// check ek
	if (memcmp(ek, ekExpected, kem->length_public_key)) {
		goto err;
	}

	// perform decapsulation
	rc = OQS_KEM_decaps(kem, k, c, dk);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "[vectors_kem] %s ERROR: OQS_KEM_decaps failed!\n", method_name);
		goto err;
	}

	// check k
	if (memcmp(k, kExpected, kem->length_shared_secret)) {
		goto err;
	}

	goto cleanup;

err:
	rc = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	rc = OQS_SUCCESS;

cleanup:
	if (kem != NULL) {
		OQS_MEM_secure_free(ek, kem->length_public_key);
		OQS_MEM_secure_free(dk, kem->length_secret_key);
		OQS_MEM_secure_free(k, kem->length_shared_secret);
	}
	OQS_KEM_free(kem);
	return rc;
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
	if (kem != NULL) {
		OQS_MEM_secure_free(kg_sk_bytes, kem->length_secret_key);
	}

	OQS_MEM_insecure_free(encdec_aft_c_bytes);
	OQS_MEM_insecure_free(encdec_aft_k_bytes);
	OQS_MEM_insecure_free(encdec_aft_pk_bytes);

	OQS_MEM_insecure_free(encdec_val_c_bytes);
	OQS_MEM_insecure_free(encdec_val_k_bytes);
	if (kem != NULL) {
		OQS_MEM_secure_free(encdec_val_sk_bytes, kem->length_secret_key);
	}

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
//  char *seed = "a3896e30892230a6c1dff667f8caee759ff84a08e3462ae484fcbca9971d7959cdc6c5ec65f10a5a24b5145aac863232ee3b2229ca3a6c4b9c8a2dafc315d9d4";
//  char *ek = "871b108fd980108768612345f2fc7317216f55576f914c3ede67878ea89046d636572ca78fd67a4efc9b68e462853b2ae8dab001b6059c390916fccf45bba50b85b7e12504ef931c52030a4efb31ce978866ea0bece660b16700d9ca4fb8eca335cb2395478e6bf845eba6161765c479e0a3c0b64773b0a8f9191789e0b939cbbce7f80317e569a96c7c3ba2596aabca4298c2383ab539a05232f990af03b936d18cb6378363523ccdb75ef237ccf9774776ba0e219c97726c6f82b3168e49081d7b358f6c2dda287796a99775e60de716ae28385b8270451b3a2b8dbb267a8a37266c70718ac30303cad7e5908d0b09897bbfb18c25975bc9f46909df4b3451dbca7a3498eb253bb4246721685b762086b903b6f6a86b2db894cd59567ac9b18a82c5fa67306ef3cd7aa5c82d46aa73111f837c2eadca7b1bf6ba81f769e0530d44b21f5ab161a159014c14ad7292b638b86e548a5c0de9300df1416e619afbb01a77588cd3e1a17546be6af108f8d3ad87429f6dba298d8119a4594310f874def0a9f3f5a20748cde73218e91b428a629e6cb24e611300d9155f6d16c6398b7d2615a0bf486b9ac1c32be69b2212baff1c5ccbb0c247075536f2bc28caa954064bf5f3963060ada289ab5a58628959347356a7ba4c84051a65547a3b56a738eb9acc0d62213cf1291006a502ec7296b42e989905a08393278480290887958c4155a975d37194d25b146344b012318a3aa98060c64946191a871408f6a0917cab9227b956e4dc8081c96b50927944381631237302a0c960a33678a268f2139ca0b5b44203863cba6d2ab37c78b369a4ba5256e173a274567b536cb0d5b6d47b3d8ad738fc022792112a4ca09389b9c1317b2342a5c4a444c73f36468529806842b5297482fb001ceb28b1be8299e612985957bf7ac16cf7e609fe532a33f25690186616762c680b1cb1e986c5f55c37a49bc32219faa8ada839937ac56f7587ac51a12abfc8be413a9b915b4c455ab1e2dba776dc8633083646f7cd95437b129095e75510dff58fa4cc6c3f4531211a3a161418b7da4e6ec8388b0197993898783d963d109590d4ac0092189baabef63fcc3df15b0ab3099078c9580b198e11";
//  char *c = "00ec7fcdb617629fead7f43cf59a7d3a0b946b5c5f472407812c15c44249e52974483eb31fb6ce5e15728708ef24f78bb61bb5c8b6ebb4e8a96e5b898e069c499c44a57efb0907234597e810f4de40d85d6eacd775b83400937ac863cfe5d406469b8d3ca4f6dbe0aced60fbecb32588f1ddaaa08ba5c5b177ab51b548d8ae2218eb2d9b02b31cb4bb6e7754b6a74856316ad3e0394025f750412023a74ab851fc03171cada993c6469fa254b5d384edab1efe8f2efb6eba37c7ba7434bb243331938849114ad398cf91038fb96c30a712f47e1c7183956838d487016f023bd9900a94c606edaa42bf0e67f2aedd03f5a581e55af29e24af061ba73da64c3869d7598a0c95c3c8330ecb4481cab2c85ccaaa366f99f3bafc980362f4d701a233d0967ded78e4f1874b42cda774b888d31f1671d6567d1636d5b2bcc117e2a256f343541c3e173e900b6d6c71bcd55479d23dee6b54e2fe32303c7955fa5f567894605ed75e65e54858222c0d065ba9231fcd82ab86af63c242d10d1d30b4176e2bb8169fbd7f03cfa044098853382083284ef8cecf05f9d0bd3d9af84f6df551e3d11e9281794d36a619f61e47901ea0dd695782241250f0b5c2a20d76d3afefcb11d015179d6a9a74121c448e315f507cb29add40da4eb05cae27320c4b18a25d7af24ac7b0bdf7382143d4d02db3e7d016f9ff5638e0df7968f785ac8000c7545a1a70d6fd43cc3ba28d82e9eb6ccb391e094ba85a49ad6e9f8fd8faa9954062a5df13c69847bee485bc6624b40b8128fe911231b4ef9b4a22ced4f8140df65a5b8180665128273fde02955a65921f0d1be139fbe1edf0ac01db1d2a5ffd95d63d33b0a7627cab4bcd37647238c80197d4fa88a7e34d4caa066dba2ae6fefff6a1e804a72d49ed443c29b2db313fdf25d98d0dee992befc4572f8b1eee0db7227b6e202ef69ea8340ae3690fc064d3edfd8d067934db3061767e4857e805e888294efa7248743fcaa1e01acdaf491de408c58869bc9624c951103c079793be1edfa9442df4dece40eabb99e9b01410fc0108b26a13cd424cd9a9de5bafa1fc";
//  char *k = "cf3bcfeb2679cb43658fcdcd01aa1505bcea1e72a165ccac7bfb66d9dc0c0e90";

//  int argc = 7;
//  char *argv[7] = {"",kemName,type,seed,ek,c,k};

//  return func(argc,argv);
// }
