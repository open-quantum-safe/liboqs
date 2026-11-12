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
#define FAEST_LAMBDA FAEST_256_LAMBDA
#define FAEST_NK (FAEST_LAMBDA / 32)
#define FAEST_R FAEST_256S_R

#if FAEST_LAMBDA == 192
#define IWD_INC 192
#else
#define IWD_INC 128
#endif

#define bf_t bf256_t
#define bf_load bf256_load
#define bf_from_bit bf256_from_bit
#define bf_store bf256_store
#define bf_zero bf256_zero
#define bf_one bf256_one
#define bf_add bf256_add
#define bf_add_inplace bf256_add_inplace
#define bf_mul bf256_mul
#define bf_mul_inplace bf256_mul_inplace
#define bf_mul_bit bf256_mul_bit
#define bf_mul_bit_8 bf256_mul_bit_8
#define bf_square bf256_square
#define bf_byte_combine bf256_byte_combine
#define bf_byte_combine_bits bf256_byte_combine_bits
#define bf_byte_combine_sq bf256_byte_combine_sq
#define bf_sq_bit bf256_sq_bit
#define bf_sq_bit_inplace bf256_sq_bit_inplace
#define bf_sum_poly_bits bf256_sum_poly_bits
#define BF_NUM_BYTES BF256_NUM_BYTES
#define BF_ALIGN BF256_ALIGN

#define zk_hash_7_ctx zk_hash_256_7_ctx
#define zk_hash_7_finalize zk_hash_256_7_finalize
#define zk_hash_7_init zk_hash_256_7_init
#define zk_hash_7_raise_and_update zk_hash_256_7_raise_and_update
#define zk_hash_7_update zk_hash_256_7_update
#define zk_hash_ctx zk_hash_256_ctx
#define zk_hash_finalize zk_hash_256_finalize
#define zk_hash_init zk_hash_256_init
#define zk_hash_update zk_hash_256_update

#define aes_prover aes_256_prover
#define aes_verifier aes_256_verifier

static_assert(D_ZK == 7, "Invalid assumptions.");

static_assert(FAEST_LAMBDA == FAEST_256S_LAMBDA, "Invalid parameters");
static_assert(FAEST_256F_ELL == FAEST_256S_ELL, "Invalid parameters");
static_assert(FAEST_256F_LAMBDA == FAEST_256S_LAMBDA, "Invalid parameters");
static_assert(FAEST_256F_Lke == FAEST_256S_Lke, "Invalid parameters");
static_assert(FAEST_256F_Nst == FAEST_256S_Nst, "Invalid parameters");
static_assert(FAEST_R == FAEST_256S_R, "Invalid parameters");
static_assert(FAEST_256F_R == FAEST_256S_R, "Invalid parameters");
static_assert(FAEST_256F_Ske == FAEST_256S_Ske, "Invalid parameters");

static_assert(FAEST_LAMBDA == FAEST_EM_256S_LAMBDA, "Invalid parameters");
static_assert(FAEST_EM_256F_LAMBDA == FAEST_EM_256S_LAMBDA, "Invalid parameters");
static_assert(FAEST_EM_256F_Lenc == FAEST_EM_256S_Lenc, "Invalid parameters");
static_assert(FAEST_EM_256F_Nst == FAEST_EM_256S_Nst, "Invalid parameters");
static_assert(FAEST_R == FAEST_EM_256S_R, "Invalid parameters");
static_assert(FAEST_EM_256F_R == FAEST_EM_256S_R, "Invalid parameters");
// for scan-build
static_assert(FAEST_EM_256F_LAMBDA * (FAEST_EM_256F_R + 1) / 8 ==
                  sizeof(aes_word_t) * FAEST_EM_256F_Nst * (FAEST_EM_256F_R + 1),
              "Invalid parameters");
// clang-format on

/* pad sizes to multiples of some value; assumes that a is a power of 2 */
#define BF_ALLOC(s) faest_aligned_alloc(BF_ALIGN, ALIGN_TO((s) * sizeof(bf_t), BF_ALIGN))

#define RCON_EVERY (4 * (FAEST_LAMBDA / 128))

#if FAEST_LAMBDA == 128
static ATTR_ALIGNED(BF_ALIGN) const uint8_t bf_c_bytes[9][BF128_NUM_BYTES] = {
    {0xe0, 0xae, 0x88, 0x34, 0xca, 0x59, 0x77, 0xec, 0x84, 0xbb, 0xbf, 0x9c, 0x43, 0xb7, 0xf4,
     0x4c},
    {0xa9, 0x46, 0x39, 0x36, 0xae, 0x02, 0xcf, 0xbf, 0xc6, 0xd2, 0x51, 0x7d, 0x4f, 0x60, 0xad,
     0x35},
    {0xca, 0x5a, 0x1b, 0xb5, 0xba, 0x07, 0x36, 0x4c, 0x08, 0xbe, 0xc0, 0x04, 0x9a, 0xd2, 0x2f,
     0xb3},
    {0x54, 0x2c, 0x93, 0x4f, 0xed, 0x10, 0x5c, 0xc9, 0x21, 0x65, 0x37, 0x86, 0xa2, 0xa7, 0x6c,
     0x18},
    {0x82, 0xb2, 0xaa, 0xb7, 0xde, 0x5c, 0x8e, 0x1f, 0x4a, 0xd7, 0x2e, 0xe5, 0x96, 0x05, 0x76,
     0xca},
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00},
    {0xa1, 0x4d, 0x8b, 0x92, 0x31, 0xae, 0xa5, 0xd8, 0x3f, 0xc8, 0x22, 0x89, 0xfe, 0xa0, 0xf7,
     0x1c},
    {0xf9, 0xdf, 0xe7, 0xb6, 0x5c, 0x3d, 0xfd, 0x88, 0xbe, 0x7c, 0xce, 0x07, 0x43, 0x63, 0x34,
     0x75},
    {0x9a, 0xb3, 0x5a, 0x0b, 0x64, 0x53, 0x3f, 0x43, 0xcc, 0x5b, 0x13, 0xdf, 0xdc, 0x30, 0x24,
     0x87},
};
#elif FAEST_LAMBDA == 192
static ATTR_ALIGNED(BF_ALIGN) const uint8_t bf_c_bytes[9][BF192_NUM_BYTES] = {
    {0xba, 0x50, 0xf4, 0x7c, 0x9e, 0x61, 0x33, 0xb2, 0x26, 0x3f, 0x63, 0xd5,
     0x19, 0x1f, 0xf6, 0x7b, 0x34, 0xdb, 0x91, 0xd4, 0x26, 0x37, 0x93, 0xda},
    {0x0c, 0x8a, 0x39, 0xf5, 0x13, 0x2c, 0x6d, 0x9c, 0x19, 0x8d, 0x32, 0x06,
     0x77, 0xe3, 0x32, 0x82, 0xf6, 0x4e, 0x75, 0x3c, 0x70, 0x0d, 0x3b, 0x0c},
    {0x62, 0x32, 0x0f, 0x49, 0x39, 0x95, 0x03, 0xfb, 0x28, 0x28, 0x65, 0x07,
     0xa7, 0x27, 0x82, 0x63, 0x11, 0x02, 0x84, 0x85, 0x8d, 0xa3, 0x70, 0x71},
    {0x42, 0x7b, 0x3a, 0xf6, 0x7c, 0x6d, 0xe6, 0x6a, 0xa2, 0x81, 0xbd, 0xb2,
     0xae, 0x93, 0xe0, 0x73, 0x71, 0xab, 0x37, 0x9f, 0x4c, 0x23, 0xee, 0x0c},
    {0xd5, 0xe8, 0xc2, 0xc0, 0xb4, 0xd8, 0x5d, 0xd5, 0x17, 0x9a, 0x34, 0xd4,
     0xc9, 0xdb, 0x46, 0x9a, 0xd3, 0x97, 0x60, 0x6d, 0xdb, 0x99, 0xd8, 0xa7},
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x6e, 0x09, 0x17, 0x2e, 0xc2, 0x44, 0x52, 0x42, 0x5f, 0xba, 0xe3, 0x8b,
     0x67, 0x2c, 0xac, 0x69, 0x90, 0x7d, 0x0c, 0x04, 0x84, 0x24, 0x7e, 0x8e},
    {0xa5, 0xc8, 0xf3, 0x83, 0x9a, 0xb3, 0x02, 0x17, 0x06, 0xe6, 0x39, 0x7c,
     0x60, 0xd6, 0xb8, 0x7f, 0x10, 0x67, 0x08, 0xe9, 0x16, 0xa4, 0xdf, 0x20},
    {0x20, 0x5d, 0x23, 0x0b, 0x41, 0x33, 0x12, 0x83, 0x26, 0x23, 0x67, 0xae,
     0x75, 0xac, 0x99, 0x1d, 0x64, 0xac, 0xa1, 0x2c, 0x21, 0x61, 0xd0, 0xce},
};
#elif FAEST_LAMBDA == 256
static ATTR_ALIGNED(BF_ALIGN) const uint8_t bf_c_bytes[9][BF256_NUM_BYTES] = {
    {0xc0, 0x89, 0x22, 0xd5, 0x2a, 0xf5, 0x5a, 0xa9, 0x2f, 0x07, 0x42,
     0x2c, 0x8d, 0xc4, 0xa5, 0x2b, 0xea, 0xb0, 0x00, 0x6c, 0x37, 0x0d,
     0x4a, 0xd1, 0xf1, 0x4a, 0x5b, 0x9c, 0x69, 0x4d, 0x4e, 0x06},
    {0x1c, 0x9d, 0x80, 0x3f, 0x83, 0xb3, 0xda, 0x55, 0x57, 0x0f, 0x3b,
     0x53, 0x1e, 0x83, 0x71, 0x17, 0x10, 0xac, 0x3f, 0xad, 0x3f, 0x57,
     0x96, 0xfb, 0x8d, 0xf6, 0x11, 0x70, 0xdb, 0xe3, 0x95, 0x61},
    {0xff, 0xef, 0x67, 0x0b, 0x92, 0x5a, 0x2f, 0x37, 0x92, 0xa8, 0xb3,
     0x4a, 0x4b, 0xa2, 0x48, 0x87, 0x95, 0xaf, 0x99, 0x9e, 0x08, 0x28,
     0x4b, 0x21, 0x02, 0x63, 0xa1, 0x4d, 0x46, 0x7a, 0x73, 0xc6},
    {0x76, 0xb9, 0xa8, 0x3c, 0x03, 0x00, 0x98, 0x31, 0xad, 0x83, 0xb3,
     0x6c, 0x59, 0x1f, 0x61, 0x30, 0x6b, 0x19, 0x49, 0x11, 0xa3, 0x04,
     0xf4, 0xfe, 0x0d, 0x05, 0x0c, 0xed, 0x04, 0x46, 0x69, 0x24},
    {0x22, 0xfb, 0xc5, 0xe1, 0x3b, 0x1c, 0xaf, 0xcb, 0xea, 0xa0, 0xca,
     0x35, 0xd8, 0xe5, 0x9c, 0xbb, 0x6f, 0xb3, 0xa6, 0x5f, 0x00, 0x72,
     0x97, 0x0b, 0x7e, 0xdf, 0xeb, 0xa1, 0xf4, 0xd4, 0xa8, 0xa1},
    {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x28, 0x9c, 0x81, 0x3f, 0x82, 0xb3, 0xda, 0x55, 0x56, 0x0f, 0x3b,
     0x53, 0x1e, 0x83, 0x71, 0x17, 0x11, 0xac, 0x3f, 0xad, 0x3f, 0x57,
     0x96, 0xfb, 0x8d, 0xf6, 0x11, 0x70, 0xdb, 0xe3, 0x95, 0x61},
    {0xb1, 0x02, 0x4e, 0x52, 0x73, 0x78, 0x54, 0xd0, 0x12, 0x29, 0x86,
     0xfa, 0x7d, 0x0e, 0x35, 0xd0, 0xd5, 0xe8, 0x26, 0xcd, 0x67, 0xe9,
     0x9b, 0xda, 0xf7, 0x9f, 0xe8, 0x1e, 0x74, 0xec, 0x2c, 0x70},
    {0x5b, 0xa9, 0x10, 0x3c, 0xd2, 0xd2, 0x20, 0x90, 0x59, 0x65, 0x47,
     0x86, 0x2a, 0x30, 0xff, 0x09, 0x95, 0x57, 0x21, 0x77, 0xee, 0xb4,
     0x9d, 0x00, 0x3c, 0x3e, 0x64, 0x80, 0x8d, 0x94, 0xd3, 0xe6},
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
static void aes_f256_f2_conjugates_byte_1(bf_t* y, uint8_t state) {
  bf_byte_combine_bits(&y[0], state);
  for (unsigned int j = 1; j != 8; ++j) {
    bf_square(&y[j], &y[j - 1]);
  }
}

static void aes_f256_f2_conjugates_byte(bf_t* y, const bf_t* state) {
  bf_t x[8];
  bf_byte_combine(&y[0], state);
  bf_sq_bit(x, state);
  for (unsigned int j = 1; j != 7; ++j) {
    bf_byte_combine(&y[j], x);
    bf_sq_bit_inplace(x);
  }
  bf_byte_combine(&y[7], x);
}

// STATE TO BYTES
static void aes_state_to_bytes_verifier(bf_t* out_key, const bf_t* k_key, unsigned int Nst) {
  const unsigned int Nst_bytes = Nst * 4;
  for (unsigned int i = 0; i < Nst_bytes; i++) {
    bf_byte_combine(&out_key[i], k_key + i * 8);
  }
}

static void aes_state_to_bytes_prover(bf_t out_deg[][2], const uint8_t* k, const bf_t* k_tag,
                                      unsigned int Nst) {
  const unsigned int Nst_bytes = Nst * 4;
  for (unsigned int i = 0; i < Nst_bytes; i++) {
    bf_byte_combine_bits(&out_deg[i][1], k[i]);
    bf_byte_combine(&out_deg[i][0], k_tag + i * 8);
  }
}

// SBOX AFFINE
static void aes_sbox_eval_prover(bf_t state_deg7[][8], const uint8_t* state_bits,
                                 bf_t* state_bits_tag, unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  for (unsigned int j = 0; j < Nstbytes; j++) {
    bf_t state_conj[8];
    bf_t state_conj_tag[8];
    aes_f256_f2_conjugates_byte_1(state_conj, state_bits[j]);
    aes_f256_f2_conjugates_byte(state_conj_tag, state_bits_tag + j * 8);

    // line 3+5: zeta is precomputed as bf_c
    for (unsigned int m = 0; m < 7; m++) {
      state_deg7[j][m] = bf_zero();
    }
    bf_load(&state_deg7[j][7], bf_c_bytes[8]);

    bf_t p2_deg[2][5];
    bf_t f2_deg[2][4];

    for (unsigned int i = 0; i < 2; i++) {
      for (unsigned int m = 0; m < 5; m++) {
        p2_deg[i][m] = bf_zero();
      }
      for (unsigned int m = 0; m < 4; m++) {
        f2_deg[i][m] = bf_zero();
      }

      // Only the current pair of quadratic and linear factors is needed.
      bf_t p1_deg[2][3];
      bf_t f1_deg[2][2];
      for (unsigned int k = 0; k < 2; k++) {
        const unsigned int idx = 4 * i + 2 * k;
        bf_mul(&p1_deg[k][0], &state_conj_tag[idx], &state_conj_tag[idx + 1]);

        bf_t tmp;
        bf_mul(&tmp, &state_conj[idx], &state_conj_tag[idx + 1]);
        bf_mul(&p1_deg[k][1], &state_conj_tag[idx], &state_conj[idx + 1]);
        bf_add_inplace(&p1_deg[k][1], &tmp);
        bf_mul(&p1_deg[k][2], &state_conj[idx], &state_conj[idx + 1]);

        bf_t constant;
        bf_load(&constant, bf_c_bytes[idx]);
        bf_mul(&f1_deg[k][0], &constant, &state_conj_tag[idx + 1]);
        bf_mul(&f1_deg[k][1], &constant, &state_conj[idx + 1]);
        bf_load(&constant, bf_c_bytes[idx + 1]);
        bf_mul(&tmp, &constant, &state_conj_tag[idx]);
        bf_add_inplace(&f1_deg[k][0], &tmp);
        bf_mul(&tmp, &constant, &state_conj[idx]);
        bf_add_inplace(&f1_deg[k][1], &tmp);
      }

      for (unsigned int m = 0; m < 3; m++) {
        for (unsigned int n = 0; n < 3; n++) {
          bf_t tmp;
          bf_mul(&tmp, &p1_deg[0][m], &p1_deg[1][n]);
          bf_add_inplace(&p2_deg[i][m + n], &tmp);
        }
      }

      for (unsigned int m = 0; m < 2; m++) {
        for (unsigned int n = 0; n < 3; n++) {
          bf_t tmp;
          bf_mul(&tmp, &f1_deg[0][m], &p1_deg[1][n]);
          bf_add_inplace(&f2_deg[i][m + n], &tmp);

          bf_mul(&tmp, &f1_deg[1][m], &p1_deg[0][n]);
          bf_add_inplace(&f2_deg[i][m + n], &tmp);
        }
      }
    }

    for (unsigned int m = 0; m < 4; m++) {
      for (unsigned int n = 0; n < 5; n++) {
        bf_t tmp;
        bf_mul(&tmp, &f2_deg[0][m], &p2_deg[1][n]);
        bf_add_inplace(&state_deg7[j][n + m], &tmp);

        bf_mul(&tmp, &f2_deg[1][m], &p2_deg[0][n]);
        bf_add_inplace(&state_deg7[j][n + m], &tmp);
      }
    }
  }
}

static void aes_sbox_eval_verifier(bf_t* state_key, const bf_t* state_bits_key, const bf_t* delta_i,
                                   unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  bf_t state_conj_key[8];

  // line 3+5: zeta is precomputed as bf_c
  bf_t zeta_delta_i;
  bf_load(&zeta_delta_i, bf_c_bytes[8]);
  bf_mul_inplace(&zeta_delta_i, &delta_i[6]);

  // line 6
  for (unsigned int j = 0; j < Nstbytes; j++) {
    aes_f256_f2_conjugates_byte(state_conj_key, state_bits_key + j * 8);
    bf_t p1_key[4];
    bf_t f1_key[4];
    bf_t p2_key[2];
    bf_t f2_key[2];

    // line 8
    for (unsigned int i = 0; i < 4; i++) {
      // line 9
      bf_mul(&p1_key[i], &state_conj_key[i * 2], &state_conj_key[i * 2 + 1]); // deg-1 * deg-1

      // line 10
      bf_t prod_1_key, prod_2_key;
      bf_load(&prod_1_key, bf_c_bytes[i * 2]);
      bf_load(&prod_2_key, bf_c_bytes[i * 2 + 1]);
      bf_mul_inplace(&prod_1_key, &state_conj_key[i * 2 + 1]); // deg-0 * deg-1
      bf_mul_inplace(&prod_2_key, &state_conj_key[i * 2]);     // deg-0 * deg-1

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
}

// SHIFT ROWS
static void aes_shiftrows_prover(bf_t out_deg[][8], const bf_t in_deg[][8], unsigned int Nst) {
  for (unsigned int r = 0; r < 4; r++) {
    for (unsigned int c = 0; c < Nst; c++) {
      unsigned int i;
      if ((Nst != 8) || (r <= 1)) {
        i = 4 * ((c + r) % Nst) + r;
      } else {
        i = 4 * ((c + r + 1) % Nst) + r;
      }
      for (unsigned int m = 0; m < 8; m++) {
        out_deg[4 * c + r][m] = in_deg[i][m];
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
static ATTR_ALIGNED(BF_ALIGN) const uint8_t bf_bc_2_bytes[BF128_NUM_BYTES] = {
    0x0d, 0xce, 0x60, 0x55, 0xac, 0xe8, 0x3f, 0xa1, 0x1c, 0x9a, 0x97, 0xa9, 0x55, 0x85, 0x3d, 0x05};
#elif FAEST_LAMBDA == 192
static ATTR_ALIGNED(BF_ALIGN) const uint8_t bf_bc_2_bytes[BF192_NUM_BYTES] = {
    0x63, 0x97, 0x38, 0x6f, 0xd5, 0xa3, 0xc8, 0xcc, 0xea, 0xbd, 0x6e, 0x96,
    0x6c, 0xd7, 0x65, 0xe6, 0x62, 0x36, 0x6b, 0x0e, 0x14, 0xc8, 0x0b, 0x31};
#elif FAEST_LAMBDA == 256
static ATTR_ALIGNED(BF_ALIGN) const uint8_t bf_bc_2_bytes[BF256_NUM_BYTES] = {
    0xe7, 0xfe, 0xde, 0x0b, 0x42, 0x88, 0x97, 0x96, 0x67, 0x4e, 0x47, 0xa0, 0x38, 0x8d, 0xd6, 0xbe,
    0x6a, 0xe1, 0xf1, 0xf8, 0x45, 0x98, 0x22, 0xdf, 0x33, 0x58, 0xc9, 0x20, 0xcf, 0xa8, 0xc9, 0x04};
#else
#error "unexpected security parameter"
#endif

static void aes_mix_column(bf_t* y, const bf_t* in, unsigned int stride) {
  bf_t sum;
  bf_add(&sum, &in[0], &in[stride]);
  bf_add_inplace(&sum, &in[2 * stride]);
  bf_add_inplace(&sum, &in[3 * stride]);

  for (unsigned int r = 0; r != 4; ++r) {
    bf_t adjacent;
    bf_t product;
    bf_load(&product, bf_bc_2_bytes);
    bf_add(&adjacent, &in[r * stride], &in[((r + 1) % 4) * stride]);
    bf_mul_inplace(&product, &adjacent);
    bf_add(&y[r * stride], &in[r * stride], &sum);
    bf_add_inplace(&y[r * stride], &product);
  }
}

static void aes_mix_columns_prover(bf_t y_deg[][8], const bf_t in_deg[][8], unsigned int Nst) {
  for (unsigned int c = 0; c < Nst; c++) {
    for (unsigned int m = 0; m < 8; ++m) {
      aes_mix_column(&y_deg[4 * c][m], &in_deg[4 * c][m], 8);
    }
  }
}

static void aes_mix_columns_verifier(bf_t* y_key, const bf_t* in_key, unsigned int Nst) {
  for (unsigned int c = 0; c < Nst; c++) {
    aes_mix_column(y_key + 4 * c, in_key + 4 * c, 1);
  }
}

// ADD ROUND KEY BYTES
static void aes_add_round_key_bytes_prover_degree_7(bf_t out_deg[][8], const bf_t in_deg[][8],
                                                    const bf_t k_deg[][2], unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  for (unsigned int i = 0; i < Nstbytes; i++) {
    memcpy(out_deg[i], in_deg[i], sizeof(bf_t) * 6);
    bf_add(&out_deg[i][6], &in_deg[i][6], &k_deg[i][0]);
    bf_add(&out_deg[i][7], &in_deg[i][7], &k_deg[i][1]);
  }
}

// Use shift_tag if key is degree-1 instead of degree-2
static void aes_add_round_key_bytes_verifier(bf_t* y_key, const bf_t* in_key, const bf_t* k_key,
                                             const bf_t* delta_i, unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  for (unsigned int i = 0; i < Nstbytes; i++) {
    // Multiply tag by delta to align degrees
    bf_t tmp;
    bf_mul(&tmp, &k_key[i], &delta_i[5]);
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
static inline void constant_to_vole_prover(bf_t* tag, unsigned int n) {
  // the val stay the same as the val is a pub const!
  // for constant values the tag is zero
  memset(tag, 0, sizeof(bf_t) * n);
}

static void constant_to_vole_verifier(bf_t* key, const uint8_t* val, const bf_t* delta,
                                      unsigned int n) {
  const unsigned int full_bits = n & ~(8u - 1);

  unsigned int i = 0;
  for (; i < full_bits; i += 8) {
    bf_mul_bit_8(&key[i], delta, val[i / 8]);
  }
  for (; i < n; i++) {
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

static void aes_inverse_sbox_eval_prover(bf_t b_deg[][8], const uint8_t* s_dash_dash,
                                         const bf_t* s_dash_dash_tag, unsigned int Nst) {
  const unsigned int Nstbytes = Nst * 4;

  bf_t s_tag[8];
  bf_t state_conj[8];
  bf_t state_conj_tag[8];

  for (unsigned int j = 0; j < Nstbytes; j++) {
    uint8_t s;
    aes_inverse_affine_byte_prover(&s, s_tag, s_dash_dash[j], s_dash_dash_tag + j * 8);
    aes_f256_f2_conjugates_byte_1(state_conj, s);
    aes_f256_f2_conjugates_byte(state_conj_tag, s_tag);

    b_deg[j][0] = state_conj_tag[1];
    b_deg[j][1] = state_conj[1];
    for (unsigned int i = 2; i < 8; i++) {
      const bf_t* t = &state_conj_tag[i];
      const bf_t* v = &state_conj[i];

      // Process in reverse so that the previous coefficients remain available.
      bf_mul(&b_deg[j][i], &b_deg[j][i - 1], v);
      for (unsigned int m = i - 1; m > 0; m--) {
        bf_t tmp;
        bf_mul(&tmp, &b_deg[j][m - 1], v);
        bf_mul_inplace(&b_deg[j][m], t);
        bf_add_inplace(&b_deg[j][m], &tmp);
      }
      bf_mul_inplace(&b_deg[j][0], t);
    }
  }
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

static void aes_inverse_sbox_eval_verifier(bf_t* b_key, const bf_t* s_dash_dash_tag,
                                           const bf_t* delta_i, unsigned int Nst) {

  const unsigned int Nstbytes = Nst * 4;

  bf_t s_tag[8];
  bf_t state_conj_tag[8];

  for (unsigned int j = 0; j < Nstbytes; j++) {
    aes_inverse_affine_byte_verifier(s_tag, s_dash_dash_tag + j * 8, &delta_i[0]);
    aes_f256_f2_conjugates_byte(state_conj_tag, s_tag);
    b_key[j] = state_conj_tag[1];
    for (unsigned int i = 2; i < 8; i++) {
      bf_mul_inplace(&b_key[j], &state_conj_tag[i]);
    }
  }
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
static void aes_keyexp_backward_byte_prover(uint8_t* y, bf_t* y_tag, bf_t* x_tilde_tag,
                                            const uint8_t* x, const bf_t* x_tag, const uint8_t* key,
                                            const bf_t* key_tag, unsigned int j) {
  const unsigned int key_offset = (j / 4) * IWD_INC + (j % 4) * 8;
  uint8_t x_tilde               = x[j] ^ key[key_offset / 8];
  for (unsigned int bit_i = 0; bit_i < 8; bit_i++) {
    bf_add(&x_tilde_tag[bit_i], &x_tag[j * 8 + bit_i], &key_tag[key_offset + bit_i]);
  }
  if (j % RCON_EVERY == 0) {
    x_tilde ^= aes_round_constants[j / RCON_EVERY];
  }
  aes_inverse_affine_byte_prover(y, y_tag, x_tilde, x_tilde_tag);
}

static void aes_keyexp_backward_byte_verifier(bf_t* y_key, bf_t* x_tilde_key, const bf_t* x_key,
                                              const bf_t* key_key, const bf_t* delta,
                                              unsigned int j) {
  const unsigned int key_offset = (j / 4) * IWD_INC + (j % 4) * 8;
  for (unsigned int bit_i = 0; bit_i < 8; bit_i++) {
    bf_add(&x_tilde_key[bit_i], &x_key[j * 8 + bit_i], &key_key[key_offset + bit_i]);
    if (j % RCON_EVERY == 0) {
      bf_t rcon_key;
      const uint8_t c = get_bit(aes_round_constants[j / RCON_EVERY], bit_i);
      constant_to_vole_verifier(&rcon_key, &c, delta, 1);
      bf_add_inplace(&x_tilde_key[bit_i], &rcon_key);
    }
  }
  aes_inverse_affine_byte_verifier(y_key, x_tilde_key, delta);
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
  // One inverse-affine byte and its input scratch, reused for every constraint.
  bf_t w_flat_tag[16];

  // line 5
  for (unsigned int j = 0, iwd = 32 * (FAEST_NK - 1); j < Ske; j += 4, iwd += IWD_INC) {

    for (unsigned int r = 0; r < 4; r++) {
      uint8_t w_flat;
      aes_keyexp_backward_byte_prover(&w_flat, w_flat_tag, w_flat_tag + 8, w + FAEST_LAMBDA / 8,
                                      w_tag + FAEST_LAMBDA, k, k_tag, j + r);
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

      bf_byte_combine_bits(&w_hat, w_flat); // lifted output
      bf_square(&w_hat_sq, &w_hat);         // lifted output sq

      // done by both prover and verifier
      bf_byte_combine(&k_hat_tag, k_tag + (iwd + 8 * r_prime)); // lifted key tag
      bf_byte_combine_sq(&k_hat_tag_sq,
                         k_tag + (iwd + 8 * r_prime)); // lifted key tag sq

      bf_byte_combine(&w_hat_tag, w_flat_tag);       // lifted output tag
      bf_byte_combine_sq(&w_hat_tag_sq, w_flat_tag); // lifted output tag sq

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
}

static void aes_expkey_constraints_verifier(zk_hash_ctx* hasher, bf_t* k_key, const bf_t* w_key,
                                            const bf_t* delta_i, const faest_paramset_t* params) {
  const unsigned int Ske = params->Ske;

  // ::1
  aes_keyexp_forward_verifier(k_key, w_key);
  // ::2
  bf_t w_flat_key[16];

  // ::3-5,::7
  for (unsigned int j = 0, iwd = 32 * (FAEST_NK - 1); j < Ske; j += 4, iwd += IWD_INC) {
    // ::9
    for (unsigned int r = 0; r < 4; r++) {
      aes_keyexp_backward_byte_verifier(w_flat_key, w_flat_key + 8, w_key + FAEST_LAMBDA, k_key,
                                        &delta_i[0], j + r);
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

      bf_byte_combine(&w_hat_key, w_flat_key);       // lifted output tag
      bf_byte_combine_sq(&w_hat_key_sq, w_flat_key); // lifted output tag sq

      // ::17-20
      // also raise degree
      bf_t v;
      bf_t tmp;
      bf_mul(&v, &k_hat_key_sq, &w_hat_key);
      bf_mul(&tmp, &delta_i[0], &k_hat_key);
      bf_add_inplace(&v, &tmp);
      bf_mul_inplace(&v, &delta_i[4]);
      zk_hash_update(hasher, &v);

      bf_mul(&v, &k_hat_key, &w_hat_key_sq);
      bf_mul(&tmp, &delta_i[0], &w_hat_key);
      bf_add_inplace(&v, &tmp);
      bf_mul_inplace(&v, &delta_i[4]);
      zk_hash_update(hasher, &v);
    }
  }
}

// // ENC CSTRNTS
static void aes_enc_constraints_prover(zk_hash_7_ctx* hasher, const uint8_t* owf_in,
                                       const bf_t* owf_in_tag, const uint8_t* owf_out,
                                       const bf_t* owf_out_tag, const uint8_t* w, const bf_t* w_tag,
                                       const uint8_t* k, const bf_t* k_tag, unsigned int Nst) {
  const unsigned int Nstbits  = 32 * Nst;
  const unsigned int Nstbytes = Nstbits / 8;

  /// line 1
  uint8_t* state_bits  = faest_malloc(4 * Nstbytes);
  bf_t* state_bits_tag = BF_ALLOC(4 * Nstbits + 16 * Nstbytes);
  assert(state_bits);
  assert(state_bits_tag);

  aes_add_round_key_prover(state_bits, state_bits_tag, owf_in, owf_in_tag, k, k_tag, Nst);

  uint8_t* s_tilde      = &state_bits[Nstbytes];
  bf_t* s_tilde_tag     = &state_bits_tag[Nstbits];
  uint8_t* s_dash_dash  = &state_bits[2 * Nstbytes];
  bf_t* s_dash_dash_tag = &state_bits_tag[2 * Nstbits];
  uint8_t* s            = &state_bits[3 * Nstbytes];
  bf_t* s_tag           = &state_bits_tag[3 * Nstbits];

  bf_t(*b_deg)[8] = (bf_t(*)[8]) & s_tag[Nstbits];

  // line 2
  for (unsigned int r = 0; r < FAEST_R / 2; r++) {
    // line 3
    // The forward state is consumed before the inverse S-box fills b_deg.
    bf_t(*state_deg7)[8] = b_deg;

    // line 4
    aes_sbox_eval_prover(state_deg7, state_bits, state_bits_tag, Nst);

    // line 5
    bf_t(*state_deg7_tmp)[8] = state_deg7 + Nstbytes;
    aes_shiftrows_prover(state_deg7_tmp, (const bf_t(*)[8])state_deg7, Nst);

    // line 6
    aes_mix_columns_prover(state_deg7, (const bf_t(*)[8])state_deg7_tmp, Nst);

    // line 7
    // s_tag is only needed for the bitwise MixColumns at the end of the round.
    bf_t(*k_deg)[2] = (bf_t(*)[2])s_tag;
    aes_state_to_bytes_prover(k_deg, k + (2 * r + 1) * Nstbytes, k_tag + (2 * r + 1) * Nstbits,
                              Nst);
    aes_add_round_key_bytes_prover_degree_7(state_deg7_tmp, (const bf_t(*)[8])state_deg7,
                                            (const bf_t(*)[2])k_deg, Nst);

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
        bf_add(&tmp[m], &b_deg[byte_i][m], &state_deg7_tmp[byte_i][m]);
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
  faest_free(state_bits);
}

static void aes_enc_constraints_verifier(zk_hash_ctx* hasher, const bf_t* owf_in_key,
                                         const bf_t* owf_out_key, const bf_t* w_key,
                                         const bf_t* rkeys_key, const bf_t* delta_i,
                                         unsigned int Nst) {
  const unsigned int Nstbits  = 32 * Nst;
  const unsigned int Nstbytes = Nstbits / 8;

  /// line 1
  bf_t* state_bits_key = BF_ALLOC(4 * Nstbits + 2 * Nstbytes);
  aes_add_round_key_verifier(state_bits_key, owf_in_key, rkeys_key, Nst);

  bf_t* s_tilde_key     = &state_bits_key[Nstbits];
  bf_t* s_dash_dash_key = &state_bits_key[2 * Nstbits];

  bf_t* s_key = &state_bits_key[3 * Nstbits];
  bf_t* b_key = s_key + Nstbits; // Nstbytes

  // line 2
  for (unsigned int r = 0; r < FAEST_R / 2; r++) {
    // line 3
    // Reuse the forward state for the inverse S-box result later in the round.
    bf_t* state_key = b_key; // Nstbytes

    // line 4
    aes_sbox_eval_verifier(state_key, state_bits_key, delta_i, Nst);

    // line 5
    bf_t* state_key_tmp = state_key + Nstbytes; // Nstbytes
    aes_shiftrows_verifier(state_key_tmp, state_key, Nst);

    // line 6
    aes_mix_columns_verifier(state_key, state_key_tmp, Nst);

    // line 7
    bf_t* k_key = s_key; // Consumed before bitwise MixColumns at the end of the round.
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
  uint8_t* in     = faest_malloc((beta + FAEST_R + 2) * blocksize / 8);
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
          rkeys[idx] = (uint8_t)(round_keys.round_keys[r][n] >> (8 * i));
        }
      }
    }
    constant_to_vole_prover(rkeys_tag, (FAEST_R + 1) * Nst * 4 * 8);

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
  faest_free(in);
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
  bf_mul_inplace(&v, &delta_i[4]);
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
        for (unsigned int i = 0; i < 4; ++i, idx += 8) {
          uint8_t rk_byte = (uint8_t)(round_keys.round_keys[r][n] >> (8 * i));
          bf_mul_bit_8(&rkeys_key[idx], &delta_i[0], rk_byte);
        }
      }
    }
    // ::10-11
    memcpy(in_key, w_key, blocksize * sizeof(bf_t));
    for (unsigned int i = 0; i < blocksize; i += 8) {
      bf_mul_bit_8(&out_key[i], &delta_i[0], owf_out[i / 8]);
      for (unsigned int j = 0; j < 8; ++j) {
        bf_add_inplace(&out_key[i + j], &w_key[i + j]);
      }
    }
  } else {
    // line 13
    constant_to_vole_verifier(in_key, owf_in, &delta_i[0], blocksize);

    // line 14-15
    // if beta=2, load both public key blocks
    constant_to_vole_verifier(out_key, owf_out, &delta_i[0], beta * blocksize);

    // line 16
    aes_expkey_constraints_verifier(hasher, rkeys_key, w_key, delta_i, params);
  }
  // line 18
  for (unsigned int b = 0; b < beta; b++) {
    // line 20
    if (b == 1) {
      // adding one and multiplying with delta
      bf_add_inplace(&in_key[0], &delta_i[0]);
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
  bf_t x1_sum[D_ZK];
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
  bf_t bf_delta[D_ZK];
  bf_load(&bf_delta[0], delta);
  bf_square(&bf_delta[1], &bf_delta[0]);
  bf_mul(&bf_delta[2], &bf_delta[1], &bf_delta[0]);
  bf_square(&bf_delta[3], &bf_delta[1]);
  bf_mul(&bf_delta[4], &bf_delta[3], &bf_delta[0]);
  bf_square(&bf_delta[5], &bf_delta[2]);
  bf_mul(&bf_delta[6], &bf_delta[5], &bf_delta[0]);

  // line 3
  {
    const unsigned int full_bits_ell = ell & ~(8u - 1);

    unsigned int i = 0;
    for (; i < full_bits_ell; i += 8) {
      bf_t tmp[8];
      bf_mul_bit_8(tmp, &bf_delta[0], d[i / 8]);
      for (unsigned int j = 0; j < 8; ++j) {
        bf_add_inplace(&Q_key[i + j], &tmp[j]);
      }
    }

    for (; i < ell; i++) {
      bf_t tmp;
      bf_mul_bit(&tmp, &bf_delta[0], ptr_get_bit(d, i));
      bf_add_inplace(&Q_key[i], &tmp);
    }
  }

  // line 7
  for (unsigned int i = 0; i < ell; i++) {
    bf_t u;
    bf_add(&u, &Q_key[i], &bf_delta[0]); // 1 (delta) - w
    bf_mul_inplace(&u, &Q_key[i]);       // w * (1 (delta) - w)
    bf_mul_inplace(&u, &bf_delta[4]);
    zk_hash_update(&b_ctx, &u);
  }

  // line 9
  aes_constraints_verifier(&b_ctx, Q_key, owf_in, owf_out, bf_delta, params);

  faest_aligned_free(Q_key);

  // line 12
  uint8_t q_tilde[FAEST_LAMBDA / 8];
  {
    // line 5
    bf_t q_star;
    bf_sum_poly_bits(&q_star, q_bar);
    for (unsigned int j = 1; j < D_ZK - 1; j++) {
      bf_t tmp;
      bf_sum_poly_bits(&tmp, q_bar + j * lambda_bytes);
      bf_mul_inplace(&tmp, &bf_delta[j - 1]);
      bf_add_inplace(&q_star, &tmp);
    }
    zk_hash_finalize(q_tilde, &b_ctx, &q_star);
  }

  // line 14
  bf_t acc;
  bf_load(&acc, a1toi_tilde);
  bf_mul_inplace(&acc, &bf_delta[0]);

  bf_t tmp;
  bf_load(&tmp, q_tilde);
  bf_add_inplace(&acc, &tmp);

  for (unsigned int j = 1; j < D_ZK - 1; j++) {
    bf_load(&tmp, a1toi_tilde + j * lambda_bytes);
    bf_mul_inplace(&tmp, &bf_delta[j]);
    bf_add_inplace(&acc, &tmp);
  }

  bf_store(a0_tilde, &acc);
}
