// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>

/*
 * The dispatch proofs do not reason about allocator internals.  These models
 * satisfy source-level references from free helpers that are outside the
 * reachable proof claims.
 */
void OQS_MEM_insecure_free(void *ptr) {
	(void) ptr;
}

void OQS_MEM_secure_free(void *ptr, size_t len) {
	(void) ptr;
	(void) len;
}
