
// mostly copied from github/vscrypto RLWE project
// then refactored to find into liboqs

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "vscrypto_kex.h"
#include "vscrypto_defs.h"

// various operations in FFT/CRT domain
#define POINTWISE_MUL(v, b, e0, q, m) { unsigned _i; for (_i = 0; _i < m; ++_i) { MUL_MOD((v)[_i], (e0)[_i], (b)[_i], (q)); } }
#define POINTWISE_ADD(v, b, e0, q, m) { unsigned _i; for (_i = 0; _i < m; ++_i) { ADD_MOD((v)[_i], (e0)[_i], (b)[_i], (q)); } }
#define POINTWISE_MUL_ADD(v, b, e0, e1, q, m) { unsigned _i; for (_i = 0; _i < m; ++_i) { MUL_MOD((v)[_i], (e0)[_i], (b)[_i], (q)); ADD_MOD((v)[_i], (v)[_i], (e1)[_i], (q)); } }

// 11**37 - 1
#define MAX_HI 0xffd1390a0adc2fb8
#define MAX_LO 0xdabbb8174d95c99a

static void sample_mod_11(RINGELT *out, int nr, OQS_RAND *oqsrnd) {
  static uint64_t hi, lo;  // ok to share across equally secure oqsrnd's
  static unsigned countdown = 1;
  uint64_t X[11] = { 0x0, 0x1745d1745d1745d1, 0x2e8ba2e8ba2e8ba2, 0x45d1745d1745d174, 0x5d1745d1745d1745, 0x745d1745d1745d17, 0x8ba2e8ba2e8ba2e8, 0xa2e8ba2e8ba2e8ba, 0xba2e8ba2e8ba2e8b, 0xd1745d1745d1745d, 0xe8ba2e8ba2e8ba2e };  // elevenths of 2**64
  unsigned i;

  while (nr-- > 0) {
    if (!--countdown) {
      while ( (hi=oqsrnd->rand_64(oqsrnd))>MAX_HI
            || ((lo=oqsrnd->rand_64(oqsrnd))>MAX_LO && hi==MAX_HI) ) ;
      countdown = 37;
    }
    i = 5 * (hi%11);
    *(out++) = (lo-55*(lo>55)+i) % 11;
    hi /= 11;
    lo = X[i/5] + (lo+i%11)/11;
  }
  return;
}

static void sample_secret(ring_ctx_t *ctx, RINGELT *s, OQS_RAND *oqsrnd) {
  unsigned i;

  // vscrypto code had an inefficient uniform sampler, we can do much
  // better with sample_mod_11 above but only if B=5 as in vscrypto code
  assert(ctx->B==5);

  s[ctx->modulus_m-1] = 0;
  sample_mod_11(s,ctx->m_limit,oqsrnd);
  for (i=0; i<ctx->m_limit; i++) if (s[i] > 5) s[i] += ctx->prime_q - 11;

  return;
}

/* Round and cross-round */
static void round_and_cross_round(ring_ctx_t *ctx, uint64_t *modular_rnd, uint64_t *cross_rnd, const RINGELT *v, OQS_RAND *oqsrnd) {
  const unsigned muwords = ctx->muwords;
  const unsigned mlim = ctx->m_limit;
  const unsigned q = ctx->prime_q;
  const unsigned q_1_4 = ctx->q_1_4;
  const unsigned q_2_4 = ctx->q_2_4;
  const unsigned q_3_4 = ctx->q_3_4;
  uint16_t i = 0;
  uint64_t r = oqsrnd->rand_64(oqsrnd);
  RINGELT word = 0, pos = 0, rbit = 0, val;

  memset((void *) modular_rnd, 0, muwords*sizeof(uint64_t));
  memset((void *) cross_rnd, 0, muwords*sizeof(uint64_t));

  for (i = 0; i < mlim; ++i) {
    val = v[i];
    /*Randomize rounding procedure - probabilistic nudge*/
    if ((q&3) == 1) {
      if (val == 0) {
        if (r & 1) val = (q-1);
        rbit++;
        if (rbit >= 64) {
          r = oqsrnd->rand_64(oqsrnd); rbit = 0;
        }
        else r = (r >> 1);
      }
      else if (val == q_1_4-1) {
        if (r & 1) val = q_1_4;
        rbit++;
        if (rbit >= 64) {
          r = oqsrnd->rand_64(oqsrnd); rbit = 0;
        }
        else r = (r >> 1);
      }
    }
    else {
      if (val == 0) {
        if (r & 1) val = (q-1);
        rbit++;
        if (rbit >= 64) {
          r = oqsrnd->rand_64(oqsrnd); rbit = 0;
        }
        else r = (r >> 1);
      }
      else if (val == q_3_4-1) {
        if (r & 1) val = q_3_4;
        rbit++;
        if (rbit >= 64) {
          r = oqsrnd->rand_64(oqsrnd); rbit = 0;
        }
        else r = (r >> 1);
      }
    }

    /*Modular rounding process*/
    if (val > q_1_4 && val < q_3_4) modular_rnd[word] |= (1UL << pos);

    /*Cross Rounding process*/
    if ((val > q_1_4 && val <= q_2_4) || val >= q_3_4) cross_rnd[word] |= (1UL << pos);

    pos++;
    if (pos == 64) {
      word++; pos = 0;
    }

  }


}


/* Reconcile */
static void rec(ring_ctx_t *ctx, uint64_t *r, RINGELT *w, const uint64_t *b) {
  RINGELT i = 0;
  RINGELT word = 0, pos = 0;
  const unsigned muwords = ctx->muwords;
  const unsigned mlim = ctx->m_limit;
  const unsigned r0_l = ctx->r0_l;
  const unsigned r1_l = ctx->r1_l;
  const unsigned r0_u = ctx->r0_u;
  const unsigned r1_u = ctx->r1_u;

  memset((void *) r, 0, muwords*sizeof(uint64_t));

  for (i = 0; i < mlim; ++i) {
    if ((b[word] >> pos) & 1UL) {
      if (w[i] > r1_l && w[i] < r1_u) r[word] |= (1UL << pos);
    }
    else {
      if (w[i] > r0_l && w[i] < r0_u) r[word] |= (1UL << pos);
    }
    pos++;
    if (pos == 64) {
      word++; pos = 0;
    }
  }
}

/* Construct Alice's private / public key pair. Return all elements in the Fourier Domain
 * input:  none
 * output: private key s_1=s[n]...s[2*n-1] in Fourier Domain                : m RINGELTs
 *         noise term s_0=s[0]...s[n-1] in Fourier Domain, not needed again : m RINGELTs
 *         public key b in Fourier Domain                                   : m RINGELTs
 */
void GLOBAL(KEM1_Generate)(ring_ctx_t *ctx, RINGELT *s, RINGELT *b, OQS_RAND *oqsrnd) {
  const unsigned m = ctx->modulus_m;
  const unsigned q = ctx->prime_q;
  const RINGELT *a = ctx->a;

  sample_secret(ctx,s,oqsrnd);
  sample_secret(ctx,s+m,oqsrnd);
  /*Fourier Transform secret keys*/
  ctx->FFT(FFT_FWD,s);
  ctx->FFT(FFT_FWD,s+m);
  POINTWISE_MUL_ADD(b, a, s+m, s, q, m); //Combine with a to produce s_1*a+s_0 in the Fourier domain. Alice's public key.

}

static void map_to_cyclotomic(ring_ctx_t *ctx, RINGELT *v) {
  if (!ctx->m_is_pot) {
    const unsigned modulus_m = ctx->modulus_m;
    const unsigned mlim = ctx->m_limit;
    const unsigned q = ctx->prime_q;
    unsigned i;
    for (i=0; i<mlim; i++) SUB_MOD(v[i],v[i],v[mlim],q);
    for (i=mlim; i<modulus_m; i++) v[i] = 0;
  }
}

/* Encapsulation routine. Returns an element in R_q x R_2
 * input:  Alice's public key b in Fourier Domain : m RINGELTs
 * output: Bob's public key u in Fourier Domain   : m RINGELTs
 *         reconciliation data cr_v               : mlim bits, packed into uint64_t's
 *         shared secret mu                       : mlim bits, packed into uint64_t's
 */
void GLOBAL(KEM1_Encapsulate)(ring_ctx_t *ctx, RINGELT *u, uint64_t *cr_v, uint64_t *mu, const RINGELT *b, OQS_RAND *oqsrnd) {
  const unsigned q = ctx->prime_q;
  const unsigned m = ctx->modulus_m;
  const RINGELT *a = ctx->a;
  RINGELT e[3*m];
  RINGELT v[m];

  /*Sample Bob's ephemeral keys*/
  sample_secret(ctx,e,oqsrnd);
  sample_secret(ctx,e+m,oqsrnd);
  sample_secret(ctx,e+2*m,oqsrnd);
  /*Fourer Transform e0 and e1*/
  ctx->FFT(FFT_FWD,e); ctx->FFT(FFT_FWD,e+m);
  POINTWISE_MUL_ADD(u, a, e, e+m, q, m); //Combine with a to produce e_0*a+e_1 in the Fourier domain. Bob's public key.

  POINTWISE_MUL(v, b, e, q, m); //Create v = e0*b
  ctx->FFT(FFT_REV,v); //Undo the Fourier Transform
  map_to_cyclotomic(ctx,v);

  POINTWISE_ADD(v, v, e+2*m, q, m); //Create v = e0*b+e2

  round_and_cross_round(ctx, mu, cr_v, v, oqsrnd);
}

/* Decapsulation routine.
 * input:  Bob's public key u in Fourier Domain      : m RINGELTs
 *         Alice's private key s_1 in Fourier Domain : m RINGELTs
 *         reconciliation data cr_v                  : mlim bits, packed into uint64_t's
 * output: shared secret mu                          : mlim bits, packed into uint64_t's
 */
void GLOBAL(KEM1_Decapsulate)(ring_ctx_t *ctx, uint64_t *mu, const RINGELT *u, const RINGELT *s_1, const uint64_t *cr_v) {
  const unsigned m = ctx->modulus_m;
  const unsigned q = ctx->prime_q;
  RINGELT w[m];

  POINTWISE_MUL(w, s_1, u, q, m); //Create w = s1*u
  ctx->FFT(FFT_REV,w); //Undo the Fourier Transform
  map_to_cyclotomic(ctx,w);
  rec(ctx,mu, w, cr_v);

}

