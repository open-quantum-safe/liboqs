/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: packing functions
**************************************************************************************/

#include <string.h>
#include "api.h"
#include "params.h"
#include "poly.h"

static void pack_sk(unsigned char *sk, poly s, poly_k e, unsigned char *seeds) { // Pack secret key sk
	unsigned int i, k;

	for (i = 0; i < PARAM_N; i++)
		sk[i] = (unsigned char) s[i];

	sk += PARAM_N;
	for (k = 0; k < PARAM_K; k++)
		for (i = 0; i < PARAM_N; i++)
			sk[k * PARAM_N + i] = (unsigned char) e[k * PARAM_N + i];

	memcpy(&sk[PARAM_K * PARAM_N], seeds, 2 * CRYPTO_SEEDBYTES);
}

#if defined(_qTESLA_p_I_)

static void encode_pk(unsigned char *pk, const poly_k t, const unsigned char *seedA) { // Encode public key pk
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) pk;

	for (i = 0; i < (PARAM_N * PARAM_K * PARAM_Q_LOG / 32); i += PARAM_Q_LOG) {
		pt[i] = (uint32_t)(t[j] | (t[j + 1] << 29));
		pt[i + 1] = (uint32_t)((t[j + 1] >> 3) | (t[j + 2] << 26));
		pt[i + 2] = (uint32_t)((t[j + 2] >> 6) | (t[j + 3] << 23));
		pt[i + 3] = (uint32_t)((t[j + 3] >> 9) | (t[j + 4] << 20));
		pt[i + 4] = (uint32_t)((t[j + 4] >> 12) | (t[j + 5] << 17));
		pt[i + 5] = (uint32_t)((t[j + 5] >> 15) | (t[j + 6] << 14));
		pt[i + 6] = (uint32_t)((t[j + 6] >> 18) | (t[j + 7] << 11));
		pt[i + 7] = (uint32_t)((t[j + 7] >> 21) | (t[j + 8] << 8));
		pt[i + 8] = (uint32_t)((t[j + 8] >> 24) | (t[j + 9] << 5));
		pt[i + 9] = (uint32_t)((t[j + 9] >> 27) | (t[j + 10] << 2) | (t[j + 11] << 31));
		pt[i + 10] = (uint32_t)((t[j + 11] >> 1) | (t[j + 12] << 28));
		pt[i + 11] = (uint32_t)((t[j + 12] >> 4) | (t[j + 13] << 25));
		pt[i + 12] = (uint32_t)((t[j + 13] >> 7) | (t[j + 14] << 22));
		pt[i + 13] = (uint32_t)((t[j + 14] >> 10) | (t[j + 15] << 19));
		pt[i + 14] = (uint32_t)((t[j + 15] >> 13) | (t[j + 16] << 16));
		pt[i + 15] = (uint32_t)((t[j + 16] >> 16) | (t[j + 17] << 13));
		pt[i + 16] = (uint32_t)((t[j + 17] >> 19) | (t[j + 18] << 10));
		pt[i + 17] = (uint32_t)((t[j + 18] >> 22) | (t[j + 19] << 7));
		pt[i + 18] = (uint32_t)((t[j + 19] >> 25) | (t[j + 20] << 4));
		pt[i + 19] = (uint32_t)((t[j + 20] >> 28) | (t[j + 21] << 1) | (t[j + 22] << 30));
		pt[i + 20] = (uint32_t)((t[j + 22] >> 2) | (t[j + 23] << 27));
		pt[i + 21] = (uint32_t)((t[j + 23] >> 5) | (t[j + 24] << 24));
		pt[i + 22] = (uint32_t)((t[j + 24] >> 8) | (t[j + 25] << 21));
		pt[i + 23] = (uint32_t)((t[j + 25] >> 11) | (t[j + 26] << 18));
		pt[i + 24] = (uint32_t)((t[j + 26] >> 14) | (t[j + 27] << 15));
		pt[i + 25] = (uint32_t)((t[j + 27] >> 17) | (t[j + 28] << 12));
		pt[i + 26] = (uint32_t)((t[j + 28] >> 20) | (t[j + 29] << 9));
		pt[i + 27] = (uint32_t)((t[j + 29] >> 23) | (t[j + 30] << 6));
		pt[i + 28] = (uint32_t)((t[j + 30] >> 26) | (t[j + 31] << 3));
		j += 32;
	}
	memcpy(&pk[PARAM_N * PARAM_K * PARAM_Q_LOG / 8], seedA, CRYPTO_SEEDBYTES);
}

static void decode_pk(int32_t *pk, unsigned char *seedA, const unsigned char *pk_in) { // Decode public key pk
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) pk_in, *pp = (uint32_t *) pk, mask29 = (1 << PARAM_Q_LOG) - 1;

	for (i = 0; i < PARAM_N * PARAM_K; i += 32) {
		pp[i] = pt[j] & mask29;
		pp[i + 1] = ((pt[j + 0] >> 29) | (pt[j + 1] << 3)) & mask29;
		pp[i + 2] = ((pt[j + 1] >> 26) | (pt[j + 2] << 6)) & mask29;
		pp[i + 3] = ((pt[j + 2] >> 23) | (pt[j + 3] << 9)) & mask29;
		pp[i + 4] = ((pt[j + 3] >> 20) | (pt[j + 4] << 12)) & mask29;
		pp[i + 5] = ((pt[j + 4] >> 17) | (pt[j + 5] << 15)) & mask29;
		pp[i + 6] = ((pt[j + 5] >> 14) | (pt[j + 6] << 18)) & mask29;
		pp[i + 7] = ((pt[j + 6] >> 11) | (pt[j + 7] << 21)) & mask29;
		pp[i + 8] = ((pt[j + 7] >> 8) | (pt[j + 8] << 24)) & mask29;
		pp[i + 9] = ((pt[j + 8] >> 5) | (pt[j + 9] << 27)) & mask29;
		pp[i + 10] = (pt[j + 9] >> 2) & mask29;
		pp[i + 11] = ((pt[j + 9] >> 31) | (pt[j + 10] << 1)) & mask29;
		pp[i + 12] = ((pt[j + 10] >> 28) | (pt[j + 11] << 4)) & mask29;
		pp[i + 13] = ((pt[j + 11] >> 25) | (pt[j + 12] << 7)) & mask29;
		pp[i + 14] = ((pt[j + 12] >> 22) | (pt[j + 13] << 10)) & mask29;
		pp[i + 15] = ((pt[j + 13] >> 19) | (pt[j + 14] << 13)) & mask29;
		pp[i + 16] = ((pt[j + 14] >> 16) | (pt[j + 15] << 16)) & mask29;
		pp[i + 17] = ((pt[j + 15] >> 13) | (pt[j + 16] << 19)) & mask29;
		pp[i + 18] = ((pt[j + 16] >> 10) | (pt[j + 17] << 22)) & mask29;
		pp[i + 19] = ((pt[j + 17] >> 7) | (pt[j + 18] << 25)) & mask29;
		pp[i + 20] = ((pt[j + 18] >> 4) | (pt[j + 19] << 28)) & mask29;
		pp[i + 21] = (pt[j + 19] >> 1) & mask29;
		pp[i + 22] = ((pt[j + 19] >> 30) | (pt[j + 20] << 2)) & mask29;
		pp[i + 23] = ((pt[j + 20] >> 27) | (pt[j + 21] << 5)) & mask29;
		pp[i + 24] = ((pt[j + 21] >> 24) | (pt[j + 22] << 8)) & mask29;
		pp[i + 25] = ((pt[j + 22] >> 21) | (pt[j + 23] << 11)) & mask29;
		pp[i + 26] = ((pt[j + 23] >> 18) | (pt[j + 24] << 14)) & mask29;
		pp[i + 27] = ((pt[j + 24] >> 15) | (pt[j + 25] << 17)) & mask29;
		pp[i + 28] = ((pt[j + 25] >> 12) | (pt[j + 26] << 20)) & mask29;
		pp[i + 29] = ((pt[j + 26] >> 9) | (pt[j + 27] << 23)) & mask29;
		pp[i + 30] = ((pt[j + 27] >> 6) | (pt[j + 28] << 26)) & mask29;
		pp[i + 31] = pt[j + 28] >> 3;
		j += 29;
	}
	memcpy(seedA, &pk_in[PARAM_N * PARAM_K * PARAM_Q_LOG / 8], CRYPTO_SEEDBYTES);
}

#define maskb1 ((1 << (PARAM_B_BITS + 1)) - 1)

static void encode_sig(unsigned char *sm, unsigned char *c, poly z) { // Encode signature sm
	unsigned int i, j = 0;
	uint32_t *t = (uint32_t *) z;
	uint32_t *pt = (uint32_t *) sm;

	for (i = 0; i < (PARAM_N * (PARAM_B_BITS + 1) / 32); i += 10) {
		pt[i] = (uint32_t)((t[j] & ((1 << 20) - 1)) | (t[j + 1] << 20));
		pt[i + 1] = (uint32_t)(((t[j + 1] >> 12) & ((1 << 8) - 1)) | ((t[j + 2] & maskb1) << 8) | (t[j + 3] << 28));
		pt[i + 2] = (uint32_t)(((t[j + 3] >> 4) & ((1 << 16) - 1)) | (t[j + 4] << 16));
		pt[i + 3] = (uint32_t)(((t[j + 4] >> 16) & ((1 << 4) - 1)) | ((t[j + 5] & maskb1) << 4) | (t[j + 6] << 24));
		pt[i + 4] = (uint32_t)(((t[j + 6] >> 8) & ((1 << 12) - 1)) | (t[j + 7] << 12));
		pt[i + 5] = (uint32_t)((t[j + 8] & ((1 << 20) - 1)) | (t[j + 9] << 20));
		pt[i + 6] = (uint32_t)(((t[j + 9] >> 12) & ((1 << 8) - 1)) | ((t[j + 10] & maskb1) << 8) | (t[j + 11] << 28));
		pt[i + 7] = (uint32_t)(((t[j + 11] >> 4) & ((1 << 16) - 1)) | (t[j + 12] << 16));
		pt[i + 8] = (uint32_t)(((t[j + 12] >> 16) & ((1 << 4) - 1)) | ((t[j + 13] & maskb1) << 4) | (t[j + 14] << 24));
		pt[i + 9] = (uint32_t)(((t[j + 14] >> 8) & ((1 << 12) - 1)) | (t[j + 15] << 12));
		j += 16;
	}
	memcpy(&sm[PARAM_N * (PARAM_B_BITS + 1) / 8], c, CRYPTO_C_BYTES);
}

static void decode_sig(unsigned char *c, poly z, const unsigned char *sm) { // Decode signature sm
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) sm;

	for (i = 0; i < PARAM_N; i += 16) {
		z[i] = ((int32_t) pt[j + 0] << 12) >> 12;
		z[i + 1] = (int32_t)(pt[j + 0] >> 20) | ((int32_t)(pt[j + 1] << 24) >> 12);
		z[i + 2] = ((int32_t) pt[j + 1] << 4) >> 12;
		z[i + 3] = (int32_t)(pt[j + 1] >> 28) | ((int32_t)(pt[j + 2] << 16) >> 12);
		z[i + 4] = (int32_t)(pt[j + 2] >> 16) | ((int32_t)(pt[j + 3] << 28) >> 12);
		z[i + 5] = ((int32_t) pt[j + 3] << 8) >> 12;
		z[i + 6] = (int32_t)(pt[j + 3] >> 24) | ((int32_t)(pt[j + 4] << 20) >> 12);
		z[i + 7] = (int32_t) pt[j + 4] >> 12;
		z[i + 8] = ((int32_t) pt[j + 5] << 12) >> 12;
		z[i + 9] = (int32_t)(pt[j + 5] >> 20) | ((int32_t)(pt[j + 6] << 24) >> 12);
		z[i + 10] = ((int32_t) pt[j + 6] << 4) >> 12;
		z[i + 11] = (int32_t)(pt[j + 6] >> 28) | ((int32_t)(pt[j + 7] << 16) >> 12);
		z[i + 12] = (int32_t)(pt[j + 7] >> 16) | ((int32_t)(pt[j + 8] << 28) >> 12);
		z[i + 13] = ((int32_t) pt[j + 8] << 8) >> 12;
		z[i + 14] = (int32_t)(pt[j + 8] >> 24) | ((int32_t)(pt[j + 9] << 20) >> 12);
		z[i + 15] = (int32_t) pt[j + 9] >> 12;
		j += 10;
	}
	memcpy(c, &sm[PARAM_N * (PARAM_B_BITS + 1) / 8], CRYPTO_C_BYTES);
}

#elif defined(_qTESLA_p_III_)

static void encode_pk(unsigned char *pk, const poly_k t, const unsigned char *seedA) { // Encode public key pk
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) pk;

	for (i = 0; i < (PARAM_N * PARAM_K * PARAM_Q_LOG / 32); i += 15) {
		pt[i + 0] = (uint32_t)(t[j + 0] | (t[j + 1] << 30));
		pt[i + 1] = (uint32_t)((t[j + 1] >> 2) | (t[j + 2] << 28));
		pt[i + 2] = (uint32_t)((t[j + 2] >> 4) | (t[j + 3] << 26));
		pt[i + 3] = (uint32_t)((t[j + 3] >> 6) | (t[j + 4] << 24));
		pt[i + 4] = (uint32_t)((t[j + 4] >> 8) | (t[j + 5] << 22));
		pt[i + 5] = (uint32_t)((t[j + 5] >> 10) | (t[j + 6] << 20));
		pt[i + 6] = (uint32_t)((t[j + 6] >> 12) | (t[j + 7] << 18));
		pt[i + 7] = (uint32_t)((t[j + 7] >> 14) | (t[j + 8] << 16));
		pt[i + 8] = (uint32_t)((t[j + 8] >> 16) | (t[j + 9] << 14));
		pt[i + 9] = (uint32_t)((t[j + 9] >> 18) | (t[j + 10] << 12));
		pt[i + 10] = (uint32_t)((t[j + 10] >> 20) | (t[j + 11] << 10));
		pt[i + 11] = (uint32_t)((t[j + 11] >> 22) | (t[j + 12] << 8));
		pt[i + 12] = (uint32_t)((t[j + 12] >> 24) | (t[j + 13] << 6));
		pt[i + 13] = (uint32_t)((t[j + 13] >> 26) | (t[j + 14] << 4));
		pt[i + 14] = (uint32_t)((t[j + 14] >> 28) | (t[j + 15] << 2));
		j += 16;
	}
	memcpy(&pk[PARAM_N * PARAM_K * PARAM_Q_LOG / 8], seedA, CRYPTO_SEEDBYTES);
}

#define maskq ((1 << PARAM_Q_LOG) - 1)

static void decode_pk(int32_t *pk, unsigned char *seedA, const unsigned char *pk_in) { // Decode public key pk
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) pk_in, *t = (uint32_t *) pk;

	for (i = 0; i < PARAM_N * PARAM_K; i += 16) {
		t[i + 0] = (pt[j + 0]) & maskq;
		t[i + 1] = ((pt[j + 0] >> 30) | (pt[j + 1] << 2)) & maskq;
		t[i + 2] = ((pt[j + 1] >> 28) | (pt[j + 2] << 4)) & maskq;
		t[i + 3] = ((pt[j + 2] >> 26) | (pt[j + 3] << 6)) & maskq;
		t[i + 4] = ((pt[j + 3] >> 24) | (pt[j + 4] << 8)) & maskq;
		t[i + 5] = ((pt[j + 4] >> 22) | (pt[j + 5] << 10)) & maskq;
		t[i + 6] = ((pt[j + 5] >> 20) | (pt[j + 6] << 12)) & maskq;
		t[i + 7] = ((pt[j + 6] >> 18) | (pt[j + 7] << 14)) & maskq;
		t[i + 8] = ((pt[j + 7] >> 16) | (pt[j + 8] << 16)) & maskq;
		t[i + 9] = ((pt[j + 8] >> 14) | (pt[j + 9] << 18)) & maskq;
		t[i + 10] = ((pt[j + 9] >> 12) | (pt[j + 10] << 20)) & maskq;
		t[i + 11] = ((pt[j + 10] >> 10) | (pt[j + 11] << 22)) & maskq;
		t[i + 12] = ((pt[j + 11] >> 8) | (pt[j + 12] << 24)) & maskq;
		t[i + 13] = ((pt[j + 12] >> 6) | (pt[j + 13] << 26)) & maskq;
		t[i + 14] = ((pt[j + 13] >> 4) | (pt[j + 14] << 28)) & maskq;
		t[i + 15] = ((pt[j + 14] >> 2)) & maskq;
		j += 15;
	}
	memcpy(seedA, &pk_in[PARAM_N * PARAM_K * PARAM_Q_LOG / 8], CRYPTO_SEEDBYTES);
}

#define maskb1 ((1 << (PARAM_B_BITS + 1)) - 1)

static void encode_sig(unsigned char *sm, unsigned char *c, poly z) { // Encode signature sm
	unsigned int i, j = 0;
	uint32_t *t = (uint32_t *) z;
	uint32_t *pt = (uint32_t *) sm;

	for (i = 0; i < (PARAM_N * (PARAM_B_BITS + 1) / 32); i += 11) {
		pt[i + 0] = (uint32_t)((t[j + 0] & ((1 << 22) - 1)) | (t[j + 1] << 22));
		pt[i + 1] = (uint32_t)(((t[j + 1] >> 10) & ((1 << 12) - 1)) | (t[j + 2] << 12));
		pt[i + 2] = (uint32_t)(((t[j + 2] >> 20) & ((1 << 2) - 1)) | ((t[j + 3] & maskb1) << 2) | (t[j + 4] << 24));
		pt[i + 3] = (uint32_t)(((t[j + 4] >> 8) & ((1 << 14) - 1)) | (t[j + 5] << 14));
		pt[i + 4] = (uint32_t)(((t[j + 5] >> 18) & ((1 << 4) - 1)) | ((t[j + 6] & maskb1) << 4) | (t[j + 7] << 26));
		pt[i + 5] = (uint32_t)(((t[j + 7] >> 6) & ((1 << 16) - 1)) | (t[j + 8] << 16));
		pt[i + 6] = (uint32_t)(((t[j + 8] >> 16) & ((1 << 6) - 1)) | ((t[j + 9] & maskb1) << 6) | (t[j + 10] << 28));
		pt[i + 7] = (uint32_t)(((t[j + 10] >> 4) & ((1 << 18) - 1)) | (t[j + 11] << 18));
		pt[i + 8] = (uint32_t)(((t[j + 11] >> 14) & ((1 << 8) - 1)) | ((t[j + 12] & maskb1) << 8) | (t[j + 13] << 30));
		pt[i + 9] = (uint32_t)(((t[j + 13] >> 2) & ((1 << 20) - 1)) | (t[j + 14] << 20));
		pt[i + 10] = (uint32_t)(((t[j + 14] >> 12) & ((1 << 10) - 1)) | (t[j + 15] << 10));
		j += 16;
	}
	memcpy(&sm[PARAM_N * (PARAM_B_BITS + 1) / 8], c, CRYPTO_C_BYTES);
}

static void decode_sig(unsigned char *c, poly z, const unsigned char *sm) { // Decode signature sm
	unsigned int i, j = 0;
	uint32_t *pt = (uint32_t *) sm;

	for (i = 0; i < PARAM_N; i += 16) {
		z[i + 0] = ((int32_t) pt[j + 0] << 10) >> 10;
		z[i + 1] = (int32_t)(pt[j + 0] >> 22) | ((int32_t)(pt[j + 1] << 20) >> 10);
		z[i + 2] = (int32_t)(pt[j + 1] >> 12) | ((int32_t)(pt[j + 2] << 30) >> 10);
		z[i + 3] = ((int32_t) pt[j + 2] << 8) >> 10;
		z[i + 4] = (int32_t)(pt[j + 2] >> 24) | ((int32_t)(pt[j + 3] << 18) >> 10);
		z[i + 5] = (int32_t)(pt[j + 3] >> 14) | ((int32_t)(pt[j + 4] << 28) >> 10);
		z[i + 6] = ((int32_t) pt[j + 4] << 6) >> 10;
		z[i + 7] = (int32_t)(pt[j + 4] >> 26) | ((int32_t)(pt[j + 5] << 16) >> 10);
		z[i + 8] = (int32_t)(pt[j + 5] >> 16) | ((int32_t)(pt[j + 6] << 26) >> 10);
		z[i + 9] = ((int32_t) pt[j + 6] << 4) >> 10;
		z[i + 10] = (int32_t)(pt[j + 6] >> 28) | ((int32_t)(pt[j + 7] << 14) >> 10);
		z[i + 11] = (int32_t)(pt[j + 7] >> 18) | ((int32_t)(pt[j + 8] << 24) >> 10);
		z[i + 12] = ((int32_t) pt[j + 8] << 2) >> 10;
		z[i + 13] = (int32_t)(pt[j + 8] >> 30) | ((int32_t)(pt[j + 9] << 12) >> 10);
		z[i + 14] = (int32_t)(pt[j + 9] >> 20) | ((int32_t)(pt[j + 10] << 22) >> 10);
		z[i + 15] = (int32_t) pt[j + 10] >> 10;
		j += 11;
	}
	memcpy(c, &sm[PARAM_N * (PARAM_B_BITS + 1) / 8], CRYPTO_C_BYTES);
}

#endif
