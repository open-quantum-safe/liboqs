#include <string.h>
#include <stdint.h>

#include "ggm.h"
#include "sdith_prng.h"
#include "vole_private.h"
#include "KeccakSpongetimes4.h"

#ifndef NDEBUG
#define MSTACK_DEF(bytelen)                              \
  uint8_t stack_var[bytelen] __attribute((aligned(32))); \
  uint8_t* stack_cur = stack_var;                        \
  uint8_t* const stack_end = stack_var + bytelen

#define MSTACK_ALLOC(bytelen) \
  (void*)stack_cur;           \
  stack_cur += bytelen;       \
  CREQUIRE(stack_cur <= stack_end, "manual stack overflow!!")
#else
#define MSTACK_DEF(bytelen)                              \
  uint8_t stack_var[bytelen] __attribute((aligned(32))); \
  uint8_t* stack_cur = stack_var

#define MSTACK_ALLOC(bytelen) \
  (void*)stack_cur;           \
  stack_cur += bytelen
#endif

#define BATCH_XOF_STRIDE (((sizeof(KeccakWidth1600times4_SpongeInstance) + 31) / 32) * 32)
#define BATCH_XOF(base, idx) ((KeccakWidth1600times4_SpongeInstance*)((uint8_t*)(base) + (idx) * BATCH_XOF_STRIDE))

/** @brief converts delta0 to delta1
 * delta0 has kappa*tau bits, encoding tau grey code encodings
 * delta1 has lambda bits, only the lowest kappa*tau are used, the rest is zero.
 */
EXPORT void delta1_from_delta0_ref(const vole_parameters* vole_params, flambda_t* delta1, const bitvec_t* delta0) {
  // bit_vector delta1_from_delta0(uint64_t kappa, uint64_t tau, uint64_t lambda, const bit_vector& delta0) {
  const uint64_t kappa = vole_params->KAPPA;
  const uint64_t tau = vole_params->TAU;
  CASSERT(vole_params->LAMBDA >= tau * kappa, "invalid lambda");
  CASSERT(tau * kappa > 0, "invalid kappa, tau");
  CASSERT(delta0 != delta1, "in_place not supported");
  memset(delta1, 0, vole_params->lambda_bytes);
  for (uint64_t i = 0; i < tau; i++) {
    uint64_t pos = extract_kappabit_uint(kappa, i * kappa, delta0);
    xorto_kappabit_uint(kappa, i * kappa, delta1, pos ^ (pos >> 1));
  }
}

/**
 * number of tmp bytes needed by the function
 */
EXPORT uint64_t prover_generate_midsize_grey_vole_from_seeds_bfs_ct_tmp_bytes(  //
    const vole_parameters* vole_params,                               // dimensions
    uint64_t L                   // number of pairs to generate [Lslice next mult of 32]
) {
  const uint64_t Lbytesx = L >> 3;
  const uint64_t Lslice = (Lbytesx + 31) & UINT64_C(-32);
  const uint64_t TAU = vole_params->TAU;
  const uint64_t KAPPA = vole_params->KAPPA;
  const uint64_t LAMBDA_BYTES = vole_params->lambda_bytes;
  const uint64_t SEED_BYTES = LAMBDA_BYTES;
  const uint64_t COMMIT_BYTES = 2 * LAMBDA_BYTES;
  const uint64_t EXT_BYTES = vole_params->extended_node_seed_bytes();

  const uint64_t TOTAL_LEAVES = TAU << KAPPA;  // total number of leaves in the tree
  const uint64_t TOTAL_NODES = (TOTAL_LEAVES << 1) - 1;
  //const uint64_t TREE_DEPTH = ceil_log2i(TAU) + KAPPA;
  //const uint64_t THETA = TREE_DEPTH + 2;  // theta = ceil(log2(tau*2^kappa)) + 2
  //const uint64_t nleaves = UINT64_C(1) << KAPPA;
  CASSERT(L % 8 == 0, "currently, we support only L multiple of 8");

  // tree representation
  const uint64_t xof_vector_ctx_bytes = vole_params->xof_vector.ctx_bytes(TAU);
  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint64_t*, masked_salts, 32, 2 * SEED_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, seeds, 32, (TOTAL_NODES + 1) * SEED_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, ext_seeds, 32, 4 * TAU * EXT_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, commit_buffer, 32, TAU* COMMIT_BYTES);
  COMP_SPACE_MAP_ALIGNED(bitvec_t*, tmp_r, 32, Lslice);
  COMP_SPACE_MAP_ALIGNED(xof_vector_ctx*, commit_xofs, 32, xof_vector_ctx_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t**, commit_ptrs, 8, TAU * sizeof(uint8_t*));
  COMP_SPACE_MAP_ALIGNED(bitvec_t**, uu, 8, TAU * sizeof(bitvec_t*));
  COMP_SPACE_MAP_ALIGNED(bitmat_t**, vv, 8, TAU * sizeof(bitmat_t*));
  COMP_SPACE_RETURN();
}


/**
 * Generate L vole pairs over (F2,F2^KAPPA) (grey code + BFS version)
 * @param u (output): L bits:  Xor of all ri's,
 * @param v (output): matrix KAPPA x L bits: Xor of ri.grey(i)
 */
EXPORT void prover_generate_midsize_grey_vole_from_seeds_bfs_ct_ref(  //
    const vole_parameters* vole_params,                               // dimensions
    uint64_t L,                   // number of pairs to generate [Lslice next mult of 32]
    hash_t* commits_hash,         // [out] hash of the leaves commits (2.lambda bytes)
    bitmat_t* u,                  // [out] TAU x (L bits on Lslice bytes)
    bitmat_t* v,                  // [out] TAU x KAPPA x (L bits on Lslice bytes) row major
    const salt_t* global_salt,    // [in] global salt for the GGM tree
    const seed_t* ggm_root_seed,  // [in] root seed for the GGM tree
    uint8_t* tmp_space)           // [tmp] scratch space
{
  const uint64_t Lbytesx = L >> 3;
  const uint64_t Lslice = (Lbytesx + 31) & UINT64_C(-32);
  const uint64_t TAU = vole_params->TAU;
  const uint64_t KAPPA = vole_params->KAPPA;
  const uint64_t LAMBDA_BYTES = vole_params->lambda_bytes;
  const uint64_t SEED_BYTES = LAMBDA_BYTES;
  const uint64_t COMMIT_BYTES = 2 * LAMBDA_BYTES;
  const uint64_t EXT_BYTES = vole_params->extended_node_seed_bytes();

  const uint64_t TOTAL_LEAVES = TAU << KAPPA;  // total number of leaves in the tree
  const uint64_t TOTAL_NODES = (TOTAL_LEAVES << 1) - 1;
  const uint64_t TREE_DEPTH = ceil_log2i(TAU) + KAPPA;
  const uint64_t THETA = TREE_DEPTH + 2;  // theta = ceil(log2(tau*2^kappa)) + 2
  const uint64_t nleaves = UINT64_C(1) << KAPPA;
  CASSERT(L % 8 == 0, "currently, we support only L multiple of 8");

  // tree representation
  const uint64_t xof_vector_ctx_bytes = vole_params->xof_vector.ctx_bytes(TAU);
  TMP_SPACE_MAP_ALIGNED(uint64_t*, masked_salts, 32, 2 * SEED_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, seeds, 32, (TOTAL_NODES + 1) * SEED_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, ext_seeds, 32, 4 * TAU * EXT_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, commit_buffer, 32, TAU* COMMIT_BYTES);
  TMP_SPACE_MAP_ALIGNED(bitvec_t*, tmp_r, 32, Lslice);
  TMP_SPACE_MAP_ALIGNED(xof_vector_ctx*, commit_xofs, 32, xof_vector_ctx_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t**, commit_ptrs, 8, TAU * sizeof(uint8_t*));
  TMP_SPACE_MAP_ALIGNED(bitvec_t**, uu, 8, TAU * sizeof(bitvec_t*));
  TMP_SPACE_MAP_ALIGNED(bitmat_t**, vv, 8, TAU * sizeof(bitmat_t*));
  // the two domain-separated salts: one for the GGM tree, one for the vole rng
  uint64_t* const ggm_masked_salt = masked_salts;
  uint64_t* const vole_masked_salt = masked_salts + (SEED_BYTES >> 3);
  compute_tweaked_salts(vole_params, ggm_masked_salt, vole_masked_salt, global_salt);
  CASSERT(THETA <= GGM_TWEAK_BITS, "bug! the tree is too deep for the salt tweak field");
  for (uint64_t i = 0; i < TAU; ++i) {  // some handy double pointers for vole
    uu[i] = u + i * Lslice;
    vv[i] = v + i * KAPPA * Lslice;
  }
  for (uint64_t i = 0; i < TAU; ++i) {  // some handy double pointers for xofs
    commit_ptrs[i] = commit_buffer + i * COMMIT_BYTES;
  }
  uint8_t* const subhashes = commit_buffer;  // we will share that space since the subhash size is 2lambda
  uint8_t** const subhashes_ptrs = commit_ptrs;

  // memcpy the root of the tree
  memcpy(seeds + SEED_BYTES, ggm_root_seed, SEED_BYTES);

  // expand all the seeds using the seed_rng
  uint64_t next_node_idx = 2;                  // also equal to the num leaves to write at that depth
  const uint8_t* rd_ptr = seeds + SEED_BYTES;  // root position (node_id = 1)
  uint8_t* wr_ptr = seeds + 2 * SEED_BYTES;    // left child (node_id=2)
  // expand the first two depths of the tree one by one
  while (next_node_idx < 8) {
    vole_params->extend_leaf_seed(ext_seeds, rd_ptr);
    vole_params->ggm_extseed_rng_lr(wr_ptr, ggm_masked_salt, ext_seeds, next_node_idx, THETA);
    wr_ptr += 2 * SEED_BYTES;
    rd_ptr += SEED_BYTES;
    next_node_idx += 2;
  }
  // expand the rest of the depths 4x by 4x
  while (next_node_idx <= TOTAL_NODES) {
    vole_params->extend_leaf_seed_4x(ext_seeds, rd_ptr);
    // I8: expand the 4 sibling parents (node_idx, +2, +4, +6) in one call. The cat1 avx2
    // build batches them into one 8-block encrypt; the other builds do four single calls.
    vole_params->ggm_extseed_rng_lr_x4(wr_ptr, ggm_masked_salt, ext_seeds, next_node_idx, THETA);
    wr_ptr += 8 * SEED_BYTES;
    rd_ptr += 4 * SEED_BYTES;
    next_node_idx += 8;
  }
  CASSERT(next_node_idx == TOTAL_NODES + 1, "we should have filled the tree");

  // expand all the leaf seeds into commit and vole randomness
  memset(u, 0, TAU * Lslice);
  memset(v, 0, TAU * KAPPA * Lslice);
  vole_params->xof_vector.init_and_seed(commit_xofs, TAU, &HASH_BAVC_PREFIX, 1);

  uint64_t l = 0;
  next_node_idx = TOTAL_LEAVES;
  const uint8_t* next_leaf_seed = seeds + TOTAL_LEAVES * SEED_BYTES;  // ptr of the first leaf seed
  const uint8_t* const end_leaf_seed = seeds + (TOTAL_NODES + 1) * SEED_BYTES;
  while (next_leaf_seed < end_leaf_seed) {
    // expand 4 * TAU leaf seeds and advance the read pointer
    uint8_t* out_ext_seed = ext_seeds;
    for (uint64_t i = 0; i < TAU; ++i) {
      // expand the leaf seed -> expanded buffer
      vole_params->extend_leaf_seed_4x(out_ext_seed,   // output
                                       next_leaf_seed  // leaf seed
      );
      next_leaf_seed += 4 * LAMBDA_BYTES;
      out_ext_seed += 4 * EXT_BYTES;
    }
    CASSERT(out_ext_seed == ext_seeds + 4 * TAU * EXT_BYTES, "out_ext_seed should be at the end of the buffer");

    // process the 4 * TAU expanded leaves (it corresponds to 4 positions)
    const uint8_t* in_expanded_seed = ext_seeds;
    for (uint64_t nb = 0; nb < 4; ++nb) {
      const uint64_t loc = (l + 1 < nleaves) ? binval_of(l + 1) : KAPPA - 1;
      const uint64_t loc_offset = loc * Lslice;
      uint8_t* out_commit = commit_buffer;
      for (uint64_t i = 0; i < TAU; ++i) {
        // compute the commit and add it to the commit buffer
        vole_params->ggm_extseed_rng_commit(out_commit,        // out
                                            ggm_masked_salt,   // salt
                                            in_expanded_seed,  // expanded_seed
                                            next_node_idx      // node_idx
        );
        // compute the vole_rng and process it
        vole_params->vole_extseed_rng(tmp_r, Lslice,       // out (only Lbytes would suffice here)
                                      vole_masked_salt,    // salt
                                      in_expanded_seed,    // expanded_seed
                                      i);                  // repetition index
        vole_params->bitvec_cascade_xor_to(Lslice, vv[i] + loc_offset, uu[i], tmp_r);
        // advance pointers
        in_expanded_seed += EXT_BYTES;
        out_commit += COMMIT_BYTES;
        next_node_idx += 1;
      }
      // hash the commits
      vole_params->xof_vector.seed(commit_xofs, TAU, commit_ptrs, COMMIT_BYTES);
      // advance the pointers
      ++l;
    }
    CASSERT(in_expanded_seed == ext_seeds + 4 * TAU * EXT_BYTES, "we should have consumed all the expanded seeds");
  }
  CASSERT(l == nleaves, "we should have processed all the leaves");
  CASSERT(next_leaf_seed == end_leaf_seed, "we should have processed all the leaves");
  CASSERT(next_node_idx == TOTAL_NODES + 1, "we should have processed all the leaves");

  // now, hash the per-rep subhashes into the final commits hash:
  // commits_hash = H(HASH_BAVC_PREFIX, global_salt, subhashes). the full salt is
  // bound first (it commits all lambda bits, whereas the tweaked salts drop the
  // low GGM_TWEAK_BITS and the top prefix bits), then the per-rep subhashes.
  vole_params->xof_vector.finalize_and_output(commit_xofs, TAU, subhashes_ptrs, 2 * LAMBDA_BYTES);
  xof_ctx commits_xof;
  vole_params->xof.xof_init_and_seed(&commits_xof, &HASH_BAVC_PREFIX, 1);
  vole_params->xof.xof_seed(&commits_xof, global_salt, LAMBDA_BYTES);
  vole_params->xof.xof_seed(&commits_xof, subhashes, 2 * TAU * LAMBDA_BYTES);
  vole_params->xof.xof_finalize_and_output(&commits_xof, commits_hash, 2 * LAMBDA_BYTES);
}

/**
 * Open L vole pairs over (F2,F2^KAPPA) (grey code version) -- tmp bytes needed
 */
EXPORT uint64_t verifier_open_midsize_grey_vole_from_seeds_bfs_tmp_bytes(  //
    const vole_parameters* vole_params,                          // dimensions
    uint64_t L,                                                  // number of pairs to generate
    uint64_t topen                                               // (max) size of the sibling path
    ) {
  const uint64_t Lbytesxx = L >> 3;
  const uint64_t Lslice = (Lbytesxx + 31) & UINT64_C(-32);
  const uint64_t TAU = vole_params->TAU;
  const uint64_t KAPPA = vole_params->KAPPA;
  const uint64_t LAMBDA_BYTES = vole_params->lambda_bytes;
  const uint64_t EXT_BYTES = vole_params->extended_node_seed_bytes();

  const uint64_t SEED_BYTES = LAMBDA_BYTES;
  const uint64_t COMMIT_BYTES = 2 * LAMBDA_BYTES;

  const uint64_t TOTAL_LEAVES = TAU << KAPPA;  // total number of leaves in the tree
  const uint64_t TOTAL_NODES = (TOTAL_LEAVES << 1) - 1;
  const uint64_t xof_vector_ctx_bytes = vole_params->xof_vector.ctx_bytes(TAU);

  CASSERT(L % 8 == 0, "currently, we support only L multiple of 8");

  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint64_t*, masked_salts, 32, 2 * SEED_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, seeds, 32, (TOTAL_NODES + 1) * SEED_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, ext_seeds, 32, 4 * TAU * EXT_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, commit_buffer, 32, TAU* COMMIT_BYTES);
  COMP_SPACE_MAP_ALIGNED(bitvec_t*, tmp_r, 32, Lslice);
  COMP_SPACE_MAP_ALIGNED(xof_vector_ctx*, commit_xofs, 32, xof_vector_ctx_bytes);
  COMP_SPACE_MAP_ALIGNED(uint8_t**, commit_ptrs, 8, TAU * sizeof(uint8_t*));
  COMP_SPACE_MAP_ALIGNED(bitvec_t**, ss, 8, TAU * sizeof(bitvec_t*));
  COMP_SPACE_MAP_ALIGNED(bitmat_t**, qq, 8, TAU * sizeof(bitmat_t*));
  COMP_SPACE_MAP_ALIGNED(uint32_t*, hidden_nodes_idx, 8, (2 * TAU + topen + 1) * sizeof(uint32_t));
  COMP_SPACE_MAP_ALIGNED(uint32_t*, sibling_nodes_idx, 8, (topen + 1) * sizeof(uint32_t));
  COMP_SPACE_RETURN();
}

/**
 * Open L vole pairs over (F2,F2^KAPPA) (grey code version)
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
    const salt_t* global_salt,                                   // [in] global salt for the GGM tree
    const flambda_t* delta1,                                     // verifier's point delta1
    uint8_t* tmp_space) {
  const uint64_t Lbytesxx = L >> 3;
  const uint64_t Lslice = (Lbytesxx + 31) & UINT64_C(-32);
  const uint64_t TAU = vole_params->TAU;
  const uint64_t KAPPA = vole_params->KAPPA;
  const uint64_t LAMBDA_BYTES = vole_params->lambda_bytes;
  const uint64_t EXT_BYTES = vole_params->extended_node_seed_bytes();
  const uint64_t nleaves = UINT64_C(1) << KAPPA;

  const uint64_t SEED_BYTES = LAMBDA_BYTES;
  const uint64_t COMMIT_BYTES = 2 * LAMBDA_BYTES;

  const uint64_t TOTAL_LEAVES = TAU << KAPPA;  // total number of leaves in the tree
  const uint64_t TOTAL_NODES = (TOTAL_LEAVES << 1) - 1;
  const uint64_t TREE_DEPTH = ceil_log2i(TAU) + KAPPA;
  const uint64_t THETA = TREE_DEPTH + 2;  // theta = ceil(log2(tau*2^kappa)) + 2
  const uint64_t xof_vector_ctx_bytes = vole_params->xof_vector.ctx_bytes(TAU);

  CASSERT(L % 8 == 0, "currently, we support only L multiple of 8");

  TMP_SPACE_MAP_ALIGNED(uint64_t*, masked_salts, 32, 2 * SEED_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, seeds, 32, (TOTAL_NODES + 1) * SEED_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, ext_seeds, 32, 4 * TAU * EXT_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, commit_buffer, 32, TAU* COMMIT_BYTES);
  TMP_SPACE_MAP_ALIGNED(bitvec_t*, tmp_r, 32, Lslice);
  TMP_SPACE_MAP_ALIGNED(xof_vector_ctx*, commit_xofs, 32, xof_vector_ctx_bytes);
  TMP_SPACE_MAP_ALIGNED(uint8_t**, commit_ptrs, 8, TAU * sizeof(uint8_t*));
  TMP_SPACE_MAP_ALIGNED(bitvec_t**, ss, 8, TAU * sizeof(bitvec_t*));
  TMP_SPACE_MAP_ALIGNED(bitmat_t**, qq, 8, TAU * sizeof(bitmat_t*));
  TMP_SPACE_MAP_ALIGNED(uint32_t*, hidden_nodes_idx, 8, (2 * TAU + topen + 1) * sizeof(uint32_t));
  TMP_SPACE_MAP_ALIGNED(uint32_t*, sibling_nodes_idx, 8, (topen + 1) * sizeof(uint32_t));
  // the two domain-separated salts: one for the GGM tree, one for the vole rng
  uint64_t* const ggm_masked_salt = masked_salts;
  uint64_t* const vole_masked_salt = masked_salts + (SEED_BYTES >> 3);
  compute_tweaked_salts(vole_params, ggm_masked_salt, vole_masked_salt, global_salt);
  CASSERT(THETA <= GGM_TWEAK_BITS, "bug! the tree is too deep for the salt tweak field");
  for (uint64_t i = 0; i < TAU; ++i) {
    qq[i] = ((uint8_t*)q) + i * KAPPA * Lslice;
    ss[i] = ((uint8_t*)coor_terms) + i * Lslice;
  }
  for (uint64_t i = 0; i < TAU; ++i) {  // some handy double pointers for xofs
    commit_ptrs[i] = commit_buffer + i * COMMIT_BYTES;
  }
  uint8_t* const subhashes = commit_buffer;  // we will share that space since the subhash size is 2lambda
  uint8_t** const subhashes_ptrs = commit_ptrs;

  // step 1: compute the sibling node indexes
#ifndef NDEBUG
  uint32_t num_leaves = TAU << KAPPA;
  for (uint32_t i = 0; i < TAU - 1; ++i) {
    CREQUIRE(hidden_leaves_idx[i] < hidden_leaves_idx[i + 1], "leave nodes not ordered");
  }
  CREQUIRE(hidden_leaves_idx[0] >= num_leaves, "incorrect leave node index");
  CREQUIRE(hidden_leaves_idx[TAU - 1] < 2 * num_leaves, "incorrect leave node index");
#endif
  // copy the leaf indexes in reversed order
  hidden_nodes_idx[0] = -1;  // arbitrary large delimiter
  for (uint32_t i = 0; i < TAU; ++i) {
    hidden_nodes_idx[TAU - i] = hidden_leaves_idx[i];
  }
  uint64_t istart = 1;
  uint64_t iend = TAU + 1;
  uint64_t isibling = 0;
  sibling_nodes_idx[isibling++] = -1;  // arbitrary large delimiter
  while (iend - istart >= 2) {
    const uint32_t first_idx = hidden_nodes_idx[istart];
    const uint32_t second_idx = hidden_nodes_idx[istart + 1];
    const uint32_t next_idx = first_idx >> 1;
    CASSERT(first_idx > 1, "bug1");
    CASSERT(hidden_nodes_idx[iend - 1] > next_idx, "bug2");
    if ((first_idx ^ second_idx) == 1) {
      // two hidden nodes with the same parent: no sibling
      istart += 2;
    } else {
      // single child out of at least 2: one sibling
      sibling_nodes_idx[isibling++] = first_idx ^ UINT32_C(1);
      istart += 1;
    }
    hidden_nodes_idx[iend++] = next_idx;
  }
  for (uint32_t hidden_idx = hidden_nodes_idx[istart]; hidden_idx != 1; hidden_idx >>= 1) {
    sibling_nodes_idx[isibling++] = hidden_idx ^ UINT32_C(1);
  }
  CREQUIRE(isibling == topen + 1, "we should have found all the siblings (+ delim )");
#ifndef NDEBUG
  for (uint32_t i = 0; i < topen; ++i) {
    CREQUIRE(sibling_nodes_idx[i] > sibling_nodes_idx[i + 1], "sibling nodes not ordered");
  }
#endif

  // step 2: expand all the seeds (replacing siblings)
  CASSERT(topen >= 1, "BUG! invalid parameter");
  uint64_t next_node_idx = 2;                  // also equal to the num leaves to write at that depth
  const uint8_t* rd_ptr = seeds + SEED_BYTES;  // root position (node_id = 1)
  uint8_t* wr_ptr = seeds + 2 * SEED_BYTES;    // left child (node_id=2)
  const uint32_t* next_sibling = sibling_nodes_idx + topen;
  const uint8_t* next_sibling_seed = ((uint8_t*)sibling_seeds) + (topen - 1) * SEED_BYTES;

  // pretend that the root is zero (we know it is hidden!)
  CASSERT(*next_sibling > 1, "the root is necessarily hidden");
  memset(seeds + SEED_BYTES, 0, SEED_BYTES);

  // expand the first two depths of the tree one by one
  while (next_node_idx < 8) {
    vole_params->extend_leaf_seed(ext_seeds, rd_ptr);
    vole_params->ggm_extseed_rng_lr(wr_ptr, ggm_masked_salt, ext_seeds, next_node_idx, THETA);
    wr_ptr += 2 * SEED_BYTES;
    rd_ptr += SEED_BYTES;
    next_node_idx += 2;
    // replace the sibling seeds
    while (*next_sibling < next_node_idx) {
      memcpy(seeds + (*next_sibling) * SEED_BYTES, next_sibling_seed, SEED_BYTES);
      next_sibling_seed -= SEED_BYTES;
      --next_sibling;
    }
  }
  // expand the rest of the depths 4x by 4x
  while (next_node_idx <= TOTAL_NODES) {
    vole_params->extend_leaf_seed_4x(ext_seeds, rd_ptr);
    // I8: expand the 4 sibling parents (node_idx, +2, +4, +6) in one call. The cat1 avx2
    // build batches them into one 8-block encrypt; the other builds do four single calls.
    vole_params->ggm_extseed_rng_lr_x4(wr_ptr, ggm_masked_salt, ext_seeds, next_node_idx, THETA);
    wr_ptr += 8 * SEED_BYTES;
    rd_ptr += 4 * SEED_BYTES;
    next_node_idx += 8;
    // replace the sibling seeds
    while (*next_sibling < next_node_idx) {
      memcpy(seeds + (*next_sibling) * SEED_BYTES, next_sibling_seed, SEED_BYTES);
      next_sibling_seed -= SEED_BYTES;
      --next_sibling;
    }
  }
  CASSERT(next_sibling == sibling_nodes_idx, "we should have exhausted the siblings");
  CASSERT(next_sibling_seed == sibling_seeds - SEED_BYTES, "we should have exhausted the siblings");
  CASSERT(next_node_idx == TOTAL_NODES + 1, "we should have expanded the tree");

  // step 3: expand all the leaf seeds into commit and vole randomness
  memset(q, 0, TAU * KAPPA * Lslice);
  vole_params->xof_vector.init_and_seed(commit_xofs, TAU, &HASH_BAVC_PREFIX, 1);

  uint64_t l = 0;
  next_node_idx = TOTAL_LEAVES;
  const uint8_t* next_leaf_seed = seeds + TOTAL_LEAVES * SEED_BYTES;  // ptr of the first leaf seed
  const uint8_t* const end_leaf_seed = seeds + (TOTAL_NODES + 1) * SEED_BYTES;
  const uint32_t* next_hidden_leaf_idx = hidden_nodes_idx + TAU;
  const uint8_t* next_hidden_leaf_commit = hidden_leaves_commit;
  CASSERT(*next_hidden_leaf_idx >= TOTAL_LEAVES, "BUG! the first hidden leaf is necessarily a leaf");
  CASSERT(*(next_hidden_leaf_idx + 1) < TOTAL_NODES, "BUG! the 'first' hidden leaf is not the first");
  while (next_leaf_seed < end_leaf_seed) {
    // expand 4 * TAU leaf seeds and advance the read pointer
    uint8_t* out_ext_seed = ext_seeds;
    for (uint64_t i = 0; i < TAU; ++i) {
      // expand the leaf seed -> expanded buffer
      vole_params->extend_leaf_seed_4x(out_ext_seed,   // output
                                       next_leaf_seed  // leaf seed
      );
      next_leaf_seed += 4 * LAMBDA_BYTES;
      out_ext_seed += 4 * EXT_BYTES;
    }
    CASSERT(out_ext_seed == ext_seeds + 4 * TAU * EXT_BYTES, "out_ext_seed should be at the end of the buffer");

    // process the 4 * TAU expanded leaves (it corresponds to 4 positions)
    const uint8_t* in_expanded_seed = ext_seeds;
    for (uint64_t nb = 0; nb < 4; ++nb) {
      const uint64_t loc = (l + 1 < nleaves) ? binval_of(l + 1) : KAPPA - 1;
      const uint64_t loc_offset = loc * Lslice;
      uint8_t* out_commit = commit_buffer;
      for (uint64_t i = 0; i < TAU; ++i) {
        // compute the commit and add it to the commit buffer
        if (next_node_idx == *next_hidden_leaf_idx) {
          // copy the commit
          memcpy(out_commit, next_hidden_leaf_commit, COMMIT_BYTES);
          // and process hidden vole
          vole_params->bitvec_xor_to(Lslice, qq[i] + loc_offset, ss[i]);
          // advance hidden leaf ptr
          --next_hidden_leaf_idx;
          next_hidden_leaf_commit += COMMIT_BYTES;
        } else {
          // otherwise derive it from the seed
          vole_params->ggm_extseed_rng_commit(out_commit,        // out
                                              ggm_masked_salt,   // salt
                                              in_expanded_seed,  // expanded_seed
                                              next_node_idx      // node_idx
          );
          // and process vole with seed
          vole_params->vole_extseed_rng(tmp_r, Lslice,       // out (only Lbytes would suffice here)
                                        vole_masked_salt,    // salt
                                        in_expanded_seed,    // expanded_seed
                                        i);                  // repetition index
          vole_params->bitvec_cascade_xor_to(Lslice, qq[i] + loc_offset, ss[i], tmp_r);
        }
        // advance pointers
        in_expanded_seed += EXT_BYTES;
        out_commit += COMMIT_BYTES;
        next_node_idx += 1;
      }
      // hash the commits
      vole_params->xof_vector.seed(commit_xofs, TAU, commit_ptrs, COMMIT_BYTES);
      // advance the pointers
      ++l;
    }
    CASSERT(in_expanded_seed == ext_seeds + 4 * TAU * EXT_BYTES, "we should have consumed all the expanded seeds");
  }
  CASSERT(l == nleaves, "we should have processed all the leaves");
  CASSERT(next_leaf_seed == end_leaf_seed, "we should have processed all the leaves");
  CASSERT(next_node_idx == TOTAL_NODES + 1, "we should have processed all the leaves");
  CASSERT(next_hidden_leaf_idx == hidden_nodes_idx, "BUG! we should have consumed all the hidden leaves");
  CASSERT(next_hidden_leaf_commit == hidden_leaves_commit + TAU * COMMIT_BYTES,
          "BUG! we should have consumed all the hidden leaves");

  // now, hash the per-rep subhashes into the final commits hash (same layout as
  // the prover's): commits_hash = H(HASH_BAVC_PREFIX, global_salt, subhashes).
  vole_params->xof_vector.finalize_and_output(commit_xofs, TAU, subhashes_ptrs, 2 * LAMBDA_BYTES);
  xof_ctx commits_xof;
  vole_params->xof.xof_init_and_seed(&commits_xof, &HASH_BAVC_PREFIX, 1);
  vole_params->xof.xof_seed(&commits_xof, global_salt, LAMBDA_BYTES);
  vole_params->xof.xof_seed(&commits_xof, subhashes, 2 * TAU * LAMBDA_BYTES);
  vole_params->xof.xof_finalize_and_output(&commits_xof, commits_hash, 2 * LAMBDA_BYTES);

  // postprocess q using delta
  const uint64_t* deltap64 = delta1;
  for (uint64_t i = 0; i < TAU; ++i) {
    uint32_t delta_i = extract_kappabit_uint(KAPPA, i * KAPPA, deltap64);
    for (uint64_t j = 0; j < KAPPA; ++j) {
      if ((delta_i >> j) & 1) {
        vole_params->bitvec_xor_to(Lslice, qq[i] + j * Lslice, ss[i]);
      }
    }
  }
}


enum node_action_t {
  NODE_ACTION_NONE = 0,
  NODE_ACTION_HIDDEN_RIGHT = 1,
  NODE_ACTION_HIDDEN_LEFT = 2,
  NODE_ACTION_HIDDEN_RIGHT_SIBLING_LEFT = 3,
  NODE_ACTION_HIDDEN_LEFT_SIBLING_RIGHT = 4
};

EXPORT void compute_tweaked_salts( //
    const vole_parameters* vole_params, // params
    uint64_t* ggm_masked_salt,          // [out] masked salt for the GGM tree
    uint64_t* vole_masked_salt,         // [out] masked salt for the vole RNG (if not nullptr)
    const salt_t* global_salt           // [in]
) {
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  const uint64_t msb_idx = (lambda_bytes >> 3) - 1;
  memcpy(ggm_masked_salt, global_salt, lambda_bytes);
  // clear the GGM_TWEAK_BITS least significant bits (used for the actual tweak/ctr)
  ggm_masked_salt[0] &= (UINT64_C(-1) << GGM_TWEAK_BITS);
  if (vole_params->proofow_variant == PROOFOW_VARIANT_CIPHER) {
    // cipher PoW uses the 2-bits domain separation prefix "01"
    ggm_masked_salt[msb_idx] &= UINT64_C(-1) >> 2;
    ggm_masked_salt[msb_idx] |= UINT64_C(1) << 62;
  } else {
    // shake PoW uses the 1-bit domain separation prefix "1"
    ggm_masked_salt[msb_idx] |= UINT64_C(1) << 63;
  }
  if (vole_masked_salt) {
    // same algorithm but different prefixes
    memcpy(vole_masked_salt, global_salt, lambda_bytes);
    // clear the GGM_TWEAK_BITS least significant bits (used for the actual tweak/ctr)
    vole_masked_salt[0] &= (UINT64_C(-1) << GGM_TWEAK_BITS);
    if (vole_params->proofow_variant == PROOFOW_VARIANT_CIPHER) {
      // cipher PoW uses the 2-bits domain separation prefix "00"
      vole_masked_salt[msb_idx] &= UINT64_C(-1) >> 2;
    } else {
      // shake PoW uses the 1-bit domain separation prefix "0"
      vole_masked_salt[msb_idx] &= UINT64_C(-1) >> 1;
    }
  }
}

EXPORT uint64_t full_ggm_tree_open_sibling_path_from_root_tmp_bytes(  //
    const vole_parameters* vole_params,                               //
    const uint64_t sibling_path_size                                  // number of sibling seeds
) {
  const uint64_t TAU = vole_params->TAU;
  // const uint64_t KAPPA = vole_params->KAPPA;
  // const uint64_t TREE_DEPTH = ceil_log2i(TAU) + KAPPA;
  // const uint64_t THETA = TREE_DEPTH + 2;  // theta = ceil(log2(tau*2^kappa)) + 2
  const uint64_t BUF_SIZE_MAX = (2 * TAU - 1) + sibling_path_size;
  const uint64_t SEED_BYTES = vole_params->lambda_bytes;
  // const uint64_t COMMIT_BYTES = 2 * SEED_BYTES;
  const uint64_t EXT_BYTES = vole_params->extended_node_seed_bytes();

  COMP_SPACE_INIT();
  COMP_SPACE_MAP_ALIGNED(uint64_t*, ggm_masked_salt, 32, SEED_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, ext_seed, 32, EXT_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, lr_seeds, 32, 2 * SEED_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint8_t*, seeds, 32, 2 * BUF_SIZE_MAX * SEED_BYTES);
  COMP_SPACE_MAP_ALIGNED(uint32_t*, node_indexes, 8, BUF_SIZE_MAX * sizeof(uint32_t));
  COMP_SPACE_MAP_ALIGNED(uint8_t*, node_actions, 8, BUF_SIZE_MAX * sizeof(uint8_t));
  COMP_SPACE_RETURN();
}

EXPORT void full_ggm_tree_open_sibling_path_from_root(  //
    const vole_parameters* vole_params,                 //
    seed_t* sibling_seeds,                              // sibling seeds (in decreasing node index order)
    commit_t* hidden_leaves_commits,                    // hidden leaves commits (in increasing leaf order)
    const seed_t* root_seed,                            // the ggm root seed
    const salt_t* global_salt,                          // the global salt
    const uint32_t* hidden_leaves_idx,                  // index of hidden leaves (node index)
    const uint64_t sibling_path_size,                   // number of sibling seeds
    uint8_t* tmp_space) {
  const uint64_t TAU = vole_params->TAU;
  const uint64_t KAPPA = vole_params->KAPPA;
  const uint64_t TREE_DEPTH = ceil_log2i(TAU) + KAPPA;
  const uint64_t THETA = TREE_DEPTH + 2;  // theta = ceil(log2(tau*2^kappa)) + 2
  const uint64_t BUF_SIZE_MAX = (2 * TAU - 1) + sibling_path_size;
  const uint64_t SEED_BYTES = vole_params->lambda_bytes;
  const uint64_t COMMIT_BYTES = 2 * SEED_BYTES;
  const uint64_t EXT_BYTES = vole_params->extended_node_seed_bytes();

  TMP_SPACE_MAP_ALIGNED(uint64_t*, ggm_masked_salt, 32, SEED_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, ext_seed, 32, EXT_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, lr_seeds, 32, 2 * SEED_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint8_t*, seeds, 32, 2 * BUF_SIZE_MAX * SEED_BYTES);
  TMP_SPACE_MAP_ALIGNED(uint32_t*, node_indexes, 8, BUF_SIZE_MAX * sizeof(uint32_t));
  TMP_SPACE_MAP_ALIGNED(uint8_t*, node_actions, 8, BUF_SIZE_MAX * sizeof(uint8_t));
  // only the GGM salt is needed here: this path does not draw any vole randomness
  compute_tweaked_salts(vole_params, ggm_masked_salt, NULL, global_salt);
  CASSERT(THETA <= GGM_TWEAK_BITS, "bug! the tree is too deep for the salt tweak field");
#ifndef NDEBUG
  const uint64_t TOTAL_LEAVES = (TAU << KAPPA);
  for (uint64_t i = 0; i < TAU - 1; ++i) {
    CREQUIRE(hidden_leaves_idx[i] < hidden_leaves_idx[i + 1], "incorrect leave order");
  }
  CREQUIRE(hidden_leaves_idx[0] >= TOTAL_LEAVES, "incorrect leave node index");
  CREQUIRE(hidden_leaves_idx[TAU - 1] < 2 * TOTAL_LEAVES, "incorrect leave node index");
#endif
  // copy the leaf indexes in reversed order
  uint32_t istart = 0;
  uint32_t iend = TAU;
  for (uint32_t i = 0; i < TAU; ++i) {
    node_indexes[TAU - i - 1] = hidden_leaves_idx[i];
  }
  uint32_t first_idx;
  while (iend - istart >= 2) {
    first_idx = node_indexes[istart];  // *(bstart++);
    const uint32_t second_idx = node_indexes[istart + 1];
    const uint32_t next_idx = first_idx >> 1;
    CASSERT(first_idx > 1, "bug1");
    CASSERT(node_indexes[iend - 1] > next_idx, "bug2");
    if ((first_idx ^ second_idx) == 1) {
      // two hidden nodes with the same parent
      node_actions[istart] = NODE_ACTION_HIDDEN_RIGHT;
      node_actions[istart + 1] = NODE_ACTION_HIDDEN_LEFT;
      istart += 2;
    } else {
      // single child out of at least 2
      if (first_idx & 1) {
        node_actions[istart] = NODE_ACTION_HIDDEN_RIGHT_SIBLING_LEFT;
      } else {
        node_actions[istart] = NODE_ACTION_HIDDEN_LEFT_SIBLING_RIGHT;
      }
      istart += 1;
    }
    node_indexes[iend++] = next_idx;
  }
  while ((first_idx = node_indexes[istart]) != 1) {
    if (first_idx & 1) {
      node_actions[istart] = NODE_ACTION_HIDDEN_RIGHT_SIBLING_LEFT;
    } else {
      node_actions[istart] = NODE_ACTION_HIDDEN_LEFT_SIBLING_RIGHT;
    }
    istart += 1;
    node_indexes[iend++] = first_idx >> 1;
  }
#ifndef NDEBUG
  CREQUIRE(iend == BUF_SIZE_MAX, "we should have processed all the nodes");
  for (uint64_t i = 0; i < BUF_SIZE_MAX - 1; i++) {
    CREQUIRE(node_indexes[i] > node_indexes[i + 1], "incorrect node index");
  }
  CREQUIRE(node_indexes[BUF_SIZE_MAX - 1] == 1, "the last node should be the root");
#endif
  // second pass in decreasing order: fill-in the seeds
  int64_t dend = BUF_SIZE_MAX - 1;
  int64_t spath_idx = sibling_path_size - 1;
  memcpy(seeds + dend * SEED_BYTES, root_seed, SEED_BYTES);
  for (int64_t dstart = BUF_SIZE_MAX - 2; dstart >= 0; --dstart) {
    switch (node_actions[dstart]) {
      case NODE_ACTION_HIDDEN_LEFT: {
        CASSERT(dstart > 0 && node_actions[dstart - 1] == NODE_ACTION_HIDDEN_RIGHT, "incorrect node action");
        // and continue
      }
      case NODE_ACTION_HIDDEN_LEFT_SIBLING_RIGHT:
      case NODE_ACTION_HIDDEN_RIGHT_SIBLING_LEFT: {
        // expand the seed and store left
        CASSERT(dend >= TAU, "incorrect node action");
        vole_params->extend_leaf_seed(ext_seed, seeds + dend * SEED_BYTES);
        vole_params->ggm_extseed_rng_lr(lr_seeds, ggm_masked_salt, ext_seed, node_indexes[dstart] & UINT32_C(-2), THETA);
        --dend;
        break;
      }
      case NODE_ACTION_HIDDEN_RIGHT:
        break;
      default:
        abort();  // unknown case
    }
    switch (node_actions[dstart]) {
      case NODE_ACTION_HIDDEN_LEFT: {
        memcpy(seeds + dstart * SEED_BYTES, lr_seeds, SEED_BYTES);
        break;
      }
      case NODE_ACTION_HIDDEN_RIGHT: {
        memcpy(seeds + dstart * SEED_BYTES, lr_seeds + SEED_BYTES, SEED_BYTES);
        break;
      }
      case NODE_ACTION_HIDDEN_LEFT_SIBLING_RIGHT: {
        memcpy(seeds + dstart * SEED_BYTES, lr_seeds, SEED_BYTES);
        memcpy(sibling_seeds + spath_idx * SEED_BYTES, lr_seeds + SEED_BYTES, SEED_BYTES);
        --spath_idx;
        break;
      }
      case NODE_ACTION_HIDDEN_RIGHT_SIBLING_LEFT: {
        memcpy(seeds + dstart * SEED_BYTES, lr_seeds + SEED_BYTES, SEED_BYTES);
        memcpy(sibling_seeds + spath_idx * SEED_BYTES, lr_seeds, SEED_BYTES);
        --spath_idx;
        break;
      }
      default:
        abort();  // unknown case
    }
  }
  CASSERT(spath_idx == -1, "we should have processed all the sibling seeds");
  CASSERT(dend == TAU - 1, "we should have processed all the nodes");
  // final pass: compute the hidden leaves commits
  for (uint64_t i = 0; i < TAU; ++i) {
    vole_params->extend_leaf_seed(ext_seed, seeds + (TAU - i - 1) * SEED_BYTES);
    vole_params->ggm_extseed_rng_commit(hidden_leaves_commits + i * COMMIT_BYTES, ggm_masked_salt, ext_seed,
                                        node_indexes[TAU - i - 1]);
  }
}

EXPORT void prover_midsize_to_fullsize_std_vole_ct_ref(const vole_parameters* vole_params, uint64_t L, bitvec_t* out_u,
                                                       bitmat_t* out_corr, flambda_t* out_v, const bitmat_t* in_u,
                                                       const bitmat_t* in_v) {
  // in_v points to a lambda x L_byte region, and the last lambda - tau.kappa rows are set to zero

  const uint64_t Lbytesx = (L + 7) / 8;
  const uint64_t Lslice = (Lbytesx + 31) & UINT64_C(-32);

  // out_v = transpose(in_v)
  (*vole_params->matrix_lambda_transpose)(out_v, in_v, L, Lslice);

  // out_u = first L bits of in_u
  memcpy(out_u, in_u, Lbytesx);

  // out_corr = in_u[i+1] ^ in_u[0]
  // attention: out_corr has an LByte slice, whereas in_u has a Lslice slice
  for (uint64_t i = 0; i < vole_params->TAU - 1; i++) {
    vole_params->bitvec_xor(Lslice, out_corr + i * Lslice, in_u + (i + 1) * Lslice, in_u);
  }
}

EXPORT void verifier_midsize_to_fullsize_std_vole_ref(const vole_parameters* vole_params, uint64_t L, flambda_t* out_q,
                                                      const bitmat_t* in_q) {
  const uint64_t Lbytes = (L + 7) / 8;
  const uint64_t Lslice = (Lbytes + 31) & UINT64_C(-32);
  // out_q = transpose(in_q)
  (*vole_params->matrix_lambda_transpose)(out_q, in_q, L, Lslice);
}

EXPORT uint64_t vole_consistency_check_matrix_nrows(  //
    const vole_parameters* vole_params)  // dimensions                                      // number of pairs to treat
{
  // num_cchk_pairs = LAMBDA + B, B = 16 (Thibauld Feneuil / FAEST spec): the consistency-check matrix
  // size for the claimed soundness level (LAMBDA+16 is already a multiple of 8). Value-identical to the
  // old ceil8(KAPPA*TAU+16) for every shipped set, but the authoritative form -- KAPPA*TAU only
  // coincided with LAMBDA and would UNDER-size the check for faster params where KAPPA*TAU < LAMBDA.
  return vole_params->LAMBDA + 16;
}

EXPORT uint64_t vole_consistency_check_matrix_ncols(  //
    const vole_parameters* vole_params,               // dimensions
    uint64_t L) {
  return L - vole_consistency_check_matrix_nrows(vole_params);
}

EXPORT void both_vole_consistency_check_matrix(         //
    const vole_parameters* vole_params,                 // dimensions
    uint64_t L,                                         // number of pairs to treat
    bitmat_t* chk_matrix,                               // [out] cst_check_dim x (L - cst_check_dim)
    const hash_t* chk_seed1, uint64_t chk_seed1_bytes)  // [in] rng seed and tweak
{
  const uint64_t cchk_nrows = vole_consistency_check_matrix_nrows(vole_params);
  const uint64_t cchk_ncols = vole_consistency_check_matrix_ncols(vole_params, L);
  const uint64_t cchk_col_bytes = (cchk_ncols + 7) >> 3;
  xof_ctx cchk_matrix_rng;
  vole_params->xof.xof_init_and_seed(&cchk_matrix_rng, chk_seed1, chk_seed1_bytes);
  vole_params->xof.xof_finalize_and_output(&cchk_matrix_rng, chk_matrix, cchk_nrows * cchk_col_bytes);
  // mask last columns with zeroes if needed
  uint8_t cchk_last_mask = 0xFF >> ((-cchk_ncols) & 7);
  uint8_t* const cm = (uint8_t*)chk_matrix;
  for (uint64_t i = 0; i < cchk_nrows; i++) {
    cm[i * cchk_col_bytes + cchk_col_bytes - 1] &= cchk_last_mask;
  }
}

EXPORT void prover_vole_consistency_check(  //
    const vole_parameters* vole_params,     // dimensions
    uint64_t L,                             // number of pairs to treat
    bitvec_t* chk_u,                        // [out] cst check of size KAPPA.TAU+B
    flambda_t* chk_v,                       // [out] chk_v
    const bitvec_t* u,                      // [in] vector of length L
    const flambda_t* v,                     // [in] vector of length L
    const bitvec_t* chk_matrix)             // [in] cst check matrix
{
  const uint64_t lambda = vole_params->LAMBDA;
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  const uint64_t cchk_nrows = vole_consistency_check_matrix_nrows(vole_params);
  const uint64_t cchk_ncols = vole_consistency_check_matrix_ncols(vole_params, L);
  const uint64_t cchk_nrows_bytes = cchk_nrows >> 3;
  const uint8_t* const uu = (const uint8_t*)u;
  const uint8_t* const vv = (const uint8_t*)v;
  vole_params->matrix_vector_product_f2(cchk_nrows, cchk_ncols, chk_u, chk_matrix, uu + cchk_nrows_bytes);
  vole_params->bitvec_xor_to(cchk_nrows_bytes, chk_u, uu);
  vole_params->matrix_f2_times_vector_flambda(lambda, cchk_nrows, cchk_ncols, chk_v, chk_matrix,
                                              vv + cchk_nrows * lambda_bytes);
  vole_params->bitvec_xor_to(cchk_nrows * lambda_bytes, chk_v, vv);
}

EXPORT void verifier_vole_consistency_check(  //
    const vole_parameters* vole_params,       // dimensions
    uint64_t L,                               // number of pairs to treat
    flambda_t* chk_v,                         // [out] chk_v
    const bitvec_t* chk_u,                    // [in] cst check of size KAPPA.TAU+B
    const flambda_t* q,                       // [in] vector of length L
    const bitvec_t* chk_matrix,               // [in] cst check matrix (L-cst_check_dim) x L
    const bitvec_t* delta1)                   // [in] delta1 (kappa.tau bits)
{
  const uint64_t lambda = vole_params->LAMBDA;
  const uint64_t lambda_bytes = vole_params->lambda_bytes;
  const uint64_t cchk_nrows = vole_consistency_check_matrix_nrows(vole_params);
  const uint64_t cchk_ncols = vole_consistency_check_matrix_ncols(vole_params, L);
  const uint8_t* const qq = (const uint8_t*)q;
  vole_params->matrix_f2_times_vector_flambda(lambda, cchk_nrows, cchk_ncols, chk_v, chk_matrix,
                                              qq + cchk_nrows * lambda_bytes);
  vole_params->bitvec_xor_to(cchk_nrows * lambda_bytes, chk_v, qq);
  const uint8_t* const cu = (uint8_t*)chk_u;
  uint8_t* const cv = (uint8_t*)chk_v;
  for (uint64_t i = 0; i < cchk_nrows; ++i) {
    if ((cu[i >> 3] >> (i & 7)) & 1) {
      vole_params->flambda_sum(cv + i * lambda_bytes, cv + i * lambda_bytes, delta1);
    }
  }
}
