/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "bitstream.h"
#include "macros.h"

uint64_t oqs_sig_picnic_bitstream_get_bits(bitstream_t* bs, unsigned int num_bits) {
  ASSUME(1 <= num_bits && num_bits <= 64);

  const uint8_t* p              = &bs->buffer.r[bs->position / 8];
  const unsigned int skip_bits  = bs->position % 8;
  const unsigned int start_bits = 8 - skip_bits;

  bs->position += num_bits;
  uint64_t ret = (*p++ & ((1 << start_bits) - 1));

  if (num_bits <= start_bits) {
    return ret >> (start_bits - num_bits);
  }

  num_bits -= start_bits;
  for (; num_bits >= 8; num_bits -= 8, ++p) {
    ret = ret << 8 | *p;
  }

  if (num_bits > 0) {
    ret = ret << num_bits | ((*p >> (8 - num_bits)) & ((1 << num_bits) - 1));
  }

  return ret;
}

uint8_t oqs_sig_picnic_bitstream_get_bits_8(bitstream_t* bs, unsigned int num_bits) {
  ASSUME(1 <= num_bits && num_bits <= 8);

  const uint8_t* p              = &bs->buffer.r[bs->position / 8];
  const unsigned int skip_bits  = bs->position % 8;
  const unsigned int start_bits = 8 - skip_bits;

  bs->position += num_bits;
  uint8_t ret = (*p++ & ((1 << start_bits) - 1));

  if (num_bits <= start_bits) {
    return ret >> (start_bits - num_bits);
  }

  num_bits -= start_bits;
  if (num_bits > 0) {
    ret = ret << num_bits | ((*p >> (8 - num_bits)) & ((1 << num_bits) - 1));
  }

  return ret;
}

uint32_t oqs_sig_picnic_bitstream_get_bits_32(bitstream_t* bs, unsigned int num_bits) {
  ASSUME(1 <= num_bits && num_bits <= 32);

  const uint8_t* p              = &bs->buffer.r[bs->position / 8];
  const unsigned int skip_bits  = bs->position % 8;
  const unsigned int start_bits = 8 - skip_bits;

  bs->position += num_bits;
  uint32_t ret = (*p++ & ((1 << start_bits) - 1));

  if (num_bits <= start_bits) {
    return ret >> (start_bits - num_bits);
  }

  num_bits -= start_bits;
  for (; num_bits >= 8; num_bits -= 8, ++p) {
    ret = ret << 8 | *p;
  }

  if (num_bits > 0) {
    ret = ret << num_bits | ((*p >> (8 - num_bits)) & ((1 << num_bits) - 1));
  }

  return ret;
}

void oqs_sig_picnic_bitstream_put_bits(bitstream_t* bs, uint64_t value, unsigned int num_bits) {
  ASSUME(1 <= num_bits && num_bits <= 64);

  const unsigned int skip_bits = bs->position % 8;
  uint8_t* p                   = &bs->buffer.w[bs->position / 8];

  bs->position += num_bits;
  if (skip_bits) {
    // the upper skip_bits of current pos have already been taken
    const unsigned int start_bits = 8 - skip_bits;
    const unsigned int bits       = num_bits < start_bits ? num_bits : start_bits;

    *p++ |= (value >> (num_bits - bits)) << (8 - skip_bits - bits);
    num_bits -= bits;
  }

  for (; num_bits >= 8; num_bits -= 8, ++p) {
    *p = value >> (num_bits - 8);
  }

  if (num_bits > 0) {
    *p = (value & ((1 << num_bits) - 1)) << (8 - num_bits);
  }
}

void oqs_sig_picnic_bitstream_put_bits_8(bitstream_t* bs, uint8_t value, unsigned int num_bits) {
  ASSUME(1 <= num_bits && num_bits <= 8);

  const unsigned int skip_bits = bs->position % 8;
  uint8_t* p                   = &bs->buffer.w[bs->position / 8];

  bs->position += num_bits;
  if (skip_bits) {
    // the upper skip_bits of current pos have already been taken
    const unsigned int start_bits = 8 - skip_bits;
    const unsigned int bits       = num_bits < start_bits ? num_bits : start_bits;

    *p++ |= (value >> (num_bits - bits)) << (8 - skip_bits - bits);
    num_bits -= bits;
  }

  if (num_bits > 0) {
    *p = (value & ((1 << num_bits) - 1)) << (8 - num_bits);
  }
}

void oqs_sig_picnic_bitstream_put_bits_32(bitstream_t* bs, uint32_t value, unsigned int num_bits) {
  ASSUME(1 <= num_bits && num_bits <= 32);

  const unsigned int skip_bits = bs->position % 8;
  uint8_t* p                   = &bs->buffer.w[bs->position / 8];

  bs->position += num_bits;
  if (skip_bits) {
    // the upper skip_bits of current pos have already been taken
    const unsigned int start_bits = 8 - skip_bits;
    const unsigned int bits       = num_bits < start_bits ? num_bits : start_bits;

    *p++ |= (value >> (num_bits - bits)) << (8 - skip_bits - bits);
    num_bits -= bits;
  }

  for (; num_bits >= 8; num_bits -= 8, ++p) {
    *p = value >> (num_bits - 8);
  }

  if (num_bits > 0) {
    *p = (value & ((1 << num_bits) - 1)) << (8 - num_bits);
  }
}
