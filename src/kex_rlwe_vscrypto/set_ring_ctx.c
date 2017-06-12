
#include <stdio.h>
#include <stdlib.h>

#include "vscrypto_defs.h"
#include "set_ring_ctx.h"

#define BOUND_B (5)

void GLOBAL(set_ring_ctx)(ring_ctx_t *ctx, unsigned modulus_m, unsigned prime_q, RINGELT *a, void (*FFT)(int,FFTSHORT*)) {

  // supplied args
  ctx->modulus_m = modulus_m;
  ctx->prime_q = prime_q;
  ctx->a = a;
  ctx->FFT = FFT;

  // derived values
  ctx->m_is_pot = ((modulus_m&(modulus_m-1)) == 0);
  ctx->m_limit = modulus_m - !ctx->m_is_pot;  // m if power of 2, m-1 otherwise
  ctx->muwords = (ctx->m_limit+63)/64;
  ctx->B = BOUND_B;
  ctx->log2B = 0;
  do {
    ctx->log2B++;
    ctx->Bmask = (1<<ctx->log2B) - 1;
  } while (ctx->Bmask < 2*BOUND_B);
  ctx->q_1_4 = prime_q / 4;
  ctx->q_2_4 = prime_q / 2;
  ctx->q_3_4 = (prime_q+1)*3 / 4;
  ctx->r0_l = (prime_q-1)*3 / 8;
  ctx->r0_u = (prime_q+1)*7 / 8;
  ctx->r1_l = prime_q - 1 - ctx->r0_u;
  ctx->r1_u = prime_q - ctx->r0_l;

  return;
}

