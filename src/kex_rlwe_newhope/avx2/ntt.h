#ifndef NTT_H
#define NTT_H

#include "inttypes.h"

extern int32_t psis_bitrev[];
extern int32_t psis_inv[];
extern double omegas_double[];
extern double omegas_inv_double[];

void OQS_bitrev_vector(int32_t* poly);
void OQS_pwmul_double(int32_t* poly, const int32_t* factors);
void OQS_ntt_double(int32_t*,const double*,const double*);

#endif
