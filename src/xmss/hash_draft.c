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

#include "hash_draft.h"

void
toByte(uint8_t *buf, const uint32_t x, const uint32_t y)
{
	/*
	 * If x and y are non-negative integers, we define Z = toByte(x, y) to
	 * be the y-byte string containing the binary representation of x in big
	 * endian byte-order.
	 */

	buf[y - 1] = x & 0xFF;
	if ( y > 1 )
		buf[y - 2] = (x >> 8) & 0xFF;
	if ( y > 2)
		buf[y - 3] = (x >> 16) & 0xFF;
	if ( y > 3 )
		buf[y - 4] = (x >> 24) & 0xFF;
	if ( y > 4 )
		memset(&buf[0], 0, y * sizeof(*buf) - 4);
}

void
hash256(unsigned char *out, const unsigned char *in,
    const unsigned int inlen, const int algo)
{
	if (algo == 2) {
		SHA256(in, inlen, out);
	} else if (algo == 3) {
		SHAKE128(out, 32, in, inlen);
	}
}

void
hash512(unsigned char *out, const unsigned char *in,
    const unsigned int inlen, const int algo)
{
	if (algo == 2) {
		SHA512(in, inlen, out);
	} else if (algo == 3) {
		SHAKE256(out, 64, in, inlen);
	}
}

int
F(unsigned char *out, const unsigned char *in, const unsigned char *key,
    const unsigned int len)
{
	/*
	 * returns 1 if the message digest length is not 32 or 64 bytes
	 * both 'len' and 'in' are of length n
	 */
	unsigned char byte[len], left[3 * len], right[2 * len];

	/* toByte(0, len) */
	memset(byte, 0, sizeof(byte));
	memset(left, 0, sizeof(left));
	memset(right, 0, sizeof(right));
	concat_bytes(right, key, len, in, len);
	concat_bytes(left, byte, len, right, 2 * len);
	if (len == 32) {
		/* toByte(0, 32) || key || M */
		hash256(out, left, 3 * len, SHA);
	} else if (len == 64) {
		/* toByte(0, 64) || key || M */
		hash512(out, left, 3 * len, SHA);
	} else {
		return 1;
	}

	return 0;
}

int
H(unsigned char *out, const unsigned char *in, const unsigned char *key,
    const unsigned int len)
{
	/*
	 * returns 1 if the message digest length is not 32 or 64 bytes
	 * 'len' is of length n; 'in' is of length 2 * n
	 */
	unsigned char byte[len], left[4 * len], right[3 * len];

	/* toByte(1, len) */
	memset(byte, 0, sizeof(byte));
	toByte(byte, 1, len);
	memset(left, 0, sizeof(left));
	memset(right, 0, sizeof(right));
	concat_bytes(right, key, len, in, 2 * len);
	concat_bytes(left, byte, len, right, 3 * len);
	if (len == 32) {
		/* toByte(1, 32) || key || M */
		hash256(out, left, 4 * len, SHA);
	} else if (len == 64) {
		/* toByte(1, 64) || key || M */
		hash512(out, left, 4 * len, SHA);
	}
	else {
		return 1;
	}

	return 0;
}

int
H_msg(unsigned char *out, const unsigned char *in, const unsigned int msglen,
    const unsigned char *key, const unsigned int len)
{
	/* returns 1 if the message digest length is not 32 or 64 bytes */
	unsigned char tmp[4 * len + msglen];
	unsigned char right[3 * len + msglen];
	unsigned char byte[len];
	memset(tmp, 0, sizeof(tmp));
	memset(right, 0, sizeof(right));
	memset(byte, 0, sizeof(byte));
	toByte(byte, 2, len);
	concat_bytes(right, key, 3 * len, in, msglen);
	concat_bytes(tmp, byte, len, right, 3 * len + msglen);
	if (len == 32) {
		/* toByte(2, 32) || KEY || M */
		hash256(out, tmp, 4 * len + msglen, SHA);
	} else if (len == 64) {
		/* toByte(2, 64) || KEY || M */
		hash512(out, tmp, 4 * len + msglen, SHA);
	} else {
		return 1;
	}

	return 0;
}

void
concat_bytes(unsigned char *concat, const unsigned char *left,
    const unsigned int bytes_left, const unsigned char *right,
    const unsigned int bytes_right)
{
	memmove(concat, left, bytes_left);
	memmove(concat + bytes_left, right, bytes_right);
}

