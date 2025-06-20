#include <sqisign_namespace.h>
.intel_syntax noprefix

.set pbytes,32
.set plimbs,4

#ifdef __APPLE__
.section __TEXT,__const
#else
.section .rodata
#endif
p_plus_1: .quad 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x4100000000000000

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
  xor    rax, rax
  mov    r8, [rsi]
  mov    r9, [rsi+8]
  mov    r10, [rsi+16]
  mov    r11, [rsi+24]
  mov    r12, [rsi+32]
  mov    r13, [rsi+40]
  add    r8, [rdx] 
  adc    r9, [rdx+8] 
  adc    r10, [rdx+16] 
  adc    r11, [rdx+24]
  adc    r12, [rdx+32]
  adc    r13, [rdx+40]
  mov    rax, r13
  sar    rax, 63
  mov    rdx, [rip+p+40]
  and    rdx, rax
  sub    r8, rax
  sbb    r9, rax
  sbb    r10, rax
  sbb    r11, rax
  sbb    r12, rax
  sbb    r13, rdx
  
  mov    rax, r13
  sar    rax, 63
  mov    rdx, [rip+p+40]
  and    rdx, rax
  sub    r8, rax
  sbb    r9, rax
  sbb    r10, rax
  sbb    r11, rax
  sbb    r12, rax
  sbb    r13, rdx
  mov    [rdi], r8
  mov    [rdi+8], r9 
  mov    [rdi+16], r10 
  mov    [rdi+24], r11
  mov    [rdi+32], r12
  mov    [rdi+40], r13
  pop    r13
  pop    r12
  ret

.global fp_sub
fp_sub:
  push   r12 
  push   r13
  xor    rax, rax
  mov    r8, [rsi]
  mov    r9, [rsi+8]
  mov    r10, [rsi+16]
  mov    r11, [rsi+24]
  mov    r12, [rsi+32]
  mov    r13, [rsi+40]
  sub    r8, [rdx] 
  sbb    r9, [rdx+8] 
  sbb    r10, [rdx+16] 
  sbb    r11, [rdx+24]
  sbb    r12, [rdx+32]
  sbb    r13, [rdx+40]
  sbb    rax, 0
  
  mov    rdx, [rip+p+40]
  and    rdx, rax  
  add    r8, rax  
  adc    r9, rax
  adc    r10, rax 
  adc    r11, rax
  adc    r12, rax
  adc    r13, rdx
  
  mov    rax, r13
  sar    rax, 63  
  mov    rdx, [rip+p+40]
  and    rdx, rax  
  add    r8, rax  
  adc    r9, rax
  adc    r10, rax 
  adc    r11, rax
  adc    r12, rax
  adc    r13, rdx

  mov    [rdi], r8
  mov    [rdi+8], r9 
  mov    [rdi+16], r10 
  mov    [rdi+24], r11 
  mov    [rdi+32], r12
  mov    [rdi+40], r13
  pop    r13
  pop    r12
  ret
  
///////////////////////////////////////////////////////////////// MACROS
// z = a x bi + z
// Inputs: base memory pointer M1 (a),
//         bi pre-stored in rdx,
//         accumulator z in [Z0:Z6]
// Output: [Z0:Z6]
// Temps:  regs T0:T1
/////////////////////////////////////////////////////////////////
.macro MULADD64x384 M1, Z0, Z1, Z2, Z3, Z4, Z5, Z6, T0, T1, C
    mulx   \T0, \T1, \M1     // A0*B0
    xor    \C, \C
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

.macro MULADD64x64 M1, Z0, Z1, Z2, Z3, Z4, Z5, T0, T1
    mulx   \T0, \T1, \M1     // A0*B0
    xor    rax, rax
    adox   \Z4, \T1
    adox   \Z5, \T0
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
    mov    rcx, rdx
    sub    rsp, 96
	
	// [rdi0:5] <- 2p - b1
	mov    r8, [rip+p2] 
	mov    r9, [rip+p2+8] 
	mov    r10, r9
	mov    r11, r9
	mov    r12, r9
	mov    r13, [rip+p2+40]
	mov    rax, [rcx+48]
	mov    rdx, [rcx+56]
	sub    r8, rax
	sbb    r9, rdx
	mov    rax, [rcx+64]
	mov    rdx, [rcx+72]
	sbb    r10, rax
	sbb    r11, rdx
	mov    rax, [rcx+80]
	mov    rdx, [rcx+88]
	sbb    r12, rax
	sbb    r13, rdx
	mov    [rdi], r8
	mov    [rdi+8], r9
	mov    [rdi+16], r10
	mov    [rdi+24], r11
	mov    [rdi+32], r12
	mov    [rdi+40], r13

    // Correcting a to [0,p)
    xor    rax, rax
    mov    r8, [rsi+48]
    mov    r9, [rsi+56]
    mov    r10, [rsi+64]
    mov    r11, [rsi+72]
    mov    r12, [rsi+80]
    mov    r13, [rsi+88]
    mov    rbx, [rip+p]
    mov    rdx, [rip+p+40]
    sub    r8, rbx
    sbb    r9, rbx
    sbb    r10, rbx
    sbb    r11, rbx
    sbb    r12, rbx
    sbb    r13, rdx
    sbb    rax, 0
    and    rdx, rax
    add    r8, rax
    adc    r9, rax
    adc    r10, rax
    adc    r11, rax
    adc    r12, rax
    adc    r13, rdx
    mov    [rsp+48], r8
    mov    [rsp+56], r9
    mov    [rsp+64], r10
    mov    [rsp+72], r11
    mov    [rsp+80], r12
    mov    [rsp+88], r13
    
    xor    rax, rax
    mov    r8, [rsi]
    mov    r10, [rsi+8]
    mov    r12, [rsi+16]
    mov    r13, [rsi+24]
    mov    r14, [rsi+32]
    mov    r15, [rsi+40]
    mov    rdx, [rip+p+40]
    sub    r8, rbx
    sbb    r10, rbx
    sbb    r12, rbx
    sbb    r13, rbx
    sbb    r14, rbx
    sbb    r15, rdx
    sbb    rax, 0
    and    rdx, rax
    add    r8, rax
    adc    r10, rax
    adc    r12, rax
    adc    r13, rax
    adc    r14, rax
    adc    r15, rdx
    mov    [rsp], r8
    mov    [rsp+8], r10
    mov    [rsp+16], r12
    mov    [rsp+24], r13
    mov    [rsp+32], r14
    mov    [rsp+40], r15

    // [r8:r14] <- z = a0 x b00 - a1 x b10
    mov    rdx, [rcx]
    mulx   r9, r8, r8   
    xor    rax, rax
    mulx   r10, r11, r10  
    adox   r9, r11        
    mulx   r11, r12, r12  
    adox   r10, r12        
    mulx   r12, r13, r13  
    adox   r11, r13        
    mulx   r13, r14, r14
    adox   r12, r14      
    mulx   r14, r15, r15
    adox   r13, r15 
    adox   r14, rax

    mov    rdx, [rdi]    
    MULADD64x384 [rsp+48], r8, r9, r10, r11, r12, r13, r14, r15, rbx, rax
    // [r9:r14] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r8                 // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r9, r10, r11, r12, r13, r14, r15, rbx
    
    // [r9:r14, r8] <- z = a0 x b01 - a1 x b11 + z
    mov    rdx, [rcx+8]
    MULADD64x384 [rsp], r9, r10, r11, r12, r13, r14, r8, r15, rbx, r8
    mov    rdx, [rdi+8]    
    MULADD64x384 [rsp+48], r9, r10, r11, r12, r13, r14, r8, r15, rbx, rax
    // [r10:r14, r8] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r9                 // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r10, r11, r12, r13, r14, r8, r15, rbx
    
    // [r10:r14, r8:r9] <- z = a0 x b02 - a1 x b12 + z 
    mov    rdx, [rcx+16]
    MULADD64x384 [rsp], r10, r11, r12, r13, r14, r8, r9, r15, rbx, r9
    mov    rdx, [rdi+16]    
    MULADD64x384 [rsp+48], r10, r11, r12, r13, r14, r8, r9, r15, rbx, rax
    // [r11:r14, r8:r9] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r10                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r11, r12, r13, r14, r8, r9, r15, rbx
    
    // [r11:r14, r8:r10] <- z = a0 x b03 - a1 x b13 + z 
    mov    rdx, [rcx+24]
    MULADD64x384 [rsp], r11, r12, r13, r14, r8, r9, r10, r15, rbx, r10
    mov    rdx, [rdi+24]    
    MULADD64x384 [rsp+48], r11, r12, r13, r14, r8, r9, r10, r15, rbx, rax
    // [r14, r8:r10] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r11                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r12, r13, r14, r8, r9, r10, r15, rbx
    
    // [r12:r14, r8:r11] <- z = a0 x b04 - a1 x b14 + z 
    mov    rdx, [rcx+32]
    MULADD64x384 [rsp], r12, r13, r14, r8, r9, r10, r11, r15, rbx, r11
    mov    rdx, [rdi+32]    
    MULADD64x384 [rsp+48], r12, r13, r14, r8, r9, r10, r11, r15, rbx, rax
    // [r14, r8:r11] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r12                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r13, r14, r8, r9, r10, r11, r15, rbx
    
    // [r13:r14, r8:r12] <- z = a0 x b05 - a1 x b15 + z 
    mov    rdx, [rcx+40]
    MULADD64x384 [rsp], r13, r14, r8, r9, r10, r11, r12, r15, rbx, r12
    mov    rdx, [rdi+40]    
    MULADD64x384 [rsp+48], r13, r14, r8, r9, r10, r11, r12, r15, rbx, rax
    // [r14, r8:r12] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r13                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r14, r8, r9, r10, r11, r12, r15, rbx

	// Final correction   
    mov    rax, r12
    sar    rax, 63
    mov    rdx, [rip+p+40]
    and    rdx, rax
    sub    r14, rax
    sbb    r8, rax
    sbb    r9, rax
    sbb    r10, rax
    sbb    r11, rax
    sbb    r12, rdx

    mov    [rdi], r14          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12 
    add    rsp, 96
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
    mov    rcx, rdx
	sub    rsp, 96

    // Correcting a to [0,p)
    xor    rax, rax
    mov    r8, [rsi+48]
    mov    r9, [rsi+56]
    mov    r10, [rsi+64]
    mov    r11, [rsi+72]
    mov    r12, [rsi+80]
    mov    r13, [rsi+88]
    mov    rbx, [rip+p]
    mov    rdx, [rip+p+40]
    sub    r8, rbx
    sbb    r9, rbx
    sbb    r10, rbx
    sbb    r11, rbx
    sbb    r12, rbx
    sbb    r13, rdx
    sbb    rax, 0
    and    rdx, rax
    add    r8, rax
    adc    r9, rax
    adc    r10, rax
    adc    r11, rax
    adc    r12, rax
    adc    r13, rdx
    mov    [rsp+48], r8
    mov    [rsp+56], r9
    mov    [rsp+64], r10
    mov    [rsp+72], r11
    mov    [rsp+80], r12
    mov    [rsp+88], r13
    
    xor    rax, rax
    mov    r8, [rsi]
    mov    r10, [rsi+8]
    mov    r12, [rsi+16]
    mov    r13, [rsi+24]
    mov    r14, [rsi+32]
    mov    r15, [rsi+40]
    mov    rdx, [rip+p+40]
    sub    r8, rbx
    sbb    r10, rbx
    sbb    r12, rbx
    sbb    r13, rbx
    sbb    r14, rbx
    sbb    r15, rdx
    sbb    rax, 0
    and    rdx, rax
    add    r8, rax
    adc    r10, rax
    adc    r12, rax
    adc    r13, rax
    adc    r14, rax
    adc    r15, rdx
    mov    [rsp], r8
    mov    [rsp+8], r10
    mov    [rsp+16], r12
    mov    [rsp+24], r13
    mov    [rsp+32], r14
    mov    [rsp+40], r15
    
    // [r8:r14] <- z = a0 x b10 + a1 x b00
    mov    rdx, [rcx+48]
    mulx   r9, r8, r8   
    xor    rax, rax
    mulx   r10, r11, r10  
    adox   r9, r11        
    mulx   r11, r12, r12  
    adox   r10, r12        
    mulx   r12, r13, r13  
    adox   r11, r13        
    mulx   r13, r14, r14
    adox   r12, r14      
    mulx   r14, r15, r15
    adox   r13, r15 
    adox   r14, rax

    mov    rdx, [rcx]    
    MULADD64x384 [rsp+48], r8, r9, r10, r11, r12, r13, r14, r15, rbx, rax
    // [r9:r14] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r8                 // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r9, r10, r11, r12, r13, r14, r15, rbx
    
    // [r9:r14, r8] <- z = a0 x b01 - a1 x b11 + z
    mov    rdx, [rcx+56]
    MULADD64x384 [rsi], r9, r10, r11, r12, r13, r14, r8, r15, rbx, r8
    mov    rdx, [rcx+8]    
    MULADD64x384 [rsp+48], r9, r10, r11, r12, r13, r14, r8, r15, rbx, rax
    // [r10:r14, r8] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r9                 // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r10, r11, r12, r13, r14, r8, r15, rbx
    
    // [r10:r14, r8:r9] <- z = a0 x b02 - a1 x b12 + z 
    mov    rdx, [rcx+64]
    MULADD64x384 [rsp], r10, r11, r12, r13, r14, r8, r9, r15, rbx, r9
    mov    rdx, [rcx+16]    
    MULADD64x384 [rsp+48], r10, r11, r12, r13, r14, r8, r9, r15, rbx, rax
    // [r11:r14, r8:r9] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r10                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r11, r12, r13, r14, r8, r9, r15, rbx
    
    // [r11:r14, r8:r10] <- z = a0 x b03 - a1 x b13 + z 
    mov    rdx, [rcx+72]
    MULADD64x384 [rsp], r11, r12, r13, r14, r8, r9, r10, r15, rbx, r10
    mov    rdx, [rcx+24]    
    MULADD64x384 [rsp+48], r11, r12, r13, r14, r8, r9, r10, r15, rbx, rax
    // [r14, r8:r10] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r11                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r12, r13, r14, r8, r9, r10, r15, rbx
    
    // [r12:r14, r8:r11] <- z = a0 x b04 - a1 x b14 + z 
    mov    rdx, [rcx+80]
    MULADD64x384 [rsp], r12, r13, r14, r8, r9, r10, r11, r15, rbx, r11
    mov    rdx, [rcx+32]    
    MULADD64x384 [rsp+48], r12, r13, r14, r8, r9, r10, r11, r15, rbx, rax
    // [r14, r8:r11] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r12                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r13, r14, r8, r9, r10, r11, r15, rbx
    
    // [r13:r14, r8:r12] <- z = a0 x b05 - a1 x b15 + z 
    mov    rdx, [rcx+88]
    MULADD64x384 [rsp], r13, r14, r8, r9, r10, r11, r12, r15, rbx, r12
    mov    rdx, [rcx+40]    
    MULADD64x384 [rsp+48], r13, r14, r8, r9, r10, r11, r12, r15, rbx, rax
    // [r14, r8:r12] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, r13                // rdx <- z0 
    MULADD64x64 [rip+p_plus_1+40], r14, r8, r9, r10, r11, r12, r15, rbx

	// Final correction   
    mov    rax, r12
    sar    rax, 63
    mov    rdx, [rip+p+40]
    and    rdx, rax
    sub    r14, rax
    sbb    r8, rax
    sbb    r9, rax
    sbb    r10, rax
    sbb    r11, rax
    sbb    r12, rdx
   
    mov    [rdi], r14          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12
    add    rsp, 96
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
//         accumulator z in [Z0:Z6], pre-stores a0 x b
// Output: [Z0:Z6]
// Temps:  regs T0:T1
/////////////////////////////////////////////////////////////////
.macro FPMUL384x384 M0, M1, Z0, Z1, Z2, Z3, Z4, Z5, Z6, T0, T1           
    // [Z1:Z6] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z0                 // rdx <- z0
    MULADD64x64 [rip+p_plus_1+40], \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \T0, \T1
    
    // [Z1:Z6, Z0] <- z = a01 x a1 + z 
    mov    rdx, 8\M0
    MULADD64x384 \M1, \Z1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z0, \T0, \T1, \Z0
    // [Z2:Z6, Z0] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z1                 // rdx <- z0
    MULADD64x64 [rip+p_plus_1+40], \Z2, \Z3, \Z4, \Z5, \Z6, \Z0, \T0, \T1
    
    // [Z2:Z6, Z0:Z1] <- z = a02 x a1 + z  
    mov    rdx, 16\M0
    MULADD64x384 \M1, \Z2, \Z3, \Z4, \Z5, \Z6, \Z0, \Z1, \T0, \T1, \Z1
    // [Z3:Z6, Z0:Z1] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z2                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+40], \Z3, \Z4, \Z5, \Z6, \Z0, \Z1, \T0, \T1
    
    // [Z3:Z6, Z0:Z2] <- z = a03 x a1 + z
    mov    rdx, 24\M0
    MULADD64x384 \M1, \Z3, \Z4, \Z5, \Z6, \Z0, \Z1, \Z2, \T0, \T1, \Z2
    // [Z4:Z6, Z0:Z2] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z3                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+40], \Z4, \Z5, \Z6, \Z0, \Z1, \Z2, \T0, \T1
    
    // [Z4:Z6, Z0:Z3] <- z = a04 x a1 + z
    mov    rdx, 32\M0
    MULADD64x384 \M1, \Z4, \Z5, \Z6, \Z0, \Z1, \Z2, \Z3, \T0, \T1, \Z3
    // [Z5:Z6, Z0:Z3] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z4                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+40], \Z5, \Z6, \Z0, \Z1, \Z2, \Z3, \T0, \T1
    
    // [Z5:Z6, Z0:Z4] <- z = a05 x a1 + z
    mov    rdx, 40\M0
    MULADD64x384 \M1, \Z5, \Z6, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1, \Z4
    // [Z6, Z0:Z4] <- z = (z0 x p_plus_1 + z)/2^64
    mov    rdx, \Z5                // rdx <- z0
    MULADD64x64 [rip+p_plus_1+40], \Z6, \Z0, \Z1, \Z2, \Z3, \Z4, \T0, \T1
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

	// a0 + a1
	mov    rdx, [rsi]
	mov    r9, [rsi+8]
	mov    r10, [rsi+16]
	mov    r11, [rsi+24]
	mov    r12, [rsi+32]
	mov    r13, [rsi+40]
	add    rdx, [rsi+48]
	adc    r9, [rsi+56]
	adc    r10, [rsi+64]
	adc    r11, [rsi+72]
	adc    r12, [rsi+80]
	adc    r13, [rsi+88]
	mov    [rdi], rdx
	mov    [rdi+8], r9
	mov    [rdi+16], r10
	mov    [rdi+24], r11
	mov    [rdi+32], r12
	mov    [rdi+40], r13
	
	// a0 - a1
	mov    r8, [rsi]
	mov    r10, [rsi+8]
	mov    r12, [rsi+16]
	mov    r13, [rsi+24]
	mov    r14, [rsi+32]
	mov    r15, [rsi+40]  
    xor    rax, rax
	sub    r8, [rsi+48]
	sbb    r10, [rsi+56]
	sbb    r12, [rsi+64]
	sbb    r13, [rsi+72]
	sbb    r14, [rsi+80]
	sbb    r15, [rsi+88]
    sbb    rax, 0
  
    mov    rcx, [rip+p+40]
    and    rcx, rax  
    add    r8, rax  
    adc    r10, rax
    adc    r12, rax 
    adc    r13, rax
    adc    r14, rax
    adc    r15, rcx
  
    mov    rax, r15
    sar    rax, 63
    mov    rcx, [rip+p+40]
    and    rcx, rax  
    add    r8, rax  
    adc    r10, rax
    adc    r12, rax 
    adc    r13, rax
    adc    r14, rax
    adc    r15, rcx
    
	mov    [rdi+48], r8               
	mov    [rdi+56], r10 
	mov    [rdi+64], r12 
	mov    [rdi+72], r13 
	mov    [rdi+80], r14 
	mov    [rdi+88], r15 
    
    // [r8:r14] <- z = a00 x a1
    mulx   r9, r8, r8   
    xor    rax, rax
    mulx   r10, r11, r10  
    adox   r9, r11        
    mulx   r11, r12, r12  
    adox   r10, r12        
    mulx   r12, r13, r13  
    adox   r11, r13        
    mulx   r13, r14, r14
    adox   r12, r14      
    mulx   r14, r15, r15
    adox   r13, r15 
    adox   r14, rax

    FPMUL384x384 [rdi], [rdi+48], r8, r9, r10, r11, r12, r13, r14, r15, rcx

	// Final correction   
    mov    rax, r12
    sar    rax, 63
    mov    rdx, [rip+p+40]
    and    rdx, rax
    sub    r14, rax
    sbb    r8, rax
    sbb    r9, rax
    sbb    r10, rax
    sbb    r11, rax
    sbb    r12, rdx

    mov    [rdi], r14          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12 
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
	
	mov    rdx, [rsi]
	mov    r9, [rsi+8]
	mov    r10, [rsi+16]
	mov    r11, [rsi+24]
	mov    r12, [rsi+32]
	mov    r13, [rsi+40]
	add    rdx, rdx
	adc    r9, r9
	adc    r10, r10
	adc    r11, r11
	adc    r12, r12
	adc    r13, r13
	sub    rsp, 48
	mov    [rsp+8], r9
	mov    [rsp+16], r10 
	mov    [rsp+24], r11   
	mov    [rsp+32], r12 
	mov    [rsp+40], r13 
    
    // [r8:r12] <- z = a00 x a1
    mulx   r9, r8, [rsi+48]
    xor    rax, rax 
    mulx   r10, r11, [rsi+56]
    adox   r9, r11        
    mulx   r11, r12, [rsi+64]
    adox   r10, r12        
    mulx   r12, r13, [rsi+72]
    adox   r11, r13        
    mulx   r13, r14, [rsi+80]
    adox   r12, r14      
    mulx   r14, r15, [rsi+88]
    adox   r13, r15 
    adox   r14, rax

	FPMUL384x384 [rsp], [rsi+48], r8, r9, r10, r11, r12, r13, r14, r15, rcx
	add    rsp, 48

	// Final correction   
    mov    rax, r12
    sar    rax, 63
    mov    rdx, [rip+p+40]
    and    rdx, rax
    sub    r14, rax
    sbb    r8, rax
    sbb    r9, rax
    sbb    r10, rax
    sbb    r11, rax
    sbb    r12, rdx
    
    mov    [rdi], r14          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12 
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
    mov    rcx, rdx 
     
    // [r8:r14] <- z = a x b0
    mov    rdx, [rcx]
    mulx   r9, r8, [rsi]         
    xor    rax, rax
    mulx   r10, r11, [rsi+8]
    adox   r9, r11        
    mulx   r11, r12, [rsi+16] 
    adox   r10, r12        
    mulx   r12, r13, [rsi+24]
    adox   r11, r13      
    mulx   r13, r14, [rsi+32]
    adox   r12, r14      
    mulx   r14, r15, [rsi+40]   
    adox   r13, r15 
    adox   r14, rax

	FPMUL384x384 [rcx], [rsi], r8, r9, r10, r11, r12, r13, r14, r15, rbx

	// Final correction   
    mov    rax, r12
    sar    rax, 63
    mov    rdx, [rip+p+40]
    and    rdx, rax
    sub    r14, rax
    sbb    r8, rax
    sbb    r9, rax
    sbb    r10, rax
    sbb    r11, rax
    sbb    r12, rdx
    
    mov    [rdi], r14          
    mov    [rdi+8], r8         
    mov    [rdi+16], r9         
    mov    [rdi+24], r10        
    mov    [rdi+32], r11        
    mov    [rdi+40], r12 
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
