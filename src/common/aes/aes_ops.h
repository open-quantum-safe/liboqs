/**
 * \file aes_ops.h
 * \brief Header defining the callback API for OQS AES
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef OQS_AES_OPS_H
#define OQS_AES_OPS_H

#include <stdint.h>
#include <stdlib.h>

#include <oqs/common.h>

#if defined(__cplusplus)
extern "C" {
#endif

/** Data structure implemented by cryptographic provider for AES operations.
 */
struct OQS_AES_callbacks {
	/**
	 * Implementation of function OQS_AES128_ECB_load_schedule.
	 */
	void (*AES128_ECB_load_schedule)(const uint8_t *key, void **ctx);

	/**
	 * Implementation of function OQS_AES256_CTR_inc_init.
	 */
	void (*AES128_CTR_inc_init)(const uint8_t *key, void **ctx);

	/**
	 * Implementation of function OQS_AES256_CTR_inc_iv.
	 */
	void (*AES128_CTR_inc_iv)(const uint8_t *iv, size_t iv_len, void *ctx);

	/**
	 * Implementation of function OQS_AES256_CTR_inc_ivu64.
	 */
	void (*AES128_CTR_inc_ivu64)(uint64_t iv, void *ctx);

	/**
	 * Implementation of function OQS_AES128_free_schedule.
	 */
	void (*AES128_free_schedule)(void *ctx);

	/**
	 * Implementation of function OQS_AES128_ECB_enc.
	 */
	void (*AES128_ECB_enc)(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext);

	/**
	 * Implementation of function OQS_AES128_ECB_enc_sch.
	 */
	void (*AES128_ECB_enc_sch)(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);

	/**
	* Implementation of function OQS_AES128_CTR_inc_stream_iv.
	*/
	void (*AES128_CTR_inc_stream_iv)(const uint8_t *iv, size_t iv_len, const void *ctx, uint8_t *out, size_t out_len);

	/**
	 * Implementation of function OQS_AES256_ECB_load_schedule.
	 */
	void (*AES256_ECB_load_schedule)(const uint8_t *key, void **ctx);

	/**
	 * Implementation of function OQS_AES256_CTR_inc_init.
	 */
	void (*AES256_CTR_inc_init)(const uint8_t *key, void **ctx);

	/**
	 * Implementation of function OQS_AES256_CTR_inc_iv.
	 */
	void (*AES256_CTR_inc_iv)(const uint8_t *iv, size_t iv_len, void *ctx);

	/**
	 * Implementation of function OQS_AES256_CTR_inc_ivu64.
	 */
	void (*AES256_CTR_inc_ivu64)(uint64_t iv, void *ctx);

	/**
	 * Implementation of function OQS_AES256_free_schedule.
	 */
	void (*AES256_free_schedule)(void *ctx);

	/**
	 * Implementation of function OQS_AES256_ECB_enc.
	 */
	void (*AES256_ECB_enc)(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext);

	/**
	 * Implementation of function OQS_AES256_ECB_enc_sch.
	 */
	void (*AES256_ECB_enc_sch)(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);

	/**
	 * Implementation of function OQS_AES256_CTR_inc_stream_iv.
	 */
	void (*AES256_CTR_inc_stream_iv)(const uint8_t *iv, size_t iv_len, const void *ctx, uint8_t *out, size_t out_len);

	/**
	 * Implementation of function OQS_AES256_CTR_inc_stream_blks.
	 */
	void (*AES256_CTR_inc_stream_blks)(void *ctx, uint8_t *out, size_t out_blks);

	/**
	 * Implementation of function OQS_AES192_ECB_load_schedule.
	 */
	void (*AES192_ECB_load_schedule)(const uint8_t *key, void **ctx);

	/**
	 * Implementation of function OQS_AES192_free_schedule.
	 */
	void (*AES192_free_schedule)(void *ctx);

	/**
	 * Implementation of function OQS_AES192_ECB_enc.
	 */
	void (*AES192_ECB_enc)(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext);

	/**
	 * Implementation of function OQS_AES192_ECB_enc_sch.
	 */
	void (*AES192_ECB_enc_sch)(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);
};

/**
 * Set callback functions for AES operations.
 *
 * This function may be called before OQS_init to switch the
 * cryptographic provider for AES operations. If it is not called, or if it is
 * called with NULL, the default provider is used. The default provider is
 * determined at build time and, for distributable builds, by CPU feature
 * detection.
 *
 * The structure is not copied, so it must remain valid until the callbacks are
 * replaced. Key schedules belong to the provider that created them: do not call
 * this function while key schedules created through the current callbacks are
 * still in use, or while another thread may be performing AES operations.
 *
 * @param[in] new_callbacks Callback functions defined in OQS_AES_callbacks, or
 *                          NULL to restore the default provider
 */
OQS_API void OQS_AES_set_callbacks(struct OQS_AES_callbacks *new_callbacks);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // OQS_AES_OPS_H
