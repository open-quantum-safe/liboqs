.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS2048677_AVX2_square_1_677
.global _PQCLEAN_NTRUHPS2048677_AVX2_square_1_677
PQCLEAN_NTRUHPS2048677_AVX2_square_1_677:
_PQCLEAN_NTRUHPS2048677_AVX2_square_1_677:
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
mov $0x7fffffff80000, %r12
pext %r12, %r11, %r10
mov $0xaaaaaaaaaaaaaaaa, %r13
pdep %r13, %r10, %r10
xor %r10, 0(%rdi)
mov $0xfff8000000000000, %r14
pext %r14, %r11, %r10
mov $0x2aaaaaa, %r15
pdep %r15, %r10, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0x7ffff, %r10
mov $0x1555555555, %r9
pdep %r9, %r10, %r10
mov %r10, 80(%rdi)
mov 48(%rsi), %r11
mov %r11, %r10
and $0x7ffff, %r10
mov $0xaaaaaaaaa8000000, %r8
pdep %r8, %r10, %r10
xor %r10, 8(%rdi)
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 16(%rdi)
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
mov 56(%rsi), %r11
mov %r11, %r10
and $0x7ffff, %r10
pdep %r8, %r10, %r10
xor %r10, 24(%rdi)
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov 64(%rsi), %r11
mov %r11, %r10
and $0x7ffff, %r10
pdep %r8, %r10, %r10
xor %r10, 40(%rdi)
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov 72(%rsi), %r11
mov %r11, %r10
and $0x7ffff, %r10
pdep %r8, %r10, %r10
xor %r10, 56(%rdi)
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov 80(%rsi), %r11
mov %r11, %r10
and $0x7ffff, %r10
pdep %r8, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1ffff80000, %rdx
pext %rdx, %r11, %r10
mov $0xaaaaaaaaa, %rcx
pdep %rcx, %r10, %r10
xor %r10, 80(%rdi)
movq $0x0, 88(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
