.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS2048677_AVX2_square_3_677
.global _PQCLEAN_NTRUHPS2048677_AVX2_square_3_677
PQCLEAN_NTRUHPS2048677_AVX2_square_3_677:
_PQCLEAN_NTRUHPS2048677_AVX2_square_3_677:
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
mov $0x1fe00000, %rdx
pext %rdx, %r11, %r10
mov $0x808080808080808, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x1fe0000000, %rax
pext %rax, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x1fe000000000, %rbx
pext %rbx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x1fe00000000000, %r12
pext %r12, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x1fe0000000000000, %r13
pext %r13, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 32(%rdi)
mov $0xe000000000000000, %r14
pext %r14, %r11, %r10
mov $0x80808, %r15
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
and $0xff, %r10
pdep %rbp, %r10, %r10
mov %r10, 64(%rdi)
mov $0xff00, %r9
pext %r9, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 72(%rdi)
mov $0x1f0000, %r8
pext %r8, %r11, %r10
mov $0x101010101, %rdx
pdep %rdx, %r10, %r10
mov %r10, 80(%rdi)
mov 16(%rsi), %r11
mov $0x3fc0000000000, %rax
pext %rax, %r11, %r10
mov $0x4040404040404040, %rbx
pdep %rbx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x3fc000000000000, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 8(%rdi)
mov $0xfc00000000000000, %r13
pext %r13, %r11, %r10
mov $0x404040404040, %rcx
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x1f, %r10
mov $0x808080808000000, %r14
pdep %r14, %r10, %r10
xor %r10, 40(%rdi)
mov $0x1fe0, %r15
pext %r15, %r11, %r10
mov $0x808080808080808, %r9
pdep %r9, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1fe000, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 56(%rdi)
mov $0x1fe00000, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 64(%rdi)
mov $0x1fe0000000, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 72(%rdi)
mov $0x3e000000000, %rax
pext %rax, %r11, %r10
mov $0x808080808, %r12
pdep %r12, %r10, %r10
xor %r10, 80(%rdi)
mov 24(%rsi), %r11
mov $0xc000000000000000, %rbx
pext %rbx, %r11, %r10
mov $0x202, %r13
pdep %r13, %r10, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
and $0x3, %r10
mov $0x4040000000000000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3fc, %r14
pext %r14, %r11, %r10
mov $0x4040404040404040, %r15
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
mov $0x3fc00, %rbp
pext %rbp, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
mov $0x3fc0000, %r8
pext %r8, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov $0x3fc000000, %rdx
pext %rdx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
mov $0x3fc00000000, %r9
pext %r9, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov $0x3fc0000000000, %rax
pext %rax, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 64(%rdi)
mov $0x3fc000000000000, %r12
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov $0x3c00000000000000, %rbx
pext %rbx, %r11, %r10
mov $0x40404040, %r13
pdep %r13, %r10, %r10
xor %r10, 80(%rdi)
mov 32(%rsi), %r11
mov %r11, %r10
and $0x3f, %r10
mov $0x202020202020000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x3fc0, %r14
pext %r14, %r11, %r10
mov $0x202020202020202, %rbp
pdep %rbp, %r10, %r10
xor %r10, 8(%rdi)
mov $0x3fc000, %r8
pext %r8, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3fc00000, %rdx
pext %rdx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 24(%rdi)
mov $0x3fc0000000, %r9
pext %r9, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 32(%rdi)
mov $0x3fc000000000, %rax
pext %rax, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 40(%rdi)
mov $0x3fc00000000000, %r12
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 48(%rdi)
mov $0x3fc0000000000000, %r15
pext %r15, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 56(%rdi)
mov $0xc000000000000000, %rbx
pext %rbx, %r11, %r10
mov $0x202, %r13
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
mov 40(%rsi), %r11
mov $0x7f80000, %rcx
pext %rcx, %r11, %r10
mov $0x1010101010101010, %r14
pdep %r14, %r10, %r10
xor %r10, 0(%rdi)
mov $0x7f8000000, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 8(%rdi)
mov $0x7f800000000, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 16(%rdi)
mov $0x7f80000000000, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 24(%rdi)
mov $0x7f8000000000000, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0xf800000000000000, %r12
pext %r12, %r11, %r10
mov $0x1010101010, %r15
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
and $0x3f, %r10
mov $0x202020202020000, %rbp
pdep %rbp, %r10, %r10
xor %r10, 64(%rdi)
mov $0x3fc0, %rbx
pext %rbx, %r11, %r10
mov $0x202020202020202, %r13
pdep %r13, %r10, %r10
xor %r10, 72(%rdi)
mov $0x7c000, %rcx
pext %rcx, %r11, %r10
mov $0x202020202, %r8
pdep %r8, %r10, %r10
xor %r10, 80(%rdi)
mov 48(%rsi), %r11
mov $0xff0000000000, %rdx
pext %rdx, %r11, %r10
mov $0x8080808080808080, %r9
pdep %r9, %r10, %r10
xor %r10, 0(%rdi)
mov $0xff000000000000, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 8(%rdi)
mov $0xff00000000000000, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x7, %r10
mov $0x1010100000000000, %r12
pdep %r12, %r10, %r10
xor %r10, 40(%rdi)
mov $0x7f8, %r15
pext %r15, %r11, %r10
mov $0x1010101010101010, %rbp
pdep %rbp, %r10, %r10
xor %r10, 48(%rdi)
mov $0x7f800, %rbx
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 56(%rdi)
mov $0x7f80000, %r13
pext %r13, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 64(%rdi)
mov $0x7f8000000, %rcx
pext %rcx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 72(%rdi)
mov $0xf800000000, %r8
pext %r8, %r11, %r10
mov $0x1010101010, %rdx
pdep %rdx, %r10, %r10
xor %r10, 80(%rdi)
mov 56(%rsi), %r11
mov $0xf000000000000000, %rax
pext %rax, %r11, %r10
mov $0x4040404, %r14
pdep %r14, %r10, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
and $0xff, %r10
pdep %r9, %r10, %r10
xor %r10, 24(%rdi)
mov $0xff00, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 32(%rdi)
mov $0xff0000, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 40(%rdi)
mov $0xff000000, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 48(%rdi)
mov $0xff00000000, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 56(%rdi)
mov $0xff0000000000, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 64(%rdi)
mov $0xff000000000000, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 72(%rdi)
mov $0xf00000000000000, %r8
pext %r8, %r11, %r10
mov $0x80808080, %rdx
pdep %rdx, %r10, %r10
xor %r10, 80(%rdi)
mov 64(%rsi), %r11
mov %r11, %r10
and $0xf, %r10
mov $0x404040400000000, %rax
pdep %rax, %r10, %r10
xor %r10, 0(%rdi)
mov $0xff0, %r14
pext %r14, %r11, %r10
mov $0x404040404040404, %r12
pdep %r12, %r10, %r10
xor %r10, 8(%rdi)
mov $0xff000, %r15
pext %r15, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 16(%rdi)
mov $0xff00000, %rbx
pext %rbx, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 24(%rdi)
mov $0xff0000000, %r13
pext %r13, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 32(%rdi)
mov $0xff000000000, %rcx
pext %rcx, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 40(%rdi)
mov $0xff00000000000, %rbp
pext %rbp, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 48(%rdi)
mov $0xff0000000000000, %r9
pext %r9, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 56(%rdi)
mov $0xf000000000000000, %r8
pext %r8, %r11, %r10
mov $0x4040404, %rdx
pdep %rdx, %r10, %r10
xor %r10, 64(%rdi)
mov 72(%rsi), %r11
mov $0x1fe0000, %rax
pext %rax, %r11, %r10
mov $0x2020202020202020, %r14
pdep %r14, %r10, %r10
xor %r10, 0(%rdi)
mov $0x1fe000000, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 8(%rdi)
mov $0x1fe00000000, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 16(%rdi)
mov $0x1fe0000000000, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 24(%rdi)
mov $0x1fe000000000000, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0xfe00000000000000, %rbp
pext %rbp, %r11, %r10
mov $0x20202020202020, %r9
pdep %r9, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
and $0xf, %r10
mov $0x404040400000000, %r12
pdep %r12, %r10, %r10
xor %r10, 64(%rdi)
mov $0xff0, %r8
pext %r8, %r11, %r10
mov $0x404040404040404, %rdx
pdep %rdx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1f000, %rax
pext %rax, %r11, %r10
mov $0x404040404, %r15
pdep %r15, %r10, %r10
xor %r10, 80(%rdi)
mov 80(%rsi), %r11
mov %r11, %r10
and $0x1, %r10
rol $61, %r10
xor %r10, 40(%rdi)
mov $0x1fe, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1fe00, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 56(%rdi)
mov $0x1fe0000, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 64(%rdi)
mov $0x1fe000000, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1e00000000, %r9
pext %r9, %r11, %r10
mov $0x20202020, %r12
pdep %r12, %r10, %r10
xor %r10, 80(%rdi)
movq $0x0, 88(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
