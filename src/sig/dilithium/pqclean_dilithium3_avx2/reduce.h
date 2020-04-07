#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

void PQCLEAN_DILITHIUM3_AVX2_reduce_avx(uint32_t a[N]);
void PQCLEAN_DILITHIUM3_AVX2_csubq_avx(uint32_t a[N]);

#endif
