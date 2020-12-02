#ifndef PQCLEAN_KYBER102490S_CLEAN_NTT_H
#define PQCLEAN_KYBER102490S_CLEAN_NTT_H
#include "params.h"
#include <stdint.h>

extern const int16_t PQCLEAN_KYBER102490S_CLEAN_zetas[128];

void PQCLEAN_KYBER102490S_CLEAN_ntt(int16_t r[256]);

void PQCLEAN_KYBER102490S_CLEAN_invntt(int16_t r[256]);

void PQCLEAN_KYBER102490S_CLEAN_basemul(int16_t r[2], const int16_t a[2], const int16_t b[2], int16_t zeta);

#endif
