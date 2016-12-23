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

#include "wots_draft.h"

/* May be used for debug reasons: */
void
hex_dump(unsigned char *a, size_t len)
{
	for (size_t i = 0; i < len; i++)
		printf("%02x", a[i]);
}

void
do_xor(unsigned char *xored, const unsigned char *left,
    const unsigned char *right, const unsigned int bytes)
{
	for (unsigned int i = 0; i < bytes; i++)
		xored[i] = XOR(left[i], right[i]);
}

/* This is algorithm 1: base_w(X, w, out_len) */
void
base_w(const unsigned char *X, const unsigned int w, unsigned int out_len,
    int *bas)
{
	unsigned int in = 0, out = 0, total = 0, bits = 0, consumed = 0;

	for (consumed = 0; consumed < out_len; consumed++) {
		if (bits == 0) {
			total = X[in];
			in++;
			bits += 8;
		}
		bits -= (int)log2(w);
		bas[out] = (total >> bits) & (w - 1);
		out++;
	}
}

/* This is algorithm 2: the chaining function */
int
chain(unsigned char *chained, const unsigned char *X, const unsigned int i,
    const unsigned int s, const wots_param *para, const unsigned char *seed,
    unsigned char adrs[32])
{
	/* returns 1 if i + s is too large (greater than w - 1) */
	unsigned int n = para->n;
	if (s == 0) {
		memcpy(chained, X, n * sizeof(*chained));
		return 0;
	}
	if ((i + s) > ((para->w) - 1))
		return 1;

	unsigned char tmp[n];
	memset(tmp, 0, n * sizeof(*tmp));
	chain(tmp, X, i, s - 1, para, seed, adrs);
	setHashAddress(adrs, i + s - 1);
	setKeyAndMask(adrs, 0);
	unsigned char key[n];
	memset(key, 0, n * sizeof(*key));
	PRF(key, seed, adrs, n);
	setKeyAndMask(adrs, 1);
	unsigned char bitmask[n];
	memset(bitmask, 0, n * sizeof(*bitmask));
	PRF(bitmask, seed, adrs, n);
	unsigned char xored[n];
	memset(xored, 0, n * sizeof(*xored));
	do_xor(xored, tmp, bitmask, n);


	F(chained, xored, key, n);

	return 0;
}

/*
 * This is algorithm 3: generating the WOTS+ private key
 * Take care of a suitable random source (uniformely at random).
 */
int
genWotsPrivKey(const wots_param *para, unsigned char *sk)
{
	/* returns 1 if call to RAND_bytes failed */

	int check = 0;
	check = RAND_bytes(sk, para->n*para->len);
	return (1 != check);
}

/* This is algorithm 4: */
void
genWotsPubKey(unsigned char *pk, const wots_param *para,
    const unsigned char *sk, const unsigned char *seed, unsigned char adrs[32])
{
	unsigned int len = para->len, w = para->w;

	for (unsigned int i = 0; i < len; i++) {
		setChainAddress(adrs, i);
		chain(&(pk[i * (para->n)]), &(sk[i * (para->n)]), 0, w - 1, para,
		    seed, adrs); /* Writing to pk[i] */
	}
}

/* This is algorithm 5: */
void
genWotsSig(const unsigned char *sk, const wots_param *para,
    const unsigned char *M, unsigned char *sig, const unsigned char *seed,
    unsigned char adrs[32])
{
	unsigned int i = 0, w = para->w;
	int csum = 0;
	unsigned int len = para->len, len_1 = para->len_1, len_2 = para->len_2;
	int tmp_msg[len_1];
	memset(tmp_msg, 0, len_1 * sizeof(*tmp_msg));
	int msg[len];
	memset(msg, 0, len * sizeof(*msg));
	base_w(M, w, len_1, tmp_msg);
	unsigned int len_2_bytes = 0;
	for (i = 0; i < len_1; i++)
		csum = csum + (int)w - 1 - tmp_msg[i];
	csum = csum << (8 - (int)((int)(len_2 * log2(w)) % 8));
	len_2_bytes = ((len_2 * (int)log2(w)) + 7) / 8;
	unsigned char byte[len_2_bytes];
	memset(byte, 0, len_2_bytes * sizeof(*byte));
	toByte(byte, csum, len_2_bytes);
	int tmp[len_2];
	for (unsigned int q = 0 ; q < len_2; q++)
		tmp[q] = 0;
	base_w(byte, w, len_2, tmp);
	/* Pseudo: msg = msg || tmp; */
	concat_int(msg, tmp_msg, len_1, tmp, len_2);
	for (i = 0; i < len; i++) {
		setChainAddress(adrs, i);
		chain(&sig[i * para->n], &sk[i * para->n], 0,
		    (unsigned int)msg[i], para, seed, adrs);
	}
}

/*
 * This is algorithm 6:
 * Please note that we have a 'temporary' WOTS+ public key tmp_pk
 * which is yet to be compared to the actual WOTS+ public key. 
 * This is done by XMSS.
 */
void
wotsPubKeyFromSig(const wots_param *para, const unsigned char *M,
    const unsigned char *sig, unsigned char *tmp_pk, const unsigned char *seed,
    unsigned char adrs[32])
{
	unsigned int i = 0, w = para->w;
	int csum = 0;
	unsigned int len_2_bytes = 0;
	unsigned int len = para->len, len_1 = para->len_1, len_2 = para->len_2;
	
	/* Transform to base_w representation */
	int tmp_msg[len_1];
	memset(tmp_msg, 0, len_1 * sizeof(*tmp_msg));
	int msg[len];
	memset(msg, 0, len * sizeof(*msg));
	base_w(M, w, len_1, tmp_msg);

	/* Compute the checksum */
	for (i = 0; i < len_1; i++)
		csum = csum + (int)w - 1 - tmp_msg[i];

	/* Convert csum to a base_w representation */
	csum = csum << ( 8 - (int)((int)(len_2 * log2(w)) % 8));
	len_2_bytes = ((len_2 * (int)log2(w)) + 7) / 8;

	/* Build the actual message */
	unsigned char byte[len_2_bytes];
	memset(byte, 0, len_2_bytes * sizeof(*byte));
	toByte(byte, csum, len_2_bytes);
	int tmp[len_2];
	for (unsigned int q = 0 ; q < len_2; q++)
		tmp[q] = 0;
	base_w(byte, w, len_2, tmp);

	/* Pseudo: msg = msg || tmp; */
	concat_int(msg, tmp_msg, len_1, tmp, len_2);
	for (i = 0; i < len; i++) {
		setChainAddress(adrs, i);
		chain(&tmp_pk[i * para->n], &sig[i * para->n],
		    (unsigned int)msg[i], w - 1 - (unsigned int)msg[i],
		    para, seed, adrs);
	}
}

unsigned int
calcLen1(const unsigned int w, const unsigned int n)
{
	unsigned int len_1 =
	    (unsigned int)ceil(8.0 * (double)n / (int)log2((double)w));
	return len_1;
}

unsigned int
calcLen2(const unsigned int w, const unsigned int len_1)
{
	unsigned int len_2 = (unsigned int)floor((int)log2((double)len_1 *
	    ((double)w - 1.0)) / (int)log2((double)w)) + 1;
	return len_2;
}

/* Most trivial function, but let's go for readability: */
unsigned int
calcLen(const unsigned int len_1, const unsigned int len_2)
{
	return len_1 + len_2;
}

int
wots_param_init(wots_param *para, const unsigned int n, const unsigned int w)
{
	if (!((n == 32 || n == 64) && (w == 4 || w == 16)))
		return 1;

	para->n = n;
	para->w = w;
	para->len_1 = (unsigned int)ceil(8.0 * (double)para->n /
	    log2((double)para->w));
	para->len_2 = (unsigned int)floor(log2((double)para->len_1 *
	    ((double)para->w - 1.0)) / log2((double)para->w)) + 1;
	para->len = para->len_1 + para->len_2;

	return 0;
}

void
concat_int(int *concat, const int *left, const unsigned int bytes_left,
    const int *right, const unsigned int bytes_right)
{
	memcpy(concat, left, bytes_left * sizeof(int));
	memcpy(concat + bytes_left, right, bytes_right * sizeof(int));
}

