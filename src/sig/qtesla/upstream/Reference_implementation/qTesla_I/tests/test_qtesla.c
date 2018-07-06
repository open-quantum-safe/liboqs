/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: testing and benchmarking code
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../random/random.h"
#include "cpucycles.h"
#include "../api.h"
#include "../poly.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MLEN 59
#define NRUNS 5000
#define NTESTS 100


static int cmp_llu(const void *a, const void*b)
{
  if (*(unsigned long long *)a < *(unsigned long long *)b) return -1;
  if (*(unsigned long long *)a > *(unsigned long long *)b) return 1;
  return 0;
}


static unsigned long long median(unsigned long long *l, size_t llen)
{
  qsort(l,llen,sizeof(unsigned long long),cmp_llu);

  if (llen%2) return l[llen/2];
  else return (l[llen/2-1]+l[llen/2])/2;
}


static unsigned long long average(unsigned long long *t, size_t tlen)
{
  unsigned long long acc=0;
  size_t i;
  for (i=0; i<tlen; i++)
    acc += t[i];
  return acc/(tlen);
}


static void print_results(const char *s, unsigned long long *t, size_t tlen)
{
  size_t i;
  printf("%s", s);
  printf("\n");
  printf("median:  %llu cycles\n", median(t, tlen));
  printf("average: %llu cycles\n", average(t, tlen-1));
  printf("\n");
}


unsigned char mi[MLEN];
unsigned char mo[MLEN+CRYPTO_BYTES];
unsigned char sm[MLEN+CRYPTO_BYTES];
unsigned char pk[CRYPTO_PUBLICKEYBYTES];
unsigned char sk[CRYPTO_SECRETKEYBYTES];
unsigned long long smlen, mlen;

extern unsigned long long rejwctr;
extern unsigned long long rejyzctr;
extern unsigned long long ctr_keygen;
extern unsigned long long ctr_sign;

static int fd = -1;


void rndbytes(unsigned char *x,unsigned long long xlen)
{
  int i;

  if (fd == -1) {
    for (;;) {
      fd = open("/dev/urandom",O_RDONLY);
      if (fd != -1) break;
      sleep(1);
    }
  }

  while (xlen > 0) {
    if (xlen < 1048576) i = xlen; else i = 1048576;
    i = read(fd,x,i);
    if (i < 1) {
      sleep(1);
      continue;
    }
    x += i;
    xlen -= i;
  }
}


#ifdef DEBUG  

int print_accrates()
{
  int r;
  double rejw=.0, rejyz=.0, rejctr=.0, rejctrkg=.0;
  unsigned long long i, j;

  for (i=0; i<NTESTS; i++){
    crypto_sign_keypair(pk, sk);
    rejctrkg+=ctr_keygen;
  }

  // Print acceptance rate for keygen. The counter increased by PARAM_K for each try
  printf("Acceptance rate of Keygen : %.2f\n", (double)((PARAM_K+1)*NTESTS)/((double)rejctrkg)); fflush(stdout);
 
  for (i=0; i<NTESTS; i++)
  {
    randombytes(mi, MLEN);
    crypto_sign(sm, &smlen, mi, MLEN, sk);    
    rejctr+=ctr_sign;
    rejw+=rejwctr;
    rejyz+=rejyzctr;
  }
  
  printf("Acceptance rate of v\t  : %.2f\n",1/((rejw/NTESTS)+1));
  printf("Acceptance rate of z\t  : %.2f\n",1/((rejyz/(NTESTS+rejw))+1));
  printf("Acceptance rate of Signing: %.2f\n",(double)NTESTS/rejctr);
  printf("\n");
 
  return 0;
}

#endif


int main(void)
{
  unsigned int i, j;
  unsigned char r;
  unsigned long long cycles0[NRUNS], cycles1[NRUNS], cycles2[NRUNS];
  int valid, response;
    
  printf("\n");
  printf("===========================================================================================\n");
  printf("Testing signature scheme qTESLA, system %s, tests for %d iterations\n", CRYPTO_ALGNAME, NRUNS);
  printf("===========================================================================================\n");

  printf("\nCRYPTO_PUBLICKEY_BYTES: %d\n", CRYPTO_PUBLICKEYBYTES);
  printf("CRYPTO_SECRETKEY_BYTES: %lu\n", CRYPTO_SECRETKEYBYTES);
  printf("CRYPTO_SIGNATURE_BYTES: %d\n\n", CRYPTO_BYTES);

#ifdef DEBUG  
  print_accrates();
#endif

  for (i = 0; i < NRUNS; i++) {
    randombytes(mi, MLEN);

    cycles0[i] = cpucycles();
    crypto_sign_keypair(pk, sk);
    cycles0[i] = cpucycles() - cycles0[i];

    cycles1[i] = cpucycles();
    crypto_sign(sm, &smlen, mi, MLEN, sk);
    cycles1[i] = cpucycles() - cycles1[i];

    cycles2[i] = cpucycles();
    valid = crypto_sign_open(mo, &mlen, sm, smlen, pk);
    cycles2[i] = cpucycles() - cycles2[i];

    if (valid != 0) {
      printf("Signature verification FAILED. \n");
      return -1;
    } else if (mlen != MLEN) {
      printf("crypto_sign_open returned BAD message length. \n");
      return -1;
    }

    for (j = 0; j < mlen; j++) {
      if (mi[j] != mo[j]) {
        printf ("crypto_sign_open returned BAD message value. \n");
        return -1;
      }
    }

    // Change something in the signature somewhere    
    randombytes(&r, 1);
    sm[r % (MLEN+CRYPTO_BYTES)] ^= 1;
    response = crypto_sign_open(mo, &mlen, sm, smlen, pk);
    if (response == 0) {
      printf("Corrupted signature VERIFIED. \n");
      return -1;
    }
  }
  printf("Signature tests PASSED... \n\n");

  print_results("qTESLA keygen: ", cycles0, NRUNS);
  print_results("qTESLA sign: ", cycles1, NRUNS);
  print_results("qTESLA verify: ", cycles2, NRUNS);

  return 0;
}
