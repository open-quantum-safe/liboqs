// SPDX-License-Identifier: MIT

#ifndef OQS_SIG_STFL_LMS_WRAP_H
#define OQS_SIG_STFL_LMS_WRAP_H

//#include <oqs/oqs.h>
#include "external/hss.h"
#include "external/hss_sign_inc.h"


/**
 * @brief OQS_LMS_KEY object for HSS key pair
 */
typedef struct OQS_LMS_KEY_DATA oqs_lms_key_data;

typedef struct OQS_LMS_KEY_DATA {

	/* Tree levels. */
	unsigned levels;

	/* Array, 8 levels max, of LMS types */
	param_set_t lm_type[8];

	/* Array, 8 levels max, of LM OTS types */
	param_set_t lm_ots_type[8];

	/* LMS public key */
	unsigned char public_key[60];

	/* internal nodes info of the Merkle tree */
	unsigned char *aux_data;

	/* Length of aux data */
	size_t len_aux_data;

	/* User defined data that may be used for the SAFETY functions */
	void *data;

} oqs_lms_key_data;


typedef struct OQS_LMS_SIG_DATA oqs_lms_sig_data;

typedef struct OQS_LMS_SIG_DATA {


	/* message buffer */
	unsigned char *message;

	/* Length of msg buffer */
	size_t len_msg_buf;

	/* signature buffer */
	unsigned char *signature;

	/* Length of sig buffer */
	size_t len_sig_buf;

} oqs_lms_sig_data;

#endif //OQS_SIG_STFL_LMS_H

