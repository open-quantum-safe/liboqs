#ifndef PQCLEAN_MCELIECE348864F_VEC_FFT_H
#define PQCLEAN_MCELIECE348864F_VEC_FFT_H
/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#include "params.h"
#include "vec.h"
#include <stdint.h>

void PQCLEAN_MCELIECE348864F_VEC_fft(vec  /*out*/[][ GFBITS ], vec * /*in*/);

#endif

