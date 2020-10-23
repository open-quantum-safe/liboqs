#ifndef FFT_H
#define FFT_H


/**
 * @file fft.h
 * Header file of fft.c
 */

#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC256_CLEAN_fft_t(uint16_t *f, const uint16_t *w, size_t f_coeffs);

void PQCLEAN_HQC256_CLEAN_fft_t_preprocess_bch_codeword(uint16_t *w, const uint64_t *vector);


void PQCLEAN_HQC256_CLEAN_fft(uint16_t *w, const uint16_t *f, size_t f_coeffs);

void PQCLEAN_HQC256_CLEAN_fft_retrieve_bch_error_poly(uint64_t *error, const uint16_t *w);


#endif
