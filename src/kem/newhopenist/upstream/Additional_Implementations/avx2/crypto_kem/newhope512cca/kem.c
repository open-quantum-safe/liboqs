#include <string.h>
#include "api.h"
#include "cpapke.h"
#include "params.h"
#include "rng.h"
#include "fips202.h"
#include "verify.h"

 
int crypto_kem_keypair(unsigned char *pk, unsigned char *sk)
{
  size_t i;

  cpapke_keypair(pk, sk);                                                   /* First put the actual secret key into sk */
  sk += NEWHOPE_CPAPKE_SECRETKEYBYTES;

  for(i=0;i<NEWHOPE_CPAPKE_PUBLICKEYBYTES;i++)                              /* Append the public key for re-encryption */
    sk[i] = pk[i];
  sk += NEWHOPE_CPAPKE_PUBLICKEYBYTES;

  shake256(sk, NEWHOPE_SYMBYTES, pk, NEWHOPE_CPAPKE_PUBLICKEYBYTES);        /* Append the hash of the public key */
  sk += NEWHOPE_SYMBYTES;

  randombytes(sk, NEWHOPE_SYMBYTES);                                        /* Append the value z for pseudo-random output on reject */

  return 0;
}


int crypto_kem_enc(unsigned char *ct, unsigned char *ss, const unsigned char *pk)
{
  unsigned char krq[3*NEWHOPE_SYMBYTES];                                                /* Will contain key, coins, qrom-hash */
  unsigned char buf[2*NEWHOPE_SYMBYTES];
  int i;

  randombytes(buf,NEWHOPE_SYMBYTES);
  
  shake256(buf,NEWHOPE_SYMBYTES,buf,NEWHOPE_SYMBYTES);                                  /* Don't release system RNG output */
  shake256(buf+NEWHOPE_SYMBYTES, NEWHOPE_SYMBYTES, pk, NEWHOPE_CCAKEM_PUBLICKEYBYTES);  /* Multitarget countermeasure for coins + contributory KEM */
  shake256(krq, 3*NEWHOPE_SYMBYTES, buf, 2*NEWHOPE_SYMBYTES);

  cpapke_enc(ct, buf, pk, krq+NEWHOPE_SYMBYTES);                                        /* coins are in krq+NEWHOPE_SYMBYTES */

  for(i=0;i<NEWHOPE_SYMBYTES;i++)
    ct[i+NEWHOPE_CPAPKE_CIPHERTEXTBYTES] = krq[i+2*NEWHOPE_SYMBYTES];                   /* copy Targhi-Unruh hash into ct */

  shake256(krq+NEWHOPE_SYMBYTES, NEWHOPE_SYMBYTES, ct, NEWHOPE_CCAKEM_CIPHERTEXTBYTES); /* overwrite coins in krq with h(c) */
  shake256(ss, NEWHOPE_SYMBYTES, krq, 2*NEWHOPE_SYMBYTES);                              /* hash concatenation of pre-k and h(c) to ss */
  return 0;
}

int crypto_kem_dec(unsigned char *ss, const unsigned char *ct, const unsigned char *sk)
{
  int i, fail;
  unsigned char cmp[NEWHOPE_CCAKEM_CIPHERTEXTBYTES];
  unsigned char buf[2*NEWHOPE_SYMBYTES];
  unsigned char krq[3*NEWHOPE_SYMBYTES];                                         /* Will contain key, coins, qrom-hash */
  const unsigned char *pk = sk+NEWHOPE_CPAPKE_SECRETKEYBYTES;

  cpapke_dec(buf, ct, sk);

  for(i=0;i<NEWHOPE_SYMBYTES;i++)                                                /* Use hash of pk stored in sk */
    buf[NEWHOPE_SYMBYTES+i] = sk[NEWHOPE_CCAKEM_SECRETKEYBYTES-2*NEWHOPE_SYMBYTES+i];
  shake256(krq, 3*NEWHOPE_SYMBYTES, buf, 2*NEWHOPE_SYMBYTES);

  cpapke_enc(cmp, buf, pk, krq+NEWHOPE_SYMBYTES);                                /* coins are in krq+NEWHOPE_SYMBYTES */

  for(i=0;i<NEWHOPE_SYMBYTES;i++)
    cmp[i+NEWHOPE_CPAPKE_CIPHERTEXTBYTES] = krq[i+2*NEWHOPE_SYMBYTES];

  fail = verify(ct, cmp, NEWHOPE_CCAKEM_CIPHERTEXTBYTES);

  shake256(krq+NEWHOPE_SYMBYTES, NEWHOPE_SYMBYTES, ct, NEWHOPE_CCAKEM_CIPHERTEXTBYTES); /* overwrite coins in krq with h(c)  */
  cmov(krq, sk+NEWHOPE_CCAKEM_SECRETKEYBYTES-NEWHOPE_SYMBYTES, NEWHOPE_SYMBYTES, fail); /* Overwrite pre-k with z on re-encryption failure */
  shake256(ss, NEWHOPE_SYMBYTES, krq, 2*NEWHOPE_SYMBYTES);                       /* hash concatenation of pre-k and h(c) to k */

  return -fail;
}
