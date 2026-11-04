#ifndef QRUOV_TAU_H
#define QRUOV_TAU_H


#if (QRUOV_q == 127) && (QRUOV_L == 3) && (QRUOV_v == 156) && (QRUOV_m == 54)
#define QRUOV_tau_n1    4267
#define QRUOV_tau_n2    2916
#define QRUOV_tau_v      192
#define QRUOV_tau_m       82
#elif (QRUOV_q == 127) && (QRUOV_L == 10) && (QRUOV_v == 540) && (QRUOV_m == 60)
#define QRUOV_tau_n1   15135
#define QRUOV_tau_n2    3357
#define QRUOV_tau_v      594
#define QRUOV_tau_m       89
#elif (QRUOV_q == 31) && (QRUOV_L == 3) && (QRUOV_v == 165) && (QRUOV_m == 60)
#define QRUOV_tau_n1    4959
#define QRUOV_tau_n2    3571
#define QRUOV_tau_v      226
#define QRUOV_tau_m      104
#elif (QRUOV_q == 31) && (QRUOV_L == 10) && (QRUOV_v == 600) && (QRUOV_m == 70)
#define QRUOV_tau_n1   19242
#define QRUOV_tau_n2    4518
#define QRUOV_tau_v      704
#define QRUOV_tau_m      116
#elif (QRUOV_q == 7) && (QRUOV_L == 10) && (QRUOV_v == 740) && (QRUOV_m == 100)
#define QRUOV_tau_n1   32629
#define QRUOV_tau_n2    8947
#define QRUOV_tau_v     1024
#define QRUOV_tau_m      201
#elif (QRUOV_q == 127) && (QRUOV_L == 3) && (QRUOV_v == 228) && (QRUOV_m == 78)
#define QRUOV_tau_n1    9020
#define QRUOV_tau_n2    6123
#define QRUOV_tau_v      283
#define QRUOV_tau_m      120
#elif (QRUOV_q == 127) && (QRUOV_L == 10) && (QRUOV_v == 820) && (QRUOV_m == 90)
#define QRUOV_tau_n1   34603
#define QRUOV_tau_n2    7600
#define QRUOV_tau_v      902
#define QRUOV_tau_m      134
#elif (QRUOV_q == 31) && (QRUOV_L == 3) && (QRUOV_v == 246) && (QRUOV_m == 87)
#define QRUOV_tau_n1   10878
#define QRUOV_tau_n2    7655
#define QRUOV_tau_v      338
#define QRUOV_tau_m      154
#elif (QRUOV_q == 31) && (QRUOV_L == 10) && (QRUOV_v == 890) && (QRUOV_m == 100)
#define QRUOV_tau_n1   41974
#define QRUOV_tau_n2    9507
#define QRUOV_tau_v     1046
#define QRUOV_tau_m      169
#elif (QRUOV_q == 7) && (QRUOV_L == 10) && (QRUOV_v == 1100) && (QRUOV_m == 140)
#define QRUOV_tau_n1   71432
#define QRUOV_tau_n2   18461
#define QRUOV_tau_v     1526
#define QRUOV_tau_m      289
#elif (QRUOV_q == 127) && (QRUOV_L == 3) && (QRUOV_v == 306) && (QRUOV_m == 105)
#define QRUOV_tau_n1   16144
#define QRUOV_tau_n2   11018
#define QRUOV_tau_v      380
#define QRUOV_tau_m      162
#elif (QRUOV_q == 127) && (QRUOV_L == 10) && (QRUOV_v == 1110) && (QRUOV_m == 120)
#define QRUOV_tau_n1   63119
#define QRUOV_tau_n2   13669
#define QRUOV_tau_v     1220
#define QRUOV_tau_m      179
#elif (QRUOV_q == 127) && (QRUOV_L == 10) && (QRUOV_v == 1040) && (QRUOV_m == 110)
#define QRUOV_tau_n1   55473
#define QRUOV_tau_n2   11760
#define QRUOV_tau_v     1148
#define QRUOV_tau_m      168
#elif (QRUOV_q == 31) && (QRUOV_L == 3) && (QRUOV_v == 324) && (QRUOV_m == 114)
#define QRUOV_tau_n1   18738
#define QRUOV_tau_n2   13145
#define QRUOV_tau_v      447
#define QRUOV_tau_m      203
#elif (QRUOV_q == 31) && (QRUOV_L == 10) && (QRUOV_v == 1120) && (QRUOV_m == 120)
#define QRUOV_tau_n1   66236
#define QRUOV_tau_n2   14326
#define QRUOV_tau_v     1324
#define QRUOV_tau_m      210
#elif (QRUOV_q == 7) && (QRUOV_L == 10) && (QRUOV_v == 1490) && (QRUOV_m == 190)
#define QRUOV_tau_n1  130305
#define QRUOV_tau_n2   33694
#define QRUOV_tau_v     2065
#define QRUOV_tau_m      391
#else
#error "Unknown QR-UOV parameter tuple for tau table"
#endif

#endif // QRUOV_TAU_H
