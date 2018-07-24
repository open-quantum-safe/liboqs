#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

#define MONT 4193792U // 2^32 % Q
#define QINV 4236238847U // -q^(-1) mod 2^32

/* a <= Q*2^32 = > r < 2*Q */
uint32_t montgomery_reduce(uint64_t a);

/* r < 2*Q */
uint32_t reduce32(uint32_t a);

/* r < Q */
uint32_t freeze(uint32_t a);

#endif
