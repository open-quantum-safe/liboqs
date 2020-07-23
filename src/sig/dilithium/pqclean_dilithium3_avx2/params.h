#ifndef PARAMS_H
#define PARAMS_H

#include "config.h"

#define SEEDBYTES 32
#define CRHBYTES 48
#define N 256
#define Q 8380417
#define ROOT_OF_UNITY 1753
#define D 14
#define GAMMA1 ((Q - 1)/16)
#define GAMMA2 (GAMMA1/2)
#define ALPHA (2*GAMMA2)

#if DILITHIUM_MODE == 1
#define K 3
#define L 2
#define ETA 7
#define BETA 375
#define OMEGA 64

#elif DILITHIUM_MODE == 2
#define K 4
#define L 3
#define ETA 6
#define BETA 325
#define OMEGA 80

#elif DILITHIUM_MODE == 3
#define K 5
#define L 4
#define ETA 5
#define BETA 275
#define OMEGA 96

#elif DILITHIUM_MODE == 4
#define K 6
#define L 5
#define ETA 3
#define BETA 175
#define OMEGA 120

#else
#error "DILITHIUM_MODE must be 1, 2, 3, or 4"
#endif

#define POLYT1_PACKEDBYTES  288
#define POLYT0_PACKEDBYTES  448
#define POLYZ_PACKEDBYTES   640
#define POLYW1_PACKEDBYTES  128
#if ETA <= 3
#define POLYETA_PACKEDBYTES  96
#else
#define POLYETA_PACKEDBYTES 128
#endif

#define CRYPTO_PUBLICKEYBYTES (SEEDBYTES + K*POLYT1_PACKEDBYTES)
#define CRYPTO_SECRETKEYBYTES (2*SEEDBYTES + CRHBYTES \
                               + L*POLYETA_PACKEDBYTES \
                               + K*POLYETA_PACKEDBYTES \
                               + K*POLYT0_PACKEDBYTES)
#define CRYPTO_BYTES (L*POLYZ_PACKEDBYTES + OMEGA + K + N/8 + 8)

#endif
