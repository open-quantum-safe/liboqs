// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>

#include "kem_ml_kem_internal.h"

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
	int failing_statuses[] = {-1, -2, -3, 1};

	if (assert_status(0, OQS_SUCCESS) != EXIT_SUCCESS) {
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < sizeof(failing_statuses) / sizeof(failing_statuses[0]); i++) {
		if (assert_status(failing_statuses[i], OQS_ERROR) != EXIT_SUCCESS) {
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
