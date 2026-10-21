#include <string.h>

#include "vole_private.h"

/**
 * Recombine in_L = lambda. out_L full-size std vole pairs over F2
 * To form out_L full-size std vole F2^LAMBDA
 */
EXPORT void prover_f2_to_flambda_deg1_std_vole_ct_ref(  //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t num_pairs,                                 // number of output pairs to produce
    fpoly_t* out_f,                                     // [out] deg-1 fpoly[num_pairs]
    const bitvec_t* in_u,                               // [in] in_L bits  where in_L = num_pairs * LAMBDA
    const flambda_t* in_v)                              // [in] flambda[in_L]
{
  uint8_t* out = (uint8_t*)out_f;
  const uint8_t* u = (uint8_t*)in_u;
  const uint8_t* v = (uint8_t*)in_v;
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  const uint64_t lambda = vole_params->LAMBDA;
  CASSERT_ALIGNMENT(out_f, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(in_v, alignment_of(lambda_bytes));

  for (size_t i = 0; i < num_pairs; i++) {
    (*vole_params->flambda_sum_pow2)(out + lambda_bytes * 2 * i, v + lambda_bytes * lambda * i);
    memcpy(out + lambda_bytes * (2 * i + 1), u + lambda_bytes * i, lambda_bytes);
  }
}

/**
 * Recombine in_L = lambda. out_L full-size std vole pairs over F2
 * To form out_L full-size std vole F2^LAMBDA
 */
EXPORT void verifier_f2_to_flambda_deg1_std_vole_ref(  //
    const vole_parameters* vole_params,                // dimensions
    uint64_t num_pairs,                                // number of output pairs to produce
    flambda_t* out_q,                                  // [out] gflambda[num_pairs]
    const flambda_t* in_q)                             // [in] gflambda in_q[in_L]  where in_L = num_pairs * LAMBDA
{
  uint8_t* out = (uint8_t*)out_q;
  const uint8_t* in = (uint8_t*)in_q;
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  const uint64_t lambda = vole_params->LAMBDA;
  CASSERT_ALIGNMENT(out_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(in_q, alignment_of(lambda_bytes));

  for (size_t i = 0; i < num_pairs; i++) {
    (*vole_params->flambda_sum_pow2)(out + lambda_bytes * i, in + lambda_bytes * lambda * i);
  }
}

/**
 * convert d degree 1 std-vole over flambda to one degree-d vole
 */
EXPORT void prover_flambda_deg1_to_degd_vole_ref(  //
    const vole_parameters* vole_params,            // dimensions
    uint64_t out_degree,                           // degree of the output pair
    fpoly_t* out_f,                                // single poly_t of degree out_degree
    const fpoly_t* in_f)                           // out_degree poly[out_degree]
{
  CASSERT(out_degree > 0, "degree 0 is not supported");
  // this function can work in place
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(out_f, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(in_f, alignment_of(lambda_bytes));
  const uint8_t* const in = (uint8_t*)in_f;
  uint8_t* const out = (uint8_t*)out_f;

  // copy first coefficient of first polynomial to out_f
  vole_params->flambda_set(out, in);

  for (size_t i = 1; i < out_degree; i++) {
    (*vole_params->flambda_sum)(out + lambda_bytes * i,          // result
                                in + lambda_bytes * (2 * i),     // low coefficient of i-th in_f poly
                                in + lambda_bytes * (2 * i - 1)  // high coefficient of (i-1)-th in_f poly
    );
  }

  // copy second coefficient of last polynomial to out_f
  vole_params->flambda_set(out + out_degree * lambda_bytes, in + (2 * out_degree - 1) * lambda_bytes);
}

/**
 * convert d degree 1 std-vole to one degree-d vole
 */
EXPORT void verifier_flambda_deg1_to_degd_vole_ref(  //
    const vole_parameters* vole_params,              // dimensions
    uint64_t out_degree,                             // degree of the output pair
    flambda_t* out_q,                                // [out] single gflambda
    const flambda_t* in_q,                           // [in] gflambda[out_deg]
    const flambda_t* delta)                          // verifier's point powers
{
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(out_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(in_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(delta, alignment_of(lambda_bytes));
  const uint8_t* const in = (uint8_t*)in_q;
  uint8_t* const out = (uint8_t*)out_q;
  // we use a temporary so that the function works in place
  flambda_max_t tmp;

  (*vole_params->flambda_set)(tmp, in + (out_degree - 1) * lambda_bytes);
  for (int64_t i = out_degree - 2; i >= 0; i--) {
    (*vole_params->flambda_product)(tmp, tmp, delta);
    (*vole_params->flambda_sum)(tmp, tmp, in + i * lambda_bytes);
  }
  (*vole_params->flambda_set)(out, tmp);
}

/**
 * convert a degree-d vole from std to cst
 */
EXPORT void prover_flambda_degd_std_to_cst_vole_ct_ref(  //
    const vole_parameters* vole_params,                  // dimensions
    uint64_t degree,                                     // degree d of of the pair to process
    fpoly_t* out_f,                                      // [out] single poly of degree d
    const fpoly_t* in_f)                                 // [in] single poly of degree d
{
  // mirror f
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(out_f, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(in_f, alignment_of(lambda_bytes));
  uint8_t* res = (uint8_t*)out_f;
  const uint8_t* in = (const uint8_t*)in_f;
  if (in_f == out_f) {
    // in place variant
    uint64_t half_d = (degree + 1) >> 1;
    for (uint64_t i = 0; i < half_d; i++) {
      uint8_t* r1 = res + i * lambda_bytes;
      uint8_t* r2 = res + (degree - i) * lambda_bytes;
      // swap r1 <-> r2
      vole_params->flambda_sum(r1, r1, r2);
      vole_params->flambda_sum(r2, r2, r1);
      vole_params->flambda_sum(r1, r1, r2);
    }
  } else {
    // out of place variant
    for (uint64_t i = 0; i <= degree; i++) {
      memcpy(res + i * lambda_bytes, in + (degree - i) * lambda_bytes, lambda_bytes);
    }
  }
}

/**
 * convert d degree 1 std-vole to one degree-d cst-vole
 */
EXPORT void verifier_flambda_degd_std_to_cst_vole_ref(  //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t degree,                                    // output degree
    flambda_t* out_q,                                   // [out] single evaluation
    const flambda_t* in_q,                              // [in] single evaluation
    const flambda_t* delta_pow_d)                       // [in] verifier's point to the power d
{
#ifndef NDEBUG
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CREQUIRE_ALIGNMENT(out_q, alignment_of(lambda_bytes));
  CREQUIRE_ALIGNMENT(in_q, alignment_of(lambda_bytes));
  CREQUIRE_ALIGNMENT(delta_pow_d, alignment_of(lambda_bytes));
#endif
  vole_params->flambda_product(out_q, in_q, delta_pow_d);
}

/**
 * convert packed degree 1 std-vole to packed cst-vole
 */
EXPORT void prover_f2_std_to_cst_vole_ct_ref(     //
    const vole_parameters* vole_params,           // dimensions
    uint64_t num_pairs,                           // num_pairs to process
    bitvec_t* out_u, flambda_t* out_v,            // [out] bit[num_pairs], flambda[num_pairs]
    const bitvec_t* in_u, const flambda_t* in_v)  // [in] bit[num_pairs], flambda[num_pairs]
{
#ifndef NDEBUG
  uint64_t lambda_bytes = vole_params->lambda_bytes;
  CREQUIRE_ALIGNMENT(in_v, alignment_of(lambda_bytes));
  CREQUIRE_ALIGNMENT(out_v, alignment_of(lambda_bytes));
#endif
  if (in_u == out_u && in_v == out_v) return;
  memcpy(out_u, in_u, (num_pairs + 7) >> 3);
  memcpy(out_v, in_v, vole_params->lambda_bytes * num_pairs);
}

/**
 * convert packed degree 1 std-vole to packed cst-vole
 */
EXPORT void verifier_f2_std_to_cst_vole_ref(  //
    const vole_parameters* vole_params,       // dimensions
    uint64_t num_pairs,                       // num_pairs to process
    flambda_t* out_q,                         // [out] gflambda[num_pairs]
    const flambda_t* in_q,                    // [in] gflambda[num_pairs]
    const flambda_t* delta2)                  // [in] verifier's point
{
  const uint8_t* in = (uint8_t*)in_q;
  uint8_t* out = (uint8_t*)out_q;
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(in_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(out_q, alignment_of(lambda_bytes));

  for (size_t i = 0; i < num_pairs; i++) {
    (*vole_params->flambda_product)(out + lambda_bytes * i, in + lambda_bytes * i, delta2);
  }
}

/** @brief converts delta1 to delta2
 * delta2 is the field inverse of delta1
 */
EXPORT void delta2_from_delta1_ref(const vole_parameters* vole_params, flambda_t* delta2, const flambda_t* delta1) {
  vole_params->flambda_inverse(delta2, delta1);
}
