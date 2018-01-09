#include "reduce.h"
#include "params.h"

/* Incomplete-reduction routines; for details on allowed input ranges
 * and produced output ranges, see the description in the paper: 
 * https://cryptojedi.org/papers/#newhope */


static const uint32_t qinv = 12287; // -inverse_mod(p,2^18)
static const uint32_t rlog = 18;

uint16_t montgomery_reduce(uint32_t a)
{
  uint32_t u;

  u = (a * qinv);
  u &= ((1<<rlog)-1);
  u *= NEWHOPE_Q;
  a = a + u;
  return a >> 18;
}


uint16_t barrett_reduce(uint16_t a)
{
  uint32_t u;

  u = ((uint32_t) a * 5) >> 16;
  u *= NEWHOPE_Q;
  a -= u;
  return a;
}

