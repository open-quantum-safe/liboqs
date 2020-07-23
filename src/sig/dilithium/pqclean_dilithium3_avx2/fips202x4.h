#ifndef FIPS202X4_H
#define FIPS202X4_H

#include <stddef.h>
#include <stdint.h>
#include <immintrin.h>

#define FIPS202X4_NAMESPACE(s) pqcrystals_fips202x4_avx2##s

typedef struct {
  __m256i s[25];
} keccakx4_state;

#define shake128x4_absorb FIPS202X4_NAMESPACE(_shake128x4_absorb)
void shake128x4_absorb(keccakx4_state *state,
                       const uint8_t *in0,
                       const uint8_t *in1,
                       const uint8_t *in2,
                       const uint8_t *in3,
                       size_t inlen);

#define shake128x4_squeezeblocks FIPS202X4_NAMESPACE(_shake128x4_squeezeblocks)
void shake128x4_squeezeblocks(uint8_t *out0,
                              uint8_t *out1,
                              uint8_t *out2,
                              uint8_t *out3,
                              size_t nblocks,
                              keccakx4_state *state);

#define shake256x4_absorb FIPS202X4_NAMESPACE(_shake256x4_absorb)
void shake256x4_absorb(keccakx4_state *state,
                       const uint8_t *in0,
                       const uint8_t *in1,
                       const uint8_t *in2,
                       const uint8_t *in3,
                       size_t inlen);

#define shake256x4_squeezeblocks FIPS202X4_NAMESPACE(_shake256x4_squeezeblocks)
void shake256x4_squeezeblocks(uint8_t *out0,
                              uint8_t *out1,
                              uint8_t *out2,
                              uint8_t *out3,
                              size_t nblocks,
                              keccakx4_state *state);

#define shake128x4 FIPS202X4_NAMESPACE(_shake128x4)
void shake128x4(uint8_t *out0,
                uint8_t *out1,
                uint8_t *out2,
                uint8_t *out3,
                size_t outlen,
                const uint8_t *in0,
                const uint8_t *in1,
                const uint8_t *in2,
                const uint8_t *in3,
                size_t inlen);

#define shake256x4 FIPS202X4_NAMESPACE(_shake256x4)
void shake256x4(uint8_t *out0,
                uint8_t *out1,
                uint8_t *out2,
                uint8_t *out3,
                size_t outlen,
                const uint8_t *in0,
                const uint8_t *in1,
                const uint8_t *in2,
                const uint8_t *in3,
                size_t inlen);

#endif
