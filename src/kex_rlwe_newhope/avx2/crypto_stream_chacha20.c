#include "crypto_stream_chacha20.h"
#include <stddef.h>

extern void OQS_chacha_avx2(const unsigned char *k, const unsigned char *n, const unsigned char *in, unsigned char *out, size_t inlen, size_t rounds);

int OQS_crypto_stream_chacha20(unsigned char *out, unsigned long long outlen, const unsigned char *n, const unsigned char *k)
{
	OQS_chacha_avx2(k, n, NULL, out, (size_t)outlen, 20);
	return 0;
}
