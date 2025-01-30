/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../common.h"
#if defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_OPT)
/* simpasm: header-end */

/********************************************
 *             poly_tobytes()               *
 ********************************************/

        data0 .req v0
        data1 .req v1
        out0  .req v2
        out1  .req v3
        out2  .req v4
        tmp   .req v5

        dst   .req x0
        src   .req x1
        count .req x2

        .text
        .global MLKEM_ASM_NAMESPACE(poly_tobytes_asm_opt)
        .balign 4
MLKEM_ASM_NAMESPACE(poly_tobytes_asm_opt):

        mov count, #16
poly_tobytes_asm_opt_asm_loop_start:
        ld2 {data0.8h, data1.8h}, [src], #32

        // r[3 * i + 0] = (t0 >> 0);
        xtn out0.8b, data0.8h

        // r[3 * i + 1] = (t0 >> 8);
        shrn out1.8b, data0.8h, #8
        xtn tmp.8b, data1.8h
        // r[3 * i + 1] = (t0 >> 8) | (t1 << 4);
        sli out1.8b, tmp.8b, #4

        // r[3 * i + 2] = (t1 >> 4);
        shrn out2.8b, data1.8h, #4

        st3 {out0.8b, out1.8b, out2.8b}, [dst], #24

        subs count, count, #1
        cbnz count, poly_tobytes_asm_opt_asm_loop_start
        ret

        .unreq data0
        .unreq data1
        .unreq out0
        .unreq out1
        .unreq out2
        .unreq tmp
        .unreq dst
        .unreq src
        .unreq count

/* simpasm: footer-start */
#endif /* MLKEM_NATIVE_ARITH_BACKEND_AARCH64_OPT */
