#ifndef MQDSS_MQ_H
#define MQDSS_MQ_H

#include "gf31.h"

/* Evaluates the MQ function on a vector of N gf31 elements x (expected to be
   in reduced 5-bit representation). Expects the coefficients in F to be in
   signed representation (i.e. [-15, 15], packed bytewise).
   Outputs M gf31 elements in unique 16-bit representation as fx. */
void PQCLEAN_MQDSS64_CLEAN_MQ(gf31 *fx, const gf31 *x, const signed char *F);

/* Evaluates the bilinear polar form of the MQ function (i.e. G) on a vector of
   N gf31 elements x (expected to be in reduced 5-bit representation). Expects
   the coefficients in F to be in signed representation (i.e. [-15, 15], packed
   bytewise). Outputs M gf31 elements in unique 16-bit representation as fx. */
void PQCLEAN_MQDSS64_CLEAN_G(gf31 *fx, const gf31 *x, const gf31 *y, const signed char *F);

#endif
