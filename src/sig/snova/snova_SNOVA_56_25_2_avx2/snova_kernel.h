#ifndef SNOVA_KNL_H
#define SNOVA_KNL_H

#include "gf16_matrix_inline.h"
#include "snova.h"
#include "snova_common.h"

static gf16m_t S[l_SNOVA] = {0};
static uint32_t xS[l_SNOVA][lsq_SNOVA] = {0};
static int S_is_init = 0;
// GF[x]/(x^4+x+1) reduction
static inline uint32_t gf16_reduce(uint32_t idx) {
	uint32_t res, upper;

	res = idx & 0x49249249; // Octal 0o11111111111
	upper = idx >> 12;
	res = res ^ upper ^ (upper << 3);
	upper = res >> 12;
	res = res ^ upper ^ (upper << 3);
	upper = res >> 12;
	res = res ^ upper ^ (upper << 3);

	return res & 0x249;
}

// Conversion 4 bit -> 32 bit representation
static inline uint32_t gf16_from_nibble(uint8_t idx) {
	uint32_t middle = idx | idx << 4;
	return (middle & 0x41) | ((middle << 2) & 0x208);
}

// Conversion 32 bit -> 4 bit representation
static inline uint8_t gf16_to_nibble(uint32_t idx) {
	uint32_t res = gf16_reduce(idx);
	res = res | (res >> 4);
	return (res & 0x5) | ((res >> 2) & 0xa);
}

// Conversion 32 bit -> 4 bit representation
static inline uint8_t xgf16_to_nibble(uint32_t res) {
	res = res | (res >> 4);
	return (res & 0x5) | ((res >> 2) & 0xa);
}

// Constant time GF16 inverse
// x^16 - x = 0 implies x^14 = x^-1
static inline uint32_t gf16_inv(uint32_t val) {
	val = gf16_reduce(val);
	uint32_t res2 = gf16_reduce(val * val);
	uint32_t res4 = gf16_reduce(res2 * res2);
	uint32_t res8 = gf16_reduce(res4 * res4);

	return gf16_reduce(res2 * ((res4 * res8) & 0x49249249));
}


/**
 * Generate elements of F16[S]
 */
static void gen_S_array(void) {
	if (S_is_init) {
		return;
	}

	S_is_init = 1;
	be_aI(S[0], 1);
	be_the_S(S[1]);
	for (int index = 2; index < l_SNOVA; index++) {
		gf16m_mul(S[index - 1], S[1], S[index]);
	}

	for (int index = 0; index < l_SNOVA; index++)
		for (int ij = 0; ij < lsq_SNOVA; ij++) {
			xS[index][ij] = gf16_from_nibble(S[index][ij]);
		}
}

/**
 * pk expand from seed
 *
 * Using AES-CTR encryption as a hash function
 * AES ciphertext padded with zeros.
 * The iv is also padded with zeros.
 * Using input value as the AES key.
 * The ciphertext obtained from AES encryption serves as the output of the hash
 * function.
 * @param pt_public_key_seed - Pointer to the hash input. (Fixed length of 16)
 * @param out_pk - Pointer to the hash output. (Fixed length of
 * bytes_prng_public)
 */
#if PK_EXPAND_SHAKE
static void pk_expand(const uint8_t *pt_public_key_seed, uint8_t *out_pk) {
	uint64_t pk_bytes[(bytes_prng_public + 7) / 8];
	snova_shake(pt_public_key_seed, 16, pk_bytes, 8 * ((bytes_prng_public + 7) / 8));
	memcpy(out_pk, pk_bytes, bytes_prng_public);
}
#else
int AES_128_CTR(unsigned char *output, size_t outputByteLen, const unsigned char *input, size_t inputByteLen);
static void pk_expand(const uint8_t *pt_public_key_seed, uint8_t *out_pk) {
	AES_128_CTR(out_pk, bytes_prng_public, pt_public_key_seed, 16);
}
#endif

/**
 * @param c - output
 * @param pt_matrix - input
 */
static void gen_a_FqS(gf16_t *c, gf16m_t pt_matrix) {
	gf16m_t temp;
	be_aI(pt_matrix, c[0]);
	for (int i = 1; i < rank - 1; ++i) {
		gf16m_scale(S[i], c[i], temp);
		gf16m_add(pt_matrix, temp, pt_matrix);
	}
	gf16m_scale(S[rank - 1], (c[rank - 1] != 0) ? c[rank - 1] : 16 - (c[0] + (c[0] == 0)), temp);
	gf16m_add(pt_matrix, temp, pt_matrix);
	SNOVA_CLEAR(temp);
}

// Constant time version of gen_a_FqS
static void gen_a_FqS_ct(gf16_t *c, gf16m_t pt_matrix) {
	uint32_t xTemp[lsq_SNOVA] = {0};
	uint32_t cX = gf16_from_nibble(c[0]);

	for (int ij = 0; ij < l_SNOVA; ij++) {
		xTemp[ij * l_SNOVA + ij] = cX;
	}

	for (int i1 = 1; i1 < l_SNOVA - 1; i1++) {
		cX = gf16_from_nibble(c[i1]);
		for (int ij = 0; ij < lsq_SNOVA; ij++) {
			xTemp[ij] ^= cX * xS[i1][ij];
		}
	}

	uint8_t zero = ct_gf16_is_not_zero(c[rank - 1]);
	uint8_t val = zero * c[rank - 1] + (1 - zero) * (15 + ct_gf16_is_not_zero(c[0]) - c[0]);

	cX = gf16_from_nibble(val);
	for (int ij = 0; ij < lsq_SNOVA; ij++) {
		xTemp[ij] ^= cX * xS[l_SNOVA - 1][ij];
	}

	for (int ij = 0; ij < lsq_SNOVA; ij++) {
		pt_matrix[ij] = gf16_to_nibble(xTemp[ij]);
	}

	SNOVA_CLEAR(xTemp);
}

/**
 * Generate the linear map T12
 * @param T12 - output
 * @param seed - input
 */
static void gen_seeds_and_T12(T12_t T12, const uint8_t *seed) {
	gf16_t *pt_array;
	uint8_t prng_output_private[bytes_prng_private];
	gf16_t GF16_prng_output_private[GF16s_prng_private];

	shake256(seed, seed_length_private, prng_output_private, bytes_prng_private);
	convert_bytes_to_GF16s(prng_output_private, GF16_prng_output_private, GF16s_prng_private);

	pt_array = GF16_prng_output_private;
	for (int j = 0; j < v_SNOVA; ++j) {
		for (int k = 0; k < o_SNOVA; ++k) {
			gen_a_FqS_ct(pt_array, T12[j][k]);
			pt_array += rank;
		}
	}

	// Clear Secret!
	SNOVA_CLEAR(prng_output_private);
	SNOVA_CLEAR(GF16_prng_output_private);
}

/**
 * Generate the random part of public key
 * @param map - P11 P12 P21 Aalpha Balpha Qalpha1 Qalpha2
 * @param pt_public_key_seed - input
 */

static void gen_A_B_Q_P(map_group1 *map, const uint8_t *pt_public_key_seed) {
	uint8_t prng_output_public[bytes_prng_public];
	uint8_t Q_temp[(sizeof(Qalpha1_t) + sizeof(Qalpha2_t)) / l_SNOVA];
	// ----- pt temp -----
	pk_expand(pt_public_key_seed, prng_output_public);
#if FIXED_ABQ
	convert_bytes_to_GF16s(prng_output_public, (uint8_t *)map, GF16s_prng_public - sizeof(Q_temp));
	memcpy(map->Aalpha, fixed_abq, 4 * m_SNOVA * alpha_SNOVA * lsq_SNOVA);
#else
	convert_bytes_to_GF16s(prng_output_public, (uint8_t *)map, GF16s_prng_public - sizeof(Q_temp));
	convert_bytes_to_GF16s(prng_output_public + sizeof(prng_output_public) - ((sizeof(Q_temp) + 1) >> 1), Q_temp, sizeof(Q_temp));

	for (int pi = 0; pi < m_SNOVA; ++pi) {
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			be_invertible_by_add_aS(map->Aalpha[pi][alpha]);
		}
	}
	for (int pi = 0; pi < m_SNOVA; ++pi) {
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			be_invertible_by_add_aS(map->Balpha[pi][alpha]);
		}
	}

	gf16_t *pt_array = Q_temp;
	for (int pi = 0; pi < m_SNOVA; ++pi) {
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			gen_a_FqS(pt_array, map->Qalpha1[pi][alpha]);
			pt_array += l_SNOVA;
		}
	}
	for (int pi = 0; pi < m_SNOVA; ++pi) {
		for (int alpha = 0; alpha < alpha_SNOVA; ++alpha) {
			gen_a_FqS(pt_array, map->Qalpha2[pi][alpha]);
			pt_array += l_SNOVA;
		}
	}
#endif
}

/**
 * P22 byte to GF16
 * @param P22_gf16s - output
 * @param P22_bytes - input
 */
static void input_P22(uint8_t *P22_gf16s, const uint8_t *P22_bytes) {
	convert_bytes_to_GF16s(P22_bytes, P22_gf16s, m_SNOVA * o_SNOVA * o_SNOVA * lsq_SNOVA);
}

/**
 * Pack expanded private key. esk = (key_elems, pt_private_key_seed).
 * @param esk - pointer to output expanded private key.
 * @param key_elems - pointer to input snova key elements.
 * @param pt_private_key_seed - pointer to input private key seed.
 */
static void sk_pack(uint8_t *esk, snova_key_elems *key_elems, const uint8_t *pt_private_key_seed) {
	uint8_t *sk_gf16_ptr = (uint8_t *)(key_elems->map1.Aalpha);
	convert_GF16s_to_bytes_merger_in_half(esk, sk_gf16_ptr, (bytes_sk - (seed_length_public + seed_length_private)) * 2);
	memcpy(esk + (bytes_sk - (seed_length_public + seed_length_private)), key_elems->pk.pt_public_key_seed, seed_length_public);
	memcpy(esk + (bytes_sk - seed_length_private), pt_private_key_seed, seed_length_private);
}

/**
 * Unpack expanded secret key. skupk = (esk).
 * @param skupk - pointer to output private key (unpack).
 * @param esk - pointer to input expanded private key.
 */
static void sk_unpack(sk_gf16 *skupk, const uint8_t *esk) {
	convert_bytes_to_GF16s_cut_in_half(esk, (uint8_t *)skupk, (bytes_sk - (seed_length_public + seed_length_private)) * 2);
	memcpy(skupk->pt_public_key_seed, esk + (bytes_sk - (seed_length_public + seed_length_private)),
	       seed_length_public + seed_length_private);
}

/**
 * Pack public key. pk = (key_elems).
 */
static void pk_pack(uint8_t *pk, snova_key_elems *key_elems) {
	memcpy(pk, &key_elems->pk, bytes_pk);
}

/**
 * Unpack expend public key.
 */
static void pkx_unpack(public_key_expand *pkx_unpck, public_key_expand_pack *pkx_pck) {
	convert_bytes_to_GF16s((uint8_t *)(pkx_pck) + seed_length_public, (uint8_t *)(pkx_unpck) + seed_length_public, sizeof(P22_t) + sizeof(map_group1));
	memcpy(pkx_unpck->pt_public_key_seed, pkx_pck->pt_public_key_seed, seed_length_public);
}

/**
 * Pack expend public key.
 */
static void pkx_pack(public_key_expand_pack *pkx_pck, public_key_expand *pkx_unpck) {
	convert_GF16s_to_bytes((uint8_t *)(pkx_pck) + seed_length_public, (uint8_t *)(pkx_unpck) + seed_length_public, sizeof(P22_t) + sizeof(map_group1));
	memcpy(pkx_pck->pt_public_key_seed, pkx_unpck->pt_public_key_seed, seed_length_public);
}

/**
 * expand public key
 * @param pkx - output
 * @param pk - input
 */
static void expand_public_core(public_key_expand *pkx, const uint8_t *pk) {
	public_key *pk_stru = (public_key *)pk;
	memcpy(pkx->pt_public_key_seed, pk_stru->pt_public_key_seed, sizeof(pk_stru->pt_public_key_seed));
	// generate PRNG part of public key
	gen_A_B_Q_P(&(pkx->map1), pk_stru->pt_public_key_seed);
	// read  P22
	input_P22((uint8_t *)pkx->P22, (uint8_t *)pk_stru->P22);
}

/**
 * expand public key
 * @param pkx - output
 * @param pk - input
 */
static void expand_public_pack_core(uint8_t *pkx_pck, const uint8_t *pk) {
	public_key_expand pkx_unpack;
	public_key *pk_stru = (public_key *)pk;
	memcpy(pkx_unpack.pt_public_key_seed, pk_stru->pt_public_key_seed, sizeof(pk_stru->pt_public_key_seed));
	// generate PRNG part of public key
	gen_A_B_Q_P(&(pkx_unpack.map1), pk_stru->pt_public_key_seed);
	// read  P22
	input_P22((uint8_t *)pkx_unpack.P22, (uint8_t *)pk_stru->P22);
	// pack gf16 -> bytes
	pkx_pack((public_key_expand_pack *)pkx_pck, &pkx_unpack);
}

/**
 * createHashOut
 */
static void createSignedHash(const uint8_t *digest, uint64_t bytes_digest, const uint8_t *pt_public_key_seed,
                             const uint8_t *array_salt, uint8_t *signed_hash_out) {
	Keccak_HashInstance hashInstance;
	Keccak_HashInitialize_SHAKE256(&hashInstance);
	Keccak_HashUpdate(&hashInstance, pt_public_key_seed, 8 * seed_length_public);
	Keccak_HashUpdate(&hashInstance, digest, 8 * bytes_digest);
	Keccak_HashUpdate(&hashInstance, array_salt, 8 * bytes_salt);
	Keccak_HashFinal(&hashInstance, NULL);
	Keccak_HashSqueeze(&hashInstance, signed_hash_out, 8 * bytes_hash);
}

#endif
