#include "sdith_ct_utils.h"

#include <inttypes.h>

#include "vole_private.h"

// Reference constant-time helpers.
//
// As stated in sdith_ct_utils.h, these are the readable definitions, not the
// hardened implementations: they say what each function computes, and the _avx
// ones in ct_utils_avx2.c say how to compute it without giving the compiler
// room to reintroduce a branch. A C compiler may legally turn any of the masks
// below back into a conditional jump, so a build that must actually be
// constant-time has to use the _avx table.

EXPORT uint64_t mask_lt_ct_ref(uint64_t secret_a, uint64_t secret_b) {
  return -(uint64_t)(secret_a < secret_b);
}

EXPORT uint64_t mask_eq_ct_ref(uint64_t secret_a, uint64_t secret_b) {
  return -(uint64_t)(secret_a == secret_b);
}

// Schoolbook restoring division, most significant bit first: bring one bit of n
// down into the remainder, and subtract d whenever the remainder has grown past
// it. rem stays below 2*d < 2^33, hence the 64-bit accumulator.
EXPORT void div_rem_u32_ct_ref(uint32_t* q, uint32_t* r, uint32_t secret_n, uint32_t public_d) {
  CREQUIRE(public_d != 0, "division by zero");
  uint64_t rem = 0;
  uint32_t quo = 0;
  for (int64_t i = 31; i >= 0; --i) {
    rem = (rem << 1) | ((secret_n >> i) & 1);
    if (rem >= public_d) {
      rem -= public_d;
      quo |= (uint32_t)1 << i;
    }
  }
  *q = quo;
  *r = (uint32_t)rem;
}

// res ^= b, or res unchanged, depending on the mask: a whole-vector select
// written as an AND so that both outcomes run the same instructions.
EXPORT void bitvec_xor_to_masked_ct_ref(bitvec_t* res, uint64_t bytelen, const bitvec_t* secret_b,
                                        uint64_t secret_mask) {
  uint8_t* const r = (uint8_t*)res;
  const uint8_t* const b = (const uint8_t*)secret_b;
  const uint8_t m8 = (uint8_t)secret_mask;
  for (uint64_t i = 0; i < bytelen; ++i) {
    r[i] ^= b[i] & m8;
  }
}

// The field lands at a position known to the attacker, so the bytes it touches
// can be addressed directly: only the bits being written are secret. Both the
// offset and the width are public, so the byte range is exact -- the caller's
// vector never needs slack past the last field.
EXPORT void bitvec_xoru32_ct_ref(bitvec_t* res, uint64_t bytelen, uint64_t public_bitpos, uint64_t public_nbits,
                                 uint32_t secret_value) {
  uint8_t* const r = (uint8_t*)res;
  CREQUIRE(public_nbits <= 32, "field too wide (%" PRId64 " bits)", public_nbits);
  const uint64_t first_byte = public_bitpos >> 3;
  const uint64_t shift = public_bitpos & 7;
  const uint64_t nbytes = (shift + public_nbits + 7) >> 3;
  CREQUIRE(first_byte + nbytes <= bytelen,  //
           "field [%" PRId64 ",+%" PRId64 ") out of range (bytelen %" PRId64 ")", public_bitpos, public_nbits, bytelen);
  // keep only the low public_nbits bits: the width is public, so is the mask
  const uint32_t field_mask = public_nbits >= 32 ? UINT32_C(-1) : (UINT32_C(1) << public_nbits) - 1;
  const uint64_t shifted = (uint64_t)(secret_value & field_mask) << shift;
  for (uint64_t k = 0; k < nbytes; ++k) {
    r[first_byte + k] ^= (uint8_t)(shifted >> (8 * k));
  }
}

// Here the position itself is secret, so it cannot be used to address memory:
// every word of the vector is read and written, and a mask picks the one that
// actually changes. Cost is the whole vector per call instead of one byte.
EXPORT void bitvec_xorbit_ct_ref(bitvec_t* res, uint64_t bytelen, uint64_t secret_bitpos, uint64_t secret_mask) {
  uint8_t* const r = (uint8_t*)res;
  // debug-only: a range check on a secret would itself be a branch on a secret.
  // Out of range is harmless anyway -- no byte matches, so the call is a no-op.
  CASSERT((secret_bitpos >> 3) < bytelen,  //
          "bitpos %" PRId64 " out of range (bytelen %" PRId64 ")", secret_bitpos, bytelen);
  const uint64_t target_byte = secret_bitpos >> 3;
  const uint8_t bit = (uint8_t)1 << (secret_bitpos & 7);
  for (uint64_t i = 0; i < bytelen; ++i) {
    r[i] ^= bit & (uint8_t)mask_eq_ct_ref(i, target_byte) & (uint8_t)secret_mask;
  }
}
