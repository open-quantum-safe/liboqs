// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <oqs/common.h>

#include "aes.h"

/* NULL means "use the default backend", see oqs_aes_default_callbacks(). */
static const struct OQS_AES_callbacks *callbacks = NULL;

static inline const struct OQS_AES_callbacks *aes_cb(void) {
	return callbacks != NULL ? callbacks : oqs_aes_default_callbacks();
}

OQS_API void OQS_AES_set_callbacks(struct OQS_AES_callbacks *new_callbacks) {
	callbacks = new_callbacks;
}

void OQS_AES128_ECB_load_schedule(const uint8_t *key, void **schedule) {
	aes_cb()->AES128_ECB_load_schedule(key, schedule);
}

void OQS_AES128_CTR_inc_init(const uint8_t *key, void **_schedule) {
	aes_cb()->AES128_CTR_inc_init(key, _schedule);
}

void OQS_AES128_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *_schedule) {
	aes_cb()->AES128_CTR_inc_iv(iv, iv_len, _schedule);
}

void OQS_AES128_CTR_inc_ivu64(uint64_t iv, void *_schedule) {
	aes_cb()->AES128_CTR_inc_ivu64(iv, _schedule);
}

void OQS_AES128_free_schedule(void *schedule) {
	aes_cb()->AES128_free_schedule(schedule);
}

void OQS_AES256_ECB_load_schedule(const uint8_t *key, void **schedule) {
	aes_cb()->AES256_ECB_load_schedule(key, schedule);
}

void OQS_AES256_CTR_inc_init(const uint8_t *key, void **schedule) {
	aes_cb()->AES256_CTR_inc_init(key, schedule);
}

void OQS_AES256_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *schedule) {
	aes_cb()->AES256_CTR_inc_iv(iv, iv_len, schedule);
}

void OQS_AES256_CTR_inc_ivu64(uint64_t iv, void *schedule) {
	aes_cb()->AES256_CTR_inc_ivu64(iv, schedule);
}

void OQS_AES256_free_schedule(void *schedule) {
	aes_cb()->AES256_free_schedule(schedule);
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	aes_cb()->AES128_ECB_enc(plaintext, plaintext_len, key, ciphertext);
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	aes_cb()->AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
}

void OQS_AES128_CTR_inc_stream_iv(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	aes_cb()->AES128_CTR_inc_stream_iv(iv, iv_len, schedule, out, out_len);
}

void OQS_AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	aes_cb()->AES256_ECB_enc(plaintext, plaintext_len, key, ciphertext);
}

void OQS_AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	aes_cb()->AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
}

void OQS_AES256_CTR_inc_stream_iv(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	aes_cb()->AES256_CTR_inc_stream_iv(iv, iv_len, schedule, out, out_len);
}

void OQS_AES256_CTR_inc_stream_blks(void *schedule, uint8_t *out, size_t out_blks) {
	aes_cb()->AES256_CTR_inc_stream_blks(schedule, out, out_blks);
}

void OQS_AES192_ECB_load_schedule(const uint8_t *key, void **ctx) {
	aes_cb()->AES192_ECB_load_schedule(key, ctx);
}

void OQS_AES192_free_schedule(void *ctx) {
	aes_cb()->AES192_free_schedule(ctx);
}

void OQS_AES192_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	aes_cb()->AES192_ECB_enc(plaintext, plaintext_len, key, ciphertext);
}

void OQS_AES192_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	aes_cb()->AES192_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
}
