#ifndef RNG_PRIVATE_H
#define RNG_PRIVATE_H

#include <string.h>

#include "aes128_ctrle.h"  // ctr128_t (also pulls in rijndael256_ctrle.h -> ctr256_t)
#include "rijndael256_ctrle.h"
#include "sdith_prng.h"
#include "vole_private.h"

// union that represents an aes128 little endian counter
typedef union {
  __uint128_t u128;
  uint64_t v64[2];
} aesblk;

/**
 * @brief Domain-separated CTR plaintext, low 64 bits (round 3).
 * will compute: MapToBitsLE(tweak, theta) || salt[theta:salt_bitsize]
 */
static inline void domain_sep_ptx_lo2( //
  uint64_t* const outctr,  // aesctr 32-bytes aligned
  const void* const masked_salt, const uint64_t salt_bytes,  // salt [no specific alignment]
  const uint64_t tweak)
{
  memcpy(outctr, masked_salt, salt_bytes);
  outctr[0] |= tweak;
}

#ifdef __x86_64__
#include <immintrin.h>

/**
 * @brief avx2 form of domain_sep_ptx_lo2, for a 128-bit CTR block.
 *
 * Same plaintext, built in a vector register: the caller stores it once with a
 * single aligned 16-byte store, which is exactly the width the CTR kernel loads
 * it back at. Going through memory in two 8-byte pieces instead (what the
 * memcpy + scalar-or pair compiles to) makes the kernel's 16-byte load overlap
 * two narrower stores, which costs a store-forwarding stall on every call --
 * about as much as encrypting five extra blocks.
 */
static inline __m128i domain_sep_ptx_lo2_m128( //
  const void* const masked_salt,  // salt [no specific alignment]
  const uint64_t tweak)
{
  return _mm_or_si128(_mm_loadu_si128((const __m128i*)masked_salt),
                      _mm_cvtsi64_si128((long long)tweak));
}

/**
 * @brief 256-bit CTR block held as the two halves the rijndael256 kernels read.
 * Those kernels take the block as two 16-byte loads, so keeping it in two
 * registers lets the caller answer them with two matching 16-byte stores.
 */
typedef struct {
  __m128i lo;
  __m128i hi;
} ctr256_vec;

/** @brief 192-bit salt in a 256-bit block: the 8 bytes above the salt stay zero. */
static inline ctr256_vec domain_sep_ptx_lo2_m256_from192( //
  const void* const masked_salt,  // 24-byte salt [no specific alignment]
  const uint64_t tweak)
{
  const uint8_t* const s = (const uint8_t*)masked_salt;
  uint64_t hi64;
  memcpy(&hi64, s + 16, 8);
  ctr256_vec res = {domain_sep_ptx_lo2_m128(s, tweak), _mm_cvtsi64_si128((long long)hi64)};
  return res;
}

/** @brief 256-bit salt filling the whole 256-bit block. */
static inline ctr256_vec domain_sep_ptx_lo2_m256( //
  const void* const masked_salt,  // 32-byte salt [no specific alignment]
  const uint64_t tweak)
{
  const uint8_t* const s = (const uint8_t*)masked_salt;
  ctr256_vec res = {domain_sep_ptx_lo2_m128(s, tweak), _mm_loadu_si128((const __m128i*)(s + 16))};
  return res;
}
#endif  // __x86_64__

/**
 * @brief Parameters sanity check:
 * ensures that the tweak remains bounded by 2^theta and that the salt has its first
 * theta bits to zero.
 * These checks are meant to run in DEBUG mode only to catch bugs in the source code or
 * in the parametrization of the signature,
 * These checks do not need to run in production (calling in CASSERT is ok).
 */
static inline int domain_sep_check( //
  const void* salt,
  uint64_t tweak,
  uint64_t nblocks,
  uint64_t theta)
{
  uint64_t THETA_MASK = (UINT64_C(1) << theta) - 1;
  uint64_t in_salt;
  memcpy(&in_salt, salt, 8);
  // check that the input salt and the mask are disjoint
  if ((in_salt & THETA_MASK) != 0) return 0;
  // check that the tweak won't overflow
  if ((tweak & ~THETA_MASK) != 0) return 0;
  if (((tweak + nblocks - 1) & ~THETA_MASK) != 0) return 0;
  return 1;
}

// I8: define a naive x4 LR expansion (four single PERNODE calls on parents
// node_idx0, +2, +4, +6). Used where there is no batched kernel (the ref build,
// and cat3/cat5 avx2). Byte-identical to the batched cat1 kernel, so the tree
// loop can always call the x4 pointer with no branch. LR_STRIDE = 2 * seed bytes
// (the 2-seed output per node), EXT_STRIDE = one extended key.
#define GGM_EXTSEED_RNG_LR_X4_NAIVE(NAME, PERNODE, LR_STRIDE, EXT_STRIDE)                          \
  EXPORT void NAME(void* lr_out, const void* salt, const void* extended_seeds, uint64_t node_idx0, \
                   uint32_t theta) {                                                               \
    uint8_t* o = (uint8_t*)lr_out;                                                                 \
    const uint8_t* e = (const uint8_t*)extended_seeds;                                             \
    for (int i = 0; i < 4; i++) {                                                                  \
      PERNODE(o + (uint64_t)i * (LR_STRIDE), salt, e + (uint64_t)i * (EXT_STRIDE),                 \
              node_idx0 + 2 * i, theta);                                                           \
    }                                                                                              \
  }


union proofow_ctr128_t {
  uint8_t v8[16];
  uint64_t v64[2];
} __attribute__((aligned(16)));

struct proofow_state128_t {
  union proofow_ctr128_t p[2]; // aes128 plaintext
  union proofow_ctr128_t k[2]; // aes128 key
  union proofow_ctr128_t c[2]; // aes128 ciphertext
  union proofow_ctr128_t rk[2][11]; // 11 round keys of aes128
  uint8_t h_piop[32];
  uint64_t delta0_out_bytes;
  uint64_t mask_w;
} __attribute__((aligned(32)));

union proofow_ctr256_t {
  uint8_t v8[32];
  uint64_t v64[4];
} __attribute__((aligned(32)));

struct proofow_state256_t {
  union proofow_ctr256_t p[2]; // rijndael256 plaintext
  union proofow_ctr256_t k[2]; // rijndael256 key
  union proofow_ctr256_t c[2]; // rijndael256 ciphertext
  union proofow_ctr256_t rk[2][15]; // 15 round keys of rijndael256 (RIJNDAEL256_RK_BYTES each)
  uint8_t h_piop[64];
  uint64_t delta0_out_bytes;
  uint64_t mask_w;
} __attribute__((aligned(32)));

struct proofow_shake_cat1_t {
  uint8_t delta0_and_vgrind[32]; // at least 16 + w
  uint8_t h_piop[32];
  uint64_t delta0_and_vgrind_bytes;
  uint64_t delta0_out_bytes;
  uint64_t w;
  uint64_t kappa_tau;
} __attribute__((aligned(32)));

struct proofow_shake_cat3_t {
  uint8_t delta0_and_vgrind[32]; // at least 24 + w
  uint8_t h_piop[48];
  uint64_t delta0_and_vgrind_bytes;
  uint64_t delta0_out_bytes;
  uint64_t w;
  uint64_t kappa_tau;
} __attribute__((aligned(32)));

struct proofow_shake_cat5_t {
  uint8_t delta0_and_vgrind[64]; // at least 32 + w
  uint8_t h_piop[64];
  uint64_t delta0_and_vgrind_bytes;
  uint64_t delta0_out_bytes;
  uint64_t w;
  uint64_t kappa_tau;
} __attribute__((aligned(32)));

struct matrix_rng_aes128_cat1_t {
  ctr128_t rk[11];         // round keys (11 blocks for aes128)
  ctr128_t last_blk_mask;  // AND mask to apply to the last PRG block
  uint64_t blocks_per_row; // number of PRG blocks to generate per row
  uint64_t row_bit_size;   // bits per row (bits beyond it are masked off)
} __attribute__((aligned(32)));  //

struct matrix_rng_rijndael256_cat3_t {
  ctr256_t rk[15];         // round keys (15 blocks for rijndael256)
  ctr256_t last_blk_mask;  // AND mask to apply to the last (256-bit) PRG block
  uint64_t blocks_per_row; // number of PRG blocks to generate per row
  uint64_t row_bit_size;   // bits per row (bits beyond it are masked off)
} __attribute__((aligned(32)));  //

struct matrix_rng_rijndael256_cat5_t {
  ctr256_t rk[15];         // round keys (15 blocks for rijndael256)
  ctr256_t last_blk_mask;  // AND mask to apply to the last PRG block
  uint64_t blocks_per_row; // number of PRG blocks to generate per row
  uint64_t row_bit_size;   // bits per row (bits beyond it are masked off)
} __attribute__((aligned(32)));  //

// Fills res[0..res_size) with a little-endian bitmask keeping the low b bits.
// Special case: b == 0 means the last PRG block is entirely used (the row bit
// size is a multiple of the block bit size), so every bit is kept (all ones).
// Shared by the ref and avx matrix_rng init (mask setup is scalar either way).
static inline void set_2_b_minus_one(uint64_t* res, uint64_t res_size, uint64_t b) {
  if (b == 0) {
    for (uint64_t i=0; i<res_size; ++i) res[i] = UINT64_C(-1);
    return;
  }
  for (uint64_t i=0; i<res_size; ++i) {
    if (b >= 64) {
      res[i] = UINT64_C(-1);
      b -= 64;
    } else {
      res[i] = (UINT64_C(1) << b) - 1;
      b = 0;
    }
  }
}

// Body of MATRIX_RNG_PREPROCESS_CHALL, parameterised by the field. With
// k = lambda/64, it builds prep_chall[j] = chall[j / k] * X^(64 * (j % k)).
// Shared by the ref and avx matrix_rng so the two tables can never disagree;
// only the product differs (ref bit loop vs pclmul).
static inline void preprocess_chall_generic(flambda_t* prep_chall, const flambda_t* chall, const uint64_t nelems,
                                            const uint64_t lambda_bytes, const uint64_t k,
                                            FLAMBDA_PRODUCT_F* product) {
  uint8_t* const out = (uint8_t*) prep_chall;
  const uint8_t* const in = (const uint8_t*) chall;
  for (uint64_t j = 0; j < nelems; ++j) {
    // X^(64*m) as a field element: the single bit at position 64*m
    flambda_max_t xpow = {0, 0, 0, 0};
    xpow[j % k] = 1;
    product(out + j * lambda_bytes, in + (j / k) * lambda_bytes, xpow);
  }
}

// --- keygen prng (see keygen_rng_ctx in sdith_prng.h) -----------------------
//
// A pure CTR keystream read as little-endian uint32s. The blocks are produced a
// few at a time rather than one by one: the keystream is contiguous, so a batch
// is a single cipher call and the output is unchanged, but the per-call counter
// setup and (on avx2) the pipeline fill are amortised over the whole batch. The
// leftover of the last batch is simply never read.
#define KEYGEN_RNG_BUF_BLOCKS 4

struct keygen_rng_aes128_cat1_t {
  ctr128_t buf[KEYGEN_RNG_BUF_BLOCKS];  // keystream buffer
  ctr128_t rk[11];                      // round keys (11 blocks for aes128)
  uint64_t next_block;                  // counter of the next block to produce
  uint64_t off;                         // read offset in buf (== buf size when empty)
} __attribute__((aligned(32)));

// cat3 and cat5 share this layout and every function but the init: the only
// difference between them is the key the schedule is run on.
struct keygen_rng_rijndael256_t {
  ctr256_t buf[KEYGEN_RNG_BUF_BLOCKS];  // keystream buffer
  ctr256_t rk[15];                      // round keys (15 blocks for rijndael256)
  uint64_t next_block;                  // counter of the next block to produce
  uint64_t off;                         // read offset in buf (== buf size when empty)
} __attribute__((aligned(32)));

// Marks the buffer as empty, so that the first draw refills it from block 0.
#define KEYGEN_RNG_INIT_EMPTY(S) \
  do {                           \
    (S)->next_block = 0;         \
    (S)->off = sizeof((S)->buf); \
  } while (0)

// Body of the draw function, parameterised by the cipher (ref or avx2 kernel):
// refill the buffer when it runs dry, then hand out the next little-endian
// uint32. Shared by the ref and avx paths so the two can only ever differ in
// the cipher call itself.
#define DEFINE_KEYGEN_RNG_NEXT_U32(NAME, STATE_T, CTR_T, CTRLE_NBLOCKS)         \
  EXPORT uint32_t NAME(keygen_rng_ctx* rng) {                                   \
    struct STATE_T* const s = (struct STATE_T*)rng;                             \
    if (s->off == sizeof(s->buf)) {                                             \
      CTR_T ctr = {};                                                           \
      ctr.v64[0] = s->next_block;                                               \
      CTRLE_NBLOCKS(s->buf, s->rk, ctr.v64, KEYGEN_RNG_BUF_BLOCKS);             \
      s->next_block += KEYGEN_RNG_BUF_BLOCKS;                                   \
      s->off = 0;                                                               \
    }                                                                           \
    uint32_t out;                                                               \
    memcpy(&out, (const uint8_t*)s->buf + s->off, 4);                           \
    s->off += 4;                                                                \
    return out;                                                                 \
  }

#endif  // RNG_PRIVATE_H
