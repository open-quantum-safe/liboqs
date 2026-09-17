#include <aes128_ctrle.h>
#include <immintrin.h>
#include <memory.h>
#include <rijndael256_ctrle.h>

#include "aes_ansi_ref.h"
#include "sdith_prng_private.h"
#include "vole_private.h"


// ggm seed rng, extended-seed variants (avx2): encrypt with the precomputed round
// keys (encrypt_nblocks) instead of scheduling in oneshot. Same domain sep as the
// raw variant (node_idx, not node_idx<<1), so byte-identical to the raw output.
EXPORT void ggm_seed_rng_lr_ext_cat1_aes128_avx2(void* lr_out256, const void* salt128, const void* extkey128,
                                                 uint64_t node_idx, uint32_t theta) {
  CASSERT((node_idx & 1) == 0, "bug! this function must be called on the left child index");
  CASSERT(domain_sep_check(salt128, node_idx, 2, theta), "bug! invalid separation");
  __m128i ctr = domain_sep_ptx_lo2_m128(salt128, node_idx);
  aes128_ctrle_nocarry_2block_avx2(lr_out256, extkey128, &ctr);
}
// I8: batched 4-node variant. Expands parents node_idx0, +2, +4, +6 in one interleaved
// 8-block encrypt so the AES-NI port stays saturated. extkeys4 = 4 contiguous extended
// keys (176 B each); lr_out = 4 contiguous 2-seed outputs. Byte-identical to four calls of
// ggm_seed_rng_lr_ext_cat1_aes128_avx2 (KAT-preserving).
EXPORT void ggm_seed_rng_lr_ext_cat1_aes128_x4_avx2(void* lr_out, const void* salt128,
                                                    const void* extkeys4, uint64_t node_idx0, uint32_t theta) {
  CASSERT((node_idx0 & 1) == 0, "bug! this function must be called on the left child index");
  __m128i ctr[4];
  for (int i = 0; i < 4; i++) {
    CASSERT(domain_sep_check(salt128, node_idx0 + 2 * i, 2, theta), "bug! invalid separation");
    ctr[i] = domain_sep_ptx_lo2_m128(salt128, node_idx0 + 2 * i);
  }
  aes128_ctrle_nocarry_2blk_x4keys_avx2(lr_out, extkeys4, ctr);
}
EXPORT void ggm_seed_rng_lr_ext_cat3_rijndael256_avx2(  //
    void* lr_out384, const void* salt192, const void* extkey, uint64_t node_idx, uint32_t theta) {
  CASSERT((node_idx & 1) == 0, "bug! this function must be called on the left child index");
  uint8_t out512[64] = {};
  CASSERT(domain_sep_check(salt192, node_idx, 2, theta), "bug! invalid separation");
  ctr256_vec ctr = domain_sep_ptx_lo2_m256_from192(salt192, node_idx);
  rijndael256_ctrle_nocarry_2block_avx2(out512, extkey, &ctr);
  // cat3 Enc = LSB_192(Rijndael-256): keep the low 24 bytes of each block
  memcpy(lr_out384, out512, 24);
  memcpy((uint8_t*)lr_out384 + 24, out512 + 32, 24);
}
EXPORT void ggm_seed_rng_lr_ext_cat5_rijndael256_avx2(  //
    void* lr_out512, const void* salt256, const void* extkey, uint64_t node_idx, uint32_t theta) {
  CASSERT((node_idx & 1) == 0, "bug! this function must be called on the left child index");
  CASSERT(domain_sep_check(salt256, node_idx, 2, theta), "bug! invalid separation");
  ctr256_vec ctr = domain_sep_ptx_lo2_m256(salt256, node_idx);
  rijndael256_ctrle_nocarry_2block_avx2(lr_out512, extkey, &ctr);
}
// I8: naive x4 wrappers (avx2). cat3/cat5 rijndael has no batched key trick on this
// box (no vaes), so these just do the four single calls. cat1 avx2 uses the batched
// kernel above. LR_STRIDE = 2 * lambda_bytes, EXT_STRIDE = one extended key.
GGM_EXTSEED_RNG_LR_X4_NAIVE(ggm_seed_rng_lr_ext_cat3_rijndael256_x4_avx2,  //
                            ggm_seed_rng_lr_ext_cat3_rijndael256_avx2, 48, RIJNDAEL256_RK_BYTES)
GGM_EXTSEED_RNG_LR_X4_NAIVE(ggm_seed_rng_lr_ext_cat5_rijndael256_x4_avx2,  //
                            ggm_seed_rng_lr_ext_cat5_rijndael256_avx2, 64, RIJNDAEL256_RK_BYTES)

// extended leaf seed (avx2): run the key schedule once, share commit + vole.
EXPORT void extend_leaf_seed_cat1_aes128_avx2(void* extended_seed_out, const void* seed128) {
  aes128_key_schedule_x1_avx2(extended_seed_out, seed128);
}
EXPORT void extend_leaf_seed_cat3_rijndael256_avx2(void* extended_seed_out, const void* seed192) {
  uint8_t key256[32] = {};  // k || 0^64
  memcpy(key256, seed192, 24);
  rijndael256_key_schedule_x1_avx2(extended_seed_out, key256);
}
EXPORT void extend_leaf_seed_cat5_rijndael256_avx2(void* extended_seed_out, const void* seed256) {
  rijndael256_key_schedule_x1_avx2(extended_seed_out, seed256);
}

// 4x leaf-seed expansion (avx2): dispatch to the batched key-schedule kernels.
// Seeds/outputs are packed at their natural stride; output is byte-identical to
// four 1x calls (KAT-preserving). All three use an x4 kernel that interleaves the
// four independent keys to hide the SubWord (aesenclast) latency.
EXPORT void extend_leaf_seed_cat1_aes128_4x_avx2(void* extended_seed_out, const void* seeds) {
  uint8_t* o = (uint8_t*)extended_seed_out;
  const uint8_t* s = (const uint8_t*)seeds;
  aes128_key_schedule_x4_avx2(o, o + 176, o + 352, o + 528, s, s + 16, s + 32, s + 48);
}
EXPORT void extend_leaf_seed_cat3_rijndael256_4x_avx2(void* extended_seed_out, const void* seeds) {
  uint8_t* o = (uint8_t*)extended_seed_out;
  const uint8_t* s = (const uint8_t*)seeds;
  uint8_t k[4][32] = {};  // each seed padded to k || 0^64
  for (int i = 0; i < 4; i++) memcpy(k[i], s + i * 24, 24);
  rijndael256_key_schedule_x4_avx2(o, o + 480, o + 960, o + 1440, k[0], k[1], k[2], k[3]);
}
EXPORT void extend_leaf_seed_cat5_rijndael256_4x_avx2(void* extended_seed_out, const void* seeds) {
  uint8_t* o = (uint8_t*)extended_seed_out;
  const uint8_t* s = (const uint8_t*)seeds;
  rijndael256_key_schedule_x4_avx2(o, o + 480, o + 960, o + 1440, s, s + 32, s + 64, s + 96);
}

// commit rng, extended-seed cores (avx2): encrypt with the precomputed round keys.
EXPORT void ggm_commit_rng_ext_cat1_aes128_avx2(void* output256, const void* salt128, const void* extkey128,
                                                uint64_t node_idx) {
  CASSERT(domain_sep_check(salt128, node_idx << 1, 2, GGM_TWEAK_BITS), "bug! invalid separation");
  __m128i ctr = domain_sep_ptx_lo2_m128(salt128, node_idx << 1);
  aes128_ctrle_nocarry_2block_avx2(output256, extkey128, &ctr);
}
EXPORT void ggm_commit_rng_ext_cat3_rijndael256_avx2(  //
    void* output384, const void* salt192, const void* extkey, uint64_t node_idx) {
  uint8_t output512[64] = {};  // pad with zeroes
  CASSERT(domain_sep_check(salt192, node_idx << 1, 2, GGM_TWEAK_BITS), "bug! invalid separation");
  ctr256_vec ctr = domain_sep_ptx_lo2_m256_from192(salt192, node_idx << 1);
  rijndael256_ctrle_nocarry_2block_avx2(output512, extkey, &ctr);
  // cat3 Enc = LSB_192(Rijndael-256): keep the low 24 bytes of each block
  memcpy(output384, output512, 24);
  memcpy((uint8_t*)output384 + 24, output512 + 32, 24);
}
EXPORT void ggm_commit_rng_ext_cat5_rijndael256_avx2(  //
    void* output512, const void* salt256, const void* extkey, uint64_t node_idx) {
  CASSERT(domain_sep_check(salt256, node_idx << 1, 2, GGM_TWEAK_BITS), "bug! invalid separation");
  ctr256_vec ctr = domain_sep_ptx_lo2_m256(salt256, node_idx << 1);
  rijndael256_ctrle_nocarry_2block_avx2(output512, extkey, &ctr);
}

// vole rng, extended-seed cores (avx2): adopt the precomputed round keys.
// The counter starts at the salt tweaked by the repetition index (see
// VOLE_RNG_REPET_SHIFT), and the domain_sep_check asserts that the last block of
// the stream still fits in the tweak field.
EXPORT void vole_rng_ext_cat1_aes128_ctrle_nocarry_avx2(void* out, uint64_t out_bytes, const void* salt128,
                                                        const void* extseed, uint64_t repet_idx) {
  CASSERT((out_bytes & 15)==0, "bug! out_bytes must be multiple of 16");
  CASSERT(domain_sep_check(salt128, repet_idx << VOLE_RNG_REPET_SHIFT, out_bytes >> 4, GGM_TWEAK_BITS),
          "bug! invalid separation");
  __m128i ctr = domain_sep_ptx_lo2_m128(salt128, repet_idx << VOLE_RNG_REPET_SHIFT);
  aes128_ctrle_nocarry_nblocks_avx2(out, extseed, &ctr, out_bytes >> 4);
}
EXPORT void vole_rng_ext_cat3_rijndael256_ctrle_avx2(void* out, uint64_t out_bytes, const void* salt192,
                                                     const void* extseed, uint64_t repet_idx) {
  CASSERT((out_bytes & 31)==0, "bug! out_bytes must be multiple of 32");
  CASSERT(domain_sep_check(salt192, repet_idx << VOLE_RNG_REPET_SHIFT, out_bytes >> 5, GGM_TWEAK_BITS),
          "bug! invalid separation");
  ctr256_vec ctr = domain_sep_ptx_lo2_m256_from192(salt192, repet_idx << VOLE_RNG_REPET_SHIFT);
  rijndael256_ctrle_nocarry_nblocks_avx2(out, extseed, &ctr, out_bytes >> 5);
}
EXPORT void vole_rng_ext_cat5_rijndael256_ctrle_avx2(void* out, uint64_t out_bytes, const void* salt256,
                                                     const void* extseed, uint64_t repet_idx) {
  CASSERT((out_bytes & 31)==0, "bug! out_bytes must be multiple of 32");
  CASSERT(domain_sep_check(salt256, repet_idx << VOLE_RNG_REPET_SHIFT, out_bytes >> 5, GGM_TWEAK_BITS),
          "bug! invalid separation");
  ctr256_vec ctr = domain_sep_ptx_lo2_m256(salt256, repet_idx << VOLE_RNG_REPET_SHIFT);
  rijndael256_ctrle_nocarry_nblocks_avx2(out, extseed, &ctr, out_bytes >> 5);
}

EXPORT void proofow_init_cipher_cat1_avx(                             //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
) {
  static const uint64_t H_PIOP_BYTES = 32;
  struct proofow_state128_t* const s = (struct proofow_state128_t*)proofow_state;
  memcpy(s->h_piop, h_piop, H_PIOP_BYTES);
  s->delta0_out_bytes = (tau * kappa + 7) >> 3;
  CASSERT(proofow_w > 0 && proofow_w < 64, "invalid proofow_w");
  // each iteration runs the block cipher twice, so 2^(w-1) iterations already cost 2^w
  // cipher calls: the check must cover w-1 bits, not w.
  s->mask_w = (UINT64_C(1) << (proofow_w - 1)) - 1;  // 2^(w-1) - 1
  xof_ctx xof;
  xof_init_and_seed_shake128(&xof, &PROOFOW_H0_PREFIX, 1);
  xof_seed_shake128(&xof, h_piop, H_PIOP_BYTES);
  xof_finalize_shake128(&xof);
  xof_output_shake128(&xof, s->p, 64);  // covers p0,p1,k0,k1
  s->p[0].v64[1] |= UINT64_C(1) << 63;  // ensures the msb of p0 is 1
  s->p[1].v64[1] |= UINT64_C(1) << 63;  // ensures the msb of p1 is 1
  s->k[0].v64[0] &= UINT64_C(-2);       // ensures the lsb of k0 is 0
  s->k[1].v64[0] |= UINT64_C(1);        // ensures the lsb of k1 is 1
  aes128_key_schedule_x2_avx2(s->rk[0], s->rk[1], s->k[0].v64, s->k[1].v64);
}

EXPORT int proofow_grind_w_cipher_cat1_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out) {
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 32; // CAT1
  struct proofow_state128_t* const s = (struct proofow_state128_t*)proofow_state;
  uint64_t ctr = *ctr_in_out;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  // low-level 2-key grind: fills s->c and ctr with the first winning candidate.
  if (!aes128_proofow_grind_avx2(s->c, &ctr, s->p, s->rk, s->mask_w)) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake128(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake128(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake128(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake128(&xof, s->c, 32); // covers c0, c1
  xof_finalize_and_output_shake128(&xof, delta0_out, s->delta0_out_bytes);
  *ctr_in_out = ctr;
  return 1;
}

EXPORT int proofow_verify_w_cipher_cat1_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 32;  // CAT1
  struct proofow_state128_t* const s = (struct proofow_state128_t*)proofow_state;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  aes128_ctrle_nocarry_1block_avx2(s->c[0].v64, s->rk[0], s->p[0].v64);
  aes128_ctrle_nocarry_1block_avx2(s->c[1].v64, s->rk[1], s->p[1].v64);
  if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) != 0) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake128(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake128(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake128(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake128(&xof, s->c, 32);                          // covers c0, c1
  xof_finalize_and_output_shake128(&xof, delta0_out, s->delta0_out_bytes);
  return 1;
}

// cipher-based proof of work (cat5: rijndael256 block cipher + shake256 xof), avx2 variant.
// Mirrors the cat1 avx trio: the grind delegates its inner loop to the low-level
// rijndael256_proofow_grind_avx2 (the rijndael256 analogue of aes128_proofow_grind_avx2).

EXPORT void proofow_init_cipher_cat5_avx(                             //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
) {
  static const uint64_t H_PIOP_BYTES = 64;  // CAT5: 2*lambda = 512 bits
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  memcpy(s->h_piop, h_piop, H_PIOP_BYTES);
  s->delta0_out_bytes = (tau * kappa + 7) >> 3;
  CASSERT(proofow_w > 0 && proofow_w < 64, "invalid proofow_w");
  // each iteration runs the block cipher twice, so 2^(w-1) iterations already cost 2^w
  // cipher calls: the check must cover w-1 bits, not w.
  s->mask_w = (UINT64_C(1) << (proofow_w - 1)) - 1;  // 2^(w-1) - 1
  xof_ctx xof;
  xof_init_and_seed_shake256(&xof, &PROOFOW_H0_PREFIX, 1);
  xof_seed_shake256(&xof, h_piop, H_PIOP_BYTES);
  xof_finalize_shake256(&xof);
  xof_output_shake256(&xof, s->p, 128);  // covers p0,p1,k0,k1 (32 bytes each)
  s->p[0].v64[3] |= UINT64_C(1) << 63;   // ensures the msb of p0 is 1
  s->p[1].v64[3] |= UINT64_C(1) << 63;   // ensures the msb of p1 is 1
  s->k[0].v64[0] &= UINT64_C(-2);        // ensures the lsb of k0 is 0
  s->k[1].v64[0] |= UINT64_C(1);         // ensures the lsb of k1 is 1
  rijndael256_key_schedule_x2_avx2(s->rk[0], s->rk[1], s->k[0].v64, s->k[1].v64);
}

EXPORT int proofow_grind_w_cipher_cat5_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out) {
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 64;  // CAT5
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  uint64_t ctr = *ctr_in_out;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  // low-level 2-key grind: fills s->c and ctr with the first winning candidate.
  if (!rijndael256_proofow_grind_avx2(s->c, &ctr, s->p, s->rk, s->mask_w)) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake256(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake256(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake256(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake256(&xof, s->c, 64);                          // covers c0, c1 (32 bytes each)
  xof_finalize_and_output_shake256(&xof, delta0_out, s->delta0_out_bytes);
  *ctr_in_out = ctr;
  return 1;
}

EXPORT int proofow_verify_w_cipher_cat5_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 64;  // CAT5
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  rijndael256_ctrle_nocarry_1block_avx2(s->c[0].v64, s->rk[0], s->p[0].v64);
  rijndael256_ctrle_nocarry_1block_avx2(s->c[1].v64, s->rk[1], s->p[1].v64);
  if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) != 0) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake256(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake256(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake256(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake256(&xof, s->c, 64);                          // covers c0, c1 (32 bytes each)
  xof_finalize_and_output_shake256(&xof, delta0_out, s->delta0_out_bytes);
  return 1;
}

// cipher-based proof of work (cat3: rijndael256 + shake256), avx2 variant. Identical to
// cat5_avx except h_piop is 48 bytes; reuses the same proofow_state256_t workspace and the
// same low-level rijndael256_proofow_grind_avx2.

EXPORT void proofow_init_cipher_cat3_avx(                             //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
) {
  static const uint64_t H_PIOP_BYTES = 48;  // CAT3: 2*lambda = 384 bits
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  memcpy(s->h_piop, h_piop, H_PIOP_BYTES);
  s->delta0_out_bytes = (tau * kappa + 7) >> 3;
  CASSERT(proofow_w > 0 && proofow_w < 64, "invalid proofow_w");
  // each iteration runs the block cipher twice, so 2^(w-1) iterations already cost 2^w
  // cipher calls: the check must cover w-1 bits, not w.
  s->mask_w = (UINT64_C(1) << (proofow_w - 1)) - 1;  // 2^(w-1) - 1
  xof_ctx xof;
  xof_init_and_seed_shake256(&xof, &PROOFOW_H0_PREFIX, 1);
  xof_seed_shake256(&xof, h_piop, H_PIOP_BYTES);
  xof_finalize_shake256(&xof);
  xof_output_shake256(&xof, s->p, 128);  // covers p0,p1,k0,k1 (32 bytes each)
  s->p[0].v64[3] |= UINT64_C(1) << 63;   // ensures the msb of p0 is 1
  s->p[1].v64[3] |= UINT64_C(1) << 63;   // ensures the msb of p1 is 1
  s->k[0].v64[0] &= UINT64_C(-2);        // ensures the lsb of k0 is 0
  s->k[1].v64[0] |= UINT64_C(1);         // ensures the lsb of k1 is 1
  rijndael256_key_schedule_x2_avx2(s->rk[0], s->rk[1], s->k[0].v64, s->k[1].v64);
}

EXPORT int proofow_grind_w_cipher_cat3_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out) {
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 48;  // CAT3
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  uint64_t ctr = *ctr_in_out;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  // low-level 2-key grind: fills s->c and ctr with the first winning candidate.
  if (!rijndael256_proofow_grind_avx2(s->c, &ctr, s->p, s->rk, s->mask_w)) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake256(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake256(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake256(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake256(&xof, s->c, 64);                          // covers c0, c1 (32 bytes each)
  xof_finalize_and_output_shake256(&xof, delta0_out, s->delta0_out_bytes);
  *ctr_in_out = ctr;
  return 1;
}

EXPORT int proofow_verify_w_cipher_cat3_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 48;  // CAT3
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  rijndael256_ctrle_nocarry_1block_avx2(s->c[0].v64, s->rk[0], s->p[0].v64);
  rijndael256_ctrle_nocarry_1block_avx2(s->c[1].v64, s->rk[1], s->p[1].v64);
  if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) != 0) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake256(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake256(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake256(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake256(&xof, s->c, 64);                          // covers c0, c1 (32 bytes each)
  xof_finalize_and_output_shake256(&xof, delta0_out, s->delta0_out_bytes);
  return 1;
}
// --- matrix_rng (avx2) ---------------------------------------------------------
// avx2 counterpart of the matrix_rng_* ref functions. The struct layout, the
// blocks_per_row computation and the last-block mask (set_2_b_minus_one) are all
// shared with the ref path; only the key schedule / CTR encryption and the
// last-block masking use the avx2 kernels. The avx2 key schedule is bit-identical
// to the ref one, so a context built by either init works with either get_row.

// AND the last PRG block with the stored mask using vector intrinsics.
static inline void apply_last_blk_mask_m128(void* blk, const void* mask) {
  const __m128i b = _mm_loadu_si128((const __m128i*)blk);
  const __m128i m = _mm_loadu_si128((const __m128i*)mask);
  _mm_storeu_si128((__m128i*)blk, _mm_and_si128(b, m));
}
static inline void apply_last_blk_mask_m256(void* blk, const void* mask) {
  const __m256i b = _mm256_loadu_si256((const __m256i*)blk);
  const __m256i m = _mm256_loadu_si256((const __m256i*)mask);
  _mm256_storeu_si256((__m256i*)blk, _mm256_and_si256(b, m));
}

EXPORT void matrix_rng_init_aes128_cat1_avx(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size) {
  struct matrix_rng_aes128_cat1_t* r = (struct matrix_rng_aes128_cat1_t*)rng;
  aes128_key_schedule_x1_avx2(r->rk, seed);
  r->blocks_per_row = ((row_bit_size + 127) >> 7);  // aes128 uses 128-bit block sizes
  r->row_bit_size = row_bit_size;
  set_2_b_minus_one(r->last_blk_mask.v64, 2, row_bit_size & 127);
}

EXPORT void matrix_rng_init_rijndael256_cat3_avx(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size) {
  struct matrix_rng_rijndael256_cat3_t* r = (struct matrix_rng_rijndael256_cat3_t*)rng;
  // the actual rijndael key is padded with zeroes
  uint64_t key256[4] = {};  // ZERO
  memcpy(key256, seed, 24);
  rijndael256_key_schedule_x1_avx2(r->rk, key256);
  // cat3 use the full 256-bit outputs of rijndael256
  r->blocks_per_row = ((row_bit_size + 255) >> 8);
  r->row_bit_size = row_bit_size;
  set_2_b_minus_one(r->last_blk_mask.v64, 4, row_bit_size & 255);
}

EXPORT void matrix_rng_init_rijndael256_cat5_avx(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size) {
  struct matrix_rng_rijndael256_cat5_t* r = (struct matrix_rng_rijndael256_cat5_t*)rng;
  rijndael256_key_schedule_x1_avx2(r->rk, seed);
  r->blocks_per_row = ((row_bit_size + 255) >> 8);  // rijndael256 uses 256-bit block sizes
  r->row_bit_size = row_bit_size;
  set_2_b_minus_one(r->last_blk_mask.v64, 4, row_bit_size & 255);
}

EXPORT void matrix_rng_get_row_aes128_cat1_avx(const matrix_rng_t* rng, void* out, const uint64_t row_index) {
  struct matrix_rng_aes128_cat1_t* r = (struct matrix_rng_aes128_cat1_t*)rng;
  CASSERT((((uint64_t)out) & 15) == 0, "out must be 16-byte aligned");
  ctr128_t* const cout = (ctr128_t*)out;
  const uint64_t bpr = r->blocks_per_row;
  ctr128_t ctr = {};
  ctr.v64[0] = row_index * bpr;
  aes128_ctrle_nocarry_nblocks_avx2(out, r->rk, ctr.v64, bpr);
  apply_last_blk_mask_m128(cout[bpr - 1].v8, r->last_blk_mask.v8);
}

EXPORT void matrix_rng_get_row_rijndael256_cat3_avx(const matrix_rng_t* rng, void* out, const uint64_t row_index) {
  struct matrix_rng_rijndael256_cat3_t* r = (struct matrix_rng_rijndael256_cat3_t*)rng;
  // cat3 and cat5 produce identical outputs
  CASSERT((((uint64_t)out) & 31) == 0, "out must be 32-byte aligned");
  ctr256_t* const cout = (ctr256_t*)out;
  const uint64_t bpr = r->blocks_per_row;
  ctr256_t ctr = {};
  ctr.v64[0] = row_index * bpr;
  rijndael256_ctrle_nocarry_nblocks_avx2(out, r->rk, ctr.v64, bpr);
  apply_last_blk_mask_m256(cout[bpr - 1].v8, r->last_blk_mask.v8);
}

EXPORT void matrix_rng_get_row_rijndael256_cat5_avx(const matrix_rng_t* rng, void* out, const uint64_t row_index) {
  struct matrix_rng_rijndael256_cat5_t* r = (struct matrix_rng_rijndael256_cat5_t*)rng;
  CASSERT((((uint64_t)out) & 31) == 0, "out must be 32-byte aligned");
  ctr256_t* const cout = (ctr256_t*)out;
  const uint64_t bpr = r->blocks_per_row;
  ctr256_t ctr = {};
  ctr.v64[0] = row_index * bpr;
  rijndael256_ctrle_nocarry_nblocks_avx2(out, r->rk, ctr.v64, bpr);
  apply_last_blk_mask_m256(cout[bpr - 1].v8, r->last_blk_mask.v8);
}

// Batched row generation (see MATRIX_RNG_GET_ROWS). Consecutive rows are a
// contiguous stretch of the CTR keystream, so the batch is one cipher call
// followed by the per-row last-block masking. Byte-identical to a get_row loop.

EXPORT void matrix_rng_get_rows_aes128_cat1_avx(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                const uint64_t nrows) {
  struct matrix_rng_aes128_cat1_t* r = (struct matrix_rng_aes128_cat1_t*)rng;
  CASSERT((((uint64_t)out) & 15) == 0, "out must be 16-byte aligned");
  ctr128_t* const cout = (ctr128_t*)out;
  const uint64_t bpr = r->blocks_per_row;
  ctr128_t ctr = {};
  ctr.v64[0] = first_row * bpr;
  aes128_ctrle_nocarry_nblocks_avx2(out, r->rk, ctr.v64, nrows * bpr);
  for (uint64_t i = 0; i < nrows; ++i) {
    apply_last_blk_mask_m128(cout[i * bpr + bpr - 1].v8, r->last_blk_mask.v8);
  }
}

EXPORT void matrix_rng_get_rows_rijndael256_cat3_avx(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                     const uint64_t nrows) {
  struct matrix_rng_rijndael256_cat3_t* r = (struct matrix_rng_rijndael256_cat3_t*)rng;
  // cat3 and cat5 produce identical outputs
  CASSERT((((uint64_t)out) & 31) == 0, "out must be 32-byte aligned");
  ctr256_t* const cout = (ctr256_t*)out;
  const uint64_t bpr = r->blocks_per_row;
  ctr256_t ctr = {};
  ctr.v64[0] = first_row * bpr;
  rijndael256_ctrle_nocarry_nblocks_avx2(out, r->rk, ctr.v64, nrows * bpr);
  for (uint64_t i = 0; i < nrows; ++i) {
    apply_last_blk_mask_m256(cout[i * bpr + bpr - 1].v8, r->last_blk_mask.v8);
  }
}

// --- fused row generation x challenge (cat1: aes128 + gf128, avx2) ----------
//
// See MATRIX_RNG_ROWS_TIMES_CHALL. This is the one place where the matrix prng
// and the field meet: the aes rounds issue on the cipher port and the carry-less
// multiplies on the vector-shuffle port, so emitting them in a single loop lets
// the field reduction of one row run underneath the encryption of the next. Kept
// here rather than in a new file because everything it needs is already in
// scope: the prng context layout (sdith_prng_private.h) and GF128_PV
// (vole_private.h).
//
// Row generation is bit-identical to matrix_rng_get_row_aes128_cat1_avx (same
// counters, same last-block mask) and the reduction is the rearrangement
// documented on MATRIX_RNG_PREPROCESS_CHALL, so the result is identical to the
// get_rows + gf128_flambda_dot_product pair.

// clmul operand selectors (same encoding as the ones in the gf*_avx2 sources)
#define PCLMUL_LO_X_LO 0x00
#define PCLMUL_LO_X_HI 0x10

/** the bpr aes128 blocks of one row, last block masked. Kept as values so that,
 *  when bpr is a compile-time constant, the row never reaches memory. */
#define AES128_ROW_GEN(out, rk, ctr, bpr, lastmask)                                              \
  do {                                                                                           \
    const __m128i k0_ = _mm_loadu_si128((const __m128i*)(rk));                                   \
    uint64_t t_ = 0;                                                                             \
    for (; t_ + 4 <= (bpr); t_ += 4) {                                                           \
      __m128i s0_ = _mm_xor_si128(_mm_set_epi64x(0, (long long)((ctr) + t_ + 0)), k0_);          \
      __m128i s1_ = _mm_xor_si128(_mm_set_epi64x(0, (long long)((ctr) + t_ + 1)), k0_);          \
      __m128i s2_ = _mm_xor_si128(_mm_set_epi64x(0, (long long)((ctr) + t_ + 2)), k0_);          \
      __m128i s3_ = _mm_xor_si128(_mm_set_epi64x(0, (long long)((ctr) + t_ + 3)), k0_);          \
      for (int o_ = 16; o_ <= 144; o_ += 16) {                                                   \
        const __m128i kk_ = _mm_loadu_si128((const __m128i*)((const uint8_t*)(rk) + o_));        \
        s0_ = _mm_aesenc_si128(s0_, kk_);                                                        \
        s1_ = _mm_aesenc_si128(s1_, kk_);                                                        \
        s2_ = _mm_aesenc_si128(s2_, kk_);                                                        \
        s3_ = _mm_aesenc_si128(s3_, kk_);                                                        \
      }                                                                                          \
      const __m128i kl_ = _mm_loadu_si128((const __m128i*)((const uint8_t*)(rk) + 160));         \
      (out)[t_ + 0] = _mm_aesenclast_si128(s0_, kl_);                                            \
      (out)[t_ + 1] = _mm_aesenclast_si128(s1_, kl_);                                            \
      (out)[t_ + 2] = _mm_aesenclast_si128(s2_, kl_);                                            \
      (out)[t_ + 3] = _mm_aesenclast_si128(s3_, kl_);                                            \
    }                                                                                            \
    for (; t_ < (bpr); t_++) {                                                                   \
      __m128i s_ = _mm_xor_si128(_mm_set_epi64x(0, (long long)((ctr) + t_)), k0_);               \
      for (int o_ = 16; o_ <= 144; o_ += 16)                                                     \
        s_ = _mm_aesenc_si128(s_, _mm_loadu_si128((const __m128i*)((const uint8_t*)(rk) + o_))); \
      (out)[t_] = _mm_aesenclast_si128(s_, _mm_loadu_si128((const __m128i*)((const uint8_t*)(rk) + 160)));  \
    }                                                                                            \
    (out)[(bpr) - 1] = _mm_and_si128((out)[(bpr) - 1], (lastmask));                              \
  } while (0)

/** sum_j h_j * prep_chall[j], h_j the 64-bit words of the row. The high part of
 *  the accumulator is 64 bits wide, so X^128 == GF128_PV folds in one clmul. */
#define GF128_ROW_REDUCE(dst, rowwords, pc, nw)                                        \
  do {                                                                                 \
    const __m128i pv_ = _mm_set_epi64x(0, GF128_PV);                                   \
    __m128i a0_ = _mm_setzero_si128(), a1_ = _mm_setzero_si128();                      \
    for (uint64_t j_ = 0; j_ < (nw); ++j_) {                                           \
      const __m128i h_ = _mm_loadl_epi64((const __m128i*)((const uint64_t*)(rowwords) + j_)); \
      const __m128i c_ = _mm_loadu_si128((const __m128i*)(pc) + j_);                   \
      a0_ = _mm_xor_si128(a0_, _mm_clmulepi64_si128(h_, c_, PCLMUL_LO_X_LO));          \
      a1_ = _mm_xor_si128(a1_, _mm_clmulepi64_si128(h_, c_, PCLMUL_LO_X_HI));          \
    }                                                                                  \
    __m128i u_ = _mm_xor_si128(a0_, _mm_slli_si128(a1_, 8));                           \
    const __m128i hi_ = _mm_srli_si128(a1_, 8);                                        \
    (dst) = _mm_xor_si128(u_, _mm_clmulepi64_si128(hi_, pv_, PCLMUL_LO_X_LO));         \
  } while (0)

/** the fused loop, software-pipelined: row i is generated while row i-1 reduces. */
#define AES128_GF128_ROWS_TIMES_CHALL(dest, rk, bpr, lastmask, pc, nw, first_row, nrows, NBLK) \
  do {                                                                                         \
    __m128i row_[2][NBLK];                                                                     \
    AES128_ROW_GEN(row_[0], rk, (first_row) * (bpr), bpr, lastmask);                           \
    for (uint64_t i_ = 1; i_ < (nrows); ++i_) {                                                \
      AES128_ROW_GEN(row_[i_ & 1], rk, ((first_row) + i_) * (bpr), bpr, lastmask);             \
      GF128_ROW_REDUCE((dest)[i_ - 1], row_[(i_ - 1) & 1], pc, nw);                            \
    }                                                                                          \
    GF128_ROW_REDUCE((dest)[(nrows) - 1], row_[((nrows) - 1) & 1], pc, nw);                    \
  } while (0)

// Specialisations. nwords fixes both the reduction length and bpr = ceil(nwords/2),
// so one constant drives the whole kernel; with it fixed the two row buffers stay
// in registers instead of spilling, which is worth as much as the reduction
// rewrite itself. NWORDS 1..16 covers row_bit_size up to 1024.
#define DEFINE_ROWS_TIMES_CHALL_CAT1(NW)                                                                  \
  static void rows_times_chall_cat1_nw##NW(const struct matrix_rng_aes128_cat1_t* r, __m128i* dest,        \
                                           const void* pc, uint64_t first_row, uint64_t nrows) {           \
    const __m128i lm = _mm_loadu_si128((const __m128i*)r->last_blk_mask.v8);                               \
    AES128_GF128_ROWS_TIMES_CHALL(dest, r->rk, ((NW) + 1) / 2, lm, pc, NW, first_row, nrows, ((NW) + 1) / 2); \
  }
DEFINE_ROWS_TIMES_CHALL_CAT1(1)
DEFINE_ROWS_TIMES_CHALL_CAT1(2)
DEFINE_ROWS_TIMES_CHALL_CAT1(3)
DEFINE_ROWS_TIMES_CHALL_CAT1(4)
DEFINE_ROWS_TIMES_CHALL_CAT1(5)
DEFINE_ROWS_TIMES_CHALL_CAT1(6)
DEFINE_ROWS_TIMES_CHALL_CAT1(7)
DEFINE_ROWS_TIMES_CHALL_CAT1(8)
DEFINE_ROWS_TIMES_CHALL_CAT1(9)
DEFINE_ROWS_TIMES_CHALL_CAT1(10)
DEFINE_ROWS_TIMES_CHALL_CAT1(11)
DEFINE_ROWS_TIMES_CHALL_CAT1(12)
DEFINE_ROWS_TIMES_CHALL_CAT1(13)
DEFINE_ROWS_TIMES_CHALL_CAT1(14)
DEFINE_ROWS_TIMES_CHALL_CAT1(15)
DEFINE_ROWS_TIMES_CHALL_CAT1(16)
#define ROWS_TIMES_CHALL_CAT1_MAX_NWORDS 16

typedef void ROWS_TIMES_CHALL_CAT1_FN(const struct matrix_rng_aes128_cat1_t*, __m128i*, const void*, uint64_t,
                                      uint64_t);
static ROWS_TIMES_CHALL_CAT1_FN* const ROWS_TIMES_CHALL_CAT1[ROWS_TIMES_CHALL_CAT1_MAX_NWORDS + 1] = {
    0,
    rows_times_chall_cat1_nw1,  rows_times_chall_cat1_nw2,  rows_times_chall_cat1_nw3,
    rows_times_chall_cat1_nw4,  rows_times_chall_cat1_nw5,  rows_times_chall_cat1_nw6,
    rows_times_chall_cat1_nw7,  rows_times_chall_cat1_nw8,  rows_times_chall_cat1_nw9,
    rows_times_chall_cat1_nw10, rows_times_chall_cat1_nw11, rows_times_chall_cat1_nw12,
    rows_times_chall_cat1_nw13, rows_times_chall_cat1_nw14, rows_times_chall_cat1_nw15,
    rows_times_chall_cat1_nw16,
};

/** generic fallback for row sizes beyond the specialised set (rows go via memory).
 *  The two row buffers are bounded rather than sized at runtime: 128 blocks is a
 *  16384-bit row, far above any rsd_codim, and keeps the stack use fixed at 4kB. */
#define ROWS_TIMES_CHALL_CAT1_MAX_BLOCKS 128
static void rows_times_chall_cat1_generic(const struct matrix_rng_aes128_cat1_t* r, __m128i* dest, const void* pc,
                                          uint64_t nwords, uint64_t first_row, uint64_t nrows) {
  const __m128i lm = _mm_loadu_si128((const __m128i*)r->last_blk_mask.v8);
  const uint64_t bpr = r->blocks_per_row;
  CREQUIRE(bpr <= ROWS_TIMES_CHALL_CAT1_MAX_BLOCKS, "row too wide (%ld blocks)", (long)bpr);
  __m128i row[2 * ROWS_TIMES_CHALL_CAT1_MAX_BLOCKS];
  AES128_ROW_GEN(row, r->rk, first_row * bpr, bpr, lm);
  for (uint64_t i = 1; i < nrows; ++i) {
    AES128_ROW_GEN(row + (i & 1) * bpr, r->rk, (first_row + i) * bpr, bpr, lm);
    GF128_ROW_REDUCE(dest[i - 1], row + ((i - 1) & 1) * bpr, pc, nwords);
  }
  GF128_ROW_REDUCE(dest[nrows - 1], row + ((nrows - 1) & 1) * bpr, pc, nwords);
}

EXPORT void matrix_rng_rows_times_chall_aes128_cat1_avx(const matrix_rng_t* rng, void* dest, const void* prep_chall,
                                                        const uint64_t first_row, const uint64_t nrows) {
  const struct matrix_rng_aes128_cat1_t* r = (const struct matrix_rng_aes128_cat1_t*)rng;
  if (nrows == 0) return;
  const uint64_t nwords = matrix_rng_prep_chall_nelems(r->row_bit_size);
  if (nwords <= ROWS_TIMES_CHALL_CAT1_MAX_NWORDS) {
    ROWS_TIMES_CHALL_CAT1[nwords](r, (__m128i*)dest, prep_chall, first_row, nrows);
  } else {
    rows_times_chall_cat1_generic(r, (__m128i*)dest, prep_chall, nwords, first_row, nrows);
  }
}

// --- fused row generation x challenge (cat3/cat5: rijndael256 + gf192/gf256) --
//
// Same contract as the cat1 kernel above, but the port profile is different and
// so is the shape. Rijndael-256's ShiftRows crosses the two 128-bit halves, so
// each round needs two pshufb per block on top of the two aesenc -- and pshufb
// issues on the same port as the carry-less multiplier. The cipher therefore
// already occupies the port the reduction wants, and there is no free overlap to
// win: software-pipelining the rows measured no better than reducing each row
// straight after generating it (unlike cat1, where it is worth 20%). So these
// keep a single row buffer, which halves the live vector registers and lets the
// wider specialisations stay in registers.
//
// The gain here comes from specialisation: the row is held in registers instead
// of going out to memory and back through two calls.

enum { RIJ256_ROUNDS = 14 };

/** one row as 2*bpr halves: row[2t] = low half of block t, row[2t+1] = high half.
 *  Rounds mirror sdith_rij_encN in lib/aes (bit-identical output); the counter
 *  block's high half is zero, so round 0 leaves it at the round key. */
#define RIJ256_ROW_GEN(row, rk, ctr, bpr, lmlo, lmhi)                                                     \
  do {                                                                                                    \
    const __m128i RMASK_ = _mm_set_epi32(0x03020d0c, 0x0f0e0908, 0x0b0a0504, 0x07060100);                 \
    const __m128i BMASK_ = _mm_set_epi32(0x80000000, 0x80800000, 0x80800000, 0x80808000);                 \
    const uint8_t* const rkb_ = (const uint8_t*)(rk);                                                     \
    const __m128i kk0_ = _mm_loadu_si128((const __m128i*)rkb_);                                           \
    const __m128i kk1_ = _mm_loadu_si128((const __m128i*)(rkb_ + 16));                                    \
    for (uint64_t t_ = 0; t_ < (bpr); ++t_) {                                                             \
      (row)[2 * t_] = _mm_xor_si128(_mm_set_epi64x(0, (long long)((ctr) + t_)), kk0_);                    \
      (row)[2 * t_ + 1] = kk1_;                                                                           \
    }                                                                                                     \
    for (int r_ = 1; r_ < RIJ256_ROUNDS; ++r_) {                                                          \
      const __m128i rk0_ = _mm_loadu_si128((const __m128i*)(rkb_ + 32 * r_));                             \
      const __m128i rk1_ = _mm_loadu_si128((const __m128i*)(rkb_ + 32 * r_ + 16));                        \
      for (uint64_t t_ = 0; t_ < (bpr); ++t_) {                                                           \
        const __m128i l_ = (row)[2 * t_], h_ = (row)[2 * t_ + 1];                                         \
        (row)[2 * t_] =                                                                                   \
            _mm_aesenc_si128(_mm_shuffle_epi8(_mm_blendv_epi8(l_, h_, BMASK_), RMASK_), rk0_);            \
        (row)[2 * t_ + 1] =                                                                               \
            _mm_aesenc_si128(_mm_shuffle_epi8(_mm_blendv_epi8(h_, l_, BMASK_), RMASK_), rk1_);            \
      }                                                                                                   \
    }                                                                                                     \
    {                                                                                                     \
      const __m128i rk0_ = _mm_loadu_si128((const __m128i*)(rkb_ + 32 * RIJ256_ROUNDS));                  \
      const __m128i rk1_ = _mm_loadu_si128((const __m128i*)(rkb_ + 32 * RIJ256_ROUNDS + 16));             \
      for (uint64_t t_ = 0; t_ < (bpr); ++t_) {                                                           \
        const __m128i l_ = (row)[2 * t_], h_ = (row)[2 * t_ + 1];                                         \
        (row)[2 * t_] =                                                                                   \
            _mm_aesenclast_si128(_mm_shuffle_epi8(_mm_blendv_epi8(l_, h_, BMASK_), RMASK_), rk0_);        \
        (row)[2 * t_ + 1] =                                                                               \
            _mm_aesenclast_si128(_mm_shuffle_epi8(_mm_blendv_epi8(h_, l_, BMASK_), RMASK_), rk1_);        \
      }                                                                                                   \
    }                                                                                                     \
    (row)[2 * (bpr) - 2] = _mm_and_si128((row)[2 * (bpr) - 2], (lmlo));                                   \
    (row)[2 * (bpr) - 1] = _mm_and_si128((row)[2 * (bpr) - 1], (lmhi));                                   \
  } while (0)

// Word j of the row is qword (j & 1) of register row[j >> 1]. Rather than
// extracting it (a shuffle, on the contended port), the words are consumed two at
// a time from one register and the qword is picked by the clmul selector, which
// costs nothing. ASEL is 0 for an even word, 1 for an odd one.
#define GF192_ACC(w, pcj, ASEL)                                                     \
  do {                                                                              \
    const __m128i e01_ = _mm_loadu_si128((const __m128i*)(pcj));                    \
    const __m128i e2_ = _mm_loadl_epi64((const __m128i*)((const uint8_t*)(pcj) + 16)); \
    a0_ = _mm_xor_si128(a0_, _mm_clmulepi64_si128((w), e01_, (ASEL) | 0x00));       \
    a1_ = _mm_xor_si128(a1_, _mm_clmulepi64_si128((w), e01_, (ASEL) | 0x10));       \
    a2_ = _mm_xor_si128(a2_, _mm_clmulepi64_si128((w), e2_, (ASEL) | 0x00));        \
  } while (0)

/** sum_j h_j * prep_chall[j] in gf192. The accumulated value spans 256 bits, so
 *  its 64-bit tail folds through X^192 == GF192_PV in a single clmul. */
#define GF192_ROW_REDUCE(dst, row, pc, nw)                                          \
  do {                                                                              \
    const __m128i pv_ = _mm_set_epi64x(0, GF192_PV);                                \
    const uint8_t* const pcb_ = (const uint8_t*)(pc);                               \
    __m128i a0_ = _mm_setzero_si128(), a1_ = _mm_setzero_si128(), a2_ = _mm_setzero_si128(); \
    uint64_t j_ = 0;                                                                \
    for (; j_ + 2 <= (nw); j_ += 2) {                                               \
      const __m128i w_ = (row)[j_ >> 1];                                            \
      GF192_ACC(w_, pcb_ + 24 * j_, 0x00);                                          \
      GF192_ACC(w_, pcb_ + 24 * (j_ + 1), 0x01);                                    \
    }                                                                               \
    if (j_ < (nw)) GF192_ACC((row)[j_ >> 1], pcb_ + 24 * j_, 0x00);                 \
    const __m128i u01_ = _mm_xor_si128(a0_, _mm_slli_si128(a1_, 8));                \
    const __m128i u23_ = _mm_xor_si128(a2_, _mm_srli_si128(a1_, 8));                \
    _mm_storeu_si128((__m128i*)(dst), _mm_xor_si128(u01_, _mm_clmulepi64_si128(u23_, pv_, 0x01))); \
    ((uint64_t*)(dst))[2] = (uint64_t)_mm_cvtsi128_si64(u23_);                      \
  } while (0)

#define GF256_ACC(w, pcj, ASEL)                                                     \
  do {                                                                              \
    const __m128i e01_ = _mm_loadu_si128((const __m128i*)(pcj));                    \
    const __m128i e23_ = _mm_loadu_si128((const __m128i*)((const uint8_t*)(pcj) + 16)); \
    a0_ = _mm_xor_si128(a0_, _mm_clmulepi64_si128((w), e01_, (ASEL) | 0x00));       \
    a1_ = _mm_xor_si128(a1_, _mm_clmulepi64_si128((w), e01_, (ASEL) | 0x10));       \
    a2_ = _mm_xor_si128(a2_, _mm_clmulepi64_si128((w), e23_, (ASEL) | 0x00));       \
    a3_ = _mm_xor_si128(a3_, _mm_clmulepi64_si128((w), e23_, (ASEL) | 0x10));       \
  } while (0)

/** sum_j h_j * prep_chall[j] in gf256; 320-bit accumulator, 64-bit tail folded
 *  through X^256 == GF256_PV in a single clmul. */
#define GF256_ROW_REDUCE(dst, row, pc, nw)                                          \
  do {                                                                              \
    const __m128i pv_ = _mm_set_epi64x(0, GF256_PV);                                \
    const uint8_t* const pcb_ = (const uint8_t*)(pc);                               \
    __m128i a0_ = _mm_setzero_si128(), a1_ = _mm_setzero_si128();                   \
    __m128i a2_ = _mm_setzero_si128(), a3_ = _mm_setzero_si128();                   \
    uint64_t j_ = 0;                                                                \
    for (; j_ + 2 <= (nw); j_ += 2) {                                               \
      const __m128i w_ = (row)[j_ >> 1];                                            \
      GF256_ACC(w_, pcb_ + 32 * j_, 0x00);                                          \
      GF256_ACC(w_, pcb_ + 32 * (j_ + 1), 0x01);                                    \
    }                                                                               \
    if (j_ < (nw)) GF256_ACC((row)[j_ >> 1], pcb_ + 32 * j_, 0x00);                 \
    const __m128i u01_ = _mm_xor_si128(a0_, _mm_slli_si128(a1_, 8));                \
    const __m128i u23_ =                                                            \
        _mm_xor_si128(_mm_xor_si128(a2_, _mm_srli_si128(a1_, 8)), _mm_slli_si128(a3_, 8)); \
    const __m128i w4_ = _mm_srli_si128(a3_, 8);                                     \
    _mm_storeu_si128((__m128i*)(dst), _mm_xor_si128(u01_, _mm_clmulepi64_si128(w4_, pv_, 0x00))); \
    _mm_storeu_si128((__m128i*)((uint8_t*)(dst) + 16), u23_);                       \
  } while (0)

// Specialisations, indexed by the row word count. As for cat1 one constant drives
// the kernel: bpr = ceil(nwords/4) for a 256-bit block. NW 1..16 covers
// row_bit_size up to 1024, i.e. every rsd_codim in use.
#define DEFINE_ROWS_TIMES_CHALL_RIJ(CAT, NW, CTXT, REDUCE, LB)                                     \
  static void rows_times_chall_##CAT##_nw##NW(const struct CTXT* r, uint8_t* dest, const void* pc, \
                                              uint64_t first_row, uint64_t nrows) {                \
    const __m128i lmlo = _mm_loadu_si128((const __m128i*)r->last_blk_mask.v8);                     \
    const __m128i lmhi = _mm_loadu_si128((const __m128i*)(r->last_blk_mask.v8 + 16));              \
    enum { BPR = ((NW) + 3) / 4 };                                                                 \
    __m128i row[2 * BPR];                                                                          \
    for (uint64_t i = 0; i < nrows; ++i) {                                                         \
      RIJ256_ROW_GEN(row, r->rk, (first_row + i) * BPR, BPR, lmlo, lmhi);                          \
      REDUCE(dest + i * (LB), row, pc, NW);                                                        \
    }                                                                                              \
  }

#define DEFINE_ROWS_TIMES_CHALL_CAT3(NW) \
  DEFINE_ROWS_TIMES_CHALL_RIJ(cat3, NW, matrix_rng_rijndael256_cat3_t, GF192_ROW_REDUCE, 24)
#define DEFINE_ROWS_TIMES_CHALL_CAT5(NW) \
  DEFINE_ROWS_TIMES_CHALL_RIJ(cat5, NW, matrix_rng_rijndael256_cat5_t, GF256_ROW_REDUCE, 32)

#define DEFINE_ROWS_TIMES_CHALL_ALL(M) \
  M(1) M(2) M(3) M(4) M(5) M(6) M(7) M(8) M(9) M(10) M(11) M(12) M(13) M(14) M(15) M(16)
DEFINE_ROWS_TIMES_CHALL_ALL(DEFINE_ROWS_TIMES_CHALL_CAT3)
DEFINE_ROWS_TIMES_CHALL_ALL(DEFINE_ROWS_TIMES_CHALL_CAT5)
#define ROWS_TIMES_CHALL_RIJ_MAX_NWORDS 16
#define ROWS_TIMES_CHALL_RIJ_TABLE(CAT)                                                              \
  {0,                                                                                                \
   rows_times_chall_##CAT##_nw1,  rows_times_chall_##CAT##_nw2,  rows_times_chall_##CAT##_nw3,        \
   rows_times_chall_##CAT##_nw4,  rows_times_chall_##CAT##_nw5,  rows_times_chall_##CAT##_nw6,        \
   rows_times_chall_##CAT##_nw7,  rows_times_chall_##CAT##_nw8,  rows_times_chall_##CAT##_nw9,        \
   rows_times_chall_##CAT##_nw10, rows_times_chall_##CAT##_nw11, rows_times_chall_##CAT##_nw12,       \
   rows_times_chall_##CAT##_nw13, rows_times_chall_##CAT##_nw14, rows_times_chall_##CAT##_nw15,       \
   rows_times_chall_##CAT##_nw16}

typedef void ROWS_TIMES_CHALL_CAT3_FN(const struct matrix_rng_rijndael256_cat3_t*, uint8_t*, const void*, uint64_t,
                                      uint64_t);
typedef void ROWS_TIMES_CHALL_CAT5_FN(const struct matrix_rng_rijndael256_cat5_t*, uint8_t*, const void*, uint64_t,
                                      uint64_t);
static ROWS_TIMES_CHALL_CAT3_FN* const ROWS_TIMES_CHALL_CAT3[ROWS_TIMES_CHALL_RIJ_MAX_NWORDS + 1] =
    ROWS_TIMES_CHALL_RIJ_TABLE(cat3);
static ROWS_TIMES_CHALL_CAT5_FN* const ROWS_TIMES_CHALL_CAT5[ROWS_TIMES_CHALL_RIJ_MAX_NWORDS + 1] =
    ROWS_TIMES_CHALL_RIJ_TABLE(cat5);

// Generic fallback for rows wider than the specialised set. Correctness path
// only: with a runtime block count the row spills and this is slower than the
// get_rows + flambda_dot_product pair it replaces, but no rsd_codim in use gets
// anywhere near it. Bounded to keep the stack use fixed (2kB).
#define ROWS_TIMES_CHALL_RIJ_MAX_BLOCKS 64
#define DEFINE_ROWS_TIMES_CHALL_RIJ_GENERIC(CAT, CTXT, REDUCE, LB)                                       \
  static void rows_times_chall_##CAT##_generic(const struct CTXT* r, uint8_t* dest, const void* pc,      \
                                               uint64_t nwords, uint64_t first_row, uint64_t nrows) {    \
    const __m128i lmlo = _mm_loadu_si128((const __m128i*)r->last_blk_mask.v8);                           \
    const __m128i lmhi = _mm_loadu_si128((const __m128i*)(r->last_blk_mask.v8 + 16));                    \
    const uint64_t bpr = r->blocks_per_row;                                                              \
    CREQUIRE(bpr <= ROWS_TIMES_CHALL_RIJ_MAX_BLOCKS, "row too wide (%ld blocks)", (long)bpr);            \
    __m128i row[2 * ROWS_TIMES_CHALL_RIJ_MAX_BLOCKS];                                                    \
    for (uint64_t i = 0; i < nrows; ++i) {                                                               \
      RIJ256_ROW_GEN(row, r->rk, (first_row + i) * bpr, bpr, lmlo, lmhi);                                \
      REDUCE(dest + i * (LB), row, pc, nwords);                                                          \
    }                                                                                                    \
  }
DEFINE_ROWS_TIMES_CHALL_RIJ_GENERIC(cat3, matrix_rng_rijndael256_cat3_t, GF192_ROW_REDUCE, 24)
DEFINE_ROWS_TIMES_CHALL_RIJ_GENERIC(cat5, matrix_rng_rijndael256_cat5_t, GF256_ROW_REDUCE, 32)

// Pre-processed challenge, avx path: same loop as the ref one (in
// sdith_prng_private.h), with the pclmul product instead of the bit-by-bit
// reference. It is only ceil(row_bits/64) products, but the ref product is a
// 192/256-iteration shift-and-xor loop, which is enough to show up next to the
// row reductions it feeds.
EXPORT void matrix_rng_preprocess_chall_aes128_cat1_avx(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                        const flambda_t* chall) {
  const struct matrix_rng_aes128_cat1_t* r = (const struct matrix_rng_aes128_cat1_t*)rng;
  preprocess_chall_generic(prep_chall, chall, matrix_rng_prep_chall_nelems(r->row_bit_size),  //
                           16, 2, gf128_flambda_product_pclmul);
}

EXPORT void matrix_rng_preprocess_chall_rijndael256_cat3_avx(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                             const flambda_t* chall) {
  const struct matrix_rng_rijndael256_cat3_t* r = (const struct matrix_rng_rijndael256_cat3_t*)rng;
  preprocess_chall_generic(prep_chall, chall, matrix_rng_prep_chall_nelems(r->row_bit_size),  //
                           24, 3, gf192_flambda_product_pclmul);
}

EXPORT void matrix_rng_preprocess_chall_rijndael256_cat5_avx(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                             const flambda_t* chall) {
  const struct matrix_rng_rijndael256_cat5_t* r = (const struct matrix_rng_rijndael256_cat5_t*)rng;
  preprocess_chall_generic(prep_chall, chall, matrix_rng_prep_chall_nelems(r->row_bit_size),  //
                           32, 4, gf256_flambda_product_pclmul);
}

EXPORT void matrix_rng_rows_times_chall_rijndael256_cat3_avx(const matrix_rng_t* rng, void* dest,
                                                             const void* prep_chall, const uint64_t first_row,
                                                             const uint64_t nrows) {
  const struct matrix_rng_rijndael256_cat3_t* r = (const struct matrix_rng_rijndael256_cat3_t*)rng;
  if (nrows == 0) return;
  const uint64_t nwords = matrix_rng_prep_chall_nelems(r->row_bit_size);
  if (nwords <= ROWS_TIMES_CHALL_RIJ_MAX_NWORDS) {
    ROWS_TIMES_CHALL_CAT3[nwords](r, (uint8_t*)dest, prep_chall, first_row, nrows);
  } else {
    rows_times_chall_cat3_generic(r, (uint8_t*)dest, prep_chall, nwords, first_row, nrows);
  }
}

EXPORT void matrix_rng_rows_times_chall_rijndael256_cat5_avx(const matrix_rng_t* rng, void* dest,
                                                             const void* prep_chall, const uint64_t first_row,
                                                             const uint64_t nrows) {
  const struct matrix_rng_rijndael256_cat5_t* r = (const struct matrix_rng_rijndael256_cat5_t*)rng;
  if (nrows == 0) return;
  const uint64_t nwords = matrix_rng_prep_chall_nelems(r->row_bit_size);
  if (nwords <= ROWS_TIMES_CHALL_RIJ_MAX_NWORDS) {
    ROWS_TIMES_CHALL_CAT5[nwords](r, (uint8_t*)dest, prep_chall, first_row, nrows);
  } else {
    rows_times_chall_cat5_generic(r, (uint8_t*)dest, prep_chall, nwords, first_row, nrows);
  }
}

EXPORT void matrix_rng_get_rows_rijndael256_cat5_avx(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                     const uint64_t nrows) {
  struct matrix_rng_rijndael256_cat5_t* r = (struct matrix_rng_rijndael256_cat5_t*)rng;
  CASSERT((((uint64_t)out) & 31) == 0, "out must be 32-byte aligned");
  ctr256_t* const cout = (ctr256_t*)out;
  const uint64_t bpr = r->blocks_per_row;
  ctr256_t ctr = {};
  ctr.v64[0] = first_row * bpr;
  rijndael256_ctrle_nocarry_nblocks_avx2(out, r->rk, ctr.v64, nrows * bpr);
  for (uint64_t i = 0; i < nrows; ++i) {
    apply_last_blk_mask_m256(cout[i * bpr + bpr - 1].v8, r->last_blk_mask.v8);
  }
}

// --- keygen prng (avx2) -----------------------------------------------------
// avx2 counterpart of the keygen_rng_* ref functions. The context layout and the
// draw loop are shared with the ref path (DEFINE_KEYGEN_RNG_NEXT_U32); only the
// key schedule and the CTR encryption use the avx2 kernels, which are
// bit-identical to the ref ones, so a context built by either init works with
// either draw function.

EXPORT void keygen_rng_init_aes128_cat1_avx(keygen_rng_ctx* rng, const seed_t* sk_seed) {
  struct keygen_rng_aes128_cat1_t* s = (struct keygen_rng_aes128_cat1_t*)rng;
  aes128_key_schedule_x1_avx2(s->rk, sk_seed);
  KEYGEN_RNG_INIT_EMPTY(s);
}

EXPORT void keygen_rng_init_rijndael256_cat3_avx(keygen_rng_ctx* rng, const seed_t* sk_seed) {
  struct keygen_rng_rijndael256_t* s = (struct keygen_rng_rijndael256_t*)rng;
  // the 192-bit seed is padded with 64 zero bits in the MSB to make a rijndael256 key
  uint64_t key256[4] = {};  // ZERO
  memcpy(key256, sk_seed, 24);
  rijndael256_key_schedule_x1_avx2(s->rk, key256);
  KEYGEN_RNG_INIT_EMPTY(s);
}

EXPORT void keygen_rng_init_rijndael256_cat5_avx(keygen_rng_ctx* rng, const seed_t* sk_seed) {
  struct keygen_rng_rijndael256_t* s = (struct keygen_rng_rijndael256_t*)rng;
  rijndael256_key_schedule_x1_avx2(s->rk, sk_seed);
  KEYGEN_RNG_INIT_EMPTY(s);
}

DEFINE_KEYGEN_RNG_NEXT_U32(keygen_rng_next_u32_aes128_cat1_avx, keygen_rng_aes128_cat1_t, ctr128_t,
                           aes128_ctrle_nocarry_nblocks_avx2)
DEFINE_KEYGEN_RNG_NEXT_U32(keygen_rng_next_u32_rijndael256_avx, keygen_rng_rijndael256_t, ctr256_t,
                           rijndael256_ctrle_nocarry_nblocks_avx2)
