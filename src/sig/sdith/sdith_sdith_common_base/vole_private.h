#ifndef AQ_VOLE_IMPL_VOLE_PRIVATE_H
#define AQ_VOLE_IMPL_VOLE_PRIVATE_H

#include <stdio.h>
#include <stdlib.h>

#include "commons.h"
#include "ggm.h"
#include "sdith_algebra.h"
#include "sdith_arithmetic.h"
#include "sdith_vole_to_piop.h"
#include "sdith_piop_circuits.h"
#include "sdith_prng.h"
#include "sdith_rsd.h"
#include "sdith_signature.h"
#include "vole_parameters.h"
#include "vole_generation.h"

#define CREQUIRE(condition, error_printf...)     \
  do {                                           \
    if (!(condition)) {                          \
      fprintf(stderr, "ERROR!! :" error_printf); \
      abort();                                   \
    }                                            \
  } while (0)

#define CREQUIRE_ALIGNMENT(variable, alignment) \
  CREQUIRE(((uint64_t)(variable) & ((alignment) - 1)) == 0, "alignment problem:" __FILE__ " : %d", __LINE__);

#ifdef NDEBUG
#define CASSERT(condition, error_printf...)
#define CASSERT_ALIGNMENT(variable, alignment)
#else
#define CASSERT CREQUIRE
#define CASSERT_ALIGNMENT CREQUIRE_ALIGNMENT
#endif



#if defined(_WIN32) || defined(__APPLE__)
#define __always_inline inline __attribute((always_inline))
#endif

__always_inline uint64_t alignment_of(uint64_t x) { return (x ^ (x - 1)) >> 1; }

// irreducible polynomial constant for GF128
#define GF128_PV 0b10000111
#define GF192_PV 0b10000111
#define GF256_PV 0b10000100101
static const __uint128_t GF128_P = GF128_PV;
static const uint64_t GF256_P = GF256_PV;
static const uint64_t GF192_P = GF192_PV;
static const uint64_t GF121_P = 0b100100011;

// in the public API, the layout of the fields is OPAQUE
// here, we only provide possible instantiations.
// it is also possible to cast these types to __m128i for instance

union gf128_t {
  uint8_t v[16];
  __uint128_t v128;
};
union gf121_t {
  uint8_t v[16];
  __uint128_t v128;
};
union gf192_t {
  uint8_t v[24];
  uint64_t v64[3];
};
union gf256_t {
  uint8_t v[32];
  uint64_t v64[4];
};

// Constant-time carryless (GF(2)[x]) multiply helpers used by the reference
// field multiplies. gf_bmul32 is BearSSL's branchless 32x32->64 carryless
// product: only integer multiply, mask and xor, so it has no data-dependent
// branch or memory access and runs in time independent of its operands (the
// integer multiplier is constant-time on the targets we care about, incl.
// Cortex-M4 UMULL). gf_clmul64 combines three gf_bmul32 (Karatsuba) into a
// full 64x64->128 carryless product.
static __always_inline uint64_t gf_bmul32(uint32_t x, uint32_t y) {
  uint32_t x0 = x & 0x11111111u, x1 = x & 0x22222222u, x2 = x & 0x44444444u, x3 = x & 0x88888888u;
  uint32_t y0 = y & 0x11111111u, y1 = y & 0x22222222u, y2 = y & 0x44444444u, y3 = y & 0x88888888u;
  uint64_t z0 = (uint64_t)x0 * y0 ^ (uint64_t)x1 * y3 ^ (uint64_t)x2 * y2 ^ (uint64_t)x3 * y1;
  uint64_t z1 = (uint64_t)x0 * y1 ^ (uint64_t)x1 * y0 ^ (uint64_t)x2 * y3 ^ (uint64_t)x3 * y2;
  uint64_t z2 = (uint64_t)x0 * y2 ^ (uint64_t)x1 * y1 ^ (uint64_t)x2 * y0 ^ (uint64_t)x3 * y3;
  uint64_t z3 = (uint64_t)x0 * y3 ^ (uint64_t)x1 * y2 ^ (uint64_t)x2 * y1 ^ (uint64_t)x3 * y0;
  z0 &= 0x1111111111111111ull; z1 &= 0x2222222222222222ull;
  z2 &= 0x4444444444444444ull; z3 &= 0x8888888888888888ull;
  return z0 | z1 | z2 | z3;
}
static __always_inline void gf_clmul64(uint64_t a, uint64_t b, uint64_t* hi, uint64_t* lo) {
  uint32_t a0 = (uint32_t)a, a1 = (uint32_t)(a >> 32), b0 = (uint32_t)b, b1 = (uint32_t)(b >> 32);
  uint64_t z0 = gf_bmul32(a0, b0);
  uint64_t z2 = gf_bmul32(a1, b1);
  uint64_t z1 = gf_bmul32(a0 ^ a1, b0 ^ b1) ^ z0 ^ z2;
  *lo = z0 ^ (z1 << 32);
  *hi = z2 ^ (z1 >> 32);
}

extern const gf256 GF256_ONE;
extern const gf256 GF256_ZERO;
// functions by value should are for test and assertions purposes only
EXPORT uint8_t gf256v_equals(const gf256 a, const gf256 b);
EXPORT uint8_t gf256v_bitof(const gf256 a, const uint64_t position);
EXPORT gf256 gf256v_sum(const gf256 a, const gf256 b);
EXPORT gf256 gf256v_lsh(const gf256 a, const uint64_t amount);
EXPORT gf256 gf256v_rsh(const gf256 a, const uint64_t amount);
EXPORT gf256 gf256v_mul(const gf256 a, const gf256 b);
// left and right shift by pointer are not part of the private API
EXPORT void gf256p_lsh(gf256* const res, const gf256* const a, const uint64_t amount);
EXPORT void gf256p_rsh(gf256* const res, const gf256* const a, const uint64_t amount);

EXPORT void gf192_sum_ref(gf192* res, const gf192* a, const gf192* b);

extern const gf192 GF192_ONE;
extern const gf192 GF192_ZERO;
// functions by value should are for test and assertions purposes only
EXPORT uint8_t gf192v_equals(const gf192 a, const gf192 b);
EXPORT uint8_t gf192v_bitof(const gf192 a, const uint64_t position);
EXPORT gf192 gf192v_sum(const gf192 a, const gf192 b);
EXPORT gf192 gf192v_lsh(const gf192 a, const uint64_t amount);
EXPORT gf192 gf192v_rsh(const gf192 a, const uint64_t amount);
EXPORT gf192 gf192v_mul(const gf192 a, const gf192 b);
// left and right shift by pointer are not part of the private API
EXPORT void gf192p_lsh(gf192* const res, const gf192* const a, const uint64_t amount);
EXPORT void gf192p_rsh(gf192* const res, const gf192* const a, const uint64_t amount);

EXPORT void gf192_sum_ref(gf192* res, const gf192* a, const gf192* b);

extern const gf128 GF128_ONE;
extern const gf128 GF128_ZERO;
// functions by value should are for test and assertions purposes only
EXPORT uint8_t gf128v_equals(const gf128 a, const gf128 b);
EXPORT uint8_t gf128v_bitof(const gf128 a, const uint64_t position);
EXPORT gf128 gf128v_sum(const gf128 a, const gf128 b);
EXPORT gf128 gf128v_lsh(const gf128 a, const uint64_t amount);
EXPORT gf128 gf128v_rsh(const gf128 a, const uint64_t amount);
EXPORT gf128 gf128v_mul(const gf128 a, const gf128 b);
// left and right shift by pointer are not part of the private API
EXPORT void gf128p_lsh(gf128* const res, const gf128* const a, const uint64_t amount);
EXPORT void gf128p_rsh(gf128* const res, const gf128* const a, const uint64_t amount);
EXPORT void gf128p_sum(gf128* const res, const gf128* const a, const gf128* const b);
EXPORT void gf128p_mul(gf128* const res, const gf128* const a, const gf128* const b);

EXPORT void gf128_sum_ref(gf128* res, const gf128* a, const gf128* b);

// binary valuation
uint16_t binval_of(uint16_t i);
uint16_t grey_of(uint16_t x);
uint16_t inv_grey_of(uint16_t x);

/** @brief extract/xorto a kappa-bit uint from a bit array.
 *  data must be 8-byte aligned and its size must be a multiple of 8. */
EXPORT uint32_t extract_kappabit_uint(uint64_t kappa, uint64_t bitpos, const void* data);
EXPORT void xorto_kappabit_uint(uint64_t kappa, uint64_t bitpos, void* data, uint64_t value);

/** raise to the power p */
EXPORT void flambda_power(const vole_parameters* vole_params, flambda_t* res, const flambda_t* x, uint64_t p);

/** test if a field element is zero (non-ct version) */
EXPORT uint8_t flambda_is_zero_nonct(const vole_parameters* vole_params, const flambda_t* x);
/** test if a bit vector is zero (non-ct version) */
EXPORT uint8_t bitvec_is_zero_nonct(const uint8_t* x, uint64_t num_bits);

#define TMP_SPACE_MAP_ALIGNED(VARTYPE, VARNAME, VARALIGN, BYTE_SIZE)                        \
  tmp_space = (uint8_t*)(((uint64_t)tmp_space + (VARALIGN - 1)) & (uint64_t)(-(VARALIGN))); \
  VARTYPE VARNAME = (VARTYPE)tmp_space;                                                     \
  tmp_space += (BYTE_SIZE)

#define TMP_SPACE_MAP(VARTYPE, VARNAME, BYTE_SIZE) \
  VARTYPE VARNAME = (VARTYPE)tmp_space;            \
  tmp_space += (BYTE_SIZE)

// this one is empty
#define TMP_SPACE_MAP_CALL(BYTE_SIZE)

#define COMP_SPACE_INIT()     \
  struct tmp_bytes_comp_t ts; \
  tmp_bytes_init(&ts)

#define COMP_SPACE_MAP_ALIGNED(VARTYPE, VARNAME, VARALIGN, BYTE_SIZE) \
  tmp_bytes_reserve_local_var_aligned(&ts, (VARALIGN), (BYTE_SIZE))

#define COMP_SPACE_MAP(VARTYPE, VARNAME, BYTE_SIZE) tmp_bytes_reserve_local_var(&ts, (BYTE_SIZE))

#define COMP_SPACE_MAP_CALL(BYTE_SIZE) tmp_bytes_reserve_call(&ts, (BYTE_SIZE))

#define COMP_SPACE_RETURN() return ts.max_size

/** helper structure to compute the tmp space required for functions */
typedef struct tmp_bytes_comp_t {
  uint64_t local_size;      // size of local variables
  uint64_t max_size;        // local variables + calls
  uint64_t last_alignment;  // last requested local alignment
} tmp_bytes_comp_t;

EXPORT void tmp_bytes_init(tmp_bytes_comp_t* tmp_bytes_comp);
EXPORT void tmp_bytes_reserve_local_var(tmp_bytes_comp_t* tmp_bytes_comp, uint64_t var_bytes);
EXPORT void tmp_bytes_reserve_local_var_aligned(tmp_bytes_comp_t* tmp_bytes_comp, uint64_t var_align,
                                                uint64_t var_bytes);
EXPORT void tmp_bytes_reserve_call(tmp_bytes_comp_t* tmp_bytes_comp, uint64_t call_tmp_bytes);

EXPORT_DECL const uint8_t HASH_BAVC_PREFIX;
EXPORT_DECL const uint8_t HASH_AUX_PREFIX;
EXPORT_DECL const uint8_t HASH_LINES_PREFIX;
EXPORT_DECL const uint8_t HASH_PIOP_PREFIX;
EXPORT_DECL const uint8_t PROOFOW_H0_PREFIX;
EXPORT_DECL const uint8_t PROOFOW_H1_PREFIX;
EXPORT_DECL const uint64_t PROOFOW_CTR_REVEALED_BYTES;

// sdith parameters: deduced parameter set
typedef struct extended_parameters_t {
  uint64_t lambda;
  uint64_t kappa;
  uint64_t tau;
  uint64_t target_topen;
  uint64_t rsd_n;
  uint64_t rsd_w;
  uint64_t mux_arities[8];
  uint64_t mux_depth;
  uint64_t rsd_npw;
  uint64_t rsd_codim;
  uint64_t proofow_w;
  uint64_t delta0_bits;
  uint64_t delta0_dwords;
  uint64_t delta0_bytess;
  uint64_t delta0_capacity;
  vole_parameters vole_params;
  uint64_t mux_inputs;
  uint64_t chall_unitary_powers;
  uint64_t degree;
  uint64_t num_inputs_pairs;
  uint64_t num_cchk_pairs;
  uint64_t num_cz_pairs;
  uint64_t real_L;
  uint64_t L;
  uint64_t Lbyte;
  uint64_t Lslice;
  uint64_t lambda_bytes;
  uint64_t cchk_matrix_nrows;
  uint64_t cchk_matrix_ncols;
  uint64_t rsd_codim_bytes;
  uint64_t rsd_codim_limbs;
  uint64_t rsd_codim_slice; // byte slice if we store the full matrix H (multiple of the rng block)
} extended_parameters_t;

/** compute the rsd codim slice (bytes), accounting for the prng block size and the actual bitsize */
EXPORT void compute_rsd_codim_slice(
  uint64_t* rsd_codim_limbs,      // [out] number of limbs
  uint64_t* rsd_codim_slice,      // [out] number of slice bytes to hold the prng blocks
  const uint64_t lambda, const uint64_t rsd_codim);

EXPORT void compute_extended_parameters(extended_parameters_t* res, const signature_parameters* sig_params);

EXPORT uint32_t ceil_log2i(uint32_t x);

#endif  // AQ_VOLE_IMPL_VOLE_PRIVATE_H
