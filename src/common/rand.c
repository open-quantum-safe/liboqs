#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>

#include <oqs/oqs.h>

static void (*oqs_randombytes_algorithm)(uint8_t *, size_t) = &OQS_randombytes_system;

OQS_STATUS OQS_randombytes_switch_algorithm(const char *algorithm) {
	if (0 == strcasecmp(OQS_RAND_alg_system, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_system;
		return OQS_SUCCESS;
	} else if (0 == strcasecmp(OQS_RAND_alg_nist_kat, algorithm)) {
		oqs_randombytes_algorithm = &OQS_randombytes_nist_kat;
		return OQS_SUCCESS;
	} else {
		return OQS_ERROR;
	}
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

	int handle;
	do {
		handle = open("/dev/urandom", O_RDONLY);
		if (handle == -1) {
			delay(0xFFFFF);
		}
	} while (handle == -1);

	int bytes_last_read, bytes_total_read, bytes_left_to_read;
	bytes_total_read = 0;
	bytes_left_to_read = bytes_to_read;
	while (bytes_left_to_read > 0) {
		do {
			bytes_last_read = read(handle, random_array + bytes_total_read, bytes_left_to_read);
			if (bytes_last_read == -1) {
				delay(0xFFFF);
			}
		} while (bytes_last_read == -1);
		bytes_total_read += bytes_last_read;
		bytes_left_to_read -= bytes_last_read;
	}
}
