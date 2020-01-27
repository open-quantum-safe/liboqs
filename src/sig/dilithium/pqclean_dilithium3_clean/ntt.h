#ifndef PQCLEAN_DILITHIUM3_CLEAN_NTT_H
#define PQCLEAN_DILITHIUM3_CLEAN_NTT_H

#include <stdint.h>

#include "params.h"

void PQCLEAN_DILITHIUM3_CLEAN_ntt(uint32_t p[N]);
void PQCLEAN_DILITHIUM3_CLEAN_invntt_frominvmont(uint32_t p[N]);

#endif
