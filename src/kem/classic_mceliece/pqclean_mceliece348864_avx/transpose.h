#ifndef PQCLEAN_MCELIECE348864_AVX_TRANSPOSE_H
#define PQCLEAN_MCELIECE348864_AVX_TRANSPOSE_H
/*
  This file is for matrix transposition
*/


#include "vec256.h"

#include <stdint.h>


void PQCLEAN_MCELIECE348864_AVX_transpose_64x64(uint64_t *in);
void PQCLEAN_MCELIECE348864_AVX_transpose_64x256_sp(vec256 *in);

#endif

