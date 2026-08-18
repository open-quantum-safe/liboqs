// SPDX-License-Identifier: MIT
#include <stdio.h>

#include <oqs/common.h>

#include "sha2.h"
#include "sha2_local.h"

#if defined(OQS_DIST_ARM64_V8_BUILD)
#define C_OR_ARM(stmt_c, stmt_arm) \
    do { \
        if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_SHA2)) {  \
            stmt_arm; \
        } else { \
            stmt_c; \
        } \
    } while(0)
#elif defined(OQS_USE_ARM_SHA2_INSTRUCTIONS)
#define C_OR_ARM(stmt_c, stmt_arm) \
    stmt_arm
#else
#define C_OR_ARM(stmt_c, stmt_arm) \
    stmt_c
#endif

static void SHA2_sha256_inc_init(OQS_SHA2_sha256_ctx *state) {
	oqs_sha2_sha256_inc_init_c((sha256ctx *) state);
}

static void SHA2_sha256_inc_ctx_clone(OQS_SHA2_sha256_ctx *dest, const OQS_SHA2_sha256_ctx *src) {
	oqs_sha2_sha256_inc_ctx_clone_c((sha256ctx *) dest, (const sha256ctx *) src);
}

static void SHA2_sha256_inc(OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t len) {
	C_OR_ARM(
	    oqs_sha2_sha256_inc_c((sha256ctx *) state, in, len),
	    oqs_sha2_sha256_inc_armv8((sha256ctx *) state, in, len)
	);
}

static void SHA2_sha256_inc_blocks(OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t inblocks) {
	C_OR_ARM(
	    oqs_sha2_sha256_inc_blocks_c((sha256ctx *) state, in, inblocks),
	    oqs_sha2_sha256_inc_blocks_armv8((sha256ctx *) state, in, inblocks)
	);
}

static void SHA2_sha256_inc_finalize(uint8_t *out, OQS_SHA2_sha256_ctx *state, const uint8_t *in, size_t inlen) {
	oqs_sha2_sha256_inc_finalize_c(out, (sha256ctx *) state, in, inlen);
}

static void SHA2_sha256_inc_ctx_release(OQS_SHA2_sha256_ctx *state) {
	oqs_sha2_sha256_inc_ctx_release_c((sha256ctx *) state);
}

static void SHA2_sha384_inc_init(OQS_SHA2_sha384_ctx *state) {
	oqs_sha2_sha384_inc_init_c((sha384ctx *)state);
}

static void SHA2_sha384_inc_ctx_clone(OQS_SHA2_sha384_ctx *dest, const OQS_SHA2_sha384_ctx *src) {
	oqs_sha2_sha384_inc_ctx_clone_c((sha384ctx *) dest, (const sha384ctx *) src);
}

static void SHA2_sha384_inc_blocks(OQS_SHA2_sha384_ctx *state, const uint8_t *in, size_t inblocks) {
	oqs_sha2_sha384_inc_blocks_c((sha384ctx *) state, in, inblocks);
}

static void SHA2_sha384_inc_finalize(uint8_t *out, OQS_SHA2_sha384_ctx *state, const uint8_t *in, size_t inlen) {
	oqs_sha2_sha384_inc_finalize_c(out, (sha384ctx *) state, in, inlen);
}

static void SHA2_sha384_inc_ctx_release(OQS_SHA2_sha384_ctx *state) {
	oqs_sha2_sha384_inc_ctx_release_c((sha384ctx *) state);
}

static void SHA2_sha512_inc_init(OQS_SHA2_sha512_ctx *state) {
	oqs_sha2_sha512_inc_init_c((sha512ctx *)state);
}

static void SHA2_sha512_inc_ctx_clone(OQS_SHA2_sha512_ctx *dest, const OQS_SHA2_sha512_ctx *src) {
	oqs_sha2_sha512_inc_ctx_clone_c((sha512ctx *) dest, (const sha512ctx *) src);
}

static void SHA2_sha512_inc_blocks(OQS_SHA2_sha512_ctx *state, const uint8_t *in, size_t inblocks) {
	oqs_sha2_sha512_inc_blocks_c((sha512ctx *) state, in, inblocks);
}

static void SHA2_sha512_inc_finalize(uint8_t *out, OQS_SHA2_sha512_ctx *state, const uint8_t *in, size_t inlen) {
	oqs_sha2_sha512_inc_finalize_c(out, (sha512ctx *) state, in, inlen);
}

static void SHA2_sha512_inc_ctx_release(OQS_SHA2_sha512_ctx *state) {
	oqs_sha2_sha512_inc_ctx_release_c((sha512ctx *) state);
}

static void SHA2_sha256(uint8_t *out, const uint8_t *in, size_t inlen) {
	C_OR_ARM (
	    oqs_sha2_sha256_c(out, in, inlen),
	    oqs_sha2_sha256_armv8(out, in, inlen)
	);
}

static void SHA2_sha384(uint8_t *out, const uint8_t *in, size_t inlen) {
	oqs_sha2_sha384_c(out, in, inlen);
}

static void SHA2_sha512(uint8_t *out, const uint8_t *in, size_t inlen) {
	oqs_sha2_sha512_c(out, in, inlen);
}

struct OQS_SHA2_callbacks sha2_default_callbacks = {
	SHA2_sha256,
	SHA2_sha256_inc_init,
	SHA2_sha256_inc_ctx_clone,
	SHA2_sha256_inc,
	SHA2_sha256_inc_blocks,
	SHA2_sha256_inc_finalize,
	SHA2_sha256_inc_ctx_release,
	SHA2_sha384,
	SHA2_sha384_inc_init,
	SHA2_sha384_inc_ctx_clone,
	SHA2_sha384_inc_blocks,
	SHA2_sha384_inc_finalize,
	SHA2_sha384_inc_ctx_release,
	SHA2_sha512,
	SHA2_sha512_inc_init,
	SHA2_sha512_inc_ctx_clone,
	SHA2_sha512_inc_blocks,
	SHA2_sha512_inc_finalize,
	SHA2_sha512_inc_ctx_release,
};
