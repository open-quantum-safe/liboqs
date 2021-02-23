#ifndef PQCLEAN_MCELIECE348864_SSE_TRANSPOSE_H
#define PQCLEAN_MCELIECE348864_SSE_TRANSPOSE_H
/*
  This file is for matrix transposition
*/


#include "vec128.h"

#include <stdint.h>

void PQCLEAN_MCELIECE348864_SSE_transpose_64x64(uint64_t *in);
void PQCLEAN_MCELIECE348864_SSE_transpose_64x128_sp(vec128 *in);

#endif

