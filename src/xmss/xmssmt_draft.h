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

#ifndef XMSSMT_DRAFT_H
#define XMSSMT_DRAFT_H

#include "xmss_draft.h"

typedef struct {
	uint64_t idx_MT;	/* single (ceil(h / 8))-byte index */
	unsigned char *SK_PRF;	/* single n-byte pseudorandom function key */
	unsigned char *root;	/* Same as PK root */
	unsigned char *seed;	/* Same as PK seed */
	unsigned int d;		/* Number of XMSS tree layers */

	/* Total tree height for entire MT structure. */
	unsigned int hmt;	/* Equal to d * (plain XMSS tree height). */

	/*
	 * Easier to use existing structure and just not use its PRF key and
	 * index than to define new structure for reduced XMSS SK.
	 */
	xmss_sk *reduced_keys;
} xmss_mt_sk;

/*
 * Again, reuses existing structure xmss_sig instead of defining a new structure
 * for reduced signatures.
 */
typedef struct {
	/* index of the used WOTS+ key pair on the bottom layer */
	uint64_t idx_sig;		/* ceil(h / 8) bytes */

	/* byte string used for randomized message hashing */
	unsigned char *randomness;	/* n bytes */

	/* d reduced XMSS signatures with message length n */
	xmss_sig *reduced_sigs;		/* (h + len) * n bytes */
} xmss_mt_sig;

void getXMSS_SK(xmss_sk *R, xmss_mt_sk *SK_MT, unsigned int x, unsigned int y);
int xmss_mt_sk_init(xmss_mt_sk *SK_MT, wots_param *para, unsigned int hmt,
    unsigned int d);
void xmss_mt_sk_free(xmss_mt_sk *SK_MT);
int xmss_mt_sig_init(xmss_mt_sig *Sig, wots_param *para, unsigned int h,
    unsigned int d);
void xmss_mt_sig_free(xmss_mt_sig *Sig, unsigned int d);
int xmss_mt_keyGen(xmss_mt_sk *SK_MT, unsigned char *pk, wots_param *para,
    const unsigned int h, const unsigned int d);
int getMSB(uint64_t source, uint64_t *extracted, unsigned int n,
    unsigned int h);
int getLSB(uint64_t source, uint64_t *extracted, unsigned int n,
    unsigned int h);
int xmss_mt_sign(xmss_mt_sk *SK_MT, unsigned char *msg, unsigned int msglen,
    unsigned char *sig);
int xmss_mt_verify(unsigned char *pk_all, unsigned char *sig,
    unsigned char *msg, unsigned int msglen, wots_param *para, unsigned int h,
    unsigned int d);
void xmss_mt_StringToSig(xmss_mt_sig *Sig, unsigned char *string,
    unsigned int n, unsigned int h, unsigned int w, unsigned int d);
int xmss_mt_SigToString(unsigned char *concat, xmss_mt_sig *Sig,
    unsigned int n, unsigned int h, unsigned int w, unsigned int d);

#endif

