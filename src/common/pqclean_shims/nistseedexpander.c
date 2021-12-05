//
//  rng.c
//
//  Created by Bassham, Lawrence E (Fed) on 8/29/17.
//  Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
/*
NIST-developed software is provided by NIST as a public service. You may use, copy, and distribute copies of the software in any medium, provided that you keep intact this entire notice. You may improve, modify, and create derivative works of the software or any portion of the software, and you may copy and distribute such modifications or works. Modified works should carry a notice stating that you changed the software and should note the date and nature of any such change. Please explicitly acknowledge the National Institute of Standards and Technology as the source of the software.

NIST-developed software is expressly provided "AS IS." NIST MAKES NO WARRANTY OF ANY KIND, EXPRESS, IMPLIED, IN FACT, OR ARISING BY OPERATION OF LAW, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND DATA ACCURACY. NIST NEITHER REPRESENTS NOR WARRANTS THAT THE OPERATION OF THE SOFTWARE WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT ANY DEFECTS WILL BE CORRECTED. NIST DOES NOT WARRANT OR MAKE ANY REPRESENTATIONS REGARDING THE USE OF THE SOFTWARE OR THE RESULTS THEREOF, INCLUDING BUT NOT LIMITED TO THE CORRECTNESS, ACCURACY, RELIABILITY, OR USEFULNESS OF THE SOFTWARE.

You are solely responsible for determining the appropriateness of using and distributing the software and you assume all risks associated with its use, including but not limited to the risks and costs of program errors, compliance with applicable laws, damage to or loss of data, programs or equipment, and the unavailability or interruption of operation. This software is not intended to be used in any situation where a failure could cause risk of injury or damage to property. The software developed by NIST employees is not subject to copyright protection within the United States.
*/
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
