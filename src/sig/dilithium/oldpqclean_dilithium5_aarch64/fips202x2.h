
/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * at https://github.com/GMUCERG/PQC_NEON/blob/main/neon/kyber or
 * public domain at https://github.com/cothan/kyber/blob/master/neon
 */

#ifndef FIPS202X2_H
#define FIPS202X2_H

#include "params.h"
#include <arm_neon.h>
#include <stddef.h>

#include <fips202.h>

typedef uint64x2_t v128;


typedef struct {
    v128 s[25];
} keccakx2_state;


#define shake128x2_absorb DILITHIUM_NAMESPACE(shake128x2_absorb)
void shake128x2_absorb(keccakx2_state *state,
                       const uint8_t *in0,
                       const uint8_t *in1,
                       size_t inlen);

#define shake128x2_squeezeblocks DILITHIUM_NAMESPACE(shake128x2_squeezeblocks)
void shake128x2_squeezeblocks(uint8_t *out0,
                              uint8_t *out1,
                              size_t nblocks,
                              keccakx2_state *state);

#define shake256x2_absorb DILITHIUM_NAMESPACE(shake256x2_absorb)
void shake256x2_absorb(keccakx2_state *state,
                       const uint8_t *in0,
                       const uint8_t *in1,
                       size_t inlen);

#define shake256x2_squeezeblocks DILITHIUM_NAMESPACE(shake256x2_squeezeblocks)
void shake256x2_squeezeblocks(uint8_t *out0,
                              uint8_t *out1,
                              size_t nblocks,
                              keccakx2_state *state);

#define shake128x2 DILITHIUM_NAMESPACE(shake128x2)
void shake128x2(uint8_t *out0,
                uint8_t *out1,
                size_t outlen,
                const uint8_t *in0,
                const uint8_t *in1,
                size_t inlen);

#define shake256x2 DILITHIUM_NAMESPACE(shake256x2)
void shake256x2(uint8_t *out0,
                uint8_t *out1,
                size_t outlen,
                const uint8_t *in0,
                const uint8_t *in1,
                size_t inlen);
#endif
