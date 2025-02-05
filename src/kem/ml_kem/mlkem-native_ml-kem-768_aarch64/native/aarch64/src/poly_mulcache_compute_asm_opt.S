/*
 * Copyright (c) 2024 The mlkem-native project authors
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../../../common.h"
#if defined(MLKEM_NATIVE_ARITH_BACKEND_AARCH64_OPT)
/* simpasm: header-end */

/* Montgomery multiplication, with precomputed Montgomery twist
 * Expects modulus in consts.h[0]. */
.macro mulmod dst, src, const, const_twisted
        sqrdmulh tmp0.8h,    \src\().8h, \const_twisted\().8h
        mul      \dst\().8h, \src\().8h, \const\().8h
        mls      \dst\().8h, tmp0.8h,    modulus.h[0]
.endm

/********************************************
 *          poly_mulcache_compute()         *
 ********************************************/


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

        .text
        .global MLKEM_ASM_NAMESPACE(poly_mulcache_compute_asm_opt)
        .balign 4
MLKEM_ASM_NAMESPACE(poly_mulcache_compute_asm_opt):
        mov wtmp, #3329
        dup modulus.8h, wtmp

        mov wtmp, #20159
        dup modulus_twisted.8h, wtmp

        mov count, #16
                                              // Instructions:    7
                                              // Expected cycles: 12
                                              // Expected IPC:    0.58

                                              // Cycle bound:     12.0
                                              // IPC bound:       0.58

                                              // Wall time:     0.01s
                                              // User time:     0.01s

                                              // ----- cycle (expected) ------>
                                              // 0                        25
                                              // |------------------------|----
        ldr q1, [x1, #16]                     // *.............................
        ldr q27, [x1], #32                    // ..*...........................
        ldr q23, [x2], #16                    // ....*.........................
        uzp2 v27.8H, v27.8H, v1.8H            // ......*.......................
        ldr q1, [x3], #16                     // .......*......................
        mul v2.8H, v27.8H, v23.8H             // .........*....................
        sqrdmulh v27.8H, v27.8H, v1.8H        // ...........*..................

                                               // ------ cycle (expected) ------>
                                               // 0                        25
                                               // |------------------------|-----
        // ldr q29, [x1, #16]                   // *..............................
        // ldr q21, [x2], #16                   // ....*..........................
        // ldr q27, [x1], #32                   // ..*............................
        // ldr q7, [x3], #16                    // .......*.......................
        // uzp2 v28.8H, v27.8H, v29.8H          // ......*........................
        // mul v2.8H, v28.8H, v21.8H            // .........*.....................
        // sqrdmulh v27.8H, v28.8H, v7.8H       // ...........*...................

        sub count, count, #1
poly_mulcache_compute_asm_opt_loop:
                                              // Instructions:    9
                                              // Expected cycles: 13
                                              // Expected IPC:    0.69

                                              // Cycle bound:     13.0
                                              // IPC bound:       0.69

                                              // Wall time:     0.09s
                                              // User time:     0.09s

                                              // ----- cycle (expected) ------>
                                              // 0                        25
                                              // |------------------------|----
        ldr q29, [x1, #16]                    // e.............................
        ldr q21, [x2], #16                    // ..e...........................
        mls v2.8H, v27.8H, v6.H[0]            // ....*.........................
        ldr q27, [x1], #32                    // .....e........................
        ldr q7, [x3], #16                     // .......e......................
        uzp2 v28.8H, v27.8H, v29.8H           // .........e....................
        str q2, [x0], #16                     // ..........*...................
        mul v2.8H, v28.8H, v21.8H             // ...........e..................
        sqrdmulh v27.8H, v28.8H, v7.8H        // ............e.................

                                                  // ------ cycle (expected) ------>
                                                  // 0                        25
                                                  // |------------------------|-----
        // ldr q3, [x1], #32                       // .....e.......'....~.......'....
        // ldr q4, [x1, #-16]                      // e............~............~....
        // ldr q1, [x2], #16                       // ..e..........'.~..........'.~..
        // ldr q2, [x3], #16                       // .......e.....'......~.....'....
        // uzp2 v0.8h, v3.8h, v4.8h                // .........e...'........~...'....
        // sqrdmulh v3.8h,    v0.8h, v2.8h         // ............e'...........~'....
        // mul      v5.8h, v0.8h, v1.8h            // ...........e.'..........~.'....
        // mls      v5.8h, v3.8h,    v6.h[0]       // ....~........'...*........'....
        // str q5, [x0], #16                       // ..........~..'.........*..'....

        sub count, count, 1
        cbnz count, poly_mulcache_compute_asm_opt_loop
                                          // Instructions:    2
                                          // Expected cycles: 5
                                          // Expected IPC:    0.40

                                          // Cycle bound:     5.0
                                          // IPC bound:       0.40

                                          // Wall time:     0.00s
                                          // User time:     0.00s

                                          // ----- cycle (expected) ------>
                                          // 0                        25
                                          // |------------------------|----
        mls v2.8H, v27.8H, v6.H[0]        // *.............................
        str q2, [x0], #16                 // ....*.........................

                                           // ------ cycle (expected) ------>
                                           // 0                        25
                                           // |------------------------|-----
        // mls v2.8H, v27.8H, v6.H[0]       // *..............................
        // str q2, [x0], #16                // ....*..........................


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

/* simpasm: footer-start */
#endif /* MLKEM_NATIVE_ARITH_BACKEND_AARCH64_OPT */
