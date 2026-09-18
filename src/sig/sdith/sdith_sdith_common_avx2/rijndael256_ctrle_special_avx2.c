/* Batched key schedule + no-carry CTR encryption for Rijndael-256 (AVX/AES-NI).
 *
 * Round-key layout matches rijndael256_ctrle_set_key_avx2 (i.e. rijndael256_avx_rk_t),
 * so these are drop-in / KAT-preserving. The no-carry CTR path and the proof-of-work
 * grind both build counter blocks straight in registers and run the rounds over N
 * interleaved blocks via sdith_rij_encN (mirroring the aes128 sdith_aes_encN /
 * sdith_ctr_lo strategy), avoiding any ECB staging buffer. The no-carry path
 * increments only the low 64 bits of the 256-bit counter block.
 */
#include <string.h>

#include "rijndael256_avx.h"
#include "rijndael256_ctrle.h"

/* -------- batched key schedule --------
 * The AVX key expansion uses aesenclast for the SubWord step and interleaves
 * independent keys to hide its latency (see rijndael256_keyexp_x*_enclast). */
void rijndael256_key_schedule_x1_avx2(void* rk0, const void* k0) { rijndael256_keyexp_x1_enclast(rk0, k0); }
void rijndael256_key_schedule_x2_avx2(void* rk0, void* rk1, const void* k0, const void* k1) {
  rijndael256_keyexp_x2_enclast(rk0, rk1, k0, k1);
}
void rijndael256_key_schedule_x3_avx2(void* rk0, void* rk1, void* rk2,  //
                                      const void* k0, const void* k1, const void* k2) {
  rijndael256_keyexp_x3_enclast(rk0, rk1, rk2, k0, k1, k2);
}
void rijndael256_key_schedule_x4_avx2(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                      const void* k0, const void* k1, const void* k2, const void* k3) {
  rijndael256_keyexp_x4_enclast(rk0, rk1, rk2, rk3, k0, k1, k2, k3);
}

/* -------- no-carry CTR encryption (direct AVX, register states) --------
 * Each 256-bit block is two 128-bit halves (lo = state cols 0..3, hi = cols 4..7);
 * only the low 64 bits of lo carry the counter. Round-key layout (rijndael256_avx_rk_t):
 * round r half0 at byte 32*r, half1 at 32*r+16; 15 round keys (rounds 0..14). The
 * BLEND(BMASK)/SHUF8(RMASK) dance compensates for Rijndael-256's ShiftRows moving bytes
 * across the two 128-bit halves. Output is bit-identical to the ECB path (KAT-preserving). */
enum { R256_ROUNDS = 14 };

/* encrypt n (1..4) interleaved blocks: plaintext in (lo[k],hi[k]) -> ciphertext to out. */
static __always_inline void sdith_rij_encN(uint8_t* out, const uint8_t* rk, __m128i* lo, __m128i* hi, int n) {
  const __m128i RMASK = _mm_set_epi32(0x03020d0c, 0x0f0e0908, 0x0b0a0504, 0x07060100);
  const __m128i BMASK = _mm_set_epi32(0x80000000, 0x80800000, 0x80800000, 0x80808000);

  // Round 0: initial xor with the master key.
  __m128i k0 = _mm_loadu_si128((const __m128i*)(rk));
  __m128i k1 = _mm_loadu_si128((const __m128i*)(rk + 16));
  for (int k = 0; k < n; k++) {
    lo[k] = _mm_xor_si128(lo[k], k0);
    hi[k] = _mm_xor_si128(hi[k], k1);
  }
  // Rounds 1..13: sub bytes / shift rows / mix columns + round key.
  for (int r = 1; r < R256_ROUNDS; r++) {
    __m128i rk0 = _mm_loadu_si128((const __m128i*)(rk + 32 * r));
    __m128i rk1 = _mm_loadu_si128((const __m128i*)(rk + 32 * r + 16));
    for (int k = 0; k < n; k++) {
      __m128i t0 = _mm_shuffle_epi8(_mm_blendv_epi8(lo[k], hi[k], BMASK), RMASK);
      __m128i t1 = _mm_shuffle_epi8(_mm_blendv_epi8(hi[k], lo[k], BMASK), RMASK);
      lo[k] = _mm_aesenc_si128(t0, rk0);
      hi[k] = _mm_aesenc_si128(t1, rk1);
    }
  }
  // Last round: no mix columns.
  __m128i rk0 = _mm_loadu_si128((const __m128i*)(rk + 32 * R256_ROUNDS));
  __m128i rk1 = _mm_loadu_si128((const __m128i*)(rk + 32 * R256_ROUNDS + 16));
  for (int k = 0; k < n; k++) {
    __m128i t0 = _mm_shuffle_epi8(_mm_blendv_epi8(lo[k], hi[k], BMASK), RMASK);
    __m128i t1 = _mm_shuffle_epi8(_mm_blendv_epi8(hi[k], lo[k], BMASK), RMASK);
    _mm_storeu_si128((__m128i*)(out + 32 * k), _mm_aesenclast_si128(t0, rk0));
    _mm_storeu_si128((__m128i*)(out + 32 * k + 16), _mm_aesenclast_si128(t1, rk1));
  }
}

/* block halves for base with only its low 64 bits incremented by i (no carry). */
static __always_inline void sdith_rij_ctr_lo(__m128i base_lo, __m128i base_hi, uint64_t i,  //
                                             __m128i* lo, __m128i* hi) {
  *lo = _mm_add_epi64(base_lo, _mm_set_epi64x(0, (long long)i));
  *hi = base_hi;
}

void rijndael256_ctrle_nocarry_1block_avx2(void* out, const void* round_keys, const void* ctr_block) {
  __m128i lo[1] = {_mm_loadu_si128((const __m128i*)ctr_block)};
  __m128i hi[1] = {_mm_loadu_si128((const __m128i*)((const uint8_t*)ctr_block + 16))};
  sdith_rij_encN((uint8_t*)out, (const uint8_t*)round_keys, lo, hi, 1);
}
void rijndael256_ctrle_nocarry_2block_avx2(void* out, const void* round_keys, const void* ctr_block) {
  __m128i base_lo = _mm_loadu_si128((const __m128i*)ctr_block);
  __m128i base_hi = _mm_loadu_si128((const __m128i*)((const uint8_t*)ctr_block + 16));
  __m128i lo[2], hi[2];
  sdith_rij_ctr_lo(base_lo, base_hi, 0, &lo[0], &hi[0]);
  sdith_rij_ctr_lo(base_lo, base_hi, 1, &lo[1], &hi[1]);
  sdith_rij_encN((uint8_t*)out, (const uint8_t*)round_keys, lo, hi, 2);
}
void rijndael256_ctrle_nocarry_nblocks_avx2(void* out, const void* round_keys, const void* ctr_block,
                                            uint64_t nblocks) {
  const uint8_t* rk = (const uint8_t*)round_keys;
  uint8_t* o = (uint8_t*)out;
  __m128i base_lo = _mm_loadu_si128((const __m128i*)ctr_block);
  __m128i base_hi = _mm_loadu_si128((const __m128i*)((const uint8_t*)ctr_block + 16));
  uint64_t i = 0;
  for (; i + 4 <= nblocks; i += 4) {
    __m128i lo[4], hi[4];
    for (int j = 0; j < 4; j++) sdith_rij_ctr_lo(base_lo, base_hi, i + j, &lo[j], &hi[j]);
    sdith_rij_encN(o + 32 * i, rk, lo, hi, 4);
  }
  int rem = (int)(nblocks - i);
  if (rem > 0) {
    __m128i lo[4], hi[4];
    for (int j = 0; j < rem; j++) sdith_rij_ctr_lo(base_lo, base_hi, i + j, &lo[j], &hi[j]);
    sdith_rij_encN(o + 32 * i, rk, lo, hi, rem);
  }
}

/* -------- proof-of-work grind (2 independent keys) --------
 * Rijndael-256 analogue of aes128_proofow_grind_avx2: the inner loop of the cipher-based
 * proofow_grind for cat3/cat5. It encrypts two 32-byte counter blocks p0,p1 under two
 * independent round-key sets rk[0],rk[1], incrementing only the low 64 bits of both
 * plaintexts by 1 each step (no carry), and stops as soon as ((c0.lo64 ^ c1.lo64) & mask_w)
 * == 0. Matches the scalar loop (rijndael256_ctrle_nocarry_1block on each block) bit-for-bit.
 *
 * Counters are processed a GROUP at a time so each key's encryption batches GROUP blocks
 * through sdith_rij_encN (hiding aesenc latency), exactly like the aes128 grind. */
int rijndael256_proofow_grind_avx2(  //
    void* out_ct512,                 // [out] c0 || c1 (64 bytes) satisfying low64(c0 ^ c1) & mask_w == 0
    uint64_t* in_out_counter,        // [in/out] starting counter (< 2^32)
    const void* in_pt512,            // [in] p0 || p1 (64 bytes) the initial plaintexts
    const void* rk,                  // [in] two round-key sets rk[2][RIJNDAEL256_RK_BYTES] (960 bytes)
    uint64_t mask_w                  // [in] 64-bit mask applied to the xor of the ciphertext low words
) {
  enum { GROUP = 4 };
  const uint64_t CTR_MAX = UINT64_C(1) << 32;
  const uint8_t* rk0 = (const uint8_t*)rk;                            // rk[0]
  const uint8_t* rk1 = (const uint8_t*)rk + RIJNDAEL256_RK_BYTES;     // rk[1]
  __m128i b0lo = _mm_loadu_si128((const __m128i*)in_pt512);
  __m128i b0hi = _mm_loadu_si128((const __m128i*)((const uint8_t*)in_pt512 + 16));
  __m128i b1lo = _mm_loadu_si128((const __m128i*)((const uint8_t*)in_pt512 + 32));
  __m128i b1hi = _mm_loadu_si128((const __m128i*)((const uint8_t*)in_pt512 + 48));
  uint64_t ctr = *in_out_counter;

  uint8_t cbuf0[GROUP * 32], cbuf1[GROUP * 32];
  while (ctr < CTR_MAX) {
    uint64_t remaining = CTR_MAX - ctr;
    int g = remaining < (uint64_t)GROUP ? (int)remaining : GROUP;
    __m128i lo0[GROUP], hi0[GROUP], lo1[GROUP], hi1[GROUP];
    for (int j = 0; j < g; j++) {
      sdith_rij_ctr_lo(b0lo, b0hi, (uint64_t)j, &lo0[j], &hi0[j]);  // add j to the low 64 bits only
      sdith_rij_ctr_lo(b1lo, b1hi, (uint64_t)j, &lo1[j], &hi1[j]);
    }
    sdith_rij_encN(cbuf0, rk0, lo0, hi0, g);
    sdith_rij_encN(cbuf1, rk1, lo1, hi1, g);
    // check candidates in increasing-counter order; return the first hit
    for (int j = 0; j < g; j++) {
      uint64_t x0, x1;
      memcpy(&x0, cbuf0 + 32 * j, 8);  // low 64 bits of ciphertext block 0
      memcpy(&x1, cbuf1 + 32 * j, 8);  // low 64 bits of ciphertext block 1
      if (((x0 ^ x1) & mask_w) == 0) {
        memcpy(out_ct512, cbuf0 + 32 * j, 32);
        memcpy((uint8_t*)out_ct512 + 32, cbuf1 + 32 * j, 32);
        *in_out_counter = ctr + (uint64_t)j;
        return 1;
      }
    }
    ctr += (uint64_t)g;
    b0lo = _mm_add_epi64(b0lo, _mm_set_epi64x(0, (long long)g));  // advance the low 64 bits (no carry)
    b1lo = _mm_add_epi64(b1lo, _mm_set_epi64x(0, (long long)g));
  }
  *in_out_counter = CTR_MAX;  // no good counter within [initial, 2^32)
  return 0;
}
