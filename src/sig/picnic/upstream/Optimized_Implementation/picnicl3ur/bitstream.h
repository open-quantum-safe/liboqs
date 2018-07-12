/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef BITSTREAM_H

#include <stddef.h>
#include <stdint.h>

typedef uint64_t bitstream_value_t;

typedef struct {
  union {
    uint8_t* buffer;
    const uint8_t* cbuffer;
  };
  size_t position;
} bitstream_t;

bitstream_value_t bitstream_get_bits(bitstream_t* bs, unsigned int num_bits);
int bitstream_put_bits(bitstream_t* bs, bitstream_value_t value, unsigned int num_bits);

#endif
