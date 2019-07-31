#ifndef _UTILS_PRNG_H_
#define _UTILS_PRNG_H_
/// @file utils_prng.h
/// @brief the interface for adapting PRNG functions.
///
///

#include "randombytes.h"

typedef struct {
    unsigned char Key[32];
    unsigned char V[16];
} prng_t;

int PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_set(prng_t *ctx, const void *prng_seed, unsigned long prng_seedlen);
int PQCLEAN_RAINBOWIIICCLASSIC_CLEAN_prng_gen(prng_t *ctx, unsigned char *out, unsigned long outlen);

#endif // _UTILS_PRNG_H_
