#include "NTT_params.h"
#include "ntt.h"
#include "params.h"
#include "reduce.h"
#include <arm_neon.h>

/*************************************************
* Name:        ntt
*
* Description: Inplace number-theoretic transform (NTT) in Rq.
*              input is in standard order, output is in bitreversed order
*
* Arguments:   - int16_t r[256]: pointer to input/output vector of elements of Zq
**************************************************/
// Merged NTT layer
void ntt(int16_t r[256]) {
    NTT(r);
}

/*************************************************
* Name:        invntt_tomont
*
* Description: Inplace inverse number-theoretic transform in Rq and
*              multiplication by Montgomery factor 2^16.
*              Input is in bitreversed order, output is in standard order
*
* Arguments:   - int16_t r[256] in {-(q-1)/2,...,(q-1)/2}
*              pointer to input/output vector of elements of Zq
**************************************************/
void invntt(int16_t r[256]) {
    iNTT(r);

}
