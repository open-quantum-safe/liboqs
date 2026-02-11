// SPDX-License-Identifier: MIT

// This tests the test vectors published by NIST ACVP


#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <oqs/oqs.h>

#include "oqs/common.h"
#include "system_info.c"
#include "../src/sig/slh_dsa/slh_dsa_c/slh_dsa.h"

#define MLDSA_RNDBYTES 32
#define MAXCTXBYTES 255

#ifdef OQS_ENABLE_SIG_ml_dsa_44
extern int PQCP_MLDSA_NATIVE_MLDSA44_C_signature_internal(uint8_t *sig,
        size_t *siglen,
        const uint8_t *m,
        size_t mlen,
        const uint8_t *pre,
        size_t prelen,
        const uint8_t rnd[MLDSA_RNDBYTES],
        const uint8_t *sk,
        int externalmu);

extern int PQCP_MLDSA_NATIVE_MLDSA44_C_signature_pre_hash_internal(uint8_t *sig,
        size_t *siglen,
        const uint8_t *ph,
        size_t phlen,
        const uint8_t *ctx,
        size_t ctxlen,
        const uint8_t rnd[MLDSA_RNDBYTES],
        const uint8_t *sk,
        int hashalg);

extern int PQCP_MLDSA_NATIVE_MLDSA44_C_verify_internal(const uint8_t *sig,
        size_t siglen,
        const uint8_t *m,
        size_t mlen,
        const uint8_t *pre,
        size_t prelen,
        const uint8_t *pk,
        int externalmu);

extern int PQCP_MLDSA_NATIVE_MLDSA44_C_verify_pre_hash_internal(const uint8_t *sig,
        size_t siglen,
        const uint8_t *ph,
        size_t phlen,
        const uint8_t *ctx,
        size_t ctxlen,
        const uint8_t *pk,
        int hashalg);
#endif

#ifdef OQS_ENABLE_SIG_ml_dsa_65
extern int PQCP_MLDSA_NATIVE_MLDSA65_C_signature_internal(uint8_t *sig,
        size_t *siglen,
        const uint8_t *m,
        size_t mlen,
        const uint8_t *pre,
        size_t prelen,
        const uint8_t rnd[MLDSA_RNDBYTES],
        const uint8_t *sk,
        int externalmu);

extern int PQCP_MLDSA_NATIVE_MLDSA65_C_signature_pre_hash_internal(uint8_t *sig,
        size_t *siglen,
        const uint8_t *ph,
        size_t phlen,
        const uint8_t *ctx,
        size_t ctxlen,
        const uint8_t rnd[MLDSA_RNDBYTES],
        const uint8_t *sk,
        int hashalg);

extern int PQCP_MLDSA_NATIVE_MLDSA65_C_verify_internal(const uint8_t *sig,
        size_t siglen,
        const uint8_t *m,
        size_t mlen,
        const uint8_t *pre,
        size_t prelen,
        const uint8_t *pk,
        int externalmu);

extern int PQCP_MLDSA_NATIVE_MLDSA65_C_verify_pre_hash_internal(const uint8_t *sig,
        size_t siglen,
        const uint8_t *ph,
        size_t phlen,
        const uint8_t *ctx,
        size_t ctxlen,
        const uint8_t *pk,
        int hashalg);
#endif

#ifdef OQS_ENABLE_SIG_ml_dsa_87
extern int PQCP_MLDSA_NATIVE_MLDSA87_C_signature_internal(uint8_t *sig,
        size_t *siglen,
        const uint8_t *m,
        size_t mlen,
        const uint8_t *pre,
        size_t prelen,
        const uint8_t rnd[MLDSA_RNDBYTES],
        const uint8_t *sk,
        int externalmu);

extern int PQCP_MLDSA_NATIVE_MLDSA87_C_signature_pre_hash_internal(uint8_t *sig,
        size_t *siglen,
        const uint8_t *ph,
        size_t phlen,
        const uint8_t *ctx,
        size_t ctxlen,
        const uint8_t rnd[MLDSA_RNDBYTES],
        const uint8_t *sk,
        int hashalg);

extern int PQCP_MLDSA_NATIVE_MLDSA87_C_verify_internal(const uint8_t *sig,
        size_t siglen,
        const uint8_t *m,
        size_t mlen,
        const uint8_t *pre,
        size_t prelen,
        const uint8_t *pk,
        int externalmu);

extern int PQCP_MLDSA_NATIVE_MLDSA87_C_verify_pre_hash_internal(const uint8_t *sig,
        size_t siglen,
        const uint8_t *ph,
        size_t phlen,
        const uint8_t *ctx,
        size_t ctxlen,
        const uint8_t *pk,
        int hashalg);
#endif

#ifdef OQS_ENABLE_SIG_SLH_DSA
extern size_t slh_sign_internal(uint8_t *sig, const uint8_t *m, size_t m_sz,
                                const uint8_t *sk, const uint8_t *addrnd,
                                const slh_param_t *prm);

extern int slh_verify_internal(const uint8_t *m, size_t m_sz, const uint8_t *sig,
                               size_t sig_sz, const uint8_t *pk,
                               const slh_param_t *prm);
#endif

struct {
	const uint8_t *pos;
} prng_state = {
	.pos = 0
};

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

/* ML_DSA-specific functions */
static inline bool is_ml_dsa(const char *method_name) {
	return (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_44))
	       || (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_65))
	       || (0 == strcmp(method_name, OQS_SIG_alg_ml_dsa_87));
}

static void MLDSA_randombytes_init(const uint8_t *entropy_input, const uint8_t *personalization_string) {
	(void) personalization_string;
	prng_state.pos = entropy_input;
}

static void MLDSA_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	memcpy(random_array, prng_state.pos, bytes_to_read);
	prng_state.pos += bytes_to_read;
}

static void MLDSA_randombytes_free(void) {
	prng_state.pos = 0;
}

/* SLH-DSA-specific functions */
static inline bool is_slh_dsa(const char *method_name) {
	return (0 == strncmp(method_name, "SLH_DSA", 7));
}

static void SLHDSA_randombytes_init(const uint8_t *entropy_input, const uint8_t *personalization_string) {
	(void) personalization_string;
	prng_state.pos = entropy_input;
}

static void SLHDSA_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	memcpy(random_array, prng_state.pos, bytes_to_read);
	prng_state.pos += bytes_to_read;
}

static void SLHDSA_randombytes_free(void) {
	prng_state.pos = 0;
}

#ifdef OQS_ENABLE_SIG_SLH_DSA
static const slh_param_t *SLHDSA_param_set(const char *method_name) {
	char *id = strstr(method_name, "PURE");
	if (id != NULL) {
		id += 5;
	} else {
		id = strstr(method_name, "PREHASH");
		id += 8;
	}

	if (!strcmp(id, "SHA2_128S")) {
		return &slh_dsa_sha2_128s;
	} else if (!strcmp(id, "SHA2_128F")) {
		return &slh_dsa_sha2_128f;
	} else if (!strcmp(id, "SHA2_192S")) {
		return &slh_dsa_sha2_192s;
	} else if (!strcmp(id, "SHA2_192F")) {
		return &slh_dsa_sha2_192f;
	} else if (!strcmp(id, "SHA2_256S")) {
		return &slh_dsa_sha2_256s;
	} else if (!strcmp(id, "SHA2_256F")) {
		return &slh_dsa_sha2_256f;
	} else if (!strcmp(id, "SHAKE_128S")) {
		return &slh_dsa_shake_128s;
	} else if (!strcmp(id, "SHAKE_128F")) {
		return &slh_dsa_shake_128f;
	} else if (!strcmp(id, "SHAKE_192S")) {
		return &slh_dsa_shake_192s;
	} else if (!strcmp(id, "SHAKE_192F")) {
		return &slh_dsa_shake_192f;
	} else if (!strcmp(id, "SHAKE_256S")) {
		return &slh_dsa_shake_256s;
	} else if (!strcmp(id, "SHAKE_256F")) {
		return &slh_dsa_shake_256f;
	} else {
		return NULL;
	}
}
#endif

int prehash_id_from_string(const char *s) {
#if defined(OQS_ENABLE_SIG_ml_dsa_44) || defined(OQS_ENABLE_SIG_ml_dsa_65) || defined(OQS_ENABLE_SIG_ml_dsa_87)
	if (!strcmp(s, "SHA2-224")) {
		return 1;
	} else if (!strcmp(s, "SHA2-256")) {
		return 2;
	} else if (!strcmp(s, "SHA2-384")) {
		return 3;
	} else if (!strcmp(s, "SHA2-512")) {
		return 4;
	} else if (!strcmp(s, "SHA2-512/224")) {
		return 5;
	} else if (!strcmp(s, "SHA2-512/256")) {
		return 6;
	} else if (!strcmp(s, "SHA3-224")) {
		return 7;
	} else if (!strcmp(s, "SHA3-256")) {
		return 8;
	} else if (!strcmp(s, "SHA3-384")) {
		return 9;
	} else if (!strcmp(s, "SHA3-512")) {
		return 10;
	} else if (!strcmp(s, "SHAKE-128")) {
		return 11;
	} else if (!strcmp(s, "SHAKE-256")) {
		return 12;
	}
#else
	(void)s;
#endif
	return -1;
}

static OQS_STATUS sig_kg_vector(const char *method_name,
                                uint8_t *prng_output_stream,
                                const uint8_t *kg_pk, const uint8_t *kg_sk) {
	uint8_t *entropy_input;
	FILE *fh = NULL;
	OQS_SIG *sig = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	OQS_STATUS rc, ret = OQS_ERROR;

	void (*randombytes_init)(const uint8_t *, const uint8_t *) = NULL;
	void (*randombytes_free)(void) = NULL;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (is_ml_dsa(method_name)) {
		OQS_randombytes_custom_algorithm(&MLDSA_randombytes);
		randombytes_init = &MLDSA_randombytes_init;
		randombytes_free = &MLDSA_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else if (is_slh_dsa(method_name)) {
		OQS_randombytes_custom_algorithm(&SLHDSA_randombytes);
		randombytes_init = &SLHDSA_randombytes_init;
		randombytes_free = &SLHDSA_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else {
		// Only ML-DSA and SLH-DSA supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	fh = stdout;

	public_key = OQS_MEM_malloc(sig->length_public_key);
	secret_key = OQS_MEM_malloc(sig->length_secret_key);
	if ((public_key == NULL) || (secret_key == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((prng_output_stream == NULL) || (kg_pk == NULL) || (kg_sk == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	rc = OQS_SIG_keypair(sig, public_key, secret_key);
	if (rc) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_keypair failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "pk: ", public_key, sig->length_public_key);
	fprintBstr(fh, "sk: ", secret_key, sig->length_secret_key);

	if (!memcmp(public_key, kg_pk, sig->length_public_key) && !memcmp(secret_key, kg_sk, sig->length_secret_key)) {
		ret = OQS_SUCCESS;
	} else {
		ret = OQS_ERROR;
		fprintf(stderr, "[vectors_sig] %s ERROR: public key or private key doesn't match!\n", method_name);
	}
	goto cleanup;

err:
	ret = OQS_ERROR;
	goto cleanup;

algo_not_enabled:
	ret = OQS_SUCCESS;

cleanup:
	if (sig != NULL) {
		OQS_MEM_secure_free(secret_key, sig->length_secret_key);
	}
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_MEM_insecure_free(public_key);
	OQS_SIG_free(sig);
	return ret;
}

#if defined(OQS_ENABLE_SIG_ml_dsa_44) || defined(OQS_ENABLE_SIG_ml_dsa_65) || defined(OQS_ENABLE_SIG_ml_dsa_87) || defined(OQS_ENABLE_SIG_SLH_DSA)
static int sig_ver_vector(const char *method_name,
                          const uint8_t *sigVer_pk_bytes,
                          const uint8_t *sigVer_msg_bytes,
                          size_t msgLen,
                          const uint8_t *sigVer_sig_bytes,
                          size_t sigLen,
                          int testPassed,
                          int extMu) {

	FILE *fh = NULL;
	OQS_SIG *sig = NULL;
	int rc = -1, ret = -1;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	fh = stdout;

	if ((sigVer_pk_bytes == NULL) || (sigVer_msg_bytes == NULL) || (sigVer_sig_bytes == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	if (!strcmp(method_name, "ML-DSA-44")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_44
		rc = PQCP_MLDSA_NATIVE_MLDSA44_C_verify_internal(sigVer_sig_bytes, sigLen, sigVer_msg_bytes, msgLen, NULL, 0, sigVer_pk_bytes, extMu);
#endif
	} else if (!strcmp(method_name, "ML-DSA-65")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_65
		rc = PQCP_MLDSA_NATIVE_MLDSA65_C_verify_internal(sigVer_sig_bytes, sigLen, sigVer_msg_bytes, msgLen, NULL, 0, sigVer_pk_bytes, extMu);
#endif
	} else if (!strcmp(method_name, "ML-DSA-87")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_87
		rc = PQCP_MLDSA_NATIVE_MLDSA87_C_verify_internal(sigVer_sig_bytes, sigLen, sigVer_msg_bytes, msgLen, NULL, 0, sigVer_pk_bytes, extMu);
#endif
	} else if (!strncmp(method_name, "SLH_DSA", 7)) {
#ifdef OQS_ENABLE_SIG_SLH_DSA
		const slh_param_t *param_set = SLHDSA_param_set(method_name);
		rc = (slh_verify_internal(sigVer_msg_bytes, msgLen, sigVer_sig_bytes, sigLen, sigVer_pk_bytes, param_set) != 0 ? 0 : 1);
#endif
	} else {
		goto err;
	}
	if ((!rc) != testPassed) {
		fprintf(stderr, "[vectors_sig] %s ERROR: ml_dsa_verify_internal failed!\n", method_name);
		goto err;
	} else {
		ret = EXIT_SUCCESS;
	}


	fprintBstr(fh, "testPassed: ", (const uint8_t *)&testPassed, 1);

	goto cleanup;

err:
	ret = EXIT_FAILURE;
	goto cleanup;

algo_not_enabled:
	ret = EXIT_SUCCESS;

cleanup:
	OQS_SIG_free(sig);
	return ret;
}

static int sig_ver_vector_ext(const char *method_name,
                              const uint8_t *sigVer_pk_bytes,
                              const uint8_t *sigVer_msg_bytes,
                              size_t msgLen,
                              const uint8_t *sigVer_sig_bytes,
                              size_t sigLen,
                              const uint8_t *sigVer_ctx,
                              size_t sigVer_ctxLen,
                              int testPassed) {

	FILE *fh = NULL;
	OQS_SIG *sig = NULL;
	int rc = -1, ret = -1;

	if ((sigVer_pk_bytes == NULL) || (sigVer_msg_bytes == NULL) || (sigVer_sig_bytes == NULL) || (sigVer_ctxLen && sigVer_ctx == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	fh = stdout;

	rc = OQS_SIG_verify_with_ctx_str(sig, sigVer_msg_bytes, msgLen, sigVer_sig_bytes, sigLen, sigVer_ctx, sigVer_ctxLen, sigVer_pk_bytes);

	if ((!rc) != testPassed) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_verify_with_ctx_str failed!\n", method_name);
		goto err;
	} else {
		ret = EXIT_SUCCESS;
	}

	// NOTE: OQS_SIG_verify should be equivalent to OQS_SIG_verify_with_ctx_str(ctx=Null, ctxlen=0)
	if ((sigVer_ctx == NULL) && (sigVer_ctxLen == 0)) {
		rc = OQS_SIG_verify(sig, sigVer_msg_bytes, msgLen, sigVer_sig_bytes, sigLen, sigVer_pk_bytes);

		if ((!rc) != testPassed) {
			fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_verify failed!\n", method_name);
			goto err;
		} else {
			ret = EXIT_SUCCESS;
		}
	}

	fprintBstr(fh, "testPassed: ", (const uint8_t *)&testPassed, 1);

	goto cleanup;

err:
	ret = EXIT_FAILURE;
	goto cleanup;

algo_not_enabled:
	ret = EXIT_SUCCESS;

cleanup:
	OQS_SIG_free(sig);
	return ret;

	/* __attribute__ doesn't work on windows, so "use" sigLen to make compiler happy */
	(void)sigLen;
}

static int sig_ver_prehash_vector_ext(const char *method_name,
                                      const uint8_t *sigVer_pk_bytes,
                                      const uint8_t *sigVer_msg_bytes,
                                      size_t msgLen,
                                      const uint8_t *sigVer_sig_bytes,
                                      size_t sigLen,
                                      const uint8_t *sigVer_ctx,
                                      size_t sigVer_ctxLen,
                                      int testPassed,
                                      int hashalg) {

	FILE *fh = NULL;
	OQS_SIG *sig = NULL;
	int rc = -1, ret = -1;

	if ((sigVer_pk_bytes == NULL) || (sigVer_msg_bytes == NULL) || (sigVer_sig_bytes == NULL) || (sigVer_ctxLen && sigVer_ctx == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	fh = stdout;

	if (!strcmp(method_name, "ML-DSA-44")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_44
		rc = PQCP_MLDSA_NATIVE_MLDSA44_C_verify_pre_hash_internal(sigVer_sig_bytes, sigLen, sigVer_msg_bytes, msgLen, sigVer_ctx, sigVer_ctxLen, sigVer_pk_bytes, hashalg);
#endif
	} else if (!strcmp(method_name, "ML-DSA-65")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_65
		rc = PQCP_MLDSA_NATIVE_MLDSA65_C_verify_pre_hash_internal(sigVer_sig_bytes, sigLen, sigVer_msg_bytes, msgLen, sigVer_ctx, sigVer_ctxLen, sigVer_pk_bytes, hashalg);
#endif
	} else if (!strcmp(method_name, "ML-DSA-87")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_87
		rc = PQCP_MLDSA_NATIVE_MLDSA87_C_verify_pre_hash_internal(sigVer_sig_bytes, sigLen, sigVer_msg_bytes, msgLen, sigVer_ctx, sigVer_ctxLen, sigVer_pk_bytes, hashalg);
#endif
	} else {
		goto err;
	}
	if ((!rc) != testPassed) {
		fprintf(stderr, "[vectors_sig] %s ERROR: mldsa_prehash_verify_internal API failed!\n", method_name);
		goto err;
	} else {
		ret = EXIT_SUCCESS;
	}

	fprintBstr(fh, "testPassed: ", (const uint8_t *)&testPassed, 1);

	goto cleanup;

err:
	ret = EXIT_FAILURE;
	goto cleanup;

algo_not_enabled:
	ret = EXIT_SUCCESS;

cleanup:
	OQS_SIG_free(sig);
	return ret;
}

static int sig_gen_vector(const char *method_name,
                          uint8_t *prng_output_stream,
                          const uint8_t *sigGen_sk, const uint8_t *sigGen_msg, size_t sigGen_msgLen, const uint8_t *sigGen_sig, int extMu) {

	FILE *fh = NULL;
	uint8_t *signature = NULL;
	OQS_SIG *sig = NULL;
	int rc = -1, ret = -1;
	size_t sigLen;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	sigLen = sig->length_signature;

	fh = stdout;

	signature = OQS_MEM_malloc(sigLen);

	if (signature == NULL) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((prng_output_stream == NULL) || (sigGen_sk == NULL) || (sigGen_msg == NULL) || (sigGen_sig == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	if (!strcmp(method_name, "ML-DSA-44")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_44
		rc = PQCP_MLDSA_NATIVE_MLDSA44_C_signature_internal(signature, &sigLen, sigGen_msg, sigGen_msgLen, NULL, 0, prng_output_stream, sigGen_sk, extMu);
#endif
	} else if (!strcmp(method_name, "ML-DSA-65")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_65
		rc = PQCP_MLDSA_NATIVE_MLDSA65_C_signature_internal(signature, &sigLen, sigGen_msg, sigGen_msgLen, NULL, 0, prng_output_stream, sigGen_sk, extMu);
#endif
	} else if (!strcmp(method_name, "ML-DSA-87")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_87
		rc = PQCP_MLDSA_NATIVE_MLDSA87_C_signature_internal(signature, &sigLen, sigGen_msg, sigGen_msgLen, NULL, 0, prng_output_stream, sigGen_sk, extMu);
#endif
	} else if (!strncmp(method_name, "SLH_DSA", 7)) {
#ifdef OQS_ENABLE_SIG_SLH_DSA
		const slh_param_t *param_set = SLHDSA_param_set(method_name);
		rc = ((int) slh_sign_internal(signature, sigGen_msg, sigGen_msgLen, sigGen_sk, prng_output_stream, param_set) > 0) ? 0 : 1;
#endif
	} else {
		goto err;
	}

	if (rc) {
		fprintf(stderr, "[vectors_sig] %s ERROR: sign_internal failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "signature: ", signature, sig->length_signature);

	if (!memcmp(signature, sigGen_sig, sigLen)) {
		ret = EXIT_SUCCESS;
	} else {
		ret = EXIT_FAILURE;
		fprintf(stderr, "[vectors_sig] %s ERROR: signature doesn't match!\n", method_name);
	}
	goto cleanup;

err:
	ret = EXIT_FAILURE;
	goto cleanup;

algo_not_enabled:
	ret = EXIT_SUCCESS;

cleanup:
	OQS_MEM_insecure_free(signature);
	OQS_SIG_free(sig);
	return ret;
}

static int sig_gen_vector_ext(const char *method_name,
                              uint8_t *prng_output_stream,
                              const uint8_t *sigGen_sk,
                              const uint8_t *sigGen_msg, size_t sigGen_msgLen,
                              const uint8_t *sigGen_ctx, size_t sigGen_ctxLen,
                              const uint8_t *sigGen_sig) {

	uint8_t *entropy_input;
	FILE *fh = NULL;
	uint8_t *signature = NULL;
	OQS_SIG *sig = NULL;
	int rc = -1, ret = -1;
	size_t sigLen;

	void (*randombytes_init)(const uint8_t *, const uint8_t *) = NULL;
	void (*randombytes_free)(void) = NULL;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (is_ml_dsa(method_name)) {
		OQS_randombytes_custom_algorithm(&MLDSA_randombytes);
		randombytes_init = &MLDSA_randombytes_init;
		randombytes_free = &MLDSA_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else if (is_slh_dsa(method_name)) {
		OQS_randombytes_custom_algorithm(&SLHDSA_randombytes);
		randombytes_init = &SLHDSA_randombytes_init;
		randombytes_free = &SLHDSA_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else {
		// Only ML-DSA and SLH-DSA supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	sigLen = sig->length_signature;

	fh = stdout;

	signature = OQS_MEM_malloc(sigLen);

	if (signature == NULL) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((prng_output_stream == NULL) || (sigGen_sk == NULL) || (sigGen_msg == NULL) || (sigGen_sig == NULL) || (sigGen_ctxLen && sigGen_ctx == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	rc = OQS_SIG_sign_with_ctx_str(sig, signature, &sigLen, sigGen_msg, sigGen_msgLen, sigGen_ctx, sigGen_ctxLen, sigGen_sk);

	if (rc) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_sign_with_ctx_str failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "signature: ", signature, sig->length_signature);

	if (!memcmp(signature, sigGen_sig, sigLen)) {
		ret = EXIT_SUCCESS;
	} else {
		ret = EXIT_FAILURE;
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_sign_with_ctx_str doesn't match!\n", method_name);
	}

	// NOTE: OQS_SIG_sign should be equivalent to OQS_SIG_sign_with_ctx_str(ctx=NULL, ctxlen=0)
	if ((sigGen_ctx == NULL) && (sigGen_ctxLen == 0)) {
		// reset PRNG to ensure identical output
		randombytes_init(entropy_input, NULL);
		rc = OQS_SIG_sign(sig, signature, &sigLen, sigGen_msg, sigGen_msgLen, sigGen_sk);
		if (rc) {
			fprintf(stderr, "[vectors_sig] %s ERROR: OQS_SIG_sign failed!\n", method_name);
			goto err;
		}
		if (!memcmp(signature, sigGen_sig, sigLen)) {
			ret = EXIT_SUCCESS;
		} else {
			ret = EXIT_FAILURE;
			fprintf(stderr, "[vectors_sig] %s ERROR: signature doesn't match!\n", method_name);
		}
	}
	goto cleanup;

err:
	ret = EXIT_FAILURE;
	goto cleanup;

algo_not_enabled:
	ret = EXIT_SUCCESS;

cleanup:
	OQS_MEM_insecure_free(signature);
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_SIG_free(sig);
	return ret;
}

static int sig_gen_prehash_vector_ext(const char *method_name,
                                      uint8_t *prng_output_stream,
                                      const uint8_t *sigGen_sk,
                                      const uint8_t *sigGen_msg,
                                      size_t sigGen_msgLen,
                                      const uint8_t *sigGen_ctx,
                                      size_t sigGen_ctxLen,
                                      const uint8_t *sigGen_sig,
                                      int algo_name) {

	uint8_t *entropy_input;
	FILE *fh = NULL;
	uint8_t *signature = NULL;
	OQS_SIG *sig = NULL;
	int rc = -1, ret = -1;
	size_t sigLen;

	void (*randombytes_init)(const uint8_t *, const uint8_t *) = NULL;
	void (*randombytes_free)(void) = NULL;

	sig = OQS_SIG_new(method_name);
	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", method_name);
		goto algo_not_enabled;
	}

	if (is_ml_dsa(method_name)) {
		OQS_randombytes_custom_algorithm(&MLDSA_randombytes);
		randombytes_init = &MLDSA_randombytes_init;
		randombytes_free = &MLDSA_randombytes_free;
		entropy_input = (uint8_t *) prng_output_stream;
	} else {
		// Only ML-DSA supported
		goto err;
	}

	randombytes_init(entropy_input, NULL);

	sigLen = sig->length_signature;

	fh = stdout;

	signature = OQS_MEM_malloc(sigLen);

	if (signature == NULL) {
		fprintf(stderr, "[vectors_sig] %s ERROR: OQS_MEM_malloc failed!\n", method_name);
		goto err;
	}

	if ((prng_output_stream == NULL) || (sigGen_sk == NULL) || (sigGen_msg == NULL) || (sigGen_sig == NULL) || (sigGen_ctxLen && sigGen_ctx == NULL)) {
		fprintf(stderr, "[vectors_sig] %s ERROR: inputs NULL!\n", method_name);
		goto err;
	}

	if (!strcmp(method_name, "ML-DSA-44")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_44
		rc = PQCP_MLDSA_NATIVE_MLDSA44_C_signature_pre_hash_internal(signature, &sigLen, sigGen_msg, sigGen_msgLen, sigGen_ctx, sigGen_ctxLen, prng_output_stream, sigGen_sk, algo_name);
#endif
	} else if (!strcmp(method_name, "ML-DSA-65")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_65
		rc = PQCP_MLDSA_NATIVE_MLDSA65_C_signature_pre_hash_internal(signature, &sigLen, sigGen_msg, sigGen_msgLen, sigGen_ctx, sigGen_ctxLen, prng_output_stream, sigGen_sk, algo_name);
#endif
	} else if (!strcmp(method_name, "ML-DSA-87")) {
#ifdef OQS_ENABLE_SIG_ml_dsa_87
		rc = PQCP_MLDSA_NATIVE_MLDSA87_C_signature_pre_hash_internal(signature, &sigLen, sigGen_msg, sigGen_msgLen, sigGen_ctx, sigGen_ctxLen, prng_output_stream, sigGen_sk, algo_name);
#endif
	} else {
		goto err;
	}
	if (rc) {
		fprintf(stderr, "[vectors_sig] %s ERROR: mldsa_verify_prehash_internal failed!\n", method_name);
		goto err;
	}
	fprintBstr(fh, "signature: ", signature, sig->length_signature);

	if (!memcmp(signature, sigGen_sig, sigLen)) {
		ret = EXIT_SUCCESS;
	} else {
		ret = EXIT_FAILURE;
		fprintf(stderr, "[vectors_sig] %s ERROR: signature doesn't match!\n", method_name);
	}
	goto cleanup;

err:
	ret = EXIT_FAILURE;
	goto cleanup;

algo_not_enabled:
	ret = EXIT_SUCCESS;

cleanup:
	OQS_MEM_insecure_free(signature);
	if (randombytes_free != NULL) {
		randombytes_free();
	}
	OQS_SIG_free(sig);
	return ret;
}
#endif

int main(int argc, char **argv) {
	OQS_STATUS rc = OQS_SUCCESS;
	bool valid_args = true;
	bool free_prng_output_stream = false;

	OQS_init();

	size_t msgLen;
	size_t ctxlen;

	char *prng_output_stream = NULL;
	char *kg_pk;
	char *kg_sk;

	char *sigGen_sk;
	char *sigGen_msg;
	char *sigGen_sig;
	char *sigGen_ctx;

	char *sigVer_pk;
	char *sigVer_msg;
	char *sigVer_sig;
	char *sigVer_ctx;

	uint8_t *prng_output_stream_bytes = NULL;
	uint8_t *kg_pk_bytes = NULL;
	uint8_t *kg_sk_bytes = NULL;

	uint8_t *sigGen_sk_bytes = NULL;
	uint8_t *sigGen_msg_bytes = NULL;
	uint8_t *sigGen_sig_bytes = NULL;
	uint8_t *sigGen_ctx_bytes = NULL;

	uint8_t *sigVer_pk_bytes = NULL;
	uint8_t *sigVer_msg_bytes = NULL;
	uint8_t *sigVer_sig_bytes = NULL;
	uint8_t *sigVer_ctx_bytes = NULL;

	OQS_SIG *sig = NULL;

	if (argc < 3) {
		valid_args = false;
		goto err;
	}

	char *alg_name = argv[1];
	char *test_name = argv[2];

	sig = OQS_SIG_new(alg_name);

	if (sig == NULL) {
		printf("[vectors_sig] %s was not enabled at compile-time.\n", alg_name);
		goto err;
	}

	if (!strcmp(test_name, "keyGen")) {
		/* vectors_sig alg_name keyGen seed pk sk ---> ml dsa (argc 6) */
		/* vectors_sig alg_name keyGen skSeed skPRF pkSeed pk sk  ---> slh dsa (argc 8) */
		if (argc != 6 && argc != 8) {
			valid_args = false;
			goto err;
		}

		if (is_ml_dsa(alg_name)) {
			prng_output_stream = argv[3];
			kg_pk = argv[4];
			kg_sk = argv[5];

			if (strlen(prng_output_stream) != 2 * MLDSA_RNDBYTES ||
			        strlen(kg_pk) != 2 * sig->length_public_key ||
			        strlen(kg_sk) != 2 * sig->length_secret_key) {
				printf("lengths bad\n");
				goto err;
			}
		} else if (is_slh_dsa(alg_name)) {
			kg_pk = argv[6];
			kg_sk = argv[7];
			prng_output_stream = kg_sk;

			if (strlen(prng_output_stream) != 2 * sig->length_secret_key ||
			        strlen(kg_pk) != 2 * sig->length_public_key ||
			        strlen(kg_sk) != 2 * sig->length_secret_key) {
				printf("lengths bad\n");
				goto err;
			}
		} else {
			printf("Only ML-DSA and SLH-DSA supported \n");
			goto err;
		}

		prng_output_stream_bytes = OQS_MEM_malloc(strlen(prng_output_stream) / 2);
		kg_pk_bytes = OQS_MEM_malloc(sig->length_public_key);
		kg_sk_bytes = OQS_MEM_malloc(sig->length_secret_key);

		if ((prng_output_stream_bytes == NULL) || (kg_pk_bytes == NULL) || (kg_sk_bytes == NULL)) {
			fprintf(stderr, "[vectors_sig] ERROR: OQS_MEM_malloc failed!\n");
			goto err;
		}

		hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
		hexStringToByteArray(kg_pk, kg_pk_bytes);
		hexStringToByteArray(kg_sk, kg_sk_bytes);


		rc = sig_kg_vector(alg_name, prng_output_stream_bytes, kg_pk_bytes, kg_sk_bytes);

	} else if (!strcmp(test_name, "sigGen_int")) {
		/* vectors_sig alg_name sigGen_int sk message signature rnd extmu ---> (argc 8) */
		if (argc != 8) {
			valid_args = false;
			goto err;
		}
		sigGen_sk = argv[3];
		sigGen_msg = argv[4];
		sigGen_sig = argv[5];
		prng_output_stream = argv[6];
		int extMu = atoi(argv[7]);
		/* still need to deal with prehash FIX */

		if (is_ml_dsa(alg_name)) {
			if ( strlen(sigGen_msg) % 2 != 0 ||
			        strlen(prng_output_stream) != 2 * MLDSA_RNDBYTES ||
			        strlen(sigGen_sig) != 2 * sig->length_signature ||
			        strlen(sigGen_sk) != 2 * sig->length_secret_key ||
			        (extMu != 0 && extMu != 1)) {
				printf("lengths bad or external mu value incorrect\n");
				goto err;
			}
		} else if (is_slh_dsa(alg_name)) {
			/* if no randomness given, use pk.seed */
			if (strlen(prng_output_stream) == 0) {
				/* Need to remember to free the output stream */
				free_prng_output_stream = true;
				/* we actually want half the length of the public key but we double for a string. give extra byte for null char. */
				prng_output_stream = OQS_MEM_malloc(sig->length_public_key + 1);
				memcpy(prng_output_stream, sigGen_sk + 2 * sig->length_public_key, sig->length_public_key);
				prng_output_stream[sig->length_public_key] = '\0';
			}

			if ( strlen(sigGen_msg) % 2 != 0 ||
			        strlen(prng_output_stream) != sig->length_public_key ||
			        strlen(sigGen_sig) != 2 * sig->length_signature ||
			        strlen(sigGen_sk) != 2 * sig->length_secret_key) {
				printf("lengths bad\n");
				goto err;
			}
		}

		msgLen = strlen(sigGen_msg) / 2;

		sigGen_sk_bytes = OQS_MEM_malloc(sig->length_secret_key);
		sigGen_msg_bytes = OQS_MEM_malloc(msgLen);
		sigGen_sig_bytes = OQS_MEM_malloc(sig->length_signature);
		prng_output_stream_bytes = OQS_MEM_malloc(strlen(prng_output_stream) / 2);

		if ((sigGen_sk_bytes == NULL) || (sigGen_msg_bytes == NULL) || (sigGen_sig_bytes == NULL) || (prng_output_stream_bytes == NULL)) {
			fprintf(stderr, "[vectors_sig] ERROR: OQS_MEM_malloc failed!\n");
			goto err;
		}

		hexStringToByteArray(sigGen_sk, sigGen_sk_bytes);
		hexStringToByteArray(sigGen_msg, sigGen_msg_bytes);
		hexStringToByteArray(sigGen_sig, sigGen_sig_bytes);
		hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);

#if defined(OQS_ENABLE_SIG_ml_dsa_44) || defined(OQS_ENABLE_SIG_ml_dsa_65) || defined(OQS_ENABLE_SIG_ml_dsa_87) || defined(OQS_ENABLE_SLH_DSA)
		rc = sig_gen_vector(alg_name, prng_output_stream_bytes, sigGen_sk_bytes, sigGen_msg_bytes, msgLen, sigGen_sig_bytes, extMu);
#else
		rc = EXIT_SUCCESS;
		goto cleanup;
#endif

	} else if (!strcmp(test_name, "sigGen_ext")) {
		/* vectors_sig alg_name sigGen_ext sk message signature context rnd ---> (argc 8) */
		if (argc != 8 && argc != 9) {
			valid_args = false;
			goto err;
		}
		sigGen_sk = argv[3];
		sigGen_msg = argv[4];
		sigGen_sig = argv[5];
		sigGen_ctx = argv[6];
		prng_output_stream = argv[7];

		if (is_ml_dsa(alg_name)) {
			if ( strlen(sigGen_msg) % 2 != 0 ||
			        strlen(sigGen_sig) != 2 * sig->length_signature ||
			        strlen(prng_output_stream) != 2 * MLDSA_RNDBYTES ||
			        strlen(sigGen_sk) != 2 * sig->length_secret_key ||
			        strlen(sigGen_ctx) > 2 * MAXCTXBYTES) {
				printf("lengths bad\n");
				goto err;
			}
		} else if (is_slh_dsa(alg_name)) {
			/* if no randomness given, use pk.seed */
			if (strlen(prng_output_stream) == 0) {
				/* Need to remember to free the output stream */
				free_prng_output_stream = true;
				/* we actually want half the length of the public key but we double for a string. give extra byte for null char. */
				prng_output_stream = OQS_MEM_malloc(sig->length_public_key + 1);
				memcpy(prng_output_stream, sigGen_sk + 2 * sig->length_public_key, sig->length_public_key);
				prng_output_stream[sig->length_public_key] = '\0';
			}

			if ( strlen(sigGen_msg) % 2 != 0 ||
			        strlen(prng_output_stream) != sig->length_public_key ||
			        strlen(sigGen_sig) != 2 * sig->length_signature ||
			        strlen(sigGen_sk) != 2 * sig->length_secret_key ||
			        strlen(sigGen_ctx) % 2 != 0 ||
			        strlen(sigGen_ctx) > 2 * MAXCTXBYTES) {
				printf("lengths bad\n");
				goto err;
			}
		}

		msgLen = strlen(sigGen_msg) / 2;
		ctxlen = strlen(sigGen_ctx) / 2;

		sigGen_sk_bytes = OQS_MEM_malloc(sig->length_secret_key);
		sigGen_msg_bytes = OQS_MEM_malloc(msgLen);
		sigGen_sig_bytes = OQS_MEM_malloc(sig->length_signature);
		prng_output_stream_bytes = OQS_MEM_malloc(strlen(prng_output_stream) / 2);
		/* allocate memory if required */
		if (ctxlen) {
			sigGen_ctx_bytes = OQS_MEM_malloc(ctxlen);
		}

		if ((sigGen_sk_bytes == NULL) || (sigGen_msg_bytes == NULL) || (sigGen_sig_bytes == NULL) || (ctxlen && sigGen_ctx_bytes == NULL) || (prng_output_stream_bytes == NULL)) {
			fprintf(stderr, "[vectors_sig] ERROR: OQS_MEM_malloc failed!\n");
			goto err;
		}

		hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
		hexStringToByteArray(sigGen_sk, sigGen_sk_bytes);
		hexStringToByteArray(sigGen_msg, sigGen_msg_bytes);
		hexStringToByteArray(sigGen_sig, sigGen_sig_bytes);
		if (ctxlen) {
			hexStringToByteArray(sigGen_ctx, sigGen_ctx_bytes);
		}

#if defined(OQS_ENABLE_SIG_ml_dsa_44) || defined(OQS_ENABLE_SIG_ml_dsa_65) || defined(OQS_ENABLE_SIG_ml_dsa_87) || defined(OQS_ENABLE_SIG_SLH_DSA)
		rc = sig_gen_vector_ext(alg_name, prng_output_stream_bytes, sigGen_sk_bytes, sigGen_msg_bytes, msgLen, sigGen_ctx_bytes, ctxlen, sigGen_sig_bytes);
#else
		rc = EXIT_SUCCESS;
		goto cleanup;
#endif

	} else if (!strcmp(test_name, "sigGen_prehash_ext")) {
		/* vectors_sig alg_name sigGen_ext sk message signature context rnd algname ---> (argc 9) */
		if (argc != 9) {
			valid_args = false;
			goto err;
		}
		sigGen_sk = argv[3];
		sigGen_msg = argv[4];
		sigGen_sig = argv[5];
		sigGen_ctx = argv[6];
		prng_output_stream = argv[7];
		char *hash_algo = argv[8];
		int prehash_id = prehash_id_from_string(hash_algo);

		if ( strlen(sigGen_msg) % 2 != 0 ||
		        strlen(sigGen_sig) != 2 * sig->length_signature ||
		        strlen(prng_output_stream) != 2 * MLDSA_RNDBYTES ||
		        strlen(sigGen_sk) != 2 * sig->length_secret_key ||
		        strlen(sigGen_ctx) > 2 * MAXCTXBYTES ||
		        (prehash_id < 1 || prehash_id > 12)) {
			printf("lengths bad or incorrect hash algo constant \n");
			goto err;
		}

		msgLen = strlen(sigGen_msg) / 2;
		ctxlen = strlen(sigGen_ctx) / 2;

		sigGen_sk_bytes = OQS_MEM_malloc(sig->length_secret_key);
		sigGen_msg_bytes = OQS_MEM_malloc(msgLen);
		sigGen_sig_bytes = OQS_MEM_malloc(sig->length_signature);
		prng_output_stream_bytes = OQS_MEM_malloc(strlen(prng_output_stream) / 2);
		/* allocate memory if required */
		if (ctxlen) {
			sigGen_ctx_bytes = OQS_MEM_malloc(ctxlen);
		}

		if ((sigGen_sk_bytes == NULL) || (sigGen_msg_bytes == NULL) || (sigGen_sig_bytes == NULL) || (ctxlen && sigGen_ctx_bytes == NULL) || (prng_output_stream_bytes == NULL)) {
			fprintf(stderr, "[vectors_sig] ERROR: OQS_MEM_malloc failed!\n");
			goto err;
		}

		hexStringToByteArray(prng_output_stream, prng_output_stream_bytes);
		hexStringToByteArray(sigGen_sk, sigGen_sk_bytes);
		hexStringToByteArray(sigGen_msg, sigGen_msg_bytes);
		hexStringToByteArray(sigGen_sig, sigGen_sig_bytes);
		if (ctxlen) {
			hexStringToByteArray(sigGen_ctx, sigGen_ctx_bytes);
		}

#if defined(OQS_ENABLE_SIG_ml_dsa_44) || defined(OQS_ENABLE_SIG_ml_dsa_65) || defined(OQS_ENABLE_SIG_ml_dsa_87)
		rc = sig_gen_prehash_vector_ext(alg_name, prng_output_stream_bytes, sigGen_sk_bytes, sigGen_msg_bytes, msgLen, sigGen_ctx_bytes, ctxlen, sigGen_sig_bytes, prehash_id);
#else
		rc = EXIT_SUCCESS;
		goto cleanup;
#endif

	} else if (!strcmp(test_name, "sigVer_int")) {
		/* vectors_sig alg_name sigVer_int pk message signature testPassed extmu ---> (argc 8) */
		if (argc != 8) {
			valid_args = false;
			goto err;
		}
		sigVer_pk = argv[3];
		sigVer_msg = argv[4];
		sigVer_sig = argv[5];
		int extMu = atoi(argv[7]);

		int sigVerPassed = atoi(argv[6]);

		if (is_ml_dsa(alg_name)) {
			if (strlen(sigVer_msg) % 2 != 0 ||
			        strlen(sigVer_sig) != 2 * sig->length_signature ||
			        strlen(sigVer_pk) != 2 * sig->length_public_key ||
			        (sigVerPassed != 0 && sigVerPassed != 1) ||
			        (extMu != 0 && extMu != 1)) {
				printf("lengths bad or incorrect verification status \n");
				goto err;
			}
		} else if (is_slh_dsa(alg_name)) {
			/* SLHDSA also tests for wrong sig lengths so we don't fail here */
			if (strlen(sigVer_msg) % 2 != 0 ||
			        strlen(sigVer_sig) % 2 != 0 ||
			        strlen(sigVer_pk) != 2 * sig->length_public_key ||
			        (sigVerPassed != 0 && sigVerPassed != 1)) {
				printf("lengths bad or incorrect verification status \n");
				goto err;
			}
		}

		msgLen = strlen(sigVer_msg) / 2;

		sigVer_pk_bytes = OQS_MEM_malloc(sig->length_public_key);
		sigVer_msg_bytes = OQS_MEM_malloc(msgLen);
		sigVer_sig_bytes = OQS_MEM_malloc(strlen(sigVer_sig) / 2);

		if ((sigVer_pk_bytes == NULL) || (sigVer_msg_bytes == NULL) || (sigVer_sig_bytes == NULL)) {
			fprintf(stderr, "[vectors_sig] ERROR: OQS_MEM_malloc failed!\n");
			goto err;
		}

		hexStringToByteArray(sigVer_pk, sigVer_pk_bytes);
		hexStringToByteArray(sigVer_msg, sigVer_msg_bytes);
		hexStringToByteArray(sigVer_sig, sigVer_sig_bytes);

#if defined(OQS_ENABLE_SIG_ml_dsa_44) || defined(OQS_ENABLE_SIG_ml_dsa_65) || defined(OQS_ENABLE_SIG_ml_dsa_87) || defined(OQS_ENABLE_SIG_SLH_DSA)
		rc = sig_ver_vector(alg_name, sigVer_pk_bytes, sigVer_msg_bytes, msgLen, sigVer_sig_bytes, strlen(sigVer_sig) / 2, sigVerPassed, extMu);
#else
		rc = EXIT_SUCCESS;
		goto cleanup;
#endif

	} else if (!strcmp(test_name, "sigVer_ext")) {
		/* vectors_sig alg_name sigVer_int pk message signature context testPassed ---> (argc 8) */
		if (argc != 8 && argc != 9) {
			valid_args = false;
			goto err;
		}
		sigVer_pk = argv[3];
		sigVer_msg = argv[4];
		sigVer_sig = argv[5];
		sigVer_ctx = argv[6];

		int sigVerPassed = atoi(argv[7]);

		if (is_ml_dsa(alg_name)) {
			if (strlen(sigVer_msg) % 2 != 0 ||
			        strlen(sigVer_sig) != 2 * sig->length_signature ||
			        strlen(sigVer_pk) != 2 * sig->length_public_key ||
			        strlen(sigVer_ctx) > 2 * MAXCTXBYTES ||
			        (sigVerPassed != 0 && sigVerPassed != 1)) {
				printf("lengths bad or incorrect verification status \n");
				goto err;
			}
		} else if (is_slh_dsa(alg_name)) {
			/* SLHDSA also tests for wrong sig lengths so we don't fail here */
			if (strlen(sigVer_msg) % 2 != 0 ||
			        strlen(sigVer_sig) % 2 != 0 ||
			        strlen(sigVer_pk) != 2 * sig->length_public_key ||
			        strlen(sigVer_ctx) % 2 != 0 ||
			        strlen(sigVer_ctx) > 2 * MAXCTXBYTES ||
			        (sigVerPassed != 0 && sigVerPassed != 1)) {
				printf("lengths bad or incorrect verification status \n");
				goto err;
			}
		}

		msgLen = strlen(sigVer_msg) / 2;
		ctxlen = strlen(sigVer_ctx) / 2;

		sigVer_pk_bytes = OQS_MEM_malloc(sig->length_public_key);
		sigVer_msg_bytes = OQS_MEM_malloc(msgLen);
		sigVer_sig_bytes = OQS_MEM_malloc(strlen(sigVer_sig) / 2);
		/* allocate memory if required */
		if (ctxlen) {
			sigVer_ctx_bytes = OQS_MEM_malloc(ctxlen);
		}

		if ((sigVer_pk_bytes == NULL) || (sigVer_msg_bytes == NULL) || (sigVer_sig_bytes == NULL) || (ctxlen && sigVer_ctx_bytes == NULL)) {
			fprintf(stderr, "[vectors_sig] ERROR: OQS_MEM_malloc failed!\n");
			goto err;
		}

		hexStringToByteArray(sigVer_pk, sigVer_pk_bytes);
		hexStringToByteArray(sigVer_msg, sigVer_msg_bytes);
		hexStringToByteArray(sigVer_sig, sigVer_sig_bytes);
		if (ctxlen) {
			hexStringToByteArray(sigVer_ctx, sigVer_ctx_bytes);
		}

#if defined(OQS_ENABLE_SIG_ml_dsa_44) || defined(OQS_ENABLE_SIG_ml_dsa_65) || defined(OQS_ENABLE_SIG_ml_dsa_87)
		rc = sig_ver_vector_ext(alg_name, sigVer_pk_bytes, sigVer_msg_bytes, msgLen, sigVer_sig_bytes, strlen(sigVer_sig) / 2, sigVer_ctx_bytes, ctxlen, sigVerPassed);
#else
		rc = EXIT_SUCCESS;
		goto cleanup;
#endif

	} else if (!strcmp(test_name, "sigVer_prehash_ext")) {
		/* vectors_sig alg_name sigVer_int pk message signature context testPassed algname---> (argc 9) */
		if (argc != 9) {
			valid_args = false;
			goto err;
		}
		sigVer_pk = argv[3];
		sigVer_msg = argv[4];
		sigVer_sig = argv[5];
		sigVer_ctx = argv[6];
		int sigVerPassed = atoi(argv[7]);
		char *hash_algo = argv[8];
		int prehash_id = prehash_id_from_string(hash_algo);

		if (strlen(sigVer_msg) % 2 != 0 ||
		        strlen(sigVer_sig) != 2 * sig->length_signature ||
		        strlen(sigVer_pk) != 2 * sig->length_public_key ||
		        strlen(sigVer_ctx) > 2 * MAXCTXBYTES ||
		        (sigVerPassed != 0 && sigVerPassed != 1) ||
		        (prehash_id < 1 || prehash_id > 12)) {
			printf("lengths bad or incorrect verification status or incorrect hash algo constant \n");
			goto err;
		}

		msgLen = strlen(sigVer_msg) / 2;
		ctxlen = strlen(sigVer_ctx) / 2;

		sigVer_pk_bytes = OQS_MEM_malloc(sig->length_public_key);
		sigVer_msg_bytes = OQS_MEM_malloc(msgLen);
		sigVer_sig_bytes = OQS_MEM_malloc(strlen(sigVer_sig) / 2);
		/* allocate memory if required */
		if (ctxlen) {
			sigVer_ctx_bytes = OQS_MEM_malloc(ctxlen);
		}

		if ((sigVer_pk_bytes == NULL) || (sigVer_msg_bytes == NULL) || (sigVer_sig_bytes == NULL) || (ctxlen && sigVer_ctx_bytes == NULL)) {
			fprintf(stderr, "[vectors_sig] ERROR: OQS_MEM_malloc failed!\n");
			goto err;
		}

		hexStringToByteArray(sigVer_pk, sigVer_pk_bytes);
		hexStringToByteArray(sigVer_msg, sigVer_msg_bytes);
		hexStringToByteArray(sigVer_sig, sigVer_sig_bytes);
		if (ctxlen) {
			hexStringToByteArray(sigVer_ctx, sigVer_ctx_bytes);
		}

#if defined(OQS_ENABLE_SIG_ml_dsa_44) || defined(OQS_ENABLE_SIG_ml_dsa_65) || defined(OQS_ENABLE_SIG_ml_dsa_87)
		rc = sig_ver_prehash_vector_ext(alg_name, sigVer_pk_bytes, sigVer_msg_bytes, msgLen, sigVer_sig_bytes, strlen(sigVer_sig) / 2, sigVer_ctx_bytes, ctxlen, sigVerPassed, prehash_id) ;
#else
		rc = EXIT_SUCCESS;
		goto cleanup;
#endif

	} else {
		printf("[vectors_sig] %s only keyGen/sigGen_int/sigGen_ext/sigGen_prehash_ext/sigVer_int/sigVer_ext/sigVer_prehash_ext supported!\n", alg_name);
		goto err;
	}
	goto cleanup;

err:
	rc = EXIT_FAILURE;
	if (!valid_args) {
		fprintf(stderr, "Usage: vectors_sig algname testname [testargs]\n");
		fprintf(stderr, "\n");
		printf("\n");
		print_system_info();
	}

cleanup:
	if (free_prng_output_stream) {
		OQS_MEM_insecure_free(prng_output_stream);
	}
	OQS_MEM_insecure_free(prng_output_stream_bytes);
	OQS_MEM_insecure_free(kg_pk_bytes);
	if (sig != NULL) {
		OQS_MEM_secure_free(kg_sk_bytes, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(sigVer_pk_bytes);
	OQS_MEM_insecure_free(sigVer_msg_bytes);
	OQS_MEM_insecure_free(sigVer_sig_bytes);
	OQS_MEM_insecure_free(sigVer_ctx_bytes);
	if (sig != NULL) {
		OQS_MEM_secure_free(sigGen_sk_bytes, sig->length_secret_key);
	}
	OQS_MEM_insecure_free(sigGen_msg_bytes);
	OQS_MEM_insecure_free(sigGen_sig_bytes);
	OQS_MEM_insecure_free(sigGen_ctx_bytes);
	OQS_SIG_free(sig);
	OQS_destroy();

	if (rc != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}
