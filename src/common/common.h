#ifndef __OQS_COMMON_H
#define __OQS_COMMON_H

#include <stdint.h>
#include <stdlib.h>

typedef int OQS_STATUS;
#define OQS_SUCCESS 0
#define OQS_ERROR -1

/* Displays hexadecimal strings */
void OQS_print_hex_string(const char *label, uint8_t *str, size_t len);

/* Partially displays hexadecimal strings */
void OQS_print_part_hex_string(const char *label, uint8_t *str, size_t len, size_t sub_len);

void OQS_MEM_cleanse(void *ptr, size_t len);
void OQS_MEM_secure_free(void *ptr, size_t len);

#if __ANDROID__
//android workaround
#define eprintf(...) printf(__VA_ARGS__);
#else
#define eprintf(...) fprintf(stderr, __VA_ARGS__);
#endif

#if defined(WINDOWS)
#define UNUSED
// __attribute__ not supported in VS
#else
#define UNUSED __attribute__((unused))
#endif

#endif
