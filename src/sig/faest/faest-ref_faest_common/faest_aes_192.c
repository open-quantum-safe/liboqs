/*
 *  SPDX-License-Identifier: MIT
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "faest_aes.h"
#include "fields.h"
#include "vole.h"
#include "universal_hashing.h"
#include "utils.h"
#include "parameters.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// clang-format off
#define FAEST_LAMBDA FAEST_192_LAMBDA
#define FAEST_NK (FAEST_LAMBDA / 32)
#define FAEST_R FAEST_192S_R

#if FAEST_LAMBDA == 192
#define IWD_INC 192
#else
#define IWD_INC 128
#endif

#define bf_t bf192_t
#define bf_load bf192_load
#define bf_from_bit bf192_from_bit
#define bf_store bf192_store
#define bf_zero bf192_zero
#define bf_one bf192_one
#define bf_add bf192_add
#define bf_add_inplace bf192_add_inplace
#define bf_mul bf192_mul
#define bf_mul_inplace bf192_mul_inplace
#define bf_mul_bit bf192_mul_bit
#define bf_square bf192_square
#define bf_byte_combine bf192_byte_combine
#define bf_byte_combine_bits bf192_byte_combine_bits
#define bf_byte_combine_sq bf192_byte_combine_sq
#define bf_sq_bit bf192_sq_bit
#define bf_sq_bit_inplace bf192_sq_bit_inplace
#define bf_sum_poly_bits bf192_sum_poly_bits
#define BF_NUM_BYTES BF192_NUM_BYTES
#define BF_ALIGN BF192_ALIGN

#define zk_hash_7_ctx zk_hash_192_7_ctx
#define zk_hash_7_finalize zk_hash_192_7_finalize
#define zk_hash_7_init zk_hash_192_7_init
#define zk_hash_7_raise_and_update zk_hash_192_7_raise_and_update
#define zk_hash_7_update zk_hash_192_7_update
#define zk_hash_ctx zk_hash_192_ctx
#define zk_hash_finalize zk_hash_192_finalize
#define zk_hash_init zk_hash_192_init
#define zk_hash_update zk_hash_192_update

#define aes_prover aes_192_prover
#define aes_verifier aes_192_verifier

static_assert(D_ZK == 7, "Invalid assumptions.");

static_assert(FAEST_LAMBDA == FAEST_192S_LAMBDA, "Invalid parameters");
static_assert(FAEST_192F_ELL == FAEST_192S_ELL, "Invalid parameters");
static_assert(FAEST_192F_LAMBDA == FAEST_192S_LAMBDA, "Invalid parameters");
static_assert(FAEST_192F_Lke == FAEST_192S_Lke, "Invalid parameters");
static_assert(FAEST_192F_Nst == FAEST_192S_Nst, "Invalid parameters");
static_assert(FAEST_R == FAEST_192S_R, "Invalid parameters");
static_assert(FAEST_192F_R == FAEST_192S_R, "Invalid parameters");
static_assert(FAEST_192F_Ske == FAEST_192S_Ske, "Invalid parameters");

static_assert(FAEST_LAMBDA == FAEST_EM_192S_LAMBDA, "Invalid parameters");
static_assert(FAEST_EM_192F_LAMBDA == FAEST_EM_192S_LAMBDA, "Invalid parameters");
static_assert(FAEST_EM_192F_Lenc == FAEST_EM_192S_Lenc, "Invalid parameters");
static_assert(FAEST_EM_192F_Nst == FAEST_EM_192S_Nst, "Invalid parameters");
static_assert(FAEST_R == FAEST_EM_192S_R, "Invalid parameters");
static_assert(FAEST_EM_192F_R == FAEST_EM_192S_R, "Invalid parameters");
// for scan-build
static_assert(FAEST_EM_192F_LAMBDA * (FAEST_EM_192F_R + 1) / 8 ==
                  sizeof(aes_word_t) * FAEST_EM_192F_Nst * (FAEST_EM_192F_R + 1),
              "Invalid parameters");
// clang-format on

/* pad sizes to multiples of some value; assumes that a is a power of 2 */
#define BF_ALLOC(s) faest_aligned_alloc(BF_ALIGN, ALIGN_TO((s) * sizeof(bf_t), BF_ALIGN))

#define RCON_EVERY (4 * (FAEST_LAMBDA / 128))

static const bf8_t Rcon[30] = {
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91,
};

#if FAEST_LAMBDA == 128
static const bf128_t bf_c[9] = {
    BF128C(UINT64_C(0xec7759ca3488aee0), UINT64_C(0x4cf4b7439cbfbb84)),
    BF128C(UINT64_C(0xbfcf02ae363946a9), UINT64_C(0x35ad604f7d51d2c6)),
    BF128C(UINT64_C(0x4c3607bab51b5aca), UINT64_C(0xb32fd29a04c0be08)),
    BF128C(UINT64_C(0xc95c10ed4f932c54), UINT64_C(0x186ca7a286376521)),
    BF128C(UINT64_C(0x1f8e5cdeb7aab282), UINT64_C(0xca760596e52ed74a)),
    BF128C(UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000000)),
    BF128C(UINT64_C(0xd8a5ae31928b4da1), UINT64_C(0x1cf7a0fe8922c83f)),
    BF128C(UINT64_C(0x88fd3d5cb6e7dff9), UINT64_C(0x7534634307ce7cbe)),
    BF128C(UINT64_C(0x433f53640b5ab39a), UINT64_C(0x872430dcdf135bcc)),
};
#elif FAEST_LAMBDA == 192
static const bf192_t bf_c[9] = {
    BF192C(UINT64_C(0xb233619e7cf450ba), UINT64_C(0x7bf61f19d5633f26),
           UINT64_C(0xda933726d491db34)),
    BF192C(UINT64_C(0x9c6d2c13f5398a0c), UINT64_C(0x8232e37706328d19),
           UINT64_C(0x0c3b0d703c754ef6)),
    BF192C(UINT64_C(0xfb039539490f3262), UINT64_C(0x638227a707652828),
           UINT64_C(0x7170a38d85840211)),
    BF192C(UINT64_C(0x6ae66d7cf63a7b42), UINT64_C(0x73e093aeb2bd81a2),
           UINT64_C(0x0cee234c9f37ab71)),
    BF192C(UINT64_C(0xd55dd8b4c0c2e8d5), UINT64_C(0x9a46dbc9d4349a17),
           UINT64_C(0xa7d899db6d6097d3)),
    BF192C(UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000000),
           UINT64_C(0x0000000000000000)),
    BF192C(UINT64_C(0x425244c22e17096e), UINT64_C(0x69ac2c678be3ba5f),
           UINT64_C(0x8e7e2484040c7d90)),
    BF192C(UINT64_C(0x1702b39a83f3c8a5), UINT64_C(0x7fb8d6607c39e606),
           UINT64_C(0x20dfa416e9086710)),
    BF192C(UINT64_C(0x831233410b235d20), UINT64_C(0x1d99ac75ae672326),
           UINT64_C(0xced061212ca1ac64)),
};
#elif FAEST_LAMBDA == 256
static const bf256_t bf_c[9] = {
    BF256C(UINT64_C(0xa95af52ad52289c0), UINT64_C(0x2ba5c48d2c42072f), UINT64_C(0xd14a0d376c00b0ea),
           UINT64_C(0x064e4d699c5b4af1)),
    BF256C(UINT64_C(0x55dab3833f809d1c), UINT64_C(0x1771831e533b0f57), UINT64_C(0xfb96573fad3fac10),
           UINT64_C(0x6195e3db7011f68d)),
    BF256C(UINT64_C(0x372f5a920b67efff), UINT64_C(0x8748a24b4ab3a892), UINT64_C(0x214b28089e99af95),
           UINT64_C(0xc6737a464da16302)),
    BF256C(UINT64_C(0x319800033ca8b976), UINT64_C(0x30611f596cb383ad), UINT64_C(0xfef404a31149196b),
           UINT64_C(0x24694604ed0c050d)),
    BF256C(UINT64_C(0xcbaf1c3be1c5fb22), UINT64_C(0xbb9ce5d835caa0ea), UINT64_C(0x0b9772005fa6b36f),
           UINT64_C(0xa1a8d4f4a1ebdf7e)),
    BF256C(UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000000),
           UINT64_C(0x0000000000000000)),
    BF256C(UINT64_C(0x55dab3823f819c28), UINT64_C(0x1771831e533b0f56), UINT64_C(0xfb96573fad3fac11),
           UINT64_C(0x6195e3db7011f68d)),
    BF256C(UINT64_C(0xd0547873524e02b1), UINT64_C(0xd0350e7dfa862912), UINT64_C(0xda9be967cd26e8d5),
           UINT64_C(0x702cec741ee89ff7)),
    BF256C(UINT64_C(0x9020d2d23c10a95b), UINT64_C(0x09ff302a86476559), UINT64_C(0x009db4ee77215795),
           UINT64_C(0xe6d3948d80643e3c)),
};
#endif

// ADD ROUND KEY
/*
Called in EncCstrnts, takes in owf_in (bits) and their tags (0 for prover, owf_in (bit) * delta for
verifier)
*/

static void aes_add_round_key_verifier(bf_t* out_key, const bf_t* in_key, const bf_t* k_key,
                                       unsigned int Nst) {
  const unsigned int Nstbits = Nst * 32;

  for (unsigned int i = 0; i < Nstbits; i++) {
    bf_add(&out_key[i], &in_key[i], &k_key[i]);
  }
}

static void aes_add_round_key_prover(uint8_t* out, bf_t* out_tag, const uint8_t* in,
                                     const bf_t* in_tag, const uint8_t* k, const bf_t* k_tag,
                                     unsigned int Nst) {
  const unsigned int Nstbits = Nst * 32;

  xor_u8_array(out, in, k, Nstbits / 8);
  aes_add_round_key_verifier(out_tag, in_tag, k_tag, Nst);
}

// F256/F2.CONJUGATES
static void aes_f256_f2_conjugates_1(bf_t* y, const uint8_t* state, unsigned int Nst) {
  const unsigned int Nst_bytes = Nst * 4;

  for (unsigned int i = 0; i != Nst_bytes; ++i) {
    bf_byte_combine_bits(&y[i * 8], state[i]);
    for (unsigned int j = 1; j != 8; ++j) {
      bf_square(&y[i * 8 + j], &y[i * 8 + j - 1]);
    }
  }
}

static void aes_f256_f2_conjugates(bf_t* y, const bf_t* state, unsigned int Nst) {
  const unsigned int Nst_bytes = Nst * 4;

  for (unsigned int i = 0; i != Nst_bytes; ++i) {
    bf_t x[8];
    bf_byte_combine(&y[i * 8], state + i * 8);
    bf_sq_bit(x, state + i * 8);
    for (unsigned int j = 1; j != 7; ++j) {
      bf_byte_combine(&y[i * 8 + j], x);
      bf_sq_bit_inplace(x);
    }
    bf_byte_combine(&y[i * 8 + 7], x);
  }
}

// STATE TO BYTES
static void aes_state_to_bytes_verifier(bf_t* out_key, const bf_t* k_key, unsigned int Nst) {
  const unsigned int Nst_bytes = Nst * 4;
  for (unsigned int i = 0; i < Nst_bytes; i++) {
    bf_byte_combine(&out_key[i], k_key + i * 8);
  }
}

static void aes_state_to_bytes_prover(bf_t** out_deg, const uint8_t* k, const bf_t* k_tag,
                                      unsigned int Nst) {
  const unsigned int Nst_bytes = Nst * 4;
  for (unsigned int i = 0; i < Nst_bytes; i++) {
    bf_byte_combine_bits(&out_deg[1][i], k[i]);
  }

  aes_state_to_bytes_verifier(out_deg[0], k_tag, Nst);
}

// SBOX AFFINE
static void aes_sbox_eval_prover(bf_t** state_deg7, const uint8_t* state_bits, bf_t* state_bits_tag,
                                 unsigned int Nst) {

  const unsigned int Nstbits  = 32 * Nst;
  const unsigned int Nstbytes = Nstbits / 8;

  bf_t* state_conj     = BF_ALLOC(2 * Nstbits);
  bf_t* state_conj_tag = state_conj + Nstbits;

  aes_f256_f2_conjugates_1(state_conj, state_bits, Nst);
  aes_f256_f2_conjugates(state_conj_tag, state_bits_tag, Nst);

  // line 3+5: zeta is precomputed as bf_c

  for (unsigned int j = 0; j < Nstbytes; j++) {
    bf_t p1_deg[3][4];
    bf_t f1_deg[2][4];
    bf_t p2_deg[5][2];
    bf_t f2_deg[4][2];

    for (unsigned int i = 0; i <= 4; i++) {
      for (unsigned int m = 0; m < 2; m++) {
        p2_deg[i][m] = bf_zero();
      }
    }
    for (unsigned int i = 0; i <= 3; i++) {
      for (unsigned int m = 0; m < 2; m++) {
        f2_deg[i][m] = bf_zero();
      }
    }

    // line 8
    for (unsigned int i = 0; i < 4; i++) {
      bf_mul(&p1_deg[0][i], &state_conj_tag[j * 8 + i * 2],
             &state_conj_tag[j * 8 + (i * 2 + 1)]); // deg-0 * deg-0

      bf_t tmp;
      bf_mul(&tmp, &state_conj[j * 8 + i * 2],
             &state_conj_tag[j * 8 + (i * 2 + 1)]); // deg-1 * deg-0
      bf_mul(&p1_deg[1][i], &state_conj_tag[j * 8 + i * 2],
             &state_conj[j * 8 + (i * 2 + 1)]); // deg-0 * deg-1
      bf_add(&p1_deg[1][i], &p1_deg[1][i], &tmp);

      bf_mul(&p1_deg[2][i], &state_conj[j * 8 + i * 2],
             &state_conj[j * 8 + (i * 2 + 1)]); // deg-1 * deg-1

      bf_t prod_1_deg[2], prod_2_deg[2];
      bf_mul(&prod_1_deg[0], &bf_c[i * 2],
             &state_conj_tag[j * 8 + (i * 2 + 1)]); // deg-0 * deg-0
      bf_mul(&prod_2_deg[0], &bf_c[i * 2 + 1],
             &state_conj_tag[j * 8 + (i * 2)]);                               // deg-0 * deg-0
      bf_mul(&prod_1_deg[1], &bf_c[i * 2], &state_conj[j * 8 + (i * 2 + 1)]); // deg-0 * deg-1
      bf_mul(&prod_2_deg[1], &bf_c[i * 2 + 1], &state_conj[j * 8 + (i * 2)]); // deg-0 * deg-1

      bf_add(&f1_deg[0][i], &prod_1_deg[0], &prod_2_deg[0]); // deg-0
      bf_add(&f1_deg[1][i], &prod_1_deg[1], &prod_2_deg[1]); // deg-1
    }

    for (unsigned int i = 0; i < 2; i++) {
      for (unsigned int m = 0; m < 3; m++) {
        for (unsigned int n = 0; n < 3; n++) {
          bf_t tmp;
          bf_mul(&tmp, &p1_deg[m][2 * i], &p1_deg[n][2 * i + 1]);
          bf_add_inplace(&p2_deg[m + n][i], &tmp);
        }
      }

      bf_t tmp;
      // first product
      for (unsigned int m = 0; m < 2; m++) {
        for (unsigned int n = 0; n < 3; n++) {
          bf_mul(&tmp, &f1_deg[m][2 * i], &p1_deg[n][2 * i + 1]);
          bf_add_inplace(&f2_deg[m + n][i], &tmp);
        }
      }

      // second product
      for (unsigned int m = 0; m < 2; m++) {
        for (unsigned int n = 0; n < 3; n++) {
          bf_mul(&tmp, &f1_deg[m][2 * i + 1], &p1_deg[n][2 * i]);
          bf_add_inplace(&f2_deg[m + n][i], &tmp);
        }
      }
    }

    // first product
    for (unsigned int m = 0; m < 8; m++) {
      state_deg7[m][j] = bf_zero();
    }
    bf_t tmp;
    for (unsigned int m = 0; m < 4; m++) {
      for (unsigned int n = 0; n < 5; n++) {
        bf_mul(&tmp, &f2_deg[m][0], &p2_deg[n][1]);
        bf_add_inplace(&state_deg7[n + m][j], &tmp);
      }
    }

    // second product
    for (unsigned int m = 0; m < 4; m++) {
      for (unsigned int n = 0; n < 5; n++) {
        bf_mul(&tmp, &f2_deg[m][1], &p2_deg[n][0]);
        bf_add_inplace(&state_deg7[n + m][j], &tmp);
      }
    }

    bf_add_inplace(&state_deg7[7][j], &bf_c[8]);
  }

  faest_aligned_free(state_conj);
}

static void aes_sbox_eval_verifier(bf_t* state_key, const bf_t* state_bits_key, const bf_t* delta_i,
                                   unsigned int Nst) {
  const unsigned int Nstbits  = 32 * Nst;
  const unsigned int Nstbytes = Nstbits / 8;

  bf_t* state_conj_key = BF_ALLOC(Nstbits);

  aes_f256_f2_conjugates(state_conj_key, state_bits_key, Nst);

  // line 3+5: zeta is precomputed as bf_c
  bf_t zeta_delta_i;
  bf_mul(&zeta_delta_i, &bf_c[8], &delta_i[7]);

  // line 6
  for (unsigned int j = 0; j < Nstbytes; j++) {
    bf_t p1_key[4];
    bf_t f1_key[4];
    bf_t p2_key[2];
    bf_t f2_key[2];

    // line 8
    for (unsigned int i = 0; i < 4; i++) {
      // line 9
      bf_mul(&p1_key[i], &state_conj_key[j * 8 + i * 2],
             &state_conj_key[j * 8 + (i * 2 + 1)]); // deg-1 * deg-1

      // line 10
      bf_t prod_1_key, prod_2_key;
      bf_mul(&prod_1_key, &bf_c[i * 2],
             &state_conj_key[j * 8 + (i * 2 + 1)]); // deg-0 * deg-1
      bf_mul(&prod_2_key, &bf_c[i * 2 + 1],
             &state_conj_key[j * 8 + (i * 2)]); // deg-0 * deg-1

      bf_add(&f1_key[i], &prod_1_key, &prod_2_key); // deg-1
    }

    // line 11
    for (unsigned int i = 0; i < 2; i++) {
      // line 12
      bf_mul(&p2_key[i], &p1_key[2 * i], &p1_key[2 * i + 1]); // deg-2 * deg-2

      // line 13
      // first product
      bf_mul(&f2_key[i], &f1_key[2 * i], &p1_key[2 * i + 1]); // deg-1 * deg-2
      // second product
      bf_t tmp;
      bf_mul(&tmp, &f1_key[2 * i + 1], &p1_key[2 * i]); // deg-1 * deg-2
      bf_add_inplace(&f2_key[i], &tmp);                 // deg-3
    }

    // line 14
    // first product
    bf_mul(&state_key[j], &f2_key[0], &p2_key[1]); // deg-3 * deg-4

    // second product
    bf_t tmp;
    bf_mul(&tmp, &f2_key[1], &p2_key[0]); // deg-3 * deg-4
    bf_add_inplace(&state_key[j], &tmp);  // deg-7

    bf_add_inplace(&state_key[j], &zeta_delta_i);
  }

  faest_aligned_free(state_conj_key);
}

// SHIFT ROWS
static void aes_shiftrows_prover(bf_t** out_deg, const bf_t** in_deg, unsigned int Nst) {

  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nst; c++) {
      unsigned int i;
      if ((Nst != 8) || (r <= 1)) {
        i = 4 * ((c + r) % Nst) + r;
      } else {
        i = 4 * ((c + r + 1) % Nst) + r;
      }
      for (unsigned int m = 0; m < 8; m++) {
        out_deg[m][4 * c + r] = in_deg[m][i];
      }
    }
  }
}

static void aes_shiftrows_verifier(bf_t* out_key, const bf_t* in_key, unsigned int Nst) {
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nst; c++) {
      if ((Nst != 8) || (r <= 1)) {
        out_key[4 * c + r] = in_key[(4 * ((c + r) % Nst) + r)];
      } else {
        out_key[4 * c + r] = in_key[(4 * ((c + r + 1) % Nst) + r)];
      }
    }
  }
}

// MIX COLOUMNS

#if FAEST_LAMBDA == 128
static const bf128_t bf_bc_2 = BF128C(UINT64_C(0xa13fe8ac5560ce0d), UINT64_C(0x053d8555a9979a1c));
#elif FAEST_LAMBDA == 192
static const bf192_t bf_bc_2 = BF192C(UINT64_C(0xccc8a3d56f389763), UINT64_C(0xe665d76c966ebdea),
                                      UINT64_C(0x310bc8140e6b3662));
#elif FAEST_LAMBDA == 256
static const bf256_t bf_bc_2 = BF256C(UINT64_C(0x969788420bdefee7), UINT64_C(0xbed68d38a0474e67),
                                      UINT64_C(0xdf229845f8f1e16a), UINT64_C(0x04c9a8cf20c95833));
#else
#error "unexpected security parameter"
#endif

static void aes_mix_column(bf_t* y, const bf_t* in) {
  bf_t sum;
  bf_add(&sum, &in[0], &in[1]);
  bf_add_inplace(&sum, &in[2]);
  bf_add_inplace(&sum, &in[3]);

  for (unsigned int r = 0; r != 4; ++r) {
    bf_t adjacent;
    bf_t product;
    bf_add(&adjacent, &in[r], &in[(r + 1) % 4]);
    bf_mul(&product, &adjacent, &bf_bc_2);
    bf_add(&y[r], &in[r], &sum);
    bf_add_inplace(&y[r], &product);
  }
}

static void aes_mix_columns_prover(bf_t** y_deg, const bf_t** in_deg, unsigned int Nst) {
  for (unsigned int c = 0; c < Nst; c++) {
    for (unsigned int m = 0; m < 8; ++m) {
      aes_mix_column(y_deg[m] + 4 * c, in_deg[m] + 4 * c);
    }
  }
}

static void aes_mix_columns_verifier(bf_t* y_key, const bf_t* in_key, unsigned int Nst) {
  for (unsigned int c = 0; c < Nst; c++) {
    aes_mix_column(y_key + 4 * c, in_key + 4 * c);
  }
}

// ADD ROUND KEY BYTES
static void aes_add_round_key_bytes_prover_degree_7(bf_t** out_deg, const bf_t** in_deg,
                                                    const bf_t** k_deg, unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  for (unsigned int m = 0; m < 6; m++) {
    memcpy(out_deg[m], in_deg[m], sizeof(bf_t) * Nstbytes);
  }
  for (unsigned int i = 0; i < Nstbytes; i++) {
    bf_add(&out_deg[7][i], &in_deg[7][i], &k_deg[1][i]);
    bf_add(&out_deg[6][i], &in_deg[6][i], &k_deg[0][i]);
  }
}

// Use shift_tag if key is degree-1 instead of degree-2
static void aes_add_round_key_bytes_verifier(bf_t* y_key, const bf_t* in_key, const bf_t* k_key,
                                             const bf_t* delta_i, unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  for (unsigned int i = 0; i < Nstbytes; i++) {
    // Multiply tag by delta to align degrees
    bf_t tmp;
    bf_mul(&tmp, &k_key[i], &delta_i[6]);
    bf_add(&y_key[i], &in_key[i], &tmp);
  }
}

// INVERSE SHIFT ROWS
static void aes_inverse_shiftrows_prover(uint8_t* out, bf_t* out_tag, const uint8_t* in,
                                         const bf_t* in_tag, unsigned int Nst) {
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nst; c++) {
      unsigned int i;
      if ((Nst != 8) || (r <= 1)) {
        i = 4 * ((c + Nst - r) % Nst) + r;
      } else {
        i = 4 * ((c + Nst - r - 1) % Nst) + r;
      }

      out[4 * c + r] = in[i];
      memcpy(&out_tag[8 * (4 * c + r)], &in_tag[8 * i], 8 * sizeof(bf_t));
    }
  }
}

static void aes_inverse_shiftrows_verifier(bf_t* out_key, const bf_t* in_key, unsigned int Nst) {
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nst; c++) {
      unsigned int i;
      if ((Nst != 8) || (r <= 1)) {
        i = 4 * ((c + Nst - r) % Nst) + r;
      } else {
        i = 4 * ((c + Nst - r - 1) % Nst) + r;
      }

      memcpy(&out_key[8 * (4 * c + r)], &in_key[8 * i], 8 * sizeof(bf_t));
    }
  }
}

// BITWISE MIX COLUMNS
static void aes_bitwise_mix_column_tags(bf_t* out, const bf_t* a, const bf_t* b) {
  for (unsigned int i_bit = 0; i_bit != 8; ++i_bit) {
    bf_t column_sum;
    bf_add(&column_sum, &a[i_bit], &a[8 + i_bit]);
    bf_add_inplace(&column_sum, &a[16 + i_bit]);
    bf_add_inplace(&column_sum, &a[24 + i_bit]);

    for (unsigned int r = 0; r != 4; ++r) {
      bf_add(&out[8 * r + i_bit], &a[8 * r + i_bit], &column_sum);
      bf_add_inplace(&out[8 * r + i_bit], &b[8 * r + i_bit]);
      bf_add_inplace(&out[8 * r + i_bit], &b[8 * ((r + 1) % 4) + i_bit]);
    }
  }
}

static void aes_bitwise_mix_column_prover(uint8_t* out, bf_t* out_tag, const uint8_t* s,
                                          const bf_t* s_tag, unsigned int Nst) {
  for (unsigned int c = 0; c < Nst; c++) {
    // ::2-3
    const uint8_t* a_bits  = &s[32 * c / 8];
    const bf_t* a_bits_tag = &s_tag[32 * c];

    uint8_t b_bits[4];
    bf_t b_bits_tag[4 * 8];

    // ::1
    for (unsigned int r = 0; r < 4; r++) {
      // :5
      const uint8_t high_bit_mask = -(a_bits[r] >> 7);
      b_bits[r]                   = (a_bits[r] << 1) ^ (high_bit_mask & UINT8_C(0x1b));

      b_bits_tag[r * 8 + 0] = a_bits_tag[r * 8 + 7];
      bf_add(&b_bits_tag[r * 8 + 1], &a_bits_tag[r * 8 + 0], &a_bits_tag[r * 8 + 7]);
      b_bits_tag[r * 8 + 2] = a_bits_tag[r * 8 + 1];
      bf_add(&b_bits_tag[r * 8 + 3], &a_bits_tag[r * 8 + 2], &a_bits_tag[r * 8 + 7]);
      bf_add(&b_bits_tag[r * 8 + 4], &a_bits_tag[r * 8 + 3], &a_bits_tag[r * 8 + 7]);
      memcpy(&b_bits_tag[r * 8 + 5], &a_bits_tag[r * 8 + 4], 3 * sizeof(bf_t));
    }

    const uint8_t column_sum = a_bits[0] ^ a_bits[1] ^ a_bits[2] ^ a_bits[3];
    for (unsigned int r = 0; r != 4; ++r) {
      out[c * 4 + r] = a_bits[r] ^ column_sum ^ b_bits[r] ^ b_bits[(r + 1) % 4];
    }

    aes_bitwise_mix_column_tags(out_tag + 32 * c, a_bits_tag, b_bits_tag);
  }
}

static void aes_bitwise_mix_column_verifier(bf_t* out_key, bf_t* s_keys_key, unsigned int Nst) {
  for (unsigned int c = 0; c < Nst; c++) {
    // ::2-3
    const bf_t* a_bits_key = &s_keys_key[32 * c];

    // ::1
    bf_t b_bits_key[4 * 8];
    for (unsigned int r = 0; r < 4; r++) {
      // :5
      b_bits_key[r * 8 + 0] = a_bits_key[r * 8 + 7];
      bf_add(&b_bits_key[r * 8 + 1], &a_bits_key[r * 8 + 0], &a_bits_key[r * 8 + 7]);
      b_bits_key[r * 8 + 2] = a_bits_key[r * 8 + 1];
      bf_add(&b_bits_key[r * 8 + 3], &a_bits_key[r * 8 + 2], &a_bits_key[r * 8 + 7]);
      bf_add(&b_bits_key[r * 8 + 4], &a_bits_key[r * 8 + 3], &a_bits_key[r * 8 + 7]);
      memcpy(&b_bits_key[r * 8 + 5], &a_bits_key[r * 8 + 4], 3 * sizeof(bf_t));
    }

    // ::6-9
    aes_bitwise_mix_column_tags(out_key + 32 * c, a_bits_key, b_bits_key);
  }
}

// CONSTANT TO VOLE
static void constant_to_vole_prover(bf_t* tag, unsigned int n) {
  // the val stay the same as the val is a pub const!
  // for constant values the tag is zero
  memset(tag, 0, sizeof(bf_t) * n);
}

static void constant_to_vole_verifier(bf_t* key, const uint8_t* val, const bf_t* delta,
                                      unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    bf_mul_bit(&key[i], delta, ptr_get_bit(val, i));
  }
}

// // INVERSE AFFINE
static void aes_inverse_affine_byte_prover(uint8_t* y_bits, bf_t* y_bits_tag, const uint8_t x_bits,
                                           const bf_t* x_bits_tag) {
  *y_bits = rotr8(x_bits, 7) ^ rotr8(x_bits, 5) ^ rotr8(x_bits, 2) ^ 0x5;

  for (unsigned int bit_i = 0; bit_i < 8; bit_i++) {
    bf_add(&y_bits_tag[bit_i], &x_bits_tag[(bit_i - 1 + 8) % 8], &x_bits_tag[(bit_i - 3 + 8) % 8]);
    bf_add_inplace(&y_bits_tag[bit_i], &x_bits_tag[(bit_i - 6 + 8) % 8]);
  }
}

static void aes_inverse_affine_prover(uint8_t* y, bf_t* y_tag, const uint8_t* x, const bf_t* x_tag,
                                      unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  for (unsigned int i = 0; i < Nstbytes; i++) {
    aes_inverse_affine_byte_prover(y + i, y_tag + i * 8, x[i], x_tag + i * 8);
  }
}

static void aes_inverse_sbox_eval_prover(bf_t** b_deg, const uint8_t* s_dash_dash,
                                         const bf_t* s_dash_dash_tag, unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  uint8_t* s  = malloc(Nstbytes); // IGNORE memory-check
  bf_t* s_tag = BF_ALLOC(Nstbytes * 8 * 3);
  aes_inverse_affine_prover(s, s_tag, s_dash_dash, s_dash_dash_tag, Nst);

  bf_t* state_conj     = s_tag + Nstbytes * 8;
  bf_t* state_conj_tag = state_conj + Nstbytes * 8;

  aes_f256_f2_conjugates_1(state_conj, s, Nst);
  free(s); // IGNORE memory-check
  aes_f256_f2_conjugates(state_conj_tag, s_tag, Nst);

  for (unsigned int j = 0; j < Nstbytes; j++) {
    b_deg[0][j] = state_conj_tag[j * 8 + 1];
    b_deg[1][j] = state_conj[j * 8 + 1];
    for (unsigned int m = 2; m < 8; m++) {
      b_deg[m][j] = bf_zero();
    }

    for (unsigned int i = 2; i < 8; i++) {
      const bf_t* t = &state_conj_tag[j * 8 + i];
      const bf_t* v = &state_conj[j * 8 + i];

      // Temporary holder
      bf_t b_deg_tmp_v[8];
      bf_t b_deg_tmp_tag[8];
      for (unsigned int m = 0; m < 8; m++) {
        b_deg_tmp_v[m]   = bf_zero();
        b_deg_tmp_tag[m] = bf_zero();
      }

      for (unsigned int m = 0; m < i; m++) {
        bf_mul(&b_deg_tmp_tag[m], &b_deg[m][j], t);   // deg-n * deg-0  -> deg-n
        bf_mul(&b_deg_tmp_v[m + 1], &b_deg[m][j], v); // deg-n * deg-1  -> deg-n+1
      }
      for (unsigned int m = 0; m < 8; m++) {
        bf_add(&b_deg[m][j], &b_deg_tmp_tag[m], &b_deg_tmp_v[m]);
      }
    }
  }

  faest_aligned_free(s_tag);
}

static void aes_inverse_affine_byte_verifier(bf_t* y_bits_key, const bf_t* x_bits_key,
                                             const bf_t* delta) {
  for (unsigned int bit_i = 0; bit_i < 8; bit_i++) {
    bf_add(&y_bits_key[bit_i], &x_bits_key[(bit_i - 1 + 8) % 8], &x_bits_key[(bit_i - 3 + 8) % 8]);
    bf_add_inplace(&y_bits_key[bit_i], &x_bits_key[(bit_i - 6 + 8) % 8]);
  }

  bf_add_inplace(&y_bits_key[0], delta);
  bf_add_inplace(&y_bits_key[2], delta);
}

static void aes_inverse_affine_verifier(bf_t* y_key, const bf_t* x_key, const bf_t* delta,
                                        unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  for (unsigned int i = 0; i < Nstbytes; i++) {
    aes_inverse_affine_byte_verifier(y_key + i * 8, x_key + i * 8, delta);
  }
}

static void aes_inverse_sbox_eval_verifier(bf_t* b_key, const bf_t* s_dash_dash_tag,
                                           const bf_t* delta_i, unsigned int Nst) {

  const unsigned int Nstbytes = Nst * 4;

  bf_t* s_tag = BF_ALLOC(Nstbytes * 8 * 2);
  aes_inverse_affine_verifier(s_tag, s_dash_dash_tag, &delta_i[1], Nst);

  bf_t* state_conj_tag = s_tag + Nstbytes * 8;
  aes_f256_f2_conjugates(state_conj_tag, s_tag, Nst);

  for (unsigned int j = 0; j < Nstbytes; j++) {
    b_key[j] = state_conj_tag[j * 8 + 1];
    for (unsigned int i = 2; i < 8; i++) {
      bf_mul_inplace(&b_key[j], &state_conj_tag[j * 8 + i]);
    }
  }

  faest_aligned_free(s_tag);
}

// EncSctrnts internal functions end!!

static bf_t* shrink_V(uint8_t** v, unsigned int ell) {
  // v was already transposed in calling code, so just load the values
  bf_t* new_v = BF_ALLOC(ell);
  assert(new_v);
  for (unsigned int row = 0; row != ell; ++row) {
    bf_load(&new_v[row], v[row]);
  }
  return new_v;
}

// // KEY EXP FWD / BKWD
static void aes_keyexp_backward_prover(uint8_t* y, bf_t* y_tag, const uint8_t* x, const bf_t* x_tag,
                                       const uint8_t* key, const bf_t* key_tag,
                                       const faest_paramset_t* params) {
  const unsigned int Ske = params->Ske;

  // ::3
  unsigned int iwd = 0;
  // ::5-6
  for (unsigned int j = 0; j < Ske; j++) {
    // ::7-10
    // for the witness
    uint8_t x_tilde = x[j] ^ key[(iwd + (j % 4) * 8) / 8];
    // for the tags of each witness bit
    bf_t x_tilde_tag[8];
    for (unsigned int bit_i = 0; bit_i < 8; bit_i++) {
      bf_add(&x_tilde_tag[bit_i], &x_tag[j * 8 + bit_i], &key_tag[iwd + (j % 4) * 8 + bit_i]);
    }

    if (j % RCON_EVERY == 0) {
      // adding round constant to the witness
      x_tilde ^= Rcon[j / RCON_EVERY];
    }

    // ::11
    aes_inverse_affine_byte_prover(y + j, y_tag + 8 * j, x_tilde, x_tilde_tag);

    // ::12-16 lines only relavant for aes-128
    if (j % 4 == 3) {
      iwd += IWD_INC;
    }
  }
}

static void aes_keyexp_backward_verifier(bf_t* y_key, const bf_t* x_key, const bf_t* key_key,
                                         const bf_t* delta, const faest_paramset_t* params) {
  const unsigned int Ske = params->Ske;

  // ::2
  bf_t x_tilde_key[8];
  // ::3
  unsigned int iwd = 0;
  // ::5-6
  for (unsigned int j = 0; j < Ske; j++) {
    // ::7
    for (unsigned int bit_i = 0; bit_i < 8; bit_i++) {
      bf_add(&x_tilde_key[bit_i], &x_key[j * 8 + bit_i],
             &key_key[iwd + (j % 4) * 8 + bit_i]); // for the tags of each witness bit
      // ::8-10
      if (j % RCON_EVERY == 0) {
        bf_t rcon_key;
        const uint8_t c = get_bit(Rcon[j / RCON_EVERY], bit_i);
        constant_to_vole_verifier(&rcon_key, &c, delta, 1);
        bf_add_inplace(&x_tilde_key[bit_i], &rcon_key);
      }
    }
    // ::11
    aes_inverse_affine_byte_verifier(y_key + 8 * j, x_tilde_key, delta);

    // ::12-16 lines only relavant for aes-128
    if (j % 4 == 3) {
      iwd += IWD_INC;
    }
  }
}

static void aes_keyexp_forward_prover(uint8_t* y, bf_t* y_tag, const uint8_t* w,
                                      const bf_t* w_tag) {
  // ::1-2
  memcpy(y, w, FAEST_LAMBDA / 8);
  memcpy(y_tag, w_tag, FAEST_LAMBDA * sizeof(bf_t));

  // ::3
  unsigned int i_wd = FAEST_LAMBDA;
  // ::4-10
  for (unsigned int j = FAEST_NK; j < 4 * (FAEST_R + 1); j++) {
    // ::5
    if ((j % FAEST_NK == 0) || ((FAEST_NK > 6) && (j % FAEST_NK == 4))) {
      // ::6
      memcpy(&y[32 * j / 8], &w[i_wd / 8], 32 / 8);
      memcpy(&y_tag[32 * j], &w_tag[i_wd], 32 * sizeof(bf_t));
      // ::7
      i_wd += 32;
      // ::8
    } else {
      // ::9-10
      xor_small_u8_array(&y[32 * j / 8], &y[32 * (j - FAEST_NK) / 8], &y[32 * (j - 1) / 8], 32 / 8);
      for (unsigned int word_idx = 0; word_idx < 32; word_idx++) {
        bf_add(&y_tag[32 * j + word_idx], &y_tag[32 * (j - FAEST_NK) + word_idx],
               &y_tag[32 * (j - 1) + word_idx]);
      }
    }
  }
}

static void aes_keyexp_forward_verifier(bf_t* y_key, const bf_t* w_key) {
  // ::1-2
  memcpy(y_key, w_key, FAEST_LAMBDA * sizeof(bf_t));

  // ::3
  unsigned int i_wd = FAEST_LAMBDA;
  // ::4-10
  for (unsigned int j = FAEST_NK; j < 4 * (FAEST_R + 1); j++) {
    // ::5
    if ((j % FAEST_NK == 0) || ((FAEST_NK > 6) && (j % FAEST_NK == 4))) {
      // ::6
      memcpy(&y_key[32 * j], &w_key[i_wd], 32 * sizeof(bf_t));
      // ::7
      i_wd += 32; // 32 bits -> 4 words
      // ::8
    } else {
      // ::9-10
      for (unsigned int word_idx = 0; word_idx < 32; word_idx++) {
        bf_add(&y_key[32 * j + word_idx], &y_key[32 * (j - FAEST_NK) + word_idx],
               &y_key[32 * (j - 1) + word_idx]);
      }
    }
  }
}

// // KEY EXP CSTRNTS
static void aes_expkey_constraints_prover(zk_hash_7_ctx* hasher, uint8_t* k, bf_t* k_tag,
                                          const uint8_t* w, const bf_t* w_tag,
                                          const faest_paramset_t* params) {
  const unsigned int Ske = params->Ske;

  // line 1
  aes_keyexp_forward_prover(k, k_tag, w, w_tag);
  // line 2
  uint8_t* w_flat  = malloc(Ske); // IGNORE memory-check
  bf_t* w_flat_tag = BF_ALLOC(8 * Ske);
  assert(w_flat);
  assert(w_flat_tag);
  aes_keyexp_backward_prover(w_flat, w_flat_tag, w + FAEST_LAMBDA / 8, w_tag + FAEST_LAMBDA, k,
                             k_tag, params);

  // line 5
  for (unsigned int j = 0, iwd = 32 * (FAEST_NK - 1); j < Ske; j += 4, iwd += IWD_INC) {

    for (unsigned int r = 0; r < 4; r++) {
#if FAEST_LAMBDA == 256
      // line 10
      unsigned int r_prime = r;

      if (j % 8 == 0) {
        r_prime = (r + 1) % 4;
      }
#else
      unsigned int r_prime = (r + 1) % 4;
#endif
      bf_t k_hat;    // expanded key witness
      bf_t w_hat;    // inverse output
      bf_t k_hat_sq; // expanded key witness sq
      bf_t w_hat_sq; // inverse output sq

      bf_t k_hat_tag;    // expanded key witness tag
      bf_t w_hat_tag;    // inverse output tag
      bf_t k_hat_tag_sq; // expanded key tag sq
      bf_t w_hat_tag_sq; // inverser output tag sq

      // line 12
      bf_byte_combine_bits(&k_hat, k[(iwd + 8 * r_prime) / 8]); // lifted key witness
      bf_square(&k_hat_sq, &k_hat);                             // lifted key witness sq

      bf_byte_combine_bits(&w_hat, w_flat[(8 * j + 8 * r) / 8]); // lifted output
      bf_square(&w_hat_sq, &w_hat);                              // lifted output sq

      // done by both prover and verifier
      bf_byte_combine(&k_hat_tag, k_tag + (iwd + 8 * r_prime)); // lifted key tag
      bf_byte_combine_sq(&k_hat_tag_sq,
                         k_tag + (iwd + 8 * r_prime)); // lifted key tag sq

      bf_byte_combine(&w_hat_tag, w_flat_tag + ((8 * j + 8 * r))); // lifted output tag
      bf_byte_combine_sq(&w_hat_tag_sq,
                         w_flat_tag + (8 * j + 8 * r)); // lifted output tag sq

      // line 19
      bf_t v1;
      bf_t tmp;
      bf_mul(&v1, &k_hat_sq, &w_hat_tag);
      bf_mul(&tmp, &k_hat_tag_sq, &w_hat);
      bf_add_inplace(&v1, &k_hat_tag);
      bf_add_inplace(&v1, &tmp);
      bf_mul(&tmp, &k_hat_tag_sq, &w_hat_tag);
      zk_hash_7_raise_and_update(hasher, &tmp, &v1);

      bf_mul(&v1, &k_hat, &w_hat_tag_sq);
      bf_mul(&tmp, &k_hat_tag, &w_hat_sq);
      bf_add_inplace(&v1, &w_hat_tag);
      bf_add_inplace(&v1, &tmp);
      bf_mul(&tmp, &k_hat_tag, &w_hat_tag_sq);
      zk_hash_7_raise_and_update(hasher, &tmp, &v1);
    }
  }
  faest_aligned_free(w_flat_tag);
  free(w_flat); // IGNORE memory-check
}

static void aes_expkey_constraints_verifier(zk_hash_ctx* hasher, bf_t* k_key, const bf_t* w_key,
                                            const bf_t* delta_i, const faest_paramset_t* params) {
  const unsigned int Ske = params->Ske;

  // ::1
  aes_keyexp_forward_verifier(k_key, w_key);
  // ::2
  bf_t* w_flat_key = BF_ALLOC(8 * Ske);
  assert(w_flat_key);
  aes_keyexp_backward_verifier(w_flat_key, w_key + FAEST_LAMBDA, k_key, &delta_i[1], params);

  // ::3-5,::7
  for (unsigned int j = 0, iwd = 32 * (FAEST_NK - 1); j < Ske; j += 4, iwd += IWD_INC) {
    // ::9
    for (unsigned int r = 0; r < 4; r++) {
#if FAEST_LAMBDA == 256
      // ::10
      unsigned int r_prime = r;
      // ::11
      if (j % 8 == 0) {
        r_prime = (r + 1) % 4;
      }
#else
      unsigned int r_prime = (r + 1) % 4;
#endif
      bf_t k_hat_key;    // expanded key witness tag
      bf_t w_hat_key;    // inverse output tag
      bf_t k_hat_key_sq; // expanded key tag sq
      bf_t w_hat_key_sq; // inverser output tag sq

      // ::12-15
      bf_byte_combine(&k_hat_key, k_key + (iwd + 8 * r_prime)); // lifted key tag
      bf_byte_combine_sq(&k_hat_key_sq,
                         k_key + (iwd + 8 * r_prime)); // lifted key tag sq

      bf_byte_combine(&w_hat_key, w_flat_key + ((8 * j + 8 * r))); // lifted output tag
      bf_byte_combine_sq(&w_hat_key_sq,
                         w_flat_key + (8 * j + 8 * r)); // lifted output tag sq

      // ::17-20
      // also raise degree
      bf_t v;
      bf_t tmp;
      bf_mul(&v, &k_hat_key_sq, &w_hat_key);
      bf_mul(&tmp, &delta_i[1], &k_hat_key);
      bf_add_inplace(&v, &tmp);
      bf_mul_inplace(&v, &delta_i[5]);
      zk_hash_update(hasher, &v);

      bf_mul(&v, &k_hat_key, &w_hat_key_sq);
      bf_mul(&tmp, &delta_i[1], &w_hat_key);
      bf_add_inplace(&v, &tmp);
      bf_mul_inplace(&v, &delta_i[5]);
      zk_hash_update(hasher, &v);
    }
  }
  faest_aligned_free(w_flat_key);
}

// // ENC CSTRNTS
static void aes_enc_constraints_prover(zk_hash_7_ctx* hasher, const uint8_t* owf_in,
                                       const bf_t* owf_in_tag, const uint8_t* owf_out,
                                       const bf_t* owf_out_tag, const uint8_t* w, const bf_t* w_tag,
                                       const uint8_t* k, const bf_t* k_tag, unsigned int Nst) {
  const unsigned int Nstbits  = 32 * Nst;
  const unsigned int Nstbytes = Nstbits / 8;

  /// line 1
  uint8_t* state_bits = malloc(4 * Nstbytes); // IGNORE memory-check
  bf_t* state_bits_tag =
      BF_ALLOC(Nstbits + 5 * 8 * Nstbytes + 3 * Nstbits + 8 * Nstbytes + 18 * Nstbytes);
  assert(state_bits);
  assert(state_bits_tag);

  aes_add_round_key_prover(state_bits, state_bits_tag, owf_in, owf_in_tag, k, k_tag, Nst);

  uint8_t* s_tilde      = &state_bits[Nstbytes];
  bf_t* s_tilde_tag     = &state_bits_tag[Nstbits + 5 * 8 * Nstbytes];
  uint8_t* s_dash_dash  = &state_bits[2 * Nstbytes];
  bf_t* s_dash_dash_tag = &state_bits_tag[Nstbits + 5 * 8 * Nstbytes + Nstbits];
  uint8_t* s            = &state_bits[3 * Nstbytes];
  bf_t* s_tag           = &state_bits_tag[Nstbits + 5 * 8 * Nstbytes + 2 * Nstbits];

  bf_t* b_deg[8];
  b_deg[0] = &s_tag[Nstbits];
  for (unsigned int m = 0; m < 8; m++) {
    if (m) {
      b_deg[m] = b_deg[0] + m * Nstbytes;
    }
    for (unsigned n = 0; n < Nstbytes; n++) {
      b_deg[m][n] = bf_zero();
    }
  }

  // line 2
  for (unsigned int r = 0; r < FAEST_R / 2; r++) {
    // line 3
    bf_t* state_deg7[8];
    for (unsigned int m = 0; m < 8; m++) {
      state_deg7[m] = &b_deg[7][Nstbytes] + m * Nstbytes;
    }

    // line 4
    aes_sbox_eval_prover(state_deg7, state_bits, state_bits_tag, Nst);

    // line 5
    bf_t* state_deg7_tmp[8];
    for (unsigned int m = 0; m < 8; m++) {
      state_deg7_tmp[m] = &state_deg7[7][Nstbytes] + m * Nstbytes;
    }
    aes_shiftrows_prover(state_deg7_tmp, (const bf_t**)state_deg7, Nst);

    // line 6
    aes_mix_columns_prover(state_deg7, (const bf_t**)state_deg7_tmp, Nst);

    // line 7
    bf_t* k_deg[2];
    for (unsigned int m = 0; m < 2; m++) {
      k_deg[m] = &state_deg7_tmp[7][Nstbytes] + m * Nstbytes;
    }
    aes_state_to_bytes_prover(k_deg, k + (2 * r + 1) * Nstbytes, k_tag + (2 * r + 1) * Nstbits,
                              Nst);
    aes_add_round_key_bytes_prover_degree_7(state_deg7_tmp, (const bf_t**)state_deg7,
                                            (const bf_t**)k_deg, Nst);

    // line 10
    if (r == FAEST_R / 2 - 1) {
      aes_add_round_key_prover(s_tilde, s_tilde_tag, owf_out, owf_out_tag, k + FAEST_R * Nstbytes,
                               k_tag + FAEST_R * Nstbits, Nst);
    } else {
      // line 11
      memcpy(s_tilde, &w[(Nstbits * r) / 8], Nstbytes);
      memcpy(s_tilde_tag, &w_tag[(Nstbits * r)], Nstbits * sizeof(bf_t));
    }

    // line 15
    aes_inverse_shiftrows_prover(s_dash_dash, s_dash_dash_tag, s_tilde, s_tilde_tag, Nst);
    // line 16
    aes_inverse_sbox_eval_prover(b_deg, s_dash_dash, s_dash_dash_tag, Nst);

    // line 17
    bf_t tmp[8];
    for (unsigned int byte_i = 0; byte_i < Nstbytes; byte_i++) {
      for (unsigned int m = 0; m < 8; m++) {
        bf_add(&tmp[m], &b_deg[m][byte_i], &state_deg7_tmp[m][byte_i]);
      }
      // NOTE: getting the z's
      zk_hash_7_update(hasher, tmp);
    }

    // line 21
    if (r != (FAEST_R / 2) - 1) {
      uint8_t* tmp_state  = s;
      bf_t* tmp_state_tag = s_tag;
      aes_bitwise_mix_column_prover(tmp_state, tmp_state_tag, s_tilde, s_tilde_tag, Nst);
      aes_add_round_key_prover(state_bits, state_bits_tag, tmp_state, tmp_state_tag,
                               k + (2 * r + 2) * Nstbytes, k_tag + (2 * r + 2) * Nstbits, Nst);
    }
  }

  faest_aligned_free(state_bits_tag);
  free(state_bits); // IGNORE memory-check
}

static void aes_enc_constraints_verifier(zk_hash_ctx* hasher, const bf_t* owf_in_key,
                                         const bf_t* owf_out_key, const bf_t* w_key,
                                         const bf_t* rkeys_key, const bf_t* delta_i,
                                         unsigned int Nst) {
  const unsigned int Nstbits  = 32 * Nst;
  const unsigned int Nstbytes = Nstbits / 8;

  /// line 1
  bf_t* state_bits_key = BF_ALLOC(Nstbits + 5 * 8 * Nstbytes + 3 * Nstbits + 4 * Nstbytes);
  aes_add_round_key_verifier(state_bits_key, owf_in_key, rkeys_key, Nst);

  bf_t* s_tilde_key     = &state_bits_key[Nstbits + 5 * 8 * Nstbytes];
  bf_t* s_dash_dash_key = &state_bits_key[Nstbits + 5 * 8 * Nstbytes + Nstbits];

  bf_t* s_key = &state_bits_key[Nstbits + 5 * 8 * Nstbytes + 2 * Nstbits];
  bf_t* b_key = s_key + Nstbits; // Nstbytes

  // line 2
  for (unsigned int r = 0; r < FAEST_R / 2; r++) {
    // line 3
    bf_t* state_key = b_key + Nstbytes; // Nstbytes

    // line 4
    aes_sbox_eval_verifier(state_key, state_bits_key, delta_i, Nst);

    // line 5
    bf_t* state_key_tmp = state_key + Nstbytes; // Nstbytes
    aes_shiftrows_verifier(state_key_tmp, state_key, Nst);

    // line 6
    aes_mix_columns_verifier(state_key, state_key_tmp, Nst);

    // line 7
    bf_t* k_key = state_key_tmp + Nstbytes; // Nstbytes
    aes_state_to_bytes_verifier(k_key, rkeys_key + (2 * r + 1) * Nstbits, Nst);
    aes_add_round_key_bytes_verifier(state_key_tmp, state_key, k_key, delta_i, Nst);

    // line 10
    if (r == FAEST_R / 2 - 1) {
      aes_add_round_key_verifier(s_tilde_key, owf_out_key, rkeys_key + FAEST_R * Nstbits, Nst);
    } else {
      // line 11
      memcpy(s_tilde_key, &w_key[Nstbits * r], Nstbits * sizeof(bf_t));
    }

    // line 15
    aes_inverse_shiftrows_verifier(s_dash_dash_key, s_tilde_key, Nst);
    // line 16
    for (unsigned n = 0; n < Nstbytes; n++) {
      b_key[n] = bf_zero();
    }
    aes_inverse_sbox_eval_verifier(b_key, s_dash_dash_key, delta_i, Nst);

    // line 17
    bf_t tmp;
    for (unsigned int byte_i = 0; byte_i < Nstbytes; byte_i++) {
      bf_add(&tmp, &b_key[byte_i], &state_key_tmp[byte_i]);
      zk_hash_update(hasher, &tmp);
    }

    // line 21
    if (r != (FAEST_R / 2) - 1) {
      bf_t* tmp_state_key = s_key;
      aes_bitwise_mix_column_verifier(tmp_state_key, s_tilde_key, Nst);
      aes_add_round_key_verifier(state_bits_key, tmp_state_key, rkeys_key + (2 * r + 2) * Nstbits,
                                 Nst);
    }
  }

  faest_aligned_free(state_bits_key);
}

// OWF CONSTRAINTS
static void aes_constraints_prover(zk_hash_7_ctx* hasher, const uint8_t* w, const bf_t* w_tag,
                                   const uint8_t* owf_in, const uint8_t* owf_out,
                                   const faest_paramset_t* params) {
  const unsigned int Lke       = params->Lke;
  const unsigned int Lenc      = params->Lenc;
  const unsigned int Nst       = params->Nst;
  const unsigned int blocksize = 32 * params->Nst;
  const unsigned int beta      = (FAEST_LAMBDA + blocksize - 1) / blocksize;
  // ::1-3 owf_in, owf_out, z and z_tag

  // line 5
  bf_t v1;
  bf_t tmp;
  bf_mul_bit(&v1, &w_tag[0], ptr_get_bit(w, 1));
  bf_mul_bit(&tmp, &w_tag[1], ptr_get_bit(w, 0));
  bf_add_inplace(&v1, &tmp);
  bf_mul(&tmp, &w_tag[0], &w_tag[1]);
  zk_hash_7_raise_and_update(hasher, &tmp, &v1);

  // line 7
  uint8_t* in     = malloc((beta + FAEST_R + 2) * blocksize / 8); // IGNORE memory-check
  uint8_t* out    = &in[blocksize / 8];
  uint8_t* rkeys  = &in[(beta + 1) * blocksize / 8];
  bf_t* in_tag    = BF_ALLOC(blocksize * (beta + FAEST_R + 2));
  bf_t* out_tag   = &in_tag[blocksize];
  bf_t* rkeys_tag = &in_tag[blocksize * (beta + 1)];
  assert(in);
  assert(in_tag);

  if (faest_is_em(params)) {
    aes_round_keys_t round_keys;
    expand_key(&round_keys, owf_in, FAEST_NK, FAEST_NK, FAEST_R);

    for (unsigned int r = 0, idx = 0; r < FAEST_R + 1; r++) {
      for (unsigned int n = 0; n < Nst; n++) {
        for (unsigned int i = 0; i < 4; ++i, ++idx) {
          rkeys[idx] = round_keys.round_keys[r][n][i];
        }
      }
    }
    memset(rkeys_tag, 0, sizeof(bf_t) * (FAEST_R + 1) * Nst * 4 * 8);

    // line 10
    memcpy(in, w, blocksize / 8);
    memcpy(in_tag, w_tag, blocksize * sizeof(bf_t));
    // line 11
    xor_u8_array(out, w, owf_out, blocksize / 8);
    memcpy(out_tag, w_tag, blocksize * sizeof(bf_t));
  } else {
    // line 13
    memcpy(in, owf_in, blocksize / 8);
    constant_to_vole_prover(in_tag, blocksize);

    // line 14-15
    memcpy(out, owf_out, beta * blocksize / 8);
    constant_to_vole_prover(out_tag, beta * blocksize);

    // line 16
    aes_expkey_constraints_prover(hasher, rkeys, rkeys_tag, w, w_tag, params);
  }

  // line 18
  for (unsigned int b = 0; b < beta; b++) {
    // line 20
    if (b == 1) {
      in[0] = in[0] ^ 0x01;
    }
    // line 21
    aes_enc_constraints_prover(hasher, in, in_tag, out + b * blocksize / 8, out_tag + b * blocksize,
                               w + (Lke + b * Lenc) / 8, w_tag + Lke + b * Lenc, rkeys, rkeys_tag,
                               Nst);
  }

  faest_aligned_free(in_tag);
  free(in); // IGNORE memory-check
}

// OWF CONSTRAINTS VERIFIER
static void aes_constraints_verifier(zk_hash_ctx* hasher, const bf_t* w_key, const uint8_t* owf_in,
                                     const uint8_t* owf_out, const bf_t* delta_i,
                                     const faest_paramset_t* params) {
  const unsigned int Lke       = params->Lke;
  const unsigned int Lenc      = params->Lenc;
  const unsigned int Nst       = params->Nst;
  const unsigned int blocksize = 32 * Nst;
  const unsigned int beta      = (FAEST_LAMBDA + blocksize - 1) / blocksize;

  // line 5
  bf_t v;
  bf_mul(&v, &w_key[0], &w_key[1]);
  bf_mul_inplace(&v, &delta_i[5]);
  zk_hash_update(hasher, &v);

  // line 7
  bf_t* rkeys_key = BF_ALLOC((FAEST_R + 2 + beta) * blocksize);
  bf_t* in_key    = &rkeys_key[(FAEST_R + 1) * blocksize];
  bf_t* out_key   = &rkeys_key[(FAEST_R + 2) * blocksize];
  assert(rkeys_key);

  if (faest_is_em(params)) {
    aes_round_keys_t round_keys;
    expand_key(&round_keys, owf_in, FAEST_NK, FAEST_NK, FAEST_R);

    unsigned int idx = 0;
    for (unsigned int r = 0; r < FAEST_R + 1; ++r) {
      for (unsigned int n = 0; n < Nst; ++n) {
        for (unsigned int i = 0; i < 4; ++i) {
          uint8_t rk_byte = round_keys.round_keys[r][n][i];
          for (unsigned int j = 0; j < 8; ++j, ++idx) {
            bf_mul_bit(&rkeys_key[idx], &delta_i[1], get_bit(rk_byte, j));
          }
        }
      }
    }
    // ::10-11
    memcpy(in_key, w_key, blocksize * sizeof(bf_t));
    for (unsigned int i = 0; i < blocksize; i++) {
      bf_mul_bit(&out_key[i], &delta_i[1], ptr_get_bit(owf_out, i));
      bf_add_inplace(&out_key[i], &w_key[i]);
    }
  } else {
    // line 13
    constant_to_vole_verifier(in_key, owf_in, &delta_i[1], blocksize);

    // line 14-15
    // if beta=2, load both public key blocks
    constant_to_vole_verifier(out_key, owf_out, &delta_i[1], beta * blocksize);

    // line 16
    aes_expkey_constraints_verifier(hasher, rkeys_key, w_key, delta_i, params);
  }
  // line 18
  for (unsigned int b = 0; b < beta; b++) {
    // line 20
    if (b == 1) {
      bf_add_inplace(&in_key[0], &delta_i[1]); // adding one and multiplying with delta
    }
    aes_enc_constraints_verifier(hasher, in_key, out_key + b * blocksize, w_key + Lke + b * Lenc,
                                 rkeys_key, delta_i, Nst);
  }

  faest_aligned_free(rkeys_key);
}

// OWF PROVER
void aes_prover(uint8_t* a0_tilde, uint8_t* a1toi_tilde, const uint8_t* w, uint8_t** V,
                const uint8_t* u_bar, const uint8_t* v_bar, const uint8_t* owf_in,
                const uint8_t* owf_out, const uint8_t* chall_2, const faest_paramset_t* params) {
  const unsigned int ell          = params->ell;
  const unsigned int lambda_bytes = params->lambda / 8;

  // line 3
  // V becomes the w_tag: ell + 2*lambda field elements
  bf_t* w_tag = shrink_V(V, ell); // This is the tag for w

  // line 7 deg-7
  zk_hash_7_ctx hasher7;
  zk_hash_7_init(&hasher7, chall_2);

  for (unsigned int ell_idx = 0; ell_idx < ell; ell_idx++) {
    bf_t bf_deg_1[2];
    bf_t bf_deg_0;
    bf_square(&bf_deg_0, &w_tag[ell_idx]); // deg-0 * deg-0

    bf_mul_bit(&bf_deg_1[0], &w_tag[ell_idx], 1 ^ ptr_get_bit(w, ell_idx)); // deg-0 * deg-1
    bf_mul_bit(&bf_deg_1[1], &w_tag[ell_idx], ptr_get_bit(w, ell_idx));     // deg-1 * deg-0
    bf_add_inplace(&bf_deg_1[0], &bf_deg_1[1]);                             // adding deg-1s

    zk_hash_7_raise_and_update(&hasher7, &bf_deg_0, bf_deg_1);
  }

  // line 9 deg-7
  aes_constraints_prover(&hasher7, w, w_tag, owf_in, owf_out, params);

  // line 13 deg-7
  bf_t x1_sum[7];
  bf_sum_poly_bits(&x1_sum[0], v_bar);
  for (unsigned int j = 1; j < D_ZK - 1; j++) {
    bf_sum_poly_bits(&x1_sum[j], u_bar + (j - 1) * lambda_bytes);

    bf_t tmp;
    bf_sum_poly_bits(&tmp, v_bar + j * lambda_bytes);
    bf_add_inplace(&x1_sum[j], &tmp);
  }
  bf_sum_poly_bits(&x1_sum[D_ZK - 1], u_bar + (D_ZK - 2) * lambda_bytes);

  // deg-7
  zk_hash_7_finalize(a0_tilde, a1toi_tilde, &hasher7, x1_sum);

  faest_aligned_free(w_tag);
}

// OWF VERIFIER
void aes_verifier(uint8_t* a0_tilde, const uint8_t* d, uint8_t** Q, const uint8_t* q_bar,
                  const uint8_t* owf_in, const uint8_t* owf_out, const uint8_t* chall_2,
                  const uint8_t* delta, const uint8_t* a1toi_tilde,
                  const faest_paramset_t* params) {
  const unsigned int ell          = params->ell;
  const unsigned int lambda_bytes = params->lambda / 8;

  // line 3
  bf_t* Q_key = shrink_V(Q, ell);

  // line 12
  zk_hash_ctx b_ctx;
  zk_hash_init(&b_ctx, chall_2);

  // line 3,5 initializing
  bf_t bf_delta[D_ZK + 1];
  bf_delta[0] = bf_one();
  bf_load(&bf_delta[1], delta);
  bf_square(&bf_delta[2], &bf_delta[1]);
  bf_mul(&bf_delta[3], &bf_delta[2], &bf_delta[1]);
  bf_square(&bf_delta[4], &bf_delta[2]);
  bf_mul(&bf_delta[5], &bf_delta[4], &bf_delta[1]);
  bf_square(&bf_delta[6], &bf_delta[3]);
  bf_mul(&bf_delta[7], &bf_delta[6], &bf_delta[1]);

  // line 3
  for (unsigned int i = 0; i < ell; i++) {
    bf_t tmp;
    bf_mul_bit(&tmp, &bf_delta[1], ptr_get_bit(d, i));
    bf_add_inplace(&Q_key[i], &tmp);
  }

  // line 5
  bf_t q_star;
  bf_sum_poly_bits(&q_star, q_bar);
  for (unsigned int j = 1; j < D_ZK - 1; j++) {
    bf_t tmp;
    bf_sum_poly_bits(&tmp, q_bar + j * lambda_bytes);
    bf_mul_inplace(&tmp, &bf_delta[j]);
    bf_add_inplace(&q_star, &tmp);
  }

  // line 7
  for (unsigned int i = 0; i < ell; i++) {
    bf_t t, u;
    bf_add(&u, &Q_key[i], &bf_delta[1]); // 1 (delta) - w
    bf_mul(&t, &Q_key[i], &u);           // w * (1 (delta) - w)
    bf_mul_inplace(&t, &bf_delta[5]);
    zk_hash_update(&b_ctx, &t);
  }

  // line 9
  aes_constraints_verifier(&b_ctx, Q_key, owf_in, owf_out, bf_delta, params);

  faest_aligned_free(Q_key);

  // line 12
  uint8_t q_tilde[FAEST_LAMBDA / 8];
  zk_hash_finalize(q_tilde, &b_ctx, &q_star);

  // line 14
  bf_t acc;
  bf_load(&acc, a1toi_tilde);
  bf_mul_inplace(&acc, &bf_delta[1]);

  bf_t tmp;
  bf_load(&tmp, q_tilde);
  bf_add_inplace(&acc, &tmp);

  for (unsigned int j = 2; j < D_ZK; j++) {
    bf_load(&tmp, a1toi_tilde + (j - 1) * lambda_bytes);
    bf_mul_inplace(&tmp, &bf_delta[j]);
    bf_add_inplace(&acc, &tmp);
  }

  bf_store(a0_tilde, &acc);
}
