#ifndef NTT_H
#define NTT_H

#include <stdint.h>

#include "nttconsts.h"
#include "params.h"

void PQCLEAN_DILITHIUM2_AVX2_ntt_levels0t2_avx(
    uint64_t *tmp,
    const uint32_t *a,
    const uint32_t *zetas
);
void PQCLEAN_DILITHIUM2_AVX2_ntt_levels3t8_avx(
    uint32_t *a,
    const uint64_t *tmp,
    const uint32_t *zetas
);

void PQCLEAN_DILITHIUM2_AVX2_invntt_levels0t4_avx(
    uint64_t *tmp,
    const uint32_t *a,
    const uint32_t *zetas_inv
);
void PQCLEAN_DILITHIUM2_AVX2_invntt_levels5t7_avx(
    uint32_t *a,
    const uint64_t *tmp,
    const uint32_t *zetas_inv
);

void PQCLEAN_DILITHIUM2_AVX2_pointwise_avx(
    uint32_t *c, const uint32_t *a, const uint32_t *b);
void PQCLEAN_DILITHIUM2_AVX2_pointwise_acc_avx(
    uint32_t *c, const uint32_t *a, const uint32_t *b);

#endif
