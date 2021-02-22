#ifndef PQCLEAN_MCELIECE348864F_AVX_FFT_H
#define PQCLEAN_MCELIECE348864F_AVX_FFT_H

/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#include <stdint.h>

#include "params.h"
#include "vec128.h"
#include "vec256.h"

void PQCLEAN_MCELIECE348864F_AVX_fft(vec256  /*out*/[][GFBITS], uint64_t * /*in*/);

#endif

