/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#include "sha.h"
#include "utilities.h"
#include "stdlib.h"
#include "byteswap.h"

#define SHA_RES_BIG_ENDIAN

#define rotr(X, imm) ((X >> imm) ^ (X << (64 - imm)))
_INLINE_ uint32_t sha_update(sha512_hash_t *hash, const uint64_t *msg, uint32_t n) {

	volatile uint64_t a, b, c, d, e, f, g, h;
	volatile uint64_t s0, s1, S0, S1, maj, ch, t1, t2;
	uint64_t w[80];
	int i;

	uint64_t k[] = K;

	if (NULL == hash || NULL == msg) {
		return 0;
	}

	while (n > 0) {
		a = hash->u.qw[0];
		b = hash->u.qw[1];
		c = hash->u.qw[2];
		d = hash->u.qw[3];
		e = hash->u.qw[4];
		f = hash->u.qw[5];
		g = hash->u.qw[6];
		h = hash->u.qw[7];

		for (i = 0; i < 80; i++) {
			if (i < 16) {
				w[i] = bswap_64(msg[i]);
			} else {
				S0 = rotr(w[(i - 15)], 1) ^ rotr(w[(i - 15)], 8) ^ (w[(i - 15)] >> 7);
				S1 = rotr(w[(i - 2)], 19) ^ rotr(w[(i - 2)], 61) ^ (w[(i - 2)] >> 6);
				w[i] = w[(i - 16)] + S0 + w[(i - 7)] + S1;
			}
			s0 = rotr(a, 28) ^ rotr(a, 34) ^ rotr(a, 39);
			s1 = rotr(e, 14) ^ rotr(e, 18) ^ rotr(e, 41);
			maj = (b & c) | (a & (b ^ c));
			ch = g ^ (e & (f ^ g));
			t2 = s0 + maj;
			t1 = h + s1 + ch + k[i] + w[i];

			h = g;
			g = f;
			f = e;
			e = d + t1;
			d = c;
			c = b;
			b = a;
			a = t1 + t2;
		}

		hash->u.qw[0] += a;
		hash->u.qw[1] += b;
		hash->u.qw[2] += c;
		hash->u.qw[3] += d;
		hash->u.qw[4] += e;
		hash->u.qw[5] += f;
		hash->u.qw[6] += g;
		hash->u.qw[7] += h;

		n--;
		msg += 16;
	}

	a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;
	s0 = 0, s1 = 0, S0 = 0, S1 = 0, maj = 0, ch = 0, t1 = 0, t2 = 0;
	OQS_MEM_cleanse(w, sizeof(w));
	return 1;
}

int sha(OUT sha_hash_t *hash_out,
        IN const uint32_t byte_length,
        IN const uint8_t *msg) {

	uint64_t i;
	uint32_t last_length;
	sha512_hash_t hash = {.u.qw = INIT_HASH};
	uint8_t last_block[(2 * HASH_BLOCK_SIZE)];

	if ((NULL == hash_out) || (NULL == msg)) {
		return 0;
	}

	if ((byte_length % HASH_BLOCK_SIZE) < 112) {
		last_length = HASH_BLOCK_SIZE;
	} else {
		last_length = (2 * HASH_BLOCK_SIZE);
	}

	for (i = 0; i < (byte_length % HASH_BLOCK_SIZE); i++) {
		last_block[i] = msg[(byte_length / HASH_BLOCK_SIZE) * HASH_BLOCK_SIZE + i];
	}

	last_block[i++] = 0x80;

	for (; i < last_length - 8; i++) {
		last_block[i] = 0;
	}

	((uint64_t *) last_block)[(last_length / 8) - 1] = bswap_64(byte_length * 8);

	sha_update(&hash, (uint64_t *) msg, byte_length / HASH_BLOCK_SIZE);
	sha_update(&hash, (uint64_t *) last_block, last_length / HASH_BLOCK_SIZE);

#ifdef SHA_RES_BIG_ENDIAN
	for (i = 0; i < (sizeof(sha_hash_t) / 8); i++) {
		hash_out->u.qw[i] = bswap_64(hash.u.qw[i]);
	}
#endif

	OQS_MEM_cleanse(hash.u.raw, sizeof(hash));
	OQS_MEM_cleanse(last_block, sizeof(last_block));

	return 1;
}
