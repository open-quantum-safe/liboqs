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

#ifndef XMSS_DRAFT_H
#define XMSS_DRAFT_H

#include "wots_draft.h"

/*
 * The data structures for the Hash Function Address Scheme
 * are in hfas_draft.h
 */
typedef struct {
	unsigned char	 node[64];
	uint32_t	 height; /* equal to y */
	/* equal to x if one wants to use that variable: */
	/* unsigned int position; */
}xmss_node;

/*
 * Public Key PK for XMSS(^MT)
 * The data structure to represent the public key for XMSS
 */
typedef struct {
	unsigned char	*root; /* n-byte root node of XMSS tree */
	unsigned char	*seed; /* n-byte seed */
}xmss_pk;

/*
 * Private / Secret Key Structure SK for XMSS
 * Please note that for use in crypto libraries and other applications
 * one would want this to be in an adapted data structure used by the 
 * specific library
 */
typedef struct {
	/*
	 * It's left to the implementor on what to save here.
	 * It also depends on the tree traversal and on how much you want 
	 * to save, e. g. the whole key or just part of it.
	 */

	uint32_t	 idx;
	unsigned int	 n;
	wots_param	 para;
	unsigned int	 h;
	unsigned char	*SK_PRF; /* n-byte */
	unsigned char	*root; /* n-byte root node of XMSS tree */
	unsigned char *seed; /* n-bytes seed */

	/* The easy and straightforward way of storing all nodes */
	xmss_node	*nodes;
	unsigned char	*wots_keys; /* len keys of length n */
}xmss_sk;

/* For simplified handling of sig: */
typedef struct {
	uint32_t	 idx_sig;
	unsigned char	*randomness;
	unsigned char	*sig_ots;
	unsigned char	*auth;
}xmss_sig;

void rand_hash(unsigned char *hash, unsigned char *left, unsigned char *right,
    unsigned char *seed, unsigned char adrs[32], const unsigned int n);
void ltree (unsigned char *pk, unsigned char *node, const wots_param *para,
    unsigned char *seed, unsigned char adrs[32]);
int treeHash (unsigned char *hash, xmss_sk *SK, const uint32_t s,
    const uint32_t t, unsigned char adrs[32]);
int xmss_sk_init(xmss_sk *SK, const wots_param *para, const unsigned int h);
int xmss_sk_init_core(xmss_sk *SK, const wots_param *para,
    const unsigned int h, unsigned char *sk_prf);
void xmss_sk_free(xmss_sk *SK);
int xmss_sig_init(xmss_sig *Sig, const wots_param *para, const unsigned int h);
void xmss_sig_free(xmss_sig *Sig);
int xmss_pk_init(xmss_pk *PK, const unsigned int n);
void xmss_pk_free(xmss_pk *PK);
int xmss_keyGen(xmss_sk *SK, unsigned char *pk, wots_param *para,
    const unsigned int h);
int xmss_keyGen_core (xmss_sk *SK, unsigned char *pk, wots_param *para,
    const unsigned int h,unsigned char *seed, unsigned char *PRF);
int buildAuth (xmss_sk *SK, unsigned char *auth_path, const uint32_t i);
int treeSig(unsigned char *sig_tmp, xmss_sk *SK, unsigned char *msg,
    unsigned char adrs[32]);
int xmss_signing(xmss_sk *SK, unsigned char *msg, const unsigned int msglen,
    unsigned char *sig, unsigned char adrs[32]);
int xmss_rootFromSig(xmss_sig *Sig, unsigned char *msg,
    unsigned char *node, unsigned char *seed, const unsigned int n,
    const unsigned int h, unsigned char adrs[32]);
int xmss_verification(unsigned char *pk_all, unsigned char *sig,
    unsigned char *msg, const unsigned int msglen, const wots_param *para,
    const unsigned int h);
void xmssSigToString(unsigned char *concat, xmss_sig *Sig, const int n,
    const int h, const int w);
void xmssPKtoString(unsigned char *concat, const xmss_pk *PK, const int n);
void xmssStringToSig(xmss_sig *Sig, unsigned char *string, const int n,
    const int h, const int w);
void xmssStringToPK(xmss_pk *PK, unsigned char *string, const int n);

#endif

