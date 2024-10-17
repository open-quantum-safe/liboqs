// SPDX-License-Identifier: Public domain

#include <oqs/oqs.h>

#include "sha2_local.h"
#include <string.h>
#include <stdint.h>
// ARM includes
#ifndef WIN32
#include <arm_acle.h>
#endif
#include <arm_neon.h>
/* Based on the public domain implementation in
 * crypto_hashblocks/sha256/dolbeau/armv8crypto
 * from http://bench.cr.yp.to/supercop.html
 * by D. J. Bernstein */

static uint64_t load_bigendian_64(const uint8_t *x) {
	return (uint64_t)(x[7]) | (((uint64_t)(x[6])) << 8) |
	       (((uint64_t)(x[5])) << 16) | (((uint64_t)(x[4])) << 24) |
	       (((uint64_t)(x[3])) << 32) | (((uint64_t)(x[2])) << 40) |
	       (((uint64_t)(x[1])) << 48) | (((uint64_t)(x[0])) << 56);
}

static void store_bigendian_64(uint8_t *x, uint64_t u) {
	x[7] = (uint8_t)u;
	u >>= 8;
	x[6] = (uint8_t)u;
	u >>= 8;
	x[5] = (uint8_t)u;
	u >>= 8;
	x[4] = (uint8_t)u;
	u >>= 8;
	x[3] = (uint8_t)u;
	u >>= 8;
	x[2] = (uint8_t)u;
	u >>= 8;
	x[1] = (uint8_t)u;
	u >>= 8;
	x[0] = (uint8_t)u;
}

static size_t crypto_hashblocks_sha256_armv8(uint8_t *statebytes,
        const uint8_t *data, size_t length) {
	static unsigned int s256cst[64] = {
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
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	unsigned long long pos = 0;
	/* load constants */
	uint32x4_t c0 = vld1q_u32(s256cst + 0);
	uint32x4_t c1 = vld1q_u32(s256cst + 4);
	uint32x4_t c2 = vld1q_u32(s256cst + 8);
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
	uint32x4_t d0 = vld1q_u32((uint32_t *)(statebytes + 0));
	uint32x4_t d1 = vld1q_u32((uint32_t *)(statebytes + 16));
	uint32x4_t s0, s1, h0, h1;
	/* make state big-endian */
	d0 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(d0)));
	d1 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(d1)));
	while (length >= 64) {
		/* load one block */
		uint32x4_t i0 = vld1q_u32((const uint32_t *)(data + pos + 0));
		uint32x4_t i1 = vld1q_u32((const uint32_t *)(data + pos + 16));
		uint32x4_t i2 = vld1q_u32((const uint32_t *)(data + pos + 32));
		uint32x4_t i3 = vld1q_u32((const uint32_t *)(data + pos + 48));
		uint32x4_t j0, j1, j2, j3;
		uint32x4_t x0, x1;

		/* copy state to working space */
		s0 = d0;
		s1 = d1;

		/* make block big-endian */
		i0 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(i0)));
		i1 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(i1)));
		i2 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(i2)));
		i3 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(i3)));

		/*
		 * This computes 16 rounds in i0..i3
		 * using 16 constants in c0..c3
		 * we need h0,h1,x0,x1 as scratch
		 */
#define DO16ROUNDS(i0, i1, i2, i3, c0, c1, c2, c3) \
    h0 = vaddq_u32(i0, c0);                        \
    x0 = vsha256hq_u32(s0, s1, h0);                \
    x1 = vsha256h2q_u32(s1, s0, h0);               \
    h1 = vaddq_u32(i1, c1);                        \
    s0 = vsha256hq_u32(x0, x1, h1);                \
    s1 = vsha256h2q_u32(x1, x0, h1);               \
    h0 = vaddq_u32(i2, c2);                        \
    x0 = vsha256hq_u32(s0, s1, h0);                \
    x1 = vsha256h2q_u32(s1, s0, h0);               \
    h1 = vaddq_u32(i3, c3);                        \
    s0 = vsha256hq_u32(x0, x1, h1);                \
    s1 = vsha256h2q_u32(x1, x0, h1)

		/*
		 * this expands the block (or previously
		 * expanded) in i0..i3 to j0..j3
		 */
#define DO16EXPANDS(i0, i1, i2, i3, j0, j1, j2, j3) \
    j0 = vsha256su0q_u32(i0, i1);                   \
    j0 = vsha256su1q_u32(j0, i2, i3);               \
    j1 = vsha256su0q_u32(i1, i2);                   \
    j1 = vsha256su1q_u32(j1, i3, j0);               \
    j2 = vsha256su0q_u32(i2, i3);                   \
    j2 = vsha256su1q_u32(j2, j0, j1);               \
    j3 = vsha256su0q_u32(i3, j0);                   \
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
	d0 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(d0)));
	d1 = vreinterpretq_u32_u8(vrev32q_u8(vreinterpretq_u8_u32(d1)));
	vst1q_u32((uint32_t *)(statebytes + 0), d0);
	vst1q_u32((uint32_t *)(statebytes + 16), d1);

	return length;
}

void oqs_sha2_sha256_inc_finalize_armv8(uint8_t *out, sha256ctx *state, const uint8_t *in, size_t inlen) {
	uint8_t padded[128];

	size_t new_inlen = state->data_len + inlen;
	size_t tmp_len = new_inlen;
	const uint8_t *new_in;
	uint8_t *tmp_in = NULL;

	if (new_inlen == inlen) {
		new_in = in;
	} else {
		// Combine incremental data with final input
		tmp_in = OQS_MEM_malloc(tmp_len);
		OQS_EXIT_IF_NULLPTR(tmp_in, "SHA2");
		memcpy(tmp_in, state->data, state->data_len);
		if (in && inlen) {
			memcpy(tmp_in + state->data_len, in, inlen);
		}
		new_in = tmp_in;
		state->data_len = 0;
	}

	uint64_t bytes = load_bigendian_64(state->ctx + 32) + new_inlen;

	crypto_hashblocks_sha256_armv8(state->ctx, new_in, new_inlen);
	new_in += new_inlen;
	new_inlen &= 63;
	new_in -= new_inlen;

	for (size_t i = 0; i < new_inlen; ++i) {
		padded[i] = new_in[i];
	}
	padded[new_inlen] = 0x80;

	if (new_inlen < 56) {
		for (size_t i = new_inlen + 1; i < 56; ++i) {
			padded[i] = 0;
		}
		padded[56] = (uint8_t)(bytes >> 53);
		padded[57] = (uint8_t)(bytes >> 45);
		padded[58] = (uint8_t)(bytes >> 37);
		padded[59] = (uint8_t)(bytes >> 29);
		padded[60] = (uint8_t)(bytes >> 21);
		padded[61] = (uint8_t)(bytes >> 13);
		padded[62] = (uint8_t)(bytes >> 5);
		padded[63] = (uint8_t)(bytes << 3);
		crypto_hashblocks_sha256_armv8(state->ctx, padded, 64);
	} else {
		for (size_t i = new_inlen + 1; i < 120; ++i) {
			padded[i] = 0;
		}
		padded[120] = (uint8_t)(bytes >> 53);
		padded[121] = (uint8_t)(bytes >> 45);
		padded[122] = (uint8_t)(bytes >> 37);
		padded[123] = (uint8_t)(bytes >> 29);
		padded[124] = (uint8_t)(bytes >> 21);
		padded[125] = (uint8_t)(bytes >> 13);
		padded[126] = (uint8_t)(bytes >> 5);
		padded[127] = (uint8_t)(bytes << 3);
		crypto_hashblocks_sha256_armv8(state->ctx, padded, 128);
	}

	for (size_t i = 0; i < 32; ++i) {
		out[i] = state->ctx[i];
	}
	oqs_sha2_sha256_inc_ctx_release_c(state);
	OQS_MEM_secure_free(tmp_in, tmp_len);
}

void oqs_sha2_sha224_inc_finalize_armv8(uint8_t *out, sha224ctx *state, const uint8_t *in, size_t inlen) {
	uint8_t tmp[32];
	oqs_sha2_sha256_inc_finalize_armv8(tmp, (sha256ctx *)state, in, inlen);

	for (size_t i = 0; i < 28; ++i) {
		out[i] = tmp[i];
	}
}

void oqs_sha2_sha256_inc_blocks_armv8(sha256ctx *state, const uint8_t *in, size_t inblocks) {
	uint64_t bytes = load_bigendian_64(state->ctx + 32);
	const uint8_t *new_in;
	size_t buf_len = 64 * inblocks;
	uint8_t *tmp_in = NULL;

	/* Process any existing incremental data first */
	if (state->data_len) {
		tmp_in = OQS_MEM_malloc(buf_len);
		OQS_EXIT_IF_NULLPTR(tmp_in, "SHA2");
		memcpy(tmp_in, state->data, state->data_len);
		memcpy(tmp_in + state->data_len, in, buf_len - state->data_len);

		/* store the reminder input as incremental data */
		memcpy(state->data, in + (buf_len - state->data_len), state->data_len);
		new_in = tmp_in;
	} else {
		new_in = in;
	}

	crypto_hashblocks_sha256_armv8(state->ctx, new_in, 64 * inblocks);
	bytes += 64 * inblocks;

	store_bigendian_64(state->ctx + 32, bytes);
	OQS_MEM_secure_free(tmp_in, buf_len);
}

void oqs_sha2_sha256_inc_armv8(sha256ctx *state, const uint8_t *in, size_t len) {
	while (len) {
		size_t incr = 64 - state->data_len;
		if (incr > len) {
			incr = len;
		}

		memcpy(state->data + state->data_len, in, incr);
		state->data_len += incr;
		in += incr;

		if (state->data_len < 64) {
			break;
		}

		/*
		 * Process a complete block now
		 */
		uint64_t bytes = load_bigendian_64(state->ctx + 32) + 64;
		crypto_hashblocks_sha256_armv8(state->ctx, state->data, 64);
		store_bigendian_64(state->ctx + 32, bytes);

		/*
		 * update the remaining input
		 */
		len -= incr;
		state->data_len = 0;
	}
}

void oqs_sha2_sha224_inc_blocks_armv8(sha224ctx *state, const uint8_t *in, size_t inblocks) {
	oqs_sha2_sha256_inc_blocks_armv8((sha256ctx *)state, in, inblocks);
}

void oqs_sha2_sha256_armv8(uint8_t *out, const uint8_t *in, size_t inlen) {
	sha256ctx state;

	oqs_sha2_sha256_inc_init_c(&state);
	oqs_sha2_sha256_inc_finalize_armv8(out, &state, in, inlen);
}

void oqs_sha2_sha224_armv8(uint8_t *out, const uint8_t *in, size_t inlen) {
	sha224ctx state;

	oqs_sha2_sha224_inc_init_c(&state);
	oqs_sha2_sha224_inc_finalize_armv8(out, &state, in, inlen);
}
