#ifndef RIJNDAEL256_AVX2__H
#define RIJNDAEL256_AVX2__H

#ifdef __cplusplus
#define EXPORT extern "C"
#include <cstdint>
#include <cstdlib>
#else
#define EXPORT
#include "stdint.h"
#endif
#include <immintrin.h>
#include <wmmintrin.h>
#include <emmintrin.h>
#include <smmintrin.h>

#define Nb 8
#define Nk 8
#define Nbytes (Nb*4)
#define Nr 14

typedef union {
  __m128i v[2];
  uint8_t b[Nbytes];
 } rijndael256_avx_state_t;

#if Nb != Nk
#error "This implementation expects Nb == Nk."
#endif

typedef union {
   uint8_t v[2][16];  // no alignment assumption shall be made here
   uint8_t b[Nbytes];
 } rijndael256_avx_key_t;

typedef struct {
  rijndael256_avx_key_t rk[Nr+1];
} rijndael256_avx_rk_t;

EXPORT void rijndael256_key_schedule_avx(rijndael256_avx_rk_t *roundkeys, const uint8_t key[32]);

// aesenclast-based batched key expansion: expand 1..4 independent rijndael256 keys
// into 1..4 round-key buffers (each RIJNDAEL256_RK_BYTES). Round keys are
// bit-identical to rijndael256_key_schedule_avx (KAT-preserving).
EXPORT void rijndael256_keyexp_x1_enclast(void *rk0, const void *k0);
EXPORT void rijndael256_keyexp_x2_enclast(void *rk0, void *rk1, const void *k0, const void *k1);
EXPORT void rijndael256_keyexp_x3_enclast(void *rk0, void *rk1, void *rk2,  //
                                          const void *k0, const void *k1, const void *k2);
EXPORT void rijndael256_keyexp_x4_enclast(void *rk0, void *rk1, void *rk2, void *rk3,  //
                                          const void *k0, const void *k1, const void *k2, const void *k3);

// NOTE: the ECB / CTR encryption cores (rijndael256_ecb_encrypt_{1,2,4}block_avx and
// rijndael256_ctr_encrypt_2blocks_avx) were removed; the CTR path lives in
// rijndael256_ctrle_special_avx2.c (rijndael256_ctrle_nocarry_* / sdith_rij_encN).

#endif
