// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <string.h>
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif

#include <oqs/oqs.h>

OQS_API const char *OQS_SIG_STFL_alg_identifier(size_t i) {

	const char *a[OQS_SIG_STFL_algs_length] = {
		OQS_SIG_STFL_alg_xmss_sha256_h10,
		OQS_SIG_STFL_alg_xmss_sha256_h16,
		OQS_SIG_STFL_alg_xmss_sha256_h20,
		OQS_SIG_STFL_alg_xmss_shake128_h10,
		OQS_SIG_STFL_alg_xmss_shake128_h16,
		OQS_SIG_STFL_alg_xmss_shake128_h20,
		OQS_SIG_STFL_alg_xmss_sha512_h10,
		OQS_SIG_STFL_alg_xmss_sha512_h16,
		OQS_SIG_STFL_alg_xmss_sha512_h20,
		OQS_SIG_STFL_alg_xmss_shake256_h10,
		OQS_SIG_STFL_alg_xmss_shake256_h16,
		OQS_SIG_STFL_alg_xmss_shake256_h20,
		OQS_SIG_STFL_alg_xmssmt_sha256_h20_2,
		OQS_SIG_STFL_alg_xmssmt_sha256_h20_4,
		OQS_SIG_STFL_alg_xmssmt_sha256_h40_2,
		OQS_SIG_STFL_alg_xmssmt_sha256_h40_4,
		OQS_SIG_STFL_alg_xmssmt_sha256_h40_8,
		OQS_SIG_STFL_alg_xmssmt_sha256_h60_3,
		OQS_SIG_STFL_alg_xmssmt_sha256_h60_6,
		OQS_SIG_STFL_alg_xmssmt_sha256_h60_12,
		OQS_SIG_STFL_alg_xmssmt_shake128_h20_2,
		OQS_SIG_STFL_alg_xmssmt_shake128_h20_4,
		OQS_SIG_STFL_alg_xmssmt_shake128_h40_2,
		OQS_SIG_STFL_alg_xmssmt_shake128_h40_4,
		OQS_SIG_STFL_alg_xmssmt_shake128_h40_8,
		OQS_SIG_STFL_alg_xmssmt_shake128_h60_3,
		OQS_SIG_STFL_alg_xmssmt_shake128_h60_6,
		OQS_SIG_STFL_alg_xmssmt_shake128_h60_12,
		OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1,
	};

	if (i >= OQS_SIG_STFL_algs_length) {
		return NULL;
	} else {
		return a[i];
	}
}


OQS_API int OQS_SIG_STFL_alg_count(void) {
	return OQS_SIG_STFL_algs_length;
}


OQS_API int OQS_SIG_STFL_alg_is_enabled(const char *method_name) {
	assert(method_name != NULL);

	if (0) {

	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12
		return 1;
#else
		return 0;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_LMS
		return 1;
#else
		return 0;
#endif
	} else {
		return 0;
	}
}


OQS_API OQS_SIG_STFL *OQS_SIG_STFL_new(const char *method_name) {
	assert(method_name != NULL);

	if (0) {

	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10
		return OQS_SIG_STFL_alg_xmss_sha256_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16
		return OQS_SIG_STFL_alg_xmss_sha256_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20
		return OQS_SIG_STFL_alg_xmss_sha256_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h10
		return OQS_SIG_STFL_alg_xmss_shake128_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
		return OQS_SIG_STFL_alg_xmss_shake128_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
		return OQS_SIG_STFL_alg_xmss_shake128_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h10
		return OQS_SIG_STFL_alg_xmss_sha512_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
		return OQS_SIG_STFL_alg_xmss_sha512_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
		return OQS_SIG_STFL_alg_xmss_sha512_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10
		return OQS_SIG_STFL_alg_xmss_shake256_h10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
		return OQS_SIG_STFL_alg_xmss_shake256_h16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
		return OQS_SIG_STFL_alg_xmss_shake256_h20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2
		return OQS_SIG_STFL_alg_xmssmt_sha256_h20_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4
		return OQS_SIG_STFL_alg_xmssmt_sha256_h20_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
		return OQS_SIG_STFL_alg_xmssmt_sha256_h40_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4
		return OQS_SIG_STFL_alg_xmssmt_sha256_h40_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8
		return OQS_SIG_STFL_alg_xmssmt_sha256_h40_8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
		return OQS_SIG_STFL_alg_xmssmt_sha256_h60_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6
		return OQS_SIG_STFL_alg_xmssmt_sha256_h60_6_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12
		return OQS_SIG_STFL_alg_xmssmt_sha256_h60_12_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2
		return OQS_SIG_STFL_alg_xmssmt_shake128_h20_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4
		return OQS_SIG_STFL_alg_xmssmt_shake128_h20_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
		return OQS_SIG_STFL_alg_xmssmt_shake128_h40_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4
		return OQS_SIG_STFL_alg_xmssmt_shake128_h40_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8
		return OQS_SIG_STFL_alg_xmssmt_shake128_h40_8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
		return OQS_SIG_STFL_alg_xmssmt_shake128_h60_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6
		return OQS_SIG_STFL_alg_xmssmt_shake128_h60_6_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12
		return OQS_SIG_STFL_alg_xmssmt_shake128_h60_12_new();
#else
		return NULL;
#endif
	}  else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_LMS
		return OQS_SIG_STFL_alg_lms_sha256_h5_w1_new();
#else
		return NULL;
#endif //OQS_ENABLE_SIG_STFL_LMS
	} else {
		return NULL;
	}
}


OQS_API OQS_STATUS OQS_SIG_STFL_keypair(const OQS_SIG_STFL *sig, uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (sig == NULL || sig->keypair == NULL || sig->keypair(public_key, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
	return OQS_ERROR;
}

OQS_API OQS_STATUS OQS_SIG_STFL_sign(const OQS_SIG_STFL *sig, uint8_t *signature, size_t *signature_len, const uint8_t *message,
                                     size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (sig == NULL || sig->sign == NULL || sig->sign(signature, signature_len, message, message_len, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_STFL_verify(const OQS_SIG_STFL *sig, const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (sig == NULL || sig->verify == NULL || sig->verify(message, message_len, signature, signature_len, public_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_STFL_sigs_remaining(const OQS_SIG_STFL *sig, unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (sig == NULL || sig->sigs_remaining == NULL || sig->sigs_remaining(remain, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API OQS_STATUS OQS_SIG_STFL_sigs_total(const OQS_SIG_STFL *sig, unsigned long long *max, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	if (sig == NULL || sig->sigs_total == NULL || sig->sigs_total(max, secret_key) != 0) {
		return OQS_ERROR;
	} else {
		return OQS_SUCCESS;
	}
}

OQS_API void OQS_SIG_STFL_free(OQS_SIG_STFL *sig) {
	OQS_MEM_insecure_free(sig);
}



// ================================= OQS_SIG_STFL_SECRET_KEY FUNCTION ===============================================


OQS_API OQS_SIG_STFL_SECRET_KEY *OQS_SIG_STFL_SECRET_KEY_new(const char *method_name) {
	assert(method_name != NULL);

	if (0) {

	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h10
		return OQS_SECRET_KEY_XMSS_SHA256_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h16
		return OQS_SECRET_KEY_XMSS_SHA256_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha256_h20
		return OQS_SECRET_KEY_XMSS_SHA256_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h10
		return OQS_SECRET_KEY_XMSS_SHAKE128_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
		return OQS_SECRET_KEY_XMSS_SHAKE128_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake128_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
		return OQS_SECRET_KEY_XMSS_SHAKE128_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h10
		return OQS_SECRET_KEY_XMSS_SHA512_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
		return OQS_SECRET_KEY_XMSS_SHA512_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_sha512_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
		return OQS_SECRET_KEY_XMSS_SHA512_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h10)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10
		return OQS_SECRET_KEY_XMSS_SHAKE256_H10_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h16)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
		return OQS_SECRET_KEY_XMSS_SHAKE256_H16_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmss_shake256_h20)) {
#ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
		return OQS_SECRET_KEY_XMSS_SHAKE256_H20_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2
		return OQS_SECRET_KEY_XMSSMT_SHA256_H20_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4
		return OQS_SECRET_KEY_XMSSMT_SHA256_H20_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
		return OQS_SECRET_KEY_XMSSMT_SHA256_H40_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4
		return OQS_SECRET_KEY_XMSSMT_SHA256_H40_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8
		return OQS_SECRET_KEY_XMSSMT_SHA256_H40_8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
		return OQS_SECRET_KEY_XMSSMT_SHA256_H60_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6
		return OQS_SECRET_KEY_XMSSMT_SHA256_H60_6_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_sha256_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12
		return OQS_SECRET_KEY_XMSSMT_SHA256_H60_12_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H20_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h20_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H20_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_2)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_2_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_4)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_4_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h40_8)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H40_8_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_3)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_3_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_6)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_6_new();
#else
		return NULL;
#endif
	} else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_xmssmt_shake128_h60_12)) {
#ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12
		return OQS_SECRET_KEY_XMSSMT_SHAKE128_H60_12_new();
#else
		return NULL;
#endif
	}  else if (0 == strcasecmp(method_name, OQS_SIG_STFL_alg_lms_sha256_n32_h5_w1)) {
#ifdef OQS_ENABLE_SIG_STFL_LMS
		return OQS_SECRET_KEY_LMS_SHA256_H5_W1_new();
#else
		return NULL;
#endif
	} else {
		return NULL;
	}
}

void OQS_SECRET_KEY_XMSS_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return;
	}

	OQS_MEM_secure_free(sk->secret_key_data, sk->length_secret_key);
	sk->secret_key_data = NULL;
}

OQS_API void OQS_SIG_STFL_SECRET_KEY_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return;
	}

	/* Call object specific free */
	if (sk->free_key) {
		sk->free_key(sk);
	}
	OQS_MEM_secure_free(sk, sizeof(sk));
}
