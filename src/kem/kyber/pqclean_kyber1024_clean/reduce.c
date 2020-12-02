#include "params.h"
#include "reduce.h"
#include <stdint.h>

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_montgomery_reduce
*
* Description: Montgomery reduction; given a 32-bit integer a, computes
*              16-bit integer congruent to a * R^-1 mod q, where R=2^16
*
* Arguments:   - int32_t a: input integer to be reduced;
*                           has to be in {-q2^15,...,q2^15-1}
*
* Returns:     integer in {-q+1,...,q-1} congruent to a * R^-1 modulo q.
**************************************************/
int16_t PQCLEAN_KYBER1024_CLEAN_montgomery_reduce(int32_t a) {
    int32_t t;
    int16_t u;

    u = (int16_t)(a * (int64_t)QINV);
    t = (int32_t)u * KYBER_Q;
    t = a - t;
    t >>= 16;
    return (int16_t)t;
}

/*************************************************
* Name:        PQCLEAN_KYBER1024_CLEAN_barrett_reduce
*
* Description: Barrett reduction; given a 16-bit integer a, computes
*              centered representative congruent to a mod q in {-(q-1)/2,...,(q-1)/2}
*
* Arguments:   - int16_t a: input integer to be reduced
*
* Returns:     integer in {-(q-1)/2,...,(q-1)/2} congruent to a modulo q.
**************************************************/
int16_t PQCLEAN_KYBER1024_CLEAN_barrett_reduce(int16_t a) {
    int16_t t;
    const int16_t v = ((1U << 26) + KYBER_Q / 2) / KYBER_Q;

    t  = ((int32_t)v * a + (1 << 25)) >> 26;
    t *= KYBER_Q;
    return a - t;
}
