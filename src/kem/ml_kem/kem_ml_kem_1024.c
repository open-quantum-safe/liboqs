// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/kem_ml_kem.h>

#if defined(OQS_ENABLE_KEM_ml_kem_1024)

OQS_KEM *OQS_KEM_ml_kem_1024_new(void) {

	OQS_KEM *kem = OQS_MEM_malloc(sizeof(OQS_KEM));
	if (kem == NULL) {
		return NULL;
	}
	kem->method_name = OQS_KEM_alg_ml_kem_1024;
	kem->alg_version = "FIPS203";

	kem->claimed_nist_level = 5;
	kem->ind_cca = true;

	kem->length_public_key = OQS_KEM_ml_kem_1024_length_public_key;
	kem->length_secret_key = OQS_KEM_ml_kem_1024_length_secret_key;
	kem->length_ciphertext = OQS_KEM_ml_kem_1024_length_ciphertext;
	kem->length_shared_secret = OQS_KEM_ml_kem_1024_length_shared_secret;

	kem->keypair = OQS_KEM_ml_kem_1024_keypair;
	kem->encaps = OQS_KEM_ml_kem_1024_encaps;
	kem->decaps = OQS_KEM_ml_kem_1024_decaps;

	return kem;
}

extern int PQCP_MLKEM_NATIVE_MLKEM1024_C_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLKEM_NATIVE_MLKEM1024_C_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCP_MLKEM_NATIVE_MLKEM1024_C_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#if defined(OQS_ENABLE_KEM_ml_kem_1024_x86_64)
extern int PQCP_MLKEM_NATIVE_MLKEM1024_X86_64_DEFAULT_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLKEM_NATIVE_MLKEM1024_X86_64_DEFAULT_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCP_MLKEM_NATIVE_MLKEM1024_X86_64_DEFAULT_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif

#if defined(OQS_ENABLE_KEM_ml_kem_1024_aarch64)
extern int PQCP_MLKEM_NATIVE_MLKEM1024_AARCH64_OPT_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLKEM_NATIVE_MLKEM1024_AARCH64_OPT_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int PQCP_MLKEM_NATIVE_MLKEM1024_AARCH64_OPT_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif

#if defined(OQS_USE_CUPQC)
#if defined(OQS_ENABLE_KEM_ml_kem_1024_cuda)
extern int cupqc_ml_kem_1024_keypair(uint8_t *pk, uint8_t *sk);
extern int cupqc_ml_kem_1024_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk);
extern int cupqc_ml_kem_1024_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);
#endif
#endif /* OQS_USE_CUPQC */

OQS_API OQS_STATUS OQS_KEM_ml_kem_1024_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_USE_CUPQC) && defined(OQS_ENABLE_KEM_ml_kem_1024_cuda)
	return (OQS_STATUS) cupqc_ml_kem_1024_keypair(public_key, secret_key);
#endif /* OQS_USE_CUPQC && OQS_ENABLE_KEM_ml_kem_1024_cuda */
#if defined(OQS_ENABLE_KEM_ml_kem_1024_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_X86_64_DEFAULT_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_KEM_ml_kem_1024_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_AARCH64_OPT_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_ml_kem_1024_encaps(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
#if defined(OQS_USE_CUPQC) && defined(OQS_ENABLE_KEM_ml_kem_1024_cuda)
	return (OQS_STATUS) cupqc_ml_kem_1024_enc(ciphertext, shared_secret, public_key);
#endif /* OQS_USE_CUPQC && OQS_ENABLE_KEM_ml_kem_1024_cuda */
#if defined(OQS_ENABLE_KEM_ml_kem_1024_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_X86_64_DEFAULT_enc(ciphertext, shared_secret, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_KEM_ml_kem_1024_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_AARCH64_OPT_enc(ciphertext, shared_secret, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_enc(ciphertext, shared_secret, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_enc(ciphertext, shared_secret, public_key);
#endif
}

OQS_API OQS_STATUS OQS_KEM_ml_kem_1024_decaps(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
#if defined(OQS_USE_CUPQC) && defined(OQS_ENABLE_KEM_ml_kem_1024_cuda)
	return (OQS_STATUS) cupqc_ml_kem_1024_dec(shared_secret, ciphertext, secret_key);
#endif /* OQS_USE_CUPQC && OQS_ENABLE_KEM_ml_kem_1024_cuda */
#if defined(OQS_ENABLE_KEM_ml_kem_1024_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_X86_64_DEFAULT_dec(shared_secret, ciphertext, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_KEM_ml_kem_1024_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_AARCH64_OPT_dec(shared_secret, ciphertext, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_dec(shared_secret, ciphertext, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLKEM_NATIVE_MLKEM1024_C_dec(shared_secret, ciphertext, secret_key);
#endif
}

#endif
