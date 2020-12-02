#ifndef PQCLEAN_KYBER76890S_CLEAN_REDUCE_H
#define PQCLEAN_KYBER76890S_CLEAN_REDUCE_H
#include "params.h"
#include <stdint.h>

#define MONT 2285 // 2^16 mod q
#define QINV 62209 // q^-1 mod 2^16

int16_t PQCLEAN_KYBER76890S_CLEAN_montgomery_reduce(int32_t a);

int16_t PQCLEAN_KYBER76890S_CLEAN_barrett_reduce(int16_t a);

#endif
