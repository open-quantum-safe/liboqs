// p500_27: p = 27 * 2^500 - 1 (505 bits, 8 limbs)
#ifndef FP_GENERIC_PARAMS_H
#define FP_GENERIC_PARAMS_H

#define FPG_N 8
#define FPG_BITS 505
#define FPG_C 27ULL
#define FPG_T 500

#define FPG_P_INIT                                                                                                     \
    { 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,                      \
      0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0x01afffffffffffffULL }
#define FPG_ONE_INIT                                                                                                   \
    { 0x0000000000000097ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,                      \
      0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0130000000000000ULL }
#define FPG_R2_INIT                                                                                                    \
    { 0xed097b425ed0f19aULL, 0x097b425ed097b425ULL, 0x7b425ed097b425edULL, 0x425ed097b425ed09ULL,                      \
      0x5ed097b425ed097bULL, 0xd097b425ed097b42ULL, 0x97b425ed097b425eULL, 0x0045ed097b425ed0ULL }
#define FPG_TWO_INV_INIT                                                                                               \
    { 0x000000000000004bULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,                      \
      0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0170000000000000ULL }
#define FPG_THREE_INV_INIT                                                                                             \
    { 0x5555555555555587ULL, 0x5555555555555555ULL, 0x5555555555555555ULL, 0x5555555555555555ULL,                      \
      0x5555555555555555ULL, 0x5555555555555555ULL, 0x5555555555555555ULL, 0x0185555555555555ULL }

#endif
