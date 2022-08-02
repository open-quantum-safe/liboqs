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
#include "macros.h"

typedef struct {
  union {
    uint8_t* w;
    const uint8_t* r;
  } buffer;
  size_t position;
} bitstream_t;

static inline uint64_t bitstream_get_bits(bitstream_t* bs, unsigned int num_bits) {
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

static inline uint8_t bitstream_get_bits_8(bitstream_t* bs, unsigned int num_bits) {
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

static inline void bitstream_put_bits(bitstream_t* bs, uint64_t value, unsigned int num_bits) {
  ASSUME(1 <= num_bits && num_bits <= 64);

  const unsigned int skip_bits = bs->position % 8;
  uint8_t* p                   = &bs->buffer.w[bs->position / 8];

  bs->position += num_bits;
  if (skip_bits) {
    // the upper skip_bits of current pos have already been taken
    const unsigned int start_bits = 8 - skip_bits;
    const unsigned int bits       = num_bits < start_bits ? num_bits : start_bits;

    *p &= (0xFF << start_bits) | (0xFF >> (skip_bits + bits)); // clear bits before setting
    *p++ |= (value >> (num_bits - bits)) << (start_bits - bits);
    num_bits -= bits;
  }

  for (; num_bits >= 8; num_bits -= 8, ++p) {
    *p = value >> (num_bits - 8);
  }

  if (num_bits > 0) {
    *p &= (0xFF >> num_bits); // clear bits before setting
    *p |= (value & ((1 << num_bits) - 1)) << (8 - num_bits);
  }
}

static inline void bitstream_put_bits_8(bitstream_t* bs, uint8_t value, unsigned int num_bits) {
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

#if defined(WITH_LOWMC_129_129_4) || defined(WITH_LOWMC_192_192_4) || defined(WITH_LOWMC_255_255_4)
static inline void mzd_to_bitstream(bitstream_t* bs, const mzd_local_t* v, const size_t width,
                                    const size_t size) {
  const uint64_t* d = &CONST_BLOCK(v, 0)->w64[width - 1];
  size_t bits       = size;
  for (; bits >= sizeof(uint64_t) * 8; bits -= sizeof(uint64_t) * 8, --d) {
    bitstream_put_bits(bs, *d, sizeof(uint64_t) * 8);
  }
  if (bits) {
    bitstream_put_bits(bs, *d >> (sizeof(uint64_t) * 8 - bits), bits);
  }
}

static inline void mzd_from_bitstream(bitstream_t* bs, mzd_local_t* v, const size_t width,
                                      const size_t size) {
  uint64_t* d = &BLOCK(v, 0)->w64[width - 1];
  uint64_t* f = BLOCK(v, 0)->w64;

  size_t bits = size;
  for (; bits >= sizeof(uint64_t) * 8; bits -= sizeof(uint64_t) * 8, --d) {
    *d = bitstream_get_bits(bs, sizeof(uint64_t) * 8);
  }
  if (bits) {
    *d = bitstream_get_bits(bs, bits) << (sizeof(uint64_t) * 8 - bits);
    --d;
  }
  for (; d >= f; --d) {
    *d = 0;
  }
}

#if defined(WITH_OPT)
#include "simd.h"

#if defined(WITH_AVX2)
ATTR_TARGET_S256
static inline void w256_to_bitstream(bitstream_t* bs, const word256 v, const size_t width,
                                     const size_t size) {
  ATTR_ALIGNED(32) uint64_t buf[4];
  mm256_store(buf, v);

  const uint64_t* d = &buf[width - 1];
  size_t bits       = size;
  for (; bits >= sizeof(uint64_t) * 8; bits -= sizeof(uint64_t) * 8, --d) {
    bitstream_put_bits(bs, *d, sizeof(uint64_t) * 8);
  }
  if (bits) {
    bitstream_put_bits(bs, *d >> (sizeof(uint64_t) * 8 - bits), bits);
  }
}

ATTR_TARGET_S256
static inline word256 w256_from_bitstream(bitstream_t* bs, const size_t width, const size_t size) {
  ATTR_ALIGNED(32) uint64_t buf[4] = {0};
  uint64_t* d                      = &buf[width - 1];
  size_t bits                      = size;
  for (; bits >= sizeof(uint64_t) * 8; bits -= sizeof(uint64_t) * 8, --d) {
    *d = bitstream_get_bits(bs, sizeof(uint64_t) * 8);
  }
  if (bits) {
    *d = bitstream_get_bits(bs, bits) << (sizeof(uint64_t) * 8 - bits);
  }

  return mm256_load(&buf[0]);
}
#endif

#if defined(WITH_SSE2) || defined(WITH_NEON)
ATTR_TARGET_S128
static inline void w128_to_bitstream(bitstream_t* bs, const word128 v[2], const size_t width,
                                     const size_t size) {
  ATTR_ALIGNED(16) uint64_t buf[4];
  mm128_store(&buf[0], v[0]);
  mm128_store(&buf[2], v[1]);

  const uint64_t* d = &buf[width - 1];
  size_t bits       = size;
  for (; bits >= sizeof(uint64_t) * 8; bits -= sizeof(uint64_t) * 8, --d) {
    bitstream_put_bits(bs, *d, sizeof(uint64_t) * 8);
  }
  if (bits) {
    bitstream_put_bits(bs, *d >> (sizeof(uint64_t) * 8 - bits), bits);
  }
}

ATTR_TARGET_S128
static inline void w128_from_bitstream(bitstream_t* bs, word128 v[2], const size_t width,
                                       const size_t size) {
  ATTR_ALIGNED(16) uint64_t buf[4] = {0};
  uint64_t* d                      = &buf[width - 1];
  size_t bits                      = size;
  for (; bits >= sizeof(uint64_t) * 8; bits -= sizeof(uint64_t) * 8, --d) {
    *d = bitstream_get_bits(bs, sizeof(uint64_t) * 8);
  }
  if (bits) {
    *d = bitstream_get_bits(bs, bits) << (sizeof(uint64_t) * 8 - bits);
  }

  v[0] = mm128_load(&buf[0]);
  v[1] = mm128_load(&buf[2]);
}
#endif
#endif
#endif
#endif
