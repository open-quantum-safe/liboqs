/**
 * @file hqc.h
 * @brief Header file for hqc.c
 */

#ifndef HQC_HQC_H
#define HQC_HQC_H

#include <immintrin.h>
#include <stdint.h>
#include "parameters.h"
#include "parsing.h"

#include "namespace.h"
#define hqc_pke_keygen HQC_NS(hqc_pke_keygen)
#define hqc_pke_encrypt HQC_NS(hqc_pke_encrypt)
#define hqc_pke_decrypt HQC_NS(hqc_pke_decrypt)

void hqc_pke_keygen(uint8_t *ek_pke, uint8_t *dk_pke, uint8_t *seed);
void hqc_pke_encrypt(uint64_t *u, uint64_t *v, const uint8_t *ek_pke, const uint64_t *m, const uint8_t *theta);
uint8_t hqc_pke_decrypt(uint64_t *m, const uint8_t *dk_pke, const uint64_t *u, const uint64_t *v);

#endif  // HQC_HQC_H
