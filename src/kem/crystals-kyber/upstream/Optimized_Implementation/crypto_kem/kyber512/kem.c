#include "api.h"
#include "rng.h"
#include "fips202.h"
#include "params.h"
#include "verify.h"
#include "indcpa.h"

// API FUNCTIONS 

/* Build a CCA-secure KEM from an IND-CPA-secure encryption scheme */

int crypto_kem_keypair(unsigned char *pk, unsigned char *sk)
{
  size_t i;
  indcpa_keypair(pk, sk);
  for(i=0;i<KYBER_INDCPA_PUBLICKEYBYTES;i++)
    sk[i+KYBER_INDCPA_SECRETKEYBYTES] = pk[i];
  sha3_256(sk+KYBER_SECRETKEYBYTES-2*KYBER_SYMBYTES,pk,KYBER_PUBLICKEYBYTES);
  randombytes(sk+KYBER_SECRETKEYBYTES-KYBER_SYMBYTES,KYBER_SYMBYTES);         /* Value z for pseudo-random output on reject */
  return 0;
}

int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{
  unsigned char  kr[2*KYBER_SYMBYTES];                                        /* Will contain key, coins */
  unsigned char buf[2*KYBER_SYMBYTES];                          

  randombytes(buf, KYBER_SYMBYTES);
  sha3_256(buf,buf,KYBER_SYMBYTES);                                           /* Don't release system RNG output */

  sha3_256(buf+KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);                     /* Multitarget countermeasure for coins + contributory KEM */
  sha3_512(kr, buf, 2*KYBER_SYMBYTES);

  indcpa_enc(ct, buf, pk, kr+KYBER_SYMBYTES);                                 /* coins are in kr+KYBER_SYMBYTES */

  sha3_256(kr+KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                     /* overwrite coins in kr with H(c) */
  sha3_256(ss, kr, 2*KYBER_SYMBYTES);                                         /* hash concatenation of pre-k and H(c) to k */
  return 0;
}

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{
  size_t i; 
  int fail;
  unsigned char cmp[KYBER_CIPHERTEXTBYTES];
  unsigned char buf[2*KYBER_SYMBYTES];
  unsigned char kr[2*KYBER_SYMBYTES];                                         /* Will contain key, coins, qrom-hash */
  const unsigned char *pk = sk+KYBER_INDCPA_SECRETKEYBYTES;

  indcpa_dec(buf, ct, sk);

  // shake256(buf+KYBER_SYMBYTES, KYBER_SYMBYTES, pk, KYBER_PUBLICKEYBYTES);  /* Multitarget countermeasure for coins + contributory KEM */
  for(i=0;i<KYBER_SYMBYTES;i++)                                               /* Save hash by storing H(pk) in sk */
    buf[KYBER_SYMBYTES+i] = sk[KYBER_SECRETKEYBYTES-2*KYBER_SYMBYTES+i];
  sha3_512(kr, buf, 2*KYBER_SYMBYTES);

  indcpa_enc(cmp, buf, pk, kr+KYBER_SYMBYTES);                                /* coins are in kr+KYBER_SYMBYTES */

  fail = verify(ct, cmp, KYBER_CIPHERTEXTBYTES);

  sha3_256(kr+KYBER_SYMBYTES, ct, KYBER_CIPHERTEXTBYTES);                     /* overwrite coins in kr with H(c)  */

  cmov(kr, sk+KYBER_SECRETKEYBYTES-KYBER_SYMBYTES, KYBER_SYMBYTES, fail);     /* Overwrite pre-k with z on re-encryption failure */

  sha3_256(ss, kr, 2*KYBER_SYMBYTES);                                         /* hash concatenation of pre-k and H(c) to k */

  return -fail;
}
