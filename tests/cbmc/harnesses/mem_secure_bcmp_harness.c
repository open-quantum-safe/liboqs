// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>

#include <oqs/oqs.h>

#define OQS_CBMC_BCMP_MAX_LEN 4

extern unsigned char nondet_uchar(void);
extern size_t nondet_size_t(void);

int main(void) {
	uint8_t a[OQS_CBMC_BCMP_MAX_LEN];
	uint8_t b[OQS_CBMC_BCMP_MAX_LEN];
	uint8_t a_before[OQS_CBMC_BCMP_MAX_LEN];
	uint8_t b_before[OQS_CBMC_BCMP_MAX_LEN];
	size_t len = nondet_size_t();
	int expected = 0;

	__CPROVER_assume(len <= OQS_CBMC_BCMP_MAX_LEN);

	for (size_t i = 0; i < OQS_CBMC_BCMP_MAX_LEN; i++) {
		a[i] = nondet_uchar();
		b[i] = nondet_uchar();
		a_before[i] = a[i];
		b_before[i] = b[i];
	}

	for (size_t i = 0; i < len; i++) {
		if (a[i] != b[i]) {
			expected = 1;
		}
	}

	int result = OQS_MEM_secure_bcmp(a, b, len);

	__CPROVER_assert(result == expected,
	                 "secure_bcmp returns zero iff all bounded bytes match");
	__CPROVER_assert(result == 0 || result == 1,
	                 "secure_bcmp result is normalized to one bit");

	for (size_t i = 0; i < OQS_CBMC_BCMP_MAX_LEN; i++) {
		__CPROVER_assert(a[i] == a_before[i],
		                 "secure_bcmp does not mutate left input");
		__CPROVER_assert(b[i] == b_before[i],
		                 "secure_bcmp does not mutate right input");
	}

	return 0;
}
