
/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html) or
 * public domain at https://github.com/pq-crystals/kyber/tree/master/ref
 */

#ifndef REDUCE_H
#define REDUCE_H

#include "params.h"
#include <stdint.h>

#define MONT (-1044) // 2^16 mod q
#define QINV (-3327) // q^-1 mod 2^16

#define montgomery_reduce KYBER_NAMESPACE(montgomery_reduce)
int16_t montgomery_reduce(int32_t a);

#define barrett_reduce KYBER_NAMESPACE(barrett_reduce)
int16_t barrett_reduce(int16_t a);

#endif
