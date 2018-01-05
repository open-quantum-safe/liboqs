#ifndef __OQS_H__
#define __OQS_H__

#define OQS_STATUS int
#define OQS_SUCCESS 0
#define OQS_ERROR -1

#include <oqs/config.h>
#include <oqs/rand.h>
#include <oqs/kem.h>

#define OQS_PRINT_HEX_STRING(label, str, len)                    \
	{                                                            \
		printf("%-20s (%4zu bytes):  ", (label), (size_t)(len)); \
		for (size_t i = 0; i < (len); i++) {                     \
			printf("%02X", ((unsigned char *) (str))[i]);        \
		}                                                        \
		printf("\n");                                            \
	}
#endif
