/**
 * @file fft.h
 * @brief Header file of fft.c
 */

#ifndef HQC_FFT_H
#define HQC_FFT_H

#include <stddef.h>
#include <stdint.h>

void fft(uint16_t *w, const uint16_t *f, size_t f_coeffs);
void fft_retrieve_error_poly(uint8_t *error, const uint16_t *w);

#endif  // HQC_FFT_H
