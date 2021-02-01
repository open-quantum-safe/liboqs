#ifndef PQCLEAN_DILITHIUM5_CLEAN_NTT_H
#define PQCLEAN_DILITHIUM5_CLEAN_NTT_H
#include "params.h"
#include <stdint.h>

void PQCLEAN_DILITHIUM5_CLEAN_ntt(int32_t a[N]);

void PQCLEAN_DILITHIUM5_CLEAN_invntt_tomont(int32_t a[N]);

#endif
