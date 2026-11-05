#ifndef SDITH_VOLE_TO_PIOP_H
#define SDITH_VOLE_TO_PIOP_H

// This header contains the functions used to convert vole pairs into piop pairs
// For each step of the conversion, there is a prover side and a verifier's side function.

#include "vole_parameters.h"

/**
 * Recombine in_L = lambda. out_L full-size std vole pairs over F2
 * To form out_L full-size std vole F2^LAMBDA
 */
EXPORT void prover_f2_to_flambda_deg1_std_vole_ct_ref(  //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t num_pairs,                                 // number of output pairs to produce
    fpoly_t* out_f,                                     // [out] deg-1 fpoly[num_pairs]
    const bitvec_t* in_u,                               // [in] in_L bits  where in_L = num_pairs * LAMBDA
    const flambda_t* in_v);                             // [in] flambda[in_L]

/**
 * Recombine in_L = lambda. out_L full-size std vole pairs over F2
 * To form out_L full-size std vole F2^LAMBDA
 */
EXPORT void verifier_f2_to_flambda_deg1_std_vole_ref(  //
    const vole_parameters* vole_params,                // dimensions
    uint64_t num_pairs,                                // number of output pairs to produce
    flambda_t* out_q,                                  // [out] gflambda[num_pairs]
    const flambda_t* in_q);                            // [in] gflambda in_q[in_L]  where in_L = num_pairs * LAMBDA

/**
 * @brief convert packed degree 1 std-vole to packed cst-vole
 * (Prover side is just the identity function)
 */
EXPORT void prover_f2_std_to_cst_vole_ct_ref(      //
    const vole_parameters* vole_params,            // dimensions
    uint64_t num_pairs,                            // num_pairs to process
    bitvec_t* out_u, flambda_t* out_v,             // [out] bit[num_pairs], flambda[num_pairs]
    const bitvec_t* in_u, const flambda_t* in_v);  // [in] bit[num_pairs], flambda[num_pairs]

/**
 * @brief convert packed degree 1 std-vole to packed cst-vole
 */
EXPORT void verifier_f2_std_to_cst_vole_ref(  //
    const vole_parameters* vole_params,       // dimensions
    uint64_t num_pairs,                       // num_pairs to process
    flambda_t* out_q,                         // [in] gflambda[num_pairs]
    const flambda_t* in_q,                    // [in] gflambda[num_pairs]
    const flambda_t* delta2);                 // [in] verifier's point

/**
 * convert d degree 1 std-vole over flambda to one degree-d vole
 */
EXPORT void prover_flambda_deg1_to_degd_vole_ref(  //
    const vole_parameters* vole_params,            // dimensions
    uint64_t out_degree,                           // degree of the output pair
    fpoly_t* out_f,                                // single poly_t of degree out_degree
    const fpoly_t* in_f);                          // out_degree poly[out_degree]

/**
 * convert d degree 1 std-vole to one degree-d vole
 */
EXPORT void verifier_flambda_deg1_to_degd_vole_ref(  //
    const vole_parameters* vole_params,              // dimensions
    uint64_t out_degree,                             // degree of the output pair
    flambda_t* out_q,                                // [out] single gflambda
    const flambda_t* in_q,                           // [in] gflambda[out_deg]
    const flambda_t* delta_powers);                  // verifier's point powers

/**
 * convert a degree-d vole from std to cst
 */
EXPORT void prover_flambda_degd_std_to_cst_vole_ct_ref(  //
    const vole_parameters* vole_params,                  // dimensions
    uint64_t degree,                                     // degree d of of the pair to process
    fpoly_t* out_f,                                      // [out] single poly of degree d
    const fpoly_t* in_f);                                // [in] single poly of degree d

/**
 * convert a degree-d vole from std to cst
 */
EXPORT void verifier_flambda_degd_std_to_cst_vole_ref(  //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t degree,                                    // output degree
    flambda_t* out_q,                                   // [out] single evaluation
    const flambda_t* in_q,                              // [in] single evaluation
    const flambda_t* delta_pow_d);                      // [in] verifier's point to the power d

#endif  // SDITH_VOLE_TO_PIOP_H
