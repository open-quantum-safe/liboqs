//*******************************************************************************************
// SIDH: an efficient supersingular isogeny cryptography library
//
// Abstract: field arithmetic in 64-bit ARMv8 assembly for P610 on Linux
//*******************************************************************************************

.text

// p610
p610:
.quad  0xFFFFFFFFFFFFFFFF
.quad  0x6E01FFFFFFFFFFFF
.quad  0xB1784DE8AA5AB02E
.quad  0x9AE7BF45048FF9AB
.quad  0xB255B2FA10C4252A
.quad  0x819010C251E7D88C
.quad  0x000000027BF6A768

// 2 * p610
p610x2:
.quad  0xFFFFFFFFFFFFFFFE
.quad  0xFFFFFFFFFFFFFFFF
.quad  0xDC03FFFFFFFFFFFF
.quad  0x62F09BD154B5605C
.quad  0x35CF7E8A091FF357
.quad  0x64AB65F421884A55
.quad  0x03202184A3CFB119
.quad  0x00000004F7ED4ED1

// 4 * p610
p610x4:
.quad  0xFFFFFFFFFFFFFFFC
.quad  0xFFFFFFFFFFFFFFFF
.quad  0xB807FFFFFFFFFFFF
.quad  0xC5E137A2A96AC0B9
.quad  0x6B9EFD14123FE6AE
.quad  0xC956CBE8431094AA
.quad  0x06404309479F6232
.quad  0x00000009EFDA9DA2

// (p610 + 1)/2^48
p610p1_nz_s6:
.quad  0x4DE8AA5AB02E6E02
.quad  0xBF45048FF9ABB178
.quad  0xB2FA10C4252A9AE7
.quad  0x10C251E7D88CB255
.quad  0x27BF6A7688190


//***********************************************************************
//  Field addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_fpadd610_asm
oqs_kem_sike_fpadd610_asm:
    // Add a + b
    ldp     x3, x4,   [x0,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#0]
    ldp     x15, x16, [x1,#16]
    adds    x3, x3, x13
    adcs    x4, x4, x14
    adcs    x5, x5, x15
    adcs    x6, x6, x16
    ldp     x7, x8,   [x0,#32]
    ldp     x9, x10,  [x0,#48]
    ldp     x13, x14, [x1,#32]
    ldp     x15, x16, [x1,#48]
    adcs    x7, x7, x13
    adcs    x8, x8, x14
    adcs    x9, x9, x15
    adcs    x10, x10, x16
    ldp     x11, x12, [x0,#64]
    ldp     x13, x14, [x1,#64]
    adcs    x11, x11, x13
    adc     x12, x12, x14
    
    //  Subtract 2xp610
    ldr     x0, p610x2
    ldr     x1, p610x2 + 8
    ldr     x13, p610x2 + 16
    subs    x3, x3, x0
    sbcs    x4, x4, x1
    sbcs    x5, x5, x1
    sbcs    x6, x6, x1
    sbcs    x7, x7, x13
    ldr     x14, p610x2 + 24
    ldr     x15, p610x2 + 32
    ldr     x16, p610x2 + 40
    sbcs    x8, x8, x14
    sbcs    x9, x9, x15
    sbcs    x10, x10, x16
    ldr     x14, p610x2 + 48
    ldr     x15, p610x2 + 56
    sbcs    x11, x11, x14
    sbcs    x12, x12, x15
    sbc     x17, xzr, xzr

    // Add 2xp610 anded with the mask in x17
    and     x0, x0, x17 
    and     x1, x1, x17 
    and     x13, x13, x17 
    and     x14, x14, x17 
    and     x15, x15, x17 
    and     x16, x16, x17  
	
    adds    x3, x3, x0
    adcs    x4, x4, x1
    adcs    x5, x5, x1
    adcs    x6, x6, x1
    adcs    x7, x7, x13
    ldr     x0, p610x2 + 24
    ldr     x1, p610x2 + 32
    and     x0, x0, x17 
    and     x1, x1, x17 
    adcs    x8, x8, x0 
    adcs    x9, x9, x1
    adcs    x10, x10, x16 
    adcs    x11, x11, x14 
    adc     x12, x12, x15

    stp     x3, x4,   [x2,#0]
    stp     x5, x6,   [x2,#16]
    stp     x7, x8,   [x2,#32]
    stp     x9, x10,  [x2,#48]
    stp     x11, x12, [x2,#64]
    ret


//***********************************************************************
//  Field subtraction
//  Operation: c [x2] = a [x0] - b [x1]
//*********************************************************************** 
.global oqs_kem_sike_fpsub610_asm
oqs_kem_sike_fpsub610_asm:
    // Subtract a - b
    ldp     x3, x4,   [x0,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#0]
    ldp     x15, x16, [x1,#16]
    subs    x3, x3, x13
    sbcs    x4, x4, x14
    sbcs    x5, x5, x15
    sbcs    x6, x6, x16
    ldp     x7, x8,   [x0,#32]
    ldp     x9, x10,  [x0,#48]
    ldp     x13, x14, [x1,#32]
    ldp     x15, x16, [x1,#48]
    sbcs    x7, x7, x13
    sbcs    x8, x8, x14
    sbcs    x9, x9, x15
    sbcs    x10, x10, x16
    ldp     x11, x12, [x0,#64]
    ldp     x13, x14, [x1,#64]
    sbcs    x11, x11, x13
    sbcs    x12, x12, x14
	sbc     x17, xzr, xzr
    
    // Add 2xp610 anded with the mask in x17
    ldr     x13, p610x2
    ldr     x14, p610x2 + 8
    ldr     x15, p610x2 + 16
    and     x13, x13, x17 
    and     x14, x14, x17 
    and     x15, x15, x17 
    adds    x3, x3, x13
    adcs    x4, x4, x14
    adcs    x5, x5, x14
    adcs    x6, x6, x14
    adcs    x7, x7, x15
    ldr     x13, p610x2 + 24
    ldr     x14, p610x2 + 32
    ldr     x15, p610x2 + 40
    and     x13, x13, x17 
    and     x14, x14, x17 
    and     x15, x15, x17 
    adcs    x8, x8, x13 
    adcs    x9, x9, x14
    adcs    x10, x10, x15 
    ldr     x13, p610x2 + 48
    ldr     x14, p610x2 + 56
    and     x13, x13, x17 
    and     x14, x14, x17 
    adcs    x11, x11, x13 
    adc     x12, x12, x14

    stp     x3, x4,   [x2,#0]
    stp     x5, x6,   [x2,#16]
    stp     x7, x8,   [x2,#32]
    stp     x9, x10,  [x2,#48]
    stp     x11, x12, [x2,#64]
    ret


///////////////////////////////////////////////////////////////// MACRO
.macro SUB610_PX  P0

    // Subtract a - b
    ldp     x3, x4,   [x0,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#0]
    ldp     x15, x16, [x1,#16]
    subs    x3, x3, x13
    sbcs    x4, x4, x14
    sbcs    x5, x5, x15
    sbcs    x6, x6, x16
    ldp     x7, x8,   [x0,#32]
    ldp     x9, x10,  [x0,#48]
    ldp     x13, x14, [x1,#32]
    ldp     x15, x16, [x1,#48]
    sbcs    x7, x7, x13
    sbcs    x8, x8, x14
    sbcs    x9, x9, x15
    sbcs    x10, x10, x16
    ldp     x11, x12, [x0,#64]
    ldp     x13, x14, [x1,#64]
    sbcs    x11, x11, x13
    sbc     x12, x12, x14
    
    ldr     x13, \P0
    ldr     x14, \P0 + 8
    ldr     x15, \P0 + 16 
    adds    x3, x3, x13
    adcs    x4, x4, x14
    adcs    x5, x5, x14
    adcs    x6, x6, x14
    adcs    x7, x7, x15
    ldr     x13, \P0 + 24
    ldr     x14, \P0 + 32
    ldr     x15, \P0 + 40
    adcs    x8, x8, x13 
    adcs    x9, x9, x14
    adcs    x10, x10, x15 
    ldr     x13, \P0 + 48
    ldr     x14, \P0 + 56
    adcs    x11, x11, x13 
    adc     x12, x12, x14

    stp     x3, x4,   [x2,#0]
    stp     x5, x6,   [x2,#16]
    stp     x7, x8,   [x2,#32]
    stp     x9, x10,  [x2,#48]
    stp     x11, x12, [x2,#64]
  .endm


//***********************************************************************
//  Multiprecision subtraction with correction with 2*p610
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 2*p610
//*********************************************************************** 
.global oqs_kem_sike_mp_sub610_p2_asm
oqs_kem_sike_mp_sub610_p2_asm:

  SUB610_PX  p610x2
  ret


//***********************************************************************
//  Multiprecision subtraction with correction with 4*p610
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 4*p610
//*********************************************************************** 
.global oqs_kem_sike_mp_sub610_p4_asm
oqs_kem_sike_mp_sub610_p4_asm:

  SUB610_PX  p610x4
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
.macro    MUL320_KARATSUBA_COMBA  MA,A0,A1,A2,A3,A4,B0,B1,B2,B3,B4,C0,C1,C2,C3,C4,C5,C6,C7,C8,C9,T0,T1

    // A0-A2 <- AH + AL, T0 <- mask
    adds    \C0, \A0, \A3
    adcs    \C1, \A1, \A4
    adcs    \C2, \A2, xzr
    adc     \T0, xzr, xzr

    // B0-B2 <- BH + BL, T1 <- mask
    adds    \C3, \B0, \B3
    adcs    \C4, \B1, \B4
    adcs    \C5, \B2, xzr
    adc     \T1, xzr, xzr
    
    // C6-C8 <- masked (BH + BL)
    sub     \T0, xzr, \T0
    sub     \T1, xzr, \T1
    and     \C6, \C3, \T0
    and     \C7, \C4, \T0
    and     \C8, \C5, \T0

    // C9,T0,T1 <- masked (AH + AL)
    mul     \A0, \C0, \C3  
    mul     \A1, \C0, \C4
    and     \C9, \C0, \T1
    and     \T0, \C1, \T1
    and     \T1, \C2, \T1

    // C6-C8 <- (AH+AL) x (BH+BL), part 1
    umulh   \A3, \C0, \C3 
    umulh   \A2, \C0, \C4
    adds    \C6, \C6, \C9   
    adcs    \C7, \C7, \T0  
    adc     \C8, \C8, \T1

    // A0-A4,C9 <- (AH+AL) x (BH+BL), low part
    MUL192_COMBA_CUT  \C0, \C1, \C2, \C3, \C4, \C5, \A0, \A1, \A2, \A3, \A4, \C9, \T0, \T1
    
    // C6-C8 <- (AH+AL) x (BH+BL), final part
    adds    \C6, \A3, \C6
    adcs    \C7, \A4, \C7
    ldp     \A3, \A4, [\MA,#0]
    adc     \C8, \C9, \C8
    ldr     \C9, [\MA,#16]

    // C0-C5 <- AL x BL
    mul     \C0, \A3, \B0
    mul     \C1, \A3, \B1  
    umulh   \C3, \A3, \B0 
    umulh   \C2, \A3, \B1 
    MUL192_COMBA_CUT  \A3, \A4, \C9, \B0, \B1, \B2, \C0, \C1, \C2, \C3, \C4, \C5, \T0, \T1
    
    // A0-A2,C6-C8 <- (AH+AL) x (BH+BL) - ALxBL
    ldp     \A3, \A4, [\MA,#24]
    subs    \A0, \A0, \C0 
    sbcs    \A1, \A1, \C1
    mul     \B0, \A3, \B3
    umulh   \C9, \A3, \B3
    sbcs    \A2, \A2, \C2
    sbcs    \C6, \C6, \C3
    mul     \B1, \A3, \B4
    umulh   \B2, \A3, \B4  
    sbcs    \C7, \C7, \C4
    sbc     \C8, \C8, \C5	 

    // B0-B2,C9 <- AH x BH 
    MUL128_COMBA_CUT  \A3, \A4, \B3, \B4, \B0, \B1, \B2, \C9, \T0
    
    // A0-A2,C6-C8 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    subs    \A0, \A0, \B0 
    sbcs    \A1, \A1, \B1
    sbcs    \A2, \A2, \B2
    sbcs    \C6, \C6, \C9
    sbcs    \C7, \C7, xzr
    sbc     \C8, \C8, xzr
    
    adds    \C3, \C3, \A0
    adcs    \C4, \C4, \A1
    adcs    \C5, \C5, \A2
    adcs    \C6, \C6, \B0
    adcs    \C7, \C7, \B1
    adcs    \C8, \C8, \B2
    adc     \C9, \C9, xzr
.endm


//***********************************************************************************
//  640-bit integer multiplication using Karatsuba (two levels), Comba (lower level)
//  Operation: c [x2] = a [x0] * b [x1]
//*********************************************************************************** 
.global oqs_kem_sike_mul610_asm
oqs_kem_sike_mul610_asm:
    sub     sp, sp, #96
    ldp     x3, x4, [x0]
    ldp     x5, x6, [x0,#16]
    ldp     x7, x8, [x0,#32]
    ldp     x10, x11, [x0,#48]
    ldp     x12, x13, [x0,#64]
    stp     x25, x26, [sp,#48]
    stp     x27, x28, [sp,#64]
    stp     x29, x30, [sp,#80]

    // x26-x30 <- AH + AL, x9 <- mask
    adds    x26, x3, x8
    stp     x21, x22, [sp,#16]
    adcs    x27, x4, x10
    stp     x23, x24, [sp,#32]
    adcs    x28, x5, x11
    ldp     x21, x22, [x1,#0]
    adcs    x29, x6, x12
    ldp     x14, x15, [x1,#48]
    adcs    x30, x7, x13
    ldp     x23, x24, [x1,#16]
    adc     x9, xzr, xzr
    ldp     x25, x13, [x1,#32]

    // x13-x17 <- BH + BL, x8 <- mask
    adds    x13, x13, x21
    ldp     x16, x17, [x1,#64]
    adcs    x14, x14, x22
    adcs    x15, x15, x23
    adcs    x16, x16, x24
    adcs    x17, x17, x25
    adc     x8, xzr, xzr
    
    // x3-x7 <- masked (BH + BL)
    sub     x8, xzr, x8
    sub     x9, xzr, x9
    stp     x19, x20, [sp,#0]
    and     x3, x13, x9
    and     x4, x14, x9
    and     x5, x15, x9
    and     x6, x16, x9
    and     x7, x17, x9

    // x8-x12 <- masked (AH + AL)
    and     x9, x27, x8
    and     x10, x28, x8
    and     x11, x29, x8
    and     x12, x30, x8
    and     x8, x26, x8

    // x8-x12 <- masked (AH+AL) + masked (BH+BL), step 1
    adds    x8, x8, x3
    adcs    x9, x9, x4
    stp     x26, x27, [x2,#0]
    adcs    x10, x10, x5
    stp     x28, x29, [x2,#16]
    adcs    x11, x11, x6
    str     x30, [x2,#32]
    adc     x12, x12, x7
    
    // x3-x7,x19-x23 <- (AH+AL) x (BH+BL), low part
    MUL320_KARATSUBA_COMBA  x2, x26, x27, x28, x29, x30, x13, x14, x15, x16, x17, x3, x4, x5, x6, x7, x19, x20, x21, x22, x23, x24, x25  
    
    // x8-x12 <- (AH+AL) x (BH+BL), final step
	stp     x3, x4, [x2,#0]
    adds    x8, x8, x19
	stp     x5, x6, [x2,#16]
    adcs    x9, x9, x20
	str     x7, [x2,#32]
    adcs    x10, x10, x21
    ldp     x13, x14, [x0]
    adcs    x11, x11, x22
    ldp     x15, x16, [x0,#16]
    adc     x12, x12, x23
	
    ldr     x17, [x0,#32]
    ldp     x26, x27, [x1,#0]
    ldp     x28, x29, [x1,#16]
    ldr     x30, [x1,#32]

    // x3-x7,x19-x23 <- AL x BL
    MUL320_KARATSUBA_COMBA  x0, x13, x14, x15, x16, x17, x26, x27, x28, x29, x30, x3, x4, x5, x6, x7, x19, x20, x21, x22, x23, x24, x25
    
    // x3-x12 <- (AH+AL) x (BH+BL) - ALxBL
	ldp     x13, x14, [x2,#0]
    subs    x13, x13, x3 
	ldp     x15, x16, [x2,#16]
    sbcs    x14, x14, x4
	ldr     x17, [x2,#32]
    sbcs    x15, x15, x5
    stp     x3, x4, [x2]
    sbcs    x16, x16, x6
    stp     x5, x6, [x2,#16]
    sbcs    x17, x17, x7
	str     x7, [x2,#32]
    sbcs    x8, x8, x19
    ldp     x3, x4, [x0,#40]
    sbcs    x9, x9, x20
    ldp     x5, x6, [x0,#56]
    ldr     x7, [x0,#72]
    sbcs    x10, x10, x21
    ldp     x26, x27, [x1,#40]
    sbcs    x11, x11, x22
    ldp     x28, x29, [x1,#56]
    ldr     x30, [x1,#72]
    sbc     x12, x12, x23	

	stp     x8, x9, [x2,#40]
	stp     x10, x11, [x2,#56]
	stp     x12, x13, [x2,#72]
	stp     x14, x15, [x2,#88]
	stp     x16, x17, [x2,#104]

    // x8-x17 <- AH x BH
    add     x0, x0, #40
    MUL320_KARATSUBA_COMBA  x0, x3, x4, x5, x6, x7, x26, x27, x28, x29, x30, x8, x9, x10, x11, x12, x13, x14, x15, x16, x17, x24, x25 
    
    // x8-x10, x19, x15-x18 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
	ldp     x7, x26, [x2,#72]
	ldp     x27, x28, [x2,#88]
    subs    x26, x26, x8 
	ldp     x29, x30, [x2,#104]
    sbcs    x27, x27, x9
    sbcs    x28, x28, x10
	ldp     x3, x4, [x2,#40]
    sbcs    x29, x29, x11
    sbcs    x30, x30, x12
	ldp     x5, x6, [x2,#56]
    sbcs    x3, x3, x13
    sbcs    x4, x4, x14
    sbcs    x5, x5, x15
    sbcs    x6, x6, x16
    sbc     x7, x7, x17
    
	adds    x19, x19, x26
	adcs    x20, x20, x27 
    ldp     x25, x26, [sp,#48]
	adcs    x21, x21, x28
	adcs    x22, x22, x29
    stp     x19, x20, [x2,#40]   
    adcs    x23, x23, x30  
    ldp     x27, x28, [sp,#64] 
    adcs    x3, x3, x8 
    stp     x21, x22, [x2,#56]
    adcs    x4, x4, x9
    ldp     x19, x20, [sp,#0]  
    adcs    x5, x5, x10 
    stp     x23, x3, [x2,#72]
    adcs    x6, x6, x11
    ldp     x21, x22, [sp,#16]
    adcs    x7, x7, x12
    stp     x4, x5, [x2,#88] 
    adcs    x13, x13, xzr
    ldp     x23, x24, [sp,#32]
    adcs    x14, x14, xzr
    stp     x6, x7, [x2,#104] 
    adcs    x15, x15, xzr
    ldp     x29, x30, [sp,#80] 
    adcs    x16, x16, xzr
    stp     x13, x14, [x2,#120]
    adc     x17, x17, xzr  
    stp     x15, x16, [x2,#136] 
    str     x17, [x2,#152]  
    
    add     sp, sp, #96
    ret
    
        
//////////////////////////////////////////// MACRO
.macro MUL128x320_COMBA_CUT  A0, A1, B0, B1, B2, B3, B4, C0, C1, C2, C3, C4, C5, C6, T0, T1, T2, T3
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
    
    mul     \T0, \A0, \B4
    umulh   \T1, \A0, \B4 
    adds    \C4, \C4, \T2
    adcs    \C5, \C5, \T3
    adc     \C6, xzr, xzr
    
    mul     \T2, \A1, \B4
    umulh   \T3, \A1, \B4 
    adds    \C4, \C4, \T0
    adcs    \C5, \C5, \T1
    adc     \C6, \C6, xzr

    adds    \C5, \C5, \T2
    adc     \C6, \C6, \T3
.endm


//**************************************************************************************
//  Montgomery reduction
//  Based on method described in Faz-Hernandez et al. https://eprint.iacr.org/2017/1015  
//  Operation: mc [x1] = ma [x0]
//  NOTE: ma=mc is not allowed
//************************************************************************************** 
.global oqs_kem_sike_rdc610_asm
oqs_kem_sike_rdc610_asm:
    sub     sp, sp, #96
    stp     x19, x20, [sp]
    stp     x21, x22, [sp,#16]
    ldp     x2, x3, [x0,#0]       // a[0-1]

    // x19-x23 <- load the prime constant
    ldr     x19, p610p1_nz_s6 + 0
    ldr     x20, p610p1_nz_s6 + 8
    ldr     x21, p610p1_nz_s6 + 16
    stp     x23, x24, [sp,#32]
    stp     x25, x26, [sp,#48]

    // a[0-1] x p610p1_nz_s6 --> result: x4:x10
    mul     x4, x2, x19           // a[0] x p610p1_nz_s6[0]
    umulh   x7, x2, x19 
    stp     x27, x28, [sp,#64]
    str     x29, [sp,#80]
    ldr     x22, p610p1_nz_s6 + 24
    ldr     x23, p610p1_nz_s6 + 32
    mul     x5, x2, x20           // a[0] x p610p1_nz_s6[1]
    umulh   x6, x2, x20
    MUL128x320_COMBA_CUT x2, x3, x19, x20, x21, x22, x23, x4, x5, x6, x7, x8, x9, x10, x26, x27, x28, x29

    orr     x26, xzr, x10, lsr #16
    lsl     x10, x10, #48
    orr     x10, x10, x9, lsr #16
    lsl     x9, x9, #48
    orr     x9, x9, x8, lsr #16
    lsl     x8, x8, #48  
    orr     x8, x8, x7, lsr #16
    lsl     x7, x7, #48
    orr     x7, x7, x6, lsr #16
    lsl     x6, x6, #48  
    ldp     x2, x3, [x0,#16]     // a[2-3]
    orr     x6, x6, x5, lsr #16
    lsl     x5, x5, #48   
    ldp     x11, x12, [x0,#32]
    orr     x5, x5, x4, lsr #16
    lsl     x4, x4, #48
    ldp     x13, x14, [x0,#48] 
    
    adds    x11, x4, x11          // a[4]
    adcs    x12, x5, x12          // a[5]
    ldp     x15, x16, [x0,#64]  
    adcs    x13, x6, x13
    adcs    x14, x7, x14  
    ldp     x17, x24, [x0,#80]  
    adcs    x15, x8, x15 
    mul     x4, x2, x19           // a[2] x p610p1_nz_s6[0]
    umulh   x7, x2, x19
    adcs    x16, x9, x16
    adcs    x17, x10, x17 
    mul     x5, x2, x20           // a[2] x p610p1_nz_s6[1]
    umulh   x6, x2, x20
    adcs    x24, x26, x24 
    adc     x25, xzr, xzr 

    // a[2-3] x p610p1_nz_s6 --> result: x4:x10
    MUL128x320_COMBA_CUT x2, x3, x19, x20, x21, x22, x23, x4, x5, x6, x7, x8, x9, x10, x26, x27, x28, x29  

    orr     x26, xzr, x10, lsr #16
    lsl     x10, x10, #48
    orr     x10, x10, x9, lsr #16
    lsl     x9, x9, #48
    orr     x9, x9, x8, lsr #16
    lsl     x8, x8, #48  
    orr     x8, x8, x7, lsr #16
    lsl     x7, x7, #48
    orr     x7, x7, x6, lsr #16
    lsl     x6, x6, #48  
    orr     x6, x6, x5, lsr #16
    lsl     x5, x5, #48   
    orr     x5, x5, x4, lsr #16
    lsl     x4, x4, #48
    
    adds    x13, x4, x13          // a[6]
    adcs    x14, x5, x14          // a[7]
    adcs    x15, x6, x15
    adcs    x16, x7, x16 
    adcs    x17, x8, x17 
    mul     x4, x11, x19           // a[4] x p610p1_nz_s6[0]
    umulh   x7, x11, x19
    adcs    x24, x9, x24
    adcs    x25, x10, x25 
    mul     x5, x11, x20           // a[4] x p610p1_nz_s6[1] 
    umulh   x6, x11, x20
    adcs    x2, x26, xzr
    adc     x3, xzr, xzr

    // a[4-5] x p610p1_nz_s6 --> result: x4:x10
    MUL128x320_COMBA_CUT x11, x12, x19, x20, x21, x22, x23, x4, x5, x6, x7, x8, x9, x10, x26, x27, x28, x29  

    orr     x26, xzr, x10, lsr #16
    lsl     x10, x10, #48
    orr     x10, x10, x9, lsr #16
    lsl     x9, x9, #48
    orr     x9, x9, x8, lsr #16
    lsl     x8, x8, #48  
    orr     x8, x8, x7, lsr #16
    lsl     x7, x7, #48
    orr     x7, x7, x6, lsr #16
    lsl     x6, x6, #48  
    orr     x6, x6, x5, lsr #16
    lsl     x5, x5, #48   
    orr     x5, x5, x4, lsr #16
    lsl     x4, x4, #48
    
    adds    x15, x4, x15          // a[8]
    adcs    x16, x5, x16          // a[9]
    adcs    x17, x6, x17
    adcs    x24, x7, x24  
    adcs    x25, x8, x25 
    mul     x4, x13, x19           // a[6] x p610p1_nz_s6[0]
    umulh   x7, x13, x19
    adcs    x2, x9, x2
    adcs    x3, x10, x3 
    mul     x5, x13, x20           // a[6] x p610p1_nz_s6[1] 
    umulh   x6, x13, x20
    adcs    x11, x26, xzr
    adc     x12, xzr, xzr

    // a[6-7] x p610p1_nz_s6 --> result: x4:x10
    MUL128x320_COMBA_CUT x13, x14, x19, x20, x21, x22, x23, x4, x5, x6, x7, x8, x9, x10, x26, x27, x28, x29 

    orr     x26, xzr, x10, lsr #16
    lsl     x10, x10, #48
    orr     x10, x10, x9, lsr #16
    lsl     x9, x9, #48
    orr     x9, x9, x8, lsr #16
    lsl     x8, x8, #48  
    orr     x8, x8, x7, lsr #16
    lsl     x7, x7, #48
    orr     x7, x7, x6, lsr #16
    lsl     x6, x6, #48  
    orr     x6, x6, x5, lsr #16
    lsl     x5, x5, #48   
    orr     x5, x5, x4, lsr #16
    lsl     x4, x4, #48
    
    adds    x17, x4, x17          // a[10]
    adcs    x24, x5, x24          // a[11]
    adcs    x25, x6, x25
    adcs    x2, x7, x2  
    adcs    x3, x8, x3 
    adcs    x11, x9, x11
    adcs    x12, x10, x12 
    stp     x17, x24, [x1,#0]     // Final result
    adcs    x13, x26, xzr
    adc     x14, xzr, xzr

    // a[8-9] x p610p1_nz_s6 --> result: x4:x10
    mul     x4, x15, x19           // a[8] x p610p1_nz_s6[0]
    umulh   x7, x15, x19
    mul     x5, x15, x20           // a[8] x p610p1_nz_s6[1] 
    umulh   x6, x15, x20
    MUL128x320_COMBA_CUT x15, x16, x19, x20, x21, x22, x23, x4, x5, x6, x7, x8, x9, x10, x26, x27, x28, x29  

    orr     x26, xzr, x10, lsr #16
    lsl     x10, x10, #48
    orr     x10, x10, x9, lsr #16
    lsl     x9, x9, #48
    orr     x9, x9, x8, lsr #16
    lsl     x8, x8, #48  
    orr     x8, x8, x7, lsr #16
    lsl     x7, x7, #48
    orr     x7, x7, x6, lsr #16
    lsl     x6, x6, #48  
    orr     x6, x6, x5, lsr #16
    lsl     x5, x5, #48   
    orr     x5, x5, x4, lsr #16
    lsl     x4, x4, #48
    
    adds    x25, x4, x25          
    adcs    x2, x5, x2  
    ldp     x19, x20, [x0,#96]           
    adcs    x3, x6, x3
    adcs    x11, x7, x11 
    ldp     x21, x22, [x0,#112] 
    adcs    x12, x8, x12 
    adcs    x13, x9, x13
    ldp     x23, x24, [x0,#128] 
    adcs    x14, x10, x14 
    adc     x15, x26, xzr	
    ldp     x4, x5, [x0,#144]
    
    adds    x25, x19, x25
    ldr     x29, [sp,#80]
    adcs    x2, x20, x2
    ldp     x27, x28, [sp, #64]
    adcs    x3, x21, x3
    stp     x25, x2, [x1,#16]   
    adcs    x11, x22, x11 
    ldp     x19, x20, [sp] 
    adcs    x12, x23, x12
    stp     x3, x11, [x1,#32]
    adcs    x13, x24, x13
    ldp     x25, x26, [sp, #48]
    adcs    x14, x4, x14
    stp     x12, x13, [x1,#48] 
    ldp     x23, x24, [sp, #32]
    adc     x15, x5, x15  
    ldp     x21, x22, [sp, #16]
    stp     x14, x15, [x1,#64]
    add     sp, sp, #96
    ret


//***********************************************************************
//  610-bit multiprecision addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_add610_asm
oqs_kem_sike_mp_add610_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]
	
    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,   [x2,#0]
    ldp     x3, x4,  [x0,#48]
    ldp     x11, x12, [x1,#48]
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    stp     x5, x6,   [x2,#16]
    ldp     x5, x6,   [x0,#64]
    ldp     x13, x14, [x1,#64]
    adcs    x7, x7, x15
    adcs    x8, x8, x16
    stp     x7, x8,   [x2,#32]
    adcs    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,  [x2,#48]
    adcs    x5, x5, x13
    adc     x6, x6, x14
    stp     x5, x6,   [x2,#64]
    ret    


//***********************************************************************
//  2x610-bit multiprecision addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_add610x2_asm
oqs_kem_sike_mp_add610x2_asm:
    ldp x3, x4,   [x0,#0]
    ldp x5, x6,   [x0,#16]
    ldp x7, x8,   [x0,#32]
    ldp x9, x10,  [x0,#48]
    ldp x11, x12, [x0,#64]

    ldp x13, x14, [x1,#0]
    ldp x15, x16, [x1,#16]
    adds x3, x3, x13
    adcs x4, x4, x14
    adcs x5, x5, x15
    adcs x6, x6, x16
    ldp x13, x14, [x1,#32]
    ldp x15, x16, [x1,#48]
    adcs x7, x7, x13
    adcs x8, x8, x14
    adcs x9, x9, x15
    adcs x10, x10, x16
    ldp x13, x14, [x1,#64]
    adcs x11, x11, x13
    adcs x12, x12, x14

    stp x3, x4,   [x2,#0]
    stp x5, x6,   [x2,#16]
    stp x7, x8,   [x2,#32]
    stp x9, x10,  [x2,#48]
    stp x11, x12, [x2,#64]
	
    ldp x3, x4,   [x0,#96]
    ldp x5, x6,   [x0,#112]
    ldp x7, x8,   [x0,#128]
    ldp x9, x10,  [x0,#144]
    ldp x11, x12, [x0,#160]

    ldp x13, x14, [x1,#96]
    ldp x15, x16, [x1,#112]
    adcs x3, x3, x13
    adcs x4, x4, x14
    adcs x5, x5, x15
    adcs x6, x6, x16
    ldp x13, x14, [x1,#128]
    ldp x15, x16, [x1,#144]
    adcs x7, x7, x13
    adcs x8, x8, x14
    adcs x9, x9, x15
    adcs x10, x10, x16
    ldp x13, x14, [x1,#160]
    adcs x11, x11, x13
    adc  x12, x12, x14

    stp x3, x4,   [x2,#96]
    stp x5, x6,   [x2,#112]
    stp x7, x8,   [x2,#128]
    stp x9, x10,  [x2,#144]
    stp x11, x12, [x2,#160]
    ret


//***********************************************************************
//  2x610-bit multiprecision subtraction/addition
//  Operation: c [x2] = a [x0] - b [x1]. If c < 0, add p610*2^640
//*********************************************************************** 
.global oqs_kem_sike_mp_subadd610x2_asm
oqs_kem_sike_mp_subadd610x2_asm:
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
    ldp     x11, x12, [x0,#64]
    ldp     x13, x14, [x1,#64]
    stp     x7, x8,   [x2,#32]
    stp     x9, x10,  [x2,#48]
    sbcs    x11, x11, x13
    sbcs    x12, x12, x14
    
    ldp     x3, x4,   [x0,#80]
    ldp     x5, x6,   [x0,#96]
    stp     x11, x12, [x2,#64]
    ldp     x13, x14, [x1,#80]
    ldp     x15, x16, [x1,#96]
    sbcs    x3, x3, x13
    sbcs    x4, x4, x14
    sbcs    x5, x5, x15
    sbcs    x6, x6, x16
    ldp     x7, x8,   [x0,#112]
    ldp     x9, x10,  [x0,#128]
    ldp     x13, x14, [x1,#112]
    ldp     x15, x16, [x1,#128]
    sbcs    x7, x7, x13
    sbcs    x8, x8, x14
    sbcs    x9, x9, x15
    sbcs    x10, x10, x16
    ldp     x11, x12, [x0,#144]
    ldp     x13, x14, [x1,#144]
    sbcs    x11, x11, x13
    sbcs    x12, x12, x14
    sbc     x0, xzr, xzr

    // Add p610 anded with the mask in x0 
    ldr     x1, p610 
    ldr     x13, p610 + 8
    ldr     x14, p610 + 16
    ldr     x15, p610 + 24
    and     x1, x1, x0 
    and     x13, x13, x0
    and     x14, x14, x0
    and     x15, x15, x0
	adds    x3, x3, x1   
	adcs    x4, x4, x1   
	adcs    x5, x5, x1   
	adcs    x6, x6, x1   
	adcs    x7, x7, x13 
	adcs    x8, x8, x14  
	adcs    x9, x9, x15  
    ldr     x13, p610 + 32
    ldr     x14, p610 + 40
    ldr     x15, p610 + 48	
    stp     x3, x4,   [x2,#80]
    stp     x5, x6,   [x2,#96]
    stp     x7, x8,   [x2,#112]
    and     x13, x13, x0
    and     x14, x14, x0
    and     x15, x15, x0
	adcs    x10, x10, x13  
	adcs    x11, x11, x14 
	adc     x12, x12, x15
    stp     x9, x10,  [x2,#128]
    stp     x11, x12, [x2,#144]
    ret


//***********************************************************************
//  Double 2x610-bit multiprecision subtraction
//  Operation: c [x2] = c [x2] - a [x0] - b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_dblsub610x2_asm
oqs_kem_sike_mp_dblsub610x2_asm:
    sub sp, sp, #64
    stp x19, x20, [sp]
    stp x21, x22, [sp, #16]
    stp x25, x26, [sp, #32]
    stp x27, x28, [sp, #48]

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

    ldp x25, x26, [x0,#0]
    ldp x27, x28, [x0,#16]
    subs x3, x3, x25
    sbcs x4, x4, x26
    sbcs x5, x5, x27
    sbcs x6, x6, x28
    ldp x25, x26, [x0,#32]
    ldp x27, x28, [x0,#48]
    sbcs x7, x7, x25
    sbcs x8, x8, x26
    sbcs x9, x9, x27
    sbcs x10, x10, x28
    ldp x25, x26, [x0,#64]
    ldp x27, x28, [x0,#80]
    sbcs x11, x11, x25
    sbcs x12, x12, x26
    sbcs x13, x13, x27
    sbcs x14, x14, x28
    ldp x25, x26, [x0,#96]
    ldp x27, x28, [x0,#112]
    sbcs x15, x15, x25
    sbcs x16, x16, x26
    sbcs x17, x17, x27
    sbcs x18, x18, x28
    ldp x25, x26, [x0,#128]
    ldp x27, x28, [x0,#144]
    sbcs x19, x19, x25
    sbcs x20, x20, x26
    sbcs x21, x21, x27
    sbc  x22, x22, x28

    ldp x25, x26, [x1,#0]
    ldp x27, x28, [x1,#16]
    subs x3, x3, x25
    sbcs x4, x4, x26
    sbcs x5, x5, x27
    sbcs x6, x6, x28
    ldp x25, x26, [x1,#32]
    ldp x27, x28, [x1,#48]
    sbcs x7, x7, x25
    sbcs x8, x8, x26
    sbcs x9, x9, x27
    sbcs x10, x10, x28
    ldp x25, x26, [x1,#64]
    ldp x27, x28, [x1,#80]
    sbcs x11, x11, x25
    sbcs x12, x12, x26
    sbcs x13, x13, x27
    sbcs x14, x14, x28
    ldp x25, x26, [x1,#96]
    ldp x27, x28, [x1,#112]
    sbcs x15, x15, x25
    sbcs x16, x16, x26
    sbcs x17, x17, x27
    sbcs x18, x18, x28
    ldp x25, x26, [x1,#128]
    ldp x27, x28, [x1,#144]
    sbcs x19, x19, x25
    sbcs x20, x20, x26
    sbcs x21, x21, x27
    sbc  x22, x22, x28

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
    
    ldp x19, x20, [sp]
    ldp x21, x22, [sp, #16]
    ldp x25, x26, [sp, #32]
    ldp x27, x28, [sp, #48]
    add sp, sp, #64
    ret
