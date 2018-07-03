#ifndef BITSTREAM_H

#include <stddef.h>
#include <stdint.h>
// #include <stdbool.h>

typedef uint64_t bitstream_value_t;

typedef struct {
  uint8_t* buffer;
  size_t position;
} bitstream_t;

bitstream_value_t bitstream_get_bits(bitstream_t* bs, unsigned int num_bits);
int bitstream_put_bits(bitstream_t* bs, bitstream_value_t value, unsigned int num_bits);

#endif
