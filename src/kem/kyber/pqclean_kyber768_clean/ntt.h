#ifndef NTT_H
#define NTT_H

#include <stdint.h>

extern int16_t PQCLEAN_KYBER768_CLEAN_zetas[128];
extern int16_t PQCLEAN_KYBER768_CLEAN_zetas_inv[128];

void PQCLEAN_KYBER768_CLEAN_ntt(int16_t *poly);
void PQCLEAN_KYBER768_CLEAN_invntt(int16_t *poly);
void PQCLEAN_KYBER768_CLEAN_basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta);

#endif
