/**
 * @file crypto_memset.c
 * @brief Implementation of safe memset
 */
#include <string.h>
#include "crypto_memset.h"

void *(*volatile memset_volatile)(void *, int, size_t) = memset;
