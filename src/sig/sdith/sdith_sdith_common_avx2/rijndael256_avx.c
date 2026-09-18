#include "rijndael256_avx.h"

// The following part is adapted from a work by Nir Drucker and Shay Gueron:
// Software optimization of Rijndael for Modern x86_64 platforms,
// in ITNG 2022 - 19th International Conference on Information Technology - New Generations (pp 147-153)
// Original code: Copyright Nir Drucker and Shay Gueron All Rights Reserved.
// SPDX-License-Identifier: Apache-2.0

// This file now provides only the Rijndael-256 key expansion (AVX/AES-NI). The
// ECB / CTR encryption cores were removed in favour of the register-based CTR path
// in rijndael256_ctrle_special_avx2.c (sdith_rij_encN / sdith_rij_ctr_lo).

#define KEY_SCHEDULE_ROUNDS 14

// AVX instructions used by the key expansion
#define SET(v1, v2, v3, v4) _mm_set_epi32((v1), (v2), (v3), (v4))
#define LOAD(mem) _mm_loadu_si128((const __m128i*)(mem))
#define STORE(mem, reg) _mm_storeu_si128((__m128i*)(mem), (reg))
#define AESENCLAST(in, key) _mm_aesenclast_si128((in), (key))
#define SLL(reg, n) _mm_slli_si128((reg), (n))
#define SHUF8(in, mask) _mm_shuffle_epi8((in), (mask))

// ---- aesenclast-based batched key expansion ---------------------------------
//
// Uses aesenclast for the SubWord step (cheaper and pipelineable than the serial
// aeskeygenassist chain) and expands up to 4 independent keys at once to hide its
// latency. The round keys are bit-identical to the classic aeskeygenassist schedule
// (KAT-preserving): the rijndael256 key schedule is the AES-256 (Nk=8) schedule,
// and since SubWord is byte-wise, SubWord(RotWord(x)) == RotWord(SubWord(x)), so a
// broadcast-then-aesenclast computes exactly the SubWord step aeskeygenassist does.
//
// Each 256-bit key state is two 128-bit halves: lo = w0..w3, hi = w4..w7. Per
// round r we produce the next lo,hi (stored contiguously, matching the old layout):
//   lo' = cascade(lo, g_lo), with g_lo = SubWord(RotWord(w7)) ^ rcon   (broadcast)
//   hi' = cascade(hi, g_hi), with g_hi = SubWord(w3 of lo')            (broadcast)

// broadcast RotWord(word3) to all four 32-bit words (bytes 13,14,15,12 of the reg)
#define R256_RWBC SET(0x0c0f0e0d, 0x0c0f0e0d, 0x0c0f0e0d, 0x0c0f0e0d)
// broadcast word3 unrotated to all four words (bytes 12,13,14,15 of the reg)
#define R256_WBC SET(0x0f0e0d0c, 0x0f0e0d0c, 0x0f0e0d0c, 0x0f0e0d0c)

static const uint8_t r256_rcon[KEY_SCHEDULE_ROUNDS] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
                                                       0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d};  // NOLINT

// new word-group = g ^ w ^ (w<<4) ^ (w<<8) ^ (w<<12), g broadcast to all 4 words.
// The w-terms are a byte-word prefix-XOR of w, factored into log steps to drop one
// pslldq + one xor per half per round (the schedule is port-5 bound). Byte-identical:
//   a = w ^ (w<<4); t = a ^ (a<<8); result = t ^ g.
static __always_inline __m128i r256_cascade(__m128i w, __m128i g) {
  __m128i a = _mm_xor_si128(w, SLL(w, 4));
  __m128i t = _mm_xor_si128(a, SLL(a, 8));
  return _mm_xor_si128(t, g);
}

// g for the low half (i%8==0): SubWord(RotWord(w7)) ^ rcon, broadcast.
static __always_inline __m128i r256_g_lo(__m128i hi, int rc) {
  return AESENCLAST(SHUF8(hi, R256_RWBC), _mm_set1_epi32(rc));
}
// g for the high half (i%8==4): SubWord(w3 of the updated lo), broadcast.
static __always_inline __m128i r256_g_hi(__m128i lo) {
  return AESENCLAST(SHUF8(lo, R256_WBC), _mm_setzero_si128());
}

#define R256_STORE_RK(rk, off, lo, hi)                    \
  do {                                                    \
    STORE((__m128i*)((uint8_t*)(rk) + (off)), (lo));      \
    STORE((__m128i*)((uint8_t*)(rk) + (off) + 16), (hi)); \
  } while (0)

void rijndael256_keyexp_x1_enclast(void* rk0, const void* k0) {
  __m128i lo0 = LOAD(k0), hi0 = LOAD((const uint8_t*)k0 + 16);
  R256_STORE_RK(rk0, 0, lo0, hi0);
  for (size_t r = 0; r < KEY_SCHEDULE_ROUNDS; r++) {
    const int rc = r256_rcon[r];
    lo0 = r256_cascade(lo0, r256_g_lo(hi0, rc));
    hi0 = r256_cascade(hi0, r256_g_hi(lo0));
    R256_STORE_RK(rk0, 32 * (r + 1), lo0, hi0);
  }
}

void rijndael256_keyexp_x2_enclast(void* rk0, void* rk1, const void* k0, const void* k1) {
  __m128i lo0 = LOAD(k0), hi0 = LOAD((const uint8_t*)k0 + 16);
  __m128i lo1 = LOAD(k1), hi1 = LOAD((const uint8_t*)k1 + 16);
  R256_STORE_RK(rk0, 0, lo0, hi0);
  R256_STORE_RK(rk1, 0, lo1, hi1);
  for (size_t r = 0; r < KEY_SCHEDULE_ROUNDS; r++) {
    const int rc = r256_rcon[r];
    lo0 = r256_cascade(lo0, r256_g_lo(hi0, rc));
    lo1 = r256_cascade(lo1, r256_g_lo(hi1, rc));
    hi0 = r256_cascade(hi0, r256_g_hi(lo0));
    hi1 = r256_cascade(hi1, r256_g_hi(lo1));
    R256_STORE_RK(rk0, 32 * (r + 1), lo0, hi0);
    R256_STORE_RK(rk1, 32 * (r + 1), lo1, hi1);
  }
}

void rijndael256_keyexp_x3_enclast(void* rk0, void* rk1, void* rk2,  //
                                   const void* k0, const void* k1, const void* k2) {
  __m128i lo0 = LOAD(k0), hi0 = LOAD((const uint8_t*)k0 + 16);
  __m128i lo1 = LOAD(k1), hi1 = LOAD((const uint8_t*)k1 + 16);
  __m128i lo2 = LOAD(k2), hi2 = LOAD((const uint8_t*)k2 + 16);
  R256_STORE_RK(rk0, 0, lo0, hi0);
  R256_STORE_RK(rk1, 0, lo1, hi1);
  R256_STORE_RK(rk2, 0, lo2, hi2);
  for (size_t r = 0; r < KEY_SCHEDULE_ROUNDS; r++) {
    const int rc = r256_rcon[r];
    lo0 = r256_cascade(lo0, r256_g_lo(hi0, rc));
    lo1 = r256_cascade(lo1, r256_g_lo(hi1, rc));
    lo2 = r256_cascade(lo2, r256_g_lo(hi2, rc));
    hi0 = r256_cascade(hi0, r256_g_hi(lo0));
    hi1 = r256_cascade(hi1, r256_g_hi(lo1));
    hi2 = r256_cascade(hi2, r256_g_hi(lo2));
    R256_STORE_RK(rk0, 32 * (r + 1), lo0, hi0);
    R256_STORE_RK(rk1, 32 * (r + 1), lo1, hi1);
    R256_STORE_RK(rk2, 32 * (r + 1), lo2, hi2);
  }
}

void rijndael256_keyexp_x4_enclast(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                   const void* k0, const void* k1, const void* k2, const void* k3) {
  __m128i lo0 = LOAD(k0), hi0 = LOAD((const uint8_t*)k0 + 16);
  __m128i lo1 = LOAD(k1), hi1 = LOAD((const uint8_t*)k1 + 16);
  __m128i lo2 = LOAD(k2), hi2 = LOAD((const uint8_t*)k2 + 16);
  __m128i lo3 = LOAD(k3), hi3 = LOAD((const uint8_t*)k3 + 16);
  R256_STORE_RK(rk0, 0, lo0, hi0);
  R256_STORE_RK(rk1, 0, lo1, hi1);
  R256_STORE_RK(rk2, 0, lo2, hi2);
  R256_STORE_RK(rk3, 0, lo3, hi3);
  for (size_t r = 0; r < KEY_SCHEDULE_ROUNDS; r++) {
    const int rc = r256_rcon[r];
    lo0 = r256_cascade(lo0, r256_g_lo(hi0, rc));
    lo1 = r256_cascade(lo1, r256_g_lo(hi1, rc));
    lo2 = r256_cascade(lo2, r256_g_lo(hi2, rc));
    lo3 = r256_cascade(lo3, r256_g_lo(hi3, rc));
    hi0 = r256_cascade(hi0, r256_g_hi(lo0));
    hi1 = r256_cascade(hi1, r256_g_hi(lo1));
    hi2 = r256_cascade(hi2, r256_g_hi(lo2));
    hi3 = r256_cascade(hi3, r256_g_hi(lo3));
    R256_STORE_RK(rk0, 32 * (r + 1), lo0, hi0);
    R256_STORE_RK(rk1, 32 * (r + 1), lo1, hi1);
    R256_STORE_RK(rk2, 32 * (r + 1), lo2, hi2);
    R256_STORE_RK(rk3, 32 * (r + 1), lo3, hi3);
  }
}

EXPORT void rijndael256_key_schedule_avx(rijndael256_avx_rk_t* roundkeys, const uint8_t key[32]) {
  rijndael256_keyexp_x1_enclast(roundkeys, key);
}
