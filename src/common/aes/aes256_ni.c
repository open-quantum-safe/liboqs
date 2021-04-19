// SPDX-License-Identifier: Public domain
// Based on public domain code by Romain Dolbeau
// http://dolbeau.name/dolbeau/crypto/crypto.html


#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <oqs/common.h>

#include <wmmintrin.h>

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
	*_schedule = malloc(15 * sizeof(__m128i));
	assert(*_schedule != NULL);
	__m128i *schedule = (__m128i *) *_schedule;
	aes256ni_setkey_encrypt(key, schedule);
}

void oqs_aes256_free_schedule_ni(void *schedule) {
	if (schedule != NULL) {
		OQS_MEM_secure_free(schedule, 15 * sizeof(__m128i));
	}
}

// From crypto_core/aes256encrypt/dolbeau/aesenc-int
static inline void aes256ni_encrypt(const __m128i rkeys[15], const unsigned char *n, unsigned char *out) {
	__m128i nv = _mm_loadu_si128((const __m128i *)n);
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

void oqs_aes256_enc_sch_block_ni(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const __m128i *schedule = (const __m128i *) _schedule;
	aes256ni_encrypt(schedule, plaintext, ciphertext);
}

void oqs_aes256_ecb_enc_sch_ni(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		oqs_aes256_enc_sch_block_ni(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}
