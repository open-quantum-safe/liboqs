#ifndef __FIELDS_ARM_NEON_H__
#define __FIELDS_ARM_NEON_H__

/* Check for ARM NEON support */
#if defined(__ARM_NEON) || defined(__ARM_NEON__)

/* This backend is little endian only.
 *
 * The GF(256^2) routines below mix two views of the same register: they build
 * uint8x16_t values with fixed byte indices (the shuff_lo/shuff_hi/mask_even
 * tables of gf256to2_*), then read them back through vreinterpretq_u16_u8 as
 * eight uint16_t lanes, assuming byte 2k holds the low half of lane k. On a big
 * endian ARM that assumption is false - vld1q_u8 and vld1q_u16 do not agree on
 * element order there.
 *
 * Auto-selection already avoids this (fields.h falls through to FIELDS_REF on
 * big endian), so reaching this #error means FIELDS_NEON was asked for
 * explicitly. Failing loudly is the point: quietly building the portable
 * backend instead would hide that the request could not be honoured.
 */
#if defined(MQOM3_TARGET_IS_BIG_ENDIAN)
#error "Error: FIELDS_NEON is little endian only (see the comment above); use FIELDS_REF on big endian ARM"
#endif

#include "fields_common.h"
#include "fields_ref.h"
#include <string.h>
#include <arm_neon.h>

/* ==========================================================================
 * Low-level NEON helpers
 * ========================================================================== */

/*
 * 128-bit table lookup: the sole ARMv7 / AArch64 divergence point.
 *
 * AArch64 provides vqtbl1q_u8 which does a full 16-byte lookup in one
 * instruction (equivalent to x86 VPSHUFB on a 128-bit operand).
 *
 * ARMv7 only has vtbl2_u8 which looks up 8 bytes at a time from a table
 * held in a pair of 64-bit registers.  We apply it to both halves of the
 * index vector and recombine.
 */
#ifdef __aarch64__
static inline uint8x16_t neon_tbl_u8q(uint8x16_t tbl, uint8x16_t idx) {
	return vqtbl1q_u8(tbl, idx);
}
#else
static inline uint8x16_t neon_tbl_u8q(uint8x16_t tbl, uint8x16_t idx) {
	uint8x8x2_t tbl2 = {{ vget_low_u8(tbl), vget_high_u8(tbl) }};
	return vcombine_u8(vtbl2_u8(tbl2, vget_low_u8(idx)),
	                   vtbl2_u8(tbl2, vget_high_u8(idx)));
}
#endif

/*
 * PMULL-based GF(256) core multiply helpers: the AArch64 / ARMv7 divergence
 * point for the "high half" and de-interleave (uzp1/uzp2) operations used by
 * gf256_mult_vectorized_neon() below.
 *
 * AArch64 provides vmull_high_p8/vuzp1q_u8/vuzp2q_u8/vmovl_high_u8 directly.
 *
 * ARMv7 lacks the "_high" naming and the split uzp1/uzp2 forms; it only has
 * vmull_p8 on 64-bit halves (so the high half is built via vget_high_p8) and
 * vuzpq_u8, which returns both de-interleaved halves as a 2-tuple (we keep
 * only the half we need).
 */
#ifdef __aarch64__
static inline poly16x8_t neon_pmull_high_p8(poly8x16_t a, poly8x16_t b) {
	return vmull_high_p8(a, b);
}
static inline uint8x16_t neon_uzp1q_u8(uint8x16_t a, uint8x16_t b) {
	return vuzp1q_u8(a, b);
}
static inline uint8x16_t neon_uzp2q_u8(uint8x16_t a, uint8x16_t b) {
	return vuzp2q_u8(a, b);
}
static inline uint16x8_t neon_movl_high_u8(uint8x16_t a) {
	return vmovl_high_u8(a);
}
#else
static inline poly16x8_t neon_pmull_high_p8(poly8x16_t a, poly8x16_t b) {
	return vmull_p8(vget_high_p8(a), vget_high_p8(b));
}
static inline uint8x16_t neon_uzp1q_u8(uint8x16_t a, uint8x16_t b) {
	return vuzpq_u8(a, b).val[0];
}
static inline uint8x16_t neon_uzp2q_u8(uint8x16_t a, uint8x16_t b) {
	return vuzpq_u8(a, b).val[1];
}
static inline uint16x8_t neon_movl_high_u8(uint8x16_t a) {
	return vmovl_u8(vget_high_u8(a));
}
#endif

/*
 * Load up to 'len' bytes into a uint8x16_t, zero-padding the remainder.
 * 'len' is always < 16 here (the caller already takes the fast vld1q_u8
 * path for full 16-byte blocks). 
 */
static inline uint8x16_t load_incomplete_neon(const uint8_t *a, uint32_t len) {
	uint8x16_t v = vdupq_n_u8(0);
	if (len >= 16) {
		return vld1q_u8(a);
	}
	if (len > 0) {
		v = vld1q_lane_u8(a + 0, v, 0);
	}
	if (len > 1) {
		v = vld1q_lane_u8(a + 1, v, 1);
	}
	if (len > 2) {
		v = vld1q_lane_u8(a + 2, v, 2);
	}
	if (len > 3) {
		v = vld1q_lane_u8(a + 3, v, 3);
	}
	if (len > 4) {
		v = vld1q_lane_u8(a + 4, v, 4);
	}
	if (len > 5) {
		v = vld1q_lane_u8(a + 5, v, 5);
	}
	if (len > 6) {
		v = vld1q_lane_u8(a + 6, v, 6);
	}
	if (len > 7) {
		v = vld1q_lane_u8(a + 7, v, 7);
	}
	if (len > 8) {
		v = vld1q_lane_u8(a + 8, v, 8);
	}
	if (len > 9) {
		v = vld1q_lane_u8(a + 9, v, 9);
	}
	if (len > 10) {
		v = vld1q_lane_u8(a + 10, v, 10);
	}
	if (len > 11) {
		v = vld1q_lane_u8(a + 11, v, 11);
	}
	if (len > 12) {
		v = vld1q_lane_u8(a + 12, v, 12);
	}
	if (len > 13) {
		v = vld1q_lane_u8(a + 13, v, 13);
	}
	if (len > 14) {
		v = vld1q_lane_u8(a + 14, v, 14);
	}
	return v;
}

/*
 * Store 'len' bytes from a uint8x16_t.
 * See load_incomplete_neon() above: same lane-wise, call-free technique
 * (vst1q_lane_u8 with compile-time lane indices), avoiding both the
 * 'bl memcpy' call and any out-of-bounds write past 'a[len - 1]'.
 */
static inline void store_incomplete_neon(uint8x16_t v, uint8_t *a, uint32_t len) {
	if (len >= 16) {
		vst1q_u8(a, v);
		return;
	}
	if (len > 0) {
		vst1q_lane_u8(a + 0, v, 0);
	}
	if (len > 1) {
		vst1q_lane_u8(a + 1, v, 1);
	}
	if (len > 2) {
		vst1q_lane_u8(a + 2, v, 2);
	}
	if (len > 3) {
		vst1q_lane_u8(a + 3, v, 3);
	}
	if (len > 4) {
		vst1q_lane_u8(a + 4, v, 4);
	}
	if (len > 5) {
		vst1q_lane_u8(a + 5, v, 5);
	}
	if (len > 6) {
		vst1q_lane_u8(a + 6, v, 6);
	}
	if (len > 7) {
		vst1q_lane_u8(a + 7, v, 7);
	}
	if (len > 8) {
		vst1q_lane_u8(a + 8, v, 8);
	}
	if (len > 9) {
		vst1q_lane_u8(a + 9, v, 9);
	}
	if (len > 10) {
		vst1q_lane_u8(a + 10, v, 10);
	}
	if (len > 11) {
		vst1q_lane_u8(a + 11, v, 11);
	}
	if (len > 12) {
		vst1q_lane_u8(a + 12, v, 12);
	}
	if (len > 13) {
		vst1q_lane_u8(a + 13, v, 13);
	}
	if (len > 14) {
		vst1q_lane_u8(a + 14, v, 14);
	}
}

/* XOR-reduce all 16 bytes of a register to a single byte */
static inline uint8_t sum_uint8_neon(uint8x16_t v) {
	uint8x8_t lo = vget_low_u8(v);
	uint8x8_t hi = vget_high_u8(v);
	uint8x8_t f  = veor_u8(lo, hi);
	f = veor_u8(f, vext_u8(f, f, 4));
	f = veor_u8(f, vext_u8(f, f, 2));
	f = veor_u8(f, vext_u8(f, f, 1));
	return vget_lane_u8(f, 0);
}

/* XOR-reduce all 8 uint16_t lanes of a register to a single uint16_t */
static inline uint16_t sum_uint16_neon(uint8x16_t v) {
	uint16x8_t v16 = vreinterpretq_u16_u8(v);
	uint16x4_t lo  = vget_low_u16(v16);
	uint16x4_t hi  = vget_high_u16(v16);
	uint16x4_t f   = veor_u16(lo, hi);
	f = veor_u16(f, vext_u16(f, f, 2));
	f = veor_u16(f, vext_u16(f, f, 1));
	return vget_lane_u16(f, 0);
}

/* Parity of all bits in a register (XOR of all bytes, then byte parity) */
static inline uint8_t parity_neon(uint8x16_t v) {
	uint8_t byte = sum_uint8_neon(v);
	byte ^= byte >> 4;
	byte ^= byte >> 2;
	byte ^= byte >> 1;
	return byte & 1;
}

/* ==========================================================================
 * GF(256) vectorized multiplication helper (used by all GF(256) and GF(256^2)
 * functions).  Implements a carry-less-multiply-and-reduce (PMULL) approach,
 * byte-lane-parallel: vmull_p8/vmull_high_p8 (baseline Advanced SIMD, not
 * gated behind the Crypto Extension - that gate only applies to the 64-bit
 * vmull_p64 used for GHASH-style GF(2^128)) compute the raw carry-less
 * product of each byte lane, which is then folded modulo the GF(256)
 * reduction polynomial 0x1B in two rounds, mirroring the classic CLMUL-based
 * reduction:
 *   p(x) = CLMUL(a,b)                    degree <= 14 (raw, unreduced)
 *   p = p_h*x^8 + p_l                    p_h: degree<=6, p_l: degree<=7
 *   t = p_l XOR CLMUL(p_h, 0x1B)         degree <= 10 (since x^8 = 0x1B mod R)
 *   t = t_h*x^8 + t_l                    t_h: degree<=2, t_l: degree<=7
 *   r = t_l XOR CLMUL(t_h, 0x1B)         degree <= 6 -> already a valid byte
 * The final fold's operand degrees are low enough (<=2 times <=3) that the
 * result fits a byte, so a non-widening vmulq_p8 suffices there (no need to
 * de-interleave/re-widen a third time).
 * ========================================================================== */
static inline uint8x16_t gf256_mult_vectorized_neon(uint8x16_t a, uint8x16_t b) {
	const poly8x16_t red = vreinterpretq_p8_u8(vdupq_n_u8(0x1B)); /* GF(256) Rijndael reduction polynomial */
	poly8x16_t pa = vreinterpretq_p8_u8(a);
	poly8x16_t pb = vreinterpretq_p8_u8(b);

	/* Raw (unreduced) carry-less product per lane, widened to 16 bits/lane */
	poly16x8_t p_lo = vmull_p8(vget_low_p8(pa), vget_low_p8(pb));
	poly16x8_t p_hi = neon_pmull_high_p8(pa, pb);

	uint8x16_t p_lo_b = vreinterpretq_u8_p16(p_lo);
	uint8x16_t p_hi_b = vreinterpretq_u8_p16(p_hi);

	uint8x16_t p_l = neon_uzp1q_u8(p_lo_b, p_hi_b);                        /* low byte of each product */
	poly8x16_t p_h = vreinterpretq_p8_u8(neon_uzp2q_u8(p_lo_b, p_hi_b));   /* high byte, degree <= 6 */

	/* Round 1 fold: p = p_h*x^8 + p_l; since x^8 = red (mod modulus), t = p_l XOR (p_h * red) */
	poly16x8_t q_lo = vmull_p8(vget_low_p8(p_h), vget_low_p8(red));
	poly16x8_t q_hi = neon_pmull_high_p8(p_h, red);

	uint16x8_t p_l_lo16 = vmovl_u8(vget_low_u8(p_l));
	uint16x8_t p_l_hi16 = neon_movl_high_u8(p_l);

	uint16x8_t t_lo = veorq_u16(vreinterpretq_u16_p16(q_lo), p_l_lo16);
	uint16x8_t t_hi = veorq_u16(vreinterpretq_u16_p16(q_hi), p_l_hi16);

	uint8x16_t t_lo_b = vreinterpretq_u8_u16(t_lo);
	uint8x16_t t_hi_b = vreinterpretq_u8_u16(t_hi);

	uint8x16_t t_l = neon_uzp1q_u8(t_lo_b, t_hi_b);                        /* degree <= 7 */
	poly8x16_t t_h = vreinterpretq_p8_u8(neon_uzp2q_u8(t_lo_b, t_hi_b));   /* degree <= 2 */

	/* Round 2 fold: degree(t_h)<=2 so degree(t_h*red)<=6 fits a byte -- non-widening vmulq_p8 suffices */
	poly8x16_t r = vmulq_p8(t_h, red);

	return veorq_u8(t_l, vreinterpretq_u8_p8(r));
}

/* ==========================================================================
 * Shared GF(2) bit-expansion helpers.
 *
 * For GF(2)xGF(256): 16 GF(256) elements per NEON register -> 2 GF(2) bytes.
 *   gf2_shuf  : distribute byte 0 to lanes 0-7, byte 1 to lanes 8-15.
 *   gf2_bsel  : isolate bit k in lane k (and lane k+8).
 *
 * For GF(2)xGF(256^2): 8 GF(256^2) elements per NEON register -> 1 GF(2) byte.
 *   gf2_shuf16: distribute the single byte to all 16 lanes.
 *   gf2_bsel16: isolate bit k in the two lanes of uint16_t element k.
 * ========================================================================== */
static const uint8_t _neon_gf2_shuf_data[16]   = {0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1};
static const uint8_t _neon_gf2_bsel_data[16]   = {1,2,4,8,16,32,64,128, 1,2,4,8,16,32,64,128};
static const uint8_t _neon_gf2_shuf16_data[16] = {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};
static const uint8_t _neon_gf2_bsel16_data[16] = {1,1, 2,2, 4,4, 8,8, 16,16, 32,32, 64,64, 128,128};

/*
 * Expand 2 bytes of packed GF(2) data (covering 16 consecutive bits) into a
 * uint8x16_t where each lane is 0xFF if the corresponding bit is set, 0x00
 * otherwise.
 * 'rem' is the number of GF(256) elements remaining (1..16); only the lanes
 * corresponding to valid bits matter.
 */
static inline uint8x16_t gf2_expand_16bits_neon(const uint8_t *gf2_ptr, uint32_t rem) {
	const uint8x16_t shuf = vld1q_u8(_neon_gf2_shuf_data);
	const uint8x16_t bsel = vld1q_u8(_neon_gf2_bsel_data);
	uint8x16_t reg = vdupq_n_u8(0);
	reg = vld1q_lane_u8(gf2_ptr, reg, 0);
	if (rem > 8) {
		reg = vld1q_lane_u8(gf2_ptr + 1, reg, 1);
	}
	uint8x16_t spread = neon_tbl_u8q(reg, shuf);
	uint8x16_t sel    = vandq_u8(spread, bsel);
	return vceqq_u8(sel, bsel); /* 0xFF where bit set, 0x00 otherwise */
}

/*
 * Expand 1 byte of packed GF(2) data (covering 8 consecutive bits) into a
 * uint8x16_t where each pair of adjacent lanes carries the same 0xFF/0x00
 * mask  - one pair per GF(256^2) element (uint16_t).
 */
static inline uint8x16_t gf2_expand_8bits_neon(const uint8_t *gf2_ptr) {
	const uint8x16_t shuf = vld1q_u8(_neon_gf2_shuf16_data);
	const uint8x16_t bsel = vld1q_u8(_neon_gf2_bsel16_data);
	uint8x16_t reg = vdupq_n_u8(0);
	reg = vld1q_lane_u8(gf2_ptr, reg, 0);
	uint8x16_t spread = neon_tbl_u8q(reg, shuf);
	uint8x16_t sel    = vandq_u8(spread, bsel);
	return vceqq_u8(sel, bsel);
}

/* ==========================================================================
 * GF(2)
 * ========================================================================== */

/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint8_t gf2_mult_neon(uint8_t a, uint8_t b) {
	return gf2_mult_ref(a, b);
}

/*
 * Vector multiplied by a constant in GF(2).
 */
static inline void gf2_constant_vect_mult_neon(uint8_t b, const uint8_t *a, uint8_t *c, uint32_t len) {
	gf2_constant_vect_mult_ref(b, a, c, len);
	return;
}

static inline uint8_t gf2_vect_mult_neon(const uint8_t *a, const uint8_t *b, uint32_t len_bits) {
	uint32_t i;
	uint8x16_t accu = vdupq_n_u8(0);
	uint32_t len = len_bits / 8;

	for (i = 0; i < len; i += 16) {
		uint32_t rem = (len - i < 16) ? (len - i) : 16;
		uint8x16_t _a = load_incomplete_neon(&a[i], rem);
		uint8x16_t _b = load_incomplete_neon(&b[i], rem);
		accu = veorq_u8(accu, vandq_u8(_a, _b));
	}
	return parity_neon(accu);
}

/* Matrix and vector multiplication over GF(2) */
static inline void gf2_mat_mult_neon(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF2_MAT_MULT(A, X, Y, n, mtype, gf2_vect_mult_neon);
}

/* GF(2) matrix transposition */
static inline void gf2_mat_transpose_neon(const uint8_t *A, uint8_t *B, uint32_t n, matrix_type mtype) {
	gf2_mat_transpose_ref(A, B, n, mtype);
}

/* ==========================================================================
 * GF(256)
 * ========================================================================== */

/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint8_t gf256_mult_neon(uint8_t x, uint8_t y) {
	return gf256_mult_ref(x, y);
}

/* ==========================================================================
 * Shared helpers for the matrix by vector products.
 *
 * The generic path built each row through load_incomplete_neon(), whose length
 * varies from row to row for the triangular cases: that degenerates into a
 * chain of up to 15 tested per-lane loads, twice per 16-byte block, which
 * costs more than the multiply itself. Instead the tail is loaded whole and
 * masked with a prefix mask taken from a static table. Both the length and the
 * table index are public loop bounds (matrix geometry), never secrets.
 * ========================================================================== */

/* gf_prefix_mask_table_neon + (16 - k) is a 16-byte mask made of k leading
 * 0xFF bytes followed by zeroes, for any k in [0, 16]. */
static const uint8_t gf_prefix_mask_table_neon[32] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static inline uint8x16_t gf_prefix_mask_neon(uint32_t k) {
	return vld1q_u8(&gf_prefix_mask_table_neon[16 - k]);
}

/* Number of 16-byte blocks of the common vector kept live at once. */
#ifndef GF_MULT_PANEL_BLOCKS_NEON
#define GF_MULT_PANEL_BLOCKS_NEON 8
#endif

/*
 * Region ("split nibble + TBL") multiply core for GF(256): given precomputed
 * lookup tables for a constant b (tbl_lo[i] = b*i, tbl_hi[i] = b*(i<<4), for
 * i in 0..15), compute a[k]*b for all 16 lanes of 'a' in one pass, since
 * a = a_hi*16 + a_lo implies a*b = tbl_hi[a_hi] XOR tbl_lo[a_lo]. Used by
 * gf256_constant_vect_mult_neon() and gf256to2_constant_vect_mult_neon()
 * below to replace the full generic multiply (gf256_mult_vectorized_neon)
 * with 2 ANDs/SHIFT + 2 TBL + 1 XOR whenever the second operand is constant
 * (loop-invariant) across the whole call.
 */
static inline uint8x16_t gf256_const_mult_block_neon(uint8x16_t a, uint8x16_t tbl_lo, uint8x16_t tbl_hi) {
	const uint8x16_t nib_mask = vdupq_n_u8(0x0F);
	uint8x16_t lo_nib = vandq_u8(a, nib_mask);
	uint8x16_t hi_nib = vshrq_n_u8(a, 4);
	return veorq_u8(neon_tbl_u8q(tbl_lo, lo_nib), neon_tbl_u8q(tbl_hi, hi_nib));
}

/*
 * Build the two region-multiply tables of a GF(256) constant with two
 * vectorized PMULL multiplications.
 * NOTE: these tables used to be filled by 32 scalar gf256_mult_ref() calls,
 * which dwarfed the actual per-block work for the short vectors the scheme
 * uses. Going through the vector multiply also removes any dependency on the
 * (possibly table driven, hence not constant time) scalar reference multiply
 * for a value that may be secret.
 */
static inline void gf256_build_const_tables_neon(uint8_t b, uint8x16_t *tbl_lo, uint8x16_t *tbl_hi) {
	const uint8x16_t idx_lo = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	const uint8x16_t idx_hi = { 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
	                            0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0
	                          };
	uint8x16_t vb = vdupq_n_u8(b);

	*tbl_lo = gf256_mult_vectorized_neon(vb, idx_lo);
	*tbl_hi = gf256_mult_vectorized_neon(vb, idx_hi);

	return;
}

/*
 * Vector multiplied by a constant in GF(256).
 */
static inline void gf256_constant_vect_mult_neon(uint8_t b, const uint8_t *a, uint8_t *c, uint32_t len) {
	uint32_t i;
	/* Build the two 16-entry region-multiply tables for constant 'b' once,
	 * outside the per-16-byte loop. */
	uint8x16_t tbl_lo, tbl_hi;
	gf256_build_const_tables_neon(b, &tbl_lo, &tbl_hi);

	for (i = 0; i < len; i += 16) {
		uint32_t rem = (len - i < 16) ? (len - i) : 16;
		uint8x16_t _a = load_incomplete_neon(&a[i], rem);
		store_incomplete_neon(gf256_const_mult_block_neon(_a, tbl_lo, tbl_hi), &c[i], rem);
	}
	return;
}

/*
 * Vector to vector multiplication in GF(256).
 * Takes two vectors of length 'len', returns a byte (element in GF(256)).
 */
static inline uint8_t gf256_vect_mult_neon(const uint8_t *a, const uint8_t *b, uint32_t len) {
	uint32_t i;
	uint8x16_t accu = vdupq_n_u8(0);

	for (i = 0; i < len; i += 16) {
		uint32_t rem = (len - i < 16) ? (len - i) : 16;
		uint8x16_t _a = load_incomplete_neon(&a[i], rem);
		uint8x16_t _b = load_incomplete_neon(&b[i], rem);
		accu = veorq_u8(accu, gf256_mult_vectorized_neon(_a, _b));
	}
	return sum_uint8_neon(accu);
}

/*
 * Vector to vector batched multiplication in GF(256).
 */
static inline void gf256_vect_mult_multiple_public_neon(uint8_t* const* c, const uint8_t *a, const uint8_t* const* b, uint32_t len, uint32_t n) {
	uint32_t i;
	for (i = 0; i < n; i++) {
		*c[i] = gf256_vect_mult_neon(a, b[i], len);
	}
}

/*
 * One panel pass of a matrix by vector product: XOR into Y the contribution of
 * the columns covered by the 16-byte blocks [p, p + np) of the common vector,
 * already loaded (and zero padded) in xb[]. When 'first' is set the result
 * overwrites Y instead of being accumulated into it.
 *
 * 'elt_sz' is the element size in bytes (1 for GF(256), 2 for GF(256^2)) and
 * 'mulfun' the corresponding vectorized multiply; 'hsum' the matching
 * horizontal reduction. Written as a macro so that both element widths share
 * the exact same loop structure without an indirect call.
 */
#define GF_MAT_MULT_PANEL_NEON(A, Y, n, p, np, xb, tri_inf, first, elt_sz, mulfun, hsum) do { \
	const uint32_t _lo = 16 * (p); \
	const size_t _nbytes = (size_t)(elt_sz) * (n); \
	uint32_t _i, _b; \
	for (_i = 0; _i < (n); _i++) { \
		uint32_t _len = (elt_sz) * ((tri_inf) ? (_i + 1) : (n)); \
		uint32_t _hi, _nfull, _tail; \
		uint8x16_t _acc0, _acc1; \
		if (_len <= _lo) { \
			/* TRI_INF: this row does not reach the current panel */ \
			continue; \
		} \
		_hi = _len; \
		if (_hi > (_lo + (16 * (np)))) { \
			_hi = _lo + (16 * (np)); \
		} \
		_nfull = (_hi - _lo) / 16; \
		_tail  = (_hi - _lo) % 16; \
		_acc0 = vdupq_n_u8(0); \
		_acc1 = vdupq_n_u8(0); \
		for (_b = 0; _b < _nfull; _b++) { \
			size_t _aoff = (_nbytes * _i) + _lo + (16 * _b); \
			_acc0 = veorq_u8(_acc0, mulfun(vld1q_u8(&(A)[_aoff]), (xb)[_b])); \
		} \
		if (_tail != 0) { \
			size_t _aoff = (_nbytes * _i) + _lo + (16 * _nfull); \
			uint8x16_t _a; \
			/* Reading the tail whole and masking it is only allowed while it \
			 * stays inside the n x n matrix; the very last row falls back to \
			 * the (slower) per-lane load. */ \
			if ((_aoff + 16) <= (_nbytes * (n))) { \
				_a = vandq_u8(vld1q_u8(&(A)[_aoff]), gf_prefix_mask_neon(_tail)); \
			} else { \
				_a = load_incomplete_neon(&(A)[_aoff], _tail); \
			} \
			_acc1 = veorq_u8(_acc1, mulfun(_a, (xb)[_nfull])); \
		} \
		{ \
			uint8x16_t _acc = veorq_u8(_acc0, _acc1); \
			if (first) { \
				(Y)[_i] = hsum(_acc); \
			} else { \
				(Y)[_i] ^= hsum(_acc); \
			} \
		} \
	} \
} while(0)

/* Matrix and vector multiplication over GF(256) */
static inline void gf256_mat_mult_neon(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	uint8x16_t xb[GF_MULT_PANEL_BLOCKS_NEON];
	const uint32_t nblocks = (n + 15) / 16;
	uint32_t b, p;

	/* TRI_SUP starts each row at a different, non block aligned column, which
	 * does not fit the scheme below; it is unused by MQOM and is left on the
	 * generic path. */
	if (mtype == TRI_SUP) {
		GF256_MAT_MULT(A, X, Y, n, mtype, gf256_vect_mult_neon);
		return;
	}

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS_NEON) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS_NEON) {
			np = GF_MULT_PANEL_BLOCKS_NEON;
		}
		for (b = 0; b < np; b++) {
			uint32_t off = 16 * (p + b);
			if ((off + 16) <= n) {
				xb[b] = vld1q_u8(&X[off]);
			} else {
				/* Zero padded, so the trailing block is consumed whole */
				xb[b] = load_incomplete_neon(&X[off], n - off);
			}
		}
		GF_MAT_MULT_PANEL_NEON(A, Y, n, p, np, xb, (mtype == TRI_INF), (p == 0),
		                       1, gf256_mult_vectorized_neon, sum_uint8_neon);
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(2) and a vector in GF(256).
 */
static inline uint8_t gf2_gf256_vect_mult_neon(const uint8_t *a_gf2, const uint8_t *b_gf256, uint32_t n) {
	uint32_t i;
	uint8x16_t accu = vdupq_n_u8(0);

	for (i = 0; i < n; i += 16) {
		uint32_t rem = (n - i < 16) ? (n - i) : 16;
		uint8x16_t sel = gf2_expand_16bits_neon(&a_gf2[i / 8], rem);
		uint8x16_t _b  = load_incomplete_neon(&b_gf256[i], rem);
		accu = veorq_u8(accu, vandq_u8(sel, _b));
	}
	return sum_uint8_neon(accu);
}

/*
 * "Hybrid" multiplication of a constant in GF(2) and a vector in GF(256).
 */
static inline void gf2_gf256_constant_vect_mult_neon(uint8_t a_gf2, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	gf2_gf256_constant_vect_mult_ref(a_gf2, b_gf256, c_gf256, n);
	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(2).
 */
static inline void gf256_gf2_constant_vect_mult_neon(uint8_t a_gf256, const uint8_t *b_gf2, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;
	uint8x16_t _a = vdupq_n_u8(a_gf256);

	for (i = 0; i < n; i += 16) {
		uint32_t rem = (n - i < 16) ? (n - i) : 16;
		uint8x16_t sel = gf2_expand_16bits_neon(&b_gf2[i / 8], rem);
		store_incomplete_neon(vandq_u8(_a, sel), &c_gf256[i], rem);
	}
	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(2).
 */
static inline uint8_t gf256_gf2_vect_mult_neon(const uint8_t *a_gf256, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256_vect_mult_neon(b_gf2, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(2) and a vector in GF(256),
 * resulting in a vector in GF(256).
 */
static inline void gf2_gf256_mat_mult_neon(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	/* NOTE: XXX: we force a REG here as it allows for better performance */
	(void)mtype;
	GF2_GF256_MAT_MULT(A, X, Y, n, REG, gf2_gf256_vect_mult_neon);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(2),
 * resulting in a vector in GF(256).
 */
static inline void gf256_gf2_mat_mult_neon(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	/* NOTE: XXX: we force a REG here as it allows for better performance */
	(void)mtype;
	GF256_GF2_MAT_MULT(A, X, Y, n, REG, gf256_gf2_vect_mult_neon);
}

/* GF(256) matrix transposition */
static inline void gf256_mat_transpose_neon(const uint8_t *A, uint8_t *B, uint32_t n, matrix_type mtype) {
	gf256_mat_transpose_ref(A, B, n, mtype);
}

/* ==========================================================================
 * GF(16) <-> GF(256) helpers
 * ========================================================================== */

/*
 * Vectorized lifting from GF(16) to GF(256).
 * Elements of GF(16) are stored two-per-byte (low nibble first).
 * Processes 16 GF(256) output elements (= 8 packed GF(16) bytes) per iteration.
 */
/* Nibble to byte lookup table implementing the GF(16) -> GF(256) lifting */
#define GF256_LIFT_FROM_GF16_LOOKUP_NEON() \
	((uint8x16_t){0x00, 0x01, 0xE0, 0xE1, 0x5D, 0x5C, 0xBD, 0xBC, \
	              0xB0, 0xB1, 0x50, 0x51, 0xED, 0xEC, 0x0D, 0x0C})

/*
 * Expand the 16 packed nibbles held in the low 8 bytes of 'packed' into 16
 * bytes, mapping each nibble through the byte lookup 'lut'.
 * With the plain lifting table this is the GF(16) -> GF(256) lifting; with a
 * table premultiplied by a constant this fuses the lifting and the constant
 * multiplication into a single pass.
 */
static inline uint8x16_t gf256_expand_nibbles_neon(uint8x16_t packed, uint8x16_t lut) {
	const uint8x16_t nib_mask = vdupq_n_u8(0x0F);
	uint8x16_t gf16 = vcombine_u8(vget_low_u8(packed), vdup_n_u8(0));
	/* Separate the two nibbles stored in each byte */
	uint8x16_t lo_nibs = vandq_u8(gf16, nib_mask);  /* even GF(16) elements */
	uint8x16_t hi_nibs = vshrq_n_u8(gf16, 4);       /* odd  GF(16) elements */
	/* Interleave: lane k = gf16_elt[k] (elements 0..15 in order) */
	uint8x16_t interleaved = vzipq_u8(lo_nibs, hi_nibs).val[0];

	return neon_tbl_u8q(lut, interleaved);
}

static inline void gf256_vect_lift_from_gf16_neon(const uint8_t *b_gf16, uint8_t *c_gf256, uint32_t len) {
	/* Lifting table: GF(16) element i -> its GF(256) canonical representative */
	const uint8x16_t lifting_tbl = GF256_LIFT_FROM_GF16_LOOKUP_NEON();
	uint32_t i;

	for (i = 0; i < len; i += 16) {
		uint32_t rem = (len - i < 16) ? (len - i) : 16;
		/* Load up to 8 bytes of GF(16) data covering 'rem' elements */
		uint32_t bytes_in = (rem + 1) / 2;
		uint8x16_t gf16_raw = load_incomplete_neon(&b_gf16[i / 2], bytes_in);

		store_incomplete_neon(gf256_expand_nibbles_neon(gf16_raw, lifting_tbl), &c_gf256[i], rem);
	}
	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(16) and a vector in GF(256).
 */
static inline void gf16_gf256_constant_vect_mult_neon(uint8_t a_gf16, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	uint8_t a_gf256;
	gf256_vect_lift_from_gf16_ref(&a_gf16, &a_gf256, 1);
	gf256_constant_vect_mult_neon(a_gf256, b_gf256, c_gf256, n);
	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(16).
 */
static inline void gf256_gf16_constant_vect_mult_neon(uint8_t a_gf256, const uint8_t *b_gf16, uint8_t *c_gf256, uint32_t n) {
	/* The lifting and the multiplication by the constant are fused: the 16
	 * entry lifting table is premultiplied by a_gf256 once, then a single TBL
	 * per output block produces the lifted and multiplied bytes directly.
	 * This removes one region multiply per block plus a full extra pass over
	 * the output buffer.
	 * NOTE: the table content depends on the (possibly secret) constant, but
	 * the lookup indices are register operands of TBL, whose timing is data
	 * independent: no secret indexed memory access is introduced. */
	const uint8x16_t lut = gf256_mult_vectorized_neon(vdupq_n_u8(a_gf256), GF256_LIFT_FROM_GF16_LOOKUP_NEON());
	uint32_t i;

	for (i = 0; i < n; i += 16) {
		uint32_t rem = (n - i < 16) ? (n - i) : 16;
		uint32_t bytes_in = (rem + 1) / 2;
		uint8x16_t gf16_raw = load_incomplete_neon(&b_gf16[i / 2], bytes_in);

		store_incomplete_neon(gf256_expand_nibbles_neon(gf16_raw, lut), &c_gf256[i], rem);
	}
	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(16) and a vector in GF(256).
 */
static inline uint8_t gf16_gf256_vect_mult_neon(const uint8_t *a_gf16, const uint8_t *b_gf256, uint32_t len) {
	uint32_t i;
	uint8x16_t accu = vdupq_n_u8(0);

	/* NOTE: the lifting is kept fully in registers here; the previous version
	 * round-tripped the lifted block through a stack buffer on every block. */
	for (i = 0; i < len; i += 16) {
		uint32_t rem = (len - i < 16) ? (len - i) : 16;
		uint8x16_t packed = load_incomplete_neon(&a_gf16[i / 2], (rem + 1) / 2);
		/* Zero the lifted elements beyond the vector length: the last loaded
		 * byte may carry a nibble past the end of the vector. */
		uint8x16_t _a = vandq_u8(gf256_expand_nibbles_neon(packed, GF256_LIFT_FROM_GF16_LOOKUP_NEON()),
		                         gf_prefix_mask_neon(rem));
		uint8x16_t _b = load_incomplete_neon(&b_gf256[i], rem);
		accu = veorq_u8(accu, gf256_mult_vectorized_neon(_a, _b));
	}
	return sum_uint8_neon(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(16).
 */
static inline uint8_t gf256_gf16_vect_mult_neon(const uint8_t *a_gf256, const uint8_t *b_gf16, uint32_t n) {
	return gf16_gf256_vect_mult_neon(b_gf16, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(16),
 * resulting in a vector in GF(256).
 */
static inline void gf256_gf16_mat_mult_neon(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	/* Same scheme as gf256_mat_mult_neon(), except that the common vector X is
	 * lifted from GF(16) once for the whole matrix instead of once per row.
	 * NOTE: like GF256_GF16_MAT_MULT (i.e. GF256_GF2_MAT_MULT), TRI_SUP is
	 * handled as REG here. */
	uint8x16_t xb[GF_MULT_PANEL_BLOCKS_NEON];
	const uint8x16_t lifting_tbl = GF256_LIFT_FROM_GF16_LOOKUP_NEON();
	const uint32_t nblocks = (n + 15) / 16;
	uint32_t b, p;

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS_NEON) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS_NEON) {
			np = GF_MULT_PANEL_BLOCKS_NEON;
		}
		for (b = 0; b < np; b++) {
			uint32_t off = 16 * (p + b);
			uint32_t nelts = (n - off) > 16 ? 16 : (n - off);
			uint8x16_t packed = load_incomplete_neon(&X[off / 2], (nelts + 1) / 2);
			xb[b] = gf256_expand_nibbles_neon(packed, lifting_tbl);
			if (nelts < 16) {
				/* Zero pad past the end of the vector */
				xb[b] = vandq_u8(xb[b], gf_prefix_mask_neon(nelts));
			}
		}
		GF_MAT_MULT_PANEL_NEON(A, Y, n, p, np, xb, (mtype == TRI_INF), (p == 0),
		                       1, gf256_mult_vectorized_neon, sum_uint8_neon);
	}

	return;
}

/* ==========================================================================
 * GF(256^2)
 * ========================================================================== */

/* NOTE: for atomic multiplication, using vectorization is suboptimal */
static inline uint16_t gf256to2_mult_neon(uint16_t x, uint16_t y) {
	return gf256to2_mult_ref(x, y);
}

/*
 * Vectorized multiplication in GF(256^2).
 * Each GF(256^2) element occupies one uint16_t lane: low byte = a0 (coeff of 1),
 * high byte = a1 (coeff of alpha), with x^2 + x + 0x20 = 0 as reduction poly.
 *
 * Uses Karatsuba over GF(256):
 *   c0 = a0*b0  XOR  0x20*(a1*b1)
 *   c1 = a0*b0  XOR  (a0 XOR a1)*(b0 XOR b1)
 */
static inline uint8x16_t gf256to2_mult_vectorized_neon(uint8x16_t _a, uint8x16_t _b) {
	const uint8x16_t beta    = vdupq_n_u8(0x20);
	/* Duplicate low byte of each uint16_t to both bytes of the lane */
	const uint8x16_t shuff_lo = { 0, 0, 2, 2, 4, 4, 6, 6, 8, 8,10,10,12,12,14,14};
	/* Duplicate high byte of each uint16_t to both bytes of the lane */
	const uint8x16_t shuff_hi = { 1, 1, 3, 3, 5, 5, 7, 7, 9, 9,11,11,13,13,15,15};
	/* Select even bytes (0xFF) vs odd bytes (0x00) in each uint16_t lane */
	const uint8x16_t mask_even = {0xFF,0x00, 0xFF,0x00, 0xFF,0x00, 0xFF,0x00,
	                               0xFF,0x00, 0xFF,0x00, 0xFF,0x00, 0xFF,0x00};

	/* Spread a0 (low) and a1 (high) to fill all 16 bytes of each lane */
	uint8x16_t a0 = neon_tbl_u8q(_a, shuff_lo); /* {a0,a0, a0,a0, ...} per element */
	uint8x16_t a1 = neon_tbl_u8q(_a, shuff_hi); /* {a1,a1, a1,a1, ...} per element */
	uint8x16_t b0 = neon_tbl_u8q(_b, shuff_lo);
	uint8x16_t b1 = neon_tbl_u8q(_b, shuff_hi);

	/* Karatsuba: four GF(256) products (all computed byte-lane-parallel) */
	uint8x16_t z00   = gf256_mult_vectorized_neon(a0, b0);              /* a0*b0 dup */
	uint8x16_t z11   = gf256_mult_vectorized_neon(a1, b1);              /* a1*b1 dup */
	uint8x16_t z11_b = gf256_mult_vectorized_neon(z11, beta);           /* 0x20*a1*b1 dup */
	uint8x16_t cross = gf256_mult_vectorized_neon(veorq_u8(a0, a1),
	                                               veorq_u8(b0, b1));   /* (a0+a1)*(b0+b1) dup */

	/* c0 (goes to low byte of each uint16_t result): a0*b0 + beta*a1*b1 */
	uint8x16_t c0_dup = veorq_u8(z00, z11_b);
	/* c1 (goes to high byte of each uint16_t result): a0*b0 + (a0+a1)*(b0+b1) */
	uint8x16_t c1_dup = veorq_u8(z00, cross);

	/* Interleave: even-position bytes from c0_dup, odd-position bytes from c1_dup */
	return vbslq_u8(mask_even, c0_dup, c1_dup);
}

/* Byte shuffle masks splitting a block of GF(256^2) elements into its two
 * GF(256) coordinates, each duplicated over the whole 16-bit lane. */
#define GF256TO2_DUP_COORD0_NEON() \
	((uint8x16_t){0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14})
#define GF256TO2_DUP_COORD1_NEON() \
	((uint8x16_t){1, 1, 3, 3, 5, 5, 7, 7, 9, 9, 11, 11, 13, 13, 15, 15})

/*
 * Multiply every GF(256^2) element of the block by the generator t (the root
 * of x^2 + x + 0x20). With b = b0 + b1*t one has t*b = (0x20*b1) + (b0+b1)*t.
 */
static inline uint8x16_t gf256to2_mult_by_t_neon(uint8x16_t b) {
	const uint8x16_t swap_msk = {1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14};
	const uint8x16_t mask_even = {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	                              0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00
	                             };
	uint8x16_t b1_dup = neon_tbl_u8q(b, GF256TO2_DUP_COORD1_NEON());
	uint8x16_t m32 = gf256_mult_vectorized_neon(b1_dup, vdupq_n_u8(0x20));
	uint8x16_t sum = veorq_u8(b, neon_tbl_u8q(b, swap_msk));

	/* Coordinate 0 (even byte) <- 0x20*b1, coordinate 1 (odd byte) <- b0+b1 */
	return vbslq_u8(mask_even, m32, sum);
}

/*
 * Multiply a block of GF(256^2) elements by a common block X, given the pair
 * {X, t*X} precomputed once: a * X = a0 * X + a1 * (t * X), where a0 and a1
 * are GF(256) scalars, so only two bytewise GF(256) products are needed
 * instead of the four of the generic Karatsuba multiply.
 */
#define GF256TO2_MAC2_NEON(a, xpair) \
	veorq_u8(gf256_mult_vectorized_neon(neon_tbl_u8q((a), GF256TO2_DUP_COORD0_NEON()), (xpair)[0]), \
	         gf256_mult_vectorized_neon(neon_tbl_u8q((a), GF256TO2_DUP_COORD1_NEON()), (xpair)[1]))

/*
 * Vector multiplied by a constant in GF(256^2).
 *
 * Same Karatsuba structure as gf256to2_mult_vectorized_neon() (c0 = a0*b0 XOR
 * beta*a1*b1, c1 = a0*b0 XOR (a0 XOR a1)*(b0 XOR b1)), but since b = (b0, b1)
 * is loop-invariant, each of the three distinct GF(256) multiplies-by-a-
 * constant involved (by b0, by beta*b1, and by (b0 XOR b1)) is turned into a
 * gf256_const_mult_block_neon() region-multiply against a table built once
 * per call. Folding the constant 'beta' into the b1 table ahead of time
 * (b1_beta = beta*b1) collapses what would otherwise be two chained multiplies
 * (a1*b1 then *beta) into a single table lookup for the a1*b1*beta term.
 */
static inline void gf256to2_constant_vect_mult_neon(uint16_t b, const uint16_t *a, uint16_t *c, uint32_t len) {
	uint32_t i;
	uint8_t b0 = (uint8_t)(b & 0xff);
	uint8_t b1 = (uint8_t)(b >> 8);
	/* fold reduction constant beta=0x20 into b1's table */
	uint8_t b1_beta = vgetq_lane_u8(gf256_mult_vectorized_neon(vdupq_n_u8(b1), vdupq_n_u8(0x20)), 0);
	uint8_t bsum = (uint8_t)(b0 ^ b1);
	uint8x16_t vtbl_b0_lo, vtbl_b0_hi;
	uint8x16_t vtbl_b1beta_lo, vtbl_b1beta_hi;
	uint8x16_t vtbl_bsum_lo, vtbl_bsum_hi;

	/* NOTE: the six tables used to be filled by 96 scalar gf256_mult_ref()
	 * calls; six vector multiplications do the same job. */
	gf256_build_const_tables_neon(b0, &vtbl_b0_lo, &vtbl_b0_hi);
	gf256_build_const_tables_neon(b1_beta, &vtbl_b1beta_lo, &vtbl_b1beta_hi);
	gf256_build_const_tables_neon(bsum, &vtbl_bsum_lo, &vtbl_bsum_hi);

	/* Duplicate low byte of each uint16_t to both bytes of the lane */
	const uint8x16_t shuff_lo = { 0, 0, 2, 2, 4, 4, 6, 6, 8, 8,10,10,12,12,14,14};
	/* Duplicate high byte of each uint16_t to both bytes of the lane */
	const uint8x16_t shuff_hi = { 1, 1, 3, 3, 5, 5, 7, 7, 9, 9,11,11,13,13,15,15};
	/* Select even bytes (0xFF) vs odd bytes (0x00) in each uint16_t lane */
	const uint8x16_t mask_even = {0xFF,0x00, 0xFF,0x00, 0xFF,0x00, 0xFF,0x00,
	                               0xFF,0x00, 0xFF,0x00, 0xFF,0x00, 0xFF,0x00};

	for (i = 0; i < (2 * len); i += 16) {
		uint32_t rem = ((2 * len) - i < 16) ? (2 * len) - i : 16;
		uint8x16_t _a = load_incomplete_neon((const uint8_t*)&a[i / 2], rem);

		/* Spread a0 (low) and a1 (high) to fill all 16 bytes of each lane */
		uint8x16_t a0_dup = neon_tbl_u8q(_a, shuff_lo);
		uint8x16_t a1_dup = neon_tbl_u8q(_a, shuff_hi);

		uint8x16_t z00_dup     = gf256_const_mult_block_neon(a0_dup, vtbl_b0_lo, vtbl_b0_hi);         /* a0*b0 dup */
		uint8x16_t z11beta_dup = gf256_const_mult_block_neon(a1_dup, vtbl_b1beta_lo, vtbl_b1beta_hi); /* beta*a1*b1 dup */
		uint8x16_t axor_dup    = veorq_u8(a0_dup, a1_dup);
		uint8x16_t cross_dup   = gf256_const_mult_block_neon(axor_dup, vtbl_bsum_lo, vtbl_bsum_hi);   /* (a0+a1)*(b0+b1) dup */

		/* c0 (low byte of each uint16_t result): a0*b0 + beta*a1*b1 */
		uint8x16_t c0_dup = veorq_u8(z00_dup, z11beta_dup);
		/* c1 (high byte of each uint16_t result): a0*b0 + (a0+a1)*(b0+b1) */
		uint8x16_t c1_dup = veorq_u8(z00_dup, cross_dup);

		store_incomplete_neon(vbslq_u8(mask_even, c0_dup, c1_dup),
		                      (uint8_t*)&c[i / 2], rem);
	}
	return;
}

/*
 * Vector to vector multiplication in GF(256^2).
 */
static inline uint16_t gf256to2_vect_mult_neon(const uint16_t *a, const uint16_t *b, uint32_t len) {
	uint32_t i;
	uint8x16_t accu = vdupq_n_u8(0);

	for (i = 0; i < (2 * len); i += 16) {
		uint32_t rem = ((2 * len) - i < 16) ? (2 * len) - i : 16;
		uint8x16_t _a = load_incomplete_neon((const uint8_t*)&a[i / 2], rem);
		uint8x16_t _b = load_incomplete_neon((const uint8_t*)&b[i / 2], rem);
		accu = veorq_u8(accu, gf256to2_mult_vectorized_neon(_a, _b));
	}
	return sum_uint16_neon(accu);
}

/*
 * Vector to vector batched multiplication in GF(256^2).
 */
static inline void gf256to2_vect_mult_multiple_public_neon(uint16_t* const* c, const uint16_t *a, const uint16_t* const* b, uint32_t len, uint32_t n) {
	uint32_t i;
	for (i = 0; i < n; i++) {
		*c[i] = gf256to2_vect_mult_neon(a, b[i], len);
	}
}

/* Matrix and vector multiplication over GF(256^2) */
static inline void gf256to2_mat_mult_neon(const uint16_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	/* Each block of the common vector is kept together with its product by the
	 * generator t, so that a row block only costs two bytewise GF(256)
	 * products instead of the four of the generic GF(256^2) multiply. */
	uint8x16_t xb[GF_MULT_PANEL_BLOCKS_NEON][2];
	const uint8_t *Ab = (const uint8_t*)(const void*)A;
	const uint8_t *Xb = (const uint8_t*)(const void*)X;
	const uint32_t nbytes = 2 * n;
	const uint32_t nblocks = (nbytes + 15) / 16;
	uint32_t b, p;

	/* TRI_SUP starts each row at a different, non block aligned column, which
	 * does not fit the scheme below; it is unused by MQOM and is left on the
	 * generic path. */
	if (mtype == TRI_SUP) {
		GF256to2_MAT_MULT(A, X, Y, n, mtype, gf256to2_vect_mult_neon);
		return;
	}

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS_NEON) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS_NEON) {
			np = GF_MULT_PANEL_BLOCKS_NEON;
		}
		for (b = 0; b < np; b++) {
			uint32_t off = 16 * (p + b);
			if ((off + 16) <= nbytes) {
				xb[b][0] = vld1q_u8(&Xb[off]);
			} else {
				xb[b][0] = load_incomplete_neon(&Xb[off], nbytes - off);
			}
			xb[b][1] = gf256to2_mult_by_t_neon(xb[b][0]);
		}
		GF_MAT_MULT_PANEL_NEON(Ab, Y, n, p, np, xb, (mtype == TRI_INF), (p == 0),
		                       2, GF256TO2_MAC2_NEON, sum_uint16_neon);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(2) and a vector in GF(256^2).
 */
static inline void gf2_gf256to2_constant_vect_mult_neon(uint8_t a_gf2, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	gf2_gf256to2_constant_vect_mult_ref(a_gf2, b_gf256to2, c_gf256to2, n);
	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(2).
 * Processes 8 GF(256^2) elements (= 1 GF(2) byte) per NEON register.
 */
static inline void gf256to2_gf2_constant_vect_mult_neon(uint16_t a_gf256to2, const uint8_t *b_gf2, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;
	uint8x16_t _a = vreinterpretq_u8_u16(vdupq_n_u16(a_gf256to2));

	for (i = 0; i < n; i += 8) {
		uint32_t rem = (n - i < 8) ? (n - i) : 8;
		uint8x16_t sel = gf2_expand_8bits_neon(&b_gf2[i / 8]);
		store_incomplete_neon(vandq_u8(_a, sel), (uint8_t*)&c_gf256to2[i], 2 * rem);
	}
	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(256).
 * Each GF(256) scalar b[i] gives a GF(256^2) element (a0*b[i], a1*b[i]).
 */
static inline void gf256to2_gf256_constant_vect_mult_neon(uint16_t a_gf256to2, const uint8_t *b_gf256, uint16_t *c_gf256to2, uint32_t n) {
	/* Each element of b_gf256 is a GF(256) scalar, so this is a plain bytewise
	 * GF(256) product of the broadcast constant by the duplicated element:
	 * two region tables built once are enough. */
	const uint8x16_t dup_msk = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};
	const uint8x16_t a_dup = vreinterpretq_u8_u16(vdupq_n_u16(a_gf256to2));
	uint32_t i;

	/* NOTE: the region multiply tables cannot be used here because the
	 * "constant" alternates a0/a1 over the byte lanes; the generic PMULL
	 * multiply is used instead, which is still far cheaper than the fully
	 * scalar reference fallback this replaces. */
	for (i = 0; i < n; i += 8) {
		uint32_t rem = (n - i < 8) ? (n - i) : 8;
		uint8x16_t _b = load_incomplete_neon(&b_gf256[i], rem);
		/* Duplicate each byte over its 16-bit lane */
		_b = neon_tbl_u8q(_b, dup_msk);
		store_incomplete_neon(gf256_mult_vectorized_neon(a_dup, _b), (uint8_t*)&c_gf256to2[i], 2 * rem);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256) and a vector in GF(256^2).
 * Each GF(256^2) element (a0, a1) is multiplied byte-wise: (s*a0, s*a1).
 */
static inline void gf256_gf256to2_constant_vect_mult_neon(uint8_t a_gf256, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	/* Multiplying a GF(256^2) vector by a GF(256) scalar is a plain bytewise
	 * GF(256) product, so the region multiply tables of the constant apply
	 * directly. */
	uint8x16_t tbl_lo, tbl_hi;
	uint32_t i;

	gf256_build_const_tables_neon(a_gf256, &tbl_lo, &tbl_hi);
	for (i = 0; i < (2 * n); i += 16) {
		uint32_t rem = ((2 * n) - i < 16) ? ((2 * n) - i) : 16;
		uint8x16_t _b = load_incomplete_neon((const uint8_t*)&b_gf256to2[i / 2], rem);
		store_incomplete_neon(gf256_const_mult_block_neon(_b, tbl_lo, tbl_hi), (uint8_t*)&c_gf256to2[i / 2], rem);
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(2) and a vector in GF(256^2).
 */
static inline uint16_t gf2_gf256to2_vect_mult_neon(const uint8_t *a_gf2, const uint16_t *b_gf256to2, uint32_t n) {
	uint32_t i;
	uint8x16_t accu = vdupq_n_u8(0);

	for (i = 0; i < n; i += 8) {
		uint32_t rem = (n - i < 8) ? (n - i) : 8;
		uint8x16_t sel = gf2_expand_8bits_neon(&a_gf2[i / 8]);
		uint8x16_t _b  = load_incomplete_neon((const uint8_t*)&b_gf256to2[i], 2 * rem);
		accu = veorq_u8(accu, vandq_u8(sel, _b));
	}
	return sum_uint16_neon(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(2).
 */
static inline uint16_t gf256to2_gf2_vect_mult_neon(const uint16_t *a_gf256to2, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256to2_vect_mult_neon(b_gf2, a_gf256to2, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(2) and a vector in GF(256^2),
 * resulting in a vector in GF(256^2).
 */
static inline void gf2_gf256to2_mat_mult_neon(const uint8_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF2_GF256to2_MAT_MULT(A, X, Y, n, mtype, gf2_gf256to2_vect_mult_neon);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(2),
 * resulting in a vector in GF(256^2).
 */
static inline void gf256to2_gf2_mat_mult_neon(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_GF2_MAT_MULT(A, X, Y, n, mtype, gf256to2_gf2_vect_mult_neon);
}

/* GF(256^2) matrix transposition */
static inline void gf256to2_mat_transpose_neon(const uint16_t *A, uint16_t *B, uint32_t n, matrix_type mtype) {
	gf256to2_mat_transpose_ref(A, B, n, mtype);
}

/*
 * "Hybrid" constant multiplication of a constant in GF(16) and a vector in GF(256^2).
 */
static inline void gf16_gf256to2_constant_vect_mult_neon(uint8_t a_gf16, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	uint8_t a_gf256;
	gf256_vect_lift_from_gf16_ref(&a_gf16, &a_gf256, 1);
	gf256_gf256to2_constant_vect_mult_neon(a_gf256, b_gf256to2, c_gf256to2, n);
	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(16).
 */
static inline void gf256to2_gf16_constant_vect_mult_neon(uint16_t a_gf256to2, const uint8_t *b_gf16, uint16_t *c_gf256to2, uint32_t n) {
	/* Use end of output buffer as temporary for the lifted GF(256) values */
	uint8_t *buf = ((uint8_t*) c_gf256to2) + n;
	gf256_vect_lift_from_gf16_neon(b_gf16, buf, n);
	gf256to2_gf256_constant_vect_mult_neon(a_gf256to2, buf, c_gf256to2, n);
	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(16) and a vector in GF(256^2).
 * Each GF(16) element is lifted to GF(256) and then multiplies the corresponding
 * GF(256^2) element byte-wise: result[i] = (a[i]*b0[i], a[i]*b1[i]).
 */
static inline uint16_t gf16_gf256to2_vect_mult_neon(const uint8_t *a_gf16, const uint16_t *b_gf256to2, uint32_t len) {
	uint32_t i;
	uint8x16_t accu = vdupq_n_u8(0);
	/* Duplicate each GF(256) byte to fill a uint16_t slot: {a,a, a,a, ...} */
	const uint8x16_t dup_msk = {0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7};

	/* NOTE: the lifting is kept fully in registers here; the previous version
	 * round-tripped the lifted block through a stack buffer on every block. */
	for (i = 0; i < (2 * len); i += 16) {
		uint32_t rem    = ((2 * len) - i < 16) ? (2 * len) - i : 16;
		uint32_t gf16_n = rem / 2; /* number of GF(16) elements for this block */
		uint8x16_t packed = load_incomplete_neon(&a_gf16[i / 4], (gf16_n + 1) / 2);
		uint8x16_t _a = vandq_u8(gf256_expand_nibbles_neon(packed, GF256_LIFT_FROM_GF16_LOOKUP_NEON()),
		                         gf_prefix_mask_neon(gf16_n));
		/* Duplicate each lifted byte: {a[0],a[0], a[1],a[1], ...} */
		_a = neon_tbl_u8q(_a, dup_msk);
		/* Load GF(256^2) elements */
		uint8x16_t _b = load_incomplete_neon((const uint8_t*)&b_gf256to2[i / 2], rem);
		/* Scalar GF(256) x GF(256^2) = byte-wise GF(256) multiplication */
		accu = veorq_u8(accu, gf256_mult_vectorized_neon(_a, _b));
	}
	return sum_uint16_neon(accu);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(16).
 */
static inline uint16_t gf256to2_gf16_vect_mult_neon(const uint16_t *a_gf256to2, const uint8_t *b_gf16, uint32_t n) {
	return gf16_gf256to2_vect_mult_neon(b_gf16, a_gf256to2, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(16),
 * resulting in a vector in GF(256^2).
 */
static inline void gf256to2_gf16_mat_mult_neon(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	/* Each element of X is a GF(16) value lifted into the GF(256) subfield, so
	 * the product against a GF(256^2) row element is a plain bytewise GF(256)
	 * product: the common vector is lifted and duplicated once per matrix.
	 * NOTE: like GF256to2_GF16_MAT_MULT (i.e. GF256_GF2_MAT_MULT), TRI_SUP is
	 * handled as REG here. */
	uint8x16_t xb[GF_MULT_PANEL_BLOCKS_NEON];
	const uint8x16_t lifting_tbl = GF256_LIFT_FROM_GF16_LOOKUP_NEON();
	const uint8x16_t dup_msk = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};
	const uint8_t *Ab = (const uint8_t*)(const void*)A;
	const uint32_t nbytes = 2 * n;
	const uint32_t nblocks = (nbytes + 15) / 16;
	uint32_t b, p;

	for (p = 0; p < nblocks; p += GF_MULT_PANEL_BLOCKS_NEON) {
		uint32_t np = nblocks - p;
		if (np > GF_MULT_PANEL_BLOCKS_NEON) {
			np = GF_MULT_PANEL_BLOCKS_NEON;
		}
		for (b = 0; b < np; b++) {
			/* A 16-byte block covers 8 GF(256^2) elements, hence 8 GF(16)
			 * elements of X packed into 4 bytes. */
			uint32_t off = 16 * (p + b);
			uint32_t nelts = ((nbytes - off) / 2) > 8 ? 8 : ((nbytes - off) / 2);
			uint8x16_t packed = load_incomplete_neon(&X[off / 4], (nelts + 1) / 2);
			uint8x16_t lifted = gf256_expand_nibbles_neon(packed, lifting_tbl);
			if (nelts < 8) {
				lifted = vandq_u8(lifted, gf_prefix_mask_neon(nelts));
			}
			/* Duplicate each lifted byte over its 16-bit lane */
			xb[b] = neon_tbl_u8q(lifted, dup_msk);
		}
		GF_MAT_MULT_PANEL_NEON(Ab, Y, n, p, np, xb, (mtype == TRI_INF), (p == 0),
		                       2, gf256_mult_vectorized_neon, sum_uint16_neon);
	}

	return;
}

#endif /* defined(__ARM_NEON) || defined(__ARM_NEON__) */

#endif /* __FIELDS_ARM_NEON_H__ */
