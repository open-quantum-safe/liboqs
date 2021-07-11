// SPDX-License-Identifier: Public domain

#include <oqs/oqs.h>

#include "sha2_local.h"

#include <stdio.h>

#if defined(OQS_DIST_ARM64v8_BUILD) || defined(OQS_USE_ARM_SHA2_INSTRUCTIONS)
#include <stdint.h>
#include <arm_acle.h>
#include <arm_neon.h>
#endif
/*
#define PQC_SHA256CTX_BYTES 40
#define PQC_SHA512CTX_BYTES 72
*/




/*
typedef struct {
	uint8_t *ctx;
} sha224ctx;
#define sha224_inc_init oqs_sha2_sha224_inc_init
#define sha224_inc_ctx_clone oqs_sha2_sha224_inc_ctx_clone
#define sha224_inc_ctx_release oqs_sha2_sha224_inc_ctx_release
#define sha224_inc_blocks oqs_sha2_sha224_inc_blocks
#define sha224_inc_finalize oqs_sha2_sha224_inc_finalize
#define sha224 oqs_sha2_sha224

typedef struct {
	uint8_t *ctx;
} sha256ctx;
#define sha256_inc_init oqs_sha2_sha256_inc_init_c
#define sha256_inc_ctx_clone oqs_sha2_sha256_inc_ctx_clone_c
#define sha256_inc_ctx_release oqs_sha2_sha256_inc_ctx_release_c
#define sha256_inc_blocks oqs_sha2_sha256_inc_blocks_c
#define sha256_inc_finalize oqs_sha2_sha256_inc_finalize_c
#define sha256 OQS_SHA2_sha256

typedef struct {
	uint8_t *ctx;
} sha384ctx;

#define sha384_inc_init oqs_sha2_sha384_inc_init_c
#define sha384_inc_ctx_clone oqs_sha2_sha384_inc_ctx_clone_c
#define sha384_inc_ctx_release oqs_sha2_sha384_inc_ctx_release_c
#define sha384_inc_blocks oqs_sha2_sha384_inc_blocks_c
#define sha384_inc_finalize oqs_sha2_sha384_inc_finalize_c
#define sha384 OQS_SHA2_sha384

typedef struct {
	uint8_t *ctx;
} sha512ctx;

#define sha512_inc_init oqs_sha2_sha512_inc_init_c
#define sha512_inc_ctx_clone oqs_sha2_sha512_inc_ctx_clone_c
#define sha512_inc_ctx_release oqs_sha2_sha512_inc_ctx_release_c
#define sha512_inc_blocks oqs_sha2_sha512_inc_blocks_c
#define sha512_inc_finalize oqs_sha2_sha512_inc_finalize_c
#define sha512 OQS_SHA2_sha512
*/
/* Based on the public domain implementation in
 * crypto_hash/sha512/ref/ from http://bench.cr.yp.to/supercop.html
 * by D. J. Bernstein */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
static uint32_t load_bigendian_32(const uint8_t *x) {
	return (uint32_t)(x[3]) | (((uint32_t)(x[2])) << 8) |
	       (((uint32_t)(x[1])) << 16) | (((uint32_t)(x[0])) << 24);
}
*/
static uint64_t load_bigendian_64(const uint8_t *x) {
	return (uint64_t)(x[7]) | (((uint64_t)(x[6])) << 8) |
	       (((uint64_t)(x[5])) << 16) | (((uint64_t)(x[4])) << 24) |
	       (((uint64_t)(x[3])) << 32) | (((uint64_t)(x[2])) << 40) |
	       (((uint64_t)(x[1])) << 48) | (((uint64_t)(x[0])) << 56);
}
/*
static void store_bigendian_32(uint8_t *x, uint64_t u) {
	x[3] = (uint8_t) u;
	u >>= 8;
	x[2] = (uint8_t) u;
	u >>= 8;
	x[1] = (uint8_t) u;
	u >>= 8;
	x[0] = (uint8_t) u;
}
*/
static void store_bigendian_64(uint8_t *x, uint64_t u) {
	x[7] = (uint8_t) u;
	u >>= 8;
	x[6] = (uint8_t) u;
	u >>= 8;
	x[5] = (uint8_t) u;
	u >>= 8;
	x[4] = (uint8_t) u;
	u >>= 8;
	x[3] = (uint8_t) u;
	u >>= 8;
	x[2] = (uint8_t) u;
	u >>= 8;
	x[1] = (uint8_t) u;
	u >>= 8;
	x[0] = (uint8_t) u;
}
/*
#define SHR(x, c) ((x) >> (c))
#define ROTR_32(x, c) (((x) >> (c)) | ((x) << (32 - (c))))
#define ROTR_64(x, c) (((x) >> (c)) | ((x) << (64 - (c))))

#define Ch(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define Sigma0_32(x) (ROTR_32(x, 2) ^ ROTR_32(x,13) ^ ROTR_32(x,22))
#define Sigma1_32(x) (ROTR_32(x, 6) ^ ROTR_32(x,11) ^ ROTR_32(x,25))
#define sigma0_32(x) (ROTR_32(x, 7) ^ ROTR_32(x,18) ^ SHR(x, 3))
#define sigma1_32(x) (ROTR_32(x,17) ^ ROTR_32(x,19) ^ SHR(x,10))

#define Sigma0_64(x) (ROTR_64(x, 28) ^ ROTR_64(x, 34) ^ ROTR_64(x, 39))
#define Sigma1_64(x) (ROTR_64(x, 14) ^ ROTR_64(x, 18) ^ ROTR_64(x, 41))
#define sigma0_64(x) (ROTR_64(x, 1) ^ ROTR_64(x, 8) ^ SHR(x, 7))
#define sigma1_64(x) (ROTR_64(x, 19) ^ ROTR_64(x, 61) ^ SHR(x, 6))

#define M_32(w0, w14, w9, w1) w0 = sigma1_32(w14) + (w9) + sigma0_32(w1) + (w0);
#define M_64(w0, w14, w9, w1) w0 = sigma1_64(w14) + (w9) + sigma0_64(w1) + (w0);

#define EXPAND_32           \
    M_32(w0, w14, w9, w1)   \
    M_32(w1, w15, w10, w2)  \
    M_32(w2, w0, w11, w3)   \
    M_32(w3, w1, w12, w4)   \
    M_32(w4, w2, w13, w5)   \
    M_32(w5, w3, w14, w6)   \
    M_32(w6, w4, w15, w7)   \
    M_32(w7, w5, w0, w8)    \
    M_32(w8, w6, w1, w9)    \
    M_32(w9, w7, w2, w10)   \
    M_32(w10, w8, w3, w11)  \
    M_32(w11, w9, w4, w12)  \
    M_32(w12, w10, w5, w13) \
    M_32(w13, w11, w6, w14) \
    M_32(w14, w12, w7, w15) \
    M_32(w15, w13, w8, w0)

#define EXPAND_64           \
    M_64(w0, w14, w9, w1)   \
    M_64(w1, w15, w10, w2)  \
    M_64(w2, w0, w11, w3)   \
    M_64(w3, w1, w12, w4)   \
    M_64(w4, w2, w13, w5)   \
    M_64(w5, w3, w14, w6)   \
    M_64(w6, w4, w15, w7)   \
    M_64(w7, w5, w0, w8)    \
    M_64(w8, w6, w1, w9)    \
    M_64(w9, w7, w2, w10)   \
    M_64(w10, w8, w3, w11)  \
    M_64(w11, w9, w4, w12)  \
    M_64(w12, w10, w5, w13) \
    M_64(w13, w11, w6, w14) \
    M_64(w14, w12, w7, w15) \
    M_64(w15, w13, w8, w0)

#define F_32(w, k)                                   \
    T1 = h + Sigma1_32(e) + Ch(e, f, g) + (k) + (w); \
    T2 = Sigma0_32(a) + Maj(a, b, c);                \
    h = g;                                           \
    g = f;                                           \
    f = e;                                           \
    e = d + T1;                                      \
    d = c;                                           \
    c = b;                                           \
    b = a;                                           \
    a = T1 + T2;

#define F_64(w, k)                                   \
    T1 = h + Sigma1_64(e) + Ch(e, f, g) + (k) + (w); \
    T2 = Sigma0_64(a) + Maj(a, b, c);                \
    h = g;                                           \
    g = f;                                           \
    f = e;                                           \
    e = d + T1;                                      \
    d = c;                                           \
    c = b;                                           \
    b = a;                                           \
    a = T1 + T2;

static size_t crypto_hashblocks_sha256(uint8_t *statebytes,
                                       const uint8_t *in, size_t inlen) {
	uint32_t state[8];
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	uint32_t e;
	uint32_t f;
	uint32_t g;
	uint32_t h;
	uint32_t T1;
	uint32_t T2;

	a = load_bigendian_32(statebytes + 0);
	state[0] = a;
	b = load_bigendian_32(statebytes + 4);
	state[1] = b;
	c = load_bigendian_32(statebytes + 8);
	state[2] = c;
	d = load_bigendian_32(statebytes + 12);
	state[3] = d;
	e = load_bigendian_32(statebytes + 16);
	state[4] = e;
	f = load_bigendian_32(statebytes + 20);
	state[5] = f;
	g = load_bigendian_32(statebytes + 24);
	state[6] = g;
	h = load_bigendian_32(statebytes + 28);
	state[7] = h;

	while (inlen >= 64) {
		uint32_t w0  = load_bigendian_32(in + 0);
		uint32_t w1  = load_bigendian_32(in + 4);
		uint32_t w2  = load_bigendian_32(in + 8);
		uint32_t w3  = load_bigendian_32(in + 12);
		uint32_t w4  = load_bigendian_32(in + 16);
		uint32_t w5  = load_bigendian_32(in + 20);
		uint32_t w6  = load_bigendian_32(in + 24);
		uint32_t w7  = load_bigendian_32(in + 28);
		uint32_t w8  = load_bigendian_32(in + 32);
		uint32_t w9  = load_bigendian_32(in + 36);
		uint32_t w10 = load_bigendian_32(in + 40);
		uint32_t w11 = load_bigendian_32(in + 44);
		uint32_t w12 = load_bigendian_32(in + 48);
		uint32_t w13 = load_bigendian_32(in + 52);
		uint32_t w14 = load_bigendian_32(in + 56);
		uint32_t w15 = load_bigendian_32(in + 60);

		F_32(w0, 0x428a2f98)
		F_32(w1, 0x71374491)
		F_32(w2, 0xb5c0fbcf)
		F_32(w3, 0xe9b5dba5)
		F_32(w4, 0x3956c25b)
		F_32(w5, 0x59f111f1)
		F_32(w6, 0x923f82a4)
		F_32(w7, 0xab1c5ed5)
		F_32(w8, 0xd807aa98)
		F_32(w9, 0x12835b01)
		F_32(w10, 0x243185be)
		F_32(w11, 0x550c7dc3)
		F_32(w12, 0x72be5d74)
		F_32(w13, 0x80deb1fe)
		F_32(w14, 0x9bdc06a7)
		F_32(w15, 0xc19bf174)

		EXPAND_32

		F_32(w0, 0xe49b69c1)
		F_32(w1, 0xefbe4786)
		F_32(w2, 0x0fc19dc6)
		F_32(w3, 0x240ca1cc)
		F_32(w4, 0x2de92c6f)
		F_32(w5, 0x4a7484aa)
		F_32(w6, 0x5cb0a9dc)
		F_32(w7, 0x76f988da)
		F_32(w8, 0x983e5152)
		F_32(w9, 0xa831c66d)
		F_32(w10, 0xb00327c8)
		F_32(w11, 0xbf597fc7)
		F_32(w12, 0xc6e00bf3)
		F_32(w13, 0xd5a79147)
		F_32(w14, 0x06ca6351)
		F_32(w15, 0x14292967)

		EXPAND_32

		F_32(w0, 0x27b70a85)
		F_32(w1, 0x2e1b2138)
		F_32(w2, 0x4d2c6dfc)
		F_32(w3, 0x53380d13)
		F_32(w4, 0x650a7354)
		F_32(w5, 0x766a0abb)
		F_32(w6, 0x81c2c92e)
		F_32(w7, 0x92722c85)
		F_32(w8, 0xa2bfe8a1)
		F_32(w9, 0xa81a664b)
		F_32(w10, 0xc24b8b70)
		F_32(w11, 0xc76c51a3)
		F_32(w12, 0xd192e819)
		F_32(w13, 0xd6990624)
		F_32(w14, 0xf40e3585)
		F_32(w15, 0x106aa070)

		EXPAND_32

		F_32(w0, 0x19a4c116)
		F_32(w1, 0x1e376c08)
		F_32(w2, 0x2748774c)
		F_32(w3, 0x34b0bcb5)
		F_32(w4, 0x391c0cb3)
		F_32(w5, 0x4ed8aa4a)
		F_32(w6, 0x5b9cca4f)
		F_32(w7, 0x682e6ff3)
		F_32(w8, 0x748f82ee)
		F_32(w9, 0x78a5636f)
		F_32(w10, 0x84c87814)
		F_32(w11, 0x8cc70208)
		F_32(w12, 0x90befffa)
		F_32(w13, 0xa4506ceb)
		F_32(w14, 0xbef9a3f7)
		F_32(w15, 0xc67178f2)

		a += state[0];
		b += state[1];
		c += state[2];
		d += state[3];
		e += state[4];
		f += state[5];
		g += state[6];
		h += state[7];

		state[0] = a;
		state[1] = b;
		state[2] = c;
		state[3] = d;
		state[4] = e;
		state[5] = f;
		state[6] = g;
		state[7] = h;

		in += 64;
		inlen -= 64;
	}

	store_bigendian_32(statebytes + 0, state[0]);
	store_bigendian_32(statebytes + 4, state[1]);
	store_bigendian_32(statebytes + 8, state[2]);
	store_bigendian_32(statebytes + 12, state[3]);
	store_bigendian_32(statebytes + 16, state[4]);
	store_bigendian_32(statebytes + 20, state[5]);
	store_bigendian_32(statebytes + 24, state[6]);
	store_bigendian_32(statebytes + 28, state[7]);

	return inlen;
}
*/

static size_t crypto_hashblocks_sha256(uint8_t *statebytes,
                                       const uint8_t *data, size_t length) {
	static unsigned int s256cst[64]= {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
		0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };
	unsigned long long pos = 0;
	/* load constants */
	uint32x4_t c0 = vld1q_u32(s256cst +  0);
	uint32x4_t c1 = vld1q_u32(s256cst +  4);
	uint32x4_t c2 = vld1q_u32(s256cst +  8);
	uint32x4_t c3 = vld1q_u32(s256cst + 12);
	uint32x4_t c4 = vld1q_u32(s256cst + 16);
	uint32x4_t c5 = vld1q_u32(s256cst + 20);
	uint32x4_t c6 = vld1q_u32(s256cst + 24);
	uint32x4_t c7 = vld1q_u32(s256cst + 28);
	uint32x4_t c8 = vld1q_u32(s256cst + 32);
	uint32x4_t c9 = vld1q_u32(s256cst + 36);
	uint32x4_t ca = vld1q_u32(s256cst + 40);
	uint32x4_t cb = vld1q_u32(s256cst + 44);
	uint32x4_t cc = vld1q_u32(s256cst + 48);
	uint32x4_t cd = vld1q_u32(s256cst + 52);
	uint32x4_t ce = vld1q_u32(s256cst + 56);
	uint32x4_t cf = vld1q_u32(s256cst + 60);
	/* load state */
	uint32x4_t d0 = vld1q_u32((uint32_t*)(statebytes +  0));
	uint32x4_t d1 = vld1q_u32((uint32_t*)(statebytes + 16));
	uint32x4_t s0, s1, h0, h1;
	/* make state big-endian */
	d0 = (uint32x4_t)vrev32q_u8((uint8x16_t)d0);
	d1 = (uint32x4_t)vrev32q_u8((uint8x16_t)d1);
	while (length	>= 64) {
		/* load one block */
		uint32x4_t i0 = vld1q_u32((uint32_t*)(data+pos+ 0));
		uint32x4_t i1 = vld1q_u32((uint32_t*)(data+pos+16));
		uint32x4_t i2 = vld1q_u32((uint32_t*)(data+pos+32));
		uint32x4_t i3 = vld1q_u32((uint32_t*)(data+pos+48));
		uint32x4_t j0, j1, j2, j3;
		uint32x4_t x0, x1;
		
		/* copy state to working space */
		s0 = d0;
		s1 = d1;
		
		/* make block big-endian */
		i0 = (uint32x4_t)vrev32q_u8((uint8x16_t)i0);
		i1 = (uint32x4_t)vrev32q_u8((uint8x16_t)i1);
		i2 = (uint32x4_t)vrev32q_u8((uint8x16_t)i2);
		i3 = (uint32x4_t)vrev32q_u8((uint8x16_t)i3);

		/*
		 * This computes 16 rounds in i0..i3
		 * using 16 constants in c0..c3
		 * we need h0,h1,x0,x1 as scratch
		 */
#define DO16ROUNDS(i0, i1, i2, i3, c0, c1, c2, c3)	\
		h0 = vaddq_u32(i0, c0);			\
		x0 = vsha256hq_u32(s0, s1, h0);		\
		x1 = vsha256h2q_u32(s1, s0, h0);	\
		h1 = vaddq_u32(i1, c1);			\
		s0 = vsha256hq_u32(x0, x1, h1);		\
		s1 = vsha256h2q_u32(x1, x0, h1);	\
		h0 = vaddq_u32(i2, c2);			\
		x0 = vsha256hq_u32(s0, s1, h0);		\
		x1 = vsha256h2q_u32(s1, s0, h0);	\
		h1 = vaddq_u32(i3, c3);			\
		s0 = vsha256hq_u32(x0, x1, h1);		\
		s1 = vsha256h2q_u32(x1, x0, h1)

		/*
		 * this expands the block (or previously
		 * expanded) in i0..i3 to j0..j3
		 */
#define DO16EXPANDS(i0, i1, i2, i3, j0, j1, j2, j3)	\
		j0 = vsha256su0q_u32(i0, i1);		\
		j0 = vsha256su1q_u32(j0, i2, i3);	\
		j1 = vsha256su0q_u32(i1, i2);		\
		j1 = vsha256su1q_u32(j1, i3, j0);	\
		j2 = vsha256su0q_u32(i2, i3);		\
		j2 = vsha256su1q_u32(j2, j0, j1);	\
		j3 = vsha256su0q_u32(i3, j0);		\
		j3 = vsha256su1q_u32(j3, j1, j2)

		DO16ROUNDS(i0, i1, i2, i3, c0, c1, c2, c3);

		DO16EXPANDS(i0, i1, i2, i3, j0, j1, j2, j3);

		DO16ROUNDS(j0, j1, j2, j3, c4, c5, c6, c7);

		DO16EXPANDS(j0, j1, j2, j3, i0, i1, i2, i3);

		DO16ROUNDS(i0, i1, i2, i3, c8, c9, ca, cb);

		DO16EXPANDS(i0, i1, i2, i3, j0, j1, j2, j3);

		DO16ROUNDS(j0, j1, j2, j3, cc, cd, ce, cf);

		/* final add to update states */
		d0 = vaddq_u32(s0, d0);
		d1 = vaddq_u32(s1, d1);

		length -= 64;
		pos += 64;
	}

	/* store back to little-endian */
	d0 = (uint32x4_t)vrev32q_u8((uint8x16_t)d0);
	d1 = (uint32x4_t)vrev32q_u8((uint8x16_t)d1);
	vst1q_u32((uint32_t*)(statebytes +  0), d0);
	vst1q_u32((uint32_t*)(statebytes + 16), d1);

	return length;

}

/*
static size_t crypto_hashblocks_sha512(uint8_t *statebytes,
                                       const uint8_t *in, size_t inlen) {
	uint64_t state[8];
	uint64_t a;
	uint64_t b;
	uint64_t c;
	uint64_t d;
	uint64_t e;
	uint64_t f;
	uint64_t g;
	uint64_t h;
	uint64_t T1;
	uint64_t T2;

	a = load_bigendian_64(statebytes + 0);
	state[0] = a;
	b = load_bigendian_64(statebytes + 8);
	state[1] = b;
	c = load_bigendian_64(statebytes + 16);
	state[2] = c;
	d = load_bigendian_64(statebytes + 24);
	state[3] = d;
	e = load_bigendian_64(statebytes + 32);
	state[4] = e;
	f = load_bigendian_64(statebytes + 40);
	state[5] = f;
	g = load_bigendian_64(statebytes + 48);
	state[6] = g;
	h = load_bigendian_64(statebytes + 56);
	state[7] = h;

	while (inlen >= 128) {
		uint64_t w0 = load_bigendian_64(in + 0);
		uint64_t w1 = load_bigendian_64(in + 8);
		uint64_t w2 = load_bigendian_64(in + 16);
		uint64_t w3 = load_bigendian_64(in + 24);
		uint64_t w4 = load_bigendian_64(in + 32);
		uint64_t w5 = load_bigendian_64(in + 40);
		uint64_t w6 = load_bigendian_64(in + 48);
		uint64_t w7 = load_bigendian_64(in + 56);
		uint64_t w8 = load_bigendian_64(in + 64);
		uint64_t w9 = load_bigendian_64(in + 72);
		uint64_t w10 = load_bigendian_64(in + 80);
		uint64_t w11 = load_bigendian_64(in + 88);
		uint64_t w12 = load_bigendian_64(in + 96);
		uint64_t w13 = load_bigendian_64(in + 104);
		uint64_t w14 = load_bigendian_64(in + 112);
		uint64_t w15 = load_bigendian_64(in + 120);

		F_64(w0, 0x428a2f98d728ae22ULL)
		F_64(w1, 0x7137449123ef65cdULL)
		F_64(w2, 0xb5c0fbcfec4d3b2fULL)
		F_64(w3, 0xe9b5dba58189dbbcULL)
		F_64(w4, 0x3956c25bf348b538ULL)
		F_64(w5, 0x59f111f1b605d019ULL)
		F_64(w6, 0x923f82a4af194f9bULL)
		F_64(w7, 0xab1c5ed5da6d8118ULL)
		F_64(w8, 0xd807aa98a3030242ULL)
		F_64(w9, 0x12835b0145706fbeULL)
		F_64(w10, 0x243185be4ee4b28cULL)
		F_64(w11, 0x550c7dc3d5ffb4e2ULL)
		F_64(w12, 0x72be5d74f27b896fULL)
		F_64(w13, 0x80deb1fe3b1696b1ULL)
		F_64(w14, 0x9bdc06a725c71235ULL)
		F_64(w15, 0xc19bf174cf692694ULL)

		EXPAND_64

		F_64(w0, 0xe49b69c19ef14ad2ULL)
		F_64(w1, 0xefbe4786384f25e3ULL)
		F_64(w2, 0x0fc19dc68b8cd5b5ULL)
		F_64(w3, 0x240ca1cc77ac9c65ULL)
		F_64(w4, 0x2de92c6f592b0275ULL)
		F_64(w5, 0x4a7484aa6ea6e483ULL)
		F_64(w6, 0x5cb0a9dcbd41fbd4ULL)
		F_64(w7, 0x76f988da831153b5ULL)
		F_64(w8, 0x983e5152ee66dfabULL)
		F_64(w9, 0xa831c66d2db43210ULL)
		F_64(w10, 0xb00327c898fb213fULL)
		F_64(w11, 0xbf597fc7beef0ee4ULL)
		F_64(w12, 0xc6e00bf33da88fc2ULL)
		F_64(w13, 0xd5a79147930aa725ULL)
		F_64(w14, 0x06ca6351e003826fULL)
		F_64(w15, 0x142929670a0e6e70ULL)

		EXPAND_64

		F_64(w0, 0x27b70a8546d22ffcULL)
		F_64(w1, 0x2e1b21385c26c926ULL)
		F_64(w2, 0x4d2c6dfc5ac42aedULL)
		F_64(w3, 0x53380d139d95b3dfULL)
		F_64(w4, 0x650a73548baf63deULL)
		F_64(w5, 0x766a0abb3c77b2a8ULL)
		F_64(w6, 0x81c2c92e47edaee6ULL)
		F_64(w7, 0x92722c851482353bULL)
		F_64(w8, 0xa2bfe8a14cf10364ULL)
		F_64(w9, 0xa81a664bbc423001ULL)
		F_64(w10, 0xc24b8b70d0f89791ULL)
		F_64(w11, 0xc76c51a30654be30ULL)
		F_64(w12, 0xd192e819d6ef5218ULL)
		F_64(w13, 0xd69906245565a910ULL)
		F_64(w14, 0xf40e35855771202aULL)
		F_64(w15, 0x106aa07032bbd1b8ULL)

		EXPAND_64

		F_64(w0, 0x19a4c116b8d2d0c8ULL)
		F_64(w1, 0x1e376c085141ab53ULL)
		F_64(w2, 0x2748774cdf8eeb99ULL)
		F_64(w3, 0x34b0bcb5e19b48a8ULL)
		F_64(w4, 0x391c0cb3c5c95a63ULL)
		F_64(w5, 0x4ed8aa4ae3418acbULL)
		F_64(w6, 0x5b9cca4f7763e373ULL)
		F_64(w7, 0x682e6ff3d6b2b8a3ULL)
		F_64(w8, 0x748f82ee5defb2fcULL)
		F_64(w9, 0x78a5636f43172f60ULL)
		F_64(w10, 0x84c87814a1f0ab72ULL)
		F_64(w11, 0x8cc702081a6439ecULL)
		F_64(w12, 0x90befffa23631e28ULL)
		F_64(w13, 0xa4506cebde82bde9ULL)
		F_64(w14, 0xbef9a3f7b2c67915ULL)
		F_64(w15, 0xc67178f2e372532bULL)

		EXPAND_64

		F_64(w0, 0xca273eceea26619cULL)
		F_64(w1, 0xd186b8c721c0c207ULL)
		F_64(w2, 0xeada7dd6cde0eb1eULL)
		F_64(w3, 0xf57d4f7fee6ed178ULL)
		F_64(w4, 0x06f067aa72176fbaULL)
		F_64(w5, 0x0a637dc5a2c898a6ULL)
		F_64(w6, 0x113f9804bef90daeULL)
		F_64(w7, 0x1b710b35131c471bULL)
		F_64(w8, 0x28db77f523047d84ULL)
		F_64(w9, 0x32caab7b40c72493ULL)
		F_64(w10, 0x3c9ebe0a15c9bebcULL)
		F_64(w11, 0x431d67c49c100d4cULL)
		F_64(w12, 0x4cc5d4becb3e42b6ULL)
		F_64(w13, 0x597f299cfc657e2aULL)
		F_64(w14, 0x5fcb6fab3ad6faecULL)
		F_64(w15, 0x6c44198c4a475817ULL)

		a += state[0];
		b += state[1];
		c += state[2];
		d += state[3];
		e += state[4];
		f += state[5];
		g += state[6];
		h += state[7];

		state[0] = a;
		state[1] = b;
		state[2] = c;
		state[3] = d;
		state[4] = e;
		state[5] = f;
		state[6] = g;
		state[7] = h;

		in += 128;
		inlen -= 128;
	}

	store_bigendian_64(statebytes + 0, state[0]);
	store_bigendian_64(statebytes + 8, state[1]);
	store_bigendian_64(statebytes + 16, state[2]);
	store_bigendian_64(statebytes + 24, state[3]);
	store_bigendian_64(statebytes + 32, state[4]);
	store_bigendian_64(statebytes + 40, state[5]);
	store_bigendian_64(statebytes + 48, state[6]);
	store_bigendian_64(statebytes + 56, state[7]);

	return inlen;
}
*/
void oqs_sha2_sha256_inc_blocks_ni(sha256ctx *state, const uint8_t *in, size_t inblocks) {
	
	uint64_t bytes = load_bigendian_64(state->ctx + 32);

	crypto_hashblocks_sha256(state->ctx, in, 64 * inblocks);
	bytes += 64 * inblocks;

	store_bigendian_64(state->ctx + 32, bytes);
}

/*
void oqs_sha2_sha512_inc_blocks_ni(sha512ctx *state, const uint8_t *in, size_t inblocks) {
	uint64_t bytes = load_bigendian_64(state->ctx + 64);

	crypto_hashblocks_sha512(state->ctx, in, 128 * inblocks);
	bytes += 128 * inblocks;

	store_bigendian_64(state->ctx + 64, bytes);
}

void oqs_sha2_sha384_inc_blocks_ni(sha384ctx *state, const uint8_t *in, size_t inblocks) {
	oqs_sha2_sha512_inc_blocks_ni((sha512ctx *) state, in, inblocks);
}
*/
void oqs_sha2_sha256_inc_finalize_ni(uint8_t *out, sha256ctx *state, const uint8_t *in, size_t inlen) {
	uint8_t padded[128];
	uint64_t bytes = load_bigendian_64(state->ctx + 32) + inlen;

	crypto_hashblocks_sha256(state->ctx, in, inlen);
	in += inlen;
	inlen &= 63;
	in -= inlen;


	for (size_t i = 0; i < inlen; ++i) {
		padded[i] = in[i];
	}
	padded[inlen] = 0x80;

	if (inlen < 56) {
		for (size_t i = inlen + 1; i < 56; ++i) {
			padded[i] = 0;
		}
		padded[56] = (uint8_t) (bytes >> 53);
		padded[57] = (uint8_t) (bytes >> 45);
		padded[58] = (uint8_t) (bytes >> 37);
		padded[59] = (uint8_t) (bytes >> 29);
		padded[60] = (uint8_t) (bytes >> 21);
		padded[61] = (uint8_t) (bytes >> 13);
		padded[62] = (uint8_t) (bytes >> 5);
		padded[63] = (uint8_t) (bytes << 3);
		crypto_hashblocks_sha256(state->ctx, padded, 64);
	} else {
		for (size_t i = inlen + 1; i < 120; ++i) {
			padded[i] = 0;
		}
		padded[120] = (uint8_t) (bytes >> 53);
		padded[121] = (uint8_t) (bytes >> 45);
		padded[122] = (uint8_t) (bytes >> 37);
		padded[123] = (uint8_t) (bytes >> 29);
		padded[124] = (uint8_t) (bytes >> 21);
		padded[125] = (uint8_t) (bytes >> 13);
		padded[126] = (uint8_t) (bytes >> 5);
		padded[127] = (uint8_t) (bytes << 3);
		crypto_hashblocks_sha256(state->ctx, padded, 128);
	}

	for (size_t i = 0; i < 32; ++i) {
		out[i] = state->ctx[i];
	}
	oqs_sha2_sha256_inc_ctx_release_c(state);
}
/*
void sha224_inc_finalize(uint8_t *out, sha224ctx *state, const uint8_t *in, size_t inlen) {
	uint8_t tmp[32];
	oqs_sha2_sha256_inc_finalize_ni(tmp, (sha256ctx *)state, in, inlen);

	for (size_t i = 0; i < 28; ++i) {
		out[i] = tmp[i];
	}
}

void oqs_sha2_sha512_inc_finalize_ni(uint8_t *out, sha512ctx *state, const uint8_t *in, size_t inlen) {
	uint8_t padded[256];
	uint64_t bytes = load_bigendian_64(state->ctx + 64) + inlen;

	crypto_hashblocks_sha512(state->ctx, in, inlen);
	in += inlen;
	inlen &= 127;
	in -= inlen;

	for (size_t i = 0; i < inlen; ++i) {
		padded[i] = in[i];
	}
	padded[inlen] = 0x80;

	if (inlen < 112) {
		for (size_t i = inlen + 1; i < 119; ++i) {
			padded[i] = 0;
		}
		padded[119] = (uint8_t) (bytes >> 61);
		padded[120] = (uint8_t) (bytes >> 53);
		padded[121] = (uint8_t) (bytes >> 45);
		padded[122] = (uint8_t) (bytes >> 37);
		padded[123] = (uint8_t) (bytes >> 29);
		padded[124] = (uint8_t) (bytes >> 21);
		padded[125] = (uint8_t) (bytes >> 13);
		padded[126] = (uint8_t) (bytes >> 5);
		padded[127] = (uint8_t) (bytes << 3);
		crypto_hashblocks_sha512(state->ctx, padded, 128);
	} else {
		for (size_t i = inlen + 1; i < 247; ++i) {
			padded[i] = 0;
		}
		padded[247] = (uint8_t) (bytes >> 61);
		padded[248] = (uint8_t) (bytes >> 53);
		padded[249] = (uint8_t) (bytes >> 45);
		padded[250] = (uint8_t) (bytes >> 37);
		padded[251] = (uint8_t) (bytes >> 29);
		padded[252] = (uint8_t) (bytes >> 21);
		padded[253] = (uint8_t) (bytes >> 13);
		padded[254] = (uint8_t) (bytes >> 5);
		padded[255] = (uint8_t) (bytes << 3);
		crypto_hashblocks_sha512(state->ctx, padded, 256);
	}

	for (size_t i = 0; i < 64; ++i) {
		out[i] = state->ctx[i];
	}
	oqs_sha2_sha512_inc_ctx_release_ni(state);
}

void oqs_sha2_sha384_inc_finalize_ni(uint8_t *out, sha384ctx *state, const uint8_t *in, size_t inlen) {
	uint8_t tmp[64];
	oqs_sha2_sha512_inc_finalize_ni(tmp, (sha512ctx *)state, in, inlen);

	for (size_t i = 0; i < 48; ++i) {
		out[i] = tmp[i];
	}
}
*/
void oqs_sha2_sha256_ni(uint8_t *out, const uint8_t *in, size_t inlen) {
	sha256ctx state;

	oqs_sha2_sha256_inc_init_c(&state);
	oqs_sha2_sha256_inc_finalize_ni(out, &state, in, inlen);
}
/*
void oqs_sha2_sha384_ni(uint8_t *out, const uint8_t *in, size_t inlen) {
	sha384ctx state;

	oqs_sha2_sha384_inc_init_c(&state);
	oqs_sha2_sha384_inc_finalize_c(out, &state, in, inlen);
}

void oqs_sha2_sha512_ni(uint8_t *out, const uint8_t *in, size_t inlen) {
	sha512ctx state;

	oqs_sha2_sha512_inc_init_c(&state);
	oqs_sha2_sha512_inc_finalize_c(out, &state, in, inlen);
}

void sha224(uint8_t *out, const uint8_t *in, size_t inlen) {
	sha224ctx state;

	sha224_inc_init(&state);
	sha224_inc_finalize(out, &state, in, inlen);
}
*/
