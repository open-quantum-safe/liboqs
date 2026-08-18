// SPDX-License-Identifier: MIT

#include <assert.h>
#include <string.h>

#include <oqs/oqs.h>

#include "aes.h"

#include "../ossl_helpers.h"

struct key_schedule {
	int for_ECB;
	EVP_CIPHER_CTX *ctx;
	uint8_t key[32];
	uint8_t iv[16];
};

static inline void br_enc64be(unsigned char *dst, uint64_t x) {
	dst[7] = (unsigned char)(x >> 56);
	dst[6] = (unsigned char)(x >> 48);
	dst[5] = (unsigned char)(x >> 40);
	dst[4] = (unsigned char)(x >> 32);
	dst[3] = (unsigned char)(x >> 24);
	dst[2] = (unsigned char)(x >> 16);
	dst[1] = (unsigned char)(x >>  8);
	dst[0] = (unsigned char)x;
}

static void AES128_ECB_load_schedule(const uint8_t *key, void **schedule) {
	*schedule = OQS_MEM_malloc(sizeof(struct key_schedule));
	OQS_EXIT_IF_NULLPTR(*schedule, "OpenSSL");
	struct key_schedule *ks = (struct key_schedule *) *schedule;
	ks->for_ECB = 1;
	ks->ctx = OSSL_FUNC(EVP_CIPHER_CTX_new)();
	OQS_EXIT_IF_NULLPTR(ks->ctx, "OpenSSL");
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ks->ctx, oqs_aes_128_ecb(), NULL, key, NULL));
	OSSL_FUNC(EVP_CIPHER_CTX_set_padding)(ks->ctx, 0);
}

static void AES128_free_schedule(void *schedule) {
	if (schedule != NULL) {
		struct key_schedule *ks = (struct key_schedule *) schedule;
		if (ks->ctx != NULL) {
			OSSL_FUNC(EVP_CIPHER_CTX_free)(ks->ctx);
		}
		OQS_MEM_cleanse(ks->key, 32);
		OQS_MEM_secure_free(schedule, sizeof(struct key_schedule));
	}
}

static void AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES128_ECB_load_schedule(key, &schedule);
	OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES128_free_schedule(schedule);
}

static void AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	int outlen;
	const struct key_schedule *ks = (const struct key_schedule *) schedule;
	SIZE_T_TO_INT_OR_EXIT(plaintext_len, plaintext_len_int)
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptUpdate)(ks->ctx, ciphertext, &outlen, plaintext, plaintext_len_int));
	assert(outlen == plaintext_len_int);
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptFinal_ex)(ks->ctx, ciphertext, &outlen));
}

static void AES128_CTR_inc_stream_iv(const uint8_t *iv, size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	EVP_CIPHER_CTX *ctr_ctx = OSSL_FUNC(EVP_CIPHER_CTX_new());
	OQS_EXIT_IF_NULLPTR(ctr_ctx, "OpenSSL");
	uint8_t iv_ctr[16];
	if (iv_len == 12) {
		memcpy(iv_ctr, iv, 12);
		iv_ctr[12] = 0;
		iv_ctr[13] = 0;
		iv_ctr[14] = 0;
		iv_ctr[15] = 0;
	} else if (iv_len == 16) {
		memcpy(iv_ctr, iv, 16);
	} else {
		exit(EXIT_FAILURE);
	}
	const struct key_schedule *ks = (const struct key_schedule *) schedule;
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ctr_ctx, oqs_aes_128_ctr(), NULL, ks->key, iv_ctr));

	SIZE_T_TO_INT_OR_EXIT(out_len, out_len_input_int)
	memset(out, 0, (size_t)out_len_input_int);
	int out_len_output;
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptUpdate)(ctr_ctx, out, &out_len_output, out, out_len_input_int));
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptFinal_ex)(ctr_ctx, out + out_len_output, &out_len_output));
	OSSL_FUNC(EVP_CIPHER_CTX_free)(ctr_ctx);
}

static void AES128_CTR_inc_init(const uint8_t *key, void **schedule) {
	*schedule = OQS_MEM_malloc(sizeof(struct key_schedule));
	OQS_EXIT_IF_NULLPTR(*schedule, "OpenSSL");

	struct key_schedule *ks = (struct key_schedule *) *schedule;
	EVP_CIPHER_CTX *ctr_ctx = OSSL_FUNC(EVP_CIPHER_CTX_new)();
	OQS_EXIT_IF_NULLPTR(ctr_ctx, "OpenSSL");

	ks->for_ECB = 0;
	ks->ctx = ctr_ctx;
	memcpy(ks->key, key, 16);
}

static void AES128_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *schedule) {
	OQS_EXIT_IF_NULLPTR(schedule, "OpenSSL");
	struct key_schedule *ks = (struct key_schedule *) schedule;
	if (iv_len == 12) {
		memcpy(ks->iv, iv, 12);
		memset(&ks->iv[12], 0, 4);
	} else if (iv_len == 16) {
		memcpy(ks->iv, iv, 16);
	} else {
		exit(EXIT_FAILURE);
	}
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ks->ctx, oqs_aes_128_ctr(), NULL, ks->key, ks->iv));
}

static void AES128_CTR_inc_ivu64(uint64_t iv, void *schedule) {
	OQS_EXIT_IF_NULLPTR(schedule, "OpenSSL");
	struct key_schedule *ks = (struct key_schedule *) schedule;
	br_enc64be(ks->iv, iv);
	memset(&ks->iv[8], 0, 8);
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ks->ctx, oqs_aes_128_ctr(), NULL, ks->key, ks->iv));
}

static void AES256_ECB_load_schedule(const uint8_t *key, void **schedule) {
	*schedule = OQS_MEM_malloc(sizeof(struct key_schedule));
	OQS_EXIT_IF_NULLPTR(*schedule, "OpenSSL");
	struct key_schedule *ks = (struct key_schedule *) *schedule;
	ks->for_ECB = 1;
	ks->ctx = OSSL_FUNC(EVP_CIPHER_CTX_new)();
	OQS_EXIT_IF_NULLPTR(ks->ctx, "OpenSSL");
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ks->ctx, oqs_aes_256_ecb(), NULL, key, NULL));
	OSSL_FUNC(EVP_CIPHER_CTX_set_padding)(ks->ctx, 0);
}

static void AES256_CTR_inc_init(const uint8_t *key, void **schedule) {
	*schedule = OQS_MEM_malloc(sizeof(struct key_schedule));
	OQS_EXIT_IF_NULLPTR(*schedule, "OpenSSL");

	struct key_schedule *ks = (struct key_schedule *) *schedule;
	EVP_CIPHER_CTX *ctr_ctx = OSSL_FUNC(EVP_CIPHER_CTX_new)();
	OQS_EXIT_IF_NULLPTR(ctr_ctx, "OpenSSL");

	ks->for_ECB = 0;
	ks->ctx = ctr_ctx;
	memcpy(ks->key, key, 32);
}

static void AES256_CTR_inc_iv(const uint8_t *iv, size_t iv_len, void *schedule) {
	OQS_EXIT_IF_NULLPTR(schedule, "OpenSSL");
	struct key_schedule *ks = (struct key_schedule *) schedule;
	if (iv_len == 12) {
		memcpy(ks->iv, iv, 12);
		memset(&ks->iv[12], 0, 4);
	} else if (iv_len == 16) {
		memcpy(ks->iv, iv, 16);
	} else {
		exit(EXIT_FAILURE);
	}
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ks->ctx, oqs_aes_256_ctr(), NULL, ks->key, ks->iv));
}

static void AES256_CTR_inc_ivu64(uint64_t iv, void *schedule) {
	OQS_EXIT_IF_NULLPTR(schedule, "OpenSSL");
	struct key_schedule *ks = (struct key_schedule *) schedule;
	br_enc64be(ks->iv, iv);
	memset(&ks->iv[8], 0, 8);
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ks->ctx, oqs_aes_256_ctr(), NULL, ks->key, ks->iv));
}

static void AES256_free_schedule(void *schedule) {
	// actually same code as AES 128
	OQS_AES128_free_schedule(schedule);
}

static void AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES256_ECB_load_schedule(key, &schedule);
	OQS_AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES256_free_schedule(schedule);
}

static void AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	// actually same code as AES 128
	OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
}

static void AES256_CTR_inc_stream_iv(const uint8_t *iv, size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	EVP_CIPHER_CTX *ctr_ctx = OSSL_FUNC(EVP_CIPHER_CTX_new)();
	OQS_EXIT_IF_NULLPTR(ctr_ctx, "OpenSSL");
	uint8_t iv_ctr[16];
	if (iv_len == 12) {
		memcpy(iv_ctr, iv, 12);
		iv_ctr[12] = 0;
		iv_ctr[13] = 0;
		iv_ctr[14] = 0;
		iv_ctr[15] = 0;
	} else if (iv_len == 16) {
		memcpy(iv_ctr, iv, 16);
	} else {
		exit(EXIT_FAILURE);
	}
	const struct key_schedule *ks = (const struct key_schedule *) schedule;
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptInit_ex)(ctr_ctx, oqs_aes_256_ctr(), NULL, ks->key, iv_ctr));

	SIZE_T_TO_INT_OR_EXIT(out_len, out_len_input_int)
	memset(out, 0, (size_t)out_len_input_int);
	int out_len_output;
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptUpdate)(ctr_ctx, out, &out_len_output, out, out_len_input_int));
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptFinal_ex)(ctr_ctx, out + out_len_output, &out_len_output));
	OSSL_FUNC(EVP_CIPHER_CTX_free)(ctr_ctx);
}

static void AES256_CTR_inc_stream_blks(void *schedule, uint8_t *out, size_t out_blks) {
	size_t out_len = out_blks * 16;
	struct key_schedule *ks = (struct key_schedule *) schedule;
	int out_len_output;
	SIZE_T_TO_INT_OR_EXIT(out_len, out_len_input_int);
	memset(out, 0, (size_t)out_len_input_int);
	OQS_OPENSSL_GUARD(OSSL_FUNC(EVP_EncryptUpdate)(ks->ctx, out, &out_len_output, out, (int) out_len));
}

struct OQS_AES_callbacks aes_default_callbacks = {
	.AES128_ECB_load_schedule = AES128_ECB_load_schedule,
	.AES128_CTR_inc_init = AES128_CTR_inc_init,
	.AES128_CTR_inc_iv = AES128_CTR_inc_iv,
	.AES128_CTR_inc_ivu64 = AES128_CTR_inc_ivu64,
	.AES128_free_schedule = AES128_free_schedule,
	.AES128_ECB_enc = AES128_ECB_enc,
	.AES128_ECB_enc_sch = AES128_ECB_enc_sch,
	.AES256_ECB_load_schedule = AES256_ECB_load_schedule,
	.AES128_CTR_inc_stream_iv = AES128_CTR_inc_stream_iv,
	.AES256_CTR_inc_init = AES256_CTR_inc_init,
	.AES256_CTR_inc_iv = AES256_CTR_inc_iv,
	.AES256_CTR_inc_ivu64 = AES256_CTR_inc_ivu64,
	.AES256_free_schedule = AES256_free_schedule,
	.AES256_ECB_enc = AES256_ECB_enc,
	.AES256_ECB_enc_sch = AES256_ECB_enc_sch,
	.AES256_CTR_inc_stream_iv = AES256_CTR_inc_stream_iv,
	.AES256_CTR_inc_stream_blks = AES256_CTR_inc_stream_blks,
};
