// SPDX-License-Identifier: Public domain
// Based on public domain code by Romain Dolbeau
// http://dolbeau.name/dolbeau/crypto/crypto.html


#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <oqs/common.h>

#include <wmmintrin.h>
#include <tmmintrin.h>

#define AES_BLOCKBYTES 16

typedef struct {
	__m128i sk_exp[15];
	__m128i iv;
} aes256ctx;

#define BE_TO_UINT32(n) (uint32_t)((((uint8_t *) &(n))[0] << 24) | (((uint8_t *) &(n))[1] << 16) | (((uint8_t *) &(n))[2] << 8) | (((uint8_t *) &(n))[3] << 0))

// From crypto_core/aes256encrypt/dolbeau/aesenc-int
static inline void aes256ni_setkey_encrypt(const unsigned char *key, __m128i rkeys[15]) {
	__m128i key0 = _mm_loadu_si128((const __m128i *)(key + 0));
	__m128i key1 = _mm_loadu_si128((const __m128i *)(key + 16));
	__m128i temp0, temp1, temp2, temp4;
	int idx = 0;

	rkeys[idx++] = key0;
	temp0 = key0;
	temp2 = key1;

	/* blockshift-based block by Cedric Bourrasset & Romain Dolbeau */
#define BLOCK1(IMM)                                \
    temp1 = _mm_aeskeygenassist_si128(temp2, IMM); \
    rkeys[idx++] = temp2;                          \
    temp4 = _mm_slli_si128(temp0,4);               \
    temp0 = _mm_xor_si128(temp0,temp4);            \
    temp4 = _mm_slli_si128(temp0,8);               \
    temp0 = _mm_xor_si128(temp0,temp4);            \
    temp1 = _mm_shuffle_epi32(temp1,0xff);         \
    temp0 = _mm_xor_si128(temp0,temp1)

#define BLOCK2(IMM)                                \
    temp1 = _mm_aeskeygenassist_si128(temp0, IMM); \
    rkeys[idx++] = temp0;                          \
    temp4 = _mm_slli_si128(temp2,4);               \
    temp2 = _mm_xor_si128(temp2,temp4);            \
    temp4 = _mm_slli_si128(temp2,8);               \
    temp2 = _mm_xor_si128(temp2,temp4);            \
    temp1 = _mm_shuffle_epi32(temp1,0xaa);         \
    temp2 = _mm_xor_si128(temp2,temp1)

	BLOCK1(0x01);
	BLOCK2(0x01);

	BLOCK1(0x02);
	BLOCK2(0x02);

	BLOCK1(0x04);
	BLOCK2(0x04);

	BLOCK1(0x08);
	BLOCK2(0x08);

	BLOCK1(0x10);
	BLOCK2(0x10);

	BLOCK1(0x20);
	BLOCK2(0x20);

	BLOCK1(0x40);
	rkeys[idx++] = temp0;
}

void oqs_aes256_load_schedule_ni(const uint8_t *key, void **_schedule) {
	*_schedule = malloc(sizeof(aes256ctx));
	OQS_EXIT_IF_NULLPTR(*_schedule, "AES");
	assert(*_schedule != NULL);
	__m128i *schedule = ((aes256ctx *) *_schedule)->sk_exp;
	aes256ni_setkey_encrypt(key, schedule);
}

void oqs_aes256_load_iv_ni(const uint8_t *iv, size_t iv_len, void *_schedule) {
	aes256ctx *ctx = _schedule;
	__m128i idx = _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1, 0);
	if (iv_len == 12) {
		const int32_t *ivi = (const int32_t *) iv;
		ctx->iv = _mm_shuffle_epi8(_mm_set_epi32(0, ivi[2], ivi[1], ivi[0]), idx);
	} else if (iv_len == 16) {
		ctx->iv = _mm_shuffle_epi8(_mm_loadu_si128((const __m128i *)iv), idx);
	} else {
		exit(EXIT_FAILURE);
	}
}

void oqs_aes256_load_iv_u64_ni(uint64_t iv, void *_schedule) {
	aes256ctx *ctx = _schedule;
	ctx->iv = _mm_loadl_epi64((__m128i *)&iv);
}

void oqs_aes256_free_schedule_ni(void *schedule) {
	if (schedule != NULL) {
		OQS_MEM_secure_free(schedule, sizeof(aes256ctx));
	}
}


// Modified from crypto_core/aes256encrypt/dolbeau/aesenc-int
static inline void aes256ni_encrypt(const __m128i rkeys[15], __m128i nv, unsigned char *out) {
	__m128i temp = _mm_xor_si128(nv, rkeys[0]);
	temp = _mm_aesenc_si128(temp, rkeys[1]);
	temp = _mm_aesenc_si128(temp, rkeys[2]);
	temp = _mm_aesenc_si128(temp, rkeys[3]);
	temp = _mm_aesenc_si128(temp, rkeys[4]);
	temp = _mm_aesenc_si128(temp, rkeys[5]);
	temp = _mm_aesenc_si128(temp, rkeys[6]);
	temp = _mm_aesenc_si128(temp, rkeys[7]);
	temp = _mm_aesenc_si128(temp, rkeys[8]);
	temp = _mm_aesenc_si128(temp, rkeys[9]);
	temp = _mm_aesenc_si128(temp, rkeys[10]);
	temp = _mm_aesenc_si128(temp, rkeys[11]);
	temp = _mm_aesenc_si128(temp, rkeys[12]);
	temp = _mm_aesenc_si128(temp, rkeys[13]);
	temp = _mm_aesenclast_si128(temp, rkeys[14]);
	_mm_storeu_si128((__m128i *)(out), temp);
}

// 4x interleaved encryption
static inline void aes256ni_encrypt_x4(const __m128i rkeys[15], __m128i n0, __m128i n1, __m128i n2, __m128i n3, unsigned char *out) {
	__m128i temp0 = _mm_xor_si128(n0, rkeys[0]);
	__m128i temp1 = _mm_xor_si128(n1, rkeys[0]);
	__m128i temp2 = _mm_xor_si128(n2, rkeys[0]);
	__m128i temp3 = _mm_xor_si128(n3, rkeys[0]);

#define AESNENCX4(IDX) \
    temp0 = _mm_aesenc_si128(temp0, rkeys[IDX]); \
    temp1 = _mm_aesenc_si128(temp1, rkeys[IDX]); \
    temp2 = _mm_aesenc_si128(temp2, rkeys[IDX]); \
    temp3 = _mm_aesenc_si128(temp3, rkeys[IDX])

	AESNENCX4(1);
	AESNENCX4(2);
	AESNENCX4(3);
	AESNENCX4(4);
	AESNENCX4(5);
	AESNENCX4(6);
	AESNENCX4(7);
	AESNENCX4(8);
	AESNENCX4(9);
	AESNENCX4(10);
	AESNENCX4(11);
	AESNENCX4(12);
	AESNENCX4(13);

	temp0 = _mm_aesenclast_si128(temp0, rkeys[14]);
	temp1 = _mm_aesenclast_si128(temp1, rkeys[14]);
	temp2 = _mm_aesenclast_si128(temp2, rkeys[14]);
	temp3 = _mm_aesenclast_si128(temp3, rkeys[14]);

	_mm_storeu_si128((__m128i *)(out + 0), temp0);
	_mm_storeu_si128((__m128i *)(out + 16), temp1);
	_mm_storeu_si128((__m128i *)(out + 32), temp2);
	_mm_storeu_si128((__m128i *)(out + 48), temp3);
}

void oqs_aes256_enc_sch_block_ni(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const __m128i *schedule = ((const aes256ctx *) _schedule)->sk_exp;
	aes256ni_encrypt(schedule, _mm_loadu_si128((const __m128i *)plaintext), ciphertext);
}

void oqs_aes256_ecb_enc_sch_ni(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		oqs_aes256_enc_sch_block_ni(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}

void oqs_aes256_ctr_enc_sch_upd_blks_ni(void *schedule, uint8_t *out, size_t out_blks) {
	aes256ctx *ctx = (aes256ctx *) schedule;
	const __m128i mask = _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1, 0);

	while (out_blks >= 4) {
		__m128i nv0 = _mm_shuffle_epi8(ctx->iv, mask);
		__m128i nv1 = _mm_shuffle_epi8(_mm_add_epi64(ctx->iv, _mm_set_epi64x(1, 0)), mask);
		__m128i nv2 = _mm_shuffle_epi8(_mm_add_epi64(ctx->iv, _mm_set_epi64x(2, 0)), mask);
		__m128i nv3 = _mm_shuffle_epi8(_mm_add_epi64(ctx->iv, _mm_set_epi64x(3, 0)), mask);
		aes256ni_encrypt_x4(schedule, nv0, nv1, nv2, nv3, out);
		ctx->iv = _mm_add_epi64(ctx->iv, _mm_set_epi64x(4, 0));
		out += 64;
		out_blks -= 4;
	}
	while (out_blks >= 1) {
		__m128i nv0 = _mm_shuffle_epi8(ctx->iv, mask);
		aes256ni_encrypt(schedule, nv0, out);
		ctx->iv = _mm_add_epi64(ctx->iv, _mm_set_epi64x(1, 0));
		out += 16;
		out_blks--;
	}
}

void oqs_aes256_ctr_enc_sch_ni(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	__m128i block;
	__m128i mask = _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1, 0);
	if (iv_len == 12) {
		const int32_t *ivi = (const int32_t *) iv;
		block = _mm_set_epi32(0, ivi[2], ivi[1], ivi[0]);
	} else if (iv_len == 16) {
		block = _mm_loadu_si128((const __m128i *)iv);
	} else {
		exit(EXIT_FAILURE);
	}

	while (out_len >= 64) {
		__m128i nv0 = block;
		__m128i nv1 = _mm_shuffle_epi8(_mm_add_epi64(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(1, 0)), mask);
		__m128i nv2 = _mm_shuffle_epi8(_mm_add_epi64(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(2, 0)), mask);
		__m128i nv3 = _mm_shuffle_epi8(_mm_add_epi64(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(3, 0)), mask);
		aes256ni_encrypt_x4(schedule, nv0, nv1, nv2, nv3, out);
		block = _mm_shuffle_epi8(_mm_add_epi64(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(4, 0)), mask);
		out += 64;
		out_len -= 64;
	}
	while (out_len >= 16) {
		aes256ni_encrypt(schedule, block, out);
		out += 16;
		out_len -= 16;
		block = _mm_shuffle_epi8(_mm_add_epi64(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(1, 0)), mask);
	}
	if (out_len > 0) {
		uint8_t tmp[16];
		aes256ni_encrypt(schedule, block, tmp);
		memcpy(out, tmp, out_len);
	}
}
