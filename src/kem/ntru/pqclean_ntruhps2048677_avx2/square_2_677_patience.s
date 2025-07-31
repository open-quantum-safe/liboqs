.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS2048677_AVX2_square_2_677
.global _PQCLEAN_NTRUHPS2048677_AVX2_square_2_677
PQCLEAN_NTRUHPS2048677_AVX2_square_2_677:
_PQCLEAN_NTRUHPS2048677_AVX2_square_2_677:
push %r15
push %r14
push %r13
push %r12
push %rbx
push %rbp
mov 0(%rsi), %r11
mov %r11, %r10
and $0xffff, %r10
mov $0x1111111111111111, %rbp
pdep %rbp, %r10, %r10
mov %r10, 0(%rdi)
mov $0xffff0000, %rbx
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 8(%rdi)
mov $0xffff00000000, %r12
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 16(%rdi)
mov $0xffff000000000000, %r13
pext %r13, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 24(%rdi)
mov 8(%rsi), %r11
mov %r11, %r10
and $0xffff, %r10
pdep %rbp, %r10, %r10
mov %r10, 32(%rdi)
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 40(%rdi)
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 48(%rdi)
pext %r13, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 56(%rdi)
mov 16(%rsi), %r11
mov $0x3fffc0000000000, %r14
pext %r14, %r11, %r10
mov $0x8888888888888888, %r15
pdep %r15, %r10, %r10
xor %r10, 0(%rdi)
mov $0xfc00000000000000, %r9
pext %r9, %r11, %r10
mov $0x888888, %r8
pdep %r8, %r10, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0xffff, %r10
pdep %rbp, %r10, %r10
mov %r10, 64(%rdi)
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 72(%rdi)
mov $0x3ff00000000, %rdx
pext %rdx, %r11, %r10
mov $0x1111111111, %rcx
pdep %rcx, %r10, %r10
mov %r10, 80(%rdi)
mov 24(%rsi), %r11
mov %r11, %r10
and $0x3ff, %r10
mov $0x8888888888000000, %rax
pdep %rax, %r10, %r10
xor %r10, 8(%rdi)
mov $0x3fffc00, %r12
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3fffc000000, %r13
pext %r13, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
pext %r9, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 40(%rdi)
mov 32(%rsi), %r11
mov %r11, %r10
and $0x3ff, %r10
pdep %rax, %r10, %r10
xor %r10, 40(%rdi)
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
pext %r13, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 64(%rdi)
pext %r9, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 72(%rdi)
mov 40(%rsi), %r11
mov $0x7fff80000, %rbx
pext %rbx, %r11, %r10
mov $0x4444444444444444, %rbp
pdep %rbp, %r10, %r10
xor %r10, 0(%rdi)
mov $0x7fff800000000, %rdx
pext %rdx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 8(%rdi)
mov $0xfff8000000000000, %rcx
pext %rcx, %r11, %r10
mov $0x4444444444444, %rax
pdep %rax, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x3ff, %r10
mov $0x8888888888000000, %r12
pdep %r12, %r10, %r10
xor %r10, 72(%rdi)
mov $0x7fc00, %r13
pext %r13, %r11, %r10
mov $0x888888888, %r14
pdep %r14, %r10, %r10
xor %r10, 80(%rdi)
mov 48(%rsi), %r11
mov %r11, %r10
and $0x7, %r10
mov $0x4440000000000000, %r15
pdep %r15, %r10, %r10
xor %r10, 16(%rdi)
mov $0x7fff8, %r9
pext %r9, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 24(%rdi)
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 32(%rdi)
pext %rdx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 40(%rdi)
pext %rcx, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 48(%rdi)
mov 56(%rsi), %r11
mov $0xf000000000000000, %r8
pext %r8, %r11, %r10
mov $0x2222, %r12
pdep %r12, %r10, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
and $0x7, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
pext %r9, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 56(%rdi)
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 64(%rdi)
pext %rdx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 72(%rdi)
mov $0xff8000000000000, %r13
pext %r13, %r11, %r10
mov $0x444444444, %r14
pdep %r14, %r10, %r10
xor %r10, 80(%rdi)
mov 64(%rsi), %r11
mov %r11, %r10
and $0xfff, %r10
mov $0x2222222222220000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
mov $0xffff000, %rax
pext %rax, %r11, %r10
mov $0x2222222222222222, %r8
pdep %r8, %r10, %r10
xor %r10, 8(%rdi)
mov $0xffff0000000, %r12
pext %r12, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 16(%rdi)
mov $0xffff00000000000, %r15
pext %r15, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 24(%rdi)
mov $0xf000000000000000, %r9
pext %r9, %r11, %r10
mov $0x2222, %rbx
pdep %rbx, %r10, %r10
xor %r10, 32(%rdi)
mov 72(%rsi), %r11
mov %r11, %r10
and $0xfff, %r10
pdep %rcx, %r10, %r10
xor %r10, 32(%rdi)
pext %rax, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 40(%rdi)
pext %r12, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 48(%rdi)
pext %r15, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 56(%rdi)
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 64(%rdi)
mov 80(%rsi), %r11
mov %r11, %r10
and $0xfff, %r10
pdep %rcx, %r10, %r10
xor %r10, 64(%rdi)
pext %rax, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1ff0000000, %rdx
pext %rdx, %r11, %r10
mov $0x222222222, %rbp
pdep %rbp, %r10, %r10
xor %r10, 80(%rdi)
movq $0x0, 88(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
