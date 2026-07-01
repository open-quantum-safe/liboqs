// SPDX-License-Identifier: MIT

#ifndef OQS_KEM_ML_KEM_INTERNAL_H
#define OQS_KEM_ML_KEM_INTERNAL_H

#include <oqs/oqs.h>

static inline OQS_STATUS oqs_ml_kem_native_status_to_oqs(int status) {
	return status == 0 ? OQS_SUCCESS : OQS_ERROR;
}

#endif
