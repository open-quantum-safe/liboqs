#include "api.h"
#include "fips202.h"
#include "indcpa.h"
#include "params.h"
#include "randombytes.h"
#include "verify.h"

/*************************************************
 * Name:        crypto_kem_keypair
 *
 * Description: Generates public and private key
 *              for CCA-secure Kyber key encapsulation mechanism
 *
 * Arguments:   - unsigned char *pk: pointer to output public key (an already
 *allocated array of CRYPTO_PUBLICKEYBYTES bytes)
 *              - unsigned char *sk: pointer to output private key (an already
 *allocated array of CRYPTO_SECRETKEYBYTES bytes)
 *
 * Returns 0 (success)
 **************************************************/
int PQCLEAN_KYBER768_crypto_kem_keypair(unsigned char *pk, unsigned char *sk) {
    size_t i;
    PQCLEAN_KYBER768_indcpa_keypair(pk, sk);
    for (i = 0; i < KYBER_INDCPA_PUBLICKEYBYTES; i++) {
        sk[i + KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
    }
    sha3_256(sk + KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);
    randombytes(sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES);             /* Value z for pseudo-random output on reject */
    return 0;
}

/*************************************************
 * Name:        crypto_kem_enc
 *
 * Description: Generates cipher text and shared
 *              secret for given public key
 *
 * Arguments:   - unsigned char *ct:       pointer to output cipher text (an
 *already allocated array of CRYPTO_CIPHERTEXTBYTES bytes)
 *              - unsigned char *ss:       pointer to output shared secret (an
 *already allocated array of CRYPTO_BYTES bytes)
 *              - const unsigned char *pk: pointer to input public key (an
 *already allocated array of CRYPTO_PUBLICKEYBYTES bytes)
 *
 * Returns 0 (success)
 **************************************************/
int PQCLEAN_KYBER768_crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk) {
    unsigned char kr[2 * KYBER_SYMBYTES]; /* Will contain key, coins */
    unsigned char buf[2 * KYBER_SYMBYTES];

    randombytes(buf, KYBER_SYMBYTES);
    sha3_256(buf, buf, KYBER_SYMBYTES);                            /* Don't release system RNG output */

    sha3_256(buf + KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);      /* Multitarget countermeasure for coins + contributory KEM */
    sha3_512(kr, buf, 2 * KYBER_SYMBYTES);

    PQCLEAN_KYBER768_indcpa_enc(ct, buf, pk, kr + KYBER_SYMBYTES); /* coins are in kr+KYBER_SYMBYTES */

    sha3_256(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);      /* overwrite coins in kr with H(c) */
    sha3_256(ss, kr, 2 * KYBER_SYMBYTES);                          /* hash concatenation of pre-k and H(c) to k */
    return 0;
}

/*************************************************
 * Name:        crypto_kem_dec
 *
 * Description: Generates shared secret for given
 *              cipher text and private key
 *
 * Arguments:   - unsigned char *ss:       pointer to output shared secret (an
 *already allocated array of CRYPTO_BYTES bytes)
 *              - const unsigned char *ct: pointer to input cipher text (an
 *already allocated array of CRYPTO_CIPHERTEXTBYTES bytes)
 *              - const unsigned char *sk: pointer to input private key (an
 *already allocated array of CRYPTO_SECRETKEYBYTES bytes)
 *
 * Returns 0.
 *
 * On failure, ss will contain a pseudo-random value.
 **************************************************/
int PQCLEAN_KYBER768_crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk) {
    size_t i;
    int fail;
    unsigned char cmp[KYBER_CIPHERTEXTBYTES];
    unsigned char buf[2 * KYBER_SYMBYTES];
    unsigned char
    kr[2 * KYBER_SYMBYTES];                                                                                     /* Will contain key, coins, qrom-hash */
    const unsigned char *pk = sk + KYBER_INDCPA_SECRETKEYBYTES;

    PQCLEAN_KYBER768_indcpa_dec(buf, ct, sk);

    for (i = 0; i < KYBER_SYMBYTES; i++) {                                                                      /* Multitarget countermeasure for coins + contributory KEM */
        buf[KYBER_SYMBYTES + i] = sk[KYBER_SECRETKEYBYTES - 2 * KYBER_SYMBYTES + i];                            /* Save hash by storing H(pk) in sk */
    }
    sha3_512(kr, buf, 2 * KYBER_SYMBYTES);

    PQCLEAN_KYBER768_indcpa_enc(cmp, buf, pk, kr + KYBER_SYMBYTES);                                             /* coins are in kr+KYBER_SYMBYTES */

    fail = PQCLEAN_KYBER768_verify(ct, cmp, KYBER_CIPHERTEXTBYTES);

    sha3_256(kr + KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                                                   /* overwrite coins in kr with H(c)  */

    PQCLEAN_KYBER768_cmov(kr, sk + KYBER_SECRETKEYBYTES - KYBER_SYMBYTES, KYBER_SYMBYTES, (unsigned char)fail); /* Overwrite pre-k with z on re-encryption failure */

    sha3_256(ss, kr, 2 * KYBER_SYMBYTES);                                                                       /* hash concatenation of pre-k and H(c) to k */

    return 0;
}
