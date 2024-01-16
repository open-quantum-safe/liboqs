// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <oqs/common.h>

#include "aes.h"
#include "aes_local.h"

static struct OQS_AES_callbacks *callbacks = &aes_default_callbacks;

OQS_API void OQS_AES_set_callbacks(struct OQS_AES_callbacks *new_callbacks) {
	callbacks = new_callbacks;
}

void OQS_AES128_ECB_load_schedule(const uint8_t *key, void **schedule) {
	callbacks->AES128_ECB_load_schedule(key, schedule);
}

void OQS_AES128_CTR_inc_init(const uint8_t *key, void **_schedule) {
	callbacks->AES128_CTR_inc_init(key, _schedule);
}

void OQS_AES128_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *_schedule) {
	callbacks->AES128_CTR_inc_iv(iv, iv_len, _schedule);
}

void OQS_AES128_CTR_inc_ivu64(uint64_t iv, void *_schedule) {
	callbacks->AES128_CTR_inc_ivu64(iv, _schedule);
}

void OQS_AES128_free_schedule(void *schedule) {
	callbacks->AES128_free_schedule(schedule);
}

void OQS_AES256_ECB_load_schedule(const uint8_t *key, void **schedule) {
	callbacks->AES256_ECB_load_schedule(key, schedule);
}

void OQS_AES256_CTR_inc_init(const uint8_t *key, void **schedule) {
	callbacks->AES256_CTR_inc_init(key, schedule);
}

void OQS_AES256_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *schedule) {
	callbacks->AES256_CTR_inc_iv(iv, iv_len, schedule);
}

void OQS_AES256_CTR_inc_ivu64(uint64_t iv, void *schedule) {
	callbacks->AES256_CTR_inc_ivu64(iv, schedule);
}

void OQS_AES256_free_schedule(void *schedule) {
	callbacks->AES256_free_schedule(schedule);
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	callbacks->AES128_ECB_enc(plaintext, plaintext_len, key, ciphertext);
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	callbacks->AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
}

void OQS_AES128_CTR_inc_stream_iv(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	callbacks->AES128_CTR_inc_stream_iv(iv, iv_len, schedule, out, out_len);
}

void OQS_AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	callbacks->AES256_ECB_enc(plaintext, plaintext_len, key, ciphertext);
}

void OQS_AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	callbacks->AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
}

void OQS_AES256_CTR_inc_stream_iv(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	callbacks->AES256_CTR_inc_stream_iv(iv, iv_len, schedule, out, out_len);
}

void OQS_AES256_CTR_inc_stream_blks(void *schedule, uint8_t *out, size_t out_blks) {
	callbacks->AES256_CTR_inc_stream_blks(schedule, out, out_blks);
}
