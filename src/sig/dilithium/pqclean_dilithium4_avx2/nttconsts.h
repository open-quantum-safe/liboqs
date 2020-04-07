#ifndef PQCLEAN_DILITHIUM4_AVX2_NTTCONSTS_H
#define PQCLEAN_DILITHIUM4_AVX2_NTTCONSTS_H

#include <immintrin.h>
#include <stdint.h>

#include "alignment.h"
#include "params.h"

typedef ALIGNED_UINT32(8) aligned_uint32x8_t;

typedef ALIGNED_UINT32(N) aligned_uint32xN_t;


extern const aligned_uint32x8_t _PQCLEAN_DILITHIUM4_AVX2_8xqinv;
extern const aligned_uint32x8_t _PQCLEAN_DILITHIUM4_AVX2_8xq;
extern const aligned_uint32x8_t _PQCLEAN_DILITHIUM4_AVX2_8x2q;
extern const aligned_uint32x8_t _PQCLEAN_DILITHIUM4_AVX2_8x256q;
extern const aligned_uint32x8_t _PQCLEAN_DILITHIUM4_AVX2_mask;
extern const aligned_uint32x8_t _PQCLEAN_DILITHIUM4_AVX2_8x23ones;
extern const aligned_uint32x8_t _PQCLEAN_DILITHIUM4_AVX2_8xdiv;

extern const aligned_uint32xN_t PQCLEAN_DILITHIUM4_AVX2_zetas;
extern const aligned_uint32xN_t PQCLEAN_DILITHIUM4_AVX2_zetas_inv;

#endif //PQCLEAN_DILITHIUM4_AVX2_NTTCONSTS_H

