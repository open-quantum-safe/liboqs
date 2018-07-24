#include <stdio.h>
#include "../params.h"
#include "../sign.h"
#include "../poly.h"
#include "../polyvec.h"
#include "../rng.h"

#define NVECTORS 10

int main(void) {
  unsigned int i, j, k, l;
  unsigned char seed[SEEDBYTES + CRHBYTES];
  poly c;
  polyvecl s, y, mat[K];
  polyveck w, tmp;
  int32_t u;

  for (i = 0; i < 48; ++i)
    seed[i] = i;

  randombytes_init(seed, NULL, 256);

  for(i = 0; i < NVECTORS; ++i) {
    printf("count = %u\n", i);

    randombytes(seed, sizeof(seed));
    printf("seed = ");
    for(j = 0; j < sizeof(seed); ++j)
      printf("%.2hhX", seed[j]);
    printf("\n");

    expand_mat(mat, seed);
    printf("A = ((");
    for(j = 0; j < K; ++j) {
      for(k = 0; k < L; ++k) {
        for(l = 0; l < N; ++l) {
          printf("%6d", mat[j].vec[k].coeffs[l]);
          if(l < N-1) printf(", ");
          else if(k < L-1) printf("), (");
          else if(j < K-1) printf(");\n     (");
          else printf("))\n");
        }
      }
    }

#if L == 2
    poly_uniform_eta_4x(&s.vec[0], &s.vec[1], &tmp.vec[0], &tmp.vec[1], seed,
                        0, 1, 2, 3);
#elif L == 3
    poly_uniform_eta_4x(&s.vec[0], &s.vec[1], &s.vec[2], &tmp.vec[0], seed,
                        0, 1, 2, 3);
#elif L == 4
    poly_uniform_eta_4x(&s.vec[0], &s.vec[1], &s.vec[2], &s.vec[3], seed,
                        0, 1, 2, 3);
#elif L == 5
    poly_uniform_eta_4x(&s.vec[0], &s.vec[1], &s.vec[2], &s.vec[3], seed,
                        0, 1, 2, 3);
    poly_uniform_eta_4x(&s.vec[4], &tmp.vec[0], &tmp.vec[1], &tmp.vec[2], seed,
                        4, 5, 6, 7);
#else
#error
#endif

    printf("s = ((");
    for(j = 0; j < L; ++j) {
      for(k = 0; k < N; ++k) {
        u = s.vec[j].coeffs[k];
        if(u > (Q-1)/2) u -= Q;
        printf("%2d", u);
        if(k < N-1) printf(", ");
        else if(j < L-1) printf("),\n     (");
        else printf(")\n");
      }
    }

#if L == 2
    poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &tmp.vec[0], &tmp.vec[1], seed,
                             0, 1, 2, 3);
#elif L == 3
    poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &tmp.vec[0], seed,
                             0, 1, 2, 3);
#elif L == 4
    poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &y.vec[3], seed,
                             0, 1, 2, 3);
#elif L == 5
    poly_uniform_gamma1m1_4x(&y.vec[0], &y.vec[1], &y.vec[2], &y.vec[3], seed,
                              0, 1, 2, 3);
    poly_uniform_gamma1m1(&y.vec[4], seed, 4);
#else
#error
#endif

    printf("y = ((");
    for(j = 0; j < L; ++j) {
      for(k = 0; k < N; ++k) {
        u = y.vec[j].coeffs[k];
        if(u > (Q-1)/2) u -= Q;
        printf("%7d", u);
        if(k < N-1) printf(", ");
        else if(j < L-1) printf("),\n     (");
        else printf(")\n");
      }
    }
    polyvecl_ntt(&y);
    for(j = 0; j < K; ++j) {
      polyvecl_pointwise_acc_invmontgomery(w.vec+j, mat+j, &y);
      poly_invntt_montgomery(w.vec+j);
    }
    polyveck_freeze(&w);
    polyveck_decompose(&w, &tmp, &w);

    printf("w1 = ((");
    for(j = 0; j < K; ++j) {
      for(k = 0; k < N; ++k) {
        printf("%2u", w.vec[j].coeffs[k]);
        if(k < N-1) printf(", ");
        else if(j < K-1) printf("),\n      (");
        else printf(")\n");
      }
    }

    challenge(&c, seed + SEEDBYTES, &w);
    printf("c = (");
    for(j = 0; j < N; ++j) {
      u = c.coeffs[j];
      if(u > (Q-1)/2) u -= Q;
      printf("%d", u);
      if(j < N-1) printf(", ");
      else printf(")\n");
    }

    printf("\n");
  }
  
  return 0;
}
