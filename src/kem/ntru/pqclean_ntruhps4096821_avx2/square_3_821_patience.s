.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS4096821_AVX2_square_3_821
.global _PQCLEAN_NTRUHPS4096821_AVX2_square_3_821
PQCLEAN_NTRUHPS4096821_AVX2_square_3_821:
_PQCLEAN_NTRUHPS4096821_AVX2_square_3_821:
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
mov $0x7f8000000000, %rdx
pext %rdx, %r11, %r10
mov $0x808080808080808, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x7f800000000000, %rax
pext %rax, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x7f80000000000000, %rbx
pext %rbx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
rol $4, %r10
and $0x8, %r10
xor %r10, 24(%rdi)
mov %r11, %r10
and $0xff, %r10
pdep %rbp, %r10, %r10
mov %r10, 64(%rdi)
mov $0xff00, %r12
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 72(%rdi)
mov $0xff0000, %r13
pext %r13, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 80(%rdi)
mov $0xff000000, %r14
pext %r14, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 88(%rdi)
mov $0x7f00000000, %r15
pext %r15, %r11, %r10
mov $0x1010101010101, %r9
pdep %r9, %r10, %r10
mov %r10, 96(%rdi)
mov 16(%rsi), %r11
mov %r11, %r10
and $0x7f, %r10
mov $0x808080808080800, %r8
pdep %r8, %r10, %r10
xor %r10, 24(%rdi)
mov $0x7f80, %rdx
pext %rdx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x7f8000, %rax
pext %rax, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x7f800000, %rbx
pext %rbx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x7f80000000, %r12
pext %r12, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x7f8000000000, %r13
pext %r13, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x7f800000000000, %r14
pext %r14, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x7f80000000000000, %rbp
pext %rbp, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 80(%rdi)
mov %r11, %r10
rol $4, %r10
and $0x8, %r10
xor %r10, 88(%rdi)
mov 24(%rsi), %r11
mov $0x3fc000, %r15
pext %r15, %r11, %r10
mov $0x4040404040404040, %r9
pdep %r9, %r10, %r10
xor %r10, 0(%rdi)
mov $0x3fc00000, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 8(%rdi)
mov $0x3fc0000000, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3fc000000000, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 24(%rdi)
mov $0x3fc00000000000, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 32(%rdi)
mov $0x3fc0000000000000, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 40(%rdi)
mov $0xc000000000000000, %r13
pext %r13, %r11, %r10
mov $0x4040, %r14
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
mov %r11, %r10
and $0x7f, %r10
mov $0x808080808080800, %rbp
pdep %rbp, %r10, %r10
xor %r10, 88(%rdi)
mov $0x3f80, %rcx
pext %rcx, %r11, %r10
mov $0x8080808080808, %r15
pdep %r15, %r10, %r10
xor %r10, 96(%rdi)
mov 32(%rsi), %r11
mov $0xff0000000000000, %r8
pext %r8, %r11, %r10
mov $0x202020202020202, %rdx
pdep %rdx, %r10, %r10
xor %r10, 0(%rdi)
mov $0xf000000000000000, %rax
pext %rax, %r11, %r10
mov $0x2020202, %rbx
pdep %rbx, %r10, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0x3f, %r10
mov $0x4040404040400000, %r12
pdep %r12, %r10, %r10
xor %r10, 48(%rdi)
mov $0x3fc0, %r9
pext %r9, %r11, %r10
mov $0x4040404040404040, %r13
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov $0x3fc000, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
mov $0x3fc00000, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 72(%rdi)
mov $0x3fc0000000, %rcx
pext %rcx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 80(%rdi)
mov $0x3fc000000000, %r15
pext %r15, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 88(%rdi)
mov $0xfc00000000000, %r8
pext %r8, %r11, %r10
mov $0x404040404040, %rdx
pdep %rdx, %r10, %r10
xor %r10, 96(%rdi)
mov 40(%rsi), %r11
mov %r11, %r10
and $0xf, %r10
mov $0x202020200000000, %rax
pdep %rax, %r10, %r10
xor %r10, 8(%rdi)
mov $0xff0, %rbx
pext %rbx, %r11, %r10
mov $0x202020202020202, %r12
pdep %r12, %r10, %r10
xor %r10, 16(%rdi)
mov $0xff000, %r9
pext %r9, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 24(%rdi)
mov $0xff00000, %r14
pext %r14, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 32(%rdi)
mov $0xff0000000, %rbp
pext %rbp, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 40(%rdi)
mov $0xff000000000, %rcx
pext %rcx, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 48(%rdi)
mov $0xff00000000000, %r15
pext %r15, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 56(%rdi)
mov $0xff0000000000000, %r13
pext %r13, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 64(%rdi)
mov $0xf000000000000000, %r8
pext %r8, %r11, %r10
mov $0x2020202, %rdx
pdep %rdx, %r10, %r10
xor %r10, 72(%rdi)
mov 48(%rsi), %r11
mov $0x7f8000000, %rax
pext %rax, %r11, %r10
mov $0x1010101010101010, %rbx
pdep %rbx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x7f800000000, %r9
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x7f80000000000, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x7f8000000000000, %rbp
pext %rbp, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 24(%rdi)
mov $0xf800000000000000, %rcx
pext %rcx, %r11, %r10
mov $0x1010101010, %r15
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
mov %r11, %r10
and $0xf, %r10
mov $0x202020200000000, %r13
pdep %r13, %r10, %r10
xor %r10, 72(%rdi)
mov $0xff0, %r12
pext %r12, %r11, %r10
mov $0x202020202020202, %r8
pdep %r8, %r10, %r10
xor %r10, 80(%rdi)
mov $0xff000, %rdx
pext %rdx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 88(%rdi)
mov $0x7f00000, %rax
pext %rax, %r11, %r10
mov $0x2020202020202, %r9
pdep %r9, %r10, %r10
xor %r10, 96(%rdi)
mov 56(%rsi), %r11
mov %r11, %r10
and $0x7, %r10
mov $0x1010100000000000, %r14
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0x7f8, %rbp
pext %rbp, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x7f800, %rcx
pext %rcx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x7f80000, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x7f8000000, %r13
pext %r13, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x7f800000000, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x7f80000000000, %rdx
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 80(%rdi)
mov $0x7f8000000000000, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 88(%rdi)
mov $0xf800000000000000, %rax
pext %rax, %r11, %r10
mov $0x1010101010, %r9
pdep %r9, %r10, %r10
xor %r10, 96(%rdi)
mov 64(%rsi), %r11
mov $0x3fc, %r14
pext %r14, %r11, %r10
mov $0x8080808080808080, %rbp
pdep %rbp, %r10, %r10
xor %r10, 0(%rdi)
mov $0x3fc00, %rcx
pext %rcx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 8(%rdi)
mov $0x3fc0000, %r15
pext %r15, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3fc000000, %r13
pext %r13, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 24(%rdi)
mov $0x3fc00000000, %r12
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 32(%rdi)
mov $0x3fc0000000000, %rdx
pext %rdx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 40(%rdi)
mov $0x3fc000000000000, %r8
pext %r8, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 48(%rdi)
mov $0xfc00000000000000, %rbx
pext %rbx, %r11, %r10
mov $0x808080808080, %rax
pdep %rax, %r10, %r10
xor %r10, 56(%rdi)
mov %r11, %r10
and $0x3, %r10
mov $0x10100000000000, %r9
pdep %r9, %r10, %r10
xor %r10, 96(%rdi)
mov 72(%rsi), %r11
mov $0xff0000000000, %r14
pext %r14, %r11, %r10
mov $0x404040404040404, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
mov $0xff000000000000, %r15
pext %r15, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 8(%rdi)
mov $0xff00000000000000, %r13
pext %r13, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x3, %r10
mov $0x8080000000000000, %r12
pdep %r12, %r10, %r10
xor %r10, 56(%rdi)
mov $0x3fc, %rdx
pext %rdx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 64(%rdi)
mov $0x3fc00, %r8
pext %r8, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 72(%rdi)
mov $0x3fc0000, %rbx
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 80(%rdi)
mov $0x3fc000000, %rax
pext %rax, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 88(%rdi)
mov $0xfc00000000, %r9
pext %r9, %r11, %r10
mov $0x808080808080, %r14
pdep %r14, %r10, %r10
xor %r10, 96(%rdi)
mov 80(%rsi), %r11
mov %r11, %r10
and $0xff, %r10
pdep %rcx, %r10, %r10
xor %r10, 24(%rdi)
mov $0xff00, %r15
pext %r15, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 32(%rdi)
mov $0xff0000, %r13
pext %r13, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 40(%rdi)
mov $0xff000000, %r12
pext %r12, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 48(%rdi)
mov $0xff00000000, %rdx
pext %rdx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 56(%rdi)
mov $0xff0000000000, %r8
pext %r8, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 64(%rdi)
mov $0xff000000000000, %rbx
pext %rbx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 72(%rdi)
mov $0xff00000000000000, %rax
pext %rax, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 80(%rdi)
mov 88(%rsi), %r11
mov $0x7f8000, %rbp
pext %rbp, %r11, %r10
mov $0x2020202020202020, %r9
pdep %r9, %r10, %r10
xor %r10, 0(%rdi)
mov $0x7f800000, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 8(%rdi)
mov $0x7f80000000, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 16(%rdi)
mov $0x7f8000000000, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 24(%rdi)
mov $0x7f800000000000, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 32(%rdi)
mov $0x7f80000000000000, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
rol $6, %r10
and $0x20, %r10
xor %r10, 48(%rdi)
mov %r11, %r10
and $0xff, %r10
pdep %rcx, %r10, %r10
xor %r10, 88(%rdi)
mov $0x7f00, %r8
pext %r8, %r11, %r10
mov $0x4040404040404, %rbx
pdep %rbx, %r10, %r10
xor %r10, 96(%rdi)
mov 96(%rsi), %r11
mov %r11, %r10
and $0x7f, %r10
mov $0x2020202020202000, %rax
pdep %rax, %r10, %r10
xor %r10, 48(%rdi)
mov $0x7f80, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 56(%rdi)
mov $0x7f8000, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 64(%rdi)
mov $0x7f800000, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 72(%rdi)
mov $0x7f80000000, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 80(%rdi)
mov $0x7f8000000000, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 88(%rdi)
mov $0x1f800000000000, %rdx
pext %rdx, %r11, %r10
mov $0x202020202020, %rcx
pdep %rcx, %r10, %r10
xor %r10, 96(%rdi)
mov 104(%rsi), %r11
mov 112(%rsi), %r11
mov 120(%rsi), %r11
movq $0x0, 104(%rdi)
movq $0x0, 112(%rdi)
movq $0x0, 120(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
