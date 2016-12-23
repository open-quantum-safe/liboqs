/*
 * Copyright (c) 2016, Denis Butin and Stefan-Lukas Gazdag
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

#include "prf_draft.h"

/*
 * PRF takes as input an n-byte key and a 32-byte index
 * and generates pseudorandom outputs (r) of length n
 */

int
PRF(unsigned char *r, const unsigned char *key, const unsigned char *index,
    const unsigned int n)
{
	unsigned char byte[n], left[2 * n + 32], right[n + 32];

	/* returns 1 if input length n is not equal to 32 or 64 */

	/* toByte(3,len) */
	memset(byte, 0, sizeof(byte));
	toByte(byte, 3, n);
	memset(left, 0, sizeof(left));
	memset(right, 0, sizeof(right));
	concat_bytes(right, key, n, index, 32);
	concat_bytes(left, byte, n, right, n + 32);

	if (n == 32) {
		/* toByte(3,32) || key || M */
		hash256(r, left, 2 * n + 32, SHA);
	} else if (n == 64) {
		/* toByte(3,64) || key || M */
		hash512(r, left, 2 * n + 32, SHA);
	} else {
		return 1;
	}

	return 0;
}

