#include <stdio.h>
#include <stdlib.h>

#include <cpu_features_macros.h>

#if defined(CPU_FEATURES_ARCH_X86)
#include <cpuinfo_x86.h>
#elif defined(CPU_FEATURES_ARCH_ARM)
#include <cpuinfo_arm.h>
#elif defined(CPU_FEATURES_ARCH_AARCH64)
#include <cpuinfo_aarch64.h>
#endif

int main(void) {
#if defined(CPU_FEATURES_ARCH_X86)
    const X86Features features = GetX86Info().features;

    if(features.avx) {
        printf("AVX;");
    }
    if(features.avx2) {
        printf("AVX2;");
    }
    if(features.avx512bw) {
        printf("AVX512BW;");
    }
    if(features.avx512dq) {
        printf("AVX512DQ;");
    }
    if(features.avx512f) {
        printf("AVX512F;");
    }
    if(features.bmi1) {
        printf("BMI;");
    }
    if(features.bmi2) {
        printf("BMI2;");
    }
    if(features.fma3) {
        printf("FMA;");
    }
    if(features.fma4) {
        printf("FMA4;");
    }
    if(features.mmx) {
        printf("MMX;");
    }
    if(features.popcnt) {
        printf("POPCNT;");
    }
    if(features.sse) {
        printf("SSE;");
    }
    if(features.sse2) {
        printf("SSE2;");
    }
    if(features.sse3) {
        printf("SSE3;");
    }
    if(features.sse4a) {
        printf("SSE4A;");
    }
#elif defined(CPU_FEATURES_ARCH_ARM)
    const ArmFeatures features = GetArmInfo().features;
    if(features.neon) {
        printf("NEON;");
    }
#elif defined(CPU_FEATURES_ARCH_AARCH64)
    const Aarch64Features features = GetAarch64Info().features;
    if(features.asimd) {
        printf("NEON;");
    }
#endif
    if(features.aes) {
        printf("AES;");
    }

    return EXIT_SUCCESS;
}
