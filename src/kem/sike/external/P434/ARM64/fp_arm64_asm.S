//*******************************************************************************************
// SIDH: an efficient supersingular isogeny cryptography library
//
// Abstract: field arithmetic in 64-bit ARMv8 assembly for P434 on Linux
//*******************************************************************************************

.text

// p434
p434:
.quad  0xFFFFFFFFFFFFFFFF
.quad  0xFDC1767AE2FFFFFF
.quad  0x7BC65C783158AEA3
.quad  0x6CFC5FD681C52056
.quad  0x0002341F27177344

// 2 * p434
p434x2:
.quad  0xFFFFFFFFFFFFFFFE
.quad  0xFFFFFFFFFFFFFFFF
.quad  0xFB82ECF5C5FFFFFF
.quad  0xF78CB8F062B15D47
.quad  0xD9F8BFAD038A40AC
.quad  0x0004683E4E2EE688

// 4 * p434
p434x4:
.quad  0xFFFFFFFFFFFFFFFC
.quad  0xFFFFFFFFFFFFFFFF 
.quad  0xF705D9EB8BFFFFFF 
.quad  0xEF1971E0C562BA8F
.quad  0xB3F17F5A07148159
.quad  0x0008D07C9C5DCD11

p434p1_nz:
.quad  0xFDC1767AE3000000
.quad  0x7BC65C783158AEA3
.quad  0x6CFC5FD681C52056
.quad  0x0002341F27177344


//***********************************************************************
//  Field addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_fpadd434_asm
oqs_kem_sike_fpadd434_asm:

    // Add a + b
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]
    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    adds    x3, x3, x11
    adcs    x4, x4, x12
    ldr     x9,       [x0,#48]
    ldr     x17,      [x1,#48]
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    ldr     x11, p434x2
    adcs    x7, x7, x15
    ldr     x12, p434x2 + 8
    adcs    x8, x8, x16
    ldr     x13, p434x2 + 16
    adc     x9, x9, x17
    
    //  Subtract 2xp434
    subs    x3, x3, x11
    ldr     x14, p434x2 + 24
    sbcs    x4, x4, x12
    ldr     x15, p434x2 + 32
    sbcs    x5, x5, x12
    ldr     x16, p434x2 + 40
    sbcs    x6, x6, x13
    sbcs    x7, x7, x14
    sbcs    x8, x8, x15
    sbcs    x9, x9, x16
    sbc     x10, xzr, xzr

    // Add 2xp434 anded with the mask in x10
    and     x11, x11, x10 
    and     x12, x12, x10 
    and     x13, x13, x10 
    and     x14, x14, x10 
    and     x15, x15, x10 
    and     x16, x16, x10   

    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,  [x2,#0]
    adcs    x5, x5, x12
    adcs    x6, x6, x13
    stp     x5, x6,  [x2,#16]
    adcs    x7, x7, x14
    adcs    x8, x8, x15 
    stp     x7, x8,  [x2,#32]
    adc     x9, x9, x16
    str     x9,      [x2,#48]
    ret


//***********************************************************************
//  Field subtraction
//  Operation: c [x2] = a [x0] - b [x1]
//*********************************************************************** 
.global oqs_kem_sike_fpsub434_asm
oqs_kem_sike_fpsub434_asm:

    // Subtract a - b
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]
    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    subs    x3, x3, x11
    sbcs    x4, x4, x12
    ldr     x9,       [x0,#48]
    ldr     x17,      [x1,#48]
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    ldr     x11, p434x2
    sbcs    x7, x7, x15
    ldr     x12, p434x2 + 8
    sbcs    x8, x8, x16
    ldr     x13, p434x2 + 16 
    sbcs    x9, x9, x17
    ldr     x14, p434x2 + 24
    sbc     x10, xzr, xzr
    
    // Add 2xp434 anded with the mask in x10
    ldr     x15, p434x2 + 32
    and     x11, x11, x10 
    ldr     x16, p434x2 + 40
    and     x12, x12, x10 
    and     x13, x13, x10
    and     x14, x14, x10 
    and     x15, x15, x10 
    and     x16, x16, x10  

    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,  [x2,#0]
    adcs    x5, x5, x12
    adcs    x6, x6, x13
    stp     x5, x6,  [x2,#16]
    adcs    x7, x7, x14
    adcs    x8, x8, x15 
    stp     x7, x8,  [x2,#32]
    adc     x9, x9, x16
    str     x9,      [x2,#48]
    ret


///////////////////////////////////////////////////////////////// MACRO
.macro SUB434_PX  P0

    // Subtract a - b
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]
    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    subs    x3, x3, x11
    sbcs    x4, x4, x12
    ldr     x9,       [x0,#48]
    ldr     x17,      [x1,#48]
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    ldr     x11, \P0
    sbcs    x7, x7, x15
    ldr     x12, \P0 + 8
    sbcs    x8, x8, x16
    ldr     x13, \P0 + 16 
    sbc     x9, x9, x17

    ldr     x14, \P0 + 24  
    adds    x3, x3, x11  
    ldr     x15, \P0 + 32
    adcs    x4, x4, x12
    ldr     x16, \P0 + 40
    stp     x3, x4,  [x2,#0]
    adcs    x5, x5, x12
    adcs    x6, x6, x13
    stp     x5, x6,  [x2,#16]
    adcs    x7, x7, x14
    adcs    x8, x8, x15 
    stp     x7, x8,  [x2,#32]
    adc     x9, x9, x16
    str     x9,      [x2,#48]
  .endm


//***********************************************************************
//  Multiprecision subtraction with correction with 2*p434
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 2*p434
//*********************************************************************** 
.global oqs_kem_sike_mp_sub434_p2_asm
oqs_kem_sike_mp_sub434_p2_asm:

  SUB434_PX  p434x2
  ret


//***********************************************************************
//  Multiprecision subtraction with correction with 4*p434
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 4*p434
//*********************************************************************** 
.global oqs_kem_sike_mp_sub434_p4_asm
oqs_kem_sike_mp_sub434_p4_asm:

  SUB434_PX  p434x4
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
.macro MUL192_COMBA_CUT_UNSAT  A0, A1, A2, B0, B1, B2, C0, C1, C2, C3, C4, C5, T0, T1, T2
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
    adc     \C3, \C3, xzr
    
    mul     \T0, \A2, \B0
    umulh   \T1, \A2, \B0
	adds    \C4, \C4, \T0
	adc     \C5, \C5, \T1
    
    mul     \T0, \A1, \B2
    umulh   \T1, \A1, \B2
    adds    \C5, \C5, \T0
    adc     \T1, \T1, xzr
    
    mul     \T0, \A2, \B1
    umulh   \T2, \A2, \B1
    adds    \C5, \C5, \T0
	and     \T0, \T0, xzr
    adc     \T1, \T1, \T2
	
	bfi     \T0, \C4, #60, #4
	lsr     \C4, \C4, #4
	bfi     \C4, \C5, #60, #4
	lsr     \C5, \C5, #4
	bfi     \C5, \T1, #60, #4
	lsr     \T1, \T1, #4
	
    adds    \C1, \C1, \T0
    adcs    \C2, \C2, \C4
    adcs    \C3, \C3, \C5
    adc     \T1, \T1, xzr
    
    mul     \C4, \A2, \B2
    umulh   \C5, \A2, \B2
	bfi     \T0, \C4, #56, #8
	lsr     \C4, \C4, #8
	bfi     \C4, \C5, #56, #8
	lsr     \C5, \C5, #8
	
    adds    \C3, \C3, \T0
    adcs    \C4, \C4, \T1
    adc     \C5, \C5, xzr
.endm

    
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


#if 0

//////////////////////////////////////////// MACRO
// Inputs:  A0,A1,A2,A3,B0,B1,B2,B3
// Outputs: C0,C1,C2,C3,A0,A1,B0,B1
////////////////////////////////////////////
.macro	MUL256_KARATSUBA_COMBA_UNSAT  A0,A1,A2,A3,B0,B1,B2,B3,C0,C1,C2,C3,T0,T1,T2,T3,T4

    // C0-C1 <- AH + AL
    adds    \C0, \A0, \A2
    adc     \C1, \A1, \A3

    // C2-C3 <- BH + BL
    adds    \C2, \B0, \B2
    adc     \C3, \B1, \B3

    // T0-T3 <- (AH+AL) x (BH+BL)
    mul     \T0, \C0, \C2
    umulh   \T3, \C0, \C2
    mul     \T1, \C0, \C3
    umulh   \T2, \C0, \C3    
    MUL128_COMBA_CUT  \C0, \C1, \C2, \C3, \T0, \T1, \T2, \T3, \T4

    // C0-C3 <- AL x BL
    mul     \C0, \A0, \B0
    umulh   \C3, \A0, \B0
    mul     \C1, \A0, \B1
    umulh   \C2, \A0, \B1    
    MUL128_COMBA_CUT  \A0, \A1, \B0, \B1, \C0, \C1, \C2, \C3, \T4
    
    // T0-T3 <- (AH+AL) x (BH+BL) - ALxBL
    mul     \A0, \A2, \B2
    umulh   \B1, \A2, \B2
    subs    \T0, \T0, \C0 
    sbcs    \T1, \T1, \C1
    mul     \A1, \A2, \B3
    umulh   \B0, \A2, \B3    
    sbcs    \T2, \T2, \C2
    sbc     \T3, \T3, \C3

    // A0-A1, B0-B1 <- AH x BH
    MUL128_COMBA_CUT  \A2, \A3, \B2, \B3, \A0, \A1, \B0, \B1, \T4
    
    // A2-A3, B2-B3 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
	and     \T4, \T4, xzr
    subs    \A2, \T0, \A0 
    sbcs    \A3, \T1, \A1
	
	bfi     \T4, \A2, #60, #4
	lsr     \A2, \A2, #4
    sbcs    \B2, \T2, \B0
	bfi     \A2, \A3, #60, #4
	lsr     \A3, \A3, #4
    sbc     \B3, \T3, \B1
	bfi     \A3, \B2, #60, #4
	lsr     \B2, \B2, #4
	adds    \C1, \C1, \T4
	bfi     \B2, \B3, #60, #4
	lsr     \B3, \B3, #4
	adcs    \C2, \C2, \A2	 
	
	bfi     \T4, \A0, #56, #8
	lsr     \A0, \A0, #8
	adcs    \C3, \C3, \A3 
	bfi     \A0, \A1, #56, #8
	lsr     \A1, \A1, #8
	adcs    \B2, \B2, xzr 
	adcs    \B3, \B3, xzr
	bfi     \A1, \B0, #56, #8
	lsr     \B0, \B0, #8
	adds    \C3, \C3, \T4 
	adcs    \A0, \A0, \B2 
	bfi     \B0, \B1, #56, #8
	lsr     \B1, \B1, #8
	adcs    \A1, \A1, \B3		
	adcs    \B0, \B0, xzr 
	adc     \B1, \B1, xzr 
.endm


//////////////////////////////////////////// MACRO
// Inputs:  A0,A1,A2,A3,B0,B1,B2,B3
// Outputs: C0,C1,C2,C3,A0,A1,B0
////////////////////////////////////////////
.macro	MUL256_KARATSUBA_COMBA_UNSAT_B  A0,A1,A2,A3,B0,B1,B2,B3,C0,C1,C2,C3,T0,T1,T2,T3,T4

    // C0-C1 <- AH + AL
    adds    \C0, \A0, \A2
    adc     \C1, \A1, \A3

    // C2-C3 <- BH + BL
    adds    \C2, \B0, \B2
    adc     \C3, \B1, \B3

    // T0-T3 <- (AH+AL) x (BH+BL)
    mul     \T0, \C0, \C2
    umulh   \T3, \C0, \C2
    mul     \T1, \C0, \C3
    umulh   \T2, \C0, \C3    
    MUL128_COMBA_CUT  \C0, \C1, \C2, \C3, \T0, \T1, \T2, \T3, \T4

    // C0-C3 <- AL x BL
    mul     \C0, \A0, \B0
    umulh   \C3, \A0, \B0
    mul     \C1, \A0, \B1
    umulh   \C2, \A0, \B1    
    MUL128_COMBA_CUT  \A0, \A1, \B0, \B1, \C0, \C1, \C2, \C3, \T4
    
    // T0-T3 <- (AH+AL) x (BH+BL) - ALxBL
    mul     \A0, \A2, \B2
    umulh   \B1, \A2, \B2
    subs    \T0, \T0, \C0 
    sbcs    \T1, \T1, \C1
    mul     \A1, \A2, \B3
    umulh   \B0, \A2, \B3    
    sbcs    \T2, \T2, \C2
    sbc     \T3, \T3, \C3

    // A0-A1, B0-B1 <- AH x BH
    MUL128_COMBA_CUT  \A2, \A3, \B2, \B3, \A0, \A1, \B0, \B1, \T4
    
    // A2-A3, B2-B3 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
	and     \T4, \T4, xzr
    subs    \A2, \T0, \A0 
    sbcs    \A3, \T1, \A1
	
	bfi     \T4, \A2, #60, #4
	lsr     \A2, \A2, #4
    sbcs    \B2, \T2, \B0
	bfi     \A2, \A3, #60, #4
	lsr     \A3, \A3, #4
    sbc     \B3, \T3, \B1
	bfi     \A3, \B2, #60, #4
	lsr     \B2, \B2, #4
	adds    \C1, \C1, \T4
	bfi     \B2, \B3, #60, #4
	lsr     \B3, \B3, #4
	adcs    \C2, \C2, \A2	 
	
	bfi     \T4, \A0, #56, #8
	lsr     \A0, \A0, #8
	adcs    \C3, \C3, \A3 
	bfi     \A0, \A1, #56, #8
	lsr     \A1, \A1, #8
	adcs    \B2, \B2, xzr 
	adcs    \B3, \B3, xzr
	bfi     \A1, \B0, #56, #8
	lsr     \B0, \B0, #8
	adds    \C3, \C3, \T4 
	bfi     \B0, \B1, #56, #8
	adcs    \A0, \A0, \B2 
	adcs    \A1, \A1, \B3		
	adc     \B0, \B0, xzr 
.endm


//**************************************************************************
//  448-bit multiplication using Karatsuba (two levels), Comba (lower level)
//  Unsaturated representation
//************************************************************************** 
.global oqs_kem_sike_mul434_asm
oqs_kem_sike_mul434_asm:
    sub     sp, sp, #96
    ldr     x9,       [x0,#48]
    ldr     x17,      [x1,#48]
    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    stp     x19, x20, [sp]
    stp     x21, x22, [sp,#16]

	lsl     x9, x9, #12
	lsl     x17, x17, #12
    ldp     x5, x6, [x0,#16]
	bfxil   x9, x8, #52, #12
    str     x9,     [x2,#48]  
	bfxil   x17, x16, #52, #12
	lsl     x8, x8, #8
	lsl     x16, x16, #8
    ldp     x13, x14, [x1,#16]
	bfxil   x8, x7, #56, #8
	bfxil   x16, x15, #56, #8
	lsl     x7, x7, #8
	lsl     x15, x15, #8
    ldp     x3, x4, [x0]
	bfxil   x7, x6, #56, #8
	bfxil   x15, x14, #56, #8 
	lsl     x6, x6, #4
	lsl     x14, x14, #4
    ldp     x11, x12, [x1]
	bfxil   x6, x5, #60, #4  
	bfxil   x14, x13, #60, #4   
	lsl     x5, x5, #4
	lsl     x13, x13, #4
	bfxil   x5, x4, #60, #4		 
	bfxil   x13, x12, #60, #4
	bfm     x8, xzr, #4, #3 
    stp     x7, x8, [x2,#32]  
	bfm     x6, xzr, #4, #3  
	bfm     x4, xzr, #4, #3   
	bfm     x16, xzr, #4, #3  
	bfm     x14, xzr, #4, #3  
	bfm     x12, xzr, #4, #3     

    // x7-x10 <- AH + AL
    adds    x7, x3, x7
    adc     x8, x4, x8
    stp     x23, x24, [sp,#32]
    adds    x9, x5, x9
    adc     x10, x6, xzr 

    stp     x15, x16, [x2,#64]

    // x15-x16, x28-x29 <- BH + BL
    adds    x15, x11, x15
    stp     x25, x26, [sp,#48]
    adc     x16, x12, x16
    stp     x27, x28, [sp,#64]
    adds    x28, x13, x17
    str     x29, [sp,#80]
    adc     x29, x14, xzr

    // x19-x22, x7-x8, x15 <- (AH+AL) x (BH+BL), low part 
    MUL256_KARATSUBA_COMBA_UNSAT_B  x7, x8, x9, x10, x15, x16, x28, x29, x19, x20, x21, x22, x23, x24, x25, x26, x27

    // x23-x26, x3-x4, x11-x12  <- AL x BL 
    MUL256_KARATSUBA_COMBA_UNSAT  x3, x4, x5, x6, x11, x12, x13, x14, x23, x24, x25, x26, x27, x28, x29, x9, x10
    
    // x19-x22, x7-x8, x15-x16 <- (AH+AL) x (BH+BL) - ALxBL
    ldp     x5, x6, [x2,#32]
    ldp     x13, x14, [x2,#64]	
    subs    x19, x19, x23 
    sbcs    x20, x20, x24
	stp     x23, x24, [x2]
    mul     x23, x5, x13
    mul     x24, x5, x14 
    sbcs    x21, x21, x25
    sbcs    x22, x22, x26    
	stp     x25, x26, [x2,#16] 
    sbcs    x7, x7, x3
    umulh   x26, x5, x13 
    umulh   x25, x5, x14 
    sbcs    x8, x8, x4 
    ldr     x9,     [x2,#48]
    sbc     x15, x15, x11		

    // x23-x28 <- AH x BH
    MUL192_COMBA_CUT_UNSAT  x5, x6, x9, x13, x14, x17, x23, x24, x25, x26, x27, x28, x0, x1, x29
    
    // x19-x22, x7-x8, x15 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
	and     x0, x0, xzr	
    subs    x19, x19, x23 
    sbcs    x20, x20, x24

	ldr     x29, [x2,#24]
	bfi     x0, x19, #56, #8
	bfi     x12, x23, #48, #16
	lsr     x19, x19, #8
	lsr     x23, x23, #16
    sbcs    x21, x21, x25
	bfi     x19, x20, #56, #8
	bfi     x23, x24, #48, #16
	lsr     x20, x20, #8
	lsr     x24, x24, #16
    sbcs    x22, x22, x26
    sbcs    x7, x7, x27
	bfi     x20, x21, #56, #8
	bfi     x24, x25, #48, #16
	lsr     x21, x21, #8
	lsr     x25, x25, #16 
    sbcs    x8, x8, x28
    sbc     x15, x15, xzr
	adds    x29, x0, x29
    str     x29,      [x2,#24]
	bfi     x21, x22, #56, #8
	bfi     x25, x26, #48, #16
	lsr     x22, x22, #8
	lsr     x26, x26, #16
	adcs    x19, x19, x3
	adcs    x20, x20, x4
    stp     x19, x20, [x2,#32]
    ldp     x19, x20, [sp]
	bfi     x22, x7, #56, #8
	bfi     x26, x27, #48, #16
	lsr     x7, x7, #8
	lsr     x27, x27, #16
	adcs    x21, x21, x11
	adcs    x22, x22, x12
    stp     x21, x22, [x2,#48]
    ldp     x21, x22, [sp,#16]
	bfi     x7, x8, #56, #8
	bfi     x27, x28, #48, #16
	lsr     x8, x8, #8
	lsr     x28, x28, #16
	bfi     x8, x15, #56, #8
	lsr     x15, x15, #8
	
    ldr     x29, [sp,#80]
	adcs    x23, x23, x7
	adcs    x24, x24, x8
    stp     x23, x24, [x2,#64]
    ldp     x23, x24, [sp,#32]
	adcs    x25, x25, x15
	adcs    x26, x26, xzr
    stp     x25, x26, [x2,#80]
    ldp     x25, x26, [sp,#48]
	adcs    x27, x27, xzr
	adc     x28, x28, xzr
    stp     x27, x28, [x2,#96]    
    ldp     x27, x28, [sp,#64]
    add     sp, sp, #96
    ret

#endif

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
//  448-bit integer multiplication using Karatsuba (two levels), Comba (lower level)
//  Operation: c [x2] = a [x0] * b [x1]
//*********************************************************************************** 
.global oqs_kem_sike_mul434_asm
oqs_kem_sike_mul434_asm:
    sub     sp, sp, #96
    ldp     x3, x4, [x0]
    ldp     x7, x8, [x0,#32]
    ldp     x5, x6, [x0,#16]
    ldr     x9,     [x0,#48]
    stp     x25, x26, [sp,#48]
    stp     x27, x28, [sp,#64]
    stp     x29, x30, [sp,#80]

    // x26-x29 <- AH + AL, x7 <- mask
    adds    x26, x3, x7
    ldp     x11, x12, [x1,#0]
    adcs    x27, x4, x8
    ldp     x15, x16, [x1,#32]
    adcs    x28, x5, x9
    ldp     x13, x14, [x1,#16]
    adcs    x29, x6, xzr
    ldr     x17,      [x1,#48]
    adc     x7, xzr, xzr
    stp     x19, x20, [sp,#0]

    // x11-x14 <- BH + BL, x8 <- mask
    adds    x11, x11, x15
    adcs    x12, x12, x16
    stp     x21, x22, [sp,#16]
    adcs    x13, x13, x17
    adcs    x14, x14, xzr
    stp     x23, x24, [sp,#32]
    adc     x8, xzr, xzr
    
    // x10, x15-x17 <- masked (BH + BL)
    sub     x9, xzr, x7
    sub     x8, xzr, x8
    and     x10, x11, x9
    and     x15, x12, x9
    and     x16, x13, x9
    and     x17, x14, x9

    // x19-x22 <- masked (AH + AL)
    and     x19, x26, x8
    and     x20, x27, x8
    and     x21, x28, x8
    and     x22, x29, x8

    // x10, x15-x17 <- masked (AH+AL) + masked (BH+BL), step 1
    adds    x10, x10, x19
    adcs    x15, x15, x20
    adcs    x16, x16, x21
    stp     x26, x27, [x2,#0]
    adcs    x17, x17, x22
    
    // x8-x9, x30, x19-x23 <- (AH+AL) x (BH+BL), low part
    MUL256_KARATSUBA_COMBA  x2, x26, x27, x28, x29, x11, x12, x13, x14, x8, x9, x30, x19, x20, x21, x22, x23, x24, x25  
    
    // x8-x9, x30, x19, x10, x15-x17 <- (AH+AL) x (BH+BL), final step
    adds    x10, x10, x20
    ldp     x11, x12, [x1,#0]
    adcs    x15, x15, x21
    adcs    x16, x16, x22
    ldp     x13, x14, [x1,#16]
    adcs    x17, x17, x23

    // x20-x27 <- AL x BL
    MUL256_KARATSUBA_COMBA  x0, x3, x4, x5, x6, x11, x12, x13, x14, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29
    
    // x8-x9, x30, x19, x10, x15-x17 <- (AH+AL) x (BH+BL) - ALxBL
    ldp     x3, x4,   [x0,#32]
    ldp     x11, x12, [x1,#32]
    subs    x8, x8, x20 
    sbcs    x9, x9, x21
    stp     x20, x21, [x2]        // Output c0-c3
    ldr     x5,       [x0,#48]
    mul     x20, x3, x11   
    mul     x21, x3, x12
    sbcs    x30, x30, x22
    sbcs    x19, x19, x23
    stp     x22, x23, [x2,#16]
    ldr     x13,      [x1,#48]
    sbcs    x10, x10, x24
    umulh   x22, x3, x12     
    umulh   x23, x3, x11
    sbcs    x15, x15, x25
    sbcs    x16, x16, x26
    sbcs    x17, x17, x27

    // x20-x23, x28-x29 <- AH x BH
    MUL192_COMBA_CUT  x3, x4, x5, x11, x12, x13, x20, x21, x22, x23, x28, x29, x6, x0
    
    // x8-x9, x30, x19, x10, x15-x17 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    subs    x8, x8, x20 
    sbcs    x9, x9, x21
    sbcs    x30, x30, x22
    sbcs    x19, x19, x23
    sbcs    x10, x10, x28
    sbcs    x15, x15, x29
    sbcs    x16, x16, xzr
    sbcs    x17, x17, xzr	

    adds    x8, x8, x24 
    adcs    x9, x9, x25
    stp     x8, x9,   [x2,#32]    // Output c4-c7
    adcs    x30, x30, x26
    ldp     x25, x26, [sp,#48]
    adcs    x19, x19, x27
    stp     x30, x19, [x2,#48]  
    adcs    x10, x10, x20 
    ldp     x19, x20, [sp,#0]     
    adcs    x15, x15, x21 
    stp     x10, x15, [x2,#64]    // Output c8-c13         
    adcs    x16, x16, x22
    ldp     x21, x22, [sp,#16]
    adcs    x17, x17, x23
    stp     x16, x17, [x2,#80] 
    adcs    x28, x28, xzr
    ldp     x23, x24, [sp,#32]
    adc     x29, x29, xzr
    stp     x28, x29, [x2,#96]    
    ldp     x27, x28, [sp,#64]
    ldp     x29, x30, [sp,#80]
    add     sp, sp, #96
    ret
    
        
//////////////////////////////////////////// MACRO
.macro MUL64x256_CUT  A0, B0, B1, B2, B3, C0, C1, C2, C3, C4, T0
    mul     \T0, \A0, \B1
    umulh   \C2, \A0, \B1 
    adds    \C1, \C1, \T0
    adc     \C2, \C2, xzr

    mul     \T0, \A0, \B2
    umulh   \C3, \A0, \B2 
    adds    \C2, \C2, \T0
    adc     \C3, \C3, xzr

    mul     \T0, \A0, \B3
    umulh   \C4, \A0, \B3 
    adds    \C3, \C3, \T0
    adc     \C4, \C4, xzr
.endm
    
        
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
.global oqs_kem_sike_rdc434_asm
oqs_kem_sike_rdc434_asm:
    sub     sp, sp, #80
    stp     x21, x24, [sp,#16]
    stp     x25, x26, [sp,#32]
    ldp     x2, x3, [x0,#0]       // a[0-1]

    // Load the prime constant
    ldr     x24, p434p1_nz + 0
    ldr     x25, p434p1_nz + 8
    stp     x19, x20, [sp]
    stp     x27, x28, [sp,#48]

    // a[0-1] x p434p1_nz --> result: x4:x8, x2
    mul     x4, x2, x24           // a[0] x p434p1_nz[0]
    umulh   x7, x2, x24  
    stp     x29, x30, [sp,#64] 
    ldr     x26, p434p1_nz + 16
    ldr     x27, p434p1_nz + 24        
    mul     x5, x2, x25           // a[0] x p434p1_nz[1]
    umulh   x6, x2, x25
    ldr     x10, [x0,#24]         
    ldp     x11, x12, [x0,#32]    
    ldp     x13, x14, [x0,#48]
    MUL128x256_COMBA_CUT x2, x3, x24, x25, x26, x27, x4, x5, x6, x7, x8, x2, x28, x29, x30, x9
    
    ldr     x3, [x0,#16]          // a[2]    
    adds    x10, x4, x10          // a[3]
    adcs    x11, x5, x11          // a[4]
    ldp     x15, x16, [x0,#64]   
    adcs    x12, x6, x12
    adcs    x13, x7, x13  
    ldp     x17, x19, [x0,#80]  
    adcs    x14, x8, x14 
    mul     x4, x3, x24           // a[2] x p434p1_nz[0]
    umulh   x7, x3, x24
    adcs    x15, x2, x15
    adcs    x16, xzr, x16
    ldp     x20, x21, [x0,#96]  
    adcs    x17, xzr, x17
    adcs    x19, xzr, x19
    mul     x5, x3, x25           // a[2] x p434p1_nz[1] 
    umulh   x6, x3, x25
    adcs    x20, xzr, x20
    adc     x21, xzr, x21

    // a[2-3] x p434p1_nz --> result: x4:x8, x3
    MUL128x256_COMBA_CUT x3, x10, x24, x25, x26, x27, x4, x5, x6, x7, x8, x3, x28, x29, x30, x9
    
    adds    x12, x4, x12          // a[5]
    adcs    x13, x5, x13          // a[6]
    adcs    x14, x6, x14
    adcs    x15, x7, x15
    mul     x4, x11, x24          // a[4] x p434p1_nz[0]
    umulh   x7, x11, x24
    adcs    x16, x8, x16
    adcs    x17, x3, x17
    adcs    x19, xzr, x19
    adcs    x20, xzr, x20
    mul     x5, x11, x25          // a[4] x p434p1_nz[1]
    umulh   x6, x11, x25
    adc     x21, xzr, x21

    // a[4-5] x p434p1_nz --> result: x4:x8, x11
    MUL128x256_COMBA_CUT x11, x12, x24, x25, x26, x27, x4, x5, x6, x7, x8, x11, x28, x29, x30, x9
    
    adds    x14, x4, x14          
    adcs    x15, x5, x15 
    stp     x14, x15, [x1,#0]     // Final result         
    adcs    x16, x6, x16
    mul     x2, x13, x24          // a[6] x p434p1_nz[0]
    umulh   x3, x13, x24
    adcs    x17, x7, x17
    adcs    x19, x8, x19
    adcs    x20, x11, x20
    adc     x21, xzr, x21

    // a[6] x p434p1_nz --> result: x2:x5, x8
    MUL64x256_CUT x13, x24, x25, x26, x27, x2, x3, x4, x5, x8, x30
    
    adds    x16, x2, x16
    ldp     x25, x26, [sp, #32] 
    adcs    x17, x3, x17
    stp     x16, x17, [x1,#16] 
    ldp     x27, x28, [sp, #48]
    adcs    x19, x4, x19
    ldp     x29, x30, [sp, #64]
    adcs    x20, x5, x20  
    stp     x19, x20, [x1,#32]
    ldp     x19, x20, [sp]
    adc     x21, x8, x21
    str     x21, [x1,#48]    
    ldp     x21, x24, [sp, #16]
    add     sp, sp, #80
    ret


//***********************************************************************
//  434-bit multiprecision addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_add434_asm
oqs_kem_sike_mp_add434_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]

    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x1,#32]
    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,   [x2,#0]
    ldr     x9,       [x0,#48]
    ldr     x17,      [x1,#48]
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    stp     x5, x6,   [x2,#16]
    adcs    x7, x7, x15
    adcs    x8, x8, x16
    stp     x7, x8,   [x2,#32]
    adc     x9, x9, x17
    str     x9,       [x2,#48]
    ret    


//***********************************************************************
//  2x434-bit multiprecision addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_add434x2_asm
oqs_kem_sike_mp_add434x2_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]

    ldp     x7, x8,   [x0,#32]
    adds    x3, x3, x11
    adcs    x4, x4, x12
    stp     x3, x4,   [x2,#0]
    ldp     x11, x12, [x1,#32]
    adcs    x5, x5, x13
    ldp     x9, x10,  [x0,#48]
    ldp     x15, x16, [x1,#48]
    adcs    x6, x6, x14
    stp     x5, x6,   [x2,#16]
    adcs    x7, x7, x11
    adcs    x8, x8, x12
    stp     x7, x8,   [x2,#32]
    adcs    x9, x9, x15
    adcs    x10, x10, x16
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
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    stp     x5, x6,   [x2,#80]
    adcs    x7, x7, x15
    adc     x8, x8, x16
    stp     x7, x8,   [x2,#96]
    ret


//***********************************************************************
//  2x434-bit multiprecision subtraction/addition
//  Operation: c [x2] = a [x0] - b [x1]. If c < 0, add p434*2^448
//*********************************************************************** 
.global oqs_kem_sike_mp_subadd434x2_asm
oqs_kem_sike_mp_subadd434x2_asm:
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x1,#0]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x1,#16]
	
    ldp     x7, x8,   [x0,#32]
    subs    x3, x3, x11
    sbcs    x4, x4, x12
    stp     x3, x4,   [x2,#0]
    ldp     x11, x12, [x1,#32]
    sbcs    x5, x5, x13
    ldp     x9, x10,  [x0,#48]
    ldp     x15, x16, [x1,#48]
    sbcs    x6, x6, x14
    stp     x5, x6,   [x2,#16]
    sbcs    x7, x7, x11
    sbcs    x8, x8, x12
    stp     x7, x8,   [x2,#32]
    sbcs    x9, x9, x15
    sbcs    x10, x10, x16	
    str     x9,       [x2,#48]
    
    ldp     x3, x4,   [x0,#64]
    ldp     x11, x12, [x1,#64]
    ldp     x5, x6,   [x0,#80]
    ldp     x13, x14, [x1,#80]
	
    ldp     x7, x8,   [x0,#96]
    ldp     x15, x16, [x1,#96]
    sbcs    x3, x3, x11
    sbcs    x4, x4, x12
    ldr     x11, p434
    sbcs    x5, x5, x13
    sbcs    x6, x6, x14
    ldr     x12, p434 + 8
    sbcs    x7, x7, x15
    ldr     x13, p434 + 16
    sbcs    x8, x8, x16
    ldr     x14, p434 + 24
    sbc     x0, xzr, xzr

    // Add p434 anded with the mask in x0 
    ldr     x15, p434 + 32
    and     x11, x11, x0 
    and     x12, x12, x0 
    and     x13, x13, x0 
    and     x14, x14, x0 
    and     x15, x15, x0 
	adds    x10, x10, x11  
    str     x10,      [x2,#56] 
	adcs    x3, x3, x11    
	adcs    x4, x4, x11  
    stp     x3, x4,   [x2,#64] 
	adcs    x5, x5, x12   
	adcs    x6, x6, x13  
    stp     x5, x6,   [x2,#80] 
	adcs    x7, x7, x14   
	adc     x8, x8, x15 	
    stp     x7, x8,   [x2,#96]
    ret


//***********************************************************************
//  Double 2x434-bit multiprecision subtraction
//  Operation: c [x2] = c [x2] - a [x0] - b [x1]
//*********************************************************************** 
.global oqs_kem_sike_mp_dblsub434x2_asm
oqs_kem_sike_mp_dblsub434x2_asm:
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
    ldp     x27, x28, [x0,#64]
    sbcs    x9, x9, x29
    sbcs    x10, x10, x30
    ldp     x29, x30, [x0,#80]
    ldp     x15, x16, [x2,#96]
    sbcs    x11, x11, x27
    sbcs    x12, x12, x28
    ldp     x27, x28, [x0,#96]
    sbcs    x13, x13, x29
    sbcs    x14, x14, x30
    ldp     x29, x30, [x1,#0]
    sbcs    x15, x15, x27
    sbc     x16, x16, x28

    ldp     x27, x28, [x1,#16]
    subs    x3, x3, x29
    sbcs    x4, x4, x30
    stp     x3, x4,   [x2,#0]
    ldp     x29, x30, [x1,#32]
    sbcs    x5, x5, x27
    sbcs    x6, x6, x28
    stp     x5, x6,   [x2,#16]
    ldp     x27, x28, [x1,#48]
    sbcs    x7, x7, x29
    sbcs    x8, x8, x30
    stp     x7, x8,   [x2,#32]
    ldp     x29, x30, [x1,#64]
    sbcs    x9, x9, x27
    sbcs    x10, x10, x28
    stp     x9, x10,  [x2,#48]
    ldp     x27, x28, [x1,#80]
    sbcs    x11, x11, x29
    sbcs    x12, x12, x30
    stp     x11, x12, [x2,#64]
    ldp     x29, x30, [x1,#96]
    sbcs    x13, x13, x27
    sbcs    x14, x14, x28
    stp     x13, x14, [x2,#80]
    ldp     x27, x28, [sp, #0]
    sbcs    x15, x15, x29
    sbc     x16, x16, x30
    stp     x15, x16, [x2,#96]    
    ldp     x29, x30, [sp, #16]
    add     sp, sp, #32
    ret

#if 0

//////////////////////////////////////////// MACRO
.macro    MUL256_KARATSUBA_COMBA_B  A0,A1,A2,A3,B0,B1,B2,B3,C0,C1,C2,C3,C4,C5,C6,C7,T0,T1,T2,T3

    // T2-A1 <- AH + AL, T0 <- mask
    adds    \T2, \A0, \A2
    adcs    \T3, \A1, \A3
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
    and     \C4, \T2, \C3
    and     \C5, \T3, \C3
    mul     \C2, \T2, \C6
    mul     \C3, \T2, \T1
    and     \T0, \T0, \C7

    // C0-C1, T0 <- (AH+AL) x (BH+BL), part 1
    adds    \C0, \C4, \C0
    umulh   \C4, \T2, \T1    
    adcs    \C1, \C5, \C1
    umulh   \C5, \T2, \C6
    adc     \T0, \T0, xzr

    // C2-C5 <- (AH+AL) x (BH+BL), low part
    MUL128_COMBA_CUT  \T2, \T3, \C6, \T1, \C2, \C3, \C4, \C5, \C7
    //ldp     \A0, \A1, [\M,#0]
    
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


//***********************************************************************
//  Field addition
//  Operation: c [x2] = a [x0] + b [x1]
//*********************************************************************** 
.global oqs_kem_sike_fpcombined434_asm
oqs_kem_sike_fpcombined434_asm:
    sub     sp, sp, #96
    stp     x19, x20, [sp,#0]
    stp     x21, x22, [sp,#16]
	
    ldp     x3, x4,   [x0,#0]
    ldp     x11, x12, [x0,#56]
    ldp     x5, x6,   [x0,#16]
    ldp     x13, x14, [x0,#72]

    ldp     x7, x8,   [x0,#32]
    ldp     x15, x16, [x0,#88]
    adds    x3, x3, x11
    adcs    x4, x4, x12
    ldp     x11, x12,   [x1,#0]
    ldr     x9,       [x0,#48]
    ldr     x17,      [x0,#104]
    adcs    x5, x5, x13
    adcs    x6, x6, x14
    ldp     x13, x14,   [x1,#16]
    adcs    x7, x7, x15
    adcs    x8, x8, x16
    stp     x7, x8,   [x2,#56]
    adc     x9, x9, x17
    str     x9,       [x2,#72]
	
    ldp     x19, x20, [x1,#56]
    ldp     x21, x22, [x1,#72]
    stp     x23, x24, [sp,#32]
    stp     x25, x26, [sp,#48]
	
    ldp     x15, x16, [x1,#32]
    ldp     x23, x24, [x1,#88]
    adds    x11, x11, x19
    adcs    x12, x12, x20
    stp     x11, x12, [x2,#0]
    ldr     x17,      [x1,#48]
    ldr     x25,      [x1,#104]
    adcs    x13, x13, x21
    adcs    x14, x14, x22
    stp     x13, x14, [x2,#16]      
    adcs    x15, x15, x23
    adcs    x16, x16, x24
    stp     x15, x16, [x2,#32]      
    adc     x17, x17, x25
    str     x17,      [x2,#48]      
	

    stp     x27, x28, [sp,#64]
    stp     x29, x30, [sp,#80]

    // x26-x29 <- AH + AL, x7 <- mask
    adds    x26, x3, x7
    adcs    x27, x4, x8
    adcs    x28, x5, x9
    adcs    x29, x6, xzr
    adc     x7, xzr, xzr

    // x11-x14 <- BH + BL, x8 <- mask
    adds    x11, x11, x15
    adcs    x12, x12, x16
    adcs    x13, x13, x17
    adcs    x14, x14, xzr
    adc     x8, xzr, xzr
    
    // x10, x15-x17 <- masked (BH + BL)
    sub     x9, xzr, x7
    sub     x8, xzr, x8
    and     x10, x11, x9
    and     x15, x12, x9
    and     x16, x13, x9
    and     x17, x14, x9

    // x19-x22 <- masked (AH + AL)
    and     x19, x26, x8
    and     x20, x27, x8
    and     x21, x28, x8
    and     x22, x29, x8

    // x10, x15-x17 <- masked (AH+AL) + masked (BH+BL), step 1
    adds    x10, x10, x19
    adcs    x15, x15, x20
    adcs    x16, x16, x21
    adcs    x17, x17, x22
    
    // x8-x9, x30, x19-x23 <- (AH+AL) x (BH+BL), low part
    MUL256_KARATSUBA_COMBA_B  x26, x27, x28, x29, x11, x12, x13, x14, x8, x9, x30, x19, x20, x21, x22, x23, x24, x25, x0, x1  
    
    // x8-x9, x30, x19, x10, x15-x17 <- (AH+AL) x (BH+BL), final step
    adds    x10, x10, x20
    ldp     x11, x12, [x2,#0]
    adcs    x15, x15, x21
    adcs    x16, x16, x22
    ldp     x13, x14, [x2,#16]
    adcs    x17, x17, x23

    // x20-x27 <- AL x BL
    MUL256_KARATSUBA_COMBA_B  x3, x4, x5, x6, x11, x12, x13, x14, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x0, x1
    
    // x8-x9, x30, x19, x10, x15-x17 <- (AH+AL) x (BH+BL) - ALxBL
    ldp     x3, x4,   [x2,#56]
    ldp     x11, x12, [x2,#32]
    subs    x8, x8, x20 
    sbcs    x9, x9, x21
    stp     x20, x21, [x2]        // Output c0-c3
    ldr     x5,       [x2,#72]
    mul     x20, x3, x11   
    mul     x21, x3, x12
    sbcs    x30, x30, x22
    sbcs    x19, x19, x23
    stp     x22, x23, [x2,#16]
    ldr     x13,      [x2,#48]
    sbcs    x10, x10, x24
    umulh   x22, x3, x12     
    umulh   x23, x3, x11
    sbcs    x15, x15, x25
    sbcs    x16, x16, x26
    sbcs    x17, x17, x27

    // x20-x23, x28-x29 <- AH x BH
    MUL192_COMBA_CUT  x3, x4, x5, x11, x12, x13, x20, x21, x22, x23, x28, x29, x6, x0
    
    // x8-x9, x30, x19, x10, x15-x17 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    subs    x8, x8, x20 
    sbcs    x9, x9, x21
    sbcs    x30, x30, x22
    sbcs    x19, x19, x23
    sbcs    x10, x10, x28
    sbcs    x15, x15, x29
    sbcs    x16, x16, xzr
    sbcs    x17, x17, xzr	

    adds    x8, x8, x24 
    adcs    x9, x9, x25
    stp     x8, x9,   [x2,#32]    // Output c4-c7
    adcs    x30, x30, x26
    ldp     x25, x26, [sp,#48]
    adcs    x19, x19, x27
    stp     x30, x19, [x2,#48]  
    adcs    x10, x10, x20 
    ldp     x19, x20, [sp,#0]     
    adcs    x15, x15, x21 
    stp     x10, x15, [x2,#64]    // Output c8-c13         
    adcs    x16, x16, x22
    ldp     x21, x22, [sp,#16]
    adcs    x17, x17, x23
    stp     x16, x17, [x2,#80] 
    adcs    x28, x28, xzr
    ldp     x23, x24, [sp,#32]
    adc     x29, x29, xzr
    stp     x28, x29, [x2,#96]    
    ldp     x27, x28, [sp,#64]
    ldp     x29, x30, [sp,#80]
    add     sp, sp, #96
    ret

#endif
