/**
 * \file sha2.h
 * \brief Internal SHA2 functions that enable easy switching between native instructions
 *        and c implementations
 *
 * <b>Note this is not part of the OQS public API: implementations within liboqs can use these
 * functions, but external consumers of liboqs should not use these functions.</b>
 *
 * \author Douglas Stebila
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef OQS_SHA2_LOCAL_H
#define OQS_SHA2_LOCAL_H

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    uint8_t *ctx;
} sha256ctx;

typedef struct {
	uint8_t *ctx;
} sha384ctx;

typedef struct {
	uint8_t *ctx;
} sha512ctx;
/**
 * \brief Allocate and initialize the state for the SHA-256 incremental hashing API.
 *
 * \warning The state must be released by OQS_SHA2_sha256_inc_finalize
 * or OQS_SHA2_sha256_inc_ctx_release.
 *
 * \param state Pointer to the state
 */
void oqs_sha2_sha256_inc_init_c(sha256ctx *state);
void oqs_sha2_sha256_inc_ctx_clone_c(sha256ctx *dest, const sha256ctx *src);
void oqs_sha2_sha256_inc_blocks_c(sha256ctx *state, const uint8_t *in, size_t inblocks);
void oqs_sha2_sha256_inc_finalize_c(uint8_t *out, sha256ctx *state, const uint8_t *in, size_t inlen);
void oqs_sha2_sha256_inc_ctx_release_c(sha256ctx *state);

/**
 * \brief Process a message with SHA-384 and return the hash code in the output byte array.
 *
 * \warning The output array must be at least 48 bytes in length.
 *
 * \param output The output byte array
 * \param input The message input byte array
 * \param inplen The number of message bytes to process
 */
void oqs_sha2_sha384(uint8_t *output, const uint8_t *input, size_t inplen);
void oqs_sha2_sha384_inc_init_c(sha384ctx *state);
void oqs_sha2_sha384_inc_ctx_clone_c(sha384ctx *dest, const sha384ctx *src);
void oqs_sha2_sha384_inc_blocks_c(sha384ctx *state, const uint8_t *in, size_t inblocks);
void oqs_sha2_sha384_inc_finalize_c(uint8_t *out, sha384ctx *state, const uint8_t *in, size_t inlen);
void oqs_sha2_sha384_inc_ctx_release_c(sha384ctx *state);

/**
 * \brief Process a message with SHA-512 and return the hash code in the output byte array.
 *
 * \warning The output array must be at least 64 bytes in length.
 *
 * \param output The output byte array
 * \param input The message input byte array
 * \param inplen The number of message bytes to process
 */
void oqs_sha2_sha512(uint8_t *output, const uint8_t *input, size_t inplen);


void oqs_sha2_sha512_inc_init_c(sha512ctx *state);
void oqs_sha2_sha512_inc_ctx_clone_c(sha512ctx *dest, const sha512ctx *src);
void oqs_sha2_sha512_inc_blocks_c(sha512ctx *state, const uint8_t *in, size_t inblocks);
void oqs_sha2_sha512_inc_finalize_c(uint8_t *out, sha512ctx *state, const uint8_t *in, size_t inlen);
void oqs_sha2_sha512_inc_ctx_release_c(sha512ctx *state);


// Native instruction functions

void oqs_sha2_sha256_inc_blocks_ni(sha256ctx *state, const uint8_t *in, size_t inblocks);
void oqs_sha2_sha256_ni(uint8_t *out, const uint8_t *in, size_t inlen);

/**
 * \brief Process a message with SHA-384 and return the hash code in the output byte array.
 *
 * \warning The output array must be at least 48 bytes in length.
 *
 * \param output The output byte array
 * \param input The message input byte array
 * \param inplen The number of message bytes to process
 */
//void oqs_sha2_sha384(uint8_t *output, const uint8_t *input, size_t inplen);
void oqs_sha2_sha384_inc_init_ni(sha384ctx *state);
void oqs_sha2_sha384_inc_ctx_clone_ni(sha384ctx *dest, const sha384ctx *src);
void oqs_sha2_sha384_inc_blocks_ni(sha384ctx *state, const uint8_t *in, size_t inblocks);
void oqs_sha2_sha384_inc_finalize_ni(uint8_t *out, sha384ctx *state, const uint8_t *in, size_t inlen);
void oqs_sha2_sha384_inc_ctx_release_ni(sha384ctx *state);

void oqs_sha2_sha256_c(uint8_t *out, const uint8_t *in, size_t inlen);
void oqs_sha2_sha384_c(uint8_t *out, const uint8_t *in, size_t inlen);
void oqs_sha2_sha512_c(uint8_t *out, const uint8_t *in, size_t inlen);

#if defined(__cplusplus)
} // extern "C"
#endif

#endif // OQS_SHA2_LOCAL_H
