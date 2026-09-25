// SPDX-License-Identifier: Public domain
// Based on public domain code by Romain Dolbeau
// http://dolbeau.name/dolbeau/crypto/crypto.html
// AES-192 key expansion follows the Intel AES-NI sample (13 round keys).

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <oqs/common.h>

#include <wmmintrin.h>
#include <tmmintrin.h>

typedef struct {
	__m128i sk_exp[13];
	__m128i iv;
} aes192ctx;

static inline void KEY_192_ASSIST(__m128i *temp1, __m128i *temp2, __m128i *temp3) {
	__m128i temp4;
	*temp2 = _mm_shuffle_epi32(*temp2, 0x55);
	temp4 = _mm_slli_si128(*temp1, 0x4);
	*temp1 = _mm_xor_si128(*temp1, temp4);
	temp4 = _mm_slli_si128(temp4, 0x4);
	*temp1 = _mm_xor_si128(*temp1, temp4);
	temp4 = _mm_slli_si128(temp4, 0x4);
	*temp1 = _mm_xor_si128(*temp1, temp4);
	*temp1 = _mm_xor_si128(*temp1, *temp2);
	*temp2 = _mm_shuffle_epi32(*temp1, 0xff);
	temp4 = _mm_slli_si128(*temp3, 0x4);
	*temp3 = _mm_xor_si128(*temp3, temp4);
	*temp3 = _mm_xor_si128(*temp3, *temp2);
}

#define SHUFFLE_PD(a, b, imm) \
    _mm_castpd_si128(_mm_shuffle_pd(_mm_castsi128_pd(a), _mm_castsi128_pd(b), imm))

static inline void aes192ni_setkey_encrypt(const unsigned char *key, __m128i rkeys[13]) {
	__m128i temp1 = _mm_loadu_si128((const __m128i *)(key + 0));
	/* Load the remaining 8 key bytes without reading past the 24-byte key. */
	__m128i temp3 = _mm_loadl_epi64((const __m128i *)(key + 16));
	__m128i temp2;

	rkeys[0] = temp1;
	rkeys[1] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x1);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	rkeys[1] = SHUFFLE_PD(rkeys[1], temp1, 0);
	rkeys[2] = SHUFFLE_PD(temp1, temp3, 1);
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x2);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	rkeys[3] = temp1;
	rkeys[4] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x4);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	rkeys[4] = SHUFFLE_PD(rkeys[4], temp1, 0);
	rkeys[5] = SHUFFLE_PD(temp1, temp3, 1);
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x8);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	rkeys[6] = temp1;
	rkeys[7] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x10);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	rkeys[7] = SHUFFLE_PD(rkeys[7], temp1, 0);
	rkeys[8] = SHUFFLE_PD(temp1, temp3, 1);
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x20);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	rkeys[9] = temp1;
	rkeys[10] = temp3;
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x40);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	rkeys[10] = SHUFFLE_PD(rkeys[10], temp1, 0);
	rkeys[11] = SHUFFLE_PD(temp1, temp3, 1);
	temp2 = _mm_aeskeygenassist_si128(temp3, 0x80);
	KEY_192_ASSIST(&temp1, &temp2, &temp3);
	rkeys[12] = temp1;
}

void oqs_aes192_load_schedule_ni(const uint8_t *key, void **_schedule) {
	*_schedule = OQS_MEM_malloc(sizeof(aes192ctx));
	OQS_EXIT_IF_NULLPTR(*_schedule, "AES");
	assert(*_schedule != NULL);
	__m128i *schedule = ((aes192ctx *) *_schedule)->sk_exp;
	aes192ni_setkey_encrypt(key, schedule);
}

void oqs_aes192_free_schedule_ni(void *schedule) {
	if (schedule != NULL) {
		OQS_MEM_secure_free(schedule, sizeof(aes192ctx));
	}
}

static inline void aes192ni_encrypt(const __m128i rkeys[13], __m128i nv, unsigned char *out) {
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
	temp = _mm_aesenclast_si128(temp, rkeys[12]);
	_mm_storeu_si128((__m128i *)(out), temp);
}

void oqs_aes192_enc_sch_block_ni(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const __m128i *schedule = ((const aes192ctx *) _schedule)->sk_exp;
	aes192ni_encrypt(schedule, _mm_loadu_si128((const __m128i *)plaintext), ciphertext);
}

void oqs_aes192_ecb_enc_sch_ni(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		oqs_aes192_enc_sch_block_ni(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}
