#ifndef CONFIG_H
#define CONFIG_H

//#define DILITHIUM_MODE 2
//#define DILITHIUM_USE_AES
//#define DILITHIUM_RANDOMIZED_SIGNING
//#define USE_RDPMC
//#define DBENCH

#ifndef DILITHIUM_MODE
#define DILITHIUM_MODE 2
#endif

#ifdef DILITHIUM_USE_AES
#if DILITHIUM_MODE == 2
#define CRYPTO_ALGNAME "Dilithium2-AES"
#define DILITHIUM_NAMESPACETOP pqcrystals_dilithium2aes_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium2aes_avx2_##s
#elif DILITHIUM_MODE == 3
#define CRYPTO_ALGNAME "Dilithium3-AES"
#define DILITHIUM_NAMESPACETOP pqcrystals_dilithium3aes_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium3aes_avx2_##s
#elif DILITHIUM_MODE == 5
#define CRYPTO_ALGNAME "Dilithium5-AES"
#define DILITHIUM_NAMESPACETOP pqcrystals_dilithium5aes_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium5aes_avx2_##s
#endif
#else
#if DILITHIUM_MODE == 2
#define CRYPTO_ALGNAME "Dilithium2"
#define DILITHIUM_NAMESPACETOP pqcrystals_dilithium2_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium2_avx2_##s
#elif DILITHIUM_MODE == 3
#define CRYPTO_ALGNAME "Dilithium3"
#define DILITHIUM_NAMESPACETOP pqcrystals_dilithium3_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium3_avx2_##s
#elif DILITHIUM_MODE == 5
#define CRYPTO_ALGNAME "Dilithium5"
#define DILITHIUM_NAMESPACETOP pqcrystals_dilithium5_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_dilithium5_avx2_##s
#endif
#endif

#endif
