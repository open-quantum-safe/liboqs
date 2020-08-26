.data
.p2align 5
.text
.global PQCLEAN_NTRUHRSS701_AVX2_square_1_701
.global _PQCLEAN_NTRUHRSS701_AVX2_square_1_701
PQCLEAN_NTRUHRSS701_AVX2_square_1_701:
_PQCLEAN_NTRUHRSS701_AVX2_square_1_701:
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
and $-0x1, %r10
pdep %rbp, %r10, %r10
mov %r10, 48(%rdi)
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 56(%rdi)
mov 32(%rsi), %r11
mov %r11, %r10
and $-0x1, %r10
pdep %rbp, %r10, %r10
mov %r10, 64(%rdi)
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 72(%rdi)
mov 40(%rsi), %r11
mov $0x7fffffff80000000, %r12
pext %r12, %r11, %r10
mov $0xaaaaaaaaaaaaaaaa, %r13
pdep %r13, %r10, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0x7fffffff, %r10
mov $0x1555555555555555, %r14
pdep %r14, %r10, %r10
mov %r10, 80(%rdi)
mov 48(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
mov $0xaaaaaaaaaaaaaaa8, %r15
pdep %r15, %r10, %r10
xor %r10, 8(%rdi)
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 24(%rdi)
mov 56(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 40(%rdi)
mov 64(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 56(%rdi)
mov 72(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
mov %r11, %r10
rol $2, %r10
and $0x2, %r10
xor %r10, 72(%rdi)
mov 80(%rsi), %r11
mov %r11, %r10
and $0x7fffffff, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1fffffff80000000, %r9
pext %r9, %r11, %r10
mov $0xaaaaaaaaaaaaaaa, %r8
pdep %r8, %r10, %r10
xor %r10, 80(%rdi)
movq $0x0, 88(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
