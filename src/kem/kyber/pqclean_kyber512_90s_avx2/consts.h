#ifndef CONSTS_H
#define CONSTS_H

#include <immintrin.h>
#include <stdint.h>

typedef union {
    uint16_t as_arr[16];
    __m256i as_vec;
} aligned_uint16_t;

extern const uint16_t PQCLEAN_KYBER51290S_AVX2_zetas_exp[396];
extern const uint16_t PQCLEAN_KYBER51290S_AVX2_zetas_inv_exp[396];

extern const aligned_uint16_t PQCLEAN_KYBER51290S_AVX2_16xq;
extern const aligned_uint16_t PQCLEAN_KYBER51290S_AVX2_16xqinv;
extern const aligned_uint16_t PQCLEAN_KYBER51290S_AVX2_16xv;
extern const aligned_uint16_t PQCLEAN_KYBER51290S_AVX2_16xflo;
extern const aligned_uint16_t PQCLEAN_KYBER51290S_AVX2_16xfhi;
extern const aligned_uint16_t PQCLEAN_KYBER51290S_AVX2_16xmontsqlo;
extern const aligned_uint16_t PQCLEAN_KYBER51290S_AVX2_16xmontsqhi;
extern const aligned_uint16_t PQCLEAN_KYBER51290S_AVX2_16xmask;

#endif
