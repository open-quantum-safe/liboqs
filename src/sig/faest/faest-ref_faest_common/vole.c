/*
 *  SPDX-License-Identifier: MIT
 */

#if defined(HAVE_CONFIG_H)
#include <config.h>
#endif

#include "vole.h"

#include "compat.h"
#include "aes.h"
#include "utils.h"
#include "random_oracle.h"
#include "fields.h"

#include <stdbool.h>
#include <string.h>

// for calls to H5 where storage if re-used
static_assert(N_MASK_BYTES <= MAX_LAMBDA_BYTES, "Invalid assumptions.");
static_assert(MAX_DEPTH <= 16, "Invalid assumptions.");

static const uint32_t TWEAK_OFFSET = UINT32_C(0x80000000); // 2^31

#define TBL(base, words, i) ((base) + (size_t)(i) * (words))

static void insert_bit_slice(uint8_t* dst, unsigned int offset, uint16_t value, unsigned int bits) {
  const unsigned int bit_offset = offset & 7;
  const unsigned int byte_idx   = offset >> 3;
  const uint32_t shifted        = (uint32_t)value << bit_offset;
  assert(bits > 0 && bits <= 16);

  dst[byte_idx] |= (uint8_t)shifted;
  if (bit_offset + bits > 8) {
    dst[byte_idx + 1] |= (uint8_t)(shifted >> 8);
  }
  if (bit_offset + bits > 16) {
    dst[byte_idx + 2] |= (uint8_t)(shifted >> 16);
  }
}

static inline uint64_t load_bit_word(const uint8_t* src, unsigned int bits) {
  assert(bits > 0 && bits <= 64);

  uint64_t value = 0;
  memcpy(&value, src, (bits + 7) / 8);
  return le64toh(value) & bit_word_mask(bits);
}

/**
 * Reduce a packed GF(2) polynomial modulo a tree polynomial. Bit i represents
 * the coefficient of x^i. Tree degrees are at most MAX_DEPTH (currently 12), so
 * the input fits in a uint32_t and the remainder in a uint16_t.
 *
 * This replaces a more generic reduction of a GF(2) polynomial followed by
 * extraction of the low degree bits in VOLE.
 */
#if !defined(FAEST_TESTS)
static
#endif
    uint16_t reduce_tree_product(uint32_t value, uint16_t modulus, unsigned int degree) {
  for (unsigned int bit = 2 * degree; bit > degree; --bit) {
    const uint32_t mask = -((value >> (bit - 1)) & UINT32_C(1));
    value ^= ((uint32_t)modulus << (bit - degree - 1)) & mask;
  }
  return (uint16_t)value;
}

static void crt_transform_rows(uint8_t** dst, uint8_t** src, unsigned int row_bytes,
                               unsigned int rows, const faest_paramset_t* params) {
  for (unsigned int out = 0; out < rows; ++out) {
    memset(dst[out], 0, row_bytes);
    const uint64_t* coefficients = TBL(params->W_CRT, params->w_crt_words, out);
    for (unsigned int word = 0; word < (rows + 63) / 64; ++word) {
      // bits is a public constant
      for (uint64_t bits = coefficients[word] & bit_word_mask(MIN(rows - word * 64, 64)); bits;
           bits &= bits - 1) {
        const unsigned int bit = ctz64(bits);
        xor_u8_array_inplace(dst[out], src[word * 64 + bit], row_bytes);
      }
    }
  }
}

static void transpose_bit_matrix(uint8_t** dst, const uint8_t* src, unsigned int rows,
                                 unsigned int cols) {
  const unsigned int src_row_bytes = (cols + 7) / 8;

  uint8_t** src_rows = faest_malloc(rows * sizeof(*src_rows));
  for (unsigned int row = 0; row < rows; ++row) {
    src_rows[row] = (uint8_t*)src + row * src_row_bytes;
  }

  transpose_matrix(src_rows, dst, rows, cols);
  faest_free(src_rows);
}

static unsigned int convert_to_vole_with_scratch(const uint8_t* iv, const uint8_t* sd, bool sd0_bot,
                                                 unsigned int i, unsigned int outlen, uint8_t* u,
                                                 uint8_t* v, const faest_paramset_t* params,
                                                 uint8_t* r) {
  const unsigned int lambda        = params->lambda;
  const unsigned int tau_1         = params->tau1;
  const unsigned int k             = params->k;
  const unsigned int num_instances = bavc_max_node_index(i, tau_1, k);
  const unsigned int lambda_bytes  = lambda / 8;
  const unsigned int depth         = bavc_max_node_depth(i, tau_1, k);

#define R(column) (r + (column) * outlen)
#define V(idx) (v + (idx) * outlen)

  uint32_t tweak = i ^ TWEAK_OFFSET;

  // Step: 2
  if (!sd0_bot) {
    prg(sd, iv, tweak, R(0), lambda, outlen);
  } else {
    memset(R(0), 0, outlen);
  }

  // Step: 3..4
  for (unsigned int j = 1; j < num_instances; ++j) {
    prg(sd + lambda_bytes * j, iv, tweak, R(j), lambda, outlen);
  }

  // Step: 5..9. Ascending pairs allow each level to overwrite its input safely.
  memset(v, 0, depth * outlen);
  for (unsigned int j = 0; j < depth; j++) {
    unsigned int depthloop = num_instances >> (j + 1);
    for (unsigned int idx = 0; idx < depthloop; idx++) {
      xor_u8_array_inplace(V(j), R(2 * idx + 1), outlen);
      xor_u8_array(R(idx), R(2 * idx), R(2 * idx + 1), outlen);
    }
  }
  // Step: 10
  if (!sd0_bot && u != NULL) {
    memcpy(u, R(0), outlen);
  }
  return depth;
}

#undef R
#undef V

#if defined(FAEST_TESTS)
unsigned int convert_to_vole(const uint8_t* iv, const uint8_t* sd, bool sd0_bot, unsigned int i,
                             unsigned int outlen, uint8_t* u, uint8_t* v,
                             const faest_paramset_t* params) {
  uint8_t* scratch = faest_calloc(bavc_max_node_index(i, params->tau1, params->k), outlen);
  assert(scratch);
  const unsigned int depth =
      convert_to_vole_with_scratch(iv, sd, sd0_bot, i, outlen, u, v, params, scratch);
  faest_free(scratch);
  return depth;
}
#endif

void vole_commit(const uint8_t* rootKey, const uint8_t* iv, unsigned int ellhat,
                 const faest_paramset_t* params, bavc_t* bavc, uint8_t* c, uint8_t* c_mult,
                 uint8_t* u, uint8_t** V_dest, uint8_t* u_bar, uint8_t* v_bar) {
  const unsigned int lambda                     = params->lambda;
  const unsigned int lambda_bytes               = lambda / 8;
  const unsigned int ell_hat_bytes              = (ellhat + 7) / 8;
  const unsigned int ell                        = params->ell;
  const unsigned int ell_bytes                  = ell / 8;
  const unsigned int tau                        = params->tau;
  const unsigned int tau_1                      = params->tau1;
  const unsigned int k                          = params->k;
  const unsigned int n_mult                     = params->n_mult;
  const unsigned int n_mult_bytes               = (n_mult + 7) / 8;
  const unsigned int w_grind                    = params->w_grind;
  const unsigned int w_grind_bytes              = (w_grind + 7) / 8;
  const unsigned int lambda_minus_w_grind       = lambda - w_grind;
  const unsigned int lambda_minus_w_grind_bytes = ((lambda_minus_w_grind) + 7) / 8;

  // line 1
  bavc_commit(bavc, rootKey, iv, params);

  uint8_t* ui = faest_malloc(tau * ell_hat_bytes);
  assert(ui);

  uint8_t** V = alloc_pointer_array(lambda, ell_hat_bytes);

  uint8_t* scratch = faest_calloc(bavc_max_node_index(0, tau_1, k), ell_hat_bytes);
  assert(scratch);

  // line 3
  unsigned int v_idx = 0;
  uint8_t* sd_i      = bavc->sd;
  for (unsigned int i = 0; i < tau; ++i) {
    v_idx += convert_to_vole_with_scratch(iv, sd_i, false, i, ell_hat_bytes, ui + i * ell_hat_bytes,
                                          V[v_idx], params, scratch);
    sd_i += lambda_bytes * bavc_max_node_index(i, tau_1, k);
  }

  faest_free(scratch);
  const unsigned int mask_bits = ellhat - ell;
  uint8_t** V_columns          = alloc_pointer_array(mask_bits, lambda_minus_w_grind_bytes);
  uint8_t* mask_rows[MAX_LAMBDA];
  for (unsigned int row = 0; row < lambda_minus_w_grind; ++row) {
    mask_rows[row] = V[row] + ell_bytes;
  }
  transpose_matrix(mask_rows, V_columns, lambda_minus_w_grind, mask_bits);

  // line 6
  memcpy(u, ui, ell_bytes);

  // line 8
  for (unsigned int i = 1; i < tau; i++) {
    xor_u8_array(c + (i - 1) * ell_bytes, u, ui + i * ell_hat_bytes, ell_bytes);
  }

  // line 10
  uint8_t* u_hi_all = faest_malloc((N_MASK * w_grind + 7) / 8);
  uint8_t* u_hi     = faest_calloc(N_MASK, w_grind_bytes);
  prg(rootKey, iv, TWEAK_OFFSET - 1, u_hi_all, lambda, (N_MASK * w_grind + 7) / 8);

  for (unsigned int n_mask_idx = 0; n_mask_idx < N_MASK; ++n_mask_idx) {
    insert_bit_slice(u_hi + n_mask_idx * w_grind_bytes, 0,
                     extract_bit_slice(u_hi_all, n_mask_idx * w_grind, w_grind), w_grind);
  }

  // line 11
  uint8_t* u_low   = faest_calloc(N_MASK, lambda_bytes);
  uint8_t* r_tilde = faest_calloc(N_MASK, lambda_minus_w_grind_bytes);
  for (unsigned m = 0; m < N_MASK; ++m) {
    unsigned int ell_m   = m * bavc_max_node_depth(0, tau_1, k);
    unsigned int bit_idx = 0;

    // line 13
    for (unsigned t = 0; t < tau; t++) {
      const unsigned int depth = bavc_max_node_depth(t, tau_1, k);
      insert_bit_slice(u_low + m * lambda_bytes, bit_idx,
                       extract_bit_slice(ui + t * ell_hat_bytes, ell + ell_m, depth), depth);
      bit_idx += depth;
    }

    // line 14
    uint8_t first_prod[MAX_LAMBDA_BYTES] = {0};
    bf2_matrix_mul_tbl(first_prod, u_low + m * lambda_bytes, params->W_CRT, lambda_minus_w_grind,
                       lambda, params->w_crt_words);
    uint8_t second_prod[MAX_LAMBDA_BYTES] = {0};
    bf2_poly_mul(second_prod, &u_hi[m * w_grind_bytes], w_grind, params->M_TREE,
                 lambda_minus_w_grind + 1);
    xor_u8_array(u_bar + m * lambda_bytes, first_prod, second_prod, lambda_bytes);

    // line 17
    bit_idx = 0;
    v_idx   = 0;
    for (unsigned int tau_idx = 0; tau_idx < tau; tau_idx++) {
      uint8_t deg = bavc_max_node_depth(tau_idx, tau_1, k);

      uint32_t acc_value = 0;
      for (unsigned int d = 0; d < deg; ++d) {
        acc_value ^= (uint32_t)extract_bit_slice(V_columns[ell_m + d], v_idx, deg) << d;
      }
      v_idx += deg;

      const uint16_t reduced = reduce_tree_product(acc_value, params->TREE_MODULI[tau_idx], deg);
      insert_bit_slice(r_tilde + m * lambda_minus_w_grind_bytes, bit_idx, reduced, deg);
      bit_idx += deg;
    }
  }

  // line 19
  // a is a copy of V, so directly access V

  // line 21
  for (unsigned m = 0; m < N_MASK; m++) {
    uint8_t* c_mult_m = c_mult + m * n_mult_bytes;
    // The matrix multiply writes every valid bit; clear only unused padding bits.
    if (n_mult % 8) {
      c_mult_m[n_mult_bytes - 1] = 0;
    }
    bf2_matrix_mul_tbl(c_mult_m, u_bar + m * lambda_bytes, params->F, lambda, n_mult,
                       params->f_words);
  }

  uint8_t** v_tilde     = alloc_pointer_array(n_mult, N_MASK_BYTES);
  uint8_t** c_mult_diff = alloc_pointer_array(n_mult, N_MASK_BYTES);
  for (unsigned e = 0; e < n_mult; e++) {
    uint8_t L_e_zero[MAX_LAMBDA_BYTES] = {0};
    const uint64_t* G_e                = TBL(params->G, params->g_words, e);
    for (unsigned int word_idx = 0; word_idx < params->g_words; ++word_idx) {
      const unsigned int row_offset = word_idx * 64;
      const unsigned int word_bits  = MIN(lambda_minus_w_grind - row_offset, 64);

      // coefficients is a public constant
      for (uint64_t coefficients = G_e[word_idx] & bit_word_mask(word_bits); coefficients;
           coefficients &= coefficients - 1) {
        const unsigned int bit = ctz64(coefficients);
        xor_small_u8_array_inplace(L_e_zero, V[row_offset + bit], lambda_bytes);
      }
    }

    // line 23
    H5(v_tilde[e], iv, e, L_e_zero, lambda);

    // reuse L_e_zero as L_e_one
    xor_u8_array_inplace(L_e_zero, u, lambda_bytes);
    // reuse L_e_zero as h_e_one
    H5(L_e_zero, iv, e, L_e_zero, lambda);

    // line 26
    xor_small_u8_array(c_mult_diff[e], v_tilde[e], L_e_zero, N_MASK_BYTES);
  }

  // line 30: apply the CRT map row-wise, including the final transpose implicitly.
  crt_transform_rows(V_dest, V, ell_bytes, lambda_minus_w_grind, params);
  free_pointer_array(&V);

  // bit-packed version of line 26
  uint8_t** gate_inputs = alloc_pointer_array(N_MASK, n_mult_bytes);
  assert(gate_inputs);
  transpose_matrix(c_mult_diff, gate_inputs, n_mult, N_MASK);
  xor_u8_array_inplace(c_mult, gate_inputs[0], N_MASK * n_mult_bytes);
  free_pointer_array(&c_mult_diff);

  transpose_matrix(v_tilde, gate_inputs, n_mult, N_MASK);
  free_pointer_array(&v_tilde);

  // line 29
  for (unsigned int m = 0; m < N_MASK; m++) {
    uint8_t first_prod[MAX_LAMBDA_BYTES] = {0};
    bf2_matrix_mul_tbl(first_prod, r_tilde + m * lambda_minus_w_grind_bytes, params->W_TREE,
                       lambda_minus_w_grind, lambda, params->w_tree_words);

    uint8_t second_prod[MAX_LAMBDA_BYTES] = {0};
    bf2_matrix_mul_tbl(second_prod, gate_inputs[m], params->W_GATE, n_mult, lambda,
                       params->w_gate_words);
    xor_u8_array(v_bar + m * lambda_bytes, first_prod, second_prod, lambda_bytes);
  }
  free_pointer_array(&gate_inputs);

  free_pointer_array(&V_columns);

  faest_free(ui);
  faest_free(u_hi_all);
  faest_free(u_hi);
  faest_free(u_low);
  faest_free(r_tilde);
}

bool vole_reconstruct(uint8_t* com, uint8_t** Q_dest, const uint8_t* iv, const uint8_t* chall_3,
                      const uint8_t* decom_i, const uint8_t* c, const uint8_t* c_mult,
                      uint8_t* q_bar, uint8_t* Delta, unsigned int ellhat,
                      const faest_paramset_t* params) {
  const unsigned int lambda                     = params->lambda;
  const unsigned int lambda_bytes               = lambda / 8;
  const unsigned int ellhat_bytes               = (ellhat + 7) / 8;
  const unsigned int ell                        = params->ell;
  const unsigned int ell_bytes                  = ell / 8;
  const unsigned int tau                        = params->tau;
  const unsigned int tau_1                      = params->tau1;
  const unsigned int L                          = params->L;
  const unsigned int k                          = params->k;
  const unsigned int n_mult                     = params->n_mult;
  const unsigned int n_mult_bytes               = (n_mult + 7) / 8;
  const unsigned int w_grind                    = params->w_grind;
  const unsigned int lambda_minus_w_grind       = lambda - w_grind;
  const unsigned int lambda_minus_w_grind_bytes = ((lambda_minus_w_grind) + 7) / 8;

  // line 1
  uint16_t i_delta[MAX_TAU];
  if (!decode_all_chall_3(i_delta, chall_3, params)) {
    return false;
  }

  // line 14
  // Delta = W_crt * Dp
  // Delta has lambda output bits
  bf2_matrix_mul_tbl(Delta, chall_3, params->W_CRT, lambda_minus_w_grind, lambda,
                     params->w_crt_words);
  bavc_rec_t bavc_rec;
  bavc_rec.h = com;
  bavc_rec.s = faest_malloc((L - tau) * lambda_bytes);

  // line 3
  if (!bavc_rec.s || !bavc_reconstruct(&bavc_rec, decom_i, i_delta, iv, params)) {
    faest_free(bavc_rec.s);
    return false;
  }

  uint8_t* sd      = faest_malloc((1 << k) * lambda_bytes);
  uint8_t* scratch = faest_calloc(bavc_max_node_index(0, tau_1, k), ellhat_bytes);
  assert(sd);
  assert(scratch);

  uint8_t** Q = alloc_pointer_array(lambda, ellhat_bytes);

  unsigned int q_idx = 0;
  uint8_t* sd_i      = bavc_rec.s;
  for (unsigned int i = 0; i < tau; i++) {
    const unsigned int Ni = bavc_max_node_index(i, tau_1, k);

    for (unsigned int j = 0; j < Ni; j++) {
      if (j < i_delta[i]) {
        memcpy(sd + (j ^ i_delta[i]) * lambda_bytes, sd_i + lambda_bytes * j, lambda_bytes);
      } else if (j > i_delta[i]) {
        memcpy(sd + (j ^ i_delta[i]) * lambda_bytes, sd_i + lambda_bytes * (j - 1), lambda_bytes);
      }
    }

    // line 8
    const unsigned int ki = convert_to_vole_with_scratch(iv, sd, true, i, ellhat_bytes, NULL,
                                                         Q[q_idx], params, scratch);
    if (i != 0) {
      for (unsigned int d = 0; d < ki; ++d, ++q_idx) {
        masked_xor_u8_array(Q[q_idx], Q[q_idx], c + (i - 1) * ell_bytes, (i_delta[i] >> d) & 1,
                            ell_bytes);
      }
    } else {
      q_idx += ki;
    }
    sd_i += lambda_bytes * (Ni - 1);
  }
  faest_free(sd);
  sd = NULL;
  faest_free(bavc_rec.s);
  bavc_rec.s = sd_i = NULL;
  faest_free(scratch);

  const unsigned int mask_bits = ellhat - ell;
  uint8_t** Q_columns          = alloc_pointer_array(mask_bits, lambda_minus_w_grind_bytes);
  uint8_t* mask_rows[MAX_LAMBDA];
  for (unsigned int row = 0; row < lambda_minus_w_grind; ++row) {
    mask_rows[row] = Q[row] + ell_bytes;
  }
  transpose_matrix(mask_rows, Q_columns, lambda_minus_w_grind, mask_bits);

  // line 13
  // delta_prime can be directly read from chall_3

  // line 19
  uint8_t* r_tilde_prime = faest_calloc(N_MASK, lambda_minus_w_grind_bytes);
  // for scan-build
  assert(lambda_minus_w_grind_bytes > 0);
  for (unsigned int m = 0; m < N_MASK; m++) {
    unsigned int ell_m   = m * bavc_max_node_depth(0, tau_1, k);
    unsigned int bit_idx = 0;
    q_idx                = 0;
    for (unsigned tau_idx = 0; tau_idx < tau; tau_idx++) {
      const uint8_t deg = bavc_max_node_depth(tau_idx, tau_1, k);

      uint32_t acc_value = 0;
      for (unsigned d = 0; d < deg; d++) {
        acc_value ^= (uint32_t)extract_bit_slice(Q_columns[ell_m + d], q_idx, deg) << d;
      }
      q_idx += deg;

      const uint16_t reduced = reduce_tree_product(acc_value, params->TREE_MODULI[tau_idx], deg);
      insert_bit_slice(r_tilde_prime + m * lambda_minus_w_grind_bytes, bit_idx, reduced, deg);
      bit_idx += deg;
    }
  }

  // line 21
  // a_prime is a direct copy of Q, so use Q instead

  uint8_t** c_mult_by_e = alloc_pointer_array(n_mult, N_MASK_BYTES);
  uint8_t** q_tilde     = alloc_pointer_array(n_mult, N_MASK_BYTES);
  transpose_bit_matrix(c_mult_by_e, c_mult, N_MASK, n_mult);
  for (unsigned e = 0; e < n_mult; e++) {
    uint8_t L_e_prime[MAX_LAMBDA_BYTES] = {0};

    // line 24
    uint64_t gamma_acc  = 0;
    const uint64_t* G_e = TBL(params->G, params->g_words, e);
    for (unsigned int word_idx = 0; word_idx < params->g_words; ++word_idx) {
      const unsigned int row_offset = word_idx * 64;
      const unsigned int word_bits  = MIN(lambda_minus_w_grind - row_offset, 64);

      uint64_t coefficients = G_e[word_idx] & bit_word_mask(word_bits);
      gamma_acc ^= coefficients & load_bit_word(chall_3 + word_idx * 8, word_bits);

      // coefficients is a public constant
      for (; coefficients; coefficients &= coefficients - 1) {
        const unsigned int bit = ctz64(coefficients);
        xor_small_u8_array_inplace(L_e_prime, Q[row_offset + bit], lambda_bytes);
      }
    }

    // line 25
    // reuse L_e_prime as h_e
    H5(L_e_prime, iv, e, L_e_prime, lambda);

    // line 27
    masked_xor_small_u8_array(q_tilde[e], L_e_prime, c_mult_by_e[e], parity64(gamma_acc),
                              N_MASK_BYTES);
  }

  // line 31: apply the CRT map row-wise, including the final transpose implicitly.
  crt_transform_rows(Q_dest, Q, ell_bytes, lambda_minus_w_grind, params);
  free_pointer_array(&Q);
  free_pointer_array(&c_mult_by_e);

  // line 30
  uint8_t** gate_inputs = alloc_pointer_array(N_MASK, n_mult_bytes);
  transpose_matrix(q_tilde, gate_inputs, n_mult, N_MASK);
  free_pointer_array(&q_tilde);

  for (unsigned int m = 0; m < N_MASK; m++) {
    uint8_t first_prod[MAX_LAMBDA_BYTES] = {0};
    bf2_matrix_mul_tbl(first_prod, r_tilde_prime + m * lambda_minus_w_grind_bytes, params->W_TREE,
                       lambda_minus_w_grind, lambda, params->w_tree_words);

    uint8_t second_prod[MAX_LAMBDA_BYTES] = {0};
    bf2_matrix_mul_tbl(second_prod, gate_inputs[m], params->W_GATE, n_mult, lambda,
                       params->w_gate_words);
    xor_u8_array(q_bar + m * lambda_bytes, first_prod, second_prod, lambda_bytes);
  }
  free_pointer_array(&gate_inputs);
  faest_free(r_tilde_prime);
  r_tilde_prime = NULL;

  free_pointer_array(&Q_columns);

  return true;
}
