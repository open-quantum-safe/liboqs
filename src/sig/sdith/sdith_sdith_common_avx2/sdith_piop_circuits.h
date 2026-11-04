#ifndef SDITH_PIOP_CIRCUITS_H
#define SDITH_PIOP_CIRCUITS_H

#include "vole_parameters.h"

// vole circuit gates

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
    const flambda_t* in_rvp_v);                          // [in] packed v terms

/**
 * verifier packed secret input gate (degree 1)
 */
EXPORT void verifier_cst_vole_packed_secret_input_ref(  //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t num_inputs,                                // number of inputs to process
    fpoly_t* out_q,                                     // [out] vole evaluations
    const bitvec_t* in_pub,                             // [in] publications (packed over f2)
    const flambda_t* in_rvp_q,                          // [in] packed v terms
    const flambda_t* delta2);                           // [in] verifier's point

/**
 * prover check-zero gate (degree d)
 */
EXPORT void prover_cst_vole_check_zero_gate_ct_ref(  //
    const vole_parameters* vole_params,              // dimensions
    uint64_t degree,                                 // input degree
    fpoly_t* out_pub,                                // [out] output publication (degree d-1)
    const fpoly_t* in_f,                             // [in] input (degree d)
    const fpoly_t* in_rvp_f);                        // [in] rvp (degree d-1)

/**
 * verifier check-zero gate (degree d)
 */
EXPORT uint8_t verifier_cst_vole_check_zero_gate_ref(  //
    const vole_parameters* vole_params,                // dimensions
    uint64_t degree,                                   // input degree
    flambda_t* out_value,                              // [out] deduced output value
    const fpoly_t* in_pub,                             // [in] publication (degree d-1)
    const flambda_t* in_q,                             // [in] input (degree d)
    const flambda_t* in_rvp_q,                         // [in] rvp (degree d-1)
    const flambda_t* delta2);                          // [in] verifier's point

/**
 * prover xor gate
 */
EXPORT void prover_cst_vole_xor_gate_ct_ref(  //
    const vole_parameters* vole_params,       // dimensions
    fpoly_t* res_f,                           // [out] poly of degree 1 (cst term over f2)
    const fpoly_t* a_f, uint64_t a_degree,    // [in] a
    const fpoly_t* b_f, uint64_t b_degree);   // [in] b

/**
 * verifier xor gate
 */
EXPORT void verifier_cst_vole_xor_gate_ref(   //
    const vole_parameters* vole_params,       // dimensions
    flambda_t* res_f,                         // [out] poly of degree 1 (cst term over f2)
    const flambda_t* a_f, uint64_t a_degree,  // [in] a
    const flambda_t* b_f, uint64_t b_degree,  // [in] b
    const flambda_t* delta2);                 // [in] verifier's point (unused)

/**
 * prover mul gate
 */
EXPORT void prover_cst_vole_mul_gate_ct_ref(  //
    const vole_parameters* vole_params,       // dimensions
    fpoly_t* res_f,                           // [out] poly of degree 1 (cst term over f2)
    const fpoly_t* a_f, uint64_t a_degree,    // [in] a
    const fpoly_t* b_f, uint64_t b_degree);   // [in] b

/**
 * verifier mul gate
 */
EXPORT void verifier_cst_vole_mul_gate_ref(   //
    const vole_parameters* vole_params,       // dimensions
    flambda_t* res_q,                         // [out] poly of degree 1 (cst term over f2)
    const flambda_t* a_q, uint64_t a_degree,  // [in] a
    const flambda_t* b_q, uint64_t b_degree,  // [in] b
    const flambda_t* delta2);                 // [in] verifier's point (unused)

/**
 * prover echelon-pow2 gate (degree 1): sum(2^{ki}.a[i])
 */
EXPORT void prover_cst_vole_echelon_pow2_ct_ref(  //
    const vole_parameters* vole_params,           // dimensions
    uint64_t arity, uint64_t k,                   //
    fpoly_t* res_f,                               // [out] res of degree 1
    const fpoly_t* a_f);                          // [in] (arity) a of degree 1

/**
 * verifier echelon-pow2 gate (degree 1): sum(2^{ki}.a[i])
 */
EXPORT void verifier_cst_vole_echelon_pow2_ref(  //
    const vole_parameters* vole_params,          // dimensions
    uint64_t arity, uint64_t k,                  //
    flambda_t* res_q,                            // [out] res
    const flambda_t* a_q,                        // [in] (arity) a
    const flambda_t* delta2);                    // [in] verifier's point (unused)

/** @brief number of scratch bytes necessary for prover_cst_vole_qary_mux_gate_ct_ref */
EXPORT uint64_t prover_cst_vole_qary_mux_gate_ct_ref_tmp_bytes(  //
    const vole_parameters* vole_params,                          // dimensions
    uint64_t arity,                                              // mux arity >= 1
    uint64_t in_degree                                           // common input degree
);

/**
 * prover qary-mux gate: res = a[0] + sum(c[i-1] (a[i]-a[0]))
 * KEPT ONLY FOR TEST PURPOSES: since c is binary, use prover_cst_vole_qary_mux_gate_ct_f2_ref
 */
EXPORT void prover_cst_vole_qary_mux_gate_ct_naive(  //
    const vole_parameters* vole_params,            // dimensions
    uint64_t arity,                                // mux arity >= 1
    uint64_t in_degree,                            // common input degree
    fpoly_t* res_f,                                // [out] poly of degree d+1
    const fpoly_t* c_f,                            // [in] (arity-1) control bits of deg. 1
    const fpoly_t* a_f,                            // [in] (arity) values of deg d
    uint8_t* tmp_space);                           // scratch space

/** @brief bytes of the prepared control of a qary-mux gate of the given arity */
EXPORT uint64_t prover_cst_vole_qary_mux_ctrl_bytes(  //
    const vole_parameters* vole_params,               // dimensions
    uint64_t arity);                                  // mux arity >= 1

/**
 * Rewrite the control bits of a qary-mux gate as two "extended control" vectors, so that the
 * gate becomes a pair of plain dot products over its inputs (see below):
 *   res = a[0] + sum_{i>0} c[i-1] (a[i]-a[0])
 *       = sum_{i>=0} ctrl0[i] a[i]  +  X . sum_{i>=0} ctrl1[i] a[i]
 * with, writing c[i-1] = c0[i] + c1[i].X for i > 0,
 *   ctrl0[i] = c0[i],  ctrl1[i] = c1[i]                              (i > 0)
 *   ctrl0[0] = 1 + sum_{i>0} c0[i],  ctrl1[0] = sum_{i>0} c1[i]      (the "- a[0]" of every term)
 * The ctrl0[i] stay in F2 (the c0[i] are bits), so the first dot product is a bit-select.
 * The control is shared by every gate of a mux-circuit layer: prepare it once per layer.
 * ctrl is the concatenation of ctrl0 (arity elements) and ctrl1 (arity elements).
 */
EXPORT void prover_cst_vole_qary_mux_prepare_ctrl(  //
    const vole_parameters* vole_params,             // dimensions
    uint64_t arity,                                 // mux arity >= 1
    flambda_t* ctrl,                                // [out] (2 x arity) extended control
    const fpoly_t* c_f);                            // [in] (arity-1) control bits of deg. 1

/**
 * prover qary-mux gate on a prepared control: res = a[0] + sum(c[i-1] (a[i]-a[0]))
 */
EXPORT void prover_cst_vole_qary_mux_gate_ct_f2_prep(  //
    const vole_parameters* vole_params,                // dimensions
    uint64_t arity,                                    // mux arity >= 1
    uint64_t in_degree,                                // common input degree
    fpoly_t* res_f,                                    // [out] poly of degree d+1
    const flambda_t* ctrl,                             // [in] (2 x arity) prepared control
    const fpoly_t* a_f);                               // [in] (arity) values of deg d

/** @brief number of scratch bytes necessary for prover_cst_vole_qary_mux_gate_ct_f2_ref */
EXPORT uint64_t prover_cst_vole_qary_mux_gate_ct_f2_ref_tmp_bytes(  //
    const vole_parameters* vole_params,                             // dimensions
    uint64_t arity);                                                // mux arity >= 1

/**
 * prover qary-mux gate: res = a[0] + sum(c[i-1] (a[i]-a[0])) using product_f2
 * One-shot form: it derives the control into tmp_space on every call. A caller that applies
 * several gates sharing the same c_f (a mux-circuit layer) should instead prepare the control
 * once with prover_cst_vole_qary_mux_prepare_ctrl and call ..._f2_prep.
 */
EXPORT void prover_cst_vole_qary_mux_gate_ct_f2_ref(  //
    const vole_parameters* vole_params,               // dimensions
    uint64_t arity,                                   // mux arity >= 1
    uint64_t in_degree,                               // common input degree
    fpoly_t* res_f,                                   // [out] poly of degree d+1
    const fpoly_t* c_f,                               // [in] (arity-1) control bits of deg. 1
    const fpoly_t* a_f,                               // [in] (arity) values of deg d
    uint8_t* tmp_space);                              // scratch space

/** @brief bytes of the prepared control of a verifier qary-mux gate of the given arity */
EXPORT uint64_t verifier_cst_vole_qary_mux_ctrl_bytes(  //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t arity);                                    // mux arity >= 1

/**
 * Rewrite the control values of a qary-mux gate as one "extended control" vector, so that the
 * gate becomes a plain dot product over its inputs:
 *   res = a[0] + sum_{i>0} c[i-1] (a[i]-a[0]) = sum_{i>=0} ctrl[i] a[i]
 * with ctrl[i] = c[i-1] for i > 0, and the i == 0 entry absorbing the "- a[0]" of every term:
 *   ctrl[0] = 1 + sum_{i>0} c[i-1].
 * This is the verifier counterpart of prover_cst_vole_qary_mux_prepare_ctrl: everything here is
 * an evaluation at delta2, so there is a single vector (no degree-0/degree-1 split) and no F2
 * fast path (see the note below).
 * The control is shared by every gate of a mux-circuit layer: prepare it once per layer.
 */
EXPORT void verifier_cst_vole_qary_mux_prepare_ctrl(  //
    const vole_parameters* vole_params,               // dimensions
    uint64_t arity,                                   // mux arity >= 1
    flambda_t* ctrl,                                  // [out] (arity) extended control
    const flambda_t* c_q,                             // [in] (arity-1) control values
    const flambda_t* delta2);                         // [in] verifier's point (unused)

/**
 * verifier qary-mux gate on a prepared control: res = a[0] + sum(c[i-1] (a[i]-a[0]))
 */
EXPORT void verifier_cst_vole_qary_mux_gate_prep(  //
    const vole_parameters* vole_params,            // dimensions
    uint64_t arity,                                // mux arity >= 1
    flambda_t* res_q,                              // [out] evaluation of the poly of degree d+1
    const flambda_t* ctrl,                         // [in] (arity) prepared control
    const flambda_t* a_q,                          // [in] (arity) values of deg d
    const flambda_t* delta2);                      // [in] verifier's point (unused)

/** @brief number of scratch bytes necessary*/
EXPORT uint64_t verifier_cst_vole_qary_mux_gate_ref_tmp_bytes(  //
    const vole_parameters* vole_params,                         // dimensions
    uint64_t arity,                                             // mux arity >= 1
    uint64_t in_degree                                          // common input degree
);

/**
 * verifier qary-mux gate: res = a[0] + sum(c[i-1] (a[i]-a[0]))
 * One-shot form: it derives the control into tmp_space on every call. A caller that applies
 * several gates sharing the same c_q (a mux-circuit layer) should instead prepare the control
 * once with verifier_cst_vole_qary_mux_prepare_ctrl and call ..._gate_prep.
 */
EXPORT void verifier_cst_vole_qary_mux_gate_ref(  //
    const vole_parameters* vole_params,           // dimensions
    uint64_t arity,                               // mux arity >= 1
    uint64_t in_degree,                           // common input degree
    flambda_t* res_q,                             // [out] poly of degree d+1
    const flambda_t* c_q,                         // [in] (arity-1) control bits of deg. 1
    const flambda_t* a_q,                         // [in] (arity) values of deg d
    const flambda_t* delta2,                      // [in] verifier's point (unused)
    uint8_t* tmp_space);                          // scratch space

/*
 * There is no verifier-side F2-optimized mux gate. The prover's product_f2 fast
 * path (prover_cst_vole_qary_mux_gate_ct_f2_ref) is only valid because the
 * control bits are plaintext over F2 there (they are the constant term of u), so
 * c*(a[i]-a[0]) reduces to a bit-select. On the verifier side the control bits
 * are full-q values evaluated at delta2, so the mux gate must use full q-products
 * throughout: the extended-control rewriting above applies to both sides, but the
 * verifier's two dot products collapse into one and neither of them is over F2.
 */

/**
 * scratch bytes required by prover_cst_check_unitary_gate_ct_ref
 */
EXPORT uint64_t prover_cst_check_unitary_gate_ct_ref_tmp_bytes(const vole_parameters* vole_params);

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
    uint8_t* tmp_space);                           // scratch space

/**
 * scratch bytes required by verifier_cst_check_unitary_gate_ref
 */
EXPORT uint64_t verifier_cst_check_unitary_gate_ref_tmp_bytes(const vole_parameters* vole_params);

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
    uint8_t* tmp_space);                          // scratch space

/**
 * scratch space required by prover_cst_mux_circuit_ct_ref
 */
EXPORT uint64_t prover_cst_mux_circuit_ct_ref_tmp_bytes(  //
    const vole_parameters* vole_params,                   // dimensions
    uint64_t depth,                                       // circuit depth
    const uint64_t* arities,                              // (depth) arities (preferably in decreasing order)
    uint64_t num_inputs);                                 // scratch space

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
    const flambda_t* chall_c,               // [in] challenge coeff for unitary tests (unused if all muxes are binary)
    uint8_t* tmp_space);                    // [tmp] scratch space

/**
 * scratch space required by verifier_cst_mux_circuit_ref
 */
EXPORT uint64_t verifier_cst_mux_circuit_ref_tmp_bytes(  //
    const vole_parameters* vole_params,                  // dimensions
    uint64_t depth,                                      // circuit depth
    const uint64_t* arities,                             // (depth) arities (preferably in decreasing order)
    uint64_t num_inputs);                                // scratch space

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
    const flambda_t* chall_c,              // [in] challenge coeff for unitary tests (unused if all muxes are binary)
    const flambda_t* delta2,               // [in] verifier's point (unused)
    uint8_t* tmp_space);                   // [tmp] scratch space

#endif  // SDITH_PIOP_CIRCUITS_H
