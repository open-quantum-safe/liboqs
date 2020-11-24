#ifndef HQC_H
#define HQC_H


/**
 * @file hqc.h
 * @brief Functions of the HQC_PKE IND_CPA scheme
 */

#include <stdint.h>

void PQCLEAN_HQCRMRS256_CLEAN_hqc_pke_keygen(unsigned char *pk, unsigned char *sk);

void PQCLEAN_HQCRMRS256_CLEAN_hqc_pke_encrypt(uint64_t *u, uint64_t *v, uint8_t *m, unsigned char *theta, const unsigned char *pk);

void PQCLEAN_HQCRMRS256_CLEAN_hqc_pke_decrypt(uint8_t *m, const uint64_t *u, const uint64_t *v, const unsigned char *sk);


#endif
