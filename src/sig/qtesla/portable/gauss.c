/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: portable, constant-time Gaussian sampler
*
* SPDX-License-Identifier: MIT
**************************************************************************************/

#include <string.h>
#include "api.h"
#include <oqs/sha3.h>
#include "gauss.h"
#include "CDT32.h"


void sample_gauss_poly(poly z, const unsigned char *seed, int nonce) {
	int dmsp = nonce << 8;
	int32_t samp[CHUNK_SIZE * CDT_COLS], c[CDT_COLS], borrow, sign;
	const int32_t mask = (int32_t)((uint32_t)(-1) >> 1);

	for (int chunk = 0; chunk < PARAM_N; chunk += CHUNK_SIZE) {
		cSHAKE((uint8_t *)samp, CHUNK_SIZE * CDT_COLS * sizeof(int32_t), (int16_t)dmsp++, seed, CRYPTO_RANDOMBYTES);
		for (int i = 0; i < CHUNK_SIZE; i++) {
			z[chunk + i] = 0;
			for (int j = 1; j < CDT_ROWS; j++) {
				borrow = 0;
				for (int k = CDT_COLS - 1; k >= 0; k--) {
					c[k] = (samp[i * CDT_COLS + k] & mask) - (cdt_v[j * CDT_COLS + k] + borrow);
					borrow = c[k] >> (RADIX32 - 1);
				}
				z[chunk + i] += ~borrow & 1;
			}
			sign = samp[i * CDT_COLS] >> (RADIX32 - 1);
			z[chunk + i] = (sign & -z[chunk + i]) | (~sign & z[chunk + i]);
		}
	}
}
