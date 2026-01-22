// SPDX-License-Identifier: MIT

#include <stdlib.h>

#include <oqs/sig_ml_dsa.h>

#if defined(OQS_ENABLE_SIG_ml_dsa_87)
OQS_SIG *OQS_SIG_ml_dsa_87_new(void) {

	OQS_SIG *sig = OQS_MEM_malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_ml_dsa_87;
	sig->alg_version = "FIPS204";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;
	sig->suf_cma = true;
	sig->sig_with_ctx_support = true;

	sig->length_public_key = OQS_SIG_ml_dsa_87_length_public_key;
	sig->length_secret_key = OQS_SIG_ml_dsa_87_length_secret_key;
	sig->length_signature = OQS_SIG_ml_dsa_87_length_signature;

	sig->keypair = OQS_SIG_ml_dsa_87_keypair;
	sig->sign = OQS_SIG_ml_dsa_87_sign;
	sig->verify = OQS_SIG_ml_dsa_87_verify;
	sig->sign_with_ctx_str = OQS_SIG_ml_dsa_87_sign_with_ctx_str;
	sig->verify_with_ctx_str = OQS_SIG_ml_dsa_87_verify_with_ctx_str;

	return sig;
}

extern int PQCP_MLDSA_NATIVE_MLDSA87_C_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA87_C_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen, const uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA87_C_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen, const uint8_t *pk);

#if defined(OQS_ENABLE_SIG_ml_dsa_87_x86_64)
extern int PQCP_MLDSA_NATIVE_MLDSA87_X86_64_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA87_X86_64_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen, const uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA87_X86_64_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen, const uint8_t *pk);
#endif

#if defined(OQS_ENABLE_SIG_ml_dsa_87_aarch64)
extern int PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen, const uint8_t *sk);
extern int PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *ctx, size_t ctxlen, const uint8_t *pk);
#endif

OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_keypair(uint8_t *public_key, uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_ml_dsa_87_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_X86_64_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_ml_dsa_87_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_keypair(public_key, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_keypair(public_key, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_keypair(public_key, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_ml_dsa_87_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_X86_64_signature(signature, signature_len, message, message_len, NULL, 0, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_signature(signature, signature_len, message, message_len, NULL, 0, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_ml_dsa_87_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_signature(signature, signature_len, message, message_len, NULL, 0, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_signature(signature, signature_len, message, message_len, NULL, 0, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_signature(signature, signature_len, message, message_len, NULL, 0, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
#if defined(OQS_ENABLE_SIG_ml_dsa_87_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_X86_64_verify(signature, signature_len, message, message_len, NULL, 0, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_verify(signature, signature_len, message, message_len, NULL, 0, public_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_ml_dsa_87_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_verify(signature, signature_len, message, message_len, NULL, 0, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_verify(signature, signature_len, message, message_len, NULL, 0, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_verify(signature, signature_len, message, message_len, NULL, 0, public_key);
#endif
}
OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_sign_with_ctx_str(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *secret_key) {
#if defined(OQS_ENABLE_SIG_ml_dsa_87_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_X86_64_signature(signature, signature_len, message, message_len, ctx_str, ctx_str_len, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_signature(signature, signature_len, message, message_len, ctx_str, ctx_str_len, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_ml_dsa_87_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_signature(signature, signature_len, message, message_len, ctx_str, ctx_str_len, secret_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_signature(signature, signature_len, message, message_len, ctx_str, ctx_str_len, secret_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_signature(signature, signature_len, message, message_len, ctx_str, ctx_str_len, secret_key);
#endif
}

OQS_API OQS_STATUS OQS_SIG_ml_dsa_87_verify_with_ctx_str(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *ctx_str, size_t ctx_str_len, const uint8_t *public_key) {
#if defined(OQS_ENABLE_SIG_ml_dsa_87_x86_64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_AVX2) && OQS_CPU_has_extension(OQS_CPU_EXT_BMI2) && OQS_CPU_has_extension(OQS_CPU_EXT_POPCNT)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_X86_64_verify(signature, signature_len, message, message_len, ctx_str, ctx_str_len, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_verify(signature, signature_len, message, message_len, ctx_str, ctx_str_len, public_key);
	}
#endif /* OQS_DIST_BUILD */
#elif defined(OQS_ENABLE_SIG_ml_dsa_87_aarch64)
#if defined(OQS_DIST_BUILD)
	if (OQS_CPU_has_extension(OQS_CPU_EXT_ARM_NEON)) {
#endif /* OQS_DIST_BUILD */
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_AARCH64_verify(signature, signature_len, message, message_len, ctx_str, ctx_str_len, public_key);
#if defined(OQS_DIST_BUILD)
	} else {
		return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_verify(signature, signature_len, message, message_len, ctx_str, ctx_str_len, public_key);
	}
#endif /* OQS_DIST_BUILD */
#else
	return (OQS_STATUS) PQCP_MLDSA_NATIVE_MLDSA87_C_verify(signature, signature_len, message, message_len, ctx_str, ctx_str_len, public_key);
#endif
}
#endif
