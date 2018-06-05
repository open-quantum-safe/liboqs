
#include "api.h"
#include "rng.h"

#include "LimaAPI.h"

int crypto_encrypt_keypair( unsigned char *pk, unsigned char *sk)
{
  unsigned char seed[48];
  randombytes(seed,48);
  return -Key_Generation(pk,CRYPTO_PUBLICKEYBYTES,sk,CRYPTO_SECRETKEYBYTES,seed,48);
}



int crypto_encrypt( unsigned char *c, unsigned long long *clen, 
                    const unsigned char *m, unsigned long long mlen,
                    const unsigned char *pk)
{
  unsigned char seed[32];
  randombytes(seed,32);
  return -Enc_CCA(c,clen,m,mlen,seed,pk);
}


int crypto_encrypt_open( unsigned char *m, unsigned long long *mlen, 
                         const unsigned char *c, unsigned long long clen,
                         const unsigned char *sk)
{
  return -Dec_CCA(m,mlen,c,sk);
}



