/**
 * @file crypto_memset.c
 * @brief Implementation of safe memset
 */
#include <string.h>

void *(*volatile memset_volatile)(void *, int, size_t) = memset;
