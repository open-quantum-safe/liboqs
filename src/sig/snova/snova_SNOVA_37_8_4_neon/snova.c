#include "snova.h"

#include "gf16_matrix_inline.h"

#define i_prime(mi, alpha) ((alpha + mi) % o_SNOVA)
#define i_prime_inv(mi, alpha) ((o_SNOVA * alpha_SNOVA - alpha + mi) % o_SNOVA)

#if FIXED_ABQ
uint8_t fixed_abq[4 * m_SNOVA * alpha_SNOVA * lsq_SNOVA] = {0};
#endif

#include "snova_kernel.h"

#if OPTIMISATION != 0
#include "snova_plasma_option.h"
#else
#include "snova_ref.h"
#define gen_F gen_F_ref
#define gen_P22 gen_P22_ref
#define sign_digest_core sign_digest_core_ref
#define verify_core verify_signture_ref
#define verify_pkx_core verify_signture_pkx_ref
#endif

#if FIXED_ABQ
static void gen_ABQ(const char *abq_seed) {
	uint8_t rng_out[m_SNOVA * alpha_SNOVA * (lsq_SNOVA + l_SNOVA)];
	uint8_t q12[2 * m_SNOVA * alpha_SNOVA * l_SNOVA];

	shake256((uint8_t *)abq_seed, strlen(abq_seed), rng_out, m_SNOVA * alpha_SNOVA * (lsq_SNOVA + l_SNOVA));

	convert_bytes_to_GF16s(rng_out, fixed_abq, 2 * m_SNOVA * alpha_SNOVA * lsq_SNOVA);
	convert_bytes_to_GF16s(&rng_out[alpha_SNOVA * lsq_SNOVA], q12, 2 * m_SNOVA * alpha_SNOVA * l_SNOVA);

	for (int pi = 0; pi < m_SNOVA; ++pi) {
		for (int index = 0; index < alpha_SNOVA; ++index) {
			// be_invertible_by_add_aS(map->Aalpha[index]);
			be_invertible_by_add_aS(&fixed_abq[(pi * alpha_SNOVA + index) * lsq_SNOVA]);
		}
		for (int index = 0; index < alpha_SNOVA; ++index) {
			// be_invertible_by_add_aS(map->Balpha[index]);
			be_invertible_by_add_aS(&fixed_abq[((m_SNOVA + pi) * alpha_SNOVA + index) * lsq_SNOVA]);
		}
		for (int index = 0; index < alpha_SNOVA; ++index) {
			// gen_a_FqS(pt_array, map->Qalpha1[index]);
			gen_a_FqS(&q12[(pi * alpha_SNOVA + index) * l_SNOVA],
			          &fixed_abq[((2 * m_SNOVA + pi) * alpha_SNOVA + index) * lsq_SNOVA]);
		}
		for (int index = 0; index < alpha_SNOVA; ++index) {
			// gen_a_FqS(pt_array, map->Qalpha2[index]);
			gen_a_FqS(&q12[((m_SNOVA + pi) * alpha_SNOVA + index) * l_SNOVA],
			          &fixed_abq[((3 * m_SNOVA + pi) * alpha_SNOVA + index) * lsq_SNOVA]);
		}
	}
}
#endif

/**
 * SNOVA init
 */
static void snova_init(void) {
	static int first_time = 1;
	if (first_time) {
		first_time = 0;
		init_gf16_tables();
		gen_S_array();

#if FIXED_ABQ
		gen_ABQ("SNOVA_ABQ");
#endif

#if OPTIMISATION != 0
		snova_plasma_init();
#endif
	}
}

/**
 * generate snova key elements.
 * @param key_elems - pointer to output snova key elements.
 * @param pk_seed - pointer to input public key seed.
 * @param sk_seed - pointer to input private key elements.
 */
static void generate_keys_core(snova_key_elems *key_elems, const uint8_t *pk_seed, const uint8_t *sk_seed) {
	gen_seeds_and_T12(key_elems->T12, sk_seed);
	memcpy(key_elems->pk.pt_public_key_seed, pk_seed, seed_length_public);
	gen_A_B_Q_P(&(key_elems->map1), pk_seed);
	gen_F(&(key_elems->map2), &(key_elems->map1), key_elems->T12);
	gen_P22(key_elems->pk.P22, key_elems->T12, key_elems->map1.P21, key_elems->map2.F12);
}

int expand_secret(uint8_t *esk, const uint8_t *sk) {
	const uint8_t *pk_seed = sk;
	const uint8_t *sk_seed = sk + seed_length_public;
	snova_key_elems key_elems;

	generate_keys_core(&key_elems, pk_seed, sk_seed);
	sk_pack(esk, &key_elems, sk_seed);

	// Clear Secret!
	snova_set_zero(&key_elems, sizeof(key_elems));

	return 0;
}

/**
 * Generates public and private key. where private key is the seed of private
 * key.
 * @param pk - pointer to output public key.
 * @param ssk - pointer to output private key.
 * @param pkseed - pointer to input public key seed.
 * @param skseed - pointer to input private key seed.
 */
int generate_keys_ssk(uint8_t *pk, uint8_t *ssk, const uint8_t *pkseed, const uint8_t *skseed) {
	snova_init();
	snova_key_elems key_elems;
	generate_keys_core(&key_elems, pkseed, skseed);
	pk_pack(pk, &key_elems);
	memcpy(ssk, pkseed, seed_length_public);
	memcpy(ssk + seed_length_public, skseed, seed_length_private);

	// Clear Secret!
	SNOVA_CLEAR_BYTE(&key_elems, sizeof(key_elems));
	return 0;
}

/**
 * Generates public and private key. where private key is the expanded version.
 * @param pk - pointer to output public key.
 * @param esk - pointer to output private key. (expanded)
 * @param pkseed - pointer to input public key seed.
 * @param skseed - pointer to input private key seed.
 */
int generate_keys_esk(uint8_t *pk, uint8_t *esk, const uint8_t *pkseed, const uint8_t *skseed) {
	snova_init();
	snova_key_elems key_elems;
	generate_keys_core(&key_elems, pkseed, skseed);
	pk_pack(pk, &key_elems);
	sk_pack(esk, &key_elems, skseed);

	// Clear Secret!
	SNOVA_CLEAR_BYTE(&key_elems, sizeof(key_elems));
	return 0;
}

/**
 * Generates public key. where private key is the seed of private
 * @param pk - pointer to output public key.
 * @param ssk - pointer to output private key.
 */
int generate_pk_with_ssk(uint8_t *pk, const uint8_t *ssk) {
	snova_init();
	const uint8_t *pkseed = ssk;
	const uint8_t *skseed = ssk + seed_length_public;
	snova_key_elems key_elems;

	generate_keys_core(&key_elems, pkseed, skseed);
	pk_pack(pk, &key_elems);

	// Clear Secret!
	SNOVA_CLEAR_BYTE(&key_elems, sizeof(key_elems));
	return 0;
}

/**
 * Generates public key. where private key is the expanded version.
 * @param pk - pointer to output public key.
 * @param @param esk - pointer to output private key. (expanded)
 */
int generate_pk_with_esk(uint8_t *pk, const uint8_t *esk) {
	snova_init();

	T12_t T12;
	uint8_t public_key_seed[seed_length_public];

	{
		// Limit scope of sk_upk
		sk_gf16 sk_upk;
		sk_unpack(&sk_upk, esk);
		memcpy(public_key_seed, sk_upk.pt_public_key_seed, seed_length_public);
		memcpy(T12, sk_upk.T12, sizeof(T12_t));
	}

	snova_key_elems key_elems;
	memcpy(key_elems.pk.pt_public_key_seed, public_key_seed, seed_length_public);
	gen_A_B_Q_P(&(key_elems.map1), public_key_seed);
	gen_F(&(key_elems.map2), &(key_elems.map1), T12);
	gen_P22(key_elems.pk.P22, T12, key_elems.map1.P21, key_elems.map2.F12);
	pk_pack(pk, &key_elems);

	// Clear Secret!
	SNOVA_CLEAR_BYTE(&key_elems, sizeof(key_elems));
	return 0;
}

/**
 * Expand ssk. Release map1 after use to reduce stack usage.
 */
void sign_expand_ssk(sk_gf16 *sk_upk, const uint8_t *ssk) {
	map_group1 map1;
	T12_t T12;

	gen_A_B_Q_P(&(map1), ssk);
	gen_seeds_and_T12(T12, ssk + seed_length_public);
	gen_F((map_group2 *) sk_upk->F11, &map1, T12);

	memcpy(sk_upk->Aalpha, map1.Aalpha, sizeof(Aalpha_t));
	memcpy(sk_upk->Balpha, map1.Balpha, sizeof(Balpha_t));
	memcpy(sk_upk->Qalpha1, map1.Qalpha1, sizeof(Qalpha1_t));
	memcpy(sk_upk->Qalpha2, map1.Qalpha2, sizeof(Qalpha2_t));
	memcpy(sk_upk->T12, T12, sizeof(T12_t));
	memcpy(sk_upk->pt_public_key_seed, ssk, seed_length_public);
	memcpy(sk_upk->pt_private_key_seed, ssk + seed_length_public, seed_length_private);
}

/**
 * Compute the signature using ssk (private key seed). some preparatory work
 * before using sign_digest_core()
 * @param pt_signature - pointer to output signature.
 * @param digest - pointer to input digest.
 * @param array_salt - pointer to input salt.
 * @param ssk - pointer to input private key (seed).
 */
int sign_digest_ssk(uint8_t *pt_signature, const uint8_t *digest, uint64_t bytes_digest, uint8_t *array_salt,
                    const uint8_t *ssk) {
	snova_init();
	sk_gf16 sk_upk;
	sign_expand_ssk(&sk_upk, ssk);
	int res = sign_digest_core(pt_signature, digest, bytes_digest, array_salt, sk_upk.Aalpha, sk_upk.Balpha, sk_upk.Qalpha1,
	                           sk_upk.Qalpha2, sk_upk.T12, sk_upk.F11, sk_upk.F12, sk_upk.F21, sk_upk.pt_public_key_seed,
	                           sk_upk.pt_private_key_seed);
	// Clear Secret!
	SNOVA_CLEAR_BYTE(&sk_upk, sizeof(sk_upk));
	return res;
}

/**
 * Compute the signature using esk (). some preparatory work before using
 * sign_digest_core()
 * @param pt_signature - pointer to output signature.
 * @param digest - pointer to input digest.
 * @param array_salt - pointer to input salt.
 * @param esk - pointer to input private key (expanded).
 */
int sign_digest_esk(uint8_t *pt_signature, const uint8_t *digest, uint64_t bytes_digest, uint8_t *array_salt,
                    const uint8_t *esk) {
	snova_init();
	sk_gf16 sk_upk;
	sk_unpack(&sk_upk, esk);
	int res = sign_digest_core(pt_signature, digest, bytes_digest, array_salt, sk_upk.Aalpha, sk_upk.Balpha, sk_upk.Qalpha1,
	                           sk_upk.Qalpha2, sk_upk.T12, sk_upk.F11, sk_upk.F12, sk_upk.F21, sk_upk.pt_public_key_seed,
	                           sk_upk.pt_private_key_seed);

	// Clear Secret!
	SNOVA_CLEAR_BYTE(&sk_upk, sizeof(sk_upk));
	return res;
}

/**
 * Verifies signature.
 * @param pt_digest - pointer to input digest.
 * @param pt_signature - pointer to output signature.
 * @param pk - pointer to output public key.
 * @returns - 0 if signature could be verified correctly and -1 otherwise
 */
int verify_signture(const uint8_t *pt_digest, uint64_t bytes_digest, const uint8_t *pt_signature, const uint8_t *pk) {
	snova_init();
	return verify_core(pt_digest, bytes_digest, pt_signature, pk);
}

/**
 * Verifies signature.
 * @param pt_digest - pointer to input digest.
 * @param pt_signature - pointer to output signature.
 * @param pkx - pointer to output public key expend.
 * @returns - 0 if signature could be verified correctly and -1 otherwise
 */
int verify_signture_pkx(const uint8_t *pt_digest, uint64_t bytes_digest, const uint8_t *pt_signature, const uint8_t *pkx) {
	snova_init();
	return verify_pkx_core(pt_digest, bytes_digest, pt_signature, pkx);
}

int expand_public_pack(uint8_t *pkx, const uint8_t *pk) {
	snova_init();
	expand_public_pack_core(pkx, pk);
	return 0;
}
