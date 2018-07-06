/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: API header file
**************************************************************************************/

#include "params.h"
#include <stdint.h>


#define CRYPTO_RANDOMBYTES 32
#define CRYPTO_SEEDBYTES 32
#define CRYPTO_C_BYTES 32

// Contains signature (z,c). z is a polynomial bounded by B, c is the output of a hashed string
#define CRYPTO_BYTES ((PARAM_N*PARAM_D+7)/8 + CRYPTO_C_BYTES)
// Contains polynomial s and e, and seeds seed_a and seed_y
#define CRYPTO_SECRETKEYBYTES (2*sizeof(int16_t)*PARAM_N + 2*CRYPTO_SEEDBYTES)
// Contains seed_a and polynomial t
#define CRYPTO_PUBLICKEYBYTES ((PARAM_N*PARAM_Q_LOG+7)/8 + CRYPTO_SEEDBYTES)


#define CRYPTO_ALGNAME "qTesla-III-size"

int crypto_sign_keypair(
    unsigned char *,
    unsigned char *
    );

int crypto_sign(
    unsigned char *,unsigned long long *,
    const unsigned char *,unsigned long long,
    const unsigned char *
    );

int crypto_sign_open(
    unsigned char *,unsigned long long *,
    const unsigned char *,unsigned long long,
    const unsigned char *
    );


