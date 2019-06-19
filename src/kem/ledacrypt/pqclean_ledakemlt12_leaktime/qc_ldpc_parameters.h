#ifndef QC_LDPC_PARAMETERS_H
#define QC_LDPC_PARAMETERS_H

#include "fips202.h"

#define TRNG_BYTE_LENGTH (24)
#define HASH_BYTE_LENGTH (32)
#define HASH_FUNCTION sha3_256

#define N0              (2)
#define P               (52147)  // modulus(x) = x^P-1
#define DV              (9)      // odd number
#define M               (9)
#define M0              (5)
#define M1              (4)
#define NUM_ERRORS_T    (136)

// Derived parameters, they are useful for QC-LDPC algorithms
#define HASH_BIT_LENGTH (HASH_BYTE_LENGTH << 3)
#define K               ((N0-1)*P)
#define N               (N0*P)
#define DC              (N0*DV)

#define Q_BLOCK_WEIGHTS  {{M0,M1},{M1,M0}}
static const unsigned char qBlockWeights[N0][N0] = Q_BLOCK_WEIGHTS;

#endif
