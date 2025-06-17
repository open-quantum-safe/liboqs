.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS2048509_AVX2_square_1_509
.global _PQCLEAN_NTRUHPS2048509_AVX2_square_1_509
PQCLEAN_NTRUHPS2048509_AVX2_square_1_509:
_PQCLEAN_NTRUHPS2048509_AVX2_square_1_509:
push %r15
push %r14
push %r13
push %r12
push %rbx
push %rbp
mov 0(%rsi), %r11
mov %r11, %r10
and $-0x1, %r10
mov $0x5555555555555555, %rbp
pdep %rbp, %r10, %r10
mov %r10, 0(%rdi)
mov $0xffffffff00000000, %rbx
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 8(%rdi)
mov 8(%rsi), %r11
mov %r11, %r10
and $-0x1, %r10
pdep %rbp, %r10, %r10
mov %r10, 16(%rdi)
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 24(%rdi)
mov 16(%rsi), %r11
mov %r11, %r10
and $-0x1, %r10
pdep %rbp, %r10, %r10
mov %r10, 32(%rdi)
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 40(%rdi)
mov 24(%rsi), %r11
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
and $-0x1, %r10
pdep %rbp, %r10, %r10
mov %r10, 48(%rdi)
mov $0x7fffffff00000000, %r12
pext %r12, %r11, %r10
mov $0x1555555555555555, %r13
pdep %r13, %r10, %r10
mov %r10, 56(%rdi)
mov 32(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
mov $0xaaaaaaaaaaaaaaa8, %r14
pdep %r14, %r10, %r10
xor %r10, 0(%rdi)
mov $0x7fffffff80000000, %r15
pext %r15, %r11, %r10
mov $0xaaaaaaaaaaaaaaaa, %r9
pdep %r9, %r10, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 16(%rdi)
mov 40(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
pdep %r14, %r10, %r10
xor %r10, 16(%rdi)
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 24(%rdi)
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 32(%rdi)
mov 48(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 48(%rdi)
mov 56(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1fffffff80000000, %r8
pext %r8, %r11, %r10
mov $0xaaaaaaaaaaaaaaa, %rdx
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
