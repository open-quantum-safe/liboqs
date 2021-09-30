#ifndef PQCLEAN_MCELIECE348864_AVX_BM_H
#define PQCLEAN_MCELIECE348864_AVX_BM_H
/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/


#include "vec128.h"

void PQCLEAN_MCELIECE348864_AVX_bm(uint64_t /*out*/[ GFBITS ], vec128 /*in*/ [ GFBITS ]);

#endif

