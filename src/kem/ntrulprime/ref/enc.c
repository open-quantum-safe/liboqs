#ifdef KAT
#include <stdio.h>
#endif

#include "hide.h"
#include "randombytes.h"
#include "crypto_kem.h"

int crypto_kem_enc(
    unsigned char *cstr,
    unsigned char *k,
    const unsigned char *pk) {
	unsigned char r[32];
	randombytes(r, 32);

#ifdef KAT
	{
		int i;
		printf("encrypt r: ");
		for (i = 0; i < 32; ++i)
			printf("%02x", 255 & (int) r[i]);
		printf("\n");
	}
#endif

	hide(cstr, k, pk, r);
	return 0;
}
