#ifndef PQCLEAN_MCELIECE6688128F_VEC_BM_H
#define PQCLEAN_MCELIECE6688128F_VEC_BM_H
/*
  This file is for the inversion-free Berlekamp-Massey algorithm
  see https://ieeexplore.ieee.org/document/87857
*/

#include <stdint.h>

#include "params.h"
#include "vec.h"

void PQCLEAN_MCELIECE6688128F_VEC_bm(vec  /*out*/[][GFBITS], vec  /*in*/[][ GFBITS ]);

#endif

