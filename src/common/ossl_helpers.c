// SPDX-License-Identifier: MIT
#include <oqs/oqs.h>
#if defined(OQS_USE_OPENSSL)
#define OQS_OSSL_NO_EXTERN 1
#include "ossl_helpers.h"
#include <assert.h>
#if !defined(_WIN32)
#include <dlfcn.h>
#endif

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
#include <pthread.h>

static pthread_once_t init_once_control = PTHREAD_ONCE_INIT;
static pthread_once_t free_once_control = PTHREAD_ONCE_INIT;
#endif

static EVP_MD *sha256_ptr, *sha384_ptr, *sha512_ptr,
       *sha3_256_ptr, *sha3_384_ptr, *sha3_512_ptr,
       *shake128_ptr, *shake256_ptr;

static EVP_CIPHER *aes128_ecb_ptr, *aes128_ctr_ptr, *aes256_ecb_ptr, *aes256_ctr_ptr;

static void fetch_ossl_objects(void) {
	sha256_ptr = OSSL_FUNC(EVP_MD_fetch)(NULL, "SHA256", NULL);
	sha384_ptr = OSSL_FUNC(EVP_MD_fetch)(NULL, "SHA384", NULL);
	sha512_ptr = OSSL_FUNC(EVP_MD_fetch)(NULL, "SHA512", NULL);

	sha3_256_ptr = OSSL_FUNC(EVP_MD_fetch)(NULL, "SHA3-256", NULL);
	sha3_384_ptr = OSSL_FUNC(EVP_MD_fetch)(NULL, "SHA3-384", NULL);
	sha3_512_ptr = OSSL_FUNC(EVP_MD_fetch)(NULL, "SHA3-512", NULL);
	shake128_ptr = OSSL_FUNC(EVP_MD_fetch)(NULL, "SHAKE128", NULL);
	shake256_ptr = OSSL_FUNC(EVP_MD_fetch)(NULL, "SHAKE256", NULL);

	aes128_ecb_ptr = OSSL_FUNC(EVP_CIPHER_fetch)(NULL, "AES-128-ECB", NULL);
	aes128_ctr_ptr = OSSL_FUNC(EVP_CIPHER_fetch)(NULL, "AES-128-CTR", NULL);
	aes256_ecb_ptr = OSSL_FUNC(EVP_CIPHER_fetch)(NULL, "AES-256-ECB", NULL);
	aes256_ctr_ptr = OSSL_FUNC(EVP_CIPHER_fetch)(NULL, "AES-256-CTR", NULL);

	if (!sha256_ptr || !sha384_ptr || !sha512_ptr || !sha3_256_ptr ||
	        !sha3_384_ptr || !sha3_512_ptr || !shake128_ptr || !shake256_ptr ||
	        !aes128_ecb_ptr || !aes128_ctr_ptr || !aes256_ecb_ptr || !aes256_ctr_ptr) {
		fprintf(stderr, "liboqs warning: OpenSSL initialization failure. Is provider for SHA, SHAKE, AES enabled?\n");
	}
}

static inline void cleanup_evp_md(EVP_MD **mdp) {
	/* Always check argument is non-NULL before calling EVP_MD_free
	 * to avoid OpenSSL functions being used when they are
	 * overridden with OQS_*_set_callbacks.
	 */
	if (*mdp) {
		OSSL_FUNC(EVP_MD_free)(*mdp);
		*mdp = NULL;
	}
}

static inline void cleanup_evp_cipher(EVP_CIPHER **cipherp) {
	/* Always check argument is non-NULL before calling EVP_CIPHER_free
	 * to avoid OpenSSL functions being used when they are
	 * overridden with OQS_*_set_callbacks.
	 */
	if (*cipherp) {
		OSSL_FUNC(EVP_CIPHER_free)(*cipherp);
		*cipherp = NULL;
	}
}

static void free_ossl_objects(void) {
	cleanup_evp_md(&sha256_ptr);
	cleanup_evp_md(&sha384_ptr);
	cleanup_evp_md(&sha512_ptr);
	cleanup_evp_md(&sha3_256_ptr);
	cleanup_evp_md(&sha3_384_ptr);
	cleanup_evp_md(&sha3_512_ptr);
	cleanup_evp_md(&shake128_ptr);
	cleanup_evp_md(&shake256_ptr);
	cleanup_evp_cipher(&aes128_ecb_ptr);
	cleanup_evp_cipher(&aes128_ctr_ptr);
	cleanup_evp_cipher(&aes256_ecb_ptr);
	cleanup_evp_cipher(&aes256_ctr_ptr);
}
#endif // OPENSSL_VERSION_NUMBER >= 0x30000000L

void oqs_ossl_destroy(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	pthread_once(&free_once_control, free_ossl_objects);
#else
	if (sha256_ptr || sha384_ptr || sha512_ptr || sha3_256_ptr ||
	        sha3_384_ptr || sha3_512_ptr || shake128_ptr || shake256_ptr ||
	        aes128_ecb_ptr || aes128_ctr_ptr || aes256_ecb_ptr || aes256_ctr_ptr) {
		free_ossl_objects();
	}
#endif
#endif
}

void oqs_thread_stop(void) {
	OSSL_FUNC(OPENSSL_thread_stop)();
}

const EVP_MD *oqs_sha256(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!sha256_ptr) {
		fetch_ossl_objects();
	}
#endif
	return sha256_ptr;
#else
	return OSSL_FUNC(EVP_sha256)();
#endif
}

const EVP_MD *oqs_sha384(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!sha384_ptr) {
		fetch_ossl_objects();
	}
#endif
	return sha384_ptr;
#else
	return OSSL_FUNC(EVP_sha384)();
#endif
}

const EVP_MD *oqs_sha512(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!sha512_ptr) {
		fetch_ossl_objects();
	}
#endif
	return sha512_ptr;
#else
	return OSSL_FUNC(EVP_sha512)();
#endif
}

const EVP_MD *oqs_shake128(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!shake128_ptr) {
		fetch_ossl_objects();
	}
#endif
	return shake128_ptr;
#else
	return OSSL_FUNC(EVP_shake128)();
#endif
}

const EVP_MD *oqs_shake256(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!shake256_ptr) {
		fetch_ossl_objects();
	}
#endif
	return shake256_ptr;
#else
	return OSSL_FUNC(EVP_shake256)();
#endif
}

const EVP_MD *oqs_sha3_256(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!sha3_256_ptr) {
		fetch_ossl_objects();
	}
#endif
	return sha3_256_ptr;
#else
	return OSSL_FUNC(EVP_sha3_256)();
#endif
}

const EVP_MD *oqs_sha3_384(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!sha3_384_ptr) {
		fetch_ossl_objects();
	}
#endif
	return sha3_384_ptr;
#else
	return OSSL_FUNC(EVP_sha3_384)();
#endif
}

const EVP_MD *oqs_sha3_512(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!sha3_512_ptr) {
		fetch_ossl_objects();
	}
#endif
	return sha3_512_ptr;
#else
	return OSSL_FUNC(EVP_sha3_512)();
#endif
}

const EVP_CIPHER *oqs_aes_128_ecb(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!aes128_ecb_ptr) {
		fetch_ossl_objects();
	}
#endif
	return aes128_ecb_ptr;
#else
	return OSSL_FUNC(EVP_aes_128_ecb)();
#endif
}

const EVP_CIPHER *oqs_aes_128_ctr(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!aes128_ctr_ptr) {
		fetch_ossl_objects();
	}
#endif
	return aes128_ctr_ptr;
#else
	return OSSL_FUNC(EVP_aes_128_ctr)();
#endif
}

const EVP_CIPHER *oqs_aes_256_ecb(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!aes256_ecb_ptr) {
		fetch_ossl_objects();
	}
#endif
	return aes256_ecb_ptr;
#else
	return OSSL_FUNC(EVP_aes_256_ecb)();
#endif
}

const EVP_CIPHER *oqs_aes_256_ctr(void) {
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#if defined(OQS_USE_PTHREADS)
	if (pthread_once(&init_once_control, fetch_ossl_objects)) {
		return NULL;
	}
#else
	if (!aes256_ctr_ptr) {
		fetch_ossl_objects();
	}
#endif
	return aes256_ctr_ptr;
#else
	return OSSL_FUNC(EVP_aes_256_ctr)();
#endif
}

#if defined(OQS_DLOPEN_OPENSSL)

static void *libcrypto_dlhandle;

static void ensure_library(void);

#if defined(OQS_USE_PTHREADS)
static pthread_once_t dlopen_once_control = PTHREAD_ONCE_INIT;

#define ENSURE_LIBRARY pthread_once(&dlopen_once_control, ensure_library)
#else
#define ENSURE_LIBRARY do {         \
        if (!libcrypto_dlhandle) {  \
            ensure_library();   \
        }               \
    } while (0)
#endif // OQS_USE_PTHREADS

/* Define redirection symbols */
#if (2 <= __GNUC__ || (4 <= __clang_major__))
#define FUNC(ret, name, args, cargs)            \
    static __typeof__(name)(*_oqs_ossl_sym_##name);
#else
#define FUNC(ret, name, args, cargs)        \
    static ret(*_oqs_ossl_sym_##name)args;
#endif
#define VOID_FUNC FUNC
#include "ossl_functions.h"
#undef VOID_FUNC
#undef FUNC

/* Define redirection wrapper functions */
#define FUNC(ret, name, args, cargs)        \
ret _oqs_ossl_##name args           \
{                       \
    ENSURE_LIBRARY;             \
    assert(_oqs_ossl_sym_##name);       \
    return _oqs_ossl_sym_##name cargs;  \
}
#define VOID_FUNC(ret, name, args, cargs)        \
ret _oqs_ossl_##name args           \
{                       \
    ENSURE_LIBRARY;             \
    assert(_oqs_ossl_sym_##name);       \
    _oqs_ossl_sym_##name cargs;  \
}
#include "ossl_functions.h"
#undef VOID_FUNC
#undef FUNC

static void ensure_symbol(const char *name, void **symp) {
	if (!*symp) {
		void *sym = dlsym(libcrypto_dlhandle, name);
		if (!sym) {
			exit(EXIT_FAILURE);
		}
		*symp = sym;
	}
}

static void ensure_library(void) {
	if (!libcrypto_dlhandle) {
		libcrypto_dlhandle = dlopen(OQS_OPENSSL_CRYPTO_SONAME,
		                            RTLD_LAZY | RTLD_LOCAL);
		if (!libcrypto_dlhandle) {
			exit(EXIT_FAILURE);
		}
	}

#define ENSURE_SYMBOL(name) \
    ensure_symbol(#name, (void **)&_oqs_ossl_sym_##name)
#define FUNC(ret, name, args, cargs)        \
    ENSURE_SYMBOL(name);
#define VOID_FUNC FUNC
#include "ossl_functions.h"
#undef VOID_FUNC
#undef FUNC
#undef ENSURE_SYMBOL
}

#endif // OQS_DLOPEN_OPENSSL

#endif
