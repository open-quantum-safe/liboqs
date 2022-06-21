//*******************************************************************************************
// SIDH: an efficient supersingular isogeny cryptography library
// Copyright (c) Microsoft Corporation
//
// Website: https://github.com/microsoft/PQCrypto-SIDH
// Released under MIT license 
//
// Abstract: field arithmetic in x64 assembly for P610 on Linux
//******************************************************************************************* 

.intel_syntax noprefix 

// Format function and variable names for Mac OS X
#if defined(__APPLE__)
    #define fmt(f)    _oqs_kem_sike_##f
#else
    #define fmt(f)    oqs_kem_sike_##f
#endif

// Registers that are used for parameter passing:
#define reg_p1  rdi
#define reg_p2  rsi
#define reg_p3  rdx


.text
//***********************************************************************
//  610-bit multiprecision addition
//  Operation: c [reg_p3] = a [reg_p1] + b [reg_p2]
//*********************************************************************** 
.global fmt(mp_add610_asm)
fmt(mp_add610_asm):  
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  mov    rax, [reg_p1+32]
  add    r8, [reg_p2] 
  adc    r9, [reg_p2+8] 
  adc    r10, [reg_p2+16] 
  adc    r11, [reg_p2+24] 
  adc    rax, [reg_p2+32] 
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9
  mov    [reg_p3+16], r10
  mov    [reg_p3+24], r11
  mov    [reg_p3+32], rax

  mov    r8, [reg_p1+40]
  mov    r9, [reg_p1+48] 
  mov    r10, [reg_p1+56]
  mov    r11, [reg_p1+64] 
  mov    rax, [reg_p1+72] 
  adc    r8, [reg_p2+40] 
  adc    r9, [reg_p2+48]
  adc    r10, [reg_p2+56] 
  adc    r11, [reg_p2+64]
  adc    rax, [reg_p2+72]
  mov    [reg_p3+40], r8
  mov    [reg_p3+48], r9
  mov    [reg_p3+56], r10
  mov    [reg_p3+64], r11
  mov    [reg_p3+72], rax
  ret


//***********************************************************************
//  Field addition
//  Operation: c [reg_p3] = a [reg_p1] + b [reg_p2]
//*********************************************************************** 
.global fmt(fpadd610_asm)
fmt(fpadd610_asm):
  push   r12
  push   r13
  push   r14
  push   r15
  
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  mov    r12, [reg_p1+32]
  mov    r13, [reg_p1+40]
  mov    r14, [reg_p1+48]
  mov    r15, [reg_p1+56] 
  mov    rcx, [reg_p1+64]
  mov    rdi, [reg_p1+72]
  add    r8, [reg_p2] 
  adc    r9, [reg_p2+8] 
  adc    r10, [reg_p2+16] 
  adc    r11, [reg_p2+24] 
  adc    r12, [reg_p2+32] 
  adc    r13, [reg_p2+40] 
  adc    r14, [reg_p2+48] 
  adc    r15, [reg_p2+56]
  adc    rcx, [reg_p2+64]
  adc    rdi, [reg_p2+72]

  mov    rax, [rip+fmt(p610x2)]
  sub    r8, rax
  mov    rax, [rip+fmt(p610x2)+8]
  sbb    r9, rax
  sbb    r10, rax
  sbb    r11, rax
  mov    rax, [rip+fmt(p610x2)+32]
  sbb    r12, rax
  mov    rax, [rip+fmt(p610x2)+40]
  sbb    r13, rax
  mov    rax, [rip+fmt(p610x2)+48]
  sbb    r14, rax
  mov    rax, [rip+fmt(p610x2)+56]
  sbb    r15, rax
  mov    rax, [rip+fmt(p610x2)+64]
  sbb    rcx, rax
  mov    rax, [rip+fmt(p610x2)+72]
  sbb    rdi, rax
  mov    [reg_p3+64], rcx
  mov    [reg_p3+72], rdi
  mov    rax, 0
  sbb    rax, 0
  
  mov    rsi, [rip+fmt(p610x2)]
  and    rsi, rax
  mov    rdi, [rip+fmt(p610x2)+8]
  and    rdi, rax
  
  add    r8, rsi  
  adc    r9, rdi 
  adc    r10, rdi 
  adc    r11, rdi
  mov    [reg_p3], r8 
  mov    [reg_p3+8], r9 
  mov    [reg_p3+16], r10 
  mov    [reg_p3+24], r11 
  setc   cl
  
  mov    rdi, [rip+fmt(p610x2)+32]
  and    rdi, rax
  mov    rsi, [rip+fmt(p610x2)+40]
  and    rsi, rax
  mov    r8, [rip+fmt(p610x2)+48]
  and    r8, rax
  mov    r9, [rip+fmt(p610x2)+56]
  and    r9, rax
  mov    r10, [rip+fmt(p610x2)+64]
  and    r10, rax
  mov    r11, [rip+fmt(p610x2)+72]
  and    r11, rax
  
  bt     rcx, 0
  adc    r12, rdi
  adc    r13, rsi  
  adc    r14, r8
  adc    r15, r9
  mov    rsi, [reg_p3+64]
  mov    rdi, [reg_p3+72]
  adc    rsi, r10  
  adc    rdi, r11
  mov    [reg_p3+32], r12  
  mov    [reg_p3+40], r13
  mov    [reg_p3+48], r14
  mov    [reg_p3+56], r15  
  mov    [reg_p3+64], rsi
  mov    [reg_p3+72], rdi

  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret


//***********************************************************************
//  Field subtraction
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2]
//*********************************************************************** 
.global fmt(fpsub610_asm)
fmt(fpsub610_asm):
  push   r12
  push   r13
  push   r14
  push   r15
  
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  mov    r12, [reg_p1+32]
  mov    r13, [reg_p1+40]
  mov    r14, [reg_p1+48]
  mov    r15, [reg_p1+56] 
  mov    rcx, [reg_p1+64] 
  mov    rdi, [reg_p1+72]
  sub    r8, [reg_p2] 
  sbb    r9, [reg_p2+8] 
  sbb    r10, [reg_p2+16] 
  sbb    r11, [reg_p2+24] 
  sbb    r12, [reg_p2+32] 
  sbb    r13, [reg_p2+40] 
  sbb    r14, [reg_p2+48] 
  sbb    r15, [reg_p2+56]
  sbb    rcx, [reg_p2+64]
  sbb    rdi, [reg_p2+72]
  mov    [reg_p3+64], rcx
  mov    [reg_p3+72], rdi
  mov    rax, 0
  sbb    rax, 0
    
  mov    rsi, [rip+fmt(p610x2)]
  and    rsi, rax
  mov    rdi, [rip+fmt(p610x2)+8]
  and    rdi, rax
  
  add    r8, rsi  
  adc    r9, rdi 
  adc    r10, rdi 
  adc    r11, rdi
  mov    [reg_p3], r8 
  mov    [reg_p3+8], r9 
  mov    [reg_p3+16], r10 
  mov    [reg_p3+24], r11 
  setc   cl
  
  mov    rdi, [rip+fmt(p610x2)+32]
  and    rdi, rax
  mov    rsi, [rip+fmt(p610x2)+40]
  and    rsi, rax
  mov    r8, [rip+fmt(p610x2)+48]
  and    r8, rax
  mov    r9, [rip+fmt(p610x2)+56]
  and    r9, rax
  mov    r10, [rip+fmt(p610x2)+64]
  and    r10, rax
  mov    r11, [rip+fmt(p610x2)+72]
  and    r11, rax
  
  bt     rcx, 0
  adc    r12, rdi
  adc    r13, rsi  
  adc    r14, r8
  adc    r15, r9
  mov    rsi, [reg_p3+64]
  mov    rdi, [reg_p3+72]
  adc    rsi, r10  
  adc    rdi, r11
  mov    [reg_p3+32], r12  
  mov    [reg_p3+40], r13
  mov    [reg_p3+48], r14
  mov    [reg_p3+56], r15  
  mov    [reg_p3+64], rsi
  mov    [reg_p3+72], rdi
  
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret 


///////////////////////////////////////////////////////////////// MACRO
.macro SUB610_PX  P0
  push   r12
  push   r13
  push   r14
  push   r15
  
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  mov    r12, [reg_p1+32]
  mov    r13, [reg_p1+40]
  mov    r14, [reg_p1+48]
  mov    r15, [reg_p1+56]
  mov    rax, [reg_p1+64]
  mov    rcx, [reg_p1+72]
  sub    r8, [reg_p2] 
  sbb    r9, [reg_p2+8] 
  sbb    r10, [reg_p2+16] 
  sbb    r11, [reg_p2+24] 
  sbb    r12, [reg_p2+32] 
  sbb    r13, [reg_p2+40]
  sbb    r14, [reg_p2+48] 
  sbb    r15, [reg_p2+56] 
  sbb    rax, [reg_p2+64] 
  sbb    rcx, [reg_p2+72] 

  mov    rdi, [rip+\P0]
  mov    rsi, [rip+\P0+8]
  add    r8, rdi  
  adc    r9, rsi  
  adc    r10, rsi 
  adc    r11, rsi 
  mov    rdi, [rip+\P0+32]
  mov    rsi, [rip+\P0+40]
  adc    r12, rdi   
  adc    r13, rsi   
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9 
  mov    [reg_p3+16], r10 
  mov    [reg_p3+24], r11
  mov    [reg_p3+32], r12 
  mov    [reg_p3+40], r13
  mov    rdi, [rip+\P0+48]
  mov    rsi, [rip+\P0+56]
  adc    r14, rdi  
  adc    r15, rsi  
  mov    rdi, [rip+\P0+64]
  mov    rsi, [rip+\P0+72]
  adc    rax, rdi 
  adc    rcx, rsi  
  mov    [reg_p3+48], r14
  mov    [reg_p3+56], r15 
  mov    [reg_p3+64], rax 
  mov    [reg_p3+72], rcx
  
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  .endm


//***********************************************************************
//  Multiprecision subtraction with correction with 2*p610
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 2*p610
//*********************************************************************** 
.global fmt(mp_sub610_p2_asm)
fmt(mp_sub610_p2_asm):

  SUB610_PX  fmt(p610x2)
  ret


#ifdef _MULX_
#ifdef _ADX_

///////////////////////////////////////////////////////////////// MACRO
// z = a x bi + z
// Inputs: base memory pointer M1 (a),
//         bi pre-stored in rdx,
//         accumulator z in [M0:M2, Z3:Z10]
// Output: [M0:M2, Z3:Z10]
// Temps:  regs T0:T1
/////////////////////////////////////////////////////////////////
.macro MULADD64x640 M1, M, Z3, Z4, Z5, Z6, Z7, Z8, Z9, Z10, T0, T1, T2, C
    mulx   \T0, \T1, \M1     // A0*B0
	xor    \C, \C
    adox   \T1, \M
    adox   \T0, 8\M  
	mov    \M, \T1
    mulx   \T1, \T2, 8\M1    // A0*B1
    adcx   \T0, \T2
    adox   \T1, 16\M  
	mov    8\M, \T0   
    mulx   \T0, \T2, 16\M1   // A0*B2
    adcx   \T1, \T2
    adox   \Z3, \T0 
	mov    16\M, \T1  
    mulx   \T0, \T1, 24\M1   // A0*B3          
    adcx   \Z3, \T1
    adox   \Z4, \T0
    mulx   \T0, \T1, 32\M1   // A0*B4          
    adcx   \Z4, \T1
    adox   \Z5, \T0
    mulx   \T0, \T1, 40\M1   // A0*B5          
    adcx   \Z5, \T1
    adox   \Z6, \T0
    mulx   \T0, \T1, 48\M1   // A0*B6               
    adcx   \Z6, \T1
    adox   \Z7, \T0
    mulx   \T0, \T1, 56\M1   // A0*B7         
    adcx   \Z7, \T1
    adox   \Z8, \T0
    mulx   \T0, \T1, 64\M1   // A0*B8         
    adcx   \Z8, \T1
    adox   \Z9, \T0
    mulx   \T0, \T1, 72\M1   // A0*B9         
    adcx   \Z9, \T1
    adox   \Z10, \T0
    adc    \Z10, 0 
.endm


.macro MULADD64x640b M1, M, MM, Z3, Z4, Z5, Z6, Z7, Z8, Z9, Z10, T0, T1, T2, C
    mulx   \T0, \T1, \M1     // A0*B0
	xor    \C, \C
    adox   \T1, \M
    adox   \T0, 8\M  
	mov    24\M, \T1
    mulx   \T1, \T2, 8\M1    // A0*B1
    adcx   \T0, \T2
    adox   \T1, 16\M  
	mov    \MM, \T0   
    mulx   \T0, \T2, 16\M1   // A0*B2
    adcx   \T1, \T2
    adox   \Z3, \T0 
	mov    8\MM, \T1  
    mulx   \T0, \T1, 24\M1   // A0*B3          
    adcx   \Z3, \T1
    adox   \Z4, \T0
    mulx   \T0, \T1, 32\M1   // A0*B4    
	mov    16\MM, \Z3        
    adcx   \Z4, \T1
    adox   \Z5, \T0
    mulx   \T0, \T1, 40\M1   // A0*B5          
    adcx   \Z5, \T1
    adox   \Z6, \T0
    mulx   \T0, \T1, 48\M1   // A0*B6               
    adcx   \Z6, \T1
    adox   \Z7, \T0
    mulx   \T0, \T1, 56\M1   // A0*B7         
    adcx   \Z7, \T1
    adox   \Z8, \T0
    mulx   \T0, \T1, 64\M1   // A0*B8         
    adcx   \Z8, \T1
    adox   \Z9, \T0
    mulx   \T0, \T1, 72\M1   // A0*B9         
    adcx   \Z9, \T1
    adox   \Z10, \T0
    adc    \Z10, 0 
.endm


.macro MULADD64x384 M1, Z0, Z1, Z2, Z3, Z4, Z5, Z6, T0, T1
    mulx   \T0, \T1, \M1     // A0*B0
	xor    rax, rax
    adox   \Z0, \T1
    adox   \Z1, \T0  
    mulx   \T0, \T1, 8\M1    // A0*B1
    adcx   \Z1, \T1
    adox   \Z2, \T0    
    mulx   \T0, \T1, 16\M1   // A0*B2
    adcx   \Z2, \T1
    adox   \Z3, \T0
    mulx   \T0, \T1, 24\M1   // A0*B3          
    adcx   \Z3, \T1
    adox   \Z4, \T0
    mulx   \T0, \T1, 32\M1   // A0*B4 
    adcx   \Z4, \T1
    adox   \Z5, \T0
    mulx   \T0, \T1, 40\M1   // A0*B5 
    adcx   \Z5, \T1
    adox   \Z6, \T0
    adc    \Z6, 0    
.endm


///////////////////////////////////////////////////////////////// MACRO
// z = a x b + c x d (mod p)
// Inputs: base memory pointers M0 (a,c), M1 (b,d)
//         bi pre-stored in rdx,
//         accumulator z in [Z0:Z7], pre-stores a0 x b
// Output: [Z0:Z7]
// Temps:  MM, regs T0:T2
/////////////////////////////////////////////////////////////////
.macro FPDBLMUL640x640 M00, M01, M10, M11, MM, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, T0, T1, T2
    mov    rdx, \M11    
    MULADD64x640b \M01, \MM, \MM, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \T0, \T1, \T2, \T2        
    // [MM0:MM16, Z1:Z7] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                  // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \T0, \T1

    // [MM0:MM16, Z1:Z5, Z0] <- z = a0 x b01 - a1 x b11 + z 
    mov    rdx, 8\M10
    MULADD64x640 \M00, \MM, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \T0, \T1, \T2, \Z0
    mov    rdx, 8\M11    
    MULADD64x640b \M01, \MM, \MM, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z2:Z7, Z0] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \T0, \T1

    // [MM0:MM16, Z2:Z7, Z0:Z1] <- z = a0 x b02 + a1 x b12 + z 
    mov    rdx, 16\M10
    MULADD64x640 \M00, \MM, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \T0, \T1, \T2, \Z1
    mov    rdx, 16\M11    
    MULADD64x640b \M01, \MM, \MM, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z3:Z7, Z0:Z1] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \T0, \T1

    // [MM0:MM16, Z3:Z7, Z0:Z2] <- z = a0 x b03 + a1 x b13 + z 
    mov    rdx, 24\M10
    MULADD64x640 \M00, \MM, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \T0, \T1, \T2, \Z2
    mov    rdx, 24\M11    
    MULADD64x640b \M01, \MM, \MM, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z4:Z7, Z0:Z2] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \T0, \T1

    // [MM0:MM16, Z4:Z7, Z0:Z3] <- z = a0 x b04 + a1 x b14 + z 
    mov    rdx, 32\M10
    MULADD64x640 \M00, \MM, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \T2, \Z3
    mov    rdx, 32\M11    
    MULADD64x640b \M01, \MM, \MM, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z5:Z7, Z0:Z3] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \T0, \T1

    // [MM0:MM16, Z5:Z7, Z0:Z4] <- z = a0 x b05 + a1 x b15 + z 
    mov    rdx, 40\M10
    MULADD64x640 \M00, \MM, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1, \T2, \Z4
    mov    rdx, 40\M11    
    MULADD64x640b \M01, \MM, \MM, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z6:Z7, Z0:Z4] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1

    // [MM0:MM16, Z6:Z7, Z0:Z5] <- z = a0 x b06 + a1 x b16 + z 
    mov    rdx, 48\M10
    MULADD64x640 \M00, \MM, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1, \T2, \Z5
    mov    rdx, 48\M11    
    MULADD64x640b \M01, \MM, \MM, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z7, Z0:Z5] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1

    // [MM0:MM16, Z7, Z0:Z6] <- z = a0 x b07 + a1 x b17 + z 
    mov    rdx, 56\M10
    MULADD64x640 \M00, \MM, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1, \T2, \Z6
    mov    rdx, 56\M11    
    MULADD64x640b \M01, \MM, \MM, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z0:Z6] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1

    // [MM0:MM16, Z0:Z7] <- z = a0 x b08 + a1 x b18 + z 
    mov    rdx, 64\M10
    MULADD64x640 \M00, \MM, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \T0, \T1, \T2, \Z7
    mov    rdx, 64\M11    
    MULADD64x640b \M01, \MM, \MM, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z1:Z7] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \T0, \T1

    // [MM0:MM16, Z1:Z7, Z0] <- z = a0 x b09 + a1 x b19 + z 
    mov    rdx, 72\M10
    MULADD64x640 \M00, \MM, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \T0, \T1, \T2, \Z0
    mov    rdx, 72\M11    
    MULADD64x640b \M01, \MM, [rcx], \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \T0, \T1, \T2, \T2
    // [MM0:MM16, Z2:Z7, Z0] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \T0, \T1
.endm


//***********************************************************************
//  Multiplication in GF(p^2), non-complex part
//  Operation: c [reg_p3] = a0 x b0 - a1 x b1
//  Inputs: a = [a1, a0] stored in [reg_p1] 
//          b = [b1, b0] stored in [reg_p2] 
//  Output: c stored in [reg_p3]
//***********************************************************************
.global fmt(fp2mul610_c0_asm)
fmt(fp2mul610_c0_asm):      
    push   r12 
    push   r13 
    push   r14 
    push   r15     
    push   rbx   
    push   rbp  
	sub    rsp, 80   
    mov    rcx, reg_p3
	
	// [rsp0:rsp72] <- 8*p - b1
	mov    r8, [rip+fmt(p610x8)]  
	mov    r9, [rip+fmt(p610x8)+8]   
	mov    r12, [rip+fmt(p610x8)+32]
	mov    r13, [rip+fmt(p610x8)+40] 
	mov    rax, [reg_p2+80]
	mov    rdx, [reg_p2+88]
	mov    r10, r9 
	mov    r11, r9                         
	sub    r8, rax
	sbb    r9, rdx
	mov    rax, [reg_p2+96]
	mov    rdx, [reg_p2+104]
	sbb    r10, rax
	sbb    r11, rdx
	mov    rax, [reg_p2+112]
	mov    rdx, [reg_p2+120]
	sbb    r12, rax
	sbb    r13, rdx
	mov    [rsp], r8
	mov    [rsp+8], r9
	mov    [rsp+16], r10
	mov    [rsp+24], r11 
	mov    [rsp+32], r12  
	mov    [rsp+40], r13
	mov    r8, [rip+fmt(p610x8)+48]
	mov    r9, [rip+fmt(p610x8)+56]
	mov    r10, [rip+fmt(p610x8)+64]
	mov    r11, [rip+fmt(p610x8)+72]
	mov    rax, [reg_p2+128]
	mov    rdx, [reg_p2+136]
	sbb    r8, rax 
	sbb    r9, rdx 
	mov    rax, [reg_p2+144]
	mov    rdx, [reg_p2+152]
	sbb    r10, rax 
	sbb    r11, rdx 
	mov    [rsp+48], r8
	mov    [rsp+56], r9
	mov    [rsp+64], r10
	mov    [rsp+72], r11
    
    // [rcx0:rcx16, r11:r15, r8:r10] <- z = a0 x b00 - a1 x b10
    mov    rdx, [reg_p2]
    mulx   r9, r8, [reg_p1] 
    xor    rax, rax 
    mulx   r10, r11, [reg_p1+8] 
	mov    [rcx], r8
    adcx   r9, r11 
    mulx   r11, r12, [reg_p1+16]
	mov    [rcx+8], r9  
    adcx   r10, r12         
    mulx   r12, r13, [reg_p1+24] 
	mov    [rcx+16], r10  
    adcx   r11, r13       
    mulx   r13, r8, [reg_p1+32] 
    adcx   r12, r8      
    mulx   r14, r9, [reg_p1+40]
    adcx   r13, r9      
    mulx   r15, rax, [reg_p1+48]
    adcx   r14, rax     
    mulx   r8, r10, [reg_p1+56]
    adcx   r15, r10     
    mulx   r9, rax, [reg_p1+64]
    adcx   r8, rax    
    mulx   r10, rbx, [reg_p1+72] 
    adcx   r9, rbx    
    adc    r10, 0 

	FPDBLMUL640x640 [reg_p1], [reg_p1+80], [reg_p2], [rsp], [rcx], r11, r12, r13, r14, r15, r8, r9, r10, rbx, rbp, rax
         
    mov    [rcx+24], r13 
    mov    [rcx+32], r14   
    mov    [rcx+40], r15 
    mov    [rcx+48], r8   
    mov    [rcx+56], r9      
    mov    [rcx+64], r10                  
    mov    [rcx+72], r11
	add    rsp, 80
    pop    rbp
    pop    rbx
    pop    r15 
    pop    r14
    pop    r13
    pop    r12
    ret


//***********************************************************************
//  Multiplication in GF(p^2), complex part
//  Operation: c [reg_p3] = a0 x b1 + a1 x b0
//  Inputs: a = [a1, a0] stored in [reg_p1] 
//          b = [b1, b0] stored in [reg_p2] 
//  Output: c stored in [reg_p3]
//***********************************************************************
.global fmt(fp2mul610_c1_asm)
fmt(fp2mul610_c1_asm): 
    push   r12    
    push   r13     
    push   r14   
    push   r15        
    push   rbx    
    push   rbp 
	sub    rsp, 32 
    mov    rcx, reg_p3
    
    // [rsp0:rsp16, r11:r15, r8:r10] <- z = a0 x b10 + a1 x b00
    mov    rdx, [reg_p2]
    mulx   r9, r8, [reg_p1+80] 
    xor    rax, rax 
    mulx   r10, r11, [reg_p1+88] 
	mov    [rsp], r8
    adcx   r9, r11 
    mulx   r11, r12, [reg_p1+96]
	mov    [rsp+8], r9  
    adcx   r10, r12         
    mulx   r12, r13, [reg_p1+104] 
	mov    [rsp+16], r10  
    adcx   r11, r13       
    mulx   r13, r8, [reg_p1+112] 
    adcx   r12, r8      
    mulx   r14, r9, [reg_p1+120]
    adcx   r13, r9      
    mulx   r15, rax, [reg_p1+128]
    adcx   r14, rax     
    mulx   r8, r10, [reg_p1+136] 
    adcx   r15, r10     
    mulx   r9, rax, [reg_p1+144]
    adcx   r8, rax    
    mulx   r10, rbx, [reg_p1+152] 
    adcx   r9, rbx    
    adc    r10, 0  

	FPDBLMUL640x640 [reg_p1+80], [reg_p1], [reg_p2], [reg_p2+80], [rsp], r11, r12, r13, r14, r15, r8, r9, r10, rbx, rbp, rax
         
    mov    [rcx+24], r13 
    mov    [rcx+32], r14   
    mov    [rcx+40], r15 
    mov    [rcx+48], r8   
    mov    [rcx+56], r9      
    mov    [rcx+64], r10                  
    mov    [rcx+72], r11
	add    rsp, 32
    pop    rbp
    pop    rbx
    pop    r15 
    pop    r14
    pop    r13
    pop    r12
    ret

#else

# error "CONFIGURATION NOT SUPPORTED. TRY USE_MULX=TRUE USE_ADX=TRUE"

#endif

#else

# error "CONFIGURATION NOT SUPPORTED. TRY USE_MULX=TRUE USE_ADX=TRUE"

#endif


///////////////////////////////////////////////////////////////// MACRO
// z = z + a x b
// Inputs: base memory pointers M0 (a), M1 (b)
//         bi pre-stored in rdx,
//         accumulator z in [Z0:Z7], pre-stores a0 x b
// Output: [Z0:Z7] and OUT
// Temps:  regs T0:T2
/////////////////////////////////////////////////////////////////
.macro FPMUL640x640 M0, M1, MM, OUT, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, T0, T1, T2                   
    // [Z4:Z7, Z0:Z2] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                  // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \T0, \T1

    // [rsp8:rsp24, \Z4:\Z7, \Z0:\Z3] <- z = a0 x b11 + a1 x b01 + z 
    mov    rdx, 8\M0
    MULADD64x640b \M1, \MM, \MM, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \T2, \Z3
    // [\Z5:\Z7, \Z0:\Z3] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \T0, \T1

    // [rsp16:rsp32, \Z5:\Z7, \Z0:\Z4] <- z = a0 x b12 + a1 x b02 + z 
    mov    rdx, 16\M0
    MULADD64x640b \M1, \MM, \MM, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1, \T2, \Z4
    // [rsp24:rsp40, \Z6:\Z7, \Z0:\Z4] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1

    // [rsp24:rsp40, \Z6:\Z7, \Z0:\Z5] <- z = a0 x b13 + a1 x b03 + z 
    mov    rdx, 24\M0
    MULADD64x640b \M1, \MM, \MM, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1, \T2, \Z5
    // [rsp32:rsp48, \Z7, \Z0:\Z5] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1

    // [rsp32:rsp48, \Z7, \Z0:\Z6] <- z = a0 x b14 + a1 x b04 + z 
    mov    rdx, 32\M0
    MULADD64x640b \M1, \MM, \MM, \Z7, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1, \T2, \Z6
    // [rsp40:rsp56, \Z0:\Z6] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1

    // [rsp40:rsp56, \Z0:\Z7] <- z = a0 x b15 + a1 x b05 + z 
    mov    rdx, 40\M0
    MULADD64x640b \M1, \MM, \MM, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \T0, \T1, \T2, \Z7
    // [rsp48:rsp64, \Z1:\Z7] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \T0, \T1

    // [rsp48:rsp64, \Z1:\Z7, \Z0] <- z = a0 x b16 + a1 x b06 + z 
    mov    rdx, 48\M0
    MULADD64x640b \M1, \MM, \MM, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \T0, \T1, \T2, \Z0
    // [rsp56:rsp72, \Z2:\Z7, \Z0] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \T0, \T1

    // [rsp56:rsp72, \Z2:\Z7, \Z0:\Z1] <- z = a0 x b17 + a1 x b07 + z 
    mov    rdx, 56\M0
    MULADD64x640b \M1, \MM, \MM, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \T0, \T1, \T2, \Z1
    // [rsp64:rsp80, \Z3:\Z7, \Z0:\Z1] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \T0, \T1

    // [rsp64:rsp80, \Z3:\Z7, \Z0:\Z2] <- z = a0 x b18 + a1 x b08 + z 
    mov    rdx, 64\M0
    MULADD64x640b \M1, \MM, \MM, \Z3, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \T0, \T1, \T2, \Z2
    // [rsp72:rsp88, \Z4:\Z7, \Z0:\Z2] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \T0, \T1

    // [rsi0:rsi16, \Z4:\Z7, \Z0:\Z3] <- z = a0 x b19 + a1 x b09 + z 
    mov    rdx, 72\M0
    MULADD64x640b \M1, \MM, \OUT, \Z4, \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \T2, \Z3
    // [rsp80:rsp96, \Z5:\Z7, \Z0:\Z3] <- z = (z0 x p610p1 + z)/2^64
    mov    rdx, 24\MM                 // rdx <- z0
    MULADD64x384 [rip+fmt(p610p1)+32], \Z5, \Z6, \Z7, \Z0, \Z1, \Z2, \Z3, \T0, \T1
.endm


//***********************************************************************
//  Squaring in GF(p^2), non-complex part
//  Operation: c [reg_p2] = (a0+a1) x (a0-a1)
//  Inputs: a = [a1, a0] stored in [reg_p1] 
//  Output: c stored in [reg_p2]
//***********************************************************************
.global fmt(fp2sqr610_c0_asm)
fmt(fp2sqr610_c0_asm):  
    push   r12
    push   r13
    push   r14  
    push   r15  
    push   rbx  
    push   rbp
	sub    rsp, 32

	// a0 + a1
	mov    r8, [reg_p1]
	mov    r9, [reg_p1+8]
	mov    r10, [reg_p1+16]
	mov    r11, [reg_p1+24]
	mov    r12, [reg_p1+32]
	mov    r13, [reg_p1+40]
	add    r8, [reg_p1+80]
	adc    r9, [reg_p1+88] 
	mov    [reg_p2], r8
	adc    r10, [reg_p1+96]
	adc    r11, [reg_p1+104]
	mov    [reg_p2+8], r9
	mov    [reg_p2+16], r10
	adc    r12, [reg_p1+112]
	adc    r13, [reg_p1+120]
	mov    r14, [reg_p1+48]
	mov    r15, [reg_p1+56]
	adc    r14, [reg_p1+128]
	adc    r15, [reg_p1+136]
	mov    r9, [reg_p1+64]
	mov    r10, [reg_p1+72]
	adc    r9, [reg_p1+144]
	adc    r10, [reg_p1+152]
	mov    [reg_p2+24], r11
	mov    [reg_p2+32], r12
	mov    [reg_p2+40], r13
	mov    [reg_p2+48], r14
	mov    [reg_p2+56], r15
	mov    [reg_p2+64], r9
	mov    [reg_p2+72], r10
	
	// a0 - a1 + 4xp610
	mov    rcx, [reg_p1]
	mov    r10, [reg_p1+8]
	mov    r12, [reg_p1+16]
	mov    r13, [reg_p1+24]
	mov    r14, [reg_p1+32]
	mov    r15, [reg_p1+40]
	sub    rcx, [reg_p1+80]
	sbb    r10, [reg_p1+88]
	sbb    r12, [reg_p1+96]
	sbb    r13, [reg_p1+104] 
	sbb    r14, [reg_p1+112]
	sbb    r15, [reg_p1+120]
	mov    rbx, [reg_p1+48]
	mov    rbp, [reg_p1+56]
	mov    r8, [reg_p1+64]
	mov    rax, [reg_p1+72]
	sbb    rbx, [reg_p1+128]
	sbb    rbp, [reg_p1+136]
	sbb    r8, [reg_p1+144]
	sbb    rax, [reg_p1+152]
	add    rcx, [rip+fmt(p610x4)]	                  
	mov    rdx, [rip+fmt(p610x4)+8]
	adc    r10, rdx
	adc    r12, rdx
	adc    r13, rdx
	adc    r14, [rip+fmt(p610x4)+32]
	adc    r15, [rip+fmt(p610x4)+40]
	adc    rbx, [rip+fmt(p610x4)+48]
	adc    rbp, [rip+fmt(p610x4)+56]
	adc    r8, [rip+fmt(p610x4)+64]
	adc    rax, [rip+fmt(p610x4)+72]
	mov    [reg_p2+80], rcx                 
	mov    [reg_p2+88], r10
	mov    [reg_p2+96], r12 
	mov    [reg_p2+104], r13 
	mov    [reg_p2+112], r14 
	mov    [reg_p2+144], r8 
	mov    [reg_p2+152], rax
    
    // [rsp0:rsp16, r11:r15, r8:r10] <- z = a00 x a1
    mov    rdx, [reg_p2]
    mulx   r9, r8, rcx
	mov    [reg_p2+120], r15
    xor    rax, rax 
    mulx   r10, r11, r10 
	mov    [reg_p2+128], rbx
    adcx   r9, r11     
    mulx   r11, r12, r12 
	mov    [reg_p2+136], rbp 
    adcx   r10, r12       
    mulx   r12, r13, r13 
	mov    [rsp+24], r8
    adcx   r11, r13      
    mulx   r13, r8, r14  
	mov    [rsp], r9 
    adcx   r12, r8      
    mulx   r14, r9, r15 
	mov    [rsp+8], r10  
    adcx   r13, r9      
    mulx   r15, rax, rbx 
	mov    [rsp+16], r11  
    adcx   r14, rax     
    mulx   r8, r10, rbp  
    adcx   r15, r10     
    mulx   r9, rax, [reg_p2+144] 
    adcx   r8, rax    
    mulx   r10, rbx, [reg_p2+152] 
    adcx   r9, rbx    
    adc    r10, 0 
           
	FPMUL640x640 [reg_p2], [reg_p2+80], [rsp], [reg_p2], r8, r9, r10, r11, r12, r13, r14, r15, rbx, rbp, rax
           
    mov    [reg_p2+24], r13   
    mov    [reg_p2+32], r14 
    mov    [reg_p2+40], r15  
    mov    [reg_p2+48], r8      
    mov    [reg_p2+56], r9                  
    mov    [reg_p2+64], r10                 
    mov    [reg_p2+72], r11
	add    rsp, 32
    pop    rbp
    pop    rbx
    pop    r15 
    pop    r14
    pop    r13
    pop    r12
    ret

//***********************************************************************
//  Squaring in GF(p^2), complex part
//  Operation: c [reg_p2] = 2a0 x a1
//  Inputs: a = [a1, a0] stored in [reg_p1] 
//  Output: c stored in [reg_p2]
//***********************************************************************
.global fmt(fp2sqr610_c1_asm)
fmt(fp2sqr610_c1_asm):  
    push   r12     
    push   r13     
    push   r14 
    push   r15 
	
	mov    r8, [reg_p1]
	mov    r9, [reg_p1+8]
	mov    r10, [reg_p1+16]
	mov    r11, [reg_p1+24]
	mov    r12, [reg_p1+32]
	mov    r13, [reg_p1+40] 
	add    r8, r8
	adc    r9, r9
    push   rbx 
	adc    r10, r10
	adc    r11, r11  
    push   rbp
	adc    r12, r12
	adc    r13, r13
	mov    r14, [reg_p1+48] 
	mov    r15, [reg_p1+56] 
	adc    r14, r14
	adc    r15, r15
	mov    rbx, [reg_p1+64] 
	mov    rbp, [reg_p1+72] 
	adc    rbx, rbx
	adc    rbp, rbp
	sub    rsp, 112
	mov    [rsp+8], r9
	mov    [rsp+16], r10
	mov    [rsp+24], r11
    
    // [rsp24, rsp0:rsp16, r11:r15, r8:r10] <- z = a00 x a1
    mov    rdx, r8
    mulx   r9, r8, [reg_p1+80]
	mov    [rsp+32], r12
    xor    rax, rax
    mulx   r10, r11, [reg_p1+88] 
	mov    [rsp+40], r13
    adcx   r9, r11     
    mulx   r11, r12, [reg_p1+96]
	mov    [rsp+48], r14
    adcx   r10, r12         
    mulx   r12, r13, [reg_p1+104] 
	mov    [rsp+104], r8
    adcx   r11, r13         
    mulx   r13, r8, [reg_p1+112]
	mov    [rsp+80], r9   
    adcx   r12, r8      
    mulx   r14, r9, [reg_p1+120]
	mov    [rsp+56], r15
    adcx   r13, r9      
    mulx   r15, rax, [reg_p1+128]
	mov    [rsp+88], r10  
    adcx   r14, rax     
    mulx   r8, r10, [reg_p1+136] 
	mov    [rsp+96], r11 
    adcx   r15, r10     
    mulx   r9, rax, [reg_p1+144] 
	mov    [rsp+64], rbx
    adcx   r8, rax    
    mulx   r10, rbx, [reg_p1+152] 
	mov    [rsp+72], rbp
    adcx   r9, rbx    
    adc    r10, 0  
           
	FPMUL640x640 [rsp], [reg_p1+80], [rsp+80], [reg_p2], r8, r9, r10, r11, r12, r13, r14, r15, rbx, rbp, rax
         
    mov    [reg_p2+24], r13   
    mov    [reg_p2+32], r14 
    mov    [reg_p2+40], r15  
    mov    [reg_p2+48], r8      
    mov    [reg_p2+56], r9                  
    mov    [reg_p2+64], r10                 
    mov    [reg_p2+72], r11
	add    rsp, 112
    pop    rbp
    pop    rbx
    pop    r15 
    pop    r14
    pop    r13
    pop    r12
    ret


//***********************************************************************
//  Field multiplication in GF(p)
//  Operation: c = a x b mod p
//  Inputs: a stored in [reg_p1], b stored in [reg_p2] 
//  Output: c stored in [reg_p3]
//***********************************************************************
.global fmt(fpmul610_asm)
fmt(fpmul610_asm): 
    mov    rcx, reg_p3  
    push   r12
    push   r13 
    push   r14 
    push   r15 
    push   rbx    
    push   rbp
	sub    rsp, 32
     
    // [r8:r15] <- z = a x b0
    mov    rdx, [reg_p2]
    mulx   r9, r8, [reg_p1]
    xor    rax, rax 
	mov    [rsp+24], r8
    mulx   r10, r11, [reg_p1+8]
    adcx   r9, r11        
    mulx   r11, r12, [reg_p1+16]
    adcx   r10, r12        
    mulx   r12, r13, [reg_p1+24]
    adcx   r11, r13       
    mulx   r13, r8, [reg_p1+32]
    adcx   r12, r8      
    mulx   r14, rax, [reg_p1+40]
    adcx   r13, rax     
    mulx   r15, rax, [reg_p1+48]   
	mov    [rsp], r9
    adcx   r14, rax      
    mulx   r8, rbx, [reg_p1+56]  
	mov    [rsp+8], r10    
    adcx   r15, rbx     
    mulx   r9, rax, [reg_p1+64] 
	mov    [rsp+16], r11    
    adcx   r8, rax     
    mulx   r10, rbx, [reg_p1+72]  
    adcx   r9, rbx 
    adc    r10, 0 

	FPMUL640x640 [reg_p2], [reg_p1], [rsp], [rcx], r8, r9, r10, r11, r12, r13, r14, r15, rbx, rbp, rax
	
    mov    [rcx+24], r13   
    mov    [rcx+32], r14 
    mov    [rcx+40], r15  
    mov    [rcx+48], r8      
    mov    [rcx+56], r9                  
    mov    [rcx+64], r10                 
    mov    [rcx+72], r11
	add    rsp, 32
    pop    rbp
    pop    rbx
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    ret

