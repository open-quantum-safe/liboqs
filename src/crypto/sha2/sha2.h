#ifndef OQS_SHA2_H
#define OQS_SHA2_H

#include <stddef.h>
#include <stdint.h>

/* The incremental API allows hashing of individual input blocks; these blocks
    must be exactly 64 bytes each.
    Use the 'finalize' functions for any remaining bytes (possibly over 64). */

void OQS_SHA2_sha224_inc_init(uint8_t *state);
void OQS_SHA2_sha224_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void OQS_SHA2_sha224_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void OQS_SHA2_sha224(uint8_t *out, const uint8_t *in, size_t inlen);

void OQS_SHA2_sha256_inc_init(uint8_t *state);
void OQS_SHA2_sha256_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void OQS_SHA2_sha256_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void OQS_SHA2_sha256(uint8_t *out, const uint8_t *in, size_t inlen);

void OQS_SHA2_sha384_inc_init(uint8_t *state);
void OQS_SHA2_sha384_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void OQS_SHA2_sha384_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void OQS_SHA2_sha384(uint8_t *out, const uint8_t *in, size_t inlen);

void OQS_SHA2_sha512_inc_init(uint8_t *state);
void OQS_SHA2_sha512_inc_blocks(uint8_t *state, const uint8_t *in, size_t inblocks);
void OQS_SHA2_sha512_inc_finalize(uint8_t *out, uint8_t *state, const uint8_t *in, size_t inlen);
void OQS_SHA2_sha512(uint8_t *out, const uint8_t *in, size_t inlen);

#endif
