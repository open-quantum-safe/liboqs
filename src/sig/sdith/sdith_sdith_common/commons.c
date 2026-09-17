#include <inttypes.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdith_prng.h"
#include "vole_private.h"

/** binary valuation of i (also known as hamming weight of i ^ (i - 1) - 1) */
uint16_t binval_of(uint16_t i) {
  CASSERT(i != 0, "binval_of(0) is undefined");
  return __builtin_ctz(i);
}

// compat function, to use in case of the builtin above does not exist
uint16_t binval_of_compat(uint16_t i) {
  uint16_t u = i ^ (i - 1) ^ 1;
  // return _mm_popcnt_u32(u);
  u = (u & 0x5555) + ((u >> 1) & 0x5555);
  u = (u & 0x3333) + ((u >> 2) & 0x3333);
  u = (u & 0x0f0f) + ((u >> 4) & 0x0f0f);
  u = (u & 0x00ff) + ((u >> 8) & 0x00ff);
  return u;
}

/** grey encoding of x */
uint16_t grey_of(uint16_t x) { return x ^ (x >> 1); }

/** inverse o grey encoding of x */
uint16_t inv_grey_of(uint16_t x) {
  uint16_t u = x;
  for (uint16_t i = 1; i < 16; ++i) {
    u ^= (x >> i);
  }
  return u;
}

/** value of the pos-th bit of x (little endian index) */
uint8_t bit_of(uint16_t x, uint16_t pos) { return (x >> pos) & 1; }


/** raise to the power p */
EXPORT void flambda_power(const vole_parameters* vole_params, flambda_t* res, const flambda_t* x, uint64_t p) {
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  flambda_max_t tx;
  memcpy(tx, x, lambda_bytes);
  memset(res, 0, lambda_bytes);
  *((uint8_t*)res) = 1;
  // invariant: res * tx^p = cst
  while (p > 0) {
    if (p & 1) {
      vole_params->flambda_product(res, res, tx);
      --p;
    }
    if (p == 0) break;
    vole_params->flambda_product(tx, tx, tx);
    p >>= 1;
  }
}

/** test if a field element is zero (non-ct version) */
EXPORT uint8_t flambda_is_zero_nonct(const vole_parameters* vole_params, const flambda_t* x) {
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  const uint8_t* const xx = (const uint8_t*)x;
  for (uint64_t i = 0; i < lambda_bytes; ++i) {
    if (xx[i]) return 0;
  }
  return 1;
}

/** test if a bit vector is zero (non-ct version) */
EXPORT uint8_t bitvec_is_zero_nonct(const uint8_t* x, uint64_t num_bits) {
  while (num_bits >= 8) {
    if (*x) return 0;
    ++x;
    num_bits -= 8;
  }
  if (num_bits > 0) {
    uint8_t mask = ((uint8_t)(1) << num_bits) - 1;
    if ((*x) & mask) return 0;
  }
  return 1;
}
