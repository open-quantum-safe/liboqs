#include <oqs/oqs.h>

#include <stdio.h>

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

#if defined(USE_OPENSSL)
#include <openssl/opensslv.h>
#endif

static void print_oqs_configuration(void) {
	printf("OQS version:      %s\n", OQS_VERSION_TEXT);
#if defined(OQS_COMPILE_GIT_COMMIT)
	printf("Git commit:       %s\n", OQS_COMPILE_GIT_COMMIT);
#endif
#if defined(USE_OPENSSL)
	printf("OpenSSL enabled:  Yes (%s)\n", OPENSSL_VERSION_TEXT);
#else
	printf("OpenSSL enabled:  No\n");
#endif
#if defined(USE_AES_C)
	printf("AES:              C\n");
#elif defined(USE_AES_OPENSSL)
	printf("AES:              OpenSSL\n");
#endif
#if defined(USE_SHA2_C)
	printf("SHA-2:            C\n");
#elif defined(USE_SHA2_OPENSSL)
	printf("SHA-2:            OpenSSL\n");
#endif
#if defined(USE_SHA3_C)
	printf("SHA-3:            C\n");
#elif defined(USE_SHA3_OPENSSL)
	printf("SHA-3:            OpenSSL\n");
#endif
#if defined(OQS_COMPILE_CFLAGS)
	printf("CFLAGS:           %s\n", OQS_COMPILE_CFLAGS);
#endif
#if defined(OQS_COMPILE_LDFLAGS)
	printf("LDFLAGS:          %s\n", OQS_COMPILE_LDFLAGS);
#endif
}

static void print_system_info(void) {
	printf("Configuration info\n");
	printf("==================\n");
	print_platform_info();
	print_compiler_info();
	print_oqs_configuration();
	printf("\n");
}
