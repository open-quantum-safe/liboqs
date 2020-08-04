#ifndef PQCLEAN_HQC2561CCA2_LEAKTIME_FFT_H
#define PQCLEAN_HQC2561CCA2_LEAKTIME_FFT_H

/**
 * @file fft.h
 * Header file of fft.c
 */

#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC2561CCA2_LEAKTIME_fft_t(uint16_t *f, const uint16_t *w, size_t f_coeffs);
void PQCLEAN_HQC2561CCA2_LEAKTIME_fft_t_preprocess_bch_codeword(uint16_t *w, const uint8_t *vector);

void PQCLEAN_HQC2561CCA2_LEAKTIME_fft(uint16_t *w, const uint16_t *f, size_t f_coeffs);
void PQCLEAN_HQC2561CCA2_LEAKTIME_fft_retrieve_bch_error_poly(uint8_t *error, const uint16_t *w);

#endif
