// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_ml_kem.h>
#include <randombytes.h>

#if defined(OQS_ENABLE_KEM_ml_kem_512)

OQS_KEM *OQS_KEM_ml_kem_512_new(void) {

	OQS_KEM *kem = OQS_MEM_malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_ml_kem_512;
	kem->alg_version = "https://github.com/pq-crystals/kyber/tree/standard";

	kem->claimed_nist_level = 1;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_ml_kem_512_length_public_key;
	kem->length_secret_key = OQS_KEM_ml_kem_512_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ml_kem_512_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ml_kem_512_length_shared_secret;

	kem->keypair = OQS_KEM_ml_kem_512_keypair;
	kem->encaps = OQS_KEM_ml_kem_512_encaps;
	kem->decaps = OQS_KEM_ml_kem_512_decaps;

	return kem;
}

#define KYBER512_KEYPAIRCOINBYTES 64
#define KYBER512_SECRETKEYBYTES 1632
#define KYBER512_PUBLICKEYBYTES 800

extern int pqcrystals_ml_kem_512_ref_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t* coins);
extern int pqcrystals_ml_kem_512_ref_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int pqcrystals_ml_kem_512_ref_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#if defined(OQS_ENABLE_KEM_ml_kem_512_avx2)
extern int pqcrystals_ml_kem_512_avx2_keypair_derand(uint8_t *pk, uint8_t *sk, const uint8_t* coins);
extern int pqcrystals_ml_kem_512_avx2_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int pqcrystals_ml_kem_512_avx2_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif

OQS_API OQS_STATUS OQS_KEM_ml_kem_512_keypair(uint8_t *public_key, uint8_t *secret_key) {
  uint8_t expanded_secret_key[KYBER512_SECRETKEYBYTES];
  randombytes(secret_key, KYBER512_KEYPAIRCOINBYTES);

#if defined(OQS_ENABLE_KEM_ml_kem_512_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_ml_kem_512_avx2_keypair_derand(public_key, expanded_secret_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_ml_kem_512_ref_keypair_derand(public_key, expanded_secret_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_ml_kem_512_ref_keypair_derand(public_key, expanded_secret_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_ml_kem_512_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
#if defined(OQS_ENABLE_KEM_ml_kem_512_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) pqcrystals_ml_kem_512_avx2_enc(ciphertext, shared_secret, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) pqcrystals_ml_kem_512_ref_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) pqcrystals_ml_kem_512_ref_enc(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_ml_kem_512_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
  uint8_t public_key[KYBER512_PUBLICKEYBYTES];
  uint8_t expanded_secret_key[KYBER512_SECRETKEYBYTES];
  OQS_STATUS status;

#if defined(OQS_ENABLE_KEM_ml_kem_512_avx2)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		status = (OQS_STATUS) pqcrystals_ml_kem_512_avx2_keypair_derand(public_key, expanded_secret_key, secret_key);
    if (status != OQS_SUCCESS) {
      OQS_MEM_cleanse(public_key, KYBER512_PUBLICKEYBYTES);
      OQS_MEM_cleanse(expanded_secret_key, KYBER512_SECRETKEYBYTES);
      return status;
    }

		return (OQS_STATUS) pqcrystals_ml_kem_512_avx2_dec(shared_secret, ciphertext, expanded_secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		status = (OQS_STATUS) pqcrystals_ml_kem_512_ref_keypair_derand(public_key, expanded_secret_key, secret_key);
    if (status != OQS_SUCCESS) {
      OQS_MEM_cleanse(public_key, KYBER512_PUBLICKEYBYTES);
      OQS_MEM_cleanse(expanded_secret_key, KYBER512_SECRETKEYBYTES);
      return status;
    }

		return (OQS_STATUS) pqcrystals_ml_kem_512_ref_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	status = (OQS_STATUS) pqcrystals_ml_kem_512_ref_keypair_derand(public_key, expanded_secret_key, secret_key);
  if (status != OQS_SUCCESS) {
    OQS_MEM_cleanse(public_key, KYBER512_PUBLICKEYBYTES);
    OQS_MEM_cleanse(expanded_secret_key, KYBER512_SECRETKEYBYTES);
    return status;
  }

	return (OQS_STATUS) pqcrystals_ml_kem_512_ref_dec(shared_secret, ciphertext, secret_key);
#endif
}

#endif
