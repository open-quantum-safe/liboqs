// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/sig_ml_dsa.h>

#if defined(OQS_ENABLE_SIG_ml_dsa_44_extmu)
OQS_SIG *OQS_SIG_ml_dsa_44_extmu_new(void) {

	OQS_SIG *sig = OQS_MEM_malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_ml_dsa_44_extmu;
	sig->alg_version = "FIPS204";

	sig->claimed_nist_level = 2;
	sig->euf_cma = true;
	sig->suf_cma = true;
	sig->sig_with_ctx_support = false;

	sig->length_public_key = OQS_SIG_ml_dsa_44_extmu_length_public_key;
	sig->length_secret_key = OQS_SIG_ml_dsa_44_extmu_length_secret_key;
	sig->length_signature = OQS_SIG_ml_dsa_44_extmu_length_signature;

	sig->keypair = OQS_SIG_ml_dsa_44_extmu_keypair;
	sig->sign = OQS_SIG_ml_dsa_44_extmu_sign;
	sig->verify = OQS_SIG_ml_dsa_44_extmu_verify;
	sig->sign_with_ctx_str = OQS_SIG_ml_dsa_44_extmu_sign_with_ctx_str;
	sig->verify_with_ctx_str = OQS_SIG_ml_dsa_44_extmu_verify_with_ctx_str;

	return sig;
}

extern int PQCP_MLDSA_NATIVE_MLDSA44_C_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA44_C_signature_extmu(uint8_t *sig, size_t *siglen, const uint8_t *m, const uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA44_C_verify_extmu(const uint8_t *sig, size_t siglen, const uint8_t *m, const uint8_t *pk);

#if defined(OQS_ENABLE_SIG_ml_dsa_44_extmu_x86_64)
extern int PQCP_MLDSA_NATIVE_MLDSA44_X86_64_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA44_X86_64_signature_extmu(uint8_t *sig, size_t *siglen, const uint8_t *m, const uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA44_X86_64_verify_extmu(const uint8_t *sig, size_t siglen, const uint8_t *m, const uint8_t *pk);
#endif

#if defined(OQS_ENABLE_SIG_ml_dsa_44_extmu_aarch64)
extern int PQCP_MLDSA_NATIVE_MLDSA44_AARCH64_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA44_AARCH64_signature_extmu(uint8_t *sig, size_t *siglen, const uint8_t *m, const uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA44_AARCH64_verify_extmu(const uint8_t *sig, size_t siglen, const uint8_t *m, const uint8_t *pk);
#endif

OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_ml_dsa_44_extmu_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_X86_64_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_ml_dsa_44_extmu_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_AARCH64_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	/* extmu variants require exactly 64 bytes of input message representative */
	if (message_len != 64) {
		return OQS_ERROR;
	}
	(void)message_len;
#if defined(OQS_ENABLE_SIG_ml_dsa_44_extmu_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_X86_64_signature_extmu(signature, signature_len, message, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_signature_extmu(signature, signature_len, message, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_ml_dsa_44_extmu_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_AARCH64_signature_extmu(signature, signature_len, message, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_signature_extmu(signature, signature_len, message, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_signature_extmu(signature, signature_len, message, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	/* extmu variants require exactly 64 bytes of input message representative */
	if (message_len != 64) {
		return OQS_ERROR;
	}
	(void)message_len;
#if defined(OQS_ENABLE_SIG_ml_dsa_44_extmu_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_X86_64_verify_extmu(signature, signature_len, message, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_verify_extmu(signature, signature_len, message, public_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_ml_dsa_44_extmu_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_AARCH64_verify_extmu(signature, signature_len, message, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_verify_extmu(signature, signature_len, message, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA44_C_verify_extmu(signature, signature_len, message, public_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *secret_key) {
	if (message_len != 64) {
		return OQS_ERROR;
	}
	(void)message_len;
	if (ctx_str == NULL && ctx_str_len == 0) {
		return OQS_SIG_ml_dsa_44_extmu_sign(signature, signature_len, message, message_len, secret_key);
	} else {
		return OQS_ERROR;
	}
}

OQS_API OQS_STATUS OQS_SIG_ml_dsa_44_extmu_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *public_key) {
	if (message_len != 64) {
		return OQS_ERROR;
	}
	(void)message_len;
	if (ctx_str == NULL && ctx_str_len == 0) {
		return OQS_SIG_ml_dsa_44_extmu_verify(message, message_len, signature, signature_len, public_key);
	} else {
		return OQS_ERROR;
	}
}
#endif
