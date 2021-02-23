#ifndef PQCLEAN_MCELIECE348864_SSE_FFT_H
#define PQCLEAN_MCELIECE348864_SSE_FFT_H
/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/


#include <stdint.h>

#include "params.h"
#include "vec128.h"

void PQCLEAN_MCELIECE348864_SSE_fft(vec128  /*out*/[][GFBITS], uint64_t * /*in*/);

#endif

