#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <oqs/oqs.h>

static __inline void delay(unsigned int count) {
	while (count--) {}
}

OQS_STATUS OQS_randombytes(uint8_t *random_array, size_t bytes_to_read) {

	int lock = open("/dev/urandom", O_RDONLY);
	if (lock == -1) {
		return OQS_ERROR;
	}

	int bytes_last_read, bytes_total_read, bytes_left_to_read;
	bytes_total_read = 0;
	bytes_left_to_read = bytes_to_read;
	while (bytes_left_to_read > 0) {
		do {
			bytes_last_read = read(lock, random_array + bytes_total_read, bytes_left_to_read);
			if (bytes_last_read == -1) {
				delay(0xFFFF);
			}
		} while (bytes_last_read == -1);
		bytes_total_read += bytes_last_read;
		bytes_left_to_read -= bytes_last_read;
	}

	return OQS_SUCCESS;

}
