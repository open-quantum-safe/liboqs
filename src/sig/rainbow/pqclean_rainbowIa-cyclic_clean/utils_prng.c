/// @file utils_prng.c
/// @brief The implementation of PRNG related functions.
///

#include "utils_prng.h"
#include "aes.h"
#include "randombytes.h"
#include "utils_hash.h"
#include <stdlib.h>
#include <string.h>

static void prng_update(const unsigned char *provided_data,
                        unsigned char *Key,
                        unsigned char *V) {
    unsigned char temp[48];
    aes256ctx ctx;
    aes256_keyexp(&ctx, Key);
    for (int i = 0; i < 3; i++) {
        //increment V
        for (int j = 15; j >= 0; j--) {
            if (V[j] == 0xff) {
                V[j] = 0x00;
            } else {
                V[j]++;
                break;
            }
        }
        aes256_ecb(temp + 16 * i, V, 1, &ctx);
    }
    if (provided_data != NULL) {
        for (int i = 0; i < 48; i++) {
            temp[i] ^= provided_data[i];
        }
    }
    memcpy(Key, temp, 32);
    memcpy(V, temp + 32, 16);
}
static void randombytes_init_with_state(prng_t *state,
                                        unsigned char *entropy_input_48bytes) {
    memset(state->Key, 0x00, 32);
    memset(state->V, 0x00, 16);
    prng_update(entropy_input_48bytes, state->Key, state->V);
}

static int randombytes_with_state(prng_t *state,
                                  unsigned char *x,
                                  size_t xlen) {

    unsigned char block[16];
    int i = 0;

    aes256ctx ctx;
    aes256_keyexp(&ctx, state->Key);

    while (xlen > 0) {
        //increment V
        for (int j = 15; j >= 0; j--) {
            if (state->V[j] == 0xff) {
                state->V[j] = 0x00;
            } else {
                state->V[j]++;
                break;
            }
        }
        aes256_ecb(block, state->V, 1, &ctx);
        if (xlen > 15) {
            memcpy(x + i, block, 16);
            i += 16;
            xlen -= 16;
        } else {
            memcpy(x + i, block, xlen);
            xlen = 0;
        }
    }
    prng_update(NULL, state->Key, state->V);
    return 0;
}

int PQCLEAN_RAINBOWIACYCLIC_CLEAN_prng_set(prng_t *ctx, const void *prng_seed, unsigned long prng_seedlen) {
    unsigned char seed[48];
    if (prng_seedlen >= 48) {
        memcpy(seed, prng_seed, 48);
    } else {
        memcpy(seed, prng_seed, prng_seedlen);
        PQCLEAN_RAINBOWIACYCLIC_CLEAN_hash_msg(seed + prng_seedlen, 48 - (unsigned)prng_seedlen, (const unsigned char *)prng_seed, prng_seedlen);
    }

    randombytes_init_with_state(ctx, seed);

    return 0;
}

int PQCLEAN_RAINBOWIACYCLIC_CLEAN_prng_gen(prng_t *ctx, unsigned char *out, unsigned long outlen) {
    return randombytes_with_state(ctx, out, outlen);
}
