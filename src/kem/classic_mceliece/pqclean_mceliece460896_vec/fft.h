#ifndef PQCLEAN_MCELIECE460896_VEC_FFT_H
#define PQCLEAN_MCELIECE460896_VEC_FFT_H
/*
  This file is for the Gao-Mateer FFT
  sse http://www.math.clemson.edu/~sgao/papers/GM10.pdf
*/

#include <stdint.h>

#include "params.h"
#include "vec.h"

void PQCLEAN_MCELIECE460896_VEC_fft(vec out[][ GFBITS ], vec in[][GFBITS]);

#endif

