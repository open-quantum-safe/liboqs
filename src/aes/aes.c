#include <assert.h>

#include "aes.h"
#include "aes_local.h"

void OQS_AES128_load_schedule(const uint8_t *key, void **schedule) {
#ifndef AES_DISABLE_NI
	oqs_aes128_load_schedule_ni(key, schedule);
#else
	oqs_aes128_load_schedule_c(key, schedule);
#endif
}

void OQS_AES128_free_schedule(void *schedule) {
#ifndef AES_DISABLE_NI
	oqs_aes128_free_schedule_ni(schedule);
#else
	oqs_aes128_free_schedule_c(schedule);
#endif
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
#ifndef AES_DISABLE_NI
	oqs_aes128_ecb_enc_ni(plaintext, plaintext_len, key, ciphertext);
#else
	oqs_aes128_ecb_enc_c(plaintext, plaintext_len, key, ciphertext);
#endif
}

void OQS_AES128_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
#ifndef AES_DISABLE_NI
	oqs_aes128_ecb_dec_ni(ciphertext, ciphertext_len, key, plaintext);
#else
	oqs_aes128_ecb_dec_c(ciphertext, ciphertext_len, key, plaintext);
#endif
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
#ifndef AES_DISABLE_NI
	oqs_aes128_ecb_enc_sch_ni(plaintext, plaintext_len, schedule, ciphertext);
#else
	oqs_aes128_ecb_enc_sch_c(plaintext, plaintext_len, schedule, ciphertext);
#endif
}

void OQS_AES128_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
#ifndef AES_DISABLE_NI
	oqs_aes128_ecb_dec_sch_ni(ciphertext, ciphertext_len, schedule, plaintext);
#else
	oqs_aes128_ecb_dec_sch_c(ciphertext, ciphertext_len, schedule, plaintext);
#endif
}

inline void oqs_aes128_ecb_enc_ni(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	oqs_aes128_load_schedule_ni(key, &schedule);
	oqs_aes128_ecb_enc_sch_ni(plaintext, plaintext_len, schedule, ciphertext);
	oqs_aes128_free_schedule_ni(schedule);
}

inline void oqs_aes128_ecb_enc_c(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	oqs_aes128_load_schedule_c(key, &schedule);
	oqs_aes128_ecb_enc_sch_c(plaintext, plaintext_len, schedule, ciphertext);
	oqs_aes128_free_schedule_c(schedule);
}

inline void oqs_aes128_ecb_enc_sch_ni(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		oqs_aes128_enc_ni(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}

inline void oqs_aes128_ecb_enc_sch_c(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		oqs_aes128_enc_c(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}

inline void oqs_aes128_ecb_dec_ni(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
	void *schedule = NULL;
	oqs_aes128_load_schedule_ni(key, &schedule);
	oqs_aes128_ecb_dec_sch_ni(ciphertext, ciphertext_len, schedule, plaintext);
	oqs_aes128_free_schedule_ni(schedule);
}

inline void oqs_aes128_ecb_dec_c(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
	void *schedule = NULL;
	oqs_aes128_load_schedule_c(key, &schedule);
	oqs_aes128_ecb_dec_sch_c(ciphertext, ciphertext_len, schedule, plaintext);
	oqs_aes128_free_schedule_c(schedule);
}

inline void oqs_aes128_ecb_dec_sch_ni(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	assert(ciphertext_len % 16 == 0);
	for (size_t block = 0; block < ciphertext_len / 16; block++) {
		oqs_aes128_dec_ni(ciphertext + (16 * block), schedule, plaintext + (16 * block));
	}
}

inline void oqs_aes128_ecb_dec_sch_c(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	assert(ciphertext_len % 16 == 0);
	for (size_t block = 0; block < ciphertext_len / 16; block++) {
		oqs_aes128_dec_c(ciphertext + (16 * block), schedule, plaintext + (16 * block));
	}
}
