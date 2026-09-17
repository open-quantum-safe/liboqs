// p664_17: p = 17 * 2^664 - 1 (669 bits, 11 limbs)
#ifndef FP_GENERIC_PARAMS_H
#define FP_GENERIC_PARAMS_H

#define FPG_N 11
#define FPG_BITS 669
#define FPG_C 17ULL
#define FPG_T 664

#define FPG_P_INIT                                                                                                     \
    { 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,                      \
      0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,                      \
      0xffffffffffffffffULL, 0xffffffffffffffffULL, 0x0000000010ffffffULL }
#define FPG_ONE_INIT                                                                                                   \
    { 0x0000000f0f0f0f0fULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,                      \
      0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,                      \
      0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000001000000ULL }
#define FPG_R2_INIT                                                                                                    \
    { 0xd3b597795b3d1f00ULL, 0x0f0f0f0f0f0f0ff1ULL, 0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0f0f0fULL,                      \
      0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0f0f0fULL,                      \
      0x0f0f0f0f0f0f0f0fULL, 0x0f0f0f0f0f0f0f0fULL, 0x000000000f0f0f0fULL }
#define FPG_TWO_INV_INIT                                                                                               \
    { 0x0000000787878787ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,                      \
      0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,                      \
      0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000009000000ULL }
#define FPG_THREE_INV_INIT                                                                                             \
    { 0xaaaaaaafafafafafULL, 0xaaaaaaaaaaaaaaaaULL, 0xaaaaaaaaaaaaaaaaULL, 0xaaaaaaaaaaaaaaaaULL,                      \
      0xaaaaaaaaaaaaaaaaULL, 0xaaaaaaaaaaaaaaaaULL, 0xaaaaaaaaaaaaaaaaULL, 0xaaaaaaaaaaaaaaaaULL,                      \
      0xaaaaaaaaaaaaaaaaULL, 0xaaaaaaaaaaaaaaaaULL, 0x000000000baaaaaaULL }

#endif
