/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: sampling functions
**************************************************************************************/

#include "api.h"
#include "sample.h"
#include "params.h"
#include <oqs/sha3.h>

#define NBLOCKS_SHAKE (SHAKE_RATE / (((PARAM_B_BITS + 1) + 7) / 8))
#define BPLUS1BYTES (((PARAM_B_BITS + 1) + 7) / 8)

static void sample_y(poly y, const unsigned char *seed, int nonce) { // Sample polynomial y, such that each coefficient is in the range [-B,B]
	unsigned int i = 0, pos = 0, nblocks = PARAM_N;
	unsigned char buf[PARAM_N * BPLUS1BYTES + 1];
	unsigned int nbytes = BPLUS1BYTES;
	int16_t dmsp = (int16_t)(nonce << 8);
	int32_t y_t[4];

	OQS_SHA3_cshake128_simple4x((uint8_t *) buf, (uint8_t *) buf + (PARAM_N * nbytes / 4), (uint8_t *) buf + (2 * PARAM_N * nbytes / 4), (uint8_t *) buf + (3 * PARAM_N * nbytes / 4), PARAM_N * nbytes / 4, dmsp, dmsp + 1, dmsp + 2, dmsp + 3, seed, CRYPTO_RANDOMBYTES);
	dmsp += 4;

	while (i < PARAM_N) {
		if (pos > nblocks * nbytes - 4 * nbytes) {
			nblocks = NBLOCKS_SHAKE;
			cSHAKE((uint8_t *) buf, SHAKE_RATE, dmsp++, seed, CRYPTO_RANDOMBYTES);
			pos = 0;
		}
		y_t[0] = (*(uint32_t *) (buf + pos)) & ((1 << (PARAM_B_BITS + 1)) - 1);
		y_t[1] = (*(uint32_t *) (buf + pos + nbytes)) & ((1 << (PARAM_B_BITS + 1)) - 1);
		y_t[2] = (*(uint32_t *) (buf + pos + 2 * nbytes)) & ((1 << (PARAM_B_BITS + 1)) - 1);
		y_t[3] = (*(uint32_t *) (buf + pos + 3 * nbytes)) & ((1 << (PARAM_B_BITS + 1)) - 1);
		y_t[0] -= PARAM_B;
		y_t[1] -= PARAM_B;
		y_t[2] -= PARAM_B;
		y_t[3] -= PARAM_B;
		if (y_t[0] != (1 << PARAM_B_BITS))
			y[i++] = y_t[0];
		if (i < PARAM_N && y_t[1] != (1 << PARAM_B_BITS))
			y[i++] = y_t[1];
		if (i < PARAM_N && y_t[2] != (1 << PARAM_B_BITS))
			y[i++] = y_t[2];
		if (i < PARAM_N && y_t[3] != (1 << PARAM_B_BITS))
			y[i++] = y_t[3];
		pos += 4 * nbytes;
	}
}

static void encode_c(uint32_t *pos_list, int16_t *sign_list, unsigned char *c_bin) { // Encoding of c' by mapping the output of the hash function H to an N-element vector with entries {-1,0,1}
	int i, pos, cnt = 0;
	int16_t c[PARAM_N];
	unsigned char r[OQS_SHA3_SHAKE128_RATE];
	uint16_t dmsp = 0;

	// Use the hash value as key to generate some randomness
	OQS_SHA3_cshake128_simple(r, OQS_SHA3_SHAKE128_RATE, dmsp++, c_bin, CRYPTO_RANDOMBYTES);

	// Use rejection sampling to determine positions to be set in the new vector
	for (i = 0; i < PARAM_N; i++)
		c[i] = 0;

	for (i = 0; i < PARAM_H;) { // Sample a unique position k times. Use two bytes
		if (cnt > (OQS_SHA3_SHAKE128_RATE - 3)) {
			OQS_SHA3_cshake128_simple(r, OQS_SHA3_SHAKE128_RATE, dmsp++, c_bin, CRYPTO_RANDOMBYTES);
			cnt = 0;
		}
		pos = (r[cnt] << 8) | (r[cnt + 1]);
		pos = pos & (PARAM_N - 1); // Position is in the range [0,N-1]

		if (c[pos] == 0) { // Position has not been set yet. Determine sign
			if ((r[cnt + 2] & 1) == 1)
				c[pos] = -1;
			else
				c[pos] = 1;
			pos_list[i] = pos;
			sign_list[i] = c[pos];
			i++;
		}
		cnt += 3;
	}
}
