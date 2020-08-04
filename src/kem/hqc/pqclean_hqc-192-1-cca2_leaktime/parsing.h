#ifndef PQCLEAN_HQC1921CCA2_LEAKTIME_PARSING_H
#define PQCLEAN_HQC1921CCA2_LEAKTIME_PARSING_H

/**
 * @file parsing.h
 * @brief Header file for parsing.c
 */

#include <stdint.h>

void PQCLEAN_HQC1921CCA2_LEAKTIME_hqc_secret_key_to_string(uint8_t *sk, const uint8_t *sk_seed, const uint8_t *pk);
void PQCLEAN_HQC1921CCA2_LEAKTIME_hqc_secret_key_from_string(uint8_t *x, uint32_t *y, uint8_t *pk, const uint8_t *sk);

void PQCLEAN_HQC1921CCA2_LEAKTIME_hqc_public_key_to_string(uint8_t *pk, const uint8_t *pk_seed, const uint8_t *s);
void PQCLEAN_HQC1921CCA2_LEAKTIME_hqc_public_key_from_string(uint8_t *h, uint8_t *s, const uint8_t *pk);

void PQCLEAN_HQC1921CCA2_LEAKTIME_hqc_ciphertext_to_string(uint8_t *ct, const uint8_t *u, const uint8_t *v, const uint8_t *d);
void PQCLEAN_HQC1921CCA2_LEAKTIME_hqc_ciphertext_from_string(uint8_t *u, uint8_t *v, uint8_t *d, const uint8_t *ct);

#endif
