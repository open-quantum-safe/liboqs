#include <string.h>
#include "modq.h"
#include "params.h"
#include "r3.h"
#include "small.h"
#include "rq.h"
#include "crypto_kem.h"
#include "randombytes.h"
#include "crypto_stream_aes256ctr.h"

#if crypto_kem_PUBLICKEYBYTES != rq_encoderounded_len + 32
#error "crypto_kem_PUBLICKEYBYTES must match rq_encoderounded_len + 32"
#endif
#if crypto_kem_SECRETKEYBYTES != small_encode_len + crypto_kem_PUBLICKEYBYTES
#error "crypto_kem_SECRETKEYBYTES must match small_encode_len + crypto_kem_PUBLICKEYBYTES"
#endif

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
	unsigned char K[32];
	modq G[p];
	small a[p];
	modq A[p];

	randombytes(K, 32);
	rq_fromseed(G, K);

	small_random_weightw(a);

	rq_mult(A, G, a);
	rq_round3(A, A);

	memcpy(pk, K, 32);
	rq_encoderounded(pk + 32, A);

	small_encode(sk, a);
	memcpy(sk + small_encode_len, pk, crypto_kem_PUBLICKEYBYTES);

	return 0;
}
