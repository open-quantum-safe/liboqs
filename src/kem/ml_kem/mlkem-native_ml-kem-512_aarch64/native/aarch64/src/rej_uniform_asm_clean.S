/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

/*************************************************
 * Name:        rej_uniform_asm_clean
 *
 * Description: Run rejection sampling on uniform random bytes to generate
 *              uniform random integers mod q
 *
 * Arguments:   - int16_t *r:          pointer to output buffer of MLKEM_N
 *                                     16-bit coefficients.
 *              - const uint8_t *buf:  pointer to input buffer
 *                                     (assumed to be uniform random bytes)
 *              - unsigned buflen:     length of input buffer in bytes.
 *                                     Must be a multiple of 24.
 *
 * Returns number of sampled 16-bit integers (at most MLKEM_N).
 **************************************************/
#include "../../../common.h"
#if defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_CLEAN) || \
    defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_OPT)

// We save the output on the stack first, and copy to the actual
// output buffer only in the end. This is because the main loop can overwrite
// by up to 62 bytes, which we account for here (we use 64 bytes for alignment).
#define STACK_SIZE (2*MLKEM_N + 64)
#define STACK_OFFSET_TMP_OUTPUT 0

.macro push_stack
        sub sp, sp, #STACK_SIZE
.endm

.macro pop_stack
        add sp, sp, #STACK_SIZE
.endm

    /* Parameters */
    output                      .req x0
    buf                         .req x1
    buflen                      .req w2
    table_idx                   .req x3

    len                         .req w4

    /* Temporary output on the stack */
    xtmp                        .req x7
    output_tmp                  .req x7
    output_tmp_base             .req x8

    /* Number of coefficients sampled so far */
    count                       .req w9
    buf_consumed                .req w10

    /* Temporary registers */
    tmp                         .req w11
    final_copy_count            .req w11

    rec_idx_0                   .req w12
    rec_idx_1                   .req w13
    rec_idx_2                   .req w14
    rec_idx_3                   .req w15

    ctr0                        .req w12
    ctr1                        .req w13
    ctr2                        .req w14
    ctr3                        .req w15

    ctr01                       .req ctr0
    ctr23                       .req ctr2

    /* Vector registers */

    buf0                        .req v0
    buf1                        .req v1
    buf2                        .req v2

    tmp0                        .req v4
    tmp1                        .req v5
    tmp2                        .req v6
    tmp3                        .req v7

    sign0                       .req v4
    sign1                       .req v5
    sign2                       .req v6
    sign3                       .req v7

    val0                        .req v16
    val0q                       .req q16
    val1                        .req v17
    val1q                       .req q17
    val2                        .req v18
    val2q                       .req q18
    val3                        .req v19
    val3q                       .req q19

    t0                          .req s20
    t1                          .req s21
    t2                          .req s22
    t3                          .req s23

    table0                      .req v24
    table0q                     .req q24
    table1                      .req v25
    table1q                     .req q25
    table2                      .req v26
    table2q                     .req q26
    table3                      .req v27
    table3q                     .req q27

    mlkem_q                     .req v30
    bits                        .req v31

.text
.global MLKEM_ASM_NAMESPACE(rej_uniform_asm_clean)
.balign 4
MLKEM_ASM_NAMESPACE(rej_uniform_asm_clean):
    push_stack

    // Load 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80
    movz xtmp, 0x1
    movk xtmp, 0x2, lsl 16
    movk xtmp, 0x4, lsl 32
    movk xtmp, 0x8, lsl 48
    mov bits.d[0], xtmp

    movz xtmp, 0x10
    movk xtmp, 0x20, lsl 16
    movk xtmp, 0x40, lsl 32
    movk xtmp, 0x80, lsl 48
    mov bits.d[1], xtmp

    movz tmp, #MLKEM_Q
    dup  mlkem_q.8h, tmp

    add output_tmp_base, sp, #STACK_OFFSET_TMP_OUTPUT
    mov output_tmp, output_tmp_base

    mov count, #0
    mov len, #MLKEM_N

    cmp buflen, #48
    b.lo loop48_end

loop48:
        // Finish once we've generated sufficiently many coefficients
        cmp     count, len
        b.hs    memory_copy

        // First, we unpack the byte stream into a stream of signed
        // coefficients, interpreting each consecutive 3 bytes as two
        // signed 12-bit coefficients, presented as 16-bit integers.
        //
        // We handle 16 such triples a time, and use ld3 for the required
        // de-interleaving of the byte stream.
        sub     buflen, buflen, #48
        ld3     {buf0.16b, buf1.16b, buf2.16b}, [buf], #48

        // Unpack 16 triples of bytes into 16 pairs of 16-bit integers,
        // represented as 4 vectors val0-val3.
        zip1    tmp0.16b, buf0.16b, buf1.16b
        zip2    tmp1.16b, buf0.16b, buf1.16b
        zip1    tmp2.16b, buf1.16b, buf2.16b
        zip2    tmp3.16b, buf1.16b, buf2.16b

        bic     tmp0.8h, #0xf0, lsl 8
        bic     tmp1.8h, #0xf0, lsl 8
        ushr    tmp2.8h, tmp2.8h, #4
        ushr    tmp3.8h, tmp3.8h, #4

        zip1    val0.8h, tmp0.8h, tmp2.8h
        zip2    val1.8h, tmp0.8h, tmp2.8h
        zip1    val2.8h, tmp1.8h, tmp3.8h
        zip2    val3.8h, tmp1.8h, tmp3.8h

        // At this point, val0-val3 are the signed integers to do rejection
        // sampling on. For each of them, do the following:
        // - Check which coefficients are within range, and represent the set
        //   of lane-indices of those coefficients as an 8-bit bitmap.
        // - Move the respective lanes to the front of the vector. This is the
        //   most complex part, and is done by interpreting the 8-bit bitmap as
        //   an index into a lookup table giving the lane-table to be use for
        //   the `tbl` instruction.
        // - Write the vector to the output buffer, but merely increase the output
        //   buffer pointer by the number of valid coefficients.

        // Set valid lanes to -1 (0b1...1)
        cmhi    sign0.8h, mlkem_q.8h, val0.8h
        cmhi    sign1.8h, mlkem_q.8h, val1.8h
        cmhi    sign2.8h, mlkem_q.8h, val2.8h
        cmhi    sign3.8h, mlkem_q.8h, val3.8h

        // If lane i is valid and has value -1, retain only i-th bit
        and     sign0.16b, sign0.16b, bits.16b
        and     sign1.16b, sign1.16b, bits.16b
        and     sign2.16b, sign2.16b, bits.16b
        and     sign3.16b, sign3.16b, bits.16b

        // Get 8-bit bitmap of valid lane indices by adding lanes
        uaddlv  t0, sign0.8h
        uaddlv  t1, sign1.8h
        uaddlv  t2, sign2.8h
        uaddlv  t3, sign3.8h

        fmov    rec_idx_0, t0
        fmov    rec_idx_1, t1
        fmov    rec_idx_2, t2
        fmov    rec_idx_3, t3

        ldr     table0q, [table_idx, rec_idx_0, uxtw #4]
        ldr     table1q, [table_idx, rec_idx_1, uxtw #4]
        ldr     table2q, [table_idx, rec_idx_2, uxtw #4]
        ldr     table3q, [table_idx, rec_idx_3, uxtw #4]

        // Compute number of valid coefficients. Recall that at this
        // point, lane i has value 2^i (hence popcount 1) if its coefficient
        // is valid, and 0 otherwise.
        cnt     sign0.16b, sign0.16b
        cnt     sign1.16b, sign1.16b
        cnt     sign2.16b, sign2.16b
        cnt     sign3.16b, sign3.16b

        // Extract number of valid coefficients
        uaddlv  t0, sign0.8h
        uaddlv  t1, sign1.8h
        uaddlv  t2, sign2.8h
        uaddlv  t3, sign3.8h

        fmov    ctr0, t0
        fmov    ctr1, t1
        fmov    ctr2, t2
        fmov    ctr3, t3

        // Move valid coefficients to the front
        tbl     val0.16b, {val0.16b}, table0.16b
        tbl     val1.16b, {val1.16b}, table1.16b
        tbl     val2.16b, {val2.16b}, table2.16b
        tbl     val3.16b, {val3.16b}, table3.16b

        str     val0q, [output_tmp]
        add     output_tmp, output_tmp, ctr0, uxtw #1

        str     val1q, [output_tmp]
        add     output_tmp, output_tmp, ctr1, uxtw #1

        str     val2q, [output_tmp]
        add     output_tmp, output_tmp, ctr2, uxtw #1

        str     val3q, [output_tmp]
        add     output_tmp, output_tmp, ctr3, uxtw #1

        add     ctr01, ctr0, ctr1
        add     ctr23, ctr2, ctr3
        add     count, count, ctr01
        add     count, count, ctr23

        cmp buflen, #48
        b.hs loop48
loop48_end:

    // Finish once we've generated sufficiently many coefficients
    cmp     count, len
    b.hs    memory_copy

    cmp buflen, #24
    b.lo memory_copy

        sub     buflen, buflen, #24
        ld3     {buf0.8b, buf1.8b, buf2.8b}, [buf], #24

        zip1    tmp0.16b, buf0.16b, buf1.16b
        zip1    tmp1.16b, buf1.16b, buf2.16b

        bic     tmp0.8h, #0xf0, lsl 8
        ushr    tmp1.8h, tmp1.8h, #4

        zip1    val0.8h, tmp0.8h, tmp1.8h
        zip2    val1.8h, tmp0.8h, tmp1.8h

        cmhi    sign0.8h, mlkem_q.8h, val0.8h
        cmhi    sign1.8h, mlkem_q.8h, val1.8h

        and     sign0.16b, sign0.16b, bits.16b
        and     sign1.16b, sign1.16b, bits.16b

        uaddlv  t0, sign0.8h
        uaddlv  t1, sign1.8h

        fmov    rec_idx_0, t0
        fmov    rec_idx_1, t1

        ldr     table0q, [table_idx, rec_idx_0, uxtw #4]
        ldr     table1q, [table_idx, rec_idx_1, uxtw #4]

        cnt     sign0.16b, sign0.16b
        cnt     sign1.16b, sign1.16b

        uaddlv  t0, sign0.8h
        uaddlv  t1, sign1.8h

        fmov    ctr0, t0
        fmov    ctr1, t1

        tbl     val0.16b, {val0.16b}, table0.16b
        tbl     val1.16b, {val1.16b}, table1.16b

        str     val0q, [output_tmp]
        add     output_tmp, output_tmp, ctr0, uxtw #1

        str     val1q, [output_tmp]
        add     output_tmp, output_tmp, ctr1, uxtw #1

        add     count, count, ctr0
        add     count, count, ctr1

memory_copy:
    // min = min(count,len)
    cmp  count, len
    csel count, count, len, lo

    // Always copy MLKEM_N coefficients from the stack to the destination,
    // even if not all of them may be valid. This simplifies the loop and
    // allows us to stick to vectorized code.
    mov final_copy_count, #0
    mov output_tmp, output_tmp_base
final_copy:
        ldr val0q, [output_tmp], #64
        ldr val1q, [output_tmp, #-48]
        ldr val2q, [output_tmp, #-32]
        ldr val3q, [output_tmp, #-16]
        str val0q, [output], #64
        str val1q, [output, #-48]
        str val2q, [output, #-32]
        str val3q, [output, #-16]
        add final_copy_count, final_copy_count, #32
        cmp final_copy_count, #MLKEM_N
        b.lt final_copy

    mov w0, count
    b return

return:
    pop_stack
    ret


/****************** REGISTER DEALLOCATIONS *******************/
    .unreq output
    .unreq buf
    .unreq buflen
    .unreq table_idx
    .unreq len
    .unreq output_tmp
    .unreq output_tmp_base
    .unreq count
    .unreq buf_consumed
    .unreq tmp
    .unreq xtmp
    .unreq final_copy_count
    .unreq rec_idx_0
    .unreq rec_idx_1
    .unreq rec_idx_2
    .unreq rec_idx_3
    .unreq ctr0
    .unreq ctr1
    .unreq ctr2
    .unreq ctr3
    .unreq ctr01
    .unreq ctr23
    .unreq buf0
    .unreq buf1
    .unreq buf2
    .unreq tmp0
    .unreq tmp1
    .unreq tmp2
    .unreq tmp3
    .unreq sign0
    .unreq sign1
    .unreq sign2
    .unreq sign3
    .unreq val0
    .unreq val0q
    .unreq val1
    .unreq val1q
    .unreq val2
    .unreq val2q
    .unreq val3
    .unreq val3q
    .unreq t0
    .unreq t1
    .unreq t2
    .unreq t3
    .unreq table0
    .unreq table0q
    .unreq table1
    .unreq table1q
    .unreq table2
    .unreq table2q
    .unreq table3
    .unreq table3q
    .unreq mlkem_q
    .unreq bits

#endif /* defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_CLEAN) ||
          defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_OPT) */
