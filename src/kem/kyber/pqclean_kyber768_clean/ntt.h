#ifndef NTT_H
#define NTT_H

#include <stdint.h>

void PQCLEAN_KYBER768_ntt(uint16_t *p);
void PQCLEAN_KYBER768_invntt(uint16_t *a);

#endif
