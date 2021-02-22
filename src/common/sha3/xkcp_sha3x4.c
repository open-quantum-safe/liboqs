// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqsconfig.h>

#include "KeccakP-1600-times4-SnP.h"
#include "sha3.h"
#include "sha3x4.h"

#define KECCAK_X4_CTX_ALIGNMENT KeccakP1600times4_statesAlignment

#if KeccakP1600times4_statesSizeInBytes == 800
#define KECCAK_X4_CTX_BYTES (sizeof(uint64_t)*100)
#define KECCAK_X4_INC_CTX_BYTES (sizeof(uint64_t)*101)
#else
#error sha3x4_xkcp assumes 800 byte KeccakP1600 state
#endif

#define KeccakF1600times4_InitializeAll KeccakP1600times4_InitializeAll
#define KeccakF1600times4_ExtractBytes KeccakP1600times4_ExtractBytes
#define KeccakF1600times4_AddByte KeccakP1600times4_AddByte
#define KeccakF1600times4_AddBytes KeccakP1600times4_AddBytes
#define KeccakF1600times4_StatePermuteAll KeccakP1600times4_PermuteAll_24rounds

static void keccak_x4_inc_reset(uint64_t *s) {
	KeccakF1600times4_InitializeAll(s);
	s[100] = 0;
}

static void keccak_x4_inc_absorb(uint64_t *s, uint32_t r,
                                 const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	size_t c;

	if (s[100] && inlen + s[100] >= r) {
		c = r - s[100];
		KeccakF1600times4_AddBytes(s, 0, in0, (unsigned int)s[100], (unsigned int)c);
		KeccakF1600times4_AddBytes(s, 1, in1, (unsigned int)s[100], (unsigned int)c);
		KeccakF1600times4_AddBytes(s, 2, in2, (unsigned int)s[100], (unsigned int)c);
		KeccakF1600times4_AddBytes(s, 3, in3, (unsigned int)s[100], (unsigned int)c);
		KeccakF1600times4_StatePermuteAll(s);
		inlen -= c;
		in0 += c;
		in1 += c;
		in2 += c;
		in3 += c;
		s[100] = 0;
	}

	while (inlen >= r) {
		KeccakF1600times4_AddBytes(s, 0, in0, 0, (unsigned int)r);
		KeccakF1600times4_AddBytes(s, 1, in1, 0, (unsigned int)r);
		KeccakF1600times4_AddBytes(s, 2, in2, 0, (unsigned int)r);
		KeccakF1600times4_AddBytes(s, 3, in3, 0, (unsigned int)r);
		KeccakF1600times4_StatePermuteAll(s);
		inlen -= r;
		in0 += r;
		in1 += r;
		in2 += r;
		in3 += r;
	}

	KeccakF1600times4_AddBytes(s, 0, in0, (unsigned int)s[100], (unsigned int)inlen);
	KeccakF1600times4_AddBytes(s, 1, in1, (unsigned int)s[100], (unsigned int)inlen);
	KeccakF1600times4_AddBytes(s, 2, in2, (unsigned int)s[100], (unsigned int)inlen);
	KeccakF1600times4_AddBytes(s, 3, in3, (unsigned int)s[100], (unsigned int)inlen);
	s[100] += inlen;
}

static void keccak_x4_inc_finalize(uint64_t *s, uint32_t r, uint8_t p) {
	KeccakF1600times4_AddByte(s, 0, p, (unsigned int)s[100]);
	KeccakF1600times4_AddByte(s, 1, p, (unsigned int)s[100]);
	KeccakF1600times4_AddByte(s, 2, p, (unsigned int)s[100]);
	KeccakF1600times4_AddByte(s, 3, p, (unsigned int)s[100]);

	KeccakF1600times4_AddByte(s, 0, 0x80, (unsigned int)(r - 1));
	KeccakF1600times4_AddByte(s, 1, 0x80, (unsigned int)(r - 1));
	KeccakF1600times4_AddByte(s, 2, 0x80, (unsigned int)(r - 1));
	KeccakF1600times4_AddByte(s, 3, 0x80, (unsigned int)(r - 1));

	s[100] = 0;
}

static void keccak_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3,
                                  size_t outlen, uint64_t *s, uint32_t r) {

	while (outlen > s[100]) {
		KeccakF1600times4_ExtractBytes(s, 0, out0, (unsigned int)(r - s[100]), (unsigned int)s[100]);
		KeccakF1600times4_ExtractBytes(s, 1, out1, (unsigned int)(r - s[100]), (unsigned int)s[100]);
		KeccakF1600times4_ExtractBytes(s, 2, out2, (unsigned int)(r - s[100]), (unsigned int)s[100]);
		KeccakF1600times4_ExtractBytes(s, 3, out3, (unsigned int)(r - s[100]), (unsigned int)s[100]);
		KeccakF1600times4_StatePermuteAll(s);
		out0 += s[100];
		out1 += s[100];
		out2 += s[100];
		out3 += s[100];
		outlen -= s[100];
		s[100] = r;
	}

	KeccakF1600times4_ExtractBytes(s, 0, out0, (unsigned int)(r - s[100]), (unsigned int)outlen);
	KeccakF1600times4_ExtractBytes(s, 1, out1, (unsigned int)(r - s[100]), (unsigned int)outlen);
	KeccakF1600times4_ExtractBytes(s, 2, out2, (unsigned int)(r - s[100]), (unsigned int)outlen);
	KeccakF1600times4_ExtractBytes(s, 3, out3, (unsigned int)(r - s[100]), (unsigned int)outlen);

	s[100] -= outlen;
}

/********** SHAKE128 ***********/

void OQS_SHA3_shake128_x4(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	OQS_SHA3_shake128_x4_inc_ctx s;
	OQS_SHA3_shake128_x4_inc_init(&s);
	OQS_SHA3_shake128_x4_inc_absorb(&s, in0, in1, in2, in3, inlen);
	OQS_SHA3_shake128_x4_inc_finalize(&s);
	OQS_SHA3_shake128_x4_inc_squeeze(out0, out1, out2, out3, outlen, &s);
	OQS_SHA3_shake128_x4_inc_ctx_release(&s);
}

/* SHAKE128 incremental */

void OQS_SHA3_shake128_x4_inc_init(OQS_SHA3_shake128_x4_inc_ctx *state) {
	state->ctx = aligned_alloc(KECCAK_X4_CTX_ALIGNMENT, KECCAK_X4_INC_CTX_BYTES);
	if (state->ctx == NULL) {
		exit(111);
	}
	keccak_x4_inc_reset((uint64_t *)state->ctx);
}

void OQS_SHA3_shake128_x4_inc_absorb(OQS_SHA3_shake128_x4_inc_ctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	keccak_x4_inc_absorb((uint64_t *)state->ctx, OQS_SHA3_SHAKE128_RATE, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake128_x4_inc_finalize(OQS_SHA3_shake128_x4_inc_ctx *state) {
	keccak_x4_inc_finalize((uint64_t *)state->ctx, OQS_SHA3_SHAKE128_RATE, 0x1F);
}

void OQS_SHA3_shake128_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, OQS_SHA3_shake128_x4_inc_ctx *state) {
	keccak_x4_inc_squeeze(out0, out1, out2, out3, outlen, (uint64_t *)state->ctx, OQS_SHA3_SHAKE128_RATE);
}

void OQS_SHA3_shake128_x4_inc_ctx_clone(OQS_SHA3_shake128_x4_inc_ctx *dest, const OQS_SHA3_shake128_x4_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_X4_INC_CTX_BYTES);
}

void OQS_SHA3_shake128_x4_inc_ctx_release(OQS_SHA3_shake128_x4_inc_ctx *state) {
	free(state->ctx); // IGNORE free-check
}

void OQS_SHA3_shake128_x4_inc_ctx_reset(OQS_SHA3_shake128_x4_inc_ctx *state) {
	keccak_x4_inc_reset((uint64_t *)state->ctx);
}

/********** SHAKE256 ***********/

void OQS_SHA3_shake256_x4(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	OQS_SHA3_shake256_x4_inc_ctx s;
	OQS_SHA3_shake256_x4_inc_init(&s);
	OQS_SHA3_shake256_x4_inc_absorb(&s, in0, in1, in2, in3, inlen);
	OQS_SHA3_shake256_x4_inc_finalize(&s);
	OQS_SHA3_shake256_x4_inc_squeeze(out0, out1, out2, out3, outlen, &s);
	OQS_SHA3_shake256_x4_inc_ctx_release(&s);
}

/* SHAKE256 incremental */

void OQS_SHA3_shake256_x4_inc_init(OQS_SHA3_shake256_x4_inc_ctx *state) {
	state->ctx = aligned_alloc(KECCAK_X4_CTX_ALIGNMENT, KECCAK_X4_INC_CTX_BYTES);
	if (state->ctx == NULL) {
		exit(111);
	}
	keccak_x4_inc_reset((uint64_t *)state->ctx);
}

void OQS_SHA3_shake256_x4_inc_absorb(OQS_SHA3_shake256_x4_inc_ctx *state, const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	keccak_x4_inc_absorb((uint64_t *)state->ctx, OQS_SHA3_SHAKE256_RATE, in0, in1, in2, in3, inlen);
}

void OQS_SHA3_shake256_x4_inc_finalize(OQS_SHA3_shake256_x4_inc_ctx *state) {
	keccak_x4_inc_finalize((uint64_t *)state->ctx, OQS_SHA3_SHAKE256_RATE, 0x1F);
}

void OQS_SHA3_shake256_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3, size_t outlen, OQS_SHA3_shake256_x4_inc_ctx *state) {
	keccak_x4_inc_squeeze(out0, out1, out2, out3, outlen, (uint64_t *)state->ctx, OQS_SHA3_SHAKE256_RATE);
}

void OQS_SHA3_shake256_x4_inc_ctx_clone(OQS_SHA3_shake256_x4_inc_ctx *dest, const OQS_SHA3_shake256_x4_inc_ctx *src) {
	memcpy(dest->ctx, src->ctx, KECCAK_X4_INC_CTX_BYTES);
}

void OQS_SHA3_shake256_x4_inc_ctx_release(OQS_SHA3_shake256_x4_inc_ctx *state) {
	free(state->ctx); // IGNORE free-check
}

void OQS_SHA3_shake256_x4_inc_ctx_reset(OQS_SHA3_shake256_x4_inc_ctx *state) {
	keccak_x4_inc_reset((uint64_t *)state->ctx);
}

