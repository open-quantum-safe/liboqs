// SPDX-License-Identifier: MIT
#ifndef OQS_OSSL_HELPERS_H
#define OQS_OSSL_HELPERS_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <oqs/oqs.h>

#if defined(OQS_USE_OPENSSL)

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

void oqs_ossl_destroy(void);

// some function pointers for algorithm-switching, see discussion in
// https://github.com/open-quantum-safe/liboqs/pull/735
// and https://wiki.openssl.org/index.php/Library_Initialization
// OQS_OPENSSL_GUARD does not work on these function pointers, so we check return codes manually
extern int (*OQS_OSSL_FUNC(ERR_print_errors_fp))(FILE *);

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

#define OQS_OSSL_FUNC(name) name

#endif

#endif // OQS_OSSL_HELPERS_H
