.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS2048509_AVX2_square_3_509
.global _PQCLEAN_NTRUHPS2048509_AVX2_square_3_509
PQCLEAN_NTRUHPS2048509_AVX2_square_3_509:
_PQCLEAN_NTRUHPS2048509_AVX2_square_3_509:
push %r15
push %r14
push %r13
push %r12
push %rbx
push %rbp
mov 0(%rsi), %r11
mov %r11, %r10
and $0xff, %r10
mov $0x101010101010101, %rbp
pdep %rbp, %r10, %r10
mov %r10, 0(%rdi)
mov $0xff00, %rbx
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 8(%rdi)
mov $0xff0000, %r12
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 16(%rdi)
mov $0xff000000, %r13
pext %r13, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 24(%rdi)
mov $0xff00000000, %r14
pext %r14, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 32(%rdi)
mov $0xff0000000000, %r15
pext %r15, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 40(%rdi)
mov $0xff000000000000, %r9
pext %r9, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 48(%rdi)
mov $0xff00000000000000, %r8
pext %r8, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 56(%rdi)
mov 8(%rsi), %r11
mov %r11, %r10
and $0xff, %r10
mov $0x808080808080808, %rdx
pdep %rdx, %r10, %r10
xor %r10, 0(%rdi)
pext %rbx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 8(%rdi)
pext %r12, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 16(%rdi)
pext %r13, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
pext %r14, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 32(%rdi)
pext %r15, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 40(%rdi)
pext %r9, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 48(%rdi)
pext %r8, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
mov 16(%rsi), %r11
mov $0x80000000000000ff, %rcx
pext %rcx, %r11, %r10
mov $0x9010101010101010, %rax
pdep %rax, %r10, %r10
rol $2, %r10
xor %r10, 0(%rdi)
pext %rbx, %r11, %r10
mov $0x4040404040404040, %rbp
pdep %rbp, %r10, %r10
xor %r10, 8(%rdi)
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 16(%rdi)
pext %r13, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 24(%rdi)
pext %r14, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 32(%rdi)
pext %r15, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 40(%rdi)
pext %r9, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 48(%rdi)
mov $0x7f00000000000000, %r8
pext %r8, %r11, %r10
mov $0x40404040404040, %rdx
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
mov 24(%rsi), %r11
mov $0x800000000000007f, %rcx
pext %rcx, %r11, %r10
mov $0x8010101010101010, %rax
pdep %rax, %r10, %r10
rol $5, %r10
xor %r10, 0(%rdi)
mov $0x7f80, %rbx
pext %rbx, %r11, %r10
mov $0x202020202020202, %r12
pdep %r12, %r10, %r10
xor %r10, 8(%rdi)
mov $0x7f8000, %r13
pext %r13, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 16(%rdi)
mov $0x7f800000, %r14
pext %r14, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 24(%rdi)
mov $0x7f80000000, %r15
pext %r15, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 32(%rdi)
mov $0x7f8000000000, %r9
pext %r9, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 40(%rdi)
mov $0x7f800000000000, %rbp
pext %rbp, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 48(%rdi)
mov $0x7f80000000000000, %r8
pext %r8, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 56(%rdi)
mov 32(%rsi), %r11
pext %rcx, %r11, %r10
pdep %rax, %r10, %r10
rol $8, %r10
xor %r10, 0(%rdi)
pext %rbx, %r11, %r10
mov $0x1010101010101010, %rdx
pdep %rdx, %r10, %r10
xor %r10, 8(%rdi)
pext %r13, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 16(%rdi)
pext %r14, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
pext %r15, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 32(%rdi)
pext %r9, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 40(%rdi)
pext %rbp, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 48(%rdi)
pext %r8, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
mov 40(%rsi), %r11
mov $0xc00000000000007f, %r12
pext %r12, %r11, %r10
mov $0x8090101010101010, %rcx
pdep %rcx, %r10, %r10
rol $11, %r10
xor %r10, 0(%rdi)
pext %rbx, %r11, %r10
mov $0x8080808080808080, %rax
pdep %rax, %r10, %r10
xor %r10, 8(%rdi)
pext %r13, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 16(%rdi)
pext %r14, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 24(%rdi)
pext %r15, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 32(%rdi)
pext %r9, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 40(%rdi)
pext %rbp, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 48(%rdi)
mov $0x3f80000000000000, %r8
pext %r8, %r11, %r10
mov $0x80808080808080, %rdx
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
mov 48(%rsi), %r11
mov $0xc00000000000003f, %r12
pext %r12, %r11, %r10
mov $0x8080101010101010, %rcx
pdep %rcx, %r10, %r10
rol $14, %r10
xor %r10, 0(%rdi)
mov $0x3fc0, %rbx
pext %rbx, %r11, %r10
mov $0x404040404040404, %r13
pdep %r13, %r10, %r10
xor %r10, 8(%rdi)
mov $0x3fc000, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3fc00000, %r15
pext %r15, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 24(%rdi)
mov $0x3fc0000000, %r9
pext %r9, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov $0x3fc000000000, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 40(%rdi)
mov $0x3fc00000000000, %rax
pext %rax, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov $0x3fc0000000000000, %r8
pext %r8, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov 56(%rsi), %r11
mov %r11, %r10
and $0x3f, %r10
mov $0x2020202020200000, %rdx
pdep %rdx, %r10, %r10
xor %r10, 0(%rdi)
pext %rbx, %r11, %r10
mov $0x2020202020202020, %r12
pdep %r12, %r10, %r10
xor %r10, 8(%rdi)
pext %r14, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 16(%rdi)
pext %r15, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 24(%rdi)
pext %r9, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 32(%rdi)
pext %rbp, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 40(%rdi)
pext %rax, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1fc0000000000000, %rcx
pext %rcx, %r11, %r10
mov $0x20202020202020, %r8
pdep %r8, %r10, %r10
xor %r10, 56(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
