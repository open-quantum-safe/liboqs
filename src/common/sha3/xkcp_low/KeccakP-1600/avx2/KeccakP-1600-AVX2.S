# The eXtended Keccak Code Package (XKCP)
# https://github.com/XKCP/XKCP
#
# Copyright (c) 2006-2017, CRYPTOGAMS by <appro@openssl.org>
# Copyright (c) 2017 Ronny Van Keer
# All rights reserved.
#
# The source code in this file is licensed under the CRYPTOGAMS license.
# For further details see http://www.openssl.org/~appro/cryptogams/.
#
# Notes:
# The code for the permutation (__KeccakF1600) was generated with
# Andy Polyakov's keccak1600-avx2.pl from the CRYPTOGAMS project
# (https://github.com/dot-asm/cryptogams/blob/master/x86_64/keccak1600-avx2.pl).
# The rest of the code was written by Ronny Van Keer.
# Adaptations for macOS by Stéphane Léon.
# Adaptations for mingw-w64 (changes macOS too) by Jorrit Jongma.

#ifdef ADD_SYMBOL_SUFFIX
#define KeccakP1600_Initialize KeccakP1600_Initialize_avx2
#define KeccakP1600_AddByte KeccakP1600_AddByte_avx2
#define KeccakP1600_AddBytes KeccakP1600_AddBytes_avx2
#define KeccakP1600_OverwriteBytes KeccakP1600_OverwriteBytes_avx2
#define KeccakP1600_OverwriteWithZeroes KeccakP1600_OverwriteWithZeroes_avx2
#define KeccakP1600_Permute_Nrounds KeccakP1600_Permute_Nrounds_avx2
#define KeccakP1600_Permute_12rounds KeccakP1600_Permute_12rounds_avx2
#define KeccakP1600_Permute_24rounds KeccakP1600_Permute_24rounds_avx2
#define KeccakP1600_ExtractBytes KeccakP1600_ExtractBytes_avx2
#define KeccakP1600_ExtractAndAddBytes KeccakP1600_ExtractAndAddBytes_avx2
#define KeccakF1600_FastLoop_Absorb KeccakF1600_FastLoop_Absorb_avx2
#define KeccakP1600_12rounds_FastLoop_Absorb KeccakP1600_12rounds_FastLoop_Absorb_avx2

#define _KeccakP1600_Initialize _KeccakP1600_Initialize_avx2
#define _KeccakP1600_AddByte _KeccakP1600_AddByte_avx2
#define _KeccakP1600_AddBytes _KeccakP1600_AddBytes_avx2
#define _KeccakP1600_OverwriteBytes _KeccakP1600_OverwriteBytes_avx2
#define _KeccakP1600_OverwriteWithZeroes _KeccakP1600_OverwriteWithZeroes_avx2
#define _KeccakP1600_Permute_Nrounds _KeccakP1600_Permute_Nrounds_avx2
#define _KeccakP1600_Permute_12rounds _KeccakP1600_Permute_12rounds_avx2
#define _KeccakP1600_Permute_24rounds _KeccakP1600_Permute_24rounds_avx2
#define _KeccakP1600_ExtractBytes _KeccakP1600_ExtractBytes_avx2
#define _KeccakP1600_ExtractAndAddBytes _KeccakP1600_ExtractAndAddBytes_avx2
#define _KeccakF1600_FastLoop_Absorb _KeccakF1600_FastLoop_Absorb_avx2
#define _KeccakP1600_12rounds_FastLoop_Absorb _KeccakP1600_12rounds_FastLoop_Absorb_avx2
#endif

.text

# -----------------------------------------------------------------------------
#
# void KeccakP1600_Initialize(void *state);
#
.globl  KeccakP1600_Initialize
.globl _KeccakP1600_Initialize
#ifndef old_gas_syntax
.type   KeccakP1600_Initialize,@function
#endif
KeccakP1600_Initialize:
_KeccakP1600_Initialize:
.balign 32
    vpxor       %ymm0,%ymm0,%ymm0
    vmovdqa     %ymm0,0*32(%rdi)
    vmovdqa     %ymm0,1*32(%rdi)
    vmovdqa     %ymm0,2*32(%rdi)
    vmovdqa     %ymm0,3*32(%rdi)
    vmovdqa     %ymm0,4*32(%rdi)
    vmovdqa     %ymm0,5*32(%rdi)
    movq        $0,6*32(%rdi)
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_Initialize,.-KeccakP1600_Initialize
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_AddByte(void *state, unsigned char data, unsigned int offset);
#                                %rdi                 %rsi               %rdx
#
.globl  KeccakP1600_AddByte
.globl _KeccakP1600_AddByte
#ifndef old_gas_syntax
.type   KeccakP1600_AddByte,@function
#endif
KeccakP1600_AddByte:
_KeccakP1600_AddByte:
.balign 32
    mov         %rdx, %rax
    and         $7, %rax
    and         $0xFFFFFFF8, %edx
    lea         mapState(%rip), %r9
    mov         (%r9, %rdx), %rdx
    add         %rdx, %rdi
    add         %rax, %rdi
    xorb        %sil, (%rdi)
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_AddByte,.-KeccakP1600_AddByte
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
#                                %rdi                         %rsi               %rdx                 %rcx
#
.globl  KeccakP1600_AddBytes
.globl _KeccakP1600_AddBytes
#ifndef old_gas_syntax
.type   KeccakP1600_AddBytes,@function
#endif
KeccakP1600_AddBytes:
_KeccakP1600_AddBytes:
.balign 32
    cmp         $0, %rcx
    jz          KeccakP1600_AddBytes_Exit
    mov         %rdx, %rax                              # rax offset in lane
    and         $0xFFFFFFF8, %edx                       # rdx pointer into state index mapper
    lea         mapState(%rip), %r9
    add         %r9, %rdx
    and         $7, %rax
    jz          KeccakP1600_AddBytes_LaneAlignedCheck
    mov         $8, %r9                                 # r9 is (max) length of incomplete lane
    sub         %rax, %r9
    cmp         %rcx, %r9
    cmovae      %rcx, %r9
    sub         %r9, %rcx                               # length -= length of incomplete lane
    add         (%rdx), %rax                            # rax = pointer to state lane
    add         $8, %rdx
    add         %rdi, %rax
KeccakP1600_AddBytes_NotAlignedLoop:
    mov         (%rsi), %r8b
    inc         %rsi
    xorb        %r8b, (%rax)
    inc         %rax
    dec         %r9
    jnz         KeccakP1600_AddBytes_NotAlignedLoop
    jmp         KeccakP1600_AddBytes_LaneAlignedCheck
KeccakP1600_AddBytes_LaneAlignedLoop:
    mov         (%rsi), %r8
    add         $8, %rsi
    mov         (%rdx), %rax
    add         $8, %rdx
    add         %rdi, %rax
    xor         %r8, (%rax)
KeccakP1600_AddBytes_LaneAlignedCheck:
    sub         $8, %rcx
    jnc         KeccakP1600_AddBytes_LaneAlignedLoop
KeccakP1600_AddBytes_LastIncompleteLane:
    add         $8, %rcx
    jz          KeccakP1600_AddBytes_Exit
    mov         (%rdx), %rax
    add         %rdi, %rax
KeccakP1600_AddBytes_LastIncompleteLaneLoop:
    mov         (%rsi), %r8b
    inc         %rsi
    xor         %r8b, (%rax)
    inc         %rax
    dec         %rcx
    jnz         KeccakP1600_AddBytes_LastIncompleteLaneLoop
KeccakP1600_AddBytes_Exit:
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_AddBytes,.-KeccakP1600_AddBytes
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
#                                       %rdi                        %rsi               %rdx                 %rcx
#
.globl  KeccakP1600_OverwriteBytes
.globl _KeccakP1600_OverwriteBytes
#ifndef old_gas_syntax
.type   KeccakP1600_OverwriteBytes,@function
#endif
KeccakP1600_OverwriteBytes:
_KeccakP1600_OverwriteBytes:
.balign 32
    cmp         $0, %rcx
    jz          KeccakP1600_OverwriteBytes_Exit
    mov         %rdx, %rax                              # rax offset in lane
    and         $0xFFFFFFF8, %edx                       # rdx pointer into state index mapper
    lea         mapState(%rip), %r9
    add         %r9, %rdx
    and         $7, %rax
    jz          KeccakP1600_OverwriteBytes_LaneAlignedCheck
    mov         $8, %r9                                 # r9 is (max) length of incomplete lane
    sub         %rax, %r9
    cmp         %rcx, %r9
    cmovae      %rcx, %r9
    sub         %r9, %rcx                               # length -= length of incomplete lane
    add         (%rdx), %rax                            # rax = pointer to state lane
    add         $8, %rdx
    add         %rdi, %rax
KeccakP1600_OverwriteBytes_NotAlignedLoop:
    mov         (%rsi), %r8b
    inc         %rsi
    mov         %r8b, (%rax)
    inc         %rax
    dec         %r9
    jnz         KeccakP1600_OverwriteBytes_NotAlignedLoop
    jmp         KeccakP1600_OverwriteBytes_LaneAlignedCheck
KeccakP1600_OverwriteBytes_LaneAlignedLoop:
    mov         (%rsi), %r8
    add         $8, %rsi
    mov         (%rdx), %rax
    add         $8, %rdx
    add         %rdi, %rax
    mov         %r8, (%rax)
KeccakP1600_OverwriteBytes_LaneAlignedCheck:
    sub         $8, %rcx
    jnc         KeccakP1600_OverwriteBytes_LaneAlignedLoop
KeccakP1600_OverwriteBytes_LastIncompleteLane:
    add         $8, %rcx
    jz          KeccakP1600_OverwriteBytes_Exit
    mov         (%rdx), %rax
    add         %rdi, %rax
KeccakP1600_OverwriteBytes_LastIncompleteLaneLoop:
    mov         (%rsi), %r8b
    inc         %rsi
    mov         %r8b, (%rax)
    inc         %rax
    dec         %rcx
    jnz         KeccakP1600_OverwriteBytes_LastIncompleteLaneLoop
KeccakP1600_OverwriteBytes_Exit:
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_OverwriteBytes,.-KeccakP1600_OverwriteBytes
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_OverwriteWithZeroes(void *state, unsigned int byteCount);
#                                            %rdi                %rsi
#
.globl  KeccakP1600_OverwriteWithZeroes
.globl _KeccakP1600_OverwriteWithZeroes
#ifndef old_gas_syntax
.type   KeccakP1600_OverwriteWithZeroes,@function
#endif
KeccakP1600_OverwriteWithZeroes:
_KeccakP1600_OverwriteWithZeroes:
.balign 32
    cmp         $0, %rsi
    jz          KeccakP1600_OverwriteWithZeroes_Exit
    lea         mapState(%rip), %rdx                          # rdx pointer into state index mapper
    jmp         KeccakP1600_OverwriteWithZeroes_LaneAlignedCheck
KeccakP1600_OverwriteWithZeroes_LaneAlignedLoop:
    mov         (%rdx), %rax
    add         $8, %rdx
    add         %rdi, %rax
    movq        $0, (%rax)
KeccakP1600_OverwriteWithZeroes_LaneAlignedCheck:
    sub         $8, %rsi
    jnc         KeccakP1600_OverwriteWithZeroes_LaneAlignedLoop
KeccakP1600_OverwriteWithZeroes_LastIncompleteLane:
    add         $8, %rsi
    jz          KeccakP1600_OverwriteWithZeroes_Exit
    mov         (%rdx), %rax
    add         %rdi, %rax
KeccakP1600_OverwriteWithZeroes_LastIncompleteLaneLoop:
    movb        $0, (%rax)
    inc         %rax
    dec         %rsi
    jnz         KeccakP1600_OverwriteWithZeroes_LastIncompleteLaneLoop
KeccakP1600_OverwriteWithZeroes_Exit:
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_OverwriteWithZeroes,.-KeccakP1600_OverwriteWithZeroes
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
#                                           %rdi                  %rsi               %rdx                 %rcx
#
.globl  KeccakP1600_ExtractBytes
.globl _KeccakP1600_ExtractBytes
#ifndef old_gas_syntax
.type   KeccakP1600_ExtractBytes,@function
#endif
KeccakP1600_ExtractBytes:
_KeccakP1600_ExtractBytes:
.balign 32
    push        %rbx
    cmp         $0, %rcx
    jz          KeccakP1600_ExtractBytes_Exit
    mov         %rdx, %rax                              # rax offset in lane
    and         $0xFFFFFFF8, %edx                       # rdx pointer into state index mapper
    lea         mapState(%rip), %r9
    add         %r9, %rdx
    and         $7, %rax
    jz          KeccakP1600_ExtractBytes_LaneAlignedCheck
    mov         $8, %rbx                                # rbx is (max) length of incomplete lane
    sub         %rax, %rbx
    cmp         %rcx, %rbx
    cmovae      %rcx, %rbx
    sub         %rbx, %rcx                              # length -= length of incomplete lane
    mov         (%rdx), %r9
    add         $8, %rdx
    add         %rdi, %r9
    add         %rax, %r9
KeccakP1600_ExtractBytes_NotAlignedLoop:
    mov         (%r9), %r8b
    inc         %r9
    mov         %r8b, (%rsi)
    inc         %rsi
    dec         %rbx
    jnz         KeccakP1600_ExtractBytes_NotAlignedLoop
    jmp         KeccakP1600_ExtractBytes_LaneAlignedCheck
KeccakP1600_ExtractBytes_LaneAlignedLoop:
    mov         (%rdx), %rax
    add         $8, %rdx
    add         %rdi, %rax
    mov         (%rax), %r8
    mov         %r8, (%rsi)
    add         $8, %rsi
KeccakP1600_ExtractBytes_LaneAlignedCheck:
    sub         $8, %rcx
    jnc         KeccakP1600_ExtractBytes_LaneAlignedLoop
KeccakP1600_ExtractBytes_LastIncompleteLane:
    add         $8, %rcx
    jz          KeccakP1600_ExtractBytes_Exit
    mov         (%rdx), %rax
    add         %rdi, %rax
    mov         (%rax), %r8
KeccakP1600_ExtractBytes_LastIncompleteLaneLoop:
    mov         %r8b, (%rsi)
    shr         $8, %r8
    inc         %rsi
    dec         %rcx
    jnz         KeccakP1600_ExtractBytes_LastIncompleteLaneLoop
KeccakP1600_ExtractBytes_Exit:
    pop         %rbx
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_ExtractBytes,.-KeccakP1600_ExtractBytes
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
#                                                 %rdi                        %rsi                  %rdx                 %rcx                  %r8
#
.globl  KeccakP1600_ExtractAndAddBytes
.globl _KeccakP1600_ExtractAndAddBytes
#ifndef old_gas_syntax
.type   KeccakP1600_ExtractAndAddBytes,@function
#endif
KeccakP1600_ExtractAndAddBytes:
_KeccakP1600_ExtractAndAddBytes:
.balign 32
    push        %rbx
    push        %r10
    cmp         $0, %r8
    jz          KeccakP1600_ExtractAndAddBytes_Exit
    mov         %rcx, %rax                              # rax offset in lane
    and         $0xFFFFFFF8, %ecx                       # rcx pointer into state index mapper
    lea         mapState(%rip), %r9
    add         %r9, %rcx
    and         $7, %rax
    jz          KeccakP1600_ExtractAndAddBytes_LaneAlignedCheck
    mov         $8, %rbx                                # rbx is (max) length of incomplete lane
    sub         %rax, %rbx
    cmp         %r8, %rbx
    cmovae      %r8, %rbx
    sub         %rbx, %r8                               # length -= length of incomplete lane
    mov         (%rcx), %r9
    add         $8, %rcx
    add         %rdi, %r9
    add         %rax, %r9
KeccakP1600_ExtractAndAddBytes_NotAlignedLoop:
    mov         (%r9), %r10b
    inc         %r9
    xor         (%rsi), %r10b
    inc         %rsi
    mov         %r10b, (%rdx)
    inc         %rdx
    dec         %rbx
    jnz         KeccakP1600_ExtractAndAddBytes_NotAlignedLoop
    jmp         KeccakP1600_ExtractAndAddBytes_LaneAlignedCheck
KeccakP1600_ExtractAndAddBytes_LaneAlignedLoop:
    mov         (%rcx), %rax
    add         $8, %rcx
    add         %rdi, %rax
    mov         (%rax), %r10
    xor         (%rsi), %r10
    add         $8, %rsi
    mov         %r10, (%rdx)
    add         $8, %rdx
KeccakP1600_ExtractAndAddBytes_LaneAlignedCheck:
    sub         $8, %r8
    jnc         KeccakP1600_ExtractAndAddBytes_LaneAlignedLoop
KeccakP1600_ExtractAndAddBytes_LastIncompleteLane:
    add         $8, %r8
    jz          KeccakP1600_ExtractAndAddBytes_Exit
    mov         (%rcx), %rax
    add         %rdi, %rax
    mov         (%rax), %r10
KeccakP1600_ExtractAndAddBytes_LastIncompleteLaneLoop:
    xor         (%rsi), %r10b
    inc         %rsi
    mov         %r10b, (%rdx)
    inc         %rdx
    shr         $8, %r10
    dec         %r8
    jnz         KeccakP1600_ExtractAndAddBytes_LastIncompleteLaneLoop
KeccakP1600_ExtractAndAddBytes_Exit:
    pop         %r10
    pop         %rbx
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_ExtractAndAddBytes,.-KeccakP1600_ExtractAndAddBytes
#endif

# -----------------------------------------------------------------------------
#
# internal    
#
#ifndef old_gas_syntax
.type    __KeccakF1600,@function
#endif
.balign 32
__KeccakF1600:
.Loop_avx2:
    ######################################### Theta
    vpshufd     $0b01001110,%ymm2,%ymm13
    vpxor       %ymm3,%ymm5,%ymm12
    vpxor       %ymm6,%ymm4,%ymm9
    vpxor       %ymm1,%ymm12,%ymm12
    vpxor       %ymm9,%ymm12,%ymm12         # C[1..4]

    vpermq      $0b10010011,%ymm12,%ymm11
    vpxor       %ymm2,%ymm13,%ymm13
    vpermq      $0b01001110,%ymm13,%ymm7

    vpsrlq      $63,%ymm12,%ymm8
    vpaddq      %ymm12,%ymm12,%ymm9
    vpor        %ymm9,%ymm8,%ymm8           # ROL64(C[1..4],1)

    vpermq      $0b00111001,%ymm8,%ymm15
    vpxor       %ymm11,%ymm8,%ymm14
    vpermq      $0b00000000,%ymm14,%ymm14   # D[0..0] = ROL64(C[1],1) ^ C[4]

    vpxor       %ymm0,%ymm13,%ymm13
    vpxor       %ymm7,%ymm13,%ymm13         # C[0..0]

    vpsrlq      $63,%ymm13,%ymm7
    vpaddq      %ymm13,%ymm13,%ymm8
    vpor        %ymm7,%ymm8,%ymm8           # ROL64(C[0..0],1)

    vpxor       %ymm14,%ymm2,%ymm2          # ^= D[0..0]
    vpxor       %ymm14,%ymm0,%ymm0          # ^= D[0..0]

    vpblendd    $0b11000000,%ymm8,%ymm15,%ymm15
    vpblendd    $0b00000011,%ymm13,%ymm11,%ymm11
    vpxor       %ymm11,%ymm15,%ymm15        # D[1..4] = ROL64(C[2..4,0),1) ^ C[0..3]

    ######################################### Rho + Pi + pre-Chi shuffle
    vpsllvq     0*32-96(%r8),%ymm2,%ymm10
    vpsrlvq     0*32-96(%r9),%ymm2,%ymm2
    vpor        %ymm10,%ymm2,%ymm2

    vpxor       %ymm15,%ymm3,%ymm3          # ^= D[1..4] from Theta
    vpsllvq     2*32-96(%r8),%ymm3,%ymm11
    vpsrlvq     2*32-96(%r9),%ymm3,%ymm3
    vpor        %ymm11,%ymm3,%ymm3

    vpxor       %ymm15,%ymm4,%ymm4          # ^= D[1..4] from Theta
    vpsllvq     3*32-96(%r8),%ymm4,%ymm12
    vpsrlvq     3*32-96(%r9),%ymm4,%ymm4
    vpor        %ymm12,%ymm4,%ymm4

    vpxor       %ymm15,%ymm5,%ymm5          # ^= D[1..4] from Theta
    vpsllvq     4*32-96(%r8),%ymm5,%ymm13
    vpsrlvq     4*32-96(%r9),%ymm5,%ymm5
    vpor        %ymm13,%ymm5,%ymm5

    vpxor       %ymm15,%ymm6,%ymm6          # ^= D[1..4] from Theta
    vpermq      $0b10001101,%ymm2,%ymm10    # %ymm2 -> future %ymm3
    vpermq      $0b10001101,%ymm3,%ymm11    # %ymm3 -> future %ymm4
    vpsllvq     5*32-96(%r8),%ymm6,%ymm14
    vpsrlvq     5*32-96(%r9),%ymm6,%ymm8
    vpor        %ymm14,%ymm8,%ymm8          # %ymm6 -> future %ymm1

    vpxor       %ymm15,%ymm1,%ymm1          # ^= D[1..4] from Theta
    vpermq      $0b00011011,%ymm4,%ymm12    # %ymm4 -> future %ymm5
    vpermq      $0b01110010,%ymm5,%ymm13    # %ymm5 -> future %ymm6
    vpsllvq     1*32-96(%r8),%ymm1,%ymm15
    vpsrlvq     1*32-96(%r9),%ymm1,%ymm9
    vpor        %ymm15,%ymm9,%ymm9          # %ymm1 -> future %ymm2

    ######################################### Chi
    vpsrldq     $8,%ymm8,%ymm14
    vpandn      %ymm14,%ymm8,%ymm7                  # tgting  [0][0] [0][0] [0][0] [0][0]

    vpblendd    $0b00001100,%ymm13,%ymm9,%ymm3      #               [4][4] [2][0]
    vpblendd    $0b00001100,%ymm9,%ymm11,%ymm15     #               [4][0] [2][1]
    vpblendd    $0b00001100,%ymm11,%ymm10,%ymm5     #               [4][2] [2][4]
    vpblendd    $0b00001100,%ymm10,%ymm9,%ymm14     #               [4][3] [2][0]
    vpblendd    $0b00110000,%ymm11,%ymm3,%ymm3      #        [1][3] [4][4] [2][0]
    vpblendd    $0b00110000,%ymm12,%ymm15,%ymm15    #        [1][4] [4][0] [2][1]
    vpblendd    $0b00110000,%ymm9,%ymm5,%ymm5       #        [1][0] [4][2] [2][4]
    vpblendd    $0b00110000,%ymm13,%ymm14,%ymm14    #        [1][1] [4][3] [2][0]
    vpblendd    $0b11000000,%ymm12,%ymm3,%ymm3      # [3][2] [1][3] [4][4] [2][0]
    vpblendd    $0b11000000,%ymm13,%ymm15,%ymm15    # [3][3] [1][4] [4][0] [2][1]
    vpblendd    $0b11000000,%ymm13,%ymm5,%ymm5      # [3][3] [1][0] [4][2] [2][4]
    vpblendd    $0b11000000,%ymm11,%ymm14,%ymm14    # [3][4] [1][1] [4][3] [2][0]
    vpandn      %ymm15,%ymm3,%ymm3                  # tgting  [3][1] [1][2] [4][3] [2][4]
    vpandn      %ymm14,%ymm5,%ymm5                  # tgting  [3][2] [1][4] [4][1] [2][3]

    vpblendd    $0b00001100,%ymm9,%ymm12,%ymm6      #               [4][0] [2][3]
    vpblendd    $0b00001100,%ymm12,%ymm10,%ymm15    #               [4][1] [2][4]
    vpxor       %ymm10,%ymm3,%ymm3
    vpblendd    $0b00110000,%ymm10,%ymm6,%ymm6      #        [1][2] [4][0] [2][3]
    vpblendd    $0b00110000,%ymm11,%ymm15,%ymm15    #        [1][3] [4][1] [2][4]
    vpxor       %ymm12,%ymm5,%ymm5
    vpblendd    $0b11000000,%ymm11,%ymm6,%ymm6      # [3][4] [1][2] [4][0] [2][3]
    vpblendd    $0b11000000,%ymm9,%ymm15,%ymm15     # [3][0] [1][3] [4][1] [2][4]
    vpandn      %ymm15,%ymm6,%ymm6                  # tgting  [3][3] [1][1] [4][4] [2][2]
    vpxor       %ymm13,%ymm6,%ymm6

    vpermq      $0b00011110,%ymm8,%ymm4             # [0][1] [0][2] [0][4] [0][3]
    vpblendd    $0b00110000,%ymm0,%ymm4,%ymm15      # [0][1] [0][0] [0][4] [0][3]
    vpermq      $0b00111001,%ymm8,%ymm1             # [0][1] [0][4] [0][3] [0][2]
    vpblendd    $0b11000000,%ymm0,%ymm1,%ymm1       # [0][0] [0][4] [0][3] [0][2]
    vpandn      %ymm15,%ymm1,%ymm1                  # tgting  [0][4] [0][3] [0][2] [0][1]

    vpblendd    $0b00001100,%ymm12,%ymm11,%ymm2     #               [4][1] [2][1]
    vpblendd    $0b00001100,%ymm11,%ymm13,%ymm14    #               [4][2] [2][2]
    vpblendd    $0b00110000,%ymm13,%ymm2,%ymm2      #        [1][1] [4][1] [2][1]
    vpblendd    $0b00110000,%ymm10,%ymm14,%ymm14    #        [1][2] [4][2] [2][2]
    vpblendd    $0b11000000,%ymm10,%ymm2,%ymm2      # [3][1] [1][1] [4][1] [2][1]
    vpblendd    $0b11000000,%ymm12,%ymm14,%ymm14    # [3][2] [1][2] [4][2] [2][2]
    vpandn      %ymm14,%ymm2,%ymm2                  # tgting  [3][0] [1][0] [4][0] [2][0]
    vpxor       %ymm9,%ymm2,%ymm2

    vpermq      $0b00000000,%ymm7,%ymm7             # [0][0] [0][0] [0][0] [0][0]
    vpermq      $0b00011011,%ymm3,%ymm3             # post-Chi shuffle
    vpermq      $0b10001101,%ymm5,%ymm5
    vpermq      $0b01110010,%ymm6,%ymm6

    vpblendd    $0b00001100,%ymm10,%ymm13,%ymm4     #               [4][3] [2][2]
    vpblendd    $0b00001100,%ymm13,%ymm12,%ymm14    #               [4][4] [2][3]
    vpblendd    $0b00110000,%ymm12,%ymm4,%ymm4      #        [1][4] [4][3] [2][2]
    vpblendd    $0b00110000,%ymm9,%ymm14,%ymm14     #        [1][0] [4][4] [2][3]
    vpblendd    $0b11000000,%ymm9,%ymm4,%ymm4       # [3][0] [1][4] [4][3] [2][2]
    vpblendd    $0b11000000,%ymm10,%ymm14,%ymm14    # [3][1] [1][0] [4][4] [2][3]
    vpandn      %ymm14,%ymm4,%ymm4                  # tgting  [3][4] [1][3] [4][2] [2][1]

    vpxor       %ymm7,%ymm0,%ymm0
    vpxor       %ymm8,%ymm1,%ymm1
    vpxor       %ymm11,%ymm4,%ymm4

    ######################################### Iota
    vpxor       (%r10),%ymm0,%ymm0
    lea         32(%r10),%r10

    dec         %eax
    jnz         .Loop_avx2
    ret
#ifndef old_gas_syntax
.size   __KeccakF1600,.-__KeccakF1600
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_Permute_24rounds(void *state);
#                                        %rdi
#
.globl  KeccakP1600_Permute_24rounds
.globl _KeccakP1600_Permute_24rounds
#ifndef old_gas_syntax
.type   KeccakP1600_Permute_24rounds,@function
#endif
KeccakP1600_Permute_24rounds:
_KeccakP1600_Permute_24rounds:
.balign 32
    lea             rhotates_left+96(%rip),%r8
    lea             rhotates_right+96(%rip),%r9
    lea             iotas(%rip),%r10
    mov             $24,%eax
    lea             96(%rdi),%rdi
    vzeroupper
    vpbroadcastq    -96(%rdi),%ymm0         # load A[5][5]
    vmovdqu         8+32*0-96(%rdi),%ymm1
    vmovdqu         8+32*1-96(%rdi),%ymm2
    vmovdqu         8+32*2-96(%rdi),%ymm3
    vmovdqu         8+32*3-96(%rdi),%ymm4
    vmovdqu         8+32*4-96(%rdi),%ymm5
    vmovdqu         8+32*5-96(%rdi),%ymm6
    call            __KeccakF1600
    vmovq           %xmm0,-96(%rdi)
    vmovdqu         %ymm1,8+32*0-96(%rdi)
    vmovdqu         %ymm2,8+32*1-96(%rdi)
    vmovdqu         %ymm3,8+32*2-96(%rdi)
    vmovdqu         %ymm4,8+32*3-96(%rdi)
    vmovdqu         %ymm5,8+32*4-96(%rdi)
    vmovdqu         %ymm6,8+32*5-96(%rdi)
    vzeroupper
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_Permute_24rounds,.-KeccakP1600_Permute_24rounds
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_Permute_12rounds(void *state);
#                                        %rdi
#
.globl  KeccakP1600_Permute_12rounds
.globl _KeccakP1600_Permute_12rounds
#ifndef old_gas_syntax
.type   KeccakP1600_Permute_12rounds,@function
#endif
KeccakP1600_Permute_12rounds:
_KeccakP1600_Permute_12rounds:
.balign 32
    lea             rhotates_left+96(%rip),%r8
    lea             rhotates_right+96(%rip),%r9
    lea             iotas+12*4*8(%rip),%r10
    mov             $12,%eax
    lea             96(%rdi),%rdi
    vzeroupper
    vpbroadcastq    -96(%rdi),%ymm0         # load A[5][5]
    vmovdqu         8+32*0-96(%rdi),%ymm1
    vmovdqu         8+32*1-96(%rdi),%ymm2
    vmovdqu         8+32*2-96(%rdi),%ymm3
    vmovdqu         8+32*3-96(%rdi),%ymm4
    vmovdqu         8+32*4-96(%rdi),%ymm5
    vmovdqu         8+32*5-96(%rdi),%ymm6
    call            __KeccakF1600
    vmovq           %xmm0,-96(%rdi)
    vmovdqu         %ymm1,8+32*0-96(%rdi)
    vmovdqu         %ymm2,8+32*1-96(%rdi)
    vmovdqu         %ymm3,8+32*2-96(%rdi)
    vmovdqu         %ymm4,8+32*3-96(%rdi)
    vmovdqu         %ymm5,8+32*4-96(%rdi)
    vmovdqu         %ymm6,8+32*5-96(%rdi)
    vzeroupper
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_Permute_12rounds,.-KeccakP1600_Permute_12rounds
#endif

# -----------------------------------------------------------------------------
#
# void KeccakP1600_Permute_Nrounds(void *state, unsigned int nrounds);
#                                        %rdi                %rsi
#
.globl  KeccakP1600_Permute_Nrounds
.globl _KeccakP1600_Permute_Nrounds
#ifndef old_gas_syntax
.type   KeccakP1600_Permute_Nrounds,@function
#endif
KeccakP1600_Permute_Nrounds:
_KeccakP1600_Permute_Nrounds:
.balign 32
    lea             rhotates_left+96(%rip),%r8
    lea             rhotates_right+96(%rip),%r9
    lea             iotas+24*4*8(%rip),%r10
    mov             %rsi,%rax
    shl             $2+3,%rsi
    sub             %rsi, %r10
    lea             96(%rdi),%rdi
    vzeroupper
    vpbroadcastq    -96(%rdi),%ymm0         # load A[5][5]
    vmovdqu         8+32*0-96(%rdi),%ymm1
    vmovdqu         8+32*1-96(%rdi),%ymm2
    vmovdqu         8+32*2-96(%rdi),%ymm3
    vmovdqu         8+32*3-96(%rdi),%ymm4
    vmovdqu         8+32*4-96(%rdi),%ymm5
    vmovdqu         8+32*5-96(%rdi),%ymm6
    call            __KeccakF1600
    vmovq           %xmm0,-96(%rdi)
    vmovdqu         %ymm1,8+32*0-96(%rdi)
    vmovdqu         %ymm2,8+32*1-96(%rdi)
    vmovdqu         %ymm3,8+32*2-96(%rdi)
    vmovdqu         %ymm4,8+32*3-96(%rdi)
    vmovdqu         %ymm5,8+32*4-96(%rdi)
    vmovdqu         %ymm6,8+32*5-96(%rdi)
    vzeroupper
    ret
#ifndef old_gas_syntax
.size   KeccakP1600_Permute_Nrounds,.-KeccakP1600_Permute_Nrounds
#endif

# -----------------------------------------------------------------------------
#
# size_t KeccakF1600_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);
#                                          %rdi                %rsi                            %rdx         %rcx
#
.globl  KeccakF1600_FastLoop_Absorb
.globl _KeccakF1600_FastLoop_Absorb
#ifndef old_gas_syntax
.type   KeccakF1600_FastLoop_Absorb,@function
#endif
KeccakF1600_FastLoop_Absorb:
_KeccakF1600_FastLoop_Absorb:
.balign 32
    push            %rbx
    push            %r10
    shr             $3, %rcx                # rcx = data length in lanes
    mov             %rdx, %rbx              # rbx = initial data pointer
    cmp             %rsi, %rcx
    jb              KeccakF1600_FastLoop_Absorb_Exit
    vzeroupper
    cmp             $21, %rsi    
    jnz             KeccakF1600_FastLoop_Absorb_Not21Lanes
    sub             $21, %rcx
    lea             rhotates_left+96(%rip),%r8
    lea             rhotates_right+96(%rip),%r9
    lea             96(%rdi),%rdi
    vpbroadcastq    -96(%rdi),%ymm0         # load A[5][5]
    vmovdqu         8+32*0-96(%rdi),%ymm1
    vmovdqu         8+32*1-96(%rdi),%ymm2
    vmovdqu         8+32*2-96(%rdi),%ymm3
    vmovdqu         8+32*3-96(%rdi),%ymm4
    vmovdqu         8+32*4-96(%rdi),%ymm5
    vmovdqu         8+32*5-96(%rdi),%ymm6
KeccakF1600_FastLoop_Absorb_Loop21Lanes:    
    vpbroadcastq    (%rdx),%ymm7
    vmovdqu         8(%rdx),%ymm8

    vmovdqa         map2(%rip), %xmm15
    vpcmpeqd        %ymm14, %ymm14, %ymm14
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm9

    vmovdqa         mask3_21(%rip), %ymm14
    vpxor           %ymm10, %ymm10, %ymm10
    vmovdqa         map3(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm10

    vmovdqa         mask4_21(%rip), %ymm14
    vpxor           %ymm11, %ymm11, %ymm11
    vmovdqa         map4(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm11

    vmovdqa         mask5_21(%rip), %ymm14
    vpxor           %ymm12, %ymm12, %ymm12
    vmovdqa         map5(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm12

    vmovdqa         mask6_21(%rip), %ymm14
    vpxor           %ymm13, %ymm13, %ymm13
    vmovdqa         map6(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm13

    vpxor           %ymm7,%ymm0,%ymm0
    vpxor           %ymm8,%ymm1,%ymm1
    vpxor           %ymm9,%ymm2,%ymm2
    vpxor           %ymm10,%ymm3,%ymm3
    vpxor           %ymm11,%ymm4,%ymm4
    vpxor           %ymm12,%ymm5,%ymm5
    vpxor           %ymm13,%ymm6,%ymm6
    add             $21*8, %rdx
    lea             iotas(%rip),%r10
    mov             $24,%eax
    call            __KeccakF1600
    sub             $21, %rcx
    jnc             KeccakF1600_FastLoop_Absorb_Loop21Lanes
KeccakF1600_FastLoop_Absorb_SaveAndExit:
    vmovq           %xmm0,-96(%rdi)
    vmovdqu         %ymm1,8+32*0-96(%rdi)
    vmovdqu         %ymm2,8+32*1-96(%rdi)
    vmovdqu         %ymm3,8+32*2-96(%rdi)
    vmovdqu         %ymm4,8+32*3-96(%rdi)
    vmovdqu         %ymm5,8+32*4-96(%rdi)
    vmovdqu         %ymm6,8+32*5-96(%rdi)
KeccakF1600_FastLoop_Absorb_Exit:
    vzeroupper
    mov             %rdx, %rax               # return number of bytes processed
    sub             %rbx, %rax
    pop             %r10
    pop             %rbx
    ret
KeccakF1600_FastLoop_Absorb_Not21Lanes:
    cmp             $17, %rsi    
    jnz             KeccakF1600_FastLoop_Absorb_Not17Lanes
    sub             $17, %rcx
    lea             rhotates_left+96(%rip),%r8
    lea             rhotates_right+96(%rip),%r9
    lea             96(%rdi),%rdi
    vpbroadcastq    -96(%rdi),%ymm0         # load A[5][5]
    vmovdqu         8+32*0-96(%rdi),%ymm1
    vmovdqu         8+32*1-96(%rdi),%ymm2
    vmovdqu         8+32*2-96(%rdi),%ymm3
    vmovdqu         8+32*3-96(%rdi),%ymm4
    vmovdqu         8+32*4-96(%rdi),%ymm5
    vmovdqu         8+32*5-96(%rdi),%ymm6
KeccakF1600_FastLoop_Absorb_Loop17Lanes:    
    vpbroadcastq    (%rdx),%ymm7
    vmovdqu         8(%rdx),%ymm8

    vmovdqa         mask2_17(%rip), %ymm14
    vpxor           %ymm9, %ymm9, %ymm9
    vmovdqa         map2(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm9

    vmovdqa         mask3_17(%rip), %ymm14
    vpxor           %ymm10, %ymm10, %ymm10
    vmovdqa         map3(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm10

    vmovdqa         mask4_17(%rip), %ymm14
    vpxor           %ymm11, %ymm11, %ymm11
    vmovdqa         map4(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm11

    vmovdqa         mask5_17(%rip), %ymm14
    vpxor           %ymm12, %ymm12, %ymm12
    vmovdqa         map5(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm12

    vmovdqa         mask6_17(%rip), %ymm14
    vpxor           %ymm13, %ymm13, %ymm13
    vmovdqa         map6(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm13

    vpxor           %ymm7,%ymm0,%ymm0
    vpxor           %ymm8,%ymm1,%ymm1
    vpxor           %ymm9,%ymm2,%ymm2
    vpxor           %ymm10,%ymm3,%ymm3
    vpxor           %ymm11,%ymm4,%ymm4
    vpxor           %ymm12,%ymm5,%ymm5
    vpxor           %ymm13,%ymm6,%ymm6
    add             $17*8, %rdx
    lea             iotas(%rip),%r10
    mov             $24,%eax
    call            __KeccakF1600
    sub             $17, %rcx
    jnc             KeccakF1600_FastLoop_Absorb_Loop17Lanes
    jmp             KeccakF1600_FastLoop_Absorb_SaveAndExit
KeccakF1600_FastLoop_Absorb_Not17Lanes:
    lea             mapState(%rip), %r9
    mov             %rsi, %rax
KeccakF1600_FastLoop_Absorb_LanesAddLoop:
    mov             (%rdx), %r8
    add             $8, %rdx
    mov             (%r9), %r10
    add             $8, %r9
    add             %rdi, %r10
    xor             %r8, (%r10)
    sub             $1, %rax
    jnz             KeccakF1600_FastLoop_Absorb_LanesAddLoop
    sub             %rsi, %rcx
    push            %rdi
    push            %rsi
    push            %rdx
    push            %rcx
#ifdef no_plt
    call            KeccakP1600_Permute_24rounds
#else
    call            KeccakP1600_Permute_24rounds@PLT
#endif
    pop             %rcx
    pop             %rdx
    pop             %rsi
    pop             %rdi
    cmp             %rsi, %rcx
    jae             KeccakF1600_FastLoop_Absorb_Not17Lanes
    jmp             KeccakF1600_FastLoop_Absorb_Exit
#ifndef old_gas_syntax
.size   KeccakF1600_FastLoop_Absorb,.-KeccakF1600_FastLoop_Absorb
#endif

# -----------------------------------------------------------------------------
#
# size_t KeccakP1600_12rounds_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);
#                                          %rdi                %rsi                            %rdx         %rcx
#
.globl  KeccakP1600_12rounds_FastLoop_Absorb
.globl _KeccakP1600_12rounds_FastLoop_Absorb
#ifndef old_gas_syntax
.type   KeccakP1600_12rounds_FastLoop_Absorb,@function
#endif
KeccakP1600_12rounds_FastLoop_Absorb:
_KeccakP1600_12rounds_FastLoop_Absorb:
.balign 32
    push            %rbx
    push            %r10
    shr             $3, %rcx                # rcx = data length in lanes
    mov             %rdx, %rbx              # rbx = initial data pointer
    cmp             %rsi, %rcx
    jb              KeccakP1600_12rounds_FastLoop_Absorb_Exit
    vzeroupper
    cmp             $21, %rsi    
    jnz             KeccakP1600_12rounds_FastLoop_Absorb_Not21Lanes
    sub             $21, %rcx
    lea             rhotates_left+96(%rip),%r8
    lea             rhotates_right+96(%rip),%r9
    lea             96(%rdi),%rdi
    vpbroadcastq    -96(%rdi),%ymm0         # load A[5][5]
    vmovdqu         8+32*0-96(%rdi),%ymm1
    vmovdqu         8+32*1-96(%rdi),%ymm2
    vmovdqu         8+32*2-96(%rdi),%ymm3
    vmovdqu         8+32*3-96(%rdi),%ymm4
    vmovdqu         8+32*4-96(%rdi),%ymm5
    vmovdqu         8+32*5-96(%rdi),%ymm6
KeccakP1600_12rounds_FastLoop_Absorb_Loop21Lanes:    
    vpbroadcastq    (%rdx),%ymm7
    vmovdqu         8(%rdx),%ymm8

    vmovdqa         map2(%rip), %xmm15
    vpcmpeqd        %ymm14, %ymm14, %ymm14
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm9

    vmovdqa         mask3_21(%rip), %ymm14
    vpxor           %ymm10, %ymm10, %ymm10
    vmovdqa         map3(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm10

    vmovdqa         mask4_21(%rip), %ymm14
    vpxor           %ymm11, %ymm11, %ymm11
    vmovdqa         map4(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm11

    vmovdqa         mask5_21(%rip), %ymm14
    vpxor           %ymm12, %ymm12, %ymm12
    vmovdqa         map5(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm12

    vmovdqa         mask6_21(%rip), %ymm14
    vpxor           %ymm13, %ymm13, %ymm13
    vmovdqa         map6(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm13

    vpxor           %ymm7,%ymm0,%ymm0
    vpxor           %ymm8,%ymm1,%ymm1
    vpxor           %ymm9,%ymm2,%ymm2
    vpxor           %ymm10,%ymm3,%ymm3
    vpxor           %ymm11,%ymm4,%ymm4
    vpxor           %ymm12,%ymm5,%ymm5
    vpxor           %ymm13,%ymm6,%ymm6
    add             $21*8, %rdx
    lea             iotas+12*4*8(%rip),%r10
    mov             $12,%eax
    call            __KeccakF1600
    sub             $21, %rcx
    jnc             KeccakP1600_12rounds_FastLoop_Absorb_Loop21Lanes
KeccakP1600_12rounds_FastLoop_Absorb_SaveAndExit:
    vmovq           %xmm0,-96(%rdi)
    vmovdqu         %ymm1,8+32*0-96(%rdi)
    vmovdqu         %ymm2,8+32*1-96(%rdi)
    vmovdqu         %ymm3,8+32*2-96(%rdi)
    vmovdqu         %ymm4,8+32*3-96(%rdi)
    vmovdqu         %ymm5,8+32*4-96(%rdi)
    vmovdqu         %ymm6,8+32*5-96(%rdi)
KeccakP1600_12rounds_FastLoop_Absorb_Exit:
    vzeroupper
    mov             %rdx, %rax               # return number of bytes processed
    sub             %rbx, %rax
    pop             %r10
    pop             %rbx
    ret
KeccakP1600_12rounds_FastLoop_Absorb_Not21Lanes:
    cmp             $17, %rsi    
    jnz             KeccakP1600_12rounds_FastLoop_Absorb_Not17Lanes
    sub             $17, %rcx
    lea             rhotates_left+96(%rip),%r8
    lea             rhotates_right+96(%rip),%r9
    lea             96(%rdi),%rdi
    vpbroadcastq    -96(%rdi),%ymm0         # load A[5][5]
    vmovdqu         8+32*0-96(%rdi),%ymm1
    vmovdqu         8+32*1-96(%rdi),%ymm2
    vmovdqu         8+32*2-96(%rdi),%ymm3
    vmovdqu         8+32*3-96(%rdi),%ymm4
    vmovdqu         8+32*4-96(%rdi),%ymm5
    vmovdqu         8+32*5-96(%rdi),%ymm6
KeccakP1600_12rounds_FastLoop_Absorb_Loop17Lanes:    
    vpbroadcastq    (%rdx),%ymm7
    vmovdqu         8(%rdx),%ymm8

    vmovdqa         mask2_17(%rip), %ymm14
    vpxor           %ymm9, %ymm9, %ymm9
    vmovdqa         map2(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm9

    vmovdqa         mask3_17(%rip), %ymm14
    vpxor           %ymm10, %ymm10, %ymm10
    vmovdqa         map3(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm10

    vmovdqa         mask4_17(%rip), %ymm14
    vpxor           %ymm11, %ymm11, %ymm11
    vmovdqa         map4(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm11

    vmovdqa         mask5_17(%rip), %ymm14
    vpxor           %ymm12, %ymm12, %ymm12
    vmovdqa         map5(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm12

    vmovdqa         mask6_17(%rip), %ymm14
    vpxor           %ymm13, %ymm13, %ymm13
    vmovdqa         map6(%rip), %xmm15
    vpgatherdq      %ymm14, (%rdx, %xmm15, 1), %ymm13

    vpxor           %ymm7,%ymm0,%ymm0
    vpxor           %ymm8,%ymm1,%ymm1
    vpxor           %ymm9,%ymm2,%ymm2
    vpxor           %ymm10,%ymm3,%ymm3
    vpxor           %ymm11,%ymm4,%ymm4
    vpxor           %ymm12,%ymm5,%ymm5
    vpxor           %ymm13,%ymm6,%ymm6
    add             $17*8, %rdx
    lea             iotas+12*4*8(%rip),%r10
    mov             $12,%eax
    call            __KeccakF1600
    sub             $17, %rcx
    jnc             KeccakP1600_12rounds_FastLoop_Absorb_Loop17Lanes
    jmp             KeccakP1600_12rounds_FastLoop_Absorb_SaveAndExit
KeccakP1600_12rounds_FastLoop_Absorb_Not17Lanes:
    lea             mapState(%rip), %r9
    mov             %rsi, %rax
KeccakP1600_12rounds_FastLoop_Absorb_LanesAddLoop:
    mov             (%rdx), %r8
    add             $8, %rdx
    mov             (%r9), %r10
    add             $8, %r9
    add             %rdi, %r10
    xor             %r8, (%r10)
    sub             $1, %rax
    jnz             KeccakP1600_12rounds_FastLoop_Absorb_LanesAddLoop
    sub             %rsi, %rcx
    push            %rdi
    push            %rsi
    push            %rdx
    push            %rcx
#ifdef no_plt
    call            KeccakP1600_Permute_12rounds
#else
    call            KeccakP1600_Permute_12rounds@PLT
#endif
    pop             %rcx
    pop             %rdx
    pop             %rsi
    pop             %rdi
    cmp             %rsi, %rcx
    jae             KeccakP1600_12rounds_FastLoop_Absorb_Not17Lanes
    jmp             KeccakP1600_12rounds_FastLoop_Absorb_Exit
#ifndef old_gas_syntax
.size   KeccakP1600_12rounds_FastLoop_Absorb,.-KeccakP1600_12rounds_FastLoop_Absorb
#endif

#ifndef old_gas_syntax
.section .rodata
#endif
.equ    ALLON,        0xFFFFFFFFFFFFFFFF

.balign 64
rhotates_left:
    .quad     3,   18,    36,    41         # [2][0] [4][0] [1][0] [3][0]
    .quad     1,   62,    28,    27         # [0][1] [0][2] [0][3] [0][4]
    .quad    45,    6,    56,    39         # [3][1] [1][2] [4][3] [2][4]
    .quad    10,   61,    55,     8         # [2][1] [4][2] [1][3] [3][4]
    .quad     2,   15,    25,    20         # [4][1] [3][2] [2][3] [1][4]
    .quad    44,   43,    21,    14         # [1][1] [2][2] [3][3] [4][4]
rhotates_right:
    .quad    64-3,  64-18,  64-36,  64-41
    .quad    64-1,  64-62,  64-28,  64-27
    .quad    64-45, 64-6,   64-56,  64-39
    .quad    64-10, 64-61,  64-55,  64-8
    .quad    64-2,  64-15,  64-25,  64-20
    .quad    64-44, 64-43,  64-21,  64-14
iotas:
    .quad    0x0000000000000001, 0x0000000000000001, 0x0000000000000001, 0x0000000000000001
    .quad    0x0000000000008082, 0x0000000000008082, 0x0000000000008082, 0x0000000000008082
    .quad    0x800000000000808a, 0x800000000000808a, 0x800000000000808a, 0x800000000000808a
    .quad    0x8000000080008000, 0x8000000080008000, 0x8000000080008000, 0x8000000080008000
    .quad    0x000000000000808b, 0x000000000000808b, 0x000000000000808b, 0x000000000000808b
    .quad    0x0000000080000001, 0x0000000080000001, 0x0000000080000001, 0x0000000080000001
    .quad    0x8000000080008081, 0x8000000080008081, 0x8000000080008081, 0x8000000080008081
    .quad    0x8000000000008009, 0x8000000000008009, 0x8000000000008009, 0x8000000000008009
    .quad    0x000000000000008a, 0x000000000000008a, 0x000000000000008a, 0x000000000000008a
    .quad    0x0000000000000088, 0x0000000000000088, 0x0000000000000088, 0x0000000000000088
    .quad    0x0000000080008009, 0x0000000080008009, 0x0000000080008009, 0x0000000080008009
    .quad    0x000000008000000a, 0x000000008000000a, 0x000000008000000a, 0x000000008000000a
    .quad    0x000000008000808b, 0x000000008000808b, 0x000000008000808b, 0x000000008000808b
    .quad    0x800000000000008b, 0x800000000000008b, 0x800000000000008b, 0x800000000000008b
    .quad    0x8000000000008089, 0x8000000000008089, 0x8000000000008089, 0x8000000000008089
    .quad    0x8000000000008003, 0x8000000000008003, 0x8000000000008003, 0x8000000000008003
    .quad    0x8000000000008002, 0x8000000000008002, 0x8000000000008002, 0x8000000000008002
    .quad    0x8000000000000080, 0x8000000000000080, 0x8000000000000080, 0x8000000000000080
    .quad    0x000000000000800a, 0x000000000000800a, 0x000000000000800a, 0x000000000000800a
    .quad    0x800000008000000a, 0x800000008000000a, 0x800000008000000a, 0x800000008000000a
    .quad    0x8000000080008081, 0x8000000080008081, 0x8000000080008081, 0x8000000080008081
    .quad    0x8000000000008080, 0x8000000000008080, 0x8000000000008080, 0x8000000000008080
    .quad    0x0000000080000001, 0x0000000080000001, 0x0000000080000001, 0x0000000080000001
    .quad    0x8000000080008008, 0x8000000080008008, 0x8000000080008008, 0x8000000080008008

mapState:
    .quad     0*8,  1*8,  2*8,  3*8,  4*8
    .quad     7*8, 21*8, 10*8, 15*8, 20*8
    .quad     5*8, 13*8, 22*8, 19*8, 12*8
    .quad     8*8,  9*8, 18*8, 23*8, 16*8
    .quad     6*8, 17*8, 14*8, 11*8, 24*8

.balign 16
map2:
    .long    10*8, 20*8,  5*8, 15*8
map3:
    .long    16*8,  7*8, 23*8, 14*8
map4:
    .long    11*8, 22*8,  8*8, 19*8
map5:
    .long    21*8, 17*8, 13*8,  9*8
map6:
    .long     6*8, 12*8, 18*8, 24*8

.balign 32
mask3_21:
    .quad    ALLON, ALLON,     0, ALLON
mask4_21:
    .quad    ALLON,     0, ALLON, ALLON
mask5_21:
    .quad    0,     ALLON, ALLON, ALLON
mask6_21:
    .quad    ALLON, ALLON, ALLON,     0

mask2_17:
    .quad    ALLON,     0, ALLON, ALLON
mask3_17:
    .quad    ALLON, ALLON,     0, ALLON
mask4_17:
    .quad    ALLON,     0, ALLON,     0
mask5_17:
    .quad        0,     0, ALLON, ALLON
mask6_17:
    .quad    ALLON, ALLON,     0,     0

.asciz  "Keccak-1600 for AVX2, CRYPTOGAMS by <appro@openssl.org>"
