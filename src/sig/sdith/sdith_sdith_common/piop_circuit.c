#include <string.h>

#include "vole_private.h"

static const flambda_max_t FLAMBDA_ONE = {1, 0, 0, 0};

/**
 * prover packed secret input gate (degree 1)
 */
EXPORT void prover_cst_vole_packed_secret_input_ct_ref(  //
    const vole_parameters* vole_params,                  // dimensions
    uint64_t num_inputs,                                 // number of inputs to process
    bitvec_t* out_pub,                                   // [out] output publication (packed over f2)
    fpoly_t* out_f,                                      // [out] poly of degree 1 (cst term over f2)
    const bitvec_t* in_value,                            // [in] secret bit values (packed over f2)
    const bitvec_t* in_rvp_u,                            // [in] packed u terms
    const flambda_t* in_rvp_v)                           // [in] packed v terms
{
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(in_rvp_v, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(out_f, alignment_of(lambda_bytes));
  uint8_t* const f = (uint8_t*)out_f;
  const uint8_t* const rvp_v = (uint8_t*)in_rvp_v;
  const uint8_t* const inval = (uint8_t*)in_value;
  vole_params->bitvec_xor((num_inputs + 7) >> 3, out_pub, in_rvp_u, in_value);

  uint64_t leftover_bits = num_inputs & 7;
  if (leftover_bits) {
    uint8_t* out_pub_u8 = (uint8_t*)out_pub;
    uint64_t idx = ((num_inputs + 7) >> 3) - 1;
    uint64_t and_mask = (1 << leftover_bits) - 1;
    out_pub_u8[idx] &= and_mask;
  }

  memset(out_f, 0, 2 * num_inputs * lambda_bytes);
  for (uint64_t i = 0; i < num_inputs; i++) {
    uint8_t* const fi = f + i * 2 * lambda_bytes;
    fi[0] = (inval[i >> 3] >> (i & 7)) & 1;
    vole_params->flambda_set(fi + lambda_bytes, rvp_v + i * lambda_bytes);
  }
}

/**
 * verifier packed secret input gate (degree 1)
 */
EXPORT void verifier_cst_vole_packed_secret_input_ref(  //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t num_inputs,                                // number of inputs to process
    fpoly_t* out_q,                                     // [out] vole evaluations
    const bitvec_t* in_pub,                             // [in] publications (packed over f2)
    const flambda_t* in_rvp_q,                          // [in] packed v terms
    const flambda_t* delta2)                            // [in] verifier's point
{
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(in_rvp_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(out_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(delta2, alignment_of(lambda_bytes));

  uint8_t* const q = (uint8_t*)out_q;
  const uint8_t* const rvp_q = (uint8_t*)in_rvp_q;
  const uint8_t* const inpub = (uint8_t*)in_pub;
  uint8_t buf[32] = {0};

#ifndef NDEBUG
  uint64_t leftover_bits = num_inputs & 7;
  if (leftover_bits) {
    uint64_t mask = ((1 << (8 - leftover_bits)) - 1) << leftover_bits;
    uint64_t idx = ((num_inputs + 7) >> 3) - 1;
    CASSERT((inpub[idx] & mask) == 0, "[in] publications does not have trailing zeros");
  }
#endif  // NDEBUG

  for (uint64_t i = 0; i < num_inputs; i++) {
    buf[0] = (inpub[i >> 3] >> (i & 7)) & 1;
    vole_params->flambda_sum(q + i * lambda_bytes, rvp_q + i * lambda_bytes, buf);
  }
}

/**
 * prover check-zero gate (degree d)
 */
EXPORT void prover_cst_vole_check_zero_gate_ct_ref(  //
    const vole_parameters* vole_params,              // dimensions
    uint64_t degree,                                 // input degree
    fpoly_t* out_pub,                                // [out] output publication (degree d-1) -- not aligned
    const fpoly_t* in_f,                             // [in] input (degree d)
    const fpoly_t* in_rvp_f)                         // [in] rvp (degree d-1)
{
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(in_rvp_f, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(in_f, alignment_of(lambda_bytes));
  uint8_t* const pub = (uint8_t*)out_pub;
  const uint8_t* const in = (uint8_t*)in_f;
  const uint8_t* const rvp = (uint8_t*)in_rvp_f;
  flambda_max_t pubi;  // we use a temp variable for alignment issues
  for (uint64_t i = 0; i < degree; i++) {
    vole_params->flambda_sum(pubi, rvp + i * lambda_bytes, in + (i + 1) * lambda_bytes);
    memcpy(pub + i * lambda_bytes, pubi, lambda_bytes);
  }
}

/**
 * verifier check-zero gate (degree d)
 */
EXPORT uint8_t verifier_cst_vole_check_zero_gate_ref(  //
    const vole_parameters* vole_params,                // dimensions
    uint64_t degree,                                   // input degree
    flambda_t* out_value,                              // [out] deduced output value
    const fpoly_t* in_pub,                             // [in] publication (degree d-1) -- not aligned
    const flambda_t* in_q,                             // [in] input (degree d)
    const flambda_t* in_rvp_q,                         // [in] rvp (degree d-1)
    const flambda_t* delta2)                           // [in] verifier's point
{
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(in_rvp_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(in_q, alignment_of(lambda_bytes));
  const uint8_t* const pub = (uint8_t*)in_pub;
  flambda_max_t pubi;  // tmp variable for alignment purposes
  // compute pub(delta2) via horner method
  memcpy(out_value, pub + (degree - 1) * lambda_bytes, lambda_bytes);
  for (int64_t i = degree - 2; i >= 0; i--) {
    vole_params->flambda_product(out_value, out_value, delta2);
    memcpy(pubi, pub + i * lambda_bytes, lambda_bytes);
    vole_params->flambda_sum(out_value, out_value, pubi);
  }
  // compute (pub(delta2) + rvp_q) * delta2
  vole_params->flambda_sum(out_value, out_value, in_rvp_q);
  vole_params->flambda_product(out_value, out_value, delta2);
  // add in_q -> the constant term shall remain
  vole_params->flambda_sum(out_value, out_value, in_q);
  // zero test (does not need to be ct, nor efficient)
  const uint8_t* const out = (uint8_t*)out_value;
  for (uint64_t i = 0; i < lambda_bytes; i++) {
    if (out[i]) return 0;
  }
  return 1;
}

/**
 * prover xor gate
 */
EXPORT void prover_cst_vole_xor_gate_ct_ref(  //
    const vole_parameters* vole_params,       // dimensions
    fpoly_t* res_f,                           // [out] poly of degree 1 (cst term over f2)
    const fpoly_t* a_f, uint64_t a_degree,    // [in] a
    const fpoly_t* b_f, uint64_t b_degree)    // [in] b
{
  uint8_t* res = (uint8_t*)res_f;
  const uint8_t* a = (uint8_t*)a_f;
  const uint8_t* b = (uint8_t*)b_f;
  const uint64_t lambda_bytes = vole_params->lambda_bytes;

  // addition
  if (a_degree < b_degree) {
    for (size_t i = 0; i <= a_degree; i++) {
      (*vole_params->flambda_sum)(res + lambda_bytes * i, a + lambda_bytes * i, b + lambda_bytes * i);
    }
    for (size_t i = a_degree + 1; i <= b_degree; i++) {
      (*vole_params->flambda_set)(res + lambda_bytes * i, b + lambda_bytes * i);
    }
  } else {
    for (size_t i = 0; i <= b_degree; i++) {
      (*vole_params->flambda_sum)(res + lambda_bytes * i, a + lambda_bytes * i, b + lambda_bytes * i);
    }
    for (size_t i = b_degree + 1; i <= a_degree; i++) {
      (*vole_params->flambda_set)(res + lambda_bytes * i, a + lambda_bytes * i);
    }
  }
}

/**
 * verifier xor gate
 */
EXPORT void verifier_cst_vole_xor_gate_ref(   //
    const vole_parameters* vole_params,       // dimensions
    flambda_t* res_f,                         // [out] poly of degree 1 (cst term over f2)
    const flambda_t* a_f, uint64_t a_degree,  // [in] a
    const flambda_t* b_f, uint64_t b_degree,  // [in] b
    const flambda_t* delta2)                  // [in] verifier's point (unused)
{
  (*vole_params->flambda_sum)(res_f, a_f, b_f);
}

/**
 * prover mul gate
 */
EXPORT void prover_cst_vole_mul_gate_ct_ref(  //
    const vole_parameters* vole_params,       // dimensions
    fpoly_t* res_f,                           // [out] poly of degree 1 (cst term over f2)
    const fpoly_t* a_f, uint64_t a_degree,    // [in] a
    const fpoly_t* b_f, uint64_t b_degree)    // [in] b
{
  uint8_t* res = (uint8_t*)res_f;
  const uint8_t* a = (uint8_t*)a_f;
  const uint8_t* b = (uint8_t*)b_f;
  const size_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT(res != a && res != b, "function does not work in place");
  flambda_max_t tmp;

  // zeroize result
  memset(res, 0, lambda_bytes * (a_degree + b_degree + 1));

  // Schoolbook multiplication
  for (size_t i_a = 0; i_a <= a_degree; i_a++) {
    for (size_t i_b = 0; i_b <= b_degree; i_b++) {
      (*vole_params->flambda_product)(tmp, a + lambda_bytes * i_a, b + lambda_bytes * i_b);
      (*vole_params->flambda_sum)(res + lambda_bytes * (i_a + i_b), res + lambda_bytes * (i_a + i_b), tmp);
    }
  }
}

/**
 * verifier mul gate
 */
EXPORT void verifier_cst_vole_mul_gate_ref(   //
    const vole_parameters* vole_params,       // dimensions
    flambda_t* res_q,                         // [out] poly of degree 1 (cst term over f2)
    const flambda_t* a_q, uint64_t a_degree,  // [in] a
    const flambda_t* b_q, uint64_t b_degree,  // [in] b
    const flambda_t* delta2)                  // [in] verifier's point (unused)
{
  (*vole_params->flambda_product)(res_q, a_q, b_q);
}

/**
 * prover echelon-pow2 gate (degree 1): sum(2^{ki}.a[i])
 */
EXPORT void prover_cst_vole_echelon_pow2_ct_ref(  //
    const vole_parameters* vole_params,           // dimensions
    uint64_t arity, uint64_t k,                   //
    fpoly_t* res_f,                               // [out] res of degree 1
    const fpoly_t* a_f)                           // [in] (arity) a of degree 1
{
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  uint8_t* const res = (uint8_t*)res_f;
  const uint8_t* const a = (uint8_t*)a_f;
  // note: we can speed-up the first one one since the cst term is over f2
  vole_params->flambda_echelon_pow2(k, res, a, arity, 2 * lambda_bytes);
  vole_params->flambda_echelon_pow2(k, res + lambda_bytes, a + lambda_bytes, arity, 2 * lambda_bytes);
}
/**
 * verifier echelon-pow2 gate (degree 1): sum(2^{ki}.a[i])
 */
EXPORT void verifier_cst_vole_echelon_pow2_ref(  //
    const vole_parameters* vole_params,          // dimensions
    uint64_t arity, uint64_t k,                  //
    flambda_t* res_q,                            // [out] res
    const flambda_t* a_q,                        // [in] (arity) a
    const flambda_t* delta2)                     // [in] verifier's point (unused)
{
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  uint8_t* const res = (uint8_t*)res_q;
  const uint8_t* const a = (uint8_t*)a_q;
  vole_params->flambda_echelon_pow2(k, res, a, arity, lambda_bytes);
}

/** @brief number of scratch bytes necessary*/
EXPORT uint64_t prover_cst_check_unitary_gate_ct_ref_tmp_bytes(const vole_parameters* vole_params) {
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t1, 32, 3 * lambda_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t2, 32, 2 * lambda_bytes);
  COMP_SPACE_RETURN();
}

/**
 * prover check unitary gate:
 * res = coeff * (sum a[i].2^{i}) * (sum a[i].2^{ki}) + (sum a[i].2^{(k+1)i}) where k = arity
 */
EXPORT void prover_cst_check_unitary_gate_ct_ref(  //
    const vole_parameters* vole_params,            // dimensions
    uint64_t arity,                                // arity >= 1
    fpoly_t* res_f,                                // [out] res of degree 2
    const fpoly_t* a_f,                            // [in] (arity) a of deg 1
    const flambda_t* coeff,                        // [in] challenge coeff
    uint8_t* tmp_space) {                          // scratch space
  const uint64_t lambda_bytes = vole_params->lambda_bytes;

  TMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t1, 32, 3 * lambda_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t2, 32, 2 * lambda_bytes);

  prover_cst_vole_echelon_pow2_ct_ref(vole_params, arity, 1, tmp_fpoly_t1, a_f);
  prover_cst_vole_echelon_pow2_ct_ref(vole_params, arity - 1, arity, tmp_fpoly_t2, a_f);
  prover_cst_vole_mul_gate_ct_ref(vole_params, res_f, tmp_fpoly_t1, 1, tmp_fpoly_t2, 1);

  prover_cst_vole_echelon_pow2_ct_ref(vole_params, arity - 1, arity + 1, tmp_fpoly_t2, a_f);

  // Add using the xor gate
  prover_cst_vole_xor_gate_ct_ref(vole_params, tmp_fpoly_t1, res_f, 2, tmp_fpoly_t2, 1);

  // Multiply by coeff
  prover_cst_vole_mul_gate_ct_ref(vole_params, res_f, tmp_fpoly_t1, 2, coeff, 0);
}

/** @brief number of scratch bytes necessary*/
EXPORT uint64_t verifier_cst_check_unitary_gate_ref_tmp_bytes(const vole_parameters* vole_params) {
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t1, 32, lambda_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t2, 32, lambda_bytes);
  COMP_SPACE_RETURN();
}

/**
 * verifier check unitary gate:
 * res = coeff * (sum a[i].2^{i}) * (sum a[i].2^{ki}) + (sum a[i].2^{(k+1)i}) where k = arity
 */
EXPORT void verifier_cst_check_unitary_gate_ref(  //
    const vole_parameters* vole_params,           // dimensions
    uint64_t arity,                               // mux arity >= 1
    flambda_t* res_q,                             // [out] res
    const flambda_t* a_q,                         // [in] (arity) a
    const flambda_t* coeff,                       // [in] challenge coeff
    const flambda_t* delta2,                      // [in] verifier's point (unused)
    uint8_t* tmp_space) {                         // scratch space
  const uint64_t lambda_bytes = vole_params->lambda_bytes;

  TMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t1, 32, lambda_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t2, 32, lambda_bytes);

  verifier_cst_vole_echelon_pow2_ref(vole_params, arity, 1, tmp_fpoly_t1, a_q, delta2);
  verifier_cst_vole_echelon_pow2_ref(vole_params, arity - 1, arity, tmp_fpoly_t2, a_q, delta2);
  verifier_cst_vole_mul_gate_ref(vole_params, res_q, tmp_fpoly_t1, 1, tmp_fpoly_t2, 1, delta2);

  verifier_cst_vole_echelon_pow2_ref(vole_params, arity - 1, arity + 1, tmp_fpoly_t2, a_q, delta2);

  // Add using the xor gate
  verifier_cst_vole_xor_gate_ref(vole_params, tmp_fpoly_t1, res_q, 2, tmp_fpoly_t2, 1, delta2);

  // Multiply by coeff
  verifier_cst_vole_mul_gate_ref(vole_params, res_q, tmp_fpoly_t1, 2, coeff, 0, delta2);
}

EXPORT uint64_t prover_cst_vole_qary_mux_gate_ct_ref_tmp_bytes(const vole_parameters* vole_params, uint64_t arity,
                                                               uint64_t in_degree) {
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t1, 32, (in_degree + 2) * lambda_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t2, 32, (in_degree + 2) * lambda_bytes);
  COMP_SPACE_RETURN();
}

/**
 * prover qary-mux gate: res = a[0] + sum(c[i-1] (a[i]-a[0]))
 */
EXPORT void prover_cst_vole_qary_mux_gate_ct_naive(  //
    const vole_parameters* vole_params,              // dimensions
    uint64_t real_arity,                             // mux arity >= 1
    uint64_t in_degree,                              // common input degree
    fpoly_t* res_f,                                  // [out] poly of degree d+1
    const fpoly_t* c_f,                              // [in] (arity-1) control bits of deg. 1
    const fpoly_t* a_f,                              // [in] (real_arity) values of deg d
    uint8_t* tmp_space) {                            // scratch space
  const uint64_t lambda_bytes = vole_params->lambda_bytes;

  const uint8_t* a = (uint8_t*)a_f;
  const uint8_t* c = (uint8_t*)c_f;
  uint64_t a_poly_bytes = (in_degree + 1) * lambda_bytes;
  uint64_t c_poly_bytes = 2 * lambda_bytes;

  TMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t1, 32, (in_degree + 2) * lambda_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, tmp_fpoly_t2, 32, (in_degree + 2) * lambda_bytes);

  uint8_t* res_f_u8 = (uint8_t*)res_f;

  // a[0]
  memcpy(res_f, a, (in_degree + 1) * lambda_bytes);

  // zeroing out the last coefficient
  memset(res_f_u8 + (in_degree + 1) * lambda_bytes, 0, lambda_bytes);

  for (uint64_t i = 1; i < real_arity; i++) {
    prover_cst_vole_xor_gate_ct_ref(vole_params, tmp_fpoly_t1, a + i * a_poly_bytes, in_degree, a, in_degree);

    // multiple the sum by a[i-1]
    prover_cst_vole_mul_gate_ct_ref(vole_params, tmp_fpoly_t2, c + (i - 1) * c_poly_bytes, 1, tmp_fpoly_t1, in_degree);

    // sum the parts together
    prover_cst_vole_xor_gate_ct_ref(vole_params, res_f, tmp_fpoly_t2, in_degree + 1, res_f, in_degree + 1);
  }
}

EXPORT uint64_t verifier_cst_vole_qary_mux_ctrl_bytes(const vole_parameters* vole_params, uint64_t arity) {
  return arity * vole_params->lambda_bytes;
}

/**
 * Rewrite the control values as the extended control vector described in sdith_piop_circuits.h.
 */
EXPORT void verifier_cst_vole_qary_mux_prepare_ctrl(  //
    const vole_parameters* vole_params,               // dimensions
    uint64_t real_arity,                              // mux arity >= 1
    flambda_t* ctrl,                                  // [out] (real_arity) extended control
    const flambda_t* c_q,                             // [in] (real_arity-1) control values
    const flambda_t* delta2) {                        // [in] verifier's point (unused)
  const uint64_t lambda_bytes = vole_params->lambda_bytes;

  const uint8_t* c = (uint8_t*)c_q;
  uint8_t* ctrl_u8 = (uint8_t*)ctrl;

  // ctrl[0] accumulates all the other control values
  (*vole_params->flambda_set)(ctrl_u8, FLAMBDA_ONE);
  for (uint64_t i = 1; i < real_arity; ++i) {
    const uint8_t* ci = c + (i - 1) * lambda_bytes;
    (*vole_params->flambda_set)(ctrl_u8 + i * lambda_bytes, ci);
    (*vole_params->flambda_sum)(ctrl_u8, ctrl_u8, ci);
  }
}

/**
 * verifier qary-mux gate on a prepared control: res = a[0] + sum(c[i-1] (a[i]-a[0]))
 */
EXPORT void verifier_cst_vole_qary_mux_gate_prep(  //
    const vole_parameters* vole_params,            // dimensions
    uint64_t real_arity,                           // mux arity >= 1
    flambda_t* res_q,                              // [out] evaluation of the poly of degree d+1
    const flambda_t* ctrl,                         // [in] (real_arity) prepared control
    const flambda_t* a_q,                          // [in] (real_arity) values of deg d
    const flambda_t* delta2) {                     // [in] verifier's point (unused)
  // res = sum_i ctrl[i] . a[i]: the verifier's values are single field elements, so the inputs
  // of a gate are already the contiguous vector a dot product wants.
  (*vole_params->flambda_dot_product)(res_q, a_q, ctrl, real_arity);
}

/** @brief number of scratch bytes necessary*/
EXPORT uint64_t verifier_cst_vole_qary_mux_gate_ref_tmp_bytes(const vole_parameters* vole_params, uint64_t arity,
                                                              uint64_t in_degree) {
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, ctrl, 32, verifier_cst_vole_qary_mux_ctrl_bytes(vole_params, arity));
  COMP_SPACE_RETURN();
}

/**
 * verifier qary-mux gate: res = a[0] + sum(c[i-1] (a[i]-a[0]))
 */
EXPORT void verifier_cst_vole_qary_mux_gate_ref(  //
    const vole_parameters* vole_params,           // dimensions
    uint64_t real_arity,                          // mux arity >= 1
    uint64_t in_degree,                           // common input degree
    flambda_t* res_q,                             // [out] poly of degree d+1
    const flambda_t* c_q,                         // [in] (arity-1) control bits of deg. 1
    const flambda_t* a_q,                         // [in] (real_arity) values of deg d
    const flambda_t* delta2,                      // [in] verifier's point (unused)
    uint8_t* tmp_space) {                         // scratch space
  TMP_SPACE_MAP_ALIGNED(uint8_t*, ctrl, 32, verifier_cst_vole_qary_mux_ctrl_bytes(vole_params, real_arity));
  verifier_cst_vole_qary_mux_prepare_ctrl(vole_params, real_arity, ctrl, c_q, delta2);
  verifier_cst_vole_qary_mux_gate_prep(vole_params, real_arity, res_q, ctrl, a_q, delta2);
}

EXPORT uint64_t prover_cst_vole_qary_mux_ctrl_bytes(const vole_parameters* vole_params, uint64_t arity) {
  return 2 * arity * vole_params->lambda_bytes;
}

/**
 * Rewrite the control bits as the two extended control vectors described in sdith_piop_circuits.h.
 */
EXPORT void prover_cst_vole_qary_mux_prepare_ctrl(  //
    const vole_parameters* vole_params,             // dimensions
    uint64_t real_arity,                            // mux arity >= 1
    flambda_t* ctrl,                                // [out] (2 x real_arity) extended control
    const fpoly_t* c_f) {                           // [in] (real_arity-1) control bits of deg. 1
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  const uint64_t c_poly_bytes = 2 * lambda_bytes;

  const uint8_t* c = (uint8_t*)c_f;
  uint8_t* ctrl0 = (uint8_t*)ctrl;
  uint8_t* ctrl1 = ctrl0 + real_arity * lambda_bytes;

  // ctrl{0,1}[0] accumulate the coefficients of all the other control bits
  (*vole_params->flambda_set)(ctrl0, FLAMBDA_ONE);
  memset(ctrl1, 0, lambda_bytes);
  for (uint64_t i = 1; i < real_arity; ++i) {
    const uint8_t* ci = c + (i - 1) * c_poly_bytes;
    (*vole_params->flambda_set)(ctrl0 + i * lambda_bytes, ci);
    (*vole_params->flambda_set)(ctrl1 + i * lambda_bytes, ci + lambda_bytes);
    (*vole_params->flambda_sum)(ctrl0, ctrl0, ci);
    (*vole_params->flambda_sum)(ctrl1, ctrl1, ci + lambda_bytes);
  }
}

/**
 * prover qary-mux gate on a prepared control: res = a[0] + sum(c[i-1] (a[i]-a[0]))
 */
EXPORT void prover_cst_vole_qary_mux_gate_ct_f2_prep(  //
    const vole_parameters* vole_params,                // dimensions
    uint64_t real_arity,                               // mux arity >= 1
    uint64_t in_degree,                                // common input degree
    fpoly_t* res_f,                                    // [out] poly of degree d+1
    const flambda_t* ctrl,                             // [in] (2 x real_arity) prepared control
    const fpoly_t* a_f) {                              // [in] (real_arity) values of deg d
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  // input i lives at a + i * a_poly_bytes: for a fixed coefficient j, the inputs are a strided vector
  const uint64_t a_poly_bytes = (in_degree + 1) * lambda_bytes;

  const uint8_t* a = (uint8_t*)a_f;
  const uint8_t* ctrl0 = (uint8_t*)ctrl;
  const uint8_t* ctrl1 = ctrl0 + real_arity * lambda_bytes;
  uint8_t* res_f_u8 = (uint8_t*)res_f;

  memset(res_f_u8, 0, (in_degree + 2) * lambda_bytes);
  for (uint64_t j = 0; j <= in_degree; ++j) {
    // res[j] += sum_i ctrl0[i] . a[i][j]  (a bit-select: the ctrl0[i] are in F2)
    (*vole_params->flambda_dot_product_f2_acc)(res_f_u8 + j * lambda_bytes,  //
                                               a + j * lambda_bytes, a_poly_bytes, ctrl0, real_arity);
    // res[j+1] += sum_i ctrl1[i] . a[i][j]
    (*vole_params->flambda_dot_product_acc)(res_f_u8 + (j + 1) * lambda_bytes,  //
                                            a + j * lambda_bytes, a_poly_bytes, ctrl1, real_arity);
  }
}

EXPORT uint64_t prover_cst_vole_qary_mux_gate_ct_f2_ref_tmp_bytes(const vole_parameters* vole_params,
                                                                  uint64_t arity) {
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, ctrl, 32, prover_cst_vole_qary_mux_ctrl_bytes(vole_params, arity));
  COMP_SPACE_RETURN();
}

/**
 * prover qary-mux gate: res = a[0] + sum(c[i-1] (a[i]-a[0])) using product_f2
 */
EXPORT void prover_cst_vole_qary_mux_gate_ct_f2_ref(  //
    const vole_parameters* vole_params,               // dimensions
    uint64_t real_arity,                              // mux arity >= 1
    uint64_t in_degree,                               // common input degree
    fpoly_t* res_f,                                   // [out] poly of degree d+1
    const fpoly_t* c_f,                               // [in] (arity-1) control bits of deg. 1
    const fpoly_t* a_f,                               // [in] (real_arity) values of deg d
    uint8_t* tmp_space) {                             // scratch space
  TMP_SPACE_MAP_ALIGNED(uint8_t*, ctrl, 32, prover_cst_vole_qary_mux_ctrl_bytes(vole_params, real_arity));
  prover_cst_vole_qary_mux_prepare_ctrl(vole_params, real_arity, ctrl, c_f);
  prover_cst_vole_qary_mux_gate_ct_f2_prep(vole_params, real_arity, in_degree, res_f, ctrl, a_f);
}

void tmp_bytes_init(tmp_bytes_comp_t* tmp_bytes_comp) {
  tmp_bytes_comp->local_size = 0;
  tmp_bytes_comp->last_alignment = 1;
  tmp_bytes_comp->max_size = 0;
}

void tmp_bytes_reserve_local_var(tmp_bytes_comp_t* tmp_bytes_comp, uint64_t var_bytes) {
  tmp_bytes_comp->local_size += var_bytes;
  if (tmp_bytes_comp->local_size > tmp_bytes_comp->max_size) {
    tmp_bytes_comp->max_size = tmp_bytes_comp->local_size;
  }
}

void tmp_bytes_reserve_local_var_aligned(tmp_bytes_comp_t* tmp_bytes_comp, uint64_t var_align, uint64_t var_bytes) {
  if (var_align > tmp_bytes_comp->last_alignment) {
    tmp_bytes_comp->local_size =
        (tmp_bytes_comp->local_size + tmp_bytes_comp->last_alignment - 1) & (uint64_t)(-tmp_bytes_comp->last_alignment);
    tmp_bytes_comp->local_size += var_align - tmp_bytes_comp->last_alignment;
    tmp_bytes_comp->last_alignment = var_align;
  } else {
    tmp_bytes_comp->local_size = (tmp_bytes_comp->local_size + var_align - 1) & (uint64_t)(-var_align);
  }
  tmp_bytes_comp->local_size += var_bytes;
  if (tmp_bytes_comp->local_size > tmp_bytes_comp->max_size) {
    tmp_bytes_comp->max_size = tmp_bytes_comp->local_size;
  }
}

void tmp_bytes_reserve_call(tmp_bytes_comp_t* tmp_bytes_comp, uint64_t call_tmp_bytes) {
  uint64_t ms = tmp_bytes_comp->local_size + call_tmp_bytes;
  if (ms > tmp_bytes_comp->max_size) {
    tmp_bytes_comp->max_size = ms;
  }
}

/**
 * Validate the shape of a mux-circuit and return its largest arity.
 *
 * The layer loops below rely on every layer having at least one output: num_out == 0 would make
 * "num_out - 1" wrap around and the last mux would be applied with a nonsense arity. That cannot
 * happen as soon as num_inputs >= 1 and every arity is >= 1, since ceil(n/arity) >= 1 for n >= 1.
 * The last layer must also collapse to a single output, otherwise the circuit would silently drop
 * the inputs beyond the first mux: iterating the ceil-division is an overflow-free way to check
 * the documented "num_inputs <= product of the arities".
 */
static uint64_t mux_circuit_check_shape(  //
    uint64_t depth,                       // circuit depth
    const uint64_t* arities,              // (depth) arities
    uint64_t num_inputs) {                // number of inputs
  CREQUIRE(depth >= 1, "mux circuit depth must be >= 1");
  CREQUIRE(num_inputs >= 1, "mux circuit must have at least one input");
  uint64_t max_arity = 1;
  uint64_t num_inputs_at_depth = num_inputs;
  for (uint64_t d = 0; d < depth; ++d) {
    CREQUIRE(arities[d] >= 1, "mux arity must be >= 1");
    if (arities[d] > max_arity) max_arity = arities[d];
    num_inputs_at_depth = (num_inputs_at_depth + arities[d] - 1) / arities[d];
  }
  CREQUIRE(num_inputs_at_depth == 1, "mux arities too small for the number of inputs");
  return max_arity;
}

/**
 * scratch space required by prover_cst_mux_circuit_ct_ref
 */
EXPORT uint64_t prover_cst_mux_circuit_ct_ref_tmp_bytes(  //
    const vole_parameters* vole_params,                   // dimensions
    uint64_t depth,                                       // circuit depth
    const uint64_t* arities,                              // (depth) arities (preferably in decreasing order)
    uint64_t num_inputs) {                                // scratch space

  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  // tmp space for calculations of check_unitary_gate: 5 * lambda_bytes
  // tmp space to hold intermediate values for check_unitary_gate: 3 * lambda_bytes;
  // tmp space for calculations of qary_mux_gate: 2 * (depth + 2)
  // tmp space to hold 2 intermediate values for qary_mux_gate: 2*3*max_inputs*lambda_bytes
  // tmp space for the prepared control of a layer (full and last, incomplete, mux)
  const uint64_t max_arity = mux_circuit_check_shape(depth, arities, num_inputs);
  uint64_t max_inputs = ((num_inputs + arities[0] - 1) / arities[0]);

  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, intermediate_unitary_values, 32, 3 * lambda_bytes);  // 3*lambda_bytes;
  COMP_SPACE_MAP_ALIGNED(uint8_t*, intermediate_unitary_quary1, 32, 3 * max_inputs * lambda_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, intermediate_unitary_quary2, 32, 3 * max_inputs * lambda_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, mux_ctrl, 32, prover_cst_vole_qary_mux_ctrl_bytes(vole_params, max_arity));
  COMP_SPACE_MAP_ALIGNED(uint8_t*, mux_ctrl_last, 32, prover_cst_vole_qary_mux_ctrl_bytes(vole_params, max_arity));
  COMP_SPACE_MAP_CALL(prover_cst_check_unitary_gate_ct_ref_tmp_bytes(vole_params));
  COMP_SPACE_RETURN();
}

const flambda_max_t FLAMBDA_2P32 = {UINT64_C(1) << 32, 0, 0, 0};

/**
 * prover mux-circuit:
 */
EXPORT void prover_cst_mux_circuit_ct_ref(  //
    const vole_parameters* vole_params,     // dimensions
    uint64_t depth,                         // circuit depth
    const uint64_t* arities,                // (depth) arities (preferably in decreasing order)
    uint64_t num_inputs,                    // number of inputs (<= product of arities)
    fpoly_t* res_f,                         // [out] res of degree depth
    const fpoly_t* c_f,                     // [in] (sum (arity[i]-1)) ctrl bits of deg 1
    const flambda_t* a,                     // [in] challenge input values
    const flambda_t* chall_c,               // [in] challenge value for unitary tests
    uint8_t* tmp_space) {                   // [tmp] scratch space
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  const uint64_t max_arity = mux_circuit_check_shape(depth, arities, num_inputs);
  uint64_t max_inputs = ((num_inputs + arities[0] - 1) / arities[0]);
  CASSERT_ALIGNMENT(res_f, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(c_f, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(a, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(chall_c, alignment_of(lambda_bytes));

  TMP_SPACE_MAP_ALIGNED(uint8_t*, intermediate_unitary_values, 32, 3 * lambda_bytes);  // 3*lambda_bytes;
  TMP_SPACE_MAP_ALIGNED(uint8_t*, intermediate_unitary_quary1, 32, 3 * max_inputs * lambda_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, intermediate_unitary_quary2, 32, 3 * max_inputs * lambda_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, mux_ctrl, 32, prover_cst_vole_qary_mux_ctrl_bytes(vole_params, max_arity));
  TMP_SPACE_MAP_ALIGNED(uint8_t*, mux_ctrl_last, 32, prover_cst_vole_qary_mux_ctrl_bytes(vole_params, max_arity));
  TMP_SPACE_MAP_CALL(prover_cst_check_unitary_gate_ct_ref_tmp_bytes(vole_params));

  uint8_t* c = (uint8_t*)c_f;
  uint64_t c_bytes = 2 * lambda_bytes;
  flambda_max_t unary_cur_chall;
  uint64_t unary_next_chall_power = 0;

  // tmp
  uint8_t* res_f_u8 = (uint8_t*)res_f;
  memset(res_f_u8, 0, (depth + 1) * lambda_bytes);

  // correct for check_unitary_sum
  uint64_t c_pos = 0;
  uint64_t num_inputs_at_depth = num_inputs;
  uint8_t* a_in = (uint8_t*)a;
  uint8_t* output_qary = intermediate_unitary_quary1;

  for (uint64_t d = 0; d < depth; d++) {
    if (arities[d] > 2) {
      // prepare the next unary challenge coefficient
      if (unary_next_chall_power == 0) {
        vole_params->flambda_set(unary_cur_chall, chall_c);
      } else {
        CASSERT(unary_next_chall_power <= vole_params->LAMBDA - 32, "unary challenge overflow (bad parameter set)");
        vole_params->flambda_product(unary_cur_chall, unary_cur_chall, FLAMBDA_2P32);
      }
      unary_next_chall_power += 32;
      // apply the check
      prover_cst_check_unitary_gate_ct_ref(vole_params, arities[d] - 1, intermediate_unitary_values,
                                           c + c_pos * c_bytes, unary_cur_chall, tmp_space);

      prover_cst_vole_xor_gate_ct_ref(vole_params, res_f, res_f, depth, intermediate_unitary_values, 2);
      // prepare the next challenge point
    }

    const uint64_t num_out = (num_inputs_at_depth + arities[d] - 1) / arities[d];
    uint64_t unitary_qary_in_size = (d + 1) * lambda_bytes;
    uint64_t unitary_qary_out_size = (d + 2) * lambda_bytes;

    // every mux of the layer shares the same control bits: prepare it once
    prover_cst_vole_qary_mux_prepare_ctrl(vole_params, arities[d], mux_ctrl, c + c_pos * c_bytes);

    for (uint64_t j = 0; j + 1 < num_out; j++) {
      prover_cst_vole_qary_mux_gate_ct_f2_prep(vole_params, arities[d], d,                //
                                               output_qary + j * unitary_qary_out_size,   //
                                               mux_ctrl,                                  //
                                               a_in + j * arities[d] * unitary_qary_in_size);
    }

    // the last mux can be incomplete (fewer inputs => a control of its own)
    {
      uint64_t j = num_out - 1;  // num_out >= 1: guaranteed by mux_circuit_check_shape
      uint64_t last_arity = num_inputs_at_depth - j * arities[d];
      CASSERT(last_arity >= 1 && last_arity <= arities[d], "bad last mux arity");
      prover_cst_vole_qary_mux_prepare_ctrl(vole_params, last_arity, mux_ctrl_last, c + c_pos * c_bytes);
      prover_cst_vole_qary_mux_gate_ct_f2_prep(vole_params, last_arity, d,                //
                                               output_qary + j * unitary_qary_out_size,   //
                                               mux_ctrl_last,                             //
                                               a_in + j * arities[d] * unitary_qary_in_size);
    }

    // return final value
    if (d == depth - 1) {
      prover_cst_vole_xor_gate_ct_ref(vole_params, res_f, res_f, depth, output_qary, d + 1);
    }

    c_pos += arities[d] - 1;
    num_inputs_at_depth = num_out;
    if (d % 2 == 0) {
      a_in = intermediate_unitary_quary1;
      output_qary = intermediate_unitary_quary2;
    } else {
      a_in = intermediate_unitary_quary2;
      output_qary = intermediate_unitary_quary1;
    }
  }
}

/**
 * scratch space required by verifier_cst_mux_circuit_ref
 */
EXPORT uint64_t verifier_cst_mux_circuit_ref_tmp_bytes(  //
    const vole_parameters* vole_params,                  // dimensions
    uint64_t depth,                                      // circuit depth
    const uint64_t* arities,                             // (depth) arities (preferably in decreasing order)
    uint64_t num_inputs) {                               // scratch space
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  // tmp space for calculations: 2 * lambda_bytes
  // tmp space to hold intermediate values for check_unitary_gate: 2*lambda_bytes bytes
  // tmp space to hold two intermediate values for qary_mux_gate: 2*3*max_inputs*lambda_bytes
  // tmp space for the prepared control of a layer (full and last, incomplete, mux)
  const uint64_t max_arity = mux_circuit_check_shape(depth, arities, num_inputs);
  const uint64_t max_inputs = ((num_inputs + arities[0] - 1) / arities[0]);
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint8_t*, unitary_chk, 32, lambda_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, in_out_even, 32, max_inputs* lambda_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, in_out_odd, 32, max_inputs* lambda_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, mux_ctrl, 32, verifier_cst_vole_qary_mux_ctrl_bytes(vole_params, max_arity));
  COMP_SPACE_MAP_ALIGNED(uint8_t*, mux_ctrl_last, 32, verifier_cst_vole_qary_mux_ctrl_bytes(vole_params, max_arity));
  COMP_SPACE_MAP_CALL(verifier_cst_check_unitary_gate_ref_tmp_bytes(vole_params));  // unitary chk
  COMP_SPACE_RETURN();
}

/**
 * verifier mux-circuit:
 */
EXPORT void verifier_cst_mux_circuit_ref(  //
    const vole_parameters* vole_params,    // dimensions
    uint64_t depth,                        // circuit depth
    const uint64_t* arities,               // (depth) arities (preferably in decreasing order)
    uint64_t num_inputs,                   // number of inputs (<= product of arities)
    flambda_t* res_q,                      // [out] res of degree depth
    const flambda_t* c_q,                  // [in] (sum (arity[i]-1)) ctrl bits of deg 1
    const flambda_t* a,                    // [in] challenge input values
    const flambda_t* chall_c,              // [in] challenge value for unitary tests
    const flambda_t* delta2,               // [in] verifier's point (unused)
    uint8_t* tmp_space) {                  // [tmp] scratch space
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  CASSERT_ALIGNMENT(res_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(c_q, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(a, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(chall_c, alignment_of(lambda_bytes));
  CASSERT_ALIGNMENT(delta2, alignment_of(lambda_bytes));

  const uint64_t max_arity = mux_circuit_check_shape(depth, arities, num_inputs);
  const uint64_t max_inputs = ((num_inputs + arities[0] - 1) / arities[0]);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, unitary_chk, 32, lambda_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, in_out_even, 32, max_inputs* lambda_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, in_out_odd, 32, max_inputs* lambda_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, mux_ctrl, 32, verifier_cst_vole_qary_mux_ctrl_bytes(vole_params, max_arity));
  TMP_SPACE_MAP_ALIGNED(uint8_t*, mux_ctrl_last, 32, verifier_cst_vole_qary_mux_ctrl_bytes(vole_params, max_arity));
  TMP_SPACE_MAP_CALL(verifier_cst_check_unitary_gate_ref_tmp_bytes(vole_params));  // unitary chk

  uint8_t* c = (uint8_t*)c_q;
  uint64_t c_bytes = lambda_bytes;
  flambda_max_t unary_cur_chall;
  uint64_t unary_next_chall_power = 0;

  // tmp
  uint8_t* res_q_u8 = (uint8_t*)res_q;
  memset(res_q_u8, 0, lambda_bytes);

  uint64_t c_pos = 0;
  uint64_t num_inputs_at_depth = num_inputs;
  uint8_t* a_in = (uint8_t*)a;
  uint8_t* output_qary = in_out_even;
  uint64_t unitary_qary_in_size = lambda_bytes;
  uint64_t unitary_qary_out_size = lambda_bytes;
  for (uint64_t d = 0; d < depth; d++) {
    if (arities[d] > 2) {
      // prepare the next unary challenge coefficient
      if (unary_next_chall_power == 0) {
        vole_params->flambda_set(unary_cur_chall, chall_c);
      } else {
        CASSERT(unary_next_chall_power <= vole_params->LAMBDA - 32, "unary challenge overflow (bad parameter set)");
        vole_params->flambda_product(unary_cur_chall, unary_cur_chall, FLAMBDA_2P32);
      }
      unary_next_chall_power += 32;
      // apply the check
      verifier_cst_check_unitary_gate_ref(vole_params, arities[d] - 1,  //
                                          unitary_chk,                  //
                                          c + c_pos * c_bytes,          //
                                          unary_cur_chall, delta2, tmp_space);
      verifier_cst_vole_xor_gate_ref(vole_params, res_q, res_q, depth, unitary_chk, 2, delta2);
    }

    const uint64_t num_out = (num_inputs_at_depth + arities[d] - 1) / arities[d];

    // every mux of the layer shares the same control values: prepare it once
    verifier_cst_vole_qary_mux_prepare_ctrl(vole_params, arities[d], mux_ctrl, c + c_pos * c_bytes, delta2);

    for (uint64_t j = 0; j + 1 < num_out; j++) {
      verifier_cst_vole_qary_mux_gate_prep(vole_params, arities[d],                       //
                                           output_qary + j * unitary_qary_out_size,       //
                                           mux_ctrl,                                      //
                                           a_in + j * arities[d] * unitary_qary_in_size,  //
                                           delta2);
    }

    // the last mux can be incomplete (fewer inputs => a control of its own)
    {
      uint64_t j = num_out - 1;  // num_out >= 1: guaranteed by mux_circuit_check_shape
      uint64_t last_arity = num_inputs_at_depth - j * arities[d];
      CASSERT(last_arity >= 1 && last_arity <= arities[d], "bad last mux arity");
      verifier_cst_vole_qary_mux_prepare_ctrl(vole_params, last_arity, mux_ctrl_last, c + c_pos * c_bytes, delta2);
      verifier_cst_vole_qary_mux_gate_prep(vole_params, last_arity,                        //
                                           output_qary + j * unitary_qary_out_size,       //
                                           mux_ctrl_last,                                 //
                                           a_in + j * arities[d] * unitary_qary_in_size,  //
                                           delta2);
    }

    // return final value
    if (d == depth - 1) {
      verifier_cst_vole_xor_gate_ref(vole_params, res_q, res_q, depth, output_qary, d + 1, delta2);
    }

    c_pos += arities[d] - 1;
    num_inputs_at_depth = num_out;

    if (d % 2 == 0) {
      a_in = in_out_even;
      output_qary = in_out_odd;
    } else {
      a_in = in_out_odd;
      output_qary = in_out_even;
    }
  }
}
