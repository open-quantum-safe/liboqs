#ifndef CONFIG_H
#define CONFIG_H

//#define DILITHIUM_MODE 3
//#define DILITHIUM_USE_AES
//#define DILITHIUM_RANDOMIZED_SIGNING
//#define USE_RDPMC
//#define DBENCH

#ifndef DILITHIUM_MODE
#define DILITHIUM_MODE 3
#endif

#ifdef DILITHIUM_USE_AES
#if DILITHIUM_MODE == 1
#define CRYPTO_ALGNAME "Dilithium1-AES"
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium1aes_avx2##s
#elif DILITHIUM_MODE == 2
#define CRYPTO_ALGNAME "Dilithium2-AES"
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium2aes_avx2##s
#elif DILITHIUM_MODE == 3
#define CRYPTO_ALGNAME "Dilithium3-AES"
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium3aes_avx2##s
#elif DILITHIUM_MODE == 4
#define CRYPTO_ALGNAME "Dilithium4-AES"
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium4aes_avx2##s
#endif
#else
#if DILITHIUM_MODE == 1
#define CRYPTO_ALGNAME "Dilithium1"
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium1_avx2##s
#elif DILITHIUM_MODE == 2
#define CRYPTO_ALGNAME "Dilithium2"
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium2_avx2##s
#elif DILITHIUM_MODE == 3
#define CRYPTO_ALGNAME "Dilithium3"
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium3_avx2##s
#elif DILITHIUM_MODE == 4
#define CRYPTO_ALGNAME "Dilithium4"
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium4_avx2##s
#endif
#endif

#endif
