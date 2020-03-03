/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker and Shay Gueron
 * AWS Cryptographic Algorithms Group.
 * (ndrucker@amazon.com, gueron@amazon.com)
 *
 * The rotation functions are based on the Barrel shifter described in [1]
 * and some modifed snippet from [2]
 * [1] Chou, T.: QcBits: Constant-Time Small-Key Code-Based Cryptography.
 *     In: Gier-lichs, B., Poschmann, A.Y. (eds.) Cryptographic Hardware
 *     and Embedded Systems– CHES 2016. pp. 280–300. Springer Berlin Heidelberg,
 *     Berlin, Heidelberg (2016)
 *
 * [2] Guimarães, Antonio, Diego F Aranha, and Edson Borin. 2019.
 *     “Optimized Implementation of QC-MDPC Code-Based Cryptography.”
 *     Concurrency and Computation: Practice and Experience 31 (18):
 *     e5089. https://doi.org/10.1002/cpe.5089.
 */

#include "decode.h"
#include "utilities.h"
#include <immintrin.h>

#define R_ZMM_HALF_LOG2 UPTOPOW2(R_ZMM / 2)

_INLINE_ void
rotate512_big(OUT syndrome_t *out, IN const syndrome_t *in, size_t zmm_num) {
	// For preventing overflows (comparison in bytes)
	bike_static_assert(sizeof(*out) > (ZMM_SIZE * (R_ZMM + (2 * R_ZMM_HALF_LOG2))),
	                   rotr_big_err);
	memcpy(out, in, sizeof(*in));

	for (uint32_t idx = R_ZMM_HALF_LOG2; idx >= 1; idx >>= 1) {
		const uint8_t mask = secure_l32_mask(zmm_num, idx);
		zmm_num            = zmm_num - (idx & mask);

		for (size_t i = 0; i < (R_ZMM + idx); i++) {
			const __m512i a = _mm512_loadu_si512(&out->qw[8 * (i + idx)]);
			_mm512_mask_storeu_epi64(&out->qw[8 * i], mask, a);
		}
	}
}

// The rotate512_small funciton is a derivative of the code described in [1]
_INLINE_ void
rotate512_small(OUT syndrome_t *out, IN const syndrome_t *in, size_t bitscount) {
	__m512i       previous     = _mm512_setzero_si512();
	const int     count64      = (int)bitscount & 0x3f;
	const __m512i count64_512  = _mm512_set1_epi64(count64);
	const __m512i count64_512r = _mm512_set1_epi64((int)64 - count64);

	const __m512i num_full_qw = _mm512_set1_epi8(bitscount >> 6);
	const __m512i one         = _mm512_set1_epi64(1);
	__m512i       a0, a1;

	__m512i idx = _mm512_setr_epi64(0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);

	// Positions above 7 are taken from the second register in
	// _mm512_permutex2var_epi64
	idx          = _mm512_add_epi64(idx, num_full_qw);
	__m512i idx1 = _mm512_add_epi64(idx, one);

	for (int i = R_ZMM; i >= 0; i--) {
		// Load the next 512 bits
		const __m512i in512 = _mm512_loadu_si512(&in->qw[8 * i]);

		// Rotate the current and previous 512 registers so that their quadwords
		// will be in the right positions.
		a0 = _mm512_permutex2var_epi64(in512, idx, previous);
		a1 = _mm512_permutex2var_epi64(in512, idx1, previous);

		a0 = _mm512_srlv_epi64(a0, count64_512);
		a1 = _mm512_sllv_epi64(a1, count64_512r);

		// Shift less than 64 (quadwords internal)
		const __m512i out512 = _mm512_or_si512(a0, a1);

		// Store the rotated value
		_mm512_storeu_si512(&out->qw[8 * i], out512);
		previous = in512;
	}
}

void
rotate_right(OUT syndrome_t *out,
             IN const syndrome_t *in,
             IN const uint32_t    bitscount) {
	// 1) Rotate in granularity of 512 bits blocks using ZMMs
	rotate512_big(out, in, (bitscount / 512));
	// 2) Rotate in smaller granularity (less than 512 bits) using ZMMs
	rotate512_small(out, out, (bitscount % 512));
}
