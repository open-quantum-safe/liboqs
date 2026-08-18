.data
.p2align 5
.text
.global PQCLEAN_NTRUHRSS701_AVX2_square_6_701
.global _PQCLEAN_NTRUHRSS701_AVX2_square_6_701
PQCLEAN_NTRUHRSS701_AVX2_square_6_701:
_PQCLEAN_NTRUHRSS701_AVX2_square_6_701:
push %r15
push %r14
push %r13
push %r12
push %rbx
push %rbp
mov 0(%rsi), %r11
mov $0x80100200400801, %rbp
pext %rbp, %r11, %r10
mov $0x9249, %rbx
pdep %rbx, %r10, %r10
mov %r10, 0(%rdi)
mov $0x100200400801002, %r12
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 8(%rdi)
mov $0x200400801002004, %r13
pext %r13, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 16(%rdi)
mov $0x400801002004008, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 24(%rdi)
mov $0x801002004008010, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 32(%rdi)
mov $0x1002004008010020, %r9
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 40(%rdi)
mov $0x2004008010020040, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 48(%rdi)
mov $0x4008010020040080, %rdx
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 56(%rdi)
mov $0x8010020040080100, %rcx
pext %rcx, %r11, %r10
pdep %rbx, %r10, %r10
mov %r10, 64(%rdi)
mov $0x20040080100200, %rax
pext %rax, %r11, %r10
mov $0x1249, %rbp
pdep %rbp, %r10, %r10
mov %r10, 72(%rdi)
mov $0x40080100200400, %r12
pext %r12, %r11, %r10
pdep %rbp, %r10, %r10
mov %r10, 80(%rdi)
mov 8(%rsi), %r11
pext %r13, %r11, %r10
mov $0x249240000, %r14
pdep %r14, %r10, %r10
xor %r10, 0(%rdi)
mov $0x400801002004008, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 8(%rdi)
mov $0x801002004008010, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 16(%rdi)
mov $0x1002004008010020, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 24(%rdi)
mov $0x2004008010020040, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0x4008010020040080, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 40(%rdi)
mov $0x8010020040080100, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
pext %rax, %r11, %r10
mov $0x49240000, %r12
pdep %r12, %r10, %r10
xor %r10, 56(%rdi)
mov $0x40080100200400, %rbp
pext %rbp, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 64(%rdi)
mov $0x80100200400801, %r13
pext %r13, %r11, %r10
mov $0x49248000, %r15
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov $0x100200400801002, %r9
pext %r9, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 80(%rdi)
mov 16(%rsi), %r11
mov $0x801002004008010, %r8
pext %r8, %r11, %r10
mov $0x9249000000000, %rdx
pdep %rdx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x1002004008010020, %rcx
pext %rcx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x2004008010020040, %rbx
pext %rbx, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x4008010020040080, %r14
pext %r14, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x8010020040080100, %rax
pext %rax, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 32(%rdi)
mov $0x20040080100200, %rbp
pext %rbp, %r11, %r10
mov $0x1249000000000, %r12
pdep %r12, %r10, %r10
xor %r10, 40(%rdi)
mov $0x40080100200400, %r13
pext %r13, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 48(%rdi)
mov $0x80100200400801, %r9
pext %r9, %r11, %r10
mov $0x1249200000000, %r15
pdep %r15, %r10, %r10
xor %r10, 56(%rdi)
mov $0x100200400801002, %r8
pext %r8, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 64(%rdi)
mov $0x200400801002004, %rcx
pext %rcx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov $0x400801002004008, %rbx
pext %rbx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 80(%rdi)
mov 24(%rsi), %r11
mov $0x1002008010020040, %r14
pext %r14, %r11, %r10
mov $0x9249000000000000, %rax
pdep %rax, %r10, %r10
rol $6, %r10
xor %r10, 0(%rdi)
mov $0x2004010020040080, %rdx
pext %rdx, %r11, %r10
pdep %rax, %r10, %r10
rol $6, %r10
xor %r10, 8(%rdi)
mov $0x4008020040080100, %rbp
pext %rbp, %r11, %r10
pdep %rax, %r10, %r10
rol $6, %r10
xor %r10, 16(%rdi)
mov $0x8010040080100200, %r13
pext %r13, %r11, %r10
pdep %rax, %r10, %r10
rol $6, %r10
xor %r10, 24(%rdi)
mov $0x20080100200400, %r12
pext %r12, %r11, %r10
mov $0x9248000000000000, %r9
pdep %r9, %r10, %r10
rol $3, %r10
xor %r10, 32(%rdi)
mov $0x40100200400801, %r8
pext %r8, %r11, %r10
pdep %rax, %r10, %r10
rol $3, %r10
xor %r10, 40(%rdi)
mov $0x80200400801002, %rcx
pext %rcx, %r11, %r10
pdep %rax, %r10, %r10
rol $3, %r10
xor %r10, 48(%rdi)
mov $0x100400801002004, %rbx
pext %rbx, %r11, %r10
pdep %rax, %r10, %r10
rol $3, %r10
xor %r10, 56(%rdi)
mov $0x200801002004008, %r15
pext %r15, %r11, %r10
pdep %rax, %r10, %r10
rol $3, %r10
xor %r10, 64(%rdi)
mov $0x401002004008010, %r14
pext %r14, %r11, %r10
pdep %rax, %r10, %r10
rol $3, %r10
xor %r10, 72(%rdi)
mov $0x800004008010020, %rdx
pext %rdx, %r11, %r10
mov $0x8249000000000000, %rbp
pdep %rbp, %r10, %r10
rol $3, %r10
xor %r10, 80(%rdi)
mov 32(%rsi), %r11
mov $0x4008010020040080, %r13
pext %r13, %r11, %r10
mov $0x924900, %r12
pdep %r12, %r10, %r10
xor %r10, 0(%rdi)
mov $0x8010020040080100, %r9
pext %r9, %r11, %r10
pdep %r12, %r10, %r10
xor %r10, 8(%rdi)
mov $0x20040080100200, %r8
pext %r8, %r11, %r10
mov $0x124900, %rcx
pdep %rcx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x40080100200400, %rbx
pext %rbx, %r11, %r10
pdep %rcx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x80100200400801, %r15
pext %r15, %r11, %r10
mov $0x124920, %r14
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0x100200400801002, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 40(%rdi)
mov $0x200400801002004, %rdx
pext %rdx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
mov $0x400801002004008, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 56(%rdi)
mov $0x801002004008010, %r13
pext %r13, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 64(%rdi)
mov $0x1002004008010020, %r9
pext %r9, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 72(%rdi)
mov $0x2004008010020040, %r12
pext %r12, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 80(%rdi)
mov 40(%rsi), %r11
pext %r8, %r11, %r10
mov $0x4924000000, %rbx
pdep %rbx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x40080100200400, %rcx
pext %rcx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 8(%rdi)
pext %r15, %r11, %r10
mov $0x4924800000, %rax
pdep %rax, %r10, %r10
xor %r10, 16(%rdi)
mov $0x100200400801002, %rdx
pext %rdx, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 24(%rdi)
mov $0x200400801002004, %rbp
pext %rbp, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 32(%rdi)
mov $0x400801002004008, %r13
pext %r13, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 40(%rdi)
mov $0x801002004008010, %r9
pext %r9, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 48(%rdi)
mov $0x1002004008010020, %r12
pext %r12, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 56(%rdi)
mov $0x2004008010020040, %r14
pext %r14, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 64(%rdi)
mov $0x4008010020040080, %r8
pext %r8, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 72(%rdi)
mov $0x8010020040080100, %rcx
pext %rcx, %r11, %r10
pdep %rax, %r10, %r10
xor %r10, 80(%rdi)
mov 48(%rsi), %r11
pext %r15, %r11, %r10
mov $0x124920000000000, %rbx
pdep %rbx, %r10, %r10
xor %r10, 0(%rdi)
pext %rdx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 8(%rdi)
pext %rbp, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 16(%rdi)
pext %r13, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 24(%rdi)
pext %r9, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 32(%rdi)
pext %r12, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 40(%rdi)
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 48(%rdi)
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 56(%rdi)
pext %rcx, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x20040080100200, %rax
pext %rax, %r11, %r10
mov $0x24920000000000, %r15
pdep %r15, %r10, %r10
xor %r10, 72(%rdi)
mov $0x40080100200400, %rdx
pext %rdx, %r11, %r10
pdep %r15, %r10, %r10
xor %r10, 80(%rdi)
mov 56(%rsi), %r11
mov $0x100200400802004, %rbp
pext %rbp, %r11, %r10
mov $0x9249000000000000, %r13
pdep %r13, %r10, %r10
rol $11, %r10
xor %r10, 0(%rdi)
mov $0x200400801004008, %r9
pext %r9, %r11, %r10
pdep %r13, %r10, %r10
rol $11, %r10
xor %r10, 8(%rdi)
mov $0x400801002008010, %r12
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
rol $11, %r10
xor %r10, 16(%rdi)
mov $0x801002004010020, %r14
pext %r14, %r11, %r10
pdep %r13, %r10, %r10
rol $11, %r10
xor %r10, 24(%rdi)
mov $0x1002004008020040, %r8
pext %r8, %r11, %r10
pdep %r13, %r10, %r10
rol $11, %r10
xor %r10, 32(%rdi)
mov $0x2004008010040080, %rcx
pext %rcx, %r11, %r10
pdep %r13, %r10, %r10
rol $11, %r10
xor %r10, 40(%rdi)
mov $0x4008010020080100, %rbx
pext %rbx, %r11, %r10
pdep %r13, %r10, %r10
rol $11, %r10
xor %r10, 48(%rdi)
mov $0x8010020040100200, %rax
pext %rax, %r11, %r10
pdep %r13, %r10, %r10
rol $11, %r10
xor %r10, 56(%rdi)
mov $0x20040080200400, %rdx
pext %rdx, %r11, %r10
mov $0x9248000000000000, %r15
pdep %r15, %r10, %r10
rol $8, %r10
xor %r10, 64(%rdi)
mov $0x40080100400801, %rbp
pext %rbp, %r11, %r10
pdep %r13, %r10, %r10
rol $8, %r10
xor %r10, 72(%rdi)
mov $0x80100200001002, %r9
pext %r9, %r11, %r10
mov $0x9209000000000000, %r12
pdep %r12, %r10, %r10
rol $8, %r10
xor %r10, 80(%rdi)
mov 64(%rsi), %r11
mov $0x400801002004008, %r14
pext %r14, %r11, %r10
mov $0x12492000, %r8
pdep %r8, %r10, %r10
xor %r10, 0(%rdi)
mov $0x801002004008010, %rcx
pext %rcx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 8(%rdi)
mov $0x1002004008010020, %rbx
pext %rbx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 16(%rdi)
mov $0x2004008010020040, %rax
pext %rax, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 24(%rdi)
mov $0x4008010020040080, %rdx
pext %rdx, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 32(%rdi)
mov $0x8010020040080100, %r15
pext %r15, %r11, %r10
pdep %r8, %r10, %r10
xor %r10, 40(%rdi)
mov $0x20040080100200, %rbp
pext %rbp, %r11, %r10
mov $0x2492000, %r13
pdep %r13, %r10, %r10
xor %r10, 48(%rdi)
mov $0x40080100200400, %r9
pext %r9, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 56(%rdi)
mov $0x80100200400801, %r12
pext %r12, %r11, %r10
mov $0x2492400, %r14
pdep %r14, %r10, %r10
xor %r10, 64(%rdi)
mov $0x100200400801002, %rcx
pext %rcx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 72(%rdi)
mov $0x200400801002004, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 80(%rdi)
mov 72(%rsi), %r11
mov $0x1002004008010020, %rax
pext %rax, %r11, %r10
mov $0x492480000000, %rdx
pdep %rdx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x2004008010020040, %r15
pext %r15, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x4008010020040080, %r8
pext %r8, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x8010020040080100, %rbp
pext %rbp, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
mov $0x20040080100200, %r9
pext %r9, %r11, %r10
mov $0x92480000000, %r13
pdep %r13, %r10, %r10
xor %r10, 32(%rdi)
mov $0x40080100200400, %r12
pext %r12, %r11, %r10
pdep %r13, %r10, %r10
xor %r10, 40(%rdi)
mov $0x80100200400801, %rcx
pext %rcx, %r11, %r10
mov $0x92490000000, %rbx
pdep %rbx, %r10, %r10
xor %r10, 48(%rdi)
mov $0x100200400801002, %r14
pext %r14, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 56(%rdi)
mov $0x200400801002004, %rax
pext %rax, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 64(%rdi)
mov $0x400801002004008, %r15
pext %r15, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 72(%rdi)
mov $0x801002004008010, %r8
pext %r8, %r11, %r10
pdep %rbx, %r10, %r10
xor %r10, 80(%rdi)
mov 80(%rsi), %r11
mov $0x8010020040080, %rbp
pext %rbp, %r11, %r10
mov $0x2492000000000000, %rdx
pdep %rdx, %r10, %r10
xor %r10, 0(%rdi)
mov $0x10020040080100, %r9
pext %r9, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 8(%rdi)
mov $0x20040080100200, %r12
pext %r12, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 16(%rdi)
mov $0x40080100200400, %r13
pext %r13, %r11, %r10
pdep %rdx, %r10, %r10
xor %r10, 24(%rdi)
pext %rcx, %r11, %r10
mov $0x2492400000000000, %r14
pdep %r14, %r10, %r10
xor %r10, 32(%rdi)
mov $0x100200400801002, %rax
pext %rax, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 40(%rdi)
mov $0x200400801002004, %r15
pext %r15, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 48(%rdi)
mov $0x400801002004008, %r8
pext %r8, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 56(%rdi)
mov $0x801002004008010, %rbx
pext %rbx, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 64(%rdi)
mov $0x1002004008010020, %rbp
pext %rbp, %r11, %r10
pdep %r14, %r10, %r10
xor %r10, 72(%rdi)
mov $0x4008010020040, %r9
pext %r9, %r11, %r10
mov $0x492400000000000, %r12
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
