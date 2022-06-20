//*******************************************************************************************
// SIDH: an efficient supersingular isogeny cryptography library
// Copyright (c) Microsoft Corporation
//
// Website: https://github.com/microsoft/PQCrypto-SIDH
// Released under MIT license 
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


.text
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


#ifdef _MULX_
#ifdef _ADX_

///////////////////////////////////////////////////////////////// MACRO
// z = a x bi + z
// Inputs: base memory pointer M1 (a),
//         bi pre-stored in rdx,
//         accumulator z in [Z0:Z5] or [Z0:Z8]
// Output: [Z0:Z5] or [Z0:Z8]
// Temps:  regs T0:T1
/////////////////////////////////////////////////////////////////
.macro MULADD64x512 M1, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, Z8, T0, T1, C
	xor    \C, \C
    mulx   \T0, \T1, \M1     // A0*B0
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
    mulx   \T0, \T1, 48\M1   // A0*B6               
    adcx   \Z6, \T1
    adox   \Z7, \T0
    mulx   \T0, \T1, 56\M1   // A0*B7         
    adcx   \Z7, \T1
    adox   \Z8, \T0
    adc    \Z8, 0 
.endm


.macro MULADD64x320 M1, Z0, Z1, Z2, Z3, Z4, Z5, T0, T1
	xor    \T0, \T0
    mulx   \T0, \T1, \M1     // A0*B0
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
    adc    \Z5, 0    
.endm


///////////////////////////////////////////////////////////////// MACRO
// z = a x b + c x d (mod p)
// Inputs: base memory pointers M0 (a,c), M1 (b,d)
//         bi pre-stored in rdx,
//         accumulator z in [Z0:Z8], pre-stores a0 x b
// Output: [Z0:Z8]
// Temps:  regs T0:T1
/////////////////////////////////////////////////////////////////
.macro FPDBLMUL512x512 M00, M01, M10, M11, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, Z8, T0, T1  
    mov    rdx, \M11        
    MULADD64x512 \M01, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \T0, \T1, \T0        
    // [Z1:Z8, Z0] <- z = (z0 x p503p1 + z)/2^64  
    mov    rdx, \Z0          // rdx <- z0 
    MULADD64x320 [rip+fmt(p503p1)+24], \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \T0, \T1
    
    // [Z1:Z8, Z0] <- z = a0 x b01 - a1 x b11 + z 
    mov    rdx, 8\M10
    MULADD64x512 \M00, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \T0, \T1, \Z0
    mov    rdx, 8\M11    
    MULADD64x512 \M01, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \T0, \T1, \T0
    // [Z2:Z8, Z0] <- z = (z0 x p503p1 + z)/2^64  
    mov    rdx, \Z1          // rdx <- z0 
    MULADD64x320 [rip+fmt(p503p1)+24], \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \T0, \T1 
    
    // [Z2:Z8, Z0:Z1] <- z = a0 x b02 - a1 x b12 + z 
    mov    rdx, 16\M10
    MULADD64x512 \M00, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \T0, \T1, \Z1
    mov    rdx, 16\M11    
    MULADD64x512 \M01, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \T0, \T1, \T0
    // [Z3:Z8, Z0:Z1] <- z = (z0 x p503p1 + z)/2^64  
    mov    rdx, \Z2         // rdx <- z0 
    MULADD64x320 [rip+fmt(p503p1)+24], \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \T0, \T1
    
    // [Z3:Z8, Z0:Z2] <- z = a0 x b03 - a1 x b13 + z  
    mov    rdx, 24\M10
    MULADD64x512 \M00, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \T0, \T1, \Z2
    mov    rdx, 24\M11    
    MULADD64x512 \M01, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \T0, \T1, \T0
    // [Z4:Z8, Z0:Z2] <- z = (z0 x p503p1 + z)/2^64  
    mov    rdx, \Z3         // rdx <- z0 
    MULADD64x320 [rip+fmt(p503p1)+24], \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \T0, \T1
    
    // [Z4:Z8, Z0:Z3] <- z = a0 x b04 - a1 x b14 + z 
    mov    rdx, 32\M10
    MULADD64x512 \M00, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \Z3
    mov    rdx, 32\M11    
    MULADD64x512 \M01, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \T0
    // [Z5:Z8, Z0:Z3] <- z = (z0 x p503p1 + z)/2^64   
    mov    rdx, \Z4         // rdx <- z0 
    MULADD64x320 [rip+fmt(p503p1)+24], \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \T0, \T1
    
    // [Z5:Z8, Z0:Z4] <- z = a0 x b05 - a1 x b15 + z
    mov    rdx, 40\M10
    MULADD64x512 \M00, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1, \Z4
    mov    rdx, 40\M11    
    MULADD64x512 \M01, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1, \T0
    // [Z6:Z8, Z0:Z4] <- z = (z0 x p503p1 + z)/2^64  
    mov    rdx, \Z5         // rdx <- z0 
    MULADD64x320 [rip+fmt(p503p1)+24], \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1 
    
    // [Z6:Z8, Z0:Z5] <- z = a0 x b06 - a1 x b16 + z
    mov    rdx, 48\M10
    MULADD64x512 \M00, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1, \Z5
    mov    rdx, 48\M11    
    MULADD64x512 \M01, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1, \T0
    // [Z7, Z0:Z5] <- z = (z0 x p503p1 + z)/2^64  
    mov    rdx, \Z6         // rdx <- z0 
    MULADD64x320 [rip+fmt(p503p1)+24], \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1
    
    // [Z7, Z0:Z6] <- z = a0 x b07 - a1 x b17 + z  
    mov    rdx, 56\M10
    MULADD64x512 \M00, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1, \Z6
    mov    rdx, 56\M11    
    MULADD64x512 \M01, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1, \T0
    // [Z8, Z0:Z6] <- z = (z0 x p503p1 + z)/2^64  
    mov    rdx, \Z7         // rdx <- z0 
    mov    [rcx], \Z8  
    mov    [rcx+8], \Z0 
    MULADD64x320 [rip+fmt(p503p1)+24], \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1 
.endm


//***********************************************************************
//  Multiplication in GF(p^2), non-complex part
//  Operation: c [reg_p3] = a0 x b0 - a1 x b1
//  Inputs: a = [a1, a0] stored in [reg_p1] 
//          b = [b1, b0] stored in [reg_p2] 
//  Output: c stored in [reg_p3]
//***********************************************************************
.global fmt(fp2mul503_c0_asm)
fmt(fp2mul503_c0_asm):     
    push   r12 
    mov    rcx, reg_p3
	
	// [rcx0:7] <- 8*p - b1
	mov    r8, [rip+fmt(p503x8)]  
	mov    r9, [rip+fmt(p503x8)+8]   
	mov    r11, [rip+fmt(p503x8)+24]
	mov    r12, [rip+fmt(p503x8)+32] 
	mov    rax, [reg_p2+64]
	mov    rdx, [reg_p2+72]
	mov    r10, r9                 
	sub    r8, rax
    push   r13 
	sbb    r9, rdx
	mov    rax, [reg_p2+80]
	mov    rdx, [reg_p2+88]
	sbb    r10, rax
    push   r14 
	sbb    r11, rdx
	mov    rax, [reg_p2+96]
	mov    rdx, [reg_p2+104]
	mov    [rcx], r8
	mov    [rcx+8], r9
	mov    r13, [rip+fmt(p503x8)+40]
	mov    r14, [rip+fmt(p503x8)+48]
	sbb    r12, rax
    push   r15 
	sbb    r13, rdx
	mov    rax, [reg_p2+112]
	mov    rdx, [reg_p2+120]
	mov    r15, [rip+fmt(p503x8)+56]
	sbb    r14, rax 
	sbb    r15, rdx 
	mov    [rcx+16], r10
    
    // [r8:r14, rax, r15] <- z = a0 x b00 - a1 x b10
    mov    rdx, [reg_p2]
    mulx   r9, r8, [reg_p1] 
	mov    [rcx+24], r11   
    xor    rax, rax
    mulx   r10, r11, [reg_p1+8]
	mov    [rcx+32], r12     
    adcx   r9, r11        
    mulx   r11, r12, [reg_p1+16]
	mov    [rcx+40], r13     
    adcx   r10, r12        
    mulx   r12, r13, [reg_p1+24] 
	mov    [rcx+48], r14     
    adcx   r11, r13       
    mulx   r13, r14, [reg_p1+32] 
	mov    [rcx+56], r15      
    adcx   r12, r14      
    mulx   r14, rax, [reg_p1+40] 
    push   rbx
    adcx   r13, rax      
    mulx   r15, rax, [reg_p1+48]
    push   rbp 
    adcx   r14, rax     
    mulx   rax, rbx, [reg_p1+56] 
    mov    rdx, [rcx]  
    adcx   r15, rbx     
    adc    rax, 0 

	FPDBLMUL512x512 [reg_p1], [reg_p1+64], [reg_p2], [rcx], r8, r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp 
                  
    mov    [rcx+16], r9        
    mov    [rcx+24], r10      
    mov    [rcx+32], r11      
    mov    [rcx+40], r12      
    mov    [rcx+48], r13    
    mov    [rcx+56], r14
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
.global fmt(fp2mul503_c1_asm)
fmt(fp2mul503_c1_asm):
    mov    rcx, reg_p3
    
    // [r8, r9:r15, rax] <- z = a0 x b10 + a1 x b00
    mov    rdx, [reg_p2]
    mulx   r9, r8, [reg_p1+64]     // a0 x b10
    xor    rax, rax   
    push   r12
    mulx   r10, r11, [reg_p1+72]  
    push   r13  
    adcx   r9, r11        
    mulx   r11, r12, [reg_p1+80]  
    push   r14   
    adcx   r10, r12        
    mulx   r12, r13, [reg_p1+88] 
    push   r15    
    adcx   r11, r13       
    mulx   r13, r14, [reg_p1+96]   
    push   rbx  
    adcx   r12, r14      
    mulx   r14, r15, [reg_p1+104] 
    push   rbp   
    adcx   r13, r15      
    mulx   r15, rbp, [reg_p1+112]
    adcx   r14, rbp     
    mulx   rax, rbx, [reg_p1+120] 
    adcx   r15, rbx     
    adc    rax, 0 

	FPDBLMUL512x512 [reg_p1+64], [reg_p1], [reg_p2], [reg_p2+64], r8, r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp 
           
    mov    [rcx+16], r9 
    mov    [rcx+24], r10   
    mov    [rcx+32], r11      
    mov    [rcx+40], r12                  
    mov    [rcx+48], r13    
    mov    [rcx+56], r14
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


///////////////////////////////////////////////////////////////// MACRO
// z = z + a x b
// Inputs: base memory pointers M0 (a), M1 (b)
//         bi pre-stored in rdx,
//         accumulator z in [Z0:Z8], pre-stores a0 x b
// Output: [Z0:Z7]
// Temps:  regs T0:T1
/////////////////////////////////////////////////////////////////
.macro FPMUL512x512 M0, M1, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, Z8, T0, T1           
    // [Z1:Z7, Z8] <- z = (z0 x p503p1 + z)/2^64
    mov    rdx, \Z0                 // rdx <- z0
    MULADD64x320 [rip+fmt(p503p1)+24], \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \T0, \T1
    
    // [Z1:Z7, Z8, Z0] <- z = a01 x a1 + z  
    mov    rdx, 8\M0
    MULADD64x512 \M1, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \T0, \T1, \Z0
    // [Z2:Z7, Z8, Z0] <- z = (z0 x p503p1 + z)/2^64
    mov    rdx, \Z1                 // rdx <- z0
    MULADD64x320 [rip+fmt(p503p1)+24], \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \T0, \T1
    
    // [Z2:Z7, Z8, Z0:Z1] <- z = a02 x a1 + z 
    mov    rdx, 16\M0
    MULADD64x512 \M1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \T0, \T1, \Z1
    // [Z3:Z7, Z8, Z0:Z1] <- z = (z0 x p503p1 + z)/2^64
    mov    rdx, \Z2                // rdx <- z0
    MULADD64x320 [rip+fmt(p503p1)+24], \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \T0, \T1
    
    // [Z3:Z7, Z8, Z8, Z0:Z2] <- z = a03 x a1 + z 
    mov    rdx, 24\M0
    MULADD64x512 \M1, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \T0, \T1, \Z2
    // [Z4:Z7, Z8, Z0:Z2] <- z = (z0 x p503p1 + z)/2^64
    mov    rdx, \Z3                // rdx <- z0
    MULADD64x320 [rip+fmt(p503p1)+24], \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \T0, \T1
    
    // [Z4:Z7, Z8, Z0:Z3] <- z = a04 x a1 + z 
    mov    rdx, 32\M0
    MULADD64x512 \M1, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \Z3
    // [Z5:Z7, Z8, Z0:Z3] <- z = (z0 x p503p1 + z)/2^64
    mov    rdx, \Z4                // rdx <- z0
    MULADD64x320 [rip+fmt(p503p1)+24], \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \T0, \T1
    
    // [Z5:Z7, Z8, Z0:Z4] <- z = a05 x a1 + z 
    mov    rdx, 40\M0
    MULADD64x512 \M1, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1, \Z4
    // [Z6:Z7, Z8, Z0:Z4] <- z = (z0 x p503p1 + z)/2^64
    mov    rdx, \Z5                // rdx <- z0
    MULADD64x320 [rip+fmt(p503p1)+24], \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1
    
    // [Z6:Z7, Z8, Z0:Z5] <- z = a06 x a1 + z 
    mov    rdx, 48\M0
    MULADD64x512 \M1, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1, \Z5
    // [Z7, Z8, Z0:Z5] <- z = (z0 x p503p1 + z)/2^64
    mov    rdx, \Z6                // rdx <- z0
    MULADD64x320 [rip+fmt(p503p1)+24], \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1       
    
    // [Z7, Z8, Z0:Z6] <- z = a07 x a1 + z 
    mov    rdx, 56\M0
    MULADD64x512 \M1, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1, \Z6
    // [Z8, Z0:Z6] <- z = (z0 x p503p1 + z)/2^64
    mov    rdx, \Z7                // rdx <- z0
    MULADD64x320 [rip+fmt(p503p1)+24], \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1 
.endm


//***********************************************************************
//  Squaring in GF(p^2), non-complex part
//  Operation: c [reg_p2] = (a0+a1) x (a0-a1)
//  Inputs: a = [a1, a0] stored in [reg_p1] 
//  Output: c stored in [reg_p2]
//***********************************************************************
.global fmt(fp2sqr503_c0_asm)
fmt(fp2sqr503_c0_asm):   
    push   r12
    push   r13

	// a0 + a1
	mov    r8, [reg_p1]
	mov    r9, [reg_p1+8]
	mov    r10, [reg_p1+16]
	mov    r11, [reg_p1+24]
	mov    r12, [reg_p1+32]
	mov    r13, [reg_p1+40]
	add    r8, [reg_p1+64]
	adc    r9, [reg_p1+72] 
    push   r14 
	adc    r10, [reg_p1+80]
	adc    r11, [reg_p1+88] 
    push   r15
	adc    r12, [reg_p1+96]
	adc    r13, [reg_p1+104]
	mov    r14, [reg_p1+48]
	mov    r15, [reg_p1+56]
	adc    r14, [reg_p1+112]
	adc    r15, [reg_p1+120]
	mov    [reg_p2+8], r9
	mov    [reg_p2+16], r10
	mov    [reg_p2+24], r11
	mov    [reg_p2+32], r12
	mov    [reg_p2+40], r13
	mov    [reg_p2+48], r14
	mov    [reg_p2+56], r15
	
	// a0 - a1 + 4xp503
	mov    rcx, [reg_p1]
	mov    r10, [reg_p1+8]
	mov    r12, [reg_p1+16]
	mov    r13, [reg_p1+24]
	mov    r14, [reg_p1+32]
	mov    r15, [reg_p1+40]
	sub    rcx, [reg_p1+64]
	sbb    r10, [reg_p1+72]  
    push   rbx 
	sbb    r12, [reg_p1+80]
	sbb    r13, [reg_p1+88] 
    push   rbp
	sbb    r14, [reg_p1+96]
	sbb    r15, [reg_p1+104]
	mov    rbx, [reg_p1+48]
	mov    rbp, [reg_p1+56]
	sbb    rbx, [reg_p1+112]
	sbb    rbp, [reg_p1+120]
	add    rcx, [rip+fmt(p503x4)]                    
	mov    rdx, [rip+fmt(p503x4)+8]
	adc    r10, rdx
	adc    r12, rdx
	adc    r13, [rip+fmt(p503x4)+24]
	adc    r14, [rip+fmt(p503x4)+32]
	adc    r15, [rip+fmt(p503x4)+40]
	adc    rbx, [rip+fmt(p503x4)+48]
	adc    rbp, [rip+fmt(p503x4)+56]
	mov    [reg_p2+64], rcx                 
	mov    [reg_p2+72], r10
    
    // [r8:r15, rbp] <- z = a00 x a1
    mov    rdx, r8
    mulx   r9, r8, rcx   
    xor    rax, rax 
	mov    [reg_p2+80], r12  
    mulx   r10, r11, r10  
	mov    [reg_p2+88], r13 
    adox   r9, r11        
    mulx   r11, r12, r12  
	mov    [reg_p2+96], r14 
    adox   r10, r12        
    mulx   r12, r13, r13 
	mov    [reg_p2+104], r15  
    adox   r11, r13       
    mulx   r13, r14, r14  
	mov    [reg_p2+112], rbx 
    adox   r12, r14      
    mulx   r14, r15, r15 
	mov    [reg_p2+120], rbp 
    adox   r13, r15      
    mulx   r15, rbp, rbx  
    adox   r14, rbp      
    mulx   rbp, rbx, [reg_p2+120]  
    adox   r15, rbx 
    adox   rbp, rax
           
	FPMUL512x512 [reg_p2], [reg_p2+64], r8, r9, r10, r11, r12, r13, r14, r15, rbp, rbx, rcx
                 
    mov    [reg_p2], rbp
    mov    [reg_p2+8], r8         
    mov    [reg_p2+16], r9      
    mov    [reg_p2+24], r10     
    mov    [reg_p2+32], r11     
    mov    [reg_p2+40], r12     
    mov    [reg_p2+48], r13
    mov    [reg_p2+56], r14
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
.global fmt(fp2sqr503_c1_asm)
fmt(fp2sqr503_c1_asm):   
    push   r12
    push   r13 
	
	mov    r8, [reg_p1]
	mov    r9, [reg_p1+8]
	mov    r10, [reg_p1+16]
	mov    r11, [reg_p1+24]
	mov    r12, [reg_p1+32]
	mov    r13, [reg_p1+40] 
	add    r8, r8
	adc    r9, r9
    push   r14 
	adc    r10, r10
	adc    r11, r11 
    push   r15 
	adc    r12, r12
	mov    r14, [reg_p1+48] 
	mov    r15, [reg_p1+56] 
	adc    r13, r13 
    push   rbx 
	adc    r14, r14 
    push   rbp
	adc    r15, r15
	sub    rsp, 64
	mov    [rsp+8], r9
	mov    [rsp+16], r10
    
    // [r8:r15, rbp] <- z = a00 x a1
    mov    rdx, r8
    mulx   r9, r8, [reg_p1+64]  
	mov    [rsp+24], r11  
    xor    rax, rax   
    mulx   r10, r11, [reg_p1+72]  
	mov    [rsp+32], r12 
    adox   r9, r11        
    mulx   r11, r12, [reg_p1+80] 
	mov    [rsp+40], r13  
    adox   r10, r12        
    mulx   r12, r13, [reg_p1+88]  
	mov    [rsp+48], r14 
    adox   r11, r13       
    mulx   r13, r14, [reg_p1+96] 
	mov    [rsp+56], r15  
    adox   r12, r14      
    mulx   r14, r15, [reg_p1+104]   
    adox   r13, r15      
    mulx   r15, rbp, [reg_p1+112]  
    adox   r14, rbp      
    mulx   rbp, rbx, [reg_p1+120]  
    adox   r15, rbx 
    adox   rbp, rax
           
	FPMUL512x512 [rsp], [reg_p1+64], r8, r9, r10, r11, r12, r13, r14, r15, rbp, rbx, rcx
         
    mov    [reg_p2], rbp         
    mov    [reg_p2+8], r8         
    mov    [reg_p2+16], r9      
    mov    [reg_p2+24], r10     
    mov    [reg_p2+32], r11     
    mov    [reg_p2+40], r12     
    mov    [reg_p2+48], r13
    mov    [reg_p2+56], r14
	add    rsp, 64
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
.global fmt(fpmul503_asm)
fmt(fpmul503_asm): 
    mov    rcx, reg_p3
     
    // [r8:r15] <- z = a x b0
    mov    rdx, [reg_p2]
    mulx   r9, r8, [reg_p1]  
    push   r12
    xor    rax, rax 
    mulx   r10, r11, [reg_p1+8]
    push   r13 
    adox   r9, r11        
    mulx   r11, r12, [reg_p1+16]
    push   r14 
    adox   r10, r12        
    mulx   r12, r13, [reg_p1+24]   
    push   r15 
    adox   r11, r13       
    mulx   r13, r14, [reg_p1+32]    
    push   rbx   
    adox   r12, r14      
    mulx   r14, r15, [reg_p1+40]  
    push   rbp
    adox   r13, r15      
    mulx   r15, rbx, [reg_p1+48]  
    adox   r14, rbx      
    mulx   rbx, rbp, [reg_p1+56]  
    adox   r15, rbp 
    adox   rax, rbx 

	FPMUL512x512 [reg_p2], [reg_p1], r8, r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp

    mov    [rcx], rax 
    mov    [rcx+8], r8         
    mov    [rcx+16], r9         
    mov    [rcx+24], r10      
    mov    [rcx+32], r11      
    mov    [rcx+40], r12      
    mov    [rcx+48], r13     
    mov    [rcx+56], r14
    pop    rbp
    pop    rbx
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    ret