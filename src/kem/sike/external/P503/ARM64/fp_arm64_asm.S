//*******************************************************************************************
// SIDH: an efficient supersingular isogeny cryptography library
//
// Abstract: field arithmetic in 64-bit ARMv8 assembly for P503 on Linux
//*******************************************************************************************

.text

// p503
p503:
.quad  0xFFFFFFFFFFFFFFFF
.quad  0xABFFFFFFFFFFFFFF
.quad  0x13085BDA2211E7A0
.quad  0x1B9BF6C87B7E7DAF
.quad  0x6045C6BDDA77A4D0
.quad  0x004066F541811E1E

// 2 * p503
p503x2:
.quad  0xFFFFFFFFFFFFFFFE
.quad  0xFFFFFFFFFFFFFFFF
.quad  0x57FFFFFFFFFFFFFF
.quad  0x2610B7B44423CF41
.quad  0x3737ED90F6FCFB5E
.quad  0xC08B8D7BB4EF49A0
.quad  0x0080CDEA83023C3C

// 4 * p503
p503x4:
.quad  0xFFFFFFFFFFFFFFFC
.quad  0xFFFFFFFFFFFFFFFF
.quad  0xAFFFFFFFFFFFFFFF
.quad  0x4C216F6888479E82
.quad  0x6E6FDB21EDF9F6BC
.quad  0x81171AF769DE9340
.quad  0x01019BD506047879

p503p1_nz_s8:
.quad  0x85BDA2211E7A0AC
.quad  0x9BF6C87B7E7DAF13
.quad  0x45C6BDDA77A4D01B
.quad  0x4066F541811E1E60


//***********************************************************************
//  Field addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_fpadd503_asm
oqs_kem_sike_fpadd503_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]

    // Add a + b
    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    adds    x3, x3, x11
    adcs    x4, x4, x12
    ldp     x9, x10,  [x0,#48]
    ldp     x17, x18, [x1,#48]
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    ldr     x11, p503x2
    adcs    x7, x7, x15
    ldr     x12, p503x2 + 8
    adcs    x8, x8, x16
    ldr     x13, p503x2 + 16
    adcs    x9, x9, x17
    ldr     x14, p503x2 + 24
    adc     x10, x10, x18
    
    //  Subtract 2xp503
    ldr     x15, p503x2 + 32
    subs    x3, x3, x11
    sbcs    x4, x4, x12
    ldr     x16, p503x2 + 40
    sbcs    x5, x5, x12
    sbcs    x6, x6, x13
    ldr     x17, p503x2 + 48
    sbcs    x7, x7, x14
    sbcs    x8, x8, x15
    sbcs    x9, x9, x16
    sbcs    x10, x10, x17
    sbc     x18, xzr, xzr

    // Add 2xp503 anded with the mask in x18
    and     x11, x11, x18 
    and     x12, x12, x18 
    and     x13, x13, x18 
    and     x14, x14, x18 
    and     x15, x15, x18 
    and     x16, x16, x18
    and     x17, x17, x18   

    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,  [x2,#0]
    adcs    x5, x5, x12
    adcs    x6, x6, x13
    stp     x5, x6,  [x2,#16]
    adcs    x7, x7, x14
    adcs    x8, x8, x15 
    stp     x7, x8,  [x2,#32]
    adcs    x9, x9, x16
    adc     x10, x10, x17
    stp     x9, x10, [x2,#48]
    ret


//***********************************************************************
//  Field subtraction
//  Operation: c [x2] = a [x0] - b [x1]
//*********************************************************************** 
.global oqs_kem_sike_fpsub503_asm
oqs_kem_sike_fpsub503_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]

    // Subtract a - b
    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    subs    x3, x3, x11
    sbcs    x4, x4, x12
    ldp     x9, x10,  [x0,#48]
    ldp     x17, x18, [x1,#48]
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    ldr     x11, p503x2
    sbcs    x7, x7, x15
    ldr     x12, p503x2 + 8
    sbcs    x8, x8, x16
    ldr     x13, p503x2 + 16
    sbcs    x9, x9, x17
    ldr     x14, p503x2 + 24
    sbcs    x10, x10, x18
    sbc     x18, xzr, xzr
    
    // Add 2xp503 anded with the mask in x18
    ldr     x15, p503x2 + 32
    and     x11, x11, x18 
    ldr     x16, p503x2 + 40 
    and     x12, x12, x18
    ldr     x17, p503x2 + 48
    and     x13, x13, x18 
    and     x14, x14, x18 
    and     x15, x15, x18 
    and     x16, x16, x18
    and     x17, x17, x18   

    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,  [x2,#0]
    adcs    x5, x5, x12
    adcs    x6, x6, x13
    stp     x5, x6,  [x2,#16]
    adcs    x7, x7, x14
    adcs    x8, x8, x15 
    stp     x7, x8,  [x2,#32]
    adcs    x9, x9, x16
    adc     x10, x10, x17 
    stp     x9, x10, [x2,#48]
    ret


///////////////////////////////////////////////////////////////// MACRO
.macro SUB503_PX  P0

    // Subtract a - b
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]
    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    subs    x3, x3, x11
    sbcs    x4, x4, x12
    ldp     x9, x10,  [x0,#48]
    ldp     x17, x18, [x1,#48]
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    ldr     x11, \P0
    sbcs    x7, x7, x15
    ldr     x12, \P0 + 8
    sbcs    x8, x8, x16
    ldr     x13, \P0 + 16
    sbcs    x9, x9, x17
    ldr     x14, \P0 + 24
    sbc     x10, x10, x18

    ldr     x15, \P0 + 32
    adds    x3, x3, x11
    ldr     x16, \P0 + 40 
    adcs    x4, x4, x12
    ldr     x17, \P0 + 48
    stp     x3, x4,  [x2,#0]
    adcs    x5, x5, x12
    adcs    x6, x6, x13
    stp     x5, x6,  [x2,#16]
    adcs    x7, x7, x14
    adcs    x8, x8, x15 
    stp     x7, x8,  [x2,#32]
    adcs    x9, x9, x16
    adc     x10, x10, x17 
    stp     x9, x10, [x2,#48]
  .endm


//***********************************************************************
//  Multiprecision subtraction with correction with 2*p503
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 2*p503
//*********************************************************************** 
.global oqs_kem_sike_mp_sub503_p2_asm
oqs_kem_sike_mp_sub503_p2_asm:

  SUB503_PX  p503x2
  ret


//***********************************************************************
//  Multiprecision subtraction with correction with 4*p503
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 4*p503
//*********************************************************************** 
.global oqs_kem_sike_mp_sub503_p4_asm
oqs_kem_sike_mp_sub503_p4_asm:

  SUB503_PX  p503x4
  ret

    
//////////////////////////////////////////// MACRO
.macro MUL128_COMBA_CUT  A0, A1, B0, B1, C0, C1, C2, C3, T0
    mul     \A0, \A1, \B0
    umulh   \B0, \A1, \B0
    adds    \C1, \C1, \C3
    adc     \C2, \C2, xzr
    
    mul     \T0, \A1, \B1
    umulh   \B1, \A1, \B1
    adds    \C1, \C1, \A0
    adcs    \C2, \C2, \B0
    adc     \C3, xzr, xzr
    
    adds    \C2, \C2, \T0
    adc     \C3, \C3, \B1
.endm


//////////////////////////////////////////// MACRO
.macro    MUL256_KARATSUBA_COMBA  M,A0,A1,A2,A3,B0,B1,B2,B3,C0,C1,C2,C3,C4,C5,C6,C7,T0,T1

    // A0-A1 <- AH + AL, T0 <- mask
    adds    \A0, \A0, \A2
    adcs    \A1, \A1, \A3
    adc     \T0, xzr, xzr

    // C6, T1 <- BH + BL, C7 <- mask
    adds    \C6, \B0, \B2
    adcs    \T1, \B1, \B3
    adc     \C7, xzr, xzr
    
    // C0-C1 <- masked (BH + BL)
    sub     \C2, xzr, \T0
    sub     \C3, xzr, \C7
    and     \C0, \C6, \C2
    and     \C1, \T1, \C2

    // C4-C5 <- masked (AH + AL), T0 <- combined carry
    and     \C4, \A0, \C3
    and     \C5, \A1, \C3
    mul     \C2, \A0, \C6
    mul     \C3, \A0, \T1
    and     \T0, \T0, \C7

    // C0-C1, T0 <- (AH+AL) x (BH+BL), part 1
    adds    \C0, \C4, \C0
    umulh   \C4, \A0, \T1    
    adcs    \C1, \C5, \C1
    umulh   \C5, \A0, \C6
    adc     \T0, \T0, xzr

    // C2-C5 <- (AH+AL) x (BH+BL), low part
    MUL128_COMBA_CUT  \A0, \A1, \C6, \T1, \C2, \C3, \C4, \C5, \C7
    ldp     \A0, \A1, [\M,#0]
    
    // C2-C5, T0 <- (AH+AL) x (BH+BL), final part
    adds    \C4, \C0, \C4
    umulh   \C7, \A0, \B0 
    umulh   \T1, \A0, \B1 
    adcs    \C5, \C1, \C5
    mul     \C0, \A0, \B0
    mul     \C1, \A0, \B1  
    adc     \T0, \T0, xzr

    // C0-C1, T1, C7 <- AL x BL
    MUL128_COMBA_CUT  \A0, \A1, \B0, \B1, \C0, \C1, \T1, \C7, \C6
    
    // C2-C5, T0 <- (AH+AL) x (BH+BL) - ALxBL
    mul     \A0, \A2, \B2
    umulh   \B0, \A2, \B2
    subs    \C2, \C2, \C0 
    sbcs    \C3, \C3, \C1
    sbcs    \C4, \C4, \T1
    mul     \A1, \A2, \B3
    umulh   \C6, \A2, \B3   
    sbcs    \C5, \C5, \C7
    sbc     \T0, \T0, xzr

    // A0, A1, C6, B0 <- AH x BH 
    MUL128_COMBA_CUT  \A2, \A3, \B2, \B3, \A0, \A1, \C6, \B0, \B1
    
    // C2-C5, T0 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    subs    \C2, \C2, \A0 
    sbcs    \C3, \C3, \A1
    sbcs    \C4, \C4, \C6
    sbcs    \C5, \C5, \B0
    sbc     \T0, \T0, xzr
    
    adds    \C2, \C2, \T1 
    adcs    \C3, \C3, \C7
    adcs    \C4, \C4, \A0
    adcs    \C5, \C5, \A1
    adcs    \C6, \T0, \C6
    adc     \C7, \B0, xzr
.endm


//***********************************************************************************
//  512-bit integer multiplication using Karatsuba (two levels), Comba (lower level)
//  Operation: c [x2] = a [x0] * b [x1]
//*********************************************************************************** 
.global oqs_kem_sike_mul503_asm
oqs_kem_sike_mul503_asm:
    sub     sp, sp, #96
    ldp     x3, x4, [x0]
    ldp     x5, x6, [x0,#16]
    ldp     x7, x8, [x0,#32]
    ldp     x9, x10, [x0,#48]
    stp     x25, x26, [sp,#48]
    stp     x27, x28, [sp,#64]
    str     x29, [sp, #80]

    // x26-x29 <- AH + AL, x7 <- mask
    adds    x26, x3, x7
    ldp     x11, x12, [x1,#0]
    adcs    x27, x4, x8
    ldp     x13, x14, [x1,#16]
    adcs    x28, x5, x9
    ldp     x15, x16, [x1,#32]
    adcs    x29, x6, x10
    ldp     x17, x18, [x1,#48]
    adc     x7, xzr, xzr
    stp     x19, x20, [sp,#0]

    // x11-x14 <- BH + BL, x8 <- mask
    adds    x11, x11, x15
    stp     x21, x22, [sp,#16]
    adcs    x12, x12, x16
    stp     x23, x24, [sp,#32]
    adcs    x13, x13, x17
    adcs    x14, x14, x18
    adc     x8, xzr, xzr
    
    // x15-x18 <- masked (BH + BL)
    sub     x9, xzr, x7
    sub     x10, xzr, x8
    and     x15, x11, x9
    and     x16, x12, x9
    and     x17, x13, x9
    and     x18, x14, x9

    // x19-x22 <- masked (AH + AL)
    and     x19, x26, x10
    and     x20, x27, x10
    and     x21, x28, x10
    and     x22, x29, x10

    // x15-x18 <- masked (AH+AL) + masked (BH+BL), step 1
    adds    x15, x15, x19
    adcs    x16, x16, x20
    adcs    x17, x17, x21
    stp     x26, x27, [x2,#0]
    adc     x18, x18, x22
    
    // x8-x10,x19-x23 <- (AH+AL) x (BH+BL), low part
    MUL256_KARATSUBA_COMBA  x2, x26, x27, x28, x29, x11, x12, x13, x14, x8, x9, x10, x19, x20, x21, x22, x23, x24, x25  
    
    // x15-x18 <- (AH+AL) x (BH+BL), final step
    adds    x15, x15, x20
    ldp     x11, x12, [x1,#0]
    adcs    x16, x16, x21
    adcs    x17, x17, x22
    ldp     x13, x14, [x1,#16]
    adc     x18, x18, x23

    // x20-x27 <- AL x BL
    MUL256_KARATSUBA_COMBA  x0, x3, x4, x5, x6, x11, x12, x13, x14, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29
    
    // x8-x10, x19, x15-x18 <- (AH+AL) x (BH+BL) - ALxBL
    subs    x8, x8, x20 
    ldp     x3, x4, [x0,#32]
    sbcs    x9, x9, x21
    ldp     x5, x6, [x0,#48]
    sbcs    x10, x10, x22
    ldp     x11, x12, [x1,#32]
    sbcs    x19, x19, x23
    ldp     x13, x14, [x1,#48]
    sbcs    x15, x15, x24
    stp     x20, x21, [x2]
    sbcs    x16, x16, x25
    stp     x22, x23, [x2,#16]
    sbcs    x17, x17, x26
    stp     x24, x25, [x2,#32]
    sbc     x18, x18, x27


    // x20-x25, x1, x7 <- AH x BH
    add     x0, x0, #32
    MUL256_KARATSUBA_COMBA  x0, x3, x4, x5, x6, x11, x12, x13, x14, x20, x21, x22, x23, x24, x25, x1, x7, x28, x29
    
    // x8-x10, x19, x15-x18 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    subs    x8, x8, x20 
    sbcs    x9, x9, x21
    ldp     x3, x4, [x2,#32]
    sbcs    x10, x10, x22
    sbcs    x19, x19, x23
    ldr     x29, [sp,#80]
    sbcs    x15, x15, x24
    sbcs    x16, x16, x25
    sbcs    x17, x17, x1
    sbc     x18, x18, x7
    
    adds    x8, x8, x3 
    adcs    x9, x9, x4
    stp     x8, x9, [x2,#32]
    adcs    x10, x10, x26
    adcs    x19, x19, x27
    stp     x10, x19, [x2,#48]    
    adcs    x15, x15, x20 
    ldp     x19, x20, [sp,#0]  
    ldp     x27, x28, [sp,#64]   
    adcs    x16, x16, x21
    stp     x15, x16, [x2,#64]
    adcs    x17, x17, x22
    ldp     x21, x22, [sp,#16]
    adcs    x18, x18, x23
    stp     x17, x18, [x2,#80] 
    adcs    x24, x24, xzr
    stp     x24, x25, [x2,#96] 
    adcs    x25, x25, xzr
    ldp     x23, x24, [sp,#32]
    adcs    x1, x1, xzr
    ldp     x25, x26, [sp,#48]
    adc     x7, x7, xzr
    stp     x1, x7,   [x2,#112]    
    
    add     sp, sp, #96
    ret
    
        
//////////////////////////////////////////// MACRO
.macro MUL128x256_COMBA_CUT  A0, A1, B0, B1, B2, B3, C0, C1, C2, C3, C4, C5, T0, T1, T2, T3
    mul     \T0, \A1, \B0
    umulh   \T1, \A1, \B0 
    adds    \C1, \C1, \C3
    adc     \C2, \C2, xzr 

    mul     \T2, \A0, \B2
    umulh   \T3, \A0, \B2 
    adds    \C1, \C1, \T0
    adcs    \C2, \C2, \T1
    adc     \C3, xzr, xzr
    
    mul     \T0, \A1, \B1
    umulh   \T1, \A1, \B1    
    adds    \C2, \C2, \T2
    adcs    \C3, \C3, \T3
    adc     \C4, xzr, xzr
    
    mul     \T2, \A0, \B3
    umulh   \T3, \A0, \B3 
    adds    \C2, \C2, \T0
    adcs    \C3, \C3, \T1
    adc     \C4, \C4, xzr
    
    mul     \T0, \A1, \B2
    umulh   \T1, \A1, \B2 
    adds    \C3, \C3, \T2
    adcs    \C4, \C4, \T3
    adc     \C5, xzr, xzr
    
    mul     \T2, \A1, \B3
    umulh   \T3, \A1, \B3 
    adds    \C3, \C3, \T0
    adcs    \C4, \C4, \T1
    adc     \C5, \C5, xzr
    adds    \C4, \C4, \T2
    adc     \C5, \C5, \T3
.endm


//**************************************************************************************
//  Montgomery reduction
//  Based on method described in Faz-Hernandez et al. https://eprint.iacr.org/2017/1015  
//  Operation: mc [x1] = ma [x0]
//  NOTE: ma=mc is not allowed
//************************************************************************************** 
.global oqs_kem_sike_rdc503_asm
oqs_kem_sike_rdc503_asm:
    sub     sp, sp, #96
    stp     x23, x24, [sp, #32]
    stp     x25, x26, [sp, #48]
    ldp     x2, x3, [x0,#0]       // a[0-1]

    // Load the prime constant
    ldr     x24, p503p1_nz_s8 + 0
    ldr     x25, p503p1_nz_s8 + 8 
    stp     x19, x20, [sp]
    stp     x21, x22, [sp, #16] 

    // a[0-1] x p503p1_nz_s8 --> result: x4:x9
    mul     x4, x2, x24           // a[0] x p503p1_nz_s8[0]
    umulh   x7, x2, x24 
    stp     x27, x28, [sp, #64]
    stp     x29, x30, [sp, #80] 
    ldr     x26, p503p1_nz_s8 + 16
    ldr     x27, p503p1_nz_s8 + 24       
    mul     x5, x2, x25           // a[0] x p503p1_nz_s8[1]
    umulh   x6, x2, x25
    MUL128x256_COMBA_CUT x2, x3, x24, x25, x26, x27, x4, x5, x6, x7, x8, x9, x28, x29, x30, x10    

    orr     x10, xzr, x9, lsr #8
    lsl     x9, x9, #56
    orr     x9, x9, x8, lsr #8
    lsl     x8, x8, #56  
    orr     x8, x8, x7, lsr #8
    lsl     x7, x7, #56
    orr     x7, x7, x6, lsr #8
    lsl     x6, x6, #56  
    ldp     x3, x11, [x0,#16]     // a[2]
    orr     x6, x6, x5, lsr #8
    lsl     x5, x5, #56   
    ldp     x12, x13, [x0,#32]
    orr     x5, x5, x4, lsr #8
    lsl     x4, x4, #56
    ldp     x14, x15, [x0,#48] 
    
    adds    x11, x4, x11          // a[3]
    adcs    x12, x5, x12          // a[4]
    ldp     x16, x17, [x0,#64]  
    adcs    x13, x6, x13
    adcs    x14, x7, x14  
    ldp     x18, x19, [x0,#80]  
    adcs    x15, x8, x15  
    mul     x4, x3, x24           // a[2] x p503p1_nz_s8[0]
    umulh   x7, x3, x24
    adcs    x16, x9, x16
    adcs    x17, x10, x17
    ldp     x20, x21, [x0,#96]  
    adcs    x18, xzr, x18 
    adcs    x19, xzr, x19        
    ldp     x22, x23, [x0,#112] 
    mul     x5, x3, x25           // a[2] x p503p1_nz_s8[1] 
    umulh   x6, x3, x25
    adcs    x20, xzr, x20
    adcs    x21, xzr, x21
    adcs    x22, xzr, x22
    adc     x23, xzr, x23

    // a[2-3] x p503p1_nz_s8 --> result: x4:x9
    MUL128x256_COMBA_CUT x3, x11, x24, x25, x26, x27, x4, x5, x6, x7, x8, x9, x28, x29, x30, x10

    orr     x10, xzr, x9, lsr #8
    lsl     x9, x9, #56
    orr     x9, x9, x8, lsr #8
    lsl     x8, x8, #56
    orr     x8, x8, x7, lsr #8
    lsl     x7, x7, #56
    orr     x7, x7, x6, lsr #8
    lsl     x6, x6, #56
    orr     x6, x6, x5, lsr #8
    lsl     x5, x5, #56
    orr     x5, x5, x4, lsr #8
    lsl     x4, x4, #56
    
    adds    x13, x4, x13          // a[5]
    adcs    x14, x5, x14          // a[6]
    adcs    x15, x6, x15
    adcs    x16, x7, x16
    mul     x4, x12, x24          // a[4] x p503p1_nz_s8[0]
    umulh   x7, x12, x24
    adcs    x17, x8, x17
    adcs    x18, x9, x18
    adcs    x19, x10, x19
    adcs    x20, xzr, x20
    mul     x5, x12, x25          // a[4] x p503p1_nz_s8[1]
    umulh   x6, x12, x25
    adcs    x21, xzr, x21
    adcs    x22, xzr, x22
    adc     x23, xzr, x23

    // a[4-5] x p503p1_nz_s8 --> result: x4:x9
    MUL128x256_COMBA_CUT x12, x13, x24, x25, x26, x27, x4, x5, x6, x7, x8, x9, x28, x29, x30, x10    

    orr     x10, xzr, x9, lsr #8
    lsl     x9, x9, #56
    orr     x9, x9, x8, lsr #8
    lsl     x8, x8, #56
    orr     x8, x8, x7, lsr #8
    lsl     x7, x7, #56
    orr     x7, x7, x6, lsr #8
    lsl     x6, x6, #56
    orr     x6, x6, x5, lsr #8
    lsl     x5, x5, #56
    orr     x5, x5, x4, lsr #8
    lsl     x4, x4, #56
    
    adds    x15, x4, x15          // a[7]
    adcs    x16, x5, x16          // a[8]
    adcs    x17, x6, x17
    adcs    x18, x7, x18
    mul     x4, x14, x24          // a[6] x p503p1_nz_s8[0]
    umulh   x7, x14, x24
    adcs    x19, x8, x19
    adcs    x20, x9, x20
    adcs    x21, x10, x21
    mul     x5, x14, x25          // a[6] x p503p1_nz_s8[1]
    umulh   x6, x14, x25
    adcs    x22, xzr, x22
    adc     x23, xzr, x23

    // a[6-7] x p503p1_nz_s8 --> result: x4:x9
    MUL128x256_COMBA_CUT x14, x15, x24, x25, x26, x27, x4, x5, x6, x7, x8, x9, x28, x29, x30, x10

    orr     x10, xzr, x9, lsr #8
    lsl     x9, x9, #56
    orr     x9, x9, x8, lsr #8
    lsl     x8, x8, #56
    orr     x8, x8, x7, lsr #8
    lsl     x7, x7, #56
    orr     x7, x7, x6, lsr #8
    lsl     x6, x6, #56
    ldp     x25, x26, [sp, #48]
    orr     x6, x6, x5, lsr #8
    lsl     x5, x5, #56
    ldp     x27, x28, [sp, #64]
    orr     x5, x5, x4, lsr #8
    lsl     x4, x4, #56
    ldp     x29, x30, [sp, #80]
    
    adds    x17, x4, x17
    stp     x16, x17, [x1,#0]     // Final result
    adcs    x18, x5, x18
    adcs    x19, x6, x19
    stp     x18, x19, [x1,#16]   
    adcs    x20, x7, x20 
    adcs    x21, x8, x21
    stp     x20, x21, [x1,#32]
    ldp     x19, x20, [sp]
    adcs    x22, x9, x22
    adc     x23, x10, x23
    stp     x22, x23, [x1,#48]    
    ldp     x21, x22, [sp, #16]
    ldp     x23, x24, [sp, #32]
    add     sp, sp, #96
    ret


//***********************************************************************
//  503-bit multiprecision addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_add503_asm
oqs_kem_sike_mp_add503_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]

    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,   [x2,#0]
    ldp     x9, x10,  [x0,#48]
    ldp     x17, x18, [x1,#48]
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    stp     x5, x6,   [x2,#16]
    adcs    x7, x7, x15
    adcs    x8, x8, x16
    stp     x7, x8,   [x2,#32]
    adcs    x9, x9, x17
    adc     x10, x10, x18
    stp     x9, x10,  [x2,#48]
    ret    


//***********************************************************************
//  2x503-bit multiprecision addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_add503x2_asm
oqs_kem_sike_mp_add503x2_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]

    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,   [x2,#0]
    ldp     x9, x10,  [x0,#48]
    ldp     x17, x18, [x1,#48]
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    stp     x5, x6,   [x2,#16]
    adcs    x7, x7, x15
    adcs    x8, x8, x16
    stp     x7, x8,   [x2,#32]
    adcs    x9, x9, x17
    adcs    x10, x10, x18
    stp     x9, x10,  [x2,#48]
    
    ldp     x3, x4,   [x0,#64]
    ldp     x11, x12, [x1,#64]
    ldp     x5, x6,   [x0,#80]
    ldp     x13, x14, [x1,#80]

    ldp     x7, x8,   [x0,#96]
    ldp     x15, x16, [x1,#96]
    adcs    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,   [x2,#64]
    ldp     x9, x10,  [x0,#112]
    ldp     x17, x18, [x1,#112]
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    stp     x5, x6,   [x2,#80]
    adcs    x7, x7, x15
    adcs    x8, x8, x16
    stp     x7, x8,   [x2,#96]
    adcs    x9, x9, x17
    adc     x10, x10, x18
    stp     x9, x10,  [x2,#112]
    ret


//***********************************************************************
//  2x503-bit multiprecision subtraction/addition
//  Operation: c [x2] = a [x0] - b [x1]. If c < 0, add p503*2^512
//*********************************************************************** 
.global oqs_kem_sike_mp_subadd503x2_asm
oqs_kem_sike_mp_subadd503x2_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]
    subs    x3, x3, x11
    sbcs    x4, x4, x12
    stp     x3, x4,   [x2,#0]
    ldp     x7, x8,   [x0,#32]
    ldp     x11, x12, [x1,#32]
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    stp     x5, x6,   [x2,#16]
    ldp     x9, x10,  [x0,#48]
    ldp     x13, x14, [x1,#48]
    sbcs    x7, x7, x11
    sbcs    x8, x8, x12
    stp     x7, x8,   [x2,#32]
    sbcs    x9, x9, x13
    sbcs    x10, x10, x14
    stp     x9, x10,  [x2,#48]
    
    ldp     x3, x4,   [x0,#64]
    ldp     x11, x12, [x1,#64]
    ldp     x5, x6,   [x0,#80]
    ldp     x13, x14, [x1,#80]
    sbcs    x3, x3, x11
    sbcs    x4, x4, x12
    ldp     x7, x8,   [x0,#96]
    ldp     x11, x12, [x1,#96]
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    ldp     x9, x10,  [x0,#112]
    ldp     x13, x14, [x1,#112]
    sbcs    x7, x7, x11 
    ldr     x11, p503
    sbcs    x8, x8, x12
    ldr     x12, p503 + 8
    sbcs    x9, x9, x13
    ldr     x13, p503 + 16
    sbcs    x10, x10, x14
    ldr     x14, p503 + 24
    sbc     x0, xzr, xzr

    // Add p503 anded with the mask in x0
    ldr     x15, p503 + 32
    ldr     x16, p503 + 40
    and     x11, x11, x0 
    and     x12, x12, x0 
    and     x13, x13, x0 
    and     x14, x14, x0 
    and     x15, x15, x0  
    and     x16, x16, x0  
    adds    x3, x3, x11    
    adcs    x4, x4, x11 
    stp     x3, x4,   [x2,#64]  
    adcs    x5, x5, x11   
    adcs    x6, x6, x12  
    stp     x5, x6,   [x2,#80] 
    adcs    x7, x7, x13   
    adcs    x8, x8, x14 
    stp     x7, x8,   [x2,#96] 
    adcs    x9, x9, x15  
    adc     x10, x10, x16     
    stp     x9, x10,  [x2,#112]
    ret


//***********************************************************************
//  Double 2x503-bit multiprecision subtraction
//  Operation: c [x2] = c [x2] - a [x0] - b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_dblsub503x2_asm
oqs_kem_sike_mp_dblsub503x2_asm:
    sub     sp, sp, #32
    stp     x27, x28, [sp, #0]
    stp     x29, x30, [sp, #16]
    ldp     x3, x4,   [x2,#0]
    ldp     x5, x6,   [x2,#16]

    ldp     x27, x28, [x0,#0]
    ldp     x29, x30, [x0,#16]
    ldp     x7, x8,   [x2,#32]
    ldp     x9, x10,  [x2,#48]
    subs    x3, x3, x27
    sbcs    x4, x4, x28
    sbcs    x5, x5, x29
    sbcs    x6, x6, x30
    ldp     x27, x28, [x0,#32]
    ldp     x29, x30, [x0,#48]
    ldp     x11, x12, [x2,#64]
    ldp     x13, x14, [x2,#80]
    sbcs    x7, x7, x27
    sbcs    x8, x8, x28
    sbcs    x9, x9, x29
    sbcs    x10, x10, x30
    ldp     x27, x28, [x0,#64]
    ldp     x29, x30, [x0,#80]
    ldp     x15, x16, [x2,#96]
    ldp     x17, x18, [x2,#112]
    sbcs    x11, x11, x27
    sbcs    x12, x12, x28
    sbcs    x13, x13, x29
    sbcs    x14, x14, x30
    ldp     x27, x28, [x0,#96]
    ldp     x29, x30, [x0,#112]
    sbcs    x15, x15, x27
    sbcs    x16, x16, x28
    ldp     x27, x28, [x1,#0]
    sbcs    x17, x17, x29
    sbc     x18, x18, x30

    ldp     x29, x30, [x1,#16]
    subs    x3, x3, x27
    sbcs    x4, x4, x28
    stp     x3, x4,   [x2,#0]
    ldp     x27, x28, [x1,#32]
    sbcs    x5, x5, x29
    sbcs    x6, x6, x30
    stp     x5, x6,   [x2,#16]
    ldp     x29, x30, [x1,#48]
    sbcs    x7, x7, x27
    sbcs    x8, x8, x28
    stp     x7, x8,   [x2,#32]
    ldp     x27, x28, [x1,#64]
    sbcs    x9, x9, x29
    sbcs    x10, x10, x30
    stp     x9, x10,  [x2,#48]
    ldp     x29, x30, [x1,#80]
    sbcs    x11, x11, x27
    sbcs    x12, x12, x28
    stp     x11, x12, [x2,#64]
    ldp     x27, x28, [x1,#96]
    sbcs    x13, x13, x29
    sbcs    x14, x14, x30
    stp     x13, x14, [x2,#80]
    ldp     x29, x30, [x1,#112]
    sbcs    x15, x15, x27
    sbcs    x16, x16, x28
    stp     x15, x16, [x2,#96]
    ldp     x27, x28, [sp, #0]
    sbcs    x17, x17, x29
    sbc     x18, x18, x30
    stp     x17, x18, [x2,#112]    
    ldp     x29, x30, [sp, #16]
    add     sp, sp, #32
    ret
