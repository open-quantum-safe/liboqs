#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

int16_t PQCLEAN_KYBER76890S_AVX2_reduce_avx(int16_t *r);
int16_t PQCLEAN_KYBER76890S_AVX2_csubq_avx(int16_t *r);
int16_t PQCLEAN_KYBER76890S_AVX2_frommont_avx(int16_t *r);

#endif
