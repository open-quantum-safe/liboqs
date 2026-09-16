/*
 * rijndael_arm_aes.c
 *
 * Rijndael encryption using ARM Crypto Extension hardware intrinsics
 * (vaeseq_u8 / vaesmcq_u8). Supports AES-128, AES-256, and Rijndael-256
 * (256-bit block / 256-bit key) as used by MQOM3.
 *
 * ARM AESE semantics vs AES-NI:
 *   ARM:    vaeseq_u8(state, rk) = ShiftRows(SubBytes(state XOR rk))
 *   AES-NI: _mm_aesenc_si128(state, rk) = MixColumns(ShiftRows(SubBytes(state))) XOR rk
 * Both produce correct AES ciphertext when used with the standard key schedule.
 *
 * Rijndael-256 ShiftRows compensation:
 *   A 256-bit block is kept in two 128-bit halves (ml, mr). Before each AESE
 *   instruction, a blend+shuffle rearranges bytes across the halves so that
 *   AES's ShiftRows {0,1,2,3} emulates Rijndael-256's ShiftRows {0,1,3,4}.
 */

#include "rijndael_platform.h"

#if defined(RIJNDAEL_ARM_AES)

#include "rijndael_arm_aes.h"

/*
 * Rcon constants for AES key schedule (indices 1..14).
 * These are public constants, not secret data.
 */
static const uint8_t aes_rcon[15] = {
    0x00,  /* index 0: unused */
    0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
    0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d
};

/*
 * InvShiftRows shuffle table.
 *
 * Since AESE(x, 0) = ShiftRows(SubBytes(x)), applying InvShiftRows afterwards
 * recovers SubBytes(x):
 *
 *   SubBytes(x) = InvShiftRows( AESE(x, 0) )
 *
 * This gives a constant-time SubBytes using hardware AES instructions only,
 * with no data-dependent table lookups. The InvShiftRows permutation for a
 * 16-byte AES state in column-major order (state[4c+r], rows shifted by r):
 *   output[4c+r] = input[4*((c-r+4)%4)+r]
 * which gives the shuffle indices below.
 */
static const uint8_t arm_aes_inv_sr[16] = {
     0, 13, 10,  7,
     4,  1, 14, 11,
     8,  5,  2, 15,
    12,  9,  6,  3
};

/* ======== ARMv7 / AArch64 16-byte table lookup compatibility ======== */

/* AArch64 has vqtbl1q_u8 (full 16-byte table); ARMv7 only has vtbl2_u8 (8-byte) */
static inline uint8x16_t arm_vtbl16(uint8x16_t tbl, uint8x16_t idx)
{
#ifdef __aarch64__
    return vqtbl1q_u8(tbl, idx);
#else
    uint8x8x2_t tbl2;
    tbl2.val[0] = vget_low_u8(tbl);
    tbl2.val[1] = vget_high_u8(tbl);
    return vcombine_u8(vtbl2_u8(tbl2, vget_low_u8(idx)),
                       vtbl2_u8(tbl2, vget_high_u8(idx)));
#endif
}

/* ======== Rijndael-256 ShiftRows compensation constants ======== */

/*
 * Rijndael-256 has an 8x4 state (8 columns), which shifts rows by {0,1,3,4}
 * bytes. AES has a 4x4 state shifting by {0,1,2,3}. When splitting the 256-bit
 * state into two 128-bit halves (ml = columns 0-3, mr = columns 4-7) and
 * applying AESE to each half, an additional compensation step is required.
 *
 * The blend mask selects bytes from ml or mr to build the two adjusted halves:
 *   0x80 -> take from mr (sign bit 1 -> mask 0xFF for vbslq_u8)
 *   0x00 -> take from ml (sign bit 0 -> mask 0x00 for vbslq_u8)
 *
 * The shuffle permutation then reorders bytes within each half to complete
 * the row-shift correction before AESE is applied.
 *
 * These constants are the little-endian byte representation of the x86 SSE
 * equivalents: _mm_set_epi32(0x80000000,0x80800000,0x80800000,0x80808000)
 * and _mm_set_epi32(0x03020d0c,0x0f0e0908,0x0b0a0504,0x07060100).
 */
static const uint8_t rijndael256_sr_blend[16] = {
    0x00, 0x80, 0x80, 0x80,
    0x00, 0x00, 0x80, 0x80,
    0x00, 0x00, 0x80, 0x80,
    0x00, 0x00, 0x00, 0x80
};
static const uint8_t rijndael256_sr_shuff[16] = {
    0x00, 0x01, 0x06, 0x07,
    0x04, 0x05, 0x0a, 0x0b,
    0x08, 0x09, 0x0e, 0x0f,
    0x0c, 0x0d, 0x02, 0x03
};

/*
 * rijndael256_compensate_sr: rearrange (ml, mr) so that the AES ShiftRows
 * built into AESE produces Rijndael-256's row shifts.
 * Outputs (tmpl, tmpr) to be fed to AESE.
 */
static inline void rijndael256_compensate_sr(uint8x16_t ml, uint8x16_t mr,
                                             uint8x16_t *tmpl, uint8x16_t *tmpr)
{
    uint8x16_t blend = vld1q_u8(rijndael256_sr_blend);
    uint8x16_t shuff = vld1q_u8(rijndael256_sr_shuff);
    /* vshrq_n_s8 by 7: 0x80 -> 0xFF (all ones), 0x00 -> 0x00 (all zeros) */
    uint8x16_t mask = (uint8x16_t)vshrq_n_s8((int8x16_t)blend, 7);
    /* vbslq_u8(mask, a, b): where mask=0xFF pick from a, where mask=0x00 pick from b */
    *tmpl = arm_vtbl16(vbslq_u8(mask, mr, ml), shuff);
    *tmpr = arm_vtbl16(vbslq_u8(mask, ml, mr), shuff);
}

/* ======== AES-128 encryption (10 rounds, rk[0..10]) ======== */

/*
 * ARM AESE/AESMC loop for AES-128.
 * After vaeseq_u8(s, k[i]): state = ShiftRows(SubBytes(s XOR k[i]))
 * Then vaesmcq_u8 applies MixColumns. Last round omits MixColumns and XORs k[10].
 */
#define DO_ENC_AES128(s, rk) do { \
    (s) = vaeseq_u8((s), (rk)[0]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[1]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[2]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[3]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[4]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[5]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[6]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[7]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[8]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[9]); \
    (s) = veorq_u8((s), (rk)[10]); \
} while(0)

/*
 * _ARM_AES_STEP(s, rk, i): one AES round with MixColumns for a single state.
 * Writing AESE immediately before AESMC on the same register preserves
 * AESE+AESMC micro-op fusion on Cortex-A55/A76 (2cy latency, 1/cy throughput).
 * When multiple _ARM_AES_STEP calls expand consecutively, each pair (s=AESE; s=AESMC)
 * is independent of adjacent pairs so the OOO engine pipelines all streams in parallel.
 */
#define _ARM_AES_STEP(s, rk, i)  (s) = vaeseq_u8((s), (rk)[(i)]); (s) = vaesmcq_u8(s)

/* AES-128 encryption for 2 independent streams (round-by-round interleaved). */
#define DO_ENC_AES128_x2(s1, s2, rk1, rk2) do { \
    _ARM_AES_STEP(s1,rk1,0); _ARM_AES_STEP(s2,rk2,0); \
    _ARM_AES_STEP(s1,rk1,1); _ARM_AES_STEP(s2,rk2,1); \
    _ARM_AES_STEP(s1,rk1,2); _ARM_AES_STEP(s2,rk2,2); \
    _ARM_AES_STEP(s1,rk1,3); _ARM_AES_STEP(s2,rk2,3); \
    _ARM_AES_STEP(s1,rk1,4); _ARM_AES_STEP(s2,rk2,4); \
    _ARM_AES_STEP(s1,rk1,5); _ARM_AES_STEP(s2,rk2,5); \
    _ARM_AES_STEP(s1,rk1,6); _ARM_AES_STEP(s2,rk2,6); \
    _ARM_AES_STEP(s1,rk1,7); _ARM_AES_STEP(s2,rk2,7); \
    _ARM_AES_STEP(s1,rk1,8); _ARM_AES_STEP(s2,rk2,8); \
    (s1) = vaeseq_u8((s1), (rk1)[9]); (s2) = vaeseq_u8((s2), (rk2)[9]); \
    (s1) = veorq_u8((s1), (rk1)[10]); (s2) = veorq_u8((s2), (rk2)[10]); \
} while(0)

/* AES-128 encryption for 4 independent streams. */
#define DO_ENC_AES128_x4(s1, s2, s3, s4, rk1, rk2, rk3, rk4) do { \
    _ARM_AES_STEP(s1,rk1,0); _ARM_AES_STEP(s2,rk2,0); _ARM_AES_STEP(s3,rk3,0); _ARM_AES_STEP(s4,rk4,0); \
    _ARM_AES_STEP(s1,rk1,1); _ARM_AES_STEP(s2,rk2,1); _ARM_AES_STEP(s3,rk3,1); _ARM_AES_STEP(s4,rk4,1); \
    _ARM_AES_STEP(s1,rk1,2); _ARM_AES_STEP(s2,rk2,2); _ARM_AES_STEP(s3,rk3,2); _ARM_AES_STEP(s4,rk4,2); \
    _ARM_AES_STEP(s1,rk1,3); _ARM_AES_STEP(s2,rk2,3); _ARM_AES_STEP(s3,rk3,3); _ARM_AES_STEP(s4,rk4,3); \
    _ARM_AES_STEP(s1,rk1,4); _ARM_AES_STEP(s2,rk2,4); _ARM_AES_STEP(s3,rk3,4); _ARM_AES_STEP(s4,rk4,4); \
    _ARM_AES_STEP(s1,rk1,5); _ARM_AES_STEP(s2,rk2,5); _ARM_AES_STEP(s3,rk3,5); _ARM_AES_STEP(s4,rk4,5); \
    _ARM_AES_STEP(s1,rk1,6); _ARM_AES_STEP(s2,rk2,6); _ARM_AES_STEP(s3,rk3,6); _ARM_AES_STEP(s4,rk4,6); \
    _ARM_AES_STEP(s1,rk1,7); _ARM_AES_STEP(s2,rk2,7); _ARM_AES_STEP(s3,rk3,7); _ARM_AES_STEP(s4,rk4,7); \
    _ARM_AES_STEP(s1,rk1,8); _ARM_AES_STEP(s2,rk2,8); _ARM_AES_STEP(s3,rk3,8); _ARM_AES_STEP(s4,rk4,8); \
    (s1)=vaeseq_u8((s1),(rk1)[9]); (s2)=vaeseq_u8((s2),(rk2)[9]); \
    (s3)=vaeseq_u8((s3),(rk3)[9]); (s4)=vaeseq_u8((s4),(rk4)[9]); \
    (s1)=veorq_u8((s1),(rk1)[10]); (s2)=veorq_u8((s2),(rk2)[10]); \
    (s3)=veorq_u8((s3),(rk3)[10]); (s4)=veorq_u8((s4),(rk4)[10]); \
} while(0)

/* AES-128 encryption for 8 independent streams. ARM has 32 NEON registers,
 * so all 8 states fit simultaneously with no spill, enabling full x8 interleave. */
#define DO_ENC_AES128_x8(s1,s2,s3,s4,s5,s6,s7,s8, rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8) do { \
    _ARM_AES_STEP(s1,rk1,0); _ARM_AES_STEP(s2,rk2,0); _ARM_AES_STEP(s3,rk3,0); _ARM_AES_STEP(s4,rk4,0); \
    _ARM_AES_STEP(s5,rk5,0); _ARM_AES_STEP(s6,rk6,0); _ARM_AES_STEP(s7,rk7,0); _ARM_AES_STEP(s8,rk8,0); \
    _ARM_AES_STEP(s1,rk1,1); _ARM_AES_STEP(s2,rk2,1); _ARM_AES_STEP(s3,rk3,1); _ARM_AES_STEP(s4,rk4,1); \
    _ARM_AES_STEP(s5,rk5,1); _ARM_AES_STEP(s6,rk6,1); _ARM_AES_STEP(s7,rk7,1); _ARM_AES_STEP(s8,rk8,1); \
    _ARM_AES_STEP(s1,rk1,2); _ARM_AES_STEP(s2,rk2,2); _ARM_AES_STEP(s3,rk3,2); _ARM_AES_STEP(s4,rk4,2); \
    _ARM_AES_STEP(s5,rk5,2); _ARM_AES_STEP(s6,rk6,2); _ARM_AES_STEP(s7,rk7,2); _ARM_AES_STEP(s8,rk8,2); \
    _ARM_AES_STEP(s1,rk1,3); _ARM_AES_STEP(s2,rk2,3); _ARM_AES_STEP(s3,rk3,3); _ARM_AES_STEP(s4,rk4,3); \
    _ARM_AES_STEP(s5,rk5,3); _ARM_AES_STEP(s6,rk6,3); _ARM_AES_STEP(s7,rk7,3); _ARM_AES_STEP(s8,rk8,3); \
    _ARM_AES_STEP(s1,rk1,4); _ARM_AES_STEP(s2,rk2,4); _ARM_AES_STEP(s3,rk3,4); _ARM_AES_STEP(s4,rk4,4); \
    _ARM_AES_STEP(s5,rk5,4); _ARM_AES_STEP(s6,rk6,4); _ARM_AES_STEP(s7,rk7,4); _ARM_AES_STEP(s8,rk8,4); \
    _ARM_AES_STEP(s1,rk1,5); _ARM_AES_STEP(s2,rk2,5); _ARM_AES_STEP(s3,rk3,5); _ARM_AES_STEP(s4,rk4,5); \
    _ARM_AES_STEP(s5,rk5,5); _ARM_AES_STEP(s6,rk6,5); _ARM_AES_STEP(s7,rk7,5); _ARM_AES_STEP(s8,rk8,5); \
    _ARM_AES_STEP(s1,rk1,6); _ARM_AES_STEP(s2,rk2,6); _ARM_AES_STEP(s3,rk3,6); _ARM_AES_STEP(s4,rk4,6); \
    _ARM_AES_STEP(s5,rk5,6); _ARM_AES_STEP(s6,rk6,6); _ARM_AES_STEP(s7,rk7,6); _ARM_AES_STEP(s8,rk8,6); \
    _ARM_AES_STEP(s1,rk1,7); _ARM_AES_STEP(s2,rk2,7); _ARM_AES_STEP(s3,rk3,7); _ARM_AES_STEP(s4,rk4,7); \
    _ARM_AES_STEP(s5,rk5,7); _ARM_AES_STEP(s6,rk6,7); _ARM_AES_STEP(s7,rk7,7); _ARM_AES_STEP(s8,rk8,7); \
    _ARM_AES_STEP(s1,rk1,8); _ARM_AES_STEP(s2,rk2,8); _ARM_AES_STEP(s3,rk3,8); _ARM_AES_STEP(s4,rk4,8); \
    _ARM_AES_STEP(s5,rk5,8); _ARM_AES_STEP(s6,rk6,8); _ARM_AES_STEP(s7,rk7,8); _ARM_AES_STEP(s8,rk8,8); \
    (s1)=vaeseq_u8((s1),(rk1)[9]); (s2)=vaeseq_u8((s2),(rk2)[9]); \
    (s3)=vaeseq_u8((s3),(rk3)[9]); (s4)=vaeseq_u8((s4),(rk4)[9]); \
    (s5)=vaeseq_u8((s5),(rk5)[9]); (s6)=vaeseq_u8((s6),(rk6)[9]); \
    (s7)=vaeseq_u8((s7),(rk7)[9]); (s8)=vaeseq_u8((s8),(rk8)[9]); \
    (s1)=veorq_u8((s1),(rk1)[10]); (s2)=veorq_u8((s2),(rk2)[10]); \
    (s3)=veorq_u8((s3),(rk3)[10]); (s4)=veorq_u8((s4),(rk4)[10]); \
    (s5)=veorq_u8((s5),(rk5)[10]); (s6)=veorq_u8((s6),(rk6)[10]); \
    (s7)=veorq_u8((s7),(rk7)[10]); (s8)=veorq_u8((s8),(rk8)[10]); \
} while(0)

/* ======== AES-256 encryption (14 rounds, rk[0..14]) ======== */

#define DO_ENC_AES256(s, rk) do { \
    (s) = vaeseq_u8((s), (rk)[0]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[1]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[2]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[3]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[4]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[5]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[6]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[7]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[8]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[9]);  (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[10]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[11]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[12]); (s) = vaesmcq_u8(s); \
    (s) = vaeseq_u8((s), (rk)[13]); \
    (s) = veorq_u8((s), (rk)[14]); \
} while(0)

/* AES-256 encryption for 2 independent streams (round-by-round interleaved). */
#define DO_ENC_AES256_x2(s1, s2, rk1, rk2) do { \
    _ARM_AES_STEP(s1,rk1, 0); _ARM_AES_STEP(s2,rk2, 0); \
    _ARM_AES_STEP(s1,rk1, 1); _ARM_AES_STEP(s2,rk2, 1); \
    _ARM_AES_STEP(s1,rk1, 2); _ARM_AES_STEP(s2,rk2, 2); \
    _ARM_AES_STEP(s1,rk1, 3); _ARM_AES_STEP(s2,rk2, 3); \
    _ARM_AES_STEP(s1,rk1, 4); _ARM_AES_STEP(s2,rk2, 4); \
    _ARM_AES_STEP(s1,rk1, 5); _ARM_AES_STEP(s2,rk2, 5); \
    _ARM_AES_STEP(s1,rk1, 6); _ARM_AES_STEP(s2,rk2, 6); \
    _ARM_AES_STEP(s1,rk1, 7); _ARM_AES_STEP(s2,rk2, 7); \
    _ARM_AES_STEP(s1,rk1, 8); _ARM_AES_STEP(s2,rk2, 8); \
    _ARM_AES_STEP(s1,rk1, 9); _ARM_AES_STEP(s2,rk2, 9); \
    _ARM_AES_STEP(s1,rk1,10); _ARM_AES_STEP(s2,rk2,10); \
    _ARM_AES_STEP(s1,rk1,11); _ARM_AES_STEP(s2,rk2,11); \
    _ARM_AES_STEP(s1,rk1,12); _ARM_AES_STEP(s2,rk2,12); \
    (s1) = vaeseq_u8((s1), (rk1)[13]); (s2) = vaeseq_u8((s2), (rk2)[13]); \
    (s1) = veorq_u8((s1), (rk1)[14]);  (s2) = veorq_u8((s2), (rk2)[14]); \
} while(0)

/* AES-256 encryption for 4 independent streams. */
#define DO_ENC_AES256_x4(s1, s2, s3, s4, rk1, rk2, rk3, rk4) do { \
    _ARM_AES_STEP(s1,rk1, 0); _ARM_AES_STEP(s2,rk2, 0); _ARM_AES_STEP(s3,rk3, 0); _ARM_AES_STEP(s4,rk4, 0); \
    _ARM_AES_STEP(s1,rk1, 1); _ARM_AES_STEP(s2,rk2, 1); _ARM_AES_STEP(s3,rk3, 1); _ARM_AES_STEP(s4,rk4, 1); \
    _ARM_AES_STEP(s1,rk1, 2); _ARM_AES_STEP(s2,rk2, 2); _ARM_AES_STEP(s3,rk3, 2); _ARM_AES_STEP(s4,rk4, 2); \
    _ARM_AES_STEP(s1,rk1, 3); _ARM_AES_STEP(s2,rk2, 3); _ARM_AES_STEP(s3,rk3, 3); _ARM_AES_STEP(s4,rk4, 3); \
    _ARM_AES_STEP(s1,rk1, 4); _ARM_AES_STEP(s2,rk2, 4); _ARM_AES_STEP(s3,rk3, 4); _ARM_AES_STEP(s4,rk4, 4); \
    _ARM_AES_STEP(s1,rk1, 5); _ARM_AES_STEP(s2,rk2, 5); _ARM_AES_STEP(s3,rk3, 5); _ARM_AES_STEP(s4,rk4, 5); \
    _ARM_AES_STEP(s1,rk1, 6); _ARM_AES_STEP(s2,rk2, 6); _ARM_AES_STEP(s3,rk3, 6); _ARM_AES_STEP(s4,rk4, 6); \
    _ARM_AES_STEP(s1,rk1, 7); _ARM_AES_STEP(s2,rk2, 7); _ARM_AES_STEP(s3,rk3, 7); _ARM_AES_STEP(s4,rk4, 7); \
    _ARM_AES_STEP(s1,rk1, 8); _ARM_AES_STEP(s2,rk2, 8); _ARM_AES_STEP(s3,rk3, 8); _ARM_AES_STEP(s4,rk4, 8); \
    _ARM_AES_STEP(s1,rk1, 9); _ARM_AES_STEP(s2,rk2, 9); _ARM_AES_STEP(s3,rk3, 9); _ARM_AES_STEP(s4,rk4, 9); \
    _ARM_AES_STEP(s1,rk1,10); _ARM_AES_STEP(s2,rk2,10); _ARM_AES_STEP(s3,rk3,10); _ARM_AES_STEP(s4,rk4,10); \
    _ARM_AES_STEP(s1,rk1,11); _ARM_AES_STEP(s2,rk2,11); _ARM_AES_STEP(s3,rk3,11); _ARM_AES_STEP(s4,rk4,11); \
    _ARM_AES_STEP(s1,rk1,12); _ARM_AES_STEP(s2,rk2,12); _ARM_AES_STEP(s3,rk3,12); _ARM_AES_STEP(s4,rk4,12); \
    (s1)=vaeseq_u8((s1),(rk1)[13]); (s2)=vaeseq_u8((s2),(rk2)[13]); \
    (s3)=vaeseq_u8((s3),(rk3)[13]); (s4)=vaeseq_u8((s4),(rk4)[13]); \
    (s1)=veorq_u8((s1),(rk1)[14]);  (s2)=veorq_u8((s2),(rk2)[14]); \
    (s3)=veorq_u8((s3),(rk3)[14]);  (s4)=veorq_u8((s4),(rk4)[14]); \
} while(0)

/* AES-256 encryption for 8 independent streams. */
#define DO_ENC_AES256_x8(s1,s2,s3,s4,s5,s6,s7,s8, rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8) do { \
    _ARM_AES_STEP(s1,rk1, 0); _ARM_AES_STEP(s2,rk2, 0); _ARM_AES_STEP(s3,rk3, 0); _ARM_AES_STEP(s4,rk4, 0); \
    _ARM_AES_STEP(s5,rk5, 0); _ARM_AES_STEP(s6,rk6, 0); _ARM_AES_STEP(s7,rk7, 0); _ARM_AES_STEP(s8,rk8, 0); \
    _ARM_AES_STEP(s1,rk1, 1); _ARM_AES_STEP(s2,rk2, 1); _ARM_AES_STEP(s3,rk3, 1); _ARM_AES_STEP(s4,rk4, 1); \
    _ARM_AES_STEP(s5,rk5, 1); _ARM_AES_STEP(s6,rk6, 1); _ARM_AES_STEP(s7,rk7, 1); _ARM_AES_STEP(s8,rk8, 1); \
    _ARM_AES_STEP(s1,rk1, 2); _ARM_AES_STEP(s2,rk2, 2); _ARM_AES_STEP(s3,rk3, 2); _ARM_AES_STEP(s4,rk4, 2); \
    _ARM_AES_STEP(s5,rk5, 2); _ARM_AES_STEP(s6,rk6, 2); _ARM_AES_STEP(s7,rk7, 2); _ARM_AES_STEP(s8,rk8, 2); \
    _ARM_AES_STEP(s1,rk1, 3); _ARM_AES_STEP(s2,rk2, 3); _ARM_AES_STEP(s3,rk3, 3); _ARM_AES_STEP(s4,rk4, 3); \
    _ARM_AES_STEP(s5,rk5, 3); _ARM_AES_STEP(s6,rk6, 3); _ARM_AES_STEP(s7,rk7, 3); _ARM_AES_STEP(s8,rk8, 3); \
    _ARM_AES_STEP(s1,rk1, 4); _ARM_AES_STEP(s2,rk2, 4); _ARM_AES_STEP(s3,rk3, 4); _ARM_AES_STEP(s4,rk4, 4); \
    _ARM_AES_STEP(s5,rk5, 4); _ARM_AES_STEP(s6,rk6, 4); _ARM_AES_STEP(s7,rk7, 4); _ARM_AES_STEP(s8,rk8, 4); \
    _ARM_AES_STEP(s1,rk1, 5); _ARM_AES_STEP(s2,rk2, 5); _ARM_AES_STEP(s3,rk3, 5); _ARM_AES_STEP(s4,rk4, 5); \
    _ARM_AES_STEP(s5,rk5, 5); _ARM_AES_STEP(s6,rk6, 5); _ARM_AES_STEP(s7,rk7, 5); _ARM_AES_STEP(s8,rk8, 5); \
    _ARM_AES_STEP(s1,rk1, 6); _ARM_AES_STEP(s2,rk2, 6); _ARM_AES_STEP(s3,rk3, 6); _ARM_AES_STEP(s4,rk4, 6); \
    _ARM_AES_STEP(s5,rk5, 6); _ARM_AES_STEP(s6,rk6, 6); _ARM_AES_STEP(s7,rk7, 6); _ARM_AES_STEP(s8,rk8, 6); \
    _ARM_AES_STEP(s1,rk1, 7); _ARM_AES_STEP(s2,rk2, 7); _ARM_AES_STEP(s3,rk3, 7); _ARM_AES_STEP(s4,rk4, 7); \
    _ARM_AES_STEP(s5,rk5, 7); _ARM_AES_STEP(s6,rk6, 7); _ARM_AES_STEP(s7,rk7, 7); _ARM_AES_STEP(s8,rk8, 7); \
    _ARM_AES_STEP(s1,rk1, 8); _ARM_AES_STEP(s2,rk2, 8); _ARM_AES_STEP(s3,rk3, 8); _ARM_AES_STEP(s4,rk4, 8); \
    _ARM_AES_STEP(s5,rk5, 8); _ARM_AES_STEP(s6,rk6, 8); _ARM_AES_STEP(s7,rk7, 8); _ARM_AES_STEP(s8,rk8, 8); \
    _ARM_AES_STEP(s1,rk1, 9); _ARM_AES_STEP(s2,rk2, 9); _ARM_AES_STEP(s3,rk3, 9); _ARM_AES_STEP(s4,rk4, 9); \
    _ARM_AES_STEP(s5,rk5, 9); _ARM_AES_STEP(s6,rk6, 9); _ARM_AES_STEP(s7,rk7, 9); _ARM_AES_STEP(s8,rk8, 9); \
    _ARM_AES_STEP(s1,rk1,10); _ARM_AES_STEP(s2,rk2,10); _ARM_AES_STEP(s3,rk3,10); _ARM_AES_STEP(s4,rk4,10); \
    _ARM_AES_STEP(s5,rk5,10); _ARM_AES_STEP(s6,rk6,10); _ARM_AES_STEP(s7,rk7,10); _ARM_AES_STEP(s8,rk8,10); \
    _ARM_AES_STEP(s1,rk1,11); _ARM_AES_STEP(s2,rk2,11); _ARM_AES_STEP(s3,rk3,11); _ARM_AES_STEP(s4,rk4,11); \
    _ARM_AES_STEP(s5,rk5,11); _ARM_AES_STEP(s6,rk6,11); _ARM_AES_STEP(s7,rk7,11); _ARM_AES_STEP(s8,rk8,11); \
    _ARM_AES_STEP(s1,rk1,12); _ARM_AES_STEP(s2,rk2,12); _ARM_AES_STEP(s3,rk3,12); _ARM_AES_STEP(s4,rk4,12); \
    _ARM_AES_STEP(s5,rk5,12); _ARM_AES_STEP(s6,rk6,12); _ARM_AES_STEP(s7,rk7,12); _ARM_AES_STEP(s8,rk8,12); \
    (s1)=vaeseq_u8((s1),(rk1)[13]); (s2)=vaeseq_u8((s2),(rk2)[13]); \
    (s3)=vaeseq_u8((s3),(rk3)[13]); (s4)=vaeseq_u8((s4),(rk4)[13]); \
    (s5)=vaeseq_u8((s5),(rk5)[13]); (s6)=vaeseq_u8((s6),(rk6)[13]); \
    (s7)=vaeseq_u8((s7),(rk7)[13]); (s8)=vaeseq_u8((s8),(rk8)[13]); \
    (s1)=veorq_u8((s1),(rk1)[14]); (s2)=veorq_u8((s2),(rk2)[14]); \
    (s3)=veorq_u8((s3),(rk3)[14]); (s4)=veorq_u8((s4),(rk4)[14]); \
    (s5)=veorq_u8((s5),(rk5)[14]); (s6)=veorq_u8((s6),(rk6)[14]); \
    (s7)=veorq_u8((s7),(rk7)[14]); (s8)=veorq_u8((s8),(rk8)[14]); \
} while(0)

/* ======== Rijndael-256 encryption (14 rounds, rk[0..29]) ======== */

/*
 * Each round processes both halves (ml, mr) of the 256-bit block.
 * Full rounds: compensate + AESE(k[2i]) + AESMC, AESE(k[2i+1]) + AESMC.
 * Final round: compensate + AESE(k[26]) + AESE(k[27]), no AESMC.
 * Final AddRoundKey: XOR k[28] and k[29].
 */
#define DO_ENC_RIJNDAEL256(ml, mr, rk) do { \
    uint8x16_t _tl, _tr; \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[0])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[1])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[2])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[3])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[4])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[5])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[6])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[7])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[8])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[9])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[10])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[11])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[12])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[13])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[14])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[15])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[16])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[17])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[18])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[19])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[20])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[21])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[22])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[23])); \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaesmcq_u8(vaeseq_u8(_tl, (rk)[24])); \
    (mr) = vaesmcq_u8(vaeseq_u8(_tr, (rk)[25])); \
    /* Final round: no AESMC */ \
    rijndael256_compensate_sr((ml), (mr), &_tl, &_tr); \
    (ml) = vaeseq_u8(_tl, (rk)[26]); \
    (mr) = vaeseq_u8(_tr, (rk)[27]); \
    /* Final AddRoundKey */ \
    (ml) = veorq_u8((ml), (rk)[28]); \
    (mr) = veorq_u8((mr), (rk)[29]); \
} while(0)

/*
 * _RJ256_FULL_ROUND_x2: one interleaved Rijndael-256 full round for 2 streams.
 * ki = first key index for this round (even; uses ki and ki+1).
 * _bl/_sh/_mk must be preloaded by the caller (rijndael256_sr_blend/shuff and mask).
 * Temps _a/_b/_c/_d are uint8x16_t scratch registers declared by the caller.
 */
#define _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, ki, _a,_b,_c,_d, _bl,_sh,_mk) \
    (_a)=arm_vtbl16(vbslq_u8((_mk),(mr1),(ml1)),(_sh)); \
    (_b)=arm_vtbl16(vbslq_u8((_mk),(ml1),(mr1)),(_sh)); \
    (_c)=arm_vtbl16(vbslq_u8((_mk),(mr2),(ml2)),(_sh)); \
    (_d)=arm_vtbl16(vbslq_u8((_mk),(ml2),(mr2)),(_sh)); \
    (ml1)=vaesmcq_u8(vaeseq_u8((_a),(rk1)[(ki)]));   (mr1)=vaesmcq_u8(vaeseq_u8((_b),(rk1)[(ki)+1])); \
    (ml2)=vaesmcq_u8(vaeseq_u8((_c),(rk2)[(ki)]));   (mr2)=vaesmcq_u8(vaeseq_u8((_d),(rk2)[(ki)+1]))

/* Rijndael-256 encryption for 2 independent streams (round-by-round interleaved). */
#define DO_ENC_RIJNDAEL256_x2(ml1,mr1, ml2,mr2, rk1,rk2) do { \
    uint8x16_t _a, _b, _c, _d; \
    const uint8x16_t _bl = vld1q_u8(rijndael256_sr_blend); \
    const uint8x16_t _sh = vld1q_u8(rijndael256_sr_shuff); \
    const uint8x16_t _mk = (uint8x16_t)vshrq_n_s8((int8x16_t)_bl, 7); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2,  0, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2,  2, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2,  4, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2,  6, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2,  8, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, 10, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, 12, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, 14, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, 16, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, 18, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, 20, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, 22, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x2(ml1,mr1,ml2,mr2, rk1,rk2, 24, _a,_b,_c,_d, _bl,_sh,_mk); \
    /* Final round: compensate_sr then AESE without AESMC */ \
    (_a)=arm_vtbl16(vbslq_u8(_mk,(mr1),(ml1)),_sh); (_b)=arm_vtbl16(vbslq_u8(_mk,(ml1),(mr1)),_sh); \
    (_c)=arm_vtbl16(vbslq_u8(_mk,(mr2),(ml2)),_sh); (_d)=arm_vtbl16(vbslq_u8(_mk,(ml2),(mr2)),_sh); \
    (ml1)=vaeseq_u8(_a,(rk1)[26]); (mr1)=vaeseq_u8(_b,(rk1)[27]); \
    (ml2)=vaeseq_u8(_c,(rk2)[26]); (mr2)=vaeseq_u8(_d,(rk2)[27]); \
    /* Final AddRoundKey */ \
    (ml1)=veorq_u8((ml1),(rk1)[28]); (mr1)=veorq_u8((mr1),(rk1)[29]); \
    (ml2)=veorq_u8((ml2),(rk2)[28]); (mr2)=veorq_u8((mr2),(rk2)[29]); \
} while(0)

/*
 * _RJ256_FULL_ROUND_x4: one interleaved Rijndael-256 full round for 4 streams.
 * Reuses the same 4 temp scratch registers across both pairs by scheduling
 * all 4 streams' SR compensation before the AESE+AESMC outputs overwrite them.
 */
#define _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, ki, \
                              _a,_b,_c,_d, _bl,_sh,_mk) \
    (_a)=arm_vtbl16(vbslq_u8((_mk),(mr1),(ml1)),(_sh)); \
    (_b)=arm_vtbl16(vbslq_u8((_mk),(ml1),(mr1)),(_sh)); \
    (_c)=arm_vtbl16(vbslq_u8((_mk),(mr2),(ml2)),(_sh)); \
    (_d)=arm_vtbl16(vbslq_u8((_mk),(ml2),(mr2)),(_sh)); \
    (ml1)=vaesmcq_u8(vaeseq_u8((_a),(rk1)[(ki)]));   (mr1)=vaesmcq_u8(vaeseq_u8((_b),(rk1)[(ki)+1])); \
    (ml2)=vaesmcq_u8(vaeseq_u8((_c),(rk2)[(ki)]));   (mr2)=vaesmcq_u8(vaeseq_u8((_d),(rk2)[(ki)+1])); \
    (_a)=arm_vtbl16(vbslq_u8((_mk),(mr3),(ml3)),(_sh)); \
    (_b)=arm_vtbl16(vbslq_u8((_mk),(ml3),(mr3)),(_sh)); \
    (_c)=arm_vtbl16(vbslq_u8((_mk),(mr4),(ml4)),(_sh)); \
    (_d)=arm_vtbl16(vbslq_u8((_mk),(ml4),(mr4)),(_sh)); \
    (ml3)=vaesmcq_u8(vaeseq_u8((_a),(rk3)[(ki)]));   (mr3)=vaesmcq_u8(vaeseq_u8((_b),(rk3)[(ki)+1])); \
    (ml4)=vaesmcq_u8(vaeseq_u8((_c),(rk4)[(ki)]));   (mr4)=vaesmcq_u8(vaeseq_u8((_d),(rk4)[(ki)+1]))

/* Rijndael-256 encryption for 4 independent streams.
 * 4 state pairs (8 NEON regs) + 4 temps + 3 constants = 15 regs, fits in 32. */
#define DO_ENC_RIJNDAEL256_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4) do { \
    uint8x16_t _a, _b, _c, _d; \
    const uint8x16_t _bl = vld1q_u8(rijndael256_sr_blend); \
    const uint8x16_t _sh = vld1q_u8(rijndael256_sr_shuff); \
    const uint8x16_t _mk = (uint8x16_t)vshrq_n_s8((int8x16_t)_bl, 7); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4,  0, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4,  2, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4,  4, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4,  6, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4,  8, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, 10, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, 12, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, 14, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, 16, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, 18, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, 20, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, 22, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x4(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4, rk1,rk2,rk3,rk4, 24, _a,_b,_c,_d, _bl,_sh,_mk); \
    /* Final round: compensate_sr then AESE without AESMC */ \
    (_a)=arm_vtbl16(vbslq_u8(_mk,(mr1),(ml1)),_sh); (_b)=arm_vtbl16(vbslq_u8(_mk,(ml1),(mr1)),_sh); \
    (_c)=arm_vtbl16(vbslq_u8(_mk,(mr2),(ml2)),_sh); (_d)=arm_vtbl16(vbslq_u8(_mk,(ml2),(mr2)),_sh); \
    (ml1)=vaeseq_u8(_a,(rk1)[26]); (mr1)=vaeseq_u8(_b,(rk1)[27]); \
    (ml2)=vaeseq_u8(_c,(rk2)[26]); (mr2)=vaeseq_u8(_d,(rk2)[27]); \
    (_a)=arm_vtbl16(vbslq_u8(_mk,(mr3),(ml3)),_sh); (_b)=arm_vtbl16(vbslq_u8(_mk,(ml3),(mr3)),_sh); \
    (_c)=arm_vtbl16(vbslq_u8(_mk,(mr4),(ml4)),_sh); (_d)=arm_vtbl16(vbslq_u8(_mk,(ml4),(mr4)),_sh); \
    (ml3)=vaeseq_u8(_a,(rk3)[26]); (mr3)=vaeseq_u8(_b,(rk3)[27]); \
    (ml4)=vaeseq_u8(_c,(rk4)[26]); (mr4)=vaeseq_u8(_d,(rk4)[27]); \
    /* Final AddRoundKey */ \
    (ml1)=veorq_u8((ml1),(rk1)[28]); (mr1)=veorq_u8((mr1),(rk1)[29]); \
    (ml2)=veorq_u8((ml2),(rk2)[28]); (mr2)=veorq_u8((mr2),(rk2)[29]); \
    (ml3)=veorq_u8((ml3),(rk3)[28]); (mr3)=veorq_u8((mr3),(rk3)[29]); \
    (ml4)=veorq_u8((ml4),(rk4)[28]); (mr4)=veorq_u8((mr4),(rk4)[29]); \
} while(0)

/*
 * _RJ256_FULL_ROUND_x8: one interleaved Rijndael-256 full round for 8 streams,
 * mirroring _RJ256_FULL_ROUND_x4 above (4 back-to-back independent pairs
 * instead of 2). */
#define _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                              rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, ki, \
                              _a,_b,_c,_d, _bl,_sh,_mk) \
    (_a)=arm_vtbl16(vbslq_u8((_mk),(mr1),(ml1)),(_sh)); \
    (_b)=arm_vtbl16(vbslq_u8((_mk),(ml1),(mr1)),(_sh)); \
    (_c)=arm_vtbl16(vbslq_u8((_mk),(mr2),(ml2)),(_sh)); \
    (_d)=arm_vtbl16(vbslq_u8((_mk),(ml2),(mr2)),(_sh)); \
    (ml1)=vaesmcq_u8(vaeseq_u8((_a),(rk1)[(ki)]));   (mr1)=vaesmcq_u8(vaeseq_u8((_b),(rk1)[(ki)+1])); \
    (ml2)=vaesmcq_u8(vaeseq_u8((_c),(rk2)[(ki)]));   (mr2)=vaesmcq_u8(vaeseq_u8((_d),(rk2)[(ki)+1])); \
    (_a)=arm_vtbl16(vbslq_u8((_mk),(mr3),(ml3)),(_sh)); \
    (_b)=arm_vtbl16(vbslq_u8((_mk),(ml3),(mr3)),(_sh)); \
    (_c)=arm_vtbl16(vbslq_u8((_mk),(mr4),(ml4)),(_sh)); \
    (_d)=arm_vtbl16(vbslq_u8((_mk),(ml4),(mr4)),(_sh)); \
    (ml3)=vaesmcq_u8(vaeseq_u8((_a),(rk3)[(ki)]));   (mr3)=vaesmcq_u8(vaeseq_u8((_b),(rk3)[(ki)+1])); \
    (ml4)=vaesmcq_u8(vaeseq_u8((_c),(rk4)[(ki)]));   (mr4)=vaesmcq_u8(vaeseq_u8((_d),(rk4)[(ki)+1])); \
    (_a)=arm_vtbl16(vbslq_u8((_mk),(mr5),(ml5)),(_sh)); \
    (_b)=arm_vtbl16(vbslq_u8((_mk),(ml5),(mr5)),(_sh)); \
    (_c)=arm_vtbl16(vbslq_u8((_mk),(mr6),(ml6)),(_sh)); \
    (_d)=arm_vtbl16(vbslq_u8((_mk),(ml6),(mr6)),(_sh)); \
    (ml5)=vaesmcq_u8(vaeseq_u8((_a),(rk5)[(ki)]));   (mr5)=vaesmcq_u8(vaeseq_u8((_b),(rk5)[(ki)+1])); \
    (ml6)=vaesmcq_u8(vaeseq_u8((_c),(rk6)[(ki)]));   (mr6)=vaesmcq_u8(vaeseq_u8((_d),(rk6)[(ki)+1])); \
    (_a)=arm_vtbl16(vbslq_u8((_mk),(mr7),(ml7)),(_sh)); \
    (_b)=arm_vtbl16(vbslq_u8((_mk),(ml7),(mr7)),(_sh)); \
    (_c)=arm_vtbl16(vbslq_u8((_mk),(mr8),(ml8)),(_sh)); \
    (_d)=arm_vtbl16(vbslq_u8((_mk),(ml8),(mr8)),(_sh)); \
    (ml7)=vaesmcq_u8(vaeseq_u8((_a),(rk7)[(ki)]));   (mr7)=vaesmcq_u8(vaeseq_u8((_b),(rk7)[(ki)+1])); \
    (ml8)=vaesmcq_u8(vaeseq_u8((_c),(rk8)[(ki)]));   (mr8)=vaesmcq_u8(vaeseq_u8((_d),(rk8)[(ki)+1]))

/* Rijndael-256 encryption for 8 independent streams: single interleaved loop
 * (round-by-round across all 8 lanes) instead of two back-to-back calls to
 * DO_ENC_RIJNDAEL256_x4. */
#define DO_ENC_RIJNDAEL256_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                               rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8) do { \
    uint8x16_t _a, _b, _c, _d; \
    const uint8x16_t _bl = vld1q_u8(rijndael256_sr_blend); \
    const uint8x16_t _sh = vld1q_u8(rijndael256_sr_shuff); \
    const uint8x16_t _mk = (uint8x16_t)vshrq_n_s8((int8x16_t)_bl, 7); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8,  0, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8,  2, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8,  4, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8,  6, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8,  8, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, 10, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, 12, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, 14, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, 16, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, 18, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, 20, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, 22, _a,_b,_c,_d, _bl,_sh,_mk); \
    _RJ256_FULL_ROUND_x8(ml1,mr1,ml2,mr2,ml3,mr3,ml4,mr4,ml5,mr5,ml6,mr6,ml7,mr7,ml8,mr8, \
                          rk1,rk2,rk3,rk4,rk5,rk6,rk7,rk8, 24, _a,_b,_c,_d, _bl,_sh,_mk); \
    /* Final round: compensate_sr then AESE without AESMC */ \
    (_a)=arm_vtbl16(vbslq_u8(_mk,(mr1),(ml1)),_sh); (_b)=arm_vtbl16(vbslq_u8(_mk,(ml1),(mr1)),_sh); \
    (_c)=arm_vtbl16(vbslq_u8(_mk,(mr2),(ml2)),_sh); (_d)=arm_vtbl16(vbslq_u8(_mk,(ml2),(mr2)),_sh); \
    (ml1)=vaeseq_u8(_a,(rk1)[26]); (mr1)=vaeseq_u8(_b,(rk1)[27]); \
    (ml2)=vaeseq_u8(_c,(rk2)[26]); (mr2)=vaeseq_u8(_d,(rk2)[27]); \
    (_a)=arm_vtbl16(vbslq_u8(_mk,(mr3),(ml3)),_sh); (_b)=arm_vtbl16(vbslq_u8(_mk,(ml3),(mr3)),_sh); \
    (_c)=arm_vtbl16(vbslq_u8(_mk,(mr4),(ml4)),_sh); (_d)=arm_vtbl16(vbslq_u8(_mk,(ml4),(mr4)),_sh); \
    (ml3)=vaeseq_u8(_a,(rk3)[26]); (mr3)=vaeseq_u8(_b,(rk3)[27]); \
    (ml4)=vaeseq_u8(_c,(rk4)[26]); (mr4)=vaeseq_u8(_d,(rk4)[27]); \
    (_a)=arm_vtbl16(vbslq_u8(_mk,(mr5),(ml5)),_sh); (_b)=arm_vtbl16(vbslq_u8(_mk,(ml5),(mr5)),_sh); \
    (_c)=arm_vtbl16(vbslq_u8(_mk,(mr6),(ml6)),_sh); (_d)=arm_vtbl16(vbslq_u8(_mk,(ml6),(mr6)),_sh); \
    (ml5)=vaeseq_u8(_a,(rk5)[26]); (mr5)=vaeseq_u8(_b,(rk5)[27]); \
    (ml6)=vaeseq_u8(_c,(rk6)[26]); (mr6)=vaeseq_u8(_d,(rk6)[27]); \
    (_a)=arm_vtbl16(vbslq_u8(_mk,(mr7),(ml7)),_sh); (_b)=arm_vtbl16(vbslq_u8(_mk,(ml7),(mr7)),_sh); \
    (_c)=arm_vtbl16(vbslq_u8(_mk,(mr8),(ml8)),_sh); (_d)=arm_vtbl16(vbslq_u8(_mk,(ml8),(mr8)),_sh); \
    (ml7)=vaeseq_u8(_a,(rk7)[26]); (mr7)=vaeseq_u8(_b,(rk7)[27]); \
    (ml8)=vaeseq_u8(_c,(rk8)[26]); (mr8)=vaeseq_u8(_d,(rk8)[27]); \
    /* Final AddRoundKey */ \
    (ml1)=veorq_u8((ml1),(rk1)[28]); (mr1)=veorq_u8((mr1),(rk1)[29]); \
    (ml2)=veorq_u8((ml2),(rk2)[28]); (mr2)=veorq_u8((mr2),(rk2)[29]); \
    (ml3)=veorq_u8((ml3),(rk3)[28]); (mr3)=veorq_u8((mr3),(rk3)[29]); \
    (ml4)=veorq_u8((ml4),(rk4)[28]); (mr4)=veorq_u8((mr4),(rk4)[29]); \
    (ml5)=veorq_u8((ml5),(rk5)[28]); (mr5)=veorq_u8((mr5),(rk5)[29]); \
    (ml6)=veorq_u8((ml6),(rk6)[28]); (mr6)=veorq_u8((mr6),(rk6)[29]); \
    (ml7)=veorq_u8((ml7),(rk7)[28]); (mr7)=veorq_u8((mr7),(rk7)[29]); \
    (ml8)=veorq_u8((ml8),(rk8)[28]); (mr8)=veorq_u8((mr8),(rk8)[29]); \
} while(0)

/* ======== Constant-time key schedule via AESE ======== */

/*
 * arm_rk_xor_chain: vectorized AES/Rijndael key expansion XOR chain.
 *
 * Given base_rk = [w0, w1, w2, w3] (one 128-bit round key) and tv = the
 * SubWord/RotWord/Rcon result broadcast to all four 32-bit word lanes, computes
 * the next round key [w0^t, w0^w1^t, w0^w1^w2^t, w0^w1^w2^w3^t].
 *
 * ARM has no PSLLDQ equivalent, but vextq_u8(zero, x, 12) shifts x towards
 * higher word positions by one word (= inserts a zero word at the low end):
 *   vextq_u8(0, [w0,w1,w2,w3], 12) = [0, w0, w1, w2]
 * Three such shifts and XORs build the prefix-XOR chain, matching the x86
 * PSLLDQ trick used in hardware AES key schedule implementations.
 */
static inline uint8x16_t arm_rk_xor_chain(uint8x16_t base_rk, uint8x16_t tv)
{
    uint8x16_t zero = vdupq_n_u8(0);
    uint8x16_t s;

    s       = vextq_u8(zero, base_rk, 12); /* [0,  w0, w1, w2]         */
    base_rk = veorq_u8(base_rk, s);        /* [w0, w0^w1, w1^w2, w2^w3] */
    s       = vextq_u8(zero, s, 12);       /* [0,  0,  w0, w1]          */
    base_rk = veorq_u8(base_rk, s);        /* [w0, w0^w1, w0^w1^w2, w1^w2^w3] */
    s       = vextq_u8(zero, s, 12);       /* [0,  0,  0,  w0]          */
    base_rk = veorq_u8(base_rk, s);        /* [w0, w0^w1, w0^w1^w2, w0^w1^w2^w3] */
    return veorq_u8(base_rk, tv);
}

/*
 * arm_rk_step_rotword: one AES key expansion step with RotWord + SubWord + Rcon.
 * Used at every step for AES-128 (Nk=4), and at even steps for AES-256/Rijndael-256 (Nk=8).
 *
 * SubBytes is computed as: InvShiftRows(AESE(word, 0))  - constant-time, hardware-only.
 * No S-Box table; no data-dependent memory access.
 */
static inline uint8x16_t arm_rk_step_rotword(uint8x16_t base_rk,
                                               uint8x16_t prev_rk,
                                               uint8_t rcon)
{
    /* RotWord: bring bytes [13,14,15,12] of prev_rk to positions [0,1,2,3] */
    static const uint8_t rotword_shuf[16] = {
        13, 14, 15, 12,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0
    };
    uint8x16_t tw = arm_vtbl16(prev_rk, vld1q_u8(rotword_shuf));
    /* SubBytes via AESE + InvShiftRows (constant-time) */
    tw = vaeseq_u8(tw, vdupq_n_u8(0));
    tw = arm_vtbl16(tw, vld1q_u8(arm_aes_inv_sr));
    /* XOR rcon at byte 0 (rcon is a public constant, not secret) */
    tw = vsetq_lane_u8(vgetq_lane_u8(tw, 0) ^ rcon, tw, 0);
    /* Broadcast word 0 to all four word lanes */
    uint8x16_t tv = vreinterpretq_u8_u32(
        vdupq_lane_u32(vget_low_u32(vreinterpretq_u32_u8(tw)), 0));
    return arm_rk_xor_chain(base_rk, tv);
}

/*
 * arm_rk_step_norotword: one AES key expansion step with SubWord only (no RotWord, no Rcon).
 * Used at odd steps (i%8==4) for AES-256 and Rijndael-256 (Nk=8, Nk>6 rule).
 */
static inline uint8x16_t arm_rk_step_norotword(uint8x16_t base_rk,
                                                 uint8x16_t prev_rk)
{
    /* Extract last word (bytes 12-15) without rotation */
    static const uint8_t lastword_shuf[16] = {
        12, 13, 14, 15,   0, 0, 0, 0,   0, 0, 0, 0,   0, 0, 0, 0
    };
    uint8x16_t tw = arm_vtbl16(prev_rk, vld1q_u8(lastword_shuf));
    /* SubBytes via AESE + InvShiftRows (constant-time) */
    tw = vaeseq_u8(tw, vdupq_n_u8(0));
    tw = arm_vtbl16(tw, vld1q_u8(arm_aes_inv_sr));
    /* Broadcast word 0 */
    uint8x16_t tv = vreinterpretq_u8_u32(
        vdupq_lane_u32(vget_low_u32(vreinterpretq_u32_u8(tw)), 0));
    return arm_rk_xor_chain(base_rk, tv);
}

/* ======== Interleaved key-schedule steps for x2/x4/x8 SetKey functions ========
 *
 * arm_rk_step_rotword/arm_rk_step_norotword/rijndael256_compensate_sr above are
 * pure functions of their arguments: they only touch local temporaries and the
 * caller-supplied uint8x16_t values, with no state shared between calls. So N
 * calls that each work on their own lane's rk[] array compute exactly the same
 * values as N serial calls to the single-key SetKey functions - reordering them
 * (all lanes' round i before any lane's round i+1) changes program *order*
 * only, never *which* value is computed. This is the same trick as
 * _ARM_AES_STEP/DO_ENC_AES128_xN above: interleaving independent per-lane
 * chains gives the OOO engine multiple chains to overlap while covering the
 * AESE-of-zero + double vtbl16 + Rcon-detour latency of each
 * arm_rk_step_rotword/arm_rk_step_norotword call described above those
 * functions.
 *
 * _ARM_RK_ROTWORD_STEP(rk, i, rcon): AES-128 style step, rk[i] from rk[i-1].
 * _ARM_RK_ROTWORD_PAIR(rk, k, rcon) / _ARM_RK_NOROTWORD_PAIR(rk, k): AES-256 /
 * Rijndael-256 style steps (Nk=8), rk[2k]/rk[2k+1] from rk[2k-2]/rk[2k-1] -
 * same recurrence as arm_rk_step_rotword/norotword usage in aes256_arm_aes_setkey_enc
 * and rijndael256_arm_aes_setkey_enc below, just factored into a macro so the
 * per-lane call sites stay one-line and easy to compare against each other.
 * _ARM_RK_COMPENSATE_PAIR(rk, k): Rijndael-256 ShiftRows-compensation of
 * round-key pair k, applied (only) after all raw key words are expanded.
 */
#define _ARM_RK_ROTWORD_STEP(rk, i, rcon) \
    (rk)[(i)] = arm_rk_step_rotword((rk)[(i) - 1], (rk)[(i) - 1], (rcon))

#define _ARM_RK_ROTWORD_PAIR(rk, k, rcon) \
    (rk)[2 * (k)] = arm_rk_step_rotword((rk)[2 * (k) - 2], (rk)[2 * (k) - 1], (rcon))

#define _ARM_RK_NOROTWORD_PAIR(rk, k) \
    (rk)[2 * (k) + 1] = arm_rk_step_norotword((rk)[2 * (k) - 1], (rk)[2 * (k)])

#define _ARM_RK_COMPENSATE_PAIR(rk, k) do { \
    uint8x16_t _ctl, _ctr; \
    rijndael256_compensate_sr((rk)[2 * (k)], (rk)[2 * (k) + 1], &_ctl, &_ctr); \
    (rk)[2 * (k)] = _ctl; (rk)[2 * (k) + 1] = _ctr; \
} while(0)

/* ======== SetKey functions  - round keys computed directly into ctx->rk[] ======== */

/*
 * AES-128: Nk=4, Nr=10, 11 round keys.
 * Every step applies RotWord + SubWord + Rcon.
 */
WEAK int aes128_arm_aes_setkey_enc(rijndael_arm_aes_ctx_aes128 *ctx, const uint8_t key[16])
{
    ctx->rk[0]  = vld1q_u8(key);
    ctx->rk[1]  = arm_rk_step_rotword(ctx->rk[0],  ctx->rk[0],  aes_rcon[1]);
    ctx->rk[2]  = arm_rk_step_rotword(ctx->rk[1],  ctx->rk[1],  aes_rcon[2]);
    ctx->rk[3]  = arm_rk_step_rotword(ctx->rk[2],  ctx->rk[2],  aes_rcon[3]);
    ctx->rk[4]  = arm_rk_step_rotword(ctx->rk[3],  ctx->rk[3],  aes_rcon[4]);
    ctx->rk[5]  = arm_rk_step_rotword(ctx->rk[4],  ctx->rk[4],  aes_rcon[5]);
    ctx->rk[6]  = arm_rk_step_rotword(ctx->rk[5],  ctx->rk[5],  aes_rcon[6]);
    ctx->rk[7]  = arm_rk_step_rotword(ctx->rk[6],  ctx->rk[6],  aes_rcon[7]);
    ctx->rk[8]  = arm_rk_step_rotword(ctx->rk[7],  ctx->rk[7],  aes_rcon[8]);
    ctx->rk[9]  = arm_rk_step_rotword(ctx->rk[8],  ctx->rk[8],  aes_rcon[9]);
    ctx->rk[10] = arm_rk_step_rotword(ctx->rk[9],  ctx->rk[9],  aes_rcon[10]);
    ctx->rtype = AES128;
    return 0;
}

/*
 * AES-128 x2/x4/x8: round-by-round interleaved key schedule across 2/4/8
 * independent lanes (see the comment block above _ARM_RK_ROTWORD_STEP).
 * Each round is fully unrolled, same as aes128_arm_aes_setkey_enc above and
 * DO_ENC_AES128_x2/x4/x8 earlier in this file.
 */
WEAK int aes128_arm_aes_setkey_enc_x2(rijndael_arm_aes_ctx_aes128_x2 *ctx,
                                       const uint8_t key1[16], const uint8_t key2[16])
{
    uint8x16_t *rk1, *rk2;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk;
    rk2 = ctx->ctx[1].rk;
    rk1[0] = vld1q_u8(key1); rk2[0] = vld1q_u8(key2);
    _ARM_RK_ROTWORD_STEP(rk1, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_STEP(rk2, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_STEP(rk1, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_STEP(rk2, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_STEP(rk1, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_STEP(rk2, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_STEP(rk1, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_STEP(rk2, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_STEP(rk1, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_STEP(rk2, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_STEP(rk1, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_STEP(rk2, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_STEP(rk1, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_STEP(rk2, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_STEP(rk1, 8, aes_rcon[8]);   _ARM_RK_ROTWORD_STEP(rk2, 8, aes_rcon[8]);
    _ARM_RK_ROTWORD_STEP(rk1, 9, aes_rcon[9]);   _ARM_RK_ROTWORD_STEP(rk2, 9, aes_rcon[9]);
    _ARM_RK_ROTWORD_STEP(rk1, 10, aes_rcon[10]); _ARM_RK_ROTWORD_STEP(rk2, 10, aes_rcon[10]);
    ctx->ctx[0].rtype = AES128;
    ctx->ctx[1].rtype = AES128;
    return 0;
}

WEAK int aes128_arm_aes_setkey_enc_x4(rijndael_arm_aes_ctx_aes128_x4 *ctx,
                                       const uint8_t key1[16], const uint8_t key2[16],
                                       const uint8_t key3[16], const uint8_t key4[16])
{
    uint8x16_t *rk1, *rk2, *rk3, *rk4;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk; rk2 = ctx->ctx[1].rk;
    rk3 = ctx->ctx[2].rk; rk4 = ctx->ctx[3].rk;
    rk1[0] = vld1q_u8(key1); rk2[0] = vld1q_u8(key2);
    rk3[0] = vld1q_u8(key3); rk4[0] = vld1q_u8(key4);
    _ARM_RK_ROTWORD_STEP(rk1, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_STEP(rk2, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_STEP(rk3, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_STEP(rk4, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_STEP(rk1, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_STEP(rk2, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_STEP(rk3, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_STEP(rk4, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_STEP(rk1, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_STEP(rk2, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_STEP(rk3, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_STEP(rk4, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_STEP(rk1, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_STEP(rk2, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_STEP(rk3, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_STEP(rk4, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_STEP(rk1, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_STEP(rk2, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_STEP(rk3, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_STEP(rk4, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_STEP(rk1, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_STEP(rk2, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_STEP(rk3, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_STEP(rk4, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_STEP(rk1, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_STEP(rk2, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_STEP(rk3, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_STEP(rk4, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_STEP(rk1, 8, aes_rcon[8]);   _ARM_RK_ROTWORD_STEP(rk2, 8, aes_rcon[8]);
    _ARM_RK_ROTWORD_STEP(rk3, 8, aes_rcon[8]);   _ARM_RK_ROTWORD_STEP(rk4, 8, aes_rcon[8]);
    _ARM_RK_ROTWORD_STEP(rk1, 9, aes_rcon[9]);   _ARM_RK_ROTWORD_STEP(rk2, 9, aes_rcon[9]);
    _ARM_RK_ROTWORD_STEP(rk3, 9, aes_rcon[9]);   _ARM_RK_ROTWORD_STEP(rk4, 9, aes_rcon[9]);
    _ARM_RK_ROTWORD_STEP(rk1, 10, aes_rcon[10]); _ARM_RK_ROTWORD_STEP(rk2, 10, aes_rcon[10]);
    _ARM_RK_ROTWORD_STEP(rk3, 10, aes_rcon[10]); _ARM_RK_ROTWORD_STEP(rk4, 10, aes_rcon[10]);
    ctx->ctx[0].rtype = AES128; ctx->ctx[1].rtype = AES128;
    ctx->ctx[2].rtype = AES128; ctx->ctx[3].rtype = AES128;
    return 0;
}

WEAK int aes128_arm_aes_setkey_enc_x8(rijndael_arm_aes_ctx_aes128_x8 *ctx,
                                       const uint8_t key1[16], const uint8_t key2[16],
                                       const uint8_t key3[16], const uint8_t key4[16],
                                       const uint8_t key5[16], const uint8_t key6[16],
                                       const uint8_t key7[16], const uint8_t key8[16])
{
    uint8x16_t *rk1, *rk2, *rk3, *rk4, *rk5, *rk6, *rk7, *rk8;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk; rk2 = ctx->ctx[1].rk;
    rk3 = ctx->ctx[2].rk; rk4 = ctx->ctx[3].rk;
    rk5 = ctx->ctx[4].rk; rk6 = ctx->ctx[5].rk;
    rk7 = ctx->ctx[6].rk; rk8 = ctx->ctx[7].rk;
    rk1[0] = vld1q_u8(key1); rk2[0] = vld1q_u8(key2);
    rk3[0] = vld1q_u8(key3); rk4[0] = vld1q_u8(key4);
    rk5[0] = vld1q_u8(key5); rk6[0] = vld1q_u8(key6);
    rk7[0] = vld1q_u8(key7); rk8[0] = vld1q_u8(key8);
    _ARM_RK_ROTWORD_STEP(rk1, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_STEP(rk2, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_STEP(rk3, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_STEP(rk4, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_STEP(rk5, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_STEP(rk6, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_STEP(rk7, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_STEP(rk8, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_STEP(rk1, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_STEP(rk2, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_STEP(rk3, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_STEP(rk4, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_STEP(rk5, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_STEP(rk6, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_STEP(rk7, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_STEP(rk8, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_STEP(rk1, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_STEP(rk2, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_STEP(rk3, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_STEP(rk4, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_STEP(rk5, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_STEP(rk6, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_STEP(rk7, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_STEP(rk8, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_STEP(rk1, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_STEP(rk2, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_STEP(rk3, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_STEP(rk4, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_STEP(rk5, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_STEP(rk6, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_STEP(rk7, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_STEP(rk8, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_STEP(rk1, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_STEP(rk2, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_STEP(rk3, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_STEP(rk4, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_STEP(rk5, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_STEP(rk6, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_STEP(rk7, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_STEP(rk8, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_STEP(rk1, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_STEP(rk2, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_STEP(rk3, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_STEP(rk4, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_STEP(rk5, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_STEP(rk6, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_STEP(rk7, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_STEP(rk8, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_STEP(rk1, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_STEP(rk2, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_STEP(rk3, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_STEP(rk4, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_STEP(rk5, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_STEP(rk6, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_STEP(rk7, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_STEP(rk8, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_STEP(rk1, 8, aes_rcon[8]);   _ARM_RK_ROTWORD_STEP(rk2, 8, aes_rcon[8]);
    _ARM_RK_ROTWORD_STEP(rk3, 8, aes_rcon[8]);   _ARM_RK_ROTWORD_STEP(rk4, 8, aes_rcon[8]);
    _ARM_RK_ROTWORD_STEP(rk5, 8, aes_rcon[8]);   _ARM_RK_ROTWORD_STEP(rk6, 8, aes_rcon[8]);
    _ARM_RK_ROTWORD_STEP(rk7, 8, aes_rcon[8]);   _ARM_RK_ROTWORD_STEP(rk8, 8, aes_rcon[8]);
    _ARM_RK_ROTWORD_STEP(rk1, 9, aes_rcon[9]);   _ARM_RK_ROTWORD_STEP(rk2, 9, aes_rcon[9]);
    _ARM_RK_ROTWORD_STEP(rk3, 9, aes_rcon[9]);   _ARM_RK_ROTWORD_STEP(rk4, 9, aes_rcon[9]);
    _ARM_RK_ROTWORD_STEP(rk5, 9, aes_rcon[9]);   _ARM_RK_ROTWORD_STEP(rk6, 9, aes_rcon[9]);
    _ARM_RK_ROTWORD_STEP(rk7, 9, aes_rcon[9]);   _ARM_RK_ROTWORD_STEP(rk8, 9, aes_rcon[9]);
    _ARM_RK_ROTWORD_STEP(rk1, 10, aes_rcon[10]); _ARM_RK_ROTWORD_STEP(rk2, 10, aes_rcon[10]);
    _ARM_RK_ROTWORD_STEP(rk3, 10, aes_rcon[10]); _ARM_RK_ROTWORD_STEP(rk4, 10, aes_rcon[10]);
    _ARM_RK_ROTWORD_STEP(rk5, 10, aes_rcon[10]); _ARM_RK_ROTWORD_STEP(rk6, 10, aes_rcon[10]);
    _ARM_RK_ROTWORD_STEP(rk7, 10, aes_rcon[10]); _ARM_RK_ROTWORD_STEP(rk8, 10, aes_rcon[10]);
    ctx->ctx[0].rtype = AES128; ctx->ctx[1].rtype = AES128;
    ctx->ctx[2].rtype = AES128; ctx->ctx[3].rtype = AES128;
    ctx->ctx[4].rtype = AES128; ctx->ctx[5].rtype = AES128;
    ctx->ctx[6].rtype = AES128; ctx->ctx[7].rtype = AES128;
    return 0;
}

/*
 * AES-256: Nk=8, Nr=14, 15 round keys.
 * Even steps (new key from rk[2k-2]): RotWord + SubWord + Rcon using rk[2k-1] last word.
 * Odd steps  (new key from rk[2k-1]): SubWord only using rk[2k] last word.
 */
WEAK int aes256_arm_aes_setkey_enc(rijndael_arm_aes_ctx_aes256 *ctx, const uint8_t key[32])
{
    ctx->rk[0]  = vld1q_u8(key);
    ctx->rk[1]  = vld1q_u8(key + 16);
    ctx->rk[2]  = arm_rk_step_rotword  (ctx->rk[0],  ctx->rk[1],  aes_rcon[1]);
    ctx->rk[3]  = arm_rk_step_norotword(ctx->rk[1],  ctx->rk[2]);
    ctx->rk[4]  = arm_rk_step_rotword  (ctx->rk[2],  ctx->rk[3],  aes_rcon[2]);
    ctx->rk[5]  = arm_rk_step_norotword(ctx->rk[3],  ctx->rk[4]);
    ctx->rk[6]  = arm_rk_step_rotword  (ctx->rk[4],  ctx->rk[5],  aes_rcon[3]);
    ctx->rk[7]  = arm_rk_step_norotword(ctx->rk[5],  ctx->rk[6]);
    ctx->rk[8]  = arm_rk_step_rotword  (ctx->rk[6],  ctx->rk[7],  aes_rcon[4]);
    ctx->rk[9]  = arm_rk_step_norotword(ctx->rk[7],  ctx->rk[8]);
    ctx->rk[10] = arm_rk_step_rotword  (ctx->rk[8],  ctx->rk[9],  aes_rcon[5]);
    ctx->rk[11] = arm_rk_step_norotword(ctx->rk[9],  ctx->rk[10]);
    ctx->rk[12] = arm_rk_step_rotword  (ctx->rk[10], ctx->rk[11], aes_rcon[6]);
    ctx->rk[13] = arm_rk_step_norotword(ctx->rk[11], ctx->rk[12]);
    ctx->rk[14] = arm_rk_step_rotword  (ctx->rk[12], ctx->rk[13], aes_rcon[7]);
    ctx->rtype = AES256;
    return 0;
}

/*
 * AES-256 x2/x4/x8: round-by-round interleaved key schedule across 2/4/8
 * independent lanes (see the comment block above _ARM_RK_ROTWORD_STEP).
 * Same recurrence as aes256_arm_aes_setkey_enc above: k=1..7, rk[2k] via
 * _ARM_RK_ROTWORD_PAIR then rk[2k+1] via _ARM_RK_NOROTWORD_PAIR, except the
 * last step (k=7) only produces rk[14] - there is no rk[15] (array holds 15
 * round keys, indices 0..14), so the norotword half-step is skipped there,
 * exactly like the single-key version above.
 */
WEAK int aes256_arm_aes_setkey_enc_x2(rijndael_arm_aes_ctx_aes256_x2 *ctx,
                                       const uint8_t key1[32], const uint8_t key2[32])
{
    uint8x16_t *rk1, *rk2;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk;
    rk2 = ctx->ctx[1].rk;
    rk1[0] = vld1q_u8(key1);      rk2[0] = vld1q_u8(key2);
    rk1[1] = vld1q_u8(key1 + 16); rk2[1] = vld1q_u8(key2 + 16);
    _ARM_RK_ROTWORD_PAIR(rk1, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_PAIR(rk2, 1, aes_rcon[1]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 1);              _ARM_RK_NOROTWORD_PAIR(rk2, 1);
    _ARM_RK_ROTWORD_PAIR(rk1, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_PAIR(rk2, 2, aes_rcon[2]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 2);              _ARM_RK_NOROTWORD_PAIR(rk2, 2);
    _ARM_RK_ROTWORD_PAIR(rk1, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_PAIR(rk2, 3, aes_rcon[3]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 3);              _ARM_RK_NOROTWORD_PAIR(rk2, 3);
    _ARM_RK_ROTWORD_PAIR(rk1, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_PAIR(rk2, 4, aes_rcon[4]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 4);              _ARM_RK_NOROTWORD_PAIR(rk2, 4);
    _ARM_RK_ROTWORD_PAIR(rk1, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_PAIR(rk2, 5, aes_rcon[5]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 5);              _ARM_RK_NOROTWORD_PAIR(rk2, 5);
    _ARM_RK_ROTWORD_PAIR(rk1, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_PAIR(rk2, 6, aes_rcon[6]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 6);              _ARM_RK_NOROTWORD_PAIR(rk2, 6);
    _ARM_RK_ROTWORD_PAIR(rk1, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_PAIR(rk2, 7, aes_rcon[7]);
    ctx->ctx[0].rtype = AES256;
    ctx->ctx[1].rtype = AES256;
    return 0;
}

WEAK int aes256_arm_aes_setkey_enc_x4(rijndael_arm_aes_ctx_aes256_x4 *ctx,
                                       const uint8_t key1[32], const uint8_t key2[32],
                                       const uint8_t key3[32], const uint8_t key4[32])
{
    uint8x16_t *rk1, *rk2, *rk3, *rk4;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk; rk2 = ctx->ctx[1].rk;
    rk3 = ctx->ctx[2].rk; rk4 = ctx->ctx[3].rk;
    rk1[0] = vld1q_u8(key1);      rk2[0] = vld1q_u8(key2);
    rk3[0] = vld1q_u8(key3);      rk4[0] = vld1q_u8(key4);
    rk1[1] = vld1q_u8(key1 + 16); rk2[1] = vld1q_u8(key2 + 16);
    rk3[1] = vld1q_u8(key3 + 16); rk4[1] = vld1q_u8(key4 + 16);
    _ARM_RK_ROTWORD_PAIR(rk1, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_PAIR(rk2, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_PAIR(rk3, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_PAIR(rk4, 1, aes_rcon[1]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 1);              _ARM_RK_NOROTWORD_PAIR(rk2, 1);
    _ARM_RK_NOROTWORD_PAIR(rk3, 1);              _ARM_RK_NOROTWORD_PAIR(rk4, 1);
    _ARM_RK_ROTWORD_PAIR(rk1, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_PAIR(rk2, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_PAIR(rk3, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_PAIR(rk4, 2, aes_rcon[2]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 2);              _ARM_RK_NOROTWORD_PAIR(rk2, 2);
    _ARM_RK_NOROTWORD_PAIR(rk3, 2);              _ARM_RK_NOROTWORD_PAIR(rk4, 2);
    _ARM_RK_ROTWORD_PAIR(rk1, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_PAIR(rk2, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_PAIR(rk3, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_PAIR(rk4, 3, aes_rcon[3]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 3);              _ARM_RK_NOROTWORD_PAIR(rk2, 3);
    _ARM_RK_NOROTWORD_PAIR(rk3, 3);              _ARM_RK_NOROTWORD_PAIR(rk4, 3);
    _ARM_RK_ROTWORD_PAIR(rk1, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_PAIR(rk2, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_PAIR(rk3, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_PAIR(rk4, 4, aes_rcon[4]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 4);              _ARM_RK_NOROTWORD_PAIR(rk2, 4);
    _ARM_RK_NOROTWORD_PAIR(rk3, 4);              _ARM_RK_NOROTWORD_PAIR(rk4, 4);
    _ARM_RK_ROTWORD_PAIR(rk1, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_PAIR(rk2, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_PAIR(rk3, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_PAIR(rk4, 5, aes_rcon[5]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 5);              _ARM_RK_NOROTWORD_PAIR(rk2, 5);
    _ARM_RK_NOROTWORD_PAIR(rk3, 5);              _ARM_RK_NOROTWORD_PAIR(rk4, 5);
    _ARM_RK_ROTWORD_PAIR(rk1, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_PAIR(rk2, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_PAIR(rk3, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_PAIR(rk4, 6, aes_rcon[6]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 6);              _ARM_RK_NOROTWORD_PAIR(rk2, 6);
    _ARM_RK_NOROTWORD_PAIR(rk3, 6);              _ARM_RK_NOROTWORD_PAIR(rk4, 6);
    _ARM_RK_ROTWORD_PAIR(rk1, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_PAIR(rk2, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_PAIR(rk3, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_PAIR(rk4, 7, aes_rcon[7]);
    ctx->ctx[0].rtype = AES256; ctx->ctx[1].rtype = AES256;
    ctx->ctx[2].rtype = AES256; ctx->ctx[3].rtype = AES256;
    return 0;
}

WEAK int aes256_arm_aes_setkey_enc_x8(rijndael_arm_aes_ctx_aes256_x8 *ctx,
                                       const uint8_t key1[32], const uint8_t key2[32],
                                       const uint8_t key3[32], const uint8_t key4[32],
                                       const uint8_t key5[32], const uint8_t key6[32],
                                       const uint8_t key7[32], const uint8_t key8[32])
{
    uint8x16_t *rk1, *rk2, *rk3, *rk4, *rk5, *rk6, *rk7, *rk8;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk; rk2 = ctx->ctx[1].rk;
    rk3 = ctx->ctx[2].rk; rk4 = ctx->ctx[3].rk;
    rk5 = ctx->ctx[4].rk; rk6 = ctx->ctx[5].rk;
    rk7 = ctx->ctx[6].rk; rk8 = ctx->ctx[7].rk;
    rk1[0] = vld1q_u8(key1);      rk2[0] = vld1q_u8(key2);
    rk3[0] = vld1q_u8(key3);      rk4[0] = vld1q_u8(key4);
    rk5[0] = vld1q_u8(key5);      rk6[0] = vld1q_u8(key6);
    rk7[0] = vld1q_u8(key7);      rk8[0] = vld1q_u8(key8);
    rk1[1] = vld1q_u8(key1 + 16); rk2[1] = vld1q_u8(key2 + 16);
    rk3[1] = vld1q_u8(key3 + 16); rk4[1] = vld1q_u8(key4 + 16);
    rk5[1] = vld1q_u8(key5 + 16); rk6[1] = vld1q_u8(key6 + 16);
    rk7[1] = vld1q_u8(key7 + 16); rk8[1] = vld1q_u8(key8 + 16);
    _ARM_RK_ROTWORD_PAIR(rk1, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_PAIR(rk2, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_PAIR(rk3, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_PAIR(rk4, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_PAIR(rk5, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_PAIR(rk6, 1, aes_rcon[1]);
    _ARM_RK_ROTWORD_PAIR(rk7, 1, aes_rcon[1]);   _ARM_RK_ROTWORD_PAIR(rk8, 1, aes_rcon[1]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 1);              _ARM_RK_NOROTWORD_PAIR(rk2, 1);
    _ARM_RK_NOROTWORD_PAIR(rk3, 1);              _ARM_RK_NOROTWORD_PAIR(rk4, 1);
    _ARM_RK_NOROTWORD_PAIR(rk5, 1);              _ARM_RK_NOROTWORD_PAIR(rk6, 1);
    _ARM_RK_NOROTWORD_PAIR(rk7, 1);              _ARM_RK_NOROTWORD_PAIR(rk8, 1);
    _ARM_RK_ROTWORD_PAIR(rk1, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_PAIR(rk2, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_PAIR(rk3, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_PAIR(rk4, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_PAIR(rk5, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_PAIR(rk6, 2, aes_rcon[2]);
    _ARM_RK_ROTWORD_PAIR(rk7, 2, aes_rcon[2]);   _ARM_RK_ROTWORD_PAIR(rk8, 2, aes_rcon[2]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 2);              _ARM_RK_NOROTWORD_PAIR(rk2, 2);
    _ARM_RK_NOROTWORD_PAIR(rk3, 2);              _ARM_RK_NOROTWORD_PAIR(rk4, 2);
    _ARM_RK_NOROTWORD_PAIR(rk5, 2);              _ARM_RK_NOROTWORD_PAIR(rk6, 2);
    _ARM_RK_NOROTWORD_PAIR(rk7, 2);              _ARM_RK_NOROTWORD_PAIR(rk8, 2);
    _ARM_RK_ROTWORD_PAIR(rk1, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_PAIR(rk2, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_PAIR(rk3, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_PAIR(rk4, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_PAIR(rk5, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_PAIR(rk6, 3, aes_rcon[3]);
    _ARM_RK_ROTWORD_PAIR(rk7, 3, aes_rcon[3]);   _ARM_RK_ROTWORD_PAIR(rk8, 3, aes_rcon[3]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 3);              _ARM_RK_NOROTWORD_PAIR(rk2, 3);
    _ARM_RK_NOROTWORD_PAIR(rk3, 3);              _ARM_RK_NOROTWORD_PAIR(rk4, 3);
    _ARM_RK_NOROTWORD_PAIR(rk5, 3);              _ARM_RK_NOROTWORD_PAIR(rk6, 3);
    _ARM_RK_NOROTWORD_PAIR(rk7, 3);              _ARM_RK_NOROTWORD_PAIR(rk8, 3);
    _ARM_RK_ROTWORD_PAIR(rk1, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_PAIR(rk2, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_PAIR(rk3, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_PAIR(rk4, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_PAIR(rk5, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_PAIR(rk6, 4, aes_rcon[4]);
    _ARM_RK_ROTWORD_PAIR(rk7, 4, aes_rcon[4]);   _ARM_RK_ROTWORD_PAIR(rk8, 4, aes_rcon[4]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 4);              _ARM_RK_NOROTWORD_PAIR(rk2, 4);
    _ARM_RK_NOROTWORD_PAIR(rk3, 4);              _ARM_RK_NOROTWORD_PAIR(rk4, 4);
    _ARM_RK_NOROTWORD_PAIR(rk5, 4);              _ARM_RK_NOROTWORD_PAIR(rk6, 4);
    _ARM_RK_NOROTWORD_PAIR(rk7, 4);              _ARM_RK_NOROTWORD_PAIR(rk8, 4);
    _ARM_RK_ROTWORD_PAIR(rk1, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_PAIR(rk2, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_PAIR(rk3, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_PAIR(rk4, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_PAIR(rk5, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_PAIR(rk6, 5, aes_rcon[5]);
    _ARM_RK_ROTWORD_PAIR(rk7, 5, aes_rcon[5]);   _ARM_RK_ROTWORD_PAIR(rk8, 5, aes_rcon[5]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 5);              _ARM_RK_NOROTWORD_PAIR(rk2, 5);
    _ARM_RK_NOROTWORD_PAIR(rk3, 5);              _ARM_RK_NOROTWORD_PAIR(rk4, 5);
    _ARM_RK_NOROTWORD_PAIR(rk5, 5);              _ARM_RK_NOROTWORD_PAIR(rk6, 5);
    _ARM_RK_NOROTWORD_PAIR(rk7, 5);              _ARM_RK_NOROTWORD_PAIR(rk8, 5);
    _ARM_RK_ROTWORD_PAIR(rk1, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_PAIR(rk2, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_PAIR(rk3, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_PAIR(rk4, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_PAIR(rk5, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_PAIR(rk6, 6, aes_rcon[6]);
    _ARM_RK_ROTWORD_PAIR(rk7, 6, aes_rcon[6]);   _ARM_RK_ROTWORD_PAIR(rk8, 6, aes_rcon[6]);
    _ARM_RK_NOROTWORD_PAIR(rk1, 6);              _ARM_RK_NOROTWORD_PAIR(rk2, 6);
    _ARM_RK_NOROTWORD_PAIR(rk3, 6);              _ARM_RK_NOROTWORD_PAIR(rk4, 6);
    _ARM_RK_NOROTWORD_PAIR(rk5, 6);              _ARM_RK_NOROTWORD_PAIR(rk6, 6);
    _ARM_RK_NOROTWORD_PAIR(rk7, 6);              _ARM_RK_NOROTWORD_PAIR(rk8, 6);
    _ARM_RK_ROTWORD_PAIR(rk1, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_PAIR(rk2, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_PAIR(rk3, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_PAIR(rk4, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_PAIR(rk5, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_PAIR(rk6, 7, aes_rcon[7]);
    _ARM_RK_ROTWORD_PAIR(rk7, 7, aes_rcon[7]);   _ARM_RK_ROTWORD_PAIR(rk8, 7, aes_rcon[7]);
    ctx->ctx[0].rtype = AES256; ctx->ctx[1].rtype = AES256;
    ctx->ctx[2].rtype = AES256; ctx->ctx[3].rtype = AES256;
    ctx->ctx[4].rtype = AES256; ctx->ctx[5].rtype = AES256;
    ctx->ctx[6].rtype = AES256; ctx->ctx[7].rtype = AES256;
    return 0;
}

/*
 * Rijndael-256: Nb=8, Nk=8, Nr=14 -> 30 half-block (128-bit) round keys.
 * The word expansion rule is identical to AES-256 (Nk=8), but runs for
 * Nbx(Nr+1) = 8x15 = 120 words instead of 4x15 = 60, needing 14 pairs
 * of steps rather than 7. Rcon indices go up to 14.
 *
 * ARM AESE XOR semantics differ from AES-NI:
 *   ARM:    AESE(state, rk) = ShiftRows(SubBytes(state XOR rk))   <- XOR BEFORE SubBytes
 *   AES-NI: AESENC(state, rk) = MixColumns(ShiftRows(SubBytes(state))) XOR rk <- XOR AFTER
 *
 * The compensate_sr step rearranges state bytes so AES ShiftRows emulates Rijndael-256
 * ShiftRows. Because SubBytes is a point-wise S-box (commutes with byte permutations),
 * the round key XOR that happens INSIDE AESE must be applied to the PERMUTED state.
 * This requires rk = compensate_sr(raw_key_pair): pre-permuting the round keys so that
 * P(state) XOR P(key) = P(state XOR key).
 *
 * Therefore, after expanding the standard Rijndael-256 round keys, we apply
 * compensate_sr to every pair that is used inside AESE (pairs 0..13 = rk[0..27]).
 * The final pair (rk[28..29]) is used in a plain XOR (final AddRoundKey), so it
 * must remain unpermuted.
 */
WEAK int rijndael256_arm_aes_setkey_enc(rijndael_arm_aes_ctx_rijndael256 *ctx,
                                         const uint8_t key[32])
{
    int k;
    uint8x16_t tl, tr;

    ctx->rk[0] = vld1q_u8(key);
    ctx->rk[1] = vld1q_u8(key + 16);
    for(k = 1; k <= 14; k++) {
        ctx->rk[2 * k]     = arm_rk_step_rotword  (ctx->rk[2 * k - 2],
                                                    ctx->rk[2 * k - 1],
                                                    aes_rcon[k]);
        ctx->rk[2 * k + 1] = arm_rk_step_norotword(ctx->rk[2 * k - 1],
                                                    ctx->rk[2 * k]);
    }
    /* Pre-permute pairs 0..13 (rk[0..27]) via compensate_sr so that AESE's
     * early XOR operates on the correctly permuted key material. */
    for(k = 0; k <= 13; k++) {
        rijndael256_compensate_sr(ctx->rk[2 * k], ctx->rk[2 * k + 1], &tl, &tr);
        ctx->rk[2 * k]     = tl;
        ctx->rk[2 * k + 1] = tr;
    }
    /* rk[28] and rk[29] (pair 14) stay unpermuted: used in plain XOR for
     * the final AddRoundKey, not inside AESE. */
    ctx->rtype = RIJNDAEL_256_256;
    return 0;
}

/*
 * Rijndael-256 x2/x4/x8: round-by-round interleaved key schedule across 2/4/8
 * independent lanes (see the comment block above _ARM_RK_ROTWORD_STEP). */
WEAK int rijndael256_arm_aes_setkey_enc_x2(rijndael_arm_aes_ctx_rijndael256_x2 *ctx,
                                            const uint8_t key1[32], const uint8_t key2[32])
{
    uint8x16_t *rk1, *rk2;
    int k;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk;
    rk2 = ctx->ctx[1].rk;
    rk1[0] = vld1q_u8(key1);      rk2[0] = vld1q_u8(key2);
    rk1[1] = vld1q_u8(key1 + 16); rk2[1] = vld1q_u8(key2 + 16);
    for(k = 1; k <= 14; k++) {
        _ARM_RK_ROTWORD_PAIR(rk1, k, aes_rcon[k]);   _ARM_RK_ROTWORD_PAIR(rk2, k, aes_rcon[k]);
        _ARM_RK_NOROTWORD_PAIR(rk1, k);              _ARM_RK_NOROTWORD_PAIR(rk2, k);
    }
    /* Pre-permute pairs 0..13 (rk[0..27]) via compensate_sr, per-lane, same
     * reasoning as the single-key setkey above; pair 14 (rk[28..29]) stays
     * unpermuted. */
    for(k = 0; k <= 13; k++) {
        _ARM_RK_COMPENSATE_PAIR(rk1, k); _ARM_RK_COMPENSATE_PAIR(rk2, k);
    }
    ctx->ctx[0].rtype = RIJNDAEL_256_256;
    ctx->ctx[1].rtype = RIJNDAEL_256_256;
    return 0;
}

WEAK int rijndael256_arm_aes_setkey_enc_x4(rijndael_arm_aes_ctx_rijndael256_x4 *ctx,
                                            const uint8_t key1[32], const uint8_t key2[32],
                                            const uint8_t key3[32], const uint8_t key4[32])
{
    uint8x16_t *rk1, *rk2, *rk3, *rk4;
    int k;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk; rk2 = ctx->ctx[1].rk;
    rk3 = ctx->ctx[2].rk; rk4 = ctx->ctx[3].rk;
    rk1[0] = vld1q_u8(key1);      rk2[0] = vld1q_u8(key2);
    rk3[0] = vld1q_u8(key3);      rk4[0] = vld1q_u8(key4);
    rk1[1] = vld1q_u8(key1 + 16); rk2[1] = vld1q_u8(key2 + 16);
    rk3[1] = vld1q_u8(key3 + 16); rk4[1] = vld1q_u8(key4 + 16);
    for(k = 1; k <= 14; k++) {
        _ARM_RK_ROTWORD_PAIR(rk1, k, aes_rcon[k]);   _ARM_RK_ROTWORD_PAIR(rk2, k, aes_rcon[k]);
        _ARM_RK_ROTWORD_PAIR(rk3, k, aes_rcon[k]);   _ARM_RK_ROTWORD_PAIR(rk4, k, aes_rcon[k]);
        _ARM_RK_NOROTWORD_PAIR(rk1, k);              _ARM_RK_NOROTWORD_PAIR(rk2, k);
        _ARM_RK_NOROTWORD_PAIR(rk3, k);              _ARM_RK_NOROTWORD_PAIR(rk4, k);
    }
    for(k = 0; k <= 13; k++) {
        _ARM_RK_COMPENSATE_PAIR(rk1, k); _ARM_RK_COMPENSATE_PAIR(rk2, k);
        _ARM_RK_COMPENSATE_PAIR(rk3, k); _ARM_RK_COMPENSATE_PAIR(rk4, k);
    }
    ctx->ctx[0].rtype = RIJNDAEL_256_256; ctx->ctx[1].rtype = RIJNDAEL_256_256;
    ctx->ctx[2].rtype = RIJNDAEL_256_256; ctx->ctx[3].rtype = RIJNDAEL_256_256;
    return 0;
}

WEAK int rijndael256_arm_aes_setkey_enc_x8(rijndael_arm_aes_ctx_rijndael256_x8 *ctx,
                                            const uint8_t key1[32], const uint8_t key2[32],
                                            const uint8_t key3[32], const uint8_t key4[32],
                                            const uint8_t key5[32], const uint8_t key6[32],
                                            const uint8_t key7[32], const uint8_t key8[32])
{
    uint8x16_t *rk1, *rk2, *rk3, *rk4, *rk5, *rk6, *rk7, *rk8;
    int k;
    if (ctx == NULL) {
        return -1;
    }
    rk1 = ctx->ctx[0].rk; rk2 = ctx->ctx[1].rk;
    rk3 = ctx->ctx[2].rk; rk4 = ctx->ctx[3].rk;
    rk5 = ctx->ctx[4].rk; rk6 = ctx->ctx[5].rk;
    rk7 = ctx->ctx[6].rk; rk8 = ctx->ctx[7].rk;
    rk1[0] = vld1q_u8(key1);      rk2[0] = vld1q_u8(key2);
    rk3[0] = vld1q_u8(key3);      rk4[0] = vld1q_u8(key4);
    rk5[0] = vld1q_u8(key5);      rk6[0] = vld1q_u8(key6);
    rk7[0] = vld1q_u8(key7);      rk8[0] = vld1q_u8(key8);
    rk1[1] = vld1q_u8(key1 + 16); rk2[1] = vld1q_u8(key2 + 16);
    rk3[1] = vld1q_u8(key3 + 16); rk4[1] = vld1q_u8(key4 + 16);
    rk5[1] = vld1q_u8(key5 + 16); rk6[1] = vld1q_u8(key6 + 16);
    rk7[1] = vld1q_u8(key7 + 16); rk8[1] = vld1q_u8(key8 + 16);
    for(k = 1; k <= 14; k++) {
        _ARM_RK_ROTWORD_PAIR(rk1, k, aes_rcon[k]);   _ARM_RK_ROTWORD_PAIR(rk2, k, aes_rcon[k]);
        _ARM_RK_ROTWORD_PAIR(rk3, k, aes_rcon[k]);   _ARM_RK_ROTWORD_PAIR(rk4, k, aes_rcon[k]);
        _ARM_RK_ROTWORD_PAIR(rk5, k, aes_rcon[k]);   _ARM_RK_ROTWORD_PAIR(rk6, k, aes_rcon[k]);
        _ARM_RK_ROTWORD_PAIR(rk7, k, aes_rcon[k]);   _ARM_RK_ROTWORD_PAIR(rk8, k, aes_rcon[k]);
        _ARM_RK_NOROTWORD_PAIR(rk1, k);              _ARM_RK_NOROTWORD_PAIR(rk2, k);
        _ARM_RK_NOROTWORD_PAIR(rk3, k);              _ARM_RK_NOROTWORD_PAIR(rk4, k);
        _ARM_RK_NOROTWORD_PAIR(rk5, k);              _ARM_RK_NOROTWORD_PAIR(rk6, k);
        _ARM_RK_NOROTWORD_PAIR(rk7, k);              _ARM_RK_NOROTWORD_PAIR(rk8, k);
    }
    for(k = 0; k <= 13; k++) {
        _ARM_RK_COMPENSATE_PAIR(rk1, k); _ARM_RK_COMPENSATE_PAIR(rk2, k);
        _ARM_RK_COMPENSATE_PAIR(rk3, k); _ARM_RK_COMPENSATE_PAIR(rk4, k);
        _ARM_RK_COMPENSATE_PAIR(rk5, k); _ARM_RK_COMPENSATE_PAIR(rk6, k);
        _ARM_RK_COMPENSATE_PAIR(rk7, k); _ARM_RK_COMPENSATE_PAIR(rk8, k);
    }
    ctx->ctx[0].rtype = RIJNDAEL_256_256; ctx->ctx[1].rtype = RIJNDAEL_256_256;
    ctx->ctx[2].rtype = RIJNDAEL_256_256; ctx->ctx[3].rtype = RIJNDAEL_256_256;
    ctx->ctx[4].rtype = RIJNDAEL_256_256; ctx->ctx[5].rtype = RIJNDAEL_256_256;
    ctx->ctx[6].rtype = RIJNDAEL_256_256; ctx->ctx[7].rtype = RIJNDAEL_256_256;
    return 0;
}

/* ======== Single-block encryption functions ======== */

WEAK int aes128_arm_aes_enc(const rijndael_arm_aes_ctx_aes128 *ctx,
                             const uint8_t data_in[16], uint8_t data_out[16])
{
    uint8x16_t s;
    if ((ctx == NULL) || (ctx->rtype != AES128)) {
        return -1;
    }
    s = vld1q_u8(data_in);
    DO_ENC_AES128(s, ctx->rk);
    vst1q_u8(data_out, s);
    return 0;
}

WEAK int aes256_arm_aes_enc(const rijndael_arm_aes_ctx_aes256 *ctx,
                             const uint8_t data_in[16], uint8_t data_out[16])
{
    uint8x16_t s;
    if ((ctx == NULL) || (ctx->rtype != AES256)) {
        return -1;
    }
    s = vld1q_u8(data_in);
    DO_ENC_AES256(s, ctx->rk);
    vst1q_u8(data_out, s);
    return 0;
}

WEAK int rijndael256_arm_aes_enc(const rijndael_arm_aes_ctx_rijndael256 *ctx,
                                  const uint8_t data_in[32], uint8_t data_out[32])
{
    uint8x16_t ml, mr;
    if ((ctx == NULL) || (ctx->rtype != RIJNDAEL_256_256)) {
        return -1;
    }
    ml = vld1q_u8(data_in);
    mr = vld1q_u8(data_in + 16);
    DO_ENC_RIJNDAEL256(ml, mr, ctx->rk);
    vst1q_u8(data_out, ml);
    vst1q_u8(data_out + 16, mr);
    return 0;
}

/* ======== x2 multi-context encryption (round-by-round interleaved) ======== */

WEAK int aes128_arm_aes_enc_x2(const rijndael_arm_aes_ctx_aes128 *ctx1,
                                const rijndael_arm_aes_ctx_aes128 *ctx2,
                                const uint8_t pt1[16], const uint8_t pt2[16],
                                uint8_t ct1[16], uint8_t ct2[16])
{
    uint8x16_t s1, s2;
    if ((ctx1 == NULL) || (ctx1->rtype != AES128) || (ctx2 == NULL) || (ctx2->rtype != AES128)) {
        return -1;
    }
    s1 = vld1q_u8(pt1); s2 = vld1q_u8(pt2);
    DO_ENC_AES128_x2(s1, s2, ctx1->rk, ctx2->rk);
    vst1q_u8(ct1, s1); vst1q_u8(ct2, s2);
    return 0;
}

WEAK int aes256_arm_aes_enc_x2(const rijndael_arm_aes_ctx_aes256 *ctx1,
                                const rijndael_arm_aes_ctx_aes256 *ctx2,
                                const uint8_t pt1[16], const uint8_t pt2[16],
                                uint8_t ct1[16], uint8_t ct2[16])
{
    uint8x16_t s1, s2;
    if ((ctx1 == NULL) || (ctx1->rtype != AES256) || (ctx2 == NULL) || (ctx2->rtype != AES256)) {
        return -1;
    }
    s1 = vld1q_u8(pt1); s2 = vld1q_u8(pt2);
    DO_ENC_AES256_x2(s1, s2, ctx1->rk, ctx2->rk);
    vst1q_u8(ct1, s1); vst1q_u8(ct2, s2);
    return 0;
}

WEAK int rijndael256_arm_aes_enc_x2(const rijndael_arm_aes_ctx_rijndael256 *ctx1,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx2,
                                     const uint8_t pt1[32], const uint8_t pt2[32],
                                     uint8_t ct1[32], uint8_t ct2[32])
{
    uint8x16_t ml1, mr1, ml2, mr2;
    if ((ctx1 == NULL) || (ctx1->rtype != RIJNDAEL_256_256) || (ctx2 == NULL) || (ctx2->rtype != RIJNDAEL_256_256)) {
        return -1;
    }
    ml1 = vld1q_u8(pt1);      mr1 = vld1q_u8(pt1 + 16);
    ml2 = vld1q_u8(pt2);      mr2 = vld1q_u8(pt2 + 16);
    DO_ENC_RIJNDAEL256_x2(ml1, mr1, ml2, mr2, ctx1->rk, ctx2->rk);
    vst1q_u8(ct1, ml1); vst1q_u8(ct1 + 16, mr1);
    vst1q_u8(ct2, ml2); vst1q_u8(ct2 + 16, mr2);
    return 0;
}

/* ======== x4 multi-context encryption (round-by-round interleaved) ======== */

WEAK int aes128_arm_aes_enc_x4(const rijndael_arm_aes_ctx_aes128 *ctx1,
                                const rijndael_arm_aes_ctx_aes128 *ctx2,
                                const rijndael_arm_aes_ctx_aes128 *ctx3,
                                const rijndael_arm_aes_ctx_aes128 *ctx4,
                                const uint8_t pt1[16], const uint8_t pt2[16],
                                const uint8_t pt3[16], const uint8_t pt4[16],
                                uint8_t ct1[16], uint8_t ct2[16],
                                uint8_t ct3[16], uint8_t ct4[16])
{
    uint8x16_t s1, s2, s3, s4;
    if ((ctx1 == NULL) || (ctx1->rtype != AES128) || (ctx2 == NULL) || (ctx2->rtype != AES128) ||
        (ctx3 == NULL) || (ctx3->rtype != AES128) || (ctx4 == NULL) || (ctx4->rtype != AES128)) {
        return -1;
    }
    s1 = vld1q_u8(pt1); s2 = vld1q_u8(pt2);
    s3 = vld1q_u8(pt3); s4 = vld1q_u8(pt4);
    DO_ENC_AES128_x4(s1, s2, s3, s4, ctx1->rk, ctx2->rk, ctx3->rk, ctx4->rk);
    vst1q_u8(ct1, s1); vst1q_u8(ct2, s2); vst1q_u8(ct3, s3); vst1q_u8(ct4, s4);
    return 0;
}

WEAK int aes256_arm_aes_enc_x4(const rijndael_arm_aes_ctx_aes256 *ctx1,
                                const rijndael_arm_aes_ctx_aes256 *ctx2,
                                const rijndael_arm_aes_ctx_aes256 *ctx3,
                                const rijndael_arm_aes_ctx_aes256 *ctx4,
                                const uint8_t pt1[16], const uint8_t pt2[16],
                                const uint8_t pt3[16], const uint8_t pt4[16],
                                uint8_t ct1[16], uint8_t ct2[16],
                                uint8_t ct3[16], uint8_t ct4[16])
{
    uint8x16_t s1, s2, s3, s4;
    if ((ctx1 == NULL) || (ctx1->rtype != AES256) || (ctx2 == NULL) || (ctx2->rtype != AES256) ||
        (ctx3 == NULL) || (ctx3->rtype != AES256) || (ctx4 == NULL) || (ctx4->rtype != AES256)) {
        return -1;
    }
    s1 = vld1q_u8(pt1); s2 = vld1q_u8(pt2);
    s3 = vld1q_u8(pt3); s4 = vld1q_u8(pt4);
    DO_ENC_AES256_x4(s1, s2, s3, s4, ctx1->rk, ctx2->rk, ctx3->rk, ctx4->rk);
    vst1q_u8(ct1, s1); vst1q_u8(ct2, s2); vst1q_u8(ct3, s3); vst1q_u8(ct4, s4);
    return 0;
}

WEAK int rijndael256_arm_aes_enc_x4(const rijndael_arm_aes_ctx_rijndael256 *ctx1,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx2,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx3,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx4,
                                     const uint8_t pt1[32], const uint8_t pt2[32],
                                     const uint8_t pt3[32], const uint8_t pt4[32],
                                     uint8_t ct1[32], uint8_t ct2[32],
                                     uint8_t ct3[32], uint8_t ct4[32])
{
    uint8x16_t ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4;
    if ((ctx1 == NULL) || (ctx1->rtype != RIJNDAEL_256_256) || (ctx2 == NULL) || (ctx2->rtype != RIJNDAEL_256_256) ||
        (ctx3 == NULL) || (ctx3->rtype != RIJNDAEL_256_256) || (ctx4 == NULL) || (ctx4->rtype != RIJNDAEL_256_256)) {
        return -1;
    }
    ml1 = vld1q_u8(pt1);      mr1 = vld1q_u8(pt1 + 16);
    ml2 = vld1q_u8(pt2);      mr2 = vld1q_u8(pt2 + 16);
    ml3 = vld1q_u8(pt3);      mr3 = vld1q_u8(pt3 + 16);
    ml4 = vld1q_u8(pt4);      mr4 = vld1q_u8(pt4 + 16);
    DO_ENC_RIJNDAEL256_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4,
                          ctx1->rk, ctx2->rk, ctx3->rk, ctx4->rk);
    vst1q_u8(ct1, ml1); vst1q_u8(ct1 + 16, mr1);
    vst1q_u8(ct2, ml2); vst1q_u8(ct2 + 16, mr2);
    vst1q_u8(ct3, ml3); vst1q_u8(ct3 + 16, mr3);
    vst1q_u8(ct4, ml4); vst1q_u8(ct4 + 16, mr4);
    return 0;
}

/* ======== x8 multi-context encryption (round-by-round interleaved) ======== */

WEAK int aes128_arm_aes_enc_x8(const rijndael_arm_aes_ctx_aes128 *ctx1,
                                const rijndael_arm_aes_ctx_aes128 *ctx2,
                                const rijndael_arm_aes_ctx_aes128 *ctx3,
                                const rijndael_arm_aes_ctx_aes128 *ctx4,
                                const rijndael_arm_aes_ctx_aes128 *ctx5,
                                const rijndael_arm_aes_ctx_aes128 *ctx6,
                                const rijndael_arm_aes_ctx_aes128 *ctx7,
                                const rijndael_arm_aes_ctx_aes128 *ctx8,
                                const uint8_t pt1[16], const uint8_t pt2[16],
                                const uint8_t pt3[16], const uint8_t pt4[16],
                                const uint8_t pt5[16], const uint8_t pt6[16],
                                const uint8_t pt7[16], const uint8_t pt8[16],
                                uint8_t ct1[16], uint8_t ct2[16],
                                uint8_t ct3[16], uint8_t ct4[16],
                                uint8_t ct5[16], uint8_t ct6[16],
                                uint8_t ct7[16], uint8_t ct8[16])
{
    uint8x16_t s1, s2, s3, s4, s5, s6, s7, s8;
    if ((ctx1 == NULL) || (ctx1->rtype != AES128) || (ctx2 == NULL) || (ctx2->rtype != AES128) ||
        (ctx3 == NULL) || (ctx3->rtype != AES128) || (ctx4 == NULL) || (ctx4->rtype != AES128) ||
        (ctx5 == NULL) || (ctx5->rtype != AES128) || (ctx6 == NULL) || (ctx6->rtype != AES128) ||
        (ctx7 == NULL) || (ctx7->rtype != AES128) || (ctx8 == NULL) || (ctx8->rtype != AES128)) {
        return -1;
    }
    s1 = vld1q_u8(pt1); s2 = vld1q_u8(pt2);
    s3 = vld1q_u8(pt3); s4 = vld1q_u8(pt4);
    s5 = vld1q_u8(pt5); s6 = vld1q_u8(pt6);
    s7 = vld1q_u8(pt7); s8 = vld1q_u8(pt8);
    DO_ENC_AES128_x8(s1, s2, s3, s4, s5, s6, s7, s8,
                     ctx1->rk, ctx2->rk, ctx3->rk, ctx4->rk,
                     ctx5->rk, ctx6->rk, ctx7->rk, ctx8->rk);
    vst1q_u8(ct1, s1); vst1q_u8(ct2, s2); vst1q_u8(ct3, s3); vst1q_u8(ct4, s4);
    vst1q_u8(ct5, s5); vst1q_u8(ct6, s6); vst1q_u8(ct7, s7); vst1q_u8(ct8, s8);
    return 0;
}

WEAK int aes256_arm_aes_enc_x8(const rijndael_arm_aes_ctx_aes256 *ctx1,
                                const rijndael_arm_aes_ctx_aes256 *ctx2,
                                const rijndael_arm_aes_ctx_aes256 *ctx3,
                                const rijndael_arm_aes_ctx_aes256 *ctx4,
                                const rijndael_arm_aes_ctx_aes256 *ctx5,
                                const rijndael_arm_aes_ctx_aes256 *ctx6,
                                const rijndael_arm_aes_ctx_aes256 *ctx7,
                                const rijndael_arm_aes_ctx_aes256 *ctx8,
                                const uint8_t pt1[16], const uint8_t pt2[16],
                                const uint8_t pt3[16], const uint8_t pt4[16],
                                const uint8_t pt5[16], const uint8_t pt6[16],
                                const uint8_t pt7[16], const uint8_t pt8[16],
                                uint8_t ct1[16], uint8_t ct2[16],
                                uint8_t ct3[16], uint8_t ct4[16],
                                uint8_t ct5[16], uint8_t ct6[16],
                                uint8_t ct7[16], uint8_t ct8[16])
{
    uint8x16_t s1, s2, s3, s4, s5, s6, s7, s8;
    if ((ctx1 == NULL) || (ctx1->rtype != AES256) || (ctx2 == NULL) || (ctx2->rtype != AES256) ||
        (ctx3 == NULL) || (ctx3->rtype != AES256) || (ctx4 == NULL) || (ctx4->rtype != AES256) ||
        (ctx5 == NULL) || (ctx5->rtype != AES256) || (ctx6 == NULL) || (ctx6->rtype != AES256) ||
        (ctx7 == NULL) || (ctx7->rtype != AES256) || (ctx8 == NULL) || (ctx8->rtype != AES256)) {
        return -1;
    }
    s1 = vld1q_u8(pt1); s2 = vld1q_u8(pt2);
    s3 = vld1q_u8(pt3); s4 = vld1q_u8(pt4);
    s5 = vld1q_u8(pt5); s6 = vld1q_u8(pt6);
    s7 = vld1q_u8(pt7); s8 = vld1q_u8(pt8);
    DO_ENC_AES256_x8(s1, s2, s3, s4, s5, s6, s7, s8,
                     ctx1->rk, ctx2->rk, ctx3->rk, ctx4->rk,
                     ctx5->rk, ctx6->rk, ctx7->rk, ctx8->rk);
    vst1q_u8(ct1, s1); vst1q_u8(ct2, s2); vst1q_u8(ct3, s3); vst1q_u8(ct4, s4);
    vst1q_u8(ct5, s5); vst1q_u8(ct6, s6); vst1q_u8(ct7, s7); vst1q_u8(ct8, s8);
    return 0;
}

/*
 * Rijndael-256 x8: 16 half-block state regs + 4 temps + 3 constants = 23 regs.
 * Uses DO_ENC_RIJNDAEL256_x8, a genuine single round-by-round interleave
 * across all 8 streams (see _RJ256_FULL_ROUND_x8 above), rather than two
 * back-to-back calls to DO_ENC_RIJNDAEL256_x4: the latter only gives the
 * compiler two independent 4-way chains with a macro-call boundary between
 * them, while the former exposes all 8 lanes' independence to the scheduler
 * at once, matching the same reasoning as DO_ENC_AES128_x8 above. Verified
 * via cross-compiled disassembly (see _RJ256_FULL_ROUND_x8's comment) that
 * this does not introduce any stack spills of live NEON state.
 */
WEAK int rijndael256_arm_aes_enc_x8(const rijndael_arm_aes_ctx_rijndael256 *ctx1,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx2,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx3,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx4,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx5,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx6,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx7,
                                     const rijndael_arm_aes_ctx_rijndael256 *ctx8,
                                     const uint8_t pt1[32], const uint8_t pt2[32],
                                     const uint8_t pt3[32], const uint8_t pt4[32],
                                     const uint8_t pt5[32], const uint8_t pt6[32],
                                     const uint8_t pt7[32], const uint8_t pt8[32],
                                     uint8_t ct1[32], uint8_t ct2[32],
                                     uint8_t ct3[32], uint8_t ct4[32],
                                     uint8_t ct5[32], uint8_t ct6[32],
                                     uint8_t ct7[32], uint8_t ct8[32])
{
    uint8x16_t ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4, ml5, mr5, ml6, mr6, ml7, mr7, ml8, mr8;
    if ((ctx1 == NULL) || (ctx1->rtype != RIJNDAEL_256_256) || (ctx2 == NULL) || (ctx2->rtype != RIJNDAEL_256_256) ||
        (ctx3 == NULL) || (ctx3->rtype != RIJNDAEL_256_256) || (ctx4 == NULL) || (ctx4->rtype != RIJNDAEL_256_256) ||
        (ctx5 == NULL) || (ctx5->rtype != RIJNDAEL_256_256) || (ctx6 == NULL) || (ctx6->rtype != RIJNDAEL_256_256) ||
        (ctx7 == NULL) || (ctx7->rtype != RIJNDAEL_256_256) || (ctx8 == NULL) || (ctx8->rtype != RIJNDAEL_256_256)) {
        return -1;
    }
    ml1 = vld1q_u8(pt1);      mr1 = vld1q_u8(pt1 + 16);
    ml2 = vld1q_u8(pt2);      mr2 = vld1q_u8(pt2 + 16);
    ml3 = vld1q_u8(pt3);      mr3 = vld1q_u8(pt3 + 16);
    ml4 = vld1q_u8(pt4);      mr4 = vld1q_u8(pt4 + 16);
    ml5 = vld1q_u8(pt5);      mr5 = vld1q_u8(pt5 + 16);
    ml6 = vld1q_u8(pt6);      mr6 = vld1q_u8(pt6 + 16);
    ml7 = vld1q_u8(pt7);      mr7 = vld1q_u8(pt7 + 16);
    ml8 = vld1q_u8(pt8);      mr8 = vld1q_u8(pt8 + 16);
    DO_ENC_RIJNDAEL256_x8(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4,
                          ml5, mr5, ml6, mr6, ml7, mr7, ml8, mr8,
                          ctx1->rk, ctx2->rk, ctx3->rk, ctx4->rk,
                          ctx5->rk, ctx6->rk, ctx7->rk, ctx8->rk);
    vst1q_u8(ct1, ml1); vst1q_u8(ct1 + 16, mr1);
    vst1q_u8(ct2, ml2); vst1q_u8(ct2 + 16, mr2);
    vst1q_u8(ct3, ml3); vst1q_u8(ct3 + 16, mr3);
    vst1q_u8(ct4, ml4); vst1q_u8(ct4 + 16, mr4);
    vst1q_u8(ct5, ml5); vst1q_u8(ct5 + 16, mr5);
    vst1q_u8(ct6, ml6); vst1q_u8(ct6 + 16, mr6);
    vst1q_u8(ct7, ml7); vst1q_u8(ct7 + 16, mr7);
    vst1q_u8(ct8, ml8); vst1q_u8(ct8 + 16, mr8);
    return 0;
}

/* ======== Generic x2/x4/x8 single-ctx and ECB boilerplate ========
 * MAKE_PARALLEL_FUNCS_XX_IMPL (not MAKE_GENERIC_FUNCS_XX_IMPL) is required
 * here: the latter would synthesize enc_x2_x2/x4_x4/x8_x8 as N serial calls
 * to the single-block enc(), discarding all the round-by-round interleaving
 * done above in aes128/aes256/rijndael256_arm_aes_enc_x2/x4/x8. Matches the
 * same choice already made in rijndael_aes_ni.c for the same reason.
 *
 * MAKE_GENERIC_SETKEY_XX_IMPL is intentionally NOT used for aes128/aes256/
 * rijndael256 here: it would synthesize setkey_enc_x2/x4/x8 as N serial calls
 * to the single-key setkey_enc(), discarding the round-by-round interleaved
 * key schedules hand-written above (aes128/aes256/rijndael256_arm_aes_setkey_enc_x2/x4/x8),
 * which exist precisely to avoid that serial fallback (see the comment block
 * above _ARM_RK_ROTWORD_STEP). Only the enc_xN_xN/ECB boilerplate remains
 * generic below. */
MAKE_PARALLEL_FUNCS_XX_IMPL(aes128, arm_aes, 16)
MAKE_PARALLEL_FUNCS_XX_IMPL(aes256, arm_aes, 16)
MAKE_PARALLEL_FUNCS_XX_IMPL(rijndael256, arm_aes, 32)

/* AES-128 / AES-256 ECB: dedicated same-key implementations rather than
 * MAKE_PARALLEL_FUNCS_ECB_IMPL.
 */
WEAK int aes128_arm_aes_setkey_enc_ecb(rijndael_arm_aes_ctx_aes128_ecb *ctx, const uint8_t key[16])
{
    return aes128_arm_aes_setkey_enc(ctx, key);
}

WEAK int aes128_arm_aes_enc_ecb(const rijndael_arm_aes_ctx_aes128_ecb *ctx, uint32_t nblocks,
                                 const uint8_t *in, uint8_t *out)
{
    uint32_t i = 0;

    if ((ctx == NULL) || (ctx->rtype != AES128)) {
        return -1;
    }
    for (; (i + 8) <= nblocks; i += 8) {
        uint8x16_t s1, s2, s3, s4, s5, s6, s7, s8;
        s1 = vld1q_u8(&in[(i + 0) * 16]); s2 = vld1q_u8(&in[(i + 1) * 16]);
        s3 = vld1q_u8(&in[(i + 2) * 16]); s4 = vld1q_u8(&in[(i + 3) * 16]);
        s5 = vld1q_u8(&in[(i + 4) * 16]); s6 = vld1q_u8(&in[(i + 5) * 16]);
        s7 = vld1q_u8(&in[(i + 6) * 16]); s8 = vld1q_u8(&in[(i + 7) * 16]);
        DO_ENC_AES128_x8(s1, s2, s3, s4, s5, s6, s7, s8,
                         ctx->rk, ctx->rk, ctx->rk, ctx->rk,
                         ctx->rk, ctx->rk, ctx->rk, ctx->rk);
        vst1q_u8(&out[(i + 0) * 16], s1); vst1q_u8(&out[(i + 1) * 16], s2);
        vst1q_u8(&out[(i + 2) * 16], s3); vst1q_u8(&out[(i + 3) * 16], s4);
        vst1q_u8(&out[(i + 4) * 16], s5); vst1q_u8(&out[(i + 5) * 16], s6);
        vst1q_u8(&out[(i + 6) * 16], s7); vst1q_u8(&out[(i + 7) * 16], s8);
    }
    if ((nblocks - i) >= 4) {
        uint8x16_t s1, s2, s3, s4;
        s1 = vld1q_u8(&in[(i + 0) * 16]); s2 = vld1q_u8(&in[(i + 1) * 16]);
        s3 = vld1q_u8(&in[(i + 2) * 16]); s4 = vld1q_u8(&in[(i + 3) * 16]);
        DO_ENC_AES128_x4(s1, s2, s3, s4, ctx->rk, ctx->rk, ctx->rk, ctx->rk);
        vst1q_u8(&out[(i + 0) * 16], s1); vst1q_u8(&out[(i + 1) * 16], s2);
        vst1q_u8(&out[(i + 2) * 16], s3); vst1q_u8(&out[(i + 3) * 16], s4);
        i += 4;
    }
    if ((nblocks - i) >= 2) {
        uint8x16_t s1, s2;
        s1 = vld1q_u8(&in[(i + 0) * 16]); s2 = vld1q_u8(&in[(i + 1) * 16]);
        DO_ENC_AES128_x2(s1, s2, ctx->rk, ctx->rk);
        vst1q_u8(&out[(i + 0) * 16], s1); vst1q_u8(&out[(i + 1) * 16], s2);
        i += 2;
    }
    if (i < nblocks) {
        uint8x16_t s1;
        s1 = vld1q_u8(&in[i * 16]);
        DO_ENC_AES128(s1, ctx->rk);
        vst1q_u8(&out[i * 16], s1);
    }
    return 0;
}

WEAK int aes256_arm_aes_setkey_enc_ecb(rijndael_arm_aes_ctx_aes256_ecb *ctx, const uint8_t key[32])
{
    return aes256_arm_aes_setkey_enc(ctx, key);
}

WEAK int aes256_arm_aes_enc_ecb(const rijndael_arm_aes_ctx_aes256_ecb *ctx, uint32_t nblocks,
                                 const uint8_t *in, uint8_t *out)
{
    uint32_t i = 0;

    if ((ctx == NULL) || (ctx->rtype != AES256)) {
        return -1;
    }
    for (; (i + 8) <= nblocks; i += 8) {
        uint8x16_t s1, s2, s3, s4, s5, s6, s7, s8;
        s1 = vld1q_u8(&in[(i + 0) * 16]); s2 = vld1q_u8(&in[(i + 1) * 16]);
        s3 = vld1q_u8(&in[(i + 2) * 16]); s4 = vld1q_u8(&in[(i + 3) * 16]);
        s5 = vld1q_u8(&in[(i + 4) * 16]); s6 = vld1q_u8(&in[(i + 5) * 16]);
        s7 = vld1q_u8(&in[(i + 6) * 16]); s8 = vld1q_u8(&in[(i + 7) * 16]);
        DO_ENC_AES256_x8(s1, s2, s3, s4, s5, s6, s7, s8,
                         ctx->rk, ctx->rk, ctx->rk, ctx->rk,
                         ctx->rk, ctx->rk, ctx->rk, ctx->rk);
        vst1q_u8(&out[(i + 0) * 16], s1); vst1q_u8(&out[(i + 1) * 16], s2);
        vst1q_u8(&out[(i + 2) * 16], s3); vst1q_u8(&out[(i + 3) * 16], s4);
        vst1q_u8(&out[(i + 4) * 16], s5); vst1q_u8(&out[(i + 5) * 16], s6);
        vst1q_u8(&out[(i + 6) * 16], s7); vst1q_u8(&out[(i + 7) * 16], s8);
    }
    if ((nblocks - i) >= 4) {
        uint8x16_t s1, s2, s3, s4;
        s1 = vld1q_u8(&in[(i + 0) * 16]); s2 = vld1q_u8(&in[(i + 1) * 16]);
        s3 = vld1q_u8(&in[(i + 2) * 16]); s4 = vld1q_u8(&in[(i + 3) * 16]);
        DO_ENC_AES256_x4(s1, s2, s3, s4, ctx->rk, ctx->rk, ctx->rk, ctx->rk);
        vst1q_u8(&out[(i + 0) * 16], s1); vst1q_u8(&out[(i + 1) * 16], s2);
        vst1q_u8(&out[(i + 2) * 16], s3); vst1q_u8(&out[(i + 3) * 16], s4);
        i += 4;
    }
    if ((nblocks - i) >= 2) {
        uint8x16_t s1, s2;
        s1 = vld1q_u8(&in[(i + 0) * 16]); s2 = vld1q_u8(&in[(i + 1) * 16]);
        DO_ENC_AES256_x2(s1, s2, ctx->rk, ctx->rk);
        vst1q_u8(&out[(i + 0) * 16], s1); vst1q_u8(&out[(i + 1) * 16], s2);
        i += 2;
    }
    if (i < nblocks) {
        uint8x16_t s1;
        s1 = vld1q_u8(&in[i * 16]);
        DO_ENC_AES256(s1, ctx->rk);
        vst1q_u8(&out[i * 16], s1);
    }
    return 0;
}

/* Rijndael-256 ECB: same treatment as AES-128/AES-256 above. With 30 round
 * keys per context the redundant reload the generic macro forces is larger
 * still, and the ShiftRows compensation constants also get reloaded per
 * context instead of once. */
WEAK int rijndael256_arm_aes_setkey_enc_ecb(rijndael_arm_aes_ctx_rijndael256_ecb *ctx, const uint8_t key[32])
{
    return rijndael256_arm_aes_setkey_enc(ctx, key);
}

WEAK int rijndael256_arm_aes_enc_ecb(const rijndael_arm_aes_ctx_rijndael256_ecb *ctx, uint32_t nblocks,
                                      const uint8_t *in, uint8_t *out)
{
    uint32_t i = 0;

    if ((ctx == NULL) || (ctx->rtype != RIJNDAEL_256_256)) {
        return -1;
    }
    for (; (i + 4) <= nblocks; i += 4) {
        uint8x16_t ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4;
        ml1 = vld1q_u8(&in[(i + 0) * 32]); mr1 = vld1q_u8(&in[((i + 0) * 32) + 16]);
        ml2 = vld1q_u8(&in[(i + 1) * 32]); mr2 = vld1q_u8(&in[((i + 1) * 32) + 16]);
        ml3 = vld1q_u8(&in[(i + 2) * 32]); mr3 = vld1q_u8(&in[((i + 2) * 32) + 16]);
        ml4 = vld1q_u8(&in[(i + 3) * 32]); mr4 = vld1q_u8(&in[((i + 3) * 32) + 16]);
        DO_ENC_RIJNDAEL256_x4(ml1, mr1, ml2, mr2, ml3, mr3, ml4, mr4,
                              ctx->rk, ctx->rk, ctx->rk, ctx->rk);
        vst1q_u8(&out[(i + 0) * 32], ml1); vst1q_u8(&out[((i + 0) * 32) + 16], mr1);
        vst1q_u8(&out[(i + 1) * 32], ml2); vst1q_u8(&out[((i + 1) * 32) + 16], mr2);
        vst1q_u8(&out[(i + 2) * 32], ml3); vst1q_u8(&out[((i + 2) * 32) + 16], mr3);
        vst1q_u8(&out[(i + 3) * 32], ml4); vst1q_u8(&out[((i + 3) * 32) + 16], mr4);
    }
    if ((nblocks - i) >= 2) {
        uint8x16_t ml1, mr1, ml2, mr2;
        ml1 = vld1q_u8(&in[(i + 0) * 32]); mr1 = vld1q_u8(&in[((i + 0) * 32) + 16]);
        ml2 = vld1q_u8(&in[(i + 1) * 32]); mr2 = vld1q_u8(&in[((i + 1) * 32) + 16]);
        DO_ENC_RIJNDAEL256_x2(ml1, mr1, ml2, mr2, ctx->rk, ctx->rk);
        vst1q_u8(&out[(i + 0) * 32], ml1); vst1q_u8(&out[((i + 0) * 32) + 16], mr1);
        vst1q_u8(&out[(i + 1) * 32], ml2); vst1q_u8(&out[((i + 1) * 32) + 16], mr2);
        i += 2;
    }
    if (i < nblocks) {
        uint8x16_t ml1, mr1;
        ml1 = vld1q_u8(&in[i * 32]); mr1 = vld1q_u8(&in[(i * 32) + 16]);
        DO_ENC_RIJNDAEL256(ml1, mr1, ctx->rk);
        vst1q_u8(&out[i * 32], ml1); vst1q_u8(&out[(i * 32) + 16], mr1);
    }
    return 0;
}


#else /* !RIJNDAEL_ARM_AES */
/*
 * Dummy definition to avoid the empty translation unit ISO C warning
 */
typedef int dummy;
#endif /* RIJNDAEL_ARM_AES */
