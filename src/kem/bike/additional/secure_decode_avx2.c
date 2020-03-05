/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker and Shay Gueron
 * AWS Cryptographic Algorithms Group.
 * (ndrucker@amazon.com, gueron@amazon.com)
 *
 * The rotate functions are based on the Barrel shifter described in [1] and
 * some code snippets from [2]:
 *
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

#define R_YMM_HALF_LOG2 UPTOPOW2(R_YMM / 2)

_INLINE_ void
rotate256_big(OUT syndrome_t *out, IN const syndrome_t *in, IN size_t ymm_num) {
	// For preventing overflows (comparison in bytes)
	bike_static_assert(sizeof(*out) > (YMM_SIZE * (R_YMM + (2 * R_YMM_HALF_LOG2))),
	                   rotr_big_err);

	const __m256i all_one_mask = _mm256_set1_epi8(-1);
	memcpy(out, in, sizeof(*in));

	for (uint32_t idx = R_YMM_HALF_LOG2; idx >= 1; idx >>= 1) {
		const uint8_t mask       = secure_l32_mask(ymm_num, idx);
		const __m256i blend_mask = _mm256_set1_epi8(mask);
		ymm_num                  = ymm_num - (idx & mask);

		for (size_t i = 0; i < (R_YMM + idx); i++) {
			// AVX2 does not have masks so we must use blend.
			// We use _mm256_maskload_epi32 instead of _mm256_loadu_si256 to avoid
			// casting alignment issues (from 64 bits to 256)
			const __m256i a = _mm256_maskload_epi32(
			                      (const int *)&out->qw[4 * (i + idx)], all_one_mask);
			__m256i b =
			    _mm256_maskload_epi32((const int *)&out->qw[4 * i], all_one_mask);
			b = _mm256_blendv_epi8(b, a, blend_mask);
			_mm256_maskstore_epi32((int *)&out->qw[4 * i], all_one_mask, b);
		}
	}
}

_INLINE_ void
rotate256_small(OUT syndrome_t *out, IN const syndrome_t *in, size_t count) {
	__m256i        carry_in   = _mm256_setzero_si256();
	const int      count64    = (int)count & 0x3f;
	const uint64_t count_mask = (count >> 5) & 0xe;

	__m256i       idx = _mm256_setr_epi32(0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7);
	const __m256i zero_mask    = _mm256_setr_epi64x(0, -1, -1, -1);
	const __m256i all_one_mask = _mm256_set1_epi8(-1);
	const __m256i count_vet    = _mm256_set1_epi8(count_mask);

	ALIGN(32)
	const uint8_t zero_mask2_buf[] = {
		0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x84, 0x84, 0x84,
		0x84, 0x84, 0x84, 0x84, 0x84, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82,
		0x82, 0x82, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
	};
	__m256i zero_mask2 = _mm256_loadu_si256((const __m256i *)zero_mask2_buf);

	zero_mask2 = _mm256_sub_epi8(zero_mask2, count_vet);
	idx        = _mm256_add_epi8(idx, count_vet);

	for (int i = R_YMM; i >= 0; i--) {
		// Load the next 256 bits
		__m256i in256 =
		    _mm256_maskload_epi32((const int *)&in->qw[4 * i], all_one_mask);

		// Rotate the current and previous 256 registers so that their quadwords
		// will be in the right positions.
		__m256i carry_out = _mm256_permutevar8x32_epi32(in256, idx);
		in256             = _mm256_blendv_epi8(carry_in, carry_out, zero_mask2);

		// Shift less than 64 (quadwords internal)
		__m256i inner_carry = _mm256_blendv_epi8(carry_in, in256, zero_mask);
		inner_carry         = _mm256_permute4x64_epi64(inner_carry, 0x39);
		const __m256i out256 =
		    _mm256_or_si256(_mm256_srli_epi64(in256, count64),
		                    _mm256_slli_epi64(inner_carry, (int)64 - count64));

		// Store the rotated value
		_mm256_maskstore_epi32((int *)&out->qw[4 * i], all_one_mask, out256);
		carry_in = carry_out;
	}
}

void
rotate_right(OUT syndrome_t *out,
             IN const syndrome_t *in,
             IN const uint32_t    bitscount) {
	// 1) Rotate in granularity of 256 bits blocks using YMMs
	rotate256_big(out, in, (bitscount / 256));
	// 2) Rotate in smaller granularity (less than 256 bits) using YMMs
	rotate256_small(out, out, (bitscount % 256));
}
