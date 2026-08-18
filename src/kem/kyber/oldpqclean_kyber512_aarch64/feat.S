
/*
MIT License

Copyright (c) 2020 Bas Westerbaan
Copyright (c) 2023: Hanno Becker, Vincent Hwang, Matthias J. Kannwischer, Bo-Yin Yang, and Shang-Yi Yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#if (__APPLE__ && __ARM_FEATURE_CRYPTO) || (__ARM_FEATURE_SHA3)

.macro round
    ; Execute theta, but without xoring into the state yet.
    ; Compute parities p[i] = a[i] ^ a[5+i] ^ ... ^ a[20+i].
    eor3.16b v25, v0, v5, v10
    eor3.16b v26, v1, v6, v11
    eor3.16b v27, v2, v7, v12
    eor3.16b v28, v3, v8, v13
    eor3.16b v29, v4, v9, v14

    eor3.16b v25, v25, v15, v20
    eor3.16b v26, v26, v16, v21
    eor3.16b v27, v27, v17, v22
    eor3.16b v28, v28, v18, v23
    eor3.16b v29, v29, v19, v24

    rax1.2d v30, v29, v26 ; d[0] = rotl(p[1], 1) ^ p[4]
    rax1.2d v29, v27, v29 ; d[3] = rotl(p[4], 1) ^ p[2]
    rax1.2d v27, v25, v27 ; d[1] = rotl(p[2], 1) ^ p[0]
    rax1.2d v25, v28, v25 ; d[4] = rotl(p[0], 1) ^ p[3]
    rax1.2d v28, v26, v28 ; d[2] = rotl(p[3], 1) ^ p[1]

    ; Xor parities from step theta into the state at the same time
    ; as executing rho and pi.
    eor.16b v0, v0,  v30
    mov.16b v31, v1
    xar.2d v1,  v6,  v27, 20
    xar.2d v6,  v9,  v25, 44
    xar.2d v9,  v22, v28, 3
    xar.2d v22, v14, v25, 25
    xar.2d v14, v20, v30, 46
    xar.2d v20, v2,  v28, 2
    xar.2d v2,  v12, v28, 21
    xar.2d v12, v13, v29, 39
    xar.2d v13, v19, v25, 56
    xar.2d v19, v23, v29, 8
    xar.2d v23, v15, v30, 23
    xar.2d v15, v4,  v25, 37
    xar.2d v4,  v24, v25, 50
    xar.2d v24, v21, v27, 62
    xar.2d v21, v8,  v29, 9
    xar.2d v8,  v16, v27, 19
    xar.2d v16, v5,  v30, 28
    xar.2d v5,  v3,  v29, 36
    xar.2d v3,  v18, v29, 43
    xar.2d v18, v17, v28, 49
    xar.2d v17, v11, v27, 54
    xar.2d v11, v7,  v28, 58
    xar.2d v7,  v10, v30, 61
    xar.2d v10, v31, v27, 63

    ; Chi
    bcax.16b v25, v0,  v2,  v1
    bcax.16b v26, v1,  v3,  v2
    bcax.16b v2,  v2,  v4,  v3
    bcax.16b v3,  v3,  v0,  v4
    bcax.16b v4,  v4,  v1,  v0
    mov.16b v0, v25
    mov.16b v1, v26

    bcax.16b v25, v5,  v7,  v6
    bcax.16b v26, v6,  v8,  v7
    bcax.16b v7,  v7,  v9,  v8
    bcax.16b v8,  v8,  v5,  v9
    bcax.16b v9,  v9,  v6,  v5
    mov.16b v5, v25
    mov.16b v6, v26

    bcax.16b v25, v10,  v12,  v11
    bcax.16b v26, v11,  v13,  v12
    bcax.16b v12, v12,  v14,  v13
    bcax.16b v13, v13,  v10,  v14
    bcax.16b v14, v14,  v11,  v10
    mov.16b v10, v25
    mov.16b v11, v26

    bcax.16b v25, v15,  v17,  v16
    bcax.16b v26, v16,  v18,  v17
    bcax.16b v17, v17,  v19,  v18
    bcax.16b v18, v18,  v15,  v19
    bcax.16b v19, v19,  v16,  v15
    mov.16b v15, v25
    mov.16b v16, v26

    bcax.16b v25, v20,  v22,  v21
    bcax.16b v26, v21,  v23,  v22
    bcax.16b v22, v22,  v24,  v23
    bcax.16b v23, v23,  v20,  v24
    bcax.16b v24, v24,  v21,  v20
    mov.16b v20, v25
    mov.16b v21, v26

    ; iota
    ld1r {v25.2d}, [x1], #8
    eor.16b v0, v0, v25
.endm

.align 4
.global PQCLEAN_KYBER512_AARCH64_f1600x2
.global _PQCLEAN_KYBER512_AARCH64_f1600x2
PQCLEAN_KYBER512_AARCH64_f1600x2:
_PQCLEAN_KYBER512_AARCH64_f1600x2:
    stp d8,  d9,  [sp,#-16]!
    stp d10, d11, [sp,#-16]!
    stp d12, d13, [sp,#-16]!
    stp d14, d15, [sp,#-16]!

    mov x2, x0
    mov x3, #24

    ld1.2d {v0,  v1,  v2,  v3},  [x0], #64
    ld1.2d {v4,  v5,  v6,  v7},  [x0], #64
    ld1.2d {v8,  v9,  v10, v11}, [x0], #64
    ld1.2d {v12, v13, v14, v15}, [x0], #64
    ld1.2d {v16, v17, v18, v19}, [x0], #64
    ld1.2d {v20, v21, v22, v23}, [x0], #64
    ld1.2d {v24}, [x0]

loop:
    round

    subs x3, x3, #1
    cbnz x3, loop

    mov x0, x2
    st1.2d {v0,  v1,  v2,  v3},  [x0], #64
    st1.2d {v4,  v5,  v6,  v7},  [x0], #64
    st1.2d {v8,  v9,  v10, v11}, [x0], #64
    st1.2d {v12, v13, v14, v15}, [x0], #64
    st1.2d {v16, v17, v18, v19}, [x0], #64
    st1.2d {v20, v21, v22, v23}, [x0], #64
    st1.2d {v24}, [x0]

    ldp d14, d15, [sp], #16
    ldp d12, d13, [sp], #16
    ldp d10, d11, [sp], #16
    ldp d8,  d9,  [sp], #16

    ret lr

#endif
