#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <oqs/common.h>
#include "aes.h"

#include <wmmintrin.h>

static __m128i key_expand(__m128i key, __m128i keygened) {
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	// The last 4 bytes from aeskeygenassist store the values we want so
	// and they need to be xored all four sets of bytes in the result so
	keygened = _mm_shuffle_epi32(keygened, _MM_SHUFFLE(3, 3, 3, 3));
	return _mm_xor_si128(key, keygened);
}

static __m128i key_expand_aes256_additional(__m128i key, __m128i key2) {
	__m128i key_with_round_constant = _mm_aeskeygenassist_si128(key2, 0x00);
	key_with_round_constant = _mm_shuffle_epi32(key_with_round_constant, _MM_SHUFFLE(2, 2, 2, 2));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	return _mm_xor_si128(key, key_with_round_constant);
}

void OQS_AES128_load_schedule(const uint8_t *key, void **_schedule, UNUSED int for_encryption) {
	*_schedule = malloc(10 * 2 * 16);
	assert(*_schedule != NULL);
	__m128i *schedule = (__m128i *) *_schedule;
	schedule[0] = _mm_loadu_si128((const __m128i *) key);
	schedule[1] = key_expand(schedule[0], _mm_aeskeygenassist_si128(schedule[0], 0x01));
	schedule[2] = key_expand(schedule[1], _mm_aeskeygenassist_si128(schedule[1], 0x02));
	schedule[3] = key_expand(schedule[2], _mm_aeskeygenassist_si128(schedule[2], 0x04));
	schedule[4] = key_expand(schedule[3], _mm_aeskeygenassist_si128(schedule[3], 0x08));
	schedule[5] = key_expand(schedule[4], _mm_aeskeygenassist_si128(schedule[4], 0x10));
	schedule[6] = key_expand(schedule[5], _mm_aeskeygenassist_si128(schedule[5], 0x20));
	schedule[7] = key_expand(schedule[6], _mm_aeskeygenassist_si128(schedule[6], 0x40));
	schedule[8] = key_expand(schedule[7], _mm_aeskeygenassist_si128(schedule[7], 0x80));
	schedule[9] = key_expand(schedule[8], _mm_aeskeygenassist_si128(schedule[8], 0x1b));
	schedule[10] = key_expand(schedule[9], _mm_aeskeygenassist_si128(schedule[9], 0x36));
	// generate decryption keys in reverse order.
	// schedule[10] is shared by last encryption and first decryption rounds
	// schedule[0] is shared by first encryption round and last decryption round
	for (size_t i = 0; i < 9; i++) {
		schedule[11 + i] = _mm_aesimc_si128(schedule[9 - i]);
	}
}

void OQS_AES128_free_schedule(void *schedule) {
	if (schedule != NULL) {
		OQS_MEM_secure_free(schedule, 10 * 2 * 16);
	}
}

static void aes128_enc_block(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	__m128i *schedule = (__m128i *) _schedule;
	__m128i m = _mm_loadu_si128((__m128i *) plaintext);

	m = _mm_xor_si128(m, schedule[0]);
	for (size_t i = 1; i < 10; i++) {
		m = _mm_aesenc_si128(m, schedule[i]);
	}
	m = _mm_aesenclast_si128(m, schedule[10]);

	_mm_storeu_si128((__m128i *) ciphertext, m);
}

static void aes128_dec_block(const uint8_t *ciphertext, const void *_schedule, uint8_t *plaintext) {
	__m128i *schedule = (__m128i *) _schedule;
	__m128i m = _mm_loadu_si128((__m128i *) ciphertext);

	m = _mm_xor_si128(m, schedule[10]);
	for (size_t i = 1; i < 10; i++) {
		m = _mm_aesdec_si128(m, schedule[10 + i]);
	}
	m = _mm_aesdeclast_si128(m, schedule[0]);

	_mm_storeu_si128((__m128i *) plaintext, m);
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES128_load_schedule(key, &schedule, 1);
	OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES128_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
	void *schedule = NULL;
	OQS_AES128_load_schedule(key, &schedule, 0);
	OQS_AES128_ECB_dec_sch(ciphertext, ciphertext_len, schedule, plaintext);
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		aes128_enc_block(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}

void OQS_AES128_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	assert(ciphertext_len % 16 == 0);
	for (size_t block = 0; block < ciphertext_len / 16; block++) {
		aes128_dec_block(ciphertext + (16 * block), schedule, plaintext + (16 * block));
	}
}

void OQS_AES256_load_schedule(const uint8_t *key, void **_schedule, UNUSED int for_encryption) {
	*_schedule = malloc(14 * 2 * 16);
	assert(*_schedule != NULL);
	__m128i *schedule = (__m128i *) *_schedule;
	schedule[0] = _mm_loadu_si128((const __m128i *) key);
	schedule[1] = _mm_loadu_si128((const __m128i *) (key + 16));
	schedule[2] = key_expand(schedule[0], _mm_aeskeygenassist_si128(schedule[1], 0x01));
	schedule[3] = key_expand_aes256_additional(schedule[1], schedule[2]);
	schedule[4] = key_expand(schedule[2], _mm_aeskeygenassist_si128(schedule[3], 0x02));
	schedule[5] = key_expand_aes256_additional(schedule[3], schedule[4]);
	schedule[6] = key_expand(schedule[4], _mm_aeskeygenassist_si128(schedule[5], 0x04));
	schedule[7] = key_expand_aes256_additional(schedule[5], schedule[6]);
	schedule[8] = key_expand(schedule[6], _mm_aeskeygenassist_si128(schedule[7], 0x08));
	schedule[9] = key_expand_aes256_additional(schedule[7], schedule[8]);
	schedule[10] = key_expand(schedule[8], _mm_aeskeygenassist_si128(schedule[9], 0x10));
	schedule[11] = key_expand_aes256_additional(schedule[9], schedule[10]);
	schedule[12] = key_expand(schedule[10], _mm_aeskeygenassist_si128(schedule[11], 0x20));
	schedule[13] = key_expand_aes256_additional(schedule[11], schedule[12]);
	schedule[14] = key_expand(schedule[12], _mm_aeskeygenassist_si128(schedule[13], 0x40));
	// generate decryption keys in reverse order.
	// schedule[14] is shared by last encryption and first decryption rounds
	// schedule[0] is shared by first encryption round and last decryption round
	for (size_t i = 0; i < 13; i++) {
		schedule[15 + i] = _mm_aesimc_si128(schedule[13 - i]);
	}
}

void OQS_AES256_free_schedule(void *schedule) {
	if (schedule != NULL) {
		OQS_MEM_secure_free(schedule, 14 * 2 * 16);
	}
}

static void aes256_enc_block(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	__m128i *schedule = (__m128i *) _schedule;
	__m128i m = _mm_loadu_si128((__m128i *) plaintext);

	m = _mm_xor_si128(m, schedule[0]);
	for (size_t i = 1; i < 14; i++) {
		m = _mm_aesenc_si128(m, schedule[i]);
	}
	m = _mm_aesenclast_si128(m, schedule[14]);

	_mm_storeu_si128((__m128i *) ciphertext, m);
}

static void aes256_dec_block(const uint8_t *ciphertext, const void *_schedule, uint8_t *plaintext) {
	__m128i *schedule = (__m128i *) _schedule;
	__m128i m = _mm_loadu_si128((__m128i *) ciphertext);

	m = _mm_xor_si128(m, schedule[14]);
	for (size_t i = 1; i < 14; i++) {
		m = _mm_aesdec_si128(m, schedule[14 + i]);
	}
	m = _mm_aesdeclast_si128(m, schedule[0]);

	_mm_storeu_si128((__m128i *) plaintext, m);
}

void OQS_AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES256_load_schedule(key, &schedule, 1);
	OQS_AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES256_free_schedule(schedule);
}

void OQS_AES256_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
	void *schedule = NULL;
	OQS_AES256_load_schedule(key, &schedule, 0);
	OQS_AES256_ECB_dec_sch(ciphertext, ciphertext_len, schedule, plaintext);
	OQS_AES256_free_schedule(schedule);
}

void OQS_AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		aes256_enc_block(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}

void OQS_AES256_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	assert(ciphertext_len % 16 == 0);
	for (size_t block = 0; block < ciphertext_len / 16; block++) {
		aes256_dec_block(ciphertext + (16 * block), schedule, plaintext + (16 * block));
	}
}
