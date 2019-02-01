/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "io.h"

#include <string.h>
#include "compat.h"

void oqs_sig_picnic_mzd_to_char_array(uint8_t* dst, const mzd_local_t* data, size_t len) {
  const size_t word_count = len / sizeof(uint64_t);
  const block_t* block    = CONST_BLOCK(data, 0);

  for (size_t i = word_count; i; --i, dst += sizeof(uint64_t)) {
    const uint64_t tmp = htobe64(block->w64[i - 1]);
    memcpy(dst, &tmp, sizeof(tmp));
  }
}

void oqs_sig_picnic_mzd_from_char_array(mzd_local_t* result, const uint8_t* data, size_t len) {
  const size_t word_count = len / sizeof(uint64_t);
  block_t* block          = BLOCK(result, 0);

  for (size_t i = word_count; i; --i, data += sizeof(uint64_t)) {
    uint64_t tmp;
    memcpy(&tmp, data, sizeof(tmp));
    block->w64[i - 1] = be64toh(tmp);
  }
}

/* unused
void print_hex(FILE* out, const uint8_t* data, size_t len) {
  for (size_t i = len; i; --i, ++data) {
    fprintf(out, "%02X", *data);
  }
}
*/
