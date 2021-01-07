// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <oqs/common.h>

#include "sha3.h"

#if defined(OQS_USE_AVX2_INSTRUCTIONS) && defined(OQS_PORTABLE_BUILD)
#define C_OR_AVX2(stmt_c, stmt_avx2) \
    OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions(); \
    if (available_cpu_extensions.AVX2_ENABLED) { \
        stmt_avx2; \
    } else { \
        stmt_c; \
    }
#elif defined(OQS_USE_AVX2_INSTRUCTIONS) /* && !defined(OQS_PORTABLE_BUILD) */
#define  C_OR_AVX2(stmt_c, stmt_avx2) \
    stmt_avx2;
#else /* !defined(OQS_USE_AVX2_INSTRUCTIONS) */
#define  C_OR_AVX2(stmt_c, stmt_avx2) \
    stmt_c;
#endif

#if defined(OQS_PORTABLE_BUILD) || !defined(OQS_USE_AVX2_INSTRUCTIONS)
static inline void OQS_SHA3_shake128_4x_c(uint8_t *output0, uint8_t *output1, uint8_t *output2, uint8_t *output3, size_t outlen,
        const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	OQS_SHA3_shake128(output0, outlen, in0, inlen);
	OQS_SHA3_shake128(output1, outlen, in1, inlen);
	OQS_SHA3_shake128(output2, outlen, in2, inlen);
	OQS_SHA3_shake128(output3, outlen, in3, inlen);
}
#endif

#if defined(OQS_USE_AVX2_INSTRUCTIONS)
void OQS_SHA3_shake128_4x_avx2(uint8_t *output0, uint8_t *output1, uint8_t *output2, uint8_t *output3, unsigned long long outlen,
                               const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, unsigned long long inlen);
#endif

void OQS_SHA3_shake128_4x(uint8_t *output0, uint8_t *output1, uint8_t *output2, uint8_t *output3, size_t outlen,
                          const uint8_t *in0, const uint8_t *in1, const uint8_t *in2, const uint8_t *in3, size_t inlen) {
	C_OR_AVX2(
	    OQS_SHA3_shake128_4x_c(output0, output1, output2, output3, outlen, in0, in1, in2, in3, inlen),
	    OQS_SHA3_shake128_4x_avx2(output0, output1, output2, output3, (unsigned long long) outlen, in0, in1, in2, in3, (unsigned long long) inlen)
	)
}
