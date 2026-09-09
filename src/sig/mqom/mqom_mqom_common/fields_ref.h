#ifndef __FIELDS_REF_H__
#define __FIELDS_REF_H__

#include "fields_common.h"
#include <string.h>

/* === GF(2) === */
/* GF(2) elements addition is xor, multiplication is and */
static inline uint8_t gf2_mult_ref(uint8_t a, uint8_t b) {
	return a & b;
}

/*
 * Vector multiplied by a constant in GF(2).
 */
static inline void gf2_constant_vect_mult_ref(uint8_t b, const uint8_t *a, uint8_t *c, uint32_t len) {
	uint32_t i;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;
	uint8_t b_ = b & mask;

	/* Constant-time selection */
	for (i = 0; i < (len / 8); i++) {
		c[i] = -(b_) & a[i];
	}

	return;
}

/*
 * Vector to vector multiplication in GF(2).
 * Takes two vectors of length 'len', and returns a bit (element in GF(2))
 */
static inline uint8_t gf2_vect_mult_ref(const uint8_t *a, const uint8_t *b, uint32_t len) {
	uint32_t i;
	uint8_t res;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t acc;
	volatile uint8_t mask = 1;

	acc = 0;
	for (i = 0; i < (len / 8); i++) {
		acc ^= (a[i] & b[i]);
	}
	/* Compute the parity of acc as the output result */
	res = 0;
	for (i = 0; i < 8; i++) {
		res ^= (acc >> i) & mask;
	}

	return res;
}

/* Matrix and vector multiplication over GF(2)
 * C = A * X, where X is a vector
 * Matrix is supposed to be square n x n, and vector n x 1
 * The output is a vector n x 1
 * */
static inline void gf2_mat_mult_ref(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF2_MAT_MULT(A, X, Y, n, mtype, gf2_vect_mult_ref);
}

/* GF(2) matrix transposition */
static inline void gf2_mat_transpose_ref(const uint8_t *A, uint8_t *B, uint32_t n, matrix_type mtype) {
	uint32_t i, j;
	uint32_t msize = (n / 8);
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;

	/* Zeroize the matrix */
	for (i = 0; i < (n * msize); i++) {
		B[i] = 0;
	}
	switch (mtype) {
	case REG: {
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				B[(n * j / 8) + (i / 8)] |= ((A[(n * i / 8) + (j / 8)] >> (j % 8)) & mask) << (i % 8);
			}
		}
		break;
	}
	case TRI_SUP: {
		for (i = 0; i < n; i++) {
			for (j = i; j < n; j++) {
				B[(n * j / 8) + (i / 8)] |= ((A[(n * i / 8) + (j / 8)] >> (j % 8)) & mask) << (i % 8);
			}
		}
		break;
	}
	case TRI_INF: {
		for (i = 0; i < n; i++) {
			for (j = 0; j <= i; j++) {
				B[(n * j / 8) + (i / 8)] |= ((A[(n * i / 8) + (j / 8)] >> (j % 8)) & mask) << (i % 8);
			}
		}
		break;
	}
	default: {
		break;
	}
	}
}

/* === GF(256) === */
#define GF256_MODULUS 0x1B /* The GF(2^8) modulus (Rijndael polynomial) */

/* Broadcast the low bit of each byte of 'm' to the whole byte, i.e. compute
 * m * 0xFF lane-wise. Since only bits 0, 8, 16 and 24 of 'm' can be set, the
 * product never carries across byte boundaries and (m << 8) - m is exactly
 * m * 0xFF modulo 2^32. This costs 2 operations instead of the 9 needed by
 * the "shift/mask/or" doubling ladder, and unlike a multiplication by 0xFF
 * it maps to a plain shift + subtract that SIMD back-ends (and every
 * auto-vectorizer) handle natively. */
static inline uint32_t gf256_lane_bcast_x4(uint32_t m) {
	return (m << 8) - m;
}

/* XOR-fold the four bytes of a SWAR word into a single GF(256) element. */
static inline uint8_t gf256_lane_fold_x4(uint32_t v) {
	v ^= (v >> 16);
	v ^= (v >> 8);
	return (uint8_t)v;
}

/* The two GF(256) table options are mutually exclusive */
#if defined(USE_GF256_TABLE_MULT) && defined(USE_GF256_TABLE_LOG_EXP)
#error "USE_GF256_TABLE_MULT and USE_GF256_TABLE_LOG_EXP are mutually exclusive (and differ in constant-timeness): pick one"
#endif

#if defined(USE_GF256_TABLE_MULT)
/* If we are asked to, we use the 65 kB multiplication table */
#include "gf256_mult_table.h"
static inline uint8_t gf256_mult_ref(uint8_t x, uint8_t y) {
	return gf256_mult_table[((uint16_t)y << 8) | x];
}
#elif defined(USE_GF256_TABLE_LOG_EXP)
/* If we are asked to, we use the log/exp tables, we use three 256 bytes tables in SRAM to be constant time
 * XXX: to be in SRAM in embedded systems, we do not use the 'const' keyword hereafter on purpose */
/* The 'exp' table of Rijndael GF(256), doubled on 512 bytes to avoid a modular reduction mod 255 */
__attribute__((weak)) EMBEDDED_SRAM const uint8_t rijndael_exp[] = {
	0x01, 0x03, 0x05, 0x0f, 0x11, 0x33, 0x55, 0xff, 0x1a, 0x2e, 0x72, 0x96, 0xa1, 0xf8,
	0x13, 0x35, 0x5f, 0xe1, 0x38, 0x48, 0xd8, 0x73, 0x95, 0xa4, 0xf7, 0x02, 0x06, 0x0a,
	0x1e, 0x22, 0x66, 0xaa, 0xe5, 0x34, 0x5c, 0xe4, 0x37, 0x59, 0xeb, 0x26, 0x6a, 0xbe,
	0xd9, 0x70, 0x90, 0xab, 0xe6, 0x31, 0x53, 0xf5, 0x04, 0x0c, 0x14, 0x3c, 0x44, 0xcc,
	0x4f, 0xd1, 0x68, 0xb8, 0xd3, 0x6e, 0xb2, 0xcd, 0x4c, 0xd4, 0x67, 0xa9, 0xe0, 0x3b,
	0x4d, 0xd7, 0x62, 0xa6, 0xf1, 0x08, 0x18, 0x28, 0x78, 0x88, 0x83, 0x9e, 0xb9, 0xd0,
	0x6b, 0xbd, 0xdc, 0x7f, 0x81, 0x98, 0xb3, 0xce, 0x49, 0xdb, 0x76, 0x9a, 0xb5, 0xc4,
	0x57, 0xf9, 0x10, 0x30, 0x50, 0xf0, 0x0b, 0x1d, 0x27, 0x69, 0xbb, 0xd6, 0x61, 0xa3,
	0xfe, 0x19, 0x2b, 0x7d, 0x87, 0x92, 0xad, 0xec, 0x2f, 0x71, 0x93, 0xae, 0xe9, 0x20,
	0x60, 0xa0, 0xfb, 0x16, 0x3a, 0x4e, 0xd2, 0x6d, 0xb7, 0xc2, 0x5d, 0xe7, 0x32, 0x56,
	0xfa, 0x15, 0x3f, 0x41, 0xc3, 0x5e, 0xe2, 0x3d, 0x47, 0xc9, 0x40, 0xc0, 0x5b, 0xed,
	0x2c, 0x74, 0x9c, 0xbf, 0xda, 0x75, 0x9f, 0xba, 0xd5, 0x64, 0xac, 0xef, 0x2a, 0x7e,
	0x82, 0x9d, 0xbc, 0xdf, 0x7a, 0x8e, 0x89, 0x80, 0x9b, 0xb6, 0xc1, 0x58, 0xe8, 0x23,
	0x65, 0xaf, 0xea, 0x25, 0x6f, 0xb1, 0xc8, 0x43, 0xc5, 0x54, 0xfc, 0x1f, 0x21, 0x63,
	0xa5, 0xf4, 0x07, 0x09, 0x1b, 0x2d, 0x77, 0x99, 0xb0, 0xcb, 0x46, 0xca, 0x45, 0xcf,
	0x4a, 0xde, 0x79, 0x8b, 0x86, 0x91, 0xa8, 0xe3, 0x3e, 0x42, 0xc6, 0x51, 0xf3, 0x0e,
	0x12, 0x36, 0x5a, 0xee, 0x29, 0x7b, 0x8d, 0x8c, 0x8f, 0x8a, 0x85, 0x94, 0xa7, 0xf2,
	0x0d, 0x17, 0x39, 0x4b, 0xdd, 0x7c, 0x84, 0x97, 0xa2, 0xfd, 0x1c, 0x24, 0x6c, 0xb4,
	0xc7, 0x52, 0xf6, 0x01, 0x03, 0x05, 0x0f, 0x11, 0x33, 0x55, 0xff, 0x1a, 0x2e, 0x72,
	0x96, 0xa1, 0xf8, 0x13, 0x35, 0x5f, 0xe1, 0x38, 0x48, 0xd8, 0x73, 0x95, 0xa4, 0xf7,
	0x02, 0x06, 0x0a, 0x1e, 0x22, 0x66, 0xaa, 0xe5, 0x34, 0x5c, 0xe4, 0x37, 0x59, 0xeb,
	0x26, 0x6a, 0xbe, 0xd9, 0x70, 0x90, 0xab, 0xe6, 0x31, 0x53, 0xf5, 0x04, 0x0c, 0x14,
	0x3c, 0x44, 0xcc, 0x4f, 0xd1, 0x68, 0xb8, 0xd3, 0x6e, 0xb2, 0xcd, 0x4c, 0xd4, 0x67,
	0xa9, 0xe0, 0x3b, 0x4d, 0xd7, 0x62, 0xa6, 0xf1, 0x08, 0x18, 0x28, 0x78, 0x88, 0x83,
	0x9e, 0xb9, 0xd0, 0x6b, 0xbd, 0xdc, 0x7f, 0x81, 0x98, 0xb3, 0xce, 0x49, 0xdb, 0x76,
	0x9a, 0xb5, 0xc4, 0x57, 0xf9, 0x10, 0x30, 0x50, 0xf0, 0x0b, 0x1d, 0x27, 0x69, 0xbb,
	0xd6, 0x61, 0xa3, 0xfe, 0x19, 0x2b, 0x7d, 0x87, 0x92, 0xad, 0xec, 0x2f, 0x71, 0x93,
	0xae, 0xe9, 0x20, 0x60, 0xa0, 0xfb, 0x16, 0x3a, 0x4e, 0xd2, 0x6d, 0xb7, 0xc2, 0x5d,
	0xe7, 0x32, 0x56, 0xfa, 0x15, 0x3f, 0x41, 0xc3, 0x5e, 0xe2, 0x3d, 0x47, 0xc9, 0x40,
	0xc0, 0x5b, 0xed, 0x2c, 0x74, 0x9c, 0xbf, 0xda, 0x75, 0x9f, 0xba, 0xd5, 0x64, 0xac,
	0xef, 0x2a, 0x7e, 0x82, 0x9d, 0xbc, 0xdf, 0x7a, 0x8e, 0x89, 0x80, 0x9b, 0xb6, 0xc1,
	0x58, 0xe8, 0x23, 0x65, 0xaf, 0xea, 0x25, 0x6f, 0xb1, 0xc8, 0x43, 0xc5, 0x54, 0xfc,
	0x1f, 0x21, 0x63, 0xa5, 0xf4, 0x07, 0x09, 0x1b, 0x2d, 0x77, 0x99, 0xb0, 0xcb, 0x46,
	0xca, 0x45, 0xcf, 0x4a, 0xde, 0x79, 0x8b, 0x86, 0x91, 0xa8, 0xe3, 0x3e, 0x42, 0xc6,
	0x51, 0xf3, 0x0e, 0x12, 0x36, 0x5a, 0xee, 0x29, 0x7b, 0x8d, 0x8c, 0x8f, 0x8a, 0x85,
	0x94, 0xa7, 0xf2, 0x0d, 0x17, 0x39, 0x4b, 0xdd, 0x7c, 0x84, 0x97, 0xa2, 0xfd, 0x1c,
	0x24, 0x6c, 0xb4, 0xc7, 0x52, 0xf6, 0x01, 0x03
};
/* The 'log' table of Rijndael GF(256) */
__attribute__((weak)) EMBEDDED_SRAM const uint8_t rijndael_log[] = {
	0x00, 0x00, 0x19, 0x01, 0x32, 0x02, 0x1a, 0xc6, 0x4b, 0xc7, 0x1b, 0x68, 0x33, 0xee,
	0xdf, 0x03, 0x64, 0x04, 0xe0, 0x0e, 0x34, 0x8d, 0x81, 0xef, 0x4c, 0x71, 0x08,
	0xc8, 0xf8, 0x69, 0x1c, 0xc1, 0x7d, 0xc2, 0x1d, 0xb5, 0xf9, 0xb9, 0x27, 0x6a,
	0x4d, 0xe4, 0xa6, 0x72, 0x9a, 0xc9, 0x09, 0x78, 0x65, 0x2f, 0x8a, 0x05, 0x21,
	0x0f, 0xe1, 0x24, 0x12, 0xf0, 0x82, 0x45, 0x35, 0x93, 0xda, 0x8e, 0x96, 0x8f,
	0xdb, 0xbd, 0x36, 0xd0, 0xce, 0x94, 0x13, 0x5c, 0xd2, 0xf1, 0x40, 0x46, 0x83,
	0x38, 0x66, 0xdd, 0xfd, 0x30, 0xbf, 0x06, 0x8b, 0x62, 0xb3, 0x25, 0xe2, 0x98,
	0x22, 0x88, 0x91, 0x10, 0x7e, 0x6e, 0x48, 0xc3, 0xa3, 0xb6, 0x1e, 0x42, 0x3a,
	0x6b, 0x28, 0x54, 0xfa, 0x85, 0x3d, 0xba, 0x2b, 0x79, 0x0a, 0x15, 0x9b, 0x9f,
	0x5e, 0xca, 0x4e, 0xd4, 0xac, 0xe5, 0xf3, 0x73, 0xa7, 0x57, 0xaf, 0x58, 0xa8,
	0x50, 0xf4, 0xea, 0xd6, 0x74, 0x4f, 0xae, 0xe9, 0xd5, 0xe7, 0xe6, 0xad, 0xe8,
	0x2c, 0xd7, 0x75, 0x7a, 0xeb, 0x16, 0x0b, 0xf5, 0x59, 0xcb, 0x5f, 0xb0, 0x9c,
	0xa9, 0x51, 0xa0, 0x7f, 0x0c, 0xf6, 0x6f, 0x17, 0xc4, 0x49, 0xec, 0xd8, 0x43,
	0x1f, 0x2d, 0xa4, 0x76, 0x7b, 0xb7, 0xcc, 0xbb, 0x3e, 0x5a, 0xfb, 0x60, 0xb1,
	0x86, 0x3b, 0x52, 0xa1, 0x6c, 0xaa, 0x55, 0x29, 0x9d, 0x97, 0xb2, 0x87, 0x90,
	0x61, 0xbe, 0xdc, 0xfc, 0xbc, 0x95, 0xcf, 0xcd, 0x37, 0x3f, 0x5b, 0xd1, 0x53,
	0x39, 0x84, 0x3c, 0x41, 0xa2, 0x6d, 0x47, 0x14, 0x2a, 0x9e, 0x5d, 0x56, 0xf2,
	0xd3, 0xab, 0x44, 0x11, 0x92, 0xd9, 0x23, 0x20, 0x2e, 0x89, 0xb4, 0x7c, 0xb8,
	0x26, 0x77, 0x99, 0xe3, 0xa5, 0x67, 0x4a, 0xed, 0xde, 0xc5, 0x31, 0xfe, 0x18,
	0x0d, 0x63, 0x8c, 0x80, 0xc0, 0xf7, 0x70, 0x07
};
/* The 'zero mask' table to deal with specific case of a zero value during
 * the GF(256) multiplication */
__attribute__((weak)) EMBEDDED_SRAM const uint8_t zero_mask[] = {
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
static inline uint8_t gf256_mult_ref(uint8_t x, uint8_t y) {
	uint8_t log_x = rijndael_log[x];
	uint8_t log_y = rijndael_log[y];
	uint8_t res = rijndael_exp[log_x + log_y] & zero_mask[x] & zero_mask[y];
	return res;
}
#else
/* 32-bit based multiplication */
#define GF256_MODULUS_X4 0x1b1b1b1bu
static inline uint32_t gf256_lane_xtime_x4(uint32_t v) {
	uint32_t msb = v & 0x80808080u;
	uint32_t shifted = (v & 0x7f7f7f7fu) << 1;
	uint32_t red = (msb - (msb >> 7)) & GF256_MODULUS_X4;
	return shifted ^ red;
}
static inline uint32_t gf256_mult4_ref(uint32_t x, uint32_t y) {
	uint32_t acc = 0, a = x;

#define STEP(i) do { \
    uint32_t m = ( (y >> (i)) & 0x01010101u ); \
    m |= (m << 1) & 0xFEFEFEFEu;               \
    m |= (m << 2) & 0xFCFCFCFCu;               \
    m |= (m << 4) & 0xF0F0F0F0u;               \
    acc ^= (a & m); \
    a = gf256_lane_xtime_x4(a); \
} while (0)

	STEP(0);
	STEP(1);
	STEP(2);
	STEP(3);
	STEP(4);
	STEP(5);
	STEP(6);
	STEP(7);

#undef STEP
	return acc;
}

static inline uint8_t gf256_mult_ref(uint8_t x, uint8_t y) {
	/* XXX: NOTE: we perform the GF(256) multiplication on 32 bits as it is more optimal on most platforms */
	return (uint8_t)gf256_mult4_ref((uint32_t)x, (uint32_t)y);
}

#if !defined(NO_FIELDS_REF_SWAR_OPT)
/* "Vectorized" SWAR (SIMD Within A Register) */
#define GF256_MULT_X4
#endif /* NO_FIELDS_REF_SWAR_OPT */
#endif

#if defined(GF256_MULT_X4)
/* ==== SWAR helpers shared by the GF(16) "hybrid" routines ====
 *
 * All of them work on four GF(256) elements packed in a 32 bits word, one per
 * byte. They are branchless and table-free, so unlike their scalar
 * counterparts they need no 'volatile' barrier: there is no way for a compiler
 * to turn a lane-wise mask built out of shifts, ANDs and a subtraction into a
 * data dependent branch or a memory lookup.
 *
 * These helpers assume that byte lane k of a word loaded with memcpy() is
 * memory byte k, i.e. a little endian layout. On big endian targets (SPARC64
 * and friends) the callers fall back to the scalar loops, which are already
 * validated there; this keeps the lane bookkeeping trivial and costs strictly
 * nothing on the platforms MQOM3 actually targets, since the predicate below
 * is constant folded at compile time. */
static inline int gf_swar_lanes_are_le(void) {
	const uint16_t val = 0x0102;
	const uint8_t *buf = (const uint8_t *)(&val);

	return (buf[0] == 0x02);
}

/* Spread the four GF(16) nibbles packed in the low 16 bits of 'nibs'
 * (nibble j in bits 4j..4j+3) over the four bytes of a SWAR word. */
static inline uint32_t gf16_spread_x4(uint32_t nibs) {
	nibs = (nibs | (nibs << 8)) & 0x00FF00FFu;
	nibs = (nibs | (nibs << 4)) & 0x0F0F0F0Fu;

	return nibs;
}

/* Lift four GF(16) elements (one per byte of 'w', in the low nibble) to their
 * GF(256) representatives. This is the very same linear map as the scalar
 * gf16_gf256_mult_ref()/gf256_vect_lift_from_gf16_ref(), applied to the four
 * lanes at once. */
static inline uint32_t gf16_lift_x4(uint32_t w) {
	uint32_t m1 = gf256_lane_bcast_x4((w >> 1) & 0x01010101u);
	uint32_t m2 = gf256_lane_bcast_x4((w >> 2) & 0x01010101u);
	uint32_t m3 = gf256_lane_bcast_x4((w >> 3) & 0x01010101u);

	return (w & 0x01010101u) ^ (m1 & 0xE0E0E0E0u) ^ (m2 & 0x5D5D5D5Du) ^ (m3 & 0xB0B0B0B0u);
}

/* Load four consecutive GF(16) elements starting at the even index 'i' of a
 * packed vector and return them lifted to GF(256), one per byte lane. */
static inline uint32_t gf16_load_lift_x4(const uint8_t *a_gf16, uint32_t i) {
	uint32_t nibs = (uint32_t)a_gf16[i / 2] | ((uint32_t)a_gf16[(i / 2) + 1] << 8);

	return gf16_lift_x4(gf16_spread_x4(nibs));
}

/* Duplicate each of the two low bytes of 'w' inside its own 16 bits lane, i.e.
 * turn [s0, s1, *, *] into [s0, s0, s1, s1]. Used to multiply two GF(256^2)
 * elements (two bytes each) by their respective GF(256) scalar. */
static inline uint32_t gf256_dup_lo_x2(uint32_t w) {
	w &= 0x0000FFFFu;
	w = (w | (w << 8)) & 0x00FF00FFu;

	return w | (w << 8);
}

/* Broadcast a single GF(256) element over the four lanes of a SWAR word.
 * Done with shifts rather than a multiplication by 0x01010101: 's' is secret
 * here, and a few cores (early-terminating multipliers) take a number of
 * cycles that depends on the operand. Compilers usually turn the multiply
 * into shifts anyway, so this costs nothing while making the constant-time
 * property hold in C rather than relying on the back-end. */
static inline uint32_t gf256_splat_x4(uint8_t s) {
	uint32_t v = (uint32_t)s;

	v |= (v << 8);
	v |= (v << 16);
	return v;
}
#endif /* GF256_MULT_X4 */

/*
 * Vector multiplied by a constant in GF(256).
 */
static inline void gf256_constant_vect_mult_ref(uint8_t b, const uint8_t *a, uint8_t *c, uint32_t len) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
	uint32_t b_ = (uint32_t)b | ((uint32_t)b << 8) | ((uint32_t)b << 16) | ((uint32_t)b << 24);
	while (i < (4 * (len / 4))) {
		uint32_t a_;
		memcpy(&a_, &a[i], 4);
		uint32_t c_ = gf256_mult4_ref(a_, b_);
		memcpy(&c[i], &c_, 4);
		i += 4;
	}
#endif
	while (i < len) {
		c[i] = gf256_mult_ref(a[i], b);
		i++;
	}

	return;
}

/*
 * Vector to vector multiplication in GF(256).
 * Takes two vectors of length 'len', and returns a byte (element in GF(256))
 */
static inline uint8_t gf256_vect_mult_ref(const uint8_t *a, const uint8_t *b, uint32_t len) {

	uint32_t i;
	uint32_t res32;

	res32 = 0;
	i = 0;
#if defined(GF256_MULT_X4)
	while (i < (4 * (len / 4))) {
		uint32_t a_, b_;
		memcpy(&a_, &a[i], 4);
		memcpy(&b_, &b[i], 4);
		res32 ^= gf256_mult4_ref(a_, b_);
		i += 4;
	}
#endif
	while (i < len) {
		res32 ^= gf256_mult_ref(a[i], b[i]);
		i++;
	}

#if defined(GF256_MULT_X4)
	res32 = (res32 & 0xff) ^ ((res32 >> 8) & 0xff) ^ ((res32 >> 16) & 0xff) ^ (res32 >> 24);
#endif

	return (uint8_t)res32;
}

#if defined(GF256_MULT_X4)
static inline void gf256_mult4_public_precomputation_ref(uint32_t y_pows[8],uint32_t y) {
	y_pows[0] = y;
	y_pows[1] = gf256_lane_xtime_x4(y_pows[0]);
	y_pows[2] = gf256_lane_xtime_x4(y_pows[1]);
	y_pows[3] = gf256_lane_xtime_x4(y_pows[2]);
	y_pows[4] = gf256_lane_xtime_x4(y_pows[3]);
	y_pows[5] = gf256_lane_xtime_x4(y_pows[4]);
	y_pows[6] = gf256_lane_xtime_x4(y_pows[5]);
	y_pows[7] = gf256_lane_xtime_x4(y_pows[6]);
	return;
}
#endif

static inline uint32_t gf256_mult4_public_preprocessed_ref(uint32_t x, const uint32_t y_pows[8]) {
	uint32_t acc = 0;

/* gf256_lane_bcast_x4 rather than a multiplication by 0xFF: 'x' is secret,
 * and a multiply is not constant time on every core (see the helper). */
#define STEP(i) do { \
    uint32_t m = gf256_lane_bcast_x4( (x >> (i)) & 0x01010101u ); \
    acc ^= (y_pows[i] & m); \
} while (0)

	STEP(0);
	STEP(1);
	STEP(2);
	STEP(3);
	STEP(4);
	STEP(5);
	STEP(6);
	STEP(7);

#undef STEP
	return acc;
}

#if defined(GF256_MULT_X4)
/* Number of elements of the fixed vector whose xtime ladders are kept
 * precomputed at once during a matrix by vector or batched vector product.
 * Every ladder is built exactly once per product whatever this value is, so the
 * tile only bounds the stack footprint.
 * The tiles are sized so that the precomputed ladders stay around half a
 * kilobyte of stack: 64 elements (16 blocks x 32 bytes = 512 bytes) for
 * GF(256), 32 elements (8 blocks x 3 ladders x 32 bytes = 768 bytes) for
 * GF(256^2). Both can be overridden at compile time to trade a bit of speed
 * for stack on very constrained targets. */
#ifndef GF256_MAT_MULT_TILE
#define GF256_MAT_MULT_TILE 64
#endif
#ifndef GF256TO2_MAT_MULT_TILE
#define GF256TO2_MAT_MULT_TILE 32
#endif

/* An override must stay a multiple of 4: the tile is consumed in 4-element
 * blocks (NB_BLOCKS below truncates otherwise), and the tile loops advance by
 * TILE while the precomputed ladders are indexed from a 4-aligned column, so a
 * non-multiple desynchronizes the ladder from the row it multiplies. The
 * product would come out wrong on every call, silently. */
#if (GF256_MAT_MULT_TILE % 4) != 0
#error "GF256_MAT_MULT_TILE must be a multiple of 4"
#endif
#if (GF256TO2_MAT_MULT_TILE % 4) != 0
#error "GF256TO2_MAT_MULT_TILE must be a multiple of 4"
#endif
/* A zero tile would make the tiling loops spin forever. */
#if GF256_MAT_MULT_TILE < 4
#error "GF256_MAT_MULT_TILE must be at least 4"
#endif
#if GF256TO2_MAT_MULT_TILE < 4
#error "GF256TO2_MAT_MULT_TILE must be at least 4"
#endif

#define GF256_MAT_MULT_NB_BLOCKS (GF256_MAT_MULT_TILE / 4)
#define GF256TO2_MAT_MULT_NB_BLOCKS (GF256TO2_MAT_MULT_TILE / 4)

/* Column range [lo, hi) of row 'i' for each matrix shape. */
static inline uint32_t gf_mat_row_lo(uint32_t i, uint32_t n, matrix_type mtype) {
	(void)n;
	return (mtype == TRI_SUP) ? i : 0;
}
static inline uint32_t gf_mat_row_hi(uint32_t i, uint32_t n, matrix_type mtype) {
	return (mtype == TRI_INF) ? (i + 1) : n;
}

/* Same circuit as gf256_mult4_public_preprocessed_ref(), but reading the ladder
 * from a "doubling major" table pows[k][b] passed as its row 'b' plus the row
 * stride: consecutive blocks 'b' of the tile are then contiguous in memory for
 * a fixed doubling 'k', which is what lets an auto-vectorizer widen the loop
 * over blocks. */
static inline uint32_t gf256_mult4_tiled_ladder_ref(uint32_t x, const uint32_t *pows, uint32_t stride) {
	uint32_t acc = 0;

/* gf256_lane_bcast_x4 rather than a multiplication by 0xFF: 'x' is secret,
 * and a multiply is not constant time on every core (see the helper). */
#define STEP(k) do { \
    uint32_t m = gf256_lane_bcast_x4( (x >> (k)) & 0x01010101u ); \
    acc ^= (pows[(k) * (stride)] & m); \
} while (0)

	STEP(0);
	STEP(1);
	STEP(2);
	STEP(3);
	STEP(4);
	STEP(5);
	STEP(6);
	STEP(7);

#undef STEP
	return acc;
}
#endif /* GF256_MULT_X4 */

/*
 * Vector to vector batched multiplication in GF(256).
 * Takes n+1 vectors a, b[0], ..., b[n-1] of length 'len'
 * Returns n bytes c[0], ..., c[n-1] (elements in GF(256))
 *   where c[i] is the vector to vector multiplication between a and b[i]
 * Assume that a is a non-sensitive vector
 */
static inline void gf256_vect_mult_multiple_public_ref(uint8_t* const* c, const uint8_t *a, const uint8_t* const* b, uint32_t len, uint32_t n) {
	uint32_t i, e;

	for(e = 0; e < n; e++) {
		*c[e] = 0;
	}

#if defined(GF256_MULT_X4)
	{
		/* Same tiling as gf256_mat_mult_ref(): the xtime ladder of a tile of
		 * 'a' is built once and reused by the n vectors b[e], and the fold down
		 * to a single byte plus the store through c[e] only happen once per
		 * tile instead of once per block. */
		uint32_t pows[8][GF256_MAT_MULT_NB_BLOCKS];
		uint32_t t, b_idx, full = (4 * (len / 4));
		for (t = 0; t < full; t += GF256_MAT_MULT_TILE) {
			uint32_t tile_hi = t + GF256_MAT_MULT_TILE;
			uint32_t nb_blocks;
			if (tile_hi > full) {
				tile_hi = full;
			}
			nb_blocks = (tile_hi - t) / 4;
			for (b_idx = 0; b_idx < nb_blocks; b_idx++) {
				uint32_t ladder[8];
				uint32_t k, a_;
				memcpy(&a_, &a[t + (4 * b_idx)], 4);
				gf256_mult4_public_precomputation_ref(ladder, a_);
				for (k = 0; k < 8; k++) {
					pows[k][b_idx] = ladder[k];
				}
			}
			for (e = 0; e < n; e++) {
				const uint8_t *brow = &b[e][t];
				uint32_t acc = 0;
				for (b_idx = 0; b_idx < nb_blocks; b_idx++) {
					uint32_t b_;
					memcpy(&b_, &brow[4 * b_idx], 4);
					acc ^= gf256_mult4_tiled_ladder_ref(b_, &pows[0][b_idx], GF256_MAT_MULT_NB_BLOCKS);
				}
				*c[e] ^= gf256_lane_fold_x4(acc);
			}
		}
		i = full;
	}
#else
	i = 0;
#endif
	while (i < len) {
		for (e = 0; e < n; e++) {
			*c[e] ^= gf256_mult_ref(a[i], b[e][i]);
		}
		i++;
	}
}

/* Matrix and vector multiplication over GF(256)
 * C += A * X, where X is a vector
 * Matrix is supposed to be square n x n, and vector n x 1
 * The output is a vector n x 1
 * */
static inline void gf256_mat_mult_ref(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
#if defined(GF256_MULT_X4)
	/* In a matrix by vector product the vector is shared by every row, so its
	 * eight lane-wise doublings are computed once and reused instead of being
	 * rebuilt for each coefficient: this removes the whole xtime chain from the
	 * inner loop, i.e. roughly half of its operations. The vector is walked by
	 * tiles so that the precomputed ladders always fit a small stack buffer.
	 * NOTE: the two "public" ladder helpers are plain shift/mask/XOR circuits,
	 * without any table nor branch, so this stays perfectly constant time even
	 * though X is a secret vector here. */
	uint32_t i, t, b;
	uint32_t pows[8][GF256_MAT_MULT_NB_BLOCKS];

	if ((mtype != REG) && (mtype != TRI_SUP) && (mtype != TRI_INF)) {
		return;
	}
	for (i = 0; i < n; i++) {
		Y[i] = 0;
	}
	for (t = 0; t < n; t += GF256_MAT_MULT_TILE) {
		uint32_t tile_hi = t + GF256_MAT_MULT_TILE;
		uint32_t nb_blocks;
		if (tile_hi > n) {
			tile_hi = n;
		}
		nb_blocks = (tile_hi - t) / 4;
		for (b = 0; b < nb_blocks; b++) {
			uint32_t ladder[8];
			uint32_t k, x_;
			memcpy(&x_, &X[t + (4 * b)], 4);
			gf256_mult4_public_precomputation_ref(ladder, x_);
			for (k = 0; k < 8; k++) {
				pows[k][b] = ladder[k];
			}
		}
		for (i = 0; i < n; i++) {
			uint32_t lo = gf_mat_row_lo(i, n, mtype);
			uint32_t hi = gf_mat_row_hi(i, n, mtype);
			uint32_t c, blk_lo, blk_hi;
			uint32_t acc = 0;
			if (lo < t) {
				lo = t;
			}
			if (hi > tile_hi) {
				hi = tile_hi;
			}
			if (lo >= hi) {
				continue;
			}
			blk_lo = ((lo + 3) & ~((uint32_t)3));
			blk_hi = (hi & ~((uint32_t)3));
			if (blk_lo >= blk_hi) {
				/* Too short for a single aligned block: plain scalar loop. */
				for (c = lo; c < hi; c++) {
					Y[i] ^= gf256_mult_ref(A[(n * i) + c], X[c]);
				}
				continue;
			}
			for (c = lo; c < blk_lo; c++) {
				Y[i] ^= gf256_mult_ref(A[(n * i) + c], X[c]);
			}
			{
				const uint8_t *arow = &A[(n * i) + blk_lo];
				uint32_t bb, b_lo = ((blk_lo - t) / 4), b_hi = ((blk_hi - t) / 4);
				for (bb = b_lo; bb < b_hi; bb++) {
					uint32_t a_;
					memcpy(&a_, &arow[4 * (bb - b_lo)], 4);
					acc ^= gf256_mult4_tiled_ladder_ref(a_, &pows[0][bb], GF256_MAT_MULT_NB_BLOCKS);
				}
			}
			for (c = blk_hi; c < hi; c++) {
				Y[i] ^= gf256_mult_ref(A[(n * i) + c], X[c]);
			}
			Y[i] ^= gf256_lane_fold_x4(acc);
		}
	}
#else
	GF256_MAT_MULT(A, X, Y, n, mtype, gf256_vect_mult_ref);
#endif
}

/*
 * "Hybrid" multiplication of a constant in GF(2) and a vector in GF(256)
 */
static inline void gf2_gf256_constant_vect_mult_ref(uint8_t a_gf2, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;

	for (i = 0; i < n; i++) {
		c_gf256[i] = b_gf256[i] & -(a_gf2 & mask);
	}

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(2)
 */
static inline void gf256_gf2_constant_vect_mult_ref(uint8_t a_gf256, const uint8_t *b_gf2, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;
	for (i = 0; i < n; i++) {
		c_gf256[i] = a_gf256 & -((b_gf2[i / 8] >> (i % 8)) & mask);
	}
	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(2) and a vector in GF(256)
 */
static inline uint8_t gf2_gf256_vect_mult_ref(const uint8_t *a_gf2, const uint8_t *b_gf256, uint32_t n) {
	uint32_t i;
	uint8_t res;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;
	res = 0;
	for (i = 0; i < n; i++) {
		res ^= b_gf256[i] & -((a_gf2[i / 8] >> (i % 8)) & mask);
	}
	return res;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(2)
 */
static inline uint8_t gf256_gf2_vect_mult_ref(const uint8_t *a_gf256, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256_vect_mult_ref(b_gf2, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(2) and a vector in GF(256), resulting
 *  in a vector in GF(256)
 */
static inline void gf2_gf256_mat_mult_ref(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF2_GF256_MAT_MULT(A, X, Y, n, mtype, gf2_gf256_vect_mult_ref);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(2), resulting
 *  in a vector in GF(256)
 */
static inline void gf256_gf2_mat_mult_ref(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF256_GF2_MAT_MULT(A, X, Y, n, mtype, gf256_gf2_vect_mult_ref);
}

/* GF(256) matrix transposition */
static inline void gf256_mat_transpose_ref(const uint8_t *A, uint8_t *B, uint32_t n, matrix_type mtype) {
	GF256_MAT_TRANSPOSE(A, B, n, mtype);
}

/*
 * Lift of an element in GF(16) to its representative in GF(256)
 */
static inline uint8_t gf16_lift_ref(uint8_t a_gf16) {
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;

	uint16_t a_gf256 = (a_gf16 & mask) ^ (-((a_gf16 >> 1) & mask) & 0xE0) ^ (-((a_gf16 >> 2) & mask) & 0x5D) ^ (-(a_gf16 >> 3) & 0xB0);
	return (uint8_t)a_gf256;
}

/*
 * "Hybrid" multiplication of an element in GF(16) and an element in GF(256)
 */
static inline uint8_t gf16_gf256_mult_ref(uint8_t a_gf16, const uint8_t b_gf256) {
	return gf256_mult_ref(gf16_lift_ref(a_gf16), b_gf256);
}

static inline void gf256_vect_lift_from_gf16_ref(const uint8_t *a_gf16, uint8_t *b_gf256, uint32_t n) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
	if (gf_swar_lanes_are_le()) {
		while (i < (4 * (n / 4))) {
			uint32_t l_ = gf16_load_lift_x4(a_gf16, i);
			memcpy(&b_gf256[i], &l_, 4);
			i += 4;
		}
	}
#endif
	while (i < n) {
		uint8_t x_gf16 = (a_gf16[i / 2] >> (4 * (i % 2))) & 0x0F;
		b_gf256[i] = gf16_lift_ref(x_gf16);
		i++;
	}

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(16) and a vector in GF(256)
 */
static inline void gf16_gf256_constant_vect_mult_ref(uint8_t a_gf16, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
	{
		/* The constant is lifted once and its xtime ladder is precomputed, so
		 * that each block of four elements only costs eight mask/AND/XOR
		 * triplets instead of a full generic SWAR multiplication. Every lane
		 * carries the same scalar, so this loop is insensitive to the byte
		 * ordering of the platform.
		 * NOTE: despite their name, the two "public" helpers below are plain
		 * shift/mask/XOR circuits without any table nor branch, hence perfectly
		 * constant time even when the precomputed operand is secret. */
		uint32_t a_pows[8];
		gf256_mult4_public_precomputation_ref(a_pows, gf256_splat_x4(gf16_lift_ref(a_gf16)));
		while (i < (4 * (n / 4))) {
			uint32_t b_, c_;
			memcpy(&b_, &b_gf256[i], 4);
			c_ = gf256_mult4_public_preprocessed_ref(b_, a_pows);
			memcpy(&c_gf256[i], &c_, 4);
			i += 4;
		}
	}
#endif
	while (i < n) {
		c_gf256[i] = gf16_gf256_mult_ref(a_gf16, b_gf256[i]);
		i++;
	}

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(16)
 */
static inline void gf256_gf16_constant_vect_mult_ref(uint8_t a_gf256, const uint8_t *b_gf16, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
	if (gf_swar_lanes_are_le()) {
		uint32_t a_pows[8];
		gf256_mult4_public_precomputation_ref(a_pows, gf256_splat_x4(a_gf256));
		while (i < (4 * (n / 4))) {
			uint32_t c_ = gf256_mult4_public_preprocessed_ref(gf16_load_lift_x4(b_gf16, i), a_pows);
			memcpy(&c_gf256[i], &c_, 4);
			i += 4;
		}
	}
#endif
	while (i < n) {
		c_gf256[i] = gf16_gf256_mult_ref((b_gf16[i / 2] >> (4 * (i % 2))) & 0x0F, a_gf256);
		i++;
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(16) and a vector in GF(256)
 */
static inline uint8_t gf16_gf256_vect_mult_ref(const uint8_t *a_gf16, const uint8_t *b_gf256, uint32_t n) {
	uint32_t i;
	uint8_t res;

	res = 0;
	i = 0;
#if defined(GF256_MULT_X4)
	if (gf_swar_lanes_are_le()) {
		/* Four GF(16) elements are lifted at once and multiplied with the four
		 * matching GF(256) elements by a single SWAR multiplication: the
		 * lifting is amortized over the whole block and the accumulator is
		 * only folded down to a single byte at the very end. */
		uint32_t acc32 = 0;
		while (i < (4 * (n / 4))) {
			uint32_t b_;
			memcpy(&b_, &b_gf256[i], 4);
			acc32 ^= gf256_mult4_ref(gf16_load_lift_x4(a_gf16, i), b_);
			i += 4;
		}
		res = gf256_lane_fold_x4(acc32);
	}
#endif
	while (i < n) {
		/* Conditionally add the element from GF(256) in a constant time fashion */
		res ^= gf16_gf256_mult_ref((a_gf16[i / 2] >> (4 * (i % 2))) & 0x0F, b_gf256[i]);
		i++;
	}

	return res;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(16)
 */
static inline uint8_t gf256_gf16_vect_mult_ref(const uint8_t *a_gf256, const uint8_t *b_gf16, uint32_t n) {
	return gf16_gf256_vect_mult_ref(b_gf16, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(16), resulting
 *  in a vector in GF(256)
 */
static inline void gf256_gf16_mat_mult_ref(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF256_GF16_MAT_MULT(A, X, Y, n, mtype, gf256_gf16_vect_mult_ref);
}

/* === GF(256^2) === */
/*
 * For GF(2^16) elements, we use a GF(256^2) representation as two elements (a1, a0), with
 * a0 and a1 in GF(256) and a1 * X + a0 representing the element as a polynomial. The reduction
 * polynomial with coefficients in GF(256) is X^2 + X + 32, as it is irreductible in this field.
 *
 */
static inline uint16_t gf256to2_mult_ref(uint16_t a, uint16_t b) {
	uint8_t c0, c1, a0, a1, b0, b1, a1b1, a0b0;

	a0 = a & 0xff;
	a1 = a >> 8;
	b0 = b & 0xff;
	b1 = b >> 8;
	/**/
	a1b1 = gf256_mult_ref(a1, b1);
	a0b0 = gf256_mult_ref(a0, b0);
	/**/
	c0 = a0b0 ^ gf256_mult_ref(a1b1, 32);
	c1 = a0b0 ^ gf256_mult_ref(a0 ^ a1, b0 ^ b1);

	return (((uint16_t)c1 << 8) ^ c0);
}

#if defined(GF256_MULT_X4)
static inline uint64_t gf256to2_mult4_ref(uint64_t a, uint64_t b) {
	uint32_t c0, c1, a0, a1, b0, b1, a1b1, a0b0;
	uint64_t res;

	a0 = (a & 0xff) | ((a & 0xff0000ull) >> 8) | ((a & 0xff00000000ull) >> 16) | ((a & 0xff000000000000ull) >> 24);
	a1 = ((a & 0xff00ull) >> 8) | ((a & 0xff000000ull) >> 16) | ((a & 0xff0000000000ull) >> 24) | ((a & 0xff00000000000000ull) >> 32);
	b0 = (b & 0xff) | ((b & 0xff0000ull) >> 8) | ((b & 0xff00000000ull) >> 16) | ((b & 0xff000000000000ull) >> 24);
	b1 = ((b & 0xff00ull) >> 8) | ((b & 0xff000000ull) >> 16) | ((b & 0xff0000000000ull) >> 24) | ((b & 0xff00000000000000ull) >> 32);
	/**/
	a1b1 = gf256_mult4_ref(a1, b1);
	a0b0 = gf256_mult4_ref(a0, b0);
	/**/
	c0 = a0b0 ^ gf256_mult4_ref(a1b1, 0x20202020);
	c1 = a0b0 ^ gf256_mult4_ref(a0 ^ a1, b0 ^ b1);

	res  = ((uint64_t)c0 & 0xff) | ((uint64_t)(c0 & 0xff00) << 8) | ((uint64_t)(c0 & 0xff0000) << 16) | ((uint64_t)(c0 & 0xff000000) << 24);
	res |= (((uint64_t)c1 & 0xff) << 8) | ((uint64_t)(c1 & 0xff00) << 16) | ((uint64_t)(c1 & 0xff0000) << 24) | ((uint64_t)(c1 & 0xff000000) << 32);
	return res;
}

/* Split four packed GF(256^2) elements into the SWAR word of their low halves
 * and the SWAR word of their high halves. Both operands of a product go through
 * the very same split, so on a big endian platform the four lanes are simply
 * visited in reverse order, which is harmless for a lane-wise product and for
 * the XOR folding that ends a dot product. */
static inline void gf256to2_split_x4(uint64_t a, uint32_t *a0, uint32_t *a1) {
	*a0 = (uint32_t)((a & 0xff) | ((a & 0xff0000ull) >> 8) | ((a & 0xff00000000ull) >> 16) | ((a & 0xff000000000000ull) >> 24));
	*a1 = (uint32_t)(((a & 0xff00ull) >> 8) | ((a & 0xff000000ull) >> 16) | ((a & 0xff0000000000ull) >> 24) | ((a & 0xff00000000000000ull) >> 32));

	return;
}

/* Recombine the three GF(256) partial sums of a GF(256^2) dot product. The
 * multiplication by 32 of the (a1.b1) term and the Karatsuba recombination are
 * linear, so they are applied once on the folded accumulators instead of once
 * per block inside the accumulation loop. */
static inline uint16_t gf256to2_recombine_x4(uint32_t acc00, uint32_t acc11, uint32_t accss) {
	uint8_t p0 = gf256_lane_fold_x4(acc00);
	uint8_t p1 = gf256_lane_fold_x4(acc11);
	uint8_t ps = gf256_lane_fold_x4(accss);
	uint8_t c0 = p0 ^ gf256_mult_ref(p1, 32);
	uint8_t c1 = p0 ^ ps;

	return (uint16_t)(((uint16_t)c1 << 8) ^ c0);
}
#endif

#if defined(GF256_MULT_X4)
static inline void gf256to2_mult4_public_precomputation_ref(uint32_t a_pows[24], uint64_t a) {
	uint32_t a0, a1;
	a0 = (a & 0xff) | ((a & 0xff0000ull) >> 8) | ((a & 0xff00000000ull) >> 16) | ((a & 0xff000000000000ull) >> 24);
	a1 = ((a & 0xff00ull) >> 8) | ((a & 0xff000000ull) >> 16) | ((a & 0xff0000000000ull) >> 24) | ((a & 0xff00000000000000ull) >> 32);

	gf256_mult4_public_precomputation_ref(&a_pows[0], a0);
	gf256_mult4_public_precomputation_ref(&a_pows[8], a1);
	gf256_mult4_public_precomputation_ref(&a_pows[16], a0 ^ a1);
	return;
}
#endif

#if defined(GF256_MULT_X4)
static inline uint64_t gf256to2_mult4_public_preprocessed_ref(uint64_t b, const uint32_t a_pows[24]) {
	uint32_t c0, c1, b0, b1, a1b1, a0b0;
	uint64_t res;

	b0 = (b & 0xff) | ((b & 0xff0000ull) >> 8) | ((b & 0xff00000000ull) >> 16) | ((b & 0xff000000000000ull) >> 24);
	b1 = ((b & 0xff00ull) >> 8) | ((b & 0xff000000ull) >> 16) | ((b & 0xff0000000000ull) >> 24) | ((b & 0xff00000000000000ull) >> 32);
	/**/
	a1b1 = gf256_mult4_public_preprocessed_ref(b1, &a_pows[8]);
	a0b0 = gf256_mult4_public_preprocessed_ref(b0, &a_pows[0]);
	/**/
	c0 = a0b0 ^ gf256_mult4_ref(a1b1, 0x20202020);
	c1 = a0b0 ^ gf256_mult4_public_preprocessed_ref(b0 ^ b1, &a_pows[16]);

	res  = ((uint64_t)c0 & 0xff) | ((uint64_t)(c0 & 0xff00) << 8) | ((uint64_t)(c0 & 0xff0000) << 16) | ((uint64_t)(c0 & 0xff000000) << 24);
	res |= (((uint64_t)c1 & 0xff) << 8) | ((uint64_t)(c1 & 0xff00) << 16) | ((uint64_t)(c1 & 0xff0000) << 24) | ((uint64_t)(c1 & 0xff000000) << 32);
	return res;
}
#endif

/*
 * Vector multiplied by a constant in GF(256^2).
 */
static inline void gf256to2_constant_vect_mult_ref(uint16_t b, const uint16_t *a, uint16_t *c, uint32_t len) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
	/* NOTE: the generic SWAR multiplication is kept here on purpose. Feeding it
	 * with the loop invariant constant lets the compiler hoist the eight lane
	 * masks of each of the three Karatsuba products out of the loop, which
	 * turns out to beat the precomputed xtime ladder used elsewhere (measured
	 * both with and without auto-vectorization). */
	uint64_t b_ = ((uint64_t)b) | ((uint64_t)b << 16) | ((uint64_t)b << 32) | ((uint64_t)b << 48);
	while (i < (4 * (len / 4))) {
		uint64_t a_;
		memcpy(&a_, &a[i], 8);
		uint64_t c_ = gf256to2_mult4_ref(a_, b_);
		memcpy(&c[i], &c_, 8);
		i += 4;
	}
#endif
	while (i < len) {
		c[i] = gf256to2_mult_ref(a[i], b);
		i++;
	}

	return;
}

/*
 * GF(2^16) scalar multiplication
 */
static inline uint16_t gf256to2_vect_mult_ref(const uint16_t *a, const uint16_t *b, uint32_t len) {
	uint32_t i;
	uint16_t res;

	i = 0;
#if defined(GF256_MULT_X4)
	{
		/* The three Karatsuba partial products are accumulated separately: the
		 * multiplication by 32 and the final recombination are linear, so they
		 * are pulled out of the loop and applied only once. */
		uint32_t acc00 = 0, acc11 = 0, accss = 0;
		while (i < (4 * (len / 4))) {
			uint64_t a_, b_;
			uint32_t a0, a1, b0, b1;
			memcpy(&a_, &a[i], 8);
			memcpy(&b_, &b[i], 8);
			gf256to2_split_x4(a_, &a0, &a1);
			gf256to2_split_x4(b_, &b0, &b1);
			acc00 ^= gf256_mult4_ref(a0, b0);
			acc11 ^= gf256_mult4_ref(a1, b1);
			accss ^= gf256_mult4_ref(a0 ^ a1, b0 ^ b1);
			i += 4;
		}
		res = gf256to2_recombine_x4(acc00, acc11, accss);
	}
#else
	res = 0;
#endif
	while (i < len) {
		res ^= gf256to2_mult_ref(a[i], b[i]);
		i++;
	}

	return res;
}

/*
 * Vector to vector batched multiplication in GF(256^2).
 * Takes n+1 vectors a, b[0], ..., b[n-1] of length 'len'
 * Returns n double-bytes c[0], ..., c[n-1] (elements in GF(256^2))
 *   where c[i] is the vector to vector multiplication between a and b[i]
 * Assume that a is a non-sensitive vector
 */
static inline void gf256to2_vect_mult_multiple_public_ref(uint16_t* const* c, const uint16_t *a, const uint16_t* const* b, uint32_t len, uint32_t n) {
	uint32_t i, e;

	for(e = 0; e < n; e++) {
		*c[e] = 0;
	}

#if defined(GF256_MULT_X4)
	{
		/* Same tiling as gf256to2_mat_mult_ref(): the three xtime ladders of a
		 * tile of 'a' are built once and reused by the n vectors b[e]. Keeping
		 * the three Karatsuba partial sums separate along a tile also pulls the
		 * multiplication by 32, the byte re-interleaving and the store through
		 * c[e] out of the innermost loop. */
		uint32_t pows0[8][GF256TO2_MAT_MULT_NB_BLOCKS];
		uint32_t pows1[8][GF256TO2_MAT_MULT_NB_BLOCKS];
		uint32_t powss[8][GF256TO2_MAT_MULT_NB_BLOCKS];
		uint32_t t, b_idx, full = (4 * (len / 4));
		for (t = 0; t < full; t += GF256TO2_MAT_MULT_TILE) {
			uint32_t tile_hi = t + GF256TO2_MAT_MULT_TILE;
			uint32_t nb_blocks;
			if (tile_hi > full) {
				tile_hi = full;
			}
			nb_blocks = (tile_hi - t) / 4;
			for (b_idx = 0; b_idx < nb_blocks; b_idx++) {
				uint32_t ladder[24];
				uint32_t k;
				uint64_t a_;
				memcpy(&a_, &a[t + (4 * b_idx)], 8);
				gf256to2_mult4_public_precomputation_ref(ladder, a_);
				for (k = 0; k < 8; k++) {
					pows0[k][b_idx] = ladder[k];
					pows1[k][b_idx] = ladder[8 + k];
					powss[k][b_idx] = ladder[16 + k];
				}
			}
			for (e = 0; e < n; e++) {
				const uint16_t *brow = &b[e][t];
				uint32_t acc00 = 0, acc11 = 0, accss = 0;
				for (b_idx = 0; b_idx < nb_blocks; b_idx++) {
					uint64_t b_;
					uint32_t b0, b1;
					memcpy(&b_, &brow[4 * b_idx], 8);
					gf256to2_split_x4(b_, &b0, &b1);
					acc00 ^= gf256_mult4_tiled_ladder_ref(b0, &pows0[0][b_idx], GF256TO2_MAT_MULT_NB_BLOCKS);
					acc11 ^= gf256_mult4_tiled_ladder_ref(b1, &pows1[0][b_idx], GF256TO2_MAT_MULT_NB_BLOCKS);
					accss ^= gf256_mult4_tiled_ladder_ref(b0 ^ b1, &powss[0][b_idx], GF256TO2_MAT_MULT_NB_BLOCKS);
				}
				*c[e] ^= gf256to2_recombine_x4(acc00, acc11, accss);
			}
		}
		i = full;
	}
#else
	i = 0;
#endif
	while (i < len) {
		for (e = 0; e < n; e++) {
			*c[e] ^= gf256to2_mult_ref(a[i], b[e][i]);
		}
		i++;
	}
}

/*
 * GF(2^16) matrix multiplication
 */
static inline void gf256to2_mat_mult_ref(const uint16_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
#if defined(GF256_MULT_X4)
	/* Same idea as gf256_mat_mult_ref(): the vector is shared by every row, so
	 * its three xtime ladders (X0, X1 and X0 ^ X1) are built once per tile and
	 * reused by all the rows, which removes the xtime chains from the inner
	 * loop. The three Karatsuba partial sums are accumulated separately and
	 * recombined once per row and per tile, so that the multiplication by 32
	 * and the byte re-interleaving also leave the inner loop. */
	uint32_t i, t, b;
	uint32_t pows0[8][GF256TO2_MAT_MULT_NB_BLOCKS];
	uint32_t pows1[8][GF256TO2_MAT_MULT_NB_BLOCKS];
	uint32_t powss[8][GF256TO2_MAT_MULT_NB_BLOCKS];

	if ((mtype != REG) && (mtype != TRI_SUP) && (mtype != TRI_INF)) {
		return;
	}
	for (i = 0; i < n; i++) {
		Y[i] = 0;
	}
	for (t = 0; t < n; t += GF256TO2_MAT_MULT_TILE) {
		uint32_t tile_hi = t + GF256TO2_MAT_MULT_TILE;
		uint32_t nb_blocks;
		if (tile_hi > n) {
			tile_hi = n;
		}
		nb_blocks = (tile_hi - t) / 4;
		for (b = 0; b < nb_blocks; b++) {
			uint32_t ladder[24];
			uint32_t k;
			uint64_t x_;
			memcpy(&x_, &X[t + (4 * b)], 8);
			gf256to2_mult4_public_precomputation_ref(ladder, x_);
			for (k = 0; k < 8; k++) {
				pows0[k][b] = ladder[k];
				pows1[k][b] = ladder[8 + k];
				powss[k][b] = ladder[16 + k];
			}
		}
		for (i = 0; i < n; i++) {
			uint32_t lo = gf_mat_row_lo(i, n, mtype);
			uint32_t hi = gf_mat_row_hi(i, n, mtype);
			uint32_t c, blk_lo, blk_hi;
			uint32_t acc00 = 0, acc11 = 0, accss = 0;
			if (lo < t) {
				lo = t;
			}
			if (hi > tile_hi) {
				hi = tile_hi;
			}
			if (lo >= hi) {
				continue;
			}
			blk_lo = ((lo + 3) & ~((uint32_t)3));
			blk_hi = (hi & ~((uint32_t)3));
			if (blk_lo >= blk_hi) {
				/* Too short for a single aligned block: plain scalar loop. */
				for (c = lo; c < hi; c++) {
					Y[i] ^= gf256to2_mult_ref(A[(n * i) + c], X[c]);
				}
				continue;
			}
			for (c = lo; c < blk_lo; c++) {
				Y[i] ^= gf256to2_mult_ref(A[(n * i) + c], X[c]);
			}
			{
				const uint16_t *arow = &A[(n * i) + blk_lo];
				uint32_t bb, b_lo = ((blk_lo - t) / 4), b_hi = ((blk_hi - t) / 4);
				for (bb = b_lo; bb < b_hi; bb++) {
					uint64_t a_;
					uint32_t a0, a1;
					memcpy(&a_, &arow[4 * (bb - b_lo)], 8);
					gf256to2_split_x4(a_, &a0, &a1);
					acc00 ^= gf256_mult4_tiled_ladder_ref(a0, &pows0[0][bb], GF256TO2_MAT_MULT_NB_BLOCKS);
					acc11 ^= gf256_mult4_tiled_ladder_ref(a1, &pows1[0][bb], GF256TO2_MAT_MULT_NB_BLOCKS);
					accss ^= gf256_mult4_tiled_ladder_ref(a0 ^ a1, &powss[0][bb], GF256TO2_MAT_MULT_NB_BLOCKS);
				}
			}
			for (c = blk_hi; c < hi; c++) {
				Y[i] ^= gf256to2_mult_ref(A[(n * i) + c], X[c]);
			}
			Y[i] ^= gf256to2_recombine_x4(acc00, acc11, accss);
		}
	}
#else
	GF256to2_MAT_MULT(A, X, Y, n, mtype, gf256to2_vect_mult_ref);
#endif
}

/*
 * "Hybrid" constant multiplication of a constant in GF(2) and a vector in GF(256^2)
 */
static inline void gf2_gf256to2_constant_vect_mult_ref(uint8_t a_gf2, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;

	for (i = 0; i < n; i++) {
		c_gf256to2[i] = b_gf256to2[i] & -(uint16_t)(a_gf2 & mask);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(2)
 */
static inline void gf256to2_gf2_constant_vect_mult_ref(uint16_t a_gf256to2, const uint8_t *b_gf2, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;
	for (i = 0; i < n; i++) {
		c_gf256to2[i] = a_gf256to2 & -(uint16_t)((b_gf2[i / 8] >> (i % 8)) & mask);
	}
	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(2) and a vector in GF(256^2)
 */
static inline uint16_t gf2_gf256to2_vect_mult_ref(const uint8_t *a_gf2, const uint16_t *b_gf256to2, uint32_t n) {
	uint32_t i;
	uint16_t res;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;
	res = 0;
	for (i = 0; i < n; i++) {
		res ^= b_gf256to2[i] & -(uint16_t)((a_gf2[i / 8] >> (i % 8)) & mask);
	}
	return res;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(2)
 */
static inline uint16_t gf256to2_gf2_vect_mult_ref(const uint16_t *a_gf256to2, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256to2_vect_mult_ref(b_gf2, a_gf256to2, n);
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256) and a vector in GF(256^2)
 * Used as helper for gf16_gf256to2_constant_vect_mult in AVX implementations.
 */
static inline void gf256_gf256to2_constant_vect_mult_ref(uint8_t a_gf256, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256to2[i] = (gf256_mult_ref(a_gf256, b_gf256to2[i] >> 8) << 8) | gf256_mult_ref(a_gf256, b_gf256to2[i] & 0xff);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(256)
 * Used as helper for gf256to2_gf16_constant_vect_mult in AVX implementations.
 */
static inline void gf256to2_gf256_constant_vect_mult_ref(uint16_t a_gf256to2, const uint8_t *b_gf256, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256to2[i] = (gf256_mult_ref(b_gf256[i], a_gf256to2 >> 8) << 8) | gf256_mult_ref(b_gf256[i], a_gf256to2 & 0xff);
	}

	return;
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(2) and a vector in GF(256^2), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf2_gf256to2_mat_mult_ref(const uint8_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF2_GF256to2_MAT_MULT(A, X, Y, n, mtype, gf2_gf256to2_vect_mult_ref);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(2), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256to2_gf2_mat_mult_ref(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_GF2_MAT_MULT(A, X, Y, n, mtype, gf256to2_gf2_vect_mult_ref);
}

/* GF(256^2) matrix transposition */
static inline void gf256to2_mat_transpose_ref(const uint16_t *A, uint16_t *B, uint32_t n, matrix_type mtype) {
	GF256to2_MAT_TRANSPOSE(A, B, n, mtype);
}

/*
 * "Hybrid" multiplication of an element in GF(16) and an element in GF(256^2)
 */

static inline uint16_t gf16_gf256to2_mult_ref(uint8_t a_gf16, const uint16_t b_gf256to2) {
	uint16_t res = (((uint16_t)gf16_gf256_mult_ref(a_gf16, b_gf256to2 >> 8)) << 8) | gf16_gf256_mult_ref(a_gf16, b_gf256to2 & 0xFF);
	return res;
}

static inline void gf256to2_vect_lift_from_gf16_ref(const uint8_t *a_gf16, uint16_t *b_gf256to2, uint32_t n) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
	if (gf_swar_lanes_are_le()) {
		while (i < (4 * (n / 4))) {
			/* Lift four elements at once, then interleave them with the zero
			 * high halves of the GF(256^2) representation. */
			uint32_t l_ = gf16_load_lift_x4(a_gf16, i);
			uint32_t lo_ = (l_ & 0x000000FFu) | ((l_ & 0x0000FF00u) << 8);
			uint32_t hi_ = ((l_ >> 16) & 0x000000FFu) | ((l_ & 0xFF000000u) >> 8);
			memcpy(&b_gf256to2[i], &lo_, 4);
			memcpy(&b_gf256to2[i + 2], &hi_, 4);
			i += 4;
		}
	}
#endif
	while (i < n) {
		uint8_t x_gf16 = (a_gf16[i / 2] >> (4 * (i % 2))) & 0x0F;
		b_gf256to2[i] = gf16_lift_ref(x_gf16);
		i++;
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(16) and a vector in GF(256^2)
 */
static inline void gf16_gf256to2_constant_vect_mult_ref(uint8_t a_gf16, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
	{
		/* A GF(16) (hence GF(256)) scalar multiplies both halves of a GF(256^2)
		 * element the same way, so the whole vector is just a byte string
		 * scaled by a single GF(256) constant: splat the lifted constant over
		 * the four lanes and precompute its xtime ladder once. Because every
		 * lane carries the same scalar, this loop is insensitive to the byte
		 * ordering of the platform.
		 * NOTE: despite their name, the two "public" helpers below are plain
		 * shift/mask/XOR circuits without any table nor branch, hence perfectly
		 * constant time even when the precomputed operand is secret. */
		uint32_t a_pows[8];
		gf256_mult4_public_precomputation_ref(a_pows, gf256_splat_x4(gf16_lift_ref(a_gf16)));
		while (i < (2 * (n / 2))) {
			uint32_t b_, c_;
			memcpy(&b_, &b_gf256to2[i], 4);
			c_ = gf256_mult4_public_preprocessed_ref(b_, a_pows);
			memcpy(&c_gf256to2[i], &c_, 4);
			i += 2;
		}
	}
#endif
	while (i < n) {
		c_gf256to2[i] = gf16_gf256to2_mult_ref(a_gf16, b_gf256to2[i]);
		i++;
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(16)
 */
static inline void gf256to2_gf16_constant_vect_mult_ref(uint16_t a_gf256to2, const uint8_t *b_gf16, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
	if (gf_swar_lanes_are_le()) {
		/* Here the constant is the GF(256^2) element: replicate its (a0, a1)
		 * byte pair over the two 16 bits lanes of a SWAR word, precompute the
		 * xtime ladder once, and feed it with the lifted GF(16) elements
		 * duplicated inside their own lane. */
		uint32_t a_pows[8];
		uint32_t a_ = ((uint32_t)a_gf256to2 & 0xFFFFu);
		a_ |= (a_ << 16);
		gf256_mult4_public_precomputation_ref(a_pows, a_);
		while (i < (4 * (n / 4))) {
			uint32_t l_ = gf16_load_lift_x4(b_gf16, i);
			uint32_t c_;
			c_ = gf256_mult4_public_preprocessed_ref(gf256_dup_lo_x2(l_), a_pows);
			memcpy(&c_gf256to2[i], &c_, 4);
			c_ = gf256_mult4_public_preprocessed_ref(gf256_dup_lo_x2(l_ >> 16), a_pows);
			memcpy(&c_gf256to2[i + 2], &c_, 4);
			i += 4;
		}
	}
#endif
	while (i < n) {
		c_gf256to2[i] = gf16_gf256to2_mult_ref((b_gf16[i / 2] >> (4 * (i % 2))) & 0x0F, a_gf256to2);
		i++;
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(16) and a vector in GF(256^2)
 */
static inline uint16_t gf16_gf256to2_vect_mult_ref(const uint8_t *a_gf16, const uint16_t *b_gf256to2, uint32_t n) {
	uint32_t i;
	uint16_t res;

	res = 0;
	i = 0;
#if defined(GF256_MULT_X4)
	if (gf_swar_lanes_are_le()) {
		/* Four GF(16) elements are lifted at once, each one duplicated over the
		 * two bytes of its GF(256^2) partner, so that eight GF(256)
		 * multiplications are carried out by two SWAR multiplications. The
		 * accumulator keeps the low and high halves in the even and odd lanes
		 * and is only folded down once, at the end of the dot product. */
		uint32_t acc32 = 0;
		while (i < (4 * (n / 4))) {
			uint32_t l_ = gf16_load_lift_x4(a_gf16, i);
			uint32_t b_;
			memcpy(&b_, &b_gf256to2[i], 4);
			acc32 ^= gf256_mult4_ref(gf256_dup_lo_x2(l_), b_);
			memcpy(&b_, &b_gf256to2[i + 2], 4);
			acc32 ^= gf256_mult4_ref(gf256_dup_lo_x2(l_ >> 16), b_);
			i += 4;
		}
		res = (uint16_t)(acc32 ^ (acc32 >> 16));
	}
#endif
	while (i < n) {
		res ^= gf16_gf256to2_mult_ref((a_gf16[i / 2] >> (4 * (i % 2))) & 0x0F, b_gf256to2[i]);
		i++;
	}

	return res;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(16)
 */
static inline uint16_t gf256to2_gf16_vect_mult_ref(const uint16_t *a_gf256to2, const uint8_t *b_gf16, uint32_t n) {
	return gf16_gf256to2_vect_mult_ref(b_gf16, a_gf256to2, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(16), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256to2_gf16_mat_mult_ref(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_GF16_MAT_MULT(A, X, Y, n, mtype, gf256to2_gf16_vect_mult_ref);
}

#endif /* __FIELDS_REF_H__ */
