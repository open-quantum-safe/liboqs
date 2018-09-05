/* ****************************** *
 * Titanium_CCA_super             *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * CCA KEM                        *
 * ****************************** */
 
#include "kem.h"
#include "encrypt.h"
#include "api.h"
#include "param.h"
#include "fastrandombytes.h"
#include "rng.h"
#include "keccak.h"
#include <string.h>
#include <stdint.h>

#define HASH_LENGTH 32 /* 256-bit hash */

/* PK <-- key_cpa(pk)
 * SK <-- seed_sk_cca || PK */
int crypto_kem_keypair(unsigned char *pk, unsigned char *sk)
{
	unsigned char seed[CRYPTO_RANDOMBYTES];
	unsigned char buf[CRYPTO_RANDOMBYTES + D_BYTES];
	unsigned char sk_cpa[CRYPTO_RANDOMBYTES];
	
	randombytes(seed, CRYPTO_RANDOMBYTES);
	
	fastrandombytes_setseed(seed);
	fastrandombytes(buf, CRYPTO_RANDOMBYTES + D_BYTES);
	
	crypto_encrypt_keypair(pk, sk_cpa, buf);
	
	memcpy(sk, seed, CRYPTO_RANDOMBYTES);
	memcpy(sk + CRYPTO_RANDOMBYTES, pk, CRYPTO_PUBLICKEYBYTES);
	
	return 0;
}

int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{
	unsigned char buf[D_BYTES + HASH_LENGTH * 2];
	unsigned char hash[CRYPTO_RANDOMBYTES + HASH_LENGTH];
	unsigned long long ct_len;
	
	/* generate a random message m */
	randombytes(buf, D_BYTES);
	
	/* (h1,h2) <-- shake(m) */
	SHAKE256(hash, CRYPTO_RANDOMBYTES + HASH_LENGTH, buf, D_BYTES);
	
	/* use h1 as the seed to encrypt m */
	crypto_encrypt(ct, &ct_len, buf, D_BYTES, pk, hash);
	
	/* ct <-- ct || h2 */
	memcpy(ct + ct_len, hash + CRYPTO_RANDOMBYTES, HASH_LENGTH);
	
	/* ss <-- shake(m, shake(ct))
	 * might be faster than memcpy(ct) and ss <-- shake(m, ct) */
	SHAKE256(buf + D_BYTES, HASH_LENGTH * 2, ct, CRYPTO_CIPHERTEXTBYTES);
	SHAKE256(ss, CRYPTO_BYTES, buf, D_BYTES + HASH_LENGTH * 2);

	return 0;
}

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{
	unsigned char buf[D_BYTES + HASH_LENGTH * 2];
	unsigned char hash[CRYPTO_RANDOMBYTES + HASH_LENGTH];
	unsigned char ct_prime[CRYPTO_CIPHERTEXTBYTES];
	unsigned long long ct_prime_len;
	unsigned long long m_prime_len;
	unsigned char sk_cca[CRYPTO_RANDOMBYTES + D_BYTES], sk_cpa[CRYPTO_RANDOMBYTES];
	
	uint32_t i;
	uint32_t cmp;

	/* recover the sk_cpa */
	fastrandombytes_setseed(sk);
	fastrandombytes(sk_cca, CRYPTO_RANDOMBYTES + D_BYTES);
	fastrandombytes_setseed(sk_cca);
	fastrandombytes(sk_cpa, CRYPTO_RANDOMBYTES);
	
	/* decrypt ct to get m' */
	crypto_encrypt_open(buf, &m_prime_len, ct, CRYPTO_CIPHERTEXTBYTES, sk_cpa);
	
	/* (h1',h2') <-- shake(m') */ 
	SHAKE256(hash, CRYPTO_RANDOMBYTES + HASH_LENGTH, buf, m_prime_len);	
	
	/* re-encrypt m' to ct' by using the PK stored in SK and the seed h1' */
	crypto_encrypt(ct_prime, &ct_prime_len, buf, m_prime_len, sk + CRYPTO_RANDOMBYTES, hash);
	
	/* ct' <-- ct' || h2' */
	memcpy(ct_prime + ct_prime_len, hash + CRYPTO_RANDOMBYTES, HASH_LENGTH);
	
	/* constant time cmp = (ct' == ct) */
	cmp = 0;
	for (i = 0; i < CRYPTO_CIPHERTEXTBYTES; i++)
	{
		cmp |= ct_prime[i] ^ ct[i];
	}
	cmp = -((-cmp) >> 31);
	
	/* constant time x = cmp ? (r, shake(ct)) : (m', shake(ct)) */
	SHAKE256(buf + D_BYTES, HASH_LENGTH * 2, ct, CRYPTO_CIPHERTEXTBYTES);
	
	for (i = 0; i < D_BYTES; i++)
	{
		buf[i] ^= cmp & (sk_cca[CRYPTO_RANDOMBYTES + i] ^ buf[i]);
	}
	
	/* ss <-- shake(x) */
	SHAKE256(ss, CRYPTO_BYTES, buf, D_BYTES + HASH_LENGTH * 2);
	
	return 0;
}
