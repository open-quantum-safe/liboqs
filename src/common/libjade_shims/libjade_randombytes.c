// SPDX-License-Identifier: Apache-2.0 OR CC0-1.0

#if defined(__unix__) || defined(__APPLE__)
#include <oqs/rand.h>
#include "libjade_randombytes.h"

void __jasmin_syscall_randombytes__(uint8_t *_x, uint64_t xlen) {
	OQS_randombytes(_x, xlen);
}
#endif
