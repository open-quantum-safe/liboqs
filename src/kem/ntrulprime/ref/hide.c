#include <stdio.h>
#include <string.h>
#include "crypto_hash_sha512.h"
#include "crypto_kem.h"
#include "params.h"
#include "rq.h"
#include "hide.h"

#if crypto_kem_CIPHERTEXTBYTES != rq_encoderounded_len + 32 + 128
#error "crypto_kem_CIPHERTEXTBYTES must match rq_encoderounded_len + 32 + 128"
#endif

void hide(unsigned char *cstr, unsigned char *k, const unsigned char *pk, const unsigned char *r) {
	modq G[p];
	modq A[p];
	unsigned char k12[64];
	unsigned char k34[64];
	small b[p];
	modq B[p];
	modq C[p];
	int i;

	rq_fromseed(G, pk);
	rq_decoderounded(A, pk + 32);

	crypto_hash_sha512(k12, r, 32);
	small_seeded_weightw(b, k12);
	crypto_hash_sha512(k34, k12 + 32, 32);

	rq_mult(B, G, b);
	rq_round3(B, B);

	rq_mult(C, A, b);
	for (i = 0; i < 256; ++i) {
		modq x = C[i];
		x = modq_sum(x, 2295 * (1 & (r[i / 8] >> (i & 7))));
		x = ((x + 2156) * 114 + 16384) >> 15;
		C[i] = x; /* between 0 and 15 */
	}

	memcpy(cstr, k34, 32);
	cstr += 32;
	memcpy(k, k34 + 32, 32);

	rq_encoderounded(cstr, B);
	cstr += rq_encoderounded_len;

	for (i = 0; i < 128; ++i)
		*cstr++ = C[2 * i] + (C[2 * i + 1] << 4);
}
