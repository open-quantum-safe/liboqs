// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0

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
        OQS_SIG_STFL *sig = (OQS_SIG_STFL *)OQS_MEM_malloc(sizeof(OQS_SIG_STFL)); \
        if (sig == NULL) { \
                return NULL; \
        } \
        memset(sig, 0, sizeof(OQS_SIG_STFL)); \
\
        XMSS_SIGGEN(xmss_v, XMSS_V) \
        sig->method_name = OQS_SIG_STFL_alg_xmss##xmss_v; \
        sig->alg_version = "https://datatracker.ietf.org/doc/html/rfc8391"; \
        sig->euf_cma = true; \
        sig->suf_cma = false; \
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
