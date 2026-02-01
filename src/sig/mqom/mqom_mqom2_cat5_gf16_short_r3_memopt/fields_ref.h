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

/* Matrix and vector multiplication over GF(256)
 * C += A * X, where X is a vector
 * Matrix is supposed to be square n x n, and vector n x 1
 * The output is a vector n x 1
 * */
static inline void gf256_mat_mult_ref(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF256_MAT_MULT(A, X, Y, n, mtype, gf256_vect_mult_ref);
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
		/* Conditionally add the element from GF(256) in a constant time fashion */
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
 * "Hybrid" multiplication of an element in GF(4) and an element in GF(256)
 */
// 0xBC = 0b10111100
static inline uint8_t gf256_mult_0xBC_ref(const uint8_t x) {
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t res;

	res = x;
	res =     (-(res >> 7) & GF256_MODULUS) ^ (res << 1);
	res = x ^ (-(res >> 7) & GF256_MODULUS) ^ (res << 1);
	res = x ^ (-(res >> 7) & GF256_MODULUS) ^ (res << 1);
	res = x ^ (-(res >> 7) & GF256_MODULUS) ^ (res << 1);
	res = x ^ (-(res >> 7) & GF256_MODULUS) ^ (res << 1);
	res =     (-(res >> 7) & GF256_MODULUS) ^ (res << 1);
	res =     (-(res >> 7) & GF256_MODULUS) ^ (res << 1);

	return res;
}
static inline uint8_t gf4_gf256_mult_ref(uint8_t a_gf4, const uint8_t b_gf256) {
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;

	uint8_t x = gf256_mult_0xBC_ref(b_gf256);
	uint8_t res = (-(a_gf4 >> 1) & x) ^ (-(a_gf4 & mask) & b_gf256);
	return res;
}

/*
 * "Hybrid" multiplication of a constant in GF(4) and a vector in GF(256)
 */
static inline void gf4_gf256_constant_vect_mult_ref(uint8_t a_gf4, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256[i] = gf4_gf256_mult_ref(a_gf4, b_gf256[i]);
	}

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(4)
 */
static inline void gf256_gf4_constant_vect_mult_ref(uint8_t a_gf256, const uint8_t *b_gf4, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256[i] = gf4_gf256_mult_ref((b_gf4[i / 4] >> (2 * (i % 4))) & 0x03, a_gf256);
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(4) and a vector in GF(256)
 */
static inline uint8_t gf4_gf256_vect_mult_ref(const uint8_t *a_gf4, const uint8_t *b_gf256, uint32_t n) {
	uint32_t i;
	uint8_t res;

	res = 0;
	for (i = 0; i < n; i++) {
		/* Conditionally add the element from GF(256) in a constant time fashion */
		res ^= gf4_gf256_mult_ref((a_gf4[i / 4] >> (2 * (i % 4))) & 0x03, b_gf256[i]);
	}

	return res;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(4)
 */
static inline uint8_t gf256_gf4_vect_mult_ref(const uint8_t *a_gf256, const uint8_t *b_gf4, uint32_t n) {
	return gf4_gf256_vect_mult_ref(b_gf4, a_gf256, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(4), resulting
 *  in a vector in GF(256)
 */
static inline void gf256_gf4_mat_mult_ref(const uint8_t *A, const uint8_t *X, uint8_t *Y, uint32_t n, matrix_type mtype) {
	GF256_GF4_MAT_MULT(A, X, Y, n, mtype, gf256_gf4_vect_mult_ref);
}

/*
 * "Hybrid" multiplication of an element in GF(16) and an element in GF(256)
 */
static inline uint8_t gf16_gf256_mult_ref(uint8_t a_gf16, const uint8_t b_gf256) {
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;

	uint16_t a_gf256 = (a_gf16 & mask) ^ (-((a_gf16 >> 1) & mask) & 0xE0) ^ (-((a_gf16 >> 2) & mask) & 0x5D) ^ (-(a_gf16 >> 3) & 0xB0);
	return gf256_mult_ref(a_gf256, b_gf256);
}

static inline void gf256_vect_lift_from_gf16_ref(const uint8_t *a_gf16, uint8_t *b_gf256, uint32_t n) {
	uint32_t i;
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;

	for (i = 0; i < n; i++) {
		uint8_t x_gf16 = (a_gf16[i / 2] >> (4 * (i % 2))) & 0x0F;
		b_gf256[i] = (x_gf16 & mask) ^ (-((x_gf16 >> 1) & mask) & 0xE0) ^ (-((x_gf16 >> 2) & mask) & 0x5D) ^ (-(x_gf16 >> 3) & 0xB0);
	}

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(16) and a vector in GF(256)
 */
static inline void gf16_gf256_constant_vect_mult_ref(uint8_t a_gf16, const uint8_t *b_gf256, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256[i] = gf16_gf256_mult_ref(a_gf16, b_gf256[i]);
	}

	return;
}

/*
 * "Hybrid" multiplication of a constant in GF(256) and a vector in GF(16)
 */
static inline void gf256_gf16_constant_vect_mult_ref(uint8_t a_gf256, const uint8_t *b_gf16, uint8_t *c_gf256, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256[i] = gf16_gf256_mult_ref((b_gf16[i / 2] >> (4 * (i % 2))) & 0x0F, a_gf256);
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
	for (i = 0; i < n; i++) {
		/* Conditionally add the element from GF(256) in a constant time fashion */
		res ^= gf16_gf256_mult_ref((a_gf16[i / 2] >> (4 * (i % 2))) & 0x0F, b_gf256[i]);
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
#endif

/*
 * Vector multiplied by a constant in GF(256^2).
 */
static inline void gf256to2_constant_vect_mult_ref(uint16_t b, const uint16_t *a, uint16_t *c, uint32_t len) {
	uint32_t i;

	i = 0;
#if defined(GF256_MULT_X4)
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
	uint64_t res64;

	res64 = 0;
	i = 0;
#if defined(GF256_MULT_X4)
	while (i < (4 * (len / 4))) {
		uint64_t a_, b_;
		memcpy(&a_, &a[i], 8);
		memcpy(&b_, &b[i], 8);
		res64 ^= gf256to2_mult4_ref(a_, b_);
		i += 4;
	}
#endif
	while (i < len) {
		res64 ^= gf256to2_mult_ref(a[i], b[i]);
		i++;
	}

#if defined(GF256_MULT_X4)
	res64 = (res64 & 0xffff) ^ ((res64 >> 16) & 0xffff) ^ ((res64 >> 32) & 0xffff) ^ ((res64 >> 48) & 0xffff);
#endif

	return (uint16_t)res64;
}

/*
 * GF(2^16) matrix multiplication
 */
static inline void gf256to2_mat_mult_ref(const uint16_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_MAT_MULT(A, X, Y, n, mtype, gf256to2_vect_mult_ref);
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
 * "Hybrid" constant multiplication of a constant in GF(256) and a vector in GF(256^2)
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
 */
static inline void gf256to2_gf256_constant_vect_mult_ref(uint16_t a_gf256to2, const uint8_t *b_gf256, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256to2[i] = (gf256_mult_ref(b_gf256[i], a_gf256to2 >> 8) << 8) | gf256_mult_ref(b_gf256[i], a_gf256to2 & 0xff);
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(2)
 */
static inline uint16_t gf256to2_gf2_vect_mult_ref(const uint16_t *a_gf256to2, const uint8_t *b_gf2, uint32_t n) {
	return gf2_gf256to2_vect_mult_ref(b_gf2, a_gf256to2, n);
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256) and a vector in GF(256^2)
 */
static inline uint16_t gf256_gf256to2_vect_mult_ref(const uint8_t *a_gf256, const uint16_t *b_gf256to2, uint32_t n) {
	uint32_t i;
	uint16_t res;

	res = 0;
	for (i = 0; i < n; i++) {
		/* Note: the multiplication of an element in GF(256) and an element in GF(256^2)
		 * simply consists in two multiplications in GF(256) (this is multiplying a constant by a degree 1 polynomial)
		 * */
		res ^= (gf256_mult_ref(a_gf256[i], b_gf256to2[i] >> 8) << 8) | gf256_mult_ref(a_gf256[i], b_gf256to2[i] & 0xff);
	}

	return res;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(256)
 */
static inline uint16_t gf256to2_gf256_vect_mult_ref(const uint16_t *a_gf256to2, const uint8_t *b_gf256, uint32_t n) {
	return gf256_gf256to2_vect_mult_ref(b_gf256, a_gf256to2, n);
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

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256) and a vector in GF(256^2), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256_gf256to2_mat_mult_ref(const uint8_t *A, const uint16_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_MAT_MULT(A, X, Y, n, mtype, gf256_gf256to2_vect_mult_ref);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(256), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256to2_gf256_mat_mult_ref(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_MAT_MULT(A, X, Y, n, mtype, gf256to2_gf256_vect_mult_ref);
}

/* GF(256^2) matrix transposition */
static inline void gf256to2_mat_transpose_ref(const uint16_t *A, uint16_t *B, uint32_t n, matrix_type mtype) {
	GF256to2_MAT_TRANSPOSE(A, B, n, mtype);
}

/*
 * "Hybrid" multiplication of an element in GF(4) and an element in GF(256^2)
 */
static inline uint16_t gf4_gf256to2_mult_ref(uint8_t a_gf4, const uint16_t b_gf256to2) {
	uint16_t res = (((uint16_t)gf4_gf256_mult_ref(a_gf4, b_gf256to2 >> 8)) << 8) | gf4_gf256_mult_ref(a_gf4, b_gf256to2 & 0xFF);
	return res;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(4) and a vector in GF(256^2)
 */
static inline void gf4_gf256to2_constant_vect_mult_ref(uint8_t a_gf4, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256to2[i] = gf4_gf256to2_mult_ref(a_gf4, b_gf256to2[i]);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(4)
 */
static inline void gf256to2_gf4_constant_vect_mult_ref(uint16_t a_gf256to2, const uint8_t *b_gf4, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256to2[i] = gf4_gf256to2_mult_ref((b_gf4[i / 4] >> (2 * (i % 4))) & 0x03, a_gf256to2);
	}

	return;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(4) and a vector in GF(256^2)
 */
static inline uint16_t gf4_gf256to2_vect_mult_ref(const uint8_t *a_gf4, const uint16_t *b_gf256to2, uint32_t n) {
	uint32_t i;
	uint16_t res;

	res = 0;
	for (i = 0; i < n; i++) {
		res ^= gf4_gf256to2_mult_ref((a_gf4[i / 4] >> (2 * (i % 4))) & 0x03, b_gf256to2[i]);
	}

	return res;
}

/*
 * "Hybrid" scalar multiplication of a vector in GF(256^2) and a vector in GF(4)
 */
static inline uint16_t gf256to2_gf4_vect_mult_ref(const uint16_t *a_gf256to2, const uint8_t *b_gf4, uint32_t n) {
	return gf4_gf256to2_vect_mult_ref(b_gf4, a_gf256to2, n);
}

/*
 * "Hybrid" matrix multiplication of a matrix in GF(256^2) and a vector in GF(4), resulting
 *  in a vector in GF(256^2)
 */
static inline void gf256to2_gf4_mat_mult_ref(const uint16_t *A, const uint8_t *X, uint16_t *Y, uint32_t n, matrix_type mtype) {
	GF256to2_GF4_MAT_MULT(A, X, Y, n, mtype, gf256to2_gf4_vect_mult_ref);
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
	/* XXX: NOTE: the 'volatile' keyword is here to avoid compiler
	 * optimizations that can lead to non-constant time operations.
	 * See https://blog.cr.yp.to/20240803-clang.html for more details on this */
	volatile uint8_t mask = 1;

	for (i = 0; i < n; i++) {
		uint8_t x_gf16 = (a_gf16[i / 2] >> (4 * (i % 2))) & 0x0F;
		b_gf256to2[i] = (x_gf16 & mask) ^ (-((x_gf16 >> 1) & mask) & 0xE0) ^ (-((x_gf16 >> 2) & mask) & 0x5D) ^ (-(x_gf16 >> 3) & 0xB0);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(16) and a vector in GF(256^2)
 */
static inline void gf16_gf256to2_constant_vect_mult_ref(uint8_t a_gf16, const uint16_t *b_gf256to2, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256to2[i] = gf16_gf256to2_mult_ref(a_gf16, b_gf256to2[i]);
	}

	return;
}

/*
 * "Hybrid" constant multiplication of a constant in GF(256^2) and a vector in GF(16)
 */
static inline void gf256to2_gf16_constant_vect_mult_ref(uint16_t a_gf256to2, const uint8_t *b_gf16, uint16_t *c_gf256to2, uint32_t n) {
	uint32_t i;

	for (i = 0; i < n; i++) {
		c_gf256to2[i] = gf16_gf256to2_mult_ref((b_gf16[i / 2] >> (4 * (i % 2))) & 0x0F, a_gf256to2);
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
	for (i = 0; i < n; i++) {
		res ^= gf16_gf256to2_mult_ref((a_gf16[i / 2] >> (4 * (i % 2))) & 0x0F, b_gf256to2[i]);
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
