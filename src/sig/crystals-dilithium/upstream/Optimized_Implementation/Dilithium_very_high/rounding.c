#include <stdint.h>
#include "params.h"
#include "reduce.h"

/*************************************************
* Name:        power2round
* 
* Description: For finite field element a, compute a0, a1 such that
*              a = a1*2^D + a0 with -2^{D/2} < a0 <= 2^{D/2}. Assumes a to be
*              standard representative.
*
* Arguments:   - uint32_t a: input element
*              - uint32_t *a0: pointer to output element Q + a0
*
* Returns a1.
**************************************************/
uint32_t power2round(uint32_t a, uint32_t *a0)  {
  int32_t t;
  /* Centralized remainder mod 2^D */
  t = a & ((1 << D) - 1);
  t -= (1 << (D-1)) + 1;
  t += (t >> 31) & (1 << D);
  t -= (1 << (D-1)) - 1;
  *a0 = Q + t;
  a = (a - t) >> D;
  return a;
}

/*************************************************
* Name:        decompose
* 
* Description: For finite field element a, compute high and low bits a0, a1 such
*              that a = a1*ALPHA + a0 with -ALPHA/2 < a0 <= ALPHA/2 except
*              if a = Q-1 where a1 = 0 and a0 = -1. Assumes a to be standard
*              representative.
*
* Arguments:   - uint32_t a: input element
*              - uint32_t *a0: pointer to output element Q + a0
*
* Returns a1.
**************************************************/
uint32_t decompose(uint32_t a, uint32_t *a0) {
#if ALPHA != (Q-1)/16
#error "decompose assumes ALPHA == (Q-1)/16"
#endif
  int32_t t, u;
  /* Centralized remainder mod ALPHA */
  t = a & 0x7FFFF;
  t += (a >> 19) << 9;
  t -= ALPHA/2 + 1;
  t += (t >> 31) & ALPHA;
  t -= ALPHA/2 - 1;
  a -= t;

  /* Divide by ALPHA (possible to avoid) */
  u = a - 1;
  u >>= 31;
  a = (a >> 19) + 1;
  a -= u & 1;

  /* Border case */
  *a0 = Q + t - (a >> 4);
  a &= 0xF;
  return a;
}

/*************************************************
* Name:        make_hint
* 
* Description: Compute hint bit indicating whether or not high bits of two
*              finite field elements differ.
*
* Arguments:   - uint32_t a: first input element
*              - uint32_t b: second input element
*
* Returns 1 if high bits of a and b differ and 0 otherwise.
**************************************************/
unsigned int make_hint(const uint32_t a, const uint32_t b) {
  uint32_t t;
  return decompose(a, &t) != decompose(freeze(a + b), &t);
}

/*************************************************
* Name:        use_hint
* 
* Description: Correct high bits according to hint.
*
* Arguments:   - uint32_t a: input element
*              - unsigned int hint: hint bit
*
* Returns corrected high bits.
**************************************************/
uint32_t use_hint(const uint32_t a, const unsigned int hint) {
  uint32_t a0, a1;
  
  a1 = decompose(a, &a0);
  if(hint == 0)
    return a1;
  else if(a0 > Q)
    return (a1 == (Q - 1)/ALPHA - 1) ? 0 : a1 + 1;
  else
    return (a1 == 0) ? (Q - 1)/ALPHA - 1 : a1 - 1;

  /* If decompose does not divide out ALPHA: 
  if(hint == 0)
    return a1;
  else if(a0 > Q)
    return (a1 == Q - 1 - ALPHA) ? 0 : a1 + ALPHA;
  else
    return (a1 == 0) ? Q - 1 - ALPHA : a1 - ALPHA;
  */
}
