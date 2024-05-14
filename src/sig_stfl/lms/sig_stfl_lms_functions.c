// SPDX-License-Identifier: MIT

#include <string.h>
#include "sig_stfl_lms.h"
#include "external/config.h"
#include "external/hss_verify_inc.h"
#include "external/hss_sign_inc.h"
#include "external/hss.h"
#include "sig_stfl_lms_wrap.h"
#include <stdio.h>

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
} oqs_lms_key_data;

OQS_API OQS_STATUS OQS_SIG_STFL_alg_lms_sign(uint8_t *signature, size_t *signature_length, const uint8_t *message,
        size_t message_len, OQS_SIG_STFL_SECRET_KEY *secret_key) {

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

OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_left(unsigned long long *remain, const OQS_SIG_STFL_SECRET_KEY *secret_key) {

	if (remain == NULL  || secret_key == NULL) {
		return OQS_ERROR;
	}

	remain = 0;
	return OQS_SUCCESS;
}

OQS_API OQS_STATUS OQS_SIG_STFL_lms_sigs_total(uint64_t *total, const OQS_SIG_STFL_SECRET_KEY *secret_key) {

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
		//TODO log error.
		return -1;
	}

	oqs_key_data = malloc(sizeof(oqs_lms_key_data));
	if (oqs_key_data) {
		oqs_key_data->levels = 1;
		if (sk->length_secret_key) {
			oqs_key_data->len_sec_key = sk->length_secret_key;
			oqs_key_data->sec_key = (uint8_t *)malloc(sk->length_secret_key * sizeof(uint8_t));
			if (oqs_key_data->sec_key) {
				memset(oqs_key_data->sec_key, 0, sk->length_secret_key);
			} else {
				OQS_MEM_insecure_free(oqs_key_data);
				oqs_key_data = NULL;
				return -1;
			}
		} else {
			OQS_MEM_insecure_free(oqs_key_data);
			oqs_key_data = NULL;
			return -1;
		}

		//Aux Data
		size_t len_aux_data = DEFAULT_AUX_DATA;
		uint8_t *aux_data =  malloc(sizeof(uint8_t) * len_aux_data);
		if (aux_data) {
			oqs_key_data->aux_data = aux_data;
			oqs_key_data->len_aux_data = len_aux_data;
		} else {
			OQS_MEM_insecure_free( oqs_key_data->sec_key);
			OQS_MEM_insecure_free(oqs_key_data);
			return -1;
		}
	} else {
		//TODO log error
		return -1;
	}

	/* Set lms param set */
	switch (oid) {
	case 0x1:
		oqs_key_data->lm_type[0] = LMS_SHA256_N32_H5;
		oqs_key_data->lm_ots_type[0] = LMOTS_SHA256_N32_W1;
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
		OQS_MEM_insecure_free(oqs_key_data->sec_key);
		OQS_MEM_insecure_free(oqs_key_data->aux_data);
		OQS_MEM_insecure_free(oqs_key_data);
		oqs_key_data = NULL;
		return -1;
	}

	/* TODO: store key pair, file handler */

	ret = 0;
	return ret;
}

int oqs_sig_stfl_lms_sign(OQS_SIG_STFL_SECRET_KEY *sk,
                          uint8_t *sm, size_t *smlen,
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

void oqs_secret_lms_key_free(OQS_SIG_STFL_SECRET_KEY *sk) {
	if (sk == NULL) {
		return;
	}

	//TODO: cleanup lock_key

	if (sk->sig) {
		OQS_MEM_insecure_free(sk->sig);
		sk->sig = NULL;
	}

	if (sk->secret_key_data) {
		oqs_lms_key_data *key_data = (oqs_lms_key_data *)sk->secret_key_data;
		if (key_data) {
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
size_t oqs_serialize_lms_key(const OQS_SIG_STFL_SECRET_KEY *sk,  uint8_t **sk_key) {

	oqs_lms_key_data *lms_key_data = NULL;
	size_t key_len = 0;
	if (sk) {
		uint8_t *sk_key_buf = NULL;
		lms_key_data = sk->secret_key_data;
		if (lms_key_data && lms_key_data->sec_key) {
			size_t buf_size_needed = lms_key_data->len_aux_data + lms_key_data->len_sec_key;
			key_len = buf_size_needed;
			/* pass back serialized data */
			if (sk_key) {
				if (buf_size_needed) {
					sk_key_buf = malloc(buf_size_needed * sizeof(uint8_t));
					if (sk_key_buf) {

						/*
						 * Serialized data is sec_key followed by aux data
						 * So aux data begins after buffer top + sec_key length
						 */
						if (lms_key_data->len_sec_key) {
							memcpy(sk_key_buf, lms_key_data->sec_key, lms_key_data->len_sec_key);
						}

						if (lms_key_data->len_aux_data) {
							memcpy(sk_key_buf + lms_key_data->len_sec_key, lms_key_data->aux_data, lms_key_data->len_aux_data);
						}

						*sk_key = sk_key_buf;
						key_len = sk->length_secret_key + lms_key_data->len_aux_data;
					}
				}
			} //sk_key
		}
	} //sk
	return key_len;
}

/*
 * Convert LMS byte string to secret key object
 * Writes secret key + aux data if present
 * key_len is priv key length + aux length
 */
int oqs_deserialize_lms_key(OQS_SIG_STFL_SECRET_KEY *sk, size_t key_len, const uint8_t *sk_buf) {
	int oqs_status = -1;
	oqs_lms_key_data *lms_key_data = NULL;
	uint8_t priv_ky_len = hss_get_private_key_len((unsigned )(1), NULL, NULL);

	if ((!sk) || (key_len == 0) || (key_len < priv_ky_len) || (!sk_buf)) {
		return oqs_status;
	}

	if (sk->secret_key_data) {
		//Key data already present
		//We dont want to trample over data
		return oqs_status;
	}

	uint8_t *lms_sk = NULL;
	uint8_t *lms_aux = NULL;

	unsigned levels = 0;

	int key_buf_left = key_len - priv_ky_len;

	param_set_t lm_type[ MAX_HSS_LEVELS ];
	param_set_t lm_ots_type[ MAX_HSS_LEVELS ];

	// validate sk_buf for lms params
	if (hss_get_parameter_set(&levels,
	                          lm_type,
	                          lm_ots_type,
	                          NULL,
	                          (void *)sk_buf)) {
		return oqs_status;
	}

	lms_key_data = malloc(sizeof(oqs_lms_key_data));
	if (lms_key_data) {
		lms_sk = malloc(priv_ky_len * sizeof(uint8_t));
		if (lms_sk) {
			memcpy(lms_sk, sk_buf, priv_ky_len);
			lms_key_data->sec_key = lms_sk;
			lms_key_data->len_sec_key = priv_ky_len;
		} else {
			OQS_MEM_insecure_free(lms_key_data);
			return oqs_status;
		}

		if (key_buf_left) {
			lms_aux = malloc(key_buf_left * sizeof(uint8_t));
			if (lms_aux) {
				memcpy(lms_aux, (sk_buf + priv_ky_len), key_buf_left);
				lms_key_data->aux_data = lms_aux;
				lms_key_data->len_aux_data = key_buf_left;
			} else {
				OQS_MEM_insecure_free(lms_key_data);
				OQS_MEM_insecure_free(lms_sk);
				return oqs_status;
			}
		}

		sk->secret_key_data = lms_key_data;
		oqs_status = 0;
	}
	return oqs_status;
}
