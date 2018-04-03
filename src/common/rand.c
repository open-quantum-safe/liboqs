#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>

#include <oqs/oqs.h>

static char *oqs_randombytes_selected_algorithm = OQS_RAND_alg_system;

bool OQS_randombytes_switch_algorithm(const char *algorithm) {
	if (0 == strcasecmp(algorithm, OQS_RAND_alg_system)) {
		oqs_randombytes_selected_algorithm = OQS_RAND_alg_system;
		return true;
	} else if (0 == strcasecmp(algorithm, OQS_RAND_alg_nist_kat)) {
		oqs_randombytes_selected_algorithm = OQS_RAND_alg_nist_kat;
		return true;
	} else {
		return false;
	}
}

void OQS_randombytes(uint8_t *random_array, size_t bytes_to_read) {
	if (0 == strcasecmp(oqs_randombytes_selected_algorithm, OQS_RAND_alg_nist_kat)) {
		OQS_randombytes_nist_kat(random_array, bytes_to_read);
	} else {
		OQS_randombytes_system(random_array, bytes_to_read);
	}
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
