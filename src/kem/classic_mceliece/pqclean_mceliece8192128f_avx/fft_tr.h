#ifndef PQCLEAN_MCELIECE8192128F_AVX_FFT_TR_H
#define PQCLEAN_MCELIECE8192128F_AVX_FFT_TR_H
/*
  This file is for transpose of the Gao-Mateer FFT
*/


#include "params.h"
#include "vec256.h"

void PQCLEAN_MCELIECE8192128F_AVX_fft_tr(vec256 * /*out*/, vec256  /*in*/[][ GFBITS ]);

#endif

