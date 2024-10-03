// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <oqs/common.h>

#include "aes.h"
#include "aes_local.h"

static struct OQS_AES_callbacks *callbacks = &aes_default_callbacks;

OQS_API void OQS_AES_set_callbacks(struct OQS_AES_callbacks *new_callbacks) {
	if (new_callbacks != NULL) {
		callbacks = new_callbacks;
	}
}

void OQS_AES128_ECB_load_schedule(const uint8_t *key, void **schedule) {
	if (key != NULL && schedule != NULL) {
		callbacks->AES128_ECB_load_schedule(key, schedule);
	}
}

void OQS_AES128_CTR_inc_init(const uint8_t *key, void **_schedule) {
	if (key != NULL && _schedule != NULL) {
		callbacks->AES128_CTR_inc_init(key, _schedule);
	}
}

void OQS_AES128_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *_schedule) {
	if (iv != NULL && _schedule != NULL) {
		callbacks->AES128_CTR_inc_iv(iv, iv_len, _schedule);
	}
}

void OQS_AES128_CTR_inc_ivu64(uint64_t iv, void *_schedule) {
	if (_schedule != NULL) {
		callbacks->AES128_CTR_inc_ivu64(iv, _schedule);
	}
}

void OQS_AES128_free_schedule(void *schedule) {
	if (schedule != NULL) {
		callbacks->AES128_free_schedule(schedule);
	}
}

void OQS_AES256_ECB_load_schedule(const uint8_t *key, void **schedule) {
	if (key != NULL && schedule != NULL) {
		callbacks->AES256_ECB_load_schedule(key, schedule);
	}
}

void OQS_AES256_CTR_inc_init(const uint8_t *key, void **schedule) {
	if (key != NULL && schedule != NULL) {
		callbacks->AES256_CTR_inc_init(key, schedule);
	}
}

void OQS_AES256_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *schedule) {
	if (iv != NULL && schedule != NULL) {
		callbacks->AES256_CTR_inc_iv(iv, iv_len, schedule);
	}
}

void OQS_AES256_CTR_inc_ivu64(uint64_t iv, void *schedule) {
	if (schedule != NULL) {
		callbacks->AES256_CTR_inc_ivu64(iv, schedule);
	}
}

void OQS_AES256_free_schedule(void *schedule) {
	if (schedule != NULL) {
		callbacks->AES256_free_schedule(schedule);
	}
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	if (plaintext != NULL && key != NULL && ciphertext != NULL) {
		callbacks->AES128_ECB_enc(plaintext, plaintext_len, key, ciphertext);
	}
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	if (plaintext != NULL && schedule != NULL && ciphertext != NULL) {
		callbacks->AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	}
}

void OQS_AES128_CTR_inc_stream_iv(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	if (iv != NULL && schedule != NULL && out != NULL) {
		callbacks->AES128_CTR_inc_stream_iv(iv, iv_len, schedule, out, out_len);
	}
}

void OQS_AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	if (plaintext != NULL && key != NULL && ciphertext != NULL) {
		callbacks->AES256_ECB_enc(plaintext, plaintext_len, key, ciphertext);
	}
}

void OQS_AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	if (plaintext != NULL && schedule != NULL && ciphertext != NULL) {
		callbacks->AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	}
}

void OQS_AES256_CTR_inc_stream_iv(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	if (iv != NULL && schedule != NULL && out != NULL) {
		callbacks->AES256_CTR_inc_stream_iv(iv, iv_len, schedule, out, out_len);
	}
}

void OQS_AES256_CTR_inc_stream_blks(void *schedule, uint8_t *out, size_t out_blks) {
	if (schedule != NULL && out != NULL) {
		callbacks->AES256_CTR_inc_stream_blks(schedule, out, out_blks);
	}
}
