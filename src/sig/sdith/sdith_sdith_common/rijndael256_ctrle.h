#ifndef RIJNDAEL256_CTRLE_H
#define RIJNDAEL256_CTRLE_H

#ifdef __cplusplus
#define EXPORT extern "C"
#include <cstdint>
#else
#define EXPORT
#include "stdint.h"
#endif

#define RIJNDAEL256_RK_BYTES 480
#define RIJNDAEL256_RNG_BYTES 608

/** (special API) batched key schedule: expand 1..4 independent rijndael256 keys
 * into 1..4 round-key buffers (each RIJNDAEL256_RK_BYTES). Output is bit-identical
 * to rijndael256_ctrle_set_key (KAT-preserving). The avx2 path uses aesenclast for
 * the SubWord step and interleaves the independent keys to hide its latency; the ref
 * path just loops the scalar x1. */
EXPORT void rijndael256_key_schedule_x1_ref(void* rk0, const void* k0);
EXPORT void rijndael256_key_schedule_x2_ref(void* rk0, void* rk1, const void* k0, const void* k1);
EXPORT void rijndael256_key_schedule_x3_ref(void* rk0, void* rk1, void* rk2,  //
                                            const void* k0, const void* k1, const void* k2);
EXPORT void rijndael256_key_schedule_x4_ref(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                            const void* k0, const void* k1, const void* k2, const void* k3);
EXPORT void rijndael256_key_schedule_x1_avx2(void* rk0, const void* k0);
EXPORT void rijndael256_key_schedule_x2_avx2(void* rk0, void* rk1, const void* k0, const void* k1);
EXPORT void rijndael256_key_schedule_x3_avx2(void* rk0, void* rk1, void* rk2,  //
                                             const void* k0, const void* k1, const void* k2);
EXPORT void rijndael256_key_schedule_x4_avx2(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                             const void* k0, const void* k1, const void* k2, const void* k3);

/** (special API) no-carry CTR encryption. Encrypts N consecutive 32-byte blocks:
 * out[i] = Rijndael256(round_keys, ctr_block with its low 64 bits += i), i in 0..N-1.
 * Only the low 64 bits of the counter increment (no carry). Stateless: ctr_block
 * (32 bytes) is not modified. Chains into rijndael256_ctrle_nocarry_nblocks.
 * (3/4-block variants added only if they beat the 2-block chaining.) */
EXPORT void rijndael256_ctrle_nocarry_1block_ref(void* out, const void* round_keys, const void* ctr_block);
EXPORT void rijndael256_ctrle_nocarry_2block_ref(void* out, const void* round_keys, const void* ctr_block);
EXPORT void rijndael256_ctrle_nocarry_nblocks_ref(void* out, const void* round_keys, const void* ctr_block,
                                                  uint64_t nblocks);
EXPORT void rijndael256_ctrle_nocarry_1block_avx2(void* out, const void* round_keys, const void* ctr_block);
EXPORT void rijndael256_ctrle_nocarry_2block_avx2(void* out, const void* round_keys, const void* ctr_block);
EXPORT void rijndael256_ctrle_nocarry_nblocks_avx2(void* out, const void* round_keys, const void* ctr_block,
                                                   uint64_t nblocks);

/**
 * (special API) Rijndael-256 proof-of-work grind over two independent keys.
 * The Rijndael-256 analogue of aes128_proofow_grind_avx2. Encrypts the two 32-byte
 * plaintext blocks in_pt512 (p0 || p1, 64 bytes) under the two round-key sets rk
 * (rk[2], each RIJNDAEL256_RK_BYTES, contiguous = 960 bytes). If the ciphertexts
 * satisfy the grinding condition ((c0.lo64 ^ c1.lo64) & mask_w) == 0, success: stores
 * the two 32-byte ciphertexts in out_ct512 (c0 || c1), stores the winning counter in
 * *in_out_counter, and returns 1. Otherwise increments the counter and the low 64 bits
 * of both plaintexts by 1 (no carry into the upper bits) and retries. If the counter
 * reaches 2^32 with no hit, stores 2^32 in *in_out_counter and returns 0. The winning
 * counter is the smallest one in [initial, 2^32).
 */
EXPORT int rijndael256_proofow_grind_avx2(  //
    void* out_ct512,                        // [out] c0 || c1 (64 bytes) satisfying low64(c0 ^ c1) & mask_w == 0
    uint64_t* in_out_counter,               // [in/out] starting counter (< 2^32)
    const void* in_pt512,                   // [in] p0 || p1 (64 bytes) the initial plaintexts
    const void* rk,                         // [in] two round-key sets rk[2][RIJNDAEL256_RK_BYTES] (960 bytes)
    uint64_t mask_w                         // [in] the grinding condition
);

typedef union {
  uint64_t v64[4];
  uint8_t v8[32];
} ctr256_t;

typedef union {
  uint64_t v64[4];
  uint8_t v8[32];
} ctr256_align32_t __attribute__((aligned(32)));

#endif  // RIJNDAEL256_CTRLE_H
