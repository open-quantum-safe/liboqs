#ifndef PQCLEAN_MCELIECE348864F_AVX_BM_H
#define PQCLEAN_MCELIECE348864F_AVX_BM_H
/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/


#include "vec128.h"

void PQCLEAN_MCELIECE348864F_AVX_bm(uint64_t * /*out*/, vec128 * /*in*/);

#endif

