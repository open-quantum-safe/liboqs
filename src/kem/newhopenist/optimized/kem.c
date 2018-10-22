#include <string.h>
#include "api.h"
#include "cpapke.h"
#include "params.h"
#include "verify.h"

#include <oqs/rand.h>
#include <oqs/sha3.h>

/*************************************************
* Name:        crypto_kem_keypair
*
* Description: Generates public and private key
*              for CCA secure NewHope key encapsulation
*              mechanism
*
* Arguments:   - unsigned char *pk: pointer to output public key (an already allocated array of CRYPTO_PUBLICKEYBYTES bytes)
*              - unsigned char *sk: pointer to output private key (an already allocated array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
OQS_API OQS_STATUS crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
	size_t i;

	cpapke_keypair(pk, sk);
	sk += NEWHOPE_CPAPKE_SECRETKEYBYTES;

	for (i = 0; i < NEWHOPE_CPAPKE_PUBLICKEYBYTES; i++) /* Append the public key for re-encryption */
		sk[i] = pk[i];
	sk += NEWHOPE_CPAPKE_PUBLICKEYBYTES;

	OQS_SHA3_shake256(sk, NEWHOPE_SYMBYTES, pk, NEWHOPE_CPAPKE_PUBLICKEYBYTES); /* Append the hash of the public key */
	sk += NEWHOPE_SYMBYTES;

	OQS_randombytes(sk, NEWHOPE_SYMBYTES); /* Append the value s for pseudo-random output on reject */

	return OQS_SUCCESS;
}

/*************************************************
* Name:        crypto_kem_enc
*
* Description: Generates cipher text and shared
*              secret for given public key
*
* Arguments:   - unsigned char *ct:       pointer to output cipher text (an already allocated array of CRYPTO_CIPHERTEXTBYTES bytes)
*              - unsigned char *ss:       pointer to output shared secret (an already allocated array of CRYPTO_BYTES bytes)
*              - const unsigned char *pk: pointer to input public key (an already allocated array of CRYPTO_PUBLICKEYBYTES bytes)
*
* Returns 0 (success)
**************************************************/
OQS_API OQS_STATUS crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
	unsigned char k_coins_d[3 * NEWHOPE_SYMBYTES]; /* Will contain key, coins, qrom-hash */
	unsigned char buf[2 * NEWHOPE_SYMBYTES];
	int i;

	OQS_randombytes(buf, NEWHOPE_SYMBYTES);

	OQS_SHA3_shake256(buf, NEWHOPE_SYMBYTES, buf, NEWHOPE_SYMBYTES);                                /* Don't release system RNG output */
	OQS_SHA3_shake256(buf + NEWHOPE_SYMBYTES, NEWHOPE_SYMBYTES, pk, NEWHOPE_CCAKEM_PUBLICKEYBYTES); /* Multitarget countermeasure for coins + contributory KEM */
	OQS_SHA3_shake256(k_coins_d, 3 * NEWHOPE_SYMBYTES, buf, 2 * NEWHOPE_SYMBYTES);

	cpapke_enc(ct, buf, pk, k_coins_d + NEWHOPE_SYMBYTES); /* coins are in k_coins_d+NEWHOPE_SYMBYTES */

	for (i = 0; i < NEWHOPE_SYMBYTES; i++)
		ct[i + NEWHOPE_CPAPKE_CIPHERTEXTBYTES] = k_coins_d[i + 2 * NEWHOPE_SYMBYTES]; /* copy Targhi-Unruh hash into ct */

	OQS_SHA3_shake256(k_coins_d + NEWHOPE_SYMBYTES, NEWHOPE_SYMBYTES, ct, NEWHOPE_CCAKEM_CIPHERTEXTBYTES); /* overwrite coins in k_coins_d with h(c) */
	OQS_SHA3_shake256(ss, NEWHOPE_SYMBYTES, k_coins_d, 2 * NEWHOPE_SYMBYTES);                              /* hash concatenation of pre-k and h(c) to ss */
	return OQS_SUCCESS;
}

/*************************************************
* Name:        crypto_kem_dec
*
* Description: Generates shared secret for given
*              cipher text and private key
*
* Arguments:   - unsigned char *ss:       pointer to output shared secret (an already allocated array of CRYPTO_BYTES bytes)
*              - const unsigned char *ct: pointer to input cipher text (an already allocated array of CRYPTO_CIPHERTEXTBYTES bytes)
*              - const unsigned char *sk: pointer to input private key (an already allocated array of CRYPTO_SECRETKEYBYTES bytes)
*
* Returns 0 for sucess or -1 for failure
*
* On failure, ss will contain a randomized value.
**************************************************/
OQS_API OQS_STATUS crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
	int i, fail;
	unsigned char ct_cmp[NEWHOPE_CCAKEM_CIPHERTEXTBYTES];
	unsigned char buf[2 * NEWHOPE_SYMBYTES];
	unsigned char k_coins_d[3 * NEWHOPE_SYMBYTES]; /* Will contain key, coins, qrom-hash */
	const unsigned char *pk = sk + NEWHOPE_CPAPKE_SECRETKEYBYTES;

	cpapke_dec(buf, ct, sk);

	for (i = 0; i < NEWHOPE_SYMBYTES; i++) /* Use hash of pk stored in sk */
		buf[NEWHOPE_SYMBYTES + i] = sk[NEWHOPE_CCAKEM_SECRETKEYBYTES - 2 * NEWHOPE_SYMBYTES + i];
	OQS_SHA3_shake256(k_coins_d, 3 * NEWHOPE_SYMBYTES, buf, 2 * NEWHOPE_SYMBYTES);

	cpapke_enc(ct_cmp, buf, pk, k_coins_d + NEWHOPE_SYMBYTES); /* coins are in k_coins_d+NEWHOPE_SYMBYTES */

	for (i = 0; i < NEWHOPE_SYMBYTES; i++)
		ct_cmp[i + NEWHOPE_CPAPKE_CIPHERTEXTBYTES] = k_coins_d[i + 2 * NEWHOPE_SYMBYTES];

	fail = verify(ct, ct_cmp, NEWHOPE_CCAKEM_CIPHERTEXTBYTES);

	OQS_SHA3_shake256(k_coins_d + NEWHOPE_SYMBYTES, NEWHOPE_SYMBYTES, ct, NEWHOPE_CCAKEM_CIPHERTEXTBYTES); /* overwrite coins in k_coins_d with h(c)  */
	cmov(k_coins_d, sk + NEWHOPE_CCAKEM_SECRETKEYBYTES - NEWHOPE_SYMBYTES, NEWHOPE_SYMBYTES, fail);        /* Overwrite pre-k with z on re-encryption failure */
	OQS_SHA3_shake256(ss, NEWHOPE_SYMBYTES, k_coins_d, 2 * NEWHOPE_SYMBYTES);                              /* hash concatenation of pre-k and h(c) to k */

	return (OQS_STATUS) -fail;
}
