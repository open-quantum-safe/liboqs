#include <stdint.h>
#include <stdio.h>
#include "speed.h"
#include "cpucycles.h"
#include "../poly.h"
#include "../ntt.h"
#include "../reduce.h"
#include "../randombytes.h"

#define NTESTS 10000

static void poly_naivemul(poly *c, const poly *a, const poly *b) {
  unsigned int i,j;
  uint32_t r[2*N];

  for(i = 0; i < 2*N; i++)
    r[i] = 0;

  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++) {
      r[i+j] += ((uint64_t)a->coeffs[i] * b->coeffs[j]) % Q;
      r[i+j] %= Q;
    }

  for(i = N; i < 2*N-1; i++) {
    r[i-N] = r[i-N] + Q - r[i];
    r[i-N] %= Q;
  }

  for(i = 0; i < N; i++)
    c->coeffs[i] = r[i];
}

static void random_pol(poly *a) {
  unsigned int i;
  uint32_t t;

  i = 0;
  while(i < N) {
    randombytes((unsigned char *)&t, 4);
    t &= 0x7FFFFF;
    if(t < Q) a->coeffs[i++] = t;
  }
}

int main(void) {
  unsigned int i, j;
  unsigned long long t1[NTESTS], t2[NTESTS];
  poly a, b, c1, c2;
  
  for(i = 0; i < NTESTS; ++i) {
    random_pol(&a);
    random_pol(&b);

    t1[i] = cpucycles();
    poly_naivemul(&c1, &a, &b);
    t1[i] = cpucycles() - t1[i];

    t2[i] = cpucycles();
    ntt(a.coeffs, a.coeffs, zetas);
    ntt(b.coeffs, b.coeffs, zetas);
    pointwise_mul(c2.coeffs, a.coeffs, b.coeffs);
    invntt(c2.coeffs, c2.coeffs, zetas_inv);
    t2[i] = cpucycles() - t2[i];

    for(j = 0; j < N; ++j)
      if(c2.coeffs[j] % Q != c1.coeffs[j])
        printf("FAILURE: c2[%u]: %u %u\n", j, c1.coeffs[j], c2.coeffs[j]);
  }

  print_results("naive: ", t1, NTESTS);
  print_results("fft_avx: ", t2, NTESTS);
  return 0;
}
