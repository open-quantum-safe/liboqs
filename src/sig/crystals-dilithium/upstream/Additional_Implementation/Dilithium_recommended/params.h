#ifndef PARAMS_H
#define PARAMS_H

#define MODE 2

#define SEEDBYTES 32U
#define CRHBYTES 48U
#define N 256U
#define Q 8380417U
#define QBITS 23U
#define ROOT_OF_UNITY 1753U
#define D 14U
#define GAMMA1 ((Q - 1U)/16U)
#define GAMMA2 (GAMMA1/2U)
#define ALPHA (2U*GAMMA2)

#if MODE == 0
#define K 3U
#define L 2U
#define ETA 7U
#define SETABITS 4U
#define BETA 375U
#define OMEGA 64U

#elif MODE == 1
#define K 4U
#define L 3U
#define ETA 6U
#define SETABITS 4U
#define BETA 325U
#define OMEGA 80U

#elif MODE == 2
#define K 5U
#define L 4U
#define ETA 5U
#define SETABITS 4U
#define BETA 275U
#define OMEGA 96U

#elif MODE == 3
#define K 6U
#define L 5U
#define ETA 3U
#define SETABITS 3U
#define BETA 175U
#define OMEGA 120U

#endif

#define POL_SIZE_PACKED ((N*QBITS)/8)
#define POLT1_SIZE_PACKED ((N*(QBITS - D))/8)
#define POLT0_SIZE_PACKED ((N*D)/8)
#define POLETA_SIZE_PACKED ((N*SETABITS)/8)
#define POLZ_SIZE_PACKED ((N*(QBITS - 3))/8)
#define POLW1_SIZE_PACKED ((N*4)/8)

#define POLVECK_SIZE_PACKED (K*POL_SIZE_PACKED)
#define POLVECL_SIZE_PACKED (L*POL_SIZE_PACKED)
#define PK_SIZE_PACKED (SEEDBYTES + K*POLT1_SIZE_PACKED)
#define SK_SIZE_PACKED (2*SEEDBYTES + (L + K)*POLETA_SIZE_PACKED + CRHBYTES + K*POLT0_SIZE_PACKED)
#define SIG_SIZE_PACKED (L*POLZ_SIZE_PACKED + (OMEGA + K) + (N/8 + 8))

#endif
