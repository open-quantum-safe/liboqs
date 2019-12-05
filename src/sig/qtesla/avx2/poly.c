/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: NTT, modular reduction and polynomial functions
**************************************************************************************/

#include "poly.h"
#include <oqs/sha3.h>
#include "api.h"

// OQS note: commented out because files are included in the same file
//extern poly zeta;
//extern poly zetainv;

static void poly_uniform(poly_k a, const unsigned char *seed) { // Generation of polynomials "a_i"
	unsigned int pos = 0, i = 0, nbytes = (PARAM_Q_LOG + 7) / 8;
	unsigned int nblocks = PARAM_GEN_A;
	uint32_t val1, val2, val3, val4, mask = (uint32_t)(1 << PARAM_Q_LOG) - 1;
	unsigned char buf[OQS_SHA3_SHAKE128_RATE * PARAM_GEN_A];
	uint16_t dmsp = 0;

	OQS_SHA3_cshake128_simple(buf, OQS_SHA3_SHAKE128_RATE * PARAM_GEN_A, dmsp++, seed, CRYPTO_RANDOMBYTES);

	while (i < PARAM_K * PARAM_N) {
		if (pos > OQS_SHA3_SHAKE128_RATE * nblocks - 4 * nbytes) {
			nblocks = 1;
			OQS_SHA3_cshake128_simple(buf, OQS_SHA3_SHAKE128_RATE * nblocks, dmsp++, seed, CRYPTO_RANDOMBYTES);
			pos = 0;
		}
		val1 = (*(uint32_t *) (buf + pos)) & mask;
		pos += nbytes;
		val2 = (*(uint32_t *) (buf + pos)) & mask;
		pos += nbytes;
		val3 = (*(uint32_t *) (buf + pos)) & mask;
		pos += nbytes;
		val4 = (*(uint32_t *) (buf + pos)) & mask;
		pos += nbytes;
		if (val1 < PARAM_Q && i < PARAM_K * PARAM_N)
			a[i++] = reduce((int64_t) val1 * PARAM_R2_INVN);
		if (val2 < PARAM_Q && i < PARAM_K * PARAM_N)
			a[i++] = reduce((int64_t) val2 * PARAM_R2_INVN);
		if (val3 < PARAM_Q && i < PARAM_K * PARAM_N)
			a[i++] = reduce((int64_t) val3 * PARAM_R2_INVN);
		if (val4 < PARAM_Q && i < PARAM_K * PARAM_N)
			a[i++] = reduce((int64_t) val4 * PARAM_R2_INVN);
	}
}

static int32_t reduce(int64_t a) { // Montgomery reduction
	int64_t u;

	u = (a * PARAM_QINV) & 0xFFFFFFFF;
	u *= PARAM_Q;
	a += u;
	return (int32_t)(a >> 32);
}

static int64_t barr_reduce64(int64_t a) { // Barrett reduction
	int64_t u = (a * PARAM_BARR_MULT) >> PARAM_BARR_DIV;
	return a - u * PARAM_Q;
}

static int32_t barr_reduce(int32_t a) { // Barrett reduction
	digit_t u = ((int64_t) a * PARAM_BARR_MULT) >> PARAM_BARR_DIV;
	return a - (digit_t) u * PARAM_Q;
}

static void poly_ntt(poly2x x_ntt, const poly x) { // Call to NTT function. Avoids input destruction.
	// Output is in extended form.

	poly_ntt_asm(x_ntt, x, zeta);
}

static void poly_mul(poly result, const poly x, const poly2x y) { // Polynomial multiplication result = x*y, with in place reduction for (X^N+1)
	// The inputs x and y are assumed to be in NTT form
	// Input y is in extended form.
	poly2x prod;

	poly_pmul_asm(prod, x, y);
	poly_intt_asm(result, prod, zetainv);
}

static void poly_add(poly result, const poly x, const poly y) { // Polynomial addition result = x+y

	for (int i = 0; i < PARAM_N; i++)
		result[i] = x[i] + y[i];
}

static void poly_add_correct(poly result, const poly x, const poly y) { // Polynomial addition result = x+y with correction

	for (int i = 0; i < PARAM_N; i++) {
		result[i] = x[i] + y[i];
		result[i] += (result[i] >> (RADIX32 - 1)) & PARAM_Q; // If result[i] < 0 then add q
		result[i] -= PARAM_Q;
		result[i] += (result[i] >> (RADIX32 - 1)) & PARAM_Q; // If result[i] >= q then subtract q
	}
}

static void poly_sub(poly result, const poly x, const poly y) { // Polynomial subtraction result = x-y

	for (int i = 0; i < PARAM_N; i++)
		result[i] = x[i] - y[i];
}

static void poly_sub_reduce(poly result, const poly x, const poly y) { // Polynomial subtraction result = x-y

	for (int i = 0; i < PARAM_N; i++)
		result[i] = barr_reduce(x[i] - y[i]);
}

/********************************************************************************************
* Name:        sparse_mul8
* Description: performs sparse polynomial multiplication
* Parameters:  inputs:
*              - const unsigned char* s: part of the secret key
*              - const uint32_t pos_list[PARAM_H]: list of indices of nonzero elements in c
*              - const int16_t sign_list[PARAM_H]: list of signs of nonzero elements in c
*              outputs:
*              - poly prod: product of 2 polynomials
*
* Note: pos_list[] and sign_list[] contain public information since c is public
*********************************************************************************************/
static void sparse_mul8(poly prod, const unsigned char *s, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]) {
	int i, j, pos;
	int8_t *t = (int8_t *) s;

	for (i = 0; i < PARAM_N; i++)
		prod[i] = 0;

	for (i = 0; i < PARAM_H; i++) {
		pos = pos_list[i];
		for (j = 0; j < pos; j++) {
			prod[j] = prod[j] - sign_list[i] * t[j + PARAM_N - pos];
		}
		for (j = pos; j < PARAM_N; j++) {
			prod[j] = prod[j] + sign_list[i] * t[j - pos];
		}
	}
}

/********************************************************************************************
* Name:        sparse_mul32
* Description: performs sparse polynomial multiplication 
* Parameters:  inputs:
*              - const int32_t* pk: part of the public key
*              - const uint32_t pos_list[PARAM_H]: list of indices of nonzero elements in c
*              - const int16_t sign_list[PARAM_H]: list of signs of nonzero elements in c
*              outputs:
*              - poly prod: product of 2 polynomials
*********************************************************************************************/
static void sparse_mul32(poly prod, const int32_t *pk, const uint32_t pos_list[PARAM_H], const int16_t sign_list[PARAM_H]) {
	int i, j, pos;
	int64_t temp[PARAM_N] = {0};

	for (i = 0; i < PARAM_H; i++) {
		pos = pos_list[i];
		for (j = 0; j < pos; j++) {
			temp[j] = temp[j] - sign_list[i] * pk[j + PARAM_N - pos];
		}
		for (j = pos; j < PARAM_N; j++) {
			temp[j] = temp[j] + sign_list[i] * pk[j - pos];
		}
	}
	for (i = 0; i < PARAM_N; i++)
		prod[i] = (int32_t) barr_reduce64(temp[i]);
}
