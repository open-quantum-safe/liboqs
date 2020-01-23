#include <stdint.h>

#include "params.h"
#include "reduce.h"

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_montgomery_reduce
*
* Description: For finite field element a with 0 <= a <= Q*2^32,
*              compute r \equiv a*2^{-32} (mod Q) such that 0 <= r < 2*Q.
*
* Arguments:   - uint64_t: finite field element a
*
* Returns r.
**************************************************/
uint32_t PQCLEAN_DILITHIUM2_CLEAN_montgomery_reduce(uint64_t a) {
    uint64_t t;

    t = a * QINV;
    t &= (1ULL << 32) - 1;
    t *= Q;
    t = a + t;
    t >>= 32;
    return (uint32_t)t;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_reduce32
*
* Description: For finite field element a, compute r \equiv a (mod Q)
*              such that 0 <= r < 2*Q.
*
* Arguments:   - uint32_t: finite field element a
*
* Returns r.
**************************************************/
uint32_t PQCLEAN_DILITHIUM2_CLEAN_reduce32(uint32_t a) {
    uint32_t t;

    t = a & 0x7FFFFF;
    a >>= 23;
    t += (a << 13) - a;
    return t;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_csubq
*
* Description: Subtract Q if input coefficient is bigger than Q.
*
* Arguments:   - uint32_t: finite field element a
*
* Returns r.
**************************************************/
uint32_t PQCLEAN_DILITHIUM2_CLEAN_csubq(uint32_t a) {
    a -= Q;
    a += ((int32_t)a >> 31) & Q;
    return a;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_freeze
*
* Description: For finite field element a, compute standard
*              representative r = a mod Q.
*
* Arguments:   - uint32_t: finite field element a
*
* Returns r.
**************************************************/
uint32_t PQCLEAN_DILITHIUM2_CLEAN_freeze(uint32_t a) {
    a = PQCLEAN_DILITHIUM2_CLEAN_reduce32(a);
    a = PQCLEAN_DILITHIUM2_CLEAN_csubq(a);
    return a;
}
