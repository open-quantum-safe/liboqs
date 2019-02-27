#ifdef KAT
#include <stdio.h>
#endif

#include "params.h"
#include "small.h"
#include "rq.h"
#include "r3.h"
#include "hide.h"
#include "crypto_kem.h"

static int verify(const unsigned char *x, const unsigned char *y) {
	unsigned int differentbits = 0;
	int i;
	for (i = 0; i < crypto_kem_CIPHERTEXTBYTES; ++i)
		differentbits |= x[i] ^ y[i];
	return (1 & ((differentbits - 1) >> 8)) - 1;
}

int crypto_kem_dec(
    unsigned char *k,
    const unsigned char *cstr,
    const unsigned char *sk) {
	small a[p];
	modq B[p];
	modq aB[p];
	modq C[256];
	unsigned char r[32];
	unsigned char checkcstr[crypto_kem_CIPHERTEXTBYTES];
	unsigned char maybek[32];
	int i;
	int result;

	small_decode(a, sk);
	sk += small_encode_len;
	rq_decoderounded(B, cstr + 32);
	rq_mult(aB, B, a);

	for (i = 0; i < 128; ++i) {
		crypto_uint32 x = cstr[32 + rq_encoderounded_len + i];
		C[2 * i] = (x & 15) * 287 - 2007;
		C[2 * i + 1] = (x >> 4) * 287 - 2007;
	}

	for (i = 0; i < 256; ++i)
		C[i] = -(modq_freeze(C[i] - aB[i] + 4 * w + 1) >> 14);

	for (i = 0; i < 32; ++i)
		r[i] = 0;
	for (i = 0; i < 256; ++i)
		r[i / 8] |= (C[i] << (i & 7));

#ifdef KAT
	{
		int j;
		printf("decrypt r: ");
		for (j = 0; j < 32; ++j)
			printf("%02x", 255 & (int) r[j]);
		printf("\n");
	}
#endif

	hide(checkcstr, maybek, sk, r);
	result = verify(cstr, checkcstr);

	for (i = 0; i < 32; ++i)
		k[i] = maybek[i] & ~result;
	return result;
}
