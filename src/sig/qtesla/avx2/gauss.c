/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: portable, constant-time Gaussian sampler
**************************************************************************************/

#include <string.h>
#include "api.h"
#include <oqs/sha3.h>
#include "gauss.h"

#define CDT_simple // Select simple CDT-based sampler

#if defined(CDT_Batcher) // Using CDT sampler based on Batcher's algorithm

#if (RADIX == 32)
#include "CDT32.h"
#elif (RADIX == 64)
#include "CDT64.h"
#endif

#define DFIELD ((sdigit_t)(~(digit_t) 0 >> 1))

#define PRODIFF(diff, a_u, a_v, k)                                            \
	{                                                                         \
		diff = (diff + (a_v[k] & DFIELD) - (a_u[k] & DFIELD)) >> (RADIX - 1); \
	}

#define PROSWAP(swap, diff, a_u, a_v, k) \
	{                                    \
		swap = (a_u[k] ^ a_v[k]) & diff; \
		a_u[k] ^= swap;                  \
		a_v[k] ^= swap;                  \
	}

#define PROSWAPG(swap, diff, g_u, g_v)       \
	{                                        \
		swap = (g_u ^ g_v) & (int32_t) diff; \
		g_u ^= swap;                         \
		g_v ^= swap;                         \
	}

#define MINMAX0(swap, diff, a_u, a_v)     \
	{                                     \
		PRODIFF(diff, a_u, a_v, 0);       \
		PROSWAP(swap, diff, a_u, a_v, 0); \
	}

#if CDT_COLS > 1
#define MINMAX1(swap, diff, a_u, a_v)     \
	{                                     \
		PRODIFF(diff, a_u, a_v, 1);       \
		MINMAX0(swap, diff, a_u, a_v);    \
		PROSWAP(swap, diff, a_u, a_v, 1); \
	}
#else
#define MINMAX1(swap, diff, a_u, a_v) MINMAX0(swap, diff, a_u, a_v)
#endif

#if CDT_COLS > 2
#define MINMAX2(swap, diff, a_u, a_v)     \
	{                                     \
		PRODIFF(diff, a_u, a_v, 2);       \
		MINMAX1(swap, diff, a_u, a_v);    \
		PROSWAP(swap, diff, a_u, a_v, 2); \
	}
#else
#define MINMAX2(swap, diff, a_u, a_v) MINMAX1(swap, diff, a_u, a_v)
#endif

#if CDT_COLS > 3
#define MINMAX3(swap, diff, a_u, a_v)     \
	{                                     \
		PRODIFF(diff, a_u, a_v, 3);       \
		MINMAX2(swap, diff, a_u, a_v);    \
		PROSWAP(swap, diff, a_u, a_v, 3); \
	}
#else
#define MINMAX3(swap, diff, a_u, a_v) MINMAX2(swap, diff, a_u, a_v)
#endif

#if CDT_COLS > 4
#define MINMAX4(swap, diff, a_u, a_v)     \
	{                                     \
		PRODIFF(diff, a_u, a_v, 4);       \
		MINMAX3(swap, diff, a_u, a_v);    \
		PROSWAP(swap, diff, a_u, a_v, 4); \
	}
#else
#define MINMAX4(swap, diff, a_u, a_v) MINMAX3(swap, diff, a_u, a_v)
#endif

#if CDT_COLS > 5
#define MINMAX5(swap, diff, a_u, a_v)     \
	{                                     \
		PRODIFF(diff, a_u, a_v, 5);       \
		MINMAX4(swap, diff, a_u, a_v);    \
		PROSWAP(swap, diff, a_u, a_v, 5); \
	}
#else
#define MINMAX5(swap, diff, a_u, a_v) MINMAX4(swap, diff, a_u, a_v)
#endif

#if CDT_COLS > 6
#define MINMAX6(swap, diff, a_u, a_v)     \
	{                                     \
		PRODIFF(diff, a_u, a_v, 6);       \
		MINMAX5(swap, diff, a_u, a_v);    \
		PROSWAP(swap, diff, a_u, a_v, 6); \
	}
#else
#define MINMAX6(swap, diff, a_u, a_v) MINMAX5(swap, diff, a_u, a_v)
#endif

#if CDT_COLS > 7
#define MINMAX7(swap, diff, a_u, a_v)     \
	{                                     \
		PRODIFF(diff, a_u, a_v, 7);       \
		MINMAX6(swap, diff, a_u, a_v);    \
		PROSWAP(swap, diff, a_u, a_v, 7); \
	}
#else
#define MINMAX7(swap, diff, a_u, a_v) MINMAX6(swap, diff, a_u, a_v)
#endif

#if CDT_COLS <= 8
#define MINIMAX(a_u, a_v, g_u, g_v)      \
	{                                    \
		sdigit_t diff = 0, swapa;        \
		int32_t swapg;                   \
		MINMAX7(swapa, diff, a_u, a_v);  \
		PROSWAPG(swapg, diff, g_u, g_v); \
	}
#else
#error "Unsupported precision"
#endif

/**
 * Sort the key-ord array using Knuth's iterative merge-exchange sorting.
 *
 * @param    a      the sampling key array to sort in-place.
 * @param    g      the accompanying sampling order array to sort together.
 * @param    n      the array size.
 */
static void knuthMergeExchangeKG(sdigit_t a[/*n*CDT_COLS*/], int32_t g[/*n*/], size_t n) {
	size_t t = 1;
	while (t < n - t) {
		t += t;
	}
	for (size_t p = t; p > 0; p >>= 1) {
		sdigit_t *ap = a + p * CDT_COLS;
		sdigit_t *a_i = a, *ap_i = ap;
		int32_t *gp = g + p;
		for (size_t i = 0; i < n - p; i++, a_i += CDT_COLS, ap_i += CDT_COLS) {
			if (!(i & p)) {
				MINIMAX(a_i, ap_i, g[i], gp[i]);
			}
		}
		for (size_t q = t; q > p; q >>= 1) {
			sdigit_t *ap_i = ap, *aq_i = a + q * CDT_COLS;
			int32_t *gq = g + q;
			for (size_t i = 0; i < n - q; i++, ap_i += CDT_COLS, aq_i += CDT_COLS) {
				if (!(i & p)) {
					MINIMAX(ap_i, aq_i, gp[i], gq[i]);
				}
			}
		}
	}
}

#define MINMAXG(a_u, a_v)                                                            \
	{                                                                                \
		int32_t diff = ((a_v & 0x7FFFFFFFL) - (a_u & 0x7FFFFFFFL)) >> (RADIX32 - 1); \
		int32_t swap = (a_u ^ a_v) & diff;                                           \
		a_u ^= swap;                                                                 \
		a_v ^= swap;                                                                 \
	}

/*
 * Sort the sampling order array using Knuth's iterative merge-exchange sorting.
 *
 * @param    a      the sampling order array to sort in-place.
 * @param    n      the array size.
 */
static void knuthMergeExchangeG(int32_t a[/*n*/], size_t n) {
	size_t t = 1;
	while (t < n - t) {
		t += t;
	}
	for (size_t p = t; p > 0; p >>= 1) {
		int32_t *ap = a + p;
		for (size_t i = 0; i < n - p; i++) {
			if (!(i & p)) {
				MINMAXG(a[i], ap[i]);
			}
		}
		for (size_t q = t; q > p; q >>= 1) {
			int32_t *aq = a + q;
			for (size_t i = 0; i < n - q; i++) {
				if (!(i & p)) {
					MINMAXG(ap[i], aq[i]);
				}
			}
		}
	}
}

static void kmxGauss(int32_t z[/*CHUNK_SIZE*/], const unsigned char *seed, int nonce) { // Generate CHUNK_SIZE samples from the normal distribution in constant-time
	sdigit_t sampk[(CHUNK_SIZE + CDT_ROWS) * CDT_COLS];
	int32_t sampg[CHUNK_SIZE + CDT_ROWS];

	// Fill each entry's sorting key with uniformly random data, and append the CDT values
	cSHAKE((uint8_t *) sampk, CHUNK_SIZE * CDT_COLS * sizeof(sdigit_t), (int16_t) nonce, seed, CRYPTO_RANDOMBYTES);
	memcpy(sampk + CHUNK_SIZE * CDT_COLS, cdt_v, CDT_ROWS * CDT_COLS * sizeof(sdigit_t));

	// Keep track each entry's sampling order
	for (int32_t i = 0; i < CHUNK_SIZE; i++)
		sampg[i] = i << 16;
	// Append the CDT Gaussian indices (prefixed with a sentinel)
	for (int32_t i = 0; i < CDT_ROWS; i++)
		sampg[CHUNK_SIZE + i] = 0xFFFF0000L ^ i;

	// Constant-time sorting according to the uniformly random sorting key
	knuthMergeExchangeKG(sampk, sampg, CHUNK_SIZE + CDT_ROWS);

	// Set each entry's Gaussian index
	int32_t prev_inx = 0;
	for (int i = 0; i < CHUNK_SIZE + CDT_ROWS; i++) {
		int32_t curr_inx = sampg[i] & 0xFFFFL;
		// prev_inx < curr_inx => prev_inx - curr_inx < 0 => (prev_inx - curr_inx) >> 31 = 0xF...F else 0x0...0
		prev_inx ^= (curr_inx ^ prev_inx) & ((prev_inx - curr_inx) >> (RADIX32 - 1));
		int32_t neg = (int32_t)(sampk[i * CDT_COLS] >> (RADIX - 1)); // Only the (so far unused) msb of the leading word
		sampg[i] |= ((neg & -prev_inx) ^ (~neg & prev_inx)) & 0xFFFFL;
	}

	// Sort all index entries according to their sampling order as sorting key
	knuthMergeExchangeG(sampg, CHUNK_SIZE + CDT_ROWS);

	// Discard the trailing entries (corresponding to the CDT) and sample the signs
	for (int i = 0; i < CHUNK_SIZE; i++) {
		z[i] = (sampg[i] << (RADIX32 - 16)) >> (RADIX32 - 16);
	}
}

static void sample_gauss_poly(poly z, const unsigned char *seed, int nonce) { // Gaussian sampler
	int dmsp = nonce << 8;

	for (int chunk = 0; chunk < PARAM_N; chunk += CHUNK_SIZE) {
		kmxGauss(z + chunk, seed, dmsp++);
	}
}

#elif defined(CDT_simple) // Using simple CDT-based sampler

#include "CDT32.h"

static void sample_gauss_poly(poly z, const unsigned char *seed, int nonce) {
	int dmsp = nonce << 8;
	int32_t samp[CHUNK_SIZE * CDT_COLS], c[CDT_COLS], borrow, sign;
	const int32_t mask = (int32_t)((uint32_t)(-1) >> 1);

	for (int chunk = 0; chunk < PARAM_N; chunk += CHUNK_SIZE) {
		cSHAKE((uint8_t *) samp, CHUNK_SIZE * CDT_COLS * sizeof(int32_t), (int16_t) dmsp++, seed, CRYPTO_RANDOMBYTES);
		for (int i = 0; i < CHUNK_SIZE; i++) {
			z[chunk + i] = 0;
			for (int j = 1; j < CDT_ROWS; j++) {
				borrow = 0;
				for (int k = CDT_COLS - 1; k >= 0; k--) {
					c[k] = (samp[i * CDT_COLS + k] & mask) - (cdt_v[j * CDT_COLS + k] + borrow);
					borrow = c[k] >> (RADIX32 - 1);
				}
				z[chunk + i] += ~borrow & 1;
			}
			sign = samp[i * CDT_COLS] >> (RADIX32 - 1);
			z[chunk + i] = (sign & -z[chunk + i]) | (~sign & z[chunk + i]);
		}
	}
}

#endif
