#ifndef PQCLEAN_MCELIECE460896_AVX_BM_H
#define PQCLEAN_MCELIECE460896_AVX_BM_H
/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/

#include "vec128.h"
#include "vec256.h"

void PQCLEAN_MCELIECE460896_AVX_bm(vec128 * /*out*/, vec256 * /*in*/);

#endif

