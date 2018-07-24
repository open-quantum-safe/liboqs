#include <stdint.h>
#include "params.h"
#include "reduce.h"

/*************************************************
* Name:        montgomery_reduce
* 
* Description: For finite field element a with 0 <= a <= Q*2^32,
*              compute r \equiv a*2^{-32} (mod Q) such that 0 < r < 2*Q.
*              
* Arguments:   - uint64_t: element a
*
* Returns r.
**************************************************/
uint32_t montgomery_reduce(uint64_t a) {
  const uint64_t qinv = QINV;
  uint64_t t;

  t = a * qinv;
  t &= (1ULL << 32) - 1;
  t *= Q;
  t = a + t;
  return t >> 32;
}

/*************************************************
* Name:        reduce32
* 
* Description: For finite field element a, compute r \equiv a (mod Q)
*              such that 0 <= r < 2*Q.
*              
* Arguments:   - uint32_t: element a
*
* Returns r.
**************************************************/
uint32_t reduce32(uint32_t a) {
  uint32_t t;

  t = a & 0x7FFFFF;
  a >>= 23;
  t += ((a << 13) - a);
  return t;
}

/*************************************************
* Name:        freeze
* 
* Description: For finite field element a, compute standard
*              representative r = a mod Q.
*              
* Arguments:   - uint32_t: element a
*
* Returns r.
**************************************************/
uint32_t freeze(uint32_t a) {
  a = reduce32(a);
  a -= Q;
  a += ((int32_t)a >> 31) & Q;
  return a;
}
