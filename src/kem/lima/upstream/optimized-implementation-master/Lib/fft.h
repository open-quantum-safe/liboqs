#ifndef _fft
#define _fft

#include "lima.h"

/* Need to call this for all fft algorithms 
 *  - It sets up the FFT data associated to the specific Lima parameters
 */
void precompute_fft_data(lima_params_t* params);

/* This performs the forward FFT
 *   - x=FFT(f)
 *   - Assumes x has size correctly allocated.
 */
void fft(lima_params_t* params, const int f[], int x[]);

/* This performs the inverse FFT
 *   - f=FFT^{-1}(x)
 *   - Assumes f has size correctly allocated.
 */
void fft_inv(lima_params_t* params, const int x[], int f[]);

#endif
