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

#ifdef OQS_DLOPEN_OPENSSL

#define FUNC(ret, name, args, cargs) ret _oqs_ossl_##name args;
#define VOID_FUNC FUNC
#include "ossl_functions.h"
#undef VOID_FUNC
#undef FUNC

#define OSSL_FUNC(name) _oqs_ossl_##name

#else

#define OSSL_FUNC(name) name

#endif

#endif

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // OQS_OSSL_HELPERS_H
