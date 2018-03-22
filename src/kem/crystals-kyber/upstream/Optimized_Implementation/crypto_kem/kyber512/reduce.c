#include "reduce.h"
#include "params.h"


static const uint32_t qinv = 7679; // -inverse_mod(q,2^18)
static const uint32_t rlog = 18;

uint16_t montgomery_reduce(uint32_t a)
{
  uint32_t u;

  u = (a * qinv);
  u &= ((1<<rlog)-1);
  u *= KYBER_Q;
  a = a + u;
  return a >> rlog;
}

//static const uint16_t sinv = 8; // trunc((2^16/q))

uint16_t barrett_reduce(uint16_t a)
{
  uint32_t u;

  u = a >> 13;//((uint32_t) a * sinv) >> 16;
  u *= KYBER_Q;
  a -= u;
  return a;
}

uint16_t freeze(uint16_t x)
{
  uint16_t m,r;
  int16_t c;
  r = barrett_reduce(x);

  m = r - KYBER_Q;
  c = m;
  c >>= 15; 
  r = m ^ ((r^m)&c); 

  return r;
}


