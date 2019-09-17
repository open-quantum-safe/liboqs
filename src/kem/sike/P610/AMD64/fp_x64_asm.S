//*******************************************************************************************
// SIDH: an efficient supersingular isogeny cryptography library
//
// Abstract: field arithmetic in x64 assembly for P610 on Linux 
//*******************************************************************************************  

.intel_syntax noprefix 

// Registers that are used for parameter passing:
#define reg_p1  rdi
#define reg_p2  rsi
#define reg_p3  rdx


.text
//***********************************************************************
//  Field addition
//  Operation: c [reg_p3] = a [reg_p1] + b [reg_p2]
//*********************************************************************** 
.global fpadd610_asm
fpadd610_asm:
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

  mov    rax, [rip+p610x2]
  sub    r8, rax
  mov    rax, [rip+p610x2+8]
  sbb    r9, rax
  sbb    r10, rax
  sbb    r11, rax
  mov    rax, [rip+p610x2+32]
  sbb    r12, rax
  mov    rax, [rip+p610x2+40]
  sbb    r13, rax
  mov    rax, [rip+p610x2+48]
  sbb    r14, rax
  mov    rax, [rip+p610x2+56]
  sbb    r15, rax
  mov    rax, [rip+p610x2+64]
  sbb    rcx, rax
  mov    rax, [rip+p610x2+72]
  sbb    rdi, rax
  mov    [reg_p3+64], rcx
  mov    [reg_p3+72], rdi
  mov    rax, 0
  sbb    rax, 0
  
  mov    rsi, [rip+p610x2]
  and    rsi, rax
  mov    rdi, [rip+p610x2+8]
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
  
  mov    rdi, [rip+p610x2+32]
  and    rdi, rax
  mov    rsi, [rip+p610x2+40]
  and    rsi, rax
  mov    r8, [rip+p610x2+48]
  and    r8, rax
  mov    r9, [rip+p610x2+56]
  and    r9, rax
  mov    r10, [rip+p610x2+64]
  and    r10, rax
  mov    r11, [rip+p610x2+72]
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
.global fpsub610_asm
fpsub610_asm:
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
    
  mov    rsi, [rip+p610x2]
  and    rsi, rax
  mov    rdi, [rip+p610x2+8]
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
  
  mov    rdi, [rip+p610x2+32]
  and    rdi, rax
  mov    rsi, [rip+p610x2+40]
  and    rsi, rax
  mov    r8, [rip+p610x2+48]
  and    r8, rax
  mov    r9, [rip+p610x2+56]
  and    r9, rax
  mov    r10, [rip+p610x2+64]
  and    r10, rax
  mov    r11, [rip+p610x2+72]
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


#ifdef _MULX_

/////////////////////////////////////////////////////////////////////////// MACRO
// Schoolbook integer multiplication
// Inputs:  memory pointers M0 and M1
// Outputs: memory pointer C
// Temps:   regs T0:T7
///////////////////////////////////////////////////////////////////////////
#ifdef _ADX_

.macro MUL320_SCHOOL M0, M1, C, T0, T1, T2, T3, T4, T5, T6, T7 
    mov    rdx, \M0
    mulx   \T0, \T1, \M1    
    mulx   \T2, \T3, 8\M1
    mov    \C, \T1             // C0_final 
    xor    rax, rax
    mulx   \T4, \T5, 16\M1 
    adox   \T0, \T3               
    adox   \T2, \T5     
    mulx   \T1, \T3, 24\M1
    adox   \T4, \T3         
    mulx   \T5, \T6, 32\M1 
    adox   \T1, \T6     
    adox   \T5, rax        
    
    mov    rdx, 8\M0 
    mulx   \T6, \T7, \M1 
    xor    rax, rax
    adcx   \T0, \T7 
    mov    8\C, \T0            // C1_final 
    adcx   \T2, \T6     
    mulx   \T6, \T7, 8\M1
    adox   \T2, \T7 
    adcx   \T4, \T6        
    mulx   \T0, \T6, 16\M1 
    adox   \T4, \T6  
    adcx   \T0, \T1     
    mulx   \T1, \T7, 24\M1   
    adcx   \T1, \T5 
    adox   \T0, \T7   
    mulx   \T5, \T6, 32\M1 
    adcx   \T5, rax         
    adox   \T1, \T6  
    adox   \T5, rax         
    
    mov    rdx, 16\M0 
    mulx   \T6, \T7, \M1
    xor    rax, rax 
    adcx   \T2, \T7 
    mov    16\C, \T2           // C2_final 
    adcx   \T4, \T6     
    mulx   \T6, \T7, 8\M1
    adox   \T4, \T7 
    adcx   \T0, \T6        
    mulx   \T2, \T6, 16\M1
    adox   \T0, \T6 
    adcx   \T1, \T2     
    mulx   \T2, \T7, 24\M1   
    adcx   \T5, \T2          
    adox   \T1, \T7   
    mulx   \T2, \T6, 32\M1   
    adcx   \T2, rax 
    adox   \T5, \T6 
    adox   \T2, rax           
    
    mov    rdx, 24\M0 
    mulx   \T6, \T7, \M1
    xor    rax, rax 
    adcx   \T4, \T7 
    mov    24\C, \T4           // C3_final 
    adcx   \T0, \T6     
    mulx   \T6, \T7, 8\M1
    adox   \T0, \T7
    adcx   \T1, \T6        
    mulx   \T4, \T6, 16\M1
    adox   \T1, \T6  
    adcx   \T5, \T4     
    mulx   \T4, \T7, 24\M1   
    adcx   \T2, \T4        
    adox   \T5, \T7   
    mulx   \T4, \T6, 32\M1   
    adcx   \T4, rax 
    adox   \T2, \T6  
    adox   \T4, rax         
    
    mov    rdx, 32\M0 
    mulx   \T6, \T7, \M1 
    xor    rax, rax
    adcx   \T0, \T7 
    mov    32\C, \T0           // C4_final 
    adcx   \T1, \T6     
    mulx   \T6, \T7, 8\M1
    adox   \T1, \T7 
    adcx   \T5, \T6        
    mulx   \T0, \T6, 16\M1 
    adox   \T5, \T6 
    adcx   \T2, \T0     
    mulx   \T0, \T7, 24\M1   
    adcx   \T4, \T0 
    adox   \T2, \T7  
    mulx   \T0, \T6, 32\M1   
    adcx   \T0, rax           
    adox   \T4, \T6 
    adox   \T0, rax 

    mov    40\C, \T1 
    mov    48\C, \T5 
    mov    56\C, \T2 
    mov    64\C, \T4
    mov    72\C, \T0
.endm

#else

.macro MUL320_SCHOOL M0, M1, C, T0, T1, T2, T3, T4, T5, T6, T7
    mov    rdx, \M0
    mulx   \T0, \T1, \M1    
    mulx   \T2, \T3, 8\M1
    mov    \C, \T1             // C0_final 
    mulx   \T4, \T5, 16\M1 
    add    \T0, \T3               
    adc    \T2, \T5     
    mulx   \T1, \T3, 24\M1
    adc    \T3, \T4         
    mulx   \T5, \T6, 32\M1 
    adc    \T1, \T6     
    adc    \T5, 0        
    
    mov    rdx, 8\M0 
    mulx   \T6, \T7, \M1 
    add    \T0, \T7 
    mov    8\C, \T0            // C1_final 
    adc    \T2, \T6     
    mulx   \T6, \T7, 8\M1
    adc    \T3, \T6        
    mulx   \T0, \T4, 16\M1
    adc    \T0, \T1     
    mulx   \T1, \T6, 24\M1   
    adc    \T5, \T1  
    mulx   \T1, rax, 32\M1     
    adc    \T1, 0 
        
    add    \T2, \T7 
    adc    \T3, \T4  
    adc    \T0, \T6  
    adc    \T5, rax  
    adc    \T1, 0         
    
    mov    rdx, 16\M0 
    mulx   \T4, \T6, \M1 
    add    \T2, \T6 
    mov    16\C, \T2           // C2_final 
    adc    \T3, \T4     
    mulx   \T6, \T7, 8\M1
    adc    \T0, \T6        
    mulx   \T2, \T4, 16\M1 
    adc    \T2, \T5     
    mulx   \T5, \T6, 24\M1   
    adc    \T1, \T5 
    mulx   \T5, rax, 32\M1     
    adc    \T5, 0 
        
    add    \T3, \T7
    adc    \T0, \T4  
    adc    \T2, \T6  
    adc    \T1, rax 
    adc    \T5, 0          
    
    mov    rdx, 24\M0
    mulx   \T4, \T6, \M1 
    add    \T3, \T6 
    mov    24\C, \T3           // C3_final 
    adc    \T0, \T4     
    mulx   \T6, \T7, 8\M1
    adc    \T2, \T6        
    mulx   \T3, \T4, 16\M1 
    adc    \T1, \T3     
    mulx   \T3, \T6, 24\M1   
    adc    \T3, \T5 
    mulx   \T5, rax, 32\M1     
    adc    \T5, 0
        
    add    \T0, \T7
    adc    \T2, \T4  
    adc    \T1, \T6  
    adc    \T3, rax 
    adc    \T5, 0       
    
    mov    rdx, 32\M0 
    mulx   \T4, \T6, \M1 
    add    \T0, \T6 
    mov    32\C, \T0           // C4_final 
    adc    \T2, \T4     
    mulx   \T6, \T7, 8\M1
    adc    \T1, \T6        
    mulx   \T0, \T4, 16\M1 
    adc    \T3, \T0     
    mulx   \T0, \T6, 24\M1   
    adc    \T0, \T5 
    mulx   \T5, rax, 32\M1     
    adc    \T5, 0
        
    add    \T2, \T7 
    adc    \T1, \T4  
    adc    \T3, \T6 
    adc    \T0, rax 
    adc    \T5, 0 
    mov    40\C, \T2 
    mov    48\C, \T1 
    mov    56\C, \T3 
    mov    64\C, \T0
    mov    72\C, \T5 
.endm

#endif


//*****************************************************************************
//  610-bit multiplication using Karatsuba (one level), schoolbook (two levels)
//***************************************************************************** 
.global mul610_asm
mul610_asm:    
    push   r12
    push   r13 
    push   r14 
    push   r15
    mov    rcx, reg_p3 

    // [rsp] <- AH + AL, rax <- mask
    xor    rax, rax
    mov    r8, [reg_p1]
    mov    r9, [reg_p1+8]
    mov    r10, [reg_p1+16]
    mov    r11, [reg_p1+24] 
    mov    r12, [reg_p1+32] 
    push   rbx 
    sub    rsp, 112
    add    r8, [reg_p1+40]
    adc    r9, [reg_p1+48]
    adc    r10, [reg_p1+56]
    adc    r11, [reg_p1+64]
    adc    r12, [reg_p1+72]
    sbb    rax, 0
    mov    [rsp], r8
    mov    [rsp+8], r9
    mov    [rsp+16], r10
    mov    [rsp+24], r11
    mov    [rsp+32], r12

    // [rsp+40] <- BH + BL, rdx <- mask
    xor    rdx, rdx
    mov    r8, [reg_p2]
    mov    r9, [reg_p2+8]
    mov    rbx, [reg_p2+16]
    mov    r13, [reg_p2+24] 
    mov    r14, [reg_p2+32]    
    add    r8, [reg_p2+40]
    adc    r9, [reg_p2+48]
    adc    rbx, [reg_p2+56]
    adc    r13, [reg_p2+64]
    adc    r14, [reg_p2+72]
    sbb    rdx, 0
    mov    [rsp+40], r8
    mov    [rsp+48], r9
    mov    [rsp+56], rbx
    mov    [rsp+64], r13
    mov    [rsp+72], r14     
    
    // [rcx] <- masked (BH + BL)
    and    r8, rax
    and    r9, rax
    and    rbx, rax
    and    r13, rax
    and    r14, rax    
    mov    [rcx], r8
    mov    [rcx+8], r9

    // r8-r12 <- masked (AH + AL)
    mov    r8, [rsp]
    mov    r9, [rsp+8]
    and    r8, rdx
    and    r9, rdx
    and    r10, rdx
    and    r11, rdx
    and    r12, rdx

    // [rsp+80] <- masked (AH + AL) + masked (BH + BL)
    mov    rax, [rcx]
    mov    rdx, [rcx+8]
    add    r8, rax
    adc    r9, rdx
    adc    r10, rbx
    adc    r11, r13
    adc    r12, r14        
    mov    [rsp+80], r8
    mov    [rsp+88], r9
    mov    [rsp+96], r10
    mov    [rsp+104], r11

    // [rcx] <- AL x BL
    MUL320_SCHOOL  [reg_p1], [reg_p2], [rcx], r8, r9, r10, r11, rbx, r13, r14, r15     // Result C0-C4 

    // [rcx+80] <- (AH+AL) x (BH+BL), low part 
    MUL320_SCHOOL  [rsp], [rsp+40], [rcx+80], r8, r9, r10, r11, rbx, r13, r14, r15

    // [rsp] <- AH x BH 
    MUL320_SCHOOL  [reg_p1+40], [reg_p2+40], [rsp], r8, r9, r10, r11, rbx, r13, r14, r15
    
    // r8-r12 <- (AH+AL) x (BH+BL), final step
    mov    r8, [rsp+80]
    mov    r9, [rsp+88]
    mov    r10, [rsp+96]
    mov    r11, [rsp+104]
    mov    rax, [rcx+120]
    add    r8, rax
    mov    rax, [rcx+128]
    adc    r9, rax
    mov    rax, [rcx+136]
    adc    r10, rax
    mov    rax, [rcx+144]
    adc    r11, rax
    mov    rax, [rcx+152]
    adc    r12, rax
    
    // rdi,rdx,rbx,r13,r14,r8-r12 <- (AH+AL) x (BH+BL) - ALxBL
    mov    rdi, [rcx+80]
    sub    rdi, [rcx]
    mov    rdx, [rcx+88]
    sbb    rdx, [rcx+8]
    mov    rbx, [rcx+96]
    sbb    rbx, [rcx+16]
    mov    r13, [rcx+104]
    sbb    r13, [rcx+24]
    mov    r14, [rcx+112]     
    sbb    r14, [rcx+32]  
    sbb    r8, [rcx+40]
    sbb    r9, [rcx+48]
    sbb    r10, [rcx+56]
    sbb    r11, [rcx+64]
    sbb    r12, [rcx+72]
    
    // rdi,rdx,rbx,r13,r14,r8-r12 <- (AH+AL) x (BH+BL) - ALxBL - AHxBH
    sub    rdi, [rsp]
    sbb    rdx, [rsp+8]
    sbb    rbx, [rsp+16]
    sbb    r13, [rsp+24]
    sbb    r14, [rsp+32]  
    sbb    r8, [rsp+40]
    sbb    r9, [rsp+48]
    sbb    r10, [rsp+56]
    sbb    r11, [rsp+64]
    sbb    r12, [rsp+72]
    
    mov    rax, [rcx+40]
    add    rax, rdi
    mov    [rcx+40], rax    // Result C5-C9
    mov    rax, [rcx+48]
    adc    rax, rdx
    mov    [rcx+48], rax 
    mov    rax, [rcx+56]
    adc    rax, rbx
    mov    [rcx+56], rax 
    mov    rax, [rcx+64]
    adc    rax, r13
    mov    [rcx+64], rax 
    mov    rax, [rcx+72]
    adc    rax, r14           
    mov    [rcx+72], rax 
    mov    rax, [rsp]
    adc    r8, rax 
    mov    [rcx+80], r8    // Result C10-C19
    mov    rax, [rsp+8]
    adc    r9, rax
    mov    [rcx+88], r9 
    mov    rax, [rsp+16]
    adc    r10, rax
    mov    [rcx+96], r10 
    mov    rax, [rsp+24]
    adc    r11, rax
    mov    [rcx+104], r11 
    mov    rax, [rsp+32]
    adc    r12, rax
    mov    [rcx+112], r12 
    mov    r8, [rsp+40]
    mov    r9, [rsp+48]
    mov    r10, [rsp+56]
    mov    r11, [rsp+64]
    mov    r12, [rsp+72]
    adc    r8, 0
    adc    r9, 0
    adc    r10, 0
    adc    r11, 0
    adc    r12, 0
    add    rsp, 112   
    mov    [rcx+120], r8 
    mov    [rcx+128], r9 
    mov    [rcx+136], r10 
    mov    [rcx+144], r11 
    mov    [rcx+152], r12 
      
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
.global mul610_asm
mul610_asm:

  ret

# error "CONFIGURATION NOT SUPPORTED. TRY USE_MULX=TRUE"

#endif


#ifdef _MULX_

///////////////////////////////////////////////////////////////// MACRO
// Schoolbook integer multiplication
// Inputs:  memory pointers M0 and M1
// Outputs: regs T0:T7
// Temps:   regs T8
/////////////////////////////////////////////////////////////////

#ifdef _ADX_
.macro MUL128x384_SCHOOL M0, M1, T0, T1, T2, T3, T4, T5, T6, T7, T8
    mov    rdx, \M0
    mulx   \T1, \T0, \M1       // T0 <- C0_final    
    mulx   \T2, \T4, 8\M1
    xor    rax, rax
    mulx   \T3, \T5, 16\M1 
    adox   \T1, \T4               
    adox   \T2, \T5     
    mulx   \T4, \T7, 24\M1
    adox   \T3, \T7         
    mulx   \T5, \T6, 32\M1 
    adox   \T4, \T6         
    mulx   \T7, \T8, 40\M1           
    adox   \T5, \T8         
    adox   \T7, rax   
    
    mov    rdx, 8\M0 
    mulx   \T8, \T6, \M1 
    adcx   \T1, \T6            // T1 <- C1_final 
    adcx   \T2, \T8    
    mulx   \T6, \T8, 8\M1
    adox   \T2, \T8  
    adcx   \T3, \T6        
    mulx   \T6, \T8, 16\M1
    adox   \T3, \T8
    adcx   \T4, \T6     
    mulx   \T6, \T8, 24\M1
    adox   \T4, \T8     
    adcx   \T5, \T6  
    mulx   \T6, \T8, 32\M1 
    adox   \T5, \T8 
    adcx   \T6, \T7 
    mulx   \T7, \T8, 40\M1
    adcx   \T7, rax  
    adox   \T6, \T8          
    adox   \T7, rax
.endm

#else

.macro MUL128x384_SCHOOL M0, M1, T0, T1, T2, T3, T4, T5, T6, T7, T8
    mov    rdx, \M0
    mulx   \T1, \T0, \M1       // T0 <- C0_final 
    mulx   \T2, \T3, 8\M1
    add    \T1, \T3               
    adc    \T2, 0  

    mov    rdx, 8\M0   
    xor    \T5, \T5
    mulx   \T3, \T4, \M1 
    add    \T1, \T4               
    adc    \T2, \T3  
    adc    \T5, 0  
      
    xor    \T6, \T6
    mulx   \T3, \T4, 8\M1
    add    \T2, \T4  
    adc    \T3, \T5           
    adc    \T6, 0 
        
    mov    rdx, \M0         
    mulx   \T4, \T5, 16\M1 
    add    \T2, \T5  
    adc    \T3, \T4           
    adc    \T6, 0  
        
    xor    \T7, \T7        
    mulx   \T4, \T5, 24\M1 
    add    \T3, \T5  
    adc    \T4, \T6           
    adc    \T7, 0  

    mov    rdx, 8\M0 
    mulx   \T5, \T6, 16\M1 
    add    \T3, \T6               
    adc    \T4, \T5  
    adc    \T7, 0    
        
    xor    \T6, \T6        
    mulx   \T5, \T8, 24\M1 
    add    \T4, \T8  
    adc    \T5, \T7           
    adc    \T6, 0  
        
    mov    rdx, \M0        
    mulx   \T7, \T8, 32\M1 
    add    \T4, \T8  
    adc    \T5, \T7           
    adc    \T6, 0      
        
    xor    \T7, \T7        
    mulx   \T8, rax, 40\M1 
    add    \T5, rax  
    adc    \T6, \T8          
    adc    \T7, 0  
        
    mov    rdx, 8\M0        
    mulx   \T8, rax, 32\M1 
    add    \T5, rax  
    adc    \T6, \T8         
    adc    \T7, 0   
        
    mov    rdx, 8\M0        
    mulx   \T8, rax, 40\M1 
    add    \T6, rax  
    adc    \T7, \T8  
.endm
#endif

  
//**************************************************************************************
//  Montgomery reduction
//  Based on method described in Faz-Hernandez et al. https://eprint.iacr.org/2017/1015  
//  Operation: c [reg_p2] = a [reg_p1]
//  NOTE: a=c is not allowed
//************************************************************************************** 
.global rdc610_asm
rdc610_asm:
    push   r12
    push   r13 
    push   r14 
    push   r15  

    // a[0-1] x p610p1_nz --> result: r8:r15 
    MUL128x384_SCHOOL [reg_p1], [rip+p610p1+32], r8, r9, r10, r11, r12, r13, r14, r15, rcx     

    xor    rcx, rcx
    add    r8, [reg_p1+32]  
    adc    r9, [reg_p1+40]  
    adc    r10, [reg_p1+48]   
    adc    r11, [reg_p1+56]   
    adc    r12, [reg_p1+64]   
    adc    r13, [reg_p1+72]   
    adc    r14, [reg_p1+80]  
    adc    r15, [reg_p1+88]   
    adc    rcx, [reg_p1+96] 
    mov    [reg_p1+32], r8  
    mov    [reg_p1+40], r9  
    mov    [reg_p1+48], r10  
    mov    [reg_p1+56], r11  
    mov    [reg_p1+64], r12  
    mov    [reg_p1+72], r13  
    mov    [reg_p1+80], r14
    mov    [reg_p1+88], r15  
    mov    [reg_p1+96], rcx  
    mov    r8, [reg_p1+104]  
    mov    r9, [reg_p1+112]  
    mov    r10, [reg_p1+120]
    mov    r11, [reg_p1+128]
    mov    r12, [reg_p1+136]
    mov    r13, [reg_p1+144]
    mov    r14, [reg_p1+152]
    adc    r8, 0
    adc    r9, 0
    adc    r10, 0
    adc    r11, 0
    adc    r12, 0
    adc    r13, 0
    adc    r14, 0
    mov    [reg_p1+104], r8  
    mov    [reg_p1+112], r9  
    mov    [reg_p1+120], r10  
    mov    [reg_p1+128], r11  
    mov    [reg_p1+136], r12 
    mov    [reg_p1+144], r13 
    mov    [reg_p1+152], r14

    // a[2-3] x p610p1_nz --> result: r8:r15
    MUL128x384_SCHOOL [reg_p1+16], [rip+p610p1+32], r8, r9, r10, r11, r12, r13, r14, r15, rcx 

    xor    rcx, rcx
    add    r8, [reg_p1+48]  
    adc    r9, [reg_p1+56]  
    adc    r10, [reg_p1+64]   
    adc    r11, [reg_p1+72]  
    adc    r12, [reg_p1+80]   
    adc    r13, [reg_p1+88]   
    adc    r14, [reg_p1+96]  
    adc    r15, [reg_p1+104]
    adc    rcx, [reg_p1+112]
    mov    [reg_p1+48], r8  
    mov    [reg_p1+56], r9  
    mov    [reg_p1+64], r10  
    mov    [reg_p1+72], r11   
    mov    [reg_p1+80], r12  
    mov    [reg_p1+88], r13  
    mov    [reg_p1+96], r14
    mov    [reg_p1+104], r15
    mov    [reg_p1+112], rcx
    mov    r8, [reg_p1+120]
    mov    r9, [reg_p1+128]
    mov    r10, [reg_p1+136] 
    mov    r11, [reg_p1+144] 
    mov    r12, [reg_p1+152] 
    adc    r8, 0
    adc    r9, 0
    adc    r10, 0  
    adc    r11, 0 
    adc    r12, 0  
    mov    [reg_p1+120], r8  
    mov    [reg_p1+128], r9  
    mov    [reg_p1+136], r10 
    mov    [reg_p1+144], r11 
    mov    [reg_p1+152], r12 

    // a[4-5] x p610p1_nz --> result: r8:r15
    MUL128x384_SCHOOL [reg_p1+32], [rip+p610p1+32], r8, r9, r10, r11, r12, r13, r14, r15, rcx 

    xor    rcx, rcx
    add    r8, [reg_p1+64]  
    adc    r9, [reg_p1+72]  
    adc    r10, [reg_p1+80]   
    adc    r11, [reg_p1+88]  
    adc    r12, [reg_p1+96]   
    adc    r13, [reg_p1+104]   
    adc    r14, [reg_p1+112]  
    adc    r15, [reg_p1+120]
    adc    rcx, [reg_p1+128]
    mov    [reg_p1+64], r8  
    mov    [reg_p1+72], r9  
    mov    [reg_p1+80], r10  
    mov    [reg_p1+88], r11   
    mov    [reg_p1+96], r12  
    mov    [reg_p1+104], r13  
    mov    [reg_p1+112], r14
    mov    [reg_p1+120], r15
    mov    [reg_p1+128], rcx
    mov    r8, [reg_p1+136]
    mov    r9, [reg_p1+144]
    mov    r10, [reg_p1+152] 
    adc    r8, 0
    adc    r9, 0
    adc    r10, 0 
    mov    [reg_p1+136], r8  
    mov    [reg_p1+144], r9  
    mov    [reg_p1+152], r10 

    // a[6-7] x p610p1_nz --> result: r8:r15
    MUL128x384_SCHOOL [reg_p1+48], [rip+p610p1+32], r8, r9, r10, r11, r12, r13, r14, r15, rcx 

    xor    rcx, rcx
    add    r8, [reg_p1+80]  
    adc    r9, [reg_p1+88]  
    adc    r10, [reg_p1+96]   
    adc    r11, [reg_p1+104]  
    adc    r12, [reg_p1+112]   
    adc    r13, [reg_p1+120]   
    adc    r14, [reg_p1+128]  
    adc    r15, [reg_p1+136]
    adc    rcx, [reg_p1+144]
    mov    [reg_p2], r8         // C0_final
    mov    [reg_p2+8], r9       // C1_final
    mov    [reg_p1+96], r10  
    mov    [reg_p1+104], r11   
    mov    [reg_p1+112], r12  
    mov    [reg_p1+120], r13  
    mov    [reg_p1+128], r14
    mov    [reg_p1+136], r15
    mov    [reg_p1+144], rcx
    mov    r8, [reg_p1+152] 
    adc    r8, 0
    mov    [reg_p1+152], r8

    // a[8-9] x p610p1_nz --> result: r8:r15
    MUL128x384_SCHOOL [reg_p1+64], [rip+p610p1+32], r8, r9, r10, r11, r12, r13, r14, r15, rcx
    
    // Final result C2:C9
    add    r8, [reg_p1+96]  
    adc    r9, [reg_p1+104]  
    adc    r10, [reg_p1+112]   
    adc    r11, [reg_p1+120]  
    adc    r12, [reg_p1+128]   
    adc    r13, [reg_p1+136]   
    adc    r14, [reg_p1+144]   
    adc    r15, [reg_p1+152] 
    mov    [reg_p2+16], r8
    mov    [reg_p2+24], r9  
    mov    [reg_p2+32], r10   
    mov    [reg_p2+40], r11  
    mov    [reg_p2+48], r12  
    mov    [reg_p2+56], r13 
    mov    [reg_p2+64], r14 
    mov    [reg_p2+72], r15

    pop    r15
    pop    r14
    pop    r13
    pop    r12
    ret

  #else
  
//***********************************************************************
//  Montgomery reduction
//  Based on comba method
//  Operation: c [reg_p2] = a [reg_p1]
//  NOTE: a=c is not allowed
//*********************************************************************** 
.global rdc610_asm
rdc610_asm:

  ret

# error "CONFIGURATION NOT SUPPORTED. TRY USE_MULX=TRUE"

  #endif


//***********************************************************************
//  610-bit multiprecision addition
//  Operation: c [reg_p3] = a [reg_p1] + b [reg_p2]
//*********************************************************************** 
.global mp_add610_asm
mp_add610_asm:  
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
//  2x610-bit multiprecision subtraction/addition
//  Operation: c [x2] = a [x0] - b [x1]. If c < 0, add p610*2^640
//*********************************************************************** 
.global mp_subadd610x2_asm
mp_subadd610x2_asm:
  push   r12
  push   r13 
  push   r14 
  push   r15
  push   rbx
  xor    rax, rax
  mov    r8, [reg_p1]
  mov    r9, [reg_p1+8]
  mov    r10, [reg_p1+16]
  mov    r11, [reg_p1+24]
  mov    rcx, [reg_p1+32]
  sub    r8, [reg_p2] 
  sbb    r9, [reg_p2+8] 
  sbb    r10, [reg_p2+16] 
  sbb    r11, [reg_p2+24] 
  sbb    rcx, [reg_p2+32] 
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9
  mov    [reg_p3+16], r10
  mov    [reg_p3+24], r11
  mov    [reg_p3+32], rcx

  mov    r8, [reg_p1+40]
  mov    r9, [reg_p1+48]
  mov    r10, [reg_p1+56] 
  mov    r11, [reg_p1+64]
  mov    rcx, [reg_p1+72] 
  sbb    r8, [reg_p2+40] 
  sbb    r9, [reg_p2+48] 
  sbb    r10, [reg_p2+56]
  sbb    r11, [reg_p2+64] 
  sbb    rcx, [reg_p2+72]
  mov    [reg_p3+40], r8
  mov    [reg_p3+48], r9
  mov    [reg_p3+56], r10
  mov    [reg_p3+64], r11
  mov    [reg_p3+72], rcx
  
  mov    r8, [reg_p1+80]
  mov    r9, [reg_p1+88] 
  mov    r10, [reg_p1+96]
  mov    r11, [reg_p1+104]
  mov    rcx, [reg_p1+112]
  sbb    r8, [reg_p2+80]
  sbb    r9, [reg_p2+88]
  sbb    r10, [reg_p2+96] 
  sbb    r11, [reg_p2+104] 
  sbb    rcx, [reg_p2+112]
  mov    [reg_p3+80], r8 
  mov    [reg_p3+88], r9
  mov    [reg_p3+96], r10
  mov    [reg_p3+104], r11
  mov    [reg_p3+112], rcx
  
  mov    r8, [reg_p1+120]
  mov    r9, [reg_p1+128]
  mov    r10, [reg_p1+136]
  mov    r11, [reg_p1+144]
  mov    rcx, [reg_p1+152]
  sbb    r8, [reg_p2+120] 
  sbb    r9, [reg_p2+128] 
  sbb    r10, [reg_p2+136] 
  sbb    r11, [reg_p2+144] 
  sbb    rcx, [reg_p2+152]
  sbb    rax, 0
  
  // Add p610 anded with the mask in rax 
  mov    r12, [rip+p610]
  mov    r13, [rip+p610+32]
  mov    r14, [rip+p610+40]
  mov    r15, [rip+p610+48]
  mov    rdi, [rip+p610+56]
  mov    rsi, [rip+p610+64]
  mov    rbx, [rip+p610+72]
  and    r12, rax
  and    r13, rax
  and    r14, rax
  and    r15, rax
  and    rdi, rax
  and    rsi, rax
  and    rbx, rax
  mov    rax, [reg_p3+80]
  add    rax, r12
  mov    [reg_p3+80], rax
  mov    rax, [reg_p3+88]
  adc    rax, r12
  mov    [reg_p3+88], rax
  mov    rax, [reg_p3+96]
  adc    rax, r12
  mov    [reg_p3+96], rax
  adc    r12, [reg_p3+104]
  adc    r13, [reg_p3+112]
  mov    [reg_p3+104], r12
  mov    [reg_p3+112], r13
  adc    r8, r14
  adc    r9, r15
  adc    r10, rdi
  adc    r11, rsi
  adc    rcx, rbx
  
  mov    [reg_p3+120], r8
  mov    [reg_p3+128], r9
  mov    [reg_p3+136], r10
  mov    [reg_p3+144], r11
  mov    [reg_p3+152], rcx
  pop    rbx
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret


//***********************************************************************
//  Double 2x610-bit multiprecision subtraction
//  Operation: c [reg_p3] = c [reg_p3] - a [reg_p1] - b [reg_p2]
//*********************************************************************** 
.global mp_dblsub610x2_asm
mp_dblsub610x2_asm:
  push   r12
  push   r13
  push   r14
  push   r15
  push   rbx
  
  xor    rax, rax
  mov    r8, [reg_p3]
  mov    r9, [reg_p3+8]
  mov    r10, [reg_p3+16]
  mov    r11, [reg_p3+24]
  mov    r12, [reg_p3+32]
  mov    r13, [reg_p3+40]
  mov    r14, [reg_p3+48]
  mov    r15, [reg_p3+56]
  mov    rbx, [reg_p3+64]
  mov    rcx, [reg_p3+72]
  sub    r8, [reg_p1]
  sbb    r9, [reg_p1+8] 
  sbb    r10, [reg_p1+16] 
  sbb    r11, [reg_p1+24] 
  sbb    r12, [reg_p1+32] 
  sbb    r13, [reg_p1+40] 
  sbb    r14, [reg_p1+48] 
  sbb    r15, [reg_p1+56]
  sbb    rbx, [reg_p1+64]
  sbb    rcx, [reg_p1+72]
  adc    rax, 0
  sub    r8, [reg_p2]
  sbb    r9, [reg_p2+8] 
  sbb    r10, [reg_p2+16] 
  sbb    r11, [reg_p2+24] 
  sbb    r12, [reg_p2+32] 
  sbb    r13, [reg_p2+40] 
  sbb    r14, [reg_p2+48] 
  sbb    r15, [reg_p2+56]
  sbb    rbx, [reg_p2+64]
  sbb    rcx, [reg_p2+72]
  adc    rax, 0  
  mov    [reg_p3], r8
  mov    [reg_p3+8], r9
  mov    [reg_p3+16], r10
  mov    [reg_p3+24], r11
  mov    [reg_p3+32], r12
  mov    [reg_p3+40], r13
  mov    [reg_p3+48], r14
  mov    [reg_p3+56], r15
  mov    [reg_p3+64], rbx
  mov    [reg_p3+72], rcx
    
  mov    r8, [reg_p3+80]
  mov    r9, [reg_p3+88]
  mov    r10, [reg_p3+96]
  mov    r11, [reg_p3+104]
  mov    r12, [reg_p3+112]
  mov    r13, [reg_p3+120]
  mov    r14, [reg_p3+128]
  mov    r15, [reg_p3+136]
  mov    rbx, [reg_p3+144]
  mov    rcx, [reg_p3+152]
  sub    r8, rax 
  sbb    r8, [reg_p1+80] 
  sbb    r9, [reg_p1+88] 
  sbb    r10, [reg_p1+96] 
  sbb    r11, [reg_p1+104] 
  sbb    r12, [reg_p1+112] 
  sbb    r13, [reg_p1+120] 
  sbb    r14, [reg_p1+128] 
  sbb    r15, [reg_p1+136]
  sbb    rbx, [reg_p1+144]
  sbb    rcx, [reg_p1+152]
  sub    r8, [reg_p2+80] 
  sbb    r9, [reg_p2+88] 
  sbb    r10, [reg_p2+96] 
  sbb    r11, [reg_p2+104] 
  sbb    r12, [reg_p2+112] 
  sbb    r13, [reg_p2+120] 
  sbb    r14, [reg_p2+128] 
  sbb    r15, [reg_p2+136]
  sbb    rbx, [reg_p2+144]
  sbb    rcx, [reg_p2+152]
  mov    [reg_p3+80], r8
  mov    [reg_p3+88], r9
  mov    [reg_p3+96], r10
  mov    [reg_p3+104], r11
  mov    [reg_p3+112], r12
  mov    [reg_p3+120], r13
  mov    [reg_p3+128], r14
  mov    [reg_p3+136], r15
  mov    [reg_p3+144], rbx
  mov    [reg_p3+152], rcx
  
  pop    rbx
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret