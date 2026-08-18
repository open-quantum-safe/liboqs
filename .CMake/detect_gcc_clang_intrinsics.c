// SPDX-License-Identifier: MIT

#include <stdio.h>

int main(void) {
#if defined(__ADX__)
	printf("ADX;");
#endif
#if defined(__AES__)
	printf("AES;");
#endif
#if defined(__AVX__)
	printf("AVX;");
#endif
#if defined(__AVX2__)
	printf("AVX2;");
#endif
#if defined(__AVX512BW__)
	printf("AVX512BW;");
#endif
#if defined(__AVX512DQ__)
	printf("AVX512DQ;");
#endif
#if defined(__AVX512F__)
	printf("AVX512F;");
#endif
#if defined(__VPCLMULQDQ__)
	printf("VPCLMULQDQ;");
#endif
#if defined(__BMI__)
	printf("BMI1;");
#endif
#if defined(__BMI2__)
	printf("BMI2;");
#endif
#if defined(__FMA__)
	printf("FMA;");
#endif
#if defined(__PCLMUL__)
	printf("PCLMULQDQ;");
#endif
#if defined(__POPCNT__)
	printf("POPCNT;");
#endif
#if defined(__SSE__)
	printf("SSE;");
#endif
#if defined(__SSE2__)
	printf("SSE2;");
#endif
#if defined(__SSE3__)
	printf("SSE3;");
#endif
#if defined(__ARM_FEATURE_AES)
	printf("ARM_AES;");
#endif
#if (defined(__APPLE__) && defined(__aarch64__)) || defined(__ARM_FEATURE_SHA2)
	printf("ARM_SHA2;");
#endif
#if defined(__ARM_FEATURE_SHA3)
	printf("ARM_SHA3;");
#endif
#if defined(__ARM_NEON)
	printf("ARM_NEON;");
#endif
	return 0;
}
