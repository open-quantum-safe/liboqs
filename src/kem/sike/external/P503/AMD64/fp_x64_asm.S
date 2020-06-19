//*******************************************************************************************
// SIDH: an efficient supersingular isogeny cryptography library
//
// Abstract: field arithmetic in x64 assembly for P503 on Linux
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

// Define addition instructions
#ifdef _MULX_
#ifdef _ADX_

#define ADD1    adox
#define ADC1    adox
#define ADD2    adcx
#define ADC2    adcx

#else

#define ADD1    add
#define ADC1    adc
#define ADD2    add
#define ADC2    adc

#endif   
#endif


.text
//***********************************************************************
//  Field addition
//  Operation: c [reg_p3] = a [reg_p1] + b [reg_p2]
//*********************************************************************** 
.global fmt(fpadd503_asm)
fmt(fpadd503_asm):
  push   r12
  push   r13
  push   r14
  push   r15
  
  xor    rax, rax
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  mov    r12, [reg_p1+32]
  mov    r13, [reg_p1+40]
  mov    r14, [reg_p1+48]
  mov    r15, [reg_p1+56] 
  add    r8, [reg_p2] 
  adc    r9, [reg_p2+8] 
  adc    r10, [reg_p2+16] 
  adc    r11, [reg_p2+24] 
  adc    r12, [reg_p2+32] 
  adc    r13, [reg_p2+40] 
  adc    r14, [reg_p2+48] 
  adc    r15, [reg_p2+56]

  mov    rcx, [rip+fmt(p503x2)]
  sub    r8, rcx
  mov    rcx, [rip+fmt(p503x2)+8]
  sbb    r9, rcx
  sbb    r10, rcx
  mov    rcx, [rip+fmt(p503x2)+24]
  sbb    r11, rcx
  mov    rcx, [rip+fmt(p503x2)+32]
  sbb    r12, rcx
  mov    rcx, [rip+fmt(p503x2)+40]
  sbb    r13, rcx
  mov    rcx, [rip+fmt(p503x2)+48]
  sbb    r14, rcx
  mov    rcx, [rip+fmt(p503x2)+56]
  sbb    r15, rcx
  sbb    rax, 0
  
  mov    rdi, [rip+fmt(p503x2)]
  and    rdi, rax
  mov    rsi, [rip+fmt(p503x2)+8]
  and    rsi, rax
  mov    rcx, [rip+fmt(p503x2)+24]
  and    rcx, rax
  
  add    r8, rdi  
  adc    r9, rsi  
  adc    r10, rsi 
  adc    r11, rcx 
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9 
  mov    [reg_p3+16], r10 
  mov    [reg_p3+24], r11 
  setc   cl

  mov    r8, [rip+fmt(p503x2)+32]
  and    r8, rax
  mov    r9, [rip+fmt(p503x2)+40]
  and    r9, rax
  mov    r10, [rip+fmt(p503x2)+48]
  and    r10, rax
  mov    r11, [rip+fmt(p503x2)+56]
  and    r11, rax
  
  bt     rcx, 0
  adc    r12, r8   
  adc    r13, r9  
  adc    r14, r10  
  adc    r15, r11  
  mov    [reg_p3+32], r12 
  mov    [reg_p3+40], r13 
  mov    [reg_p3+48], r14 
  mov    [reg_p3+56], r15 
  
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret


//***********************************************************************
//  Field subtraction
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2]
//*********************************************************************** 
.global fmt(fpsub503_asm)
fmt(fpsub503_asm):
  push   r12
  push   r13
  push   r14
  push   r15
  
  xor    rax, rax
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  mov    r12, [reg_p1+32]
  mov    r13, [reg_p1+40]
  mov    r14, [reg_p1+48]
  mov    r15, [reg_p1+56]
  sub    r8, [reg_p2] 
  sbb    r9, [reg_p2+8] 
  sbb    r10, [reg_p2+16] 
  sbb    r11, [reg_p2+24] 
  sbb    r12, [reg_p2+32] 
  sbb    r13, [reg_p2+40] 
  sbb    r14, [reg_p2+48] 
  sbb    r15, [reg_p2+56]
  sbb    rax, 0
  
  mov    rdi, [rip+fmt(p503x2)]
  and    rdi, rax
  mov    rsi, [rip+fmt(p503x2)+8]
  and    rsi, rax
  mov    rcx, [rip+fmt(p503x2)+24]
  and    rcx, rax
  
  add    r8, rdi  
  adc    r9, rsi  
  adc    r10, rsi 
  adc    r11, rcx 
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9 
  mov    [reg_p3+16], r10 
  mov    [reg_p3+24], r11 
  setc   cl

  mov    r8, [rip+fmt(p503x2)+32]
  and    r8, rax
  mov    r9, [rip+fmt(p503x2)+40]
  and    r9, rax
  mov    r10, [rip+fmt(p503x2)+48]
  and    r10, rax
  mov    r11, [rip+fmt(p503x2)+56]
  and    r11, rax
  
  bt     rcx, 0
  adc    r12, r8   
  adc    r13, r9  
  adc    r14, r10  
  adc    r15, r11  
  mov    [reg_p3+32], r12 
  mov    [reg_p3+40], r13 
  mov    [reg_p3+48], r14 
  mov    [reg_p3+56], r15 
  
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret


///////////////////////////////////////////////////////////////// MACRO
.macro SUB503_PX  P0
  push   r12
  push   r13
  push   r14
  
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  mov    r12, [reg_p1+32]
  mov    r13, [reg_p1+40]
  mov    r14, [reg_p1+48]
  mov    rcx, [reg_p1+56]
  sub    r8, [reg_p2] 
  sbb    r9, [reg_p2+8] 
  sbb    r10, [reg_p2+16] 
  sbb    r11, [reg_p2+24] 
  sbb    r12, [reg_p2+32] 
  sbb    r13, [reg_p2+40] 
  sbb    r14, [reg_p2+48] 
  sbb    rcx, [reg_p2+56]

  mov    rax, [rip+\P0]
  mov    rdi, [rip+\P0+8]
  mov    rsi, [rip+\P0+24]
  add    r8, rax
  mov    rax, [rip+\P0+32]  
  adc    r9, rdi  
  adc    r10, rdi 
  adc    r11, rsi 
  adc    r12, rax
  mov    rdi, [rip+\P0+40]
  mov    rsi, [rip+\P0+48]
  mov    rax, [rip+\P0+56]
  adc    r13, rdi  
  adc    r14, rsi
  adc    rcx, rax   
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9 
  mov    [reg_p3+16], r10 
  mov    [reg_p3+24], r11
  mov    [reg_p3+32], r12 
  mov    [reg_p3+40], r13
  mov    [reg_p3+48], r14
  mov    [reg_p3+56], rcx
  
  pop    r14
  pop    r13
  pop    r12
  .endm


//***********************************************************************
//  Multiprecision subtraction with correction with 2*p503
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 2*p503
//*********************************************************************** 
.global fmt(mp_sub503_p2_asm)
fmt(mp_sub503_p2_asm):

  SUB503_PX  fmt(p503x2)
  ret


//***********************************************************************
//  Multiprecision subtraction with correction with 4*p503
//  Operation: c [reg_p3] = a [reg_p1] - b [reg_p2] + 4*p503
//*********************************************************************** 
.global fmt(mp_sub503_p4_asm)
fmt(mp_sub503_p4_asm):

  SUB503_PX  fmt(p503x4)
  ret


#ifdef _MULX_
    
///////////////////////////////////////////////////////////////// MACRO
// Schoolbook integer multiplication
// Inputs:  memory pointers M0 and M1
// Outputs: memory pointer C and regs T1, T2, T3, T4, T8
// Temps:   regs T0:T9
/////////////////////////////////////////////////////////////////

#ifdef _ADX_
.macro MUL256_SCHOOL M0, M1, C, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9
    mov    rdx, \M0
    mulx   \T0, \T1, \M1     // T0:T1 = A0*B0
    mov    \C, \T1           // C0_final
    mulx   \T1, \T2, 8\M1    // T1:T2 = A0*B1
    xor    rax, rax   
    adox   \T0, \T2        
    mulx   \T2, \T3, 16\M1   // T2:T3 = A0*B2
    adox   \T1, \T3        
    mulx   \T3, \T4, 24\M1   // T3:T4 = A0*B3
    adox   \T2, \T4 
           
    mov    rdx, 8\M0
    mulx   \T5, \T4, \M1     // T5:T4 = A1*B0
    adox   \T3, rax 
    xor    rax, rax   
    mulx   \T6, \T7, 8\M1    // T6:T7 = A1*B1
    adox   \T4, \T0
    mov    8\C, \T4          // C1_final  
    adcx   \T5, \T7      
    mulx   \T7, \T8, 16\M1   // T7:T8 = A1*B2
    adcx   \T6, \T8  
    adox   \T5, \T1      
    mulx   \T8, \T9, 24\M1   // T8:T9 = A1*B3
    adcx   \T7, \T9        
    adcx   \T8, rax   
    adox   \T6, \T2
    
    mov    rdx, 16\M0
    mulx   \T1, \T0, \M1     // T1:T0 = A2*B0
    adox   \T7, \T3
    adox   \T8, rax
    xor    rax, rax 
    mulx   \T2, \T3, 8\M1    // T2:T3 = A2*B1
    adox   \T0, \T5   
    mov    16\C, \T0         // C2_final 
    adcx   \T1, \T3    
    mulx   \T3, \T4, 16\M1   // T3:T4 = A2*B2
    adcx   \T2, \T4 
    adox   \T1, \T6       
    mulx   \T4,\T9, 24\M1    // T3:T4 = A2*B3
    adcx   \T3, \T9        
    mov    rdx, 24\M0
    adcx   \T4, rax         

    adox   \T2, \T7
    adox   \T3, \T8
    adox   \T4, rax

    mulx   \T5, \T0, \M1     // T5:T0 = A3*B0
    xor    rax, rax 
    mulx   \T6, \T7, 8\M1    // T6:T7 = A3*B1
    adcx   \T5, \T7 
    adox   \T1, \T0       
    mulx   \T7, \T8, 16\M1   // T7:T8 = A3*B2
    adcx   \T6, \T8  
    adox   \T2, \T5      
    mulx   \T8, \T9, 24\M1   // T8:T9 = A3*B3
    adcx   \T7, \T9        
    adcx   \T8, rax         

    adox   \T3, \T6
    adox   \T4, \T7
    adox   \T8, rax
.endm 

#else

.macro MUL256_SCHOOL M0, M1, C, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9
    mov    rdx, \M0
    mulx   \T0, \T1, \M1     // T0:T1 = A0*B0
    mov    \C, \T1           // C0_final
    mulx   \T1, \T2, 8\M1    // T1:T2 = A0*B1
    add    \T0, \T2        
    mulx   \T2, \T3, 16\M1   // T2:T3 = A0*B2
    adc    \T1, \T3         
    mulx   \T3, \T4, 24\M1   // T3:T4 = A0*B3
    adc    \T2, \T4        
    mov    rdx, 8\M0
    adc    \T3, 0         

    mulx   \T5, \T4, \M1     // T5:T4 = A1*B0
    mulx   \T6, \T7, 8\M1    // T6:T7 = A1*B1
    add    \T5, \T7        
    mulx   \T7, \T8, 16\M1   // T7:T8 = A1*B2
    adc    \T6, \T8        
    mulx   \T8, \T9, 24\M1   // T8:T9 = A1*B3
    adc    \T7, \T9        
    adc    \T8, 0         

    add    \T4, \T0
    mov    8\C, \T4          // C1_final
    adc    \T5, \T1
    adc    \T6, \T2
    adc    \T7, \T3
    mov    rdx, 16\M0
    adc    \T8, 0

    mulx   \T1, \T0, \M1     // T1:T0 = A2*B0
    mulx   \T2, \T3, 8\M1    // T2:T3 = A2*B1
    add    \T1, \T3        
    mulx   \T3, \T4, 16\M1   // T3:T4 = A2*B2
    adc    \T2, \T4        
    mulx   \T4,\T9, 24\M1    // T3:T4 = A2*B3
    adc    \T3, \T9        
    mov    rdx, 24\M0
    adc    \T4, 0          

    add    \T0, \T5
    mov    16\C, \T0         // C2_final
    adc    \T1, \T6
    adc    \T2, \T7
    adc    \T3, \T8
    adc    \T4, 0

    mulx   \T5, \T0, \M1     // T5:T0 = A3*B0
    mulx   \T6, \T7, 8\M1    // T6:T7 = A3*B1
    add    \T5, \T7        
    mulx   \T7, \T8, 16\M1   // T7:T8 = A3*B2
    adc    \T6, \T8        
    mulx   \T8, \T9, 24\M1   // T8:T9 = A3*B3
    adc    \T7, \T9         
    adc    \T8, 0         

    add    \T1, \T0
    adc    \T2, \T5
    adc    \T3, \T6
    adc    \T4, \T7
    adc    \T8, 0
.endm
#endif


//*****************************************************************************
//  503-bit multiplication using Karatsuba (one level), schoolbook (one level)
//***************************************************************************** 
.global fmt(mul503_asm)
fmt(mul503_asm):    
    push   r12
    push   r13 
    push   r14 
    push   r15
    mov    rcx, reg_p3 

    // r8-r11 <- AH + AL, rax <- mask
    xor    rax, rax
    mov    r8, [reg_p1]
    mov    r9, [reg_p1+8]
    mov    r10, [reg_p1+16]
    mov    r11, [reg_p1+24] 
    push   rbx 
    push   rbp
    sub    rsp, 96
    add    r8, [reg_p1+32]
    adc    r9, [reg_p1+40]
    adc    r10, [reg_p1+48]
    adc    r11, [reg_p1+56]
    sbb    rax, 0
    mov    [rsp], r8
    mov    [rsp+8], r9
    mov    [rsp+16], r10
    mov    [rsp+24], r11

    // r12-r15 <- BH + BL, rbx <- mask
    xor    rbx, rbx
    mov    r12, [reg_p2]
    mov    r13, [reg_p2+8]
    mov    r14, [reg_p2+16]
    mov    r15, [reg_p2+24]
    add    r12, [reg_p2+32]
    adc    r13, [reg_p2+40]
    adc    r14, [reg_p2+48]
    adc    r15, [reg_p2+56]
    sbb    rbx, 0
    mov    [rsp+32], r12
    mov    [rsp+40], r13
    mov    [rsp+48], r14
    mov    [rsp+56], r15
    
    // r12-r15 <- masked (BH + BL)
    and    r12, rax
    and    r13, rax
    and    r14, rax
    and    r15, rax

    // r8-r11 <- masked (AH + AL)
    and    r8, rbx
    and    r9, rbx
    and    r10, rbx
    and    r11, rbx

    // r8-r11 <- masked (AH + AL) + masked (AH + AL)
    add    r8, r12
    adc    r9, r13
    adc    r10, r14
    adc    r11, r15
    mov    [rsp+64], r8
    mov    [rsp+72], r9
    mov    [rsp+80], r10
    mov    [rsp+88], r11

    // [rcx+64], r9-r12, rbx <- (AH+AL) x (BH+BL), low part 
    MUL256_SCHOOL  [rsp], [rsp+32], [rcx+64], r8, r9, r10, r11, r12, r13, r14, r15, rbx, rbp 
    mov    [rcx+88], r9  
    mov    [rcx+96], r10 
    mov    [rcx+104], r11
    mov    [rcx+112], r12
    mov    [rcx+120], rbx

    // [rcx], r9-r12, rbx <- AL x BL
    MUL256_SCHOOL  [reg_p1], [reg_p2], [rcx], r8, r9, r10, r11, r12, r13, r14, r15, rbx, rbp     // Result C0-C3 
    mov    [rcx+24], r9  
    mov    [rcx+32], r10 
    mov    [rcx+40], r11
    mov    [rcx+48], r12
    mov    [rcx+56], rbx

    // [rsp], rbx, rbp, r13-r15 <- AH x BH 
    MUL256_SCHOOL  [reg_p1+32], [reg_p2+32], [rsp], r8, rbx, rbp, r13, r14, r9, r10, r11, r15, r12
    
    // r8-r11 <- (AH+AL) x (BH+BL), final step
    mov    r8, [rsp+64]
    mov    r9, [rsp+72]
    mov    r10, [rsp+80]
    mov    r11, [rsp+88]
    mov    rax, [rcx+96]
    add    r8, rax
    mov    rax, [rcx+104]
    adc    r9, rax
    mov    rax, [rcx+112]
    adc    r10, rax
    mov    rax, [rcx+120]
    adc    r11, rax
    
    // r8-r12, rdi, rsi, rdx <- (AH+AL) x (BH+BL) - ALxBL
    mov    r12, [rcx+64]
    mov    rdi, [rcx+72]
    mov    rsi, [rcx+80]
    mov    rdx, [rcx+88]
    sub    r12, [rcx]
    sbb    rdi, [rcx+8]
    sbb    rsi, [rcx+16]
    sbb    rdx, [rcx+24]
    sbb    r8, [rcx+32]
    sbb    r9, [rcx+40]
    sbb    r10, [rcx+48]
    sbb    r11, [rcx+56]
    
    // r8-r12, rdi, rsi, rdx <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    sub    r12, [rsp]
    sbb    rdi, [rsp+8]
    sbb    rsi, [rsp+16]
    sbb    rdx, rbx
    sbb    r8, rbp
    sbb    r9, r13
    sbb    r10, r14
    sbb    r11, r15
    
    add    r12, [rcx+32]
    mov    [rcx+32], r12    // Result C4-C7
    adc    rdi, [rcx+40]
    mov    [rcx+40], rdi 
    adc    rsi, [rcx+48]
    mov    [rcx+48], rsi 
    adc    rdx, [rcx+56]
    mov    [rcx+56], rdx 
    mov    rax, [rsp]
    adc    r8, rax 
    mov    [rcx+64], r8    // Result C8-C15
    mov    rax, [rsp+8]
    adc    r9, rax
    mov    [rcx+72], r9 
    mov    rax, [rsp+16]
    adc    r10, rax
    mov    [rcx+80], r10 
    adc    r11, rbx
    mov    [rcx+88], r11
    adc    rbp, 0
    mov    [rcx+96], rbp
    adc    r13, 0
    mov    [rcx+104], r13
    adc    r14, 0
    mov    [rcx+112], r14
    adc    r15, 0
    mov    [rcx+120], r15  
    
    add    rsp, 96    
    pop    rbp  
    pop    rbx
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    ret

#else

//***********************************************************************
//  Integer multiplication
//  Based on Karatsuba method
//  Operation: c [reg_p3] = a [reg_p1] * b [reg_p2]
//  NOTE: a=c or b=c are not allowed
//***********************************************************************
.global fmt(mul503_asm)
fmt(mul503_asm):
  push   r12
  push   r13
  push   r14
  mov    rcx, reg_p3
  
  // rcx[0-3] <- AH+AL
  xor    rax, rax
  mov    r8, [reg_p1+32]
  mov    r9, [reg_p1+40]
  mov    r10, [reg_p1+48]
  mov    r11, [reg_p1+56]
  add    r8, [reg_p1] 
  adc    r9, [reg_p1+8] 
  adc    r10, [reg_p1+16] 
  adc    r11, [reg_p1+24] 
  push   r15  
  mov    [rcx], r8
  mov    [rcx+8], r9
  mov    [rcx+16], r10
  mov    [rcx+24], r11
  sbb    rax, 0 
  sub    rsp, 80           // Allocating space in stack
       
  // r12-r15 <- BH+BL
  xor    rdx, rdx
  mov    r12, [reg_p2+32]
  mov    r13, [reg_p2+40]
  mov    r14, [reg_p2+48]
  mov    r15, [reg_p2+56]
  add    r12, [reg_p2] 
  adc    r13, [reg_p2+8] 
  adc    r14, [reg_p2+16] 
  adc    r15, [reg_p2+24] 
  sbb    rdx, 0 
  mov    [rsp+64], rax
  mov    [rsp+72], rdx
  
  // (rsp[0-3],r8,r9,r10,r11) <- (AH+AL)*(BH+BL)
  mov    rax, [rcx]
  mul    r12
  mov    [rsp], rax        // c0
  mov    r8, rdx
  
  xor    r9, r9
  mov    rax, [rcx]
  mul    r13
  add    r8, rax
  adc    r9, rdx
  
  xor    r10, r10
  mov    rax, [rcx+8] 
  mul    r12
  add    r8, rax
  mov    [rsp+8], r8       // c1 
  adc    r9, rdx
  adc    r10, 0
  
  xor    r8, r8
  mov    rax, [rcx] 
  mul    r14
  add    r9, rax 
  adc    r10, rdx 
  adc    r8, 0
  
  mov    rax, [rcx+16] 
  mul    r12
  add    r9, rax
  adc    r10, rdx 
  adc    r8, 0
  
  mov    rax, [rcx+8] 
  mul    r13
  add    r9, rax
  mov    [rsp+16], r9      // c2 
  adc    r10, rdx 
  adc    r8, 0
  
  xor    r9, r9
  mov    rax, [rcx] 
  mul    r15
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, [rcx+24] 
  mul    r12
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, [rcx+8] 
  mul    r14
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, [rcx+16] 
  mul    r13
  add    r10, rax
  mov    [rsp+24], r10     // c3 
  adc    r8, rdx 
  adc    r9, 0
  
  xor    r10, r10
  mov    rax, [rcx+8] 
  mul    r15
  add    r8, rax
  adc    r9, rdx 
  adc    r10, 0
  
  mov    rax, [rcx+24] 
  mul    r13
  add    r8, rax
  adc    r9, rdx 
  adc    r10, 0
  
  mov    rax, [rcx+16] 
  mul    r14
  add    r8, rax
  mov    [rsp+32], r8      // c4 
  adc    r9, rdx 
  adc    r10, 0
  
  xor    r11, r11
  mov    rax, [rcx+16]
  mul    r15
  add    r9, rax
  adc    r10, rdx
  adc    r11, 0

  mov    rax, [rcx+24] 
  mul    r14
  add    r9, rax          // c5 
  adc    r10, rdx
  adc    r11, 0

  mov    rax, [rcx+24] 
  mul    r15
  add    r10, rax         // c6 
  adc    r11, rdx         // c7 
  
  mov    rax, [rsp+64]
  and    r12, rax
  and    r13, rax
  and    r14, rax
  and    r15, rax
  add    r12, r8
  adc    r13, r9
  adc    r14, r10
  adc    r15, r11

  mov    rax, [rsp+72]  
  mov    r8, [rcx]
  mov    r9, [rcx+8]
  mov    r10, [rcx+16]
  mov    r11, [rcx+24]
  and    r8, rax
  and    r9, rax
  and    r10, rax
  and    r11, rax
  add    r8, r12
  adc    r9, r13
  adc    r10, r14
  adc    r11, r15
  mov    [rsp+32], r8
  mov    [rsp+40], r9
  mov    [rsp+48], r10
  mov    [rsp+56], r11
  
  // rcx[0-7] <- AL*BL
  mov    r11, [reg_p1]
  mov    rax, [reg_p2] 
  mul    r11
  xor    r9, r9
  mov    [rcx], rax        // c0
  mov    r8, rdx
  
  mov    r14, [reg_p1+16] 
  mov    rax, [reg_p2+8]
  mul    r11
  xor    r10, r10
  add    r8, rax
  adc    r9, rdx

  mov    r12, [reg_p1+8] 
  mov    rax, [reg_p2] 
  mul    r12
  add    r8, rax
  mov    [rcx+8], r8       // c1 
  adc    r9, rdx
  adc    r10, 0
  
  xor    r8, r8
  mov    rax, [reg_p2+16] 
  mul    r11
  add    r9, rax
  adc    r10, rdx 
  adc    r8, 0
  
  mov    r13, [reg_p2] 
  mov    rax, r14 
  mul    r13
  add    r9, rax
  adc    r10, rdx 
  adc    r8, 0
  
  mov    rax, [reg_p2+8] 
  mul    r12
  add    r9, rax
  mov    [rcx+16], r9      // c2 
  adc    r10, rdx 
  adc    r8, 0
  
  xor    r9, r9
  mov    rax, [reg_p2+24] 
  mul    r11
  mov    r15, [reg_p1+24] 
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, r15 
  mul    r13
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, [reg_p2+16] 
  mul    r12
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, [reg_p2+8] 
  mul    r14
  add    r10, rax
  mov    [rcx+24], r10     // c3 
  adc    r8, rdx 
  adc    r9, 0
  
  xor    r10, r10
  mov    rax, [reg_p2+24] 
  mul    r12
  add    r8, rax
  adc    r9, rdx 
  adc    r10, 0
  
  mov    rax, [reg_p2+8] 
  mul    r15
  add    r8, rax
  adc    r9, rdx 
  adc    r10, 0
  
  mov    rax, [reg_p2+16] 
  mul    r14
  add    r8, rax
  mov    [rcx+32], r8     // c4 
  adc    r9, rdx 
  adc    r10, 0
  
  xor    r8, r8
  mov    rax, [reg_p2+24]
  mul    r14
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0

  mov    rax, [reg_p2+16] 
  mul    r15
  add    r9, rax
  mov    [rcx+40], r9      // c5 
  adc    r10, rdx
  adc    r8, 0

  mov    rax, [reg_p2+24] 
  mul    r15
  add    r10, rax
  mov    [rcx+48], r10     // c6 
  adc    r8, rdx   
  mov    [rcx+56], r8      // c7 

  // rcx[8-15] <- AH*BH
  mov    r11, [reg_p1+32]
  mov    rax, [reg_p2+32] 
  mul    r11
  xor    r9, r9
  mov    [rcx+64], rax     // c0
  mov    r8, rdx
  
  mov    r14, [reg_p1+48] 
  mov    rax, [reg_p2+40]
  mul    r11
  xor    r10, r10
  add    r8, rax
  adc    r9, rdx

  mov    r12, [reg_p1+40] 
  mov    rax, [reg_p2+32] 
  mul    r12
  add    r8, rax
  mov    [rcx+72], r8      // c1 
  adc    r9, rdx
  adc    r10, 0
  
  xor    r8, r8
  mov    rax, [reg_p2+48] 
  mul    r11
  add    r9, rax
  adc    r10, rdx 
  adc    r8, 0
  
  mov    r13, [reg_p2+32] 
  mov    rax, r14 
  mul    r13
  add    r9, rax
  adc    r10, rdx 
  adc    r8, 0
  
  mov    rax, [reg_p2+40] 
  mul    r12
  add    r9, rax
  mov    [rcx+80], r9      // c2 
  adc    r10, rdx 
  adc    r8, 0
  
  xor    r9, r9
  mov    rax, [reg_p2+56] 
  mul    r11
  mov    r15, [reg_p1+56] 
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, r15 
  mul    r13
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, [reg_p2+48] 
  mul    r12
  add    r10, rax
  adc    r8, rdx 
  adc    r9, 0
  
  mov    rax, [reg_p2+40] 
  mul    r14
  add    r10, rax
  mov    [rcx+88], r10     // c3 
  adc    r8, rdx 
  adc    r9, 0
  
  xor    r10, r10
  mov    rax, [reg_p2+56] 
  mul    r12
  add    r8, rax
  adc    r9, rdx 
  adc    r10, 0
  
  mov    rax, [reg_p2+40] 
  mul    r15
  add    r8, rax
  adc    r9, rdx 
  adc    r10, 0
  
  mov    rax, [reg_p2+48] 
  mul    r14
  add    r8, rax
  mov    [rcx+96], r8      // c4 
  adc    r9, rdx 
  adc    r10, 0
  
  xor    r8, r8
  mov    rax, [reg_p2+56]
  mul    r14
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0

  mov    rax, [reg_p2+48] 
  mul    r15
  add    r9, rax
  mov    [rcx+104], r9     // c5 
  adc    r10, rdx
  adc    r8, 0

  mov    rax, [reg_p2+56] 
  mul    r15
  add    r10, rax
  mov    [rcx+112], r10    // c6 
  adc    r8, rdx   
  mov    [rcx+120], r8     // c7 
      
  // [r8-r15] <- (AH+AL)*(BH+BL) - AL*BL 
  mov    r8,  [rsp]
  sub    r8,  [rcx] 
  mov    r9,  [rsp+8]
  sbb    r9,  [rcx+8]
  mov    r10, [rsp+16]
  sbb    r10, [rcx+16]
  mov    r11, [rsp+24]
  sbb    r11, [rcx+24] 
  mov    r12, [rsp+32]
  sbb    r12, [rcx+32]
  mov    r13, [rsp+40]
  sbb    r13, [rcx+40] 
  mov    r14, [rsp+48]
  sbb    r14, [rcx+48] 
  mov    r15, [rsp+56]
  sbb    r15, [rcx+56]
      
  // [r8-r15] <- (AH+AL)*(BH+BL) - AL*BL - AH*BH
  mov    rax, [rcx+64]
  sub    r8,  rax 
  mov    rax, [rcx+72]
  sbb    r9,  rax
  mov    rax, [rcx+80]
  sbb    r10, rax
  mov    rax, [rcx+88]
  sbb    r11, rax 
  mov    rax, [rcx+96]
  sbb    r12, rax
  mov    rdx, [rcx+104]
  sbb    r13, rdx
  mov    rdi, [rcx+112]
  sbb    r14, rdi 
  mov    rsi, [rcx+120]
  sbb    r15, rsi 
      
  // Final result
  add    r8,  [rcx+32] 
  mov    [rcx+32], r8
  adc    r9,  [rcx+40]
  mov    [rcx+40], r9
  adc    r10, [rcx+48]
  mov    [rcx+48], r10
  adc    r11, [rcx+56]
  mov    [rcx+56], r11
  adc    r12, [rcx+64]
  mov    [rcx+64], r12
  adc    r13, [rcx+72]
  mov    [rcx+72], r13
  adc    r14, [rcx+80] 
  mov    [rcx+80], r14
  adc    r15, [rcx+88] 
  mov    [rcx+88], r15
  adc    rax, 0
  mov    [rcx+96], rax
  adc    rdx, 0
  mov    [rcx+104], rdx
  adc    rdi, 0
  mov    [rcx+112], rdi
  adc    rsi, 0
  mov    [rcx+120], rsi
    
  add    rsp, 80           // Restoring space in stack
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret

#endif


#ifdef _MULX_

///////////////////////////////////////////////////////////////// MACRO
// Schoolbook integer multiplication
// Inputs:  regs I0 and I1, and memory pointer M1
// Outputs: regs T0:T5
// Temps:   regs T0:T5
/////////////////////////////////////////////////////////////////

#ifdef _ADX_
.macro MUL128x256_SCHOOL I0, I1, M1, T0, T1, T2, T3, T4, T5
    mulx   \T2, \T4, 8\M1
    xor    rax, rax
    mulx   \T3, \T5, 16\M1 
    ADD1   \T1, \T4               
    ADC1   \T2, \T5     
    mulx   \T4, \T5, 24\M1
    ADC1   \T3, \T5 
    ADC1   \T4, rax   
    
    xor    rax, rax
    mov    rdx, \I1 
    mulx   \I1, \T5, \M1 
    ADD2   \T1, \T5            // T1 <- C1_final 
    ADC2   \T2, \I1     
    mulx   \T5, \I1, 8\M1
    ADC2   \T3, \T5 
    ADD1   \T2, \I1        
    mulx   \T5, \I1, 16\M1
    ADC2   \T4, \T5 
    ADC1   \T3, \I1     
    mulx   \T5, \I1, 24\M1   
    ADC2   \T5, rax         
    ADC1   \T4, \I1  
    ADC1   \T5, rax 
.endm

#else

.macro MUL128x256_SCHOOL I0, I1, M1, T0, T1, T2, T3, T4, T5 
    mulx   \T2, \T4, 8\M1
    mulx   \T3, \T5, 16\M1 
    add    \T1, \T4               
    adc    \T2, \T5     
    mulx   \T4, \T5, 24\M1
    adc    \T3, \T5 
    adc    \T4, 0   
    
    mov    rdx, \I1 
    mulx   \I1, \T5, \M1 
    add    \T1, \T5            // T1 <- C1_final 
    adc    \T2, \I1     
    mulx   \T5, \I1, 8\M1
    adc    \T3, \T5       
    mulx   \T5, rax, 16\M1
    adc    \T4, \T5     
    mulx   \T5, rdx, 24\M1 
    adc    \T5, 0
    add    \T2, \I1  
    adc    \T3, rax        
    adc    \T4, rdx  
    adc    \T5, 0 
.endm
#endif

  
//**************************************************************************************
//  Montgomery reduction
//  Based on method described in Faz-Hernandez et al. https://eprint.iacr.org/2017/1015  
//  Operation: c [reg_p2] = a [reg_p1]
//************************************************************************************** 
.global fmt(rdc503_asm)
fmt(rdc503_asm):

    // a[0-1] x 64xp503p1_nz --> result: r8:r13  
    mov    rdx, [reg_p1]
    mov    rcx, [reg_p1+8]  
    mulx   r9, r8, [rip+fmt(p503p1x64)]   // result r8  
    push   rbx
    push   rbp
    push   r12
    push   r13 
    push   r14 
    push   r15
    MUL128x256_SCHOOL rdx, rcx, [rip+fmt(p503p1x64)], r8, r9, r10, r11, r12, r13     

    xor    r15, r15
    shrd   r15, r8, 6 
    shrd   r8, r9, 6 
    shrd   r9, r10, 6 
    shrd   r10, r11, 6 
    shrd   r11, r12, 6 
    shrd   r12, r13, 6 
    shr    r13, 6
    mov    rdx, [reg_p1+16] 
    mov    r14, [reg_p1+80] 
    add    r15, [reg_p1+24]
    adc    r8, [reg_p1+32]  
    adc    r9, [reg_p1+40]  
    adc    r10, [reg_p1+48]   
    adc    r11, [reg_p1+56]   
    adc    r12, [reg_p1+64]   
    adc    r13, [reg_p1+72]  
    mulx   rbx, rcx, [rip+fmt(p503p1x64)]   // result rcx
    adc    r14, 0
    mov    [reg_p2], r8  
    mov    [reg_p2+8], r9  
    mov    [reg_p2+16], r10  
    mov    [reg_p2+24], r11  
    mov    [reg_p2+32], r12   
    mov    [reg_p2+40], r13    
    mov    [reg_p2+48], r14 
    mov    r9, [reg_p1+88]  
    mov    r10, [reg_p1+96]  
    mov    r11, [reg_p1+104]  
    mov    r12, [reg_p1+112]
    mov    rdi, [reg_p1+120]
    adc    r9, 0
    adc    r10, 0
    adc    r11, 0
    adc    r12, 0
    adc    rdi, 0

    // a[2-3] x 64xp503p1_nz --> result: rcx, rbx, rbp, r14, r8, r13
    MUL128x256_SCHOOL rdx, r15, [rip+fmt(p503p1x64)], rcx, rbx, rbp, r14, r8, r13 

    xor    r15, r15
    shrd   r15, rcx, 6 
    shrd   rcx, rbx, 6 
    shrd   rbx, rbp, 6 
    shrd   rbp, r14, 6 
    shrd   r14, r8, 6  
    shrd   r8, r13, 6 
    shr    r13, 6
    mov    rdx, [reg_p2]
    add    r15, [reg_p2+8]
    adc    rcx, [reg_p2+16]  
    adc    rbx, [reg_p2+24]  
    adc    rbp, [reg_p2+32]   
    adc    r14, [reg_p2+40]  
    adc    r8, [reg_p2+48]
    mov    [reg_p2+16], rcx  
    mov    [reg_p2+24], rbx  
    mov    [reg_p2+32], rbp   
    mov    [reg_p2+40], r14 
    mov    [reg_p2+48], r8 
    mulx   rbp, rbx, [rip+fmt(p503p1x64)]   // result rbx    
    adc    r9, r13 
    adc    r10, 0
    adc    r11, 0
    adc    r12, 0
    adc    rdi, 0

    // a[4-5] x 64xp503p1_nz --> result: rbx, rbp, r14, r8, r13, rcx
    MUL128x256_SCHOOL rdx, r15, [rip+fmt(p503p1x64)], rbx, rbp, r14, r8, r13, rcx  

    xor    r15, r15
    shrd   r15, rbx, 6 
    shrd   rbx, rbp, 6 
    shrd   rbp, r14, 6 
    shrd   r14, r8, 6 
    shrd   r8, r13, 6 
    shrd   r13, rcx, 6 
    shr    rcx, 6
    mov    rdx, [reg_p2+16]
    add    r15, [reg_p2+24]
    adc    rbx, [reg_p2+32]  
    adc    rbp, [reg_p2+40]  
    adc    r14, [reg_p2+48]  
    mov    [reg_p2], rbx              // Final result c0
    mov    [reg_p2+8], rbp   
    mov    [reg_p2+16], r14
    adc    r9, r8 
    adc    r10, r13  
    mulx   r14, rbp, [rip+fmt(p503p1x64)]   // result rbp  
    adc    r11, rcx
    adc    r12, 0
    adc    rdi, 0

    // a[6-7] x 64xp503p1_nz --> result: rbp, r14, r8, r13, rcx, rbx
    MUL128x256_SCHOOL rdx, r15, [rip+fmt(p503p1x64)], rbp, r14, r8, r13, rcx, rbx  
    
    xor    r15, r15
    shrd   r15, rbp, 6 
    shrd   rbp, r14, 6 
    shrd   r14, r8, 6 
    shrd   r8, r13, 6 
    shrd   r13, rcx, 6 
    shrd   rcx, rbx, 6 
    shr    rbx, 6 
    add    r15, [reg_p2+8]
    adc    rbp, [reg_p2+16] 
    mov    [reg_p2+8], r15       // Final result c1-c7
    mov    [reg_p2+16], rbp  
    adc    r9, r14 
    adc    r10, r8 
    adc    r11, r13
    adc    r12, rcx
    adc    rdi, rbx    
    mov    [reg_p2+24], r9  
    mov    [reg_p2+32], r10  
    mov    [reg_p2+40], r11   
    mov    [reg_p2+48], r12
    mov    [reg_p2+56], rdi

    pop    r15
    pop    r14
    pop    r13
    pop    r12
    pop    rbp
    pop    rbx
    ret
    
  #else
  
//***********************************************************************
//  Montgomery reduction
//  Based on comba method
//  Operation: c [reg_p2] = a [reg_p1]
//  NOTE: a=c is not allowed
//*********************************************************************** 
.global fmt(rdc503_asm)
fmt(rdc503_asm):
  push   r12
  push   r13 
  push   r14 
  push   r15 

  mov    r11, [reg_p1]
  mov    rax, [rip+fmt(p503p1)+24] 
  mul    r11
  xor    r8, r8
  add    rax, [reg_p1+24]
  mov    [reg_p2+24], rax    // z3
  adc    r8, rdx
  
  xor    r9, r9
  mov    rax, [rip+fmt(p503p1)+32] 
  mul    r11
  xor    r10, r10
  add    r8, rax
  adc    r9, rdx

  mov    r12, [reg_p1+8]
  mov    rax, [rip+fmt(p503p1)+24] 
  mul    r12
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  add    r8, [reg_p1+32]
  mov    [reg_p2+32], r8    // z4
  adc    r9, 0
  adc    r10, 0
  
  xor    r8, r8
  mov    rax, [rip+fmt(p503p1)+40] 
  mul    r11
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    rax, [rip+fmt(p503p1)+32] 
  mul    r12
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    r13, [reg_p1+16]
  mov    rax, [rip+fmt(p503p1)+24] 
  mul    r13
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  add    r9, [reg_p1+40]
  mov    [reg_p2+40], r9    // z5
  adc    r10, 0
  adc    r8, 0
  
  xor    r9, r9
  mov    rax, [rip+fmt(p503p1)+48] 
  mul    r11
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  
  mov    rax, [rip+fmt(p503p1)+40] 
  mul    r12
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  
  mov    rax, [rip+fmt(p503p1)+32]
  mul    r13
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  
  mov    r14, [reg_p2+24]
  mov    rax, [rip+fmt(p503p1)+24] 
  mul    r14
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  add    r10, [reg_p1+48]
  mov    [reg_p2+48], r10   // z6
  adc    r8, 0
  adc    r9, 0
  
  xor    r10, r10
  mov    rax, [rip+fmt(p503p1)+56] 
  mul    r11
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  
  mov    rax, [rip+fmt(p503p1)+48] 
  mul    r12
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  
  mov    rax, [rip+fmt(p503p1)+40] 
  mul    r13
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  
  mov    rax, [rip+fmt(p503p1)+32] 
  mul    r14
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  
  mov    r15, [reg_p2+32]
  mov    rax, [rip+fmt(p503p1)+24] 
  mul    r15
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  add    r8, [reg_p1+56]
  mov    [reg_p2+56], r8    // z7
  adc    r9, 0
  adc    r10, 0
  
  xor    r8, r8
  mov    rax, [rip+fmt(p503p1)+56] 
  mul    r12
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    rax, [rip+fmt(p503p1)+48] 
  mul    r13
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    rax, [rip+fmt(p503p1)+40] 
  mul    r14
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    rax, [rip+fmt(p503p1)+32] 
  mul    r15
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    rcx, [reg_p2+40]
  mov    rax, [rip+fmt(p503p1)+24] 
  mul    rcx
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  add    r9, [reg_p1+64]
  mov    [reg_p2], r9        // z0
  adc    r10, 0
  adc    r8, 0
  
  xor    r9, r9
  mov    rax, [rip+fmt(p503p1)+56] 
  mul    r13
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0

  mov    rax, [rip+fmt(p503p1)+48] 
  mul    r14
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0

  mov    rax, [rip+fmt(p503p1)+40]
  mul    r15
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0

  mov    rax, [rip+fmt(p503p1)+32]
  mul    rcx
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  
  mov    r13, [reg_p2+48]
  mov    rax, [rip+fmt(p503p1)+24]
  mul    r13
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  add    r10, [reg_p1+72]
  mov    [reg_p2+8], r10     // z1
  adc    r8, 0
  adc    r9, 0
  
  xor    r10, r10
  mov    rax, [rip+fmt(p503p1)+56] 
  mul    r14
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  
  mov    rax, [rip+fmt(p503p1)+48] 
  mul    r15
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  
  mov    rax, [rip+fmt(p503p1)+40] 
  mul    rcx
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  
  mov    rax, [rip+fmt(p503p1)+32] 
  mul    r13
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  
  mov    r14, [reg_p2+56]
  mov    rax, [rip+fmt(p503p1)+24] 
  mul    r14
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  add    r8, [reg_p1+80]
  mov    [reg_p2+16], r8     // z2
  adc    r9, 0
  adc    r10, 0
  
  xor    r8, r8
  mov    rax, [rip+fmt(p503p1)+56] 
  mul    r15
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    rax, [rip+fmt(p503p1)+48] 
  mul    rcx
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    rax, [rip+fmt(p503p1)+40] 
  mul    r13
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  
  mov    rax, [rip+fmt(p503p1)+32] 
  mul    r14
  add    r9, rax
  adc    r10, rdx
  adc    r8, 0
  add    r9, [reg_p1+88]
  mov    [reg_p2+24], r9     // z3
  adc    r10, 0
  adc    r8, 0
  
  xor    r9, r9
  mov    rax, [rip+fmt(p503p1)+56] 
  mul    rcx
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  
  mov    rax, [rip+fmt(p503p1)+48] 
  mul    r13
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  
  mov    rax, [rip+fmt(p503p1)+40] 
  mul    r14
  add    r10, rax
  adc    r8, rdx
  adc    r9, 0
  add    r10, [reg_p1+96]
  mov    [reg_p2+32], r10    // z4
  adc    r8, 0
  adc    r9, 0
  
  xor    r10, r10
  mov    rax, [rip+fmt(p503p1)+56] 
  mul    r13
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0

  mov    rax, [rip+fmt(p503p1)+48] 
  mul    r14
  add    r8, rax
  adc    r9, rdx
  adc    r10, 0
  add    r8, [reg_p1+104]    // z5
  mov    [reg_p2+40], r8     // z5
  adc    r9, 0
  adc    r10, 0
  
  mov    rax, [rip+fmt(p503p1)+56] 
  mul    r14
  add    r9, rax
  adc    r10, rdx
  add    r9, [reg_p1+112]    // z6
  mov    [reg_p2+48], r9     // z6
  adc    r10, 0  
  add    r10, [reg_p1+120]   // z7
  mov    [reg_p2+56], r10    // z7

  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret

  #endif


//***********************************************************************
//  503-bit multiprecision addition
//  Operation: c [reg_p3] = a [reg_p1] + b [reg_p2]
//*********************************************************************** 
.global fmt(mp_add503_asm)
fmt(mp_add503_asm): 
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  add    r8, [reg_p2] 
  adc    r9, [reg_p2+8] 
  adc    r10, [reg_p2+16] 
  adc    r11, [reg_p2+24] 
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9
  mov    [reg_p3+16], r10
  mov    [reg_p3+24], r11
  
  mov    r8, [reg_p1+32]
  mov    r9, [reg_p1+40]
  mov    r10, [reg_p1+48]
  mov    r11, [reg_p1+56]
  adc    r8, [reg_p2+32] 
  adc    r9, [reg_p2+40] 
  adc    r10, [reg_p2+48] 
  adc    r11, [reg_p2+56]
  mov    [reg_p3+32], r8
  mov    [reg_p3+40], r9
  mov    [reg_p3+48], r10
  mov    [reg_p3+56], r11
  ret


//***********************************************************************
//  2x503-bit multiprecision subtraction/addition
//  Operation: c [x2] = a [x0] - b [x1]. If c < 0, add p503*2^512
//*********************************************************************** 
.global fmt(mp_subadd503x2_asm)
fmt(mp_subadd503x2_asm):
  push   r12
  push   r13 
  push   r14 
  push   r15
  xor    rax, rax
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  sub    r8, [reg_p2] 
  sbb    r9, [reg_p2+8] 
  sbb    r10, [reg_p2+16] 
  sbb    r11, [reg_p2+24] 
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9
  mov    [reg_p3+16], r10
  mov    [reg_p3+24], r11

  mov    r8, [reg_p1+32]
  mov    r9, [reg_p1+40]
  mov    r10, [reg_p1+48] 
  mov    r11, [reg_p1+56]
  sbb    r8, [reg_p2+32] 
  sbb    r9, [reg_p2+40] 
  sbb    r10, [reg_p2+48]
  sbb    r11, [reg_p2+56] 
  mov    [reg_p3+32], r8
  mov    [reg_p3+40], r9
  mov    [reg_p3+48], r10
  mov    [reg_p3+56], r11

  mov    r8, [reg_p1+64]
  mov    r9, [reg_p1+72]
  mov    r10, [reg_p1+80] 
  mov    r11, [reg_p1+88]
  sbb    r8, [reg_p2+64] 
  sbb    r9, [reg_p2+72] 
  sbb    r10, [reg_p2+80]
  sbb    r11, [reg_p2+88] 
  mov    [reg_p3+64], r8
  mov    [reg_p3+72], r9
  mov    [reg_p3+80], r10
  mov    [reg_p3+88], r11
  
  mov    r12, [reg_p1+96]
  mov    r13, [reg_p1+104] 
  mov    r14, [reg_p1+112]
  mov    r15, [reg_p1+120]
  sbb    r12, [reg_p2+96]
  sbb    r13, [reg_p2+104]
  sbb    r14, [reg_p2+112]  
  sbb    r15, [reg_p2+120] 
  sbb    rax, 0
  
  // Add p503 anded with the mask in rax 
  mov    r8, [rip+fmt(p503)]
  mov    r9, [rip+fmt(p503)+24]
  mov    r10, [rip+fmt(p503)+32]
  mov    r11, [rip+fmt(p503)+40]
  mov    rdi, [rip+fmt(p503)+48]
  mov    rsi, [rip+fmt(p503)+56]
  and    r8, rax
  and    r9, rax
  and    r10, rax
  and    r11, rax
  and    rdi, rax
  and    rsi, rax
  mov    rax, [reg_p3+64]
  add    rax, r8
  mov    [reg_p3+64], rax
  mov    rax, [reg_p3+72]
  adc    rax, r8
  mov    [reg_p3+72], rax
  mov    rax, [reg_p3+80]
  adc    rax, r8
  mov    [reg_p3+80], rax
  mov    rax, [reg_p3+88]
  adc    rax, r9
  mov    [reg_p3+88], rax
  adc    r12, r10
  adc    r13, r11
  adc    r14, rdi
  adc    r15, rsi
  
  mov    [reg_p3+96], r12
  mov    [reg_p3+104], r13
  mov    [reg_p3+112], r14
  mov    [reg_p3+120], r15
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret


//***********************************************************************
//  Double 2x503-bit multiprecision subtraction
//  Operation: c [reg_p3] = c [reg_p3] - a [reg_p1] - b [reg_p2]
//*********************************************************************** 
.global fmt(mp_dblsub503x2_asm)
fmt(mp_dblsub503x2_asm):
  push   r12
  push   r13
  
  mov    r8, [reg_p3]
  mov    r9, [reg_p3+8]
  mov    r10, [reg_p3+16]
  mov    r11, [reg_p3+24]
  mov    r12, [reg_p3+32]
  mov    r13, [reg_p3+40]
  sub    r8, [reg_p1]
  sbb    r9, [reg_p1+8] 
  sbb    r10, [reg_p1+16] 
  sbb    r11, [reg_p1+24] 
  sbb    r12, [reg_p1+32] 
  sbb    r13, [reg_p1+40]
  setc   al 
  sub    r8, [reg_p2]
  sbb    r9, [reg_p2+8] 
  sbb    r10, [reg_p2+16] 
  sbb    r11, [reg_p2+24] 
  sbb    r12, [reg_p2+32] 
  sbb    r13, [reg_p2+40] 
  setc   cl
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9
  mov    [reg_p3+16], r10
  mov    [reg_p3+24], r11
  mov    [reg_p3+32], r12
  mov    [reg_p3+40], r13
  
  mov    r8, [reg_p3+48]
  mov    r9, [reg_p3+56]
  mov    r10, [reg_p3+64]
  mov    r11, [reg_p3+72]
  mov    r12, [reg_p3+80]
  mov    r13, [reg_p3+88]
  bt     rax, 0  
  sbb    r8, [reg_p1+48] 
  sbb    r9, [reg_p1+56]
  sbb    r10, [reg_p1+64] 
  sbb    r11, [reg_p1+72] 
  sbb    r12, [reg_p1+80] 
  sbb    r13, [reg_p1+88]
  setc   al 
  bt     rcx, 0  
  sbb    r8, [reg_p2+48] 
  sbb    r9, [reg_p2+56]
  sbb    r10, [reg_p2+64] 
  sbb    r11, [reg_p2+72] 
  sbb    r12, [reg_p2+80] 
  sbb    r13, [reg_p2+88] 
  setc   cl
  mov    [reg_p3+48], r8
  mov    [reg_p3+56], r9
  mov    [reg_p3+64], r10
  mov    [reg_p3+72], r11
  mov    [reg_p3+80], r12
  mov    [reg_p3+88], r13
    
  mov    r8, [reg_p3+96]
  mov    r9, [reg_p3+104]
  mov    r10, [reg_p3+112]
  mov    r11, [reg_p3+120]
  bt     rax, 0  
  sbb    r8, [reg_p1+96] 
  sbb    r9, [reg_p1+104] 
  sbb    r10, [reg_p1+112] 
  sbb    r11, [reg_p1+120]
  bt     rcx, 0  
  sbb    r8, [reg_p2+96] 
  sbb    r9, [reg_p2+104] 
  sbb    r10, [reg_p2+112] 
  sbb    r11, [reg_p2+120]
  mov    [reg_p3+96], r8
  mov    [reg_p3+104], r9
  mov    [reg_p3+112], r10
  mov    [reg_p3+120], r11
  
  pop    r13
  pop    r12
  ret
