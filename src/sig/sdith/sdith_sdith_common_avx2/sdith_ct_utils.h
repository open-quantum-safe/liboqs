#ifndef SDITH_CT_UTILS_H
#define SDITH_CT_UTILS_H

// Constant-time helpers.
//
// Scope: this is a digital signature, so the verifier only ever touches public
// data and has nothing to protect. Only the prover side must be constant-time,
// and only with respect to the secret key (the rsd solution), the keygen
// entropy, and the root ggm seed. Everything derived from the public key, from
// the message, or from the parameters may branch and index freely.
//
// "Constant-time" here means: no branch and no memory address depends on a
// secret. Secret *values* may flow through arithmetic and through AES-NI, which
// are data-independent. The idiom is a mask -- 0 or ~0 (all ones) -- built from
// a secret comparison and then AND-ed into the value that would otherwise have
// been selected by an `if`.
//
// As everywhere else in this codebase there is a _ref and an _avx
// implementation of each function, and as everywhere else the _ref one is the
// readable definition rather than the fast (here: rather than the hardened)
// one: it is written the way the algorithm reads, and a C compiler is free to
// turn its masks back into branches. The _avx implementations are the ones that
// are actually constant-time -- they pin the mask arithmetic down in inline
// assembly, which the compiler cannot rewrite. The two agree bit for bit, so
// the _ref ones stay usable as the test oracle.
//
// Functions are suffixed _ct_ref / _ct_avx; inside the ct_utils struct the _ct
// is implied and dropped.

#include "commons.h"

// Naming convention below: an input that may carry a secret is prefixed
// secret_, one that is required to be public is prefixed public_. Lengths
// (bytelen) are public everywhere in this codebase and are left unprefixed.
// The distinction is load-bearing: bitvec_xoru32 may only be handed a PUBLIC
// bit position (it writes at that address), while bitvec_xorbit accepts a
// SECRET one (and pays a full sweep of the vector for it).

/** mask_lt: -(a < b), i.e. ~0 when a < b and 0 otherwise (unsigned) */
/** mask_eq: -(a == b), i.e. ~0 when a == b and 0 otherwise */
typedef uint64_t CT_MASK_F(uint64_t secret_a, uint64_t secret_b);

/** q = n / d and r = n % d. */
typedef void CT_DIV_REM_U32_F(uint32_t* q, uint32_t* r, uint32_t secret_n, uint32_t public_d);

/** res ^= b & mask, over bytelen bytes. mask is 0 or ~0 (a whole-vector select). */
typedef void CT_BITVEC_XOR_TO_MASKED_F(bitvec_t* res, uint64_t bytelen, const bitvec_t* secret_b,
                                       uint64_t secret_mask);

/** res[public_bitpos .. public_bitpos+public_nbits) ^= the low public_nbits
 *  bits of value (public_nbits <= 32). Only the value is secret. */
typedef void CT_BITVEC_XORU32_F(bitvec_t* res, uint64_t bytelen, uint64_t public_bitpos, uint64_t public_nbits,
                                uint32_t secret_value);

/** res[secret_bitpos] ^= 1, if mask is ~0 (nothing if mask is 0). */
typedef void CT_BITVEC_XORBIT_F(bitvec_t* res, uint64_t bytelen, uint64_t secret_bitpos, uint64_t secret_mask);

EXPORT uint64_t mask_lt_ct_ref(uint64_t secret_a, uint64_t secret_b);
EXPORT uint64_t mask_eq_ct_ref(uint64_t secret_a, uint64_t secret_b);
EXPORT void div_rem_u32_ct_ref(uint32_t* q, uint32_t* r, uint32_t secret_n, uint32_t public_d);
EXPORT void bitvec_xor_to_masked_ct_ref(bitvec_t* res, uint64_t bytelen, const bitvec_t* secret_b,
                                        uint64_t secret_mask);
EXPORT void bitvec_xoru32_ct_ref(bitvec_t* res, uint64_t bytelen, uint64_t public_bitpos, uint64_t public_nbits,
                                uint32_t secret_value);
EXPORT void bitvec_xorbit_ct_ref(bitvec_t* res, uint64_t bytelen, uint64_t secret_bitpos, uint64_t secret_mask);

#ifdef __x86_64__
EXPORT uint64_t mask_lt_ct_avx(uint64_t secret_a, uint64_t secret_b);
EXPORT uint64_t mask_eq_ct_avx(uint64_t secret_a, uint64_t secret_b);
EXPORT void div_rem_u32_ct_avx(uint32_t* q, uint32_t* r, uint32_t secret_n, uint32_t public_d);
EXPORT void bitvec_xor_to_masked_ct_avx(bitvec_t* res, uint64_t bytelen, const bitvec_t* secret_b,
                                        uint64_t secret_mask);
EXPORT void bitvec_xoru32_ct_avx(bitvec_t* res, uint64_t bytelen, uint64_t public_bitpos, uint64_t public_nbits,
                                uint32_t secret_value);
EXPORT void bitvec_xorbit_ct_avx(bitvec_t* res, uint64_t bytelen, uint64_t secret_bitpos, uint64_t secret_mask);
#endif

typedef struct ct_utils_t {
  CT_MASK_F* mask_lt;
  CT_MASK_F* mask_eq;
  CT_DIV_REM_U32_F* div_rem_u32;
  CT_BITVEC_XOR_TO_MASKED_F* bitvec_xor_to_masked;
  CT_BITVEC_XORU32_F* bitvec_xoru32;
  CT_BITVEC_XORBIT_F* bitvec_xorbit;
} ct_utils;

static const ct_utils ct_utils_ref = {
    mask_lt_ct_ref,                //
    mask_eq_ct_ref,                //
    div_rem_u32_ct_ref,            //
    bitvec_xor_to_masked_ct_ref,   //
    bitvec_xoru32_ct_ref,          //
    bitvec_xorbit_ct_ref,          //
};
// ONLY_REF_IMPLEMENTATION drops the *_avx2.c translation units, so this table
// would hold undefined references: it is only compiled with the avx2 backend.
#if defined(__x86_64__) && !defined(ONLY_REF_IMPLEMENTATION)
static const ct_utils ct_utils_avx = {
    mask_lt_ct_avx,                //
    mask_eq_ct_avx,                //
    div_rem_u32_ct_avx,            //
    bitvec_xor_to_masked_ct_avx,   //
    bitvec_xoru32_ct_avx,          //
    bitvec_xorbit_ct_avx,          //
};
#endif

#endif  // SDITH_CT_UTILS_H
