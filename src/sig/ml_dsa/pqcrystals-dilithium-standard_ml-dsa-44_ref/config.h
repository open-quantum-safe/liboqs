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
#define DILITHIUM_NAMESPACETOP pqcrystals_ml_dsa_44_ref
#define DILITHIUM_NAMESPACE(s) pqcrystals_ml_dsa_44_ref_##s
#elif DILITHIUM_MODE == 3
#define CRYPTO_ALGNAME "ML-DSA-65"
#define DILITHIUM_NAMESPACETOP pqcrystals_ml_dsa_65_ref
#define DILITHIUM_NAMESPACE(s) pqcrystals_ml_dsa_65_ref_##s
#elif DILITHIUM_MODE == 5
#define CRYPTO_ALGNAME "ML-DSA-87"
#define DILITHIUM_NAMESPACETOP pqcrystals_ml_dsa_87_ref
#define DILITHIUM_NAMESPACE(s) pqcrystals_ml_dsa_87_ref_##s
#endif

#endif
