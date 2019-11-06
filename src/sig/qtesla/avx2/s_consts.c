/*************************************************************************************
* qTESLA: an efficient post-quantum signature scheme based on the R-LWE problem
*
* Abstract: constants for x64 assembly implementation
**************************************************************************************/

#include "params.h"
#include <stdint.h>

#if defined(_qTESLA_p_I_)

int64_t oqs_sig_qteslapi_N = PARAM_N;
int32_t oqs_sig_qteslapi_Nx4 = 4 * PARAM_N;
int64_t oqs_sig_qteslapi_H = PARAM_H;
int64_t oqs_sig_qteslapi_N2m64 = 2 * PARAM_N - 64;
int64_t oqs_sig_qteslapi_N4m128 = 4 * PARAM_N - 128;
int32_t oqs_sig_qteslapi_ZEROS[8] __attribute__((aligned(32))) = {0, 0, 0, 0, 0, 0, 0, 0};
int32_t oqs_sig_qteslapi_PERM_MASK[8] __attribute__((aligned(32))) = {1, 3, 5, 7, 0, 2, 4, 6};
uint32_t oqs_sig_qteslapi_PARAM_Qx4[8] __attribute__((aligned(32))) = {PARAM_Q, 0, PARAM_Q, 0, PARAM_Q, 0, PARAM_Q, 0};
uint32_t oqs_sig_qteslapi_PARAM_Rx4[8] __attribute__((aligned(32))) = {PARAM_R, 0, PARAM_R, 0, PARAM_R, 0, PARAM_R, 0};
uint32_t oqs_sig_qteslapi_PARAM_QINVx4[8] __attribute__((aligned(32))) = {PARAM_QINV, 0, PARAM_QINV, 0, PARAM_QINV, 0, PARAM_QINV, 0};
uint32_t oqs_sig_qteslapi_PARAM_BARRx4[8] __attribute__((aligned(32))) = {PARAM_BARR_MULT, 0, PARAM_BARR_MULT, 0, PARAM_BARR_MULT, 0, PARAM_BARR_MULT, 0};

#else

int64_t oqs_sig_qteslapiii_N = PARAM_N;
int32_t oqs_sig_qteslapiii_Nx4 = 4 * PARAM_N;
int64_t oqs_sig_qteslapiii_H = PARAM_H;
int64_t oqs_sig_qteslapiii_N2m64 = 2 * PARAM_N - 64;
int64_t oqs_sig_qteslapiii_N4m128 = 4 * PARAM_N - 128;
int32_t oqs_sig_qteslapiii_ZEROS[8] __attribute__((aligned(32))) = {0, 0, 0, 0, 0, 0, 0, 0};
int32_t oqs_sig_qteslapiii_PERM_MASK[8] __attribute__((aligned(32))) = {1, 3, 5, 7, 0, 2, 4, 6};
uint32_t oqs_sig_qteslapiii_PARAM_Qx4[8] __attribute__((aligned(32))) = {PARAM_Q, 0, PARAM_Q, 0, PARAM_Q, 0, PARAM_Q, 0};
uint32_t oqs_sig_qteslapiii_PARAM_Rx4[8] __attribute__((aligned(32))) = {PARAM_R, 0, PARAM_R, 0, PARAM_R, 0, PARAM_R, 0};
uint32_t oqs_sig_qteslapiii_PARAM_QINVx4[8] __attribute__((aligned(32))) = {PARAM_QINV, 0, PARAM_QINV, 0, PARAM_QINV, 0, PARAM_QINV, 0};
uint32_t oqs_sig_qteslapiii_PARAM_BARRx4[8] __attribute__((aligned(32))) = {PARAM_BARR_MULT, 0, PARAM_BARR_MULT, 0, PARAM_BARR_MULT, 0, PARAM_BARR_MULT, 0};

#endif
