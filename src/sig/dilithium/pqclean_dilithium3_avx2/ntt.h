#ifndef NTT_H
#define NTT_H

#include <stdint.h>
#include "params.h"

#define ntt_avx DILITHIUM_NAMESPACE(_ntt_avx)
void ntt_avx(uint32_t a[N], const uint32_t *qdata);
#define invntt_avx DILITHIUM_NAMESPACE(_invntt_avx)
void invntt_avx(uint32_t a[N], const uint32_t *qdata);

#define pointwise_avx DILITHIUM_NAMESPACE(_pointwise_avx)
void pointwise_avx(uint32_t c[N],
                   const uint32_t a[N],
                   const uint32_t b[N],
                   const uint32_t *qdata);
#define pointwise_acc_avx DILITHIUM_NAMESPACE(_pointwise_acc_avx)
void pointwise_acc_avx(uint32_t c[N],
                       const uint32_t *a,
                       const uint32_t *b,
                       const uint32_t *qdata);

#endif
