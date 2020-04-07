#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

int16_t PQCLEAN_KYBER768_AVX2_reduce_avx(int16_t *r);
int16_t PQCLEAN_KYBER768_AVX2_csubq_avx(int16_t *r);
int16_t PQCLEAN_KYBER768_AVX2_frommont_avx(int16_t *r);

#endif
