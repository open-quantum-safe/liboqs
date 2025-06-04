// SPDX-License-Identifier: MIT

#include <string.h>
#include <stdio.h>
#include <oqs/oqs.h>
#include "./external/config.h"
#include "sig_stfl_lms_wrap.h"
#include "sig_stfl_lms.h"

OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w1_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w1_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w1_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h25_w1_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h25_w2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h25_w4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h25_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w1_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h5_w8_h5_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w4_h5_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w8_h5_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w2_h10_w2_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w4_h10_w4_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h10_w8_h10_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w8_h5_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w8_h10_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h15_w8_h15_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w8_h5_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w8_h10_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w8_h15_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);
OQS_STATUS OQS_SIG_STFL_alg_lms_sha256_h20_w8_h20_w8_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key);

/* Convert LMS secret key object to byte string */
static OQS_STATUS OQS_SECRET_KEY_LMS_serialize_key(uint8_t **sk_buf_ptr, size_t *sk_len, const OQS_SIG_STFL_SECRET_KEY *sk);

/* Insert lms byte string in an LMS secret key object */
static OQS_STATUS OQS_SECRET_KEY_LMS_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, const uint8_t *sk_buf, const size_t sk_len, void *context);

static void OQS_SECRET_KEY_LMS_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context);

// ======================== LMS Maccros ======================== //
// macro to en/disable OQS_SIG_STFL-only structs used only in sig&gen case:
#ifdef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
#define LMS_SIGGEN(lms_variant, LMS_VARIANT) \
        sig->oid = OQS_LMS_ID_##lms_variant; \
        sig->sigs_remaining = OQS_SIG_STFL_lms_sigs_left; \
        sig->sigs_total = OQS_SIG_STFL_lms_sigs_total; \
        sig->keypair = OQS_SIG_STFL_alg_lms_##lms_variant##_keypair; \
        sig->sign = OQS_SIG_STFL_alg_lms_sign;
#else
#define LMS_SIGGEN(lms_variant, LMS_VARIANT)
#endif
// generator for all alg-specific functions:
#define LMS_ALG(lms_variant, LMS_VARIANT) \
OQS_SIG_STFL *OQS_SIG_STFL_alg_lms_##lms_variant##_new(void) { \
\
        OQS_SIG_STFL *sig = (OQS_SIG_STFL *)OQS_MEM_malloc(sizeof(OQS_SIG_STFL)); \
        if (sig == NULL) { \
                return NULL; \
        } \
        memset(sig, 0, sizeof(OQS_SIG_STFL)); \
\
        LMS_SIGGEN(lms_variant, ) \
        sig->method_name = OQS_SIG_STFL_alg_lms_##lms_variant; \
        sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8554"; \
        sig->euf_cma = true; \
        sig->suf_cma = false; \
\
        sig->length_public_key = OQS_SIG_STFL_alg_lms_length_public_key; \
        sig->length_secret_key = OQS_SIG_STFL_alg_lms_length_private_key; \
        sig->length_signature = OQS_SIG_STFL_alg_lms_##lms_variant##_length_signature; \
\
        sig->verify = OQS_SIG_STFL_alg_lms_verify; \
\
        return sig;\
} \
\
OQS_STATUS OQS_SIG_STFL_alg_lms_##lms_variant##_keypair(uint8_t *public_key, OQS_SIG_STFL_SECRET_KEY *secret_key) {\
        if (secret_key == NULL || public_key == NULL) {\
                return OQS_ERROR;\
        }\
\
        if (oqs_sig_stfl_lms_keypair(public_key, secret_key, (const uint32_t)OQS_LMS_ID_##lms_variant) != 0) {\
                return OQS_ERROR;\
        }\
        return OQS_SUCCESS;\
}\
\
OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_LMS_##LMS_VARIANT##_new(void) {\
\
        OQS_SIG_STFL_SECRET_KEY *sk = OQS_MEM_malloc(sizeof(OQS_SIG_STFL_SECRET_KEY));\
        if (sk == NULL) {\
                return NULL;\
        }\
        OQS_MEM_cleanse(sk, sizeof(OQS_SIG_STFL_SECRET_KEY));\
\
        sk->length_secret_key = OQS_SIG_STFL_alg_lms_length_private_key;\
\
        sk->serialize_key = OQS_SECRET_KEY_LMS_serialize_key;\
\
        sk->deserialize_key = OQS_SECRET_KEY_LMS_deserialize_key;\
\
        sk->lock_key = NULL;\
\
        sk->unlock_key = NULL;\
\
        sk->secure_store_scrt_key = NULL;\
\
        sk->free_key = OQS_SECRET_KEY_LMS_free;\
\
        sk->set_scrt_key_store_cb = OQS_SECRET_KEY_LMS_set_store_cb;\
\
        return sk;\
}

// ======================== LMS-SHA256 H5/W1 ======================== //
LMS_ALG(sha256_h5_w1, SHA256_H5_W1)

// ======================== LMS-SHA256 H5/W2 ======================== //

LMS_ALG(sha256_h5_w2, SHA256_H5_W2)

// ======================== LMS-SHA256 H5/W4 ======================== //

LMS_ALG(sha256_h5_w4, SHA256_H5_W4)

// ======================== LMS-SHA256 H5/W8 ======================== //

LMS_ALG(sha256_h5_w8, SHA256_H5_W8)

// ======================== LMS-SHA256 H10/W1 ======================== //

LMS_ALG(sha256_h10_w1, SHA256_H10_W1)

// ======================== LMS-SHA256 H10/W2 ======================== //

LMS_ALG(sha256_h10_w2, SHA256_H10_W2)

// ======================== LMS-SHA256 H10/W4 ======================== //

LMS_ALG(sha256_h10_w4, SHA256_H10_W4)

// ======================== LMS-SHA256 H10/W8 ======================== //

LMS_ALG(sha256_h10_w8, SHA256_H10_W8)

// ======================== LMS-SHA256 H15/W1 ======================== //

LMS_ALG(sha256_h15_w1, SHA256_H15_W1)

// ======================== LMS-SHA256 H15/W2 ======================== //

LMS_ALG(sha256_h15_w2, SHA256_H15_W2)

// ======================== LMS-SHA256 H15/W4 ======================== //

LMS_ALG(sha256_h15_w4, SHA256_H15_W4)

// ======================== LMS-SHA256 H15/W8 ======================== //

LMS_ALG(sha256_h15_w8, SHA256_H15_W8)

// ======================== LMS-SHA256 H20/W1 ======================== //

LMS_ALG(sha256_h20_w1, SHA256_H20_W1)

// // ======================== LMS-SHA256 H20/W2 ======================== //

LMS_ALG(sha256_h20_w2, SHA256_H20_W2)

// ======================== LMS-SHA256 H20/W4 ======================== //

LMS_ALG(sha256_h20_w4, SHA256_H20_W4)

// ======================== LMS-SHA256 H20/W8 ======================== //

LMS_ALG(sha256_h20_w8, SHA256_H20_W8)

// ======================== LMS-SHA256 H25/W1 ======================== //

LMS_ALG(sha256_h25_w1, SHA256_H25_W1)

// ======================== LMS-SHA256 H25/W2 ======================== //

LMS_ALG(sha256_h25_w2, SHA256_H25_W2)

// ======================== LMS-SHA256 H25/W4 ======================== //

LMS_ALG(sha256_h25_w4, SHA256_H25_W4)

// ======================== LMS-SHA256 H25/W8 ======================== //

LMS_ALG(sha256_h25_w8, SHA256_H25_W8)

//
//2-Level LMS
// ======================== LMS-SHA256 H5/W8, H5/W8 ======================== //

LMS_ALG(sha256_h5_w8_h5_w8, SHA256_H5_W8_H5_W8)

// ======================== LMS-SHA256 H10/W2, H10/W2 ======================== //

LMS_ALG(sha256_h10_w2_h10_w2, SHA256_H10_W2_H10_W2)

// ======================== LMS-SHA256 H10/W4, H5/W8 ======================== //

LMS_ALG(sha256_h10_w4_h5_w8, SHA256_H10_W4_H5_W8)

// ======================== LMS-SHA256 H10/W4, H10/W4 ======================== //

LMS_ALG(sha256_h10_w4_h10_w4, SHA256_H10_W4_H10_W4)

// ======================== LMS-SHA256 H10/W8, H5/W8 ======================== //

LMS_ALG(sha256_h10_w8_h5_w8, SHA256_H10_W8_H5_W8)

// ======================== LMS-SHA256 H10/W8, H10/W8 ======================== //

LMS_ALG(sha256_h10_w8_h10_w8, SHA256_H10_W8_H10_W8)

// ======================== LMS-SHA256 H15/W8, H5/W8 ======================== //

LMS_ALG(sha256_h15_w8_h5_w8, SHA256_H15_W8_H5_W8)

// ======================== LMS-SHA256 H15/W8, H10/W8 ======================== //

LMS_ALG(sha256_h15_w8_h10_w8, SHA256_H15_W8_H10_W8)

// ======================== LMS-SHA256 H15/W8, H15/W8 ======================== //

LMS_ALG(sha256_h15_w8_h15_w8, SHA256_H15_W8_H15_W8)

// ======================== LMS-SHA256 H20/W8, H5/W8 ======================== //

LMS_ALG(sha256_h20_w8_h5_w8, SHA256_H20_W8_H5_W8)

// ======================== LMS-SHA256 H20/W8, H10/W8 ======================== //

LMS_ALG(sha256_h20_w8_h10_w8, SHA256_H20_W8_H10_W8)

// ======================== LMS-SHA256 H20/W8, H15/W8 ======================== //

LMS_ALG(sha256_h20_w8_h15_w8, SHA256_H20_W8_H15_W8)

// ======================== LMS-SHA256 H20/W8, H20/W8 ======================== //

LMS_ALG(sha256_h20_w8_h20_w8, SHA256_H20_W8_H20_W8)
//2-Level LMS

void OQS_SECRET_KEY_LMS_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	oqs_secret_lms_key_free(sk);
}

/* Convert LMS secret key object to byte string */
static OQS_STATUS OQS_SECRET_KEY_LMS_serialize_key(uint8_t **sk_buf_ptr, size_t *sk_len, const OQS_SIG_STFL_SECRET_KEY *sk) {
	OQS_STATUS status;
	if (sk->lock_key && sk->mutex) {
		sk->lock_key(sk->mutex);
	}

	status = oqs_serialize_lms_key(sk_buf_ptr, sk_len, sk);

	if (sk->unlock_key && sk->mutex) {
		sk->unlock_key(sk->mutex);
	}
	return status;
}

/* Insert lms byte string in an LMS secret key object */
static OQS_STATUS OQS_SECRET_KEY_LMS_deserialize_key(OQS_SIG_STFL_SECRET_KEY *sk, const uint8_t *sk_buf, const size_t sk_len, void *context) {
	return oqs_deserialize_lms_key(sk, sk_buf, sk_len, context);
}

static void OQS_SECRET_KEY_LMS_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context) {
	if (sk && store_cb && context) {
		oqs_lms_key_set_store_cb(sk, store_cb, context);
	}
}
