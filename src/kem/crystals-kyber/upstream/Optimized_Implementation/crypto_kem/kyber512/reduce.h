#ifndef REDUCE_H
#define REDUCE_H

#include <stdint.h>

uint16_t freeze(uint16_t x);

uint16_t montgomery_reduce(uint32_t a);

uint16_t barrett_reduce(uint16_t a);

#endif
