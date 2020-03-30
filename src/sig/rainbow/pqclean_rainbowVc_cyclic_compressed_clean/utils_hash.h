#ifndef _UTILS_HASH_H_
#define _UTILS_HASH_H_
/// @file utils_hash.h
/// @brief the interface for adapting hash functions.
///

#include <stddef.h>

int PQCLEAN_RAINBOWVCCYCLICCOMPRESSED_CLEAN_hash_msg(unsigned char *digest, size_t len_digest, const unsigned char *m, size_t mlen);

#endif // _UTILS_HASH_H_
