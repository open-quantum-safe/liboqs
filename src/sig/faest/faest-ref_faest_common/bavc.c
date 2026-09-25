/*
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bavc.h"
#include "random_oracle.h"
#include "compat.h"
#include "aes.h"
#include "instances.h"
#include "universal_hashing.h"
#include "parameters.h"

#include <assert.h>
#include <string.h>

#define NODE(nodes, node, lambda_bytes) (&nodes[(node) * (lambda_bytes)])

static void expand_seeds(uint8_t* nodes, const uint8_t* iv, const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;

  for (unsigned int alpha = 0; alpha < params->L - 1; ++alpha) {
    // the nodes are located in memory consecutively
    prg_2_lambda(NODE(nodes, alpha, lambda_bytes), iv, alpha,
                 NODE(nodes, 2 * alpha + 1, lambda_bytes), params->lambda);
  }
}

static uint8_t* generate_seeds(const uint8_t* root_seed, const uint8_t* iv,
                               const faest_paramset_t* params) {
  unsigned int lambda_bytes = params->lambda / 8;
  uint8_t* nodes            = calloc(2 * params->L - 1, lambda_bytes); // IGNORE memory-check
  assert(nodes);

  memcpy(NODE(nodes, 0, lambda_bytes), root_seed, lambda_bytes);
  expand_seeds(nodes, iv, params);

  return nodes;
}

// FAEST.LeafCommit
static inline void faest_leaf_commit_128(uint8_t* sd, uint8_t* com, const uint8_t* key,
                                         const uint8_t* iv, uint32_t tweak, const uint8_t* uhash) {
  ATTR_ALIGNED(16) uint8_t buffer[FAEST_128_LAMBDA / 8 * 4];
  prg_4_lambda(key, iv, tweak, buffer, FAEST_128_LAMBDA);
  leaf_hash_128(com, uhash, buffer);
  memcpy(sd, buffer, FAEST_128_LAMBDA / 8);
}

static inline void faest_leaf_commit_192(uint8_t* sd, uint8_t* com, const uint8_t* key,
                                         const uint8_t* iv, uint32_t tweak, const uint8_t* uhash) {
  ATTR_ALIGNED(16) uint8_t buffer[FAEST_192_LAMBDA / 8 * 4];
  prg_4_lambda(key, iv, tweak, buffer, FAEST_192_LAMBDA);
  leaf_hash_192(com, uhash, buffer);
  memcpy(sd, buffer, FAEST_192_LAMBDA / 8);
}

static inline void faest_leaf_commit_256(uint8_t* sd, uint8_t* com, const uint8_t* key,
                                         const uint8_t* iv, uint32_t tweak, const uint8_t* uhash) {
  ATTR_ALIGNED(16) uint8_t buffer[FAEST_256_LAMBDA / 8 * 4];
  prg_4_lambda(key, iv, tweak, buffer, FAEST_256_LAMBDA);
  leaf_hash_256(com, uhash, buffer);
  memcpy(sd, buffer, FAEST_256_LAMBDA / 8);
}

// FAEST-EM.LeafCommit
static inline void faest_em_leaf_commit(uint8_t* sd, uint8_t* com, const uint8_t* key,
                                        const uint8_t* iv, uint32_t tweak, unsigned int lambda) {
  const unsigned int lambda_bytes = lambda / 8;

  memcpy(sd, key, lambda_bytes);
  prg_2_lambda(key, iv, tweak, com, lambda);
}

#if defined(FAEST_TESTS)
void leaf_commit(uint8_t* sd, uint8_t* com, const uint8_t* key, const uint8_t* iv, uint32_t tweak,
                 const uint8_t* uhash, const faest_paramset_t* params) {
  if (faest_is_em(params)) {
    faest_em_leaf_commit(sd, com, key, iv, tweak, params->lambda);
  } else {
    switch (params->lambda) {
    case 256: {
      faest_leaf_commit_256(sd, com, key, iv, tweak, uhash);
      break;
    }
    case 192: {
      faest_leaf_commit_192(sd, com, key, iv, tweak, uhash);
      break;
    }
    default: {
      faest_leaf_commit_128(sd, com, key, iv, tweak, uhash);
      break;
    }
    }
  }
}
#endif

// BAVC.PosInTree
ATTR_PURE static inline unsigned int pos_in_tree(unsigned int i, unsigned int j,
                                                 const faest_paramset_t* params) {
  const unsigned int tmp = 1 << (params->k - 1);
  if (j < tmp) {
    return params->L - 1 + params->tau * j + i;
  }
  // mod 2^(k-1) is the same as & 2^(k-1)-1
  const unsigned int mask = tmp - 1;
  return params->L - 1 + params->tau * tmp + params->tau1 * (j & mask) + i;
}

static void compute_uhash(uint8_t* uhash, size_t uhash_len, const uint8_t* iv,
                          unsigned int lambda) {
  H0_context_t uhash_ctx;
  H0_init(&uhash_ctx, lambda);
  H0_update(&uhash_ctx, iv, IV_SIZE);
  H0_final(&uhash_ctx, uhash, uhash_len);
}

static void bavc_commit_faest_128_loop(bavc_t* bavc, H1_context_t* h1_com_ctx, const uint8_t* nodes,
                                       const uint8_t* iv, const faest_paramset_t* params) {
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = FAEST_128_LAMBDA / 8;
  const unsigned int com_size     = lambda_bytes * 3; // size of com_ij

  uint8_t uhash[FAEST_128_LAMBDA / 8 * 3];
  compute_uhash(uhash, sizeof(uhash), iv, FAEST_128_LAMBDA);

  // Step: 4..5
  // compute commitments for remaining instances
  for (unsigned int i = 0, offset = 0; i < params->tau; ++i) {
    H1_context_t h1_ctx;
    H1_init(&h1_ctx, FAEST_128_LAMBDA);

    const unsigned int N_i = bavc_max_node_index(i, params->tau1, params->k);
    for (unsigned int j = 0; j < N_i; ++j, ++offset) {
      const unsigned int alpha = pos_in_tree(i, j, params);
      faest_leaf_commit_128(bavc->sd + offset * lambda_bytes, bavc->com + offset * com_size,
                            NODE(nodes, alpha, lambda_bytes), iv, i + L - 1, uhash);
      H1_update(&h1_ctx, bavc->com + offset * com_size, com_size);
    }

    uint8_t hi[FAEST_128_LAMBDA / 8 * 2];
    // Step 11
    H1_final(&h1_ctx, hi, sizeof(hi));
    // Step 12
    H1_update(h1_com_ctx, hi, sizeof(hi));
  }
}

static void bavc_commit_faest_192_loop(bavc_t* bavc, H1_context_t* h1_com_ctx, const uint8_t* nodes,
                                       const uint8_t* iv, const faest_paramset_t* params) {
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = FAEST_192_LAMBDA / 8;
  const unsigned int com_size     = lambda_bytes * 3; // size of com_ij

  uint8_t uhash[FAEST_192_LAMBDA / 8 * 3];
  compute_uhash(uhash, sizeof(uhash), iv, FAEST_192_LAMBDA);

  // Step: 4..5
  // compute commitments for remaining instances
  for (unsigned int i = 0, offset = 0; i < params->tau; ++i) {
    H1_context_t h1_ctx;
    H1_init(&h1_ctx, FAEST_192_LAMBDA);

    const unsigned int N_i = bavc_max_node_index(i, params->tau1, params->k);
    for (unsigned int j = 0; j < N_i; ++j, ++offset) {
      const unsigned int alpha = pos_in_tree(i, j, params);
      faest_leaf_commit_192(bavc->sd + offset * lambda_bytes, bavc->com + offset * com_size,
                            NODE(nodes, alpha, lambda_bytes), iv, i + L - 1, uhash);
      H1_update(&h1_ctx, bavc->com + offset * com_size, com_size);
    }

    uint8_t hi[FAEST_192_LAMBDA / 8 * 2];
    // Step 11
    H1_final(&h1_ctx, hi, sizeof(hi));
    // Step 12
    H1_update(h1_com_ctx, hi, sizeof(hi));
  }
}

static void bavc_commit_faest_256_loop(bavc_t* bavc, H1_context_t* h1_com_ctx, const uint8_t* nodes,
                                       const uint8_t* iv, const faest_paramset_t* params) {
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = FAEST_256_LAMBDA / 8;
  const unsigned int com_size     = lambda_bytes * 3; // size of com_ij

  uint8_t uhash[FAEST_256_LAMBDA / 8 * 3];
  compute_uhash(uhash, sizeof(uhash), iv, FAEST_256_LAMBDA);

  // Step: 4..5
  // compute commitments for remaining instances
  for (unsigned int i = 0, offset = 0; i < params->tau; ++i) {
    H1_context_t h1_ctx;
    H1_init(&h1_ctx, FAEST_256_LAMBDA);

    const unsigned int N_i = bavc_max_node_index(i, params->tau1, params->k);
    for (unsigned int j = 0; j < N_i; ++j, ++offset) {
      const unsigned int alpha = pos_in_tree(i, j, params);
      faest_leaf_commit_256(bavc->sd + offset * lambda_bytes, bavc->com + offset * com_size,
                            NODE(nodes, alpha, lambda_bytes), iv, i + L - 1, uhash);
      H1_update(&h1_ctx, bavc->com + offset * com_size, com_size);
    }

    uint8_t hi[FAEST_256_LAMBDA / 8 * 2];
    // Step 11
    H1_final(&h1_ctx, hi, sizeof(hi));
    // Step 12
    H1_update(h1_com_ctx, hi, sizeof(hi));
  }
}

// BAVC.Commit for FAEST
static void bavc_commit_faest(bavc_t* bavc, const uint8_t* root_key, const uint8_t* iv,
                              const faest_paramset_t* params) {
  const unsigned int lambda       = params->lambda;
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = lambda / 8;
  const unsigned int com_size     = lambda_bytes * 3; // size of com_ij

  H1_context_t h1_com_ctx;
  H1_init(&h1_com_ctx, lambda);

  // Generating the tree (k)
  uint8_t* nodes = generate_seeds(root_key, iv, params);

  // Initialzing stuff
  bavc->h   = malloc(lambda_bytes * 2); // IGNORE memory-check
  bavc->com = malloc(L * com_size); // IGNORE memory-check
  bavc->sd  = malloc(L * lambda_bytes); // IGNORE memory-check
  assert(bavc->h && bavc->com && bavc->sd);

  // Step: 1..3
  bavc->k = NODE(nodes, 0, lambda_bytes);

  assert(bavc->h);
  assert(bavc->com);
  assert(bavc->sd);
  assert(bavc->k);

  // Step: 4..5
  // compute commitments for remaining instances
  switch (lambda) {
  case 256: {
    bavc_commit_faest_256_loop(bavc, &h1_com_ctx, nodes, iv, params);
    break;
  }
  case 192: {
    bavc_commit_faest_192_loop(bavc, &h1_com_ctx, nodes, iv, params);
    break;
  }
  default: {
    bavc_commit_faest_128_loop(bavc, &h1_com_ctx, nodes, iv, params);
    break;
  }
  }

  // Step 12
  H1_final(&h1_com_ctx, bavc->h, lambda_bytes * 2);
}

// BAVC.Commit for FAEST-EM
static void bavc_commit_faest_em(bavc_t* bavc, const uint8_t* rootKey, const uint8_t* iv,
                                 const faest_paramset_t* params) {
  const unsigned int lambda       = params->lambda;
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = lambda / 8;
  const unsigned int com_size     = lambda_bytes * 2; // size of com_ij

  H1_context_t h1_com_ctx;
  H1_init(&h1_com_ctx, lambda);

  // Generating the tree
  uint8_t* nodes = generate_seeds(rootKey, iv, params);

  // Initialzing stuff
  bavc->h   = malloc(lambda_bytes * 2); // IGNORE memory-check
  bavc->com = malloc(L * com_size); // IGNORE memory-check
  bavc->sd  = malloc(L * lambda_bytes); // IGNORE memory-check
  assert(bavc->h && bavc->com && bavc->sd);

  // Step: 1..3
  bavc->k = NODE(nodes, 0, lambda_bytes);

  assert(bavc->h);
  assert(bavc->com);
  assert(bavc->sd);
  assert(bavc->k);

  // Step: 4..5
  // compute commitments for remaining instances
  for (unsigned int i = 0, offset = 0; i < params->tau; ++i) {
    H1_context_t h1_ctx;
    H1_init(&h1_ctx, lambda);

    const unsigned int N_i = bavc_max_node_index(i, params->tau1, params->k);
    for (unsigned int j = 0; j < N_i; ++j, ++offset) {
      const unsigned int alpha = pos_in_tree(i, j, params);
      faest_em_leaf_commit(bavc->sd + offset * lambda_bytes, bavc->com + offset * com_size,
                           NODE(nodes, alpha, lambda_bytes), iv, i + L - 1, lambda);
      H1_update(&h1_ctx, bavc->com + offset * com_size, com_size);
    }

    uint8_t hi[MAX_LAMBDA_BYTES * 2];
    // Step 11
    H1_final(&h1_ctx, hi, lambda_bytes * 2);
    // Step 12
    H1_update(&h1_com_ctx, hi, lambda_bytes * 2);
  }

  // Step 12
  H1_final(&h1_com_ctx, bavc->h, lambda_bytes * 2);
}

void bavc_commit(bavc_t* bavc, const uint8_t* root_key, const uint8_t* iv,
                 const faest_paramset_t* params) {
  if (faest_is_em(params)) {
    bavc_commit_faest_em(bavc, root_key, iv, params);
  } else {
    bavc_commit_faest(bavc, root_key, iv, params);
  }
}

bool bavc_open(uint8_t* decom_i, const bavc_t* vc, const uint16_t* i_delta,
               const faest_paramset_t* params) {
  const unsigned int lambda       = params->lambda;
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = lambda / 8;
  const unsigned int k            = params->k;
  const unsigned int tau          = params->tau;
  const unsigned int tau_1        = params->tau1;
  const unsigned int com_size     = faest_is_em(params) ? (2 * lambda_bytes) : (3 * lambda_bytes);

  uint8_t* decom_i_end = decom_i + com_size * tau + params->T_open * lambda_bytes;

  // Step 5
  uint8_t* s = calloc((2 * L - 1 + 7) / 8, 1); // IGNORE memory-check
  assert(s);
  // Step 6
  unsigned int nh = 0;

  // Step 7..15
  for (unsigned int i = 0; i < tau; ++i) {
    unsigned int alpha = pos_in_tree(i, i_delta[i], params);
    ptr_set_bit(s, alpha, 1);
    ++nh;

    while (alpha > 0 && ptr_get_bit(s, (alpha - 1) / 2) == 0) {
      alpha = (alpha - 1) / 2;
      ptr_set_bit(s, alpha, 1);
      ++nh;
    }
  }

  // Step 16..17
  if (nh - 2 * tau + 1 > params->T_open) {
    free(s); // IGNORE memory-check
    return false;
  }

  // Step 3
  const uint8_t* com = vc->com;
  for (unsigned int i = 0; i < tau; ++i) {
    decom_i = faest_mempcpy(decom_i, com + i_delta[i] * com_size, com_size);
    com += bavc_max_node_index(i, tau_1, k) * com_size;
  }

  // Step 19..25
  for (int i = L - 2; i >= 0; --i) {
    ptr_set_bit(s, i, ptr_get_bit(s, 2 * i + 1) | ptr_get_bit(s, 2 * i + 2));
    if ((ptr_get_bit(s, 2 * i + 1) ^ ptr_get_bit(s, 2 * i + 2)) == 1) {
      const unsigned int alpha = 2 * i + 1 + ptr_get_bit(s, 2 * i + 1);
      decom_i = faest_mempcpy(decom_i, NODE(vc->k, alpha, lambda_bytes), lambda_bytes);
    }
  }

  memset(decom_i, 0, decom_i_end - decom_i);

  free(s); // IGNORE memory-check
  return true;
}

static bool reconstruct_keys(uint8_t* s, uint8_t* keys, const uint8_t* decom_i,
                             const uint16_t* i_delta, const uint8_t* iv,
                             const faest_paramset_t* params) {
  const unsigned int lambda       = params->lambda;
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = lambda / 8;
  const unsigned int tau          = params->tau;

  const uint8_t* nodes = decom_i + (faest_is_em(params) ? 2 : 3) * tau * lambda_bytes;
  const uint8_t* end   = nodes + params->T_open * lambda_bytes;

  // Step 7..10
  for (unsigned int i = 0; i < tau; ++i) {
    unsigned int alpha = pos_in_tree(i, i_delta[i], params);
    ptr_set_bit(s, alpha, 1);
  }

  // Step 12.12
  for (int i = L - 2; i >= 0; --i) {
    ptr_set_bit(s, i, ptr_get_bit(s, 2 * i + 1) | ptr_get_bit(s, 2 * i + 2));
    if ((ptr_get_bit(s, 2 * i + 1) ^ ptr_get_bit(s, 2 * i + 2)) == 1) {
      if (nodes == end) {
        return false;
      }

      const unsigned int alpha = 2 * i + 1 + ptr_get_bit(s, 2 * i + 1);
      memcpy(keys + alpha * lambda_bytes, nodes, lambda_bytes);
      nodes += lambda_bytes;
    }
  }

  for (; nodes != end; ++nodes) {
    if (*nodes) {
      return false;
    }
  }

  for (unsigned int i = 0; i != L - 1; ++i) {
    if (!ptr_get_bit(s, i)) {
      prg_2_lambda(keys + i * lambda_bytes, iv, i, keys + (2 * i + 1) * lambda_bytes, lambda);
    }
  }

  return true;
}

static void bavc_reconstruct_faest_128_loop(bavc_rec_t* bavc_rec, H1_context_t* h1_com_ctx,
                                            const uint8_t* s, const uint8_t* keys,
                                            const uint8_t* decom_i, const uint8_t* iv,
                                            const faest_paramset_t* params) {
  // Initializing
  const unsigned int L     = params->L;
  const unsigned int k     = params->k;
  const unsigned int tau   = params->tau;
  const unsigned int tau_1 = params->tau1;

  uint8_t uhash[FAEST_128_LAMBDA / 8 * 3];
  compute_uhash(uhash, sizeof(uhash), iv, FAEST_128_LAMBDA);

  // Step 6
  for (unsigned int i = 0, offset = 0; i != tau; ++i) {
    H1_context_t h1_ctx;
    H1_init(&h1_ctx, FAEST_128_LAMBDA);

    const unsigned int N_i = bavc_max_node_index(i, tau_1, k);
    for (unsigned int j = 0; j != N_i; ++j) {
      const unsigned int alpha = pos_in_tree(i, j, params);
      if (ptr_get_bit(s, alpha)) {
        H1_update(&h1_ctx, decom_i + i * 3 * FAEST_128_LAMBDA / 8, 3 * FAEST_128_LAMBDA / 8);
      } else {
        uint8_t com[3 * FAEST_128_LAMBDA / 8];
        faest_leaf_commit_128(bavc_rec->s + offset * FAEST_128_LAMBDA / 8, com,
                              keys + alpha * FAEST_128_LAMBDA / 8, iv, i + L - 1, uhash);
        ++offset;
        H1_update(&h1_ctx, com, sizeof(com));
      }
    }

    uint8_t hi[FAEST_128_LAMBDA / 8 * 2];
    H1_final(&h1_ctx, hi, sizeof(hi));
    H1_update(h1_com_ctx, hi, sizeof(hi));
  }
}

static void bavc_reconstruct_faest_192_loop(bavc_rec_t* bavc_rec, H1_context_t* h1_com_ctx,
                                            const uint8_t* s, const uint8_t* keys,
                                            const uint8_t* decom_i, const uint8_t* iv,
                                            const faest_paramset_t* params) {
  // Initializing
  const unsigned int L     = params->L;
  const unsigned int k     = params->k;
  const unsigned int tau   = params->tau;
  const unsigned int tau_1 = params->tau1;

  uint8_t uhash[FAEST_192_LAMBDA / 8 * 3];
  compute_uhash(uhash, sizeof(uhash), iv, FAEST_192_LAMBDA);

  // Step 6
  for (unsigned int i = 0, offset = 0; i != tau; ++i) {
    H1_context_t h1_ctx;
    H1_init(&h1_ctx, FAEST_192_LAMBDA);

    const unsigned int N_i = bavc_max_node_index(i, tau_1, k);
    for (unsigned int j = 0; j != N_i; ++j) {
      const unsigned int alpha = pos_in_tree(i, j, params);
      if (ptr_get_bit(s, alpha)) {
        H1_update(&h1_ctx, decom_i + i * 3 * FAEST_192_LAMBDA / 8, 3 * FAEST_192_LAMBDA / 8);
      } else {
        uint8_t com[3 * FAEST_192_LAMBDA / 8];
        faest_leaf_commit_192(bavc_rec->s + offset * FAEST_192_LAMBDA / 8, com,
                              keys + alpha * FAEST_192_LAMBDA / 8, iv, i + L - 1, uhash);
        ++offset;
        H1_update(&h1_ctx, com, sizeof(com));
      }
    }

    uint8_t hi[FAEST_192_LAMBDA / 8 * 2];
    H1_final(&h1_ctx, hi, sizeof(hi));
    H1_update(h1_com_ctx, hi, sizeof(hi));
  }
}

static void bavc_reconstruct_faest_256_loop(bavc_rec_t* bavc_rec, H1_context_t* h1_com_ctx,
                                            const uint8_t* s, const uint8_t* keys,
                                            const uint8_t* decom_i, const uint8_t* iv,
                                            const faest_paramset_t* params) {
  // Initializing
  const unsigned int L     = params->L;
  const unsigned int k     = params->k;
  const unsigned int tau   = params->tau;
  const unsigned int tau_1 = params->tau1;

  uint8_t uhash[FAEST_256_LAMBDA / 8 * 3];
  compute_uhash(uhash, sizeof(uhash), iv, FAEST_256_LAMBDA);

  // Step 6
  for (unsigned int i = 0, offset = 0; i != tau; ++i) {
    H1_context_t h1_ctx;
    H1_init(&h1_ctx, FAEST_256_LAMBDA);

    const unsigned int N_i = bavc_max_node_index(i, tau_1, k);
    for (unsigned int j = 0; j != N_i; ++j) {
      const unsigned int alpha = pos_in_tree(i, j, params);
      if (ptr_get_bit(s, alpha)) {
        H1_update(&h1_ctx, decom_i + i * 3 * FAEST_256_LAMBDA / 8, 3 * FAEST_256_LAMBDA / 8);
      } else {
        uint8_t com[3 * FAEST_256_LAMBDA / 8];
        faest_leaf_commit_256(bavc_rec->s + offset * FAEST_256_LAMBDA / 8, com,
                              keys + alpha * FAEST_256_LAMBDA / 8, iv, i + L - 1, uhash);
        ++offset;
        H1_update(&h1_ctx, com, sizeof(com));
      }
    }

    uint8_t hi[FAEST_256_LAMBDA / 8 * 2];
    H1_final(&h1_ctx, hi, sizeof(hi));
    H1_update(h1_com_ctx, hi, sizeof(hi));
  }
}

static bool bavc_reconstruct_faest(bavc_rec_t* bavc_rec, const uint8_t* decom_i,
                                   const uint16_t* i_delta, const uint8_t* iv,
                                   const faest_paramset_t* params) {
  // Initializing
  const unsigned int lambda       = params->lambda;
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = lambda / 8;

  // Step 6
  uint8_t* s = calloc((2 * L - 1 + 7) / 8, 1); // IGNORE memory-check
  assert(s);
  uint8_t* keys = calloc(2 * params->L - 1, lambda_bytes); // IGNORE memory-check
  assert(keys);

  if (!reconstruct_keys(s, keys, decom_i, i_delta, iv, params)) {
    free(keys); // IGNORE memory-check
    free(s); // IGNORE memory-check
    return false;
  }

  H1_context_t h1_com_ctx;
  H1_init(&h1_com_ctx, lambda);

  switch (lambda) {
  case 256: {
    bavc_reconstruct_faest_256_loop(bavc_rec, &h1_com_ctx, s, keys, decom_i, iv, params);
    break;
  }
  case 192: {
    bavc_reconstruct_faest_192_loop(bavc_rec, &h1_com_ctx, s, keys, decom_i, iv, params);
    break;
  }
  default: {
    bavc_reconstruct_faest_128_loop(bavc_rec, &h1_com_ctx, s, keys, decom_i, iv, params);
    break;
  }
  }

  H1_final(&h1_com_ctx, bavc_rec->h, lambda_bytes * 2);

  free(keys); // IGNORE memory-check
  free(s); // IGNORE memory-check
  return true;
}

static bool bavc_reconstruct_faest_em(bavc_rec_t* bavc_rec, const uint8_t* decom_i,
                                      const uint16_t* i_delta, const uint8_t* iv,
                                      const faest_paramset_t* params) {
  // Initializing
  const unsigned int lambda       = params->lambda;
  const unsigned int L            = params->L;
  const unsigned int lambda_bytes = lambda / 8;
  const unsigned int k            = params->k;
  const unsigned int tau          = params->tau;
  const unsigned int tau_1        = params->tau1;
  const unsigned int com_size     = lambda_bytes * 2; // size of com_ij

  // Step 6
  uint8_t* s = calloc((2 * L - 1 + 7) / 8, 1); // IGNORE memory-check
  assert(s);
  uint8_t* keys = calloc(2 * params->L - 1, lambda_bytes); // IGNORE memory-check
  assert(keys);

  // Step 7..10
  if (!reconstruct_keys(s, keys, decom_i, i_delta, iv, params)) {
    free(keys); // IGNORE memory-check
    free(s); // IGNORE memory-check
    return false;
  }

  H1_context_t h1_com_ctx;
  H1_init(&h1_com_ctx, lambda);

  for (unsigned int i = 0, offset = 0; i != tau; ++i) {
    H1_context_t h1_ctx;
    H1_init(&h1_ctx, lambda);

    const unsigned int N_i = bavc_max_node_index(i, tau_1, k);
    for (unsigned int j = 0; j != N_i; ++j) {
      const unsigned int alpha = pos_in_tree(i, j, params);
      if (ptr_get_bit(s, alpha)) {
        H1_update(&h1_ctx, decom_i + i * com_size, com_size);
      } else {
        uint8_t com[2 * MAX_LAMBDA_BYTES];
        faest_em_leaf_commit(bavc_rec->s + offset * lambda_bytes, com, keys + alpha * lambda_bytes,
                             iv, i + L - 1, lambda);
        ++offset;
        H1_update(&h1_ctx, com, com_size);
      }
    }

    uint8_t hi[MAX_LAMBDA_BYTES * 2];
    H1_final(&h1_ctx, hi, lambda_bytes * 2);
    H1_update(&h1_com_ctx, hi, lambda_bytes * 2);
  }

  H1_final(&h1_com_ctx, bavc_rec->h, lambda_bytes * 2);

  free(keys); // IGNORE memory-check
  free(s); // IGNORE memory-check
  return true;
}

bool bavc_reconstruct(bavc_rec_t* bavc_rec, const uint8_t* decom_i, const uint16_t* i_delta,
                      const uint8_t* iv, const faest_paramset_t* params) {
  return faest_is_em(params) ? bavc_reconstruct_faest_em(bavc_rec, decom_i, i_delta, iv, params)
                             : bavc_reconstruct_faest(bavc_rec, decom_i, i_delta, iv, params);
}

void bavc_clear(bavc_t* com) {
  free(com->sd); // IGNORE memory-check
  free(com->com); // IGNORE memory-check
  free(com->h); // IGNORE memory-check
  free(com->k); // IGNORE memory-check
}
