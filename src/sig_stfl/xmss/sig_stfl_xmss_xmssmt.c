// SPDX-License-Identifier: Apache-2.0 AND MIT

#include <string.h>
#include <stdlib.h>

#include <oqs/oqs.h>
#include "sig_stfl_xmss.h"

#include "external/xmss.h"

#if defined(__GNUC__) || defined(__clang__)
#define XMSS_UNUSED_ATT __attribute__((unused))
#else
#define XMSS_UNUSED_ATT
#endif


// macro to en/disable OQS_SIG_STFL-only structs used only in sig&gen case:
#ifdef OQS_ALLOW_XMSS_KEY_AND_SIG_GEN
#define XMSS_SIGGEN(xmss_v, XMSS_V) \
        sig->oid = OQS_SIG_STFL_alg_xmss##xmss_v##_oid; \
        sig->sigs_remaining = OQS_SIG_STFL_alg_xmss##xmss_v##_sigs_remaining;\
        sig->sigs_total = OQS_SIG_STFL_alg_xmss##xmss_v##_sigs_total;\
        sig->keypair = OQS_SIG_STFL_alg_xmss##xmss_v##_keypair;\
        sig->sign = OQS_SIG_STFL_alg_xmss##xmss_v##_sign;        
#else
#define XMSS_SIGGEN(xmss_v, XMSS_V)
#endif

// generator for all alg-specific functions:
#define XMSS_ALG(mt, xmss_v, XMSS_V) \
OQS_SIG_STFL *OQS_SIG_STFL_alg_xmss##xmss_v##_new(void) { \
\
        OQS_SIG_STFL *sig = (OQS_SIG_STFL *)malloc(sizeof(OQS_SIG_STFL)); \
        if (sig == NULL) { \
                return NULL; \
        } \
        memset(sig, 0, sizeof(OQS_SIG_STFL)); \
\
        XMSS_SIGGEN(xmss_v, XMSS_V) \
        sig->method_name = OQS_SIG_STFL_alg_xmss##xmss_v; \
        sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8391"; \
        sig->euf_cma = true; \
\
        sig->length_public_key = OQS_SIG_STFL_alg_xmss##xmss_v##_length_pk; \
        sig->length_secret_key = OQS_SIG_STFL_alg_xmss##xmss_v##_length_sk; \
        sig->length_signature = OQS_SIG_STFL_alg_xmss##xmss_v##_length_signature; \
\
        sig->verify = OQS_SIG_STFL_alg_xmss##xmss_v##_verify;\
\
        return sig;\
} \
\
OQS_SIG_STFL_SECRET_KEY *OQS_SECRET_KEY_XMSS##XMSS_V##_new(void) {\
        return OQS_SECRET_KEY_XMSS_new(OQS_SIG_STFL_alg_xmss##xmss_v##_length_sk);\
}\
\
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss##xmss_v##_keypair(XMSS_UNUSED_ATT uint8_t *public_key, XMSS_UNUSED_ATT OQS_SIG_STFL_SECRET_KEY *secret_key) {\
\
        if (public_key == NULL || secret_key == NULL || secret_key->secret_key_data == NULL) {\
                return OQS_ERROR;\
        }\
\
        if (xmss##mt##_keypair(public_key, secret_key->secret_key_data, OQS_SIG_STFL_alg_xmss##xmss_v##_oid)) {\
                return OQS_ERROR;\
        }\
\
        return OQS_SUCCESS;\
}\
\
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss##xmss_v##_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key) {\
        return OQS_SIG_STFL_alg_xmss##mt##_sign(signature, signature_len, message, message_len, secret_key);\
}\
\
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss##xmss_v##_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {\
        return OQS_SIG_STFL_alg_xmss##mt##_verify(message, message_len, signature, signature_len, public_key);\
}\
\
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss##xmss_v##_sigs_remaining(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key) {\
        return OQS_SIG_STFL_alg_xmss##mt##_sigs_remaining(remain, secret_key);\
}\
\
OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss##xmss_v##_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key) {\
        return OQS_SIG_STFL_alg_xmss##mt##_sigs_total(total, secret_key);\
}


// #ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h10
// XMSS_ALG(, _sha512_h10, _SHA512_H10)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h16
// XMSS_ALG(, _sha512_h16, _SHA512_H16)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmss_sha512_h20
// XMSS_ALG(, _sha512_h20, _SHA512_H20)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h10
// XMSS_ALG(, _shake128_h10, _SHAKE128_H10)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h16
// XMSS_ALG(, _shake128_h16, _SHAKE128_H16)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmss_shake128_h20
// XMSS_ALG(, _shake128_h20, _SHAKE128_H20)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h10
// XMSS_ALG(, _shake256_h10, _SHAKE256_H10)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h16
// XMSS_ALG(, _shake256_h16, _SHAKE256_H16)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmss_shake256_h20
// XMSS_ALG(, _shake256_h20, _SHAKE256_H20)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_2
// XMSS_ALG(mt, mt_sha256_h20_2, MT_SHA256_H20_2)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h20_4
// XMSS_ALG(mt, mt_sha256_h20_4, MT_SHA256_H20_4)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_2
// XMSS_ALG(mt, mt_sha256_h40_2, MT_SHA256_H40_2)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_4
// XMSS_ALG(mt, mt_sha256_h40_4, MT_SHA256_H40_4)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h40_8
// XMSS_ALG(mt, mt_sha256_h40_8, MT_SHA256_H40_8)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_3
// XMSS_ALG(mt, mt_sha256_h60_3, MT_SHA256_H60_3)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_6
// XMSS_ALG(mt, mt_sha256_h60_6, MT_SHA256_H60_6)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_sha256_h60_12
// XMSS_ALG(mt, mt_sha256_h60_12, MT_SHA256_H60_12)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_2
// XMSS_ALG(mt, mt_shake128_h20_2, MT_SHAKE128_H20_2)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h20_4
// XMSS_ALG(mt, mt_shake128_h20_4, MT_SHAKE128_H20_4)
// #endif
// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_2
// XMSS_ALG(mt, mt_shake128_h40_2, MT_SHAKE128_H40_2)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_4
// XMSS_ALG(mt, mt_shake128_h40_4, MT_SHAKE128_H40_4)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h40_8
// XMSS_ALG(mt, mt_shake128_h40_8, MT_SHAKE128_H40_8)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_3
// XMSS_ALG(mt, mt_shake128_h60_3, MT_SHAKE128_H60_3)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_6
// XMSS_ALG(mt, mt_shake128_h60_6, MT_SHAKE128_H60_6)
// #endif

// #ifdef OQS_ENABLE_SIG_STFL_xmssmt_shake128_h60_12
// XMSS_ALG(mt, mt_shake128_h60_12, MT_SHAKE128_H60_12)
// #endif
