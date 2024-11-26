#ifndef CONFIG_H
#define CONFIG_H

//#define DILITHIUM_MODE 2
#define DILITHIUM_RANDOMIZED_SIGNING
//#define USE_RDPMC
//#define DBENCH

#ifndef DILITHIUM_MODE
#define DILITHIUM_MODE 2
#endif

#if DILITHIUM_MODE == 2
#define CRYPTO_ALGNAME "ML-DSA-44"
#define DILITHIUM_NAMESPACETOP pqcrystals_ml_dsa_44_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_ml_dsa_44_avx2_##s
#elif DILITHIUM_MODE == 3
#define CRYPTO_ALGNAME "ML-DSA-65"
#define DILITHIUM_NAMESPACETOP pqcrystals_ml_dsa_65_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_ml_dsa_65_avx2_##s
#elif DILITHIUM_MODE == 5
#define CRYPTO_ALGNAME "ML-DSA-87"
#define DILITHIUM_NAMESPACETOP pqcrystals_ml_dsa_87_avx2
#define DILITHIUM_NAMESPACE(s) pqcrystals_ml_dsa_87_avx2_##s
#endif

#endif
