/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: API header file
**************************************************************************************/

#include "params.h"
#include <stdint.h>

#if defined(_qTESLA_p_I_)
#define CRYPTO_ALGNAME "qTesla-p-I"
#elif defined(_qTESLA_p_III_)
#define CRYPTO_ALGNAME "qTesla-p-III"
#endif

#define CRYPTO_RANDOMBYTES 32
#define CRYPTO_SEEDBYTES 32
#define CRYPTO_C_BYTES 32
#define HM_BYTES 64

// Contains signature (z,c). z is a polynomial bounded by B, c is the output of a hashed string
#define CRYPTO_BYTES ((PARAM_N * (PARAM_B_BITS + 1) + 7) / 8 + CRYPTO_C_BYTES)
// Contains polynomial s and e, and seeds seed_a and seed_y
#define CRYPTO_SECRETKEYBYTES ((PARAM_K + 1) * PARAM_S_BITS * PARAM_N / 8 + 2 * CRYPTO_SEEDBYTES)
// Contains seed_a and polynomials t
#define CRYPTO_PUBLICKEYBYTES ((PARAM_K * PARAM_Q_LOG * PARAM_N + 7) / 8 + CRYPTO_SEEDBYTES)

static int crypto_sign_keypair(
    unsigned char *,
    unsigned char *);

static int crypto_sign(
    unsigned char *, unsigned long long *,
    const unsigned char *, unsigned long long,
    const unsigned char *);

static int crypto_sign_open(
    unsigned char *, unsigned long long /* * */,
    const unsigned char *, unsigned long long,
    const unsigned char *);
