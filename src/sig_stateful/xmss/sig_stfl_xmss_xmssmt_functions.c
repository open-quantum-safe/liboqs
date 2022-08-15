// SPDX-License-Identifier: MIT

#include <string.h>
#include "sig_stfl_xmss_xmssmt.h"

#include "./external/params.h"
#include "./external/utils.h"
#include "./external/xmss.h"
#include "./external/xmss_namespace.h"

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || message == NULL || signature == NULL) {
		return OQS_ERROR;
	}

	if (xmss_sign(secret_key, signature, (unsigned long long *)signature_length, message, (unsigned long long)message_len) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmss_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (message == NULL || signature == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	if (xmss_verify(message, (unsigned long long *)&message_len, signature, (unsigned long long)signature_len, public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message, size_t message_len, OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL || message == NULL || signature == NULL) {
		return OQS_ERROR;
	}

	if (xmssmt_sign(secret_key, signature, (unsigned long long *)signature_length, message, (unsigned long long)message_len) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_xmssmt_verify(const uint8_t *message, size_t message_len, const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {
	if (message == NULL || signature == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	if (xmssmt_verify(message, (unsigned long long *)&message_len, signature, (unsigned long long)signature_len, public_key) != 0) {
		return OQS_ERROR;
	}
	return OQS_SUCCESS;
}

unsigned long long OQS_SECRET_KEY_xmss_sigs_left(const OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL) {
		return -1;
	}

	xmss_params params;
	unsigned int i;
	uint32_t oid = 0;
	for (i = 0; i < XMSS_OID_LEN; i++) {
		oid |= secret_key->secret_key[XMSS_OID_LEN - i - 1] << (i * 8);
	}
	oqs_sig_stfl_xmss_parse_oid(&params, oid);

	unsigned long long max = OQS_SECRET_KEY_xmss_sigs_total(secret_key);
	unsigned long long idx = oqs_sig_stfl_xmss_bytes_to_ull(secret_key->secret_key + XMSS_OID_LEN, params.index_bytes);
	return (max - idx);
}

unsigned long long OQS_SECRET_KEY_xmss_sigs_total(const OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL) {
		return -1;
	}

	xmss_params params;
	unsigned int i;
	uint32_t oid = 0;
	for (i = 0; i < XMSS_OID_LEN; i++) {
		oid |= secret_key->secret_key[XMSS_OID_LEN - i - 1] << (i * 8);
	}
	oqs_sig_stfl_xmss_parse_oid(&params, oid);

	return oqs_sig_stfl_xmss_bytes_to_ull(secret_key->secret_key + secret_key->length_secret_key - params.bytes_for_max, params.bytes_for_max);
}

unsigned long long OQS_SECRET_KEY_xmssmt_sigs_left(const OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL) {
		return -1;
	}

	xmss_params params;
	unsigned int i;
	uint32_t oid = 0;
	for (i = 0; i < XMSS_OID_LEN; i++) {
		oid |= secret_key->secret_key[XMSS_OID_LEN - i - 1] << (i * 8);
	}
	oqs_sig_stfl_xmssmt_parse_oid(&params, oid);

	unsigned long long max = OQS_SECRET_KEY_xmssmt_sigs_total(secret_key);
	unsigned long long idx = oqs_sig_stfl_xmss_bytes_to_ull(secret_key->secret_key + XMSS_OID_LEN, params.index_bytes);
	return (max - idx);
}


unsigned long long OQS_SECRET_KEY_xmssmt_sigs_total(const OQS_SECRET_KEY *secret_key) {
	if (secret_key == NULL) {
		return -1;
	}

	xmss_params params;
	unsigned int i;
	uint32_t oid = 0;
	for (i = 0; i < XMSS_OID_LEN; i++) {
		oid |= secret_key->secret_key[XMSS_OID_LEN - i - 1] << (i * 8);
	}
	oqs_sig_stfl_xmssmt_parse_oid(&params, oid);

	return oqs_sig_stfl_xmss_bytes_to_ull(secret_key->secret_key + secret_key->length_secret_key - params.bytes_for_max, params.bytes_for_max);
}

OQS_SECRET_KEY *OQS_SIG_STFL_alg_xmss_derive_subkey(OQS_SECRET_KEY *master_key, const unsigned long long number_of_sigs) {

	OQS_SECRET_KEY *subkey = (OQS_SECRET_KEY *)malloc(sizeof(OQS_SECRET_KEY));
	if (subkey == NULL) {
		return NULL;
	}

	// Copy all the essential details of the master key to the subkey.
	subkey->length_secret_key = master_key->length_secret_key;
	subkey->data = master_key->data;
	subkey->sigs_left = OQS_SECRET_KEY_xmss_sigs_left;
	subkey->sigs_total = OQS_SECRET_KEY_xmss_sigs_total;

	subkey->secret_key = (uint8_t *)malloc(subkey->length_secret_key * sizeof(uint8_t));

	// Derive the subkey.
	if (xmss_derive_subkey(master_key, subkey, number_of_sigs) != 0) {
		OQS_SECRET_KEY_free(subkey);
		return NULL;
	}
	return subkey;
}

OQS_SECRET_KEY *OQS_SIG_STFL_alg_xmssmt_derive_subkey(OQS_SECRET_KEY *master_key, const unsigned long long number_of_sigs) {

	OQS_SECRET_KEY *subkey = (OQS_SECRET_KEY *)malloc(sizeof(OQS_SECRET_KEY));
	if (subkey == NULL) {
		return NULL;
	}

	// Copy all the essential details of the master key to the subkey.
	subkey->length_secret_key = master_key->length_secret_key;
	subkey->data = master_key->data;
	subkey->sigs_left = OQS_SECRET_KEY_xmss_sigs_left;
	subkey->sigs_total = OQS_SECRET_KEY_xmss_sigs_total;

	subkey->secret_key = (uint8_t *)malloc(subkey->length_secret_key * sizeof(uint8_t));

	// Derive the subkey.
	if (xmssmt_derive_subkey(master_key, subkey, number_of_sigs) != 0) {
		OQS_SECRET_KEY_free(subkey);
		return NULL;
	}
	return subkey;
}
