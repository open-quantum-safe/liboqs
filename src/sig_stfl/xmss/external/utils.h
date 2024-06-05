// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0
#ifndef XMSS_UTILS_H
#define XMSS_UTILS_H

#include "namespace.h"
#include <oqs/common.h>
/**
 * Converts the value of 'in' to 'outlen' bytes in big-endian byte order.
 */
#define ull_to_bytes XMSS_INNER_NAMESPACE(ull_to_bytes)
void ull_to_bytes(unsigned char *out, unsigned int outlen,
                  unsigned long long in);

/**
 * Converts the inlen bytes in 'in' from big-endian byte order to an integer.
 */
#define bytes_to_ull XMSS_INNER_NAMESPACE(bytes_to_ull)
unsigned long long bytes_to_ull(const unsigned char *in, unsigned int inlen);

#endif
