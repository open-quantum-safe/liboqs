#ifndef HQC_H
#define HQC_H


/**
 * @file hqc.h
 * @brief Functions of the HQC_PKE IND_CPA scheme
 */

#include <stdint.h>

void PQCLEAN_HQC128_CLEAN_hqc_pke_keygen(uint8_t *pk, uint8_t *sk);

void PQCLEAN_HQC128_CLEAN_hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint8_t *m, unsigned char *theta, const unsigned char *pk);

uint8_t PQCLEAN_HQC128_CLEAN_hqc_pke_decrypt(uint8_t *m, uint8_t *sigma, const uint64_t *u, const uint64_t *v, const unsigned char *sk);


#endif
