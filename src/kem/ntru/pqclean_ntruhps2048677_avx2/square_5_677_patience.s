.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS2048677_AVX2_square_5_677
.global _PQCLEAN_NTRUHPS2048677_AVX2_square_5_677
PQCLEAN_NTRUHPS2048677_AVX2_square_5_677:
_PQCLEAN_NTRUHPS2048677_AVX2_square_5_677:
push %r15
push %r14
push %r13
push %r12
push %rbx
push %rbp
mov 0(%rsi), %r11
mov $0x180000000001, %rbp
pext %rbp, %r11, %r10
mov $0x40000000400001, %rbx
pdep %rbx, %r10, %r10
mov %r10, 0(%rdi)
mov $0xc00000, %r12
pext %r12, %r11, %r10
mov $0x800000008000000, %r13
pdep %r13, %r10, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
and $0x2, %r10
rol $31, %r10
xor %r10, 0(%rdi)
mov $0x600000000004, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 8(%rdi)
mov $0x3000000, %r15
pext %r15, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0x8, %r10
rol $29, %r10
xor %r10, 8(%rdi)
mov $0x1800000000010, %r9
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 16(%rdi)
mov $0xc000000, %r8
pext %r8, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x20, %r10
rol $27, %r10
xor %r10, 16(%rdi)
mov $0x6000000000040, %rdx
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 24(%rdi)
mov $0x30000000, %rcx
pext %rcx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 24(%rdi)
mov %r11, %r10
and $0x80, %r10
rol $25, %r10
xor %r10, 24(%rdi)
mov $0x18000000000100, %rax
pext %rax, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 32(%rdi)
mov $0xc0000000, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov %r11, %r10
and $0x200, %r10
rol $23, %r10
xor %r10, 32(%rdi)
mov $0x60000000000400, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 40(%rdi)
mov $0x300000000, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
and $0x800, %r10
rol $21, %r10
xor %r10, 40(%rdi)
mov $0x180000000001000, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 48(%rdi)
mov $0xc00000000, %r9
pext %r9, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov %r11, %r10
and $0x2000, %r10
rol $19, %r10
xor %r10, 48(%rdi)
mov $0x600000000004000, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 56(%rdi)
mov $0x3000000000, %rdx
pext %rdx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov %r11, %r10
and $0x8000, %r10
rol $17, %r10
xor %r10, 56(%rdi)
mov $0x1800000000010000, %rcx
pext %rcx, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 64(%rdi)
mov $0xc000000000, %rax
pext %rax, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
mov %r11, %r10
and $0x20000, %r10
rol $15, %r10
xor %r10, 64(%rdi)
mov $0x6000000000040000, %rbp
pext %rbp, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 72(%rdi)
mov $0x30000000000, %r12
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 72(%rdi)
mov %r11, %r10
and $0x80000, %r10
rol $13, %r10
xor %r10, 72(%rdi)
mov $0x8000000000200000, %r14
pext %r14, %r11, %r10
mov $0x400000000000010, %r15
pdep %r15, %r10, %r10
rol $28, %r10
mov %r10, 80(%rdi)
mov $0x40000100000, %r9
pext %r9, %r11, %r10
mov $0x8000001000000000, %r8
pdep %r8, %r10, %r10
rol $28, %r10
xor %r10, 80(%rdi)
mov 8(%rsi), %r11
mov $0x80000c0000000000, %rdx
pext %rdx, %r11, %r10
mov $0x8000001000000010, %rcx
pdep %rcx, %r10, %r10
rol $3, %r10
xor %r10, 0(%rdi)
mov $0x600000, %rax
pext %rax, %r11, %r10
mov $0x20000000200, %rbp
pdep %rbp, %r10, %r10
rol $3, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
and $0x3, %r10
mov $0x400000004000, %rbx
pdep %rbx, %r10, %r10
rol $3, %r10
xor %r10, 0(%rdi)
mov $0x300000000000, %r12
pext %r12, %r11, %r10
mov $0x8000000080, %r13
pdep %r13, %r10, %r10
xor %r10, 8(%rdi)
mov $0x1800000, %r14
pext %r14, %r11, %r10
mov $0x100000001000, %r15
pdep %r15, %r10, %r10
xor %r10, 8(%rdi)
mov $0xc, %r9
pext %r9, %r11, %r10
mov $0x2000000020000, %r8
pdep %r8, %r10, %r10
xor %r10, 8(%rdi)
mov $0xc00000000000, %rdx
pext %rdx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 16(%rdi)
mov $0x6000000, %rcx
pext %rcx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 16(%rdi)
mov $0x30, %rax
pext %rax, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3000000000000, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 24(%rdi)
mov $0x18000000, %rbx
pext %rbx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
mov $0xc0, %r12
pext %r12, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 24(%rdi)
mov $0xc000000000000, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov $0x60000000, %r9
pext %r9, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
mov $0x300, %rdx
pext %rdx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 32(%rdi)
mov $0x30000000000000, %rcx
pext %rcx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 40(%rdi)
mov $0x180000000, %rax
pext %rax, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov $0xc00, %rbp
pext %rbp, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 40(%rdi)
mov $0xc0000000000000, %rbx
pext %rbx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov $0x600000000, %r12
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
mov $0x3000, %r14
pext %r14, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 48(%rdi)
mov $0x300000000000000, %r9
pext %r9, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov $0x1800000000, %rdx
pext %rdx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov $0xc000, %rcx
pext %rcx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 56(%rdi)
mov $0xc00000000000000, %rax
pext %rax, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
mov $0x6000000000, %rbp
pext %rbp, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 64(%rdi)
mov $0x30000, %rbx
pext %rbx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 64(%rdi)
mov $0x3000000000000000, %r12
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 72(%rdi)
mov $0x18000000000, %r14
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov $0xc0000, %r9
pext %r9, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 72(%rdi)
mov $0x4000020000000000, %rdx
pext %rdx, %r11, %r10
mov $0x8000000000000010, %rcx
pdep %rcx, %r10, %r10
rol $8, %r10
xor %r10, 80(%rdi)
mov %r11, %r10
and $0x100000, %r10
rol $61, %r10
xor %r10, 80(%rdi)
mov 16(%rsi), %r11
mov $0x8000080000200000, %rax
pext %rax, %r11, %r10
mov $0x400000080000010, %rbp
pdep %rbp, %r10, %r10
rol $25, %r10
xor %r10, 0(%rdi)
mov $0x40000400001, %rbx
pext %rbx, %r11, %r10
mov $0x8000001000000200, %r12
pdep %r12, %r10, %r10
rol $25, %r10
xor %r10, 0(%rdi)
mov $0x300000000002, %r13
pext %r13, %r11, %r10
mov $0x100000001000004, %r14
pdep %r14, %r10, %r10
xor %r10, 8(%rdi)
mov $0x1800000, %r15
pext %r15, %r11, %r10
mov $0x2000000020000000, %r9
pdep %r9, %r10, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0x4, %r10
rol $32, %r10
xor %r10, 8(%rdi)
mov $0xc00000000008, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 16(%rdi)
mov $0x6000000, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x10, %r10
rol $30, %r10
xor %r10, 16(%rdi)
mov $0x3000000000020, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 24(%rdi)
mov $0x18000000, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 24(%rdi)
mov %r11, %r10
and $0x40, %r10
rol $28, %r10
xor %r10, 24(%rdi)
mov $0xc000000000080, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0x60000000, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 32(%rdi)
mov %r11, %r10
and $0x100, %r10
rol $26, %r10
xor %r10, 32(%rdi)
mov $0x30000000000200, %r12
pext %r12, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 40(%rdi)
mov $0x180000000, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
and $0x400, %r10
rol $24, %r10
xor %r10, 40(%rdi)
mov $0xc0000000000800, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
mov $0x600000000, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 48(%rdi)
mov %r11, %r10
and $0x1000, %r10
rol $22, %r10
xor %r10, 48(%rdi)
mov $0x300000000002000, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 56(%rdi)
mov $0x1800000000, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 56(%rdi)
mov %r11, %r10
and $0x4000, %r10
rol $20, %r10
xor %r10, 56(%rdi)
mov $0xc00000000008000, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 64(%rdi)
mov $0x6000000000, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 64(%rdi)
mov %r11, %r10
and $0x10000, %r10
rol $18, %r10
xor %r10, 64(%rdi)
mov $0x3000000000020000, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 72(%rdi)
mov $0x18000000000, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 72(%rdi)
mov %r11, %r10
and $0x40000, %r10
rol $16, %r10
xor %r10, 72(%rdi)
mov $0x4000000000100000, %r13
pext %r13, %r11, %r10
mov $0x400000000000010, %r15
pdep %r15, %r10, %r10
rol $30, %r10
xor %r10, 80(%rdi)
mov $0x20000080000, %r8
pext %r8, %r11, %r10
mov $0x8000001000000000, %rdx
pdep %rdx, %r10, %r10
rol $30, %r10
xor %r10, 80(%rdi)
mov 24(%rsi), %r11
mov $0xc000040000000000, %rcx
pext %rcx, %r11, %r10
mov $0x8000000080000010, %rax
pdep %rax, %r10, %r10
rol $37, %r10
xor %r10, 0(%rdi)
mov $0x20000200000, %rbp
pext %rbp, %r11, %r10
mov $0x1000000200, %rbx
pdep %rbx, %r10, %r10
rol $37, %r10
xor %r10, 0(%rdi)
mov $0x100001, %r14
pext %r14, %r11, %r10
mov $0x20000004000, %r12
pdep %r12, %r10, %r10
rol $37, %r10
xor %r10, 0(%rdi)
mov $0x180000000000, %r9
pext %r9, %r11, %r10
mov $0x20000000200, %r13
pdep %r13, %r10, %r10
xor %r10, 8(%rdi)
mov $0xc00000, %r15
pext %r15, %r11, %r10
mov $0x400000004000, %r8
pdep %r8, %r10, %r10
xor %r10, 8(%rdi)
mov $0x6, %rdx
pext %rdx, %r11, %r10
mov $0x8000000080000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x600000000000, %rax
pext %rax, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3000000, %rbp
pext %rbp, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 16(%rdi)
mov $0x18, %rbx
pext %rbx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x1800000000000, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 24(%rdi)
mov $0xc000000, %r12
pext %r12, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 24(%rdi)
mov $0x60, %r9
pext %r9, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x6000000000000, %r15
pext %r15, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov $0x30000000, %rdx
pext %rdx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 32(%rdi)
mov $0x180, %rax
pext %rax, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x18000000000000, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 40(%rdi)
mov $0xc0000000, %rbx
pext %rbx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 40(%rdi)
mov $0x600, %r14
pext %r14, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x60000000000000, %r12
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov $0x300000000, %r9
pext %r9, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1800, %r15
pext %r15, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x180000000000000, %rdx
pext %rdx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov $0xc00000000, %rax
pext %rax, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 56(%rdi)
mov $0x6000, %rbp
pext %rbp, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x600000000000000, %rbx
pext %rbx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
mov $0x3000000000, %r14
pext %r14, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 64(%rdi)
mov $0x18000, %r12
pext %r12, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x1800000000000000, %r9
pext %r9, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 72(%rdi)
mov $0xc000000000, %r15
pext %r15, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 72(%rdi)
mov $0x60000, %rdx
pext %rdx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x2000010000000000, %rax
pext %rax, %r11, %r10
mov $0x8000000000000010, %rbp
pdep %rbp, %r10, %r10
rol $10, %r10
xor %r10, 80(%rdi)
mov %r11, %r10
and $0x80000, %r10
rol $0, %r10
xor %r10, 80(%rdi)
mov 32(%rsi), %r11
mov $0xc000000000000000, %rbx
pext %rbx, %r11, %r10
mov $0x20000000200000, %r14
pdep %r14, %r10, %r10
xor %r10, 0(%rdi)
mov $0x60000000000, %r12
pext %r12, %r11, %r10
mov $0x400000004000000, %r9
pdep %r9, %r10, %r10
xor %r10, 0(%rdi)
mov $0x300000, %r13
pext %r13, %r11, %r10
mov $0x8000000080000000, %r15
pdep %r15, %r10, %r10
xor %r10, 0(%rdi)
mov $0x180000000001, %r8
pext %r8, %r11, %r10
mov $0x400000004000010, %rdx
pdep %rdx, %r10, %r10
xor %r10, 8(%rdi)
mov $0xc00000, %rcx
pext %rcx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0x2, %r10
rol $35, %r10
xor %r10, 8(%rdi)
mov $0x600000000004, %rax
pext %rax, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x3000000, %rbp
pext %rbp, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x8, %r10
rol $33, %r10
xor %r10, 16(%rdi)
mov $0x1800000000010, %rbx
pext %rbx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
mov $0xc000000, %r14
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
mov %r11, %r10
and $0x20, %r10
rol $31, %r10
xor %r10, 24(%rdi)
mov $0x6000000000040, %r12
pext %r12, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x30000000, %r9
pext %r9, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
mov %r11, %r10
and $0x80, %r10
rol $29, %r10
xor %r10, 32(%rdi)
mov $0x18000000000100, %r13
pext %r13, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 40(%rdi)
mov $0xc0000000, %r8
pext %r8, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
and $0x200, %r10
rol $27, %r10
xor %r10, 40(%rdi)
mov $0x60000000000400, %rcx
pext %rcx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x300000000, %rax
pext %rax, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
mov %r11, %r10
and $0x800, %r10
rol $25, %r10
xor %r10, 48(%rdi)
mov $0x180000000001000, %rbp
pext %rbp, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
mov $0xc00000000, %rbx
pext %rbx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov %r11, %r10
and $0x2000, %r10
rol $23, %r10
xor %r10, 56(%rdi)
mov $0x600000000004000, %r14
pext %r14, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x3000000000, %r12
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 64(%rdi)
mov %r11, %r10
and $0x8000, %r10
rol $21, %r10
xor %r10, 64(%rdi)
mov $0x1800000000010000, %r9
pext %r9, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 72(%rdi)
mov $0xc000000000, %r13
pext %r13, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov %r11, %r10
and $0x20000, %r10
rol $19, %r10
xor %r10, 72(%rdi)
mov $0x2000000000080000, %r8
pext %r8, %r11, %r10
mov $0x400000000000010, %rcx
pdep %rcx, %r10, %r10
rol $32, %r10
xor %r10, 80(%rdi)
mov $0x10000040000, %rax
pext %rax, %r11, %r10
mov $0x8000001000000000, %rbp
pdep %rbp, %r10, %r10
rol $32, %r10
xor %r10, 80(%rdi)
mov 40(%rsi), %r11
mov $0x6000000000000000, %rbx
pext %rbx, %r11, %r10
mov $0x4000000040, %r14
pdep %r14, %r10, %r10
xor %r10, 0(%rdi)
mov $0x30000000000, %r12
pext %r12, %r11, %r10
mov $0x80000000800, %r9
pdep %r9, %r10, %r10
xor %r10, 0(%rdi)
mov $0x180000, %rdx
pext %rdx, %r11, %r10
mov $0x1000000010000, %r13
pdep %r13, %r10, %r10
xor %r10, 0(%rdi)
mov $0x80000c0000000000, %r15
pext %r15, %r11, %r10
mov $0x8000001000000010, %r8
pdep %r8, %r10, %r10
rol $7, %r10
xor %r10, 8(%rdi)
mov $0x600000, %rcx
pext %rcx, %r11, %r10
mov $0x20000000200, %rax
pdep %rax, %r10, %r10
rol $7, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0x3, %r10
mov $0x400000004000, %rbp
pdep %rbp, %r10, %r10
rol $7, %r10
xor %r10, 8(%rdi)
mov $0x300000000000, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 16(%rdi)
mov $0x1800000, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 16(%rdi)
mov $0xc, %r12
pext %r12, %r11, %r10
mov $0x20000000200000, %rdx
pdep %rdx, %r10, %r10
xor %r10, 16(%rdi)
mov $0xc00000000000, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 24(%rdi)
mov $0x6000000, %r8
pext %r8, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 24(%rdi)
mov $0x30, %rcx
pext %rcx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x3000000000000, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 32(%rdi)
mov $0x18000000, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov $0xc0, %rbx
pext %rbx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 32(%rdi)
mov $0xc000000000000, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 40(%rdi)
mov $0x60000000, %r12
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 40(%rdi)
mov $0x300, %r15
pext %r15, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x30000000000000, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 48(%rdi)
mov $0x180000000, %rcx
pext %rcx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov $0xc00, %rax
pext %rax, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 48(%rdi)
mov $0xc0000000000000, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 56(%rdi)
mov $0x600000000, %rbx
pext %rbx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov $0x3000, %r14
pext %r14, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x300000000000000, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 64(%rdi)
mov $0x1800000000, %r15
pext %r15, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
mov $0xc000, %r8
pext %r8, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 64(%rdi)
mov $0xc00000000000000, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 72(%rdi)
mov $0x6000000000, %rax
pext %rax, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 72(%rdi)
mov $0x30000, %rbp
pext %rbp, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1000008000000000, %rbx
pext %rbx, %r11, %r10
mov $0x8000000000000010, %r14
pdep %r14, %r10, %r10
rol $12, %r10
xor %r10, 80(%rdi)
mov %r11, %r10
and $0x40000, %r10
rol $3, %r10
xor %r10, 80(%rdi)
mov 48(%rsi), %r11
mov $0x6000000000040000, %r12
pext %r12, %r11, %r10
mov $0x80000000800002, %r15
pdep %r15, %r10, %r10
xor %r10, 0(%rdi)
mov $0x30000000000, %r8
pext %r8, %r11, %r10
mov $0x1000000010000000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
and $0x80000, %r10
rol $14, %r10
xor %r10, 0(%rdi)
mov $0x8000080000200000, %r9
pext %r9, %r11, %r10
mov $0x400000080000010, %rax
pdep %rax, %r10, %r10
rol $29, %r10
xor %r10, 8(%rdi)
mov $0x40000100001, %r13
pext %r13, %r11, %r10
mov $0x8000001000000200, %rbp
pdep %rbp, %r10, %r10
rol $29, %r10
xor %r10, 8(%rdi)
mov $0x300000400000, %rdx
pext %rdx, %r11, %r10
mov $0x1000000010000002, %rbx
pdep %rbx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x800002, %r14
pext %r14, %r11, %r10
mov $0x200000040, %r12
pdep %r12, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x4, %r10
rol $36, %r10
xor %r10, 16(%rdi)
mov $0xc00001000000, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x2000008, %r8
pext %r8, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 24(%rdi)
mov %r11, %r10
and $0x10, %r10
rol $34, %r10
xor %r10, 24(%rdi)
mov $0x3000004000000, %rcx
pext %rcx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x8000020, %r9
pext %r9, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 32(%rdi)
mov %r11, %r10
and $0x40, %r10
rol $32, %r10
xor %r10, 32(%rdi)
mov $0xc000010000000, %rax
pext %rax, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x20000080, %r13
pext %r13, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
and $0x100, %r10
rol $30, %r10
xor %r10, 40(%rdi)
mov $0x30000040000000, %rbp
pext %rbp, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x80000200, %rdx
pext %rdx, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 48(%rdi)
mov %r11, %r10
and $0x400, %r10
rol $28, %r10
xor %r10, 48(%rdi)
mov $0xc0000100000000, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x200000800, %r15
pext %r15, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 56(%rdi)
mov %r11, %r10
and $0x1000, %r10
rol $26, %r10
xor %r10, 56(%rdi)
mov $0x300000400000000, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x800002000, %rcx
pext %rcx, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 64(%rdi)
mov %r11, %r10
and $0x4000, %r10
rol $24, %r10
xor %r10, 64(%rdi)
mov $0xc00001000000000, %r9
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x2000008000, %rax
pext %rax, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 72(%rdi)
mov %r11, %r10
and $0x10000, %r10
rol $22, %r10
xor %r10, 72(%rdi)
mov $0x1000004000000000, %r13
pext %r13, %r11, %r10
mov $0x10000002, %rbp
pdep %rbp, %r10, %r10
xor %r10, 80(%rdi)
mov $0x8000020000, %rdx
pext %rdx, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 80(%rdi)
mov 56(%rsi), %r11
mov $0x3000000000000000, %r14
pext %r14, %r11, %r10
mov $0x10000000100, %r15
pdep %r15, %r10, %r10
xor %r10, 0(%rdi)
mov $0x18000000000, %r8
pext %r8, %r11, %r10
mov $0x200000002000, %rcx
pdep %rcx, %r10, %r10
xor %r10, 0(%rdi)
mov $0xc0000, %r9
pext %r9, %r11, %r10
mov $0x4000000040000, %rbx
pdep %rbx, %r10, %r10
xor %r10, 0(%rdi)
mov $0xc000040000000000, %rax
pext %rax, %r11, %r10
mov $0x8000000080000010, %r13
pdep %r13, %r10, %r10
rol $41, %r10
xor %r10, 8(%rdi)
mov $0x20000200000, %rbp
pext %rbp, %r11, %r10
mov $0x1000000200, %rdx
pdep %rdx, %r10, %r10
rol $41, %r10
xor %r10, 8(%rdi)
mov $0x100001, %r12
pext %r12, %r11, %r10
mov $0x20000004000, %r14
pdep %r14, %r10, %r10
rol $41, %r10
xor %r10, 8(%rdi)
mov $0x180000000000, %r15
pext %r15, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
mov $0xc00000, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x6, %r9
pext %r9, %r11, %r10
mov $0x80000000800000, %rax
pdep %rax, %r10, %r10
xor %r10, 16(%rdi)
mov $0x600000000000, %r13
pext %r13, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x3000000, %rbp
pext %rbp, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x18, %rdx
pext %rdx, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 24(%rdi)
mov $0x1800000000000, %r12
pext %r12, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 32(%rdi)
mov $0xc000000, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x60, %r15
pext %r15, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 32(%rdi)
mov $0x6000000000000, %r8
pext %r8, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x30000000, %r9
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x180, %r13
pext %r13, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 40(%rdi)
mov $0x18000000000000, %rbp
pext %rbp, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 48(%rdi)
mov $0xc0000000, %rdx
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x600, %r12
pext %r12, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 48(%rdi)
mov $0x60000000000000, %r14
pext %r14, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x300000000, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x1800, %r8
pext %r8, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 56(%rdi)
mov $0x180000000000000, %r9
pext %r9, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 64(%rdi)
mov $0xc00000000, %r13
pext %r13, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x6000, %rbp
pext %rbp, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 64(%rdi)
mov $0x600000000000000, %rdx
pext %rdx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x3000000000, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x18000, %r14
pext %r14, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 72(%rdi)
mov $0x800004000000000, %r15
pext %r15, %r11, %r10
mov $0x8000000000000010, %r8
pdep %r8, %r10, %r10
rol $14, %r10
xor %r10, 80(%rdi)
mov %r11, %r10
and $0x20000, %r10
rol $6, %r10
xor %r10, 80(%rdi)
mov 64(%rsi), %r11
mov $0x3000000000020000, %r9
pext %r9, %r11, %r10
mov $0x200000002000008, %r13
pdep %r13, %r10, %r10
xor %r10, 0(%rdi)
mov $0x18000000000, %rbp
pext %rbp, %r11, %r10
mov $0x4000000040000000, %rdx
pdep %rdx, %r10, %r10
xor %r10, 0(%rdi)
mov %r11, %r10
and $0x40000, %r10
rol $17, %r10
xor %r10, 0(%rdi)
mov $0xc000000000080000, %rcx
pext %rcx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 8(%rdi)
mov $0x60000000000, %r12
pext %r12, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 8(%rdi)
mov %r11, %r10
and $0x100000, %r10
rol $15, %r10
xor %r10, 8(%rdi)
mov $0x180000200000, %rbx
pext %rbx, %r11, %r10
mov $0x4000000040000008, %r14
pdep %r14, %r10, %r10
xor %r10, 16(%rdi)
mov $0x400001, %rax
pext %rax, %r11, %r10
mov $0x800000100, %r15
pdep %r15, %r10, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x2, %r10
rol $39, %r10
xor %r10, 16(%rdi)
mov $0x600000800000, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 24(%rdi)
mov $0x1000004, %r9
pext %r9, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
mov %r11, %r10
and $0x8, %r10
rol $37, %r10
xor %r10, 24(%rdi)
mov $0x1800002000000, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0x4000010, %rcx
pext %rcx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
mov %r11, %r10
and $0x20, %r10
rol $35, %r10
xor %r10, 32(%rdi)
mov $0x6000008000000, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 40(%rdi)
mov $0x10000040, %r12
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov %r11, %r10
and $0x80, %r10
rol $33, %r10
xor %r10, 40(%rdi)
mov $0x18000020000000, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
mov $0x40000100, %rbx
pext %rbx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
mov %r11, %r10
and $0x200, %r10
rol $31, %r10
xor %r10, 48(%rdi)
mov $0x60000080000000, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 56(%rdi)
mov $0x100000400, %r8
pext %r8, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov %r11, %r10
and $0x800, %r10
rol $29, %r10
xor %r10, 56(%rdi)
mov $0x180000200000000, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 64(%rdi)
mov $0x400001000, %rbp
pext %rbp, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 64(%rdi)
mov %r11, %r10
and $0x2000, %r10
rol $27, %r10
xor %r10, 64(%rdi)
mov $0x600000800000000, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1000004000, %r13
pext %r13, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov %r11, %r10
and $0x8000, %r10
rol $25, %r10
xor %r10, 72(%rdi)
mov $0x800002000000000, %r12
pext %r12, %r11, %r10
mov $0x40000008, %rdx
pdep %rdx, %r10, %r10
xor %r10, 80(%rdi)
mov $0x4000010000, %rbx
pext %rbx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 80(%rdi)
mov 72(%rsi), %r11
mov $0x1800000000000000, %rax
pext %rax, %r11, %r10
mov $0x40000000400, %r8
pdep %r8, %r10, %r10
xor %r10, 0(%rdi)
mov $0xc000000000, %r9
pext %r9, %r11, %r10
mov $0x800000008000, %rbp
pdep %rbp, %r10, %r10
xor %r10, 0(%rdi)
mov $0x60000, %rcx
pext %rcx, %r11, %r10
mov $0x10000000100000, %r14
pdep %r14, %r10, %r10
xor %r10, 0(%rdi)
mov $0x6000000000000000, %r13
pext %r13, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 8(%rdi)
mov $0x30000000000, %r12
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 8(%rdi)
mov $0x180000, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 8(%rdi)
mov $0x80000c0000000000, %rbx
pext %rbx, %r11, %r10
mov $0x8000001000000010, %r15
pdep %r15, %r10, %r10
rol $11, %r10
xor %r10, 16(%rdi)
mov $0x600000, %rax
pext %rax, %r11, %r10
mov $0x20000000200, %r9
pdep %r9, %r10, %r10
rol $11, %r10
xor %r10, 16(%rdi)
mov %r11, %r10
and $0x3, %r10
mov $0x400000004000, %rcx
pdep %rcx, %r10, %r10
rol $11, %r10
xor %r10, 16(%rdi)
mov $0x300000000000, %r13
pext %r13, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 24(%rdi)
mov $0x1800000, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 24(%rdi)
mov $0xc, %r12
pext %r12, %r11, %r10
mov $0x200000002000000, %rdx
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
mov $0xc00000000000, %rbx
pext %rbx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 32(%rdi)
mov $0x6000000, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0x30, %rax
pext %rax, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x3000000000000, %r9
pext %r9, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 40(%rdi)
mov $0x18000000, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 40(%rdi)
mov $0xc0, %r13
pext %r13, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 40(%rdi)
mov $0xc000000000000, %r8
pext %r8, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 48(%rdi)
mov $0x60000000, %r12
pext %r12, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
mov $0x300, %rbx
pext %rbx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x30000000000000, %r15
pext %r15, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 56(%rdi)
mov $0x180000000, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 56(%rdi)
mov $0xc00, %r9
pext %r9, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
mov $0xc0000000000000, %rcx
pext %rcx, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 64(%rdi)
mov $0x600000000, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 64(%rdi)
mov $0x3000, %r8
pext %r8, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x300000000000000, %r12
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1800000000, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 72(%rdi)
mov $0xc000, %r15
pext %r15, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x400002000000000, %rax
pext %rax, %r11, %r10
mov $0x8000000000000010, %r9
pdep %r9, %r10, %r10
rol $16, %r10
xor %r10, 80(%rdi)
mov %r11, %r10
and $0x10000, %r10
rol $9, %r10
xor %r10, 80(%rdi)
mov 80(%rsi), %r11
mov $0x30000, %rcx
pext %rcx, %r11, %r10
mov $0x2000000020, %r13
pdep %r13, %r10, %r10
xor %r10, 0(%rdi)
mov $0xc0000, %r8
pext %r8, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 8(%rdi)
mov $0x300001, %r12
pext %r12, %r11, %r10
mov $0x8000000080000010, %rbp
pdep %rbp, %r10, %r10
rol $38, %r10
xor %r10, 16(%rdi)
mov $0xc00000, %rbx
pext %rbx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 24(%rdi)
mov $0x6, %r14
pext %r14, %r11, %r10
mov $0x40000000400, %r15
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
mov $0x3000000, %rdx
pext %rdx, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov $0x18, %rax
pext %rax, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
mov $0xc000000, %r9
pext %r9, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 40(%rdi)
mov $0x60, %rcx
pext %rcx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov $0x30000000, %r8
pext %r8, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov $0x180, %r12
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
mov $0xc0000000, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov $0x600, %rbx
pext %rbx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov $0x300000000, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 64(%rdi)
mov $0x1800, %rdx
pext %rdx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 64(%rdi)
mov $0xc00000000, %rax
pext %rax, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 72(%rdi)
mov $0x6000, %r9
pext %r9, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov $0x1000008000, %rcx
pext %rcx, %r11, %r10
mov $0x8000000000000010, %r8
pdep %r8, %r10, %r10
rol $6, %r10
xor %r10, 80(%rdi)
movq $0x0, 88(%rdi)
pop %rbp
pop %rbx
pop %r12
pop %r13
pop %r14
pop %r15
ret
