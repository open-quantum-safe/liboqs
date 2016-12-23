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

#include "xmssmt_draft.h"

/*
 * Outputs the reduced secret key of the x^th XMSS tree on the y^th layer.
 * We use the existing XMSS SK structure to store the reduced SK, instead of
 * a new structure, which would in turn require a second version of treeHash.
 */
void
getXMSS_SK(xmss_sk *R, xmss_mt_sk *SK_MT, unsigned int x, unsigned int y)
{
	/* Find the array index of the relevant reduced key */
	int count = -1;
	for (unsigned int j = SK_MT->d ; j > y + 1; j--)
		count += (1 << (SK_MT->hmt - j * (SK_MT->hmt / SK_MT->d )));
	count += x + 1;

	/*
	 * We do not get the index from reduced_keys, since reduced keys don't
	 * have indexes. Same for SK_PRF.
	 */
	R->n = SK_MT->reduced_keys[count].n;
	R->para = SK_MT->reduced_keys[count].para;
	R->h = SK_MT->hmt / SK_MT->d;

	for (unsigned int i = 0 ; i < (unsigned int)((1<<(R->h + 1)) - 1); i++){
		memcpy(R->nodes[i].node,
		    SK_MT->reduced_keys[count].nodes[i].node, R->n);
		R->nodes[i].height = SK_MT->reduced_keys[count].nodes[i].height;
	}
	memcpy(R->wots_keys, SK_MT->reduced_keys[count].wots_keys, R->para.len
			* R->n * (1 << R->h) * sizeof(*R->wots_keys));
}

/* Initialize the XMSS^MT secret key SK_MT */
int
xmss_mt_sk_init(xmss_mt_sk *SK_MT, wots_param *para, unsigned int hmt,
    unsigned int d)
{
	/*
	 * returns 1 if a problem with calloc occurred
	 * returns 2 if the height hmt is less than 2
	 * returns 3 if d does not divide height hmt withouth remainder
	 * returns 4 if the call to RAND_bytes for SK_PRF failed
	 * returns 5 if the call to RAND_bytes for seed failed
	 */
	if (hmt < 2)
		return 2;

	if (hmt % d != 0)
		return 3;

	SK_MT->d = d;		/* Initialise d */
	SK_MT->hmt = hmt;	/* Initialise hmt */
	SK_MT->idx_MT = 0;	/* Initialise global index */

	/* Initialise SK_PRF */
	if ((SK_MT->SK_PRF = calloc(para->n, sizeof(*SK_MT->SK_PRF))) == NULL)
		return 1;

	/* Initialise root */
	if ((SK_MT->root = calloc(para->n, sizeof(*SK_MT->root))) == NULL)
		return 1;

	/* set SK_PRF to a uniformly random n-byte string */
	int rcprf = RAND_bytes(SK_MT->SK_PRF, para->n);
 	if (rcprf != 1)
		return 4;

	/* Initialise seed */
	if ((SK_MT->seed = calloc(para->n, sizeof(*SK_MT->seed))) == NULL)
		return 1;

	/* set SEED to a uniformly random n-byte string */
	unsigned int rcseed = RAND_bytes(SK_MT->seed, para->n);
	if (rcseed != 1)
		return 5;

	/*
	 * Initialise the XMSS reduced_keys in the XMSS^MT SK
	 * There are 2^(h - (i * h) / d) XMSS trees at layer i, i between 1 and
	 * d and there is one XMSS reduced key per XMSS tree
	 */
	unsigned int num = 0;
	for (unsigned int j = 1 ; j < d + 1; j++)
		num = num + (1 << (hmt - j * (hmt / d)));

	if ((SK_MT->reduced_keys = calloc(num, sizeof(*SK_MT->reduced_keys)))
	    == NULL)
		return 1;

	unsigned int count = 0;
	unsigned char adrs[32];
	memset(adrs, 0, sizeof(adrs));
	for (int32_t layer = (int32_t)(d - 1) ; layer > -1 ; layer--) {
		setLayerAddress(adrs, layer);
		for (unsigned long long int tree = 0 ; tree <
		    (unsigned long long int) (1 << (((int32_t)d - 1 - layer)
		     * (hmt / d))); tree++) {
			setTreeAddress(adrs, tree);
			xmss_sk_init(&(SK_MT->reduced_keys[count]), para,
			    hmt / d);
			memcpy(SK_MT->reduced_keys[count].SK_PRF, SK_MT->SK_PRF,
			    para->n);
			/* Will now call treeHash to compute nodes */
			unsigned char root[para->n];
			memset(root, 0, para->n);
			memcpy(SK_MT->reduced_keys[count].seed, SK_MT->seed,
			    para->n);
			treeHash(root, &(SK_MT->reduced_keys[count]), 0,
			    hmt / d, adrs);
			count++;
		}
	}
	return 0;
}

void
xmss_mt_sk_free(xmss_mt_sk *SK_MT)
{
	unsigned int num = 0;

	free(SK_MT->SK_PRF);
	free(SK_MT->root);
	free(SK_MT->seed);

	for (unsigned int j = 1 ; j < SK_MT->d + 1; j++)
		num = num + (1 << (SK_MT->hmt - j * (SK_MT->hmt / SK_MT->d)));

	for (unsigned int i = 0 ; i < num ; i++)
		xmss_sk_free(&(SK_MT->reduced_keys[i]));

	free(SK_MT->reduced_keys);
}

int
xmss_mt_sig_init(xmss_mt_sig *Sig, wots_param *para, unsigned int h,
    unsigned int d)
{
	/*
	 * returns 1 if a problem with calloc occurred
	 * returns 2 if the initalization of xmss_sig failed
	 */

	Sig->idx_sig = 0;
	Sig->randomness = calloc(para->n, sizeof(*Sig->randomness));
	if (Sig->randomness == NULL)
		return 1;

	Sig->reduced_sigs = calloc(d, sizeof(*Sig->reduced_sigs));
	if (Sig->reduced_sigs == NULL)
		return 1;

	for (unsigned int i = 0 ; i < d; i++) {
		if ((xmss_sig_init(&(Sig->reduced_sigs[i]), para, h / d)) == 1)
			return 2;
	}

	return 0;
}

void
xmss_mt_sig_free(xmss_mt_sig *Sig, unsigned int d)
{
	free(Sig->randomness);
	for (unsigned int i = 0 ; i < d; i++)
		xmss_sig_free(&(Sig->reduced_sigs[i]));

	free(Sig->reduced_sigs);
}

/*
 * Algorithm 15: XMSSMT_keyGen
 * Generate an XMSS^MT key pair
 */
int xmss_mt_keyGen(xmss_mt_sk *SK_MT, unsigned char *pk, wots_param *para,
    const unsigned int h, const unsigned int d)
{
	/*
	 * returns 1 if a problem with the initialization
	 *           of the XMSS^MT PK occurred
	 * returns 2 if a problem with the initialization
	 *           of the XMSS^MT SK occurred
	 */

	unsigned char adrs[32];
	memset(adrs, 0, sizeof(adrs)); /* toByte(adrs, 0, 32) */

	/* ADRS.setLayerAddress(d - 1) */
	setLayerAddress(adrs, (uint32_t)(d - 1));

	if ((xmss_mt_sk_init(SK_MT, para, h, d)) == 1)
		return 2;

	unsigned int n = para->n;

	/*
	 * We return a string for the PK, but work with structures
	 * internally.
	 */
	xmss_pk PK; /* Formats of XMSS_MT PK and XMSS PK are identical */

	/* Initialise the XMSS_MT PK */
	if ((xmss_pk_init(&PK, n)) == 1)
		return 1;

	xmss_sk R;	/* To store a reduced XMSS SK extracted from SK_MT */
	if (xmss_sk_init(&R, &(SK_MT->reduced_keys[0].para), SK_MT->hmt / d)
	    == 1)
		return 2;

	getXMSS_SK(&R, SK_MT, 0, d - 1); /* Store reduced secret key in R */

	/* PK_MT = root || SEED */
	memcpy(PK.seed, SK_MT->seed, n * sizeof(*PK.seed));

	unsigned char root[n];	/* This will store the result of treeHash */
	memset(root, 0, n * sizeof(*root));
	memcpy(R.seed, SK_MT->seed, n);
	treeHash(root, &R, 0, SK_MT->hmt / d, adrs); /* Compute root */

	/* PK_MT = root || SEED */
	memcpy(PK.root, root, n * sizeof(*PK.root));
	memcpy(SK_MT->root, root, n * sizeof(*SK_MT->root));

	/* Can use legacy function since same format as XMSS PK */
	xmssPKtoString(pk, &PK, n);

	/* Free the struct, since we end up working with strings */
	xmss_pk_free(&PK);
	xmss_sk_free(&R);

	return 0;
}

/*
 * Extract the n most significant bits from source, store them in extracted.
 * Source is assumed to have length h bits -- this is used for idx_sig
 */
int
getMSB(uint64_t source, uint64_t *extracted, unsigned int n, unsigned int h)
{
	/*
	 * returns 1 if one tries to extract 0 bits
	 * returns 2 if one tries do extract more bits than possible
	 * returns 3 if one tries to extract more than 64 bits
	 */

	if (n == 0)
		return 1;

	if (n > h)
		return 2;

	if (h > 64)
		return 3;

	*extracted = source << (64 - h);
	*extracted = *extracted >> (64 - n);

	return 0;
}

/* Like getMSB, but for the n least significant bits. */
int
getLSB(uint64_t source, uint64_t *extracted, unsigned int n, unsigned int h)
{
	/*
	 * returns 1 if one tries to extract 0 bits
	 * returns 2 if one tries do extract more bits than possible
	 * returns 3 if one tries to extract more than 64 bits
	 */

	if (n == 0)
		return 1;

	if (n > h)
		return 2;

	if (h > 64)
		return 3;

	*extracted = source << (64 - n);
	*extracted = *extracted >> (64 - n);

	return 0;
}

/*
 * Algorithm 16: XMSSMT_sign
 * Generate an XMSS^MT signature and update the XMSS^MT secret key
 */
int
xmss_mt_sign(xmss_mt_sk *SK_MT, unsigned char *msg, unsigned int msglen,
    unsigned char *sig)
{
	/*
	 * returns 1 if the index is too large for the tree
	 * returns 2 if the initialization of xmss_mt_sig failed
	 */
	unsigned int n = SK_MT->reduced_keys[0].n;
	unsigned char adrs[32], SK_PRF[n], sk_root[n];
	memset(adrs, 0, sizeof(adrs)); /* ADRS = toByte(0, 32) */
	/* SK_PRF = getSK_PRF(SK_MT) */
	memcpy(SK_PRF, SK_MT->SK_PRF, sizeof(SK_PRF));
	memcpy(sk_root, SK_MT->root, sizeof(sk_root));

	uint64_t idx_sig = SK_MT->idx_MT; /* idx_sig = getIdx(SK_MT) */
	unsigned int h = SK_MT->hmt, d = SK_MT->d;

	if (idx_sig >= (uint64_t)(1 << h))
		return 1;

	/*
	 * if ((idx_sig + 1) == (uint64_t)(1 << h)) {
	 * // One might want to be informed that this is the last message
	 * // to be signed
	 * }
	 */

	SK_MT->idx_MT = idx_sig + 1; /* setIdx(SK_MT, idx_sig + 1) */

	unsigned char r[n];
	memset(r, 0, sizeof(r));
	unsigned char msg_tmp[n];
	memset(msg_tmp, 0, sizeof(msg_tmp));
	unsigned char tobyte_idx[32];
	memset(tobyte_idx, 0, sizeof(tobyte_idx));
	toByte(tobyte_idx, idx_sig, 32);
	PRF(r, SK_PRF, tobyte_idx, n); /* byte[n] */
	unsigned char input_tmp[3 * n];
	memset(input_tmp, 0, 3 * n * sizeof(unsigned char));
	unsigned char rand_root_tmp[2 * n];
	memset(rand_root_tmp, 0, 2 * n * sizeof(unsigned char));
	unsigned char tobyte_tmp[n];
	memset(tobyte_tmp, 0, n * sizeof(unsigned char));
	toByte(tobyte_tmp, idx_sig, n);
	concat_bytes(rand_root_tmp, r, n, sk_root, n);
	concat_bytes(input_tmp, rand_root_tmp, 2 * n, tobyte_tmp, n);
	H_msg(msg_tmp, msg, msglen, input_tmp, n); /* byte[n] */

	wots_param para = SK_MT->reduced_keys[0].para;
	xmss_mt_sig Sig_MT;
	if ((xmss_mt_sig_init(&Sig_MT, &para, h, d)) == 1)
		return 2;

	Sig_MT.idx_sig = idx_sig; /* Sig_MT = idx_sig */

	/* idx_tree = (h - h / d) most significant bits of idx_sig */
	uint64_t idx_tree = 0, idx_leaf = 0;
	getMSB(idx_sig, &idx_tree, h - h / d, h);

	/* idx_leaf = (h / d) least significant bits of idx_sig */
	getLSB(idx_sig, &idx_leaf, h / d, h);

	/* SK = idx_leaf || getXMSS_SK(SK_MT, idx_tree, 0) ||
		SK_PRF  || toByte(0, n) || SEED */
	xmss_sk SK;
	xmss_sk_init(&SK, &para, h / d);
	getXMSS_SK(&SK, SK_MT, idx_tree, 0);
	SK.idx = idx_leaf;
	memcpy(SK.SK_PRF, SK_PRF, n * sizeof(*SK.SK_PRF));
	memcpy(SK.seed, SK_MT->seed, n * sizeof(*SK.seed));

	setLayerAddress(adrs, 0); /* ADRS.setLayerAddress(0) */
	setTreeAddress(adrs, idx_tree); /* ADRS.setTreeAddress(idx_tree) */
	unsigned char sig_tmp[para.len * n + (h / d) * n];
	memset(sig_tmp, 0, (para.len * n + (h / d) * n) *sizeof(unsigned char));
	treeSig(sig_tmp, &SK, msg_tmp, adrs);

	/*
	 * Reduced signatures contain no index and randomness,
	 * so no need to copy them
	 */
	memcpy(Sig_MT.randomness, r, n * sizeof(unsigned char));
	memcpy(Sig_MT.reduced_sigs[0].sig_ots, &sig_tmp[0],
	     para.len * n * sizeof(unsigned char));
	memcpy(Sig_MT.reduced_sigs[0].auth, &sig_tmp[para.len * n],
	     n * (h / d) * sizeof(unsigned char));

	unsigned char root[n];

	for (unsigned int j = 1; j < d; j++) {
		treeHash(root, &SK, 0, h / d, adrs);

		/* idx_leaf = (h / d) least significant bits of idx_tree */
		getLSB(idx_tree, &idx_leaf, h / d, h);

		/*
		 * idx_tree = (h - j * (h / d))
		 * most significant bits of idx_tree
		 */
		getMSB(idx_tree, &idx_tree, h - h / d, h);

		/* SK = idx_leaf || getXMSS_SK(SK_MT, idx_tree, j) ||
			SK_PRF || toByte(0, n) || SEED */
		getXMSS_SK(&SK, SK_MT, idx_tree, j);
		SK.idx = idx_leaf;
		memcpy(SK.SK_PRF, SK_PRF, n * sizeof(*SK.SK_PRF));

		setLayerAddress(adrs, j); /* ADRS.setLayerAddress(j) */
		/* ADRS.setTreeAddress(idx_tree) */
		setTreeAddress(adrs, idx_tree);

		treeSig(sig_tmp, &SK, root, adrs);

		/* Sig_MT = Sig_MT || Sig_tmp */
		memcpy(Sig_MT.reduced_sigs[j].sig_ots, &sig_tmp[0],
		    para.len * n * sizeof(unsigned char));
		memcpy(Sig_MT.reduced_sigs[j].auth, &sig_tmp[para.len * n],
		    (h / d) * n * sizeof(unsigned char));
	}
	xmss_mt_SigToString(sig, &Sig_MT, n, h, para.w, d);
	xmss_sk_free(&SK);
	xmss_mt_sig_free(&Sig_MT, d);
	return 0;
}

/*
 * Algorithm 17: XMSSMT_verify
 * Verify an XMSS^MT signature Sig_MT on a message M using an XMSS^MT
 * public key PK_MT
 */
int
xmss_mt_verify(unsigned char *pk_all, unsigned char *sig, unsigned char *msg,
    unsigned int msglen, wots_param *para, unsigned int h, unsigned int d)
{
	/*
	 * returns 0 if verification succeeds
	 * returns 1 if verification fails
	 * returns 2 if the initialization of xmss_mt_sig failed
	 * returns 3 if the initialization of xmss_pk failed
	 */
	xmss_mt_sig Sig_MT;
	if ((xmss_mt_sig_init(&Sig_MT, para, h, d)) == 1)
		return 2;

	unsigned int w = para->w, n = para->n;
	xmss_mt_StringToSig(&Sig_MT, sig, n, h, w, d);

	uint64_t idx_sig = Sig_MT.idx_sig; /* idx_sig = getIdx(Sig_MT) */

	xmss_pk PK_MT; /* Use existing XMSS PK structure */
	if ((xmss_pk_init(&PK_MT, n)) == 1)
		return 3;

	xmssStringToPK(&PK_MT, pk_all, n);

	unsigned char seed[n], adrs[32];
	/* SEED = getSEED(PK_MT) */
	memcpy(seed, PK_MT.seed, sizeof(seed));
	memset(adrs, 0, sizeof(adrs)); /* ADRS = toByte(0, 32) */

	unsigned char r[n];
	memset(r, 0, sizeof(r));
	memcpy(r, Sig_MT.randomness, n * sizeof(unsigned char));
	unsigned char msg_tmp[n];
	memset(msg_tmp, 0, sizeof(msg_tmp));
	unsigned char tobyte_tmp[n];
	memset(tobyte_tmp, 0, n * sizeof(unsigned char));
	unsigned char input_tmp[3 * n];
	memset(input_tmp, 0, 3 * n * sizeof(unsigned char));
	unsigned char rand_root_tmp[2 * n];
	memset(rand_root_tmp, 0, 2 * n * sizeof(unsigned char));
	toByte(tobyte_tmp, idx_sig, n);
	concat_bytes(rand_root_tmp, r, n, PK_MT.root, n);
	concat_bytes(input_tmp, rand_root_tmp, 2 * n, tobyte_tmp, n);
	H_msg(msg_tmp, msg, msglen, input_tmp, n); /* byte[n] */

	uint64_t idx_leaf = 0, idx_tree = 0;
	/* idx_leaf = (h / d) least significant bits of idx_sig */
	getLSB(idx_sig, &idx_leaf, h / d, h);
	/* idx_tree = (h - h / d) most significant bits of idx_sig */
	getMSB(idx_sig, &idx_tree, h - h / d, h);
	
	xmss_sig Sigp; /* Sig' */
	xmss_sig_init(&Sigp, para, h / d);

	/* Sig' = getXMSSSignature(Sig_MT, 0) */
	Sigp.idx_sig = idx_leaf;
	memcpy(Sigp.randomness, Sig_MT.randomness,
	    n * sizeof(*Sigp.randomness));
	memcpy(Sigp.sig_ots, Sig_MT.reduced_sigs[0].sig_ots,
	    para->len * n * sizeof(*Sigp.sig_ots));
	memcpy(Sigp.auth, Sig_MT.reduced_sigs[0].auth,
	    (h / d) * n * sizeof(*Sigp.auth));
	setLayerAddress(adrs, 0); /* ADRS.setLayerAddress(0) */
	setTreeAddress(adrs, idx_tree); /* ADRS.setTreeAddress(idx_tree) */

	/* byte[n] node = XMSS_rootFromSig(idx_leaf, getSig_ots(Sig'), 
			  getAuth(Sig'), M', SEED, ADRS) */
	unsigned char node[n];
	memset(node, 0, sizeof(node));
	
	xmss_rootFromSig(&Sigp, msg_tmp, node, seed, n, h/d, adrs);
	for (unsigned int j = 1; j < d; j++) {
		/* idx_leaf = (h / d) least significant bits of idx_tree */
		getLSB(idx_tree, &idx_leaf, h / d, h);

		/*
		 * idx_tree = (h - j * h / d)
		 * most significant bits of idx_tree
		 */
		getMSB(idx_tree, &idx_tree, h - h / d, h);
		
		/* Sig' = getXMSSSignature(Sig_MT, j) */
		Sigp.idx_sig = idx_leaf;
		memcpy(Sigp.sig_ots, Sig_MT.reduced_sigs[j].sig_ots,
		    para->len * n * sizeof(*Sigp.sig_ots));
		memcpy(Sigp.auth, Sig_MT.reduced_sigs[j].auth,
		    (h / d) * n * sizeof(*Sigp.auth));

		/* ADRS.setLayerAddress(j) */
		setLayerAddress(adrs, (uint32_t) j);
		/* ADRS.setTreeAddress(idx_tree) */
		setTreeAddress(adrs, idx_tree);

		xmss_rootFromSig(&Sigp, node, node, seed, n, h/d, adrs);
		/* node = XMSS_rootFromSig(idx_leaf, getSig_ots(Sig'), 
			  getAuth(Sig'), node, SEED, ADRS) */
	}
	xmss_sig_free(&Sigp);
	xmss_mt_sig_free(&Sig_MT, d);

	int success = 1;
	if (memcmp(node, PK_MT.root, n) == 0)
		success = 0;
	xmss_pk_free(&PK_MT);
	return success; /* 0 if verification successful, else 1 */
}

void
xmss_mt_StringToSig(xmss_mt_sig *Sig, unsigned char *string, unsigned int n,
    unsigned int h, unsigned int w, unsigned int d)
{
	unsigned int len1 = calcLen1(w, n);
	unsigned int len = calcLen(len1, calcLen2(w, len1));

	/* Extract idx_sig from string */
	unsigned int length_idx = ceil(h / 8.0), intermediate_length = 0;
	uint64_t idx = 0;

	for (unsigned int i = 0; i < length_idx; i++)
		idx |= (uint64_t)string[i] << 8 * (length_idx - 1 - i);

	Sig->idx_sig = idx;

	/* Extract randomness */
	memcpy(Sig->randomness, &string[length_idx],
	    n * sizeof(*Sig->randomness));
	intermediate_length = length_idx + n;

	/* Extract the array of d reduced signatures */
	for (unsigned int i = 0 ; i < d; i++) {
		memcpy(Sig->reduced_sigs[i].sig_ots,
		    &string[intermediate_length],
		    len * n * sizeof(unsigned char));
		intermediate_length += len * n;
		memcpy(Sig->reduced_sigs[i].auth, &string[intermediate_length],
		    (h / d) * n * sizeof(unsigned char));
		intermediate_length += (h / d) * n;
	}
}

int
xmss_mt_SigToString(unsigned char *concat, xmss_mt_sig *Sig, unsigned int n,
    unsigned int h, unsigned int w, unsigned int d)
{
	/* returns 1 if a problem with calloc occurred */
	unsigned int len1 = calcLen1(w, n);
	unsigned int len = calcLen(len1, calcLen2(w, len1));

	unsigned int length_idx = ceil(h / 8.0), intermediate_length = 0;
	uint64_t idx = Sig->idx_sig;
	unsigned char *idx_sig_char;
	if ((idx_sig_char = calloc(length_idx, sizeof(*idx_sig_char))) == NULL)
		return 1;
	for (unsigned int i = 0; i < length_idx; i++) {
		idx_sig_char[i] = (unsigned char)
		    (idx >> (8 * (length_idx - 1 - i))) & 0xFF;
	}
	concat_bytes(concat, idx_sig_char, length_idx, Sig->randomness, n);
	intermediate_length = n + length_idx;
	for (unsigned int j = 0; j < d; j++) {
		concat_bytes(concat, concat, intermediate_length,
		    Sig->reduced_sigs[j].sig_ots, len * n);
		intermediate_length += len * n;
		concat_bytes(concat, concat, intermediate_length,
		    Sig->reduced_sigs[j].auth, (h / d) * n);
		intermediate_length += (h / d) * n;
	}
	free(idx_sig_char);
	return 0;
}

