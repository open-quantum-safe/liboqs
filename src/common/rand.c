#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <oqs/oqs.h>

static __inline void delay(unsigned int count) {
	while (count--) {
	}
}

void OQS_randombytes(uint8_t *random_array, size_t bytes_to_read) {

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
