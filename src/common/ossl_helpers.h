// SPDX-License-Identifier: MIT
#ifndef OQS_OSSL_HELPERS_H
#define OQS_OSSL_HELPERS_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <oqs/oqs.h>

#if defined(OQS_USE_OPENSSL) || defined(OQS_USE_AES_OPENSSL) || defined(OQS_USE_SHA2_OPENSSL) || defined(OQS_USE_SHA3_OPENSSL)

#include <stdio.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

void oqs_ossl_destroy(void);
void oqs_thread_stop(void);

// SHA functions (available when SHA2 or SHA3 OpenSSL is enabled)
#if defined(OQS_USE_SHA2_OPENSSL) || defined(OQS_USE_SHA3_OPENSSL)
const EVP_MD *oqs_sha256(void);
const EVP_MD *oqs_sha384(void);
const EVP_MD *oqs_sha512(void);
const EVP_MD *oqs_shake128(void);
const EVP_MD *oqs_shake256(void);
const EVP_MD *oqs_sha3_256(void);
const EVP_MD *oqs_sha3_384(void);
const EVP_MD *oqs_sha3_512(void);
#endif

// AES functions (available when AES OpenSSL is enabled)
#if defined(OQS_USE_AES_OPENSSL)
const EVP_CIPHER *oqs_aes_128_ecb(void);
const EVP_CIPHER *oqs_aes_128_ctr(void);
const EVP_CIPHER *oqs_aes_256_ecb(void);
const EVP_CIPHER *oqs_aes_256_ctr(void);
#endif

#endif

#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus)
#define FUNC(ret, name, args, cargs) ret _oqs_ossl_##name args;
#define VOID_FUNC FUNC
#include "ossl_functions.h"
#undef VOID_FUNC
#undef FUNC

#define OQS_OSSL_FUNC(name) _oqs_ossl_##name

#else

#if defined(OQS_DLOPEN_OPENSSL)
#define FUNC(ret, name, args, cargs) ret _oqs_ossl_##name args;
#define VOID_FUNC FUNC
#include "ossl_functions.h"
#undef VOID_FUNC
#undef FUNC

#define OQS_OSSL_FUNC(name) _oqs_ossl_##name
#else
#define OQS_OSSL_FUNC(name) name
#endif

#endif

// Function declarations are now handled by the FUNC macro expansion above
// when OQS_DLOPEN_OPENSSL is defined, eliminating the need for separate
// function pointer declarations

#endif // OQS_OSSL_HELPERS_H
