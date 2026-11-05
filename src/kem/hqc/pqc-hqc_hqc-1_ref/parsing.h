/**
 * @file parsing.h
 * @brief Header file for parsing.c
 */

#ifndef HQC_PARSING_H
#define HQC_PARSING_H

#include <stdint.h>
#include "parameters.h"
#include "namespace.h"

#define hqc_dk_pke_from_string PQCHQC_NAMESPACE(hqc_dk_pke_from_string)
#define hqc_ek_pke_from_string PQCHQC_NAMESPACE(hqc_ek_pke_from_string)
#define hqc_c_kem_to_string PQCHQC_NAMESPACE(hqc_c_kem_to_string)
#define hqc_c_kem_from_string PQCHQC_NAMESPACE(hqc_c_kem_from_string)

void hqc_dk_pke_from_string(uint64_t *y, const uint8_t *dk_pke);
void hqc_ek_pke_from_string(uint64_t *h, uint64_t *s, const uint8_t *ek_pke);

void hqc_c_kem_to_string(uint8_t *ct, const uint64_t *u, const uint64_t *v, const uint8_t *salt);
void hqc_c_kem_from_string(uint64_t *u, uint64_t *v, uint8_t *salt, const uint8_t *ct);

#endif  // HQC_PARSING_H
