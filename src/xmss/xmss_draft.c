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

#include "xmss_draft.h"

/*
 * This is algorithm 7: RAND_HASH
 * Used to show randomized hashing in the draft.
 * Also useful as separate function here.
 */
void
rand_hash(unsigned char *hash, unsigned char *left, unsigned char *right,
    unsigned char *seed, unsigned char adrs[32], const unsigned int n)
{
	unsigned char bm_0[n], bm_1[n], key[n];
	memset(bm_0, 0, n * sizeof(unsigned char));
	memset(bm_1, 0, n * sizeof(unsigned char));
	memset(key, 0, n * sizeof(unsigned char));
	setKeyAndMask(adrs, 0);
	PRF(key, seed, adrs, n);
	setKeyAndMask(adrs, 1);
	PRF(bm_0, seed, adrs, n);
	setKeyAndMask(adrs, 2);
	PRF(bm_1, seed, adrs, n);
	unsigned char xor_left[n], xor_right[n];
	memset(xor_left, 0, n * sizeof(unsigned char));
	memset(xor_right, 0, n * sizeof(unsigned char));
	do_xor(xor_left, left, bm_0, n);
	do_xor(xor_right, right, bm_1, n);
	unsigned char concat[n+n];
	memset(concat, 0, (n+n) * sizeof(unsigned char));
	concat_bytes(concat, xor_left, n, xor_right, n);
	H(hash, concat, key, n);
	/* H(KEY, (LEFT XOR BM_0) || (RIGHT XOR BM_1)) */
}

/* This is algorithm 8: ltree */
void
ltree (unsigned char *pk, unsigned char *node, const wots_param *para,
    unsigned char *seed, unsigned char adrs[32])
{
	unsigned int n = para->n, len_tmp = para->len;
	uint32_t i = 0, treeHeight = 0;
	setTreeHeight(adrs, 0);
	while (len_tmp > 1) {
		for (i = 0; i < (uint32_t)floor(len_tmp / 2.0); i++) {
			setTreeIndex(adrs, i);
			/* setting pk[i] */
			rand_hash(&pk[i * n], &pk[2 * i * n],
			    &pk[(2 * i + 1) * n], seed, adrs, n);
		}
		if (len_tmp % 2 == 1) {
			memcpy(&pk[((int)floor(len_tmp / 2.0)) * n],
			    &pk[(len_tmp - 1) * n], n * sizeof(unsigned char));
		}
		len_tmp = (unsigned int)ceil(len_tmp / 2.0);
		treeHeight++;
		setTreeHeight(adrs, treeHeight);
	}
	memcpy(node, &pk[0], n * sizeof(unsigned char));
}

/* This is algorithm 9: treeHash */
int
treeHash (unsigned char *hash, xmss_sk *SK, const uint32_t s,
    const uint32_t t, unsigned char adrs[32])
{
	/*
	 * returns 1 if s is not a multiple of 2^t
	 * returns 2 if next access to SK->wots_keys would be out of bounds
	 */
	unsigned int n = SK->n;
	xmss_node stack[t + 1];
	wots_param par = SK->para;
	for (uint32_t i = 0 ; i < (t + 1); i++) {
		memset(stack[i].node, 0, n * sizeof(unsigned char));
		stack[i].height = 999;
	}
	int nodenumber = 0, topnode = -1;
	unsigned char pk[par.len * n], node[n], tmp_node[n];
	memset(pk, 0, par.len * n * sizeof(unsigned char));
	memset(node, 0, n * sizeof(unsigned char));
	memset(tmp_node, 0, n * sizeof(unsigned char));
	uint32_t height = 0, index = 0;
	/* This is just a single WOTS+ secret key: */
	unsigned char sk[par.len * n];
	memset(sk, 0, par.len * n * sizeof(unsigned char));
	unsigned int sum = 0;
	/*
	 * Specifically for XMSS^MT:
	 * Don't generate WOTS+ keys if already done, so check if empty
	 */
	for (unsigned int i = 0; i < ((unsigned int)(1<<t) * par.len * n); ++i)
		sum |= SK->wots_keys[i];

	if ((s == 0) && (t == SK->h) && (sum == 0)) {
		for (unsigned int num = 0; num < (unsigned int)(1<<t); num++) {
			genWotsPrivKey(&SK->para, sk);
			memcpy(&SK->wots_keys[num * par.len * n], sk,
			    par.len * n * sizeof(unsigned char));
		}
	}

	if (s % ( 1 << t ) != 0)
		return 1;

	for (unsigned int i = 0; i < (unsigned int)(1 << t); i++) {
		setType(adrs, 0);
		setOTSAddress(adrs, s + i);
		/* getWOTS_SK(SK, sk, s + i) */

		if ((s + i) > (unsigned int)((1 << SK->h) - 1))
			return 2;

		memcpy(sk, &SK->wots_keys[(s + i) * par.len * n],
		    par.len * n * sizeof(unsigned char));

		genWotsPubKey(pk, &SK->para, sk, SK->seed, adrs);
		setType(adrs, 1);
		setLTreeAddress(adrs, (s + i));
		ltree(pk, node, &SK->para, SK->seed, adrs);
		setType(adrs, 2);
		setTreeHeight(adrs, 0);
		height = 0;
		setTreeIndex(adrs, (s + i));
		index = i + s;
		/* Not required by specification: Save all nodes to SK */
		if ((s == 0) && (t == (uint32_t) SK->h)) {
			memcpy(SK->nodes[nodenumber].node, node,
			    n * sizeof(unsigned char));
			SK->nodes[nodenumber].height = height;
			nodenumber++;
		}

		while (height == stack[topnode].height && topnode > -1) {
			getTreeIndex(adrs, &index);
			setTreeIndex(adrs, ((index - 1) / 2));
			rand_hash(tmp_node, stack[topnode].node, node,
			    SK->seed, adrs, n);
			memset(stack[topnode].node, 0,
			    n * sizeof(unsigned char));
			stack[topnode].height = 0;
			topnode--;
			memcpy(node, tmp_node,
			    n * sizeof(unsigned char));
			getTreeHeight(adrs, &height);
			setTreeHeight(adrs, ++height);
			if ((s == 0) && (t == (uint32_t)SK->h)) {
				memcpy(SK->nodes[nodenumber].node, node,
				    n * sizeof(unsigned char));
				SK->nodes[nodenumber].height = height;
				nodenumber++;
			}
		}
		topnode++;
		memcpy(stack[topnode].node, node, n * sizeof(unsigned char));
		stack[topnode].height = height;
	}
	memcpy(hash, stack[topnode].node, n * sizeof(unsigned char));
	return 0;
}

/* Initialize the XMSS secret key SK */
int
xmss_sk_init(xmss_sk *SK, const wots_param *para, const unsigned int h)
{
	fprintf(stderr, "[xmss_draft.c] Entered xmss_sk_init()\n");
	unsigned char sk_prf[para->n];
	unsigned int check = 0;
	check = RAND_bytes(sk_prf, para->n);
	if (check != 1)
		return 3;
	return xmss_sk_init_core(SK, para, h, sk_prf);
}

/* Deterministic core functionality of xmss_sk_init */
int
xmss_sk_init_core(xmss_sk *SK, const wots_param *para, const unsigned int h, unsigned char *sk_prf)
{
	/*
	 * returns 1 if a problem with calloc occurred
	 * returns 2 if height h is less than 2
	 * returns 3 if call to RAND_bytes for SK_PRF failed
	 */
	fprintf(stderr, "[xmss_draft.c] Entered xmss_sk_init_core()\n");
	SK->idx = 0;
	SK->n = para->n;
	SK->para = *para;
	if (h < 2)
		return 2;
	SK->h = h;
	if ((SK->SK_PRF = calloc(para->n, sizeof(unsigned char))) == NULL)
		return 1;
	memcpy(SK->SK_PRF, sk_prf, para->n);
	if ((SK->root = calloc(para->n, sizeof(unsigned char))) == NULL)
		return 1;
	if ((SK->seed = calloc(para->n, sizeof(unsigned char))) == NULL)
		return 1;
	if ((SK->nodes = calloc(((2 << h)) - 1, sizeof(xmss_node))) == NULL)
		return 1;
	if ((SK->wots_keys = calloc((para->len * para->n * (1 << h)),
	    sizeof(unsigned char))) == NULL)
		return 1;
	return 0;
}

void
xmss_sk_free(xmss_sk *SK)
{
	free(SK->SK_PRF);
	free(SK->root);
	free(SK->seed);
	free(SK->nodes);
	free(SK->wots_keys);
}

int
xmss_sig_init(xmss_sig *Sig, const wots_param *para, const unsigned int h)
{
	/* returns 1 if a problem with calloc occurred */
	Sig->idx_sig = 0;
	if ((Sig->randomness = calloc(para->n, sizeof(unsigned char))) == NULL)
		return 1;
	if ((Sig->sig_ots = calloc(para->len * para->n,
	    sizeof(unsigned char))) == NULL)
		return 1;
	if ((Sig->auth = calloc(h * para->n, sizeof(unsigned char))) == NULL)
		return 1;
	return 0;
}

void
xmss_sig_free(xmss_sig *Sig)
{
	free(Sig->randomness);
	free(Sig->sig_ots);
	free(Sig->auth);
}

int
xmss_pk_init(xmss_pk *PK, const unsigned int n)
{
	/* returns 1 if a problem with calloc occurred */
	if ((PK->root = calloc(n, sizeof(unsigned char))) == NULL)
		return 1;
	if ((PK->seed = calloc(n, sizeof(unsigned char))) == NULL)
		return 1;
	return 0;
}

void
xmss_pk_free(xmss_pk *PK)
{
	free(PK->root);
	free(PK->seed);
}

/* This is algorithm 10: */
int
xmss_keyGen (xmss_sk *SK, unsigned char *pk, wots_param *para,
    const unsigned int h)
{
	fprintf(stderr, "[xmss_draft.c] Entered xmss_keyGen()\n");
	unsigned char seed[para->n];
	unsigned char PRF[para->n];
	unsigned int check = 0;
	check = RAND_bytes(seed, para->n);
	if (check != 1)
		return 3;
	check = RAND_bytes(PRF, para->n);
	if (check != 1)
		return 3;
	return xmss_keyGen_core (SK, pk, para, h, seed, PRF);
}

/* Deterministic core functionality of algorithm 10 */
int
xmss_keyGen_core (xmss_sk *SK, unsigned char *pk, wots_param *para,
    const unsigned int h,unsigned char *seed,unsigned char *PRF)
{
	/*
	 * returns 1 if initialization of xmss_sk failed
	 * returns 2 if initialization of xmss_pk failed
	 * returns 3 if call to RAND_bytes for seed failed
	*/
	fprintf(stderr, "[xmss_draft.c] Entered xmss_keyGen_core()\n");
	if ((xmss_sk_init_core(SK, para, h, PRF)) == 1)
		return 1;
	xmss_pk PK;
	if ((xmss_pk_init(&PK, SK->n)) == 1)
		return 2;
	memcpy(PK.seed, seed, SK->n);
	unsigned char adrs[32];
	memset(adrs, 0, sizeof(adrs)); /* toByte(adrs, 0, 32) */
	memcpy(SK->seed, PK.seed, SK->n);
	treeHash(PK.root, SK, 0, h, adrs);
	memcpy(SK->root, PK.root, SK->n);
	xmssPKtoString(pk, &PK, SK->n);
	xmss_pk_free(&PK);
	return 0;
}


/*
 * This is an adaption for the non-numbered algorithm for the calculation
 * of the authentication path:
 */
int
buildAuth (xmss_sk *SK, unsigned char *auth_path, const uint32_t i)
{
	/* returns 1 if no suitable node could be found */
	unsigned int n = SK->n, h = SK->h, k = 0, occurrence = 0;
	unsigned char auth[h * n];
	uint32_t idx = i;
	/*
	 * We have SK->nodes, which is an array containing all nodes as
	 * xmss_node, a struct containing the n-byte node string and
	 * the height
	 */
	memset(auth, 0, h * n * sizeof(unsigned char));
	for (unsigned int j = 0; j < h; j++) {
		/*
		 * KISS: A simple but inefficient implementation of buildAuth.
		 * Should be replaced by a more efficient variant in actual
		 * applications.
		 */
		if ((idx % 2) == 0) {
			while (occurrence != (idx + 2)) {
				if (SK->nodes[k].height == j)
					occurrence++;
				k++;
				if (k == (unsigned int)((1 << (h + 1)) - 1))
					return 1;
			}
			k--;
			while (SK->nodes[k].height != j)
				k--;
			memcpy(&auth[j * n], SK->nodes[k].node,
			    n * sizeof(unsigned char));
			idx = idx / 2;
		}
		else
		{
			while (occurrence != idx) {
				if (SK->nodes[k].height == j)
					occurrence++;
				k++;
				if (k == (unsigned int)((1 << (h + 1)) - 1))
					return 1;
			}
			k--;
			memcpy(&auth[j * n], SK->nodes[k].node,
			    n * sizeof(unsigned char));
			idx = (idx - 1) / 2;
		}
		k = 0;
		occurrence = 0;
	}
	memcpy(auth_path, &auth, h * n * sizeof(unsigned char));
	return 0;
}

/* This is algorithm 11: treeSig */
int
treeSig(unsigned char *sig_tmp, xmss_sk *SK, unsigned char *msg,
    unsigned char adrs[32])
{
	uint32_t idx_sig = SK->idx;
	unsigned int n = SK->n, len = SK->para.len, h = SK->h;
	unsigned char auth[h * n];
	memset(auth, 0, h * n * sizeof(unsigned char));
	buildAuth(SK, auth, idx_sig);
	setType(adrs, 0);
	setOTSAddress(adrs, idx_sig);
	unsigned char sig_ots[len * n];
	memset(sig_ots, 0, len * n * sizeof(unsigned char));
	if (idx_sig > (uint32_t)((1 << h) - 1))
		return 1;
	genWotsSig(&SK->wots_keys[idx_sig * n * len], &SK->para, msg,
	    sig_ots, SK->seed, adrs);
	memcpy(sig_tmp, sig_ots, len * n * sizeof(unsigned char));
	memcpy(&sig_tmp[len * n], auth, h * n * sizeof(unsigned char));
	return 0;
}

/* This is algorithm 12: XMSS_sign */
int
xmss_signing(xmss_sk *SK, unsigned char *msg, const unsigned int msglen,
    unsigned char *sig, unsigned char adrs[32])
{
	/*
	 * returns 1 if the initialization of xmss_sig failed
	 * returns 2 if the index idx_sig is too large for the tree
	 */
	fprintf(stderr, "[xmss_draft.c] Entered xmss_signing()\n");
	wots_param para = SK->para;
	xmss_sig Sig;
	if ((xmss_sig_init(&Sig, &para, SK->h)) == 1)
		return 1;
	xmssStringToSig(&Sig, sig, para.n, SK->h, para.w);
	uint32_t idx_sig = SK->idx;
	unsigned int h = SK->h;
	if (idx_sig >= (uint32_t)(1 << h))
		return 2;
	else if ((idx_sig + 1) == (uint32_t)(1 << h))
	{
		/*
		 * One may want to be informed that this is the last message to
		 * be signed with this tree
		 */
	}
	unsigned int n = SK->n, len = SK->para.len;
	unsigned char r[n], sk_prf[n], root[n];
	memset(r, 0, n * sizeof(unsigned char));
	memset(sk_prf, 0, n * sizeof(unsigned char));
	memset(root, 0, n * sizeof(unsigned char));
	memcpy(sk_prf, SK->SK_PRF, SK->n * sizeof(unsigned char));
	memcpy(root, SK->root, SK->n * sizeof(unsigned char));
	unsigned char tobyte_idx[32];
	memset(tobyte_idx, 0, 32 * sizeof(unsigned char));
	toByte(tobyte_idx, idx_sig, 32);
	PRF(r, sk_prf, tobyte_idx, n); /* byte[n] */
	unsigned char msg_tmp[n];
	memset(msg_tmp, 0, n * sizeof(unsigned char));
	unsigned char input_tmp[3 * n];
	memset(input_tmp, 0, 3 * n * sizeof(unsigned char));
	unsigned char rand_root_tmp[2 * n];
	memset(rand_root_tmp, 0, 2 * n * sizeof(unsigned char));
	unsigned char tobyte_tmp[n];
	memset(tobyte_tmp, 0, n * sizeof(unsigned char));
	toByte(tobyte_tmp, idx_sig, n);
	concat_bytes(rand_root_tmp, r, n, root, n);
	concat_bytes(input_tmp, rand_root_tmp, 2 * n, tobyte_tmp, n);
	H_msg(msg_tmp, msg, msglen, input_tmp, n); /* byte[n] */
	unsigned char sig_tmp[len * n + h * n];
	memset(sig_tmp, 0, (len * n + h * n) * sizeof(unsigned char));
	memset(adrs, 0, 32 * sizeof(unsigned char));
	treeSig(sig_tmp, SK, msg_tmp, adrs);
	/* Sig = (idx_sig || r || treeSig(M', SK, ADRS)) */
	memcpy(Sig.randomness, r, n * sizeof(unsigned char));
	memcpy(Sig.sig_ots, sig_tmp, len * n * sizeof(unsigned char));
	memcpy(Sig.auth, &sig_tmp[len * n], h * n * sizeof(unsigned char));
	Sig.idx_sig = SK->idx;
	SK->idx = idx_sig + 1;
	xmssSigToString(sig, &Sig, para.n, SK->h, para.w);
	xmss_sig_free(&Sig);
	return 0;
}

/* This is algorithm 13: XMSS_rootFromSig */
int
xmss_rootFromSig(xmss_sig *Sig, unsigned char *msg,
    unsigned char *node, unsigned char *seed, const unsigned int n,
    const unsigned int h, unsigned char adrs[32])
{
	/*
	 * returns 1 if the index idx_sig is too large for the tree
	 * Note: we actually want to get h (the height of the tree)
	 * from the OID. This is just a workaround.
	 * If actually getting it from Sig: bytes 4 to n + 3
	 */
	unsigned char r[n];
	memset(r, 0, n * sizeof(unsigned char));
	/* If actually getting it from Sig: the last (h - 1) * n bytes */
	unsigned char auth[h * n];
	memset(auth, 0, h * n * sizeof(unsigned char));
	memcpy(r, Sig->randomness, n * sizeof(unsigned char));
	memcpy(auth, Sig->auth, h * n * sizeof(unsigned char));
	/* If actually getting it from Sig: bytes 0 - 3 */
	uint32_t idx_sig = Sig->idx_sig;
	if (idx_sig >= (uint32_t)(1 << h))
		return 1;
	wots_param para;
	para.n = n;
	para.w = 16;
	para.len_1 = calcLen1(para.w, para.n);
	para.len_2 = calcLen2(para.w, para.len_1);
	para.len = calcLen(para.len_1, para.len_2);
	unsigned char sig_ots[para.len * n];
	memset(sig_ots, 0, para.len * n * sizeof(unsigned char));
	memcpy(sig_ots, Sig->sig_ots, para.len * n * sizeof(unsigned char));
	setType(adrs, 0);
	setOTSAddress(adrs, idx_sig);
	unsigned char pk_ots[para.len * n];
	memset(pk_ots, 0, para.len * n * sizeof(unsigned char));
	wotsPubKeyFromSig(&para, msg, sig_ots, pk_ots, seed, adrs);
	setType(adrs, 1);
	setLTreeAddress(adrs, idx_sig);
	unsigned char node0[para.n], node1[para.n];
	memset(node0, 0, para.n * sizeof(unsigned char));
	memset(node1, 0, para.n * sizeof(unsigned char));
	ltree(pk_ots, node0, &para, seed, adrs);
	setType(adrs, 2);
	setTreeIndex(adrs, idx_sig);
	uint32_t  k = 0, index = 0;
	for (k = 0; k < h; k++) {
		setTreeHeight(adrs, k);
		if (((int)floor((double)((idx_sig/(1 << (uint32_t)k)) % 2)))
		     == 0) {
			getTreeIndex(adrs, &index);
			setTreeIndex(adrs, (index >> 1));
			rand_hash(node1, node0, &auth[k * n], seed, adrs, n);
		}
		else
		{
			getTreeIndex(adrs, &index);
			setTreeIndex(adrs, ((index - 1) >> 1));
			rand_hash(node1, &auth[k * n], node0, seed, adrs, n);
		}
		memcpy(node0, node1, para.n * sizeof(unsigned char));
	}
	memcpy(node, node0, para.n * sizeof(unsigned char));
	return 0;
}

/* This is algorithm 14: XMSS_verify */
int
xmss_verification(unsigned char *pk_all, unsigned char *sig, unsigned char *msg,
    const unsigned int msglen, const wots_param *para, const unsigned int h)
{
	/*
	 * returns 0 if verification succeeds
	 * returns 1 if verification fails
	 * returns 2 if index is too large for the tree
	 */
	fprintf(stderr, "[xmss_draft.c] Entered xmss_verification()\n");
	xmss_pk PK;
	xmss_pk_init(&PK, para->n);
	xmssStringToPK(&PK, pk_all, para->n);
	xmss_sig Sig;
	xmss_sig_init(&Sig, para, h);
	unsigned int n = para->n, w = para->w;
	xmssStringToSig(&Sig, sig, n, h, w);
	uint32_t idx_sig = Sig.idx_sig;
	if (idx_sig >= (uint32_t)(1 << h))
		return 2;
	unsigned char node[n], pk[n];
	memset(node, 0, n * sizeof(unsigned char));
	memset(pk, 0, n * sizeof(unsigned char));
	memcpy(pk, PK.root, n * sizeof(unsigned char));
	unsigned char adrs[32];
	memset(adrs, 0, sizeof(adrs));
	unsigned char msg_tmp[n];
	memset(msg_tmp, 0, n * sizeof(unsigned char));
	unsigned char input_tmp[3 * n];
	memset(input_tmp, 0, 3 * n * sizeof(unsigned char));
	unsigned char rand_root_tmp[2 * n];
	memset(rand_root_tmp, 0, 2 * n * sizeof(unsigned char));
	unsigned char tobyte_tmp[n];
	memset(tobyte_tmp, 0, n * sizeof(unsigned char));
	toByte(tobyte_tmp, idx_sig, n);
	unsigned char r[n];
	memset(r, 0, n * sizeof(unsigned char));
	memcpy(r, Sig.randomness, n * sizeof(unsigned char));
	concat_bytes(rand_root_tmp, r, n, pk, n);
	concat_bytes(input_tmp, rand_root_tmp, 2 * n, tobyte_tmp, n);
	H_msg(msg_tmp, msg, msglen, input_tmp, n); /* byte[n] */

	xmss_rootFromSig(&Sig, msg_tmp, node, PK.seed, n, h, adrs);
	xmss_sig_free(&Sig);
	xmss_pk_free(&PK);
	if (memcmp(node, pk, n) == 0){
		fprintf(stderr, "[xmss_draft.c] xmss_verification() SUCCEEDS\n");
		return 0;
	}
	fprintf(stderr, "[xmss_draft.c] xmss_verification() FAILS\n");
	return 1; /* 0 if verification successful, else 1 */
}

/*
 * Conversion from a xmss_sig to an unsigned char *.
 * concat_bytes requires the length in bytes of the strings to concatenate,
 * and these lengths in turn depend on n, len and h.
 * We don't need len as an explicit parameter however, since it only requires w
 * in addition to the rest and we assume w = 16.
 */
void
xmssSigToString(unsigned char *concat, xmss_sig *Sig, const int n, const int h,
    const int w)
{
	unsigned int len1 = calcLen1(w, n);
	unsigned int len = calcLen(len1, calcLen2(w, len1));
	unsigned char idx_sig_char[4] = { 0 };
	uint32_t idx_sig = Sig->idx_sig;
	idx_sig_char[0] = (unsigned char)(idx_sig >> 24) & 0xFF;
	idx_sig_char[1] = (unsigned char)(idx_sig >> 16) & 0xFF;
	idx_sig_char[2] = (unsigned char)(idx_sig >> 8) & 0xFF;
	idx_sig_char[3] = (unsigned char)(idx_sig) & 0xFF;
	/* concat idx_sig and randomness */
	concat_bytes(concat, idx_sig_char, 4, Sig->randomness, n);
	/* concat the above and sig_ots */
	concat_bytes(concat, concat, 4 + n, Sig->sig_ots, n * len);
	/* concat the above and auth */
	concat_bytes(concat, concat, (n * len) + 4 + n, Sig->auth, h * n);
}

/* Conversion from a xmss_pk to an unsigned char * */
void
xmssPKtoString(unsigned char *concat, const xmss_pk *PK, const int n)
{
	/* concat root and seed */
	concat_bytes(concat, PK->root, n, PK->seed, n);
}

void
xmssStringToSig(xmss_sig *Sig, unsigned char *string, const int n, const int h,
    const int w)
{
	unsigned int len1 = calcLen1(w, n);
	unsigned int len = calcLen(len1, calcLen2(w, len1));
	uint32_t idx_sig = 0;
	idx_sig = (uint32_t)string[0] << 24 | (uint32_t)string[1] << 16 |
	    (uint32_t)string[2] << 8 | (uint32_t)string[3];
	Sig->idx_sig = idx_sig;
	memcpy(Sig->randomness, &string[4], n * sizeof(unsigned char));
	memcpy(Sig->sig_ots, &string[4 + n], len * n * sizeof(unsigned char));
	memcpy(Sig->auth, &string[4 + n + (len * n)],
	    h * n * sizeof(unsigned char));
}

void
xmssStringToPK(xmss_pk *PK, unsigned char *string, const int n)
{
	memcpy(&PK->root[0], &string[0], n * sizeof(unsigned char));
	memcpy(&PK->seed[0], &string[n], n * sizeof(unsigned char));
}

