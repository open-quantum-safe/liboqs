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

#ifndef WOTS_DRAFT_H
#define WOTS_DRAFT_H

#include <openssl/rand.h>
#include <math.h>

#include "prf_draft.h"

#define XOR(v, w) ((v) ^ (w))

typedef struct {
	/* length in bytes (message digest, secret key,
	   public key, signature element): */
	unsigned int	 n;
	unsigned int	 w; /* Winternitz parameter */
	/* number of n-byte string elements in WOTS+ (sk, pk, sig element) */
	unsigned int	 len;
	unsigned int	 len_1;
	unsigned int	 len_2;
} wots_param;

void hex_dump(unsigned char *a, size_t len);
void do_xor(unsigned char *xored, const unsigned char *left,
    const unsigned char *right, const unsigned int bytes);
void base_w(const unsigned char *X, const unsigned int w, unsigned int out_len,
    int *bas);
int chain(unsigned char *chained, const unsigned char *X, const unsigned int i,
    const unsigned int s, const wots_param *para, const unsigned char *seed,
    unsigned char adrs[32]);
int genWotsPrivKey(const wots_param *para, unsigned char *sk);
void genWotsPubKey(unsigned char *pk, const wots_param *para,
    const unsigned char *sk, const unsigned char *seed, unsigned char adrs[32]);
void genWotsSig(const unsigned char *sk, const wots_param *para,
    const unsigned char *M, unsigned char *sig, const unsigned char *seed,
    unsigned char adrs[32]);
void wotsPubKeyFromSig(const wots_param *para, const unsigned char *M,
    const unsigned char *sig, unsigned char *tmp_pk, const unsigned char *seed,
    unsigned char adrs[32]);

unsigned int calcLen1(const unsigned int w, const unsigned int n);
unsigned int calcLen2(const unsigned int w, const unsigned int len_1);
unsigned int calcLen(const unsigned int len_1, const unsigned int len_2);
int wots_param_init(wots_param *para, const unsigned int n,
    const unsigned int w);
void concat_int(int *concat, const int *left, const unsigned int bytes_left,
    const int *right, const unsigned int bytes_right);

#endif

