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

typedef struct {
	__m128i sk_exp[11];
	__m128i iv;
} aes128ctx;

// From crypto_core/aes128ncrypt/dolbeau/aesenc-int
static inline void aes128ni_setkey_encrypt(const unsigned char *key, __m128i rkeys[11]) {
	__m128i key0 = _mm_loadu_si128((const __m128i *)(key + 0));
	__m128i temp0, temp1, temp4;
	int idx = 0;

	temp0 = key0;

	/* blockshift-based block by Cedric Bourrasset */
#define BLOCK1(IMM)                                       \
    temp1 = _mm_aeskeygenassist_si128(temp0, IMM);        \
    rkeys[idx++] = temp0;                                 \
    temp4 = _mm_slli_si128(temp0,4);                      \
    temp0 = _mm_xor_si128(temp0,temp4);                   \
    temp4 = _mm_slli_si128(temp0,8);                      \
    temp0 = _mm_xor_si128(temp0,temp4);                   \
    temp1 = _mm_shuffle_epi32(temp1,0xff);                \
    temp0 = _mm_xor_si128(temp0,temp1)

	BLOCK1(0x01);
	BLOCK1(0x02);
	BLOCK1(0x04);
	BLOCK1(0x08);
	BLOCK1(0x10);
	BLOCK1(0x20);
	BLOCK1(0x40);
	BLOCK1(0x80);
	BLOCK1(0x1b);
	BLOCK1(0x36);
	rkeys[idx++] = temp0;
}

void oqs_aes128_load_schedule_ni(const uint8_t *key, void **_schedule) {
	*_schedule = OQS_MEM_malloc(sizeof(aes128ctx));
	OQS_EXIT_IF_NULLPTR(*_schedule, "AES");
	assert(*_schedule != NULL);
	__m128i *schedule = ((aes128ctx *) *_schedule)->sk_exp;
	aes128ni_setkey_encrypt(key, schedule);
}

void oqs_aes128_load_iv_ni(const uint8_t *iv, size_t iv_len, void *_schedule) {
	aes128ctx *ctx = _schedule;
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

void oqs_aes128_load_iv_u64_ni(uint64_t iv, void *_schedule) {
	aes128ctx *ctx = _schedule;
	ctx->iv = _mm_loadl_epi64((__m128i *)&iv);
}

void oqs_aes128_free_schedule_ni(void *schedule) {
	if (schedule != NULL) {
		OQS_MEM_secure_free(schedule, sizeof(aes128ctx));
	}
}

// From crypto_core/aes128encrypt/dolbeau/aesenc-int
static inline void aes128ni_encrypt(const __m128i rkeys[11], __m128i nv, unsigned char *out) {
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
	temp = _mm_aesenclast_si128(temp, rkeys[10]);
	_mm_storeu_si128((__m128i *)(out), temp);
}

// 4x interleaved encryption
static inline void aes128ni_encrypt_x4(const __m128i rkeys[11], __m128i n0,
                                       __m128i n1, __m128i n2, __m128i n3,
                                       unsigned char *out) {
	__m128i temp0 = _mm_xor_si128(n0, rkeys[0]);
	__m128i temp1 = _mm_xor_si128(n1, rkeys[0]);
	__m128i temp2 = _mm_xor_si128(n2, rkeys[0]);
	__m128i temp3 = _mm_xor_si128(n3, rkeys[0]);

#define AESNENCX4(IDX)                                                         \
  temp0 = _mm_aesenc_si128(temp0, rkeys[IDX]);                                 \
  temp1 = _mm_aesenc_si128(temp1, rkeys[IDX]);                                 \
  temp2 = _mm_aesenc_si128(temp2, rkeys[IDX]);                                 \
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

	temp0 = _mm_aesenclast_si128(temp0, rkeys[10]);
	temp1 = _mm_aesenclast_si128(temp1, rkeys[10]);
	temp2 = _mm_aesenclast_si128(temp2, rkeys[10]);
	temp3 = _mm_aesenclast_si128(temp3, rkeys[10]);

	_mm_storeu_si128((__m128i *)(out + 0), temp0);
	_mm_storeu_si128((__m128i *)(out + 16), temp1);
	_mm_storeu_si128((__m128i *)(out + 32), temp2);
	_mm_storeu_si128((__m128i *)(out + 48), temp3);
}

void oqs_aes128_enc_sch_block_ni(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const __m128i *schedule = ((const aes128ctx *) _schedule)->sk_exp;
	aes128ni_encrypt(schedule, _mm_loadu_si128((const __m128i *)plaintext), ciphertext);
}

void oqs_aes128_ecb_enc_sch_ni(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		oqs_aes128_enc_sch_block_ni(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}

void oqs_aes128_ctr_enc_sch_upd_blks_ni(void *schedule, uint8_t *out, size_t out_blks) {
	aes128ctx *ctx = (aes128ctx *) schedule;
	const __m128i mask = _mm_set_epi8(8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1, 0);

	while (out_blks >= 4) {
		__m128i nv0 = _mm_shuffle_epi8(ctx->iv, mask);
		__m128i nv1 = _mm_shuffle_epi8(_mm_add_epi32(ctx->iv, _mm_set_epi64x(1, 0)), mask);
		__m128i nv2 = _mm_shuffle_epi8(_mm_add_epi32(ctx->iv, _mm_set_epi64x(2, 0)), mask);
		__m128i nv3 = _mm_shuffle_epi8(_mm_add_epi32(ctx->iv, _mm_set_epi64x(3, 0)), mask);
		aes128ni_encrypt_x4(schedule, nv0, nv1, nv2, nv3, out);
		ctx->iv = _mm_add_epi32(ctx->iv, _mm_set_epi64x(4, 0));
		out += 64;
		out_blks -= 4;
	}
	while (out_blks >= 1) {
		__m128i nv0 = _mm_shuffle_epi8(ctx->iv, mask);
		aes128ni_encrypt(schedule, nv0, out);
		ctx->iv = _mm_add_epi32(ctx->iv, _mm_set_epi64x(1, 0));
		out += 16;
		out_blks--;
	}
}

void oqs_aes128_ctr_enc_sch_ni(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
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
		__m128i nv1 = _mm_shuffle_epi8(_mm_add_epi32(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(1, 0)), mask);
		__m128i nv2 = _mm_shuffle_epi8(_mm_add_epi32(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(2, 0)), mask);
		__m128i nv3 = _mm_shuffle_epi8(_mm_add_epi32(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(3, 0)), mask);
		aes128ni_encrypt_x4(schedule, nv0, nv1, nv2, nv3, out);
		block = _mm_shuffle_epi8(_mm_add_epi32(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(4, 0)), mask);
		out += 64;
		out_len -= 64;
	}
	while (out_len >= 16) {
		aes128ni_encrypt(schedule, block, out);
		out += 16;
		out_len -= 16;
		block = _mm_shuffle_epi8(_mm_add_epi32(_mm_shuffle_epi8(block, mask), _mm_set_epi64x(1, 0)), mask);
	}
	if (out_len > 0) {
		uint8_t tmp[16];
		aes128ni_encrypt(schedule, block, tmp);
		memcpy(out, tmp, out_len);
	}
}
