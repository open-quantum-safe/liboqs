#ifndef __THREEBEARS_PARAMS_H__
#define __THREEBEARS_PARAMS_H__

#define VERSION              1
#define MATRIX_SEED_BYTES    24
#define ENC_SEED_BYTES       32
#define IV_BYTES             0
#define LGX                  10
#define DIGITS               312
#define DIM                  4
#define VAR_TIMES_128        96
#define LPR_BITS             4
#define FEC_BITS             18
#define CCA                  0
#define SHARED_SECRET_BYTES  32
#define PRIVATE_KEY_BYTES    40
#define PRF_KEY_BYTES        PRIVATE_KEY_BYTES

#define BEAR_NAME   "PapaBearEphem"
#define encapsulate PQCLEAN_PAPABEAREPHEM_CLEAN_encapsulate
#define decapsulate PQCLEAN_PAPABEAREPHEM_CLEAN_decapsulate
#define get_pubkey  PQCLEAN_PAPABEAREPHEM_CLEAN_get_pubkey

#define GF_BYTES ((LGX*DIGITS+7)/8)
#define PUBLIC_KEY_BYTES (MATRIX_SEED_BYTES + DIM*GF_BYTES)
#define CAPSULE_BYTES \
    (DIM*GF_BYTES + IV_BYTES + ((ENC_SEED_BYTES*8+FEC_BITS)*LPR_BITS+7)/8)

#endif
