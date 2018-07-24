#include <stdio.h>
#include "../api.h"
#include "../randombytes.h"
#include "speed.h"
#include "cpucycles.h"

#define MLEN 59
#define NTESTS 5000

int main(void)
{
  unsigned int i;
  int ret;
  unsigned long long j, mlen, smlen;
  unsigned char m[MLEN];
  unsigned char sm[MLEN + CRYPTO_BYTES];
  unsigned char m2[MLEN + CRYPTO_BYTES];
  unsigned char pk[CRYPTO_PUBLICKEYBYTES];
  unsigned char sk[CRYPTO_SECRETKEYBYTES];
  unsigned long long t0[NTESTS], t1[NTESTS], t2[NTESTS];

  for(i = 0; i < NTESTS; ++i) {
    randombytes(m, MLEN);

    t0[i] = cpucycles();
    crypto_sign_keypair(pk, sk);
    t0[i] = cpucycles() - t0[i];

    t1[i] = cpucycles();
    crypto_sign(sm, &smlen, m, MLEN, sk);
    t1[i] = cpucycles() - t1[i];

    t2[i] = cpucycles();
    ret = crypto_sign_open(m2, &mlen, sm, smlen, pk);
    t2[i] = cpucycles() - t2[i];

    if(ret) {
      printf("Verification failed\n");
      return -1;
    }

    if(mlen != MLEN) {
      printf("Message lengths don't match\n");
      return -1;
    }

    for(j = 0; j < mlen; ++j) {
      if(m[j] != m2[j]) {
        printf("Messages don't match\n");
        return -1;
      }
    }
  }

  print_results("keygen:", t0, NTESTS);
  print_results("sign: ", t1, NTESTS);
  print_results("verify: ", t2, NTESTS);
  
  return 0;
}
