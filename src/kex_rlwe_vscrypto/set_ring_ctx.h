
#ifndef _SET_RING_CTX_H
#define _SET_RING_CTX_H

#include "vscrypto_defs.h"

void GLOBAL(set_ring_ctx)(ring_ctx_t *ctx, unsigned modulus_m, unsigned prime_q, RINGELT *a, void (*FFT)(int,FFTSHORT*));

#endif /* set_ring_ctx.h */

