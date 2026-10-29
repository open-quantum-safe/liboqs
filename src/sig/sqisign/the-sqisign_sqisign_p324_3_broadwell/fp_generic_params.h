// p324_3: p = 3 * 2^324 - 1 (326 bits, 6 limbs)
#ifndef FP_GENERIC_PARAMS_H
#define FP_GENERIC_PARAMS_H

#define FPG_N 6
#define FPG_BITS 326
#define FPG_C 3ULL
#define FPG_T 324

#define FPG_P_INIT                                                                                                     \
    { 0xffffffffffffffffULL, 0xffffffffffffffffULL, 0xffffffffffffffffULL,                                             \
      0xffffffffffffffffULL, 0xffffffffffffffffULL, 0x000000000000002fULL }
#define FPG_ONE_INIT                                                                                                   \
    { 0x0555555555555555ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,                                             \
      0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000010ULL }
#define FPG_R2_INIT                                                                                                    \
    { 0xc71c71c71c71c71cULL, 0x5571c71c71c71c71ULL, 0x5555555555555555ULL,                                             \
      0x5555555555555555ULL, 0x5555555555555555ULL, 0x0000000000000015ULL }
#define FPG_TWO_INV_INIT                                                                                               \
    { 0x02aaaaaaaaaaaaaaULL, 0x0000000000000000ULL, 0x0000000000000000ULL,                                             \
      0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000020ULL }
#define FPG_THREE_INV_INIT                                                                                             \
    { 0x571c71c71c71c71cULL, 0x5555555555555555ULL, 0x5555555555555555ULL,                                             \
      0x5555555555555555ULL, 0x5555555555555555ULL, 0x0000000000000015ULL }

#endif
