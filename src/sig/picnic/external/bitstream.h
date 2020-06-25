/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <stddef.h>
#include <stdint.h>
#include "mzd_additional.h"
#include "oqs_picnic_macros.h"

typedef struct {
  union {
    uint8_t* w;
    const uint8_t* r;
  } buffer;
  size_t position;
} bitstream_t;

uint64_t bitstream_get_bits(bitstream_t* bs, unsigned int num_bits);
uint8_t bitstream_get_bits_8(bitstream_t* bs, unsigned int num_bits);
uint32_t bitstream_get_bits_32(bitstream_t* bs, unsigned int num_bits);
#if defined(PICNIC_STATIC)
void bitstream_put_bits(bitstream_t* bs, uint64_t value, unsigned int num_bits);
#endif
void bitstream_put_bits_8(bitstream_t* bs, uint8_t value, unsigned int num_bits);
void bitstream_put_bits_32(bitstream_t* bs, uint32_t value, unsigned int num_bits);
#if defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_192_192_4) || defined(WITH_LOWMC_255_255_4)
void mzd_to_bitstream(bitstream_t* bs, const mzd_local_t* v, const size_t width, const size_t size);
void mzd_from_bitstream(bitstream_t* bs, mzd_local_t* v, const size_t width, const size_t size);
#endif

#endif
