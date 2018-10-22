#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <fcntl.h>

#include <openssl/rand.h>

#include <oqs/oqs.h>

void OQS_randombytes_system(uint8_t *random_array, size_t bytes_to_read);
void OQS_randombytes_nist_kat(uint8_t *random_array, size_t bytes_to_read);
void OQS_randombytes_openssl(uint8_t *random_array, size_t bytes_to_read);

// Use OpenSSL's RAND_bytes as the default PRNG
static void (*oqs_randombytes_algorithm)(uint8_t *, size_t) = &OQS_randombytes_openssl;

OQS_STATUS OQS_randombytes_switch_algorithm(const char *algorithm) {
	if (0 == strcasecmp(OQS_RAND_alg_system, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_system;
		return OQS_SUCCESS;
	} else if (0 == strcasecmp(OQS_RAND_alg_nist_kat, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_nist_kat;
		return OQS_SUCCESS;
	} else if (0 == strcasecmp(OQS_RAND_alg_openssl, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_openssl;
		return OQS_SUCCESS;
	} else {
		return OQS_ERROR;
	}
}

void OQS_randombytes_custom_algorithm(void (*algorithm_ptr)(uint8_t *, size_t)) {
	oqs_randombytes_algorithm = algorithm_ptr;
}

void randombytes(uint8_t *random_array, size_t bytes_to_read) {
	OQS_randombytes(random_array, bytes_to_read);
}

void OQS_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	oqs_randombytes_algorithm(random_array, bytes_to_read);
}

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

	int bytes_last_read, bytes_total_read, bytes_left_to_read;
	bytes_total_read = 0;
	bytes_left_to_read = bytes_to_read;
	while (bytes_left_to_read > 0) {
		do {
			bytes_last_read = fread(random_array + bytes_total_read, 1, bytes_left_to_read, handle);
			if (bytes_last_read <= 0) {
				delay(0xFFFF);
			}
		} while (bytes_last_read <= 0);
		bytes_total_read += bytes_last_read;
		bytes_left_to_read -= bytes_last_read;
	}
	fclose(handle);
}

void OQS_randombytes_openssl(uint8_t *random_array, size_t bytes_to_read) {
	int rc;
	do {
		rc = RAND_bytes(random_array, bytes_to_read);
	} while (rc != 1);
}
