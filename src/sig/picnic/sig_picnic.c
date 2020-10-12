// SPDX-License-Identifier: MIT

#include <oqs/sig.h>

#if defined(OQS_SIG_alg_picnic_L1_UR) || defined(OQS_SIG_alg_picnic_L1_FS) || defined(OQS_SIG_alg_picnic_L1_full) || defined(OQS_SIG_alg_picnic_L3_UR) || defined(OQS_SIG_alg_picnic_L3_FS) || defined(OQS_SIG_alg_picnic_L3_full) || defined(OQS_SIG_alg_picnic_L5_UR) || defined(OQS_SIG_alg_picnic_L5_FS) || defined(OQS_SIG_alg_picnic_L5_full) || defined(OQS_SIG_alg_picnic3_L1) || defined(OQS_SIG_alg_picnic3_L3) || defined(OQS_SIG_alg_picnic3_L5)

#include <string.h>
#include <oqs/common.h>
#include "sig_picnic.h"
#include "oqs_picnic_macros.h"
#include "external/picnic.h"

static size_t PRIV_KEY_LEN[] = {
	0,
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L1_FS),
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L1_UR),
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L3_FS),
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L3_UR),
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L5_FS),
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L5_UR),
	PICNIC_PRIVATE_KEY_SIZE(Picnic3_L1),
	PICNIC_PRIVATE_KEY_SIZE(Picnic3_L3),
	PICNIC_PRIVATE_KEY_SIZE(Picnic3_L5),
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L1_full),
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L3_full),
	PICNIC_PRIVATE_KEY_SIZE(Picnic_L5_full),
};
static size_t PUB_KEY_LEN[] = {
	0,
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L1_FS),
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L1_UR),
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L3_FS),
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L3_UR),
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L5_FS),
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L5_UR),
	PICNIC_PUBLIC_KEY_SIZE(Picnic3_L1),
	PICNIC_PUBLIC_KEY_SIZE(Picnic3_L3),
	PICNIC_PUBLIC_KEY_SIZE(Picnic3_L5),
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L1_full),
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L3_full),
	PICNIC_PUBLIC_KEY_SIZE(Picnic_L5_full),
};
static size_t SIG_LEN[] = {
	0,
	PICNIC_SIGNATURE_SIZE(Picnic_L1_FS),
	PICNIC_SIGNATURE_SIZE(Picnic_L1_UR),
	PICNIC_SIGNATURE_SIZE(Picnic_L3_FS),
	PICNIC_SIGNATURE_SIZE(Picnic_L3_UR),
	PICNIC_SIGNATURE_SIZE(Picnic_L5_FS),
	PICNIC_SIGNATURE_SIZE(Picnic_L5_UR),
	PICNIC_SIGNATURE_SIZE(Picnic3_L1),
	PICNIC_SIGNATURE_SIZE(Picnic3_L3),
	PICNIC_SIGNATURE_SIZE(Picnic3_L5),
	PICNIC_SIGNATURE_SIZE(Picnic_L1_full),
	PICNIC_SIGNATURE_SIZE(Picnic_L3_full),
	PICNIC_SIGNATURE_SIZE(Picnic_L5_full),
};

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
	// picnic3's signature code checks that the sig_len value is large enough, but the OQS
	// API treats this as an output parameters, so we set it here
	*sig_len = SIG_LEN[parameters];
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
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

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
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

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

#ifdef OQS_ENABLE_SIG_picnic_L1_full

OQS_SIG *OQS_SIG_picnic_L1_full_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L1_full;
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L1_full_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L1_full_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L1_full_length_signature;

	sig->keypair = OQS_SIG_picnic_L1_full_keypair;
	sig->sign = OQS_SIG_picnic_L1_full_sign;
	sig->verify = OQS_SIG_picnic_L1_full_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_full_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L1_full, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_full_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L1_full, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L1_full_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L1_full, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L3_FS

OQS_SIG *OQS_SIG_picnic_L3_FS_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L3_FS;
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

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
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

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

#ifdef OQS_ENABLE_SIG_picnic_L3_full

OQS_SIG *OQS_SIG_picnic_L3_full_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L3_full;
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L3_full_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L3_full_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L3_full_length_signature;

	sig->keypair = OQS_SIG_picnic_L3_full_keypair;
	sig->sign = OQS_SIG_picnic_L3_full_sign;
	sig->verify = OQS_SIG_picnic_L3_full_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_full_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L3_full, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_full_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L3_full, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L3_full_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L3_full, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic_L5_FS

OQS_SIG *OQS_SIG_picnic_L5_FS_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L5_FS;
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

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
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

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

#ifdef OQS_ENABLE_SIG_picnic_L5_full

OQS_SIG *OQS_SIG_picnic_L5_full_new() {

	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic_L5_full;
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic_L5_full_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic_L5_full_length_secret_key;
	sig->length_signature = OQS_SIG_picnic_L5_full_length_signature;

	sig->keypair = OQS_SIG_picnic_L5_full_keypair;
	sig->sign = OQS_SIG_picnic_L5_full_sign;
	sig->verify = OQS_SIG_picnic_L5_full_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_full_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic_L5_full, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_full_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic_L5_full, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic_L5_full_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic_L5_full, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic3_L1
OQS_SIG *OQS_SIG_picnic3_L1_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic3_L1;
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

	sig->claimed_nist_level = 1;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic3_L1_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic3_L1_length_secret_key;
	sig->length_signature = OQS_SIG_picnic3_L1_length_signature;

	sig->keypair = OQS_SIG_picnic3_L1_keypair;
	sig->sign = OQS_SIG_picnic3_L1_sign;
	sig->verify = OQS_SIG_picnic3_L1_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L1_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic3_L1, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L1_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic3_L1, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L1_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic3_L1, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic3_L3

OQS_SIG *OQS_SIG_picnic3_L3_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic3_L3;
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

	sig->claimed_nist_level = 3;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic3_L3_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic3_L3_length_secret_key;
	sig->length_signature = OQS_SIG_picnic3_L3_length_signature;

	sig->keypair = OQS_SIG_picnic3_L3_keypair;
	sig->sign = OQS_SIG_picnic3_L3_sign;
	sig->verify = OQS_SIG_picnic3_L3_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L3_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic3_L3, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L3_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic3_L3, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L3_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic3_L3, public_key, message, message_len, signature, signature_len);
}

#endif

#ifdef OQS_ENABLE_SIG_picnic3_L5

OQS_SIG *OQS_SIG_picnic3_L5_new() {
	OQS_SIG *sig = malloc(sizeof(OQS_SIG));
	if (sig == NULL) {
		return NULL;
	}
	sig->method_name = OQS_SIG_alg_picnic3_L5;
	sig->alg_version = "https://github.com/IAIK/Picnic/tree/v3.0.3";

	sig->claimed_nist_level = 5;
	sig->euf_cma = true;

	sig->length_public_key = OQS_SIG_picnic3_L5_length_public_key;
	sig->length_secret_key = OQS_SIG_picnic3_L5_length_secret_key;
	sig->length_signature = OQS_SIG_picnic3_L5_length_signature;

	sig->keypair = OQS_SIG_picnic3_L5_keypair;
	sig->sign = OQS_SIG_picnic3_L5_sign;
	sig->verify = OQS_SIG_picnic3_L5_verify;

	return sig;
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L5_keypair(uint8_t *public_key, uint8_t *secret_key) {
	return common_picnic_keypair(Picnic3_L5, secret_key, public_key);
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L5_sign(uint8_t *signature, size_t *signature_len, const uint8_t *message, size_t message_len, const uint8_t *secret_key) {
	return common_picnic_sign(Picnic3_L5, secret_key, message, message_len, signature, signature_len);
}

OQS_API OQS_STATUS OQS_SIG_picnic3_L5_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	return common_picnic_verify(Picnic3_L5, public_key, message, message_len, signature, signature_len);
}

#endif
