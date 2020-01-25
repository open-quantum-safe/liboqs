#include <stdio.h>
#include <stdlib.h>
int main(void) {
#if defined(__GNUC__) && !defined(__clang__)
    __builtin_cpu_init();
#endif
    if(__builtin_cpu_supports("aes")) {
        printf("AES;");
    }
    if(__builtin_cpu_supports("avx")) {
        printf("AVX;");
    }
    if(__builtin_cpu_supports("avx2")) {
        printf("AVX2;");
    }
    if(__builtin_cpu_supports("avx512bw")) {
        printf("AVX512BW;");
    }
    if(__builtin_cpu_supports("avx512dq")) {
        printf("AVX512DQ;");
    }
    if(__builtin_cpu_supports("avx512f")) {
        printf("AVX512F;");
    }
    if(__builtin_cpu_supports("bmi")) {
        printf("BMI;");
    }
    if(__builtin_cpu_supports("bmi2")) {
        printf("BMI2;");
    }
    if(__builtin_cpu_supports("fma")) {
        printf("FMA;");
    }
    if(__builtin_cpu_supports("fma4")) {
        printf("FMA4;");
    }
    if(__builtin_cpu_supports("mmx")) {
        printf("MMX;");
    }
    if(__builtin_cpu_supports("popcnt")) {
        printf("POPCNT;");
    }
    if(__builtin_cpu_supports("sse")) {
        printf("SSE;");
    }
    if(__builtin_cpu_supports("sse2")) {
        printf("SSE2;");
    }
    if(__builtin_cpu_supports("sse3")) {
        printf("SSE3;");
    }
    if(__builtin_cpu_supports("sse4a")) {
        printf("SSE4A;");
    }
    return EXIT_SUCCESS;
}
