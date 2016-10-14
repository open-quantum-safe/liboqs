/**
 * \file    qcmdpc.c
 * \brief   Implementation of the Niederreiter cryptosystem using
 *          Quasi-Cyclic Medium-Density Parity-Check codes.
 * \author  Sebastian R. Verschoor <srverschoor@uwaterloo.ca>
 * \date    2016-10-14
 */

#include <stdlib.h>

#include <oqs/rand.h>

#include "nrqcmdpc.h"
#include "config.h"
#include "types.h"

#include "external/fips202.c"


/**
 * \section Polynomials
 */


/**
 * f $= polynomial
 *
 * Length (in bits) = POLY_BITS
 * Hamming weight = POLY_WEIGHT
 * Implemented with rejection sampling.
 */
static void sp_poly_rand(sp_poly_t f, OQS_RAND *rand) {
	static const index_t MASK = ((1 << POLY_INDEX_BITS) - 1);

	index_t cand, buf[3 * POLY_WEIGHT];
	size_t i, j, weight = 0;

	do {
		OQS_RAND_n(rand, (byte_t *)buf, 3 * POLY_WEIGHT * INDEX_BYTES);
		for (i = 0; i < 2 * POLY_WEIGHT && weight < POLY_WEIGHT; ++i) {
			cand = buf[i] & MASK;
			if (cand >= POLY_BITS) {
				continue;
			}
			for (j = 0; j < weight; ++j) {
				if (cand == f[j]) {
					break;
				}
			}
			if (j < weight) {
				continue;
			}
			f[weight++] = cand;
		}
	} while (weight < POLY_WEIGHT);
}

static void sp_poly_to_poly(poly_t f, const sp_poly_t g) {
	static const index_t LIMB_INDEX_MASK = (1 << LIMB_INDEX_BITS) - 1;

	size_t i, j, limb_index[POLY_WEIGHT];
	limb_t mask, bit[POLY_WEIGHT];

	for (j = 0; j < POLY_WEIGHT; ++j) {
		limb_index[j] = g[j] >> LIMB_INDEX_BITS;
		bit[j] = (limb_t)(1) << (g[j] & LIMB_INDEX_MASK);
	}
	for (i = 0; i < POLY_LIMBS; ++i) {
		f[i] = 0;
		for (j = 0; j < POLY_WEIGHT; ++j) {
			mask = limb_index[j] ^ i;
			mask = -((mask - 1) >> (LIMB_BITS - 1));
			f[i] |= bit[j] & mask;
		}
	}
}

static void poly_zero(poly_t f) {
	size_t i;
	for (i = 0; i < POLY_LIMBS; ++i) {
		f[i] = 0;
	}
}

static void poly_copy(poly_t f, const poly_t g) {
	size_t i;
	for (i = 0; i < POLY_LIMBS; ++i) {
		f[i] = g[i];
	}
}

static int poly_is_zero(const poly_t f) {
	size_t i, shift;
	byte_t nonzero_bits = 0;

	for (i = 0; i < POLY_LIMBS - 1; ++i) {
		for (shift = 0; shift < LIMB_BYTES; shift += 8) {
			nonzero_bits |= f[i] >> shift;
		}
	}
	for (shift = 0; shift < TAIL_BYTES; shift += 8) {
		nonzero_bits |= f[i] >> shift;
	}
	return ((nonzero_bits - 1) >> 8) & 1;
}

/**
 * \return f == 0 ? 0 : -1
 *
 * \warning Not in constant time.
 */
static int poly_verify_one(const poly_t f) {
	size_t i;
	if (f[0] != 1) {
		return -1;
	}
	for (i = 1; i < POLY_LIMBS; ++i) {
		if (f[i] != 0) {
			return -1;
		}
	}
	return 0;
}

/* TODO: if the processor has a built-in popcount, that is probably
 * more efficient */
static limb_t poly_hamming_weight(const poly_t f) {
	static const limb_t MASK0 = (limb_t)(~0) / 3;
	static const limb_t MASK1 = (limb_t)(~0) / 15 * 3;
	static const limb_t MASK2 = (limb_t)(~0) / 255 * 15;
	static const limb_t MULT  = (limb_t)(~0) / 255;
	limb_t wc, total = 0;
	size_t i;
	for (i = 0; i < POLY_LIMBS; ++i) {
		wc   = f[i];
		wc  -= (wc >> 1) & MASK0;
		wc   = (wc & MASK1) + ((wc >> 2) & MASK1);
		wc  += wc >> 4;
		wc  &= MASK2;
		wc  *= MULT;
		wc >>= LIMB_BITS - 8;
		total += wc;
	}
	return total;
}

/**
 * f := g + h
 *
 * Any subset of {f, g, h} may point to the same memory. Misaligned
 * overlap in memory leads to erroneous behaviour.
 */
static void poly_add(poly_t f, const poly_t g, const poly_t h) {
	size_t i;
	for (i = 0; i < POLY_LIMBS; ++i) {
		f[i] = g[i] ^ h[i];
	}
}

static void poly_mask(poly_t f, const poly_t g, const poly_t mask) {
	size_t i;
	for (i = 0; i < POLY_LIMBS; ++i) {
		f[i] = g[i] & mask[i];
	}
}

/**
 * f += g & mask
 *
 * Mask is extended to the length of \p g by repeating.
 */
static void poly_inplace_add_masked(poly_t f, const poly_t g, limb_t mask) {
	size_t i;
	for (i = 0; i < POLY_LIMBS; ++i) {
		f[i] ^= g[i] & mask;
	}
}

/* TODO: multiple carry chains? */
static void poly_inplace_mulx_modG(poly_t f) {
	size_t i;
	limb_t carry, tmp;
	carry = f[POLY_LIMBS - 1] >> (TAIL_BITS - 1);
	for (i = 0; i < POLY_LIMBS - 1; ++i) {
		tmp = f[i] >> (LIMB_BITS - 1);
		f[i] <<= 1;
		f[i] |= carry;
		carry = tmp;
	}
#if TAIL_BITS == 1
	f[i] = carry;
#else
	f[i] <<= 1;
	f[i] |= carry;
	f[i] &= TAIL_MASK;
#endif
}

/* TODO: multiple carry chains? */
static void poly_divx(poly_t f, const poly_t g) {
	size_t i;
	for (i = 0; i < POLY_LIMBS - 1; ++i) {
		f[i] = (g[i] >> 1) | (g[i + 1] << (LIMB_BITS - 1));
	}
	f[i] = g[i] >> 1;
}

/* TODO: multiple carry chains? */
static void poly_divx_modG(poly_t f, const poly_t g) {
	size_t i;
	limb_t carry;
	carry = (g[0] & 1) << (TAIL_BITS - 1);
	for (i = 0; i < POLY_LIMBS - 1; ++i) {
		f[i] = (g[i] >> 1) | (g[i + 1] << (LIMB_BITS - 1));
	}
#if TAIL_BITS == 1
	f[i] = carry;
#else
	f[i] = g[i] >> 1;
	f[i] |= carry;
#endif
}

/**
 * f := g mod G
 *
 * g is the result of a non-reduced multiplication: it is assumed to
 * have 2*`POLY_LIMBS`-1 words.
 */
static void poly_reduce(poly_t f, const limb_t *g) {
	size_t i;
	for (i = 0; i < POLY_LIMBS - 1; ++i) {
		f[i] = g[i];
		f[i] ^= g[i + POLY_LIMBS - 1] >> TAIL_BITS;
		f[i] ^= g[i + POLY_LIMBS] << (LIMB_BITS - TAIL_BITS);
	}
	f[i] = g[i] & TAIL_MASK;
}

/* This function implements schoolbook multiplication, with reduction
 * mod G postponed until the very end.  There is much room for
 * optimizations, for example, Karatsuba multiplication should achieve
 * a great speedup, as would FFT multiplication.  Using the CLMUL
 * instruction (where available) is another option (but less
 * portable).
 */
static void poly_mul(poly_t f, const poly_t g, const poly_t h) {
	byte_t bit_index;
	size_t i;
	limb_t mask, res[2 * POLY_LIMBS - 1] = {0};
	poly_t lhs;

	poly_copy(lhs, g);
	for (i = 0; i < POLY_LIMBS; ++i) {
		mask = -(h[i] & 1);
		poly_inplace_add_masked(res + i, lhs, mask);
	}
	for (bit_index = 1; bit_index < LIMB_BITS; ++bit_index) {
		poly_inplace_mulx_modG(lhs);
		for (i = 0; i < POLY_LIMBS; ++i) {
			mask = -((h[i] >> bit_index) & 1);
			poly_inplace_add_masked(res + i, lhs, mask);
		}
	}
	poly_reduce(f, res);
}

/** Compare polynomials
 *
 * This function is used in the context of the xgcd.  For equality,
 * the polynomials need to be exactly the same.  To find out which
 * polynomial is greater, only the degree matters.  If the degrees
 * are equal, it does not matter what is returned in \p lt.
 *
 * \param[out] eq f == g ? -1 : 0
 * \param[out] lt f < g ? -1 : 0
 */
static void poly_compare(poly_t eq, poly_t lt, const poly_t f, const poly_t g) {
	size_t word = POLY_LIMBS - 1, shift;
	byte_t f_byte, g_byte;
	*lt = 0;
	*eq = 1;
	shift = 8 * TAIL_BYTES;
	while (shift > 0) {
		shift -= 8;
		f_byte = (byte_t)(f[word] >> shift);
		g_byte = (byte_t)(g[word] >> shift);
		*lt |= *eq & ((f_byte - g_byte) >> 8);
		*eq &= ((f_byte ^ g_byte) - 1) >> 8;
	}
	while (word > 0) {
		--word;
		shift = LIMB_BITS;
		while (shift > 0) {
			shift -= 8;
			f_byte = (byte_t)(f[word] >> shift);
			g_byte = (byte_t)(g[word] >> shift);
			*lt |= *eq & ((f_byte - g_byte) >> 8);
			*eq &= ((f_byte ^ g_byte) - 1) >> 8;
		}
	}
	*eq = -(*eq);
	*lt = -(*lt);
}

static void poly_xgcd(poly_t f, poly_t a) {
	size_t i, j;
	poly_t g = {1}, b = {0};
	g[POLY_LIMBS - 1] = 1 << TAIL_BITS; /* g := G */

	/* Compute every iteration:
	 *      if (even(f)) { f = f_div_x;  a = a_div_x; }
	 * else if (even(g)) { g = g_div_x;  b = b_div_x; }
	 * else if (f > g)   { f = fg_div_x; a = ab_div_x; }
	 * else              { g = fg_div_x; b = ab_div_x; }
	 */
	for (i = 0; i < 2 * POLY_BITS - 1; ++i) {
		poly_t f_divx, g_divx, a_divx, b_divx, fg_divx, ab_divx;
		limb_t done, f_odd, g_odd, f_lt_g;
		limb_t f2f, f2fx, f2fgx, g2g, g2gx, g2fgx;

		poly_divx(f_divx, f);
		poly_divx(g_divx, g);
		poly_add(fg_divx, f_divx, g_divx);
		poly_divx_modG(a_divx, a);
		poly_divx_modG(b_divx, b);
		poly_add(ab_divx, a_divx, b_divx);

		poly_compare(&done, &f_lt_g, f, g);
		f_odd = -(f[0] & 1);
		g_odd = -(g[0] & 1);

#ifndef ALT_XGCD_MASKS
		f2f   =  done | ( f_odd & (~g_odd |  f_lt_g));
		f2fx  = ~done &  ~f_odd;
		f2fgx = ~done &   f_odd &   g_odd & ~f_lt_g;
		g2g   =  done | (~f_odd | ( g_odd & ~f_lt_g));
		g2gx  = ~done & ( f_odd &  ~g_odd);
		g2fgx = ~done &   f_odd &   g_odd &  f_lt_g;
#else
		f2fx  = ~f_odd;
		f2fgx = f_odd & g_odd & f_gt_g;
		g2g   = f2fx | f2fgx;
		f2f   = ~g2g;
		g2gx  = f2f & g_odd;
		g2fgx = ~(g2g | g2gx);
# endif

		for (j = 0; j < POLY_LIMBS; ++j) {
			f[j] = (f[j] & f2f) | (f_divx[j] & f2fx) | (fg_divx[j] & f2fgx);
			a[j] = (a[j] & f2f) | (a_divx[j] & f2fx) | (ab_divx[j] & f2fgx);
			g[j] = (g[j] & g2g) | (g_divx[j] & g2gx) | (fg_divx[j] & g2fgx);
			b[j] = (b[j] & g2g) | (b_divx[j] & g2gx) | (ab_divx[j] & g2fgx);
		}
	}
}

static int poly_inv(poly_t f) {
	int ret;
	poly_t inv = {1};
	poly_xgcd(f, inv);
	if ((ret = poly_verify_one(f))) {
		return ret;
	}
	poly_copy(f, inv);
	return ret;
}


/**
 * \section Errors
 */

/**
 * \def ERROR_BITS Error size (in bits)
 */
#define ERROR_BITS (POLY_COUNT * POLY_BITS)

/**
 * error $= polynomial
 *
 * Length (in bits) = POLY_COUNT * POLY_BITS
 * Hamming weight = ERROR_WEIGHT
 * Implemented with rejection sampling.
 */
static void sp_error_rand(sp_error_t error, OQS_RAND *rand) {
	static const index_t MASK = ((1 << ERROR_INDEX_BITS) - 1);

	index_t cand, buf[3 * ERROR_WEIGHT];
	size_t i, j, weight = 0;

	do {
		OQS_RAND_n(rand, (byte_t *)buf, 3 * ERROR_WEIGHT * INDEX_BYTES);
		for (i = 0; i < 2 * ERROR_WEIGHT && weight < ERROR_WEIGHT; ++i) {
			cand = buf[i] & MASK;
			if (cand >= ERROR_BITS) {
				continue;
			}
			for (j = 0; j < weight; ++j) {
				if (cand == error[j]) {
					break;
				}
			}
			if (j < weight) {
				continue;
			}
			error[weight++] = cand;
		}
	} while (weight < ERROR_WEIGHT);
}

static void sp_error_copy(sp_error_t f, const sp_error_t g) {
	size_t i;

	for (i = 0; i < ERROR_WEIGHT; ++i) {
		f[i] = g[i];
	}
}

/**
 * Align error indices > POLY_BITS to the next lower polynomial.
 * Subtraction saturates to -1.
 */
static void sp_error_align(sp_error_t error) {
	size_t i;
	index_t mask;

	for (i = 0; i < ERROR_WEIGHT; ++i) {
		error[i] -= POLY_BITS;
		mask = -(error[i] >> (INDEX_BITS - 1));
		error[i] |= mask;
	}
}

static void sp_error_to_poly(poly_t f, const sp_error_t g) {
	static const index_t LIMB_INDEX_MASK = (1 << LIMB_INDEX_BITS) - 1;

	size_t i, j, limb_index[ERROR_WEIGHT];
	limb_t mask, bit[ERROR_WEIGHT];

	for (j = 0; j < ERROR_WEIGHT; ++j) {
		limb_index[j] = g[j] >> LIMB_INDEX_BITS;
		bit[j] = (limb_t)(1) << (g[j] & LIMB_INDEX_MASK);
	}
	for (i = 0; i < POLY_LIMBS; ++i) {
		f[i] = 0;
		for (j = 0; j < ERROR_WEIGHT; ++j) {
			mask = limb_index[j] ^ i;
			mask = -((mask - 1) >> (LIMB_BITS - 1));
			f[i] |= bit[j] & mask;
		}
	}
	f[POLY_LIMBS - 1] &= TAIL_MASK;
}

static void sp_error_to_error(error_t f, const sp_error_t g) {
	size_t i;
	sp_error_t g_copy;

	sp_error_copy(g_copy, g);
	sp_error_to_poly(f[0], g_copy);
	for (i = 1; i < POLY_COUNT; ++i) {
		sp_error_align(g_copy);
		sp_error_to_poly(f[i], g_copy);
	}
}


/**
 * \section Key encapsulation mechanism (KEM)
 */


/**
 * Generate the private key and compute the inverse of the last block
 */
static void kem_gen_par_ch(
    poly_t inv,
    par_ch_t priv_key,
    OQS_RAND *rand) {
	size_t i;

	for (i = 0; i < POLY_COUNT - 1; ++i) {
		sp_poly_rand(priv_key[i], rand);
	}
	do {
		sp_poly_rand(priv_key[POLY_COUNT - 1], rand);
		sp_poly_to_poly(inv, priv_key[POLY_COUNT - 1]);
	} while (poly_inv(inv));
}


/**
 * Convert the parity-check matrix into a systematic one
 */
static void kem_to_systematic(
    sys_par_ch_t pub_key,
    const poly_t inv,
    const par_ch_t priv_key) {
	size_t i;
	poly_t priv_poly;

	for (i = 0; i < POLY_COUNT - 1; ++i) {
		sp_poly_to_poly(priv_poly, priv_key[i]);
		poly_mul(pub_key[i], inv, priv_poly);
	}
}

/**
 * Generate a random key-pair
 */
static void kem_keypair(
    sys_par_ch_t pub_key,
    par_ch_t priv_key,
    OQS_RAND *rand) {
	poly_t inv;

	kem_gen_par_ch(inv, priv_key, rand);
	kem_to_systematic(pub_key, inv, priv_key);
}

/**
 * Generate a random error
 */
static void kem_error(
    error_t error,
    OQS_RAND *rand) {
	sp_error_t sp_error;

	sp_error_rand(sp_error, rand);
	sp_error_to_error(error, sp_error);
}

/**
 * Encrypt the error
 *
 * @param[out] pub_syn  Public syndrome (ciphertext)
 * @param[in]  error      Error (plaintext)
 * @param[in]  pub_key  Systematic QC-MDPC code (public key)
 */
static void kem_encrypt(
    syn_t pub_syn,
    error_t error,
    const sys_par_ch_t pub_key) {
	syn_t buf;
	size_t i;

	poly_mul(pub_syn, pub_key[0], error[0]);
	for (i = 1; i < POLY_COUNT - 1; ++i) {
		poly_mul(buf, pub_key[i], error[i]);
		poly_add(pub_syn, pub_syn, buf);
	}
	poly_add(pub_syn, pub_syn, error[POLY_COUNT - 1]);
}

/**
 * Decode the provided (private) syndrome to find the corresponding
 * error vector.
 *
 * @param[out]    error       out: Resulting error vector
 * @param[in,out] priv_syn  in:  Private syndrome
 *                          out: Syndrome of resulting error vector
 *                               (zero upon success)
 * @param[in]     priv_key  Parity check polynomial
 */
static void kem_decode(
    error_t error,
    syn_t priv_syn,
    const par_ch_t priv_key) {
	static const limb_t threshold[] = THRESHOLDS;
	static const size_t ITERATIONS = sizeof(threshold) / sizeof(threshold[0]);

	size_t i, p, l;
	limb_t bit, mask, count;
	syn_t syn_update, masked;
	dense_par_ch_t priv_key_dense;

	for (p = 0; p < POLY_COUNT; ++p) {
		for (l = 0; l < POLY_LIMBS; ++l) {
			error[p][l] = 0;
		}
	}
	for (p = 0; p < POLY_COUNT; ++p) {
		sp_poly_to_poly(priv_key_dense[p], priv_key[p]);
	}
	for (i = 0; i < ITERATIONS; ++i) {
		poly_zero(syn_update);
		for (p = 0; p < POLY_COUNT; ++p) {
			for (l = 0; l < POLY_LIMBS - 1; ++l) {
				for (bit = 1; bit != 0; bit <<= 1) {
					poly_mask(masked, priv_key_dense[p], priv_syn);
					count = poly_hamming_weight(masked);
					mask = -((threshold[i] - count - 1) >> (LIMB_BITS - 1));
					error[p][l] ^= bit & mask;
					poly_inplace_add_masked(syn_update, priv_key_dense[p], mask);
					poly_inplace_mulx_modG(priv_key_dense[p]);
				}
			}
			for (bit = 1; bit != (limb_t)(1) << TAIL_BITS; bit <<= 1) {
				poly_mask(masked, priv_key_dense[p], priv_syn);
				count = poly_hamming_weight(masked);
				mask = -(((threshold[i] - count - 1) >> (LIMB_BITS - 1)) & 1);
				error[p][POLY_LIMBS - 1] ^= bit & mask;
				poly_inplace_add_masked(syn_update, priv_key_dense[p], mask);
				poly_inplace_mulx_modG(priv_key_dense[p]);
			}
		}
		poly_add(priv_syn, priv_syn, syn_update);
	}
}

/**
 * Decrypt the error
 *
 * @param[out]  error       Error (plaintext)
 * @param[in]   pub_syn   Public syndrome (ciphertext)
 * @param[in]   priv_key  QC-MDPC code (private key)
 *
 * @return 1 upon successful decryption, 0 otherwise
 */
static int kem_decrypt(
    error_t error,
    syn_t pub_syn,
    const par_ch_t priv_key) {
	poly_t priv_last;
	syn_t priv_syn;

	sp_poly_to_poly(priv_last, priv_key[POLY_COUNT - 1]);
	poly_mul(priv_syn, priv_last, pub_syn);
	kem_decode(error, priv_syn, priv_key);

	return poly_is_zero(priv_syn);
}

/**
 * \section Packing and unpacking data
 */

static void pack_poly(byte_t *dest, const poly_t src) {
	size_t i, j;

	for (i = 0; i < POLY_LIMBS - 1; ++i) {
		for (j = 0; j < LIMB_BYTES; ++j) {
			dest[i * LIMB_BYTES + j] = (byte_t)(src[i] >> (8 * j));
		}
	}
	for (j = 0; j < TAIL_BYTES; ++j) {
		dest[i * LIMB_BYTES + j] = (byte_t)(src[i] >> (8 * j));
	}
}

static void unpack_poly(poly_t dest, const byte_t *src) {
	size_t i, j;

	for (i = 0; i < POLY_LIMBS - 1; ++i) {
		dest[i] = src[i * LIMB_BYTES];
		for (j = 1; j < LIMB_BYTES; ++j) {
			dest[i] |= ((limb_t)src[i * LIMB_BYTES + j]) << (8 * j);
		}
	}
	dest[i] = src[i * LIMB_BYTES];
	for (j = 1; j < TAIL_BYTES; ++j) {
		dest[i] |= ((limb_t)src[i * LIMB_BYTES + j]) << (8 * j);
	}
}

static void pack_pubkey(byte_t *dest, const sys_par_ch_t src) {
	size_t i;
	byte_t *dest_poly = dest;
	for (i = 0; i < POLY_COUNT - 1; ++i) {
		pack_poly(dest_poly, src[i]);
		dest_poly += POLY_BYTES;
	}
}

static void unpack_pubkey(sys_par_ch_t dest, const byte_t *src) {
	size_t i;
	const byte_t *src_poly = src;
	for (i = 0; i < POLY_COUNT - 1; ++i) {
		unpack_poly(dest[i], src_poly);
		src_poly += POLY_BYTES;
	}
}

static void pack_error(byte_t *dest, const limb_t (*src)[POLY_LIMBS]) {
	size_t i;
	byte_t *dest_poly = dest;
	for (i = 0; i < POLY_COUNT; ++i) {
		pack_poly(dest_poly, src[i]);
		dest_poly += POLY_BYTES;
	}
}


/**
 * \section Key-exchange (KEX)
 */


int oqs_kex_code_nrqcmdpc_keypair(
    byte_t *pub_key,
    par_ch_t priv_key,
    OQS_RAND *rand) {
	sys_par_ch_t spc;

	kem_keypair(spc, priv_key, rand);
	pack_pubkey(pub_key, spc);

	return 0;
}

void oqs_kex_code_nrqcmdpc_encrypt(
    byte_t *sec_key,
    byte_t *ct,
    const byte_t *pub_key,
    OQS_RAND *rand) {
	byte_t error_packed[ERROR_BYTES];
	error_t error;
	poly_t pub_syn;
	sys_par_ch_t spc;

	unpack_pubkey(spc, pub_key);

	kem_error(error, rand);
	kem_encrypt(pub_syn, error, spc);
	pack_poly(ct, pub_syn);

	pack_error(error_packed, error);
	sha3256(sec_key, error_packed, ERROR_BYTES);
}

int oqs_kex_code_nrqcmdpc_decrypt(
    byte_t *sec_key,
    const byte_t *ct,
    const par_ch_t priv_key) {
	int ret;
	byte_t error_packed[ERROR_BYTES];
	error_t error;
	poly_t pub_syn;

	unpack_poly(pub_syn, ct);

	ret = kem_decrypt(error, pub_syn, priv_key);

	pack_error(error_packed, error);
	sha3256(sec_key, error_packed, ERROR_BYTES);

	return ret;
}
