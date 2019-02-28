#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

uint16_t PQCLEAN_KYBER768_CLEAN_freeze(uint16_t x);

uint16_t PQCLEAN_KYBER768_CLEAN_montgomery_reduce(uint32_t a);

uint16_t PQCLEAN_KYBER768_CLEAN_barrett_reduce(uint16_t a);

#endif
