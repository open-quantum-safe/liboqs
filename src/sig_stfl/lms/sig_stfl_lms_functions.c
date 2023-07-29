// SPDX-License-Identifier: MIT

#include <string.h>
#include "sig_stfl_lms.h"
#include "external/config.h"
#include "external/hss_verify_inc.h"
#include "external/hss_sign_inc.h"
#include "external/hss.h"
#include "sig_stfl_lms_wrap.h"
#include <stdio.h>

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message,
        size_t message_len, uint8_t *secret_key) {

	if (secret_key == NULL || message == NULL || signature == NULL) {
		return OQS_ERROR;
	}

	/* TODO: Make sure we have a way to update the private key */

	if (oqs_sig_stfl_lms_sign(secret_key, signature,
	                          signature_length,
	                          message, message_len) != 0) {
		return OQS_ERROR;
	}

	/* TODO: Update private key */
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_verify(const uint8_t *message, size_t message_len,
        const uint8_t *signature, size_t signature_len,
        const uint8_t *public_key) {

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

OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_left(unsigned long long *remain, const uint8_t *secret_key) {

	if (remain == NULL  || secret_key == NULL) {
		return OQS_ERROR;
	}

	remain = 0;
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_total(uint64_t *total, const uint8_t *secret_key) {

	if (total == NULL  || secret_key == NULL) {
		return OQS_ERROR;
	}

	total = 0;
	return OQS_SUCCESS;
}

/* LMS wrapper functions use internal OIDs to
 * identify the parameter set to be used
 */

bool LMS_randombytes(void *buffer, size_t length) {

	OQS_randombytes((uint8_t *)buffer, length);
	return true;
}

int oqs_sig_stfl_lms_keypair(uint8_t *pk, uint8_t *sk, const uint32_t oid) {

	int ret = -1;
	bool b_ret;
	int parse_err = 0;
	unsigned levels = 1;

	unsigned char public_key[60];
	size_t len_public_key = 60;
	unsigned char *aux_data = NULL;
	size_t max_aux_data = 10916;
	int aux_len = 0;
	oqs_lms_key_data *oqs_data = NULL;

	param_set_t lm_type[1];
	param_set_t lm_ots_type[1];

	if (!pk || !sk || !oid) {
		return -1;
	}

	/* Set lms param set */
	switch (oid) {
	case 0x1:
		lm_type[0] = LMS_SHA256_N32_H5;
		lm_ots_type[0] = LMOTS_SHA256_N32_W1;
		break;
	default:
		lm_type[0] = 0;
		lm_ots_type[0] = 0;
		parse_err = 1;
		break;
	}

	if (parse_err) {
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
	            levels,
	            lm_type,
	            lm_ots_type,
	            NULL, //File handler function?
	            (void *)sk,
	            public_key, len_public_key,
	            aux_data, aux_len,
	            NULL);
	if (b_ret) {
		memcpy(pk, public_key, len_public_key);
	}

	/* TODO: store key pair, file handler */

	ret = 0;
	return ret;
}

int oqs_sig_stfl_lms_sign(uint8_t *sk,
                          uint8_t *sm, size_t *smlen,
                          const uint8_t *m, size_t mlen) {

	size_t sig_len;
	bool status;
	unsigned char *sig = NULL;
	struct hss_working_key *w = NULL;
	struct hss_sign_inc ctx;
	w = hss_load_private_key(NULL, sk,
	                         0,
	                         NULL,
	                         0,
	                         0);
	if (!w) {
		printf( "Error loading private key\n" );
		hss_free_working_key(w);
		return 0;
	}

	/* Now, go through the file list, and generate the signatures for each */

	/* Look up the signature length */

	sig_len = hss_get_signature_len_from_working_key(w);
	if (sig_len == 0) {
		printf( "Error getting signature len\n" );
		hss_free_working_key(w);
		return 0;
	}

	sig = malloc(sig_len);
	if (!sig) {
		printf( "Error during malloc\n" );
		hss_free_working_key(w);
		return -1;
	}

	(void)hss_sign_init(
	    &ctx,                 /* Incremental signing context */
	    w,                    /* Working key */
	    NULL,                 /* Routine to update the */
	    sk,       /* private key */
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

	*smlen = sig_len;
	memcpy(sm, sig, sig_len);
	OQS_MEM_insecure_free(sig);

	return 0;
}

int oqs_sig_stfl_lms_verify(const uint8_t *m, size_t mlen,
                            const uint8_t *sm, size_t smlen,
                            const uint8_t *pk) {

	struct hss_validate_inc ctx;
	(void)hss_validate_signature_init(
	    &ctx,               /* Incremental validate context */
	    (const unsigned char *)pk,                /* Public key */
	    (const unsigned char *)sm,
	    (size_t)smlen,       /* Signature */
	    0);                 /* Use the defaults for extra info */

	(void)hss_validate_signature_update(
	    &ctx,           /* Incremental validate context */
	    (const void *) m,        /* Next piece of the message */
	    (size_t)mlen);             /* Length of this piece */

	bool status = hss_validate_signature_finalize(
	                  &ctx,               /* Incremental validate context */
	                  (const unsigned char *)sm,                /* Signature */
	                  0);                 /* Use the defaults for extra info */

	if (status) {
		/* Signature verified */
		return 0;
	} else {
		/* signature NOT verified */
		return -1;
	}
}

