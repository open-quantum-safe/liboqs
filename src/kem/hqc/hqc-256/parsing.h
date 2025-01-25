#ifndef HQC256_PARSING_H
#define HQC256_PARSING_H

/**
 * @file parsing.h
 * @brief Header file for parsing.c
 */

#include <stdint.h>
#include <immintrin.h>

void HQC256_hqc_secret_key_to_string(uint8_t *sk, const uint8_t *sk_seed, const uint8_t *sigma, const uint8_t *pk);
void HQC256_hqc_secret_key_from_string(__m256i *y256, uint8_t *sigma, uint8_t *pk, const uint8_t *sk);

void HQC256_hqc_public_key_to_string(uint8_t *pk, const uint8_t *pk_seed, const uint64_t *s);
void HQC256_hqc_public_key_from_string(uint64_t *h, uint64_t *s, const uint8_t *pk);

void HQC256_hqc_ciphertext_to_string(uint8_t *ct, const uint64_t *u, const uint64_t *v, const uint64_t *salt);
void HQC256_hqc_ciphertext_from_string(uint64_t *u, uint64_t *v, uint64_t *salt, const uint8_t *ct);

#endif
