/**
 * @file parsing.h
 * @brief Header file for parsing.c
 */

#ifndef HQC_PARSING_H
#define HQC_PARSING_H

#if defined(__i386__) || defined(__x86_64__)
#include <immintrin.h>
#endif
#include <stdint.h>
#include "data_structures.h"
#include "parameters.h"

void hqc_dk_pke_from_string(uint64_t *y, const uint8_t *dk_pke);
void hqc_ek_pke_from_string(uint64_t *h, uint64_t *s, const uint8_t *ek_pke);

void hqc_c_kem_to_string(uint8_t *ct, const ciphertext_kem_t *c_kem);
void hqc_c_kem_from_string(ciphertext_pke_t *c_pke, uint8_t *salt, const uint8_t *ct);

#endif  // HQC_PARSING_H
