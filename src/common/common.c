#include <oqs/common.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>


#if defined(_WIN32)
#include <windows.h>
#endif

#ifdef OQS_ENABLE_CPUFEATURES
#include <cpu_features_macros.h>

#if defined(CPU_FEATURES_ARCH_X86)
#include <cpuinfo_x86.h>
#elif defined(CPU_FEATURES_ARCH_ARM)
#include <cpuinfo_arm.h>
#elif defined(CPU_FEATURES_ARCH_AARCH64)
#include <cpuinfo_aarch64.h>
#endif

static OQS_RT rt_cpu_flags = {0};
static int rt_cpu_flags_initialized = 0;

OQS_API void OQS_init(void) {
#if defined(CPU_FEATURES_ARCH_X86)
	const X86Features features = GetX86Info().features;

	if (features.avx) {
		rt_cpu_flags.AVX_INSTRUCTIONS = 1;
	}
	if (features.avx2) {
		rt_cpu_flags.AVX2_INSTRUCTIONS = 1;
	}
	if (features.avx512bw & features.avx512dq & features.avx512f) {
		rt_cpu_flags.AVX512_INSTRUCTIONS = 1;
	}
	if (features.bmi1) {
		rt_cpu_flags.BMI_INSTRUCTIONS = 1;
	}
	if (features.bmi2) {
		rt_cpu_flags.BMI2_INSTRUCTIONS = 1;
	}
	if (features.fma3) {
		rt_cpu_flags.FMA_INSTRUCTIONS = 1;
	}
	if (features.fma4) {
		rt_cpu_flags.FMA4_INSTRUCTIONS = 1;
	}
	if (features.mmx) {
		rt_cpu_flags.MMX_INSTRUCTIONS = 1;
	}
	if (features.popcnt) {
		rt_cpu_flags.POPCNT_INSTRUCTIONS = 1;
	}
	if (features.sse) {
		rt_cpu_flags.SSE_INSTRUCTIONS = 1;
	}
	if (features.sse2) {
		rt_cpu_flags.SSE2_INSTRUCTIONS = 1;
	}
	if (features.sse3) {
		rt_cpu_flags.SSE3_INSTRUCTIONS = 1;
	}
	if (features.sse4a) {
		rt_cpu_flags.SSE4A_INSTRUCTIONS = 1;
	}
#elif defined(CPU_FEATURES_ARCH_ARM)
	const ArmFeatures features = GetArmInfo().features;

	if (features.neon) {
		rt_cpu_flags.NEON_INSTRUCTIONS = 1;
	}
#elif defined(CPU_FEATURES_ARCH_AARCH64)
	const Aarch64Features features = GetAarch64Info().features;

	if (features.asimd) {
		rt_cpu_flags.NEON_INSTRUCTIONS = 1;
	}
#endif
	if (features.aes) {
		rt_cpu_flags.AES_INSTRUCTIONS = 1;
	}
}

OQS_API OQS_RT OQS_RT_cpu_flags(void) {
	if (!rt_cpu_flags_initialized) {
		OQS_init();
		rt_cpu_flags_initialized = 1;
	}
	return rt_cpu_flags;
}
#else /* OQS_ENABLE_CPUFEATURES */
OQS_API void OQS_init(void) {
}
#endif /* OQS_ENABLE_CPUFEATURES */

OQS_API void OQS_MEM_cleanse(void *ptr, size_t len) {
#if defined(_WIN32)
	SecureZeroMemory(ptr, len);
#elif defined(HAVE_MEMSET_S)
	if (0U < len && memset_s(ptr, (rsize_t)len, 0, (rsize_t)len) != 0) {
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
