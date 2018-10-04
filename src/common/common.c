#include <oqs/common.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#endif

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
