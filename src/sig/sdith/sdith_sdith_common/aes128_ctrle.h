#ifndef AES128_CTRLE_H
#define AES128_CTRLE_H
#include "rijndael256_ctrle.h"

#ifdef __cplusplus
#define EXPORT extern "C"
#include <cstdint>
#else
#define EXPORT
#include "stdint.h"
#endif

/** (special API) batched key schedule: expand 1..4 independent aes128 keys into
 * 1..4 round-key buffers (each 16*11 bytes). Output is bit-identical to
 * aes128_ctrle_set_key, so these are KAT-preserving. The x2/x3/x4 avx2 variants
 * interleave the expansions to hide latency; the ref variants just loop. */
EXPORT void aes128_key_schedule_x1_ref(void* rk0, const void* k0);
EXPORT void aes128_key_schedule_x2_ref(void* rk0, void* rk1, const void* k0, const void* k1);
EXPORT void aes128_key_schedule_x3_ref(void* rk0, void* rk1, void* rk2, const void* k0, const void* k1, const void* k2);
EXPORT void aes128_key_schedule_x4_ref(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                       const void* k0, const void* k1, const void* k2, const void* k3);
EXPORT void aes128_key_schedule_x1_avx2(void* rk0, const void* k0);
EXPORT void aes128_key_schedule_x2_avx2(void* rk0, void* rk1, const void* k0, const void* k1);
EXPORT void aes128_key_schedule_x3_avx2(void* rk0, void* rk1, void* rk2, const void* k0, const void* k1, const void* k2);
EXPORT void aes128_key_schedule_x4_avx2(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                        const void* k0, const void* k1, const void* k2, const void* k3);

/** (special API) no-carry CTR encryption. Encrypts N consecutive blocks:
 * out[i] = AES(round_keys, ctr_block with its low 64 bits += i), for i in 0..N-1.
 * Only the low 64 bits of the counter are incremented (no carry into the upper
 * 64 bits). Stateless: ctr_block (16 bytes) is not modified. The fixed-size
 * building blocks chain into aes128_ctrle_nocarry_nblocks (advance the low-64
 * counter by the group size between calls). */
EXPORT void aes128_ctrle_nocarry_1block_ref(void* out, const void* round_keys, const void* ctr_block);
EXPORT void aes128_ctrle_nocarry_2block_ref(void* out, const void* round_keys, const void* ctr_block);
EXPORT void aes128_ctrle_nocarry_3blocks_ref(void* out, const void* round_keys, const void* ctr_block);
EXPORT void aes128_ctrle_nocarry_4blocks_ref(void* out, const void* round_keys, const void* ctr_block);
EXPORT void aes128_ctrle_nocarry_nblocks_ref(void* out, const void* round_keys, const void* ctr_block, uint64_t nblocks);
EXPORT void aes128_ctrle_nocarry_1block_avx2(void* out, const void* round_keys, const void* ctr_block);
EXPORT void aes128_ctrle_nocarry_2block_avx2(void* out, const void* round_keys, const void* ctr_block);
EXPORT void aes128_ctrle_nocarry_3blocks_avx2(void* out, const void* round_keys, const void* ctr_block);
EXPORT void aes128_ctrle_nocarry_4blocks_avx2(void* out, const void* round_keys, const void* ctr_block);
EXPORT void aes128_ctrle_nocarry_nblocks_avx2(void* out, const void* round_keys, const void* ctr_block, uint64_t nblocks);
// 4 keys x 2 counter blocks (8 blocks) interleaved: for key i (round keys at round_keys4 + i*176,
// base counter at ctr4blk + i*16) writes enc(ctr_i) || enc(ctr_i+1) to out + i*32. Byte-identical
// to four aes128_ctrle_nocarry_2block_avx2 calls (KAT-preserving); keeps 8 aesenc in flight.
EXPORT void aes128_ctrle_nocarry_2blk_x4keys_avx2(void* out, const void* round_keys4, const void* ctr4blk);

/**
 * (special API) AES proof-of-work grind over two independent keys.
 * Encrypts the two plaintext blocks in_pt256 (p0 || p1, 32 bytes) under the two round-key sets
 * rk128 (rk[2][11], 352 bytes),
 * If the ciphertexts satisfy the grinding condition: ((c0.lo64 ^ c1.lo64) & mask_w) == 0, success:
 * return store the counter and the ciphertexts and return 1.
 * Otherwise, increment the counter and the plaintexts by 1 (no carry into the high 64 bits) and restart.
 * If the counter reaches 2^32 with no hit, stores 2^32 in *in_out_counter and returns 0.
 * The winning counter is the smallest one in [initial, 2^32).
 */
EXPORT int aes128_proofow_grind_avx2(  //
    void* out_ct256,                   // [out] c0 || c1 (32 bytes) satisfying low64(c0 ^ c1) & mask_w == 0
    uint64_t* in_out_counter,          // [in/out] starting counter (< 2^32)
    const void* in_pt256,              // [in] p0 || p1 (32 bytes) the initial plaintexts
    const void* rk128,                 // [in] matrix of round keys v128[2][11]
    uint64_t mask_w                    // [in] the grinding condition
);

typedef union {
  __uint128_t u128;
  uint64_t v64[2];
  uint8_t v8[16];
} ctr128_t;

#endif  // AES128_CTRLE_H
