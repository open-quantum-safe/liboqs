/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
*
* Multi block SHA384 AVX2/AVX512 is due to: 
* "Multi Block (MB) SHA384 for x86_64 architectures that support 
* AVX2/AVX512 instructions set" by Shay Gueron and Regev Shemy
* https://mta.openssl.org/pipermail/openssl-dev/2016-August/008238.html
* See: https://mta.openssl.org/pipermail/openssl-dev/2016-August/008238.html
* ***************************************************************************/

#include <string.h>
#include "sha.h"
#include "utilities.h"

// Always use SHA384 code other versions are curreently unsupported
#define SHA384

_INLINE_ void sha_mb_init(OUT sha_mb_ctx *ctx,
                          IN const uint32_t num) {
	block_t init_hash[] = INIT_HASH;

	for (uint32_t i = 0; i < num; i++) {
		ctx->A[i] = init_hash[0];
		ctx->B[i] = init_hash[1];
		ctx->C[i] = init_hash[2];
		ctx->D[i] = init_hash[3];
		ctx->E[i] = init_hash[4];
		ctx->F[i] = init_hash[5];
		ctx->G[i] = init_hash[6];
		ctx->H[i] = init_hash[7];
	}
}

_INLINE_ void sha_mb_update(IN OUT sha_mb_ctx *ctx,
                            IN const uint8_t *data,
                            IN const uint32_t len,
                            IN const uint32_t num_buffers) {
	if (0 == len) {
		return;
	}

	uint8_t i;
	uint32_t frag = (len / num_buffers);
	uint32_t last = (frag + (len % num_buffers)) / HASH_BLOCK_SIZE;

	if (frag > 0) {
		hash_desc hdesc[num_buffers];
		hdesc[0].ptr = (uint8_t *) data;
		hdesc[0].blocks = frag / HASH_BLOCK_SIZE;

		for (i = 1; i < (num_buffers - 1); i++) {
			hdesc[i].ptr = (uint8_t *) data + (i * frag);
			hdesc[i].blocks = frag / HASH_BLOCK_SIZE;
		}

		hdesc[i].ptr = (uint8_t *) data + (i * frag);
		hdesc[i].blocks = last;

		sha_mb_avx(ctx, hdesc, num_buffers / 4);
	}
}

_INLINE_ uint32_t sha_prepare_last_block(OUT uint8_t *last_block,
                                         IN const uint8_t *msg,
                                         IN const uint32_t message_length) {
	uint32_t i, last_length;
	uint32_t loc;
	uint32_t ret;

	ret = ((message_length % HASH_BLOCK_SIZE) <= SLICE_REM) ? 1 : 2;
	last_length = ret * HASH_BLOCK_SIZE;
	loc = message_length - (message_length % HASH_BLOCK_SIZE);

	for (i = 0; i < (message_length % HASH_BLOCK_SIZE); i++) {
		last_block[i] = msg[loc + i];
	}

	last_block[i++] = 0x80;

	for (; i < (last_length - 8); i++) {
		last_block[i] = 0;
	}

	((uint64_t *) last_block)[(last_length / 8) - 1] = bswap_64((uint64_t) message_length * 8ULL);

	return ret;
}

_INLINE_ void sha_mb_final(OUT sha_hash_t digest[],
                           IN OUT sha_mb_ctx *ctx,
                           IN const uint32_t num_buffers,
                           IN const uint8_t *in,
                           IN const uint32_t len) {
	uint32_t i, block_length;
	uint8_t lastblock[num_buffers][(2 * HASH_BLOCK_SIZE)];
	block_length = (len / num_buffers);

	memset(lastblock, 0, 2 * num_buffers * HASH_BLOCK_SIZE);

	hash_desc hdesc[num_buffers];
	for (i = 0; i < num_buffers - 1; i++) {
		hdesc[i].ptr = lastblock[i];
		hdesc[i].blocks = sha_prepare_last_block(lastblock[i], in + i * block_length, block_length);
	}

	hdesc[i].ptr = lastblock[num_buffers - 1];
	hdesc[i].blocks = sha_prepare_last_block(lastblock[num_buffers - 1], in + i * block_length, block_length + (len % num_buffers));

	sha_mb_avx(ctx, hdesc, num_buffers / 4);

	for (i = 0; i < num_buffers; i++) {
		digest[i].u.qw[0] = bswap_64(ctx->A[i]);
		digest[i].u.qw[1] = bswap_64(ctx->B[i]);
		digest[i].u.qw[2] = bswap_64(ctx->C[i]);
		digest[i].u.qw[3] = bswap_64(ctx->D[i]);
		digest[i].u.qw[4] = bswap_64(ctx->E[i]);
		digest[i].u.qw[5] = bswap_64(ctx->F[i]);
#ifndef SHA384
		digest[i].u.qw[6] = bswap_64(ctx->G[i]);
		digest[i].u.qw[7] = bswap_64(ctx->H[i]);
#endif
	}
}

void sha_mb(OUT sha_hash_t hash_out[],
            IN const uint8_t *msg,
            IN const uint32_t byte_length,
            IN const uint32_t num) {
	ALIGN(64)
	sha_mb_ctx ctx = {0};
	sha_mb_init(&ctx, num);
	sha_mb_update(&ctx, msg, byte_length, num);
	sha_mb_final(hash_out, &ctx, num, msg, byte_length);
}
