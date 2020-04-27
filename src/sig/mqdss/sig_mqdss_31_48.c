#include <stdlib.h>

#include <oqs/sig_mqdss.h>

#if defined(OQS_ENABLE_SIG_mqdss_31_48)

OQS_SIG *OQS_SIG_mqdss_31_48_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_mqdss_31_48;
	sig->alg_version = "https://github.com/joostrijneveld/MQDSS/commit/00608d7610262ff07b1834885d32bc3fd27ef5e1";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_mqdss_31_48_length_public_key;
	sig->length_secret_key = OQS_SIG_mqdss_31_48_length_secret_key;
	sig->length_signature = OQS_SIG_mqdss_31_48_length_signature;

	sig->keypair = OQS_SIG_mqdss_31_48_keypair;
	sig->sign = OQS_SIG_mqdss_31_48_sign;
	sig->verify = OQS_SIG_mqdss_31_48_verify;

	return sig;
}

extern int PQCLEAN_MQDSS48_CLEAN_crypto_sign_keypair(uint8_t *pk, uint8_t *sk);
extern int PQCLEAN_MQDSS48_CLEAN_crypto_sign_signature(uint8_t *sig, size_t *siglen, const uint8_t *m, size_t mlen, const uint8_t *sk);
extern int PQCLEAN_MQDSS48_CLEAN_crypto_sign_verify(const uint8_t *sig, size_t siglen, const uint8_t *m, size_t mlen, const uint8_t *pk);

OQS_API OQS_STATUS OQS_SIG_mqdss_31_48_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_MQDSS48_CLEAN_crypto_sign_keypair(public_key, secret_key);
}

OQS_API OQS_STATUS OQS_SIG_mqdss_31_48_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return (OQS_STATUS) PQCLEAN_MQDSS48_CLEAN_crypto_sign_signature(signature, signature_len, message, message_len, secret_key);
}

OQS_API OQS_STATUS OQS_SIG_mqdss_31_48_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return (OQS_STATUS) PQCLEAN_MQDSS48_CLEAN_crypto_sign_verify(signature, signature_len, message, message_len, public_key);
}

#endif
