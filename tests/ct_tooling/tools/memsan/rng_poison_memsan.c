// SPDX-License-Identifier: MIT

#include <sanitizer/msan_interface.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

void OQS_randombytes(uint8_t *r, size_t len) {
	memset(r, 0xA5, len); // Fill with pattern 0xA5
	__msan_poison(r, len);
}