// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_classic_mceliece.h>

#if defined(OQS_ENABLE_KEM_classic_mceliece_348864)

OQS_KEM *OQS_KEM_classic_mceliece_348864_new() {

	OQS_KEM *kem = malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_classic_mceliece_348864;
	kem->alg_version = "SUPERCOP-20191221";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_classic_mceliece_348864_length_public_key;
	kem->length_secret_key = OQS_KEM_classic_mceliece_348864_length_secret_key;
	kem->length_ciphertext = OQS_KEM_classic_mceliece_348864_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_classic_mceliece_348864_length_shared_secret;

	kem->keypair = OQS_KEM_classic_mceliece_348864_keypair;
	kem->encaps = OQS_KEM_classic_mceliece_348864_encaps;
	kem->decaps = OQS_KEM_classic_mceliece_348864_decaps;

	return kem;
}

extern int PQCLEAN_MCELIECE348864_VEC_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
extern int PQCLEAN_MCELIECE348864_VEC_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
extern int PQCLEAN_MCELIECE348864_VEC_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);

#if defined(OQS_ENABLE_KEM_classic_mceliece_348864_avx)
extern int PQCLEAN_MCELIECE348864_AVX_crypto_kem_keypair(unsigned char *pk, unsigned char *sk);
extern int PQCLEAN_MCELIECE348864_AVX_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk);
extern int PQCLEAN_MCELIECE348864_AVX_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk);
#endif

OQS_API OQS_STATUS OQS_KEM_classic_mceliece_348864_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_classic_mceliece_348864_avx)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.POPCNT_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_MCELIECE348864_AVX_crypto_kem_keypair(public_key, secret_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_MCELIECE348864_VEC_crypto_kem_keypair(public_key, secret_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_MCELIECE348864_VEC_crypto_kem_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_classic_mceliece_348864_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
#if defined(OQS_ENABLE_KEM_classic_mceliece_348864_avx)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.POPCNT_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_MCELIECE348864_AVX_crypto_kem_enc(ciphertext, shared_secret, public_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_MCELIECE348864_VEC_crypto_kem_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_MCELIECE348864_VEC_crypto_kem_enc(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_classic_mceliece_348864_decaps(uint8_t *shared_secret, const unsigned char *ciphertext, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_KEM_classic_mceliece_348864_avx)
#if defined(OQS_PORTABLE_BUILD)
	OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions();
	if (available_cpu_extensions.AVX2_ENABLED && available_cpu_extensions.POPCNT_ENABLED) {
#endif /* OQS_PORTABLE_BUILD */
		return (OQS_STATUS) PQCLEAN_MCELIECE348864_AVX_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#if defined(OQS_PORTABLE_BUILD)
	} else {
		return (OQS_STATUS) PQCLEAN_MCELIECE348864_VEC_crypto_kem_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_PORTABLE_BUILD */
#else
	return (OQS_STATUS) PQCLEAN_MCELIECE348864_VEC_crypto_kem_dec(shared_secret, ciphertext, secret_key);
#endif
}

#endif
