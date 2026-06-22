// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include "kem_ml_kem_internal.h"

/*
 * mlkem-native returns plain int status codes:
 * 0 for success, negative values such as MLK_ERR_FAIL,
 * MLK_ERR_OUT_OF_MEMORY, and MLK_ERR_RNG_FAIL for failures.
 * The positive non-zero case is synthetic and keeps the generic fallback
 * behavior covered.
 */
enum {
	MLK_NATIVE_STATUS_SUCCESS = 0,
	MLK_NATIVE_STATUS_ERR_FAIL = -1,
	MLK_NATIVE_STATUS_ERR_OUT_OF_MEMORY = -2,
	MLK_NATIVE_STATUS_ERR_RNG_FAIL = -3,
	MLK_NATIVE_STATUS_OTHER_FAILURE = 1,
};

static int assert_status(int native_status, OQS_STATUS expected_status) {
	OQS_STATUS actual_status = oqs_ml_kem_native_status_to_oqs(native_status);

	if (actual_status != expected_status) {
		fprintf(stderr, "native status %d mapped to %d, expected %d\n",
		        native_status, actual_status, expected_status);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int main(void) {
	int failing_statuses[] = {
		MLK_NATIVE_STATUS_ERR_FAIL,
		MLK_NATIVE_STATUS_ERR_OUT_OF_MEMORY,
		MLK_NATIVE_STATUS_ERR_RNG_FAIL,
		MLK_NATIVE_STATUS_OTHER_FAILURE,
	};

	if (assert_status(MLK_NATIVE_STATUS_SUCCESS, OQS_SUCCESS) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < sizeof(failing_statuses) / sizeof(failing_statuses[0]); i++) {
		if (assert_status(failing_statuses[i], OQS_ERROR) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
