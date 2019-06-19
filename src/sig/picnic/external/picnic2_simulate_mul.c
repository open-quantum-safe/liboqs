/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <assert.h>

#include "picnic2_simulate_mul.h"

#if defined(WITH_SSE2)
#define ATTR_TARGET_S128 ATTR_TARGET_SSE2
#else
#define ATTR_TARGET_S128
#endif

static const block_t block_masks[] = {
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
    }},
};

static block_t nl_part_block_masks[] = {
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0x0000000000000000),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
    }},
    {{
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
        UINT64_C(0xffffffffffffffff),
    }},
};

/* transpose a 64x64 bit matrix using Eklundh's algorithm */
void transpose_64_64(const uint64_t* in, uint64_t* out) {
  static const uint64_t TRANSPOSE_MASKS64[6] = {
      UINT64_C(0x00000000FFFFFFFF), UINT64_C(0x0000FFFF0000FFFF), UINT64_C(0x00FF00FF00FF00FF),
      UINT64_C(0x0F0F0F0F0F0F0F0F), UINT64_C(0x3333333333333333), UINT64_C(0x5555555555555555)};

  uint32_t width = 32, nswaps = 1;
  const uint32_t logn = 6;

  // copy in to out and transpose in-place
  memcpy(out, in, 64 * sizeof(uint64_t));

  for (uint32_t i = 0; i < logn; i++) {
    uint64_t mask     = TRANSPOSE_MASKS64[i];
    uint64_t inv_mask = ~mask;

    for (uint32_t j = 0; j < nswaps; j++) {
      for (uint32_t k = 0; k < width; k++) {
        uint32_t i1 = k + 2 * width * j;
        uint32_t i2 = k + width + 2 * width * j;

        uint64_t t1 = out[i1];
        uint64_t t2 = out[i2];

        out[i1] = (t1 & mask) ^ ((t2 & mask) << width);
        out[i2] = (t2 & inv_mask) ^ ((t1 & inv_mask) >> width);
      }
    }
    nswaps *= 2;
    width /= 2;
  }
}

uint64_t tapesToWord(randomTape_t* tapes) {
  uint64_t shares;

  if (tapes->pos % 64 == 0) {
    uint64_t buffer[64];
    for (size_t i = 0; i < 64; i++) {
      buffer[i / 8 * 8 + 7 - i % 8] = ((uint64_t*)tapes->tape[i])[tapes->pos / 64];
    }
    transpose_64_64(buffer, tapes->buffer);
  }

  shares = tapes->buffer[(tapes->pos % 64) / 8 * 8 + 7 - (tapes->pos % 64) % 8];
  tapes->pos++;
  return shares;
}

void reconstructShares(uint32_t* output, shares_t* shares) {
  for (size_t i = 0; i < shares->numWords; i++) {
    setBit((uint8_t*)output, i, parity64_uint64(shares->shares[i]));
  }
}

void xor_word_array(uint32_t* out, const uint32_t* in1, const uint32_t* in2, uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    out[i] = in1[i] ^ in2[i];
  }
}

void xor_array_RC(uint8_t* out, const uint8_t* in1, const uint8_t* in2, uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    out[i] = in1[i] ^ in2[length - 1 - i];
  }
}
/* For an input bit b = 0 or 1, return the word of all b bits, i.e.,
 * extend(1) = 0xFFFFFFFFFFFFFFFF
 * extend(0) = 0x0000000000000000
 * Assumes inputs are always 0 or 1.  If this doesn't hold, add "& 1" to the
 * input.
 */
static inline uint64_t extend(uint64_t bit) {
  return ~(bit - 1);
}

/* Get one bit from a byte array */
uint8_t getBit(const uint8_t* array, uint32_t bitNumber) {
  return (array[bitNumber / 8] >> (7 - (bitNumber % 8))) & 0x01;
}

/* Set a specific bit in a byte array to a given value */
void setBit(uint8_t* bytes, uint32_t bitNumber, uint8_t val) {
  bytes[bitNumber / 8] =
      (bytes[bitNumber >> 3] & ~(1 << (7 - (bitNumber % 8)))) | (val << (7 - (bitNumber % 8)));
}

void copyShares(shares_t* dst, shares_t* src) {
  assert(dst->numWords == src->numWords);
  memcpy(dst->shares, src->shares, dst->numWords * sizeof(dst->shares[0]));
}

void mpc_matrix_mul_uint64_128(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                               shares_t* mask_shares) {
  uint8_t temp[16] = {
      0,
  };

  const uint32_t rowstride = (128) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 128; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 128 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[128 - 1 - i];

    for (uint32_t j = 0; j < 128; j += 8) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (128 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;
      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      tmp_mask->shares[j + 0] ^= mask_share & mask1->w64[0];
      tmp_mask->shares[j + 1] ^= mask_share & mask1->w64[1];
      tmp_mask->shares[j + 2] ^= mask_share & mask1->w64[2];
      tmp_mask->shares[j + 3] ^= mask_share & mask1->w64[3];
      tmp_mask->shares[j + 4] ^= mask_share & mask2->w64[0];
      tmp_mask->shares[j + 5] ^= mask_share & mask2->w64[1];
      tmp_mask->shares[j + 6] ^= mask_share & mask2->w64[2];
      tmp_mask->shares[j + 7] ^= mask_share & mask2->w64[3];
    }
  }
  memcpy(output, temp, 128 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}

void mpc_matrix_mul_uint64_192(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                               shares_t* mask_shares) {
  uint8_t temp[24] = {
      0,
  };

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 192; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 192 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[192 - 1 - i];

    for (uint32_t j = 0; j < 192; j += 8) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (192 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      tmp_mask->shares[j + 0] ^= mask_share & mask1->w64[0];
      tmp_mask->shares[j + 1] ^= mask_share & mask1->w64[1];
      tmp_mask->shares[j + 2] ^= mask_share & mask1->w64[2];
      tmp_mask->shares[j + 3] ^= mask_share & mask1->w64[3];
      tmp_mask->shares[j + 4] ^= mask_share & mask2->w64[0];
      tmp_mask->shares[j + 5] ^= mask_share & mask2->w64[1];
      tmp_mask->shares[j + 6] ^= mask_share & mask2->w64[2];
      tmp_mask->shares[j + 7] ^= mask_share & mask2->w64[3];
    }
  }
  memcpy(output, temp, 192 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}
void mpc_matrix_mul_uint64_256(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                               shares_t* mask_shares) {
  uint8_t temp[32] = {
      0,
  };

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 256; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 256 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[256 - 1 - i];

    for (uint32_t j = 0; j < 256; j += 8) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (256 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      tmp_mask->shares[j + 0] ^= mask_share & mask1->w64[0];
      tmp_mask->shares[j + 1] ^= mask_share & mask1->w64[1];
      tmp_mask->shares[j + 2] ^= mask_share & mask1->w64[2];
      tmp_mask->shares[j + 3] ^= mask_share & mask1->w64[3];
      tmp_mask->shares[j + 4] ^= mask_share & mask2->w64[0];
      tmp_mask->shares[j + 5] ^= mask_share & mask2->w64[1];
      tmp_mask->shares[j + 6] ^= mask_share & mask2->w64[2];
      tmp_mask->shares[j + 7] ^= mask_share & mask2->w64[3];
    }
  }
  memcpy(output, temp, 256 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}

void mpc_matrix_mul_z_uint64_128(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                 const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (128) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 128);
  memset(state2, 0, 128 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod        = 0;
    uint64_t new_mask_i = 0;
    for (uint32_t j = 0; j < 128 / 8; j++) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (128 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i ^= mask_shares->shares[j * 8 + 0] & mask1->w64[0];
      new_mask_i ^= mask_shares->shares[j * 8 + 1] & mask1->w64[1];
      new_mask_i ^= mask_shares->shares[j * 8 + 2] & mask1->w64[2];
      new_mask_i ^= mask_shares->shares[j * 8 + 3] & mask1->w64[3];
      new_mask_i ^= mask_shares->shares[j * 8 + 4] & mask2->w64[0];
      new_mask_i ^= mask_shares->shares[j * 8 + 5] & mask2->w64[1];
      new_mask_i ^= mask_shares->shares[j * 8 + 6] & mask2->w64[2];
      new_mask_i ^= mask_shares->shares[j * 8 + 7] & mask2->w64[3];
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] = new_mask_i;
  }
}

void mpc_matrix_mul_z_uint64_192(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                 const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (256) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 192);
  memset(state2, 0, 192 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod        = 0;
    uint64_t new_mask_i = 0;
    for (uint32_t j = 0; j < 192 / 8; j++) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (192 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i ^= mask_shares->shares[j * 8 + 0] & mask1->w64[0];
      new_mask_i ^= mask_shares->shares[j * 8 + 1] & mask1->w64[1];
      new_mask_i ^= mask_shares->shares[j * 8 + 2] & mask1->w64[2];
      new_mask_i ^= mask_shares->shares[j * 8 + 3] & mask1->w64[3];
      new_mask_i ^= mask_shares->shares[j * 8 + 4] & mask2->w64[0];
      new_mask_i ^= mask_shares->shares[j * 8 + 5] & mask2->w64[1];
      new_mask_i ^= mask_shares->shares[j * 8 + 6] & mask2->w64[2];
      new_mask_i ^= mask_shares->shares[j * 8 + 7] & mask2->w64[3];
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] = new_mask_i;
  }
}

void mpc_matrix_mul_z_uint64_256(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                 const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (256) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 256);
  memset(state2, 0, 256 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod        = 0;
    uint64_t new_mask_i = 0;
    for (uint32_t j = 0; j < 256 / 8; j++) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (256 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i ^= mask_shares->shares[j * 8 + 0] & mask1->w64[0];
      new_mask_i ^= mask_shares->shares[j * 8 + 1] & mask1->w64[1];
      new_mask_i ^= mask_shares->shares[j * 8 + 2] & mask1->w64[2];
      new_mask_i ^= mask_shares->shares[j * 8 + 3] & mask1->w64[3];
      new_mask_i ^= mask_shares->shares[j * 8 + 4] & mask2->w64[0];
      new_mask_i ^= mask_shares->shares[j * 8 + 5] & mask2->w64[1];
      new_mask_i ^= mask_shares->shares[j * 8 + 6] & mask2->w64[2];
      new_mask_i ^= mask_shares->shares[j * 8 + 7] & mask2->w64[3];
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] = new_mask_i;
  }
}

void mpc_matrix_addmul_r_uint64_128(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                    shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[16] = {
      0,
  };
  memcpy(temp, state2, 128 / 8);

  const uint32_t rowstride = (128) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];

    for (uint32_t j = 0; j < 128; j += 8) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (128 - 1 - j) / 8];

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      tmp_mask->shares[j + 0] ^= mask_share & mask1->w64[0];
      tmp_mask->shares[j + 1] ^= mask_share & mask1->w64[1];
      tmp_mask->shares[j + 2] ^= mask_share & mask1->w64[2];
      tmp_mask->shares[j + 3] ^= mask_share & mask1->w64[3];
      tmp_mask->shares[j + 4] ^= mask_share & mask2->w64[0];
      tmp_mask->shares[j + 5] ^= mask_share & mask2->w64[1];
      tmp_mask->shares[j + 6] ^= mask_share & mask2->w64[2];
      tmp_mask->shares[j + 7] ^= mask_share & mask2->w64[3];

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 128 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

void mpc_matrix_addmul_r_uint64_192(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                    shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[24] = {
      0,
  };
  memcpy(temp, state2, 192 / 8);

  const uint32_t rowstride = 256 / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];

    for (uint32_t j = 0; j < 192; j += 8) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (192 - 1 - j) / 8];

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      tmp_mask->shares[j + 0] ^= mask_share & mask1->w64[0];
      tmp_mask->shares[j + 1] ^= mask_share & mask1->w64[1];
      tmp_mask->shares[j + 2] ^= mask_share & mask1->w64[2];
      tmp_mask->shares[j + 3] ^= mask_share & mask1->w64[3];
      tmp_mask->shares[j + 4] ^= mask_share & mask2->w64[0];
      tmp_mask->shares[j + 5] ^= mask_share & mask2->w64[1];
      tmp_mask->shares[j + 6] ^= mask_share & mask2->w64[2];
      tmp_mask->shares[j + 7] ^= mask_share & mask2->w64[3];

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 192 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

void mpc_matrix_addmul_r_uint64_256(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                    shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[32] = {
      0,
  };
  memcpy(temp, state2, 256 / 8);

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];

    for (uint32_t j = 0; j < 256; j += 8) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (256 - 1 - j) / 8];

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      tmp_mask->shares[j + 0] ^= mask_share & mask1->w64[0];
      tmp_mask->shares[j + 1] ^= mask_share & mask1->w64[1];
      tmp_mask->shares[j + 2] ^= mask_share & mask1->w64[2];
      tmp_mask->shares[j + 3] ^= mask_share & mask1->w64[3];
      tmp_mask->shares[j + 4] ^= mask_share & mask2->w64[0];
      tmp_mask->shares[j + 5] ^= mask_share & mask2->w64[1];
      tmp_mask->shares[j + 6] ^= mask_share & mask2->w64[2];
      tmp_mask->shares[j + 7] ^= mask_share & mask2->w64[3];

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 256 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

void mpc_matrix_mul_nl_part_uint64_128(uint32_t* nl_part, const uint32_t* key,
                                       const uint64_t* precomputed_nl_matrix,
                                       const uint64_t* precomputed_constant_nl,
                                       shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((20 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 20 * sizeof(uint32_t));
  for (size_t i = 0; i < 128; i++) {
    uint8_t key_bit         = extend(getBit((const uint8_t*)key, 128 - 1 - i)) & 0xFF;
    const uint64_t key_mask = key_masks->shares[128 - 1 - i];

    for (uint32_t j = 0; j < 20 * 32; j += 8) {
      uint8_t matrix_byte = ((const uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      const block_t* mask1 = &nl_part_block_masks[(matrix_byte >> 0) & 0xF];
      const block_t* mask2 = &nl_part_block_masks[(matrix_byte >> 4) & 0xF];

      nl_part_masks->shares[j + 0] ^= key_mask & mask1->w64[0];
      nl_part_masks->shares[j + 1] ^= key_mask & mask1->w64[1];
      nl_part_masks->shares[j + 2] ^= key_mask & mask1->w64[2];
      nl_part_masks->shares[j + 3] ^= key_mask & mask1->w64[3];
      nl_part_masks->shares[j + 4] ^= key_mask & mask2->w64[0];
      nl_part_masks->shares[j + 5] ^= key_mask & mask2->w64[1];
      nl_part_masks->shares[j + 6] ^= key_mask & mask2->w64[2];
      nl_part_masks->shares[j + 7] ^= key_mask & mask2->w64[3];
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 20);
}

void mpc_matrix_mul_nl_part_uint64_192(uint32_t* nl_part, const uint32_t* key,
                                       const uint64_t* precomputed_nl_matrix,
                                       const uint64_t* precomputed_constant_nl,
                                       shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((30 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 30 * sizeof(uint32_t));
  for (size_t i = 0; i < 192; i++) {
    uint8_t key_bit         = extend(getBit((const uint8_t*)key, 192 - 1 - i)) & 0xFF;
    const uint64_t key_mask = key_masks->shares[192 - 1 - i];

    for (uint32_t j = 0; j < 30 * 32; j += 8) {
      uint8_t matrix_byte = ((const uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      const block_t* mask1 = &nl_part_block_masks[(matrix_byte >> 0) & 0xF];
      const block_t* mask2 = &nl_part_block_masks[(matrix_byte >> 4) & 0xF];

      nl_part_masks->shares[j + 0] ^= key_mask & mask1->w64[0];
      nl_part_masks->shares[j + 1] ^= key_mask & mask1->w64[1];
      nl_part_masks->shares[j + 2] ^= key_mask & mask1->w64[2];
      nl_part_masks->shares[j + 3] ^= key_mask & mask1->w64[3];
      nl_part_masks->shares[j + 4] ^= key_mask & mask2->w64[0];
      nl_part_masks->shares[j + 5] ^= key_mask & mask2->w64[1];
      nl_part_masks->shares[j + 6] ^= key_mask & mask2->w64[2];
      nl_part_masks->shares[j + 7] ^= key_mask & mask2->w64[3];
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 30);
}

void mpc_matrix_mul_nl_part_uint64_256(uint32_t* nl_part, const uint32_t* key,
                                       const uint64_t* precomputed_nl_matrix,
                                       const uint64_t* precomputed_constant_nl,
                                       shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((38 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 38 * sizeof(uint32_t));
  for (size_t i = 0; i < 256; i++) {
    uint8_t key_bit         = extend(getBit((const uint8_t*)key, 256 - 1 - i)) & 0xFF;
    const uint64_t key_mask = key_masks->shares[256 - 1 - i];

    for (uint32_t j = 0; j < 38 * 32; j += 8) {
      uint8_t matrix_byte = ((uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      const block_t* mask1 = &nl_part_block_masks[(matrix_byte >> 0) & 0xF];
      const block_t* mask2 = &nl_part_block_masks[(matrix_byte >> 4) & 0xF];

      nl_part_masks->shares[j + 0] ^= key_mask & mask1->w64[0];
      nl_part_masks->shares[j + 1] ^= key_mask & mask1->w64[1];
      nl_part_masks->shares[j + 2] ^= key_mask & mask1->w64[2];
      nl_part_masks->shares[j + 3] ^= key_mask & mask1->w64[3];
      nl_part_masks->shares[j + 4] ^= key_mask & mask2->w64[0];
      nl_part_masks->shares[j + 5] ^= key_mask & mask2->w64[1];
      nl_part_masks->shares[j + 6] ^= key_mask & mask2->w64[2];
      nl_part_masks->shares[j + 7] ^= key_mask & mask2->w64[3];
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 38);
}

#if defined(WITH_OPT)
#if defined(WITH_SSE2) || defined(WITH_NEON)
ATTR_TARGET_S128
void mpc_matrix_mul_s128_128(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                             shares_t* mask_shares) {
  uint8_t temp[16] = {
      0,
  };

  const uint32_t rowstride = (128) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 128; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 128 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[128 - 1 - i];
    const block_t mask_share2 = {{mask_share, mask_share, mask_share, mask_share}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)tmp_mask->shares;

    for (uint32_t j = 0; j < 128; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (128 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask2 = block_masks[(matrix_byte >> 4) & 0xf].w128[1];
      mask3 = block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask4 = block_masks[(matrix_byte >> 0) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);
    }
  }
  memcpy(output, temp, 128 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_S128
void mpc_matrix_mul_s128_192(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                             shares_t* mask_shares) {
  uint8_t temp[24] = {
      0,
  };

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 192; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 192 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[192 - 1 - i];
    const block_t mask_share2 = {{mask_share, mask_share, mask_share, mask_share}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)tmp_mask->shares;

    for (uint32_t j = 0; j < 192; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (192 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask2 = block_masks[(matrix_byte >> 4) & 0xf].w128[1];
      mask3 = block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask4 = block_masks[(matrix_byte >> 0) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);
    }
  }
  memcpy(output, temp, 192 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_S128
void mpc_matrix_mul_s128_256(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                             shares_t* mask_shares) {
  uint8_t temp[32] = {
      0,
  };

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 256; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 256 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[256 - 1 - i];
    const block_t mask_share2 = {{mask_share, mask_share, mask_share, mask_share}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)tmp_mask->shares;

    for (uint32_t j = 0; j < 256; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (256 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask2 = block_masks[(matrix_byte >> 4) & 0xf].w128[1];
      mask3 = block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask4 = block_masks[(matrix_byte >> 0) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);
    }
  }
  memcpy(output, temp, 256 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_S128
void mpc_matrix_mul_z_s128_128(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                               const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (128) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 128);
  memset(state2, 0, 128 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod            = 0;
    block_t new_mask_i      = {{0, 0, 0, 0}};
    word128* tmp_mask_block = (word128*)mask_shares->shares;
    for (uint32_t j = 0; j < 128 / 8; j++, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (128 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i.w128[0] = mm128_xor_mask(new_mask_i.w128[0], tmp_mask_block[0], mask1->w128[0]);
      new_mask_i.w128[1] = mm128_xor_mask(new_mask_i.w128[1], tmp_mask_block[1], mask1->w128[1]);
      new_mask_i.w128[0] = mm128_xor_mask(new_mask_i.w128[0], tmp_mask_block[2], mask2->w128[0]);
      new_mask_i.w128[1] = mm128_xor_mask(new_mask_i.w128[1], tmp_mask_block[3], mask2->w128[1]);
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] =
        new_mask_i.w64[0] ^ new_mask_i.w64[1] ^ new_mask_i.w64[2] ^ new_mask_i.w64[3];
  }
}

ATTR_TARGET_S128
void mpc_matrix_mul_z_s128_192(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                               const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (256) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 192);
  memset(state2, 0, 192 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod            = 0;
    block_t new_mask_i      = {{0, 0, 0, 0}};
    word128* tmp_mask_block = (word128*)mask_shares->shares;
    for (uint32_t j = 0; j < 192 / 8; j++, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (192 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i.w128[0] = mm128_xor_mask(new_mask_i.w128[0], tmp_mask_block[0], mask1->w128[0]);
      new_mask_i.w128[1] = mm128_xor_mask(new_mask_i.w128[1], tmp_mask_block[1], mask1->w128[1]);
      new_mask_i.w128[0] = mm128_xor_mask(new_mask_i.w128[0], tmp_mask_block[2], mask2->w128[0]);
      new_mask_i.w128[1] = mm128_xor_mask(new_mask_i.w128[1], tmp_mask_block[3], mask2->w128[1]);
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] =
        new_mask_i.w64[0] ^ new_mask_i.w64[1] ^ new_mask_i.w64[2] ^ new_mask_i.w64[3];
  }
}

ATTR_TARGET_S128
void mpc_matrix_mul_z_s128_256(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                               const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (256) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 256);
  memset(state2, 0, 256 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod            = 0;
    block_t new_mask_i      = {{0, 0, 0, 0}};
    word128* tmp_mask_block = (word128*)mask_shares->shares;
    for (uint32_t j = 0; j < 256 / 8; j++, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (256 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i.w128[0] = mm128_xor_mask(new_mask_i.w128[0], tmp_mask_block[0], mask1->w128[0]);
      new_mask_i.w128[1] = mm128_xor_mask(new_mask_i.w128[1], tmp_mask_block[1], mask1->w128[1]);
      new_mask_i.w128[0] = mm128_xor_mask(new_mask_i.w128[0], tmp_mask_block[2], mask2->w128[0]);
      new_mask_i.w128[1] = mm128_xor_mask(new_mask_i.w128[1], tmp_mask_block[3], mask2->w128[1]);
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] =
        new_mask_i.w64[0] ^ new_mask_i.w64[1] ^ new_mask_i.w64[2] ^ new_mask_i.w64[3];
  }
}

ATTR_TARGET_S128
void mpc_matrix_addmul_r_s128_128(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                  shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[16] = {
      0,
  };
  memcpy(temp, state2, 128 / 8);

  const uint32_t rowstride = (128) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];
    const block_t mask_share2 = {{mask_share, mask_share, mask_share, mask_share}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)tmp_mask->shares;
    for (uint32_t j = 0; j < 128; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (128 - 1 - j) / 8];

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask2 = block_masks[(matrix_byte >> 4) & 0xf].w128[1];
      mask3 = block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask4 = block_masks[(matrix_byte >> 0) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 128 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_S128
void mpc_matrix_addmul_r_s128_192(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                  shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[24] = {
      0,
  };
  memcpy(temp, state2, 192 / 8);

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];
    const block_t mask_share2 = {{mask_share, mask_share, mask_share, mask_share}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)tmp_mask->shares;
    for (uint32_t j = 0; j < 192; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (192 - 1 - j) / 8];

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask2 = block_masks[(matrix_byte >> 4) & 0xf].w128[1];
      mask3 = block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask4 = block_masks[(matrix_byte >> 0) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 192 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_S128
void mpc_matrix_addmul_r_s128_256(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                  shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[32] = {
      0,
  };
  memcpy(temp, state2, 256 / 8);

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];
    const block_t mask_share2 = {{mask_share, mask_share, mask_share, mask_share}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)tmp_mask->shares;
    for (uint32_t j = 0; j < 256; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (256 - 1 - j) / 8];

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask2 = block_masks[(matrix_byte >> 4) & 0xf].w128[1];
      mask3 = block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask4 = block_masks[(matrix_byte >> 0) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 256 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_S128
void mpc_matrix_mul_nl_part_s128_128(uint32_t* nl_part, const uint32_t* key,
                                     const uint64_t* precomputed_nl_matrix,
                                     const uint64_t* precomputed_constant_nl,
                                     shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((20 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 20 * sizeof(uint32_t));
  for (size_t i = 0; i < 128; i++) {
    uint8_t key_bit           = extend(getBit((const uint8_t*)key, 128 - 1 - i)) & 0xFF;
    const uint64_t key_mask   = key_masks->shares[128 - 1 - i];
    const block_t mask_share2 = {{key_mask, key_mask, key_mask, key_mask}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)nl_part_masks->shares;

    for (uint32_t j = 0; j < 20 * 32; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      mask1 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask2 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w128[1];
      mask3 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask4 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 20);
}

ATTR_TARGET_S128
void mpc_matrix_mul_nl_part_s128_192(uint32_t* nl_part, const uint32_t* key,
                                     const uint64_t* precomputed_nl_matrix,
                                     const uint64_t* precomputed_constant_nl,
                                     shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((30 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 30 * sizeof(uint32_t));
  for (size_t i = 0; i < 192; i++) {
    uint8_t key_bit           = extend(getBit((const uint8_t*)key, 192 - 1 - i)) & 0xFF;
    const uint64_t key_mask   = key_masks->shares[192 - 1 - i];
    const block_t mask_share2 = {{key_mask, key_mask, key_mask, key_mask}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)nl_part_masks->shares;

    for (uint32_t j = 0; j < 30 * 32; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      mask1 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask2 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w128[1];
      mask3 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask4 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 30);
}

ATTR_TARGET_S128
void mpc_matrix_mul_nl_part_s128_256(uint32_t* nl_part, const uint32_t* key,
                                     const uint64_t* precomputed_nl_matrix,
                                     const uint64_t* precomputed_constant_nl,
                                     shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((38 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 38 * sizeof(uint32_t));
  for (size_t i = 0; i < 256; i++) {
    uint8_t key_bit           = extend(getBit((const uint8_t*)key, 256 - 1 - i)) & 0xFF;
    const uint64_t key_mask   = key_masks->shares[256 - 1 - i];
    const block_t mask_share2 = {{key_mask, key_mask, key_mask, key_mask}};
    word128 mask1, mask2, mask3, mask4;

    word128* tmp_mask_block = (word128*)nl_part_masks->shares;

    for (uint32_t j = 0; j < 38 * 32; j += 8, tmp_mask_block += 4) {
      uint8_t matrix_byte = ((uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      mask1 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w128[0];
      mask2 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w128[1];
      mask3 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w128[0];
      mask4 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w128[1];

      tmp_mask_block[0] = mm128_xor_mask(tmp_mask_block[0], mask_share2.w128[0], mask1);
      tmp_mask_block[1] = mm128_xor_mask(tmp_mask_block[1], mask_share2.w128[0], mask2);
      tmp_mask_block[2] = mm128_xor_mask(tmp_mask_block[2], mask_share2.w128[0], mask3);
      tmp_mask_block[3] = mm128_xor_mask(tmp_mask_block[3], mask_share2.w128[0], mask4);
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 38);
}
#endif

#if defined(WITH_AVX2)
ATTR_TARGET_AVX2
void mpc_matrix_mul_s256_128(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                             shares_t* mask_shares) {
  uint8_t temp[16] = {
      0,
  };

  const uint32_t rowstride = (128) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 128; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 128 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[128 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(mask_share);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)tmp_mask->shares;

    for (uint32_t j = 0; j < 128; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (128 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;
      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w256;
      mask2 = block_masks[(matrix_byte >> 0) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);
    }
  }
  memcpy(output, temp, 128 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_AVX2
void mpc_matrix_mul_s256_192(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                             shares_t* mask_shares) {
  uint8_t temp[24] = {
      0,
  };

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 192; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 192 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[192 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(mask_share);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)tmp_mask->shares;

    for (uint32_t j = 0; j < 192; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (192 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;
      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w256;
      mask2 = block_masks[(matrix_byte >> 0) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);
    }
  }
  memcpy(output, temp, 192 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_AVX2
void mpc_matrix_mul_s256_256(uint32_t* output, const uint32_t* vec, const uint64_t* matrix,
                             shares_t* mask_shares) {
  uint8_t temp[32] = {
      0,
  };

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);

  for (size_t i = 0; i < 256; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)vec, 256 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[256 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(mask_share);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)tmp_mask->shares;

    for (uint32_t j = 0; j < 256; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (256 - 1 - j) / 8];
      temp[j / 8] ^= matrix_byte & vec_bit;
      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w256;
      mask2 = block_masks[(matrix_byte >> 0) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);
    }
  }
  memcpy(output, temp, 256 / 8);

  copyShares(mask_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_AVX2
void mpc_matrix_mul_z_s256_128(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                               const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (128) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 128);
  memset(state2, 0, 128 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod            = 0;
    block_t new_mask_i      = {{0, 0, 0, 0}};
    word256* tmp_mask_block = (word256*)mask_shares->shares;
    for (uint32_t j = 0; j < 128 / 8; j++, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (128 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i.w256 = mm256_xor_mask(new_mask_i.w256, tmp_mask_block[0], mask1->w256);
      new_mask_i.w256 = mm256_xor_mask(new_mask_i.w256, tmp_mask_block[1], mask2->w256);
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] =
        new_mask_i.w64[0] ^ new_mask_i.w64[1] ^ new_mask_i.w64[2] ^ new_mask_i.w64[3];
  }
}

ATTR_TARGET_AVX2
void mpc_matrix_mul_z_s256_192(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                               const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (256) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 192);
  memset(state2, 0, 192 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod            = 0;
    block_t new_mask_i      = {{0, 0, 0, 0}};
    word256* tmp_mask_block = (word256*)mask_shares->shares;
    for (uint32_t j = 0; j < 192 / 8; j++, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (192 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i.w256 = mm256_xor_mask(new_mask_i.w256, tmp_mask_block[0], mask1->w256);
      new_mask_i.w256 = mm256_xor_mask(new_mask_i.w256, tmp_mask_block[1], mask2->w256);
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] =
        new_mask_i.w64[0] ^ new_mask_i.w64[1] ^ new_mask_i.w64[2] ^ new_mask_i.w64[3];
  }
}

ATTR_TARGET_AVX2
void mpc_matrix_mul_z_s256_256(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                               const shares_t* mask_shares, const uint64_t* matrix) {
  const uint32_t rowstride = (256) / 8;
  memset(mask2_shares->shares, 0, sizeof(uint64_t) * 256);
  memset(state2, 0, 256 / 8);
  for (size_t i = 0; i < 30; i++) {
    uint8_t prod            = 0;
    block_t new_mask_i      = {{0, 0, 0, 0}};
    word256* tmp_mask_block = (word256*)mask_shares->shares;
    for (uint32_t j = 0; j < 256 / 8; j++, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[i * rowstride + (256 / 8) - 1 - j];
      uint8_t vec_byte    = ((const uint8_t*)state)[j];

      prod ^= matrix_byte & vec_byte;

      const block_t* mask1 = &block_masks[(matrix_byte >> 4) & 0xF];
      const block_t* mask2 = &block_masks[(matrix_byte >> 0) & 0xF];

      new_mask_i.w256 = mm256_xor_mask(new_mask_i.w256, tmp_mask_block[0], mask1->w256);
      new_mask_i.w256 = mm256_xor_mask(new_mask_i.w256, tmp_mask_block[1], mask2->w256);
    }
    const uint8_t parity = parity64_uint8(prod);
    setBit((uint8_t*)state2, 30 - 1 - i, parity);
    mask2_shares->shares[30 - 1 - i] =
        new_mask_i.w64[0] ^ new_mask_i.w64[1] ^ new_mask_i.w64[2] ^ new_mask_i.w64[3];
  }
}

ATTR_TARGET_AVX2
void mpc_matrix_addmul_r_s256_128(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                  shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[16] = {
      0,
  };
  memcpy(temp, state2, 128 / 8);

  const uint32_t rowstride = (128) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(mask_share);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)tmp_mask->shares;
    for (uint32_t j = 0; j < 128; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (128 - 1 - j) / 8];

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w256;
      mask2 = block_masks[(matrix_byte >> 0) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 128 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_AVX2
void mpc_matrix_addmul_r_s256_192(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                  shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[24] = {
      0,
  };
  memcpy(temp, state2, 192 / 8);

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(mask_share);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)tmp_mask->shares;
    for (uint32_t j = 0; j < 192; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (192 - 1 - j) / 8];

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w256;
      mask2 = block_masks[(matrix_byte >> 0) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 192 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_AVX2
void mpc_matrix_addmul_r_s256_256(uint32_t* state2, const uint32_t* state, shares_t* mask2_shares,
                                  shares_t* mask_shares, const uint64_t* matrix) {
  uint8_t temp[32] = {
      0,
  };
  memcpy(temp, state2, 256 / 8);

  const uint32_t rowstride = (256) / 8;
  shares_t* tmp_mask       = allocateShares(mask_shares->numWords);
  copyShares(tmp_mask, mask2_shares);

  for (size_t i = 0; i < 30; i++) {
    uint8_t vec_bit           = extend(getBit((const uint8_t*)state, 30 - 1 - i)) & 0xFF;
    const uint64_t mask_share = mask_shares->shares[30 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(mask_share);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)tmp_mask->shares;
    for (uint32_t j = 0; j < 256; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((const uint8_t*)matrix)[(i * rowstride) + (256 - 1 - j) / 8];

      mask1 = block_masks[(matrix_byte >> 4) & 0xf].w256;
      mask2 = block_masks[(matrix_byte >> 0) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);

      temp[j / 8] ^= matrix_byte & vec_bit;
    }
  }
  memcpy(state2, temp, 256 / 8);

  copyShares(mask2_shares, tmp_mask);
  freeShares(tmp_mask);
}

ATTR_TARGET_AVX2
void mpc_matrix_mul_nl_part_s256_128(uint32_t* nl_part, const uint32_t* key,
                                     const uint64_t* precomputed_nl_matrix,
                                     const uint64_t* precomputed_constant_nl,
                                     shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((20 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 20 * sizeof(uint32_t));
  for (size_t i = 0; i < 128; i++) {
    uint8_t key_bit           = extend(getBit((const uint8_t*)key, 128 - 1 - i)) & 0xFF;
    const uint64_t key_mask   = key_masks->shares[128 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(key_mask);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)nl_part_masks->shares;

    for (uint32_t j = 0; j < 20 * 32; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      mask1 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w256;
      mask2 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 20);
}

ATTR_TARGET_AVX2
void mpc_matrix_mul_nl_part_s256_192(uint32_t* nl_part, const uint32_t* key,
                                     const uint64_t* precomputed_nl_matrix,
                                     const uint64_t* precomputed_constant_nl,
                                     shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((30 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 30 * sizeof(uint32_t));
  for (size_t i = 0; i < 192; i++) {
    uint8_t key_bit           = extend(getBit((const uint8_t*)key, 192 - 1 - i)) & 0xFF;
    const uint64_t key_mask   = key_masks->shares[192 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(key_mask);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)nl_part_masks->shares;

    for (uint32_t j = 0; j < 30 * 32; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      mask1 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w256;
      mask2 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 30);
}

ATTR_TARGET_AVX2
void mpc_matrix_mul_nl_part_s256_256(uint32_t* nl_part, const uint32_t* key,
                                     const uint64_t* precomputed_nl_matrix,
                                     const uint64_t* precomputed_constant_nl,
                                     shares_t* nl_part_masks, const shares_t* key_masks) {

  const uint32_t rowstride = ((38 * 32 + 255) / 256 * 256) / 8;
  memset(nl_part, 0, 38 * sizeof(uint32_t));
  for (size_t i = 0; i < 256; i++) {
    uint8_t key_bit           = extend(getBit((const uint8_t*)key, 256 - 1 - i)) & 0xFF;
    const uint64_t key_mask   = key_masks->shares[256 - 1 - i];
    const word256 mask_share2 = _mm256_set1_epi64x(key_mask);
    word256 mask1, mask2;

    word256* tmp_mask_block = (word256*)nl_part_masks->shares;

    for (uint32_t j = 0; j < 38 * 32; j += 8, tmp_mask_block += 2) {
      uint8_t matrix_byte = ((uint8_t*)precomputed_nl_matrix)[i * rowstride + j / 8];
      ((uint8_t*)nl_part)[j / 8] ^= matrix_byte & key_bit;

      mask1 = nl_part_block_masks[(matrix_byte >> 0) & 0xf].w256;
      mask2 = nl_part_block_masks[(matrix_byte >> 4) & 0xf].w256;

      tmp_mask_block[0] = mm256_xor_mask(tmp_mask_block[0], mask_share2, mask1);
      tmp_mask_block[1] = mm256_xor_mask(tmp_mask_block[1], mask_share2, mask2);
    }
  }
  xor_word_array(nl_part, nl_part, (const uint32_t*)precomputed_constant_nl, 38);
}
#endif
#endif
