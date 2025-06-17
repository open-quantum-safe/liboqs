.data
.p2align 5
.text
.global PQCLEAN_NTRUHPS4096821_AVX2_square_6_821
.global _PQCLEAN_NTRUHPS4096821_AVX2_square_6_821
PQCLEAN_NTRUHPS4096821_AVX2_square_6_821:
_PQCLEAN_NTRUHPS4096821_AVX2_square_6_821:
push %r15
push %r14
push %r13
push %r12
push %rbx
push %rbp
mov 0(%rsi), %r11
mov $0x10008004002001, %rbp
pext %rbp, %r11, %r10
mov $0x100200400801, %rbx
pdep %rbx, %r10, %r10
mov %r10, 0(%rdi)
mov $0x20010008004002, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 8(%rdi)
mov $0x40020010008004, %r13
pext %r13, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 16(%rdi)
mov $0x80040020010008, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 24(%rdi)
mov $0x100080040020010, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 32(%rdi)
mov $0x200100080040020, %r9
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 40(%rdi)
mov $0x400200100080040, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 48(%rdi)
mov $0x800400200100080, %rdx
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 56(%rdi)
mov $0x1000800400200100, %rcx
pext %rcx, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 64(%rdi)
mov $0x2001000800400200, %rax
pext %rax, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 72(%rdi)
mov $0x4002001000800400, %rbp
pext %rbp, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 80(%rdi)
mov $0x8004002001000800, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 88(%rdi)
mov $0x8004002001000, %r13
pext %r13, %r11, %r10
mov $0x200400801, %r14
pdep %r14, %r10, %r10
mov %r10, 96(%rdi)
mov 8(%rsi), %r11
mov $0x10008004002002, %r15
pext %r15, %r11, %r10
mov $0x8010020040080000, %r9
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 0(%rdi)
mov $0x20010008004004, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 8(%rdi)
mov $0x40020010008008, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 16(%rdi)
mov $0x80040020010010, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 24(%rdi)
mov $0x100080040020020, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 32(%rdi)
mov $0x200100080040040, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 40(%rdi)
mov $0x400200100080080, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 48(%rdi)
mov $0x800400200100100, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 56(%rdi)
mov $0x1000800400200200, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 64(%rdi)
mov $0x2001000800400400, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 72(%rdi)
mov $0x4002001000800800, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 80(%rdi)
mov $0x8004002001001000, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
rol $36, %r10
xor %r10, 88(%rdi)
mov $0x8004002000001, %rdx
pext %rdx, %r11, %r10
mov $0x8010020000080000, %rcx
pdep %rcx, %r10, %r10
rol $25, %r10
xor %r10, 96(%rdi)
mov 16(%rsi), %r11
mov $0x10008004004002, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 0(%rdi)
mov $0x20010008008004, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 8(%rdi)
mov $0x40020010010008, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 16(%rdi)
mov $0x80040020020010, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 24(%rdi)
mov $0x100080040040020, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 32(%rdi)
mov $0x200100080080040, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 40(%rdi)
mov $0x400200100100080, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 48(%rdi)
mov $0x800400200200100, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 56(%rdi)
mov $0x1000800400400200, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 64(%rdi)
mov $0x2001000800800400, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 72(%rdi)
mov $0x4002001001000800, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 80(%rdi)
mov $0x8004002002001000, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
rol $27, %r10
xor %r10, 88(%rdi)
mov $0x8004000002001, %r12
pext %r12, %r11, %r10
mov $0x8010000040080000, %rbx
pdep %rbx, %r10, %r10
rol $16, %r10
xor %r10, 96(%rdi)
mov 24(%rsi), %r11
mov $0x10008008004002, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 0(%rdi)
mov $0x20010010008004, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 8(%rdi)
mov $0x40020020010008, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 16(%rdi)
mov $0x80040040020010, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 24(%rdi)
mov $0x100080080040020, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 32(%rdi)
mov $0x200100100080040, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 40(%rdi)
mov $0x400200200100080, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 48(%rdi)
mov $0x800400400200100, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 56(%rdi)
mov $0x1000800800400200, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 64(%rdi)
mov $0x2001001000800400, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 72(%rdi)
mov $0x4002002001000800, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 80(%rdi)
mov $0x8004004002001000, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
rol $18, %r10
xor %r10, 88(%rdi)
mov $0x8000004002001, %r15
pext %r15, %r11, %r10
mov $0x8000020040080000, %r8
pdep %r8, %r10, %r10
rol $7, %r10
xor %r10, 96(%rdi)
mov 32(%rsi), %r11
mov $0x10010008004002, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 0(%rdi)
mov $0x20020010008004, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 8(%rdi)
mov $0x40040020010008, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 16(%rdi)
mov $0x80080040020010, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 24(%rdi)
mov $0x100100080040020, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 32(%rdi)
mov $0x200200100080040, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 40(%rdi)
mov $0x400400200100080, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 48(%rdi)
mov $0x800800400200100, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 56(%rdi)
mov $0x1001000800400200, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 64(%rdi)
mov $0x2002001000800400, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 72(%rdi)
mov $0x4004002001000800, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 80(%rdi)
mov $0x8008004002001000, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
rol $9, %r10
xor %r10, 88(%rdi)
mov $0x8004002001, %rax
pext %rax, %r11, %r10
mov $0x4008010020000, %rbp
pdep %rbp, %r10, %r10
xor %r10, 96(%rdi)
mov 40(%rsi), %r11
mov $0x20010008004002, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 0(%rdi)
mov $0x40020010008004, %rbx
pext %rbx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 8(%rdi)
mov $0x80040020010008, %r13
pext %r13, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 16(%rdi)
mov $0x100080040020010, %r14
pext %r14, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 24(%rdi)
mov $0x200100080040020, %r15
pext %r15, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 32(%rdi)
mov $0x400200100080040, %r8
pext %r8, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 40(%rdi)
mov $0x800400200100080, %rdx
pext %rdx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1000800400200100, %rcx
pext %rcx, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 56(%rdi)
mov $0x2001000800400200, %rax
pext %rax, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 64(%rdi)
mov $0x4002001000800400, %rbp
pext %rbp, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 72(%rdi)
mov $0x8004002001000800, %r12
pext %r12, %r11, %r10
pdep %r9, %r10, %r10
xor %r10, 80(%rdi)
mov $0x8004002001000, %rbx
pext %rbx, %r11, %r10
mov $0x10020040080000, %r13
pdep %r13, %r10, %r10
xor %r10, 88(%rdi)
mov $0x10008004002001, %r14
pext %r14, %r11, %r10
mov $0x10020040080100, %r15
pdep %r15, %r10, %r10
xor %r10, 96(%rdi)
mov 48(%rsi), %r11
mov $0x20010008004002, %r8
pext %r8, %r11, %r10
mov $0x40080100200400, %rdx
pdep %rdx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x40020010008004, %rcx
pext %rcx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x80040020010008, %rax
pext %rax, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x100080040020010, %rbp
pext %rbp, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x200100080040020, %r12
pext %r12, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x400200100080040, %r9
pext %r9, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x800400200100080, %rbx
pext %rbx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1000800400200100, %r13
pext %r13, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x2001000800400200, %r14
pext %r14, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x4002001000800400, %r15
pext %r15, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x8004002001000800, %r8
pext %r8, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 80(%rdi)
mov $0x8004002001001, %rcx
pext %rcx, %r11, %r10
mov $0x8010020040080000, %rax
pdep %rax, %r10, %r10
rol $44, %r10
xor %r10, 88(%rdi)
mov $0x10008004002000, %rbp
pext %rbp, %r11, %r10
mov $0x80100200400, %r12
pdep %r12, %r10, %r10
xor %r10, 96(%rdi)
mov 56(%rsi), %r11
mov $0x20010008004002, %r9
pext %r9, %r11, %r10
mov $0x200400801002, %rbx
pdep %rbx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x40020010008004, %r13
pext %r13, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x80040020010008, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x100080040020010, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x200100080040020, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x400200100080040, %rdx
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 40(%rdi)
mov $0x800400200100080, %rcx
pext %rcx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1000800400200100, %rax
pext %rax, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x2001000800400200, %rbp
pext %rbp, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x4002001000800400, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x8004002001000800, %r9
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 80(%rdi)
mov $0x8004002001001, %r13
pext %r13, %r11, %r10
mov $0x8010020040080000, %r14
pdep %r14, %r10, %r10
rol $35, %r10
xor %r10, 88(%rdi)
mov $0x10008004002000, %r15
pext %r15, %r11, %r10
mov $0x400801002, %r8
pdep %r8, %r10, %r10
xor %r10, 96(%rdi)
mov 64(%rsi), %r11
mov $0x20010008004004, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 0(%rdi)
mov $0x40020010008008, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 8(%rdi)
mov $0x80040020010010, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 16(%rdi)
mov $0x100080040020020, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 24(%rdi)
mov $0x200100080040040, %r12
pext %r12, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 32(%rdi)
mov $0x400200100080080, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 40(%rdi)
mov $0x800400200100100, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 48(%rdi)
mov $0x1000800400200200, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 56(%rdi)
mov $0x2001000800400400, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 64(%rdi)
mov $0x4002001000800800, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 72(%rdi)
mov $0x8004002001001000, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
rol $37, %r10
xor %r10, 80(%rdi)
mov $0x8004002002001, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
rol $26, %r10
xor %r10, 88(%rdi)
mov $0x10008004000002, %rax
pext %rax, %r11, %r10
mov $0x8010020000080000, %rbp
pdep %rbp, %r10, %r10
rol $26, %r10
xor %r10, 96(%rdi)
mov 72(%rsi), %r11
mov $0x20010008008004, %r12
pext %r12, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 0(%rdi)
mov $0x40020010010008, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 8(%rdi)
mov $0x80040020020010, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 16(%rdi)
mov $0x100080040040020, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 24(%rdi)
mov $0x200100080080040, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 32(%rdi)
mov $0x400200100100080, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 40(%rdi)
mov $0x800400200200100, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 48(%rdi)
mov $0x1000800400400200, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 56(%rdi)
mov $0x2001000800800400, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 64(%rdi)
mov $0x4002001001000800, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 72(%rdi)
mov $0x8004002002001000, %r12
pext %r12, %r11, %r10
pdep %r14, %r10, %r10
rol $28, %r10
xor %r10, 80(%rdi)
mov $0x8004004002001, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
rol $17, %r10
xor %r10, 88(%rdi)
mov $0x10008000004002, %rbx
pext %rbx, %r11, %r10
mov $0x8010000040080000, %r13
pdep %r13, %r10, %r10
rol $17, %r10
xor %r10, 96(%rdi)
mov 80(%rsi), %r11
mov $0x20010010008004, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 0(%rdi)
mov $0x40020020010008, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 8(%rdi)
mov $0x80040040020010, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 16(%rdi)
mov $0x100080080040020, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 24(%rdi)
mov $0x200100100080040, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 32(%rdi)
mov $0x400200200100080, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 40(%rdi)
mov $0x800400400200100, %r12
pext %r12, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 48(%rdi)
mov $0x1000800800400200, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 56(%rdi)
mov $0x2001001000800400, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 64(%rdi)
mov $0x4002002001000800, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 72(%rdi)
mov $0x8004004002001000, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
rol $19, %r10
xor %r10, 80(%rdi)
mov $0x8008004002001, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
rol $8, %r10
xor %r10, 88(%rdi)
mov $0x10000008004002, %rdx
pext %rdx, %r11, %r10
mov $0x8000020040080000, %rcx
pdep %rcx, %r10, %r10
rol $8, %r10
xor %r10, 96(%rdi)
mov 88(%rsi), %r11
mov $0x20020010008004, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 0(%rdi)
mov $0x40040020010008, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 8(%rdi)
mov $0x80080040020010, %r12
pext %r12, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 16(%rdi)
mov $0x100100080040020, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 24(%rdi)
mov $0x200200100080040, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 32(%rdi)
mov $0x400400200100080, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 40(%rdi)
mov $0x800800400200100, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 48(%rdi)
mov $0x1001000800400200, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 56(%rdi)
mov $0x2002001000800400, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 64(%rdi)
mov $0x4004002001000800, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 72(%rdi)
mov $0x8008004002001000, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
rol $10, %r10
xor %r10, 80(%rdi)
mov $0x10008004002001, %rbp
pext %rbp, %r11, %r10
mov $0x4008010020040000, %r12
pdep %r12, %r10, %r10
xor %r10, 88(%rdi)
mov $0x10008004002, %r9
pext %r9, %r11, %r10
mov $0x8010020040000, %rbx
pdep %rbx, %r10, %r10
xor %r10, 96(%rdi)
mov 96(%rsi), %r11
mov $0x20010008004, %r13
pext %r13, %r11, %r10
mov $0x20040080100000, %r15
pdep %r15, %r10, %r10
xor %r10, 0(%rdi)
mov $0x40020010008, %r8
pext %r8, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 8(%rdi)
mov $0x80040020010, %rdx
pext %rdx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 16(%rdi)
mov $0x100080040020, %rcx
pext %rcx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 24(%rdi)
mov $0x200100080040, %rax
pext %rax, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 32(%rdi)
mov $0x400200100080, %r14
pext %r14, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 40(%rdi)
mov $0x800400200100, %rbp
pext %rbp, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1000800400200, %r12
pext %r12, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov $0x2001000800400, %r9
pext %r9, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 64(%rdi)
mov $0x4002001000800, %rbx
pext %rbx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov $0x8004002001000, %r13
pext %r13, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 80(%rdi)
mov $0x10008004002001, %r8
pext %r8, %r11, %r10
mov $0x20040080100200, %rdx
pdep %rdx, %r10, %r10
xor %r10, 88(%rdi)
mov $0x10008004002, %rcx
pext %rcx, %r11, %r10
mov $0x40080100200, %rax
pdep %rax, %r10, %r10
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
