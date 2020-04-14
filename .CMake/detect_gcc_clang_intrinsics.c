#include <stdio.h>

int main(void) {
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
#if defined(__BMI__)
	printf("BMI;");
#endif
#if defined(__BMI2__)
	printf("BMI2;");
#endif
#if defined(__FMA__)
	printf("FMA;");
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
	return 0;
}
