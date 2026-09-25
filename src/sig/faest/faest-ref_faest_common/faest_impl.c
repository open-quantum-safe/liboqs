/*
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "faest_impl.h"
#include "aes.h"
#include "faest_aes.h"
#include "randomness.h"
#include "random_oracle.h"
#include "utils.h"
#include "vole.h"
#include "universal_hashing.h"

#include <string.h>

// helpers to compute position in signature (sign)

ATTR_PURE static inline uint8_t* signature_c(uint8_t* base_ptr, unsigned int index,
                                             const faest_paramset_t* params) {
  return base_ptr + index * params->ell / 8;
}

ATTR_PURE static inline uint8_t* signature_c_mult(uint8_t* base_ptr,
                                                  const faest_paramset_t* params) {
  return base_ptr + (params->tau - 1) * params->ell / 8;
}

ATTR_PURE static inline uint8_t* signature_u_tilde(uint8_t* base_ptr,
                                                   const faest_paramset_t* params) {
  const unsigned n_mult           = params->n_mult;
  const unsigned int c_mult_bytes = N_MASK * ((n_mult + 7) / 8);

  return base_ptr + (params->tau - 1) * params->ell / 8 + c_mult_bytes;
}

ATTR_PURE static inline uint8_t* signature_d(uint8_t* base_ptr, const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;
  const unsigned n_mult           = params->n_mult;
  const unsigned int c_mult_bytes = N_MASK * ((n_mult + 7) / 8);
  const unsigned int utilde_bytes = lambda_bytes * 4;

  return base_ptr + (params->tau - 1) * params->ell / 8 + c_mult_bytes + utilde_bytes;
}

ATTR_PURE static inline uint8_t* signature_a1toi_tilde(uint8_t* base_ptr,
                                                       const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;
  const unsigned n_mult           = params->n_mult;
  const unsigned int c_mult_bytes = N_MASK * ((n_mult + 7) / 8);
  const unsigned int utilde_bytes = lambda_bytes * 4;

  return base_ptr + params->tau * params->ell / 8 + c_mult_bytes + utilde_bytes;
}

ATTR_PURE static inline uint8_t* signature_decom_i(uint8_t* base_ptr,
                                                   const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;
  const unsigned n_mult           = params->n_mult;
  const unsigned int c_mult_bytes = N_MASK * ((n_mult + 7) / 8);
  const unsigned int utilde_bytes = lambda_bytes * 4;

  return base_ptr + params->tau * params->ell / 8 + c_mult_bytes + utilde_bytes +
         (D_ZK - 1) * lambda_bytes;
}

ATTR_PURE static inline uint8_t* signature_chall_3(uint8_t* base_ptr,
                                                   const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;

  return base_ptr + params->sig_size - sizeof(uint32_t) - IV_SIZE - lambda_bytes;
}

ATTR_PURE static inline uint8_t* signature_iv_pre(uint8_t* base_ptr,
                                                  const faest_paramset_t* params) {
  return base_ptr + params->sig_size - sizeof(uint32_t) - IV_SIZE;
}

ATTR_PURE static inline uint8_t* signature_ctr(uint8_t* base_ptr, const faest_paramset_t* params) {
  return base_ptr + params->sig_size - sizeof(uint32_t);
}

// helpers to compute position in signature (verify)

ATTR_PURE static inline const uint8_t* dsignature_c(const uint8_t* base_ptr, unsigned int index,
                                                    const faest_paramset_t* params) {
  return base_ptr + index * params->ell / 8;
}

ATTR_PURE static inline const uint8_t* dsignature_c_mult(const uint8_t* base_ptr,
                                                         const faest_paramset_t* params) {
  return base_ptr + (params->tau - 1) * params->ell / 8;
}

ATTR_PURE static inline const uint8_t* dsignature_u_tilde(const uint8_t* base_ptr,
                                                          const faest_paramset_t* params) {
  const unsigned n_mult           = params->n_mult;
  const unsigned int c_mult_bytes = N_MASK * ((n_mult + 7) / 8);

  return base_ptr + (params->tau - 1) * params->ell / 8 + c_mult_bytes;
}

ATTR_PURE static inline const uint8_t* dsignature_d(const uint8_t* base_ptr,
                                                    const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;
  const unsigned n_mult           = params->n_mult;
  const unsigned int c_mult_bytes = N_MASK * ((n_mult + 7) / 8);
  const unsigned int utilde_bytes = lambda_bytes * 4;

  return base_ptr + (params->tau - 1) * params->ell / 8 + c_mult_bytes + utilde_bytes;
}

ATTR_PURE static inline const uint8_t* dsignature_a1toi_tilde(const uint8_t* base_ptr,
                                                              const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;
  const unsigned n_mult           = params->n_mult;
  const unsigned int c_mult_bytes = N_MASK * ((n_mult + 7) / 8);
  const unsigned int utilde_bytes = lambda_bytes * 4;

  return base_ptr + params->tau * params->ell / 8 + c_mult_bytes + utilde_bytes;
}

ATTR_PURE static inline const uint8_t* dsignature_decom_i(const uint8_t* base_ptr,
                                                          const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;
  const unsigned n_mult           = params->n_mult;
  const unsigned int c_mult_bytes = N_MASK * ((n_mult + 7) / 8);
  const unsigned int utilde_bytes = lambda_bytes * 4;

  return base_ptr + params->tau * params->ell / 8 + c_mult_bytes + utilde_bytes +
         (D_ZK - 1) * lambda_bytes;
}

ATTR_PURE static inline const uint8_t* dsignature_chall_3(const uint8_t* base_ptr,
                                                          const faest_paramset_t* params) {
  const unsigned int lambda_bytes = params->lambda / 8;

  return base_ptr + params->sig_size - sizeof(uint32_t) - IV_SIZE - lambda_bytes;
}

ATTR_PURE static inline const uint8_t* dsignature_iv_pre(const uint8_t* base_ptr,
                                                         const faest_paramset_t* params) {
  return base_ptr + params->sig_size - sizeof(uint32_t) - IV_SIZE;
}

ATTR_PURE static inline const uint8_t* dsignature_ctr(const uint8_t* base_ptr,
                                                      const faest_paramset_t* params) {
  return base_ptr + params->sig_size - sizeof(uint32_t);
}

// FAEST.Sign: line 3
static void hash_mu(uint8_t* mu, const uint8_t* owf_input, size_t owf_input_size,
                    const uint8_t* owf_output, size_t owf_output_size, const uint8_t* msg,
                    size_t msglen, unsigned int lambda) {
  H2_context_t h1_ctx;
  H2_init(&h1_ctx, lambda);
  H2_update(&h1_ctx, owf_input, owf_input_size);
  H2_update(&h1_ctx, owf_output, owf_output_size);
  H2_update(&h1_ctx, msg, msglen);
  H2_0_final(&h1_ctx, mu, lambda / 4); // 2 * lambda_bytes
}

ATTR_ALWAYS_INLINE ATTR_ARTIFICIAL static inline void
hash_iv(uint8_t* iv, const uint8_t* iv_pre, const uint8_t* mu, unsigned int lambda) {
  H4(iv, iv_pre, mu, lambda);
}

// FAEST.Sign: line 4 + line 5
static void hash_r_iv(uint8_t* root_key, uint8_t* iv_pre, uint8_t* iv, const uint8_t* owf_key,
                      const uint8_t* mu, const uint8_t* rho, size_t rho_size, unsigned int lambda) {
  const unsigned int lambda_bytes = lambda / 8;

  {
    H3_context_t h3_ctx;
    H3_init(&h3_ctx, lambda);
    H3_update(&h3_ctx, owf_key, lambda_bytes);
    H3_update(&h3_ctx, mu, lambda_bytes * 2);
    if (rho && rho_size) {
      H3_update(&h3_ctx, rho, rho_size);
    }
    H3_final(&h3_ctx, root_key, lambda_bytes, iv_pre);
  }

  hash_iv(iv, iv_pre, mu, lambda);
}

static void hash_challenge_1(uint8_t* chall_1, const uint8_t* mu, const uint8_t* hcom,
                             const uint8_t* c, const uint8_t* c_mult_packed, const uint8_t* iv,
                             unsigned int lambda, unsigned int ell, unsigned int tau,
                             unsigned int c_mult_size_bytes) {
  const unsigned int lambda_bytes = lambda / 8;
  const unsigned int ell_bytes    = ell / 8;

  H2_context_t h2_ctx;
  H2_init(&h2_ctx, lambda);
  H2_update(&h2_ctx, mu, lambda_bytes * 2);
  H2_update(&h2_ctx, hcom, lambda_bytes * 2);
  H2_update(&h2_ctx, c, ell_bytes * (tau - 1));
  H2_update(&h2_ctx, c_mult_packed, c_mult_size_bytes);
  H2_update(&h2_ctx, iv, IV_SIZE);
  H2_1_final(&h2_ctx, chall_1, 8 * lambda_bytes + 8);
}

static void hash_challenge_2_init(H2_context_t* h2_ctx, const uint8_t* chall_1,
                                  const uint8_t* u_tilde, unsigned int lambda) {
  const unsigned int lambda_bytes  = lambda / 8;
  const unsigned int u_tilde_bytes = lambda_bytes * 4;

  H2_init(h2_ctx, lambda);
  H2_update(h2_ctx, chall_1, 8 * lambda_bytes + 8);
  H2_update(h2_ctx, u_tilde, u_tilde_bytes);
}

static void hash_challenge_2_update_v_tilde(H2_context_t* h2_ctx, const uint8_t* v_tilde,
                                            unsigned int lambda) {
  const unsigned int lambda_bytes  = lambda / 8;
  const unsigned int v_tilde_bytes = lambda_bytes * 4;

  H2_update(h2_ctx, v_tilde, v_tilde_bytes);
}

static void hash_challenge_2_finalize(uint8_t* chall_2, H2_context_t* h2_ctx, const uint8_t* d,
                                      const unsigned lambda, unsigned int ell) {
  const unsigned int lambda_bytes = lambda / 8;
  const unsigned int ell_bytes    = ell / 8;

  H2_update(h2_ctx, d, ell_bytes);
  H2_2_final(h2_ctx, chall_2, 3 * lambda_bytes + 8);
}

static void hash_challenge_3_init(H2_context_t* h2_ctx, const uint8_t* chall_2,
                                  const uint8_t* a0_tilde, const uint8_t* a1toi_tilde,
                                  unsigned int lambda) {
  const unsigned int lambda_bytes = lambda / 8;

  H2_init(h2_ctx, lambda);
  H2_update(h2_ctx, chall_2, 3 * lambda_bytes + 8);

  H2_update(h2_ctx, a0_tilde, lambda_bytes);
  H2_update(h2_ctx, a1toi_tilde, (D_ZK - 1) * lambda_bytes);
}

static void hash_challenge_3_final(uint8_t* chall_3, const H2_context_t* ctx, uint32_t ctr,
                                   unsigned int lambda) {
  const unsigned int lambda_bytes = lambda / 8;

  H2_context_t ctx_copy;
  H2_copy(&ctx_copy, ctx);
  H2_update_u32_le(&ctx_copy, ctr);
  H2_3_final(&ctx_copy, chall_3, lambda_bytes);
}

static void hash_challenge_3(uint8_t* chall_3, const uint8_t* chall_2, const uint8_t* a0_tilde,
                             const uint8_t* a1toi_tilde, const uint8_t* ctr, unsigned int lambda) {
  H2_context_t h2_ctx;
  hash_challenge_3_init(&h2_ctx, chall_2, a0_tilde, a1toi_tilde, lambda);
  H2_update(&h2_ctx, ctr, sizeof(uint32_t));
  H2_3_final(&h2_ctx, chall_3, lambda / 8);
}

static inline bool check_challenge_3(const uint8_t* chall_3, unsigned int start,
                                     unsigned int lambda) {
  const unsigned int lambda_bytes = lambda / 8;

  uint8_t res = (start % 8) ? (chall_3[start / 8] >> (start % 8)) : 0;
  for (unsigned int bytes = (start + 7) / 8; bytes != lambda_bytes; ++bytes) {
    res |= chall_3[bytes];
  }
  return !res;
}

// AES(-EM) OWF dispatchers
static inline void aes_prove(uint8_t* a0_tilde, uint8_t* a1toi_tilde, const uint8_t* w, uint8_t** V,
                             const uint8_t* u_bar, const uint8_t* v_bar, const uint8_t* owf_in,
                             const uint8_t* owf_out, const uint8_t* chall_2,
                             const faest_paramset_t* params) {
  switch (params->lambda) {
  case 256:
    aes_256_prover(a0_tilde, a1toi_tilde, w, V, u_bar, v_bar, owf_in, owf_out, chall_2, params);
    break;
  case 192:
    aes_192_prover(a0_tilde, a1toi_tilde, w, V, u_bar, v_bar, owf_in, owf_out, chall_2, params);
    break;
  default:
    aes_128_prover(a0_tilde, a1toi_tilde, w, V, u_bar, v_bar, owf_in, owf_out, chall_2, params);
  }
}

static inline void aes_verify(uint8_t* a0_tilde, const uint8_t* d, uint8_t** Q,
                              const uint8_t* q_bar, const uint8_t* owf_in, const uint8_t* owf_out,
                              const uint8_t* chall_2, const uint8_t* chall_3,
                              const uint8_t* a1toi_tilde, const faest_paramset_t* params) {
  switch (params->lambda) {
  case 256:
    aes_256_verifier(a0_tilde, d, Q, q_bar, owf_in, owf_out, chall_2, chall_3, a1toi_tilde, params);
    break;
  case 192:
    aes_192_verifier(a0_tilde, d, Q, q_bar, owf_in, owf_out, chall_2, chall_3, a1toi_tilde, params);
    break;
  default:
    aes_128_verifier(a0_tilde, d, Q, q_bar, owf_in, owf_out, chall_2, chall_3, a1toi_tilde, params);
  }
}

static inline void compute_D(uint8_t* D, const uint8_t* Q_tilde, const uint8_t* delta,
                             const uint8_t* u_tilde, const faest_paramset_t* params) {
  unsigned int lambda_bytes = params->lambda / 8;
  switch (params->lambda) {
  case 256: {
    bf256_t bf_delta;
    bf256_load(&bf_delta, delta);
    for (unsigned int i = 0; i < 4; i++) {
      bf256_t bf_Q_tilde;
      bf256_t bf_u_tilde;
      bf256_load(&bf_Q_tilde, Q_tilde + i * lambda_bytes);
      bf256_load(&bf_u_tilde, u_tilde + i * lambda_bytes);
      bf256_mul_inplace(&bf_u_tilde, &bf_delta);
      bf256_add_inplace(&bf_Q_tilde, &bf_u_tilde);
      bf256_store(D + i * lambda_bytes, &bf_Q_tilde);
    }
    break;
  }
  case 192: {
    bf192_t bf_delta;
    bf192_load(&bf_delta, delta);
    for (unsigned int i = 0; i < 4; i++) {
      bf192_t bf_Q_tilde;
      bf192_t bf_u_tilde;
      bf192_load(&bf_Q_tilde, Q_tilde + i * lambda_bytes);
      bf192_load(&bf_u_tilde, u_tilde + i * lambda_bytes);
      bf192_mul_inplace(&bf_u_tilde, &bf_delta);
      bf192_add_inplace(&bf_Q_tilde, &bf_u_tilde);
      bf192_store(D + i * lambda_bytes, &bf_Q_tilde);
    }
    break;
  }
  default: {
    bf128_t bf_delta;
    bf128_load(&bf_delta, delta);
    for (unsigned int i = 0; i < 4; i++) {
      bf128_t bf_Q_tilde;
      bf128_t bf_u_tilde;
      bf128_load(&bf_Q_tilde, Q_tilde + i * lambda_bytes);
      bf128_load(&bf_u_tilde, u_tilde + i * lambda_bytes);
      bf128_mul_inplace(&bf_u_tilde, &bf_delta);
      bf128_add_inplace(&bf_Q_tilde, &bf_u_tilde);
      bf128_store(D + i * lambda_bytes, &bf_Q_tilde);
    }
    break;
  }
  }
}

// FAEST.Sign()
void faest_sign(uint8_t* sig, const uint8_t* msg, size_t msg_len, const uint8_t* owf_key,
                const uint8_t* owf_input, const uint8_t* owf_output, const uint8_t* witness,
                const uint8_t* rho, size_t rholen, const faest_paramset_t* params) {
  const unsigned int ell           = params->ell;
  const unsigned int ell_bytes     = params->ell / 8;
  const unsigned int lambda        = params->lambda;
  const unsigned int lambda_bytes  = lambda / 8;
  const unsigned int n_mult_bytes  = (params->n_mult + 7) / 8;
  const unsigned int ell_hat       = ell + N_MASK * bavc_max_node_depth(0, params->tau1, params->k);
  const unsigned int ell_hat_bytes = (ell_hat + 7) / 8;
  const unsigned int c_mult_bytes  = N_MASK * n_mult_bytes;

  // line 2
  uint8_t mu[MAX_LAMBDA_BYTES * 2];
  hash_mu(mu, owf_input, params->owf_input_size, owf_output, params->owf_output_size, msg, msg_len,
          lambda);

  // line 3
  uint8_t rootkey[MAX_LAMBDA_BYTES], iv[IV_SIZE];
  hash_r_iv(rootkey, signature_iv_pre(sig, params), iv, owf_key, mu, rho, rholen, lambda);

  // line 6
  bavc_t bavc;
  uint8_t* u = malloc(ell_bytes); // IGNORE memory-check
  assert(u);

  // Store u_bar and v_bar in V_row so that they can be hashed togeter. Also saves additional memory
  // allocations.
  uint8_t** V_row = alloc_pointer_array(ell + 2 * N_MASK, lambda_bytes);
  uint8_t* v_bar  = V_row[ell];
  uint8_t* u_bar  = V_row[ell + N_MASK];
  // v has \hat \ell rows, \lambda columns, storing in column-major order
  // it is actually lambda - w_grind but keeping it lambda
  uint8_t** V = alloc_pointer_array(lambda, ell_hat_bytes);

  vole_commit(rootkey, iv, ell_hat, params, &bavc, signature_c(sig, 0, params),
              signature_c_mult(sig, params), u, V, u_bar, v_bar);

  // it is actually lambda - w_grind but keeping it lambda
  transpose_matrix(V, V_row, lambda, ell);
  free_pointer_array(&V);

  H2_context_t h2_ctx;
  {
    // line 7
    uint8_t chall_1[(8 * MAX_LAMBDA_BYTES) + 8] = {0};
    hash_challenge_1(chall_1, mu, bavc.h, signature_c(sig, 0, params),
                     signature_c_mult(sig, params), signature_iv_pre(sig, params), lambda, ell,
                     params->tau, c_mult_bytes);

    // line 9
    const unsigned int uh_0_bytes = lambda_bytes * (ell + D_ZK - 1);
    // for scan-build
    assert(uh_0_bytes + lambda_bytes * 4 > 0);

    uint8_t* uh   = malloc(uh_0_bytes + lambda_bytes * 4); // IGNORE memory-check
    uint8_t* uh_0 = uh;

    memset(uh_0, 0, uh_0_bytes);
    for (unsigned int bit_idx = 0; bit_idx < ell; ++bit_idx) {
      // [u[0],..0 lambda times, u[1], 0 lambda times, ...]
      uh_0[bit_idx * lambda_bytes] = ptr_get_bit(u, bit_idx);
    }
    memcpy(uh_0 + lambda_bytes * ell, u_bar, (D_ZK - 1 + 4) * lambda_bytes);

    // line 8, line 11, line 12, line 13
    vole_hash(signature_u_tilde(sig, params), chall_1, uh, ell, lambda);
    free(uh); // IGNORE memory-check

    // To save memory consumption, the chall_2 is computed in an
    // Init-Update-Finalize style as V_tilde is only fed into to the hash and not
    // used elsewhere.
    hash_challenge_2_init(&h2_ctx, chall_1, signature_u_tilde(sig, params), lambda);
    {
      // line 14
      uint8_t V_tilde[MAX_LAMBDA_BYTES * 4] = {0};
      // V_row[0] contains V transposed, v_bar and u_bar
      vole_hash(V_tilde, chall_1, V_row[0], ell, lambda);

      // line 20
      hash_challenge_2_update_v_tilde(&h2_ctx, V_tilde, lambda);
    }
  }

  // line 19
  xor_u8_array(signature_d(sig, params), witness, u, ell_bytes);

  {
    // line 20
    uint8_t chall_2[(3 * MAX_LAMBDA_BYTES) + 8];
    hash_challenge_2_finalize(chall_2, &h2_ctx, signature_d(sig, params), lambda, ell);

    // line 22
    uint8_t a0_tilde[MAX_LAMBDA_BYTES];
    aes_prove(a0_tilde, signature_a1toi_tilde(sig, params), witness, V_row, u_bar, v_bar, owf_input,
              owf_output, chall_2, params);

    free_pointer_array(&V_row);
    free(u); // IGNORE memory-check
    u = NULL;

    // line 26
    hash_challenge_3_init(&h2_ctx, chall_2, a0_tilde, signature_a1toi_tilde(sig, params), lambda);
  }

  uint32_t ctr = 0;
  for (; true; ++ctr) {
    uint8_t* chall_3 = signature_chall_3(sig, params);
    hash_challenge_3_final(chall_3, &h2_ctx, ctr, lambda);

    // declassify chall_3 which is put into the signature
    faest_declassify(chall_3, lambda / 8);

    // line 27
    if (!check_challenge_3(chall_3, lambda - params->w_grind, lambda)) {
      continue;
    }

    // line 29
    uint16_t decoded_chall_3[MAX_TAU];
    if (!decode_all_chall_3(decoded_chall_3, chall_3, params)) {
      continue;
    }

    // line 30
    if (bavc_open(signature_decom_i(sig, params), &bavc, decoded_chall_3, params)) {
      break;
    }
  }

  hash_clear(&h2_ctx);
  bavc_clear(&bavc);

  // copy counter to signature
  ctr = htole32(ctr);
  memcpy(signature_ctr(sig, params), &ctr, sizeof(ctr));
}

int faest_verify(const uint8_t* msg, size_t msglen, const uint8_t* sig, const uint8_t* owf_input,
                 const uint8_t* owf_output, const faest_paramset_t* params) {
  const unsigned int ell           = params->ell;
  const unsigned int lambda        = params->lambda;
  const unsigned int lambda_bytes  = lambda / 8;
  const unsigned int n_mult_bytes  = (params->n_mult + 7) / 8;
  const unsigned int ell_hat       = ell + N_MASK * bavc_max_node_depth(0, params->tau1, params->k);
  const unsigned int ell_hat_bytes = (ell_hat + 7) / 8;
  const unsigned int c_mult_bytes  = N_MASK * n_mult_bytes;

  // line 3
  if (!check_challenge_3(dsignature_chall_3(sig, params), lambda - params->w_grind, lambda)) {
    return -1;
  }

  // line 1
  uint8_t mu[MAX_LAMBDA_BYTES * 2];
  hash_mu(mu, owf_input, params->owf_input_size, owf_output, params->owf_output_size, msg, msglen,
          lambda);

  // line 2
  uint8_t iv[IV_SIZE];
  hash_iv(iv, dsignature_iv_pre(sig, params), mu, lambda);

  // line 6
  // q is a \hat \ell \times \lambda matrix
  uint8_t** Q = alloc_pointer_array(lambda, ell_hat_bytes);

  // Keep q_bar directly after the transposed Q so they can be hashed together.
  uint8_t** Q_row = alloc_pointer_array(ell + N_MASK, lambda_bytes);
  uint8_t* q_bar  = Q_row[ell];
  uint8_t hcom[MAX_LAMBDA_BYTES * 2];
  uint8_t Delta[MAX_LAMBDA_BYTES] = {0};
  if (!vole_reconstruct(hcom, Q, iv, dsignature_chall_3(sig, params),
                        dsignature_decom_i(sig, params), dsignature_c(sig, 0, params),
                        dsignature_c_mult(sig, params), q_bar, Delta, ell_hat, params)) {
    free_pointer_array(&Q_row);
    free_pointer_array(&Q);
    return -1;
  }

  // it is actually lambda - w_grind but keeping it lambda
  transpose_matrix(Q, Q_row, lambda, ell);
  free_pointer_array(&Q);

  // line 9
  uint8_t chall_1[(8 * MAX_LAMBDA_BYTES) + 8] = {0};
  hash_challenge_1(chall_1, mu, hcom, dsignature_c(sig, 0, params), dsignature_c_mult(sig, params),
                   dsignature_iv_pre(sig, params), lambda, ell, params->tau, c_mult_bytes);

  // line 10
  H2_context_t chall_2_ctx;
  hash_challenge_2_init(&chall_2_ctx, chall_1, dsignature_u_tilde(sig, params), lambda);
  {
    // line 13
    uint8_t Q_tilde[MAX_LAMBDA_BYTES * 4] = {0};
    vole_hash(Q_tilde, chall_1, Q_row[0], ell, lambda);

    compute_D(Q_tilde, Q_tilde, Delta, dsignature_u_tilde(sig, params), params);

    // line 15
    hash_challenge_2_update_v_tilde(&chall_2_ctx, Q_tilde, lambda);
  }

  // line 16
  // chall_1 is no longer needed, so reuse it as chall_2
  hash_challenge_2_finalize(chall_1, &chall_2_ctx, dsignature_d(sig, params), lambda, ell);

  // line 18
  uint8_t a0_tilde[MAX_LAMBDA_BYTES];
  aes_verify(a0_tilde, dsignature_d(sig, params), Q_row, q_bar, owf_input, owf_output, chall_1,
             Delta, dsignature_a1toi_tilde(sig, params), params);
  free_pointer_array(&Q_row);

  // line 19
  // chall_1 is no longer needed, re reuse it as chall_3
  hash_challenge_3(chall_1, chall_1, a0_tilde, dsignature_a1toi_tilde(sig, params),
                   dsignature_ctr(sig, params), lambda);

  // Step 21
  return memcmp(chall_1, dsignature_chall_3(sig, params), lambda_bytes) == 0 ? 0 : -1;
}
