/* qTesla-p-III */

#include "sig_qtesla.h"

#ifdef OQS_ENABLE_SIG_qTesla_p_III

#define _qTESLA_p_III_

#if defined(WITH_AVX2) && !defined(_WIN32)
#define poly_ntt_asm oqs_sig_qteslapiii_poly_ntt_asm
#define poly_pmul_asm oqs_sig_qteslapiii_poly_pmul_asm
#define poly_intt_asm oqs_sig_qteslapiii_poly_intt_asm
#include "avx2/consts.c"
#include "avx2/s_consts.c"
#include "avx2/gauss.c"
#include "avx2/pack.c"
#include "avx2/poly.c"
#include "avx2/sample.c"
#include "avx2/sign.c"
#else
#include "portable/consts.c"
#include "portable/gauss.c"
#include "portable/pack.c"
#include "portable/poly.c"
#include "portable/sample.c"
#include "portable/sign.c"
#endif

OQS_API OQS_STATUS OQS_SIG_qTesla_p_III_keypair(uint8_t *public_key, uint8_t *secret_key) {
	int rv = crypto_sign_keypair(public_key, secret_key);
	if (rv == 0) {
		return OQS_SUCCESS;
	} else {
		return OQS_ERROR;
	}
}

OQS_API OQS_STATUS OQS_SIG_qTesla_p_III_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	int rv = crypto_sign(signature, (long long unsigned int *) signature_len, message, message_len, secret_key);
	if (rv == 0) {
		return OQS_SUCCESS;
	} else {
		return OQS_ERROR;
	}
}

OQS_API OQS_STATUS OQS_SIG_qTesla_p_III_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	int rv = crypto_sign_open((unsigned char *) message, message_len, signature, signature_len, public_key);
	if (rv == 0) {
		return OQS_SUCCESS;
	} else {
		return OQS_ERROR;
	}
}
#endif
