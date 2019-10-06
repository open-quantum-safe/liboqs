//*******************************************************************************************
// SIDH: an efficient supersingular isogeny cryptography library
//
// Abstract: field arithmetic in 64-bit ARMv8 assembly for P751 on Linux
//*******************************************************************************************

.text

// p751
p751:
.quad  0xFFFFFFFFFFFFFFFF
.quad  0xEEAFFFFFFFFFFFFF
.quad  0xE3EC968549F878A8
.quad  0xDA959B1A13F7CC76
.quad  0x084E9867D6EBE876
.quad  0x8562B5045CB25748
.quad  0x0E12909F97BADC66
.quad  0x00006FE5D541F71C

// 2 * p751
p751x2:
.quad  0xFFFFFFFFFFFFFFFE
.quad  0xFFFFFFFFFFFFFFFF
.quad  0xDD5FFFFFFFFFFFFF
.quad  0xC7D92D0A93F0F151
.quad  0xB52B363427EF98ED
.quad  0x109D30CFADD7D0ED
.quad  0x0AC56A08B964AE90
.quad  0x1C25213F2F75B8CD
.quad  0x0000DFCBAA83EE38

// (p751 + 1)/2^47
p751p1_nz_s32:
.quad    0x2D0A93F0F151DD60
.quad    0x363427EF98EDC7D9
.quad    0x30CFADD7D0EDB52B
.quad    0x6A08B964AE90109D
.quad    0x213F2F75B8CD0AC5
.quad    0xDFCBAA83EE381C25


//***********************************************************************
//  Field addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global fpadd751_asm
fpadd751_asm:
    sub     sp, sp, #16
    stp     x19, x20, [sp]

    // Add a + b
    ldp     x3, x4,   [x0,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x15, x16, [x1,#0]
    ldp     x17, x19, [x1,#16]
    adds    x3, x3, x15
    adcs    x4, x4, x16
    adcs    x5, x5, x17
    adcs    x6, x6, x19
    ldp     x7, x8,   [x0,#32]
    ldp     x9, x10,  [x0,#48]
    ldp     x15, x16, [x1,#32]
    ldp     x17, x19, [x1,#48]
    adcs    x7, x7, x15
    adcs    x8, x8, x16
    adcs    x9, x9, x17
    adcs    x10, x10, x19
    ldp     x11, x12, [x0,#64]
    ldp     x13, x14, [x0,#80]
    ldp     x15, x16, [x1,#64]
    ldp     x17, x19, [x1,#80]
    adcs    x11, x11, x15
    adcs    x12, x12, x16
    adcs    x13, x13, x17
    adc     x14, x14, x19
    
    //  Subtract 2xp751
    ldr     x16, p751x2
    ldr     x17, p751x2 + 8
    ldr     x19, p751x2 + 16
    subs    x3, x3, x16
    sbcs    x4, x4, x17
    sbcs    x5, x5, x17
    sbcs    x6, x6, x17
    sbcs    x7, x7, x17
    sbcs    x8, x8, x19
    ldr     x0, p751x2 + 24
    ldr     x1, p751x2 + 32
    ldr     x20, p751x2 + 40
    sbcs    x9, x9, x0
    sbcs    x10, x10, x1
    sbcs    x11, x11, x20
    ldr     x0, p751x2 + 48
    ldr     x1, p751x2 + 56
    ldr     x20, p751x2 + 64
    sbcs    x12, x12, x0
    sbcs    x13, x13, x1
    sbcs    x14, x14, x20
    sbc     x15, xzr, xzr

    // Add 2xp751 anded with the mask in x15
    and     x16, x16, x15 
    and     x17, x17, x15 
    and     x19, x19, x15 
    and     x0, x0, x15 
    and     x1, x1, x15 
    and     x20, x20, x15  
	
    adds    x3, x3, x16
    adcs    x4, x4, x17
    adcs    x5, x5, x17
    adcs    x6, x6, x17
    adcs    x7, x7, x17
    adcs    x8, x8, x19 
    ldr     x16, p751x2 + 24
    ldr     x17, p751x2 + 32
    ldr     x19, p751x2 + 40
    and     x16, x16, x15 
    and     x17, x17, x15 
    and     x19, x19, x15 
    adcs    x9, x9, x16
    adcs    x10, x10, x17 
    adcs    x11, x11, x19 
    adcs    x12, x12, x0 
    adcs    x13, x13, x1 
    adc     x14, x14, x20 

    stp     x3, x4,   [x2,#0]
    stp     x5, x6,   [x2,#16]
    stp     x7, x8,   [x2,#32]
    stp     x9, x10,  [x2,#48]
    stp     x11, x12, [x2,#64]
    stp     x13, x14, [x2,#80]
	
    ldp     x19, x20, [sp]
    add     sp, sp, #16
    ret


//***********************************************************************
//  Field subtraction
//  Operation: c [x2] = a [x0] - b [x1]
//*********************************************************************** 
.global fpsub751_asm
fpsub751_asm:
    sub     sp, sp, #16
    str     x19, [sp]

    // Subtract a - b
    ldp     x3, x4,   [x0,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x15, x16, [x1,#0]
    ldp     x17, x19, [x1,#16]
    subs    x3, x3, x15
    sbcs    x4, x4, x16
    sbcs    x5, x5, x17
    sbcs    x6, x6, x19
    ldp     x7, x8,   [x0,#32]
    ldp     x9, x10,  [x0,#48]
    ldp     x15, x16, [x1,#32]
    ldp     x17, x19, [x1,#48]
    sbcs    x7, x7, x15
    sbcs    x8, x8, x16
    sbcs    x9, x9, x17
    sbcs    x10, x10, x19
    ldp     x11, x12, [x0,#64]
    ldp     x13, x14, [x0,#80]
    ldp     x15, x16, [x1,#64]
    ldp     x17, x19, [x1,#80]
    sbcs    x11, x11, x15
    sbcs    x12, x12, x16
    sbcs    x13, x13, x17
    sbcs    x14, x14, x19
	sbc     x15, xzr, xzr
    
    // Add 2xp751 anded with the mask in x15
    ldr     x16, p751x2
    ldr     x17, p751x2 + 8
    ldr     x19, p751x2 + 16
    and     x16, x16, x15 
    and     x17, x17, x15 
    and     x19, x19, x15 
    adds    x3, x3, x16
    adcs    x4, x4, x17
    adcs    x5, x5, x17
    adcs    x6, x6, x17
    adcs    x7, x7, x17
    adcs    x8, x8, x19 
    ldr     x16, p751x2 + 24
    ldr     x17, p751x2 + 32
    ldr     x19, p751x2 + 40
    and     x16, x16, x15 
    and     x17, x17, x15 
    and     x19, x19, x15 
    adcs    x9, x9, x16
    adcs    x10, x10, x17 
    adcs    x11, x11, x19 
    ldr     x16, p751x2 + 48
    ldr     x17, p751x2 + 56
    ldr     x19, p751x2 + 64
    and     x16, x16, x15 
    and     x17, x17, x15 
    and     x19, x19, x15
    adcs    x12, x12, x16
    adcs    x13, x13, x17 
    adc     x14, x14, x19

    stp     x3, x4,   [x2,#0]
    stp     x5, x6,   [x2,#16]
    stp     x7, x8,   [x2,#32]
    stp     x9, x10,  [x2,#48]
    stp     x11, x12, [x2,#64]
    stp     x13, x14, [x2,#80]
	
    ldr     x19, [sp]
    add     sp, sp, #16
    ret
	
	
//////////////////////////////////////////// MACRO
.macro MUL192_COMBA_CUT  A0, A1, A2, B0, B1, B2, C0, C1, C2, C3, C4, C5, T0, T1
    mul     \C4, \A1, \B0
    umulh   \C5, \A1, \B0
    adds    \C1, \C1, \C3
    adc     \C2, \C2, xzr
    
    mul     \T1, \A1, \B1
    umulh   \C3, \A1, \B1
    adds    \C1, \C1, \C4
    adcs    \C2, \C2, \C5
    adc     \C3, \C3, xzr
    
    mul     \C4, \A0, \B2
    umulh   \C5, \A0, \B2
    adds    \C2, \C2, \T1
    adcs    \C3, \C3, \C5
    adc     \T0, xzr, xzr
    
    mul     \T1, \A2, \B0
    umulh   \C5, \A2, \B0
    adds    \C2, \C2, \C4
    adcs    \C3, \C3, \C5
    adc     \T0, \T0, xzr
    
    mul     \C4, \A1, \B2
    umulh   \C5, \A1, \B2
    adds    \C2, \C2, \T1
    adcs    \C3, \C3, \C4
    adcs    \T0, \T0, \C5
    adc     \T1, xzr, xzr
    
    mul     \C4, \A2, \B1
    umulh   \C5, \A2, \B1
    adds    \C3, \C3, \C4
    adcs    \T0, \T0, \C5
    adc     \T1, \T1, xzr
    
    mul     \C4, \A2, \B2
    umulh   \C5, \A2, \B2
    adds    \C4, \C4, \T0
    adc     \C5, \C5, \T1
.endm


//////////////////////////////////////////// MACRO
.macro    MUL384_KARATSUBA_COMBA  MA,MB,M,A0,A1,A2,A3,A4,A5,B0,B1,B2,B3,B4,B5,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,T0,T1

    // C6-C8 <- AH + AL, A1 <- mask
    adds    \C6, \A0, \A3
    adcs    \C7, \A1, \A4
    adcs    \C8, \A2, \A5
    adc     \A1, xzr, xzr

    // C9-C11 <- BH + BL, T0 <- mask
    adds    \C9, \B0, \B3
    adcs    \C10, \B1, \B4
    adcs    \C11, \B2, \B5
    adc     \T0, xzr, xzr
    
    // A2, B4, A4 <- masked (BH + BL)
	sub     \T1, xzr, \A1
    and     \A2, \C9, \T1
    and     \B4, \C10, \T1
    and     \A4, \C11, \T1

    // A0, C4-C5 <- masked (AH + AL), A1 <- combined carry
	sub     \T1, xzr, \T0
    and     \A0, \C6, \T1
    umulh   \C2, \C6, \C9  
    and     \C4, \C7, \T1
    and     \C5, \C8, \T1
    and     \A1, \A1, \T0

    // A2, B4, A4, A1 <- (AH+AL) x (BH+BL), part 1
    adds    \A2, \A2, \A0
    adcs    \B4, \B4, \C4
    mul     \A0, \C6, \C9  
    mul     \C4, \C6, \C10
    adcs    \A4, \A4, \C5
    umulh   \C5, \C6, \C10 
    adc     \A1, \A1, xzr

    // A0, C4-C5, C2-C3, A5 <- (AH+AL) x (BH+BL), low part
    MUL192_COMBA_CUT  \C6, \C7, \C8, \C9, \C10, \C11, \A0, \C4, \C5, \C2, \C3, \A5, \T0, \T1
    
    // A0, C4-C5, A2, B4, A4, A1 <- (AH+AL) x (BH+BL), final part
    adds    \A2, \A2, \C2
    adcs    \B4, \B4, \C3
    adcs    \A4, \A4, \A5
    adc     \A1, \A1, xzr

    // C0-C3, A5, B5 <- AL x BL
    stp     \A0, \A1, [\M,#160]
    ldp     \A0, \A1, [\MA,#0]
    str     \A2, [\M,#176]
    ldr     \A2, [\MA,#16]
    mul     \C0, \A0, \B0   
    mul     \C1, \A0, \B1
    umulh   \C2, \A0, \B1     
    umulh   \C3, \A0, \B0   
    MUL192_COMBA_CUT  \A0, \A1, \A2, \B0, \B1, \B2, \C0, \C1, \C2, \C3, \A5, \B5, \T0, \T1
    
    // A0, C4-C5, A2, B4, A4, A1 <- (AH+AL) x (BH+BL) - ALxBL
    ldp     \A0, \A1, [\M,#160]
    ldr     \A2, [\M,#176]
    mul     \C6, \A3, \B3  
    umulh   \C9, \A3, \B3	
    subs    \A0, \A0, \C0 
    sbcs    \C4, \C4, \C1
    sbcs    \C5, \C5, \C2
    sbcs    \A2, \A2, \C3
    sbcs    \B4, \B4, \A5
    sbcs    \A4, \A4, \B5
    sbc     \A1, \A1, xzr

    // C6-C11 <- AH x BH
    stp     \B4, \B5, [\M,#176]
    ldp     \B4, \B5, [\MB,#32]
    stp     \A4, \A5, [\M,#160]
    ldp     \A4, \A5, [\MA,#32]
    mul     \C7, \A3, \B4  
    umulh   \C8, \A3, \B4 
    MUL192_COMBA_CUT  \A3, \A4, \A5, \B3, \B4, \B5, \C6, \C7, \C8, \C9, \C10, \C11, \T0, \T1
    
    // A0, C4-C5, A2, B4, A4, A1 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    ldp     \A4, \A5, [\M,#160]
    ldp     \B4, \B5, [\M,#176]
    subs    \A0, \A0, \C6 
    sbcs    \C4, \C4, \C7
    sbcs    \C5, \C5, \C8
    sbcs    \A2, \A2, \C9
    sbcs    \B4, \B4, \C10
    sbcs    \A4, \A4, \C11
    sbc     \A1, \A1, xzr
    
    adds    \C3, \A0, \C3 
    adcs    \C4, \C4, \A5
    adcs    \C5, \C5, \B5
    adcs    \C6, \C6, \A2
    adcs    \C7, \C7, \B4
    adcs    \C8, \C8, \A4
    adcs    \C9, \C9, \A1
    adcs    \C10, \C10, xzr
    adc     \C11, \C11, xzr
.endm


//////////////////////////////////////////// MACRO
.macro    MUL384_KARATSUBA_COMBA_B  M,A0,A1,A2,A3,A4,A5,B0,B1,B2,B3,B4,B5,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10,C11,T0,T1

    // C6-C8 <- AH + AL, A1 <- mask
    adds    \C6, \A0, \A3
    adcs    \C7, \A1, \A4
    adcs    \C8, \A2, \A5
    adc     \A1, xzr, xzr

    // C9-C11 <- BH + BL, T0 <- mask
    adds    \C9, \B0, \B3
    adcs    \C10, \B1, \B4
    adcs    \C11, \B2, \B5
    adc     \T0, xzr, xzr
    
    // A2, B4, A4 <- masked (BH + BL)
	sub     \T1, xzr, \A1
    and     \A2, \C9, \T1
    and     \B4, \C10, \T1
    and     \A4, \C11, \T1

    // A0, C4-C5 <- masked (AH + AL), A1 <- combined carry
	sub     \T1, xzr, \T0
    and     \A0, \C6, \T1
    umulh   \C2, \C6, \C9  
    and     \C4, \C7, \T1
    and     \C5, \C8, \T1
    and     \A1, \A1, \T0

    // A2, B4, A4, A1 <- (AH+AL) x (BH+BL), part 1
    adds    \A2, \A2, \A0
    adcs    \B4, \B4, \C4
    mul     \A0, \C6, \C9  
    mul     \C4, \C6, \C10
    adcs    \A4, \A4, \C5
    umulh   \C5, \C6, \C10 
    adc     \A1, \A1, xzr

    // A0, C4-C5, C2-C3, A5 <- (AH+AL) x (BH+BL), low part
    MUL192_COMBA_CUT  \C6, \C7, \C8, \C9, \C10, \C11, \A0, \C4, \C5, \C2, \C3, \A5, \T0, \T1
    
    // A0, C4-C5, A2, B4, A4, A1 <- (AH+AL) x (BH+BL), final part
    adds    \A2, \A2, \C2
    adcs    \B4, \B4, \C3
    adcs    \A4, \A4, \A5
    adc     \A1, \A1, xzr

    // C0-C3, A5, B5 <- AL x BL
    stp     \A0, \A1, [\M,#128]
    ldp     \A0, \A1, [\M,#64]
    str     \A2, [\M,#144]
    ldr     \A2, [\M,#80]
    mul     \C0, \A0, \B0
    mul     \C1, \A0, \B1
    umulh   \C2, \A0, \B1 
    umulh   \C3, \A0, \B0   
    MUL192_COMBA_CUT  \A0, \A1, \A2, \B0, \B1, \B2, \C0, \C1, \C2, \C3, \A5, \B5, \T0, \T1
    
    // A0, C4-C5, A2, B4, A4, A1 <- (AH+AL) x (BH+BL) - ALxBL
    ldp     \A0, \A1, [\M,#128]
    ldr     \A2, [\M,#144]
    mul     \C6, \A3, \B3
    umulh   \C9, \A3, \B3	
    subs    \A0, \A0, \C0 
    sbcs    \C4, \C4, \C1
    sbcs    \C5, \C5, \C2
    sbcs    \A2, \A2, \C3
    sbcs    \B4, \B4, \A5
    sbcs    \A4, \A4, \B5
    sbc     \A1, \A1, xzr

    // C6-C11 <- AH x BH
    stp     \B4, \B5, [\M,#144]
    ldp     \B4, \B5, [\M,#112]
    stp     \A4, \A5, [\M,#128]
    ldp     \A4, \A5, [\M,#96]
    mul     \C7, \A3, \B4
    umulh   \C8, \A3, \B4 
    MUL192_COMBA_CUT  \A3, \A4, \A5, \B3, \B4, \B5, \C6, \C7, \C8, \C9, \C10, \C11, \T0, \T1
    
    // A0, C4-C5, A2, B4, A4, A1 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    ldp     \A4, \A5, [\M,#128]
    ldp     \B4, \B5, [\M,#144]
    subs    \A0, \A0, \C6 
    sbcs    \C4, \C4, \C7
    sbcs    \C5, \C5, \C8
    sbcs    \A2, \A2, \C9
    sbcs    \B4, \B4, \C10
    sbcs    \A4, \A4, \C11
    sbc     \A1, \A1, xzr
    
    adds    \C3, \A0, \C3 
    adcs    \C4, \C4, \A5
    adcs    \C5, \C5, \B5
    adcs    \C6, \C6, \A2
    adcs    \C7, \C7, \B4
    adcs    \C8, \C8, \A4
    adcs    \C9, \C9, \A1
    adcs    \C10, \C10, xzr
    adc     \C11, \C11, xzr
.endm


//***********************************************************************************
//  768-bit integer multiplication using Karatsuba (two levels), Comba (lower level)
//  Operation: c [x2] = a [x0] * b [x1]
//*********************************************************************************** 
.global mul751_asm
mul751_asm:
    sub     sp, sp, #96
    stp     x19, x20, [sp,#0]
    stp     x21, x22, [sp,#16]
    stp     x23, x24, [sp,#32]
    stp     x25, x26, [sp,#48]
    stp     x27, x28, [sp,#64]
    stp     x29, x30, [sp,#80]

    // x3-x8 <- AH + AL, x9 <- mask
    ldp     x3, x4, [x0,#0]
    ldp     x5, x6, [x0,#16]
    ldp     x7, x8, [x0,#32]
    ldp     x10, x11, [x0,#48]
    ldp     x12, x13, [x0,#64]
    ldp     x14, x15, [x0,#80]
    adds    x3, x3, x10
    adcs    x4, x4, x11
    adcs    x5, x5, x12
    adcs    x6, x6, x13
    adcs    x7, x7, x14
    adcs    x8, x8, x15
    adc     x9, xzr, xzr

    // x10-x15 <- BH + BL, x16 <- mask
    ldp     x10, x11, [x1,#0]
    ldp     x12, x13, [x1,#16]
    ldp     x14, x15, [x1,#32]
    ldp     x17, x19, [x1,#48]
    ldp     x20, x21, [x1,#64]
    ldp     x22, x23, [x1,#80]	
    adds    x10, x10, x17
    adcs    x11, x11, x19
    adcs    x12, x12, x20
    adcs    x13, x13, x21
    adcs    x14, x14, x22
    adcs    x15, x15, x23
    adc     x16, xzr, xzr
    
    // x19-x24 <- masked (BH + BL)
    sub     x17, xzr, x9
    and     x19, x10, x17
    and     x20, x11, x17
    and     x21, x12, x17
    and     x22, x13, x17
    and     x23, x14, x17
    and     x24, x15, x17

    // x25-x29, x17 <- masked (AH + AL), x9 <- combined carry
    sub     x17, xzr, x16
    and     x25, x3, x17
    and     x26, x4, x17
    and     x27, x5, x17
    and     x28, x6, x17
    and     x29, x7, x17
    and     x17, x8, x17
    and     x9, x9, x16

    // x2[0-56] <- x19-x24, x9 <- masked (AH+AL) + masked (BH+BL), step 1
    adds    x19, x19, x25
    adcs    x20, x20, x26
    adcs    x21, x21, x27
    adcs    x22, x22, x28
    adcs    x23, x23, X29
    adcs    x24, x24, x17
    adc     x9, x9, xzr
	stp     x19, x20, [x2,#0]
	stp     x21, x22, [x2,#16]
	stp     x23, x24, [x2,#32]
    
    // x16-x17, x19-x28 <- (AH+AL) x (BH+BL), low part
    stp     x3, x4, [x2,#64]
    stp     x5, x6, [x2,#80]
    stp     x7, x8, [x2,#96]
    stp     x14, x15, [x2,#112]
    MUL384_KARATSUBA_COMBA_B  x2, x3, x4, x5, x6, x7, x8, x10, x11, x12, x13, x14, x15, x16, x17, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30  
    
    // x2[0-104] <- x23-x28, x9 <- (AH+AL) x (BH+BL), final step
	ldp     x3, x4, [x2,#0]
	ldp     x5, x6, [x2,#16]
	ldp     x7, x8, [x2,#32]
    adds    x23, x23, x3
    adcs    x24, x24, x4
    adcs    x25, x25, x5
    adcs    x26, x26, x6
    adcs    x27, x27, x7
    adcs    x28, x28, x8
    adc     x9, x9, xzr
    ldp     x3, x4, [x0,#0]
    ldp     x5, x6, [x0,#16]
    ldp     x7, x8, [x0,#32]
    ldp     x10, x11, [x1,#0]
    ldp     x12, x13, [x1,#16]
    ldp     x14, x15, [x1,#32]
	stp     x16, x17, [x2,#0]
	stp     x19, x20, [x2,#16]
	stp     x21, x22, [x2,#32]
	stp     x23, x24, [x2,#48]
	stp     x25, x26, [x2,#64]
	stp     x27, x28, [x2,#80]

    // x16-x17, x19-x28 <- AL x BL
    MUL384_KARATSUBA_COMBA  x0, x1, x2, x3, x4, x5, x6, x7, x8, x10, x11, x12, x13, x14, x15, x16, x17, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30
    
    // x3-x8, x10-x15, x9 <- (AH+AL) x (BH+BL) - ALxBL
	ldp     x3, x4, [x2,#0]
	ldp     x5, x6, [x2,#16]
	ldp     x7, x8, [x2,#32]
	ldp     x10, x11, [x2,#48]
	ldp     x12, x13, [x2,#64]
	ldp     x14, x15, [x2,#80]
    subs    x3, x3, x16 
    sbcs    x4, x4, x17
    sbcs    x5, x5, x19
    sbcs    x6, x6, x20
    sbcs    x7, x7, x21
    sbcs    x8, x8, x22
    sbcs    x10, x10, x23
    sbcs    x11, x11, x24
    sbcs    x12, x12, x25
    sbcs    x13, x13, x26
    sbcs    x14, x14, x27
    sbcs    x15, x15, x28
    sbc     x9, x9, xzr
    stp     x16, x17, [x2]        // Output c0-c5
    stp     x19, x20, [x2,#16]
    stp     x21, x22, [x2,#32]

    adds    x3, x3, x23 
    adcs    x4, x4, x24
    adcs    x5, x5, x25
    adcs    x6, x6, x26
    adcs    x7, x7, x27
    adcs    x8, x8, x28
	
	stp     x3, x4, [x2,#48]
	adc     x3, xzr, xzr
	stp     x5, x6, [x2,#64]
	stp     x7, x8, [x2,#80]
	stp     x10, x11, [x2,#96]
	stp     x12, x13, [x2,#112]
	stp     x14, x15, [x2,#128]
    neg     x3, x3
    str     x3, [x2,#144]         // Store carry

    // x16-x17, x19-x28 <- AH x BH
    ldp     x3, x4, [x0,#48]
    ldp     x5, x6, [x0,#64]
    ldp     x7, x8, [x0,#80]
    ldp     x10, x11, [x1,#48]
    ldp     x12, x13, [x1,#64]
    ldp     x14, x15, [x1,#80]
	add     x0, x0, 48
	add     x1, x1, 48
    MUL384_KARATSUBA_COMBA  x0, x1, x2, x3, x4, x5, x6, x7, x8, x10, x11, x12, x13, x14, x15, x16, x17, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30
    
    // x3-x8, x10-x15, x9 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
	ldp     x3, x4, [x2,#48]
	ldp     x5, x6, [x2,#64]
	ldp     x7, x8, [x2,#80]
	ldp     x10, x11, [x2,#96]
	ldp     x12, x13, [x2,#112]
	ldp     x14, x15, [x2,#128]
    subs    x3, x3, x16 
    sbcs    x4, x4, x17
    sbcs    x5, x5, x19
    sbcs    x6, x6, x20
    sbcs    x7, x7, x21
    sbcs    x8, x8, x22
    sbcs    x10, x10, x23
    sbcs    x11, x11, x24
    sbcs    x12, x12, x25
    sbcs    x13, x13, x26
    sbcs    x14, x14, x27
    sbcs    x15, x15, x28
    sbc     x9, x9, xzr
	
    ldr     x1, [x2,#144]         // Restore carry
    stp     x3, x4, [x2,#48]      // Output c6-c11
    stp     x5, x6, [x2,#64]
    adds    x1, x1, #1
    stp     x7, x8, [x2,#80]
    
    adcs    x10, x10, x16          
    adcs    x11, x11, x17
    adcs    x12, x12, x19
    adcs    x13, x13, x20
    adcs    x14, x14, x21
    adcs    x15, x15, x22
    adcs    x23, x23, x9
    adcs    x24, x24, xzr
    adcs    x25, x25, xzr
    adcs    x26, x26, xzr
    adcs    x27, x27, xzr
    adc     x28, x28, xzr
	
    stp     x10, x11, [x2,#96]    // Output c12-c23
    stp     x12, x13, [x2,#112]
    stp     x14, x15, [x2,#128]
    stp     x23, x24, [x2,#144]  
    stp     x25, x26, [x2,#160]   
    stp     x27, x28, [x2,#176]

    ldp     x19, x20, [sp,#0]
    ldp     x21, x22, [sp,#16]
    ldp     x23, x24, [sp,#32]
    ldp     x25, x26, [sp,#48]
    ldp     x27, x28, [sp,#64]
    ldp     x29, x30, [sp,#80]
    add     sp, sp, #96
    ret

  
//////////////////////////////////////////// MACRO
.macro MUL256x384_COMBA_CUT  A0, A1, A2, A3, B0, B1, B2, B3, B4, B5, C0, C1, C2, C3, C4, C5, C6, C7, C8, C9, T0, T1
    mul     \C4, \A1, \B0
    umulh   \C5, \A1, \B0
    adds    \C1, \C1, \C3
    adc     \C2, \C2, xzr
    
    mul     \T0, \A1, \B1
    umulh   \C3, \A1, \B1
    adds    \C1, \C1, \C4
    adcs    \C2, \C2, \C5
    adc     \C3, \C3, xzr
    
    mul     \C4, \A0, \B2
    umulh   \C5, \A0, \B2
    adds    \C2, \C2, \T0
    adcs    \C3, \C3, \C5
    adc     \T1, xzr, xzr
    
    mul     \T0, \A2, \B0
    umulh   \C5, \A2, \B0
    adds    \C2, \C2, \C4
    adcs    \C3, \C3, \C5
    adc     \T1, \T1, xzr
    
    mul     \C4, \A1, \B2
    umulh   \C5, \A1, \B2
    adds    \C2, \C2, \T0
    adcs    \C3, \C3, \C4
    adcs    \T1, \T1, \C5
    adc     \T0, xzr, xzr
    
    mul     \C4, \A2, \B1
    umulh   \C5, \A2, \B1
    adds    \C3, \C3, \C4
    adcs    \T1, \T1, \C5
    adc     \T0, \T0, xzr
    
    mul     \C4, \A3, \B0
    umulh   \C5, \A3, \B0
    adds    \C3, \C3, \C4
    adcs    \T1, \T1, \C5
    adc     \T0, \T0, xzr
    
    mul     \C4, \A0, \B3
    umulh   \C5, \A0, \B3
    adds    \C3, \C3, \C4
    adcs    \T1, \T1, \C5
    adc     \T0, \T0, xzr
    
    mul     \C6, \A2, \B2
    umulh   \C7, \A2, \B2
    adds    \C4, \C6, \T1
    adcs    \C5, \C7, \T0
	adc     \T0, xzr, xzr
    
    mul     \C6, \A0, \B4
    umulh   \C7, \A0, \B4
    adds    \C4, \C6, \C4
    adcs    \C5, \C7, \C5
	adc     \T0, \T0, xzr
    
    mul     \C6, \A1, \B3
    umulh   \C7, \A1, \B3
    adds    \C4, \C6, \C4
    adcs    \C5, \C7, \C5
	adc     \T0, \T0, xzr
    
    mul     \C6, \A3, \B1
    umulh   \C7, \A3, \B1
    adds    \C4, \C6, \C4
    adcs    \C5, \C7, \C5
	adc     \T0, \T0, xzr
    
    mul     \C6, \A0, \B5
    umulh   \C7, \A0, \B5
    adds    \C5, \C6, \C5
    adcs    \T0, \C7, \T0
	adc     \T1, xzr, xzr
    
    mul     \C6, \A1, \B4
    umulh   \C7, \A1, \B4
    adds    \C5, \C6, \C5
    adcs    \T0, \C7, \T0
	adc     \T1, \T1, xzr
    
    mul     \C6, \A2, \B3
    umulh   \C7, \A2, \B3
    adds    \C5, \C6, \C5
    adcs    \T0, \C7, \T0
	adc     \T1, \T1, xzr
    
    mul     \C6, \A3, \B2
    umulh   \C7, \A3, \B2
    adds    \C5, \C6, \C5
    adcs    \T0, \C7, \T0
	adc     \T1, \T1, xzr
    
    mul     \C8, \A1, \B5
    umulh   \C9, \A1, \B5
    adds    \C6, \C8, \T0
    adcs    \T1, \C9, \T1
	adc     \T0, xzr, xzr
    
    mul     \C8, \A2, \B4
    umulh   \C9, \A2, \B4
    adds    \C6, \C8, \C6
    adcs    \T1, \C9, \T1
	adc     \T0, \T0, xzr
    
    mul     \C8, \A3, \B3
    umulh   \C9, \A3, \B3
    adds    \C6, \C8, \C6
    adcs    \T1, \C9, \T1
	adc     \T0, \T0, xzr
    
    mul     \C8, \A2, \B5
    umulh   \C9, \A2, \B5
    adds    \C7, \C8, \T1
    adcs    \T0, \C9, \T0
	adc     \T1, xzr, xzr
    
    mul     \C8, \A3, \B4
    umulh   \C9, \A3, \B4
    adds    \C7, \C8, \C7
    adcs    \T0, \C9, \T0
	adc     \T1, \T1, xzr
    
    mul     \C8, \A3, \B5
    umulh   \C9, \A3, \B5
    adds    \C8, \C8, \T0
    adc     \C9, \C9, \T1
.endm


//**************************************************************************************
//  Montgomery reduction
//  Based on method described in Faz-Hernandez et al. https://eprint.iacr.org/2017/1015  
//  Operation: mc [x1] = ma [x0]
//  NOTE: ma=mc is not allowed
//************************************************************************************** 
.global rdc751_asm
rdc751_asm:
    sub     sp, sp, #96
    stp     x19, x20, [sp]
    stp     x21, x22, [sp,#16]
    stp     x23, x24, [sp,#32]
    stp     x25, x26, [sp,#48]
    stp     x27, x28, [sp,#64]
    stp     x29, x30, [sp,#80]

    ldp     x2, x3, [x0,#0]       // a[0-3]
    ldp     x4, x5, [x0,#16]  

    // x23-x28 <- load the prime constant
    ldr     x23, p751p1_nz_s32 + 0
    ldr     x24, p751p1_nz_s32 + 8
    ldr     x25, p751p1_nz_s32 + 16
    ldr     x26, p751p1_nz_s32 + 24
    ldr     x27, p751p1_nz_s32 + 32
    ldr     x28, p751p1_nz_s32 + 40

    // a[0-3] x p751p1_nz_s32 --> result: x6:x15
    mul     x6, x2, x23
    umulh   x9, x2, x23
    mul     x7, x2, x24
    umulh   x8, x2, x24   
    MUL256x384_COMBA_CUT x2, x3, x4, x5, x23, x24, x25, x26, x27, x28, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x29, x30
    
    ldp     x2, x3, [x0,#32]     // a[4]
    ldp     x4, x5, [x0,#48]    
    ldp     x16, x17, [x0,#64]    

    orr     x30, xzr, x15, lsr #17
    lsl     x15, x15, #47
    orr     x15, x15, x14, lsr #17
    lsl     x14, x14, #47
    orr     x14, x14, x13, lsr #17
    lsl     x13, x13, #47
    orr     x13, x13, x12, lsr #17
    lsl     x12, x12, #47
    orr     x12, x12, x11, lsr #17
    lsl     x11, x11, #47
    orr     x11, x11, x10, lsr #17
    lsl     x10, x10, #47
    orr     x10, x10, x9, lsr #17
    lsl     x9, x9, #47
    orr     x9, x9, x8, lsr #17
    lsl     x8, x8, #47
    orr     x8, x8, x7, lsr #17
    lsl     x7, x7, #47
    orr     x7, x7, x6, lsr #17
    lsl     x6, x6, #47
    
    adds    x3, x3, x6          // a[5]
    adcs    x4, x4, x7          // a[6]
    ldp     x6, x7, [x0,#80]  
    adcs    x5, x5, x8          // a[7]
    adcs    x16, x16, x9        // a[8] 
    ldp     x8, x9, [x0,#96] 
    adcs    x17, x17, x10 
    adcs    x19, x6, x11 
    adcs    x20, x7, x12  
    ldp     x6, x7, [x0,#112]  
    adcs    x21, x8, x13 
    umulh   x8, x2, x24   
    adcs    x22, x9, x14
    umulh   x9, x2, x23
    adcs    x13, x6, x15 
    mul     x6, x2, x23    
    adcs    x14, x7, x30  
    mul     x7, x2, x24 
    adcs    x15, xzr, xzr       
	stp     x13, x14, [x0,#0]   // Store some values including carry
	str     x15, [x0,#16]

    // a[4-7] x p751p1_nz_s32 --> result: x6:x15
    MUL256x384_COMBA_CUT x2, x3, x4, x5, x23, x24, x25, x26, x27, x28, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x29, x30
    
    orr     x30, xzr, x15, lsr #17
    lsl     x15, x15, #47
    orr     x15, x15, x14, lsr #17
    lsl     x14, x14, #47
    orr     x14, x14, x13, lsr #17
    lsl     x13, x13, #47
    orr     x13, x13, x12, lsr #17
    lsl     x12, x12, #47
    orr     x12, x12, x11, lsr #17
    lsl     x11, x11, #47
    orr     x11, x11, x10, lsr #17
    lsl     x10, x10, #47
    orr     x10, x10, x9, lsr #17
    lsl     x9, x9, #47
    orr     x9, x9, x8, lsr #17
    lsl     x8, x8, #47
    orr     x8, x8, x7, lsr #17
    lsl     x7, x7, #47
    orr     x7, x7, x6, lsr #17
    lsl     x6, x6, #47  
    
    ldp     x2, x3, [x0,#0] 
    ldp     x4, x5, [x0,#128]  
    adds    x17, x17, x6        // a[9]
    adcs    x19, x19, x7        // a[10]
    adcs    x20, x20, x8        // a[11]
    adcs    x21, x21, x9        // a[12]
    ldp     x6, x7, [x0,#144] 
    umulh   x8, x16, x24     
    adcs    x22, x22, x10  
    adcs    x2, x2, x11
    adcs    x3, x3, x12  
    umulh   x9, x16, x23 
    adcs    x13, x13, x4
    adcs    x14, X14, x5 
    adcs    x4, x15, x6   
    mul     x6, x16, x23
    adcs    x5, x30, x7  
    mul     x7, x16, x24 
    adcs    x10, xzr, xzr  
	stp     x13, x14, [x0,#0]   // Store some values including carry 
	str     x10, [x0,#32] 

    // a[8-11] x p751p1_nz_s32 --> result: x6:x15
    MUL256x384_COMBA_CUT x16, x17, x19, x20, x23, x24, x25, x26, x27, x28, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15, x29, x30
    
    orr     x30, xzr, x15, lsr #17
    lsl     x15, x15, #47
    orr     x15, x15, x14, lsr #17
    lsl     x14, x14, #47
    orr     x14, x14, x13, lsr #17
    lsl     x13, x13, #47
    orr     x13, x13, x12, lsr #17
    lsl     x12, x12, #47
    orr     x12, x12, x11, lsr #17
    lsl     x11, x11, #47
    orr     x11, x11, x10, lsr #17
    lsl     x10, x10, #47
    orr     x10, x10, x9, lsr #17
    lsl     x9, x9, #47
    orr     x9, x9, x8, lsr #17
    lsl     x8, x8, #47
    orr     x8, x8, x7, lsr #17
    lsl     x7, x7, #47
    orr     x7, x7, x6, lsr #17
    lsl     x6, x6, #47    
    
    ldp     x16, x17, [x0,#0] 
    adds    x22, x22, x6  
    adcs    x2, x2, x7
    adcs    x3, x3, x8  
    adcs    x16, x16, x9 
    ldp     x6, x7, [x0,#160] 
    ldp     x8, x9, [x0,#176] 
    adcs    x17, x17, x10 
    adcs    x4, x4, x11
    adcs    x5, x5, x12
    adcs    x6, x6, x13  
    adcs    x7, x7, x14    
    adcs    x8, x8, x15     
    adc     x9, x9, x30
	
	ldr     x29, [x0,#16]
	ldr     x30, [x0,#32]
    stp     x21, x22, [x1,#0]   // Output c0-c1
    adds    x16, x16, x29  
    adcs    x17, x17, xzr
    adcs    x4, x4, xzr 
    adcs    x5, x5, xzr  
    adcs    x6, x6, x30     
    adcs    x7, x7, xzr     
    adcs    x8, x8, xzr    
    adc     x9, x9, xzr

    stp     x2, x3, [x1,#16]
    stp     x16, x17, [x1,#32]
    stp     x4, x5, [x1,#48]
    stp     x6, x7, [x1,#64]
    stp     x8, x9, [x1,#80]
    
    ldp     x19, x20, [sp]
    ldp     x21, x22, [sp, #16]
    ldp     x23, x24, [sp, #32]
    ldp     x25, x26, [sp, #48]
    ldp     x27, x28, [sp, #64]
    ldp     x29, x30, [sp, #80]
    add     sp, sp, #96
	ret


//***********************************************************************
//  751-bit multiprecision addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global mp_add751_asm
mp_add751_asm:
    ldp x3, x4,   [x0,#0]
    ldp x5, x6,   [x0,#16]
    ldp x7, x8,   [x0,#32]
    ldp x9, x10,  [x0,#48]
    ldp x11, x12, [x0,#64]
    ldp x13, x14, [x0,#80]

    ldp x15, x16, [x1,#0]
    ldp x17, x18, [x1,#16]
    adds x3, x3, x15
    adcs x4, x4, x16
    adcs x5, x5, x17
    adcs x6, x6, x18
    ldp x15, x16, [x1,#32]
    ldp x17, x18, [x1,#48]
    adcs x7, x7, x15
    adcs x8, x8, x16
    adcs x9, x9, x17
    adcs x10, x10, x18
    ldp x15, x16, [x1,#64]
    ldp x17, x18, [x1,#80]
    adcs x11, x11, x15
    adcs x12, x12, x16
    adcs x13, x13, x17
    adc  x14, x14, x18

    stp x3, x4,   [x2,#0]
    stp x5, x6,   [x2,#16]
    stp x7, x8,   [x2,#32]
    stp x9, x10,  [x2,#48]
    stp x11, x12, [x2,#64]
    stp x13, x14, [x2,#80]
    ret    


//***********************************************************************
//  2x751-bit multiprecision addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global mp_add751x2_asm
mp_add751x2_asm:
    ldp x3, x4,   [x0,#0]
    ldp x5, x6,   [x0,#16]
    ldp x7, x8,   [x0,#32]
    ldp x9, x10,  [x0,#48]
    ldp x11, x12, [x0,#64]
    ldp x13, x14, [x0,#80]

    ldp x15, x16, [x1,#0]
    ldp x17, x18, [x1,#16]
    adds x3, x3, x15
    adcs x4, x4, x16
    adcs x5, x5, x17
    adcs x6, x6, x18
    ldp x15, x16, [x1,#32]
    ldp x17, x18, [x1,#48]
    adcs x7, x7, x15
    adcs x8, x8, x16
    adcs x9, x9, x17
    adcs x10, x10, x18
    ldp x15, x16, [x1,#64]
    ldp x17, x18, [x1,#80]
    adcs x11, x11, x15
    adcs x12, x12, x16
    adcs x13, x13, x17
    adcs x14, x14, x18

    stp x3, x4,   [x2,#0]
    stp x5, x6,   [x2,#16]
    stp x7, x8,   [x2,#32]
    stp x9, x10,  [x2,#48]
    stp x11, x12, [x2,#64]
    stp x13, x14, [x2,#80]
	
    ldp x3, x4,   [x0,#96]
    ldp x5, x6,   [x0,#112]
    ldp x7, x8,   [x0,#128]
    ldp x9, x10,  [x0,#144]
    ldp x11, x12, [x0,#160]
    ldp x13, x14, [x0,#176]

    ldp x15, x16, [x1,#96]
    ldp x17, x18, [x1,#112]
    adcs x3, x3, x15
    adcs x4, x4, x16
    adcs x5, x5, x17
    adcs x6, x6, x18
    ldp x15, x16, [x1,#128]
    ldp x17, x18, [x1,#144]
    adcs x7, x7, x15
    adcs x8, x8, x16
    adcs x9, x9, x17
    adcs x10, x10, x18
    ldp x15, x16, [x1,#160]
    ldp x17, x18, [x1,#176]
    adcs x11, x11, x15
    adcs x12, x12, x16
    adcs x13, x13, x17
    adc  x14, x14, x18

    stp x3, x4,   [x2,#96]
    stp x5, x6,   [x2,#112]
    stp x7, x8,   [x2,#128]
    stp x9, x10,  [x2,#144]
    stp x11, x12, [x2,#160]
    stp x13, x14, [x2,#176]
    ret


//***********************************************************************
//  2x751-bit multiprecision subtraction/addition
//  Operation: c [x2] = a [x0] - b [x1]. If c < 0, add p751*2^768
//*********************************************************************** 
.global mp_subadd751x2_asm
mp_subadd751x2_asm:
    sub sp, sp, #16
    stp x19, x20, [sp]

    ldp     x3, x4,   [x0,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x11, x12, [x1,#0]
    ldp     x13, x14, [x1,#16]
    subs    x3, x3, x11
    sbcs    x4, x4, x12
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    ldp     x7, x8,   [x0,#32]
    ldp     x9, x10,  [x0,#48]
    ldp     x11, x12, [x1,#32]
    ldp     x13, x14, [x1,#48]
    sbcs    x7, x7, x11
    sbcs    x8, x8, x12
    sbcs    x9, x9, x13
    sbcs    x10, x10, x14
    stp     x3, x4,   [x2,#0]
    stp     x5, x6,   [x2,#16]
    stp     x7, x8,   [x2,#32]
    stp     x9, x10,  [x2,#48]

    ldp     x3, x4,   [x0,#64]
    ldp     x5, x6,   [x0,#80]
    ldp     x11, x12, [x1,#64]
    ldp     x13, x14, [x1,#80]
    sbcs    x3, x3, x11
    sbcs    x4, x4, x12
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    stp     x3, x4,   [x2,#64]
    stp     x5, x6,   [x2,#80]
    
    ldp     x3, x4,   [x0,#96]
    ldp     x5, x6,   [x0,#112]
    ldp     x15, x16, [x1,#96]
    ldp     x17, x19, [x1,#112]
    sbcs    x3, x3, x15
    sbcs    x4, x4, x16
    sbcs    x5, x5, x17
    sbcs    x6, x6, x19
    ldp     x7, x8,   [x0,#128]
    ldp     x9, x10,  [x0,#144]
    ldp     x15, x16, [x1,#128]
    ldp     x17, x19, [x1,#144]
    sbcs    x7, x7, x15
    sbcs    x8, x8, x16
    sbcs    x9, x9, x17
    sbcs    x10, x10, x19
    ldp     x11, x12,   [x0,#160]
    ldp     x13, x14,  [x0,#176]
    ldp     x15, x16, [x1,#160]
    ldp     x17, x19, [x1,#176]
    sbcs    x11, x11, x15
    sbcs    x12, x12, x16
    sbcs    x13, x13, x17
    sbcs    x14, x14, x19
    sbc     x0, xzr, xzr

    // Add p751 anded with the mask in x0 
    ldr     x1, p751 
    ldr     x15, p751 + 8
    ldr     x16, p751 + 16
    ldr     x17, p751 + 24
    ldr     x19, p751 + 32
    ldr     x20, p751 + 40
    and     x1, x1, x0 
    and     x15, x15, x0
    and     x16, x16, x0
    and     x17, x17, x0
    and     x19, x19, x0 
    and     x20, x20, x0
	adds    x3, x3, x1   
	adcs    x4, x4, x1   
	adcs    x5, x5, x1   
	adcs    x6, x6, x1   
	adcs    x7, x7, x1  
	adcs    x8, x8, x15  
	adcs    x9, x9, x16  
	adcs    x10, x10, x17  
	adcs    x11, x11, x19 
	adcs    x12, x12, x20	
    stp     x3, x4,   [x2,#96]
    stp     x5, x6,   [x2,#112]
    stp     x7, x8,   [x2,#128]
    stp     x9, x10,  [x2,#144]
    stp     x11, x12, [x2,#160]
    ldr     x1, p751 + 48
    ldr     x3, p751 + 56
    and     x1, x1, x0 
    and     x3, x3, x0 
	adcs    x13, x13, x1  
	adc     x14, x14, x3 
    stp     x13, x14,  [x2,#176]
	
    ldp     x19, x20, [sp]
    add     sp, sp, #16
    ret


//***********************************************************************
//  Double 2x751-bit multiprecision subtraction
//  Operation: c [x2] = c [x2] - a [x0] - b [x1]
//*********************************************************************** 
.global mp_dblsub751x2_asm
mp_dblsub751x2_asm:
    sub sp, sp, #96
    stp x19, x20, [sp]
    stp x21, x22, [sp, #16]
    stp x23, x24, [sp, #32]
    stp x25, x26, [sp, #48]
    stp x27, x28, [sp, #64]
    stp x29, x30, [sp, #80]
    ldp x3, x4,   [x2,#0]
    ldp x5, x6,   [x2,#16]
    ldp x7, x8,   [x2,#32]
    ldp x9, x10,  [x2,#48]
    ldp x11, x12, [x2,#64]
    ldp x13, x14, [x2,#80]
    ldp x15, x16, [x2,#96]
    ldp x17, x18, [x2,#112]
    ldp x19, x20, [x2,#128]
    ldp x21, x22, [x2,#144]
    ldp x23, x24, [x2,#160]
    ldp x25, x26, [x2,#176]

    ldp x27, x28, [x0,#0]
    ldp x29, x30, [x0,#16]
    subs x3, x3, x27
    sbcs x4, x4, x28
    sbcs x5, x5, x29
    sbcs x6, x6, x30
    ldp x27, x28, [x0,#32]
    ldp x29, x30, [x0,#48]
    sbcs x7, x7, x27
    sbcs x8, x8, x28
    sbcs x9, x9, x29
    sbcs x10, x10, x30
    ldp x27, x28, [x0,#64]
    ldp x29, x30, [x0,#80]
    sbcs x11, x11, x27
    sbcs x12, x12, x28
    sbcs x13, x13, x29
    sbcs x14, x14, x30
    ldp x27, x28, [x0,#96]
    ldp x29, x30, [x0,#112]
    sbcs x15, x15, x27
    sbcs x16, x16, x28
    sbcs x17, x17, x29
    sbcs x18, x18, x30
    ldp x27, x28, [x0,#128]
    ldp x29, x30, [x0,#144]
    sbcs x19, x19, x27
    sbcs x20, x20, x28
    sbcs x21, x21, x29
    sbcs x22, x22, x30
    ldp x27, x28, [x0,#160]
    ldp x29, x30, [x0,#176]
    sbcs x23, x23, x27
    sbcs x24, x24, x28
    sbcs x25, x25, x29
    sbc  x26, x26, x30

    ldp x27, x28, [x1,#0]
    ldp x29, x30, [x1,#16]
    subs x3, x3, x27
    sbcs x4, x4, x28
    sbcs x5, x5, x29
    sbcs x6, x6, x30
    ldp x27, x28, [x1,#32]
    ldp x29, x30, [x1,#48]
    sbcs x7, x7, x27
    sbcs x8, x8, x28
    sbcs x9, x9, x29
    sbcs x10, x10, x30
    ldp x27, x28, [x1,#64]
    ldp x29, x30, [x1,#80]
    sbcs x11, x11, x27
    sbcs x12, x12, x28
    sbcs x13, x13, x29
    sbcs x14, x14, x30
    ldp x27, x28, [x1,#96]
    ldp x29, x30, [x1,#112]
    sbcs x15, x15, x27
    sbcs x16, x16, x28
    sbcs x17, x17, x29
    sbcs x18, x18, x30
    ldp x27, x28, [x1,#128]
    ldp x29, x30, [x1,#144]
    sbcs x19, x19, x27
    sbcs x20, x20, x28
    sbcs x21, x21, x29
    sbcs x22, x22, x30
    ldp x27, x28, [x1,#160]
    ldp x29, x30, [x1,#176]
    sbcs x23, x23, x27
    sbcs x24, x24, x28
    sbcs x25, x25, x29
    sbc  x26, x26, x30

    stp x3, x4,   [x2,#0]
    stp x5, x6,   [x2,#16]
    stp x7, x8,   [x2,#32]
    stp x9, x10,  [x2,#48]
    stp x11, x12, [x2,#64]
    stp x13, x14, [x2,#80]
    stp x15, x16, [x2,#96]
    stp x17, x18, [x2,#112]
    stp x19, x20, [x2,#128]
    stp x21, x22, [x2,#144]
    stp x23, x24, [x2,#160]
    stp x25, x26, [x2,#176]	
    
    ldp x19, x20, [sp]
    ldp x21, x22, [sp, #16]
    ldp x23, x24, [sp, #32]
    ldp x25, x26, [sp, #48]
    ldp x27, x28, [sp, #64]
    ldp x29, x30, [sp, #80]
    add sp, sp, #96
    ret
