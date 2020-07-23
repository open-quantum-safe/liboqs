#ifndef REJSAMPLE_H
#define REJSAMPLE_H

#include <stdint.h>
#include "params.h"

#define rej_uniform_avx DILITHIUM_NAMESPACE(_rej_uniform_avx)
unsigned int rej_uniform_avx(uint32_t *r, const uint8_t *buf);


#define rej_eta_avx DILITHIUM_NAMESPACE(_rej_eta_avx)
unsigned int rej_eta_avx(uint32_t *r,
                         unsigned int len,
                         const uint8_t *buf,
                         unsigned int buflen);

#define rej_gamma1m1_avx DILITHIUM_NAMESPACE(_rej_uniform_gamma1m1_avx)
unsigned int rej_gamma1m1_avx(uint32_t *r,
                              unsigned int len,
                              const uint8_t *buf,
                              unsigned int buflen);

#endif
