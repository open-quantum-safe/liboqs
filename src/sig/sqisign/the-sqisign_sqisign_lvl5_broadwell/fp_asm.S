#include <sqisign_namespace.h>
.intel_syntax noprefix

.set pbytes,32
.set plimbs,4

#ifdef __APPLE__
.section __TEXT,__const
#else
.section .rodata
#endif
p_plus_1: .quad 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x01B0000000000000

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",@progbits
#endif

#include <asm_preamble.h>

.text
.p2align 4,,15

.global fp_add
fp_add:
  push   r12 
  push   r13
  push   r14
  push   r15
  xor    rax, rax
  mov    r8, [rsi]
  mov    r9, [rsi+8]
  mov    r10, [rsi+16]
  mov    r11, [rsi+24]
  mov    r12, [rsi+32]
  mov    r13, [rsi+40]
  mov    r14, [rsi+48]
  mov    r15, [rsi+56]
  add    r8, [rdx] 
  adc    r9, [rdx+8] 
  adc    r10, [rdx+16] 
  adc    r11, [rdx+24]
  adc    r12, [rdx+32]
  adc    r13, [rdx+40]
  adc    r14, [rdx+48]
  adc    r15, [rdx+56]
  mov    rax, r15
  shr    rax, 57
  neg    rax
  mov    rdx, [rip+p+56]
  and    rdx, rax
  sub    r8, rax
  sbb    r9, rax
  sbb    r10, rax
  sbb    r11, rax
  sbb    r12, rax
  sbb    r13, rax
  sbb    r14, rax
  sbb    r15, rdx
  
  mov    rax, r15
  shr    rax, 57
  neg    rax
  mov    rdx, [rip+p+56]
  and    rdx, rax
  sub    r8, rax
  sbb    r9, rax
  sbb    r10, rax
  sbb    r11, rax
  sbb    r12, rax
  sbb    r13, rax
  sbb    r14, rax
  sbb    r15, rdx
  mov    [rdi], r8
  mov    [rdi+8], r9 
  mov    [rdi+16], r10 
  mov    [rdi+24], r11
  mov    [rdi+32], r12
  mov    [rdi+40], r13
  mov    [rdi+48], r14
  mov    [rdi+56], r15
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret

.global fp_sub
fp_sub:
  push   r12 
  push   r13
  push   r14
  push   r15
  xor    rax, rax
  mov    r8, [rsi]
  mov    r9, [rsi+8]
  mov    r10, [rsi+16]
  mov    r11, [rsi+24]
  mov    r12, [rsi+32]
  mov    r13, [rsi+40]
  mov    r14, [rsi+48]
  mov    r15, [rsi+56]
  sub    r8, [rdx] 
  sbb    r9, [rdx+8] 
  sbb    r10, [rdx+16] 
  sbb    r11, [rdx+24]
  sbb    r12, [rdx+32]
  sbb    r13, [rdx+40]
  sbb    r14, [rdx+48]
  sbb    r15, [rdx+56]
  sbb    rax, 0
  
  mov    rdx, [rip+p+56]
  and    rdx, rax  
  add    r8, rax  
  adc    r9, rax
  adc    r10, rax 
  adc    r11, rax
  adc    r12, rax
  adc    r13, rax
  adc    r14, rax
  adc    r15, rdx
  
  mov    rax, r15
  sar    rax, 57
  mov    rdx, [rip+p+56]
  and    rdx, rax  
  add    r8, rax  
  adc    r9, rax
  adc    r10, rax 
  adc    r11, rax
  adc    r12, rax
  adc    r13, rax
  adc    r14, rax
  adc    r15, rdx

  mov    [rdi], r8
  mov    [rdi+8], r9 
  mov    [rdi+16], r10 
  mov    [rdi+24], r11 
  mov    [rdi+32], r12
  mov    [rdi+40], r13
  mov    [rdi+48], r14
  mov    [rdi+56], r15
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  ret

///////////////////////////////////////////////////////////////// MACRO
// z = a x bi + z
// Inputs: base memory pointer M1 (a),
//         bi pre-stored in rdx,
//         accumulator z in [Z0:Z8]
// Output: [Z0:Z8]
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

.macro MULADD64x64 M1, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, T0, T1
    xor    \T0, \T0
    mulx   \T0, \T1, \M1     // A0*B0
    adox   \Z6, \T1
    adox   \Z7, \T0
.endm

//***********************************************************************
//  Multiplication in GF(p^2), non-complex part
//  Operation: c [rdi] = a0 x b0 - a1 x b1
//  Inputs: a = [a1, a0] stored in [rsi] 
//          b = [b1, b0] stored in [rdx] 
//  Output: c stored in [rdi]
//***********************************************************************
.global fp2_mul_c0
fp2_mul_c0:    
    push   r12 
    push   r13 
    push   r14  
    push   r15   
    push   rbx  
    push   rbp
    mov    rcx, rdx
	
	// [rdi0:7] <- 2p - b1
	mov    r8, [rip+p2] 
	mov    r9, [rip+p2+8] 
	mov    r10, r9
	mov    r11, r9
	mov    r12, r9
	mov    r13, r9
	mov    r14, r9
	mov    r15, [rip+p2+56]
	mov    rax, [rcx+64]
	mov    rdx, [rcx+72]
	sub    r8, rax
	sbb    r9, rdx
	mov    rax, [rcx+80]
	mov    rdx, [rcx+88]
	sbb    r10, rax
	sbb    r11, rdx
	mov    rax, [rcx+96]
	mov    rdx, [rcx+104]
	sbb    r12, rax
	sbb    r13, rdx
	mov    rax, [rcx+112]
	mov    rdx, [rcx+120]
	sbb    r14, rax
	sbb    r15, rdx
	mov    [rdi], r8
	mov    [rdi+8], r9
	mov    [rdi+16], r10
	mov    [rdi+24], r11
	mov    [rdi+32], r12
	mov    [rdi+40], r13
	mov    [rdi+48], r14
	mov    [rdi+56], r15
    
    // [r8:r15, rax] <- z = a0 x b00 - a1 x b10
    mov    rdx, [rcx]
    mulx   r9, r8, [rsi]
    xor    rax, rax 
    mulx   r10, r11, [rsi+8]
    adcx   r9, r11        
    mulx   r11, r12, [rsi+16]
    adcx   r10, r12        
    mulx   r12, r13, [rsi+24]
    adcx   r11, r13        
    mulx   r13, r14, [rsi+32]
    adcx   r12, r14      
    mulx   r14, r15, [rsi+40]
    adcx   r13, r15     
    mulx   r15, rbp, [rsi+48]
    adcx   r14, rbp     
    mulx   rax, rbx, [rsi+56] 
    adcx   r15, rbx     
    adc    rax, 0

    mov    rdx, [rdi]    
    MULADD64x512 [rsi+64], r8, r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp, rbx
    // [r9:r14] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r8                 // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp
    
    // [r9:r15, rax, r8] <- z = a0 x b01 - a1 x b11 + z
    mov    rdx, [rcx+8]
    MULADD64x512 [rsi], r9, r10, r11, r12, r13, r14, r15, rax, r8, rbx, rbp, r8
    mov    rdx, [rdi+8]    
    MULADD64x512 [rsi+64], r9, r10, r11, r12, r13, r14, r15, rax, r8, rbx, rbp, rbx
    // [r10:r15, rax, r8] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r9                 // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r10, r11, r12, r13, r14, r15, rax, r8, rbx, rbp
    
    // [r10:r15, rax, r8:r9] <- z = a0 x b02 - a1 x b12 + z 
    mov    rdx, [rcx+16]
    MULADD64x512 [rsi], r10, r11, r12, r13, r14, r15, rax, r8, r9, rbx, rbp, r9
    mov    rdx, [rdi+16]    
    MULADD64x512 [rsi+64], r10, r11, r12, r13, r14, r15, rax, r8, r9, rbx, rbp, rbx
    // [r11:r15, rax, r8:r9] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r10                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r11, r12, r13, r14, r15, rax, r8, r9, rbx, rbp
    
    // [r11:r15, rax, r8:r10] <- z = a0 x b03 - a1 x b13 + z 
    mov    rdx, [rcx+24]
    MULADD64x512 [rsi], r11, r12, r13, r14, r15, rax, r8, r9, r10, rbx, rbp, r10
    mov    rdx, [rdi+24]    
    MULADD64x512 [rsi+64], r11, r12, r13, r14, r15, rax, r8, r9, r10, rbx, rbp, rbx
    // [r12:r15, rax, r8:r10] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r11                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r12, r13, r14, r15, rax, r8, r9, r10, rbx, rbp
    
    // [r12:r15, rax, r8:r11] <- z = a0 x b04 - a1 x b14 + z 
    mov    rdx, [rcx+32]
    MULADD64x512 [rsi], r12, r13, r14, r15, rax, r8, r9, r10, r11, rbx, rbp, r11
    mov    rdx, [rdi+32]    
    MULADD64x512 [rsi+64], r12, r13, r14, r15, rax, r8, r9, r10, r11, rbx, rbp, rbx
    // [r13:r15, rax, r8:r11] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r12                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r13, r14, r15, rax, r8, r9, r10, r11, rbx, rbp
    
    // [r13:r15, rax, r8:r12] <- z = a0 x b05 - a1 x b15 + z 
    mov    rdx, [rcx+40]
    MULADD64x512 [rsi], r13, r14, r15, rax, r8, r9, r10, r11, r12, rbx, rbp, r12
    mov    rdx, [rdi+40]    
    MULADD64x512 [rsi+64], r13, r14, r15, rax, r8, r9, r10, r11, r12, rbx, rbp, rbx
    // [r14:r15, rax, r8:r12] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r13                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56],  r14, r15, rax, r8, r9, r10, r11, r12, rbx, rbp
    
    // [r14:r15, rax, r8:r12] <- z = a0 x b06 - a1 x b16 + z 
    mov    rdx, [rcx+48]
    MULADD64x512 [rsi], r14, r15, rax, r8, r9, r10, r11, r12, r13, rbx, rbp, r13
    mov    rdx, [rdi+48]    
    MULADD64x512 [rsi+64], r14, r15, rax, r8, r9, r10, r11, r12, r13, rbx, rbp, rbx
    // [r15, rax, r8:r13] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r14               // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56],  r15, rax, r8, r9, r10, r11, r12, r13, rbx, rbp
    
    // [r15, rax, r8:r12] <- z = a0 x b06 - a1 x b16 + z 
    mov    rdx, [rcx+56]
    MULADD64x512 [rsi], r15, rax, r8, r9, r10, r11, r12, r13, r14, rbx, rbp, r14
    mov    rdx, [rdi+56]    
    MULADD64x512 [rsi+64], r15, rax, r8, r9, r10, r11, r12, r13, r14, rbx, rbp, rbx
    // [rax, r8:r14] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r15                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], rax, r8, r9, r10, r11, r12, r13, r14, rbx, rbp

    mov    [rdi], rax          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12        
    mov    [rdi+48], r13        
    mov    [rdi+56], r14 
    pop    rbp
    pop    rbx
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    ret
  
//***********************************************************************
//  Multiplication in GF(p^2), complex part
//  Operation: c [rdi] = a0 x b1 + a1 x b0
//  Inputs: a = [a1, a0] stored in [rsi] 
//          b = [b1, b0] stored in [rdx] 
//  Output: c stored in [rdi]
//***********************************************************************
.global fp2_mul_c1
fp2_mul_c1:    
    push   r12 
    push   r13 
    push   r14    
    push   r15   
    push   rbx
    push   rbp
    mov    rcx, rdx
    
    // [r8:r15, rax] <- z = a0 x b10 + a1 x b00
    mov    rdx, [rcx+64]
    mulx   r9, r8, [rsi]
    xor    rax, rax 
    mulx   r10, r11, [rsi+8]
    adcx   r9, r11        
    mulx   r11, r12, [rsi+16]
    adcx   r10, r12        
    mulx   r12, r13, [rsi+24]
    adcx   r11, r13        
    mulx   r13, r14, [rsi+32]
    adcx   r12, r14      
    mulx   r14, r15, [rsi+40]
    adcx   r13, r15     
    mulx   r15, rbp, [rsi+48]
    adcx   r14, rbp     
    mulx   rax, rbx, [rsi+56] 
    adcx   r15, rbx     
    adc    rax, 0

    mov    rdx, [rcx]    
    MULADD64x512 [rsi+64], r8, r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp, rbx
    // [r9:r14] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r8                 // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp
    
    // [r9:r15, rax, r8] <- z = a0 x b01 - a1 x b11 + z
    mov    rdx, [rcx+72]
    MULADD64x512 [rsi], r9, r10, r11, r12, r13, r14, r15, rax, r8, rbx, rbp, r8
    mov    rdx, [rcx+8]    
    MULADD64x512 [rsi+64], r9, r10, r11, r12, r13, r14, r15, rax, r8, rbx, rbp, rbx
    // [r10:r15, rax, r8] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r9                 // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r10, r11, r12, r13, r14, r15, rax, r8, rbx, rbp
    
    // [r10:r15, rax, r8:r9] <- z = a0 x b02 - a1 x b12 + z 
    mov    rdx, [rcx+80]
    MULADD64x512 [rsi], r10, r11, r12, r13, r14, r15, rax, r8, r9, rbx, rbp, r9
    mov    rdx, [rcx+16]    
    MULADD64x512 [rsi+64], r10, r11, r12, r13, r14, r15, rax, r8, r9, rbx, rbp, rbx
    // [r11:r15, rax, r8:r9] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r10                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r11, r12, r13, r14, r15, rax, r8, r9, rbx, rbp
    
    // [r11:r15, rax, r8:r10] <- z = a0 x b03 - a1 x b13 + z 
    mov    rdx, [rcx+88]
    MULADD64x512 [rsi], r11, r12, r13, r14, r15, rax, r8, r9, r10, rbx, rbp, r10
    mov    rdx, [rcx+24]    
    MULADD64x512 [rsi+64], r11, r12, r13, r14, r15, rax, r8, r9, r10, rbx, rbp, rbx
    // [r12:r15, rax, r8:r10] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r11                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r12, r13, r14, r15, rax, r8, r9, r10, rbx, rbp
    
    // [r12:r15, rax, r8:r11] <- z = a0 x b04 - a1 x b14 + z 
    mov    rdx, [rcx+96]
    MULADD64x512 [rsi], r12, r13, r14, r15, rax, r8, r9, r10, r11, rbx, rbp, r11
    mov    rdx, [rcx+32]    
    MULADD64x512 [rsi+64], r12, r13, r14, r15, rax, r8, r9, r10, r11, rbx, rbp, rbx
    // [r13:r15, rax, r8:r11] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r12                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], r13, r14, r15, rax, r8, r9, r10, r11, rbx, rbp
    
    // [r13:r15, rax, r8:r12] <- z = a0 x b05 - a1 x b15 + z 
    mov    rdx, [rcx+104]
    MULADD64x512 [rsi], r13, r14, r15, rax, r8, r9, r10, r11, r12, rbx, rbp, r12
    mov    rdx, [rcx+40]    
    MULADD64x512 [rsi+64], r13, r14, r15, rax, r8, r9, r10, r11, r12, rbx, rbp, rbx
    // [r14:r15, rax, r8:r12] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r13                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56],  r14, r15, rax, r8, r9, r10, r11, r12, rbx, rbp
    
    // [r14:r15, rax, r8:r12] <- z = a0 x b06 - a1 x b16 + z 
    mov    rdx, [rcx+112]
    MULADD64x512 [rsi], r14, r15, rax, r8, r9, r10, r11, r12, r13, rbx, rbp, r13
    mov    rdx, [rcx+48]    
    MULADD64x512 [rsi+64], r14, r15, rax, r8, r9, r10, r11, r12, r13, rbx, rbp, rbx
    // [r15, rax, r8:r13] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r14                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56],  r15, rax, r8, r9, r10, r11, r12, r13, rbx, rbp
    
    // [r15, rax, r8:r12] <- z = a0 x b06 - a1 x b16 + z 
    mov    rdx, [rcx+120]
    MULADD64x512 [rsi], r15, rax, r8, r9, r10, r11, r12, r13, r14, rbx, rbp, r14
    mov    rdx, [rcx+56]    
    MULADD64x512 [rsi+64], r15, rax, r8, r9, r10, r11, r12, r13, r14, rbx, rbp, rbx
    // [rax, r8:r14] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r15                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+56], rax, r8, r9, r10, r11, r12, r13, r14, rbx, rbp

    mov    [rdi], rax          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12        
    mov    [rdi+48], r13        
    mov    [rdi+56], r14 
    pop    rbp
    pop    rbx
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    ret

///////////////////////////////////////////////////////////////// MACRO
// z = a x b (mod p)
// Inputs: base memory pointers M0 (a), M1 (b)
//         bi pre-stored in rdx,
//         accumulator z in [Z0:Z8], pre-stores a0 x b
// Output: [Z0:Z8]
// Temps:  regs T0:T1
/////////////////////////////////////////////////////////////////
.macro FPMUL512x512 M0, M1, Z0, Z1, Z2, Z3, Z4, Z5, Z6, Z7, Z8, T0, T1           
    // [Z1:Z8] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z0                 // rdx <- z0
    MULADD64x64 [rip+p_plus_1+56], \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \T0, \T1
    
    // [Z1:Z8, Z0] <- z = a01 x a1 + z 
    mov    rdx, 8\M0
    MULADD64x512 \M1, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \T0, \T1, \Z0
    // [Z2:Z8, Z0] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z1                 // rdx <- z0
    MULADD64x64 [rip+p_plus_1+56], \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \T0, \T1
    
    // [Z2:Z8, Z0:Z1] <- z = a02 x a1 + z  
    mov    rdx, 16\M0
    MULADD64x512 \M1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \T0, \T1, \Z1
    // [Z3:Z8, Z0:Z1] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z2                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+56], \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \T0, \T1
    
    // [Z3:Z8, Z0:Z2] <- z = a03 x a1 + z
    mov    rdx, 24\M0
    MULADD64x512 \M1, \Z3, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \T0, \T1, \Z2
    // [Z4:Z8, Z0:Z2] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z3                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+56], \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \T0, \T1
    
    // [Z4:Z8, Z0:Z3] <- z = a04 x a1 + z
    mov    rdx, 32\M0
    MULADD64x512 \M1, \Z4, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \Z3
    // [Z5:Z8, Z0:Z3] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z4                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+56], \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \T0, \T1
    
    // [Z5:Z8, Z0:Z4] <- z = a05 x a1 + z
    mov    rdx, 40\M0
    MULADD64x512 \M1, \Z5, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1, \Z4
    // [Z6:Z8, Z0:Z4] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z5                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+56], \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1
    
    // [Z6:Z8, Z0:Z5] <- z = a06 x a1 + z
    mov    rdx, 48\M0
    MULADD64x512 \M1, \Z6, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1, \Z5
    // [Z7:Z8, Z0:Z5] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z6                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+56], \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \T0, \T1
    
    // [Z7:Z8, Z0:Z6] <- z = a07 x a1 + z
    mov    rdx, 56\M0
    MULADD64x512 \M1, \Z7, \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1, \Z6
    // [Z8, Z0:Z6] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z7                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+56], \Z8, \Z0, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1
.endm

//***********************************************************************
//  Squaring in GF(p^2), non-complex part
//  Operation: c [rdi] = (a0+a1) x (a0-a1)
//  Inputs: a = [a1, a0] stored in [rsi] 
//  Output: c stored in [rdi]
//***********************************************************************
.global fp2_sq_c0
fp2_sq_c0:   
    push   r12 
    push   r13
    push   r14    
    push   r15     
    push   rbx     
    push   rbp   

	// a0 + a1
	mov    rdx, [rsi]
	mov    r9, [rsi+8]
	mov    r10, [rsi+16]
	mov    r11, [rsi+24]
	mov    r12, [rsi+32]
	mov    r13, [rsi+40]
	mov    r14, [rsi+48]
	mov    r15, [rsi+56]
	add    rdx, [rsi+64]
	adc    r9, [rsi+72]
	adc    r10, [rsi+80]
	adc    r11, [rsi+88]
	adc    r12, [rsi+96]
	adc    r13, [rsi+104]
	adc    r14, [rsi+112]
	adc    r15, [rsi+120]
	mov    [rdi], rdx
	mov    [rdi+8], r9
	mov    [rdi+16], r10
	mov    [rdi+24], r11
	mov    [rdi+32], r12
	mov    [rdi+40], r13
	mov    [rdi+48], r14
	mov    [rdi+56], r15
	
	// a0 - a1 + 2p
	mov    r8, [rsi]
	mov    r10, [rsi+8]
	mov    r12, [rsi+16]
	mov    r13, [rsi+24]
	mov    r14, [rsi+32]
	mov    r15, [rsi+40]
	mov    rbx, [rsi+48]
	mov    rbp, [rsi+56]
	sub    r8, [rsi+64]
	sbb    r10, [rsi+72]
	sbb    r12, [rsi+80]
	sbb    r13, [rsi+88]
	sbb    r14, [rsi+96]
	sbb    r15, [rsi+104]
	sbb    rbx, [rsi+112]
	sbb    rbp, [rsi+120]
	mov    rax, [rip+p2]
	add    r8, rax   
	mov    rax, [rip+p2+8]                 
	adc    r10, rax
	adc    r12, rax
	adc    r13, rax
	adc    r14, rax
	adc    r15, rax
	adc    rbx, rax
	adc    rbp, [rip+p2+56]
	mov    [rdi+64], r8               
	mov    [rdi+72], r10 
	mov    [rdi+80], r12 
	mov    [rdi+88], r13 
	mov    [rdi+96], r14 
	mov    [rdi+104], r15 
	mov    [rdi+112], rbx 
	mov    [rdi+120], rbp 
    
    // [r8:r15, rax] <- z = a00 x a1
    mulx   r9, r8, r8   
    xor    rax, rax
    mulx   r10, r11, r10  
    adcx   r9, r11        
    mulx   r11, r12, r12  
    adcx   r10, r12        
    mulx   r12, r13, r13  
    adcx   r11, r13        
    mulx   r13, r14, r14
    adcx   r12, r14      
    mulx   r14, r15, r15
    adcx   r13, r15     
    mulx   r15, rbx, rbx
    adcx   r14, rbx     
    mulx   rax, rbp, rbp 
    adcx   r15, rbp     
    adc    rax, 0

    FPMUL512x512 [rdi], [rdi+64], r8, r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp

    mov    [rdi], rax          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12        
    mov    [rdi+48], r13        
    mov    [rdi+56], r14 
    pop    rbp
    pop    rbx
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    ret

//***********************************************************************
//  Squaring in GF(p^2), complex part
//  Operation: c [rdi] = 2a0 x a1
//  Inputs: a = [a1, a0] stored in [reg_p1] 
//  Output: c stored in [rdi]
//***********************************************************************
.global fp2_sq_c1
fp2_sq_c1:  
    push   r12 
    push   r13
    push   r14    
    push   r15     
    push   rbx     
    push   rbp
	
	mov    rdx, [rsi]
	mov    r9, [rsi+8]
	mov    r10, [rsi+16]
	mov    r11, [rsi+24]
	mov    r12, [rsi+32]
	mov    r13, [rsi+40]
	mov    r14, [rsi+48]
	mov    r15, [rsi+56]
	add    rdx, rdx
	adc    r9, r9
	adc    r10, r10
	adc    r11, r11
	adc    r12, r12
	adc    r13, r13
	adc    r14, r14
	adc    r15, r15
	sub    rsp, 64
	mov    [rsp+8], r9
	mov    [rsp+16], r10 
	mov    [rsp+24], r11   
	mov    [rsp+32], r12 
	mov    [rsp+40], r13 
	mov    [rsp+48], r14 
	mov    [rsp+56], r15 
    
    // [r8:r15, rax] <- z = a00 x a1
    mulx   r9, r8, [rsi+64]
    xor    rax, rax 
    mulx   r10, r11, [rsi+72]
    adcx   r9, r11        
    mulx   r11, r12, [rsi+80]
    adcx   r10, r12        
    mulx   r12, r13, [rsi+88]
    adcx   r11, r13        
    mulx   r13, r14, [rsi+96]
    adcx   r12, r14      
    mulx   r14, r15, [rsi+104]
    adcx   r13, r15     
    mulx   r15, rbp, [rsi+112]
    adcx   r14, rbp     
    mulx   rax, rbx, [rsi+120] 
    adcx   r15, rbx     
    adc    rax, 0 

	FPMUL512x512 [rsp], [rsi+64], r8, r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp
	add    rsp, 64

    mov    [rdi], rax          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12        
    mov    [rdi+48], r13        
    mov    [rdi+56], r14 
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
//  Inputs: a stored in [rsi], b stored in [rdx] 
//  Output: c stored in [rdi]
//***********************************************************************
.global fp_mul
fp_mul: 
    push   r12
    push   r13 
    push   r14    
    push   r15   
    push   rbx  
    push   rbp
    mov    rcx, rdx 
     
    // [r8:r15, rax] <- z = a x b0
    mov    rdx, [rcx]
    mulx   r9, r8, [rsi]         
    xor    rax, rax
    mulx   r10, r11, [rsi+8]
    adcx   r9, r11        
    mulx   r11, r12, [rsi+16] 
    adcx   r10, r12        
    mulx   r12, r13, [rsi+24]
    adcx   r11, r13      
    mulx   r13, r14, [rsi+32]
    adcx   r12, r14      
    mulx   r14, r15, [rsi+40]  
    adcx   r13, r15      
    mulx   r15, rbp, [rsi+48]
    adcx   r14, rbp     
    mulx   rax, rbx, [rsi+56] 
    adcx   r15, rbx     
    adc    rax, 0

	FPMUL512x512 [rcx], [rsi], r8, r9, r10, r11, r12, r13, r14, r15, rax, rbx, rbp

    mov    [rdi], rax          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12        
    mov    [rdi+48], r13        
    mov    [rdi+56], r14 
    pop    rbp
    pop    rbx
    pop    r15
    pop    r14
    pop    r13
    pop    r12
    ret
    
.global fp_sqr
fp_sqr:
    mov rdx, rsi
    jmp fp_mul
