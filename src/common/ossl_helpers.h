// SPDX-License-Identifier: MIT
#ifndef OQS_OSSL_HELPERS_H
#define OQS_OSSL_HELPERS_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#if defined(OQS_USE_OPENSSL)
void oqs_ossl_destroy(void);

void oqs_thread_stop(void);

const EVP_MD *oqs_sha256(void);

const EVP_MD *oqs_sha384(void);

const EVP_MD *oqs_sha512(void);

const EVP_MD *oqs_shake128(void);

const EVP_MD *oqs_shake256(void);

const EVP_MD *oqs_sha3_256(void);

const EVP_MD *oqs_sha3_384(void);

const EVP_MD *oqs_sha3_512(void);

const EVP_CIPHER *oqs_aes_128_ecb(void);

const EVP_CIPHER *oqs_aes_128_ctr(void);

const EVP_CIPHER *oqs_aes_256_ecb(void);

const EVP_CIPHER *oqs_aes_256_ctr(void);

#if OPENSSL_VERSION_NUMBER >= 0x30300000L
/* Returns nonzero iff the active OpenSSL provider implements
 * EVP_DigestSqueeze (probed once per process against a SHAKE128 context). */
int oqs_ossl_can_digest_squeeze(void);
#endif

#ifdef OQS_DLOPEN_OPENSSL

#define FUNC(ret, name, args, cargs) ret _oqs_ossl_##name args;
#define VOID_FUNC FUNC
#define OPTIONAL_FUNC(ret, name, args, cargs) \
    ret _oqs_ossl_##name args;                \
    int _oqs_ossl_has_##name(void);
#include "ossl_functions.h"
#undef OPTIONAL_FUNC
#undef VOID_FUNC
#undef FUNC

#define OSSL_FUNC(name) _oqs_ossl_##name
#define OSSL_FUNC_AVAILABLE(name) (_oqs_ossl_has_##name())

#else

#define OSSL_FUNC(name) name
/* In non-DLOPEN mode the linker has already bound the symbol, so it is always
 * available when the corresponding header guard admits the call site. */
#define OSSL_FUNC_AVAILABLE(name) 1

#endif

#endif

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // OQS_OSSL_HELPERS_H
