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
