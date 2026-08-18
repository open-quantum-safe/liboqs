.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS2048509_AVX2_square_6_509
.global _PQCLEAN_NTRUHPS2048509_AVX2_square_6_509
PQCLEAN_NTRUHPS2048509_AVX2_square_6_509:
_PQCLEAN_NTRUHPS2048509_AVX2_square_6_509:
push %r15
push %r14
push %r13
push %r12
push %rbx
push %rbp
mov 0(%rsi), %r11
mov $0x101010101010101, %rbp
pext %rbp, %r11, %r10
mov $0x249249, %rbx
pdep %rbx, %r10, %r10
mov %r10, 0(%rdi)
mov $0x202020202020202, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 8(%rdi)
mov $0x404040404040404, %r13
pext %r13, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 16(%rdi)
mov $0x808080808080808, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 24(%rdi)
mov $0x1010101010101010, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 32(%rdi)
mov $0x2020202020202020, %r9
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 40(%rdi)
mov $0x4040404040404040, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 48(%rdi)
mov $0x8080808080808080, %rdx
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 56(%rdi)
mov 8(%rsi), %r11
pext %rbp, %r11, %r10
mov $0x249249000000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
pext %r12, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 8(%rdi)
pext %r13, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
pext %r14, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 24(%rdi)
pext %r15, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 32(%rdi)
pext %r9, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 40(%rdi)
pext %r8, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 48(%rdi)
pext %rdx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 56(%rdi)
mov 16(%rsi), %r11
mov $0x8080810101010101, %rax
pext %rax, %r11, %r10
mov $0x9249248000000000, %rbx
pdep %rbx, %r10, %r10
rol $9, %r10
xor %r10, 0(%rdi)
mov $0x101020202020202, %rbp
pext %rbp, %r11, %r10
mov $0x9249240000000000, %r12
pdep %r12, %r10, %r10
rol $6, %r10
xor %r10, 8(%rdi)
mov $0x202040404040404, %r13
pext %r13, %r11, %r10
pdep %r12, %r10, %r10
rol $6, %r10
xor %r10, 16(%rdi)
mov $0x404080808080808, %r14
pext %r14, %r11, %r10
pdep %r12, %r10, %r10
rol $6, %r10
xor %r10, 24(%rdi)
mov $0x808101010101010, %r15
pext %r15, %r11, %r10
pdep %r12, %r10, %r10
rol $6, %r10
xor %r10, 32(%rdi)
mov $0x1010202020202020, %r9
pext %r9, %r11, %r10
pdep %r12, %r10, %r10
rol $6, %r10
xor %r10, 40(%rdi)
mov $0x2020404040404040, %r8
pext %r8, %r11, %r10
pdep %r12, %r10, %r10
rol $6, %r10
xor %r10, 48(%rdi)
mov $0x4040008080808080, %rdx
pext %rdx, %r11, %r10
mov $0x9049240000000000, %rcx
pdep %rcx, %r10, %r10
rol $6, %r10
xor %r10, 56(%rdi)
mov 24(%rsi), %r11
mov $0x8080808080808080, %rax
pext %rax, %r11, %r10
mov $0x124924800, %rbx
pdep %rbx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x101010101010101, %rbp
pext %rbp, %r11, %r10
mov $0x24924900, %r13
pdep %r13, %r10, %r10
xor %r10, 8(%rdi)
mov $0x202020202020202, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 16(%rdi)
mov $0x404040404040404, %r15
pext %r15, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 24(%rdi)
mov $0x808080808080808, %r9
pext %r9, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov $0x1010101010101010, %r8
pext %r8, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 40(%rdi)
mov $0x2020202020202020, %r12
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov $0x4040404040404040, %rdx
pext %rdx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov 32(%rsi), %r11
pext %rax, %r11, %r10
mov $0x124924800000000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
pext %rbp, %r11, %r10
mov $0x24924900000000, %rbx
pdep %rbx, %r10, %r10
xor %r10, 8(%rdi)
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 16(%rdi)
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 24(%rdi)
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 32(%rdi)
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 40(%rdi)
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 48(%rdi)
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 56(%rdi)
mov 40(%rsi), %r11
mov $0x4040404040408080, %r13
pext %r13, %r11, %r10
mov $0x9249240000000000, %rax
pdep %rax, %r10, %r10
rol $17, %r10
xor %r10, 0(%rdi)
mov $0x8080808080810101, %rcx
pext %rcx, %r11, %r10
mov $0x9249248000000000, %rbp
pdep %rbp, %r10, %r10
rol $17, %r10
xor %r10, 8(%rdi)
mov $0x101010101020202, %r14
pext %r14, %r11, %r10
pdep %rax, %r10, %r10
rol $14, %r10
xor %r10, 16(%rdi)
mov $0x202020202040404, %r15
pext %r15, %r11, %r10
pdep %rax, %r10, %r10
rol $14, %r10
xor %r10, 24(%rdi)
mov $0x404040404080808, %r9
pext %r9, %r11, %r10
pdep %rax, %r10, %r10
rol $14, %r10
xor %r10, 32(%rdi)
mov $0x808080808101010, %r8
pext %r8, %r11, %r10
pdep %rax, %r10, %r10
rol $14, %r10
xor %r10, 40(%rdi)
mov $0x1010101010202020, %r12
pext %r12, %r11, %r10
pdep %rax, %r10, %r10
rol $14, %r10
xor %r10, 48(%rdi)
mov $0x2020202020004040, %rdx
pext %rdx, %r11, %r10
mov $0x9248240000000000, %rbx
pdep %rbx, %r10, %r10
rol $14, %r10
xor %r10, 56(%rdi)
mov 48(%rsi), %r11
mov $0x4040404040404040, %r13
pext %r13, %r11, %r10
mov $0x12492480000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x8080808080808080, %rbp
pext %rbp, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x101010101010101, %r14
pext %r14, %r11, %r10
mov $0x2492490000, %r15
pdep %r15, %r10, %r10
xor %r10, 16(%rdi)
mov $0x202020202020202, %r9
pext %r9, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
mov $0x404040404040404, %r8
pext %r8, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
mov $0x808080808080808, %r12
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov $0x1010101010101010, %rax
pext %rax, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
mov $0x2020202020202020, %rdx
pext %rdx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov 56(%rsi), %r11
mov $0x40404040404040, %rbx
pext %rbx, %r11, %r10
mov $0x2492480000000000, %r13
pdep %r13, %r10, %r10
xor %r10, 0(%rdi)
mov $0x80808080808080, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 8(%rdi)
pext %r14, %r11, %r10
mov $0x2492490000000000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
pext %r9, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 24(%rdi)
pext %r8, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 32(%rdi)
pext %r12, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 40(%rdi)
pext %rax, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x20202020202020, %rdx
pext %rdx, %r11, %r10
mov $0x492490000000000, %r15
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
