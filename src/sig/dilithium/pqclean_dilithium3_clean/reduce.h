#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>
#include "params.h"

#define MONT 4193792U // 2^32 % Q
#define QINV 4236238847U // -q^(-1) mod 2^32

/* a <= Q*2^32 => r < 2*Q */
#define montgomery_reduce DILITHIUM_NAMESPACE(_montgomery_reduce)
uint32_t montgomery_reduce(uint64_t a);

/* r < 2*Q */
#define reduce32 DILITHIUM_NAMESPACE(_reduc32)
uint32_t reduce32(uint32_t a);

/* a < 2*Q => r < Q */
#define csubq DILITHIUM_NAMESPACE(_csubq)
uint32_t csubq(uint32_t a);

/* r < Q */
#define freeze DILITHIUM_NAMESPACE(_freeze)
uint32_t freeze(uint32_t a);

#endif
