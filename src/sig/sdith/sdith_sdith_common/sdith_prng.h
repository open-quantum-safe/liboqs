#ifndef AQ_VOLE_IMPL_RNG_H
#define AQ_VOLE_IMPL_RNG_H

#include "commons.h"

/**
 * Domain separation of the GGM tree and vole PRNGs.
 *
 * compute_tweaked_salts derives, from the lambda-bit global salt, one masked
 * salt per PRNG family: the two most significant bits carry the family prefix
 * and the GGM_TWEAK_BITS least significant bits are cleared. Each PRNG call ORs
 * its own tweak into that cleared field and uses the result as the lambda-bit
 * initial CTR plaintext:
 *   - GGM seed / commit rng: the tweak is the node index (times 2, since each
 *     call consumes two consecutive counters),
 *   - vole rng: the tweak is repet_idx << VOLE_RNG_REPET_SHIFT, so the block
 *     counter of the call occupies the low VOLE_RNG_REPET_SHIFT bits.
 */
#define GGM_TWEAK_BITS 24
#define VOLE_RNG_REPET_SHIFT 16

/** proof of work: state init */
typedef struct proofow_ctx_t proofow_ctx_t;
typedef uint64_t BYTES_OF_PROOFOW_CTX_F();
typedef void PROOFOW_INIT_F(                                          //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
typedef int PROOFOW_GRIND_W_F(     //
    proofow_ctx_t* proofow_state,  //
    bitvec_t* delta0_out,          //
    uint64_t* ctr_in_out           //
);
typedef int PROOFOW_VERIFY_W_F(    //
    proofow_ctx_t* proofow_state,  //
    bitvec_t* delta0_out,          //
    const uint64_t ctr_value       //
);
EXPORT uint64_t bytes_of_proofow_ctx_cipher_cat1();
EXPORT void proofow_init_cipher_cat1_ref(                             //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT void proofow_init_cipher_cat1_avx(                             //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT int proofow_grind_w_cipher_cat1_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_grind_w_cipher_cat1_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_verify_w_cipher_cat1_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr);
EXPORT int proofow_verify_w_cipher_cat1_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr);

EXPORT uint64_t bytes_of_proofow_ctx_cipher_cat5();
EXPORT void proofow_init_cipher_cat5_ref(                            //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT int proofow_grind_w_cipher_cat5_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_verify_w_cipher_cat5_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr);
EXPORT void proofow_init_cipher_cat5_avx(                            //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT int proofow_grind_w_cipher_cat5_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_verify_w_cipher_cat5_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr);

// cipher-based proof of work (cat3: rijndael256 block cipher + shake256 xof, h_piop = 48 bytes)

EXPORT uint64_t bytes_of_proofow_ctx_cipher_cat3();
EXPORT void proofow_init_cipher_cat3_ref(                            //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT int proofow_grind_w_cipher_cat3_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_verify_w_cipher_cat3_ref(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr);
EXPORT void proofow_init_cipher_cat3_avx(                            //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT int proofow_grind_w_cipher_cat3_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_verify_w_cipher_cat3_avx(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr);

// shake-based proof of work

EXPORT uint64_t bytes_of_proofow_ctx_shake_cat1();
EXPORT void proofow_init_shake_cat1(                                  //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT int proofow_grind_w_shake_cat1(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_grind_w_shake_cat1_4way(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_verify_w_shake_cat1(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr_value);

EXPORT uint64_t bytes_of_proofow_ctx_shake_cat3();
EXPORT void proofow_init_shake_cat3(                                  //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT int proofow_grind_w_shake_cat3(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_grind_w_shake_cat3_4way(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_verify_w_shake_cat3(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr_value);

EXPORT uint64_t bytes_of_proofow_ctx_shake_cat5();
EXPORT void proofow_init_shake_cat5(                                  //
    proofow_ctx_t* proofow_state,                                     //
    const uint64_t lambda, const uint64_t kappa, const uint64_t tau,  // vole params
    const uint64_t proofow_w,                                         // dims
    const hash_t* h_piop                                              // input
);
EXPORT int proofow_grind_w_shake_cat5(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_grind_w_shake_cat5_4way(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, uint64_t* ctr_in_out);
EXPORT int proofow_verify_w_shake_cat5(proofow_ctx_t* proofow_state, bitvec_t* delta0_out, const uint64_t ctr_value);


// ggm seed rng, extended-seed variants: same as ggm_seed_rng_lr_* but the
// node_seed points at the expanded round keys (skips the key schedule). Output
// is byte-identical to the raw variant fed the matching extend_leaf_seed output.
typedef void GGM_EXTSEED_RNG_LR_F(void* lr_out, const void* salt, const void* extended_seed, uint64_t node_idx, uint32_t theta);
// x4 variant (I8): expand 4 sibling parent nodes (node_idx0, +2, +4, +6) in one call.
// extended_seeds = 4 contiguous extended keys, lr_out = 4 contiguous 2-seed outputs.
// Byte-identical to four GGM_EXTSEED_RNG_LR_F calls (KAT-preserving). It is always set:
// the cat1 avx2 build uses a batched kernel (4 keys x 2 blocks = 8 cipher blocks run
// interleaved, saturating the AES-NI port); every other build uses a naive wrapper that
// just does the four single calls, so the loop can always call this with no branch.
typedef void GGM_EXTSEED_RNG_LR_X4_F(void* lr_out, const void* salt, const void* extended_seeds, uint64_t node_idx0, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat1_aes128_x4_avx2(  // batched kernel
    void* lr_out, const void* salt128, const void* extkeys4, uint64_t node_idx0, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat1_aes128_x4_ref(  // naive four calls
    void* lr_out, const void* salt128, const void* extkeys4, uint64_t node_idx0, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat3_rijndael256_x4_ref(  // naive four calls
    void* lr_out, const void* salt192, const void* extkeys4, uint64_t node_idx0, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat3_rijndael256_x4_avx2(  // naive four calls
    void* lr_out, const void* salt192, const void* extkeys4, uint64_t node_idx0, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat5_rijndael256_x4_ref(  // naive four calls
    void* lr_out, const void* salt256, const void* extkeys4, uint64_t node_idx0, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat5_rijndael256_x4_avx2(  // naive four calls
    void* lr_out, const void* salt256, const void* extkeys4, uint64_t node_idx0, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat1_aes128_ref(  //
    void* lr_out256, const void* salt128, const void* extkey128, uint64_t node_idx, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat1_aes128_avx2(  //
    void* lr_out256, const void* salt128, const void* extkey128, uint64_t node_idx, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat3_rijndael256_ref(  //
    void* lr_out384, const void* salt192, const void* extkey, uint64_t node_idx, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat3_rijndael256_avx2(  //
    void* lr_out384, const void* salt192, const void* extkey, uint64_t node_idx, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat5_rijndael256_ref(  //
    void* lr_out512, const void* salt256, const void* extkey, uint64_t node_idx, uint32_t theta);
EXPORT void ggm_seed_rng_lr_ext_cat5_rijndael256_avx2(  //
    void* lr_out512, const void* salt256, const void* extkey, uint64_t node_idx, uint32_t theta);


/**
 * Extended leaf seed: the AES/Rijndael key schedule is run once per leaf, and
 * the resulting "extended seed" is shared by both the commit rng and the vole
 * rng (saving the second, redundant key schedule). For aes/rijndael the
 * extended seed is the expanded round keys; for shake it is just the raw seed.
 *
 *  - EXTEND_LEAF_SEED_F turns a raw node_seed into an extended_seed.
 *  - EXTENDED_NODE_SEED_BYTES_F returns the byte size of the extended seed.
 *  - the ggm_commit_rng_ext_* / vole_rng_ext_* variants consume the extended
 *    seed (key/node_seed points at the round keys instead of the raw seed).
 *
 * The original ggm_commit_rng_* / vole_rng_* symbols are retained as raw-seed
 * shims (extend + ext), so callers passing a raw seed still get identical bytes.
 */
typedef void EXTEND_LEAF_SEED_F(void* extended_seed_out, const void* node_seed);
typedef uint64_t EXTENDED_NODE_SEED_BYTES_F(void);

/**
 * 4x leaf-seed expansion: extend four contiguous node_seeds into four contiguous
 * extended_seeds in one call, for the BFS ggm traversal. Inputs and outputs are
 * packed at their natural stride (node_seed size in, extended_seed size out).
 * Output is byte-identical to calling EXTEND_LEAF_SEED_F four times, so it is
 * KAT-preserving. The _ref variants call the 1x function four times; the _avx2
 * variants dispatch to the batched key-schedule kernels (aes128 x4, rijndael x2).
 */
typedef void EXTEND_LEAF_SEED_4X_F(void* extended_seed_out, const void* node_seed);

EXPORT void extend_leaf_seed_cat1_aes128_ref(void* extended_seed_out, const void* seed128);
EXPORT void extend_leaf_seed_cat1_aes128_avx2(void* extended_seed_out, const void* seed128);
EXPORT void extend_leaf_seed_cat3_rijndael256_ref(void* extended_seed_out, const void* seed192);
EXPORT void extend_leaf_seed_cat3_rijndael256_avx2(void* extended_seed_out, const void* seed192);
EXPORT void extend_leaf_seed_cat5_rijndael256_ref(void* extended_seed_out, const void* seed256);
EXPORT void extend_leaf_seed_cat5_rijndael256_avx2(void* extended_seed_out, const void* seed256);

EXPORT void extend_leaf_seed_cat1_aes128_4x_ref(void* extended_seed_out, const void* seeds);
EXPORT void extend_leaf_seed_cat1_aes128_4x_avx2(void* extended_seed_out, const void* seeds);
EXPORT void extend_leaf_seed_cat3_rijndael256_4x_ref(void* extended_seed_out, const void* seeds);
EXPORT void extend_leaf_seed_cat3_rijndael256_4x_avx2(void* extended_seed_out, const void* seeds);
EXPORT void extend_leaf_seed_cat5_rijndael256_4x_ref(void* extended_seed_out, const void* seeds);
EXPORT void extend_leaf_seed_cat5_rijndael256_4x_avx2(void* extended_seed_out, const void* seeds);

EXPORT uint64_t extended_node_seed_bytes_cat1_aes128(void);
EXPORT uint64_t extended_node_seed_bytes_cat3_rijndael256(void);
EXPORT uint64_t extended_node_seed_bytes_cat5_rijndael256(void);

// commit rng, extended-seed variants (key points at the expanded round keys).
// The masked salt produced by compute_tweaked_salts reserves the low
// GGM_TWEAK_BITS bits for the tweak, so theta is that constant and is not an argument.
typedef void GGM_EXTSEED_RNG_COMMIT_F(
  void* out,
  const void* salt,
  const void* extended_seed,
  uint64_t node_idx);
EXPORT void ggm_commit_rng_ext_cat1_aes128_ref(void* output256, const void* salt128, const void* extkey128,
                                               uint64_t node_idx);
EXPORT void ggm_commit_rng_ext_cat1_aes128_avx2(void* output256, const void* salt128, const void* extkey128,
                                                uint64_t node_idx);
EXPORT void ggm_commit_rng_ext_cat3_rijndael256_ref(void* output384, const void* salt192, const void* extkey,
                                                    uint64_t node_idx);
EXPORT void ggm_commit_rng_ext_cat3_rijndael256_avx2(void* output384, const void* salt192, const void* extkey,
                                                     uint64_t node_idx);
EXPORT void ggm_commit_rng_ext_cat5_rijndael256_ref(void* output512, const void* salt256, const void* extkey,
                                                    uint64_t node_idx);
EXPORT void ggm_commit_rng_ext_cat5_rijndael256_avx2(void* output512, const void* salt256, const void* extkey,
                                                     uint64_t node_idx);

// vole rng, extended-seed variants (node_seed points at the expanded round keys).
//
// The CTR stream starts at the tweaked salt of the repetition: the lambda-bit
// initial counter is
//     vole_masked_salt | (repet_idx << VOLE_RNG_REPET_SHIFT)
// (vole_masked_salt comes from compute_tweaked_salts and has its low
// GGM_TWEAK_BITS bits cleared, so the OR is a plain field write). The block
// counter therefore lives in the low VOLE_RNG_REPET_SHIFT bits, which bounds a
// single call to 2^VOLE_RNG_REPET_SHIFT blocks; debug builds assert it via
// domain_sep_check so the last block can never spill into the next repetition.
typedef void VOLE_EXTSEED_RNG_F(void* out, uint64_t out_bytes, const void* vole_masked_salt,
                                const void* extended_seed, uint64_t repet_idx);
EXPORT void vole_rng_ext_cat1_aes128_ctrle_ref(void* out, uint64_t out_bytes, const void* salt128,
                                               const void* extseed, uint64_t repet_idx);
EXPORT void vole_rng_ext_cat1_aes128_ctrle_nocarry_avx2(void* out, uint64_t out_bytes, const void* salt128,
                                                        const void* extseed, uint64_t repet_idx);

EXPORT void vole_rng_ext_cat3_rijndael256_ctrle_ref(void* out, uint64_t out_bytes, const void* salt192,
                                                    const void* extseed, uint64_t repet_idx);
EXPORT void vole_rng_ext_cat3_rijndael256_ctrle_avx2(void* out, uint64_t out_bytes, const void* salt192,
                                                     const void* extseed, uint64_t repet_idx);
EXPORT void vole_rng_ext_cat5_rijndael256_ctrle_ref(void* out, uint64_t out_bytes, const void* salt256,
                                                    const void* extseed, uint64_t repet_idx);
EXPORT void vole_rng_ext_cat5_rijndael256_ctrle_avx2(void* out, uint64_t out_bytes, const void* salt256,
                                                     const void* extseed, uint64_t repet_idx);

typedef struct xof_ctx_t {
  uint64_t DUMMY[224 / 8];
} xof_ctx;

typedef void XOF_INIT_F(xof_ctx* xof);
EXPORT void xof_init_shake128(xof_ctx* xof);
EXPORT void xof_init_shake256(xof_ctx* xof);

typedef void XOF_SEED_F(xof_ctx* xof, const void* in, uint64_t in_bytes);
EXPORT void xof_seed_shake128(xof_ctx* xof, const void* in, uint64_t in_bytes);
EXPORT void xof_seed_shake256(xof_ctx* xof, const void* in, uint64_t in_bytes);

typedef void XOF_FINALIZE_F(xof_ctx* xof);
EXPORT void xof_finalize_shake128(xof_ctx* xof);
EXPORT void xof_finalize_shake256(xof_ctx* xof);

typedef void XOF_OUTPUT_F(xof_ctx* xof, void* out, uint64_t out_bytes);
EXPORT void xof_output_shake128(xof_ctx* xof, void* out, uint64_t out_bytes);
EXPORT void xof_output_shake256(xof_ctx* xof, void* out, uint64_t out_bytes);

// shortcut combo
typedef void XOF_INIT_AND_SEED_F(xof_ctx* xof, const void* in, uint64_t in_bytes);
EXPORT void xof_init_and_seed_shake128(xof_ctx* xof, const void* in, uint64_t in_bytes);
EXPORT void xof_init_and_seed_shake256(xof_ctx* xof, const void* in, uint64_t in_bytes);

typedef void XOF_FINALIZE_AND_OUTPUT_F(xof_ctx* xof, void* out, uint64_t out_bytes);
EXPORT void xof_finalize_and_output_shake128(xof_ctx* xof, void* out, uint64_t out_bytes);
EXPORT void xof_finalize_and_output_shake256(xof_ctx* xof, void* out, uint64_t out_bytes);

typedef struct xof_functions_t {
  XOF_INIT_F* xof_init;
  XOF_SEED_F* xof_seed;
  XOF_FINALIZE_F* xof_finalize;
  XOF_OUTPUT_F* xof_output;
  XOF_INIT_AND_SEED_F* xof_init_and_seed;
  XOF_FINALIZE_AND_OUTPUT_F* xof_finalize_and_output;
} xof_functions;

static const xof_functions xof_shake128 = {
    xof_init_shake128,                //
    xof_seed_shake128,                //
    xof_finalize_shake128,            //
    xof_output_shake128,              //
    xof_init_and_seed_shake128,       //
    xof_finalize_and_output_shake128  //
};
static const xof_functions xof_shake256 = {
    xof_init_shake256,                //
    xof_seed_shake256,                //
    xof_finalize_shake256,            //
    xof_output_shake256,              //
    xof_init_and_seed_shake256,       //
    xof_finalize_and_output_shake256  //
};

typedef struct xof_vector_ctx_t xof_vector_ctx;

typedef uint64_t XOF_VECTOR_CTX_BYTES_F(uint64_t num_xofs);
EXPORT uint64_t xof_vector_ctx_bytes_shake128(uint64_t num_xofs);
EXPORT uint64_t xof_vector_ctx_bytes_shake256(uint64_t num_xofs);

typedef void XOF_VECTOR_INIT_AND_SEED_F(xof_vector_ctx* xof, uint64_t num_xofs, const uint8_t* data, uint64_t data_bytes);
EXPORT void xof_vector_init_and_seed_shake128(xof_vector_ctx* xof, uint64_t num_xofs, const uint8_t* data, uint64_t data_bytes);
EXPORT void xof_vector_init_and_seed_shake256(xof_vector_ctx* xof, uint64_t num_xofs, const uint8_t* data, uint64_t data_bytes);

typedef void XOF_VECTOR_SEED_F(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** data, uint64_t data_bytes);
EXPORT void xof_vector_seed_shake128(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** data, uint64_t data_bytes);
EXPORT void xof_vector_seed_shake256(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** data, uint64_t data_bytes);

typedef void XOF_VECTOR_FINALIZE_AND_OUTPUT_F(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** out_hashes, uint64_t hash_bytes);
EXPORT void xof_vector_finalize_and_output_shake128(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** out_hashes, uint64_t hash_bytes);
EXPORT void xof_vector_finalize_and_output_shake256(xof_vector_ctx* xof, uint64_t num_xofs, uint8_t** out_hashes, uint64_t hash_bytes);

typedef struct xof_vector_functions_t {
  XOF_VECTOR_CTX_BYTES_F* ctx_bytes;
  XOF_VECTOR_INIT_AND_SEED_F* init_and_seed;
  XOF_VECTOR_SEED_F* seed;
  XOF_VECTOR_FINALIZE_AND_OUTPUT_F* finalize_and_output;
} xof_vector_functions;

static const xof_vector_functions xof_vector_shake128 = {
  xof_vector_ctx_bytes_shake128,                //
  xof_vector_init_and_seed_shake128,                //
  xof_vector_seed_shake128,            //
  xof_vector_finalize_and_output_shake128 //
};

static const xof_vector_functions xof_vector_shake256 = {
  xof_vector_ctx_bytes_shake256,                //
  xof_vector_init_and_seed_shake256,                //
  xof_vector_seed_shake256,            //
  xof_vector_finalize_and_output_shake256 //
};


typedef struct matrix_rng_t {
  uint8_t DUMMY[1024]; // large enough t
} __attribute((aligned(32))) matrix_rng_t;

typedef void MATRIX_RNG_INIT(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size);
EXPORT void matrix_rng_init_aes128_cat1_ref(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size);
EXPORT void matrix_rng_init_aes128_cat1_avx(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size);
EXPORT void matrix_rng_init_rijndael256_cat3_ref(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size);
EXPORT void matrix_rng_init_rijndael256_cat3_avx(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size);
EXPORT void matrix_rng_init_rijndael256_cat5_ref(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size);
EXPORT void matrix_rng_init_rijndael256_cat5_avx(matrix_rng_t* rng, const seed_t* seed, const uint64_t row_bit_size);

typedef void MATRIX_RNG_GET_ROW(const matrix_rng_t* rng, void* out, const uint64_t row_index);
EXPORT void matrix_rng_get_row_aes128_cat1_ref(const matrix_rng_t* rng, void* out, const uint64_t row_index);
EXPORT void matrix_rng_get_row_aes128_cat1_avx(const matrix_rng_t* rng, void* out, const uint64_t row_index);
EXPORT void matrix_rng_get_row_rijndael256_cat3_ref(const matrix_rng_t* rng, void* out, const uint64_t row_index);
EXPORT void matrix_rng_get_row_rijndael256_cat3_avx(const matrix_rng_t* rng, void* out, const uint64_t row_index);
EXPORT void matrix_rng_get_row_rijndael256_cat5_ref(const matrix_rng_t* rng, void* out, const uint64_t row_index);
EXPORT void matrix_rng_get_row_rijndael256_cat5_avx(const matrix_rng_t* rng, void* out, const uint64_t row_index);

// Batched counterpart of MATRIX_RNG_GET_ROW: draws the nrows consecutive rows
// first_row, first_row+1, ..., first_row+nrows-1 and writes them back to back in
// out (no gap: the stride is the natural row size bpr*block_bytes, which is the
// rsd_codim_slice of compute_rsd_codim_slice). Because row i is the bpr cipher
// blocks at counter i*bpr, a run of consecutive rows is one contiguous stretch
// of the CTR keystream, so the whole batch is a single cipher call. Output is
// byte-identical to nrows successive get_row calls (KAT-preserving); the point
// is to amortise the per-row call and counter-setup glue, which costs as much as
// the AES itself at these row sizes.
typedef void MATRIX_RNG_GET_ROWS(const matrix_rng_t* rng, void* out, const uint64_t first_row, const uint64_t nrows);
EXPORT void matrix_rng_get_rows_aes128_cat1_ref(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                const uint64_t nrows);
EXPORT void matrix_rng_get_rows_aes128_cat1_avx(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                const uint64_t nrows);
EXPORT void matrix_rng_get_rows_rijndael256_cat3_ref(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                     const uint64_t nrows);
EXPORT void matrix_rng_get_rows_rijndael256_cat3_avx(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                     const uint64_t nrows);
EXPORT void matrix_rng_get_rows_rijndael256_cat5_ref(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                     const uint64_t nrows);
EXPORT void matrix_rng_get_rows_rijndael256_cat5_avx(const matrix_rng_t* rng, void* out, const uint64_t first_row,
                                                     const uint64_t nrows);

// Fused row generation + challenge multiplication.
//
// rows_times_chall computes dest[i] = <row(first_row+i), chall> for nrows
// consecutive rows, without ever materialising the rows. Generating a row and
// reducing it against the challenge use disjoint execution ports (the cipher
// rounds vs the carry-less multiplier), so fusing them into one loop hides the
// whole field reduction behind the cipher; splitting it across two calls, as a
// get_rows + flambda_dot_product pair must, makes the two costs additive.
//
// The challenge is passed pre-processed. Writing k = lambda/64 and splitting a
// row into 64-bit words h_j, the definition <H_i, chall_i> = sum_i H_i * chall_i
// rearranges into sum_j h_j * prep_chall[j] with
//     prep_chall[j] = chall[j / k] * X^(64 * (j % k)),
// which is what preprocess_chall builds (once per challenge, reused for every
// row). Each h_j is then a field element with only its low 64 bits populated, so
// each term is a 64-by-lambda multiply: fewer accumulators than the balanced
// Karatsuba form, and the accumulated high part is only 64 bits, so the final
// reduction is a single carry-less multiply instead of a chained fold. It also
// shortens the vector: prep_chall has ceil(row_bits/64) entries rather than the
// ceil(row_bits/lambda)*k words the limb form has to pad up to.
//
// Both take the row bit size from the rng context (stored by init), so the
// pre-processed challenge and the kernel can never disagree on its length.
// dest holds nrows field elements; prep_chall holds matrix_rng_prep_chall_nelems
// of them. Output is identical to the get_rows + flambda_dot_product pair.
typedef void MATRIX_RNG_PREPROCESS_CHALL(const matrix_rng_t* rng, flambda_t* prep_chall, const flambda_t* chall);
typedef void MATRIX_RNG_ROWS_TIMES_CHALL(const matrix_rng_t* rng, flambda_t* dest, const flambda_t* prep_chall,
                                         const uint64_t first_row, const uint64_t nrows);

/** number of field elements in the pre-processed challenge, for a given row bit size */
static inline uint64_t matrix_rng_prep_chall_nelems(const uint64_t row_bit_size) { return (row_bit_size + 63) >> 6; }

EXPORT void matrix_rng_preprocess_chall_aes128_cat1_ref(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                        const flambda_t* chall);
EXPORT void matrix_rng_preprocess_chall_aes128_cat1_avx(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                        const flambda_t* chall);
EXPORT void matrix_rng_rows_times_chall_aes128_cat1_ref(const matrix_rng_t* rng, flambda_t* dest,
                                                        const flambda_t* prep_chall, const uint64_t first_row,
                                                        const uint64_t nrows);
EXPORT void matrix_rng_rows_times_chall_aes128_cat1_avx(const matrix_rng_t* rng, flambda_t* dest,
                                                        const flambda_t* prep_chall, const uint64_t first_row,
                                                        const uint64_t nrows);
EXPORT void matrix_rng_preprocess_chall_rijndael256_cat3_ref(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                             const flambda_t* chall);
EXPORT void matrix_rng_preprocess_chall_rijndael256_cat3_avx(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                             const flambda_t* chall);
EXPORT void matrix_rng_rows_times_chall_rijndael256_cat3_ref(const matrix_rng_t* rng, flambda_t* dest,
                                                             const flambda_t* prep_chall, const uint64_t first_row,
                                                             const uint64_t nrows);
EXPORT void matrix_rng_rows_times_chall_rijndael256_cat3_avx(const matrix_rng_t* rng, flambda_t* dest,
                                                             const flambda_t* prep_chall, const uint64_t first_row,
                                                             const uint64_t nrows);
EXPORT void matrix_rng_preprocess_chall_rijndael256_cat5_ref(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                             const flambda_t* chall);
EXPORT void matrix_rng_preprocess_chall_rijndael256_cat5_avx(const matrix_rng_t* rng, flambda_t* prep_chall,
                                                             const flambda_t* chall);
EXPORT void matrix_rng_rows_times_chall_rijndael256_cat5_ref(const matrix_rng_t* rng, flambda_t* dest,
                                                             const flambda_t* prep_chall, const uint64_t first_row,
                                                             const uint64_t nrows);
EXPORT void matrix_rng_rows_times_chall_rijndael256_cat5_avx(const matrix_rng_t* rng, flambda_t* dest,
                                                             const flambda_t* prep_chall, const uint64_t first_row,
                                                             const uint64_t nrows);

// A matrix prng: init once from a seed for a fixed row bit size, then draw any
// row independently via get_row (row i = the ceil(row_bits/lambda) cipher blocks
// at counter i*bpr, with bits beyond row_bits masked to zero).
//
// Every entry is mandatory: each category has both a ref and an avx
// implementation of the fused rows_times_chall, so callers use it unconditionally
// rather than falling back to the get_rows + flambda_dot_product pair.
typedef struct matrix_rng_functions_t {
  MATRIX_RNG_INIT* matrix_rng_init;
  MATRIX_RNG_GET_ROW* matrix_rng_get_row;
  MATRIX_RNG_GET_ROWS* matrix_rng_get_rows;
  MATRIX_RNG_PREPROCESS_CHALL* matrix_rng_preprocess_chall;
  MATRIX_RNG_ROWS_TIMES_CHALL* matrix_rng_rows_times_chall;
} matrix_rng_functions;

static const matrix_rng_functions matrix_rng_cat1_aes128_ref = {
    matrix_rng_init_aes128_cat1_ref,              //
    matrix_rng_get_row_aes128_cat1_ref,           //
    matrix_rng_get_rows_aes128_cat1_ref,          //
    matrix_rng_preprocess_chall_aes128_cat1_ref,  //
    matrix_rng_rows_times_chall_aes128_cat1_ref,  //
};
static const matrix_rng_functions matrix_rng_cat3_rijndael256_ref = {
    matrix_rng_init_rijndael256_cat3_ref,              //
    matrix_rng_get_row_rijndael256_cat3_ref,           //
    matrix_rng_get_rows_rijndael256_cat3_ref,          //
    matrix_rng_preprocess_chall_rijndael256_cat3_ref,  //
    matrix_rng_rows_times_chall_rijndael256_cat3_ref,  //
};
static const matrix_rng_functions matrix_rng_cat5_rijndael256_ref = {
    matrix_rng_init_rijndael256_cat5_ref,              //
    matrix_rng_get_row_rijndael256_cat5_ref,           //
    matrix_rng_get_rows_rijndael256_cat5_ref,          //
    matrix_rng_preprocess_chall_rijndael256_cat5_ref,  //
    matrix_rng_rows_times_chall_rijndael256_cat5_ref,  //
};
// ONLY_REF_IMPLEMENTATION drops the *_avx2.c translation units, so these tables
// would hold undefined references: they are only compiled with the avx2 backend.
#if defined(__x86_64__) && !defined(ONLY_REF_IMPLEMENTATION)
static const matrix_rng_functions matrix_rng_cat1_aes128_avx = {
    matrix_rng_init_aes128_cat1_avx,              //
    matrix_rng_get_row_aes128_cat1_avx,           //
    matrix_rng_get_rows_aes128_cat1_avx,          //
    matrix_rng_preprocess_chall_aes128_cat1_avx,  //
    matrix_rng_rows_times_chall_aes128_cat1_avx,  //
};
static const matrix_rng_functions matrix_rng_cat3_rijndael256_avx = {
    matrix_rng_init_rijndael256_cat3_avx,              //
    matrix_rng_get_row_rijndael256_cat3_avx,           //
    matrix_rng_get_rows_rijndael256_cat3_avx,          //
    matrix_rng_preprocess_chall_rijndael256_cat3_avx,  //
    matrix_rng_rows_times_chall_rijndael256_cat3_avx,  //
};
static const matrix_rng_functions matrix_rng_cat5_rijndael256_avx = {
    matrix_rng_init_rijndael256_cat5_avx,              //
    matrix_rng_get_row_rijndael256_cat5_avx,           //
    matrix_rng_get_rows_rijndael256_cat5_avx,          //
    matrix_rng_preprocess_chall_rijndael256_cat5_avx,  //
    matrix_rng_rows_times_chall_rijndael256_cat5_avx,  //
};
#endif

// The keygen prng: the secret-key seed expanded into the little-endian uint32
// stream the rsd position sampler draws from. It is a plain block-cipher CTR
// keystream with a zero IV, nothing more:
//   cat1: aes128, key = the 16-byte seed
//   cat3: rijndael256, key = the 24-byte seed padded with 64 zero bits in the MSB
//   cat5: rijndael256, key = the 32-byte seed
// The counter is little-endian over the low 64 bits (the same no-carry CTR the
// rest of the prngs use), so block i is enc(i) and the stream is the blocks
// concatenated. lambda/8 is a multiple of 4 for every category, so a draw never
// straddles a block boundary.
typedef struct keygen_rng_ctx_t {
  uint8_t DUMMY[1024];  // large enough for any category
} __attribute((aligned(32))) keygen_rng_ctx;

typedef void KEYGEN_RNG_INIT(keygen_rng_ctx* rng, const seed_t* sk_seed);
EXPORT void keygen_rng_init_aes128_cat1_ref(keygen_rng_ctx* rng, const seed_t* sk_seed);
EXPORT void keygen_rng_init_aes128_cat1_avx(keygen_rng_ctx* rng, const seed_t* sk_seed);
EXPORT void keygen_rng_init_rijndael256_cat3_ref(keygen_rng_ctx* rng, const seed_t* sk_seed);
EXPORT void keygen_rng_init_rijndael256_cat3_avx(keygen_rng_ctx* rng, const seed_t* sk_seed);
EXPORT void keygen_rng_init_rijndael256_cat5_ref(keygen_rng_ctx* rng, const seed_t* sk_seed);
EXPORT void keygen_rng_init_rijndael256_cat5_avx(keygen_rng_ctx* rng, const seed_t* sk_seed);

typedef uint32_t KEYGEN_RNG_NEXT_U32(keygen_rng_ctx* rng);
EXPORT uint32_t keygen_rng_next_u32_aes128_cat1_ref(keygen_rng_ctx* rng);
EXPORT uint32_t keygen_rng_next_u32_aes128_cat1_avx(keygen_rng_ctx* rng);
// cat3 and cat5 only differ in the key they are initialised with, so they share
// their draw function: the rijndael256 one.
EXPORT uint32_t keygen_rng_next_u32_rijndael256_ref(keygen_rng_ctx* rng);
EXPORT uint32_t keygen_rng_next_u32_rijndael256_avx(keygen_rng_ctx* rng);

typedef struct keygen_rng_functions_t {
  KEYGEN_RNG_INIT* init;
  KEYGEN_RNG_NEXT_U32* next_u32;
} keygen_rng_functions;

static const keygen_rng_functions keygen_rng_cat1_aes128_ref = {
    keygen_rng_init_aes128_cat1_ref,      //
    keygen_rng_next_u32_aes128_cat1_ref,  //
};
static const keygen_rng_functions keygen_rng_cat3_rijndael256_ref = {
    keygen_rng_init_rijndael256_cat3_ref,  //
    keygen_rng_next_u32_rijndael256_ref,   //
};
static const keygen_rng_functions keygen_rng_cat5_rijndael256_ref = {
    keygen_rng_init_rijndael256_cat5_ref,  //
    keygen_rng_next_u32_rijndael256_ref,   //
};
#if defined(__x86_64__) && !defined(ONLY_REF_IMPLEMENTATION)
static const keygen_rng_functions keygen_rng_cat1_aes128_avx = {
    keygen_rng_init_aes128_cat1_avx,      //
    keygen_rng_next_u32_aes128_cat1_avx,  //
};
static const keygen_rng_functions keygen_rng_cat3_rijndael256_avx = {
    keygen_rng_init_rijndael256_cat3_avx,  //
    keygen_rng_next_u32_rijndael256_avx,   //
};
static const keygen_rng_functions keygen_rng_cat5_rijndael256_avx = {
    keygen_rng_init_rijndael256_cat5_avx,  //
    keygen_rng_next_u32_rijndael256_avx,   //
};
#endif

#endif  // AQ_VOLE_IMPL_RNG_H
