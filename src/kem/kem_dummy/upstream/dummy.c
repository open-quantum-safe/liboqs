#include <string.h>

#include "api.h"

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
	memset(pk, 0, CRYPTO_PUBLICKEYBYTES);
	memset(sk, 0, CRYPTO_SECRETKEYBYTES);
	strncpy((char *) pk, "dummy public key", CRYPTO_PUBLICKEYBYTES - 1);
	strncpy((char *) sk, "dummy secret key", CRYPTO_SECRETKEYBYTES - 1 );
	return 0;
}

int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
	memset(ct, 0, CRYPTO_CIPHERTEXTBYTES);
	memset(ss, 0, CRYPTO_BYTES);
	strncpy((char *) ct, "dummy ciphertext", CRYPTO_CIPHERTEXTBYTES - 1);
	strncpy((char *) ss, "dummy shared secret", CRYPTO_BYTES - 1);
	return 0;
}

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
	memset(ss, 0, CRYPTO_BYTES);
	strncpy((char *) ss, "dummy shared secret", CRYPTO_BYTES - 1);
	return 0;
}

int another_global_function() {
	return -1;
}
