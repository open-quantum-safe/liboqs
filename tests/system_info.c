// SPDX-License-Identifier: MIT

#include <oqs/oqs.h>

#include <stdio.h>
#include <string.h>

// based on macros in https://sourceforge.net/p/predef/wiki/Compilers/
static void print_compiler_info(void) {
#if defined(__clang__)
	printf("Compiler:         clang (%s)\n", __clang_version__);
#elif defined(__GNUC_PATCHLEVEL__)
	printf("Compiler:         gcc (%d.%d.%d)\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#elif defined(__GNUC_MINOR__)
	printf("Compiler:         gcc (%d.%d)\n", __GNUC__, __GNUC_MINOR__);
#elif defined(__INTEL_COMPILER)
	printf("Compiler:         Intel C/C++ (%d)\n", __INTEL_COMPILER);
#elif defined(_MSC_FULL_VER)
	printf("Compiler:         Microsoft C/C++ (%d)\n", _MSC_FULL_VER);
#else
	printf("Compiler:         Unknown"\n);
#endif
#if defined(OQS_COMPILE_OPTIONS)
	printf("Compile options:  %s\n", OQS_COMPILE_OPTIONS);
#endif
}

// based on macros in https://sourceforge.net/p/predef/wiki/Architectures/
static void print_platform_info(void) {
#if defined(OQS_COMPILE_BUILD_TARGET)
	printf("Target platform:  %s\n", OQS_COMPILE_BUILD_TARGET);
#elif defined(_WIN64)
	printf("Target platform:  Windows (64-bit)\n");
#elif defined(_WIN32)
	printf("Target platform:  Windows (32-bit)\n");
#else
	printf("Target platform:  Unknown\n");
#endif
}

#if defined(OQS_USE_OPENSSL)
#include <openssl/opensslv.h>
#endif

#if defined(OQS_USE_CPU_EXTENSIONS) && defined(OQS_PORTABLE_BUILD)
#define C_OR_NI(stmt_c, stmt_ni) \
    OQS_CPU_EXTENSIONS available_cpu_extensions = OQS_get_available_CPU_extensions(); \
    if (available_cpu_extensions.AES_ENABLED) { \
        stmt_ni; \
    } else { \
        stmt_c; \
    }
#elif defined(OQS_USE_CPU_EXTENSIONS) /* && !defined(OQS_PORTABLE_BUILD) */
#define  C_OR_NI(stmt_c, stmt_ni) \
    stmt_ni;
#else /* !defined(OQS_USE_CPU_EXTENSIONS) */
#define  C_OR_NI(stmt_c, stmt_ni) \
    stmt_c;
#endif

/* Display all active CPU extensions: */
static void print_cpu_extensions(void) {
#if defined(OQS_USE_CPU_EXTENSIONS) && defined(OQS_PORTABLE_BUILD)
	/* Make CPU features struct iterable */
	typedef union ext_u {
		OQS_CPU_EXTENSIONS ext_x;
		unsigned int ext_a[sizeof(OQS_CPU_EXTENSIONS) / sizeof(unsigned int)];
	} OQS_CPU_EXTENSIONS_UNION;

	OQS_CPU_EXTENSIONS_UNION ext_u;
	ext_u.ext_x = OQS_get_available_CPU_extensions();
	printf("CPU exts active:  ");
	unsigned int it = sizeof(ext_u.ext_a) / sizeof(ext_u.ext_a[0]);
	for (unsigned int i = 0; i < it; i++) {
		if (ext_u.ext_a[i]) {
			const char *aname = OQS_get_cpu_extension_name(i);
			if ((strlen(aname) > 0) && (i != it - 1)) {
				printf("%s-", aname);
			} else {
				printf("%s", aname);
			}
		}
	}
	printf("\n");
#else /* no extensions active */
	printf("CPU exts active:  None\n");
#endif
}

static void print_oqs_configuration(void) {
	printf("OQS version:      %s\n", OQS_VERSION_TEXT);
#if defined(OQS_COMPILE_GIT_COMMIT)
	printf("Git commit:       %s\n", OQS_COMPILE_GIT_COMMIT);
#endif
#if defined(OQS_USE_OPENSSL)
	printf("OpenSSL enabled:  Yes (%s)\n", OPENSSL_VERSION_TEXT);
#else
	printf("OpenSSL enabled:  No\n");
#endif
#if defined(OQS_USE_AES_OPENSSL)
	printf("AES:              OpenSSL\n");
#else
	C_OR_NI(
	    printf("AES:              C\n"),
	    printf("AES:              NI\n")
	)
#endif
#if defined(OQS_USE_SHA2_OPENSSL)
	printf("SHA-2:            OpenSSL\n");
#else
	printf("SHA-2:            C\n");
#endif
#if defined(OQS_USE_SHA3_OPENSSL)
	printf("SHA-3:            OpenSSL\n");
#else
	printf("SHA-3:            C\n");
#endif
}

static void print_system_info(void) {
	printf("Configuration info\n");
	printf("==================\n");
	print_platform_info();
	print_compiler_info();
	print_oqs_configuration();
	print_cpu_extensions();
	printf("\n");
}
