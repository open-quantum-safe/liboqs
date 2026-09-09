// SPDX-License-Identifier: MIT

/**
 * Reduced memory implementation.
 *
 * Copyright (c) 2026 SNOVA TEAM
 */

#include <stdint.h>
#include <string.h>

#include "snova.h"
#include "symmetric.h"

#define SNOVA_olr (SNOVA_o * SNOVA_lr + 1)

/**
 * Initialization
 */

#if SNOVA_l == 2

static uint16_t gf_S[SNOVA_l * SNOVA_l2] = {
	0x1, 0x0, 0x0, 0x1, 0x8, 0x7, 0x7, 0x6
};

static uint16_t gf_Sx[SNOVA_l * SNOVA_l2] = {
	0x1, 0x0, 0x0, 0x1, 0x1000, 0x111, 0x111, 0x110
};

#elif SNOVA_l == 4

static uint16_t gf_S[SNOVA_l * SNOVA_l2] = {
	0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0,
	0x1, 0x8, 0x7, 0x6, 0x5, 0x7, 0x6, 0x5, 0x4, 0x6, 0x5, 0x4, 0x3, 0x5, 0x4, 0x3,
	0x2, 0xf, 0x6, 0x9, 0x1, 0x6, 0x0, 0x7, 0x4, 0x9, 0x7, 0x3, 0x0, 0x1, 0x4, 0x0,
	0x0, 0x6, 0x3, 0x8, 0x7, 0x3, 0xa, 0x4, 0xc, 0x8, 0x4, 0x7, 0x1, 0x7, 0xc, 0x1,
	0x6
};

static uint16_t gf_Sx[SNOVA_l * SNOVA_l2] = {
	0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0,
	0x1, 0x1000, 0x111, 0x110, 0x101, 0x111, 0x110, 0x101, 0x100, 0x110, 0x101, 0x100, 0x11, 0x101, 0x100, 0x11,
	0x10, 0x1111, 0x110, 0x1001, 0x1, 0x110, 0x0, 0x111, 0x100, 0x1001, 0x111, 0x11, 0x0, 0x1, 0x100, 0x0,
	0x0, 0x110, 0x11, 0x1000, 0x111, 0x11, 0x1010, 0x100, 0x1100, 0x1000, 0x100, 0x111, 0x1, 0x111, 0x1100, 0x1,
	0x110
};

#elif SNOVA_l == 5

static uint16_t gf_S[SNOVA_l * SNOVA_l2] = {
	0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x8, 0x7, 0x6, 0x5, 0x4, 0x7,
	0x6, 0x5, 0x4, 0x3, 0x6, 0x5, 0x4, 0x3, 0x2, 0x5, 0x4, 0x3, 0x2, 0x1, 0x4, 0x3,
	0x2, 0x1, 0x9, 0xc, 0xa, 0x1, 0x5, 0x4, 0xa, 0x5, 0x1, 0x7, 0x3, 0x1, 0x1, 0x7,
	0x2, 0xe, 0x5, 0x7, 0x2, 0x1, 0x6, 0x4, 0x3, 0xe, 0x6, 0xe, 0xe, 0x5, 0x9, 0xa,
	0xd, 0x5, 0x1, 0x0, 0xd, 0xc, 0x9, 0x0, 0x5, 0x2, 0xc, 0xa, 0xd, 0x2, 0xf, 0x8,
	0xd, 0xc, 0xc, 0x8, 0x8, 0x7, 0x0, 0x6, 0x6, 0x6, 0x0, 0x8, 0x7, 0x3, 0xf, 0x6,
	0x7, 0x9, 0xc, 0xc, 0x6, 0x3, 0xc, 0x6, 0x5, 0x6, 0xf, 0xc, 0x5, 0x1
};

static uint16_t gf_Sx[SNOVA_l * SNOVA_l2] = {
	0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0,
	0x0, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0x1000, 0x111, 0x110, 0x101, 0x100, 0x111,
	0x110, 0x101, 0x100, 0x11, 0x110, 0x101, 0x100, 0x11, 0x10, 0x101, 0x100, 0x11, 0x10, 0x1, 0x100, 0x11,
	0x10, 0x1, 0x1001, 0x1100, 0x1010, 0x1, 0x101, 0x100, 0x1010, 0x101, 0x1, 0x111, 0x11, 0x1, 0x1, 0x111,
	0x10, 0x1110, 0x101, 0x111, 0x10, 0x1, 0x110, 0x100, 0x11, 0x1110, 0x110, 0x1110, 0x1110, 0x101, 0x1001, 0x1010,
	0x1101, 0x101, 0x1, 0x0, 0x1101, 0x1100, 0x1001, 0x0, 0x101, 0x10, 0x1100, 0x1010, 0x1101, 0x10, 0x1111, 0x1000,
	0x1101, 0x1100, 0x1100, 0x1000, 0x1000, 0x111, 0x0, 0x110, 0x110, 0x110, 0x0, 0x1000, 0x111, 0x11, 0x1111, 0x110,
	0x111, 0x1001, 0x1100, 0x1100, 0x110, 0x11, 0x1100, 0x110, 0x101, 0x110, 0x1111, 0x1100, 0x101, 0x1
};

#endif

#if SNOVA_o == 3 && SNOVA_l == 4 && SNOVA_r == 8
#include "abq_snova_i_k.h"
#elif SNOVA_o == 4 && SNOVA_l == 4 && SNOVA_r == 6
#include "abq_snova_i_b.h"
#elif SNOVA_o == 5 && SNOVA_l == 4 && SNOVA_r == 4
#include "abq_snova_i_s.h"
#elif SNOVA_o == 4 && SNOVA_l == 4 && SNOVA_r == 8
#include "abq_snova_iii_k.h"
#elif SNOVA_o == 5 && SNOVA_l == 4 && SNOVA_r == 6
#include "abq_snova_iii_b.h"
#elif SNOVA_o == 6 && SNOVA_l == 4 && SNOVA_r == 5
#include "abq_snova_iii_s.h"
#elif SNOVA_o == 4 && SNOVA_l == 5 && SNOVA_r == 8
#include "abq_snova_v_k.h"
#elif SNOVA_o == 5 && SNOVA_l == 5 && SNOVA_r == 6
#include "abq_snova_v_b.h"
#elif SNOVA_o == 6 && SNOVA_l == 5 && SNOVA_r == 5
#include "abq_snova_v_s.h"
#elif SNOVA_o == 17 && SNOVA_l == 2 && SNOVA_r == 2
#include "abq_snova_i_x.h"
#elif SNOVA_o == 25 && SNOVA_l == 2 && SNOVA_r == 2
#include "abq_snova_iii_x.h"
#elif SNOVA_o == 33 && SNOVA_l == 2 && SNOVA_r == 2
#include "abq_snova_v_x.h"
#else
_Static_assert(0, "Unsupported parameters");
#endif

/**
 * Utilities
 */

static inline uint16_t gf16_expand(const uint8_t a) {
	uint16_t val = a | (a << 3) | (a << 6) | (a << 9);
	return val & 0x1111;
}

static inline uint16_t gf16_compress(const uint16_t a) {
	uint16_t val = (a & 0xf) ^ ((a & 0xf0) >> 3) ^ ((a & 0xf00) >> 6) ^ ((a & 0xf000) >> 9);
	return (val ^ ((val & 0xf0) >> 3) ^ (val >> 4)) & 0xf;
}

/**
 * Constant time function. CT is according to valgrind
 */
static inline uint32_t ct_is_not_zero(uint8_t val) {
	// return (val | (val >> 1) | (val >> 2) | (val >> 3)) & 1;
	return val != 0;
}

/**
 * Constant time GF(16) inverse
 *
 * Use that x^q = x and therefore x^(q-2) = x^-1
 */
static inline uint16_t ct_gf_inverse(uint16_t val) {
	uint16_t fact = gf16_compress(val * gf16_expand(val));
	uint16_t res = fact;

	fact = gf16_compress(fact * gf16_expand(fact));
	res = gf16_compress(res * gf16_expand(fact));

	fact = gf16_compress(fact * gf16_expand(fact));
	res = gf16_compress(res * gf16_expand(fact));

	return gf16_expand(res);
}

static inline int convert_bytes_to_GF(uint8_t *gf_array, const uint8_t *byte_array, size_t num) {
	for (size_t idx = 0; idx < num / 2; idx++) {
		gf_array[2 * idx] = (byte_array[idx] & 0xf) % SNOVA_q;
		gf_array[2 * idx + 1] = (byte_array[idx] >> 4) % SNOVA_q;
	}
	if (num & 1) {
		gf_array[num - 1] = (byte_array[num / 2] & 0xf) % SNOVA_q;
		return byte_array[num / 2] & 0xF0;
	}

	return 0;
}

// Used to compress SIG(sign)
static inline void compress_gf(uint8_t *byte_array, const uint8_t *gf_array, size_t num) {
	for (size_t idx = 0; idx < num / 2; idx++) {
		byte_array[idx] = gf_array[2 * idx] ^ (gf_array[2 * idx + 1] << 4);
	}
	if (num & 1) {
		byte_array[num / 2] = gf_array[num - 1];
	}
}

static inline void hash_combined(uint8_t *hash_out, const uint8_t *m, size_t mlen, const uint8_t *pk_seed,
                                 const uint8_t *salt) {
	shake_t state;
	shake256_init(&state);
#if HASH_PK
	shake_absorb(&state, pk_seed, BYTES_PK_HASH);
#else
	shake_absorb(&state, pk_seed, SEED_LENGTH_PUBLIC);
#endif
	shake_absorb(&state, m, mlen);
	shake_absorb(&state, salt, BYTES_SALT);
	shake_finalize(&state);
	shake_squeeze(hash_out, BYTES_HASH, &state);
}

/**
 * calculate T12 matrix
 */
static inline void gen_a_FqS_sec(uint8_t *Qm, uint8_t *q) {
	for (int i1 = 0; i1 < SNOVA_l2; i1++) {
		uint16_t sum = 0;
		for (int j1 = 0; j1 < SNOVA_l; j1++) {
			sum ^= q[j1] * gf_Sx[j1 * SNOVA_l2 + i1];
		}
		Qm[i1] = gf16_compress(sum);
	}
}

/**
 * Expand T12 matrix and coefficients. Shared by genkey and sign
 */
static inline void expand_T12(uint8_t *T12, const uint8_t *seed) {
	uint8_t T12coef[SNOVA_o * SNOVA_v * SNOVA_l];
	uint8_t sk_data[(SNOVA_o * SNOVA_v * SNOVA_l + 1) / 2];

	shake256(sk_data, (SNOVA_o * SNOVA_v * SNOVA_l + 1) / 2, seed, SEED_LENGTH_PRIVATE);
	convert_bytes_to_GF(T12coef, sk_data, SNOVA_o * SNOVA_v * SNOVA_l);

	for (size_t i1 = 0; i1 < SNOVA_o * SNOVA_v; i1++) {
		gen_a_FqS_sec(&T12[i1 * SNOVA_l2], &T12coef[i1 * SNOVA_l]);
	}
}

/**
 * Optimized version of genkey.
 */
int SNOVA_NAMESPACE(genkeys)(uint8_t *pk, uint8_t *sk, const uint8_t *seed) {
	/**
	 * Gen T12 matrix
	 */
	uint8_t T12[SNOVA_o * SNOVA_v * SNOVA_l2];
	expand_T12(T12, seed + SEED_LENGTH_PUBLIC);

	/**
	 * Gen P22
	 */
	snova_pk_expander_t instance;
	snova_pk_expander_init(&instance, seed, SEED_LENGTH_PUBLIC);

	snova_pk_expander_t p12_instance;
	snova_pk_expander_init(&p12_instance, seed, SEED_LENGTH_PUBLIC);
	snova_pk_expander_goto(&p12_instance, SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2);

	snova_pk_expander_t p21_instance;
	snova_pk_expander_init(&p21_instance, seed, SEED_LENGTH_PUBLIC);
	snova_pk_expander_goto(&p21_instance, SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2);

	int idx = 0;
	uint8_t *pp22 = pk + SEED_LENGTH_PUBLIC;

	for (int mi = 0; mi < SNOVA_m1; mi++) {
		uint16_t P22x[SNOVA_o * SNOVA_o * SNOVA_l2] = {0};

		for (int nj = 0; nj < SNOVA_v; nj++) {
			uint16_t P11i[SNOVA_o * SNOVA_l2] = {0};
			for (int nk = 0; nk < SNOVA_v; nk++) {
				uint8_t P11b[SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P11b, SNOVA_l2, &instance);
				uint16_t P11x[SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_l2; i1++) {
					P11x[i1] = gf16_expand(P11b[i1]);
				}
				for (int ni = 0; ni < SNOVA_o; ni++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								P11i[ni * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
								    T12[(nk * SNOVA_o + ni) * SNOVA_l2 + k1 * SNOVA_l + j1] * P11x[i1 * SNOVA_l + k1];
			}

			for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
				P11i[i1] = gf16_expand(gf16_compress(P11i[i1]));
			}

			for (int ni = 0; ni < SNOVA_o; ni++)
				for (int nk = 0; nk < SNOVA_o; nk++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								P22x[(nk * SNOVA_o + ni) * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
								    T12[(nj * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + k1] *
								    P11i[ni * SNOVA_l2 + k1 * SNOVA_l + j1];
		}

		/**
		 * Calculate F12 and F21 contributions
		 */
		for (int nj = 0; nj < SNOVA_v; nj++)
			for (int ni = 0; ni < SNOVA_o; ni++) {
				uint8_t P12b[SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P12b, SNOVA_l2, &p12_instance);
				uint16_t P12x[SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_l2; i1++) {
					P12x[i1] = gf16_expand(P12b[i1]);
				}
				for (int nk = 0; nk < SNOVA_o; nk++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								P22x[(nk * SNOVA_o + ni) * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
								    T12[(nj * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + k1] *
								    P12x[k1 * SNOVA_l + j1];
			}

		for (int nk = 0; nk < SNOVA_o; nk++)
			for (int nj = 0; nj < SNOVA_v; nj++) {
				uint8_t P21b[SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P21b, SNOVA_l2, &p21_instance);
				uint16_t P21x[SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_l2; i1++) {
					P21x[i1] = gf16_expand(P21b[i1]);
				}
				for (int ni = 0; ni < SNOVA_o; ni++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int k1 = 0; k1 < SNOVA_l; k1++)
							for (int j1 = 0; j1 < SNOVA_l; j1++)
								P22x[(nk * SNOVA_o + ni) * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
								    P21x[i1 * SNOVA_l + k1] * T12[(nj * SNOVA_o + ni) * SNOVA_l2 + k1 * SNOVA_l + j1];
			}

		for (int i1 = 0; i1 < SNOVA_o * SNOVA_o * SNOVA_l2; i1++) {
			if (idx & 1) {
				pp22[idx / 2] ^= (gf16_compress(P22x[i1]) << 4) & 0xf0;
				idx++;
			} else {
				pp22[idx / 2] = gf16_compress(P22x[i1]) & 0xf;
				idx++;
			}
		}
	}

	snova_pk_expander_free(&instance);
	snova_pk_expander_free(&p12_instance);
	snova_pk_expander_free(&p21_instance);

	/**
	 * Output public and secret keys
	 */
	memcpy(pk, seed, SEED_LENGTH_PUBLIC);
	memcpy(sk, seed, SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE);

#if HASH_PK
	shake256(sk + SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE, BYTES_PK_HASH, pk, BYTES_PK);
#endif

	return 0;
}

/**
 * SK expansion.
 */
int SNOVA_NAMESPACE(sk_expand)(expanded_SK *skx, const uint8_t *sk) {
	memcpy(skx, sk, BYTES_SK);

	return 0;
}

/**
 * Optimized version of Sign. Deterministic using the salt provided
 */
int SNOVA_NAMESPACE(sign)(const expanded_SK *skx_arg, uint8_t *sig, const uint8_t *digest, const size_t len_digest,
                          const uint8_t *salt) {
	uint8_t *sk = (uint8_t *)skx_arg;
	uint8_t *s_pk_seed = sk;
	uint8_t *s_sk_seed = sk + SEED_LENGTH_PUBLIC;
#if HASH_PK
	uint8_t *s_pk_hash = sk + SEED_LENGTH_PUBLIC + SEED_LENGTH_PRIVATE;
#endif

	uint8_t T12[SNOVA_o * SNOVA_v * SNOVA_l2];
	expand_T12(T12, s_sk_seed);

	// Calculate message has of size l^2o
	uint8_t hash_in_GF16[GF16_HASH];

	{
		uint8_t sign_hashb[BYTES_HASH];
#if HASH_PK
		hash_combined(sign_hashb, digest, len_digest, s_pk_hash, salt);
#else
		hash_combined(sign_hashb, digest, len_digest, s_pk_seed, salt);
#endif
		convert_bytes_to_GF(hash_in_GF16, sign_hashb, GF16_HASH);
	}

	// Find a solution for T.X
	uint16_t gauss16[SNOVA_o * SNOVA_lr][SNOVA_olr];
	uint8_t signature_in_GF[SNOVA_n * SNOVA_lr] = {0};
	int flag_redo = 1;
	uint8_t num_sign = 0;

	while (1) {
		num_sign++;
		if (num_sign == 255) {
			// Probability of getting here is about q^{-255}
			memset(sig, 0, BYTES_SIGNATURE);
			return -1;
		}
		flag_redo = 0;

		// generate the vinegar value
		uint8_t whipped_sig[SNOVA_l * SNOVA_v * SNOVA_lr] = {0};

		{
			uint8_t vinegar_in_byte[NUM_GEN_SEC_BYTES];
			shake_t v_instance;

			shake256_init(&v_instance);
			shake_absorb(&v_instance, s_sk_seed, SEED_LENGTH_PRIVATE);
			shake_absorb(&v_instance, digest, BYTES_DIGEST);
			shake_absorb(&v_instance, salt, BYTES_SALT);
			shake_absorb(&v_instance, &num_sign, 1);
			shake_finalize(&v_instance);
			shake_squeeze(vinegar_in_byte, NUM_GEN_SEC_BYTES, &v_instance);

			convert_bytes_to_GF(signature_in_GF, vinegar_in_byte, SNOVA_v * SNOVA_lr);

			/**
			 * Whip signature
			 */
			{
				uint16_t whipped_sig1[SNOVA_l * SNOVA_v * SNOVA_lr] = {0};
				for (int ab = 0; ab < SNOVA_l; ++ab)
					for (int ni = 0; ni < SNOVA_v; ++ni)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_r; j1++)
								for (int k1 = 0; k1 < SNOVA_l; k1++)
									whipped_sig1[(i1 * SNOVA_v + ni) * SNOVA_lr + ab * SNOVA_r + j1] ^=
									    gf_Sx[ab * SNOVA_l2 + i1 * SNOVA_l + k1] * signature_in_GF[ni * SNOVA_lr + k1 * SNOVA_r + j1];

				for (int i1 = 0; i1 < SNOVA_v * SNOVA_l * SNOVA_lr; i1++) {
					whipped_sig[i1] = gf16_compress(whipped_sig1[i1]);
				}
			}
		}

		// Calculate Fvv
		memset(gauss16, 0, sizeof(gauss16));

		// Set the last column of gauss matrix
		for (int mi = 0; mi < SNOVA_o; ++mi)
			for (int i1 = 0; i1 < SNOVA_lr; i1++) {
				gauss16[mi * SNOVA_lr + i1][SNOVA_o * SNOVA_lr] ^= hash_in_GF16[mi * SNOVA_lr + i1];
			}

		/**
		 * Public matrix
		 */
		snova_pk_expander_t instance;
		snova_pk_expander_init(&instance, s_pk_seed, SEED_LENGTH_PUBLIC);

		snova_pk_expander_t p12_instance;
		snova_pk_expander_init(&p12_instance, s_pk_seed, SEED_LENGTH_PUBLIC);
		snova_pk_expander_goto(&p12_instance, SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2);

		snova_pk_expander_t p21_instance;
		snova_pk_expander_init(&p21_instance, s_pk_seed, SEED_LENGTH_PUBLIC);
		snova_pk_expander_goto(&p21_instance, SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2);

		/**
		 * ****************************************************************************************************************************************************
		 */

		for (int mi_prime = 0; mi_prime < SNOVA_m1; ++mi_prime) {
			uint16_t sum_t1[SNOVA_l * SNOVA_r * SNOVA_lr] = {0};
			uint16_t whipped_F12[SNOVA_l * SNOVA_o * SNOVA_lr] = {0};
			uint16_t whipped_F21[SNOVA_l * SNOVA_o * SNOVA_lr] = {0};

			for (int ni = 0; ni < SNOVA_v; ++ni) {
				// P11

				uint8_t P11b[SNOVA_v * SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P11b, SNOVA_v * SNOVA_l2, &instance);
				uint16_t P11x[SNOVA_v * SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_v * SNOVA_l2; i1++) {
					P11x[i1] = gf16_expand(P11b[i1]);
				}

				uint16_t sum_t0[SNOVA_v * SNOVA_l * SNOVA_lr] = {0};
				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t0[i1 * SNOVA_lr + b1] ^=
								    P11x[nj * SNOVA_l2 + i1 * SNOVA_l + k1] *
								    whipped_sig[(k1 * SNOVA_v + nj) * SNOVA_lr + b1];

				for (int i1 = 0; i1 < SNOVA_l * SNOVA_lr; ++i1) {
					sum_t0[i1] = gf16_expand(gf16_compress(sum_t0[i1]));
				}

				// Left, transposed whipped_sig
				for (int a1 = 0; a1 < SNOVA_l; ++a1)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t1[a1 * SNOVA_r * SNOVA_lr + i1 * SNOVA_lr + b1] ^=
								    whipped_sig[(k1 * SNOVA_v + ni) * SNOVA_lr + a1 * SNOVA_r + i1] *
								    sum_t0[k1 * SNOVA_lr + b1];

				/**
				 * Start F12
				 */
				uint16_t F12[SNOVA_o * SNOVA_l2] = {0};
				for (int nj = 0; nj < SNOVA_v; nj++)
					for (int nk = 0; nk < SNOVA_o; nk++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_l; j1++)
								for (int k1 = 0; k1 < SNOVA_l; k1++)
									F12[nk * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
									    T12[(nj * SNOVA_o + nk) * SNOVA_l2 + k1 * SNOVA_l + j1] *
									    P11x[nj * SNOVA_l2 + i1 * SNOVA_l + k1];

				for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
					F12[i1] = gf16_expand(gf16_compress(F12[i1]));
				}

				for (int nk = 0; nk < SNOVA_o; nk++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int b1 = 0; b1 < SNOVA_lr; ++b1)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								whipped_F12[(i1 * SNOVA_o + nk) * SNOVA_lr + b1] ^=
								    F12[nk * SNOVA_l2 + k1 * SNOVA_l + i1] *
								    whipped_sig[(k1 * SNOVA_v + ni) * SNOVA_lr + b1];

				/**
				 * Start whipped_F21
				 */
				uint16_t whipP11[SNOVA_l * SNOVA_lr] = {0};

				for (int nj = 0; nj < SNOVA_v; ++nj)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int k1 = 0; k1 < SNOVA_l; k1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								whipP11[k1 * SNOVA_lr + b1] ^=
								    P11x[nj * SNOVA_l2 + k1 * SNOVA_l + j1] *
								    whipped_sig[(j1 * SNOVA_v + nj) * SNOVA_lr + b1];

				for (int i1 = 0; i1 < SNOVA_l * SNOVA_lr; i1++) {
					whipP11[i1] = gf16_expand(gf16_compress(whipP11[i1]));
				}

				for (int k1 = 0; k1 < SNOVA_l; k1++)
					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int nk = 0; nk < SNOVA_o; nk++)
							for (int i1 = 0; i1 < SNOVA_l; i1++)
								for (int j1 = 0; j1 < SNOVA_r; j1++)
									whipped_F21[(b1 * SNOVA_o + nk) * SNOVA_lr + i1 * SNOVA_r + j1] ^=
									    T12[(ni * SNOVA_o + nk) * SNOVA_l2 + i1 * SNOVA_l + k1] * whipP11[k1 * SNOVA_lr + b1 * SNOVA_r + j1];
			}

			for (int i1 = 0; i1 < SNOVA_l * SNOVA_r * SNOVA_lr; i1++) {
				sum_t1[i1] = gf16_compress(sum_t1[i1]);
			}

			for (int nj = 0; nj < SNOVA_v; ++nj) {
				// Right
				uint8_t P12b[SNOVA_o * SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P12b, SNOVA_o * SNOVA_l2, &p12_instance);
				uint16_t P12x[SNOVA_o * SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
					P12x[i1] = gf16_expand(P12b[i1]);
				}
				for (int idx = 0; idx < SNOVA_o; idx++)
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int b1 = 0; b1 < SNOVA_lr; ++b1)
							for (int k1 = 0; k1 < SNOVA_l; k1++)
								whipped_F12[(i1 * SNOVA_o + idx) * SNOVA_lr + b1] ^=
								    P12x[idx * SNOVA_l2 + k1 * SNOVA_l + i1] *
								    whipped_sig[(k1 * SNOVA_v + nj) * SNOVA_lr + b1];
			}

			for (int i1 = 0; i1 < SNOVA_l * SNOVA_o * SNOVA_lr; i1++) {
				whipped_F12[i1] = gf16_compress(whipped_F12[i1]);
			}

			for (int idx = 0; idx < SNOVA_o; idx++) {
				// Right
				uint8_t P21b[SNOVA_v * SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P21b, SNOVA_v * SNOVA_l2, &p21_instance);
				uint16_t P21x[SNOVA_v * SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_v * SNOVA_l2; i1++) {
					P21x[i1] = gf16_expand(P21b[i1]);
				}
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int nj = 0; nj < SNOVA_v; ++nj)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_r; j1++)
								for (int k1 = 0; k1 < SNOVA_l; k1++)
									whipped_F21[(b1 * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r + j1] ^=
									    P21x[nj * SNOVA_l2 + i1 * SNOVA_l + k1] *
									    whipped_sig[(k1 * SNOVA_v + nj) * SNOVA_lr + b1 * SNOVA_r + j1];
			}

			for (int i1 = 0; i1 < SNOVA_l * SNOVA_o * SNOVA_lr; i1++) {
				whipped_F21[i1] = gf16_compress(whipped_F21[i1]);
			}

			uint16_t sum_t1s[SNOVA_l * SNOVA_l * SNOVA_r2] = {0};

			for (int a1 = 0; a1 < SNOVA_l; ++a1)
				for (int b1 = 0; b1 < SNOVA_l; ++b1)
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_r; j1++)
							sum_t1s[a1 * SNOVA_l * SNOVA_r2 + b1 * SNOVA_r2 + i1 * SNOVA_r + j1] =
							    sum_t1[a1 * SNOVA_r * SNOVA_lr + i1 * SNOVA_lr + b1 * SNOVA_r + j1];

			/**
			 * Alpha loop **********************************************************************************************************
			 *
			 * Apply A, B, q1 and q2, aka E matrix
			 */
			for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
				int mi = (mi_prime + SNOVA_m1 * SNOVA_alpha - alpha) % SNOVA_m1;

				if (mi >= SNOVA_o) {
					continue;
				}

				{
					/**
					 * F_vv
					 */
					uint16_t gfm_temp1[SNOVA_r2] = {0};
					uint16_t gfm_temp2[SNOVA_lr] = {0};

					// apply q1 and q2
					for (int a1 = 0; a1 < SNOVA_l; ++a1) {
						uint16_t gfm_temp0[SNOVA_r2] = {0};

						for (int b1 = 0; b1 < SNOVA_l; ++b1)
							for (int i1 = 0; i1 < SNOVA_r2; i1++)
								gfm_temp0[i1] ^= sum_t1s[a1 * SNOVA_l * SNOVA_r2 + b1 * SNOVA_r2 + i1] *
								                 fixedq2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1];

						for (int i1 = 0; i1 < SNOVA_r2; i1++) {
							gfm_temp0[i1] = gf16_compress(gfm_temp0[i1]);
						}

						for (int i1 = 0; i1 < SNOVA_r2; i1++) {
							gfm_temp1[i1] ^= gfm_temp0[i1] * fixedq1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1];
						}
					}

					for (int i1 = 0; i1 < SNOVA_r2; i1++) {
						gfm_temp1[i1] = gf16_compress(gfm_temp1[i1]);
					}

					// A and B
					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_r; k1++) {
								gfm_temp2[i1 * SNOVA_l + j1] ^=
								    gfm_temp1[i1 * SNOVA_r + k1] * fixedBm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + k1 * SNOVA_l + j1];
							}

					for (int i1 = 0; i1 < SNOVA_lr; i1++) {
						gfm_temp2[i1] = gf16_compress(gfm_temp2[i1]);
					}

					for (int i1 = 0; i1 < SNOVA_r; i1++)
						for (int j1 = 0; j1 < SNOVA_l; j1++)
							for (int k1 = 0; k1 < SNOVA_r; k1++) {
								gauss16[mi * SNOVA_lr + i1 * SNOVA_l + j1][SNOVA_o * SNOVA_lr] ^=
								    fixedAm[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + i1 * SNOVA_r + k1] * gfm_temp2[k1 * SNOVA_l + j1];
							}
				}
			}

			// compute the coefficients of Xo and put into gauss matrix and compute
			// the coefficients of Xo^t and add into gauss matrix

			for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
				int mi = (mi_prime + SNOVA_m1 * SNOVA_alpha - alpha) % SNOVA_m1;

				if (mi >= SNOVA_o) {
					continue;
				}

				{
					uint16_t gfm_temp0[SNOVA_olr] = {0};
					uint16_t gfm_temp1[SNOVA_olr] = {0};
					uint16_t gfm_temp2[SNOVA_olr] = {0};

					// Transpose
					for (int i1 = 0; i1 < SNOVA_l; i1++)
						for (int idx = 0; idx < SNOVA_o; idx++)
							for (int b1 = 0; b1 < SNOVA_l; ++b1)
								for (int j1 = 0; j1 < SNOVA_r; j1++)
									gfm_temp0[idx * SNOVA_lr + i1 * SNOVA_r + j1] ^=
									    whipped_F12[(i1 * SNOVA_o + idx) * SNOVA_lr + b1 * SNOVA_r + j1] *
									    fixedq1[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1];

					for (int i1 = 0; i1 < SNOVA_olr; i1++) {
						gfm_temp0[i1] = gf16_compress(gfm_temp0[i1]);
					}

					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int j1 = 0; j1 < SNOVA_l; j1++)
								for (int k1 = 0; k1 < SNOVA_r; k1++)
									gfm_temp1[idx * SNOVA_lr + i1 * SNOVA_l + j1] ^=
									    fixedAm[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + i1 * SNOVA_r + k1] *
									    gfm_temp0[idx * SNOVA_lr + j1 * SNOVA_r + k1];

					for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
						gfm_temp1[i1] = gf16_compress(gfm_temp1[i1]);
					}

					uint16_t Q2[SNOVA_l2];
					for (int i1 = 0; i1 < SNOVA_l2; i1++) {
						uint16_t sum = 0;
						for (int j1 = 0; j1 < SNOVA_l; j1++) {
							sum ^= fixedq2[(mi * SNOVA_alpha + alpha) * SNOVA_l + j1] * gf_S[j1 * SNOVA_l2 + i1];
						}
						Q2[i1] = gf16_expand(gf16_compress(sum));
					}

					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int j1 = 0; j1 < SNOVA_l; j1++)
								for (int k1 = 0; k1 < SNOVA_l; k1++) {
									gfm_temp2[idx * SNOVA_lr + i1 * SNOVA_l + j1] ^= gfm_temp1[idx * SNOVA_lr + i1 * SNOVA_l + k1] * Q2[k1 * SNOVA_l + j1];
								}

					for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
						gfm_temp2[i1] = gf16_compress(gfm_temp2[i1]);
					}

					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int ti1 = 0; ti1 < SNOVA_r; ti1++)
							for (int ti2 = 0; ti2 < SNOVA_l; ti2++)
								for (int tj2 = 0; tj2 < SNOVA_r; tj2++)
									for (int tj1 = 0; tj1 < SNOVA_l; tj1++)
										gauss16[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx * SNOVA_lr + tj1 * SNOVA_r + tj2] ^=
										    gfm_temp2[idx * SNOVA_lr + ti1 * SNOVA_l + tj1] * fixedBm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + tj2 * SNOVA_l + ti2];
				}

				/**
				 * End of F12
				 * *****************************************
				 * Start F21
				 */

				{
					uint16_t gfm_temp0[SNOVA_o * SNOVA_lr] = {0};
					uint16_t gfm_temp1[SNOVA_o * SNOVA_l2] = {0};
					uint16_t gfm_temp2[SNOVA_olr] = {0};

					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int b1 = 0; b1 < SNOVA_l; ++b1)
							for (int i1 = 0; i1 < SNOVA_l; i1++)
								for (int j1 = 0; j1 < SNOVA_r; j1++)
									gfm_temp0[idx * SNOVA_lr + i1 * SNOVA_r + j1] ^=
									    whipped_F21[(b1 * SNOVA_o + idx) * SNOVA_lr + i1 * SNOVA_r + j1] *
									    fixedq2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1];

					for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
						gfm_temp0[i1] = gf16_compress(gfm_temp0[i1]);
					}

					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_l; j1++)
								for (int k1 = 0; k1 < SNOVA_r; k1++) {
									gfm_temp1[idx * SNOVA_l2 + i1 * SNOVA_l + j1] ^=
									    gfm_temp0[idx * SNOVA_lr + i1 * SNOVA_r + k1] *
									    fixedBm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + k1 * SNOVA_l + j1];
								}

					for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
						gfm_temp1[i1] = gf16_compress(gfm_temp1[i1]);
					}

					uint16_t Q1[SNOVA_l2];
					for (int i1 = 0; i1 < SNOVA_l2; i1++) {
						uint16_t sum = 0;
						for (int j1 = 0; j1 < SNOVA_l; j1++) {
							sum ^= fixedq1[(mi * SNOVA_alpha + alpha) * SNOVA_l + j1] * gf_S[j1 * SNOVA_l2 + i1];
						}
						Q1[i1] = gf16_expand(gf16_compress(sum));
					}

					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int j1 = 0; j1 < SNOVA_l; j1++)
								for (int k1 = 0; k1 < SNOVA_l; k1++) {
									gfm_temp2[idx * SNOVA_l2 + i1 * SNOVA_l + j1] ^= Q1[i1 * SNOVA_l + k1] * gfm_temp1[idx * SNOVA_l2 + k1 * SNOVA_l + j1];
								}

					for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
						gfm_temp2[i1] = gf16_compress(gfm_temp2[i1]);
					}

					for (int idx = 0; idx < SNOVA_o; idx++)
						for (int ti1 = 0; ti1 < SNOVA_r; ti1++)
							for (int ti2 = 0; ti2 < SNOVA_l; ti2++)
								for (int tj2 = 0; tj2 < SNOVA_r; tj2++)
									for (int tj1 = 0; tj1 < SNOVA_l; tj1++)
										gauss16[mi * SNOVA_lr + ti1 * SNOVA_l + ti2][idx * SNOVA_lr + tj1 * SNOVA_r + tj2] ^=
										    gfm_temp2[idx * SNOVA_l2 + tj1 * SNOVA_l + ti2] * fixedAm[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + ti1 * SNOVA_r + tj2];
				}
			}
		}

		snova_pk_expander_free(&instance);
		snova_pk_expander_free(&p12_instance);
		snova_pk_expander_free(&p21_instance);

		/**
		 * Gaussian elimination
		 */

		for (int ti = 0; ti < SNOVA_o * SNOVA_lr; ti++)
			for (int tj = 0; tj < SNOVA_o * SNOVA_lr; tj++) {
				gauss16[ti][tj] = gf16_compress(gauss16[ti][tj]);
			}

		// Gaussian elimination in constant time
		for (int i = 0; i < SNOVA_o * SNOVA_lr; ++i) {
			for (int j = i + 1; j < SNOVA_o * SNOVA_lr; ++j) {
				int16_t mask = ct_is_not_zero(gauss16[i][i]) - 1;
				for (int k = 0; k < SNOVA_olr; ++k) {
					gauss16[i][k] ^= mask & gauss16[j][k];
				}
			}

			for (int k = 0; k < SNOVA_olr; ++k) {
				gauss16[i][k] = gf16_compress(gauss16[i][k]);
			}

			flag_redo |= 1 - ct_is_not_zero(gauss16[i][i]);

			uint16_t t_GF16 = ct_gf_inverse(gauss16[i][i]);
			for (int k = 0; k < SNOVA_olr; ++k) {
				gauss16[i][k] = gauss16[i][k] * t_GF16;
			}

			for (int k = 0; k < SNOVA_olr; ++k) {
				gauss16[i][k] = gf16_compress(gauss16[i][k]);
			}

			for (int j = i + 1; j < SNOVA_o * SNOVA_lr; ++j) {
				uint16_t gji = gf16_expand(gauss16[j][i]);
				for (int k = 0; k < SNOVA_olr; ++k) {
					gauss16[j][k] ^= gauss16[i][k] * gji;
				}
			}

			for (int j = i + 1; j < SNOVA_o * SNOVA_lr; ++j) {
				gauss16[j][i + 1] = gf16_compress(gauss16[j][i + 1]);
			}
		}

		if (flag_redo) {
			continue;
		}

		// Last step of Gaussian elimination
		uint8_t solution[SNOVA_olr] = {0};
		uint16_t solution16[SNOVA_o * SNOVA_lr] = {0};

		for (int i = SNOVA_o * SNOVA_lr - 1; i >= 0; --i) {
			uint16_t sum = 0;
			for (int k = i + 1; k < SNOVA_o * SNOVA_lr; ++k) {
				sum ^= gauss16[i][k] * solution16[k];
			}
			solution16[i] = gf16_expand(gf16_compress(sum ^ gauss16[i][SNOVA_o * SNOVA_lr]));
		}
		for (int i = 0; i < SNOVA_o * SNOVA_lr; i++) {
			solution[i] = gf16_compress(solution16[i]);
		}

		memcpy(signature_in_GF + SNOVA_v * SNOVA_lr, solution, SNOVA_o * SNOVA_lr);

		// Establish signature using T12
		for (int index = 0; index < SNOVA_v; ++index)
			for (int mi = 0; mi < SNOVA_o; ++mi)
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_r; j1++) {
						uint16_t sum = 0;
						for (int k1 = 0; k1 < SNOVA_l; k1++) {
							sum ^= gf16_expand(T12[(index * SNOVA_o + mi) * SNOVA_l2 + i1 * SNOVA_l + k1]) *
							       solution[mi * SNOVA_lr + k1 * SNOVA_r + j1];
						}
						signature_in_GF[index * SNOVA_lr + i1 * SNOVA_r + j1] ^= gf16_compress(sum);
					}

		memcpy(signature_in_GF + SNOVA_v * SNOVA_lr, solution, SNOVA_o * SNOVA_lr);

		break;
	}

	compress_gf(sig, signature_in_GF, SNOVA_n * SNOVA_lr);
	memcpy(sig + BYTES_SIGNATURE - BYTES_SALT, salt, BYTES_SALT);

	return 0;
}

/**
 * PK expansion.
 */
int SNOVA_NAMESPACE(pk_expand)(expanded_PK *pkx, const uint8_t *pk) {
	memcpy(pkx, pk, BYTES_PK);

	return 0;
}

/**
 * Optimized version of verify.
 */
int SNOVA_NAMESPACE(verify)(const expanded_PK *pkx, const uint8_t *sig, const uint8_t *digest, const size_t len_digest) {
	uint8_t *pk = (uint8_t *)pkx;
	uint8_t *pk_seed = pk;

	// Prepare
	uint16_t hash_in_GF[SNOVA_o * SNOVA_lr] = {0};
	{
		uint8_t *pp22 = pk + SEED_LENGTH_PUBLIC;
		int idx_p22 = 0;

		/**
		 * Whip signature
		 */
		uint8_t whipped_sig[SNOVA_l * SNOVA_n * SNOVA_lr] = {0};
		for (int idx = 0; idx < SNOVA_n; ++idx) {
			uint16_t whipped_sig1[SNOVA_l * SNOVA_lr] = {0};

			for (int ab = 0; ab < SNOVA_l; ++ab) {
				for (int i1 = 0; i1 < SNOVA_l; i1++)
					for (int j1 = 0; j1 < SNOVA_r; j1++)
						for (int k1 = 0; k1 < SNOVA_l; k1++) {
							if ((idx * SNOVA_lr + k1 * SNOVA_r + j1) & 1) {
								whipped_sig1[i1 * SNOVA_lr + ab * SNOVA_r + j1] ^=
								    gf_Sx[ab * SNOVA_l2 + i1 * SNOVA_l + k1] * (sig[(idx * SNOVA_lr + k1 * SNOVA_r + j1) / 2] >> 4);
							} else {
								whipped_sig1[i1 * SNOVA_lr + ab * SNOVA_r + j1] ^=
								    gf_Sx[ab * SNOVA_l2 + i1 * SNOVA_l + k1] * (sig[(idx * SNOVA_lr + k1 * SNOVA_r + j1) / 2] & 0xf);
							}
						}
			}

			for (int i1 = 0; i1 < SNOVA_l * SNOVA_lr; i1++) {
				whipped_sig[idx * SNOVA_l * SNOVA_lr + i1] = gf16_compress(whipped_sig1[i1]);
			}
		}

		/**
		 * stream P matrix
		 */
		snova_pk_expander_t instance;
		snova_pk_expander_init(&instance, pk_seed, SEED_LENGTH_PUBLIC);

		snova_pk_expander_t p12_instance;
		snova_pk_expander_init(&p12_instance, pk_seed, SEED_LENGTH_PUBLIC);
		snova_pk_expander_goto(&p12_instance, SNOVA_m1 * SNOVA_v * SNOVA_v * SNOVA_l2);

		snova_pk_expander_t p21_instance;
		snova_pk_expander_init(&p21_instance, pk_seed, SEED_LENGTH_PUBLIC);
		snova_pk_expander_goto(&p21_instance, SNOVA_m1 * SNOVA_v * SNOVA_n * SNOVA_l2);

		for (int mi_prime = 0; mi_prime < SNOVA_m1; ++mi_prime) {
			uint16_t sum_t1[SNOVA_l * SNOVA_r * SNOVA_lr] = {0};

			for (int ni = 0; ni < SNOVA_o; ++ni) {
				uint16_t P22[SNOVA_o * SNOVA_l2];

				for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
					if (idx_p22 & 1) {
						P22[i1] = gf16_expand(pp22[idx_p22 / 2] >> 4);
					} else {
						P22[i1] = gf16_expand(pp22[idx_p22 / 2] & 0xf);
					}
					idx_p22++;
				}

				// Right
				uint16_t sum_t0[SNOVA_l * SNOVA_lr] = {0};
				for (int nj = 0; nj < SNOVA_o; ++nj)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t0[i1 * SNOVA_lr + b1] ^=
								    P22[nj * SNOVA_l2 + i1 * SNOVA_l + k1] *
								    whipped_sig[(nj + SNOVA_v) * SNOVA_l * SNOVA_lr + k1 * SNOVA_lr + b1];

				for (int i1 = 0; i1 < SNOVA_l * SNOVA_lr; ++i1) {
					sum_t0[i1] = gf16_expand(gf16_compress(sum_t0[i1]));
				}

				// Left, transposed whipped_sig
				for (int a1 = 0; a1 < SNOVA_l; ++a1)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t1[a1 * SNOVA_r * SNOVA_lr + i1 * SNOVA_lr + b1] ^=
								    whipped_sig[(ni + SNOVA_v) * SNOVA_l * SNOVA_lr + k1 * SNOVA_lr + a1 * SNOVA_r + i1] *
								    sum_t0[k1 * SNOVA_lr + b1];
			}

			// P11
			for (int ni = 0; ni < SNOVA_v; ++ni) {
				// Right
				uint8_t P11b[SNOVA_v * SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P11b, SNOVA_v * SNOVA_l2, &instance);
				uint16_t P11x[SNOVA_v * SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_v * SNOVA_l2; i1++) {
					P11x[i1] = gf16_expand(P11b[i1]);
				}
				uint16_t sum_t0[SNOVA_l * SNOVA_lr] = {0};
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t0[i1 * SNOVA_lr + b1] ^=
								    P11x[nj * SNOVA_l2 + i1 * SNOVA_l + k1] *
								    whipped_sig[nj * SNOVA_l * SNOVA_lr + k1 * SNOVA_lr + b1];
				}

				for (int i1 = 0; i1 < SNOVA_l * SNOVA_lr; ++i1) {
					sum_t0[i1] = gf16_expand(gf16_compress(sum_t0[i1]));
				}

				// Left, transposed whipped_sig
				for (int a1 = 0; a1 < SNOVA_l; ++a1)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t1[a1 * SNOVA_r * SNOVA_lr + i1 * SNOVA_lr + b1] ^=
								    whipped_sig[ni * SNOVA_l * SNOVA_lr + k1 * SNOVA_lr + a1 * SNOVA_r + i1] *
								    sum_t0[k1 * SNOVA_lr + b1];
			}

			// P12
			for (int ni = 0; ni < SNOVA_v; ++ni) {
				// Right
				uint8_t P12b[SNOVA_o * SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P12b, SNOVA_o * SNOVA_l2, &p12_instance);
				uint16_t P12x[SNOVA_o * SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_o * SNOVA_l2; i1++) {
					P12x[i1] = gf16_expand(P12b[i1]);
				}
				uint16_t sum_t0[SNOVA_l * SNOVA_lr] = {0};
				for (int nj = 0; nj < SNOVA_o; ++nj) {
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t0[i1 * SNOVA_lr + b1] ^=
								    P12x[nj * SNOVA_l2 + i1 * SNOVA_l + k1] *
								    whipped_sig[(nj + SNOVA_v) * SNOVA_l * SNOVA_lr + k1 * SNOVA_lr + b1];
				}

				for (int i1 = 0; i1 < SNOVA_l * SNOVA_lr; ++i1) {
					sum_t0[i1] = gf16_expand(gf16_compress(sum_t0[i1]));
				}

				// Left, transposed whipped_sig
				for (int a1 = 0; a1 < SNOVA_l; ++a1)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t1[a1 * SNOVA_r * SNOVA_lr + i1 * SNOVA_lr + b1] ^=
								    whipped_sig[ni * SNOVA_l * SNOVA_lr + k1 * SNOVA_lr + a1 * SNOVA_r + i1] *
								    sum_t0[k1 * SNOVA_lr + b1];
			}

			// P21
			for (int ni = 0; ni < SNOVA_o; ++ni) {
				// Right
				uint8_t P21b[SNOVA_v * SNOVA_l2] = {0};
				snova_pk_expander_squeeze(P21b, SNOVA_v * SNOVA_l2, &p21_instance);
				uint16_t P21x[SNOVA_v * SNOVA_l2];
				for (int i1 = 0; i1 < SNOVA_v * SNOVA_l2; i1++) {
					P21x[i1] = gf16_expand(P21b[i1]);
				}
				uint16_t sum_t0[SNOVA_l * SNOVA_lr] = {0};
				for (int nj = 0; nj < SNOVA_v; ++nj) {
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_l; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t0[i1 * SNOVA_lr + b1] ^=
								    P21x[nj * SNOVA_l2 + i1 * SNOVA_l + k1] *
								    whipped_sig[nj * SNOVA_l * SNOVA_lr + k1 * SNOVA_lr + b1];
				}

				for (int i1 = 0; i1 < SNOVA_l * SNOVA_lr; ++i1) {
					sum_t0[i1] = gf16_expand(gf16_compress(sum_t0[i1]));
				}

				// Left, transposed whipped_sig
				for (int a1 = 0; a1 < SNOVA_l; ++a1)
					for (int k1 = 0; k1 < SNOVA_l; k1++)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int b1 = 0; b1 < SNOVA_lr; ++b1)
								sum_t1[a1 * SNOVA_r * SNOVA_lr + i1 * SNOVA_lr + b1] ^=
								    whipped_sig[(ni + SNOVA_v) * SNOVA_l * SNOVA_lr + k1 * SNOVA_lr + a1 * SNOVA_r + i1] *
								    sum_t0[k1 * SNOVA_lr + b1];
			}

			for (int i1 = 0; i1 < SNOVA_l * SNOVA_r * SNOVA_lr; i1++) {
				sum_t1[i1] = gf16_compress(sum_t1[i1]);
			}

			/**
			 * Apply A, B, q1 and q2, aka E matrix
			 */
			for (int alpha = 0; alpha < SNOVA_alpha; ++alpha) {
				int mi = (mi_prime + SNOVA_m1 * SNOVA_alpha - alpha) % SNOVA_m1;

				if (mi >= SNOVA_o) {
					continue;
				}

				uint16_t gfm_temp1[SNOVA_r2] = {0};
				uint16_t gfm_temp2[SNOVA_lr] = {0};

				// apply q1 and q2
				for (int a1 = 0; a1 < SNOVA_l; ++a1) {
					uint16_t gfm_temp0[SNOVA_r2] = {0};

					for (int b1 = 0; b1 < SNOVA_l; ++b1)
						for (int i1 = 0; i1 < SNOVA_r; i1++)
							for (int j1 = 0; j1 < SNOVA_r; j1++)
								gfm_temp0[i1 * SNOVA_r + j1] ^=
								    fixedq2[(mi * SNOVA_alpha + alpha) * SNOVA_l + b1] *
								    sum_t1[a1 * SNOVA_r * SNOVA_lr + i1 * SNOVA_lr + b1 * SNOVA_r + j1];

					for (int i1 = 0; i1 < SNOVA_r2; i1++) {
						gfm_temp0[i1] = gf16_compress(gfm_temp0[i1]);
					}

					for (int i1 = 0; i1 < SNOVA_r2; i1++) {
						gfm_temp1[i1] ^= gfm_temp0[i1] * fixedq1[(mi * SNOVA_alpha + alpha) * SNOVA_l + a1];
					}
				}

				for (int i1 = 0; i1 < SNOVA_r2; i1++) {
					gfm_temp1[i1] = gf16_compress(gfm_temp1[i1]);
				}

				// A and B
				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int k1 = 0; k1 < SNOVA_r; k1++)
							gfm_temp2[i1 * SNOVA_l + j1] ^=
							    gfm_temp1[i1 * SNOVA_r + k1] * fixedBm[(mi * SNOVA_alpha + alpha) * SNOVA_lr + k1 * SNOVA_l + j1];

				for (int i1 = 0; i1 < SNOVA_lr; i1++) {
					gfm_temp2[i1] = gf16_compress(gfm_temp2[i1]);
				}

				for (int i1 = 0; i1 < SNOVA_r; i1++)
					for (int j1 = 0; j1 < SNOVA_l; j1++)
						for (int k1 = 0; k1 < SNOVA_r; k1++)
							hash_in_GF[mi * SNOVA_lr + i1 * SNOVA_l + j1] ^=
							    fixedAm[(mi * SNOVA_alpha + alpha) * SNOVA_r2 + i1 * SNOVA_r + k1] * gfm_temp2[k1 * SNOVA_l + j1];
			}
		}

		snova_pk_expander_free(&instance);
		snova_pk_expander_free(&p12_instance);
		snova_pk_expander_free(&p21_instance);

		for (int i1 = 0; i1 < SNOVA_o * SNOVA_lr; i1++) {
			hash_in_GF[i1] = gf16_compress(hash_in_GF[i1]);
		}
	}

	/**
	 * Check hashes
	 */
	uint8_t signed_gf[GF16_HASH] = {0};
	const uint8_t *salt = sig + BYTES_SIGNATURE - BYTES_SALT;
	uint8_t signed_bytes[BYTES_HASH];
#if HASH_PK
	uint8_t pk_hash[BYTES_PK_HASH];
	shake256(pk_hash, BYTES_PK_HASH, pk, BYTES_PK);
	hash_combined(signed_bytes, digest, len_digest, pk_hash, salt);
#else
	hash_combined(signed_bytes, digest, len_digest, pk_seed, salt);
#endif
	convert_bytes_to_GF(signed_gf, signed_bytes, GF16_HASH);

	for (int i = 0; i < GF16_HASH; ++i) {
		if (hash_in_GF[i] != signed_gf[i]) {
			return -1;
		}
	}

	return 0;
}
