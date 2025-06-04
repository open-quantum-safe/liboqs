// SPDX-License-Identifier: MIT

#include <string.h>
#include <oqs/oqs.h>
#include "sig_stfl_lms.h"
#include "external/config.h"
#include "external/hss_verify_inc.h"
#include "external/hss_sign_inc.h"
#include "external/hss.h"
#include "external/endian.h"
#include "external/hss_internal.h"
#include "sig_stfl_lms_wrap.h"

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#define DEFAULT_AUX_DATA 10916   /* Use 10+k of aux data (which works well */
/* with the above default parameter set)  */
/**
 * @brief OQS_LMS_KEY object for HSS key pair
 */

typedef struct OQS_LMS_KEY_DATA {

	/* Tree levels. */
	uint32_t levels;

	/* Array, 8 levels max, of LMS types */
	param_set_t lm_type[8];

	/* Array, 8 levels max, of LM OTS types */
	param_set_t lm_ots_type[8];

	/* LMS public key */
	uint8_t public_key[60];

	/* Length of aux data */
	size_t len_aux_data;
	/* internal nodes info of the Merkle tree */
	uint8_t *aux_data;

	/* Length of sec_key */
	size_t len_sec_key;

	/* secret key data */
	uint8_t *sec_key;

	/* app specific */
	void *context;
} oqs_lms_key_data;

#ifndef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(UNUSED uint8_t *signature, UNUSED size_t *signature_length, UNUSED const uint8_t *message,
        UNUSED size_t message_len, UNUSED OQS_SIG_STFL_SECRET_KEY *secret_key) {
	return OQS_ERROR;
}
#else
OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message,
        size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key) {
	OQS_STATUS status = OQS_ERROR;
	OQS_STATUS rc_keyupdate = OQS_ERROR;
	oqs_lms_key_data *lms_key_data = NULL;
	uint8_t *sk_key_buf = NULL;
	size_t sk_key_buf_len = 0;
	void *context;

	if (secret_key == NULL || message == NULL || signature == NULL || signature_length == NULL) {
		return OQS_ERROR;
	}

	/* Lock secret to ensure OTS use */
	if ((secret_key->lock_key) && (secret_key->mutex)) {
		secret_key->lock_key(secret_key->mutex);
	}

	/*
	 * Don't even attempt signing without a way to safe the updated private key
	 */
	if (secret_key->secure_store_scrt_key == NULL) {
		fprintf(stderr, "No Secure-store set for secret key.\n.");
		goto err;
	}

	lms_key_data = (oqs_lms_key_data *)secret_key->secret_key_data;
	if (lms_key_data == NULL) {
		goto err;
	}

	if (oqs_sig_stfl_lms_sign(secret_key, signature,
	                          signature_length,
	                          message, message_len) != 0) {
		goto err;
	}

	/*
	 * serialize and securely store the updated private key
	 * but, delete signature and the serialized key other wise
	 */

	rc_keyupdate = oqs_serialize_lms_key(&sk_key_buf, &sk_key_buf_len, secret_key);
	if (rc_keyupdate != OQS_SUCCESS) {
		goto err;
	}

	context = secret_key->context;
	rc_keyupdate = secret_key->secure_store_scrt_key(sk_key_buf, sk_key_buf_len, context);
	if (rc_keyupdate != OQS_SUCCESS) {
		goto err;
	}

	status = OQS_SUCCESS;
	goto passed;

err:
	if (*signature_length) {
		OQS_MEM_cleanse(signature, *signature_length);
	}
	*signature_length = 0;

passed:
	OQS_MEM_secure_free(sk_key_buf, sk_key_buf_len);

	/* Unlock secret to ensure OTS use */
	if ((secret_key->unlock_key) && (secret_key->mutex)) {
		secret_key->unlock_key(secret_key->mutex);
	}
	return status;
}
#endif

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_verify(const uint8_t *message, size_t message_len,
        const uint8_t *signature, size_t signature_len, const uint8_t *public_key) {

	if (message == NULL || signature == NULL || public_key == NULL) {
		return OQS_ERROR;
	}

	if (oqs_sig_stfl_lms_verify(message, message_len,
	                            signature, signature_len,
	                            public_key) != 0 ) {
		return OQS_ERROR;
	}

	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_left(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key) {
	OQS_STATUS status;
	uint8_t *priv_key = NULL;
	unsigned long long total_sigs = 0;
	sequence_t current_count = 0;
	oqs_lms_key_data *oqs_key_data = NULL;

	if (remain == NULL  || secret_key == NULL) {
		return OQS_ERROR;
	}

	status = OQS_SIG_STFL_lms_sigs_total(&total_sigs, secret_key);
	if (status != OQS_SUCCESS) {
		return OQS_ERROR;
	}

	/* Lock secret key to ensure data integrity use */
	if ((secret_key->lock_key) && (secret_key->mutex)) {
		secret_key->lock_key(secret_key->mutex);
	}

	oqs_key_data = secret_key->secret_key_data;
	if (oqs_key_data == NULL) {
		goto err;
	}
	priv_key = oqs_key_data->sec_key;
	if (priv_key == NULL) {
		goto err;
	}

	current_count = get_bigendian(priv_key + PRIVATE_KEY_INDEX, PRIVATE_KEY_INDEX_LEN /*0, 8 */);
	*remain =  (total_sigs - (unsigned long long)current_count);

err:
	/* Unlock secret key */
	if ((secret_key->unlock_key) && (secret_key->mutex)) {
		secret_key->unlock_key(secret_key->mutex);
	}
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_total(unsigned long long *total, const OQS_SIG_STFL_SECRET_KEY *secret_key) {

	uint8_t *priv_key = NULL;
	oqs_lms_key_data *oqs_key_data = NULL;
	struct hss_working_key *working_key = NULL;

	if (total == NULL  || secret_key == NULL) {
		return OQS_ERROR;
	}

	oqs_key_data = secret_key->secret_key_data;
	if (!oqs_key_data) {
		return OQS_ERROR;
	}

	priv_key = oqs_key_data->sec_key;
	if (!priv_key) {
		return OQS_ERROR;
	}

	working_key = hss_load_private_key(NULL, priv_key,
	                                   0,
	                                   NULL,
	                                   0,
	                                   0);
	if (!working_key) {
		return OQS_ERROR;
	}

	*total = (unsigned long long)working_key->max_count;
	hss_free_working_key(working_key);
	return OQS_SUCCESS;
}

/* LMS wrapper functions use internal OIDs to
 * identify the parameter set to be used
 */

bool LMS_randombytes(void *buffer, size_t length) {

	OQS_randombytes((uint8_t *)buffer, length);
	return true;
}

#ifndef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
int oqs_sig_stfl_lms_keypair(UNUSED uint8_t *pk, UNUSED OQS_SIG_STFL_SECRET_KEY *sk, UNUSED const uint32_t oid) {
	return -1;
}
#else
int oqs_sig_stfl_lms_keypair(uint8_t *pk, OQS_SIG_STFL_SECRET_KEY *sk, const uint32_t oid) {

	int ret = -1;
	bool b_ret;
	int parse_err = 0;

	size_t len_public_key = 60;
	oqs_lms_key_data *oqs_key_data = NULL;

	if (!pk || !sk || !oid) {
		return -1;
	}

	if (sk->secret_key_data) {
		//this means a key pair has already been recreated
		return -1;
	}

	oqs_key_data = OQS_MEM_malloc(sizeof(oqs_lms_key_data));
	if (oqs_key_data == NULL) {
		return -1;
	}

	OQS_MEM_cleanse(oqs_key_data, sizeof(oqs_lms_key_data));
	if (sk->length_secret_key == 0) {
		OQS_MEM_insecure_free(oqs_key_data);
		oqs_key_data = NULL;
		return -1;
	}

	oqs_key_data->levels = 1;
	oqs_key_data->len_sec_key = sk->length_secret_key;
	oqs_key_data->sec_key = (uint8_t *)OQS_MEM_malloc(sk->length_secret_key * sizeof(uint8_t));
	if (oqs_key_data->sec_key == NULL) {
		OQS_MEM_insecure_free(oqs_key_data);
		oqs_key_data = NULL;
		return -1;
	}

	OQS_MEM_cleanse(oqs_key_data->sec_key, sk->length_secret_key);

	//Aux Data
	size_t len_aux_data = DEFAULT_AUX_DATA;
	uint8_t *aux_data =  OQS_MEM_calloc(len_aux_data, sizeof(uint8_t));
	if (aux_data == NULL) {
		OQS_MEM_insecure_free( oqs_key_data->sec_key);
		OQS_MEM_insecure_free(oqs_key_data);
		return -1;
	}

	oqs_key_data->aux_data = aux_data;
	oqs_key_data->len_aux_data = len_aux_data;
	oqs_key_data->context = sk->context;

	/* Set lms param set */
	switch (oid) {
	case OQS_LMS_ID_sha256_h5_w1:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W1;
		break;
	case OQS_LMS_ID_sha256_h5_w2:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W2;
		break;
	case OQS_LMS_ID_sha256_h5_w4:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W4;
		break;
	case OQS_LMS_ID_sha256_h5_w8:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		break;

	case OQS_LMS_ID_sha256_h10_w1:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W1;
		break;
	case OQS_LMS_ID_sha256_h10_w2:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W2;
		break;
	case OQS_LMS_ID_sha256_h10_w4:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W4;
		break;
	case OQS_LMS_ID_sha256_h10_w8:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		break;

	case OQS_LMS_ID_sha256_h15_w1:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W1;
		break;
	case OQS_LMS_ID_sha256_h15_w2:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W2;
		break;
	case OQS_LMS_ID_sha256_h15_w4:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W4;
		break;
	case OQS_LMS_ID_sha256_h15_w8:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		break;

	case OQS_LMS_ID_sha256_h20_w1:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W1;
		break;
	case OQS_LMS_ID_sha256_h20_w2:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W2;
		break;
	case OQS_LMS_ID_sha256_h20_w4:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W4;
		break;
	case OQS_LMS_ID_sha256_h20_w8:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		break;

	case OQS_LMS_ID_sha256_h25_w1:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H25;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W1;
		break;
	case OQS_LMS_ID_sha256_h25_w2:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H25;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W2;
		break;
	case OQS_LMS_ID_sha256_h25_w4:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H25;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W4;
		break;
	case OQS_LMS_ID_sha256_h25_w8:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H25;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h5_w8_h5_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h10_w8_h5_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h10_w2_h10_w2:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W2;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W2;
		break;
	case OQS_LMS_ID_sha256_h10_w4_h5_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W4;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h10_w4_h10_w4:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W4;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W4;
		break;
	case OQS_LMS_ID_sha256_h10_w8_h10_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h15_w8_h5_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h15_w8_h10_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h15_w8_h15_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h20_w8_h5_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h20_w8_h10_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H10;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h20_w8_h15_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H15;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	case OQS_LMS_ID_sha256_h20_w8_h20_w8:
		oqs_key_data->levels = 2;
		oqs_key_data->lm_type[0]     = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W8;
		oqs_key_data->lm_type[1]     = LMS_SHA256_N32_H20;
		oqs_key_data->lm_ots_type[1] = LMOTS_SHA256_N32_W8;
		break;
	default:
		oqs_key_data->lm_type[0] = 0;
		oqs_key_data->lm_ots_type[0] = 0;
		parse_err = 1;
		break;
	}

	if (parse_err) {
		OQS_MEM_insecure_free(oqs_key_data->sec_key);
		OQS_MEM_insecure_free(oqs_key_data->aux_data);
		OQS_MEM_insecure_free(oqs_key_data);
		oqs_key_data = NULL;
		return -1;
	}

	/*
	 * This creates a private key (and the correspond public key, and optionally
	 * the aux data for that key)
	 * Parameters:
	 * generate_random - the function to be called to generate randomness.  This
	 *       is assumed to be a pointer to a cryptographically secure rng,
	 *       otherwise all security is lost.  This function is expected to fill
	 *       output with 'length' uniformly distributed bits, and return 1 on
	 *       success, 0 if something went wrong
	 * levels - the number of levels for the key pair (2-8)
	 * lm_type - an array of the LM registry entries for the various levels;
	 *      entry 0 is the topmost
	 * lm_ots_type - an array of the LM-OTS registry entries for the various
	 *      levels; again, entry 0 is the topmost
	 * update_private_key, context - the function that is called when the
	 *      private key is generated; it is expected to store it to secure NVRAM
	 *      If this is NULL, then the context pointer is reinterpretted to mean
	 *      where in RAM the private key is expected to be placed
	 * public_key - where to store the public key
	 * len_public_key - length of the above buffer; see hss_get_public_key_len
	 *      if you need a hint.
	 * aux_data - where to store the optional aux data.  This is not required, but
	 *      if provided, can be used to speed up the hss_generate_working_key
	 *      process;
	 * len_aux_data - the length of the above buffer.  This is not fixed length;
	 *      the function will run different time/memory trade-offs based on the
	 *      length provided
	 *
	 * This returns true on success, false on failure
	 */
	b_ret = hss_generate_private_key(
	            LMS_randombytes,
	            oqs_key_data->levels,
	            oqs_key_data->lm_type,
	            oqs_key_data->lm_ots_type,
	            NULL, //File handler function?
	            oqs_key_data->sec_key,
	            oqs_key_data->public_key, len_public_key,
	            oqs_key_data->aux_data, oqs_key_data->len_aux_data,
	            NULL);
	if (b_ret) {
		memcpy(pk, oqs_key_data->public_key, len_public_key);
		sk->secret_key_data = oqs_key_data;
	} else {
		OQS_MEM_secure_free(oqs_key_data->sec_key, sk->length_secret_key * sizeof(uint8_t));
		OQS_MEM_insecure_free(oqs_key_data->aux_data);
		OQS_MEM_insecure_free(oqs_key_data);
		oqs_key_data = NULL;
		return -1;
	}

	ret = 0;
	return ret;
}
#endif

#ifndef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
int oqs_sig_stfl_lms_sign(UNUSED OQS_SIG_STFL_SECRET_KEY *sk, UNUSED uint8_t *signature, UNUSED size_t *signature_len,
                          UNUSED const uint8_t *m, UNUSED size_t mlen) {
	return -1;
}
#else
int oqs_sig_stfl_lms_sign(OQS_SIG_STFL_SECRET_KEY *sk,
                          uint8_t *signature, size_t *signature_len,
                          const uint8_t *m, size_t mlen) {

	size_t sig_len;
	bool status;
	uint8_t *sig = NULL;
	uint8_t *priv_key = NULL;
	oqs_lms_key_data *oqs_key_data = NULL;
	struct hss_working_key *w = NULL;
	struct hss_sign_inc ctx;
	if (sk) {
		oqs_key_data = sk->secret_key_data;
		priv_key = oqs_key_data->sec_key;
	} else {
		return -1;
	}
	w = hss_load_private_key(NULL, priv_key,
	                         0,
	                         NULL,
	                         0,
	                         0);
	if (!w) {
		hss_free_working_key(w);
		return 0;
	}

	/* Now, go through the file list, and generate the signatures for each */

	/* Look up the signature length */

	sig_len = hss_get_signature_len_from_working_key(w);
	if (sig_len == 0) {
		hss_free_working_key(w);
		return 0;
	}

	sig = OQS_MEM_malloc(sig_len);
	if (!sig) {
		hss_free_working_key(w);
		return -1;
	}

	(void)hss_sign_init(
	    &ctx,                 /* Incremental signing context */
	    w,                    /* Working key */
	    NULL,                 /* Routine to update the */
	    priv_key,       /* private key */
	    sig, sig_len,         /* Where to place the signature */
	    0);

	(void)hss_sign_update(
	    &ctx,           /* Incremental signing context */
	    m,         /* Next piece of the message */
	    mlen);             /* Length of this piece */

	status = hss_sign_finalize(
	             &ctx,               /* Incremental signing context */
	             w,                  /* Working key */
	             sig,                /* Signature */
	             0);

	if (!status) {
		hss_free_working_key(w);
		OQS_MEM_insecure_free(sig);
		return -1;
	}

	*signature_len = sig_len;
	memcpy(signature, sig, sig_len);
	OQS_MEM_insecure_free(sig);
	hss_free_working_key(w);

	return 0;
}
#endif

int oqs_sig_stfl_lms_verify(const uint8_t *m, size_t mlen,
                            const uint8_t *signature, size_t signature_len,
                            const uint8_t *pk) {

	struct hss_validate_inc ctx;
	(void)hss_validate_signature_init(
	    &ctx,               /* Incremental validate context */
	    (const unsigned char *)pk,                /* Public key */
	    (const unsigned char *)signature,
	    (size_t)signature_len,       /* Signature */
	    0);                 /* Use the defaults for extra info */

	(void)hss_validate_signature_update(
	    &ctx,           /* Incremental validate context */
	    (const void *) m,        /* Next piece of the message */
	    (size_t)mlen);             /* Length of this piece */

	bool status = hss_validate_signature_finalize(
	                  &ctx,               /* Incremental validate context */
	                  (const unsigned char *)signature,                /* Signature */
	                  0);                 /* Use the defaults for extra info */

	if (status) {
		/* Signature verified */
		return 0;
	} else {
		/* signature NOT verified */
		return -1;
	}
}

void oqs_secret_lms_key_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return;
	}

	if (sk->secret_key_data) {
		oqs_lms_key_data *key_data = (oqs_lms_key_data *)sk->secret_key_data;
		if (key_data != NULL) {
			OQS_MEM_secure_free(key_data->sec_key, key_data->len_sec_key);
			key_data->sec_key = NULL;

			OQS_MEM_secure_free(key_data->aux_data, key_data->len_aux_data);
		}

		OQS_MEM_insecure_free(key_data);
		sk->secret_key_data = NULL;
	}
}

/*
 * Convert LMS secret key object to byte string
 * Writes secret key + aux data if present
 */
OQS_STATUS oqs_serialize_lms_key(uint8_t **sk_key, size_t *sk_len, const OQS_SIG_STFL_SECRET_KEY *sk) {

	if (sk == NULL || sk_len == NULL || sk_key == NULL) {
		return OQS_ERROR;
	}

	oqs_lms_key_data *lms_key_data = sk->secret_key_data;

	if (lms_key_data == NULL || lms_key_data->sec_key == NULL) {
		return OQS_ERROR;
	}

	size_t key_len = lms_key_data->len_aux_data + lms_key_data->len_sec_key;

	if (key_len == 0) {
		return OQS_ERROR;
	}

	uint8_t *sk_key_buf = OQS_MEM_malloc(key_len * sizeof(uint8_t));

	if (sk_key_buf == NULL) {
		return OQS_ERROR;
	}
	/* pass back serialized data */
	/*
	 * Serialized data is sec_key followed by aux data
	 * So aux data begins after buffer top + sec_key length
	 */
	if (lms_key_data->len_sec_key != 0) {
		memcpy(sk_key_buf, lms_key_data->sec_key, lms_key_data->len_sec_key);
	}

	if (lms_key_data->len_aux_data != 0) {
		memcpy(sk_key_buf + lms_key_data->len_sec_key, lms_key_data->aux_data, lms_key_data->len_aux_data);
	}

	*sk_key = sk_key_buf;
	*sk_len = sk->length_secret_key + lms_key_data->len_aux_data;

	return OQS_SUCCESS;
}

/*
 * Convert LMS byte string to secret key object
 * Writes secret key + aux data if present
 * key_len is priv key length + aux length
 */
OQS_STATUS oqs_deserialize_lms_key(OQS_SIG_STFL_SECRET_KEY *sk, const uint8_t *sk_buf, const size_t sk_len, void *context) {

	oqs_lms_key_data *lms_key_data = NULL;
	uint8_t *lms_sk = NULL;
	uint8_t *lms_aux = NULL;
	size_t aux_buf_len = 0;
	size_t lms_sk_len = hss_get_private_key_len((unsigned )(1), NULL, NULL);

	if (sk == NULL || sk_buf == NULL || (sk_len == 0) || (sk_len < lms_sk_len )) {
		return OQS_ERROR;
	}

#ifndef OQS_ALLOW_LMS_KEY_AND_SIG_GEN
	return OQS_ERROR;
#endif

	aux_buf_len = sk_len - lms_sk_len;
	if (sk->secret_key_data) {
		// Key data already present
		// We dont want to trample over data
		return OQS_ERROR;
	}

	unsigned levels = 0;

	param_set_t lm_type[ MAX_HSS_LEVELS ];
	param_set_t lm_ots_type[ MAX_HSS_LEVELS ];

	// validate sk_buf for lms params
	if (!hss_get_parameter_set(&levels,
	                           lm_type,
	                           lm_ots_type,
	                           NULL,
	                           (void *)sk_buf)) {
		return OQS_ERROR;
	}

	lms_key_data = OQS_MEM_malloc(sizeof(oqs_lms_key_data));
	lms_sk = OQS_MEM_malloc(lms_sk_len * sizeof(uint8_t));

	if (lms_key_data == NULL || lms_sk == NULL) {
		goto err;
	}

	memcpy(lms_sk, sk_buf, lms_sk_len);
	lms_key_data->sec_key = lms_sk;
	lms_key_data->len_sec_key = lms_sk_len;
	lms_key_data->context = context;

	if (aux_buf_len) {
		lms_aux = OQS_MEM_malloc(aux_buf_len * sizeof(uint8_t));

		if (lms_aux == NULL) {
			goto err;
		}

		memcpy(lms_aux, sk_buf + lms_sk_len, aux_buf_len);
		lms_key_data->aux_data = lms_aux;
		lms_key_data->len_aux_data = aux_buf_len;
	}

	sk->context = context;
	sk->secret_key_data = lms_key_data;
	goto success;

err:
	OQS_MEM_secure_free(lms_key_data, sizeof(oqs_lms_key_data));
	OQS_MEM_secure_free(lms_sk, lms_sk_len);
	OQS_MEM_secure_free(lms_aux, aux_buf_len);
	return OQS_ERROR;

success:
	return OQS_SUCCESS;
}

void oqs_lms_key_set_store_cb(OQS_SIG_STFL_SECRET_KEY *sk, secure_store_sk store_cb, void *context) {

	if (sk == NULL) {
		return;
	}
	sk->secure_store_scrt_key = store_cb;
	sk->context = context;
}
