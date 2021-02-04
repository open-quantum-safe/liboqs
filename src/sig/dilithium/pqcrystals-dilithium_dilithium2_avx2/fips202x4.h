#ifndef FIPS202X4_H
#define FIPS202X4_H

#define FIPS202X4_NAMESPACE(s) pqcrystals_dilithium_fips202x4_avx2_##s

#ifdef __ASSEMBLER__
/* The C ABI on MacOS exports all symbols with a leading
 * underscore. This means that any symbols we refer to from
 * C files (functions) can't be found, and all symbols we
 * refer to from ASM also can't be found.
 *
 * This define helps us get around this
 */
#if defined(__WIN32__) || defined(__APPLE__)
#define decorate(s) _##s
#define _cdecl(s) decorate(s)
#define cdecl(s) _cdecl(FIPS202X4_NAMESPACE(##s))
#else
#define cdecl(s) FIPS202X4_NAMESPACE(##s)
#endif

#else
#include <stddef.h>
#include <stdint.h>
#include <immintrin.h>

typedef struct {
  __m256i s[25];
} keccakx4_state;

#define f1600x4 FIPS202X4_NAMESPACE(f1600x4)
void f1600x4(__m256i *s, const uint64_t *rc);

#define shake128x4_absorb_once FIPS202X4_NAMESPACE(shake128x4_absorb_once)
void shake128x4_absorb_once(keccakx4_state *state,
                            const uint8_t *in0,
                            const uint8_t *in1,
                            const uint8_t *in2,
                            const uint8_t *in3,
                            size_t inlen);

#define shake128x4_squeezeblocks FIPS202X4_NAMESPACE(shake128x4_squeezeblocks)
void shake128x4_squeezeblocks(uint8_t *out0,
                              uint8_t *out1,
                              uint8_t *out2,
                              uint8_t *out3,
                              size_t nblocks,
                              keccakx4_state *state);

#define shake256x4_absorb_once FIPS202X4_NAMESPACE(shake256x4_absorb_once)
void shake256x4_absorb_once(keccakx4_state *state,
                            const uint8_t *in0,
                            const uint8_t *in1,
                            const uint8_t *in2,
                            const uint8_t *in3,
                            size_t inlen);

#define shake256x4_squeezeblocks FIPS202X4_NAMESPACE(shake256x4_squeezeblocks)
void shake256x4_squeezeblocks(uint8_t *out0,
                              uint8_t *out1,
                              uint8_t *out2,
                              uint8_t *out3,
                              size_t nblocks,
                              keccakx4_state *state);

#define shake128x4 FIPS202X4_NAMESPACE(shake128x4)
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

#define shake256x4 FIPS202X4_NAMESPACE(shake256x4)
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
#endif
