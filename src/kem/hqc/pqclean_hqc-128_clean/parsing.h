#ifndef PARSING_H
#define PARSING_H

/**
 * @file parsing.h
 * @brief Header file for parsing.c
 */

#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC128_CLEAN_load8_arr(uint64_t *out64, size_t outlen, const uint8_t *in8, size_t inlen);

void PQCLEAN_HQC128_CLEAN_store8_arr(uint8_t *out8, size_t outlen, const uint64_t *in64, size_t inlen);

void PQCLEAN_HQC128_CLEAN_hqc_secret_key_to_string(uint8_t *sk, const uint8_t *sk_seed, const uint8_t *sigma, const uint8_t *pk);

void PQCLEAN_HQC128_CLEAN_hqc_secret_key_from_string(uint64_t *x, uint64_t *y, uint8_t *sigma, uint8_t *pk, const uint8_t *sk);

void PQCLEAN_HQC128_CLEAN_hqc_public_key_to_string(uint8_t *pk, const uint8_t *pk_seed, const uint64_t *s);

void PQCLEAN_HQC128_CLEAN_hqc_public_key_from_string(uint64_t *h, uint64_t *s, const uint8_t *pk);

void PQCLEAN_HQC128_CLEAN_hqc_ciphertext_to_string(uint8_t *ct, const uint64_t *u, const uint64_t *v, const uint8_t *salt);

void PQCLEAN_HQC128_CLEAN_hqc_ciphertext_from_string(uint64_t *u, uint64_t *v, uint8_t *salt, const uint8_t *ct);

#endif
