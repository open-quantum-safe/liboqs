#include <oqs/common.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#include <cpu_features_macros.h>

#if defined(CPU_FEATURES_ARCH_X86)
#include <cpuinfo_x86.h>
#elif defined(CPU_FEATURES_ARCH_ARM)
#include <cpuinfo_arm.h>
#elif defined(CPU_FEATURES_ARCH_AARCH64)
#include <cpuinfo_aarch64.h>
#endif

OQS_API void OQS_init(void) {
#if defined(CPU_FEATURES_ARCH_X86)
    const X86Features features = GetX86Info().features;

    if(features.avx) {
	OQS_RT_AVX_INSTRUCTIONS=1 ;
    }
    if(features.avx2) {
	OQS_RT_AVX2_INSTRUCTIONS=1 ;
    }
    if(features.avx512bw & features.avx512dq & features.avx512f) {
	OQS_RT_AVX512_INSTRUCTIONS=1 ;
    }
    if(features.bmi1) {
	OQS_RT_BMI_INSTRUCTIONS=1 ;
    }
    if(features.bmi2) {
	OQS_RT_BMI2_INSTRUCTIONS=1 ;
    }
    if(features.fma3) {
	OQS_RT_FMA_INSTRUCTIONS=1 ;
    }
    if(features.fma4) {
	OQS_RT_FMA4_INSTRUCTIONS=1 ;
    }
    if(features.mmx) {
	OQS_RT_MMX_INSTRUCTIONS=1 ;
    }
    if(features.popcnt) {
	OQS_RT_POPCNT_INSTRUCTIONS=1 ;
    }
    if(features.sse) {
	OQS_RT_SSE_INSTRUCTIONS=1 ;
    }
    if(features.sse2) {
	OQS_RT_SSE2_INSTRUCTIONS=1 ;
    }
    if(features.sse3) {
	OQS_RT_SSE3_INSTRUCTIONS=1 ;
    }
    if(features.sse4a) {
	OQS_RT_SSE4A_INSTRUCTIONS=1 ;
    }
#elif defined(CPU_FEATURES_ARCH_ARM)
    const ArmFeatures features = GetArmInfo().features;
    if(features.neon) {
	OQS_RT_NEON_INSTRUCTIONS=1;
    }
#elif defined(CPU_FEATURES_ARCH_AARCH64)
    const Aarch64Features features = GetAarch64Info().features;
    if(features.asimd) {
	OQS_RT_NEON_INSTRUCTIONS=1;
    }
#endif
    if(features.aes) {
	OQS_RT_AES_INSTRUCTIONS=1 ;
    }
}

OQS_API void OQS_MEM_cleanse(void *ptr, size_t len) {
#if defined(_WIN32)
	SecureZeroMemory(ptr, len);
#elif defined(HAVE_MEMSET_S)
	if (0U < len && memset_s(ptr, (rsize_t) len, 0, (rsize_t) len) != 0) {
		abort();
	}
#else
	typedef void *(*memset_t)(void *, int, size_t);
	static volatile memset_t memset_func = memset;
	memset_func(ptr, 0, len);
#endif
}

OQS_API void OQS_MEM_secure_free(void *ptr, size_t len) {
	if (ptr != NULL) {
		OQS_MEM_cleanse(ptr, len);
		free(ptr); // IGNORE free-check
	}
}

OQS_API void OQS_MEM_insecure_free(void *ptr) {
	free(ptr); // IGNORE free-check
}
