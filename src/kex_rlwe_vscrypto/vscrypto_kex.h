
#ifndef _VSCRYPTO_KEX_H
#define _VSCRYPTO_KEX_H

#include "oqs/rand.h"
#include "vscrypto_defs.h"

/* Construct Alice's private / public key pair. Return all elements in the Fourier Domain 	
 * input:  none
 * output: private key s_1=s[m]...s[2*m-1] in Fourier Domain
 *         noise term s_0=s[0]...s[n-1] in Fourier Domain, not needed again
 *         public key b in Fourier Domain
 */	
void GLOBAL(KEM1_Generate)(ring_ctx_t *ctx, RINGELT *s, RINGELT *b, OQS_RAND *oqsrnd);

/* Encapsulation routine. Returns an element in R_q x R_2
 * input:  Alice's public key b in Fourier Domain
 * output: Bob's public key u in Fourier Domain
 *         reconciliation data cr_v
 *         shared secret mu
 */
void GLOBAL(KEM1_Encapsulate)(ring_ctx_t *ctx, RINGELT *u, uint64_t *cr_v, uint64_t *mu, const RINGELT *b, OQS_RAND *oqsrnd);

/* Decapsulation routine.
 * input:  Bob's public key u in Fourier Domain
 *         Alice's private key s_1 in Fourier Domain
 *         reconciliation data cr_v
 * output: shared secret mu
 */
void GLOBAL(KEM1_Decapsulate)(ring_ctx_t *ctx, uint64_t *mu, const RINGELT *u, const RINGELT *s_1, const uint64_t *cr_v);

#endif /* vscrypto_kex.h */

