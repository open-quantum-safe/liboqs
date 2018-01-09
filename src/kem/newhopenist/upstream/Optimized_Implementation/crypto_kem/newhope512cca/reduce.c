#include "reduce.h"
#include "params.h"

static const uint32_t qinv = 12287; // -inverse_mod(p,2^18)
static const uint32_t rlog = 18;

/*************************************************
* Name:        verify
* 
* Description: Montgomery reduction; given a 32-bit integer a, computes
*              16-bit integer congruent to a * R^-1 mod q, 
*              where R=2^18 (see value of rlog)
*
* Arguments:   - uint32_t a: input unsigned integer to be reduced; has to be in {0,...,1073491968}
*              
* Returns:     unsigned integer in {0,...,2^14-1} congruent to a * R^-1 modulo q.
**************************************************/
uint16_t montgomery_reduce(uint32_t a)
{
  uint32_t u;

  u = (a * qinv);
  u &= ((1<<rlog)-1);
  u *= NEWHOPE_Q;
  a = a + u;
  return a >> 18;
}
