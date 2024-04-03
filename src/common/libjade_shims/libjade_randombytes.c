// SPDX-License-Identifier: MIT

#include <oqs/rand.h>
#include "libjade_randombytes.h"

void __jasmin_syscall_randombytes__(uint8_t *_x, uint64_t xlen) {
	OQS_randombytes(_x, xlen);
	// return _x;
}
