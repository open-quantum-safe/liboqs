/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../common.h"
#if defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_CLEAN)

/*
 * Some modular arithmetic macros
 */

/* Barrett reduction */
.macro barrett_reduce a
        sqdmulh tmp.8h,   \a\().8h, modulus_twisted.h[0]
        srshr   tmp.8h,   tmp.8h,   #11
        mls     \a\().8h, tmp.8h,   modulus.h[0]
.endm

/* Montgomery multiplication, with precomputed Montgomery twist
 * Expects modulus in consts.h[0]. */
.macro mulmod dst, src, const, const_twisted
        sqrdmulh tmp0.8h,    \src\().8h, \const_twisted\().8h
        mul      \dst\().8h, \src\().8h, \const\().8h
        mls      \dst\().8h, tmp0.8h,    modulus.h[0]
.endm

/* Turns signed-canonical to unsigned canonical representative
 * through conditional addition of the modulus.
 *
 * Expected modulus in `modulus`. */
.macro scalar_signed_to_unsigned a
        sshr mask.8h, \a\().8h, #15
        and mask.16b, modulus.16b, mask.16b
        add \a\().8h, \a\().8h, mask.8h
.endm

/**********************************
 *          poly_reduce()         *
 **********************************/

.global MLKEM_ASM_NAMESPACE(poly_reduce_asm_clean)

        ptr               .req x0
        count             .req x1
        wtmp              .req w2

        data              .req v0
        q_data            .req q0

        tmp               .req v1
        mask              .req v2
        modulus           .req v3
        modulus_twisted   .req v4

.balign 4
MLKEM_ASM_NAMESPACE(poly_reduce_asm_clean):

        mov wtmp, #3329 // ML-KEM modulus
        dup modulus.8h, wtmp

        mov wtmp, #20159 // Barrett twist of 1 wrt 2^27
        dup modulus_twisted.8h, wtmp

        mov count, #8
loop_start:
        ldr q_data, [ptr], #64
        barrett_reduce data
        scalar_signed_to_unsigned data
        str q_data, [ptr, #-64]

        ldr q_data, [ptr, #-48]
        barrett_reduce data
        scalar_signed_to_unsigned data
        str q_data, [ptr, #-48]

        ldr q_data, [ptr, #-32]
        barrett_reduce data
        scalar_signed_to_unsigned data
        str q_data, [ptr, #-32]

        ldr q_data, [ptr, #-16]
        barrett_reduce data
        scalar_signed_to_unsigned data
        str q_data, [ptr, #-16]

        subs count, count, #1
        cbnz count, loop_start

        ret

        .unreq ptr
        .unreq count
        .unreq wtmp

        .unreq data
        .unreq q_data

        .unreq tmp
        .unreq mask
        .unreq modulus
        .unreq modulus_twisted

/********************************************
 *          poly_mulcache_compute()         *
 ********************************************/

.global MLKEM_ASM_NAMESPACE(poly_mulcache_compute_asm_clean)

        cache_ptr         .req x0
        data_ptr          .req x1
        zeta_ptr          .req x2
        zeta_twisted_ptr  .req x3
        count             .req x4
        wtmp              .req w5

        data_odd          .req v0
        zeta              .req v1
        q_zeta            .req q1
        zeta_twisted      .req v2
        q_zeta_twisted    .req q2

        tmp0              .req v3
        q_tmp0            .req q3
        tmp1              .req v4
        q_tmp1            .req q4
        dst               .req v5
        q_dst             .req q5

        modulus           .req v6
        modulus_twisted   .req v7

.balign 4
MLKEM_ASM_NAMESPACE(poly_mulcache_compute_asm_clean):
        mov wtmp, #3329
        dup modulus.8h, wtmp

        mov wtmp, #20159
        dup modulus_twisted.8h, wtmp

        mov count, #16
mulcache_compute_loop_start:
        ldr q_tmp0, [data_ptr], #32
        ldr q_tmp1, [data_ptr, #-16]
        ldr q_zeta, [zeta_ptr], #16
        ldr q_zeta_twisted, [zeta_twisted_ptr], #16

        // The mulcache of a polynomial a + b*X in Fq[X^2-zeta] is b*zeta;
        // Since tmp0 || tmp1 represents multiple such polynomails as
        // (a0,b0,a1,b1,...), extract only the odd elements.
        uzp2 data_odd.8h, tmp0.8h, tmp1.8h
        mulmod dst, data_odd, zeta, zeta_twisted

        str q_dst, [cache_ptr], #16

        subs count, count, #1
        cbnz count, mulcache_compute_loop_start

        ret

        .unreq cache_ptr
        .unreq data_ptr
        .unreq zeta_ptr
        .unreq zeta_twisted_ptr
        .unreq count
        .unreq wtmp

        .unreq data_odd
        .unreq zeta
        .unreq q_zeta
        .unreq zeta_twisted
        .unreq q_zeta_twisted

        .unreq tmp0
        .unreq q_tmp0
        .unreq tmp1
        .unreq q_tmp1
        .unreq dst
        .unreq q_dst

        .unreq modulus
        .unreq modulus_twisted

/********************************************
 *             poly_tobytes()               *
 ********************************************/
.global MLKEM_ASM_NAMESPACE(poly_tobytes_asm_clean)

        data0 .req v0
        data1 .req v1
        out0  .req v2
        out1  .req v3
        out2  .req v4
        tmp   .req v5

        dst   .req x0
        src   .req x1
        count .req x2

.balign 4
MLKEM_ASM_NAMESPACE(poly_tobytes_asm_clean):

        mov count, #16
poly_tobytes_asm_clean_asm_loop_start:
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
        cbnz count, poly_tobytes_asm_clean_asm_loop_start
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

/**********************************
 *          poly_tomont()         *
 **********************************/
.global MLKEM_ASM_NAMESPACE(poly_tomont_asm_clean)

        src               .req x0
        count             .req x1
        wtmp              .req w2

        data              .req v0
        q_data            .req q0
        res               .req v1
        q_res             .req q1

        factor            .req v2
        factor_t          .req v3
        modulus           .req v4
        modulus_twisted   .req v5

        tmp0              .req v6

.balign 4
MLKEM_ASM_NAMESPACE(poly_tomont_asm_clean):

        mov wtmp, #3329 // ML-KEM modulus
        dup modulus.8h, wtmp

        mov wtmp, #20159 // Barrett twist of 1 wrt 2^27
        dup modulus_twisted.8h, wtmp

        mov wtmp, #-1044 // 2^16 % 3329
        dup factor.8h, wtmp

        mov wtmp, #-10276 // Barrett twist of -1044 (wrt 2^16)
        dup factor_t.8h, wtmp

        mov count, #8
poly_tomont_asm_loop:

        ldr q_data, [src], #64
        mulmod res, data, factor, factor_t
        str q_res, [src, #-64]

        ldr q_data, [src, #-48]
        mulmod res, data, factor, factor_t
        str q_res, [src, #-48]

        ldr q_data, [src, #-32]
        mulmod res, data, factor, factor_t
        str q_res, [src, #-32]

        ldr q_data, [src, #-16]
        mulmod res, data, factor, factor_t
        str q_res, [src, #-16]

        sub count, count, #1
        cbnz count, poly_tomont_asm_loop

        ret

        .unreq src
        .unreq count
        .unreq wtmp

        .unreq data
        .unreq q_data
        .unreq res
        .unreq q_res

        .unreq factor
        .unreq factor_t
        .unreq modulus
        .unreq modulus_twisted

        .unreq tmp0

#endif /* MLKEM_NATIVE_ARITH_BACKEND_AARCH64_CLEAN */
