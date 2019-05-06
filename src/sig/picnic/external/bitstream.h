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

typedef struct {
  union {
    uint8_t* w;
    const uint8_t* r;
  } buffer;
  size_t position;
} bitstream_t;

uint64_t oqs_sig_picnic_bitstream_get_bits(bitstream_t* bs, unsigned int num_bits);
uint8_t oqs_sig_picnic_bitstream_get_bits_8(bitstream_t* bs, unsigned int num_bits);
uint32_t oqs_sig_picnic_bitstream_get_bits_32(bitstream_t* bs, unsigned int num_bits);
void oqs_sig_picnic_bitstream_put_bits(bitstream_t* bs, uint64_t value, unsigned int num_bits);
void oqs_sig_picnic_bitstream_put_bits_8(bitstream_t* bs, uint8_t value, unsigned int num_bits);
void oqs_sig_picnic_bitstream_put_bits_32(bitstream_t* bs, uint32_t value, unsigned int num_bits);

#endif
