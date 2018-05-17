
#include "api.h"
#include "rng.h"

#include "LimaAPI.h"

int crypto_kem_keypair( unsigned char *pk, unsigned char *sk)
{
  unsigned char seed[48];
  randombytes(seed,48);
  return -Key_Generation(pk,CRYPTO_PUBLICKEYBYTES,sk,CRYPTO_SECRETKEYBYTES,seed,48);
}


int crypto_kem_enc( unsigned char *ct, unsigned char *ss, const unsigned char 
*pk)
{
  byte_length clen;
  unsigned char seed[80];
  randombytes(seed,80);
  int fl=Encap_CPA(ct,&clen,ss,CRYPTO_BYTES,seed,80,pk);
  if (clen!=CRYPTO_CIPHERTEXTBYTES) { fl=-1; }
  return fl;
}


int crypto_kem_dec( unsigned char *ss, const unsigned char *ct, const unsigned
 char *sk)
{
  byte_length  kl;
  int fl=Decap_CPA(ss,&kl,ct,sk);
  if (kl!=CRYPTO_BYTES) { fl=-1; }
  return fl;
}


