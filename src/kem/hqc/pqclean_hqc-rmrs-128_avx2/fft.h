#ifndef FFT_H
#define FFT_H


/**
 * @file fft.h
 * Header file of fft.c
 */

#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQCRMRS128_AVX2_fft(uint16_t *w, const uint16_t *f, size_t f_coeffs);

void PQCLEAN_HQCRMRS128_AVX2_fft_retrieve_error_poly(uint8_t *error, const uint16_t *w);


#endif
