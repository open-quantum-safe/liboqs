#ifndef FFT_TR_H
#define FFT_TR_H
/*
  This file is for transpose of the Gao-Mateer FFT
*/

#include "namespace.h"

#define fft_tr CRYPTO_NAMESPACE(fft_tr)

#include "params.h"
#include "vec256.h"

void fft_tr(vec256 *out, vec256 in[][ GFBITS ]);

#endif
