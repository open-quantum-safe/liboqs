#ifndef VOLE_GENERATION_H
#define VOLE_GENERATION_H

/// This header declares the functions needed to generate vole pairs securely

#include "vole_parameters.h"

/** @brief converts delta0 to delta1
 * delta0 has kappa*tau bits, encoding tau grey code encodings
 * delta1 has lambda bits, only the lowest kappa*tau are used, the rest is zero.
 */
EXPORT void delta1_from_delta0_ref(const vole_parameters* vole_params, flambda_t* delta1, const bitvec_t* delta0);

/** @brief converts delta1 to delta2
 * delta2 is the field inverse of delta1
 */
EXPORT void delta2_from_delta1_ref(const vole_parameters* vole_params, flambda_t* delta2, const flambda_t* delta1);

EXPORT uint64_t prover_generate_midsize_grey_vole_from_seeds_bfs_ct_tmp_bytes(  //
    const vole_parameters* vole_params,                               // dimensions
    uint64_t L                   // number of pairs to generate [Lslice next mult of 32]
);

/**
 * Compute the tweaked base salts that are used in GGM seed-expand, seed-commit, and vole-rng
 */
EXPORT void compute_tweaked_salts( //
    const vole_parameters* vole_params, // params
    uint64_t* ggm_masked_salt,          // [out] masked salt for the GGM tree
    uint64_t* vole_masked_salt,         // [out] masked salt for the vole RNG (if not nullptr)
    const salt_t* global_salt           // [in]
);

/**
 * tmp space needed to open the sibling path that corresponds to the provided hidden leaves indexes
 */
EXPORT uint64_t full_ggm_tree_open_sibling_path_from_root_tmp_bytes(   //
    const vole_parameters* vole_params, //
    const uint64_t sibling_path_size    // number of sibling seeds
    );

/**
 * Opens the sibling path that corresponds to the provided hidden leaves indexes
 */
EXPORT void full_ggm_tree_open_sibling_path_from_root(   //
    const vole_parameters* vole_params, //
    seed_t* sibling_seeds,              // sibling seeds (in decreasing node index order)
    commit_t* hidden_leaves_commits,    // hidden leaves commits (in increasing leaf order)
    const seed_t* root_seed,            // the ggm root seed
    const salt_t* global_salt,          // the global salt
    const uint32_t* hidden_leaves_idx,  // index of hidden leaves (node index)
    const uint64_t sibling_path_size,   // number of sibling seeds
    uint8_t* tmp_space);

/**
 * Generate L vole pairs over (F2,F2^KAPPA) (grey code + BFS version)
 * @param commits_hash (output): 2.lambda bytes:
 *        H(HASH_BAVC_PREFIX, global_salt, per-rep subhashes)
 * @param u (output): L bits:  Xor of all ri's,
 * @param v (output): matrix KAPPA x L bits: Xor of ri.grey(i)
 */
EXPORT void prover_generate_midsize_grey_vole_from_seeds_bfs_ct_ref(  //
    const vole_parameters* vole_params,                               // dimensions
    uint64_t L,                   // number of pairs to generate [Lslice next mult of 32]
    hash_t* commits_hash,         // [out] hash of the leaves commits (2.lambda bytes)
    bitmat_t* u,                  // [out] TAU x (L bits on Lslice bytes)
    bitmat_t* v,                  // [out] TAU x KAPPA x (L bits on Lslice bytes) row major
    const salt_t* ggm_salt,       // [in] global salt for the GGM tree
    const seed_t* ggm_root_seed,  // [in] root seed for the GGM tree
    uint8_t* tmp_space);          // [tmp] scratch space


/**
 * Open L vole pairs over (F2,F2^KAPPA) (grey code version) -- tmp bytes needed
 */
EXPORT uint64_t verifier_open_midsize_grey_vole_from_seeds_bfs_tmp_bytes(  //
    const vole_parameters* vole_params,                                    // dimensions
    uint64_t L,                                                            // number of pairs to generate
    uint64_t topen                                                         // (max) size of the sibling path
);


/**
 * Open L vole pairs over (F2,F2^KAPPA) (grey code version)
 * @param commits_hash (output): 2.lambda bytes, same layout as the prover's:
 *        H(HASH_BAVC_PREFIX, global_salt, per-rep subhashes)
 */
EXPORT void verifier_open_midsize_grey_vole_from_seeds_bfs_ref(  //
    const vole_parameters* vole_params,                          // dimensions
    uint64_t L,                                                  // number of pairs to generate
    hash_t* commits_hash,                                        // [out] hash of the leaves commits (2.lambda bytes)
    bitmat_t* q,                                                 // [out] TAU x KAPPA x L row major
    bitmat_t* coor_terms,                                        // [in/out] TAU x L bits
    const uint32_t* hidden_leaves_idx,                           // [in] TAU indices of the hidden (node) leaves
    const commit_t* hidden_leaves_commit,                        // [in] TAU commits of the hidden (node) leaves
    const seed_t* sibling_seeds,                                // [in] Topen sibling seeds
    const uint64_t topen,                                        // [in] number of sibling seeds
    const salt_t* ggm_salt,                                      // [in] global salt for the GGM tree
    const flambda_t* delta1,                                     // verifier's point delta1
    uint8_t* tmp_space);


/**
 * Concatenate and transpose L mid-size std vole pairs over (F2,F2^KAPPA)
 * To form L full-size std vole pairs over (F2,F2^LAMBDA)
 * // acceptable in_place mode:
 * // either
 * //   out_u == in_u and out_corr == in_u[1]
 * // otherwise, out_u, out_corr, in_u shall be completely disjoint
 * // also: either
 * //   out_v == in_v
 * // or otherwise, out_v, in_v must be completely disjoint
 */
EXPORT void prover_midsize_to_fullsize_std_vole_ct_ref(  //
    const vole_parameters* vole_params,                  // dimensions
    uint64_t L,                                          // number of pairs to treat
    bitvec_t* out_u,                                     // [out] L bits
    bitmat_t* out_corr,                                  // [out] (TAU-1 x L) row major
    flambda_t* out_v,                                    // [out] gflambda[L] contiguous
    const bitmat_t* in_u,                                // [in] TAU x L row major
    const bitmat_t* in_v);                               // [in] (TAU*KAPPA) x L row major

/**
 * Concatenate and transpose L mid-size std vole pairs over (F2,F2^KAPPA)
 * To form L full-size std vole pairs over (F2,F2^LAMBDA)
 * // acceptable in_place mode:
 * // also: either
 * //   out_q == in_a
 * // or otherwise, out_q, in_q must be completely disjoint
 */
EXPORT void verifier_midsize_to_fullsize_std_vole_ref(  //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t L,                                         // number of pairs to treat
    flambda_t* out_q,                                   // [out] gflambda[L]
    const bitmat_t* in_q);                              // [in] TAU*KAPPA x L row major

EXPORT uint64_t vole_consistency_check_matrix_nrows(  //
    const vole_parameters* vole_params);              // dimensions

EXPORT uint64_t vole_consistency_check_matrix_ncols(  //
    const vole_parameters* vole_params,               // dimensions
    uint64_t L);                                      // number of pairs to treat

EXPORT void both_vole_consistency_check_matrix(          //
    const vole_parameters* vole_params,                  // dimensions
    uint64_t L,                                          // number of pairs to treat
    bitmat_t* chk_matrix,                                // [out] cst_check_dim x (L - cst_check_dim)
    const hash_t* chk_seed1, uint64_t chk_seed1_bytes);  // [in] rng seed and tweak

EXPORT void prover_vole_consistency_check(  //
    const vole_parameters* vole_params,     // dimensions
    uint64_t L,                             // number of pairs to treat
    bitvec_t* chk_u,                        // [out] cst check of size KAPPA.TAU+B
    flambda_t* chk_v,                       // [out] chk_v
    const bitvec_t* u,                      // [in] vector of length L
    const flambda_t* v,                     // [in] vector of length L
    const bitvec_t* chk_matrix);            // [in] cst check matrix

EXPORT void verifier_vole_consistency_check(  //
    const vole_parameters* vole_params,       // dimensions
    uint64_t L,                               // number of pairs to treat
    flambda_t* chk_v,                         // [out] chk_v
    const bitvec_t* chk_u,                    // [in] cst check of size KAPPA.TAU+B
    const flambda_t* q,                       // [in] vector of length L
    const bitvec_t* chk_matrix,               // [in] cst check matrix (L-cst_check_dim) x L
    const bitvec_t* delta1);                  // [in] delta1 (kappa.tau bits)

#endif  // VOLE_GENERATION_H
