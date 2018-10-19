#include <oqs/sig.h>

#if defined(OQS_SIG_alg_picnic_L1_UR) || defined(OQS_SIG_alg_picnic_L1_FS) || defined(OQS_SIG_alg_picnic_L3_UR) || defined(OQS_SIG_alg_picnic_L3_FS) || defined(OQS_SIG_alg_picnic_L5_UR) || defined(OQS_SIG_alg_picnic_L5_FS)

#include <string.h>
#include <oqs/common.h>
#include "sig_picnic.h"
#include "external/picnic.h"

static size_t PRIV_KEY_LEN[] = {
    0,
    PICNIC_PRIVATE_KEY_SIZE(Picnic_L1_FS),
    PICNIC_PRIVATE_KEY_SIZE(Picnic_L1_UR),
    PICNIC_PRIVATE_KEY_SIZE(Picnic_L3_FS),
    PICNIC_PRIVATE_KEY_SIZE(Picnic_L3_UR),
    PICNIC_PRIVATE_KEY_SIZE(Picnic_L5_FS),
    PICNIC_PRIVATE_KEY_SIZE(Picnic_L5_UR)};
static size_t PUB_KEY_LEN[] = {
    0,
    PICNIC_PUBLIC_KEY_SIZE(Picnic_L1_FS),
    PICNIC_PUBLIC_KEY_SIZE(Picnic_L1_UR),
    PICNIC_PUBLIC_KEY_SIZE(Picnic_L3_FS),
    PICNIC_PUBLIC_KEY_SIZE(Picnic_L3_UR),
    PICNIC_PUBLIC_KEY_SIZE(Picnic_L5_FS),
    PICNIC_PUBLIC_KEY_SIZE(Picnic_L5_UR)};

static OQS_STATUS common_picnic_keypair(picnic_params_t parameters, uint8_t *priv, uint8_t *pub) {
	if (priv == NULL || pub == NULL) {
		return OQS_ERROR;
	}
	picnic_publickey_t pk;
	picnic_privatekey_t sk;
	int ret = picnic_keygen(parameters, &pk, &sk);
	if (ret != 0) { // DO NOT modify this return code to OQS_SUCCESS/OQS_ERROR
		return OQS_ERROR;
	}
	// serialize the public key
	int pk_len = picnic_write_public_key(&pk, pub, PUB_KEY_LEN[parameters]);
	if ((size_t) pk_len != PUB_KEY_LEN[parameters]) {
		return OQS_ERROR;
	}

	// serialize the private key
	int sk_len = picnic_write_private_key(&sk, priv, PRIV_KEY_LEN[parameters]);
	if ((size_t) sk_len != PRIV_KEY_LEN[parameters]) {
		return OQS_ERROR;
	}
	// wipe the private key
	OQS_MEM_cleanse(&sk, sizeof(picnic_privatekey_t));
	return OQS_SUCCESS;
}

static OQS_STATUS common_picnic_sign(picnic_params_t parameters, const uint8_t *priv, const uint8_t *msg, const size_t msg_len, uint8_t *sig, size_t *sig_len) {
	if (priv == NULL || msg == NULL || sig == NULL || sig_len == NULL) {
		return OQS_ERROR;
	}
	picnic_privatekey_t sk;
	// deserialize the private key
	if (picnic_read_private_key(&sk, priv, PRIV_KEY_LEN[parameters]) != 0) {
		return OQS_ERROR;
	}
	if (picnic_sign(&sk, msg, msg_len, sig, sig_len) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

static OQS_STATUS common_picnic_verify(picnic_params_t parameters, const uint8_t *pub, const uint8_t *msg, const size_t msg_len, const uint8_t *sig, const size_t sig_len) {
	if (pub == NULL || msg == NULL || sig == NULL) {
		return OQS_ERROR;
	}
	picnic_publickey_t pk;
	// deserialize the public key
	if (picnic_read_public_key(&pk, pub, PUB_KEY_LEN[parameters]) != 0) {
		return OQS_ERROR;
	}
	if (picnic_verify(&pk, msg, msg_len, sig, sig_len) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}
#endif

#ifdef OQS_ENABLE_SIG_picnic_L1_FS

OQS_SIG *OQS_SIG_picnic_L1_FS_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L1_FS;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L1_FS_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L1_FS_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L1_FS_length_signature;

	sig->keypair = OQS_SIG_picnic_L1_FS_keypair;
	sig->sign = OQS_SIG_picnic_L1_FS_sign;
	sig->verify = OQS_SIG_picnic_L1_FS_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_FS_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L1_FS, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_FS_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L1_FS, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_FS_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L1_FS, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L1_UR

OQS_SIG *OQS_SIG_picnic_L1_UR_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L1_UR;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L1_UR_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L1_UR_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L1_UR_length_signature;

	sig->keypair = OQS_SIG_picnic_L1_UR_keypair;
	sig->sign = OQS_SIG_picnic_L1_UR_sign;
	sig->verify = OQS_SIG_picnic_L1_UR_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_UR_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L1_UR, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_UR_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L1_UR, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_UR_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L1_UR, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_FS

OQS_SIG *OQS_SIG_picnic_L3_FS_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L3_FS;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L3_FS_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L3_FS_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L3_FS_length_signature;

	sig->keypair = OQS_SIG_picnic_L3_FS_keypair;
	sig->sign = OQS_SIG_picnic_L3_FS_sign;
	sig->verify = OQS_SIG_picnic_L3_FS_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_FS_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L3_FS, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_FS_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L3_FS, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_FS_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L3_FS, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_UR

OQS_SIG *OQS_SIG_picnic_L3_UR_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L3_UR;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L3_UR_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L3_UR_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L3_UR_length_signature;

	sig->keypair = OQS_SIG_picnic_L3_UR_keypair;
	sig->sign = OQS_SIG_picnic_L3_UR_sign;
	sig->verify = OQS_SIG_picnic_L3_UR_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_UR_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L3_UR, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_UR_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L3_UR, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_UR_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L3_UR, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_FS

OQS_SIG *OQS_SIG_picnic_L5_FS_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L5_FS;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L5_FS_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L5_FS_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L5_FS_length_signature;

	sig->keypair = OQS_SIG_picnic_L5_FS_keypair;
	sig->sign = OQS_SIG_picnic_L5_FS_sign;
	sig->verify = OQS_SIG_picnic_L5_FS_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_FS_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L5_FS, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_FS_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L5_FS, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_FS_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L5_FS, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_UR

OQS_SIG *OQS_SIG_picnic_L5_UR_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}

	sig->method_name = OQS_SIG_alg_picnic_L5_UR;
	sig->alg_version = "https://github.com/IAIK/Picnic/commit/423b5da7036ac3b090d50bdff1e9a8ea34e37d11";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L5_UR_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L5_UR_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L5_UR_length_signature;

	sig->keypair = OQS_SIG_picnic_L5_UR_keypair;
	sig->sign = OQS_SIG_picnic_L5_UR_sign;
	sig->verify = OQS_SIG_picnic_L5_UR_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_UR_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L5_UR, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_UR_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L5_UR, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_UR_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L5_UR, public_key, message, message_len, signature, signature_len);
}

#endif
