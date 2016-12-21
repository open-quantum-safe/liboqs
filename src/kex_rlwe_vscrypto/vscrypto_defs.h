
#ifndef _VSCRYPTO_DEFS_H
#define _VSCRYPTO_DEFS_H

#include <stdint.h>

typedef uint_fast32_t FFTSHORT;
typedef uint_fast64_t FFTLONG;
typedef FFTSHORT RINGELT;

typedef struct {
  RINGELT *a;
  unsigned modulus_m, prime_q, m_is_pot, m_limit;
  unsigned muwords;
  unsigned B, log2B, Bmask;
  unsigned q_1_4, q_2_4, q_3_4;
  unsigned r0_l, r0_u, r1_l, r1_u;
  void (*FFT)(int direction, FFTSHORT *x);
} ring_ctx_t;

#define PRIuFFTSHORT PRIuFAST32
#define PRIuFFTLONG PRIuFAST64
#define PRIuRINGELT PRIuFFTSHORT

#define FUN_MQ(F,M,Q) PASTE(F,M,Q)
#define PASTE(A,B,C) A ## _ ## B ## _ ## C

#define ADD_MOD(X,A,B,M) { if ((X=(A)+(B))>=(M)) X-=(M); }
#define SUB_MOD(X,A,B,M) ADD_MOD(X,A,(M)-(B),M)
#define MUL_MOD(X,A,B,M) { X = (FFTSHORT)( ((A)*(FFTLONG)(B)) % (M) ); }

#define FFT_FWD (1)
#define FFT_REV (0)

// conformance with liboqs convention
#define GLOBAL(X) oqs_kex_rlwe_vscrypto_ ## X

#define NR_PUBPRIV_RINGELTS(C) ((C)->modulus_m)
#define NR_RECKEY_BITS(C) ((C)->m_limit)

#endif /* vscrypto_defs.h */

