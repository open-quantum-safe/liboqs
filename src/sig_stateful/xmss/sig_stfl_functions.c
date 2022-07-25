#include <string.h>
#include "sig_stfl_xmss.h"

#include "./external/params.h"
#include "./external/xmss.h"

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || message == NULL || signature == NULL) {
		return -1;
	}

	if (xmss_sign(secret_key, signature, (unsigned long long *)signature_length, message, (unsigned long long)message_len) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (message == NULL || signature == NULL || public_key == NULL) {
		return -1;
	}

	if (xmss_sign_open(message, (unsigned long long *)&message_len, signature, (unsigned long long)signature_len, public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || message == NULL || signature == NULL) {
		return -1;
	}

	if (xmssmt_sign(secret_key, signature, (unsigned long long *)signature_length, message, (unsigned long long)message_len) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (message == NULL || signature == NULL || public_key == NULL) {
		return -1;
	}

	if (xmssmt_sign_open(message, (unsigned long long *)&message_len, signature, (unsigned long long)signature_len, public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

unsigned long long OQS_SIG_STFL_alg_xmss_xmssmt_sigs_left(const OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL) {
		return -1;
	}

	xmss_params params;
	unsigned int i;
	uint32_t oid = 0;
	for (i = 0; i < XMSS_OID_LEN; i++) {
		oid |= secret_key->secret_key[XMSS_OID_LEN - i - 1] << (i * 8);
	}
	xmss_parse_oid(&params, oid);

	unsigned long long max = OQS_SIG_STFL_alg_xmss_xmssmt_sigs_total(secret_key);
	unsigned long long idx = 0;
	for (i = 0; i < params.index_bytes; i++) {
		idx |= ((unsigned long long)secret_key->secret_key[i]) << 8 * (params.index_bytes - 1 - i);
	}
	return (max - idx);
}

unsigned long long OQS_SIG_STFL_alg_xmss_xmssmt_sigs_total(const OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL) {
		return -1;
	}

	xmss_params params;
	unsigned int i;
	uint32_t oid = 0;
	for (i = 0; i < XMSS_OID_LEN; i++) {
		oid |= secret_key->secret_key[XMSS_OID_LEN - i - 1] << (i * 8);
	}
	xmss_parse_oid(&params, oid);

	unsigned long long max = 0;
	for (unsigned int j = params.bytes_for_max; j > 0; j--) {
		max |= ((unsigned long long)secret_key->secret_key[params.sk_bytes - XMSS_OID_LEN - j] << 8 * (j - 1));
	}
	return max;
}

void perform_key_allocation(OQS_SECRET_KEY *sk) {

	// Assign the sigs_left and sigs_max functions
	sk->sigs_left = OQS_SIG_STFL_alg_xmss_xmssmt_sigs_left;
	sk->sigs_total = OQS_SIG_STFL_alg_xmss_xmssmt_sigs_total;

	// Initialize the key with length_secret_key amount of bytes.
	sk->secret_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
	memset(sk->secret_key, 0, sk->length_secret_key);
}

OQS_SECRET_KEY *OQS_SECRET_KEY_alg_derive_subkey(OQS_SECRET_KEY *master_key, const unsigned long long number_of_sigs) {

	OQS_SECRET_KEY *subkey = (OQS_SECRET_KEY *)malloc(sizeof(OQS_SECRET_KEY));

	// Copy all the essential details of the master key to the subkey.
	subkey->length_secret_key = master_key->length_secret_key;

	// Allocate the memory for the secret key.
	perform_key_allocation(subkey);

	// Derive the subkey.
	if (xmss_derive_subkey(master_key, subkey, number_of_sigs) != 0) {
		OQS_SECRET_KEY_free(subkey);
		return NULL;
	}
	return subkey;
}