#ifndef QRUOV_PARAM_H
#define QRUOV_PARAM_H

#ifndef PRG_IS_AES
#define PRG_IS_AES 1
#endif // PRG_IS_AES
#if (PRG_IS_AES != 0) && (PRG_IS_AES != 1)
#error "PRG_IS_AES must be 0 or 1"
#endif // PRG_IS_AES != 0 && PRG_IS_AES != 1

// Select one parameter set with -DQRUOV_PARAM_<cat>q<q>L<L>
// Example: -DQRUOV_PARAM_1q127L3

#if !defined(QRUOV_PARAM_1q7L10) \
 && !defined(QRUOV_PARAM_1q31L3) \
 && !defined(QRUOV_PARAM_1q31L10) \
 && !defined(QRUOV_PARAM_1q127L3) \
 && !defined(QRUOV_PARAM_1q127L10) \
 && !defined(QRUOV_PARAM_3q7L10) \
 && !defined(QRUOV_PARAM_3q31L3) \
 && !defined(QRUOV_PARAM_3q31L10) \
 && !defined(QRUOV_PARAM_3q127L3) \
 && !defined(QRUOV_PARAM_3q127L10) \
 && !defined(QRUOV_PARAM_5q7L10) \
 && !defined(QRUOV_PARAM_5q31L3) \
 && !defined(QRUOV_PARAM_5q31L10) \
 && !defined(QRUOV_PARAM_5q127L3) \
 && !defined(QRUOV_PARAM_5q127L10)
#warning "Using default parameter set QRUOV_PARAM_1q127L3"
#define QRUOV_PARAM_1q127L3
#endif

#if defined(QRUOV_PARAM_1q7L10)
#define QRUOV_security 1
#define QRUOV_q        7
#define QRUOV_L        10
#define QRUOV_v        740
#define QRUOV_m        100
#define QRUOV_fc       2
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_3q7L10)
#define QRUOV_security 3
#define QRUOV_q        7
#define QRUOV_L        10
#define QRUOV_v        1100
#define QRUOV_m        140
#define QRUOV_fc       2
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_5q7L10)
#define QRUOV_security 5
#define QRUOV_q        7
#define QRUOV_L        10
#define QRUOV_v        1490
#define QRUOV_m        190
#define QRUOV_fc       2
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_1q31L3)
#define QRUOV_security 1
#define QRUOV_q        31
#define QRUOV_L        3
#define QRUOV_v        165
#define QRUOV_m        60
#define QRUOV_fc       1
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_3q31L3)
#define QRUOV_security 3
#define QRUOV_q        31
#define QRUOV_L        3
#define QRUOV_v        246
#define QRUOV_m        87
#define QRUOV_fc       1
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_5q31L3)
#define QRUOV_security 5
#define QRUOV_q        31
#define QRUOV_L        3
#define QRUOV_v        324
#define QRUOV_m        114
#define QRUOV_fc       1
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_1q31L10)
#define QRUOV_security 1
#define QRUOV_q        31
#define QRUOV_L        10
#define QRUOV_v        600
#define QRUOV_m        70
#define QRUOV_fc       5
#define QRUOV_fc0      1
#define QRUOV_fe       3
#endif

#if defined(QRUOV_PARAM_3q31L10)
#define QRUOV_security 3
#define QRUOV_q        31
#define QRUOV_L        10
#define QRUOV_v        890
#define QRUOV_m        100
#define QRUOV_fc       5
#define QRUOV_fc0      1
#define QRUOV_fe       3
#endif

#if defined(QRUOV_PARAM_5q31L10)
#define QRUOV_security 5
#define QRUOV_q        31
#define QRUOV_L        10
#define QRUOV_v        1120
#define QRUOV_m        120
#define QRUOV_fc       5
#define QRUOV_fc0      1
#define QRUOV_fe       3
#endif

#if defined(QRUOV_PARAM_1q127L3)
#define QRUOV_security 1
#define QRUOV_q        127
#define QRUOV_L        3
#define QRUOV_v        156
#define QRUOV_m        54
#define QRUOV_fc       1
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_1q127L10)
#define QRUOV_security 1
#define QRUOV_q        127
#define QRUOV_L        10
#define QRUOV_v        540
#define QRUOV_m        60
#define QRUOV_fc       2
#define QRUOV_fc0      1
#define QRUOV_fe       3
#endif

#if defined(QRUOV_PARAM_3q127L3)
#define QRUOV_security 3
#define QRUOV_q        127
#define QRUOV_L        3
#define QRUOV_v        228
#define QRUOV_m        78
#define QRUOV_fc       1
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_3q127L10)
#define QRUOV_security 3
#define QRUOV_q        127
#define QRUOV_L        10
#define QRUOV_v        820
#define QRUOV_m        90
#define QRUOV_fc       2
#define QRUOV_fc0      1
#define QRUOV_fe       3
#endif

#if defined(QRUOV_PARAM_5q127L3)
#define QRUOV_security 5
#define QRUOV_q        127
#define QRUOV_L        3
#define QRUOV_v        306
#define QRUOV_m        105
#define QRUOV_fc       1
#define QRUOV_fc0      1
#define QRUOV_fe       1
#endif

#if defined(QRUOV_PARAM_5q127L10)
#define QRUOV_security 5
#define QRUOV_q        127
#define QRUOV_L        10
#define QRUOV_v        1040
#define QRUOV_m        110
#define QRUOV_fc       2
#define QRUOV_fc0      1
#define QRUOV_fe       3
#endif

#if QRUOV_q == 127
#define QRUOV_delta    2
#elif QRUOV_q == 31
#define QRUOV_delta    3
#elif QRUOV_q == 7
#define QRUOV_delta    4
#else
#error "Unsupported QRUOV_q"
#endif // QRUOV_q

#if QRUOV_delta < 0 || QRUOV_delta > QRUOV_m
#error "Invalid QRUOV_delta"
#endif // QRUOV_delta < 0 || QRUOV_delta > QRUOV_m

#endif // QRUOV_PARAM_H
