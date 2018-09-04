/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: NTT, modular reduction and polynomial functions
**************************************************************************************/

#include "poly.h"
#include "api.h"
#include "sha3/fips202.h"

extern poly zeta;
extern poly zetainv;

int64_t reduce(int64_t a) { // Montgomery reduction
	int64_t u;

	u = (a * PARAM_QINV) & 0xFFFFFFFF;
	u *= PARAM_Q;
	a += u;
	return a >> 32;
}

int32_t barr_reduce(int64_t a) { // Barrett reduction
	int64_t u = ((a * PARAM_BARR_MULT) >> PARAM_BARR_DIV) * PARAM_Q;
	return a - u;
}

void ntt(poly a, const poly w) { // Forward NTT transform
	int NumoProblems = PARAM_N >> 1, jTwiddle = 0;

	for (; NumoProblems > 0; NumoProblems >>= 1) {
		int jFirst, j = 0;
		for (jFirst = 0; jFirst < PARAM_N; jFirst = j + NumoProblems) {
			int W = w[jTwiddle++];
			for (j = jFirst; j < jFirst + NumoProblems; j++) {
				int temp = reduce(W * a[j + NumoProblems]);
				a[j + NumoProblems] = a[j] + (PARAM_Q - temp);
				a[j] = temp + a[j];
			}
		}
	}
}

void nttinv(poly a, const poly w) { // Inverse NTT transform
	int NumoProblems = 1, jTwiddle = 0;
	for (NumoProblems = 1; NumoProblems < PARAM_N; NumoProblems *= 2) {
		int jFirst, j = 0;
		for (jFirst = 0; jFirst < PARAM_N; jFirst = j + NumoProblems) {
			int W = w[jTwiddle++];
			for (j = jFirst; j < jFirst + NumoProblems; j++) {
				int temp = a[j];
				a[j] = barr_reduce(temp + a[j + NumoProblems]);
				a[j + NumoProblems] =
				    reduce(W * (temp + (2 * PARAM_Q - a[j + NumoProblems])));
			}
		}
	}
}

static void poly_pointwise(
    poly result, const poly x,
    const poly y) { // Pointwise polynomial multiplication result = x.y
	unsigned int i;

	for (i = 0; i < PARAM_N; i++)
		result[i] = reduce(x[i] * y[i]);
}

void poly_mul(poly result, const poly x,
              const poly y) { // Polynomial multiplication result = x*y, with in
	                          // place reduction for (X^N+1)
	// The input x is assumed to be in NTT form
	poly y_ntt;

	for (int i = 0; i < PARAM_N; i++)
		y_ntt[i] = y[i];

	ntt(y_ntt, zeta);
	poly_pointwise(result, x, y_ntt);
	nttinv(result, zetainv);
}

void poly_add(poly result, const poly x,
              const poly y) { // Polynomial addition result = x+y
	unsigned int i;

	for (i = 0; i < PARAM_N; i++)
		result[i] = x[i] + y[i];
}

void poly_sub(poly result, const poly x,
              const poly y) { // Polynomial subtraction result = x-y
	unsigned int i;

	for (i = 0; i < PARAM_N; i++)
		result[i] = barr_reduce(x[i] + (2 * PARAM_Q - y[i]));
}

void poly_uniform(poly a,
                  const unsigned char *seed) { // Generation of polynomial "a"
	unsigned int pos = 0, i = 0, nbytes = (PARAM_Q_LOG + 7) / 8;
	unsigned int nblocks = PARAM_GEN_A;
	uint32_t val1, val2, val3, val4, mask = (1 << PARAM_Q_LOG) - 1;
	unsigned char buf[SHAKE128_RATE * nblocks];
	uint16_t dmsp = 0;

	cshake128_simple(buf, SHAKE128_RATE * nblocks, dmsp++, seed,
	                 CRYPTO_RANDOMBYTES);

	while (i < PARAM_N) {
		if (pos > SHAKE128_RATE * nblocks - 4 * nbytes) {
			nblocks = 1;
			cshake128_simple(buf, SHAKE128_RATE * nblocks, dmsp++, seed,
			                 CRYPTO_RANDOMBYTES);
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
		if (val1 < PARAM_Q && i < PARAM_N)
			a[i++] = reduce((int64_t) val1 * PARAM_R2_INVN);
		if (val2 < PARAM_Q && i < PARAM_N)
			a[i++] = reduce((int64_t) val2 * PARAM_R2_INVN);
		if (val3 < PARAM_Q && i < PARAM_N)
			a[i++] = reduce((int64_t) val3 * PARAM_R2_INVN);
		if (val4 < PARAM_Q && i < PARAM_N)
			a[i++] = reduce((int64_t) val4 * PARAM_R2_INVN);
	}
}
