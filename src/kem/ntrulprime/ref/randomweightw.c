#include "params.h"
#include "randombytes.h"
#include "int32_sort.h"
#include "small.h"
#include "crypto_stream_aes256ctr.h"

const unsigned char n[16] = {0};

void small_seeded_weightw(small *f, const unsigned char *k) {
	crypto_int32 r[p];
	int i;

	crypto_stream_aes256ctr((unsigned char *) r, sizeof r, n, k);
	for (i = 0; i < p; ++i)
		r[i] ^= 0x80000000;

	for (i = 0; i < w; ++i)
		r[i] &= -2;
	for (i = w; i < p; ++i)
		r[i] = (r[i] & -3) | 1;
	int32_sort(r, p);
	for (i = 0; i < p; ++i)
		f[i] = ((small)(r[i] & 3)) - 1;
}

void small_random_weightw(small *f) {
	unsigned char k[32];
	randombytes(k, 32);
	small_seeded_weightw(f, k);
}
