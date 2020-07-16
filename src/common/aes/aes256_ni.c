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
	__m128i key0 = _mm_loadu_si128((const __m128i_u *)(key + 0));
	__m128i key1 = _mm_loadu_si128((const __m128i_u *)(key + 16));
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

// From crypto_core/aes256decrypt/dolbeau/aesenc-int
static inline void aes256ni_setkey_decrypt(const unsigned char *key, __m128i rkeys[15]) {
	__m128i tkeys[15];
	int i;
	aes256ni_setkey_encrypt(key, tkeys);
	rkeys[0] = tkeys[14];
#pragma unroll(13)
	for (i = 1 ; i < 14 ; i++) {
		rkeys[i] = _mm_aesimc_si128(tkeys[14 - i]);
	}
	rkeys[14] = tkeys[0];
}

void oqs_aes256_load_schedule_ni(const uint8_t *key, void **_schedule, int for_encryption) {
	*_schedule = malloc(15 * sizeof(__m128i));
	assert(*_schedule != NULL);
	__m128i *schedule = (__m128i *) *_schedule;
	if (for_encryption) {
		aes256ni_setkey_encrypt(key, schedule);
	} else {
		aes256ni_setkey_decrypt(key, schedule);
	}
}

void oqs_aes256_free_schedule_ni(void *schedule) {
	if (schedule != NULL) {
		OQS_MEM_secure_free(schedule, 15 * sizeof(__m128i));
	}
}

// From crypto_core/aes256encrypt/dolbeau/aesenc-int
static inline void aes256ni_encrypt(const __m128i rkeys[15], const unsigned char *n, unsigned char *out) {
	__m128i nv = _mm_load_si128((const __m128i *)n);
	int i;
	__m128i temp = _mm_xor_si128(nv, rkeys[0]);
#pragma unroll(13)
	for (i = 1 ; i < 14 ; i++) {
		temp = _mm_aesenc_si128(temp, rkeys[i]);
	}
	temp = _mm_aesenclast_si128(temp, rkeys[14]);
	_mm_store_si128((__m128i *)(out), temp);
}

void oqs_aes256_enc_sch_block_ni(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const __m128i *schedule = (const __m128i *) _schedule;
	aes256ni_encrypt(schedule, plaintext, ciphertext);
}

// From crypto_core/aes256decrypt/dolbeau/aesenc-int
static inline void aes256ni_decrypt(const __m128i rkeys[15], const unsigned char *n, unsigned char *out) {
	__m128i nv = _mm_load_si128((const __m128i *)n);
	int i;
	__m128i temp = _mm_xor_si128(nv, rkeys[0]);
#pragma unroll(13)
	for (i = 1 ; i < 14 ; i++) {
		temp = _mm_aesdec_si128(temp, rkeys[i]);
	}
	temp = _mm_aesdeclast_si128(temp, rkeys[14]);
	_mm_store_si128((__m128i *)(out), temp);
}

void oqs_aes256_dec_sch_block_ni(const uint8_t *ciphertext, const void *_schedule, uint8_t *plaintext) {
	const __m128i *schedule = (const __m128i *) _schedule;
	aes256ni_decrypt(schedule, ciphertext, plaintext);
}
