#ifndef PQCLEAN_MCELIECE8192128_AVX_TRANSPOSE_H
#define PQCLEAN_MCELIECE8192128_AVX_TRANSPOSE_H
/*
  This file is for matrix transposition
*/


#include "vec128.h"
#include "vec256.h"

void PQCLEAN_MCELIECE8192128_AVX_transpose_64x128_sp(vec128 *in);
void PQCLEAN_MCELIECE8192128_AVX_transpose_64x256_sp(vec256 *in);

#endif

