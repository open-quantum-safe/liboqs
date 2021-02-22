#ifndef PQCLEAN_MCELIECE348864_AVX_VEC_H
#define PQCLEAN_MCELIECE348864_AVX_VEC_H

#include <stdint.h>


void PQCLEAN_MCELIECE348864_AVX_vec_mul(uint64_t *h, const uint64_t *f, const uint64_t *g);

void PQCLEAN_MCELIECE348864_AVX_vec_mul_sp(uint64_t *h, const uint64_t *f, const uint64_t *g);

void PQCLEAN_MCELIECE348864_AVX_vec_add(uint64_t *h, const uint64_t *f, const uint64_t *g);

#endif
