#include "sdith_prng.h"

#include <stdlib.h>
#include <string.h>

#include "aes128_ctrle.h"
#include "KeccakHash.h"
#include "KeccakHashtimes4.h"
#include "rijndael256_ctrle.h"
#include "sdith_prng_private.h"
#include "vole_private.h"

#define STATIC_ASSERT(assert_name, condition) \
  uint8_t assert_name[(condition)?1:-1]

// ggm seed rng, extended-seed variants (ref): consume the expanded round keys via
// encrypt_nblocks instead of scheduling the key inside oneshot. Same domain sep as
// the raw variant (node_idx, not node_idx<<1), so byte-identical to the raw output.
EXPORT void ggm_seed_rng_lr_ext_cat1_aes128_ref(void* lr_out256, const void* salt128, const void* extkey128,
                                                uint64_t node_idx, uint32_t theta) {
  CASSERT((node_idx & 1) == 0, "bug! this function must be called on the left child index");
  aesblk ctr = {.v64 = {0, 0}};
  CASSERT(domain_sep_check(salt128, node_idx, 2, theta), "bug! invalid separation");
  domain_sep_ptx_lo2(ctr.v64, salt128, 16, node_idx);
  aes128_ctrle_nocarry_2block_ref(lr_out256, extkey128, ctr.v64);
}
EXPORT void ggm_seed_rng_lr_ext_cat3_rijndael256_ref(  //
    void* lr_out384, const void* salt192, const void* extkey, uint64_t node_idx, uint32_t theta) {
  CASSERT((node_idx & 1) == 0, "bug! this function must be called on the left child index");
  uint8_t out512[64] = {};
  ctr256_t ctr256 = {.v8 = {}};
  CASSERT(domain_sep_check(salt192, node_idx, 2, theta), "bug! invalid separation");
  domain_sep_ptx_lo2(ctr256.v64, salt192, 24, node_idx);
  rijndael256_ctrle_nocarry_2block_ref(out512, extkey, ctr256.v8);
  // cat3 Enc = LSB_192(Rijndael-256): keep the low 24 bytes of each block
  memcpy(lr_out384, out512, 24);
  memcpy((uint8_t*)lr_out384 + 24, out512 + 32, 24);
}
EXPORT void ggm_seed_rng_lr_ext_cat5_rijndael256_ref(  //
    void* lr_out512, const void* salt256, const void* extkey, uint64_t node_idx, uint32_t theta) {
  CASSERT((node_idx & 1) == 0, "bug! this function must be called on the left child index");
  ctr256_t ctr256 = {.v8 = {}};
  CASSERT(domain_sep_check(salt256, node_idx, 2, theta), "bug! invalid separation");
  domain_sep_ptx_lo2(ctr256.v64, salt256, 32, node_idx);
  rijndael256_ctrle_nocarry_2block_ref(lr_out512, extkey, ctr256.v8);
}

// I8: naive x4 wrappers (ref). LR_STRIDE = 2 * lambda_bytes, EXT_STRIDE = one extended key.
GGM_EXTSEED_RNG_LR_X4_NAIVE(ggm_seed_rng_lr_ext_cat1_aes128_x4_ref,       //
                            ggm_seed_rng_lr_ext_cat1_aes128_ref, 32, 16 * 11)
GGM_EXTSEED_RNG_LR_X4_NAIVE(ggm_seed_rng_lr_ext_cat3_rijndael256_x4_ref,  //
                            ggm_seed_rng_lr_ext_cat3_rijndael256_ref, 48, RIJNDAEL256_RK_BYTES)
GGM_EXTSEED_RNG_LR_X4_NAIVE(ggm_seed_rng_lr_ext_cat5_rijndael256_x4_ref,  //
                            ggm_seed_rng_lr_ext_cat5_rijndael256_ref, 64, RIJNDAEL256_RK_BYTES)

// ---------------------------------------------------------------------------
// Extended leaf seed: run the key schedule once, share it across commit + vole.
// For aes/rijndael the extended seed is the expanded round keys; the pad/schedule
// mirrors the raw commit/vole paths exactly, so outputs stay byte-identical.
// ---------------------------------------------------------------------------
EXPORT void extend_leaf_seed_cat1_aes128_ref(void* extended_seed_out, const void* seed128) {
  aes128_key_schedule_x1_ref(extended_seed_out, seed128);
}
EXPORT void extend_leaf_seed_cat3_rijndael256_ref(void* extended_seed_out, const void* seed192) {
  uint8_t key256[32] = {};  // k || 0^64
  memcpy(key256, seed192, 24);
  rijndael256_key_schedule_x1_ref(extended_seed_out, key256);
}
EXPORT void extend_leaf_seed_cat5_rijndael256_ref(void* extended_seed_out, const void* seed256) {
  rijndael256_key_schedule_x1_ref(extended_seed_out, seed256);
}

// 4x leaf-seed expansion (ref): run the 1x schedule four times with shifted
// pointers. Seeds/outputs are packed at their natural stride, so the result is
// byte-identical to four separate 1x calls (KAT-preserving).
EXPORT void extend_leaf_seed_cat1_aes128_4x_ref(void* extended_seed_out, const void* seeds) {
  uint8_t* out = (uint8_t*)extended_seed_out;
  const uint8_t* s = (const uint8_t*)seeds;
  for (int i = 0; i < 4; i++) extend_leaf_seed_cat1_aes128_ref(out + i * (16 * 11), s + i * 16);
}
EXPORT void extend_leaf_seed_cat3_rijndael256_4x_ref(void* extended_seed_out, const void* seeds) {
  uint8_t* out = (uint8_t*)extended_seed_out;
  const uint8_t* s = (const uint8_t*)seeds;
  for (int i = 0; i < 4; i++) extend_leaf_seed_cat3_rijndael256_ref(out + i * RIJNDAEL256_RK_BYTES, s + i * 24);
}
EXPORT void extend_leaf_seed_cat5_rijndael256_4x_ref(void* extended_seed_out, const void* seeds) {
  uint8_t* out = (uint8_t*)extended_seed_out;
  const uint8_t* s = (const uint8_t*)seeds;
  for (int i = 0; i < 4; i++) extend_leaf_seed_cat5_rijndael256_ref(out + i * RIJNDAEL256_RK_BYTES, s + i * 32);
}


EXPORT uint64_t extended_node_seed_bytes_cat1_aes128(void) { return 16 * 11; }
EXPORT uint64_t extended_node_seed_bytes_cat3_rijndael256(void) { return RIJNDAEL256_RK_BYTES; }
EXPORT uint64_t extended_node_seed_bytes_cat5_rijndael256(void) { return RIJNDAEL256_RK_BYTES; }

// commit rng, extended-seed cores: take the expanded round keys and encrypt.
EXPORT void ggm_commit_rng_ext_cat1_aes128_ref(void* output256, const void* salt128, const void* extkey128,
                                               uint64_t node_idx) {
  aesblk ctr = {.v64 = {0, 0}};
  CASSERT(domain_sep_check(salt128, node_idx << 1, 2, GGM_TWEAK_BITS), "bug! invalid separation");
  domain_sep_ptx_lo2(ctr.v64, salt128, 16, node_idx << 1);
  aes128_ctrle_nocarry_2block_ref(output256, extkey128, ctr.v64);
}
EXPORT void ggm_commit_rng_ext_cat3_rijndael256_ref(  //
    void* output384, const void* salt192, const void* extkey, uint64_t node_idx) {
  uint8_t output512[64] = {};    // pad with zeroes
  ctr256_t ctr256 = {.v8 = {}};  // pad with zeroes
  CASSERT(domain_sep_check(salt192, node_idx << 1, 2, GGM_TWEAK_BITS), "bug! invalid separation");
  domain_sep_ptx_lo2(ctr256.v64, salt192, 24, node_idx << 1);
  rijndael256_ctrle_nocarry_2block_ref(output512, extkey, ctr256.v8);
  // cat3 Enc = LSB_192(Rijndael-256): keep the low 24 bytes of each block
  memcpy(output384, output512, 24);
  memcpy((uint8_t*)output384 + 24, output512 + 32, 24);
}
EXPORT void ggm_commit_rng_ext_cat5_rijndael256_ref(  //
    void* output512, const void* salt256, const void* extkey, uint64_t node_idx) {
  ctr256_t ctr256 = {.v8 = {}};
  CASSERT(domain_sep_check(salt256, node_idx << 1, 2, GGM_TWEAK_BITS), "bug! invalid separation");
  domain_sep_ptx_lo2(ctr256.v64, salt256, 32, node_idx << 1);
  rijndael256_ctrle_nocarry_2block_ref(output512, extkey, ctr256.v8);
}

// vole rng, extended-seed cores: adopt the precomputed round keys, no schedule.
// The counter starts at the salt tweaked by the repetition index (see
// VOLE_RNG_REPET_SHIFT), and the domain_sep_check asserts that the last block of
// the stream still fits in the tweak field.
EXPORT void vole_rng_ext_cat1_aes128_ctrle_ref(void* out, uint64_t out_bytes, const void* salt128,
                                               const void* extseed, uint64_t repet_idx) {
  ctr128_t ctr = {};
  CASSERT((out_bytes & 15) == 0, "bug! out_bytes must be multiple of 16");
  CASSERT(domain_sep_check(salt128, repet_idx << VOLE_RNG_REPET_SHIFT, out_bytes >> 4, GGM_TWEAK_BITS),
          "bug! invalid separation");
  domain_sep_ptx_lo2(ctr.v64, salt128, 16, repet_idx << VOLE_RNG_REPET_SHIFT);
  aes128_ctrle_nocarry_nblocks_ref(out, extseed, ctr.v64, out_bytes >> 4);
}
EXPORT void vole_rng_ext_cat3_rijndael256_ctrle_ref(void* out, uint64_t out_bytes, const void* salt192,
                                                    const void* extseed, uint64_t repet_idx) {
  ctr256_align32_t ctr = {};
  CASSERT((out_bytes & 31) == 0, "bug! out_bytes must be multiple of 32");
  CASSERT(domain_sep_check(salt192, repet_idx << VOLE_RNG_REPET_SHIFT, out_bytes >> 5, GGM_TWEAK_BITS),
          "bug! invalid separation");
  domain_sep_ptx_lo2(ctr.v64, salt192, 24, repet_idx << VOLE_RNG_REPET_SHIFT);
  rijndael256_ctrle_nocarry_nblocks_ref(out, extseed, ctr.v64, out_bytes >> 5);
}
EXPORT void vole_rng_ext_cat5_rijndael256_ctrle_ref(void* out, uint64_t out_bytes, const void* salt256,
                                                    const void* extseed, uint64_t repet_idx) {
  ctr256_align32_t ctr = {};
  CASSERT((out_bytes & 31) == 0, "bug! out_bytes must be multiple of 32");
  CASSERT(domain_sep_check(salt256, repet_idx << VOLE_RNG_REPET_SHIFT, out_bytes >> 5, GGM_TWEAK_BITS),
          "bug! invalid separation");
  domain_sep_ptx_lo2(ctr.v64, salt256, 32, repet_idx << VOLE_RNG_REPET_SHIFT);
  rijndael256_ctrle_nocarry_nblocks_ref(out, extseed, ctr.v64, out_bytes >> 5);
}

EXPORT void xof_init_shake128(xof_ctx* xof) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashInitialize_SHAKE128(inst);
}
EXPORT void xof_init_shake256(xof_ctx* xof) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashInitialize_SHAKE256(inst);
}

EXPORT void xof_seed_shake128(xof_ctx* xof, const void* in, uint64_t in_bytes) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashUpdate(inst, in, in_bytes << 3);
}
EXPORT void xof_seed_shake256(xof_ctx* xof, const void* in, uint64_t in_bytes) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashUpdate(inst, in, in_bytes << 3);
}

EXPORT void xof_finalize_shake128(xof_ctx* xof) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashFinal(inst, NULL);
}
EXPORT void xof_finalize_shake256(xof_ctx* xof) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashFinal(inst, NULL);
}

EXPORT void xof_output_shake128(xof_ctx* xof, void* out, uint64_t out_bytes) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashSqueeze(inst, out, out_bytes << 3);
}
EXPORT void xof_output_shake256(xof_ctx* xof, void* out, uint64_t out_bytes) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashSqueeze(inst, out, out_bytes << 3);
}

// shortcut combo
EXPORT void xof_init_and_seed_shake128(xof_ctx* xof, const void* in, uint64_t in_bytes) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashInitialize_SHAKE128(inst);
  Keccak_HashUpdate(inst, in, in_bytes << 3);
}
EXPORT void xof_init_and_seed_shake256(xof_ctx* xof, const void* in, uint64_t in_bytes) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashInitialize_SHAKE256(inst);
  Keccak_HashUpdate(inst, in, in_bytes << 3);
}

EXPORT void xof_finalize_and_output_shake128(xof_ctx* xof, void* out, uint64_t out_bytes) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashFinal(inst, NULL);
  Keccak_HashSqueeze(inst, out, out_bytes << 3);
}
EXPORT void xof_finalize_and_output_shake256(xof_ctx* xof, void* out, uint64_t out_bytes) {
  Keccak_HashInstance* const inst = (Keccak_HashInstance*)xof;
  Keccak_HashFinal(inst, NULL);
  Keccak_HashSqueeze(inst, out, out_bytes << 3);
}

// vector shake function

static const uint64_t SINGLE_XOF_STRIDE = (sizeof(KeccakWidth1600_SpongeInstance) + 31) & UINT64_C(-32);
static const uint64_t BATCH_XOF_STRIDE = (sizeof(KeccakWidth1600times4_SpongeInstance) + 31) & UINT64_C(-32);
static const uint64_t XOF_RATE_SHAKE128 = 1344;
static const uint64_t XOF_RATE_SHAKE256 = 1088;
static const uint64_t XOF_CAPACITY_SHAKE128 = 256;
static const uint64_t XOF_CAPACITY_SHAKE256 = 512;
#define BATCH_XOF(base, idx) ((KeccakWidth1600times4_SpongeInstance*)((uint8_t*)(base) + (idx) * BATCH_XOF_STRIDE))
#define SINGLE_XOF(base, idx) ((KeccakWidth1600_SpongeInstance*)((uint8_t*)(base) + (idx) * SINGLE_XOF_STRIDE))


//typedef struct xof_vector_ctx_t xof_vector_ctx;

//typedef uint64_t XOF_VECTOR_CTX_BYTES_F(uint64_t num_xofs);
EXPORT uint64_t xof_vector_ctx_bytes_shake128(uint64_t num_xofs) {
  // use (num_xofs / 4) x batched shake and (num_xofs % 4) single ones
  const uint64_t num_batches = num_xofs >> UINT64_C(2);
  const uint64_t num_remainder = num_xofs & UINT64_C(3);
  return num_batches * BATCH_XOF_STRIDE + num_remainder * SINGLE_XOF_STRIDE;
}
EXPORT uint64_t xof_vector_ctx_bytes_shake256(uint64_t num_xofs) {
  return xof_vector_ctx_bytes_shake128(num_xofs); // same
}

//typedef void XOF_VECTOR_INIT_AND_SEED_F(xof_vector_ctx* xof, uint64_t num_xofs, const uint8_t* data, uint64_t data_bytes);
EXPORT void xof_vector_init_and_seed_shake128(xof_vector_ctx* xof, uint64_t num_xofs, const uint8_t* data, uint64_t data_bytes) {
  const uint64_t num_batches = num_xofs >> UINT64_C(2);
  const uint64_t num_remainder = num_xofs & UINT64_C(3);
  void* sbase = BATCH_XOF(xof, num_batches);
  const unsigned char* prefix_ptrs[4] = {
    (const unsigned char*)data, (const unsigned char*)data,
    (const unsigned char*)data, (const unsigned char*)data};
  for (uint64_t b = 0; b < num_batches; ++b) {
    KeccakWidth1600times4_SpongeInstance* const inst = BATCH_XOF(xof, b);
    KeccakWidth1600times4_SpongeInitialize(inst, XOF_RATE_SHAKE128, XOF_CAPACITY_SHAKE128);
    KeccakWidth1600times4_SpongeAbsorb(inst, prefix_ptrs, data_bytes);
  }
  for (uint64_t r = 0; r < num_remainder; ++r) {
    KeccakWidth1600_SpongeInstance* const inst = SINGLE_XOF(sbase, r);
    KeccakWidth1600_SpongeInitialize(inst, XOF_RATE_SHAKE128, XOF_CAPACITY_SHAKE128);
    KeccakWidth1600_SpongeAbsorb(inst, data, data_bytes);
  }
}

EXPORT void xof_vector_init_and_seed_shake256(xof_vector_ctx* xof, uint64_t num_xofs, const uint8_t* data, uint64_t data_bytes) {
  const uint64_t num_batches = num_xofs >> UINT64_C(2);
  const uint64_t num_remainder = num_xofs & UINT64_C(3);
  void* sbase = BATCH_XOF(xof, num_batches);
  const unsigned char* prefix_ptrs[4] = {
    (const unsigned char*)data, (const unsigned char*)data,
    (const unsigned char*)data, (const unsigned char*)data};
  for (uint64_t b = 0; b < num_batches; ++b) {
    KeccakWidth1600times4_SpongeInstance* const inst = BATCH_XOF(xof, b);
    KeccakWidth1600times4_SpongeInitialize(inst, XOF_RATE_SHAKE256, XOF_CAPACITY_SHAKE256);
    KeccakWidth1600times4_SpongeAbsorb(inst, prefix_ptrs, data_bytes);
  }
  for (uint64_t r = 0; r < num_remainder; ++r) {
    KeccakWidth1600_SpongeInstance* const inst = SINGLE_XOF(sbase, r);
    KeccakWidth1600_SpongeInitialize(inst, XOF_RATE_SHAKE256, XOF_CAPACITY_SHAKE256);
    KeccakWidth1600_SpongeAbsorb(inst, data, data_bytes);
  }
}

//typedef void XOF_VECTOR_SEED_F(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** data, uint64_t data_bytes);
EXPORT void xof_vector_seed_shake128(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** data, uint64_t data_bytes) {
  const uint64_t num_batches = num_xofs >> UINT64_C(2);
  const uint64_t num_remainder = num_xofs & UINT64_C(3);
  void* sbase = BATCH_XOF(xof, num_batches);
  unsigned char** dbase = data + num_batches * 4;
  for (uint64_t b = 0; b < num_batches; ++b) {
    KeccakWidth1600times4_SpongeInstance* const inst = BATCH_XOF(xof, b);
    KeccakWidth1600times4_SpongeAbsorb(inst, (const unsigned char **)(data + 4 * b), data_bytes);
  }
  for (uint64_t r = 0; r < num_remainder; ++r) {
    KeccakWidth1600_SpongeInstance* const inst = SINGLE_XOF(sbase, r);
    KeccakWidth1600_SpongeAbsorb(inst, dbase[r], data_bytes);
  }
}
EXPORT void xof_vector_seed_shake256(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** data, uint64_t data_bytes) {
  xof_vector_seed_shake128(xof, num_xofs, data, data_bytes); // same
}

//typedef void XOF_VECTOR_FINALIZE_AND_OUTPUT_F(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** out_hashes, uint64_t hash_bytes);
EXPORT void xof_vector_finalize_and_output_shake128(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** out_hashes, uint64_t hash_bytes) {
  const uint64_t num_batches = num_xofs >> UINT64_C(2);
  const uint64_t num_remainder = num_xofs & UINT64_C(3);
  void* sbase = BATCH_XOF(xof, num_batches);
  unsigned char** dbase = out_hashes + num_batches * 4;
  for (uint64_t b = 0; b < num_batches; ++b) {
    KeccakWidth1600times4_SpongeInstance* const inst = BATCH_XOF(xof, b);
    KeccakWidth1600times4_SpongeAbsorbLastFewBits(inst, 0x1F);
    KeccakWidth1600times4_SpongeSqueeze(inst, out_hashes + b * 4, hash_bytes);
  }
  for (uint64_t r = 0; r < num_remainder; ++r) {
    KeccakWidth1600_SpongeInstance* const inst = SINGLE_XOF(sbase, r);
    KeccakWidth1600_SpongeAbsorbLastFewBits(inst, 0x1F);
    KeccakWidth1600_SpongeSqueeze(inst, dbase[r], hash_bytes);
  }
}
EXPORT void xof_vector_finalize_and_output_shake256(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** out_hashes, uint64_t hash_bytes) {
  xof_vector_finalize_and_output_shake128(xof, num_xofs, out_hashes, hash_bytes); // same
}

// cipher-based proof of work

EXPORT uint64_t bytes_of_proofow_ctx_cipher_cat1() { return sizeof(struct proofow_state128_t); }

EXPORT void proofow_init_cipher_cat1_ref(                             //
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
  aes128_key_schedule_x2_ref(s->rk[0], s->rk[1], s->k[0].v64, s->k[1].v64);
}

EXPORT int proofow_grind_w_cipher_cat1_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out) {
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t CTR_MAX = UINT64_C(1) << 32;
  static const uint64_t H_PIOP_BYTES = 32; // CAT1
  struct proofow_state128_t* const s = (struct proofow_state128_t*)proofow_state;
  uint64_t ctr = *ctr_in_out;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  while (ctr < CTR_MAX) {
    aes128_ctrle_nocarry_1block_ref(s->c[0].v64, s->rk[0], s->p[0].v64);
    aes128_ctrle_nocarry_1block_ref(s->c[1].v64, s->rk[1], s->p[1].v64);
    if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) == 0) {
      xof_ctx xof;
      xof_init_and_seed_shake128(&xof, &PROOFOW_H1_PREFIX, 1);
      xof_seed_shake128(&xof, s->h_piop, H_PIOP_BYTES);
      xof_seed_shake128(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
      xof_seed_shake128(&xof, s->c, 32); // covers c0, c1
      xof_finalize_and_output_shake128(&xof, delta0_out, s->delta0_out_bytes);
      *ctr_in_out = ctr;
      return 1;
    }
    ++ctr;
    ++s->p[0].v64[0];
    ++s->p[1].v64[0];
  }
  return 0;
}

EXPORT int proofow_verify_w_cipher_cat1_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 32;  // CAT1
  struct proofow_state128_t* const s = (struct proofow_state128_t*)proofow_state;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  aes128_ctrle_nocarry_1block_ref(s->c[0].v64, s->rk[0], s->p[0].v64);
  aes128_ctrle_nocarry_1block_ref(s->c[1].v64, s->rk[1], s->p[1].v64);
  if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) != 0) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake128(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake128(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake128(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake128(&xof, s->c, 32);                          // covers c0, c1
  xof_finalize_and_output_shake128(&xof, delta0_out, s->delta0_out_bytes);
  return 1;
}

// cipher-based proof of work (cat5: rijndael256 block cipher + shake256 xof)

EXPORT uint64_t bytes_of_proofow_ctx_cipher_cat5() { return sizeof(struct proofow_state256_t); }

EXPORT void proofow_init_cipher_cat5_ref(                            //
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
  rijndael256_key_schedule_x2_ref(s->rk[0], s->rk[1], s->k[0].v64, s->k[1].v64);
}

EXPORT int proofow_grind_w_cipher_cat5_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out) {
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t CTR_MAX = UINT64_C(1) << 32;
  static const uint64_t H_PIOP_BYTES = 64;  // CAT5
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  uint64_t ctr = *ctr_in_out;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  while (ctr < CTR_MAX) {
    rijndael256_ctrle_nocarry_1block_ref(s->c[0].v64, s->rk[0], s->p[0].v64);
    rijndael256_ctrle_nocarry_1block_ref(s->c[1].v64, s->rk[1], s->p[1].v64);
    if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) == 0) {
      xof_ctx xof;
      xof_init_and_seed_shake256(&xof, &PROOFOW_H1_PREFIX, 1);
      xof_seed_shake256(&xof, s->h_piop, H_PIOP_BYTES);
      xof_seed_shake256(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
      xof_seed_shake256(&xof, s->c, 64);  // covers c0, c1 (32 bytes each)
      xof_finalize_and_output_shake256(&xof, delta0_out, s->delta0_out_bytes);
      *ctr_in_out = ctr;
      return 1;
    }
    ++ctr;
    ++s->p[0].v64[0];
    ++s->p[1].v64[0];
  }
  return 0;
}

EXPORT int proofow_verify_w_cipher_cat5_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 64;  // CAT5
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  rijndael256_ctrle_nocarry_1block_ref(s->c[0].v64, s->rk[0], s->p[0].v64);
  rijndael256_ctrle_nocarry_1block_ref(s->c[1].v64, s->rk[1], s->p[1].v64);
  if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) != 0) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake256(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake256(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake256(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake256(&xof, s->c, 64);                          // covers c0, c1 (32 bytes each)
  xof_finalize_and_output_shake256(&xof, delta0_out, s->delta0_out_bytes);
  return 1;
}

// cipher-based proof of work (cat3: rijndael256 block cipher + shake256 xof).
// Identical to cat5 except h_piop is 2*lambda = 384 bits = 48 bytes, so the
// cat5 proofow_state256_t workspace (h_piop[64]) is reused as-is.

EXPORT uint64_t bytes_of_proofow_ctx_cipher_cat3() { return sizeof(struct proofow_state256_t); }

EXPORT void proofow_init_cipher_cat3_ref(                            //
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
  rijndael256_key_schedule_x2_ref(s->rk[0], s->rk[1], s->k[0].v64, s->k[1].v64);
}

EXPORT int proofow_grind_w_cipher_cat3_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out) {
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t CTR_MAX = UINT64_C(1) << 32;
  static const uint64_t H_PIOP_BYTES = 48;  // CAT3
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  uint64_t ctr = *ctr_in_out;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  while (ctr < CTR_MAX) {
    rijndael256_ctrle_nocarry_1block_ref(s->c[0].v64, s->rk[0], s->p[0].v64);
    rijndael256_ctrle_nocarry_1block_ref(s->c[1].v64, s->rk[1], s->p[1].v64);
    if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) == 0) {
      xof_ctx xof;
      xof_init_and_seed_shake256(&xof, &PROOFOW_H1_PREFIX, 1);
      xof_seed_shake256(&xof, s->h_piop, H_PIOP_BYTES);
      xof_seed_shake256(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
      xof_seed_shake256(&xof, s->c, 64);  // covers c0, c1 (32 bytes each)
      xof_finalize_and_output_shake256(&xof, delta0_out, s->delta0_out_bytes);
      *ctr_in_out = ctr;
      return 1;
    }
    ++ctr;
    ++s->p[0].v64[0];
    ++s->p[1].v64[0];
  }
  return 0;
}

EXPORT int proofow_verify_w_cipher_cat3_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t MASK_ELL_HI = UINT64_C(-1) << 32;
  static const uint64_t H_PIOP_BYTES = 48;  // CAT3
  struct proofow_state256_t* const s = (struct proofow_state256_t*)proofow_state;
  s->p[0].v64[0] = (s->p[0].v64[0] & MASK_ELL_HI) | ctr;
  s->p[1].v64[0] = (s->p[1].v64[0] & MASK_ELL_HI) | ctr;
  rijndael256_ctrle_nocarry_1block_ref(s->c[0].v64, s->rk[0], s->p[0].v64);
  rijndael256_ctrle_nocarry_1block_ref(s->c[1].v64, s->rk[1], s->p[1].v64);
  if (((s->c[0].v64[0] ^ s->c[1].v64[0]) & s->mask_w) != 0) return 0;
  xof_ctx xof;
  xof_init_and_seed_shake256(&xof, &PROOFOW_H1_PREFIX, 1);
  xof_seed_shake256(&xof, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake256(&xof, &ctr, PROOFOW_CTR_REVEALED_BYTES);  // always 4 bytes
  xof_seed_shake256(&xof, s->c, 64);                          // covers c0, c1 (32 bytes each)
  xof_finalize_and_output_shake256(&xof, delta0_out, s->delta0_out_bytes);
  return 1;
}

/// shake-based proof of work

EXPORT uint64_t bytes_of_proofow_ctx_shake_cat1() { return sizeof(struct proofow_shake_cat1_t); }

EXPORT void proofow_init_shake_cat1(           //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
) {
  struct proofow_shake_cat1_t* const s = (struct proofow_shake_cat1_t*)proofow_state;
  memset(s->delta0_and_vgrind, 0, sizeof(s->delta0_and_vgrind));
  memcpy(s->h_piop, h_piop, (lambda * 2) >> 3);
  s->w = proofow_w;
  s->kappa_tau = kappa * tau;
  s->delta0_and_vgrind_bytes = (s->kappa_tau + proofow_w + 7) >> 3;
  s->delta0_out_bytes = (s->kappa_tau + 7) >> 3;
}

EXPORT int proofow_grind_w_shake_cat1(  //
    proofow_ctx_t* proofow_state,       //
    bitvec_t* delta0_out,               //
    uint64_t* ctr_in_out                //
) {
  static const uint64_t H_PIOP_BYTES = 32;
  static const uint64_t CTR_MAX = UINT64_C(1) << 32;
  struct proofow_shake_cat1_t* const s = (struct proofow_shake_cat1_t*)proofow_state;
  xof_ctx ctx __attribute((aligned(16)));
  for (uint64_t ctr = *ctr_in_out; ctr < CTR_MAX; ctr++) {
    xof_init_and_seed_shake128(&ctx, s->h_piop, H_PIOP_BYTES);
    xof_seed_shake128(&ctx, &ctr, 4);
    xof_finalize_and_output_shake128(&ctx, s->delta0_and_vgrind, s->delta0_and_vgrind_bytes);
    if (extract_kappabit_uint(s->w, s->kappa_tau, s->delta0_and_vgrind) == 0) {
      *ctr_in_out = ctr;
      memcpy(delta0_out, s->delta0_and_vgrind, s->delta0_out_bytes);
      return 1;
    }
  }
  // no good counter: pow has failed
  return 0;
}

EXPORT int proofow_verify_w_shake_cat1(  //
    proofow_ctx_t* proofow_state,        //
    void* delta0_out,                    //
    const uint64_t ctr_value             //
) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t H_PIOP_BYTES = 32;
  struct proofow_shake_cat1_t* const s = (struct proofow_shake_cat1_t*)proofow_state;
  const uint64_t ctr = ctr_value;
  xof_ctx ctx __attribute((aligned(16)));
  xof_init_and_seed_shake128(&ctx, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake128(&ctx, &ctr, 4);
  xof_finalize_and_output_shake128(&ctx, s->delta0_and_vgrind, s->delta0_and_vgrind_bytes);
  if (extract_kappabit_uint(s->w, s->kappa_tau, s->delta0_and_vgrind) != 0) return 0;
  memcpy(delta0_out, s->delta0_and_vgrind, s->delta0_out_bytes);
  return 1;
}

EXPORT uint64_t bytes_of_proofow_ctx_shake_cat3() { return sizeof(struct proofow_shake_cat3_t); }

EXPORT void proofow_init_shake_cat3(           //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
) {
  struct proofow_shake_cat3_t* const s = (struct proofow_shake_cat3_t*)proofow_state;
  memset(s->delta0_and_vgrind, 0, sizeof(s->delta0_and_vgrind));
  memcpy(s->h_piop, h_piop, (lambda * 2) >> 3);
  s->w = proofow_w;
  s->kappa_tau = kappa * tau;
  s->delta0_and_vgrind_bytes = (s->kappa_tau + proofow_w + 7) >> 3;
  s->delta0_out_bytes = (s->kappa_tau + 7) >> 3;
}

EXPORT int proofow_grind_w_shake_cat3(  //
    proofow_ctx_t* proofow_state,       //
    bitvec_t* delta0_out,               //
    uint64_t* ctr_in_out                //
) {
  static const uint64_t H_PIOP_BYTES = 48;
  static const uint64_t CTR_MAX = UINT64_C(1) << 32;
  struct proofow_shake_cat3_t* const s = (struct proofow_shake_cat3_t*)proofow_state;
  xof_ctx ctx __attribute((aligned(16)));
  for (uint64_t ctr = *ctr_in_out; ctr < CTR_MAX; ctr++) {
    xof_init_and_seed_shake256(&ctx, s->h_piop, H_PIOP_BYTES);
    xof_seed_shake256(&ctx, &ctr, 4);
    xof_finalize_and_output_shake256(&ctx, s->delta0_and_vgrind, s->delta0_and_vgrind_bytes);
    if (extract_kappabit_uint(s->w, s->kappa_tau, s->delta0_and_vgrind) == 0) {
      *ctr_in_out = ctr;
      memcpy(delta0_out, s->delta0_and_vgrind, s->delta0_out_bytes);
      return 1;
    }
  }
  // no good counter: pow has failed
  return 0;
}

EXPORT int proofow_verify_w_shake_cat3(  //
    proofow_ctx_t* proofow_state,        //
    void* delta0_out,                    //
    const uint64_t ctr_value             //
) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t H_PIOP_BYTES = 48;
  struct proofow_shake_cat3_t* const s = (struct proofow_shake_cat3_t*)proofow_state;
  const uint64_t ctr = ctr_value;
  xof_ctx ctx __attribute((aligned(16)));
  xof_init_and_seed_shake256(&ctx, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake256(&ctx, &ctr, 4);
  xof_finalize_and_output_shake256(&ctx, s->delta0_and_vgrind, s->delta0_and_vgrind_bytes);
  if (extract_kappabit_uint(s->w, s->kappa_tau, s->delta0_and_vgrind) != 0) return 0;
  memcpy(delta0_out, s->delta0_and_vgrind, s->delta0_out_bytes);
  return 1;
}

EXPORT uint64_t bytes_of_proofow_ctx_shake_cat5() { return sizeof(struct proofow_shake_cat5_t); }

EXPORT void proofow_init_shake_cat5(           //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
) {
  struct proofow_shake_cat5_t* const s = (struct proofow_shake_cat5_t*)proofow_state;
  memset(s->delta0_and_vgrind, 0, sizeof(s->delta0_and_vgrind));
  memcpy(s->h_piop, h_piop, (lambda * 2) >> 3);
  s->w = proofow_w;
  s->kappa_tau = kappa * tau;
  s->delta0_and_vgrind_bytes = (s->kappa_tau + proofow_w + 7) >> 3;
  s->delta0_out_bytes = (s->kappa_tau + 7) >> 3;
}

EXPORT int proofow_grind_w_shake_cat5(  //
    proofow_ctx_t* proofow_state,       //
    bitvec_t* delta0_out,               //
    uint64_t* ctr_in_out                //
) {
  static const uint64_t H_PIOP_BYTES = 64;
  static const uint64_t CTR_MAX = UINT64_C(1) << 32;
  struct proofow_shake_cat5_t* const s = (struct proofow_shake_cat5_t*)proofow_state;
  xof_ctx ctx __attribute((aligned(16)));
  for (uint64_t ctr = *ctr_in_out; ctr < CTR_MAX; ctr++) {
    xof_init_and_seed_shake256(&ctx, s->h_piop, H_PIOP_BYTES);
    xof_seed_shake256(&ctx, &ctr, 4);
    xof_finalize_and_output_shake128(&ctx, s->delta0_and_vgrind, s->delta0_and_vgrind_bytes);
    if (extract_kappabit_uint(s->w, s->kappa_tau, s->delta0_and_vgrind) == 0) {
      *ctr_in_out = ctr;
      memcpy(delta0_out, s->delta0_and_vgrind, s->delta0_out_bytes);
      return 1;
    }
  }
  // no good counter: pow has failed
  return 0;
}

EXPORT int proofow_verify_w_shake_cat5(  //
    proofow_ctx_t* proofow_state,        //
    void* delta0_out,                    //
    const uint64_t ctr_value             //
) {
  // Verification checks the single provided counter (no grind loop).
  static const uint64_t H_PIOP_BYTES = 64;
  struct proofow_shake_cat5_t* const s = (struct proofow_shake_cat5_t*)proofow_state;
  const uint64_t ctr = ctr_value;
  xof_ctx ctx __attribute((aligned(16)));
  xof_init_and_seed_shake256(&ctx, s->h_piop, H_PIOP_BYTES);
  xof_seed_shake256(&ctx, &ctr, 4);
  xof_finalize_and_output_shake128(&ctx, s->delta0_and_vgrind, s->delta0_and_vgrind_bytes);
  if (extract_kappabit_uint(s->w, s->kappa_tau, s->delta0_and_vgrind) != 0) return 0;
  memcpy(delta0_out, s->delta0_and_vgrind, s->delta0_out_bytes);
  return 1;
}

// 4-way batched SHAKE proof-of-work grind (AVX2 KeccakP-1600-times4). Drives 4
// consecutive counters through one batched permutation instead of 4 scalar ones. The <4 tail near CTR_MAX falls back to the scalar path.
#define PROOFOW_GRIND_SHAKE_4WAY(CAT, SHAKE, HPB)                                            \
  EXPORT int proofow_grind_w_shake_##CAT##_4way(proofow_ctx_t* proofow_state,                \
                                                bitvec_t* delta0_out, uint64_t* ctr_in_out) {\
    static const uint64_t H_PIOP_BYTES = HPB;                                                \
    static const uint64_t CTR_MAX = UINT64_C(1) << 32;                                       \
    struct proofow_shake_##CAT##_t* const s = (struct proofow_shake_##CAT##_t*)proofow_state;\
    uint8_t xvbuf[sizeof(KeccakWidth1600times4_SpongeInstance) + 32] __attribute((aligned(64)));\
    xof_vector_ctx* const xv = (xof_vector_ctx*)xvbuf;                                       \
    uint8_t buf[4][sizeof(s->delta0_and_vgrind)];                                            \
    uint32_t ctr32[4];                                                                       \
    uint8_t *ctr_ptrs[4], *out_ptrs[4];                                                      \
    for (int j = 0; j < 4; j++) {                                                            \
      ctr_ptrs[j] = (uint8_t*)&ctr32[j];                                                     \
      out_ptrs[j] = buf[j];                                                                  \
    }                                                                                        \
    uint64_t ctr = *ctr_in_out;                                                              \
    for (; ctr + 4 <= CTR_MAX; ctr += 4) {                                                   \
      for (int j = 0; j < 4; j++) ctr32[j] = (uint32_t)(ctr + j);                            \
      xof_vector_init_and_seed_##SHAKE(xv, 4, s->h_piop, H_PIOP_BYTES);                       \
      xof_vector_seed_##SHAKE(xv, 4, ctr_ptrs, 4);                                            \
      xof_vector_finalize_and_output_##SHAKE(xv, 4, out_ptrs, s->delta0_and_vgrind_bytes);    \
      for (int j = 0; j < 4; j++)                                                            \
        if (extract_kappabit_uint(s->w, s->kappa_tau, buf[j]) == 0) {                         \
          *ctr_in_out = ctr + j;                                                             \
          memcpy(delta0_out, buf[j], s->delta0_out_bytes);                                   \
          return 1;                                                                          \
        }                                                                                    \
    }                                                                                        \
    for (; ctr < CTR_MAX; ctr++) { /* <4 tail, byte-identical scalar path */                 \
      xof_ctx c __attribute((aligned(16)));                                                  \
      xof_init_and_seed_##SHAKE(&c, s->h_piop, H_PIOP_BYTES);                                 \
      xof_seed_##SHAKE(&c, &ctr, 4);                                                          \
      xof_finalize_and_output_##SHAKE(&c, s->delta0_and_vgrind, s->delta0_and_vgrind_bytes);  \
      if (extract_kappabit_uint(s->w, s->kappa_tau, s->delta0_and_vgrind) == 0) {             \
        *ctr_in_out = ctr;                                                                   \
        memcpy(delta0_out, s->delta0_and_vgrind, s->delta0_out_bytes);                       \
        return 1;                                                                            \
      }                                                                                      \
    }                                                                                        \
    return 0;                                                                                \
  }
PROOFOW_GRIND_SHAKE_4WAY(cat1, shake128, 32)
PROOFOW_GRIND_SHAKE_4WAY(cat3, shake256, 48)
PROOFOW_GRIND_SHAKE_4WAY(cat5, shake256, 64)

// set_2_b_minus_one (the last-block mask builder) is shared with the avx path,
// so it lives in sdith_prng_private.h.

static inline void apply_last_blk_mask(uint64_t* res, const uint64_t* mask, uint64_t res_size) {
  for (uint64_t i=0; i<res_size; ++i) {
    res[i] &= mask[i];
  }
}

EXPORT void matrix_rng_init_aes128_cat1_ref(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size) {
  struct matrix_rng_aes128_cat1_t* r = (struct matrix_rng_aes128_cat1_t*) rng;
  aes128_key_schedule_x1_ref(r->rk, seed);
  r->blocks_per_row = ((row_bit_size + 127) >> 7); // aes128 uses 128-bit block sizes
  r->row_bit_size = row_bit_size;
  set_2_b_minus_one(r->last_blk_mask.v64, 2, row_bit_size & 127);
}

EXPORT void matrix_rng_init_rijndael256_cat3_ref(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size) {
  struct matrix_rng_rijndael256_cat3_t* r = (struct matrix_rng_rijndael256_cat3_t*) rng;
  // the actual rijndael key is padded with zeroes
  uint64_t key256[4] = {}; // ZERO
  memcpy(key256, seed, 24);
  rijndael256_key_schedule_x1_ref(r->rk, key256);
  // we are using the full 256-bit rijndael block size, even if lambda is smaller.
  r->blocks_per_row = ((row_bit_size + 255) >> 8);
  r->row_bit_size = row_bit_size;
  set_2_b_minus_one(r->last_blk_mask.v64, 4, row_bit_size & 255);
}

EXPORT void matrix_rng_init_rijndael256_cat5_ref(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size) {
  struct matrix_rng_rijndael256_cat5_t* r = (struct matrix_rng_rijndael256_cat5_t*) rng;
  rijndael256_key_schedule_x1_ref(r->rk, seed);
  r->blocks_per_row = ((row_bit_size + 255) >> 8); // rijndael256 uses 256-bit block sizes
  r->row_bit_size = row_bit_size;
  set_2_b_minus_one(r->last_blk_mask.v64, 4, row_bit_size & 255);
}

EXPORT void matrix_rng_get_row_aes128_cat1_ref(const matrix_rng_t* rng, void* out, const uint64_t row_index) {
  struct matrix_rng_aes128_cat1_t* r = (struct matrix_rng_aes128_cat1_t*) rng;
  CASSERT((((uint64_t)out) & 15) == 0, "out must be 16-byte aligned");
  ctr128_t* const cout = (ctr128_t*) out;
  const uint64_t bpr = r->blocks_per_row;
  ctr128_t ctr = {};
  ctr.v64[0] = row_index * bpr;
  aes128_ctrle_nocarry_nblocks_ref(out, r->rk, ctr.v64, bpr);
  apply_last_blk_mask(cout[bpr - 1].v64, r->last_blk_mask.v64, 2);
}

EXPORT void matrix_rng_get_row_rijndael256_cat3_ref(const matrix_rng_t* rng, void* out, const uint64_t row_index) {
  struct matrix_rng_rijndael256_cat3_t* r = (struct matrix_rng_rijndael256_cat3_t*) rng;
  CASSERT((((uint64_t)out) & 31) == 0, "out must be 32-bytes aligned");
  // cat3 is providing the same full rijndael256-ctr as cat 5
  ctr256_t* const cout = (ctr256_t*) out;
  const uint64_t bpr = r->blocks_per_row;
  ctr256_t ctr = {};
  ctr.v64[0] = row_index * bpr;
  rijndael256_ctrle_nocarry_nblocks_ref(out, r->rk, ctr.v64, bpr);
  apply_last_blk_mask(cout[bpr - 1].v64, r->last_blk_mask.v64, 4);
}

EXPORT void matrix_rng_get_row_rijndael256_cat5_ref(const matrix_rng_t* rng, void* out, const uint64_t row_index) {
  struct matrix_rng_rijndael256_cat5_t* r = (struct matrix_rng_rijndael256_cat5_t*) rng;
  CASSERT((((uint64_t)out) & 31) == 0, "out must be 32-byte aligned");
  ctr256_t* const cout = (ctr256_t*) out;
  const uint64_t bpr = r->blocks_per_row;
  ctr256_t ctr = {};
  ctr.v64[0] = row_index * bpr;
  rijndael256_ctrle_nocarry_nblocks_ref(out, r->rk, ctr.v64, bpr);
  apply_last_blk_mask(cout[bpr - 1].v64, r->last_blk_mask.v64, 4);
}

// Pre-processed challenge (see MATRIX_RNG_PREPROCESS_CHALL). With k = lambda/64,
//     prep_chall[j] = chall[j / k] * X^(64 * (j % k)),
// so that <row, chall> = sum_j h_j * prep_chall[j] over the 64-bit words h_j of
// the row. The loop itself is preprocess_chall_generic in sdith_prng_private.h,
// shared with the avx path: only the field product differs, and both orderings
// of it give the very same table.

EXPORT void matrix_rng_preprocess_chall_aes128_cat1_ref(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                        const flambda_t* chall) {
  const struct matrix_rng_aes128_cat1_t* r = (const struct matrix_rng_aes128_cat1_t*) rng;
  preprocess_chall_generic(prep_chall, chall, matrix_rng_prep_chall_nelems(r->row_bit_size),
                           16, 2, gf128_flambda_product_ref);
}

EXPORT void matrix_rng_preprocess_chall_rijndael256_cat3_ref(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                             const flambda_t* chall) {
  const struct matrix_rng_rijndael256_cat3_t* r = (const struct matrix_rng_rijndael256_cat3_t*) rng;
  preprocess_chall_generic(prep_chall, chall, matrix_rng_prep_chall_nelems(r->row_bit_size),
                           24, 3, gf192_flambda_product_ref);
}

EXPORT void matrix_rng_preprocess_chall_rijndael256_cat5_ref(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                             const flambda_t* chall) {
  const struct matrix_rng_rijndael256_cat5_t* r = (const struct matrix_rng_rijndael256_cat5_t*) rng;
  preprocess_chall_generic(prep_chall, chall, matrix_rng_prep_chall_nelems(r->row_bit_size),
                           32, 4, gf256_flambda_product_ref);
}

// Reference row-times-challenge (see MATRIX_RNG_ROWS_TIMES_CHALL). This is the
// definition transcribed literally: draw the row, split it into 64-bit words h_j,
// and accumulate sum_j h_j * prep_chall[j] with the ordinary field product and
// sum. Each h_j is a field element whose low 64 bits are the word and whose other
// bits are zero. No attempt is made to be quick: it is written to stay readable
// so it can serve as the oracle for the avx kernels, which are what the x86
// build runs in production.
//
// Where there are no avx kernels this is the production path, and then the word
// decomposition does more work than it needs to. Writing k = lambda/64, the
// definition of the pre-processed challenge gives prep_chall[k*i] = chall[i] *
// X^0 = chall[i], so the sum collapses back to the plain limb dot product
//     <row, chall> = sum_i row_limb[i] * prep_chall[k*i],
// running over the ceil(row_bits/lambda) limbs instead of the ceil(row_bits/64)
// words: k times fewer field products (k = 2, 3, 4 for cat1/cat3/cat5), and no
// change to the pre-processed challenge, so the two forms stay interchangeable.
// Reading whole limbs is in bounds because compute_rsd_codim_slice guarantees
// the row is wide enough to hold rsd_codim_limbs of them. Swap the inner loop
// for that form if a portable build ever needs the speed.
//
// The row is at most MATRIX_RNG_REF_MAX_ROW_BYTES; 2kB is a 16384-bit row, far
// above any rsd_codim. The buffer is 32-byte aligned as get_row requires.
#define MATRIX_RNG_REF_MAX_ROW_BYTES 2048

static void rows_times_chall_ref_generic(                                             //
    const matrix_rng_t* rng, flambda_t* dest, const flambda_t* prep_chall,            //
    const uint64_t first_row, const uint64_t nrows,                                   //
    const uint64_t row_bit_size, const uint64_t row_bytes, const uint64_t lambda_bytes,  //
    MATRIX_RNG_GET_ROW* get_row, FLAMBDA_PRODUCT_F* product, FLAMBDA_SUM_F* sum) {
  CREQUIRE(row_bytes <= MATRIX_RNG_REF_MAX_ROW_BYTES, "row too wide (%ld bytes)", (long) row_bytes);
  const uint64_t nwords = matrix_rng_prep_chall_nelems(row_bit_size);
  const uint8_t* const pc = (const uint8_t*) prep_chall;
  uint8_t* out = (uint8_t*) dest;
  uint8_t row[MATRIX_RNG_REF_MAX_ROW_BYTES] __attribute__((aligned(32)));
  for (uint64_t i = 0; i < nrows; ++i) {
    get_row(rng, row, first_row + i);
    flambda_max_t acc = {0, 0, 0, 0};
    for (uint64_t j = 0; j < nwords; ++j) {
      // h_j: the j-th 64-bit word of the row, zero-extended to a field element
      flambda_max_t h = {0, 0, 0, 0};
      memcpy(h, row + 8 * j, 8);
      flambda_max_t term;
      product(term, h, pc + j * lambda_bytes);
      sum(acc, acc, term);
    }
    memcpy(out, acc, lambda_bytes);
    out += lambda_bytes;
  }
}

EXPORT void matrix_rng_rows_times_chall_aes128_cat1_ref(const matrix_rng_t* rng, flambda_t* dest,
                                                        const flambda_t* prep_chall, const uint64_t first_row,
                                                        const uint64_t nrows) {
  const struct matrix_rng_aes128_cat1_t* r = (const struct matrix_rng_aes128_cat1_t*) rng;
  rows_times_chall_ref_generic(rng, dest, prep_chall, first_row, nrows,       //
                               r->row_bit_size, r->blocks_per_row * 16, 16,   //
                               matrix_rng_get_row_aes128_cat1_ref, gf128_flambda_product_ref,
                               gf128_flambda_sum_ref);
}

EXPORT void matrix_rng_rows_times_chall_rijndael256_cat3_ref(const matrix_rng_t* rng, flambda_t* dest,
                                                             const flambda_t* prep_chall, const uint64_t first_row,
                                                             const uint64_t nrows) {
  const struct matrix_rng_rijndael256_cat3_t* r = (const struct matrix_rng_rijndael256_cat3_t*) rng;
  rows_times_chall_ref_generic(rng, dest, prep_chall, first_row, nrows,       //
                               r->row_bit_size, r->blocks_per_row * 32, 24,   //
                               matrix_rng_get_row_rijndael256_cat3_ref, gf192_flambda_product_ref,
                               gf192_flambda_sum_ref);
}

EXPORT void matrix_rng_rows_times_chall_rijndael256_cat5_ref(const matrix_rng_t* rng, flambda_t* dest,
                                                             const flambda_t* prep_chall, const uint64_t first_row,
                                                             const uint64_t nrows) {
  const struct matrix_rng_rijndael256_cat5_t* r = (const struct matrix_rng_rijndael256_cat5_t*) rng;
  rows_times_chall_ref_generic(rng, dest, prep_chall, first_row, nrows,       //
                               r->row_bit_size, r->blocks_per_row * 32, 32,   //
                               matrix_rng_get_row_rijndael256_cat5_ref, gf256_flambda_product_ref,
                               gf256_flambda_sum_ref);
}

// Batched row generation. Rows first_row..first_row+nrows-1 occupy the counter
// range [first_row*bpr, (first_row+nrows)*bpr), which is contiguous, so the whole
// batch is one CTR call; only the per-row last-block masking is left to do.

EXPORT void matrix_rng_get_rows_aes128_cat1_ref(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                const uint64_t nrows) {
  struct matrix_rng_aes128_cat1_t* r = (struct matrix_rng_aes128_cat1_t*) rng;
  CASSERT((((uint64_t)out) & 15) == 0, "out must be 16-byte aligned");
  ctr128_t* const cout = (ctr128_t*) out;
  const uint64_t bpr = r->blocks_per_row;
  ctr128_t ctr = {};
  ctr.v64[0] = first_row * bpr;
  aes128_ctrle_nocarry_nblocks_ref(out, r->rk, ctr.v64, nrows * bpr);
  for (uint64_t i = 0; i < nrows; ++i) {
    apply_last_blk_mask(cout[i * bpr + bpr - 1].v64, r->last_blk_mask.v64, 2);
  }
}

EXPORT void matrix_rng_get_rows_rijndael256_cat3_ref(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                     const uint64_t nrows) {
  struct matrix_rng_rijndael256_cat3_t* r = (struct matrix_rng_rijndael256_cat3_t*) rng;
  CASSERT((((uint64_t)out) & 31) == 0, "out must be 32-bytes aligned");
  // cat3 is providing the same full rijndael256-ctr as cat 5
  ctr256_t* const cout = (ctr256_t*) out;
  const uint64_t bpr = r->blocks_per_row;
  ctr256_t ctr = {};
  ctr.v64[0] = first_row * bpr;
  rijndael256_ctrle_nocarry_nblocks_ref(out, r->rk, ctr.v64, nrows * bpr);
  for (uint64_t i = 0; i < nrows; ++i) {
    apply_last_blk_mask(cout[i * bpr + bpr - 1].v64, r->last_blk_mask.v64, 4);
  }
}

EXPORT void matrix_rng_get_rows_rijndael256_cat5_ref(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                     const uint64_t nrows) {
  struct matrix_rng_rijndael256_cat5_t* r = (struct matrix_rng_rijndael256_cat5_t*) rng;
  CASSERT((((uint64_t)out) & 31) == 0, "out must be 32-byte aligned");
  ctr256_t* const cout = (ctr256_t*) out;
  const uint64_t bpr = r->blocks_per_row;
  ctr256_t ctr = {};
  ctr.v64[0] = first_row * bpr;
  rijndael256_ctrle_nocarry_nblocks_ref(out, r->rk, ctr.v64, nrows * bpr);
  for (uint64_t i = 0; i < nrows; ++i) {
    apply_last_blk_mask(cout[i * bpr + bpr - 1].v64, r->last_blk_mask.v64, 4);
  }
}

// --- keygen prng (reference) ------------------------------------------------
// The plain CTR keystream of the secret-key seed (see keygen_rng_ctx). Only the
// key differs between the three categories; the draw loop is the shared
// DEFINE_KEYGEN_RNG_NEXT_U32 body.

EXPORT void keygen_rng_init_aes128_cat1_ref(keygen_rng_ctx* rng, const seed_t* sk_seed) {
  struct keygen_rng_aes128_cat1_t* s = (struct keygen_rng_aes128_cat1_t*) rng;
  aes128_key_schedule_x1_ref(s->rk, sk_seed);
  KEYGEN_RNG_INIT_EMPTY(s);
}

EXPORT void keygen_rng_init_rijndael256_cat3_ref(keygen_rng_ctx* rng, const seed_t* sk_seed) {
  struct keygen_rng_rijndael256_t* s = (struct keygen_rng_rijndael256_t*) rng;
  // the 192-bit seed is padded with 64 zero bits in the MSB to make a rijndael256 key
  uint64_t key256[4] = {};  // ZERO
  memcpy(key256, sk_seed, 24);
  rijndael256_key_schedule_x1_ref(s->rk, key256);
  KEYGEN_RNG_INIT_EMPTY(s);
}

EXPORT void keygen_rng_init_rijndael256_cat5_ref(keygen_rng_ctx* rng, const seed_t* sk_seed) {
  struct keygen_rng_rijndael256_t* s = (struct keygen_rng_rijndael256_t*) rng;
  rijndael256_key_schedule_x1_ref(s->rk, sk_seed);
  KEYGEN_RNG_INIT_EMPTY(s);
}

DEFINE_KEYGEN_RNG_NEXT_U32(keygen_rng_next_u32_aes128_cat1_ref, keygen_rng_aes128_cat1_t, ctr128_t,
                           aes128_ctrle_nocarry_nblocks_ref)
DEFINE_KEYGEN_RNG_NEXT_U32(keygen_rng_next_u32_rijndael256_ref, keygen_rng_rijndael256_t, ctr256_t,
                           rijndael256_ctrle_nocarry_nblocks_ref)
