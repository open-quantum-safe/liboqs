// SPDX-License-Identifier: MIT

#include <stdio.h>
#if defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
#define strcasecmp _stricmp
#else
#include <unistd.h>
#include <strings.h>
#if defined(__APPLE__)
#include <sys/random.h>
#else
#include <unistd.h>
#endif
#endif
#include <fcntl.h>
#include <stdlib.h>

#include <oqs/oqs.h>

void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read);
void OQS_randombytes_nist_kat(uint8_t *random_array, size_t bytes_to_read);
#ifdef OQS_USE_OPENSSL
void OQS_randombytes_openssl(uint8_t *random_array, size_t bytes_to_read);
#endif

#ifdef OQS_USE_OPENSSL
#include <openssl/rand.h>
// Use OpenSSL's RAND_bytes as the default PRNG
static void (*oqs_randombytes_algorithm)(uint8_t *, size_t) = &OQS_randombytes_openssl;
#else
static void (*oqs_randombytes_algorithm)(uint8_t *, size_t) = &OQS_randombytes_system;
#endif
OQS_API OQS_STATUS OQS_randombytes_switch_algorithm(const char *algorithm) {
	if (0 == strcasecmp(OQS_RAND_alg_system, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_system;
		return OQS_SUCCESS;
	} else if (0 == strcasecmp(OQS_RAND_alg_nist_kat, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_nist_kat;
		return OQS_SUCCESS;
	} else if (0 == strcasecmp(OQS_RAND_alg_openssl, algorithm)) {
#ifdef OQS_USE_OPENSSL
		oqs_randombytes_algorithm = &OQS_randombytes_openssl;
		return OQS_SUCCESS;
#else
		return OQS_ERROR;
#endif
	} else {
		return OQS_ERROR;
	}
}

OQS_API void OQS_randombytes_custom_algorithm(void (*algorithm_ptr)(uint8_t *, size_t)) {
	oqs_randombytes_algorithm = algorithm_ptr;
}

OQS_API void OQS_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	oqs_randombytes_algorithm(random_array, bytes_to_read);
}

#if !defined(_WIN32)
#if defined(HAVE_GETENTROPY)
void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read) {

	int rc;
	do {
		rc = getentropy(random_array, bytes_to_read);
	} while (rc != 0);
}
#else
static __inline void delay(unsigned int count) {
	while (count--) {
	}
}

void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read) {

	FILE *handle;
	do {
		handle = fopen("/dev/urandom", "rb");
		if (handle == NULL) {
			delay(0xFFFFF);
		}
	} while (handle == NULL);

	size_t bytes_last_read, bytes_total_read, bytes_left_to_read;
	bytes_total_read = 0;
	bytes_left_to_read = bytes_to_read;
	while (bytes_left_to_read > 0) {
		do {
			bytes_last_read = fread(random_array + bytes_total_read, 1, bytes_left_to_read, handle);
			if (bytes_last_read == 0) {
				delay(0xFFFF);
			}
		} while (bytes_last_read == 0);
		bytes_total_read += bytes_last_read;
		bytes_left_to_read -= bytes_last_read;
	}
	fclose(handle);
}
#endif
#else
void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read) {
	HCRYPTPROV hCryptProv;
	if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) ||
	        !CryptGenRandom(hCryptProv, (DWORD) bytes_to_read, random_array)) {
		exit(EXIT_FAILURE); // better to fail than to return bad random data
	}
	CryptReleaseContext(hCryptProv, 0);
}
#endif

#ifdef OQS_USE_OPENSSL
void OQS_randombytes_openssl(uint8_t *random_array, size_t bytes_to_read) {
	int rc;
	SIZE_T_TO_INT_OR_EXIT(bytes_to_read, bytes_to_read_int)
	do {
		rc = RAND_bytes(random_array, bytes_to_read_int);
	} while (rc != 1);
}
#endif
