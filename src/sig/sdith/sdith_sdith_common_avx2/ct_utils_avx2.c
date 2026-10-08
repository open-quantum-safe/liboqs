#include <immintrin.h>
#include <inttypes.h>

#include "sdith_ct_utils.h"
#include "vole_private.h"

// Hardened constant-time helpers (x86-64).
//
// Same results as the _ref ones in ct_utils.c, but the mask arithmetic is
// written in inline assembly so the compiler cannot turn it back into a
// conditional jump. Two x86 idioms carry most of the work:
//
//   cmp a, b ; sbb m, m    m = -(a < b): CMP borrows when a < b, and
//                          SBB m,m computes m - m - CF = -CF.
//   neg x    ; sbb m, m    m = -(x != 0): NEG sets CF exactly when x != 0.
//
// Both are three-cycle, branch-free, and independent of the operand values.

EXPORT uint64_t mask_lt_ct_avx(uint64_t secret_a, uint64_t secret_b) {
  uint64_t m;
  // CF = (a < b) unsigned, then m = -CF
  __asm__("cmpq %2, %1\n\t"
          "sbbq %0, %0"
          : "=r"(m)
          : "r"(secret_a), "r"(secret_b)
          : "cc");
  return m;
}

EXPORT uint64_t mask_eq_ct_avx(uint64_t secret_a, uint64_t secret_b) {
  uint64_t m, t;
  // t = a ^ b; CF = (t != 0); m = -CF; then invert to get -(a == b)
  __asm__("movq %2, %1\n\t"
          "xorq %3, %1\n\t"
          "negq %1\n\t"
          "sbbq %0, %0\n\t"
          "notq %0"
          : "=r"(m), "=&r"(t)
          : "r"(secret_a), "r"(secret_b)
          : "cc");
  return m;
}

// Schoolbook restoring division (see the _ref version for the plain form), with
// the "did the remainder grow past d?" test turned into a mask: instead of
// subtracting under an if, we always subtract d & ge. Exactly 32 iterations,
// whatever n is.
EXPORT void div_rem_u32_ct_avx(uint32_t* q, uint32_t* r, uint32_t secret_n, uint32_t public_d) {
  CREQUIRE(public_d != 0, "division by zero");
  const uint64_t d = public_d;
  uint64_t rem = 0;
  uint64_t quo = 0;
  for (int64_t i = 31; i >= 0; --i) {
    rem = (rem << 1) | ((uint64_t)(secret_n >> i) & 1);
    uint64_t ge;
    // ge = -(rem >= d), i.e. the complement of -(rem < d)
    __asm__("cmpq %2, %1\n\t"
            "sbbq %0, %0\n\t"
            "notq %0"
            : "=r"(ge)
            : "r"(rem), "r"(d)
            : "cc");
    rem -= d & ge;
    quo |= ge & (UINT64_C(1) << i);
  }
  *q = (uint32_t)quo;
  *r = (uint32_t)rem;
}

// Vector AND/XOR: no branch to reintroduce, and the mask never reaches a flag
// register. bytelen is public, so the loop trip count and the tail are too.
EXPORT void bitvec_xor_to_masked_ct_avx(bitvec_t* res, uint64_t bytelen, const bitvec_t* secret_b,
                                        uint64_t secret_mask) {
  uint8_t* const r = (uint8_t*)res;
  const uint8_t* const b = (const uint8_t*)secret_b;
  const __m256i mv = _mm256_set1_epi64x((long long)secret_mask);
  uint64_t i = 0;
  for (; i + 32 <= bytelen; i += 32) {
    const __m256i rv = _mm256_loadu_si256((const __m256i*)(r + i));
    const __m256i bv = _mm256_loadu_si256((const __m256i*)(b + i));
    _mm256_storeu_si256((__m256i*)(r + i), _mm256_xor_si256(rv, _mm256_and_si256(bv, mv)));
  }
  const uint8_t m8 = (uint8_t)secret_mask;
  for (; i < bytelen; ++i) {
    r[i] ^= b[i] & m8;
  }
}

// Nothing to harden: every address here comes from public_bitpos, and the only
// secret is the bit pattern being xored in. The barrier just forbids the
// compiler from ever specialising on the value (e.g. skipping a zero).
EXPORT void bitvec_xoru32_ct_avx(bitvec_t* res, uint64_t bytelen, uint64_t public_bitpos, uint64_t public_nbits,
                                 uint32_t secret_value) {
  uint8_t* const r = (uint8_t*)res;
  CREQUIRE(public_nbits <= 32, "field too wide (%" PRId64 " bits)", public_nbits);
  const uint64_t first_byte = public_bitpos >> 3;
  const uint64_t shift = public_bitpos & 7;
  const uint64_t nbytes = (shift + public_nbits + 7) >> 3;
  CREQUIRE(first_byte + nbytes <= bytelen,  //
           "field [%" PRId64 ",+%" PRId64 ") out of range (bytelen %" PRId64 ")", public_bitpos, public_nbits, bytelen);
  const uint32_t field_mask = public_nbits >= 32 ? UINT32_C(-1) : (UINT32_C(1) << public_nbits) - 1;
  uint64_t shifted = (uint64_t)(secret_value & field_mask) << shift;
  __asm__("" : "+r"(shifted));  // opaque: the value must not drive any branch
  for (uint64_t k = 0; k < nbytes; ++k) {
    r[first_byte + k] ^= (uint8_t)(shifted >> (8 * k));
  }
}

EXPORT void bitvec_xorbit_ct_avx(bitvec_t* res, uint64_t bytelen, uint64_t secret_bitpos, uint64_t secret_mask) {
  uint8_t* const r = (uint8_t*)res;
  // debug-only: a range check on a secret would itself be a branch on a secret.
  // Out of range is harmless anyway -- no byte matches, so the call is a no-op.
  CASSERT((secret_bitpos >> 3) < bytelen,  //
          "bitpos %" PRId64 " out of range (bytelen %" PRId64 ")", secret_bitpos, bytelen);
  const uint64_t target_byte = secret_bitpos >> 3;
  // 1 << (secret_bitpos % 8). SHL by CL is data-independent on x86-64; spelling
  // it out here keeps the compiler from turning a 3-bit shift count into a
  // jump table or a chain of tests.
  uint64_t bit = 1;
  __asm__("shlq %%cl, %0" : "+r"(bit) : "c"(secret_bitpos & 7) : "cc");
  // The position is secret, so it cannot address memory: touch every byte and
  // let the mask decide which one actually changes.
  for (uint64_t i = 0; i < bytelen; ++i) {
    r[i] ^= (uint8_t)(bit & mask_eq_ct_avx(i, target_byte) & secret_mask);
  }
}
