//
//  rng.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
//  SPDX-License-Identifier: Unknown
//  Modified for PQClean by Sebastian Verschoor
//

#include "nistseedexpander.h"
#include "aes.h"
#include <string.h>

/*
 seedexpander_init()
 ctx            - stores the current state of an instance of the seed expander
 seed           - a 32 byte random value
 diversifier    - an 8 byte diversifier
 maxlen         - maximum number of bytes (less than 2**32) generated under this seed and diversifier
 */
int
seedexpander_init(AES_XOF_struct *ctx,
                  const uint8_t *seed,
                  const uint8_t *diversifier,
                  size_t maxlen) {
	ctx->length_remaining = maxlen;

	memcpy(ctx->key, seed, 32);
	memcpy(ctx->ctr, diversifier, 8);

	ctx->ctr[11] = (uint8_t) (maxlen % 256);
	maxlen >>= 8;
	ctx->ctr[10] = (uint8_t) (maxlen % 256);
	maxlen >>= 8;
	ctx->ctr[9] = (uint8_t) (maxlen % 256);
	maxlen >>= 8;
	ctx->ctr[8] = (uint8_t) (maxlen % 256);
	memset(ctx->ctr + 12, 0x00, 4);

	ctx->buffer_pos = 16;
	memset(ctx->buffer, 0x00, 16);

	return RNG_SUCCESS;
}

static void AES256_ECB(uint8_t *key, uint8_t *ctr, uint8_t *buffer) {
	aes256ctx ctx;
	aes256_ecb_keyexp(&ctx, key);
	aes256_ecb(buffer, ctr, 1, &ctx);
	aes256_ctx_release(&ctx);
}

/*
 seedexpander()
    ctx  - stores the current state of an instance of the seed expander
    x    - returns the XOF data
    xlen - number of bytes to return
 */
int
seedexpander(AES_XOF_struct *ctx, uint8_t *x, size_t xlen) {
	size_t offset;

	if ( x == NULL ) {
		return RNG_BAD_OUTBUF;
	}
	if ( xlen >= ctx->length_remaining ) {
		return RNG_BAD_REQ_LEN;
	}

	ctx->length_remaining -= xlen;

	offset = 0;
	while ( xlen > 0 ) {
		if ( xlen <= (16 - ctx->buffer_pos) ) { // buffer has what we need
			memcpy(x + offset, ctx->buffer + ctx->buffer_pos, xlen);
			ctx->buffer_pos += xlen;

			return RNG_SUCCESS;
		}

		// take what's in the buffer
		memcpy(x + offset, ctx->buffer + ctx->buffer_pos, 16 - ctx->buffer_pos);
		xlen -= 16 - ctx->buffer_pos;
		offset += 16 - ctx->buffer_pos;

		AES256_ECB(ctx->key, ctx->ctr, ctx->buffer);
		ctx->buffer_pos = 0;

		//increment the counter
		for (size_t i = 15; i >= 12; i--) {
			if ( ctx->ctr[i] == 0xff ) {
				ctx->ctr[i] = 0x00;
			} else {
				ctx->ctr[i]++;
				break;
			}
		}

	}

	return RNG_SUCCESS;
}
