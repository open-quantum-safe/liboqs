// SPDX-License-Identifier: Apache-2.0 AND MIT

#include <oqs/common.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#if (defined(OQS_USE_CPU_EXTENSIONS) && defined(OQS_PORTABLE_BUILD))

static OQS_CPU_EXTENSIONS available_cpu_extensions = { 0 };
static unsigned int available_cpu_extensions_set = 0;

#if defined(ARCH_X86_64)


#include "x86_64_helpers.h"

const char *const X64_EXTENSIONS_NAMES[] = {
#ifdef OQS_USE_AES_INSTRUCTIONS
	"AES",
#else
	"",
#endif
#ifdef OQS_USE_AVX_INSTRUCTIONS
	"AVX",
#else
	"",
#endif
#ifdef OQS_USE_AVX2_INSTRUCTIONS
	"AVX2",
#else
	"",
#endif
#ifdef OQS_USE_AVX512_INSTRUCTIONS
	"AVX512",
#else
	"",
#endif
#ifdef OQS_USE_BMI1_INSTRUCTIONS
	"BMI1",
#else
	"",
#endif
#ifdef OQS_USE_BMI2_INSTRUCTIONS
	"BMI2",
#else
	"",
#endif
#ifdef OQS_USE_PCLMUL_INSTRUCTIONS
	"PCLMUL",
#else
	"",
#endif
#ifdef OQS_USE_POPCNT_INSTRUCTIONS
	"POPCNT",
#else
	"",
#endif
#ifdef OQS_USE_SSE_INSTRUCTIONS
	"SSE",
#else
	"",
#endif
#ifdef OQS_USE_SSE2_INSTRUCTIONS
	"SSE2",
#else
	"",
#endif
#ifdef OQS_USE_SSE3_INSTRUCTIONS
	"SSE3"
#else
	"",
#endif
};

OQS_API const char *OQS_get_cpu_extension_name(unsigned int i) {
	return X64_EXTENSIONS_NAMES[i];
}
/* set_available_cpu_extensions_x86_64() has been written using:
 * https://github.com/google/cpu_features/blob/master/src/cpuinfo_x86.c
 */
static void set_available_cpu_extensions_x86_64(void) {
	cpuid_out leaf_1;
	cpuid(&leaf_1, 1);
	if (leaf_1.eax == 0) {
		return;
	}

	cpuid_out leaf_7;
	cpuid(&leaf_7, 7);

	const unsigned int has_xsave = is_bit_set(leaf_1.ecx, 26);
	const unsigned int has_osxsave = is_bit_set(leaf_1.ecx, 27);
	const uint32_t xcr0_eax = (has_xsave && has_osxsave) ? xgetbv_eax(0) : 0;

	available_cpu_extensions.AES_ENABLED = is_bit_set(leaf_1.ecx, 25);
	if (has_mask(xcr0_eax, MASK_XMM | MASK_YMM)) {
		available_cpu_extensions.AVX_ENABLED = is_bit_set(leaf_1.ecx, 28);
		available_cpu_extensions.AVX2_ENABLED = is_bit_set(leaf_7.ebx, 5);
	}
	available_cpu_extensions.PCLMUL_ENABLED = is_bit_set(leaf_1.ecx, 1);
	available_cpu_extensions.POPCNT_ENABLED = is_bit_set(leaf_1.ecx, 23);
	available_cpu_extensions.BMI1_ENABLED = is_bit_set(leaf_7.ebx, 3);
	available_cpu_extensions.BMI2_ENABLED = is_bit_set(leaf_7.ebx, 8);

	if (has_mask(xcr0_eax, MASK_XMM)) {
		available_cpu_extensions.SSE_ENABLED = is_bit_set(leaf_1.edx, 25);
		available_cpu_extensions.SSE2_ENABLED = is_bit_set(leaf_1.edx, 26);
		available_cpu_extensions.SSE3_ENABLED = is_bit_set(leaf_1.ecx, 0);
	}

	if (has_mask(xcr0_eax, MASK_XMM | MASK_YMM | MASK_MASKREG | MASK_ZMM0_15 | MASK_ZMM16_31)) {
		unsigned int avx512f = is_bit_set(leaf_7.ebx, 16);
		unsigned int avx512bw = is_bit_set(leaf_7.ebx, 30);
		unsigned int avx512dq = is_bit_set(leaf_7.ebx, 17);
		if (avx512f && avx512bw && avx512dq) {
			available_cpu_extensions.AVX512_ENABLED = 1;
		}
	}
}
#elif defined(ARCH_ARM_ANY)
static void set_available_cpu_extensions_arm(void) {
	//TODO
}

const char *const ARM_EXTENSIONS_NAMES[] = {
	"NEON"
};

OQS_API const char *OQS_get_cpu_extension_name(unsigned int i) {
	return ARM_EXTENSIONS_NAMES[i];
}
#endif /* ARCH_X86_64 or ARCH_ARM_ANY */

OQS_API OQS_CPU_EXTENSIONS OQS_get_available_CPU_extensions(void) {
	if (!available_cpu_extensions_set) {
#if defined(ARCH_X86_64)
		set_available_cpu_extensions_x86_64();
#elif defined(ARCH_ARM_ANY)
		set_available_cpu_extensions_arm();
#endif /* ARCH_X86_64 or ARCH_ARM_ANY */
		available_cpu_extensions_set = 1;
	}
	return available_cpu_extensions;
}
#endif /* OQS_USE_CPU_EXTENSIONS && OQS_PORTABLE_BUILD */

OQS_API void OQS_init(void) {
#if (defined(OQS_USE_CPU_EXTENSIONS) && defined(OQS_PORTABLE_BUILD))
	if (!available_cpu_extensions_set) {
#if defined(ARCH_X86_64)
		set_available_cpu_extensions_x86_64();
#elif defined(ARCH_ARM_ANY)
		set_available_cpu_extensions_arm();
#endif /* ARCH_X86_64 or ARCH_ARM_ANY */
		available_cpu_extensions_set = 1;
	}
#endif /* OQS_USE_CPU_EXTENSIONS && OQS_PORTABLE_BUILD */
}

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
