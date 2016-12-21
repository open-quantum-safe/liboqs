
#ifndef _FFT_CRT_H
#define _FFT_CRT_H

#include "vscrypto_defs.h"

void GLOBAL(fft_forward_1024_8816641)(FFTSHORT *x);
void GLOBAL(fft_backward_1024_8816641)(FFTSHORT *x);

void GLOBAL(fft_forward_2048_8816641)(FFTSHORT *x);
void GLOBAL(fft_backward_2048_8816641)(FFTSHORT *x);

void GLOBAL(fft_forward_1024_17633281)(FFTSHORT *x);
void GLOBAL(fft_backward_1024_17633281)(FFTSHORT *x);

void GLOBAL(fft_forward_2048_17633281)(FFTSHORT *x);
void GLOBAL(fft_backward_2048_17633281)(FFTSHORT *x);

#endif /* fft_crt_MQ.h */

