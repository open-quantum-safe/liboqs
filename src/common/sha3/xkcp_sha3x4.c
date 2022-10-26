// SPDX-License-Identifier: MIT

#include "sha3.h"
#include "sha3x4.h"

#include "xkcp_dispatch.h"

#include <oqs/common.h>
#include <oqs/oqsconfig.h>

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define KECCAK_X4_CTX_ALIGNMENT 32
#define _KECCAK_X4_CTX_BYTES (800+sizeof(uint64_t))
#define KECCAK_X4_CTX_BYTES (KECCAK_X4_CTX_ALIGNMENT * \
  ((_KECCAK_X4_CTX_BYTES + KECCAK_X4_CTX_ALIGNMENT - 1)/KECCAK_X4_CTX_ALIGNMENT))

/* The first call to Keccak_Initialize will be routed through dispatch, which
 * updates all of the function pointers used below.
 */
static KeccakX4InitFn Keccak_X4_Dispatch;
static KeccakX4InitFn *Keccak_X4_Initialize_ptr = &Keccak_X4_Dispatch;
static KeccakX4AddByteFn *Keccak_X4_AddByte_ptr = NULL;
static KeccakX4AddBytesFn *Keccak_X4_AddBytes_ptr = NULL;
static KeccakX4PermuteFn *Keccak_X4_Permute_ptr = NULL;
static KeccakX4ExtractBytesFn *Keccak_X4_ExtractBytes_ptr = NULL;

static void Keccak_X4_Dispatch(void *state) {
// TODO: Simplify this when we have a Windows-compatible AVX2 implementation of SHA3
#if defined(OQS_DIST_X86_64_BUILD)
#if defined(OQS_ENABLE_SHA3_xkcp_low_avx2)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2)) {
		Keccak_X4_Initialize_ptr = &KeccakP1600times4_InitializeAll_avx2;
		Keccak_X4_AddByte_ptr = &KeccakP1600times4_AddByte_avx2;
		Keccak_X4_AddBytes_ptr = &KeccakP1600times4_AddBytes_avx2;
		Keccak_X4_Permute_ptr = &KeccakP1600times4_PermuteAll_24rounds_avx2;
		Keccak_X4_ExtractBytes_ptr = &KeccakP1600times4_ExtractBytes_avx2;
	} else {
		Keccak_X4_Initialize_ptr = &KeccakP1600times4_InitializeAll_serial;
		Keccak_X4_AddByte_ptr = &KeccakP1600times4_AddByte_serial;
		Keccak_X4_AddBytes_ptr = &KeccakP1600times4_AddBytes_serial;
		Keccak_X4_Permute_ptr = &KeccakP1600times4_PermuteAll_24rounds_serial;
		Keccak_X4_ExtractBytes_ptr = &KeccakP1600times4_ExtractBytes_serial;
	}
#else // Windows
	Keccak_X4_Initialize_ptr = &KeccakP1600times4_InitializeAll_serial;
	Keccak_X4_AddByte_ptr = &KeccakP1600times4_AddByte_serial;
	Keccak_X4_AddBytes_ptr = &KeccakP1600times4_AddBytes_serial;
	Keccak_X4_Permute_ptr = &KeccakP1600times4_PermuteAll_24rounds_serial;
	Keccak_X4_ExtractBytes_ptr = &KeccakP1600times4_ExtractBytes_serial;
#endif
#else
	Keccak_X4_Initialize_ptr = &KeccakP1600times4_InitializeAll;
	Keccak_X4_AddByte_ptr = &KeccakP1600times4_AddByte;
	Keccak_X4_AddBytes_ptr = &KeccakP1600times4_AddBytes;
	Keccak_X4_Permute_ptr = &KeccakP1600times4_PermuteAll_24rounds;
	Keccak_X4_ExtractBytes_ptr = &KeccakP1600times4_ExtractBytes;
#endif

	(*Keccak_X4_Initialize_ptr)(state);
}

static void keccak_x4_inc_reset(uint64_t *s) {
	(*Keccak_X4_Initialize_ptr)(s);
	s[100] = 0;
}

static void keccak_x4_inc_absorb(uint64_t *s, uint32_t r,
                                 const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	uint64_t c = r - s[100];

	if (s[100] && inlen >= c) {
		(*Keccak_X4_AddBytes_ptr)(s, 0, in0, (unsigned int)s[100], (unsigned int)c);
		(*Keccak_X4_AddBytes_ptr)(s, 1, in1, (unsigned int)s[100], (unsigned int)c);
		(*Keccak_X4_AddBytes_ptr)(s, 2, in2, (unsigned int)s[100], (unsigned int)c);
		(*Keccak_X4_AddBytes_ptr)(s, 3, in3, (unsigned int)s[100], (unsigned int)c);
		(*Keccak_X4_Permute_ptr)(s);
		inlen -= c;
		in0 += c;
		in1 += c;
		in2 += c;
		in3 += c;
		s[100] = 0;
	}

	while (inlen >= r) {
		(*Keccak_X4_AddBytes_ptr)(s, 0, in0, 0, (unsigned int)r);
		(*Keccak_X4_AddBytes_ptr)(s, 1, in1, 0, (unsigned int)r);
		(*Keccak_X4_AddBytes_ptr)(s, 2, in2, 0, (unsigned int)r);
		(*Keccak_X4_AddBytes_ptr)(s, 3, in3, 0, (unsigned int)r);
		(*Keccak_X4_Permute_ptr)(s);
		inlen -= r;
		in0 += r;
		in1 += r;
		in2 += r;
		in3 += r;
	}

	(*Keccak_X4_AddBytes_ptr)(s, 0, in0, (unsigned int)s[100], (unsigned int)inlen);
	(*Keccak_X4_AddBytes_ptr)(s, 1, in1, (unsigned int)s[100], (unsigned int)inlen);
	(*Keccak_X4_AddBytes_ptr)(s, 2, in2, (unsigned int)s[100], (unsigned int)inlen);
	(*Keccak_X4_AddBytes_ptr)(s, 3, in3, (unsigned int)s[100], (unsigned int)inlen);
	s[100] += inlen;
}

static void keccak_x4_inc_finalize(uint64_t *s, uint32_t r, uint8_t p) {
	(*Keccak_X4_AddByte_ptr)(s, 0, p, (unsigned int)s[100]);
	(*Keccak_X4_AddByte_ptr)(s, 1, p, (unsigned int)s[100]);
	(*Keccak_X4_AddByte_ptr)(s, 2, p, (unsigned int)s[100]);
	(*Keccak_X4_AddByte_ptr)(s, 3, p, (unsigned int)s[100]);

	(*Keccak_X4_AddByte_ptr)(s, 0, 0x80, (unsigned int)(r - 1));
	(*Keccak_X4_AddByte_ptr)(s, 1, 0x80, (unsigned int)(r - 1));
	(*Keccak_X4_AddByte_ptr)(s, 2, 0x80, (unsigned int)(r - 1));
	(*Keccak_X4_AddByte_ptr)(s, 3, 0x80, (unsigned int)(r - 1));

	s[100] = 0;
}

static void keccak_x4_inc_squeeze(uint8_t *out0, uint8_t *out1, uint8_t *out2, uint8_t *out3,
                                  size_t outlen, uint64_t *s, uint32_t r) {

	while (outlen > s[100]) {
		(*Keccak_X4_ExtractBytes_ptr)(s, 0, out0, (unsigned int)(r - s[100]), (unsigned int)s[100]);
		(*Keccak_X4_ExtractBytes_ptr)(s, 1, out1, (unsigned int)(r - s[100]), (unsigned int)s[100]);
		(*Keccak_X4_ExtractBytes_ptr)(s, 2, out2, (unsigned int)(r - s[100]), (unsigned int)s[100]);
		(*Keccak_X4_ExtractBytes_ptr)(s, 3, out3, (unsigned int)(r - s[100]), (unsigned int)s[100]);
		(*Keccak_X4_Permute_ptr)(s);
		out0 += s[100];
		out1 += s[100];
		out2 += s[100];
		out3 += s[100];
		outlen -= s[100];
		s[100] = r;
	}

	(*Keccak_X4_ExtractBytes_ptr)(s, 0, out0, (unsigned int)(r - s[100]), (unsigned int)outlen);
	(*Keccak_X4_ExtractBytes_ptr)(s, 1, out1, (unsigned int)(r - s[100]), (unsigned int)outlen);
	(*Keccak_X4_ExtractBytes_ptr)(s, 2, out2, (unsigned int)(r - s[100]), (unsigned int)outlen);
	(*Keccak_X4_ExtractBytes_ptr)(s, 3, out3, (unsigned int)(r - s[100]), (unsigned int)outlen);

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
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_X4_CTX_ALIGNMENT, KECCAK_X4_CTX_BYTES);
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
	memcpy(dest->ctx, src->ctx, KECCAK_X4_CTX_BYTES);
}

void OQS_SHA3_shake128_x4_inc_ctx_release(OQS_SHA3_shake128_x4_inc_ctx *state) {
	OQS_MEM_aligned_free(state->ctx);
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
	state->ctx = OQS_MEM_aligned_alloc(KECCAK_X4_CTX_ALIGNMENT, KECCAK_X4_CTX_BYTES);
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
	memcpy(dest->ctx, src->ctx, KECCAK_X4_CTX_BYTES);
}

void OQS_SHA3_shake256_x4_inc_ctx_release(OQS_SHA3_shake256_x4_inc_ctx *state) {
	OQS_MEM_aligned_free(state->ctx);
}

void OQS_SHA3_shake256_x4_inc_ctx_reset(OQS_SHA3_shake256_x4_inc_ctx *state) {
	keccak_x4_inc_reset((uint64_t *)state->ctx);
}

