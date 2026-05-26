/*
 * Copyright (c) The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLK_NATIVE_PPC64LE_SRC_CONSTS_H
#define MLK_NATIVE_PPC64LE_SRC_CONSTS_H
#include "../../../common.h"

/* Offsets into the constant table */
/* check-magic: off */
#define MLK_PPC_NQ_OFFSET 0
#define MLK_PPC_QINV_OFFSET 16
#define MLK_PPC_Q_OFFSET 32
#define MLK_PPC_C20159_OFFSET 48
#define MLK_PPC_N_INV_OFFSET 64
#define MLK_PPC_N_INV_TW_OFFSET 80
#define MLK_PPC_C1353_OFFSET 96
#define MLK_PPC_ZETA_NTT_OFFSET 112
#define MLK_PPC_ZETA_INTT_OFFSET 1120
#define MLK_PPC_ZETA_NTT_TW_OFFSET 2128
#define MLK_PPC_ZETA_INTT_TW_OFFSET 3136
/* check-magic: on */

#ifndef __ASSEMBLER__
#define mlk_ppc_qdata MLK_NAMESPACE(ppc_qdata)
/* 7 groups of 8 base constants + 4 twiddle tables * 63 rows * 8 values */
/* check-magic: 2072 == 7 * 8 + 4 * 63 * 8 */
MLK_INTERNAL_DATA_DECLARATION const int16_t mlk_ppc_qdata[2072];
#endif /* !__ASSEMBLER__ */

#endif /* !MLK_NATIVE_PPC64LE_SRC_CONSTS_H */
