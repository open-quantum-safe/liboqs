	.att_syntax
	.text
	.p2align	5
	.globl	_jade_kem_kyber_kyber512_amd64_ref_dec
	.globl	jade_kem_kyber_kyber512_amd64_ref_dec
	.globl	_jade_kem_kyber_kyber512_amd64_ref_enc
	.globl	jade_kem_kyber_kyber512_amd64_ref_enc
	.globl	_jade_kem_kyber_kyber512_amd64_ref_enc_derand
	.globl	jade_kem_kyber_kyber512_amd64_ref_enc_derand
	.globl	_jade_kem_kyber_kyber512_amd64_ref_keypair
	.globl	jade_kem_kyber_kyber512_amd64_ref_keypair
	.globl	_jade_kem_kyber_kyber512_amd64_ref_keypair_derand
	.globl	jade_kem_kyber_kyber512_amd64_ref_keypair_derand
_jade_kem_kyber_kyber512_amd64_ref_dec:
jade_kem_kyber_kyber512_amd64_ref_dec:
	movq	%rsp, %rax
	leaq	-9632(%rsp), %rsp
	andq	$-8, %rsp
	movq	%rax, 9624(%rsp)
	movq	%r14, 9584(%rsp)
	movq	%r13, 9592(%rsp)
	movq	%r12, 9600(%rsp)
	movq	%rbp, 9608(%rsp)
	movq	%rbx, 9616(%rsp)
	movq	%rdi, (%rsp)
	movq	%rsi, 8(%rsp)
	leaq	88(%rsp), %rax
	movq	$0, %rcx
	movq	$0, %rdi
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$124
Ljade_kem_kyber_kyber512_amd64_ref_dec$125:
	movzbl	(%rsi,%rdi), %r10d
	incq	%rdi
	movzbl	(%rsi,%rdi), %r11d
	incq	%rdi
	movzbl	(%rsi,%rdi), %r8d
	incq	%rdi
	movzbl	(%rsi,%rdi), %r9d
	incq	%rdi
	movzbl	(%rsi,%rdi), %ebx
	incq	%rdi
	movl	%r11d, %ebp
	shrl	$2, %r11d
	andl	$3, %ebp
	shll	$8, %ebp
	orl 	%ebp, %r10d
	movl	%r8d, %ebp
	shrl	$4, %r8d
	andl	$15, %ebp
	shll	$6, %ebp
	orl 	%ebp, %r11d
	movl	%r9d, %ebp
	shrl	$6, %r9d
	andl	$63, %ebp
	shll	$4, %ebp
	orl 	%ebp, %r8d
	shll	$2, %ebx
	orl 	%ebx, %r9d
	imull	$3329, %r10d, %r10d
	addl	$512, %r10d
	shrl	$10, %r10d
	movw	%r10w, 2464(%rsp,%rcx,2)
	incq	%rcx
	imull	$3329, %r11d, %r10d
	addl	$512, %r10d
	shrl	$10, %r10d
	movw	%r10w, 2464(%rsp,%rcx,2)
	incq	%rcx
	imull	$3329, %r8d, %r8d
	addl	$512, %r8d
	shrl	$10, %r8d
	movw	%r8w, 2464(%rsp,%rcx,2)
	incq	%rcx
	imull	$3329, %r9d, %r8d
	addl	$512, %r8d
	shrl	$10, %r8d
	movw	%r8w, 2464(%rsp,%rcx,2)
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$124:
	cmpq	$512, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$125
	addq	$640, %rsi
	leaq	928(%rsp), %rcx
	call	L_poly_decompress$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$123:
	movq	%rdx, %rsi
	leaq	3488(%rsp), %rdi
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$122:
	addq	$384, %rsi
	leaq	4000(%rsp), %rdi
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$121:
	leaq	2464(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$120:
	leaq	2976(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$119:
	leaq	416(%rsp), %rdi
	leaq	3488(%rsp), %rcx
	leaq	2464(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$118:
	leaq	16(%rsp), %rsp
	leaq	1440(%rsp), %rdi
	leaq	4000(%rsp), %rcx
	leaq	2976(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$117:
	leaq	16(%rsp), %rsp
	leaq	416(%rsp), %rcx
	leaq	1440(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$116:
	leaq	416(%rsp), %rcx
	movq	$0, %rsi
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$114
Ljade_kem_kyber_kyber512_amd64_ref_dec$115:
	movw	(%rcx,%rsi,2), %di
	movswl	%di, %r8d
	imull	$20159, %r8d, %r8d
	sarl	$26, %r8d
	imull	$3329, %r8d, %r8d
	subw	%r8w, %di
	movw	%di, (%rcx,%rsi,2)
	incq	%rsi
Ljade_kem_kyber_kyber512_amd64_ref_dec$114:
	cmpq	$256, %rsi
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$115
	leaq	416(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$113:
	leaq	1440(%rsp), %rcx
	leaq	928(%rsp), %rsi
	leaq	416(%rsp), %rdi
	call	L_poly_sub$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$112:
	leaq	1440(%rsp), %rcx
	movq	$0, %rsi
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$110
Ljade_kem_kyber_kyber512_amd64_ref_dec$111:
	movw	(%rcx,%rsi,2), %di
	movswl	%di, %r8d
	imull	$20159, %r8d, %r8d
	sarl	$26, %r8d
	imull	$3329, %r8d, %r8d
	subw	%r8w, %di
	movw	%di, (%rcx,%rsi,2)
	incq	%rsi
Ljade_kem_kyber_kyber512_amd64_ref_dec$110:
	cmpq	$256, %rsi
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$111
	leaq	1440(%rsp), %rcx
	call	L_i_poly_tomsg$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$109:
	leaq	32(%rdx), %rax
	addq	$1536, %rax
	movq	(%rax), %rcx
	movq	%rcx, 120(%rsp)
	movq	8(%rax), %rcx
	movq	%rcx, 128(%rsp)
	movq	16(%rax), %rcx
	movq	%rcx, 136(%rsp)
	movq	24(%rax), %rax
	movq	%rax, 144(%rsp)
	movq	%rdx, 16(%rsp)
	leaq	152(%rsp), %rax
	leaq	88(%rsp), %rcx
	leaq	-208(%rsp), %rsp
	call	L_sha3_512_64$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$108:
	leaq	208(%rsp), %rsp
	movq	16(%rsp), %rdx
	addq	$768, %rdx
	leaq	8810(%rsp), %rsi
	leaq	88(%rsp), %rcx
	leaq	184(%rsp), %rax
	movq	%rsi, 24(%rsp)
	movq	%rdx, %rsi
	leaq	2464(%rsp), %rdi
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$107:
	addq	$384, %rsi
	leaq	2976(%rsp), %rdi
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$106:
	movq	$0, %rsi
	addq	$768, %rdx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$104
Ljade_kem_kyber_kyber512_amd64_ref_dec$105:
	movq	(%rdx), %rdi
	movq	%rdi, 56(%rsp,%rsi,8)
	addq	$8, %rdx
	incq	%rsi
Ljade_kem_kyber_kyber512_amd64_ref_dec$104:
	cmpq	$4, %rsi
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$105
	leaq	1440(%rsp), %rdx
	call	L_i_poly_frommsg$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$103:
	movq	%rax, 32(%rsp)
	movq	$1, %rax
	movq	%rax, 40(%rsp)
	movb	56(%rsp), %al
	movb	%al, 8608(%rsp)
	movb	57(%rsp), %al
	movb	%al, 8609(%rsp)
	movb	58(%rsp), %al
	movb	%al, 8610(%rsp)
	movb	59(%rsp), %al
	movb	%al, 8611(%rsp)
	movb	60(%rsp), %al
	movb	%al, 8612(%rsp)
	movb	61(%rsp), %al
	movb	%al, 8613(%rsp)
	movb	62(%rsp), %al
	movb	%al, 8614(%rsp)
	movb	63(%rsp), %al
	movb	%al, 8615(%rsp)
	movb	64(%rsp), %al
	movb	%al, 8616(%rsp)
	movb	65(%rsp), %al
	movb	%al, 8617(%rsp)
	movb	66(%rsp), %al
	movb	%al, 8618(%rsp)
	movb	67(%rsp), %al
	movb	%al, 8619(%rsp)
	movb	68(%rsp), %al
	movb	%al, 8620(%rsp)
	movb	69(%rsp), %al
	movb	%al, 8621(%rsp)
	movb	70(%rsp), %al
	movb	%al, 8622(%rsp)
	movb	71(%rsp), %al
	movb	%al, 8623(%rsp)
	movb	72(%rsp), %al
	movb	%al, 8624(%rsp)
	movb	73(%rsp), %al
	movb	%al, 8625(%rsp)
	movb	74(%rsp), %al
	movb	%al, 8626(%rsp)
	movb	75(%rsp), %al
	movb	%al, 8627(%rsp)
	movb	76(%rsp), %al
	movb	%al, 8628(%rsp)
	movb	77(%rsp), %al
	movb	%al, 8629(%rsp)
	movb	78(%rsp), %al
	movb	%al, 8630(%rsp)
	movb	79(%rsp), %al
	movb	%al, 8631(%rsp)
	movb	80(%rsp), %al
	movb	%al, 8632(%rsp)
	movb	81(%rsp), %al
	movb	%al, 8633(%rsp)
	movb	82(%rsp), %al
	movb	%al, 8634(%rsp)
	movb	83(%rsp), %al
	movb	%al, 8635(%rsp)
	movb	84(%rsp), %al
	movb	%al, 8636(%rsp)
	movb	85(%rsp), %al
	movb	%al, 8637(%rsp)
	movb	86(%rsp), %al
	movb	%al, 8638(%rsp)
	movb	87(%rsp), %al
	movb	%al, 8639(%rsp)
	movq	40(%rsp), %rax
	cmpq	$0, %rax
	je  	Ljade_kem_kyber_kyber512_amd64_ref_dec$101
	movb	$0, 8640(%rsp)
	movb	$0, 8641(%rsp)
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$102
Ljade_kem_kyber_kyber512_amd64_ref_dec$101:
	movb	$0, 8640(%rsp)
	movb	$0, 8641(%rsp)
Ljade_kem_kyber_kyber512_amd64_ref_dec$102:
	leaq	216(%rsp), %rax
	leaq	8608(%rsp), %rcx
	call	L_shake128_absorb34$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$100:
	movq	$0, %rax
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$98
Ljade_kem_kyber_kyber512_amd64_ref_dec$99:
	movq	%rax, 416(%rsp,%rax,8)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$98:
	cmpq	$64, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$99
	movq	$0, %rax
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$90
Ljade_kem_kyber_kyber512_amd64_ref_dec$91:
	movq	%rax, 48(%rsp)
	leaq	216(%rsp), %rax
	leaq	8642(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$97:
	leaq	8(%rsp), %rsp
	movq	48(%rsp), %rax
	movq	$0, %rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$92:
	movzbw	8642(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	8642(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	8642(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$95
	cmpq	$256, %rax
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$95
	movw	%dx, 416(%rsp,%rax,2)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$96:
Ljade_kem_kyber_kyber512_amd64_ref_dec$95:
	cmpw	$3329, %di
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$93
	cmpq	$256, %rax
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$93
	movw	%di, 416(%rsp,%rax,2)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$94:
Ljade_kem_kyber_kyber512_amd64_ref_dec$93:
	cmpq	$166, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$92
Ljade_kem_kyber_kyber512_amd64_ref_dec$90:
	cmpq	$256, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$91
	movq	$0, %rax
	movq	$0, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$88
Ljade_kem_kyber_kyber512_amd64_ref_dec$89:
	movw	416(%rsp,%rax,2), %dx
	movw	%dx, 6560(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$88:
	cmpq	$256, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$89
	movq	40(%rsp), %rax
	cmpq	$0, %rax
	je  	Ljade_kem_kyber_kyber512_amd64_ref_dec$86
	movb	$0, 8640(%rsp)
	movb	$1, 8641(%rsp)
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$87
Ljade_kem_kyber_kyber512_amd64_ref_dec$86:
	movb	$1, 8640(%rsp)
	movb	$0, 8641(%rsp)
Ljade_kem_kyber_kyber512_amd64_ref_dec$87:
	leaq	216(%rsp), %rax
	leaq	8608(%rsp), %rcx
	call	L_shake128_absorb34$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$85:
	movq	$0, %rax
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$83
Ljade_kem_kyber_kyber512_amd64_ref_dec$84:
	movq	%rax, 416(%rsp,%rax,8)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$83:
	cmpq	$64, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$84
	movq	$0, %rax
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$75
Ljade_kem_kyber_kyber512_amd64_ref_dec$76:
	movq	%rax, 48(%rsp)
	leaq	216(%rsp), %rax
	leaq	8642(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$82:
	leaq	8(%rsp), %rsp
	movq	48(%rsp), %rax
	movq	$0, %rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$77:
	movzbw	8642(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	8642(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	8642(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$80
	cmpq	$256, %rax
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$80
	movw	%dx, 416(%rsp,%rax,2)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$81:
Ljade_kem_kyber_kyber512_amd64_ref_dec$80:
	cmpw	$3329, %di
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$78
	cmpq	$256, %rax
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$78
	movw	%di, 416(%rsp,%rax,2)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$79:
Ljade_kem_kyber_kyber512_amd64_ref_dec$78:
	cmpq	$166, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$77
Ljade_kem_kyber_kyber512_amd64_ref_dec$75:
	cmpq	$256, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$76
	movq	$0, %rax
	movq	$256, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$73
Ljade_kem_kyber_kyber512_amd64_ref_dec$74:
	movw	416(%rsp,%rax,2), %dx
	movw	%dx, 6560(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$73:
	cmpq	$256, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$74
	movq	40(%rsp), %rax
	cmpq	$0, %rax
	je  	Ljade_kem_kyber_kyber512_amd64_ref_dec$71
	movb	$1, 8640(%rsp)
	movb	$0, 8641(%rsp)
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$72
Ljade_kem_kyber_kyber512_amd64_ref_dec$71:
	movb	$0, 8640(%rsp)
	movb	$1, 8641(%rsp)
Ljade_kem_kyber_kyber512_amd64_ref_dec$72:
	leaq	216(%rsp), %rax
	leaq	8608(%rsp), %rcx
	call	L_shake128_absorb34$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$70:
	movq	$0, %rax
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$68
Ljade_kem_kyber_kyber512_amd64_ref_dec$69:
	movq	%rax, 416(%rsp,%rax,8)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$68:
	cmpq	$64, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$69
	movq	$0, %rax
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$60
Ljade_kem_kyber_kyber512_amd64_ref_dec$61:
	movq	%rax, 48(%rsp)
	leaq	216(%rsp), %rax
	leaq	8642(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$67:
	leaq	8(%rsp), %rsp
	movq	48(%rsp), %rax
	movq	$0, %rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$62:
	movzbw	8642(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	8642(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	8642(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$65
	cmpq	$256, %rax
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$65
	movw	%dx, 416(%rsp,%rax,2)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$66:
Ljade_kem_kyber_kyber512_amd64_ref_dec$65:
	cmpw	$3329, %di
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$63
	cmpq	$256, %rax
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$63
	movw	%di, 416(%rsp,%rax,2)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$64:
Ljade_kem_kyber_kyber512_amd64_ref_dec$63:
	cmpq	$166, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$62
Ljade_kem_kyber_kyber512_amd64_ref_dec$60:
	cmpq	$256, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$61
	movq	$0, %rax
	movq	$512, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$58
Ljade_kem_kyber_kyber512_amd64_ref_dec$59:
	movw	416(%rsp,%rax,2), %dx
	movw	%dx, 6560(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$58:
	cmpq	$256, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$59
	movq	40(%rsp), %rax
	cmpq	$0, %rax
	je  	Ljade_kem_kyber_kyber512_amd64_ref_dec$56
	movb	$1, 8640(%rsp)
	movb	$1, 8641(%rsp)
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$57
Ljade_kem_kyber_kyber512_amd64_ref_dec$56:
	movb	$1, 8640(%rsp)
	movb	$1, 8641(%rsp)
Ljade_kem_kyber_kyber512_amd64_ref_dec$57:
	leaq	216(%rsp), %rax
	leaq	8608(%rsp), %rcx
	call	L_shake128_absorb34$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$55:
	movq	$0, %rax
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$53
Ljade_kem_kyber_kyber512_amd64_ref_dec$54:
	movq	%rax, 416(%rsp,%rax,8)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$53:
	cmpq	$64, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$54
	movq	$0, %rax
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$45
Ljade_kem_kyber_kyber512_amd64_ref_dec$46:
	movq	%rax, 40(%rsp)
	leaq	216(%rsp), %rax
	leaq	8642(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$52:
	leaq	8(%rsp), %rsp
	movq	40(%rsp), %rax
	movq	$0, %rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$47:
	movzbw	8642(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	8642(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	8642(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$50
	cmpq	$256, %rax
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$50
	movw	%dx, 416(%rsp,%rax,2)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$51:
Ljade_kem_kyber_kyber512_amd64_ref_dec$50:
	cmpw	$3329, %di
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$48
	cmpq	$256, %rax
	jnb 	Ljade_kem_kyber_kyber512_amd64_ref_dec$48
	movw	%di, 416(%rsp,%rax,2)
	incq	%rax
Ljade_kem_kyber_kyber512_amd64_ref_dec$49:
Ljade_kem_kyber_kyber512_amd64_ref_dec$48:
	cmpq	$166, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$47
Ljade_kem_kyber_kyber512_amd64_ref_dec$45:
	cmpq	$256, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$46
	movq	$0, %rax
	movq	$768, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$43
Ljade_kem_kyber_kyber512_amd64_ref_dec$44:
	movw	416(%rsp,%rax,2), %dx
	movw	%dx, 6560(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$43:
	cmpq	$256, %rax
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$44
	movq	32(%rsp), %rcx
	movb	$0, %al
	leaq	3488(%rsp), %rdx
	leaq	-240(%rsp), %rsp
	call	L_poly_getnoise_eta1$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$42:
	leaq	240(%rsp), %rsp
	movq	32(%rsp), %rcx
	movb	$1, %al
	leaq	4000(%rsp), %rdx
	leaq	-240(%rsp), %rsp
	call	L_poly_getnoise_eta1$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$41:
	leaq	240(%rsp), %rsp
	movq	32(%rsp), %rax
	movb	$2, %cl
	leaq	4512(%rsp), %rdx
	leaq	-176(%rsp), %rsp
	call	L_poly_getnoise_eta2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$40:
	leaq	176(%rsp), %rsp
	movq	32(%rsp), %rax
	movb	$3, %cl
	leaq	5024(%rsp), %rdx
	leaq	-176(%rsp), %rsp
	call	L_poly_getnoise_eta2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$39:
	leaq	176(%rsp), %rsp
	movq	32(%rsp), %rax
	movb	$4, %cl
	leaq	416(%rsp), %rdx
	leaq	-176(%rsp), %rsp
	call	L_poly_getnoise_eta2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$38:
	leaq	176(%rsp), %rsp
	leaq	3488(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$37:
	leaq	4000(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$36:
	leaq	5536(%rsp), %rdi
	leaq	6560(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$35:
	leaq	16(%rsp), %rsp
	leaq	928(%rsp), %rdi
	leaq	7072(%rsp), %rcx
	leaq	4000(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$34:
	leaq	16(%rsp), %rsp
	leaq	5536(%rsp), %rcx
	leaq	928(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$33:
	leaq	5536(%rsp), %rax
	movq	$0, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$31
Ljade_kem_kyber_kyber512_amd64_ref_dec$32:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$31:
	cmpq	$256, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$32
	leaq	6048(%rsp), %rdi
	leaq	7584(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$30:
	leaq	16(%rsp), %rsp
	leaq	928(%rsp), %rdi
	leaq	8096(%rsp), %rcx
	leaq	4000(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$29:
	leaq	16(%rsp), %rsp
	leaq	6048(%rsp), %rcx
	leaq	928(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$28:
	leaq	6048(%rsp), %rax
	movq	$0, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$26
Ljade_kem_kyber_kyber512_amd64_ref_dec$27:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$26:
	cmpq	$256, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$27
	leaq	928(%rsp), %rdi
	leaq	2464(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$25:
	leaq	16(%rsp), %rsp
	leaq	1952(%rsp), %rdi
	leaq	2976(%rsp), %rcx
	leaq	4000(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$24:
	leaq	16(%rsp), %rsp
	leaq	928(%rsp), %rcx
	leaq	1952(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$23:
	leaq	928(%rsp), %rax
	movq	$0, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$21
Ljade_kem_kyber_kyber512_amd64_ref_dec$22:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$21:
	cmpq	$256, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$22
	leaq	5536(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$20:
	leaq	6048(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$19:
	leaq	928(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$18:
	leaq	5536(%rsp), %rcx
	leaq	4512(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$17:
	leaq	6048(%rsp), %rcx
	leaq	5024(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$16:
	leaq	928(%rsp), %rcx
	leaq	416(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$15:
	leaq	928(%rsp), %rcx
	leaq	1440(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$14:
	leaq	5536(%rsp), %rax
	movq	$0, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$12
Ljade_kem_kyber_kyber512_amd64_ref_dec$13:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$12:
	cmpq	$256, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$13
	leaq	6048(%rsp), %rax
	movq	$0, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$10
Ljade_kem_kyber_kyber512_amd64_ref_dec$11:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$10:
	cmpq	$256, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$11
	leaq	928(%rsp), %rax
	movq	$0, %rcx
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$8
Ljade_kem_kyber_kyber512_amd64_ref_dec$9:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
Ljade_kem_kyber_kyber512_amd64_ref_dec$8:
	cmpq	$256, %rcx
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$9
	movq	24(%rsp), %rax
	movq	%rax, %rdx
	movq	$0, %rsi
	movq	$0, %rdi
	leaq	5536(%rsp), %rcx
	call	L_poly_csubq$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$7:
	leaq	6048(%rsp), %rcx
	call	L_poly_csubq$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$6:
	jmp 	Ljade_kem_kyber_kyber512_amd64_ref_dec$4
Ljade_kem_kyber_kyber512_amd64_ref_dec$5:
	movzwq	5536(%rsp,%rsi,2), %rcx
	incq	%rsi
	shlq	$10, %rcx
	addq	$1665, %rcx
	imulq	$1290167, %rcx, %rcx
	shrq	$32, %rcx
	andq	$1023, %rcx
	movzwq	5536(%rsp,%rsi,2), %r8
	incq	%rsi
	shlq	$10, %r8
	addq	$1665, %r8
	imulq	$1290167, %r8, %r8
	shrq	$32, %r8
	andq	$1023, %r8
	movzwq	5536(%rsp,%rsi,2), %r9
	incq	%rsi
	shlq	$10, %r9
	addq	$1665, %r9
	imulq	$1290167, %r9, %r9
	shrq	$32, %r9
	andq	$1023, %r9
	movzwq	5536(%rsp,%rsi,2), %r10
	incq	%rsi
	shlq	$10, %r10
	addq	$1665, %r10
	imulq	$1290167, %r10, %r10
	shrq	$32, %r10
	andq	$1023, %r10
	movw	%cx, %r11w
	andw	$255, %r11w
	movb	%r11b, (%rdx,%rdi)
	incq	%rdi
	shrw	$8, %cx
	movw	%r8w, %r11w
	shlw	$2, %r11w
	orw 	%cx, %r11w
	movb	%r11b, (%rdx,%rdi)
	incq	%rdi
	shrw	$6, %r8w
	movw	%r9w, %cx
	shlw	$4, %cx
	orw 	%r8w, %cx
	movb	%cl, (%rdx,%rdi)
	incq	%rdi
	shrw	$4, %r9w
	movw	%r10w, %cx
	shlw	$6, %cx
	orw 	%r9w, %cx
	movb	%cl, (%rdx,%rdi)
	incq	%rdi
	shrq	$2, %r10
	movb	%r10b, (%rdx,%rdi)
	incq	%rdi
Ljade_kem_kyber_kyber512_amd64_ref_dec$4:
	cmpq	$512, %rsi
	jb  	Ljade_kem_kyber_kyber512_amd64_ref_dec$5
	leaq	640(%rax), %rax
	leaq	928(%rsp), %rcx
	call	L_i_poly_compress$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$3:
	movq	8(%rsp), %rsi
	leaq	8810(%rsp), %rax
	movq	$0, %rcx
	movb	(%rax), %dl
	movb	(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	1(%rax), %dl
	movb	1(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	2(%rax), %dl
	movb	2(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	3(%rax), %dl
	movb	3(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	4(%rax), %dl
	movb	4(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	5(%rax), %dl
	movb	5(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	6(%rax), %dl
	movb	6(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	7(%rax), %dl
	movb	7(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	8(%rax), %dl
	movb	8(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	9(%rax), %dl
	movb	9(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	10(%rax), %dl
	movb	10(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	11(%rax), %dl
	movb	11(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	12(%rax), %dl
	movb	12(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	13(%rax), %dl
	movb	13(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	14(%rax), %dl
	movb	14(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	15(%rax), %dl
	movb	15(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	16(%rax), %dl
	movb	16(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	17(%rax), %dl
	movb	17(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	18(%rax), %dl
	movb	18(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	19(%rax), %dl
	movb	19(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	20(%rax), %dl
	movb	20(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	21(%rax), %dl
	movb	21(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	22(%rax), %dl
	movb	22(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	23(%rax), %dl
	movb	23(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	24(%rax), %dl
	movb	24(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	25(%rax), %dl
	movb	25(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	26(%rax), %dl
	movb	26(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	27(%rax), %dl
	movb	27(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	28(%rax), %dl
	movb	28(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	29(%rax), %dl
	movb	29(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	30(%rax), %dl
	movb	30(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	31(%rax), %dl
	movb	31(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	32(%rax), %dl
	movb	32(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	33(%rax), %dl
	movb	33(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	34(%rax), %dl
	movb	34(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	35(%rax), %dl
	movb	35(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	36(%rax), %dl
	movb	36(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	37(%rax), %dl
	movb	37(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	38(%rax), %dl
	movb	38(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	39(%rax), %dl
	movb	39(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	40(%rax), %dl
	movb	40(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	41(%rax), %dl
	movb	41(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	42(%rax), %dl
	movb	42(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	43(%rax), %dl
	movb	43(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	44(%rax), %dl
	movb	44(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	45(%rax), %dl
	movb	45(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	46(%rax), %dl
	movb	46(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	47(%rax), %dl
	movb	47(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	48(%rax), %dl
	movb	48(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	49(%rax), %dl
	movb	49(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	50(%rax), %dl
	movb	50(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	51(%rax), %dl
	movb	51(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	52(%rax), %dl
	movb	52(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	53(%rax), %dl
	movb	53(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	54(%rax), %dl
	movb	54(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	55(%rax), %dl
	movb	55(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	56(%rax), %dl
	movb	56(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	57(%rax), %dl
	movb	57(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	58(%rax), %dl
	movb	58(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	59(%rax), %dl
	movb	59(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	60(%rax), %dl
	movb	60(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	61(%rax), %dl
	movb	61(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	62(%rax), %dl
	movb	62(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	63(%rax), %dl
	movb	63(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	64(%rax), %dl
	movb	64(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	65(%rax), %dl
	movb	65(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	66(%rax), %dl
	movb	66(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	67(%rax), %dl
	movb	67(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	68(%rax), %dl
	movb	68(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	69(%rax), %dl
	movb	69(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	70(%rax), %dl
	movb	70(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	71(%rax), %dl
	movb	71(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	72(%rax), %dl
	movb	72(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	73(%rax), %dl
	movb	73(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	74(%rax), %dl
	movb	74(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	75(%rax), %dl
	movb	75(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	76(%rax), %dl
	movb	76(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	77(%rax), %dl
	movb	77(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	78(%rax), %dl
	movb	78(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	79(%rax), %dl
	movb	79(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	80(%rax), %dl
	movb	80(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	81(%rax), %dl
	movb	81(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	82(%rax), %dl
	movb	82(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	83(%rax), %dl
	movb	83(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	84(%rax), %dl
	movb	84(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	85(%rax), %dl
	movb	85(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	86(%rax), %dl
	movb	86(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	87(%rax), %dl
	movb	87(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	88(%rax), %dl
	movb	88(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	89(%rax), %dl
	movb	89(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	90(%rax), %dl
	movb	90(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	91(%rax), %dl
	movb	91(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	92(%rax), %dl
	movb	92(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	93(%rax), %dl
	movb	93(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	94(%rax), %dl
	movb	94(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	95(%rax), %dl
	movb	95(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	96(%rax), %dl
	movb	96(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	97(%rax), %dl
	movb	97(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	98(%rax), %dl
	movb	98(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	99(%rax), %dl
	movb	99(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	100(%rax), %dl
	movb	100(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	101(%rax), %dl
	movb	101(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	102(%rax), %dl
	movb	102(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	103(%rax), %dl
	movb	103(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	104(%rax), %dl
	movb	104(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	105(%rax), %dl
	movb	105(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	106(%rax), %dl
	movb	106(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	107(%rax), %dl
	movb	107(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	108(%rax), %dl
	movb	108(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	109(%rax), %dl
	movb	109(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	110(%rax), %dl
	movb	110(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	111(%rax), %dl
	movb	111(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	112(%rax), %dl
	movb	112(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	113(%rax), %dl
	movb	113(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	114(%rax), %dl
	movb	114(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	115(%rax), %dl
	movb	115(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	116(%rax), %dl
	movb	116(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	117(%rax), %dl
	movb	117(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	118(%rax), %dl
	movb	118(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	119(%rax), %dl
	movb	119(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	120(%rax), %dl
	movb	120(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	121(%rax), %dl
	movb	121(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	122(%rax), %dl
	movb	122(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	123(%rax), %dl
	movb	123(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	124(%rax), %dl
	movb	124(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	125(%rax), %dl
	movb	125(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	126(%rax), %dl
	movb	126(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	127(%rax), %dl
	movb	127(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	128(%rax), %dl
	movb	128(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	129(%rax), %dl
	movb	129(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	130(%rax), %dl
	movb	130(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	131(%rax), %dl
	movb	131(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	132(%rax), %dl
	movb	132(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	133(%rax), %dl
	movb	133(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	134(%rax), %dl
	movb	134(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	135(%rax), %dl
	movb	135(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	136(%rax), %dl
	movb	136(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	137(%rax), %dl
	movb	137(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	138(%rax), %dl
	movb	138(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	139(%rax), %dl
	movb	139(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	140(%rax), %dl
	movb	140(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	141(%rax), %dl
	movb	141(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	142(%rax), %dl
	movb	142(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	143(%rax), %dl
	movb	143(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	144(%rax), %dl
	movb	144(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	145(%rax), %dl
	movb	145(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	146(%rax), %dl
	movb	146(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	147(%rax), %dl
	movb	147(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	148(%rax), %dl
	movb	148(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	149(%rax), %dl
	movb	149(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	150(%rax), %dl
	movb	150(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	151(%rax), %dl
	movb	151(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	152(%rax), %dl
	movb	152(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	153(%rax), %dl
	movb	153(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	154(%rax), %dl
	movb	154(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	155(%rax), %dl
	movb	155(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	156(%rax), %dl
	movb	156(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	157(%rax), %dl
	movb	157(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	158(%rax), %dl
	movb	158(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	159(%rax), %dl
	movb	159(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	160(%rax), %dl
	movb	160(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	161(%rax), %dl
	movb	161(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	162(%rax), %dl
	movb	162(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	163(%rax), %dl
	movb	163(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	164(%rax), %dl
	movb	164(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	165(%rax), %dl
	movb	165(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	166(%rax), %dl
	movb	166(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	167(%rax), %dl
	movb	167(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	168(%rax), %dl
	movb	168(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	169(%rax), %dl
	movb	169(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	170(%rax), %dl
	movb	170(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	171(%rax), %dl
	movb	171(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	172(%rax), %dl
	movb	172(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	173(%rax), %dl
	movb	173(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	174(%rax), %dl
	movb	174(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	175(%rax), %dl
	movb	175(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	176(%rax), %dl
	movb	176(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	177(%rax), %dl
	movb	177(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	178(%rax), %dl
	movb	178(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	179(%rax), %dl
	movb	179(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	180(%rax), %dl
	movb	180(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	181(%rax), %dl
	movb	181(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	182(%rax), %dl
	movb	182(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	183(%rax), %dl
	movb	183(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	184(%rax), %dl
	movb	184(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	185(%rax), %dl
	movb	185(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	186(%rax), %dl
	movb	186(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	187(%rax), %dl
	movb	187(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	188(%rax), %dl
	movb	188(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	189(%rax), %dl
	movb	189(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	190(%rax), %dl
	movb	190(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	191(%rax), %dl
	movb	191(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	192(%rax), %dl
	movb	192(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	193(%rax), %dl
	movb	193(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	194(%rax), %dl
	movb	194(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	195(%rax), %dl
	movb	195(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	196(%rax), %dl
	movb	196(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	197(%rax), %dl
	movb	197(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	198(%rax), %dl
	movb	198(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	199(%rax), %dl
	movb	199(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	200(%rax), %dl
	movb	200(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	201(%rax), %dl
	movb	201(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	202(%rax), %dl
	movb	202(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	203(%rax), %dl
	movb	203(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	204(%rax), %dl
	movb	204(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	205(%rax), %dl
	movb	205(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	206(%rax), %dl
	movb	206(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	207(%rax), %dl
	movb	207(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	208(%rax), %dl
	movb	208(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	209(%rax), %dl
	movb	209(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	210(%rax), %dl
	movb	210(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	211(%rax), %dl
	movb	211(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	212(%rax), %dl
	movb	212(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	213(%rax), %dl
	movb	213(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	214(%rax), %dl
	movb	214(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	215(%rax), %dl
	movb	215(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	216(%rax), %dl
	movb	216(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	217(%rax), %dl
	movb	217(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	218(%rax), %dl
	movb	218(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	219(%rax), %dl
	movb	219(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	220(%rax), %dl
	movb	220(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	221(%rax), %dl
	movb	221(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	222(%rax), %dl
	movb	222(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	223(%rax), %dl
	movb	223(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	224(%rax), %dl
	movb	224(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	225(%rax), %dl
	movb	225(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	226(%rax), %dl
	movb	226(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	227(%rax), %dl
	movb	227(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	228(%rax), %dl
	movb	228(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	229(%rax), %dl
	movb	229(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	230(%rax), %dl
	movb	230(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	231(%rax), %dl
	movb	231(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	232(%rax), %dl
	movb	232(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	233(%rax), %dl
	movb	233(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	234(%rax), %dl
	movb	234(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	235(%rax), %dl
	movb	235(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	236(%rax), %dl
	movb	236(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	237(%rax), %dl
	movb	237(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	238(%rax), %dl
	movb	238(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	239(%rax), %dl
	movb	239(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	240(%rax), %dl
	movb	240(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	241(%rax), %dl
	movb	241(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	242(%rax), %dl
	movb	242(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	243(%rax), %dl
	movb	243(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	244(%rax), %dl
	movb	244(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	245(%rax), %dl
	movb	245(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	246(%rax), %dl
	movb	246(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	247(%rax), %dl
	movb	247(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	248(%rax), %dl
	movb	248(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	249(%rax), %dl
	movb	249(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	250(%rax), %dl
	movb	250(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	251(%rax), %dl
	movb	251(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	252(%rax), %dl
	movb	252(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	253(%rax), %dl
	movb	253(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	254(%rax), %dl
	movb	254(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	255(%rax), %dl
	movb	255(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	256(%rax), %dl
	movb	256(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	257(%rax), %dl
	movb	257(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	258(%rax), %dl
	movb	258(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	259(%rax), %dl
	movb	259(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	260(%rax), %dl
	movb	260(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	261(%rax), %dl
	movb	261(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	262(%rax), %dl
	movb	262(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	263(%rax), %dl
	movb	263(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	264(%rax), %dl
	movb	264(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	265(%rax), %dl
	movb	265(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	266(%rax), %dl
	movb	266(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	267(%rax), %dl
	movb	267(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	268(%rax), %dl
	movb	268(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	269(%rax), %dl
	movb	269(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	270(%rax), %dl
	movb	270(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	271(%rax), %dl
	movb	271(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	272(%rax), %dl
	movb	272(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	273(%rax), %dl
	movb	273(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	274(%rax), %dl
	movb	274(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	275(%rax), %dl
	movb	275(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	276(%rax), %dl
	movb	276(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	277(%rax), %dl
	movb	277(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	278(%rax), %dl
	movb	278(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	279(%rax), %dl
	movb	279(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	280(%rax), %dl
	movb	280(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	281(%rax), %dl
	movb	281(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	282(%rax), %dl
	movb	282(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	283(%rax), %dl
	movb	283(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	284(%rax), %dl
	movb	284(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	285(%rax), %dl
	movb	285(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	286(%rax), %dl
	movb	286(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	287(%rax), %dl
	movb	287(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	288(%rax), %dl
	movb	288(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	289(%rax), %dl
	movb	289(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	290(%rax), %dl
	movb	290(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	291(%rax), %dl
	movb	291(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	292(%rax), %dl
	movb	292(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	293(%rax), %dl
	movb	293(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	294(%rax), %dl
	movb	294(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	295(%rax), %dl
	movb	295(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	296(%rax), %dl
	movb	296(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	297(%rax), %dl
	movb	297(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	298(%rax), %dl
	movb	298(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	299(%rax), %dl
	movb	299(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	300(%rax), %dl
	movb	300(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	301(%rax), %dl
	movb	301(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	302(%rax), %dl
	movb	302(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	303(%rax), %dl
	movb	303(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	304(%rax), %dl
	movb	304(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	305(%rax), %dl
	movb	305(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	306(%rax), %dl
	movb	306(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	307(%rax), %dl
	movb	307(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	308(%rax), %dl
	movb	308(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	309(%rax), %dl
	movb	309(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	310(%rax), %dl
	movb	310(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	311(%rax), %dl
	movb	311(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	312(%rax), %dl
	movb	312(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	313(%rax), %dl
	movb	313(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	314(%rax), %dl
	movb	314(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	315(%rax), %dl
	movb	315(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	316(%rax), %dl
	movb	316(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	317(%rax), %dl
	movb	317(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	318(%rax), %dl
	movb	318(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	319(%rax), %dl
	movb	319(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	320(%rax), %dl
	movb	320(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	321(%rax), %dl
	movb	321(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	322(%rax), %dl
	movb	322(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	323(%rax), %dl
	movb	323(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	324(%rax), %dl
	movb	324(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	325(%rax), %dl
	movb	325(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	326(%rax), %dl
	movb	326(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	327(%rax), %dl
	movb	327(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	328(%rax), %dl
	movb	328(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	329(%rax), %dl
	movb	329(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	330(%rax), %dl
	movb	330(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	331(%rax), %dl
	movb	331(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	332(%rax), %dl
	movb	332(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	333(%rax), %dl
	movb	333(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	334(%rax), %dl
	movb	334(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	335(%rax), %dl
	movb	335(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	336(%rax), %dl
	movb	336(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	337(%rax), %dl
	movb	337(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	338(%rax), %dl
	movb	338(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	339(%rax), %dl
	movb	339(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	340(%rax), %dl
	movb	340(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	341(%rax), %dl
	movb	341(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	342(%rax), %dl
	movb	342(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	343(%rax), %dl
	movb	343(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	344(%rax), %dl
	movb	344(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	345(%rax), %dl
	movb	345(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	346(%rax), %dl
	movb	346(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	347(%rax), %dl
	movb	347(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	348(%rax), %dl
	movb	348(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	349(%rax), %dl
	movb	349(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	350(%rax), %dl
	movb	350(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	351(%rax), %dl
	movb	351(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	352(%rax), %dl
	movb	352(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	353(%rax), %dl
	movb	353(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	354(%rax), %dl
	movb	354(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	355(%rax), %dl
	movb	355(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	356(%rax), %dl
	movb	356(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	357(%rax), %dl
	movb	357(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	358(%rax), %dl
	movb	358(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	359(%rax), %dl
	movb	359(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	360(%rax), %dl
	movb	360(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	361(%rax), %dl
	movb	361(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	362(%rax), %dl
	movb	362(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	363(%rax), %dl
	movb	363(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	364(%rax), %dl
	movb	364(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	365(%rax), %dl
	movb	365(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	366(%rax), %dl
	movb	366(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	367(%rax), %dl
	movb	367(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	368(%rax), %dl
	movb	368(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	369(%rax), %dl
	movb	369(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	370(%rax), %dl
	movb	370(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	371(%rax), %dl
	movb	371(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	372(%rax), %dl
	movb	372(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	373(%rax), %dl
	movb	373(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	374(%rax), %dl
	movb	374(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	375(%rax), %dl
	movb	375(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	376(%rax), %dl
	movb	376(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	377(%rax), %dl
	movb	377(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	378(%rax), %dl
	movb	378(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	379(%rax), %dl
	movb	379(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	380(%rax), %dl
	movb	380(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	381(%rax), %dl
	movb	381(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	382(%rax), %dl
	movb	382(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	383(%rax), %dl
	movb	383(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	384(%rax), %dl
	movb	384(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	385(%rax), %dl
	movb	385(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	386(%rax), %dl
	movb	386(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	387(%rax), %dl
	movb	387(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	388(%rax), %dl
	movb	388(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	389(%rax), %dl
	movb	389(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	390(%rax), %dl
	movb	390(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	391(%rax), %dl
	movb	391(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	392(%rax), %dl
	movb	392(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	393(%rax), %dl
	movb	393(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	394(%rax), %dl
	movb	394(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	395(%rax), %dl
	movb	395(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	396(%rax), %dl
	movb	396(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	397(%rax), %dl
	movb	397(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	398(%rax), %dl
	movb	398(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	399(%rax), %dl
	movb	399(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	400(%rax), %dl
	movb	400(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	401(%rax), %dl
	movb	401(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	402(%rax), %dl
	movb	402(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	403(%rax), %dl
	movb	403(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	404(%rax), %dl
	movb	404(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	405(%rax), %dl
	movb	405(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	406(%rax), %dl
	movb	406(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	407(%rax), %dl
	movb	407(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	408(%rax), %dl
	movb	408(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	409(%rax), %dl
	movb	409(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	410(%rax), %dl
	movb	410(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	411(%rax), %dl
	movb	411(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	412(%rax), %dl
	movb	412(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	413(%rax), %dl
	movb	413(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	414(%rax), %dl
	movb	414(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	415(%rax), %dl
	movb	415(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	416(%rax), %dl
	movb	416(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	417(%rax), %dl
	movb	417(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	418(%rax), %dl
	movb	418(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	419(%rax), %dl
	movb	419(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	420(%rax), %dl
	movb	420(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	421(%rax), %dl
	movb	421(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	422(%rax), %dl
	movb	422(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	423(%rax), %dl
	movb	423(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	424(%rax), %dl
	movb	424(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	425(%rax), %dl
	movb	425(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	426(%rax), %dl
	movb	426(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	427(%rax), %dl
	movb	427(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	428(%rax), %dl
	movb	428(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	429(%rax), %dl
	movb	429(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	430(%rax), %dl
	movb	430(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	431(%rax), %dl
	movb	431(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	432(%rax), %dl
	movb	432(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	433(%rax), %dl
	movb	433(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	434(%rax), %dl
	movb	434(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	435(%rax), %dl
	movb	435(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	436(%rax), %dl
	movb	436(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	437(%rax), %dl
	movb	437(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	438(%rax), %dl
	movb	438(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	439(%rax), %dl
	movb	439(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	440(%rax), %dl
	movb	440(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	441(%rax), %dl
	movb	441(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	442(%rax), %dl
	movb	442(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	443(%rax), %dl
	movb	443(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	444(%rax), %dl
	movb	444(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	445(%rax), %dl
	movb	445(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	446(%rax), %dl
	movb	446(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	447(%rax), %dl
	movb	447(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	448(%rax), %dl
	movb	448(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	449(%rax), %dl
	movb	449(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	450(%rax), %dl
	movb	450(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	451(%rax), %dl
	movb	451(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	452(%rax), %dl
	movb	452(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	453(%rax), %dl
	movb	453(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	454(%rax), %dl
	movb	454(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	455(%rax), %dl
	movb	455(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	456(%rax), %dl
	movb	456(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	457(%rax), %dl
	movb	457(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	458(%rax), %dl
	movb	458(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	459(%rax), %dl
	movb	459(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	460(%rax), %dl
	movb	460(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	461(%rax), %dl
	movb	461(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	462(%rax), %dl
	movb	462(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	463(%rax), %dl
	movb	463(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	464(%rax), %dl
	movb	464(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	465(%rax), %dl
	movb	465(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	466(%rax), %dl
	movb	466(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	467(%rax), %dl
	movb	467(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	468(%rax), %dl
	movb	468(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	469(%rax), %dl
	movb	469(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	470(%rax), %dl
	movb	470(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	471(%rax), %dl
	movb	471(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	472(%rax), %dl
	movb	472(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	473(%rax), %dl
	movb	473(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	474(%rax), %dl
	movb	474(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	475(%rax), %dl
	movb	475(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	476(%rax), %dl
	movb	476(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	477(%rax), %dl
	movb	477(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	478(%rax), %dl
	movb	478(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	479(%rax), %dl
	movb	479(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	480(%rax), %dl
	movb	480(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	481(%rax), %dl
	movb	481(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	482(%rax), %dl
	movb	482(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	483(%rax), %dl
	movb	483(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	484(%rax), %dl
	movb	484(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	485(%rax), %dl
	movb	485(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	486(%rax), %dl
	movb	486(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	487(%rax), %dl
	movb	487(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	488(%rax), %dl
	movb	488(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	489(%rax), %dl
	movb	489(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	490(%rax), %dl
	movb	490(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	491(%rax), %dl
	movb	491(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	492(%rax), %dl
	movb	492(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	493(%rax), %dl
	movb	493(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	494(%rax), %dl
	movb	494(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	495(%rax), %dl
	movb	495(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	496(%rax), %dl
	movb	496(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	497(%rax), %dl
	movb	497(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	498(%rax), %dl
	movb	498(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	499(%rax), %dl
	movb	499(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	500(%rax), %dl
	movb	500(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	501(%rax), %dl
	movb	501(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	502(%rax), %dl
	movb	502(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	503(%rax), %dl
	movb	503(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	504(%rax), %dl
	movb	504(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	505(%rax), %dl
	movb	505(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	506(%rax), %dl
	movb	506(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	507(%rax), %dl
	movb	507(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	508(%rax), %dl
	movb	508(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	509(%rax), %dl
	movb	509(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	510(%rax), %dl
	movb	510(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	511(%rax), %dl
	movb	511(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	512(%rax), %dl
	movb	512(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	513(%rax), %dl
	movb	513(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	514(%rax), %dl
	movb	514(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	515(%rax), %dl
	movb	515(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	516(%rax), %dl
	movb	516(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	517(%rax), %dl
	movb	517(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	518(%rax), %dl
	movb	518(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	519(%rax), %dl
	movb	519(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	520(%rax), %dl
	movb	520(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	521(%rax), %dl
	movb	521(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	522(%rax), %dl
	movb	522(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	523(%rax), %dl
	movb	523(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	524(%rax), %dl
	movb	524(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	525(%rax), %dl
	movb	525(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	526(%rax), %dl
	movb	526(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	527(%rax), %dl
	movb	527(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	528(%rax), %dl
	movb	528(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	529(%rax), %dl
	movb	529(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	530(%rax), %dl
	movb	530(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	531(%rax), %dl
	movb	531(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	532(%rax), %dl
	movb	532(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	533(%rax), %dl
	movb	533(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	534(%rax), %dl
	movb	534(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	535(%rax), %dl
	movb	535(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	536(%rax), %dl
	movb	536(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	537(%rax), %dl
	movb	537(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	538(%rax), %dl
	movb	538(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	539(%rax), %dl
	movb	539(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	540(%rax), %dl
	movb	540(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	541(%rax), %dl
	movb	541(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	542(%rax), %dl
	movb	542(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	543(%rax), %dl
	movb	543(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	544(%rax), %dl
	movb	544(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	545(%rax), %dl
	movb	545(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	546(%rax), %dl
	movb	546(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	547(%rax), %dl
	movb	547(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	548(%rax), %dl
	movb	548(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	549(%rax), %dl
	movb	549(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	550(%rax), %dl
	movb	550(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	551(%rax), %dl
	movb	551(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	552(%rax), %dl
	movb	552(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	553(%rax), %dl
	movb	553(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	554(%rax), %dl
	movb	554(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	555(%rax), %dl
	movb	555(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	556(%rax), %dl
	movb	556(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	557(%rax), %dl
	movb	557(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	558(%rax), %dl
	movb	558(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	559(%rax), %dl
	movb	559(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	560(%rax), %dl
	movb	560(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	561(%rax), %dl
	movb	561(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	562(%rax), %dl
	movb	562(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	563(%rax), %dl
	movb	563(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	564(%rax), %dl
	movb	564(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	565(%rax), %dl
	movb	565(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	566(%rax), %dl
	movb	566(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	567(%rax), %dl
	movb	567(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	568(%rax), %dl
	movb	568(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	569(%rax), %dl
	movb	569(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	570(%rax), %dl
	movb	570(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	571(%rax), %dl
	movb	571(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	572(%rax), %dl
	movb	572(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	573(%rax), %dl
	movb	573(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	574(%rax), %dl
	movb	574(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	575(%rax), %dl
	movb	575(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	576(%rax), %dl
	movb	576(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	577(%rax), %dl
	movb	577(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	578(%rax), %dl
	movb	578(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	579(%rax), %dl
	movb	579(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	580(%rax), %dl
	movb	580(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	581(%rax), %dl
	movb	581(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	582(%rax), %dl
	movb	582(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	583(%rax), %dl
	movb	583(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	584(%rax), %dl
	movb	584(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	585(%rax), %dl
	movb	585(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	586(%rax), %dl
	movb	586(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	587(%rax), %dl
	movb	587(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	588(%rax), %dl
	movb	588(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	589(%rax), %dl
	movb	589(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	590(%rax), %dl
	movb	590(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	591(%rax), %dl
	movb	591(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	592(%rax), %dl
	movb	592(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	593(%rax), %dl
	movb	593(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	594(%rax), %dl
	movb	594(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	595(%rax), %dl
	movb	595(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	596(%rax), %dl
	movb	596(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	597(%rax), %dl
	movb	597(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	598(%rax), %dl
	movb	598(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	599(%rax), %dl
	movb	599(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	600(%rax), %dl
	movb	600(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	601(%rax), %dl
	movb	601(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	602(%rax), %dl
	movb	602(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	603(%rax), %dl
	movb	603(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	604(%rax), %dl
	movb	604(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	605(%rax), %dl
	movb	605(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	606(%rax), %dl
	movb	606(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	607(%rax), %dl
	movb	607(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	608(%rax), %dl
	movb	608(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	609(%rax), %dl
	movb	609(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	610(%rax), %dl
	movb	610(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	611(%rax), %dl
	movb	611(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	612(%rax), %dl
	movb	612(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	613(%rax), %dl
	movb	613(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	614(%rax), %dl
	movb	614(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	615(%rax), %dl
	movb	615(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	616(%rax), %dl
	movb	616(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	617(%rax), %dl
	movb	617(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	618(%rax), %dl
	movb	618(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	619(%rax), %dl
	movb	619(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	620(%rax), %dl
	movb	620(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	621(%rax), %dl
	movb	621(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	622(%rax), %dl
	movb	622(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	623(%rax), %dl
	movb	623(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	624(%rax), %dl
	movb	624(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	625(%rax), %dl
	movb	625(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	626(%rax), %dl
	movb	626(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	627(%rax), %dl
	movb	627(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	628(%rax), %dl
	movb	628(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	629(%rax), %dl
	movb	629(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	630(%rax), %dl
	movb	630(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	631(%rax), %dl
	movb	631(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	632(%rax), %dl
	movb	632(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	633(%rax), %dl
	movb	633(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	634(%rax), %dl
	movb	634(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	635(%rax), %dl
	movb	635(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	636(%rax), %dl
	movb	636(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	637(%rax), %dl
	movb	637(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	638(%rax), %dl
	movb	638(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	639(%rax), %dl
	movb	639(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	640(%rax), %dl
	movb	640(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	641(%rax), %dl
	movb	641(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	642(%rax), %dl
	movb	642(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	643(%rax), %dl
	movb	643(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	644(%rax), %dl
	movb	644(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	645(%rax), %dl
	movb	645(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	646(%rax), %dl
	movb	646(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	647(%rax), %dl
	movb	647(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	648(%rax), %dl
	movb	648(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	649(%rax), %dl
	movb	649(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	650(%rax), %dl
	movb	650(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	651(%rax), %dl
	movb	651(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	652(%rax), %dl
	movb	652(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	653(%rax), %dl
	movb	653(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	654(%rax), %dl
	movb	654(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	655(%rax), %dl
	movb	655(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	656(%rax), %dl
	movb	656(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	657(%rax), %dl
	movb	657(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	658(%rax), %dl
	movb	658(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	659(%rax), %dl
	movb	659(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	660(%rax), %dl
	movb	660(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	661(%rax), %dl
	movb	661(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	662(%rax), %dl
	movb	662(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	663(%rax), %dl
	movb	663(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	664(%rax), %dl
	movb	664(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	665(%rax), %dl
	movb	665(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	666(%rax), %dl
	movb	666(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	667(%rax), %dl
	movb	667(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	668(%rax), %dl
	movb	668(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	669(%rax), %dl
	movb	669(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	670(%rax), %dl
	movb	670(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	671(%rax), %dl
	movb	671(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	672(%rax), %dl
	movb	672(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	673(%rax), %dl
	movb	673(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	674(%rax), %dl
	movb	674(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	675(%rax), %dl
	movb	675(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	676(%rax), %dl
	movb	676(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	677(%rax), %dl
	movb	677(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	678(%rax), %dl
	movb	678(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	679(%rax), %dl
	movb	679(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	680(%rax), %dl
	movb	680(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	681(%rax), %dl
	movb	681(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	682(%rax), %dl
	movb	682(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	683(%rax), %dl
	movb	683(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	684(%rax), %dl
	movb	684(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	685(%rax), %dl
	movb	685(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	686(%rax), %dl
	movb	686(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	687(%rax), %dl
	movb	687(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	688(%rax), %dl
	movb	688(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	689(%rax), %dl
	movb	689(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	690(%rax), %dl
	movb	690(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	691(%rax), %dl
	movb	691(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	692(%rax), %dl
	movb	692(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	693(%rax), %dl
	movb	693(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	694(%rax), %dl
	movb	694(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	695(%rax), %dl
	movb	695(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	696(%rax), %dl
	movb	696(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	697(%rax), %dl
	movb	697(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	698(%rax), %dl
	movb	698(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	699(%rax), %dl
	movb	699(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	700(%rax), %dl
	movb	700(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	701(%rax), %dl
	movb	701(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	702(%rax), %dl
	movb	702(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	703(%rax), %dl
	movb	703(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	704(%rax), %dl
	movb	704(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	705(%rax), %dl
	movb	705(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	706(%rax), %dl
	movb	706(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	707(%rax), %dl
	movb	707(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	708(%rax), %dl
	movb	708(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	709(%rax), %dl
	movb	709(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	710(%rax), %dl
	movb	710(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	711(%rax), %dl
	movb	711(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	712(%rax), %dl
	movb	712(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	713(%rax), %dl
	movb	713(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	714(%rax), %dl
	movb	714(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	715(%rax), %dl
	movb	715(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	716(%rax), %dl
	movb	716(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	717(%rax), %dl
	movb	717(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	718(%rax), %dl
	movb	718(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	719(%rax), %dl
	movb	719(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	720(%rax), %dl
	movb	720(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	721(%rax), %dl
	movb	721(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	722(%rax), %dl
	movb	722(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	723(%rax), %dl
	movb	723(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	724(%rax), %dl
	movb	724(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	725(%rax), %dl
	movb	725(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	726(%rax), %dl
	movb	726(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	727(%rax), %dl
	movb	727(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	728(%rax), %dl
	movb	728(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	729(%rax), %dl
	movb	729(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	730(%rax), %dl
	movb	730(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	731(%rax), %dl
	movb	731(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	732(%rax), %dl
	movb	732(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	733(%rax), %dl
	movb	733(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	734(%rax), %dl
	movb	734(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	735(%rax), %dl
	movb	735(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	736(%rax), %dl
	movb	736(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	737(%rax), %dl
	movb	737(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	738(%rax), %dl
	movb	738(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	739(%rax), %dl
	movb	739(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	740(%rax), %dl
	movb	740(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	741(%rax), %dl
	movb	741(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	742(%rax), %dl
	movb	742(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	743(%rax), %dl
	movb	743(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	744(%rax), %dl
	movb	744(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	745(%rax), %dl
	movb	745(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	746(%rax), %dl
	movb	746(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	747(%rax), %dl
	movb	747(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	748(%rax), %dl
	movb	748(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	749(%rax), %dl
	movb	749(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	750(%rax), %dl
	movb	750(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	751(%rax), %dl
	movb	751(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	752(%rax), %dl
	movb	752(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	753(%rax), %dl
	movb	753(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	754(%rax), %dl
	movb	754(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	755(%rax), %dl
	movb	755(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	756(%rax), %dl
	movb	756(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	757(%rax), %dl
	movb	757(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	758(%rax), %dl
	movb	758(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	759(%rax), %dl
	movb	759(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	760(%rax), %dl
	movb	760(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	761(%rax), %dl
	movb	761(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	762(%rax), %dl
	movb	762(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	763(%rax), %dl
	movb	763(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	764(%rax), %dl
	movb	764(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	765(%rax), %dl
	movb	765(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	766(%rax), %dl
	movb	766(%rsi), %dil
	xorb	%dil, %dl
	movzbq	%dl, %rdx
	orq 	%rdx, %rcx
	movb	767(%rax), %al
	movb	767(%rsi), %dl
	xorb	%dl, %al
	movzbq	%al, %rax
	orq 	%rax, %rcx
	negq	%rcx
	shrq	$63, %rcx
	movq	16(%rsp), %rax
	addq	$64, %rax
	addq	$1536, %rax
	leaq	152(%rsp), %rdx
	negq	%rcx
	movb	(%rdx), %dil
	movb	(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, (%rdx)
	movb	1(%rdx), %dil
	movb	1(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 1(%rdx)
	movb	2(%rdx), %dil
	movb	2(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 2(%rdx)
	movb	3(%rdx), %dil
	movb	3(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 3(%rdx)
	movb	4(%rdx), %dil
	movb	4(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 4(%rdx)
	movb	5(%rdx), %dil
	movb	5(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 5(%rdx)
	movb	6(%rdx), %dil
	movb	6(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 6(%rdx)
	movb	7(%rdx), %dil
	movb	7(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 7(%rdx)
	movb	8(%rdx), %dil
	movb	8(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 8(%rdx)
	movb	9(%rdx), %dil
	movb	9(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 9(%rdx)
	movb	10(%rdx), %dil
	movb	10(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 10(%rdx)
	movb	11(%rdx), %dil
	movb	11(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 11(%rdx)
	movb	12(%rdx), %dil
	movb	12(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 12(%rdx)
	movb	13(%rdx), %dil
	movb	13(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 13(%rdx)
	movb	14(%rdx), %dil
	movb	14(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 14(%rdx)
	movb	15(%rdx), %dil
	movb	15(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 15(%rdx)
	movb	16(%rdx), %dil
	movb	16(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 16(%rdx)
	movb	17(%rdx), %dil
	movb	17(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 17(%rdx)
	movb	18(%rdx), %dil
	movb	18(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 18(%rdx)
	movb	19(%rdx), %dil
	movb	19(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 19(%rdx)
	movb	20(%rdx), %dil
	movb	20(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 20(%rdx)
	movb	21(%rdx), %dil
	movb	21(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 21(%rdx)
	movb	22(%rdx), %dil
	movb	22(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 22(%rdx)
	movb	23(%rdx), %dil
	movb	23(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 23(%rdx)
	movb	24(%rdx), %dil
	movb	24(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 24(%rdx)
	movb	25(%rdx), %dil
	movb	25(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 25(%rdx)
	movb	26(%rdx), %dil
	movb	26(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 26(%rdx)
	movb	27(%rdx), %dil
	movb	27(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 27(%rdx)
	movb	28(%rdx), %dil
	movb	28(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 28(%rdx)
	movb	29(%rdx), %dil
	movb	29(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 29(%rdx)
	movb	30(%rdx), %dil
	movb	30(%rax), %r8b
	xorb	%dil, %r8b
	andb	%cl, %r8b
	xorb	%r8b, %dil
	movb	%dil, 30(%rdx)
	movb	31(%rdx), %dil
	movb	31(%rax), %al
	xorb	%dil, %al
	andb	%cl, %al
	xorb	%al, %dil
	movb	%dil, 31(%rdx)
	movq	$768, %rcx
	leaq	184(%rsp), %rax
	leaq	-232(%rsp), %rsp
	call	L_sha3_256$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$2:
	leaq	232(%rsp), %rsp
	movq	(%rsp), %rax
	movq	$32, %rcx
	leaq	152(%rsp), %rdx
	leaq	-216(%rsp), %rsp
	call	L_shake256_64$1
Ljade_kem_kyber_kyber512_amd64_ref_dec$1:
	leaq	216(%rsp), %rsp
	xorq	%rax, %rax
	movq	9584(%rsp), %r14
	movq	9592(%rsp), %r13
	movq	9600(%rsp), %r12
	movq	9608(%rsp), %rbp
	movq	9616(%rsp), %rbx
	movq	9624(%rsp), %rsp
	ret
_jade_kem_kyber_kyber512_amd64_ref_enc:
jade_kem_kyber_kyber512_amd64_ref_enc:
	movq	%rsp, %rax
	leaq	-80(%rsp), %rsp
	andq	$-16, %rsp
	movq	%rax, 72(%rsp)
	movq	%r14, 32(%rsp)
	movq	%r13, 40(%rsp)
	movq	%r12, 48(%rsp)
	movq	%rbp, 56(%rsp)
	movq	%rbx, 64(%rsp)
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	movq	%rdx, %r12
	movq	%rsp, %rdi
	movq	$32, %rsi
	call	__jasmin_syscall_randombytes__
	movq	%rsp, %rcx
	leaq	-8800(%rsp), %rsp
	call	L__crypto_kem_enc_derand_jazz$1
Ljade_kem_kyber_kyber512_amd64_ref_enc$1:
	leaq	8800(%rsp), %rsp
	xorq	%rax, %rax
	movq	32(%rsp), %r14
	movq	40(%rsp), %r13
	movq	48(%rsp), %r12
	movq	56(%rsp), %rbp
	movq	64(%rsp), %rbx
	movq	72(%rsp), %rsp
	ret
_jade_kem_kyber_kyber512_amd64_ref_enc_derand:
jade_kem_kyber_kyber512_amd64_ref_enc_derand:
	movq	%rsp, %rax
	leaq	-80(%rsp), %rsp
	andq	$-8, %rsp
	movq	%rax, 72(%rsp)
	movq	%r14, 32(%rsp)
	movq	%r13, 40(%rsp)
	movq	%r12, 48(%rsp)
	movq	%rbp, 56(%rsp)
	movq	%rbx, 64(%rsp)
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	movq	%rdx, %r12
	movq	%rsp, %rax
	movb	(%rcx), %dl
	movb	%dl, (%rax)
	movb	1(%rcx), %dl
	movb	%dl, 1(%rax)
	movb	2(%rcx), %dl
	movb	%dl, 2(%rax)
	movb	3(%rcx), %dl
	movb	%dl, 3(%rax)
	movb	4(%rcx), %dl
	movb	%dl, 4(%rax)
	movb	5(%rcx), %dl
	movb	%dl, 5(%rax)
	movb	6(%rcx), %dl
	movb	%dl, 6(%rax)
	movb	7(%rcx), %dl
	movb	%dl, 7(%rax)
	movb	8(%rcx), %dl
	movb	%dl, 8(%rax)
	movb	9(%rcx), %dl
	movb	%dl, 9(%rax)
	movb	10(%rcx), %dl
	movb	%dl, 10(%rax)
	movb	11(%rcx), %dl
	movb	%dl, 11(%rax)
	movb	12(%rcx), %dl
	movb	%dl, 12(%rax)
	movb	13(%rcx), %dl
	movb	%dl, 13(%rax)
	movb	14(%rcx), %dl
	movb	%dl, 14(%rax)
	movb	15(%rcx), %dl
	movb	%dl, 15(%rax)
	movb	16(%rcx), %dl
	movb	%dl, 16(%rax)
	movb	17(%rcx), %dl
	movb	%dl, 17(%rax)
	movb	18(%rcx), %dl
	movb	%dl, 18(%rax)
	movb	19(%rcx), %dl
	movb	%dl, 19(%rax)
	movb	20(%rcx), %dl
	movb	%dl, 20(%rax)
	movb	21(%rcx), %dl
	movb	%dl, 21(%rax)
	movb	22(%rcx), %dl
	movb	%dl, 22(%rax)
	movb	23(%rcx), %dl
	movb	%dl, 23(%rax)
	movb	24(%rcx), %dl
	movb	%dl, 24(%rax)
	movb	25(%rcx), %dl
	movb	%dl, 25(%rax)
	movb	26(%rcx), %dl
	movb	%dl, 26(%rax)
	movb	27(%rcx), %dl
	movb	%dl, 27(%rax)
	movb	28(%rcx), %dl
	movb	%dl, 28(%rax)
	movb	29(%rcx), %dl
	movb	%dl, 29(%rax)
	movb	30(%rcx), %dl
	movb	%dl, 30(%rax)
	movb	31(%rcx), %cl
	movb	%cl, 31(%rax)
	movq	%rsp, %rcx
	leaq	-8800(%rsp), %rsp
	call	L__crypto_kem_enc_derand_jazz$1
Ljade_kem_kyber_kyber512_amd64_ref_enc_derand$1:
	leaq	8800(%rsp), %rsp
	xorq	%rax, %rax
	movq	32(%rsp), %r14
	movq	40(%rsp), %r13
	movq	48(%rsp), %r12
	movq	56(%rsp), %rbp
	movq	64(%rsp), %rbx
	movq	72(%rsp), %rsp
	ret
_jade_kem_kyber_kyber512_amd64_ref_keypair:
jade_kem_kyber_kyber512_amd64_ref_keypair:
	movq	%rsp, %rax
	leaq	-112(%rsp), %rsp
	andq	$-16, %rsp
	movq	%rax, 104(%rsp)
	movq	%r14, 64(%rsp)
	movq	%r13, 72(%rsp)
	movq	%r12, 80(%rsp)
	movq	%rbp, 88(%rsp)
	movq	%rbx, 96(%rsp)
	movq	%rdi, %rbp
	movq	%rsi, %rbx
	movq	%rsp, %rdi
	movq	$64, %rsi
	call	__jasmin_syscall_randombytes__
	movq	%rsp, %rax
	leaq	-6224(%rsp), %rsp
	call	L__crypto_kem_keypair_derand_jazz$1
Ljade_kem_kyber_kyber512_amd64_ref_keypair$1:
	leaq	6224(%rsp), %rsp
	xorq	%rax, %rax
	movq	64(%rsp), %r14
	movq	72(%rsp), %r13
	movq	80(%rsp), %r12
	movq	88(%rsp), %rbp
	movq	96(%rsp), %rbx
	movq	104(%rsp), %rsp
	ret
_jade_kem_kyber_kyber512_amd64_ref_keypair_derand:
jade_kem_kyber_kyber512_amd64_ref_keypair_derand:
	movq	%rsp, %rax
	leaq	-112(%rsp), %rsp
	andq	$-8, %rsp
	movq	%rax, 104(%rsp)
	movq	%r14, 64(%rsp)
	movq	%r13, 72(%rsp)
	movq	%r12, 80(%rsp)
	movq	%rbp, 88(%rsp)
	movq	%rbx, 96(%rsp)
	movq	%rdi, %rbp
	movq	%rsi, %rbx
	movq	%rsp, %rax
	movb	(%rdx), %cl
	movb	%cl, (%rax)
	movb	1(%rdx), %cl
	movb	%cl, 1(%rax)
	movb	2(%rdx), %cl
	movb	%cl, 2(%rax)
	movb	3(%rdx), %cl
	movb	%cl, 3(%rax)
	movb	4(%rdx), %cl
	movb	%cl, 4(%rax)
	movb	5(%rdx), %cl
	movb	%cl, 5(%rax)
	movb	6(%rdx), %cl
	movb	%cl, 6(%rax)
	movb	7(%rdx), %cl
	movb	%cl, 7(%rax)
	movb	8(%rdx), %cl
	movb	%cl, 8(%rax)
	movb	9(%rdx), %cl
	movb	%cl, 9(%rax)
	movb	10(%rdx), %cl
	movb	%cl, 10(%rax)
	movb	11(%rdx), %cl
	movb	%cl, 11(%rax)
	movb	12(%rdx), %cl
	movb	%cl, 12(%rax)
	movb	13(%rdx), %cl
	movb	%cl, 13(%rax)
	movb	14(%rdx), %cl
	movb	%cl, 14(%rax)
	movb	15(%rdx), %cl
	movb	%cl, 15(%rax)
	movb	16(%rdx), %cl
	movb	%cl, 16(%rax)
	movb	17(%rdx), %cl
	movb	%cl, 17(%rax)
	movb	18(%rdx), %cl
	movb	%cl, 18(%rax)
	movb	19(%rdx), %cl
	movb	%cl, 19(%rax)
	movb	20(%rdx), %cl
	movb	%cl, 20(%rax)
	movb	21(%rdx), %cl
	movb	%cl, 21(%rax)
	movb	22(%rdx), %cl
	movb	%cl, 22(%rax)
	movb	23(%rdx), %cl
	movb	%cl, 23(%rax)
	movb	24(%rdx), %cl
	movb	%cl, 24(%rax)
	movb	25(%rdx), %cl
	movb	%cl, 25(%rax)
	movb	26(%rdx), %cl
	movb	%cl, 26(%rax)
	movb	27(%rdx), %cl
	movb	%cl, 27(%rax)
	movb	28(%rdx), %cl
	movb	%cl, 28(%rax)
	movb	29(%rdx), %cl
	movb	%cl, 29(%rax)
	movb	30(%rdx), %cl
	movb	%cl, 30(%rax)
	movb	31(%rdx), %cl
	movb	%cl, 31(%rax)
	movb	32(%rdx), %cl
	movb	%cl, 32(%rax)
	movb	33(%rdx), %cl
	movb	%cl, 33(%rax)
	movb	34(%rdx), %cl
	movb	%cl, 34(%rax)
	movb	35(%rdx), %cl
	movb	%cl, 35(%rax)
	movb	36(%rdx), %cl
	movb	%cl, 36(%rax)
	movb	37(%rdx), %cl
	movb	%cl, 37(%rax)
	movb	38(%rdx), %cl
	movb	%cl, 38(%rax)
	movb	39(%rdx), %cl
	movb	%cl, 39(%rax)
	movb	40(%rdx), %cl
	movb	%cl, 40(%rax)
	movb	41(%rdx), %cl
	movb	%cl, 41(%rax)
	movb	42(%rdx), %cl
	movb	%cl, 42(%rax)
	movb	43(%rdx), %cl
	movb	%cl, 43(%rax)
	movb	44(%rdx), %cl
	movb	%cl, 44(%rax)
	movb	45(%rdx), %cl
	movb	%cl, 45(%rax)
	movb	46(%rdx), %cl
	movb	%cl, 46(%rax)
	movb	47(%rdx), %cl
	movb	%cl, 47(%rax)
	movb	48(%rdx), %cl
	movb	%cl, 48(%rax)
	movb	49(%rdx), %cl
	movb	%cl, 49(%rax)
	movb	50(%rdx), %cl
	movb	%cl, 50(%rax)
	movb	51(%rdx), %cl
	movb	%cl, 51(%rax)
	movb	52(%rdx), %cl
	movb	%cl, 52(%rax)
	movb	53(%rdx), %cl
	movb	%cl, 53(%rax)
	movb	54(%rdx), %cl
	movb	%cl, 54(%rax)
	movb	55(%rdx), %cl
	movb	%cl, 55(%rax)
	movb	56(%rdx), %cl
	movb	%cl, 56(%rax)
	movb	57(%rdx), %cl
	movb	%cl, 57(%rax)
	movb	58(%rdx), %cl
	movb	%cl, 58(%rax)
	movb	59(%rdx), %cl
	movb	%cl, 59(%rax)
	movb	60(%rdx), %cl
	movb	%cl, 60(%rax)
	movb	61(%rdx), %cl
	movb	%cl, 61(%rax)
	movb	62(%rdx), %cl
	movb	%cl, 62(%rax)
	movb	63(%rdx), %cl
	movb	%cl, 63(%rax)
	movq	%rsp, %rax
	leaq	-6224(%rsp), %rsp
	call	L__crypto_kem_keypair_derand_jazz$1
Ljade_kem_kyber_kyber512_amd64_ref_keypair_derand$1:
	leaq	6224(%rsp), %rsp
	xorq	%rax, %rax
	movq	64(%rsp), %r14
	movq	72(%rsp), %r13
	movq	80(%rsp), %r12
	movq	88(%rsp), %rbp
	movq	96(%rsp), %rbx
	movq	104(%rsp), %rsp
	ret
L__crypto_kem_enc_derand_jazz$1:
	movq	%r12, 8(%rsp)
	movq	%rbx, 16(%rsp)
	movq	%rbp, 24(%rsp)
	leaq	80(%rsp), %rax
	leaq	-208(%rsp), %rsp
	call	L_sha3_256_32$1
L__crypto_kem_enc_derand_jazz$111:
	leaq	208(%rsp), %rsp
	movq	8(%rsp), %rsi
	movq	$800, %rcx
	leaq	112(%rsp), %rax
	leaq	-232(%rsp), %rsp
	call	L_sha3_256$1
L__crypto_kem_enc_derand_jazz$110:
	leaq	232(%rsp), %rsp
	leaq	144(%rsp), %rax
	leaq	80(%rsp), %rcx
	leaq	-208(%rsp), %rsp
	call	L_sha3_512_64$1
L__crypto_kem_enc_derand_jazz$109:
	leaq	208(%rsp), %rsp
	movq	8(%rsp), %rdx
	leaq	80(%rsp), %rcx
	leaq	176(%rsp), %rax
	movq	%rdx, %rsi
	leaq	2456(%rsp), %rdi
	call	L_poly_frombytes$1
L__crypto_kem_enc_derand_jazz$108:
	addq	$384, %rsi
	leaq	2968(%rsp), %rdi
	call	L_poly_frombytes$1
L__crypto_kem_enc_derand_jazz$107:
	movq	$0, %rsi
	addq	$768, %rdx
	jmp 	L__crypto_kem_enc_derand_jazz$105
L__crypto_kem_enc_derand_jazz$106:
	movq	(%rdx), %rdi
	movq	%rdi, 48(%rsp,%rsi,8)
	addq	$8, %rdx
	incq	%rsi
L__crypto_kem_enc_derand_jazz$105:
	cmpq	$4, %rsi
	jb  	L__crypto_kem_enc_derand_jazz$106
	leaq	920(%rsp), %rdx
	call	L_i_poly_frommsg$1
L__crypto_kem_enc_derand_jazz$104:
	movq	%rax, 8(%rsp)
	movq	$1, %rax
	movq	%rax, 32(%rsp)
	movb	48(%rsp), %al
	movb	%al, 8600(%rsp)
	movb	49(%rsp), %al
	movb	%al, 8601(%rsp)
	movb	50(%rsp), %al
	movb	%al, 8602(%rsp)
	movb	51(%rsp), %al
	movb	%al, 8603(%rsp)
	movb	52(%rsp), %al
	movb	%al, 8604(%rsp)
	movb	53(%rsp), %al
	movb	%al, 8605(%rsp)
	movb	54(%rsp), %al
	movb	%al, 8606(%rsp)
	movb	55(%rsp), %al
	movb	%al, 8607(%rsp)
	movb	56(%rsp), %al
	movb	%al, 8608(%rsp)
	movb	57(%rsp), %al
	movb	%al, 8609(%rsp)
	movb	58(%rsp), %al
	movb	%al, 8610(%rsp)
	movb	59(%rsp), %al
	movb	%al, 8611(%rsp)
	movb	60(%rsp), %al
	movb	%al, 8612(%rsp)
	movb	61(%rsp), %al
	movb	%al, 8613(%rsp)
	movb	62(%rsp), %al
	movb	%al, 8614(%rsp)
	movb	63(%rsp), %al
	movb	%al, 8615(%rsp)
	movb	64(%rsp), %al
	movb	%al, 8616(%rsp)
	movb	65(%rsp), %al
	movb	%al, 8617(%rsp)
	movb	66(%rsp), %al
	movb	%al, 8618(%rsp)
	movb	67(%rsp), %al
	movb	%al, 8619(%rsp)
	movb	68(%rsp), %al
	movb	%al, 8620(%rsp)
	movb	69(%rsp), %al
	movb	%al, 8621(%rsp)
	movb	70(%rsp), %al
	movb	%al, 8622(%rsp)
	movb	71(%rsp), %al
	movb	%al, 8623(%rsp)
	movb	72(%rsp), %al
	movb	%al, 8624(%rsp)
	movb	73(%rsp), %al
	movb	%al, 8625(%rsp)
	movb	74(%rsp), %al
	movb	%al, 8626(%rsp)
	movb	75(%rsp), %al
	movb	%al, 8627(%rsp)
	movb	76(%rsp), %al
	movb	%al, 8628(%rsp)
	movb	77(%rsp), %al
	movb	%al, 8629(%rsp)
	movb	78(%rsp), %al
	movb	%al, 8630(%rsp)
	movb	79(%rsp), %al
	movb	%al, 8631(%rsp)
	movq	32(%rsp), %rax
	cmpq	$0, %rax
	je  	L__crypto_kem_enc_derand_jazz$102
	movb	$0, 8632(%rsp)
	movb	$0, 8633(%rsp)
	jmp 	L__crypto_kem_enc_derand_jazz$103
L__crypto_kem_enc_derand_jazz$102:
	movb	$0, 8632(%rsp)
	movb	$0, 8633(%rsp)
L__crypto_kem_enc_derand_jazz$103:
	leaq	208(%rsp), %rax
	leaq	8600(%rsp), %rcx
	call	L_shake128_absorb34$1
L__crypto_kem_enc_derand_jazz$101:
	movq	$0, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$99
L__crypto_kem_enc_derand_jazz$100:
	movq	%rax, 408(%rsp,%rax,8)
	incq	%rax
L__crypto_kem_enc_derand_jazz$99:
	cmpq	$64, %rax
	jb  	L__crypto_kem_enc_derand_jazz$100
	movq	$0, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$91
L__crypto_kem_enc_derand_jazz$92:
	movq	%rax, 40(%rsp)
	leaq	208(%rsp), %rax
	leaq	8634(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
L__crypto_kem_enc_derand_jazz$98:
	leaq	8(%rsp), %rsp
	movq	40(%rsp), %rax
	movq	$0, %rcx
L__crypto_kem_enc_derand_jazz$93:
	movzbw	8634(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	8634(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	8634(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	L__crypto_kem_enc_derand_jazz$96
	cmpq	$256, %rax
	jnb 	L__crypto_kem_enc_derand_jazz$96
	movw	%dx, 408(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_enc_derand_jazz$97:
L__crypto_kem_enc_derand_jazz$96:
	cmpw	$3329, %di
	jnb 	L__crypto_kem_enc_derand_jazz$94
	cmpq	$256, %rax
	jnb 	L__crypto_kem_enc_derand_jazz$94
	movw	%di, 408(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_enc_derand_jazz$95:
L__crypto_kem_enc_derand_jazz$94:
	cmpq	$166, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$93
L__crypto_kem_enc_derand_jazz$91:
	cmpq	$256, %rax
	jb  	L__crypto_kem_enc_derand_jazz$92
	movq	$0, %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$89
L__crypto_kem_enc_derand_jazz$90:
	movw	408(%rsp,%rax,2), %dx
	movw	%dx, 6552(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
L__crypto_kem_enc_derand_jazz$89:
	cmpq	$256, %rax
	jb  	L__crypto_kem_enc_derand_jazz$90
	movq	32(%rsp), %rax
	cmpq	$0, %rax
	je  	L__crypto_kem_enc_derand_jazz$87
	movb	$0, 8632(%rsp)
	movb	$1, 8633(%rsp)
	jmp 	L__crypto_kem_enc_derand_jazz$88
L__crypto_kem_enc_derand_jazz$87:
	movb	$1, 8632(%rsp)
	movb	$0, 8633(%rsp)
L__crypto_kem_enc_derand_jazz$88:
	leaq	208(%rsp), %rax
	leaq	8600(%rsp), %rcx
	call	L_shake128_absorb34$1
L__crypto_kem_enc_derand_jazz$86:
	movq	$0, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$84
L__crypto_kem_enc_derand_jazz$85:
	movq	%rax, 408(%rsp,%rax,8)
	incq	%rax
L__crypto_kem_enc_derand_jazz$84:
	cmpq	$64, %rax
	jb  	L__crypto_kem_enc_derand_jazz$85
	movq	$0, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$76
L__crypto_kem_enc_derand_jazz$77:
	movq	%rax, 40(%rsp)
	leaq	208(%rsp), %rax
	leaq	8634(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
L__crypto_kem_enc_derand_jazz$83:
	leaq	8(%rsp), %rsp
	movq	40(%rsp), %rax
	movq	$0, %rcx
L__crypto_kem_enc_derand_jazz$78:
	movzbw	8634(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	8634(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	8634(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	L__crypto_kem_enc_derand_jazz$81
	cmpq	$256, %rax
	jnb 	L__crypto_kem_enc_derand_jazz$81
	movw	%dx, 408(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_enc_derand_jazz$82:
L__crypto_kem_enc_derand_jazz$81:
	cmpw	$3329, %di
	jnb 	L__crypto_kem_enc_derand_jazz$79
	cmpq	$256, %rax
	jnb 	L__crypto_kem_enc_derand_jazz$79
	movw	%di, 408(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_enc_derand_jazz$80:
L__crypto_kem_enc_derand_jazz$79:
	cmpq	$166, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$78
L__crypto_kem_enc_derand_jazz$76:
	cmpq	$256, %rax
	jb  	L__crypto_kem_enc_derand_jazz$77
	movq	$0, %rax
	movq	$256, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$74
L__crypto_kem_enc_derand_jazz$75:
	movw	408(%rsp,%rax,2), %dx
	movw	%dx, 6552(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
L__crypto_kem_enc_derand_jazz$74:
	cmpq	$256, %rax
	jb  	L__crypto_kem_enc_derand_jazz$75
	movq	32(%rsp), %rax
	cmpq	$0, %rax
	je  	L__crypto_kem_enc_derand_jazz$72
	movb	$1, 8632(%rsp)
	movb	$0, 8633(%rsp)
	jmp 	L__crypto_kem_enc_derand_jazz$73
L__crypto_kem_enc_derand_jazz$72:
	movb	$0, 8632(%rsp)
	movb	$1, 8633(%rsp)
L__crypto_kem_enc_derand_jazz$73:
	leaq	208(%rsp), %rax
	leaq	8600(%rsp), %rcx
	call	L_shake128_absorb34$1
L__crypto_kem_enc_derand_jazz$71:
	movq	$0, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$69
L__crypto_kem_enc_derand_jazz$70:
	movq	%rax, 408(%rsp,%rax,8)
	incq	%rax
L__crypto_kem_enc_derand_jazz$69:
	cmpq	$64, %rax
	jb  	L__crypto_kem_enc_derand_jazz$70
	movq	$0, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$61
L__crypto_kem_enc_derand_jazz$62:
	movq	%rax, 40(%rsp)
	leaq	208(%rsp), %rax
	leaq	8634(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
L__crypto_kem_enc_derand_jazz$68:
	leaq	8(%rsp), %rsp
	movq	40(%rsp), %rax
	movq	$0, %rcx
L__crypto_kem_enc_derand_jazz$63:
	movzbw	8634(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	8634(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	8634(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	L__crypto_kem_enc_derand_jazz$66
	cmpq	$256, %rax
	jnb 	L__crypto_kem_enc_derand_jazz$66
	movw	%dx, 408(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_enc_derand_jazz$67:
L__crypto_kem_enc_derand_jazz$66:
	cmpw	$3329, %di
	jnb 	L__crypto_kem_enc_derand_jazz$64
	cmpq	$256, %rax
	jnb 	L__crypto_kem_enc_derand_jazz$64
	movw	%di, 408(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_enc_derand_jazz$65:
L__crypto_kem_enc_derand_jazz$64:
	cmpq	$166, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$63
L__crypto_kem_enc_derand_jazz$61:
	cmpq	$256, %rax
	jb  	L__crypto_kem_enc_derand_jazz$62
	movq	$0, %rax
	movq	$512, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$59
L__crypto_kem_enc_derand_jazz$60:
	movw	408(%rsp,%rax,2), %dx
	movw	%dx, 6552(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
L__crypto_kem_enc_derand_jazz$59:
	cmpq	$256, %rax
	jb  	L__crypto_kem_enc_derand_jazz$60
	movq	32(%rsp), %rax
	cmpq	$0, %rax
	je  	L__crypto_kem_enc_derand_jazz$57
	movb	$1, 8632(%rsp)
	movb	$1, 8633(%rsp)
	jmp 	L__crypto_kem_enc_derand_jazz$58
L__crypto_kem_enc_derand_jazz$57:
	movb	$1, 8632(%rsp)
	movb	$1, 8633(%rsp)
L__crypto_kem_enc_derand_jazz$58:
	leaq	208(%rsp), %rax
	leaq	8600(%rsp), %rcx
	call	L_shake128_absorb34$1
L__crypto_kem_enc_derand_jazz$56:
	movq	$0, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$54
L__crypto_kem_enc_derand_jazz$55:
	movq	%rax, 408(%rsp,%rax,8)
	incq	%rax
L__crypto_kem_enc_derand_jazz$54:
	cmpq	$64, %rax
	jb  	L__crypto_kem_enc_derand_jazz$55
	movq	$0, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$46
L__crypto_kem_enc_derand_jazz$47:
	movq	%rax, 32(%rsp)
	leaq	208(%rsp), %rax
	leaq	8634(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
L__crypto_kem_enc_derand_jazz$53:
	leaq	8(%rsp), %rsp
	movq	32(%rsp), %rax
	movq	$0, %rcx
L__crypto_kem_enc_derand_jazz$48:
	movzbw	8634(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	8634(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	8634(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	L__crypto_kem_enc_derand_jazz$51
	cmpq	$256, %rax
	jnb 	L__crypto_kem_enc_derand_jazz$51
	movw	%dx, 408(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_enc_derand_jazz$52:
L__crypto_kem_enc_derand_jazz$51:
	cmpw	$3329, %di
	jnb 	L__crypto_kem_enc_derand_jazz$49
	cmpq	$256, %rax
	jnb 	L__crypto_kem_enc_derand_jazz$49
	movw	%di, 408(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_enc_derand_jazz$50:
L__crypto_kem_enc_derand_jazz$49:
	cmpq	$166, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$48
L__crypto_kem_enc_derand_jazz$46:
	cmpq	$256, %rax
	jb  	L__crypto_kem_enc_derand_jazz$47
	movq	$0, %rax
	movq	$768, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$44
L__crypto_kem_enc_derand_jazz$45:
	movw	408(%rsp,%rax,2), %dx
	movw	%dx, 6552(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
L__crypto_kem_enc_derand_jazz$44:
	cmpq	$256, %rax
	jb  	L__crypto_kem_enc_derand_jazz$45
	movq	8(%rsp), %rcx
	movb	$0, %al
	leaq	3480(%rsp), %rdx
	leaq	-240(%rsp), %rsp
	call	L_poly_getnoise_eta1$1
L__crypto_kem_enc_derand_jazz$43:
	leaq	240(%rsp), %rsp
	movq	8(%rsp), %rcx
	movb	$1, %al
	leaq	3992(%rsp), %rdx
	leaq	-240(%rsp), %rsp
	call	L_poly_getnoise_eta1$1
L__crypto_kem_enc_derand_jazz$42:
	leaq	240(%rsp), %rsp
	movq	8(%rsp), %rax
	movb	$2, %cl
	leaq	4504(%rsp), %rdx
	leaq	-176(%rsp), %rsp
	call	L_poly_getnoise_eta2$1
L__crypto_kem_enc_derand_jazz$41:
	leaq	176(%rsp), %rsp
	movq	8(%rsp), %rax
	movb	$3, %cl
	leaq	5016(%rsp), %rdx
	leaq	-176(%rsp), %rsp
	call	L_poly_getnoise_eta2$1
L__crypto_kem_enc_derand_jazz$40:
	leaq	176(%rsp), %rsp
	movq	8(%rsp), %rax
	movb	$4, %cl
	leaq	408(%rsp), %rdx
	leaq	-176(%rsp), %rsp
	call	L_poly_getnoise_eta2$1
L__crypto_kem_enc_derand_jazz$39:
	leaq	176(%rsp), %rsp
	leaq	3480(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_enc_derand_jazz$38:
	leaq	3992(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_enc_derand_jazz$37:
	leaq	5528(%rsp), %rdi
	leaq	6552(%rsp), %rcx
	leaq	3480(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$36:
	leaq	16(%rsp), %rsp
	leaq	1432(%rsp), %rdi
	leaq	7064(%rsp), %rcx
	leaq	3992(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$35:
	leaq	16(%rsp), %rsp
	leaq	5528(%rsp), %rcx
	leaq	1432(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$34:
	leaq	5528(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$32
L__crypto_kem_enc_derand_jazz$33:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_enc_derand_jazz$32:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$33
	leaq	6040(%rsp), %rdi
	leaq	7576(%rsp), %rcx
	leaq	3480(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$31:
	leaq	16(%rsp), %rsp
	leaq	1432(%rsp), %rdi
	leaq	8088(%rsp), %rcx
	leaq	3992(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$30:
	leaq	16(%rsp), %rsp
	leaq	6040(%rsp), %rcx
	leaq	1432(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$29:
	leaq	6040(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$27
L__crypto_kem_enc_derand_jazz$28:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_enc_derand_jazz$27:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$28
	leaq	1432(%rsp), %rdi
	leaq	2456(%rsp), %rcx
	leaq	3480(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$26:
	leaq	16(%rsp), %rsp
	leaq	1944(%rsp), %rdi
	leaq	2968(%rsp), %rcx
	leaq	3992(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$25:
	leaq	16(%rsp), %rsp
	leaq	1432(%rsp), %rcx
	leaq	1944(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$24:
	leaq	1432(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$22
L__crypto_kem_enc_derand_jazz$23:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_enc_derand_jazz$22:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$23
	leaq	5528(%rsp), %rcx
	call	L_poly_invntt$1
L__crypto_kem_enc_derand_jazz$21:
	leaq	6040(%rsp), %rcx
	call	L_poly_invntt$1
L__crypto_kem_enc_derand_jazz$20:
	leaq	1432(%rsp), %rcx
	call	L_poly_invntt$1
L__crypto_kem_enc_derand_jazz$19:
	leaq	5528(%rsp), %rcx
	leaq	4504(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$18:
	leaq	6040(%rsp), %rcx
	leaq	5016(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$17:
	leaq	1432(%rsp), %rcx
	leaq	408(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$16:
	leaq	1432(%rsp), %rcx
	leaq	920(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$15:
	leaq	5528(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$13
L__crypto_kem_enc_derand_jazz$14:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_enc_derand_jazz$13:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$14
	leaq	6040(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$11
L__crypto_kem_enc_derand_jazz$12:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_enc_derand_jazz$11:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$12
	leaq	1432(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_enc_derand_jazz$9
L__crypto_kem_enc_derand_jazz$10:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_enc_derand_jazz$9:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_enc_derand_jazz$10
	movq	16(%rsp), %rax
	movq	$0, %rdx
	movq	$0, %rsi
	leaq	5528(%rsp), %rcx
	call	L_poly_csubq$1
L__crypto_kem_enc_derand_jazz$8:
	leaq	6040(%rsp), %rcx
	call	L_poly_csubq$1
L__crypto_kem_enc_derand_jazz$7:
	jmp 	L__crypto_kem_enc_derand_jazz$5
L__crypto_kem_enc_derand_jazz$6:
	movzwq	5528(%rsp,%rdx,2), %rcx
	incq	%rdx
	shlq	$10, %rcx
	addq	$1665, %rcx
	imulq	$1290167, %rcx, %rcx
	shrq	$32, %rcx
	andq	$1023, %rcx
	movzwq	5528(%rsp,%rdx,2), %rdi
	incq	%rdx
	shlq	$10, %rdi
	addq	$1665, %rdi
	imulq	$1290167, %rdi, %rdi
	shrq	$32, %rdi
	andq	$1023, %rdi
	movzwq	5528(%rsp,%rdx,2), %r8
	incq	%rdx
	shlq	$10, %r8
	addq	$1665, %r8
	imulq	$1290167, %r8, %r8
	shrq	$32, %r8
	andq	$1023, %r8
	movzwq	5528(%rsp,%rdx,2), %r9
	incq	%rdx
	shlq	$10, %r9
	addq	$1665, %r9
	imulq	$1290167, %r9, %r9
	shrq	$32, %r9
	andq	$1023, %r9
	movw	%cx, %r10w
	andw	$255, %r10w
	movb	%r10b, (%rax,%rsi)
	incq	%rsi
	shrw	$8, %cx
	movw	%di, %r10w
	shlw	$2, %r10w
	orw 	%cx, %r10w
	movb	%r10b, (%rax,%rsi)
	incq	%rsi
	shrw	$6, %di
	movw	%r8w, %cx
	shlw	$4, %cx
	orw 	%di, %cx
	movb	%cl, (%rax,%rsi)
	incq	%rsi
	shrw	$4, %r8w
	movw	%r9w, %cx
	shlw	$6, %cx
	orw 	%r8w, %cx
	movb	%cl, (%rax,%rsi)
	incq	%rsi
	shrq	$2, %r9
	movb	%r9b, (%rax,%rsi)
	incq	%rsi
L__crypto_kem_enc_derand_jazz$5:
	cmpq	$512, %rdx
	jb  	L__crypto_kem_enc_derand_jazz$6
	addq	$640, %rax
	leaq	1432(%rsp), %rcx
	call	L_poly_compress$1
L__crypto_kem_enc_derand_jazz$4:
	movq	16(%rsp), %rsi
	movq	$768, %rcx
	leaq	176(%rsp), %rax
	leaq	-232(%rsp), %rsp
	call	L_sha3_256$1
L__crypto_kem_enc_derand_jazz$3:
	leaq	232(%rsp), %rsp
	movq	24(%rsp), %rax
	movq	$32, %rcx
	leaq	144(%rsp), %rdx
	leaq	-216(%rsp), %rsp
	call	L_shake256_64$1
L__crypto_kem_enc_derand_jazz$2:
	leaq	216(%rsp), %rsp
	ret
L__crypto_kem_keypair_derand_jazz$1:
	movq	%rax, 8(%rsp)
	movq	%rbp, 16(%rsp)
	movq	%rbx, 24(%rsp)
	movq	%rax, %rcx
	movq	%rbp, 32(%rsp)
	movq	%rbx, 40(%rsp)
	leaq	128(%rsp), %rax
	leaq	-208(%rsp), %rsp
	call	L_sha3_512_32$1
L__crypto_kem_keypair_derand_jazz$93:
	leaq	208(%rsp), %rsp
	movq	128(%rsp), %rax
	movq	%rax, 96(%rsp)
	movq	160(%rsp), %rax
	movq	%rax, 64(%rsp)
	movq	136(%rsp), %rax
	movq	%rax, 104(%rsp)
	movq	168(%rsp), %rax
	movq	%rax, 72(%rsp)
	movq	144(%rsp), %rax
	movq	%rax, 112(%rsp)
	movq	176(%rsp), %rax
	movq	%rax, 80(%rsp)
	movq	152(%rsp), %rax
	movq	%rax, 120(%rsp)
	movq	184(%rsp), %rax
	movq	%rax, 88(%rsp)
	movq	$0, %rax
	movq	%rax, 48(%rsp)
	movb	96(%rsp), %al
	movb	%al, 6024(%rsp)
	movb	97(%rsp), %al
	movb	%al, 6025(%rsp)
	movb	98(%rsp), %al
	movb	%al, 6026(%rsp)
	movb	99(%rsp), %al
	movb	%al, 6027(%rsp)
	movb	100(%rsp), %al
	movb	%al, 6028(%rsp)
	movb	101(%rsp), %al
	movb	%al, 6029(%rsp)
	movb	102(%rsp), %al
	movb	%al, 6030(%rsp)
	movb	103(%rsp), %al
	movb	%al, 6031(%rsp)
	movb	104(%rsp), %al
	movb	%al, 6032(%rsp)
	movb	105(%rsp), %al
	movb	%al, 6033(%rsp)
	movb	106(%rsp), %al
	movb	%al, 6034(%rsp)
	movb	107(%rsp), %al
	movb	%al, 6035(%rsp)
	movb	108(%rsp), %al
	movb	%al, 6036(%rsp)
	movb	109(%rsp), %al
	movb	%al, 6037(%rsp)
	movb	110(%rsp), %al
	movb	%al, 6038(%rsp)
	movb	111(%rsp), %al
	movb	%al, 6039(%rsp)
	movb	112(%rsp), %al
	movb	%al, 6040(%rsp)
	movb	113(%rsp), %al
	movb	%al, 6041(%rsp)
	movb	114(%rsp), %al
	movb	%al, 6042(%rsp)
	movb	115(%rsp), %al
	movb	%al, 6043(%rsp)
	movb	116(%rsp), %al
	movb	%al, 6044(%rsp)
	movb	117(%rsp), %al
	movb	%al, 6045(%rsp)
	movb	118(%rsp), %al
	movb	%al, 6046(%rsp)
	movb	119(%rsp), %al
	movb	%al, 6047(%rsp)
	movb	120(%rsp), %al
	movb	%al, 6048(%rsp)
	movb	121(%rsp), %al
	movb	%al, 6049(%rsp)
	movb	122(%rsp), %al
	movb	%al, 6050(%rsp)
	movb	123(%rsp), %al
	movb	%al, 6051(%rsp)
	movb	124(%rsp), %al
	movb	%al, 6052(%rsp)
	movb	125(%rsp), %al
	movb	%al, 6053(%rsp)
	movb	126(%rsp), %al
	movb	%al, 6054(%rsp)
	movb	127(%rsp), %al
	movb	%al, 6055(%rsp)
	movq	48(%rsp), %rax
	cmpq	$0, %rax
	je  	L__crypto_kem_keypair_derand_jazz$91
	movb	$0, 6056(%rsp)
	movb	$0, 6057(%rsp)
	jmp 	L__crypto_kem_keypair_derand_jazz$92
L__crypto_kem_keypair_derand_jazz$91:
	movb	$0, 6056(%rsp)
	movb	$0, 6057(%rsp)
L__crypto_kem_keypair_derand_jazz$92:
	leaq	192(%rsp), %rax
	leaq	6024(%rsp), %rcx
	call	L_shake128_absorb34$1
L__crypto_kem_keypair_derand_jazz$90:
	movq	$0, %rax
	jmp 	L__crypto_kem_keypair_derand_jazz$88
L__crypto_kem_keypair_derand_jazz$89:
	movq	%rax, 392(%rsp,%rax,8)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$88:
	cmpq	$64, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$89
	movq	$0, %rax
	jmp 	L__crypto_kem_keypair_derand_jazz$80
L__crypto_kem_keypair_derand_jazz$81:
	movq	%rax, 56(%rsp)
	leaq	192(%rsp), %rax
	leaq	6058(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
L__crypto_kem_keypair_derand_jazz$87:
	leaq	8(%rsp), %rsp
	movq	56(%rsp), %rax
	movq	$0, %rcx
L__crypto_kem_keypair_derand_jazz$82:
	movzbw	6058(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	6058(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	6058(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	L__crypto_kem_keypair_derand_jazz$85
	cmpq	$256, %rax
	jnb 	L__crypto_kem_keypair_derand_jazz$85
	movw	%dx, 392(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$86:
L__crypto_kem_keypair_derand_jazz$85:
	cmpw	$3329, %di
	jnb 	L__crypto_kem_keypair_derand_jazz$83
	cmpq	$256, %rax
	jnb 	L__crypto_kem_keypair_derand_jazz$83
	movw	%di, 392(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$84:
L__crypto_kem_keypair_derand_jazz$83:
	cmpq	$166, %rcx
	jb  	L__crypto_kem_keypair_derand_jazz$82
L__crypto_kem_keypair_derand_jazz$80:
	cmpq	$256, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$81
	movq	$0, %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_keypair_derand_jazz$78
L__crypto_kem_keypair_derand_jazz$79:
	movw	392(%rsp,%rax,2), %dx
	movw	%dx, 3976(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
L__crypto_kem_keypair_derand_jazz$78:
	cmpq	$256, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$79
	movq	48(%rsp), %rax
	cmpq	$0, %rax
	je  	L__crypto_kem_keypair_derand_jazz$76
	movb	$0, 6056(%rsp)
	movb	$1, 6057(%rsp)
	jmp 	L__crypto_kem_keypair_derand_jazz$77
L__crypto_kem_keypair_derand_jazz$76:
	movb	$1, 6056(%rsp)
	movb	$0, 6057(%rsp)
L__crypto_kem_keypair_derand_jazz$77:
	leaq	192(%rsp), %rax
	leaq	6024(%rsp), %rcx
	call	L_shake128_absorb34$1
L__crypto_kem_keypair_derand_jazz$75:
	movq	$0, %rax
	jmp 	L__crypto_kem_keypair_derand_jazz$73
L__crypto_kem_keypair_derand_jazz$74:
	movq	%rax, 392(%rsp,%rax,8)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$73:
	cmpq	$64, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$74
	movq	$0, %rax
	jmp 	L__crypto_kem_keypair_derand_jazz$65
L__crypto_kem_keypair_derand_jazz$66:
	movq	%rax, 56(%rsp)
	leaq	192(%rsp), %rax
	leaq	6058(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
L__crypto_kem_keypair_derand_jazz$72:
	leaq	8(%rsp), %rsp
	movq	56(%rsp), %rax
	movq	$0, %rcx
L__crypto_kem_keypair_derand_jazz$67:
	movzbw	6058(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	6058(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	6058(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	L__crypto_kem_keypair_derand_jazz$70
	cmpq	$256, %rax
	jnb 	L__crypto_kem_keypair_derand_jazz$70
	movw	%dx, 392(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$71:
L__crypto_kem_keypair_derand_jazz$70:
	cmpw	$3329, %di
	jnb 	L__crypto_kem_keypair_derand_jazz$68
	cmpq	$256, %rax
	jnb 	L__crypto_kem_keypair_derand_jazz$68
	movw	%di, 392(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$69:
L__crypto_kem_keypair_derand_jazz$68:
	cmpq	$166, %rcx
	jb  	L__crypto_kem_keypair_derand_jazz$67
L__crypto_kem_keypair_derand_jazz$65:
	cmpq	$256, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$66
	movq	$0, %rax
	movq	$256, %rcx
	jmp 	L__crypto_kem_keypair_derand_jazz$63
L__crypto_kem_keypair_derand_jazz$64:
	movw	392(%rsp,%rax,2), %dx
	movw	%dx, 3976(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
L__crypto_kem_keypair_derand_jazz$63:
	cmpq	$256, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$64
	movq	48(%rsp), %rax
	cmpq	$0, %rax
	je  	L__crypto_kem_keypair_derand_jazz$61
	movb	$1, 6056(%rsp)
	movb	$0, 6057(%rsp)
	jmp 	L__crypto_kem_keypair_derand_jazz$62
L__crypto_kem_keypair_derand_jazz$61:
	movb	$0, 6056(%rsp)
	movb	$1, 6057(%rsp)
L__crypto_kem_keypair_derand_jazz$62:
	leaq	192(%rsp), %rax
	leaq	6024(%rsp), %rcx
	call	L_shake128_absorb34$1
L__crypto_kem_keypair_derand_jazz$60:
	movq	$0, %rax
	jmp 	L__crypto_kem_keypair_derand_jazz$58
L__crypto_kem_keypair_derand_jazz$59:
	movq	%rax, 392(%rsp,%rax,8)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$58:
	cmpq	$64, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$59
	movq	$0, %rax
	jmp 	L__crypto_kem_keypair_derand_jazz$50
L__crypto_kem_keypair_derand_jazz$51:
	movq	%rax, 56(%rsp)
	leaq	192(%rsp), %rax
	leaq	6058(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
L__crypto_kem_keypair_derand_jazz$57:
	leaq	8(%rsp), %rsp
	movq	56(%rsp), %rax
	movq	$0, %rcx
L__crypto_kem_keypair_derand_jazz$52:
	movzbw	6058(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	6058(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	6058(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	L__crypto_kem_keypair_derand_jazz$55
	cmpq	$256, %rax
	jnb 	L__crypto_kem_keypair_derand_jazz$55
	movw	%dx, 392(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$56:
L__crypto_kem_keypair_derand_jazz$55:
	cmpw	$3329, %di
	jnb 	L__crypto_kem_keypair_derand_jazz$53
	cmpq	$256, %rax
	jnb 	L__crypto_kem_keypair_derand_jazz$53
	movw	%di, 392(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$54:
L__crypto_kem_keypair_derand_jazz$53:
	cmpq	$166, %rcx
	jb  	L__crypto_kem_keypair_derand_jazz$52
L__crypto_kem_keypair_derand_jazz$50:
	cmpq	$256, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$51
	movq	$0, %rax
	movq	$512, %rcx
	jmp 	L__crypto_kem_keypair_derand_jazz$48
L__crypto_kem_keypair_derand_jazz$49:
	movw	392(%rsp,%rax,2), %dx
	movw	%dx, 3976(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
L__crypto_kem_keypair_derand_jazz$48:
	cmpq	$256, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$49
	movq	48(%rsp), %rax
	cmpq	$0, %rax
	je  	L__crypto_kem_keypair_derand_jazz$46
	movb	$1, 6056(%rsp)
	movb	$1, 6057(%rsp)
	jmp 	L__crypto_kem_keypair_derand_jazz$47
L__crypto_kem_keypair_derand_jazz$46:
	movb	$1, 6056(%rsp)
	movb	$1, 6057(%rsp)
L__crypto_kem_keypair_derand_jazz$47:
	leaq	192(%rsp), %rax
	leaq	6024(%rsp), %rcx
	call	L_shake128_absorb34$1
L__crypto_kem_keypair_derand_jazz$45:
	movq	$0, %rax
	jmp 	L__crypto_kem_keypair_derand_jazz$43
L__crypto_kem_keypair_derand_jazz$44:
	movq	%rax, 392(%rsp,%rax,8)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$43:
	cmpq	$64, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$44
	movq	$0, %rax
	jmp 	L__crypto_kem_keypair_derand_jazz$35
L__crypto_kem_keypair_derand_jazz$36:
	movq	%rax, 48(%rsp)
	leaq	192(%rsp), %rax
	leaq	6058(%rsp), %rcx
	leaq	-8(%rsp), %rsp
	call	L_shake128_squeezeblock$1
L__crypto_kem_keypair_derand_jazz$42:
	leaq	8(%rsp), %rsp
	movq	48(%rsp), %rax
	movq	$0, %rcx
L__crypto_kem_keypair_derand_jazz$37:
	movzbw	6058(%rsp,%rcx), %dx
	incq	%rcx
	movzbw	6058(%rsp,%rcx), %si
	movw	%si, %di
	shrw	$4, %di
	andw	$15, %si
	shlw	$8, %si
	orw 	%si, %dx
	incq	%rcx
	movzbw	6058(%rsp,%rcx), %si
	shlw	$4, %si
	orw 	%si, %di
	incq	%rcx
	cmpw	$3329, %dx
	jnb 	L__crypto_kem_keypair_derand_jazz$40
	cmpq	$256, %rax
	jnb 	L__crypto_kem_keypair_derand_jazz$40
	movw	%dx, 392(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$41:
L__crypto_kem_keypair_derand_jazz$40:
	cmpw	$3329, %di
	jnb 	L__crypto_kem_keypair_derand_jazz$38
	cmpq	$256, %rax
	jnb 	L__crypto_kem_keypair_derand_jazz$38
	movw	%di, 392(%rsp,%rax,2)
	incq	%rax
L__crypto_kem_keypair_derand_jazz$39:
L__crypto_kem_keypair_derand_jazz$38:
	cmpq	$166, %rcx
	jb  	L__crypto_kem_keypair_derand_jazz$37
L__crypto_kem_keypair_derand_jazz$35:
	cmpq	$256, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$36
	movq	$0, %rax
	movq	$768, %rcx
	jmp 	L__crypto_kem_keypair_derand_jazz$33
L__crypto_kem_keypair_derand_jazz$34:
	movw	392(%rsp,%rax,2), %dx
	movw	%dx, 3976(%rsp,%rcx,2)
	incq	%rax
	incq	%rcx
L__crypto_kem_keypair_derand_jazz$33:
	cmpq	$256, %rax
	jb  	L__crypto_kem_keypair_derand_jazz$34
	movb	$0, %al
	leaq	904(%rsp), %rdx
	leaq	64(%rsp), %rcx
	leaq	-240(%rsp), %rsp
	call	L_poly_getnoise_eta1$1
L__crypto_kem_keypair_derand_jazz$32:
	leaq	240(%rsp), %rsp
	movb	$1, %al
	leaq	1416(%rsp), %rdx
	leaq	64(%rsp), %rcx
	leaq	-240(%rsp), %rsp
	call	L_poly_getnoise_eta1$1
L__crypto_kem_keypair_derand_jazz$31:
	leaq	240(%rsp), %rsp
	movb	$2, %al
	leaq	1928(%rsp), %rdx
	leaq	64(%rsp), %rcx
	leaq	-240(%rsp), %rsp
	call	L_poly_getnoise_eta1$1
L__crypto_kem_keypair_derand_jazz$30:
	leaq	240(%rsp), %rsp
	movb	$3, %al
	leaq	2440(%rsp), %rdx
	leaq	64(%rsp), %rcx
	leaq	-240(%rsp), %rsp
	call	L_poly_getnoise_eta1$1
L__crypto_kem_keypair_derand_jazz$29:
	leaq	240(%rsp), %rsp
	leaq	904(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$28:
	leaq	1416(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$27:
	leaq	1928(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$26:
	leaq	2440(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$25:
	leaq	2952(%rsp), %rdi
	leaq	3976(%rsp), %rcx
	leaq	904(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$24:
	leaq	16(%rsp), %rsp
	leaq	392(%rsp), %rdi
	leaq	4488(%rsp), %rcx
	leaq	1416(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$23:
	leaq	16(%rsp), %rsp
	leaq	2952(%rsp), %rcx
	leaq	392(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$22:
	leaq	2952(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_keypair_derand_jazz$20
L__crypto_kem_keypair_derand_jazz$21:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_keypair_derand_jazz$20:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_keypair_derand_jazz$21
	leaq	2952(%rsp), %rax
	call	L_poly_frommont$1
L__crypto_kem_keypair_derand_jazz$19:
	leaq	3464(%rsp), %rdi
	leaq	5000(%rsp), %rcx
	leaq	904(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$18:
	leaq	16(%rsp), %rsp
	leaq	392(%rsp), %rdi
	leaq	5512(%rsp), %rcx
	leaq	1416(%rsp), %rsi
	leaq	-16(%rsp), %rsp
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$17:
	leaq	16(%rsp), %rsp
	leaq	3464(%rsp), %rcx
	leaq	392(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$16:
	leaq	3464(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_keypair_derand_jazz$14
L__crypto_kem_keypair_derand_jazz$15:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_keypair_derand_jazz$14:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_keypair_derand_jazz$15
	leaq	3464(%rsp), %rax
	call	L_poly_frommont$1
L__crypto_kem_keypair_derand_jazz$13:
	leaq	2952(%rsp), %rcx
	leaq	1928(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$12:
	leaq	3464(%rsp), %rcx
	leaq	2440(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$11:
	leaq	2952(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_keypair_derand_jazz$9
L__crypto_kem_keypair_derand_jazz$10:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_keypair_derand_jazz$9:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_keypair_derand_jazz$10
	leaq	3464(%rsp), %rax
	movq	$0, %rcx
	jmp 	L__crypto_kem_keypair_derand_jazz$7
L__crypto_kem_keypair_derand_jazz$8:
	movw	(%rax,%rcx,2), %dx
	movswl	%dx, %esi
	imull	$20159, %esi, %esi
	sarl	$26, %esi
	imull	$3329, %esi, %esi
	subw	%si, %dx
	movw	%dx, (%rax,%rcx,2)
	incq	%rcx
L__crypto_kem_keypair_derand_jazz$7:
	cmpq	$256, %rcx
	jb  	L__crypto_kem_keypair_derand_jazz$8
	movq	32(%rsp), %rax
	movq	40(%rsp), %rcx
	movq	%rcx, %rdx
	leaq	904(%rsp), %rcx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$6:
	addq	$384, %rdx
	leaq	1416(%rsp), %rcx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$5:
	movq	%rax, %rdx
	leaq	2952(%rsp), %rcx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$4:
	addq	$384, %rdx
	leaq	3464(%rsp), %rcx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$3:
	addq	$768, %rax
	movq	96(%rsp), %rcx
	movq	%rcx, (%rax)
	addq	$8, %rax
	movq	104(%rsp), %rcx
	movq	%rcx, (%rax)
	addq	$8, %rax
	movq	112(%rsp), %rcx
	movq	%rcx, (%rax)
	addq	$8, %rax
	movq	120(%rsp), %rcx
	movq	%rcx, (%rax)
	movq	24(%rsp), %rax
	addq	$768, %rax
	movq	16(%rsp), %rcx
	movq	(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	8(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	16(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	24(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	32(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	40(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	48(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	56(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	64(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	72(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	80(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	88(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	96(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	104(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	112(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	120(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	128(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	136(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	144(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	152(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	160(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	168(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	176(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	184(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	192(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	200(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	208(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	216(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	224(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	232(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	240(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	248(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	256(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	264(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	272(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	280(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	288(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	296(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	304(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	312(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	320(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	328(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	336(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	344(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	352(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	360(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	368(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	376(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	384(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	392(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	400(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	408(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	416(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	424(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	432(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	440(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	448(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	456(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	464(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	472(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	480(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	488(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	496(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	504(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	512(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	520(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	528(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	536(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	544(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	552(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	560(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	568(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	576(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	584(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	592(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	600(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	608(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	616(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	624(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	632(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	640(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	648(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	656(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	664(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	672(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	680(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	688(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	696(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	704(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	712(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	720(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	728(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	736(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	744(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	752(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	760(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	768(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	776(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	784(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	792(%rcx), %rcx
	movq	%rcx, (%rax)
	addq	$8, %rax
	movq	%rax, 24(%rsp)
	movq	16(%rsp), %rsi
	movq	$800, %rcx
	leaq	96(%rsp), %rax
	leaq	-232(%rsp), %rsp
	call	L_sha3_256$1
L__crypto_kem_keypair_derand_jazz$2:
	leaq	232(%rsp), %rsp
	movq	24(%rsp), %rax
	movq	8(%rsp), %rcx
	leaq	96(%rsp), %rdx
	movb	(%rdx), %sil
	movb	%sil, (%rax)
	movb	1(%rdx), %sil
	movb	%sil, 1(%rax)
	movb	2(%rdx), %sil
	movb	%sil, 2(%rax)
	movb	3(%rdx), %sil
	movb	%sil, 3(%rax)
	movb	4(%rdx), %sil
	movb	%sil, 4(%rax)
	movb	5(%rdx), %sil
	movb	%sil, 5(%rax)
	movb	6(%rdx), %sil
	movb	%sil, 6(%rax)
	movb	7(%rdx), %sil
	movb	%sil, 7(%rax)
	movb	8(%rdx), %sil
	movb	%sil, 8(%rax)
	movb	9(%rdx), %sil
	movb	%sil, 9(%rax)
	movb	10(%rdx), %sil
	movb	%sil, 10(%rax)
	movb	11(%rdx), %sil
	movb	%sil, 11(%rax)
	movb	12(%rdx), %sil
	movb	%sil, 12(%rax)
	movb	13(%rdx), %sil
	movb	%sil, 13(%rax)
	movb	14(%rdx), %sil
	movb	%sil, 14(%rax)
	movb	15(%rdx), %sil
	movb	%sil, 15(%rax)
	movb	16(%rdx), %sil
	movb	%sil, 16(%rax)
	movb	17(%rdx), %sil
	movb	%sil, 17(%rax)
	movb	18(%rdx), %sil
	movb	%sil, 18(%rax)
	movb	19(%rdx), %sil
	movb	%sil, 19(%rax)
	movb	20(%rdx), %sil
	movb	%sil, 20(%rax)
	movb	21(%rdx), %sil
	movb	%sil, 21(%rax)
	movb	22(%rdx), %sil
	movb	%sil, 22(%rax)
	movb	23(%rdx), %sil
	movb	%sil, 23(%rax)
	movb	24(%rdx), %sil
	movb	%sil, 24(%rax)
	movb	25(%rdx), %sil
	movb	%sil, 25(%rax)
	movb	26(%rdx), %sil
	movb	%sil, 26(%rax)
	movb	27(%rdx), %sil
	movb	%sil, 27(%rax)
	movb	28(%rdx), %sil
	movb	%sil, 28(%rax)
	movb	29(%rdx), %sil
	movb	%sil, 29(%rax)
	movb	30(%rdx), %sil
	movb	%sil, 30(%rax)
	movb	31(%rdx), %dl
	movb	%dl, 31(%rax)
	addq	$32, %rax
	leaq	32(%rcx), %rcx
	movb	(%rcx), %dl
	movb	%dl, (%rax)
	movb	1(%rcx), %dl
	movb	%dl, 1(%rax)
	movb	2(%rcx), %dl
	movb	%dl, 2(%rax)
	movb	3(%rcx), %dl
	movb	%dl, 3(%rax)
	movb	4(%rcx), %dl
	movb	%dl, 4(%rax)
	movb	5(%rcx), %dl
	movb	%dl, 5(%rax)
	movb	6(%rcx), %dl
	movb	%dl, 6(%rax)
	movb	7(%rcx), %dl
	movb	%dl, 7(%rax)
	movb	8(%rcx), %dl
	movb	%dl, 8(%rax)
	movb	9(%rcx), %dl
	movb	%dl, 9(%rax)
	movb	10(%rcx), %dl
	movb	%dl, 10(%rax)
	movb	11(%rcx), %dl
	movb	%dl, 11(%rax)
	movb	12(%rcx), %dl
	movb	%dl, 12(%rax)
	movb	13(%rcx), %dl
	movb	%dl, 13(%rax)
	movb	14(%rcx), %dl
	movb	%dl, 14(%rax)
	movb	15(%rcx), %dl
	movb	%dl, 15(%rax)
	movb	16(%rcx), %dl
	movb	%dl, 16(%rax)
	movb	17(%rcx), %dl
	movb	%dl, 17(%rax)
	movb	18(%rcx), %dl
	movb	%dl, 18(%rax)
	movb	19(%rcx), %dl
	movb	%dl, 19(%rax)
	movb	20(%rcx), %dl
	movb	%dl, 20(%rax)
	movb	21(%rcx), %dl
	movb	%dl, 21(%rax)
	movb	22(%rcx), %dl
	movb	%dl, 22(%rax)
	movb	23(%rcx), %dl
	movb	%dl, 23(%rax)
	movb	24(%rcx), %dl
	movb	%dl, 24(%rax)
	movb	25(%rcx), %dl
	movb	%dl, 25(%rax)
	movb	26(%rcx), %dl
	movb	%dl, 26(%rax)
	movb	27(%rcx), %dl
	movb	%dl, 27(%rax)
	movb	28(%rcx), %dl
	movb	%dl, 28(%rax)
	movb	29(%rcx), %dl
	movb	%dl, 29(%rax)
	movb	30(%rcx), %dl
	movb	%dl, 30(%rax)
	movb	31(%rcx), %cl
	movb	%cl, 31(%rax)
	ret
L_i_poly_tomsg$1:
	call	L_poly_csubq$1
L_i_poly_tomsg$2:
	movb	$0, %sil
	movw	(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	2(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	4(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	6(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	8(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	10(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	12(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	14(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, (%rax)
	movb	$0, %sil
	movw	16(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	18(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	20(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	22(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	24(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	26(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	28(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	30(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 1(%rax)
	movb	$0, %sil
	movw	32(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	34(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	36(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	38(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	40(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	42(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	44(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	46(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 2(%rax)
	movb	$0, %sil
	movw	48(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	50(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	52(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	54(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	56(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	58(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	60(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	62(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 3(%rax)
	movb	$0, %sil
	movw	64(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	66(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	68(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	70(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	72(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	74(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	76(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	78(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 4(%rax)
	movb	$0, %sil
	movw	80(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	82(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	84(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	86(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	88(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	90(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	92(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	94(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 5(%rax)
	movb	$0, %sil
	movw	96(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	98(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	100(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	102(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	104(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	106(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	108(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	110(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 6(%rax)
	movb	$0, %sil
	movw	112(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	114(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	116(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	118(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	120(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	122(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	124(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	126(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 7(%rax)
	movb	$0, %sil
	movw	128(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	130(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	132(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	134(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	136(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	138(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	140(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	142(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 8(%rax)
	movb	$0, %sil
	movw	144(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	146(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	148(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	150(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	152(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	154(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	156(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	158(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 9(%rax)
	movb	$0, %sil
	movw	160(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	162(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	164(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	166(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	168(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	170(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	172(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	174(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 10(%rax)
	movb	$0, %sil
	movw	176(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	178(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	180(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	182(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	184(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	186(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	188(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	190(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 11(%rax)
	movb	$0, %sil
	movw	192(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	194(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	196(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	198(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	200(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	202(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	204(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	206(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 12(%rax)
	movb	$0, %sil
	movw	208(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	210(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	212(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	214(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	216(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	218(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	220(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	222(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 13(%rax)
	movb	$0, %sil
	movw	224(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	226(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	228(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	230(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	232(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	234(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	236(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	238(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 14(%rax)
	movb	$0, %sil
	movw	240(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	242(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	244(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	246(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	248(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	250(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	252(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	254(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 15(%rax)
	movb	$0, %sil
	movw	256(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	258(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	260(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	262(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	264(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	266(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	268(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	270(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 16(%rax)
	movb	$0, %sil
	movw	272(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	274(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	276(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	278(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	280(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	282(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	284(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	286(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 17(%rax)
	movb	$0, %sil
	movw	288(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	290(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	292(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	294(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	296(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	298(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	300(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	302(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 18(%rax)
	movb	$0, %sil
	movw	304(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	306(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	308(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	310(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	312(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	314(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	316(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	318(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 19(%rax)
	movb	$0, %sil
	movw	320(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	322(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	324(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	326(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	328(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	330(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	332(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	334(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 20(%rax)
	movb	$0, %sil
	movw	336(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	338(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	340(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	342(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	344(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	346(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	348(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	350(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 21(%rax)
	movb	$0, %sil
	movw	352(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	354(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	356(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	358(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	360(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	362(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	364(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	366(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 22(%rax)
	movb	$0, %sil
	movw	368(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	370(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	372(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	374(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	376(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	378(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	380(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	382(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 23(%rax)
	movb	$0, %sil
	movw	384(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	386(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	388(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	390(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	392(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	394(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	396(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	398(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 24(%rax)
	movb	$0, %sil
	movw	400(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	402(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	404(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	406(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	408(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	410(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	412(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	414(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 25(%rax)
	movb	$0, %sil
	movw	416(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	418(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	420(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	422(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	424(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	426(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	428(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	430(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 26(%rax)
	movb	$0, %sil
	movw	432(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	434(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	436(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	438(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	440(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	442(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	444(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	446(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 27(%rax)
	movb	$0, %sil
	movw	448(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	450(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	452(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	454(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	456(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	458(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	460(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	462(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 28(%rax)
	movb	$0, %sil
	movw	464(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	466(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	468(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	470(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	472(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	474(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	476(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	478(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 29(%rax)
	movb	$0, %sil
	movw	480(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	482(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	484(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	486(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	488(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	490(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	492(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	494(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$7, %edi
	orb 	%dil, %sil
	movb	%sil, 30(%rax)
	movb	$0, %sil
	movw	496(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$0, %edi
	orb 	%dil, %sil
	movw	498(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$1, %edi
	orb 	%dil, %sil
	movw	500(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$2, %edi
	orb 	%dil, %sil
	movw	502(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$3, %edi
	orb 	%dil, %sil
	movw	504(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$4, %edi
	orb 	%dil, %sil
	movw	506(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$5, %edi
	orb 	%dil, %sil
	movw	508(%rcx), %di
	movzwl	%di, %edi
	shll	$1, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$1, %edi
	shll	$6, %edi
	orb 	%dil, %sil
	movw	510(%rcx), %cx
	movzwl	%cx, %ecx
	shll	$1, %ecx
	addl	$1665, %ecx
	imull	$80635, %ecx, %ecx
	shrl	$28, %ecx
	andl	$1, %ecx
	shll	$7, %ecx
	orb 	%cl, %sil
	movb	%sil, 31(%rax)
	ret
L_poly_tobytes$1:
	call	L_poly_csubq$1
L_poly_tobytes$4:
	movq	$0, %rsi
	movq	$0, %rdi
	jmp 	L_poly_tobytes$2
L_poly_tobytes$3:
	movw	(%rcx,%rsi,2), %r8w
	incq	%rsi
	movw	(%rcx,%rsi,2), %r9w
	incq	%rsi
	movw	%r8w, %r10w
	andw	$255, %r10w
	movb	%r10b, (%rdx,%rdi)
	incq	%rdi
	shrw	$8, %r8w
	movw	%r9w, %r10w
	andw	$15, %r10w
	shlw	$4, %r10w
	orw 	%r8w, %r10w
	movb	%r10b, (%rdx,%rdi)
	incq	%rdi
	shrw	$4, %r9w
	movb	%r9b, (%rdx,%rdi)
	incq	%rdi
L_poly_tobytes$2:
	cmpq	$256, %rsi
	jb  	L_poly_tobytes$3
	ret
L_poly_sub$1:
	movq	$0, %r8
	jmp 	L_poly_sub$2
L_poly_sub$3:
	movw	(%rsi,%r8,2), %r9w
	movw	(%rdi,%r8,2), %r10w
	subw	%r10w, %r9w
	movw	%r9w, (%rcx,%r8,2)
	incq	%r8
L_poly_sub$2:
	cmpq	$256, %r8
	jb  	L_poly_sub$3
	ret
L_poly_ntt$1:
	leaq	glob_data + 448(%rip), %rsi
	movq	$0, %rdi
	movq	$128, %r8
	jmp 	L_poly_ntt$4
L_poly_ntt$5:
	movq	$0, %r11
	jmp 	L_poly_ntt$6
L_poly_ntt$7:
	incq	%rdi
	movw	(%rsi,%rdi,2), %r9w
	movq	%r11, %r10
	leaq	(%r11,%r8), %r11
	jmp 	L_poly_ntt$8
L_poly_ntt$9:
	leaq	(%r10,%r8), %rbx
	movw	(%rcx,%rbx,2), %bp
	movswl	%bp, %ebp
	movswl	%r9w, %r12d
	imull	%r12d, %ebp
	imull	$62209, %ebp, %r12d
	shll	$16, %r12d
	sarl	$16, %r12d
	imull	$3329, %r12d, %r12d
	subl	%r12d, %ebp
	sarl	$16, %ebp
	movw	(%rcx,%r10,2), %r12w
	movw	%r12w, %r13w
	subw	%bp, %r13w
	movw	%r13w, (%rcx,%rbx,2)
	addw	%r12w, %bp
	movw	%bp, (%rcx,%r10,2)
	incq	%r10
L_poly_ntt$8:
	cmpq	%r11, %r10
	jb  	L_poly_ntt$9
	leaq	(%r10,%r8), %r11
L_poly_ntt$6:
	cmpq	$256, %r11
	jb  	L_poly_ntt$7
	shrq	$1, %r8
L_poly_ntt$4:
	cmpq	$2, %r8
	jnb 	L_poly_ntt$5
	movq	$0, %rsi
	jmp 	L_poly_ntt$2
L_poly_ntt$3:
	movw	(%rcx,%rsi,2), %di
	movswl	%di, %r8d
	imull	$20159, %r8d, %r8d
	sarl	$26, %r8d
	imull	$3329, %r8d, %r8d
	subw	%r8w, %di
	movw	%di, (%rcx,%rsi,2)
	incq	%rsi
L_poly_ntt$2:
	cmpq	$256, %rsi
	jb  	L_poly_ntt$3
	ret
L_poly_invntt$1:
	leaq	glob_data + 192(%rip), %rsi
	movq	$0, %rdi
	movq	$2, %r8
	jmp 	L_poly_invntt$4
L_poly_invntt$5:
	movq	$0, %r11
	jmp 	L_poly_invntt$6
L_poly_invntt$7:
	movw	(%rsi,%rdi,2), %r9w
	incq	%rdi
	movq	%r11, %r10
	leaq	(%r11,%r8), %r11
	jmp 	L_poly_invntt$8
L_poly_invntt$9:
	leaq	(%r10,%r8), %rbx
	movw	(%rcx,%rbx,2), %bp
	movw	(%rcx,%r10,2), %r12w
	movw	%bp, %r13w
	addw	%r12w, %r13w
	movswl	%r13w, %r14d
	imull	$20159, %r14d, %r14d
	sarl	$26, %r14d
	imull	$3329, %r14d, %r14d
	subw	%r14w, %r13w
	movw	%r13w, (%rcx,%r10,2)
	subw	%bp, %r12w
	movswl	%r12w, %ebp
	movswl	%r9w, %r12d
	imull	%r12d, %ebp
	imull	$62209, %ebp, %r12d
	shll	$16, %r12d
	sarl	$16, %r12d
	imull	$3329, %r12d, %r12d
	subl	%r12d, %ebp
	sarl	$16, %ebp
	movw	%bp, (%rcx,%rbx,2)
	incq	%r10
L_poly_invntt$8:
	cmpq	%r11, %r10
	jb  	L_poly_invntt$9
	leaq	(%r10,%r8), %r11
L_poly_invntt$6:
	cmpq	$256, %r11
	jb  	L_poly_invntt$7
	shlq	$1, %r8
L_poly_invntt$4:
	cmpq	$128, %r8
	jbe 	L_poly_invntt$5
	movw	254(%rsi), %si
	movq	$0, %rdi
	jmp 	L_poly_invntt$2
L_poly_invntt$3:
	movw	(%rcx,%rdi,2), %r8w
	movswl	%r8w, %r8d
	movswl	%si, %r9d
	imull	%r9d, %r8d
	imull	$62209, %r8d, %r9d
	shll	$16, %r9d
	sarl	$16, %r9d
	imull	$3329, %r9d, %r9d
	subl	%r9d, %r8d
	sarl	$16, %r8d
	movw	%r8w, (%rcx,%rdi,2)
	incq	%rdi
L_poly_invntt$2:
	cmpq	$256, %rdi
	jb  	L_poly_invntt$3
	ret
L_poly_getnoise_eta2$1:
	movq	%rdx, 8(%rsp)
	movb	(%rax), %dl
	movb	%dl, 16(%rsp)
	movb	1(%rax), %dl
	movb	%dl, 17(%rsp)
	movb	2(%rax), %dl
	movb	%dl, 18(%rsp)
	movb	3(%rax), %dl
	movb	%dl, 19(%rsp)
	movb	4(%rax), %dl
	movb	%dl, 20(%rsp)
	movb	5(%rax), %dl
	movb	%dl, 21(%rsp)
	movb	6(%rax), %dl
	movb	%dl, 22(%rsp)
	movb	7(%rax), %dl
	movb	%dl, 23(%rsp)
	movb	8(%rax), %dl
	movb	%dl, 24(%rsp)
	movb	9(%rax), %dl
	movb	%dl, 25(%rsp)
	movb	10(%rax), %dl
	movb	%dl, 26(%rsp)
	movb	11(%rax), %dl
	movb	%dl, 27(%rsp)
	movb	12(%rax), %dl
	movb	%dl, 28(%rsp)
	movb	13(%rax), %dl
	movb	%dl, 29(%rsp)
	movb	14(%rax), %dl
	movb	%dl, 30(%rsp)
	movb	15(%rax), %dl
	movb	%dl, 31(%rsp)
	movb	16(%rax), %dl
	movb	%dl, 32(%rsp)
	movb	17(%rax), %dl
	movb	%dl, 33(%rsp)
	movb	18(%rax), %dl
	movb	%dl, 34(%rsp)
	movb	19(%rax), %dl
	movb	%dl, 35(%rsp)
	movb	20(%rax), %dl
	movb	%dl, 36(%rsp)
	movb	21(%rax), %dl
	movb	%dl, 37(%rsp)
	movb	22(%rax), %dl
	movb	%dl, 38(%rsp)
	movb	23(%rax), %dl
	movb	%dl, 39(%rsp)
	movb	24(%rax), %dl
	movb	%dl, 40(%rsp)
	movb	25(%rax), %dl
	movb	%dl, 41(%rsp)
	movb	26(%rax), %dl
	movb	%dl, 42(%rsp)
	movb	27(%rax), %dl
	movb	%dl, 43(%rsp)
	movb	28(%rax), %dl
	movb	%dl, 44(%rsp)
	movb	29(%rax), %dl
	movb	%dl, 45(%rsp)
	movb	30(%rax), %dl
	movb	%dl, 46(%rsp)
	movb	31(%rax), %al
	movb	%al, 47(%rsp)
	movb	%cl, 48(%rsp)
	leaq	49(%rsp), %rax
	leaq	16(%rsp), %rcx
	leaq	-208(%rsp), %rsp
	call	L_shake256_128_33$1
L_poly_getnoise_eta2$4:
	leaq	208(%rsp), %rsp
	movq	8(%rsp), %rax
	leaq	49(%rsp), %rcx
	movq	$0, %rdx
	movq	$0, %rsi
	jmp 	L_poly_getnoise_eta2$2
L_poly_getnoise_eta2$3:
	movb	(%rcx,%rdx), %dil
	movb	%dil, %r8b
	andb	$85, %r8b
	shrb	$1, %dil
	andb	$85, %dil
	addb	%r8b, %dil
	movb	%dil, %r8b
	andb	$3, %r8b
	movb	%dil, %r9b
	shrb	$2, %r9b
	andb	$3, %r9b
	subb	%r9b, %r8b
	movsbw	%r8b, %r8w
	movw	%r8w, (%rax,%rsi,2)
	movb	%dil, %r8b
	shrb	$4, %r8b
	andb	$3, %r8b
	shrb	$6, %dil
	andb	$3, %dil
	subb	%dil, %r8b
	movsbw	%r8b, %di
	incq	%rsi
	movw	%di, (%rax,%rsi,2)
	incq	%rdx
	incq	%rsi
L_poly_getnoise_eta2$2:
	cmpq	$128, %rdx
	jb  	L_poly_getnoise_eta2$3
	ret
L_poly_getnoise_eta1$1:
	movq	%rdx, 8(%rsp)
	movb	(%rcx), %dl
	movb	%dl, 16(%rsp)
	movb	1(%rcx), %dl
	movb	%dl, 17(%rsp)
	movb	2(%rcx), %dl
	movb	%dl, 18(%rsp)
	movb	3(%rcx), %dl
	movb	%dl, 19(%rsp)
	movb	4(%rcx), %dl
	movb	%dl, 20(%rsp)
	movb	5(%rcx), %dl
	movb	%dl, 21(%rsp)
	movb	6(%rcx), %dl
	movb	%dl, 22(%rsp)
	movb	7(%rcx), %dl
	movb	%dl, 23(%rsp)
	movb	8(%rcx), %dl
	movb	%dl, 24(%rsp)
	movb	9(%rcx), %dl
	movb	%dl, 25(%rsp)
	movb	10(%rcx), %dl
	movb	%dl, 26(%rsp)
	movb	11(%rcx), %dl
	movb	%dl, 27(%rsp)
	movb	12(%rcx), %dl
	movb	%dl, 28(%rsp)
	movb	13(%rcx), %dl
	movb	%dl, 29(%rsp)
	movb	14(%rcx), %dl
	movb	%dl, 30(%rsp)
	movb	15(%rcx), %dl
	movb	%dl, 31(%rsp)
	movb	16(%rcx), %dl
	movb	%dl, 32(%rsp)
	movb	17(%rcx), %dl
	movb	%dl, 33(%rsp)
	movb	18(%rcx), %dl
	movb	%dl, 34(%rsp)
	movb	19(%rcx), %dl
	movb	%dl, 35(%rsp)
	movb	20(%rcx), %dl
	movb	%dl, 36(%rsp)
	movb	21(%rcx), %dl
	movb	%dl, 37(%rsp)
	movb	22(%rcx), %dl
	movb	%dl, 38(%rsp)
	movb	23(%rcx), %dl
	movb	%dl, 39(%rsp)
	movb	24(%rcx), %dl
	movb	%dl, 40(%rsp)
	movb	25(%rcx), %dl
	movb	%dl, 41(%rsp)
	movb	26(%rcx), %dl
	movb	%dl, 42(%rsp)
	movb	27(%rcx), %dl
	movb	%dl, 43(%rsp)
	movb	28(%rcx), %dl
	movb	%dl, 44(%rsp)
	movb	29(%rcx), %dl
	movb	%dl, 45(%rsp)
	movb	30(%rcx), %dl
	movb	%dl, 46(%rsp)
	movb	31(%rcx), %cl
	movb	%cl, 47(%rsp)
	movb	%al, 48(%rsp)
	leaq	49(%rsp), %rax
	leaq	16(%rsp), %rcx
	leaq	-208(%rsp), %rsp
	call	L_shake256_192_33$1
L_poly_getnoise_eta1$4:
	leaq	208(%rsp), %rsp
	movq	8(%rsp), %rax
	leaq	49(%rsp), %rcx
	movq	$0, %rdx
	movq	$0, %rsi
	jmp 	L_poly_getnoise_eta1$2
L_poly_getnoise_eta1$3:
	imulq	$3, %rdx, %r8
	movzbl	(%rcx,%r8), %edi
	movzbl	1(%rcx,%r8), %r9d
	shll	$8, %r9d
	orl 	%r9d, %edi
	movzbl	2(%rcx,%r8), %r8d
	shll	$16, %r8d
	orl 	%r8d, %edi
	movl	%edi, %r8d
	andl	$2396745, %r8d
	shrl	$1, %edi
	movl	%edi, %r9d
	andl	$2396745, %r9d
	addl	%r9d, %r8d
	shrl	$1, %edi
	andl	$2396745, %edi
	addl	%r8d, %edi
	movb	%dil, %r8b
	andb	$7, %r8b
	movb	%dil, %r9b
	shrb	$3, %r9b
	andb	$7, %r9b
	subb	%r9b, %r8b
	movsbw	%r8b, %r8w
	movw	%r8w, (%rax,%rsi,2)
	shrl	$6, %edi
	incq	%rsi
	movb	%dil, %r8b
	andb	$7, %r8b
	movb	%dil, %r9b
	shrb	$3, %r9b
	andb	$7, %r9b
	subb	%r9b, %r8b
	movsbw	%r8b, %r8w
	movw	%r8w, (%rax,%rsi,2)
	shrl	$6, %edi
	incq	%rsi
	movb	%dil, %r8b
	andb	$7, %r8b
	movb	%dil, %r9b
	shrb	$3, %r9b
	andb	$7, %r9b
	subb	%r9b, %r8b
	movsbw	%r8b, %r8w
	movw	%r8w, (%rax,%rsi,2)
	shrl	$6, %edi
	incq	%rsi
	movb	%dil, %r8b
	andb	$7, %r8b
	shrb	$3, %dil
	andb	$7, %dil
	subb	%dil, %r8b
	movsbw	%r8b, %di
	movw	%di, (%rax,%rsi,2)
	incq	%rsi
	incq	%rdx
L_poly_getnoise_eta1$2:
	cmpq	$64, %rdx
	jb  	L_poly_getnoise_eta1$3
	ret
L_i_poly_frommsg$1:
	movb	(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, (%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 2(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 4(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 6(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 8(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 10(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 12(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 14(%rdx)
	movb	1(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 16(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 18(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 20(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 22(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 24(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 26(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 28(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 30(%rdx)
	movb	2(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 32(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 34(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 36(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 38(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 40(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 42(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 44(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 46(%rdx)
	movb	3(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 48(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 50(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 52(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 54(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 56(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 58(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 60(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 62(%rdx)
	movb	4(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 64(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 66(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 68(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 70(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 72(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 74(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 76(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 78(%rdx)
	movb	5(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 80(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 82(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 84(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 86(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 88(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 90(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 92(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 94(%rdx)
	movb	6(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 96(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 98(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 100(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 102(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 104(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 106(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 108(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 110(%rdx)
	movb	7(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 112(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 114(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 116(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 118(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 120(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 122(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 124(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 126(%rdx)
	movb	8(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 128(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 130(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 132(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 134(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 136(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 138(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 140(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 142(%rdx)
	movb	9(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 144(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 146(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 148(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 150(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 152(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 154(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 156(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 158(%rdx)
	movb	10(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 160(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 162(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 164(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 166(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 168(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 170(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 172(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 174(%rdx)
	movb	11(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 176(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 178(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 180(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 182(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 184(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 186(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 188(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 190(%rdx)
	movb	12(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 192(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 194(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 196(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 198(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 200(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 202(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 204(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 206(%rdx)
	movb	13(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 208(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 210(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 212(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 214(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 216(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 218(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 220(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 222(%rdx)
	movb	14(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 224(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 226(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 228(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 230(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 232(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 234(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 236(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 238(%rdx)
	movb	15(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 240(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 242(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 244(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 246(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 248(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 250(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 252(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 254(%rdx)
	movb	16(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 256(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 258(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 260(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 262(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 264(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 266(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 268(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 270(%rdx)
	movb	17(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 272(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 274(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 276(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 278(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 280(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 282(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 284(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 286(%rdx)
	movb	18(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 288(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 290(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 292(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 294(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 296(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 298(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 300(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 302(%rdx)
	movb	19(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 304(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 306(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 308(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 310(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 312(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 314(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 316(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 318(%rdx)
	movb	20(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 320(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 322(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 324(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 326(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 328(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 330(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 332(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 334(%rdx)
	movb	21(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 336(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 338(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 340(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 342(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 344(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 346(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 348(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 350(%rdx)
	movb	22(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 352(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 354(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 356(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 358(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 360(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 362(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 364(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 366(%rdx)
	movb	23(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 368(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 370(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 372(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 374(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 376(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 378(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 380(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 382(%rdx)
	movb	24(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 384(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 386(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 388(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 390(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 392(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 394(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 396(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 398(%rdx)
	movb	25(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 400(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 402(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 404(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 406(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 408(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 410(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 412(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 414(%rdx)
	movb	26(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 416(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 418(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 420(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 422(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 424(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 426(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 428(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 430(%rdx)
	movb	27(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 432(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 434(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 436(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 438(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 440(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 442(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 444(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 446(%rdx)
	movb	28(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 448(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 450(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 452(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 454(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 456(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 458(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 460(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 462(%rdx)
	movb	29(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 464(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 466(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 468(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 470(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 472(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 474(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 476(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 478(%rdx)
	movb	30(%rcx), %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 480(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 482(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 484(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 486(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 488(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 490(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %di
	andw	$1, %di
	imulw	$1665, %di, %di
	movw	%di, 492(%rdx)
	shrb	$1, %sil
	movzbw	%sil, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 494(%rdx)
	movb	31(%rcx), %cl
	movzbw	%cl, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 496(%rdx)
	shrb	$1, %cl
	movzbw	%cl, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 498(%rdx)
	shrb	$1, %cl
	movzbw	%cl, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 500(%rdx)
	shrb	$1, %cl
	movzbw	%cl, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 502(%rdx)
	shrb	$1, %cl
	movzbw	%cl, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 504(%rdx)
	shrb	$1, %cl
	movzbw	%cl, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 506(%rdx)
	shrb	$1, %cl
	movzbw	%cl, %si
	andw	$1, %si
	imulw	$1665, %si, %si
	movw	%si, 508(%rdx)
	shrb	$1, %cl
	movzbw	%cl, %cx
	andw	$1, %cx
	imulw	$1665, %cx, %cx
	movw	%cx, 510(%rdx)
	ret
L_poly_frommont$1:
	movw	$1353, %cx
	movq	$0, %rdx
	jmp 	L_poly_frommont$2
L_poly_frommont$3:
	movw	(%rax,%rdx,2), %si
	movswl	%si, %esi
	movswl	%cx, %edi
	imull	%edi, %esi
	imull	$62209, %esi, %edi
	shll	$16, %edi
	sarl	$16, %edi
	imull	$3329, %edi, %edi
	subl	%edi, %esi
	sarl	$16, %esi
	movw	%si, (%rax,%rdx,2)
	incq	%rdx
L_poly_frommont$2:
	cmpq	$256, %rdx
	jb  	L_poly_frommont$3
	ret
L_poly_frombytes$1:
	movb	(%rsi), %r8b
	movb	1(%rsi), %r9b
	movb	2(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, (%rdi)
	movw	%r10w, 2(%rdi)
	movb	3(%rsi), %r8b
	movb	4(%rsi), %r9b
	movb	5(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 4(%rdi)
	movw	%r10w, 6(%rdi)
	movb	6(%rsi), %r8b
	movb	7(%rsi), %r9b
	movb	8(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 8(%rdi)
	movw	%r10w, 10(%rdi)
	movb	9(%rsi), %r8b
	movb	10(%rsi), %r9b
	movb	11(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 12(%rdi)
	movw	%r10w, 14(%rdi)
	movb	12(%rsi), %r8b
	movb	13(%rsi), %r9b
	movb	14(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 16(%rdi)
	movw	%r10w, 18(%rdi)
	movb	15(%rsi), %r8b
	movb	16(%rsi), %r9b
	movb	17(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 20(%rdi)
	movw	%r10w, 22(%rdi)
	movb	18(%rsi), %r8b
	movb	19(%rsi), %r9b
	movb	20(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 24(%rdi)
	movw	%r10w, 26(%rdi)
	movb	21(%rsi), %r8b
	movb	22(%rsi), %r9b
	movb	23(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 28(%rdi)
	movw	%r10w, 30(%rdi)
	movb	24(%rsi), %r8b
	movb	25(%rsi), %r9b
	movb	26(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 32(%rdi)
	movw	%r10w, 34(%rdi)
	movb	27(%rsi), %r8b
	movb	28(%rsi), %r9b
	movb	29(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 36(%rdi)
	movw	%r10w, 38(%rdi)
	movb	30(%rsi), %r8b
	movb	31(%rsi), %r9b
	movb	32(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 40(%rdi)
	movw	%r10w, 42(%rdi)
	movb	33(%rsi), %r8b
	movb	34(%rsi), %r9b
	movb	35(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 44(%rdi)
	movw	%r10w, 46(%rdi)
	movb	36(%rsi), %r8b
	movb	37(%rsi), %r9b
	movb	38(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 48(%rdi)
	movw	%r10w, 50(%rdi)
	movb	39(%rsi), %r8b
	movb	40(%rsi), %r9b
	movb	41(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 52(%rdi)
	movw	%r10w, 54(%rdi)
	movb	42(%rsi), %r8b
	movb	43(%rsi), %r9b
	movb	44(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 56(%rdi)
	movw	%r10w, 58(%rdi)
	movb	45(%rsi), %r8b
	movb	46(%rsi), %r9b
	movb	47(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 60(%rdi)
	movw	%r10w, 62(%rdi)
	movb	48(%rsi), %r8b
	movb	49(%rsi), %r9b
	movb	50(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 64(%rdi)
	movw	%r10w, 66(%rdi)
	movb	51(%rsi), %r8b
	movb	52(%rsi), %r9b
	movb	53(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 68(%rdi)
	movw	%r10w, 70(%rdi)
	movb	54(%rsi), %r8b
	movb	55(%rsi), %r9b
	movb	56(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 72(%rdi)
	movw	%r10w, 74(%rdi)
	movb	57(%rsi), %r8b
	movb	58(%rsi), %r9b
	movb	59(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 76(%rdi)
	movw	%r10w, 78(%rdi)
	movb	60(%rsi), %r8b
	movb	61(%rsi), %r9b
	movb	62(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 80(%rdi)
	movw	%r10w, 82(%rdi)
	movb	63(%rsi), %r8b
	movb	64(%rsi), %r9b
	movb	65(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 84(%rdi)
	movw	%r10w, 86(%rdi)
	movb	66(%rsi), %r8b
	movb	67(%rsi), %r9b
	movb	68(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 88(%rdi)
	movw	%r10w, 90(%rdi)
	movb	69(%rsi), %r8b
	movb	70(%rsi), %r9b
	movb	71(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 92(%rdi)
	movw	%r10w, 94(%rdi)
	movb	72(%rsi), %r8b
	movb	73(%rsi), %r9b
	movb	74(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 96(%rdi)
	movw	%r10w, 98(%rdi)
	movb	75(%rsi), %r8b
	movb	76(%rsi), %r9b
	movb	77(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 100(%rdi)
	movw	%r10w, 102(%rdi)
	movb	78(%rsi), %r8b
	movb	79(%rsi), %r9b
	movb	80(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 104(%rdi)
	movw	%r10w, 106(%rdi)
	movb	81(%rsi), %r8b
	movb	82(%rsi), %r9b
	movb	83(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 108(%rdi)
	movw	%r10w, 110(%rdi)
	movb	84(%rsi), %r8b
	movb	85(%rsi), %r9b
	movb	86(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 112(%rdi)
	movw	%r10w, 114(%rdi)
	movb	87(%rsi), %r8b
	movb	88(%rsi), %r9b
	movb	89(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 116(%rdi)
	movw	%r10w, 118(%rdi)
	movb	90(%rsi), %r8b
	movb	91(%rsi), %r9b
	movb	92(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 120(%rdi)
	movw	%r10w, 122(%rdi)
	movb	93(%rsi), %r8b
	movb	94(%rsi), %r9b
	movb	95(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 124(%rdi)
	movw	%r10w, 126(%rdi)
	movb	96(%rsi), %r8b
	movb	97(%rsi), %r9b
	movb	98(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 128(%rdi)
	movw	%r10w, 130(%rdi)
	movb	99(%rsi), %r8b
	movb	100(%rsi), %r9b
	movb	101(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 132(%rdi)
	movw	%r10w, 134(%rdi)
	movb	102(%rsi), %r8b
	movb	103(%rsi), %r9b
	movb	104(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 136(%rdi)
	movw	%r10w, 138(%rdi)
	movb	105(%rsi), %r8b
	movb	106(%rsi), %r9b
	movb	107(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 140(%rdi)
	movw	%r10w, 142(%rdi)
	movb	108(%rsi), %r8b
	movb	109(%rsi), %r9b
	movb	110(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 144(%rdi)
	movw	%r10w, 146(%rdi)
	movb	111(%rsi), %r8b
	movb	112(%rsi), %r9b
	movb	113(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 148(%rdi)
	movw	%r10w, 150(%rdi)
	movb	114(%rsi), %r8b
	movb	115(%rsi), %r9b
	movb	116(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 152(%rdi)
	movw	%r10w, 154(%rdi)
	movb	117(%rsi), %r8b
	movb	118(%rsi), %r9b
	movb	119(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 156(%rdi)
	movw	%r10w, 158(%rdi)
	movb	120(%rsi), %r8b
	movb	121(%rsi), %r9b
	movb	122(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 160(%rdi)
	movw	%r10w, 162(%rdi)
	movb	123(%rsi), %r8b
	movb	124(%rsi), %r9b
	movb	125(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 164(%rdi)
	movw	%r10w, 166(%rdi)
	movb	126(%rsi), %r8b
	movb	127(%rsi), %r9b
	movb	128(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 168(%rdi)
	movw	%r10w, 170(%rdi)
	movb	129(%rsi), %r8b
	movb	130(%rsi), %r9b
	movb	131(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 172(%rdi)
	movw	%r10w, 174(%rdi)
	movb	132(%rsi), %r8b
	movb	133(%rsi), %r9b
	movb	134(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 176(%rdi)
	movw	%r10w, 178(%rdi)
	movb	135(%rsi), %r8b
	movb	136(%rsi), %r9b
	movb	137(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 180(%rdi)
	movw	%r10w, 182(%rdi)
	movb	138(%rsi), %r8b
	movb	139(%rsi), %r9b
	movb	140(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 184(%rdi)
	movw	%r10w, 186(%rdi)
	movb	141(%rsi), %r8b
	movb	142(%rsi), %r9b
	movb	143(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 188(%rdi)
	movw	%r10w, 190(%rdi)
	movb	144(%rsi), %r8b
	movb	145(%rsi), %r9b
	movb	146(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 192(%rdi)
	movw	%r10w, 194(%rdi)
	movb	147(%rsi), %r8b
	movb	148(%rsi), %r9b
	movb	149(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 196(%rdi)
	movw	%r10w, 198(%rdi)
	movb	150(%rsi), %r8b
	movb	151(%rsi), %r9b
	movb	152(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 200(%rdi)
	movw	%r10w, 202(%rdi)
	movb	153(%rsi), %r8b
	movb	154(%rsi), %r9b
	movb	155(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 204(%rdi)
	movw	%r10w, 206(%rdi)
	movb	156(%rsi), %r8b
	movb	157(%rsi), %r9b
	movb	158(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 208(%rdi)
	movw	%r10w, 210(%rdi)
	movb	159(%rsi), %r8b
	movb	160(%rsi), %r9b
	movb	161(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 212(%rdi)
	movw	%r10w, 214(%rdi)
	movb	162(%rsi), %r8b
	movb	163(%rsi), %r9b
	movb	164(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 216(%rdi)
	movw	%r10w, 218(%rdi)
	movb	165(%rsi), %r8b
	movb	166(%rsi), %r9b
	movb	167(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 220(%rdi)
	movw	%r10w, 222(%rdi)
	movb	168(%rsi), %r8b
	movb	169(%rsi), %r9b
	movb	170(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 224(%rdi)
	movw	%r10w, 226(%rdi)
	movb	171(%rsi), %r8b
	movb	172(%rsi), %r9b
	movb	173(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 228(%rdi)
	movw	%r10w, 230(%rdi)
	movb	174(%rsi), %r8b
	movb	175(%rsi), %r9b
	movb	176(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 232(%rdi)
	movw	%r10w, 234(%rdi)
	movb	177(%rsi), %r8b
	movb	178(%rsi), %r9b
	movb	179(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 236(%rdi)
	movw	%r10w, 238(%rdi)
	movb	180(%rsi), %r8b
	movb	181(%rsi), %r9b
	movb	182(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 240(%rdi)
	movw	%r10w, 242(%rdi)
	movb	183(%rsi), %r8b
	movb	184(%rsi), %r9b
	movb	185(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 244(%rdi)
	movw	%r10w, 246(%rdi)
	movb	186(%rsi), %r8b
	movb	187(%rsi), %r9b
	movb	188(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 248(%rdi)
	movw	%r10w, 250(%rdi)
	movb	189(%rsi), %r8b
	movb	190(%rsi), %r9b
	movb	191(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 252(%rdi)
	movw	%r10w, 254(%rdi)
	movb	192(%rsi), %r8b
	movb	193(%rsi), %r9b
	movb	194(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 256(%rdi)
	movw	%r10w, 258(%rdi)
	movb	195(%rsi), %r8b
	movb	196(%rsi), %r9b
	movb	197(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 260(%rdi)
	movw	%r10w, 262(%rdi)
	movb	198(%rsi), %r8b
	movb	199(%rsi), %r9b
	movb	200(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 264(%rdi)
	movw	%r10w, 266(%rdi)
	movb	201(%rsi), %r8b
	movb	202(%rsi), %r9b
	movb	203(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 268(%rdi)
	movw	%r10w, 270(%rdi)
	movb	204(%rsi), %r8b
	movb	205(%rsi), %r9b
	movb	206(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 272(%rdi)
	movw	%r10w, 274(%rdi)
	movb	207(%rsi), %r8b
	movb	208(%rsi), %r9b
	movb	209(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 276(%rdi)
	movw	%r10w, 278(%rdi)
	movb	210(%rsi), %r8b
	movb	211(%rsi), %r9b
	movb	212(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 280(%rdi)
	movw	%r10w, 282(%rdi)
	movb	213(%rsi), %r8b
	movb	214(%rsi), %r9b
	movb	215(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 284(%rdi)
	movw	%r10w, 286(%rdi)
	movb	216(%rsi), %r8b
	movb	217(%rsi), %r9b
	movb	218(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 288(%rdi)
	movw	%r10w, 290(%rdi)
	movb	219(%rsi), %r8b
	movb	220(%rsi), %r9b
	movb	221(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 292(%rdi)
	movw	%r10w, 294(%rdi)
	movb	222(%rsi), %r8b
	movb	223(%rsi), %r9b
	movb	224(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 296(%rdi)
	movw	%r10w, 298(%rdi)
	movb	225(%rsi), %r8b
	movb	226(%rsi), %r9b
	movb	227(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 300(%rdi)
	movw	%r10w, 302(%rdi)
	movb	228(%rsi), %r8b
	movb	229(%rsi), %r9b
	movb	230(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 304(%rdi)
	movw	%r10w, 306(%rdi)
	movb	231(%rsi), %r8b
	movb	232(%rsi), %r9b
	movb	233(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 308(%rdi)
	movw	%r10w, 310(%rdi)
	movb	234(%rsi), %r8b
	movb	235(%rsi), %r9b
	movb	236(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 312(%rdi)
	movw	%r10w, 314(%rdi)
	movb	237(%rsi), %r8b
	movb	238(%rsi), %r9b
	movb	239(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 316(%rdi)
	movw	%r10w, 318(%rdi)
	movb	240(%rsi), %r8b
	movb	241(%rsi), %r9b
	movb	242(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 320(%rdi)
	movw	%r10w, 322(%rdi)
	movb	243(%rsi), %r8b
	movb	244(%rsi), %r9b
	movb	245(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 324(%rdi)
	movw	%r10w, 326(%rdi)
	movb	246(%rsi), %r8b
	movb	247(%rsi), %r9b
	movb	248(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 328(%rdi)
	movw	%r10w, 330(%rdi)
	movb	249(%rsi), %r8b
	movb	250(%rsi), %r9b
	movb	251(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 332(%rdi)
	movw	%r10w, 334(%rdi)
	movb	252(%rsi), %r8b
	movb	253(%rsi), %r9b
	movb	254(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 336(%rdi)
	movw	%r10w, 338(%rdi)
	movb	255(%rsi), %r8b
	movb	256(%rsi), %r9b
	movb	257(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 340(%rdi)
	movw	%r10w, 342(%rdi)
	movb	258(%rsi), %r8b
	movb	259(%rsi), %r9b
	movb	260(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 344(%rdi)
	movw	%r10w, 346(%rdi)
	movb	261(%rsi), %r8b
	movb	262(%rsi), %r9b
	movb	263(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 348(%rdi)
	movw	%r10w, 350(%rdi)
	movb	264(%rsi), %r8b
	movb	265(%rsi), %r9b
	movb	266(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 352(%rdi)
	movw	%r10w, 354(%rdi)
	movb	267(%rsi), %r8b
	movb	268(%rsi), %r9b
	movb	269(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 356(%rdi)
	movw	%r10w, 358(%rdi)
	movb	270(%rsi), %r8b
	movb	271(%rsi), %r9b
	movb	272(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 360(%rdi)
	movw	%r10w, 362(%rdi)
	movb	273(%rsi), %r8b
	movb	274(%rsi), %r9b
	movb	275(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 364(%rdi)
	movw	%r10w, 366(%rdi)
	movb	276(%rsi), %r8b
	movb	277(%rsi), %r9b
	movb	278(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 368(%rdi)
	movw	%r10w, 370(%rdi)
	movb	279(%rsi), %r8b
	movb	280(%rsi), %r9b
	movb	281(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 372(%rdi)
	movw	%r10w, 374(%rdi)
	movb	282(%rsi), %r8b
	movb	283(%rsi), %r9b
	movb	284(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 376(%rdi)
	movw	%r10w, 378(%rdi)
	movb	285(%rsi), %r8b
	movb	286(%rsi), %r9b
	movb	287(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 380(%rdi)
	movw	%r10w, 382(%rdi)
	movb	288(%rsi), %r8b
	movb	289(%rsi), %r9b
	movb	290(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 384(%rdi)
	movw	%r10w, 386(%rdi)
	movb	291(%rsi), %r8b
	movb	292(%rsi), %r9b
	movb	293(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 388(%rdi)
	movw	%r10w, 390(%rdi)
	movb	294(%rsi), %r8b
	movb	295(%rsi), %r9b
	movb	296(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 392(%rdi)
	movw	%r10w, 394(%rdi)
	movb	297(%rsi), %r8b
	movb	298(%rsi), %r9b
	movb	299(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 396(%rdi)
	movw	%r10w, 398(%rdi)
	movb	300(%rsi), %r8b
	movb	301(%rsi), %r9b
	movb	302(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 400(%rdi)
	movw	%r10w, 402(%rdi)
	movb	303(%rsi), %r8b
	movb	304(%rsi), %r9b
	movb	305(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 404(%rdi)
	movw	%r10w, 406(%rdi)
	movb	306(%rsi), %r8b
	movb	307(%rsi), %r9b
	movb	308(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 408(%rdi)
	movw	%r10w, 410(%rdi)
	movb	309(%rsi), %r8b
	movb	310(%rsi), %r9b
	movb	311(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 412(%rdi)
	movw	%r10w, 414(%rdi)
	movb	312(%rsi), %r8b
	movb	313(%rsi), %r9b
	movb	314(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 416(%rdi)
	movw	%r10w, 418(%rdi)
	movb	315(%rsi), %r8b
	movb	316(%rsi), %r9b
	movb	317(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 420(%rdi)
	movw	%r10w, 422(%rdi)
	movb	318(%rsi), %r8b
	movb	319(%rsi), %r9b
	movb	320(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 424(%rdi)
	movw	%r10w, 426(%rdi)
	movb	321(%rsi), %r8b
	movb	322(%rsi), %r9b
	movb	323(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 428(%rdi)
	movw	%r10w, 430(%rdi)
	movb	324(%rsi), %r8b
	movb	325(%rsi), %r9b
	movb	326(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 432(%rdi)
	movw	%r10w, 434(%rdi)
	movb	327(%rsi), %r8b
	movb	328(%rsi), %r9b
	movb	329(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 436(%rdi)
	movw	%r10w, 438(%rdi)
	movb	330(%rsi), %r8b
	movb	331(%rsi), %r9b
	movb	332(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 440(%rdi)
	movw	%r10w, 442(%rdi)
	movb	333(%rsi), %r8b
	movb	334(%rsi), %r9b
	movb	335(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 444(%rdi)
	movw	%r10w, 446(%rdi)
	movb	336(%rsi), %r8b
	movb	337(%rsi), %r9b
	movb	338(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 448(%rdi)
	movw	%r10w, 450(%rdi)
	movb	339(%rsi), %r8b
	movb	340(%rsi), %r9b
	movb	341(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 452(%rdi)
	movw	%r10w, 454(%rdi)
	movb	342(%rsi), %r8b
	movb	343(%rsi), %r9b
	movb	344(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 456(%rdi)
	movw	%r10w, 458(%rdi)
	movb	345(%rsi), %r8b
	movb	346(%rsi), %r9b
	movb	347(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 460(%rdi)
	movw	%r10w, 462(%rdi)
	movb	348(%rsi), %r8b
	movb	349(%rsi), %r9b
	movb	350(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 464(%rdi)
	movw	%r10w, 466(%rdi)
	movb	351(%rsi), %r8b
	movb	352(%rsi), %r9b
	movb	353(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 468(%rdi)
	movw	%r10w, 470(%rdi)
	movb	354(%rsi), %r8b
	movb	355(%rsi), %r9b
	movb	356(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 472(%rdi)
	movw	%r10w, 474(%rdi)
	movb	357(%rsi), %r8b
	movb	358(%rsi), %r9b
	movb	359(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 476(%rdi)
	movw	%r10w, 478(%rdi)
	movb	360(%rsi), %r8b
	movb	361(%rsi), %r9b
	movb	362(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 480(%rdi)
	movw	%r10w, 482(%rdi)
	movb	363(%rsi), %r8b
	movb	364(%rsi), %r9b
	movb	365(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 484(%rdi)
	movw	%r10w, 486(%rdi)
	movb	366(%rsi), %r8b
	movb	367(%rsi), %r9b
	movb	368(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 488(%rdi)
	movw	%r10w, 490(%rdi)
	movb	369(%rsi), %r8b
	movb	370(%rsi), %r9b
	movb	371(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 492(%rdi)
	movw	%r10w, 494(%rdi)
	movb	372(%rsi), %r8b
	movb	373(%rsi), %r9b
	movb	374(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 496(%rdi)
	movw	%r10w, 498(%rdi)
	movb	375(%rsi), %r8b
	movb	376(%rsi), %r9b
	movb	377(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 500(%rdi)
	movw	%r10w, 502(%rdi)
	movb	378(%rsi), %r8b
	movb	379(%rsi), %r9b
	movb	380(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 504(%rdi)
	movw	%r10w, 506(%rdi)
	movb	381(%rsi), %r8b
	movb	382(%rsi), %r9b
	movb	383(%rsi), %r10b
	movzbw	%r8b, %r8w
	movzbw	%r9b, %r11w
	andw	$15, %r11w
	shlw	$8, %r11w
	orw 	%r11w, %r8w
	movzbw	%r10b, %r10w
	shlw	$4, %r10w
	movzbw	%r9b, %r9w
	shrw	$4, %r9w
	orw 	%r9w, %r10w
	movw	%r8w, 508(%rdi)
	movw	%r10w, 510(%rdi)
	ret
L_poly_decompress$1:
	movq	$0, %rdi
	movq	$0, %r8
	jmp 	L_poly_decompress$2
L_poly_decompress$3:
	movb	(%rsi,%rdi), %r9b
	movzbw	%r9b, %r10w
	movzbw	%r9b, %r9w
	andw	$15, %r10w
	shrw	$4, %r9w
	imulw	$3329, %r10w, %r10w
	imulw	$3329, %r9w, %r9w
	addw	$8, %r10w
	addw	$8, %r9w
	shrw	$4, %r10w
	shrw	$4, %r9w
	movw	%r10w, (%rcx,%r8,2)
	incq	%r8
	movw	%r9w, (%rcx,%r8,2)
	incq	%r8
	incq	%rdi
L_poly_decompress$2:
	cmpq	$128, %rdi
	jb  	L_poly_decompress$3
	ret
L_i_poly_compress$1:
	call	L_poly_csubq$1
L_i_poly_compress$4:
	movq	$0, %rdx
	movq	$0, %rsi
	jmp 	L_i_poly_compress$2
L_i_poly_compress$3:
	movw	(%rcx,%rsi,2), %di
	movzwl	%di, %edi
	shll	$4, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$15, %edi
	incq	%rsi
	movw	(%rcx,%rsi,2), %r8w
	movzwl	%r8w, %r8d
	shll	$4, %r8d
	addl	$1665, %r8d
	imull	$80635, %r8d, %r8d
	shrl	$28, %r8d
	andl	$15, %r8d
	shll	$4, %r8d
	orl 	%r8d, %edi
	movb	%dil, (%rax,%rdx)
	incq	%rdx
	incq	%rsi
L_i_poly_compress$2:
	cmpq	$128, %rdx
	jb  	L_i_poly_compress$3
	ret
L_poly_compress$1:
	call	L_poly_csubq$1
L_poly_compress$4:
	movq	$0, %rdx
	movq	$0, %rsi
	jmp 	L_poly_compress$2
L_poly_compress$3:
	movw	(%rcx,%rsi,2), %di
	movzwl	%di, %edi
	shll	$4, %edi
	addl	$1665, %edi
	imull	$80635, %edi, %edi
	shrl	$28, %edi
	andl	$15, %edi
	incq	%rsi
	movw	(%rcx,%rsi,2), %r8w
	movzwl	%r8w, %r8d
	shll	$4, %r8d
	addl	$1665, %r8d
	imull	$80635, %r8d, %r8d
	shrl	$28, %r8d
	andl	$15, %r8d
	shll	$4, %r8d
	orl 	%r8d, %edi
	movb	%dil, (%rax,%rdx)
	incq	%rdx
	incq	%rsi
L_poly_compress$2:
	cmpq	$128, %rdx
	jb  	L_poly_compress$3
	ret
L_poly_basemul$1:
	movq	%rdi, 8(%rsp)
	movq	$64, %rdi
	movq	$0, %r8
	jmp 	L_poly_basemul$2
L_poly_basemul$3:
	leaq	glob_data + 448(%rip), %r9
	movw	(%r9,%rdi,2), %r9w
	incq	%rdi
	movw	(%rcx,%r8,2), %r10w
	movw	(%rsi,%r8,2), %r11w
	incq	%r8
	movw	(%rcx,%r8,2), %bx
	movw	(%rsi,%r8,2), %bp
	addq	$-1, %r8
	movswl	%bx, %r12d
	movswl	%bp, %r13d
	imull	%r13d, %r12d
	imull	$62209, %r12d, %r13d
	shll	$16, %r13d
	sarl	$16, %r13d
	imull	$3329, %r13d, %r13d
	subl	%r13d, %r12d
	sarl	$16, %r12d
	movswl	%r12w, %r12d
	movswl	%r9w, %r13d
	imull	%r13d, %r12d
	imull	$62209, %r12d, %r13d
	shll	$16, %r13d
	sarl	$16, %r13d
	imull	$3329, %r13d, %r13d
	subl	%r13d, %r12d
	sarl	$16, %r12d
	movswl	%r10w, %r13d
	movswl	%r11w, %r14d
	imull	%r14d, %r13d
	imull	$62209, %r13d, %r14d
	shll	$16, %r14d
	sarl	$16, %r14d
	imull	$3329, %r14d, %r14d
	subl	%r14d, %r13d
	sarl	$16, %r13d
	addw	%r13w, %r12w
	movswl	%r10w, %r10d
	movswl	%bp, %ebp
	imull	%ebp, %r10d
	imull	$62209, %r10d, %ebp
	shll	$16, %ebp
	sarl	$16, %ebp
	imull	$3329, %ebp, %ebp
	subl	%ebp, %r10d
	sarl	$16, %r10d
	movswl	%bx, %ebx
	movswl	%r11w, %r11d
	imull	%r11d, %ebx
	imull	$62209, %ebx, %r11d
	shll	$16, %r11d
	sarl	$16, %r11d
	imull	$3329, %r11d, %r11d
	subl	%r11d, %ebx
	sarl	$16, %ebx
	addw	%bx, %r10w
	movq	8(%rsp), %r11
	movw	%r12w, (%r11,%r8,2)
	incq	%r8
	movw	%r10w, (%r11,%r8,2)
	movq	%r11, 16(%rsp)
	negw	%r9w
	incq	%r8
	movw	(%rcx,%r8,2), %r10w
	movw	(%rsi,%r8,2), %r11w
	incq	%r8
	movw	(%rcx,%r8,2), %bx
	movw	(%rsi,%r8,2), %bp
	addq	$-1, %r8
	movswl	%bx, %r12d
	movswl	%bp, %r13d
	imull	%r13d, %r12d
	imull	$62209, %r12d, %r13d
	shll	$16, %r13d
	sarl	$16, %r13d
	imull	$3329, %r13d, %r13d
	subl	%r13d, %r12d
	sarl	$16, %r12d
	movswl	%r12w, %r12d
	movswl	%r9w, %r9d
	imull	%r9d, %r12d
	imull	$62209, %r12d, %r9d
	shll	$16, %r9d
	sarl	$16, %r9d
	imull	$3329, %r9d, %r9d
	subl	%r9d, %r12d
	sarl	$16, %r12d
	movw	%r12w, %r9w
	movswl	%r10w, %r12d
	movswl	%r11w, %r13d
	imull	%r13d, %r12d
	imull	$62209, %r12d, %r13d
	shll	$16, %r13d
	sarl	$16, %r13d
	imull	$3329, %r13d, %r13d
	subl	%r13d, %r12d
	sarl	$16, %r12d
	addw	%r12w, %r9w
	movswl	%r10w, %r10d
	movswl	%bp, %ebp
	imull	%ebp, %r10d
	imull	$62209, %r10d, %ebp
	shll	$16, %ebp
	sarl	$16, %ebp
	imull	$3329, %ebp, %ebp
	subl	%ebp, %r10d
	sarl	$16, %r10d
	movswl	%bx, %ebx
	movswl	%r11w, %r11d
	imull	%r11d, %ebx
	imull	$62209, %ebx, %r11d
	shll	$16, %r11d
	sarl	$16, %r11d
	imull	$3329, %r11d, %r11d
	subl	%r11d, %ebx
	sarl	$16, %ebx
	addw	%bx, %r10w
	movq	16(%rsp), %r11
	movw	%r9w, (%r11,%r8,2)
	incq	%r8
	movw	%r10w, (%r11,%r8,2)
	incq	%r8
L_poly_basemul$2:
	cmpq	$256, %r8
	jb  	L_poly_basemul$3
	ret
L_poly_csubq$1:
	movq	$0, %r8
	jmp 	L_poly_csubq$2
L_poly_csubq$3:
	movw	(%rcx,%r8,2), %r9w
	addw	$-3329, %r9w
	movw	%r9w, %r10w
	sarw	$15, %r10w
	andw	$3329, %r10w
	addw	%r10w, %r9w
	movw	%r9w, (%rcx,%r8,2)
	incq	%r8
L_poly_csubq$2:
	cmpq	$256, %r8
	jb  	L_poly_csubq$3
	ret
L_poly_add2$1:
	movq	$0, %rdi
	jmp 	L_poly_add2$2
L_poly_add2$3:
	movw	(%rcx,%rdi,2), %r8w
	movw	(%rsi,%rdi,2), %r9w
	addw	%r9w, %r8w
	movw	%r8w, (%rcx,%rdi,2)
	incq	%rdi
L_poly_add2$2:
	cmpq	$256, %rdi
	jb  	L_poly_add2$3
	ret
L_shake256_64$1:
	movq	%rax, 8(%rsp)
	movq	%rcx, 16(%rsp)
	leaq	24(%rsp), %rax
	xorq	%rcx, %rcx
	movq	%rcx, (%rax)
	movq	%rcx, 8(%rax)
	movq	%rcx, 16(%rax)
	movq	%rcx, 24(%rax)
	movq	%rcx, 32(%rax)
	movq	%rcx, 40(%rax)
	movq	%rcx, 48(%rax)
	movq	%rcx, 56(%rax)
	movq	%rcx, 64(%rax)
	movq	%rcx, 72(%rax)
	movq	%rcx, 80(%rax)
	movq	%rcx, 88(%rax)
	movq	%rcx, 96(%rax)
	movq	%rcx, 104(%rax)
	movq	%rcx, 112(%rax)
	movq	%rcx, 120(%rax)
	movq	%rcx, 128(%rax)
	movq	%rcx, 136(%rax)
	movq	%rcx, 144(%rax)
	movq	%rcx, 152(%rax)
	movq	%rcx, 160(%rax)
	movq	%rcx, 168(%rax)
	movq	%rcx, 176(%rax)
	movq	%rcx, 184(%rax)
	movq	%rcx, 192(%rax)
	movq	(%rdx), %rcx
	xorq	%rcx, (%rax)
	movq	8(%rdx), %rcx
	xorq	%rcx, 8(%rax)
	movq	16(%rdx), %rcx
	xorq	%rcx, 16(%rax)
	movq	24(%rdx), %rcx
	xorq	%rcx, 24(%rax)
	movq	32(%rdx), %rcx
	xorq	%rcx, 32(%rax)
	movq	40(%rdx), %rcx
	xorq	%rcx, 40(%rax)
	movq	48(%rdx), %rcx
	xorq	%rcx, 48(%rax)
	movq	56(%rdx), %rcx
	xorq	%rcx, 56(%rax)
	xorb	$31, 64(%rax)
	xorb	$-128, 135(%rax)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_shake256_64$9:
	leaq	224(%rsp), %rsp
	movq	16(%rsp), %rcx
	movq	8(%rsp), %rdx
	jmp 	L_shake256_64$6
L_shake256_64$7:
	movq	(%rax), %rsi
	movq	%rsi, (%rdx)
	movq	8(%rax), %rsi
	movq	%rsi, 8(%rdx)
	movq	16(%rax), %rsi
	movq	%rsi, 16(%rdx)
	movq	24(%rax), %rsi
	movq	%rsi, 24(%rdx)
	movq	32(%rax), %rsi
	movq	%rsi, 32(%rdx)
	movq	40(%rax), %rsi
	movq	%rsi, 40(%rdx)
	movq	48(%rax), %rsi
	movq	%rsi, 48(%rdx)
	movq	56(%rax), %rsi
	movq	%rsi, 56(%rdx)
	movq	64(%rax), %rsi
	movq	%rsi, 64(%rdx)
	movq	72(%rax), %rsi
	movq	%rsi, 72(%rdx)
	movq	80(%rax), %rsi
	movq	%rsi, 80(%rdx)
	movq	88(%rax), %rsi
	movq	%rsi, 88(%rdx)
	movq	96(%rax), %rsi
	movq	%rsi, 96(%rdx)
	movq	104(%rax), %rsi
	movq	%rsi, 104(%rdx)
	movq	112(%rax), %rsi
	movq	%rsi, 112(%rdx)
	movq	120(%rax), %rsi
	movq	%rsi, 120(%rdx)
	movq	128(%rax), %rsi
	movq	%rsi, 128(%rdx)
	addq	$136, %rdx
	addq	$-136, %rcx
	movq	%rdx, 8(%rsp)
	movq	%rcx, 16(%rsp)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_shake256_64$8:
	leaq	224(%rsp), %rsp
	movq	16(%rsp), %rcx
	movq	8(%rsp), %rdx
L_shake256_64$6:
	cmpq	$136, %rcx
	jnbe	L_shake256_64$7
	movq	%rcx, 8(%rsp)
	shrq	$3, %rcx
	movq	$0, %rsi
	jmp 	L_shake256_64$4
L_shake256_64$5:
	movq	(%rax,%rsi,8), %rdi
	movq	%rdi, (%rdx,%rsi,8)
	incq	%rsi
L_shake256_64$4:
	cmpq	%rcx, %rsi
	jb  	L_shake256_64$5
	shlq	$3, %rsi
	movq	8(%rsp), %rcx
	jmp 	L_shake256_64$2
L_shake256_64$3:
	movb	(%rax,%rsi), %dil
	movb	%dil, (%rdx,%rsi)
	incq	%rsi
L_shake256_64$2:
	cmpq	%rcx, %rsi
	jb  	L_shake256_64$3
	ret
L_sha3_512_64$1:
	movq	%rax, 8(%rsp)
	leaq	16(%rsp), %rax
	xorq	%rdx, %rdx
	movq	%rdx, (%rax)
	movq	%rdx, 8(%rax)
	movq	%rdx, 16(%rax)
	movq	%rdx, 24(%rax)
	movq	%rdx, 32(%rax)
	movq	%rdx, 40(%rax)
	movq	%rdx, 48(%rax)
	movq	%rdx, 56(%rax)
	movq	%rdx, 64(%rax)
	movq	%rdx, 72(%rax)
	movq	%rdx, 80(%rax)
	movq	%rdx, 88(%rax)
	movq	%rdx, 96(%rax)
	movq	%rdx, 104(%rax)
	movq	%rdx, 112(%rax)
	movq	%rdx, 120(%rax)
	movq	%rdx, 128(%rax)
	movq	%rdx, 136(%rax)
	movq	%rdx, 144(%rax)
	movq	%rdx, 152(%rax)
	movq	%rdx, 160(%rax)
	movq	%rdx, 168(%rax)
	movq	%rdx, 176(%rax)
	movq	%rdx, 184(%rax)
	movq	%rdx, 192(%rax)
	movq	(%rcx), %rdx
	xorq	%rdx, (%rax)
	movq	8(%rcx), %rdx
	xorq	%rdx, 8(%rax)
	movq	16(%rcx), %rdx
	xorq	%rdx, 16(%rax)
	movq	24(%rcx), %rdx
	xorq	%rdx, 24(%rax)
	movq	32(%rcx), %rdx
	xorq	%rdx, 32(%rax)
	movq	40(%rcx), %rdx
	xorq	%rdx, 40(%rax)
	movq	48(%rcx), %rdx
	xorq	%rdx, 48(%rax)
	movq	56(%rcx), %rcx
	xorq	%rcx, 56(%rax)
	xorb	$6, 64(%rax)
	xorb	$-128, 71(%rax)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_sha3_512_64$2:
	leaq	224(%rsp), %rsp
	movq	8(%rsp), %rcx
	movq	(%rax), %rdx
	movq	%rdx, (%rcx)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rcx)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rcx)
	movq	24(%rax), %rdx
	movq	%rdx, 24(%rcx)
	movq	32(%rax), %rdx
	movq	%rdx, 32(%rcx)
	movq	40(%rax), %rdx
	movq	%rdx, 40(%rcx)
	movq	48(%rax), %rdx
	movq	%rdx, 48(%rcx)
	movq	56(%rax), %rax
	movq	%rax, 56(%rcx)
	ret
L_sha3_256_32$1:
	movq	%rax, 8(%rsp)
	leaq	16(%rsp), %rax
	xorq	%rdx, %rdx
	movq	%rdx, (%rax)
	movq	%rdx, 8(%rax)
	movq	%rdx, 16(%rax)
	movq	%rdx, 24(%rax)
	movq	%rdx, 32(%rax)
	movq	%rdx, 40(%rax)
	movq	%rdx, 48(%rax)
	movq	%rdx, 56(%rax)
	movq	%rdx, 64(%rax)
	movq	%rdx, 72(%rax)
	movq	%rdx, 80(%rax)
	movq	%rdx, 88(%rax)
	movq	%rdx, 96(%rax)
	movq	%rdx, 104(%rax)
	movq	%rdx, 112(%rax)
	movq	%rdx, 120(%rax)
	movq	%rdx, 128(%rax)
	movq	%rdx, 136(%rax)
	movq	%rdx, 144(%rax)
	movq	%rdx, 152(%rax)
	movq	%rdx, 160(%rax)
	movq	%rdx, 168(%rax)
	movq	%rdx, 176(%rax)
	movq	%rdx, 184(%rax)
	movq	%rdx, 192(%rax)
	movq	(%rcx), %rdx
	movq	%rdx, (%rax)
	movq	8(%rcx), %rdx
	movq	%rdx, 8(%rax)
	movq	16(%rcx), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%rcx), %rcx
	movq	%rcx, 24(%rax)
	xorb	$6, 32(%rax)
	movb	$-128, 135(%rax)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_sha3_256_32$2:
	leaq	224(%rsp), %rsp
	movq	8(%rsp), %rcx
	movq	(%rax), %rdx
	movq	%rdx, (%rcx)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rcx)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rcx)
	movq	24(%rax), %rax
	movq	%rax, 24(%rcx)
	ret
L_sha3_256$1:
	movq	%rax, 8(%rsp)
	leaq	40(%rsp), %rax
	xorq	%rdx, %rdx
	movq	%rdx, (%rax)
	movq	%rdx, 8(%rax)
	movq	%rdx, 16(%rax)
	movq	%rdx, 24(%rax)
	movq	%rdx, 32(%rax)
	movq	%rdx, 40(%rax)
	movq	%rdx, 48(%rax)
	movq	%rdx, 56(%rax)
	movq	%rdx, 64(%rax)
	movq	%rdx, 72(%rax)
	movq	%rdx, 80(%rax)
	movq	%rdx, 88(%rax)
	movq	%rdx, 96(%rax)
	movq	%rdx, 104(%rax)
	movq	%rdx, 112(%rax)
	movq	%rdx, 120(%rax)
	movq	%rdx, 128(%rax)
	movq	%rdx, 136(%rax)
	movq	%rdx, 144(%rax)
	movq	%rdx, 152(%rax)
	movq	%rdx, 160(%rax)
	movq	%rdx, 168(%rax)
	movq	%rdx, 176(%rax)
	movq	%rdx, 184(%rax)
	movq	%rdx, 192(%rax)
	movq	$136, %rdx
	jmp 	L_sha3_256$7
L_sha3_256$8:
	movq	%rdx, %rdi
	shrq	$3, %rdi
	movq	$0, %r8
	jmp 	L_sha3_256$10
L_sha3_256$11:
	movq	(%rsi,%r8,8), %r9
	xorq	%r9, (%rax,%r8,8)
	incq	%r8
L_sha3_256$10:
	cmpq	%rdi, %r8
	jb  	L_sha3_256$11
	addq	%rdx, %rsi
	subq	%rdx, %rcx
	movq	%rsi, 16(%rsp)
	movq	%rcx, 24(%rsp)
	movq	%rdx, 32(%rsp)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_sha3_256$9:
	leaq	224(%rsp), %rsp
	movq	16(%rsp), %rsi
	movq	24(%rsp), %rcx
	movq	32(%rsp), %rdx
L_sha3_256$7:
	cmpq	%rdx, %rcx
	jnb 	L_sha3_256$8
	movb	$6, %dil
	movq	%rcx, %r8
	shrq	$3, %r8
	movq	$0, %r9
	jmp 	L_sha3_256$5
L_sha3_256$6:
	movq	(%rsi,%r9,8), %r10
	xorq	%r10, (%rax,%r9,8)
	incq	%r9
L_sha3_256$5:
	cmpq	%r8, %r9
	jb  	L_sha3_256$6
	shlq	$3, %r9
	jmp 	L_sha3_256$3
L_sha3_256$4:
	movb	(%rsi,%r9), %r8b
	xorb	%r8b, (%rax,%r9)
	incq	%r9
L_sha3_256$3:
	cmpq	%rcx, %r9
	jb  	L_sha3_256$4
	xorb	%dil, (%rax,%r9)
	addq	$-1, %rdx
	xorb	$-128, (%rax,%rdx)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_sha3_256$2:
	leaq	224(%rsp), %rsp
	movq	8(%rsp), %rcx
	movq	(%rax), %rdx
	movq	%rdx, (%rcx)
	movq	8(%rax), %rdx
	movq	%rdx, 8(%rcx)
	movq	16(%rax), %rdx
	movq	%rdx, 16(%rcx)
	movq	24(%rax), %rax
	movq	%rax, 24(%rcx)
	ret
L_shake128_squeezeblock$1:
	movq	%rcx, 8(%rsp)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_shake128_squeezeblock$2:
	leaq	224(%rsp), %rsp
	movq	8(%rsp), %rcx
	movb	(%rax), %dl
	movb	%dl, (%rcx)
	movb	1(%rax), %dl
	movb	%dl, 1(%rcx)
	movb	2(%rax), %dl
	movb	%dl, 2(%rcx)
	movb	3(%rax), %dl
	movb	%dl, 3(%rcx)
	movb	4(%rax), %dl
	movb	%dl, 4(%rcx)
	movb	5(%rax), %dl
	movb	%dl, 5(%rcx)
	movb	6(%rax), %dl
	movb	%dl, 6(%rcx)
	movb	7(%rax), %dl
	movb	%dl, 7(%rcx)
	movb	8(%rax), %dl
	movb	%dl, 8(%rcx)
	movb	9(%rax), %dl
	movb	%dl, 9(%rcx)
	movb	10(%rax), %dl
	movb	%dl, 10(%rcx)
	movb	11(%rax), %dl
	movb	%dl, 11(%rcx)
	movb	12(%rax), %dl
	movb	%dl, 12(%rcx)
	movb	13(%rax), %dl
	movb	%dl, 13(%rcx)
	movb	14(%rax), %dl
	movb	%dl, 14(%rcx)
	movb	15(%rax), %dl
	movb	%dl, 15(%rcx)
	movb	16(%rax), %dl
	movb	%dl, 16(%rcx)
	movb	17(%rax), %dl
	movb	%dl, 17(%rcx)
	movb	18(%rax), %dl
	movb	%dl, 18(%rcx)
	movb	19(%rax), %dl
	movb	%dl, 19(%rcx)
	movb	20(%rax), %dl
	movb	%dl, 20(%rcx)
	movb	21(%rax), %dl
	movb	%dl, 21(%rcx)
	movb	22(%rax), %dl
	movb	%dl, 22(%rcx)
	movb	23(%rax), %dl
	movb	%dl, 23(%rcx)
	movb	24(%rax), %dl
	movb	%dl, 24(%rcx)
	movb	25(%rax), %dl
	movb	%dl, 25(%rcx)
	movb	26(%rax), %dl
	movb	%dl, 26(%rcx)
	movb	27(%rax), %dl
	movb	%dl, 27(%rcx)
	movb	28(%rax), %dl
	movb	%dl, 28(%rcx)
	movb	29(%rax), %dl
	movb	%dl, 29(%rcx)
	movb	30(%rax), %dl
	movb	%dl, 30(%rcx)
	movb	31(%rax), %dl
	movb	%dl, 31(%rcx)
	movb	32(%rax), %dl
	movb	%dl, 32(%rcx)
	movb	33(%rax), %dl
	movb	%dl, 33(%rcx)
	movb	34(%rax), %dl
	movb	%dl, 34(%rcx)
	movb	35(%rax), %dl
	movb	%dl, 35(%rcx)
	movb	36(%rax), %dl
	movb	%dl, 36(%rcx)
	movb	37(%rax), %dl
	movb	%dl, 37(%rcx)
	movb	38(%rax), %dl
	movb	%dl, 38(%rcx)
	movb	39(%rax), %dl
	movb	%dl, 39(%rcx)
	movb	40(%rax), %dl
	movb	%dl, 40(%rcx)
	movb	41(%rax), %dl
	movb	%dl, 41(%rcx)
	movb	42(%rax), %dl
	movb	%dl, 42(%rcx)
	movb	43(%rax), %dl
	movb	%dl, 43(%rcx)
	movb	44(%rax), %dl
	movb	%dl, 44(%rcx)
	movb	45(%rax), %dl
	movb	%dl, 45(%rcx)
	movb	46(%rax), %dl
	movb	%dl, 46(%rcx)
	movb	47(%rax), %dl
	movb	%dl, 47(%rcx)
	movb	48(%rax), %dl
	movb	%dl, 48(%rcx)
	movb	49(%rax), %dl
	movb	%dl, 49(%rcx)
	movb	50(%rax), %dl
	movb	%dl, 50(%rcx)
	movb	51(%rax), %dl
	movb	%dl, 51(%rcx)
	movb	52(%rax), %dl
	movb	%dl, 52(%rcx)
	movb	53(%rax), %dl
	movb	%dl, 53(%rcx)
	movb	54(%rax), %dl
	movb	%dl, 54(%rcx)
	movb	55(%rax), %dl
	movb	%dl, 55(%rcx)
	movb	56(%rax), %dl
	movb	%dl, 56(%rcx)
	movb	57(%rax), %dl
	movb	%dl, 57(%rcx)
	movb	58(%rax), %dl
	movb	%dl, 58(%rcx)
	movb	59(%rax), %dl
	movb	%dl, 59(%rcx)
	movb	60(%rax), %dl
	movb	%dl, 60(%rcx)
	movb	61(%rax), %dl
	movb	%dl, 61(%rcx)
	movb	62(%rax), %dl
	movb	%dl, 62(%rcx)
	movb	63(%rax), %dl
	movb	%dl, 63(%rcx)
	movb	64(%rax), %dl
	movb	%dl, 64(%rcx)
	movb	65(%rax), %dl
	movb	%dl, 65(%rcx)
	movb	66(%rax), %dl
	movb	%dl, 66(%rcx)
	movb	67(%rax), %dl
	movb	%dl, 67(%rcx)
	movb	68(%rax), %dl
	movb	%dl, 68(%rcx)
	movb	69(%rax), %dl
	movb	%dl, 69(%rcx)
	movb	70(%rax), %dl
	movb	%dl, 70(%rcx)
	movb	71(%rax), %dl
	movb	%dl, 71(%rcx)
	movb	72(%rax), %dl
	movb	%dl, 72(%rcx)
	movb	73(%rax), %dl
	movb	%dl, 73(%rcx)
	movb	74(%rax), %dl
	movb	%dl, 74(%rcx)
	movb	75(%rax), %dl
	movb	%dl, 75(%rcx)
	movb	76(%rax), %dl
	movb	%dl, 76(%rcx)
	movb	77(%rax), %dl
	movb	%dl, 77(%rcx)
	movb	78(%rax), %dl
	movb	%dl, 78(%rcx)
	movb	79(%rax), %dl
	movb	%dl, 79(%rcx)
	movb	80(%rax), %dl
	movb	%dl, 80(%rcx)
	movb	81(%rax), %dl
	movb	%dl, 81(%rcx)
	movb	82(%rax), %dl
	movb	%dl, 82(%rcx)
	movb	83(%rax), %dl
	movb	%dl, 83(%rcx)
	movb	84(%rax), %dl
	movb	%dl, 84(%rcx)
	movb	85(%rax), %dl
	movb	%dl, 85(%rcx)
	movb	86(%rax), %dl
	movb	%dl, 86(%rcx)
	movb	87(%rax), %dl
	movb	%dl, 87(%rcx)
	movb	88(%rax), %dl
	movb	%dl, 88(%rcx)
	movb	89(%rax), %dl
	movb	%dl, 89(%rcx)
	movb	90(%rax), %dl
	movb	%dl, 90(%rcx)
	movb	91(%rax), %dl
	movb	%dl, 91(%rcx)
	movb	92(%rax), %dl
	movb	%dl, 92(%rcx)
	movb	93(%rax), %dl
	movb	%dl, 93(%rcx)
	movb	94(%rax), %dl
	movb	%dl, 94(%rcx)
	movb	95(%rax), %dl
	movb	%dl, 95(%rcx)
	movb	96(%rax), %dl
	movb	%dl, 96(%rcx)
	movb	97(%rax), %dl
	movb	%dl, 97(%rcx)
	movb	98(%rax), %dl
	movb	%dl, 98(%rcx)
	movb	99(%rax), %dl
	movb	%dl, 99(%rcx)
	movb	100(%rax), %dl
	movb	%dl, 100(%rcx)
	movb	101(%rax), %dl
	movb	%dl, 101(%rcx)
	movb	102(%rax), %dl
	movb	%dl, 102(%rcx)
	movb	103(%rax), %dl
	movb	%dl, 103(%rcx)
	movb	104(%rax), %dl
	movb	%dl, 104(%rcx)
	movb	105(%rax), %dl
	movb	%dl, 105(%rcx)
	movb	106(%rax), %dl
	movb	%dl, 106(%rcx)
	movb	107(%rax), %dl
	movb	%dl, 107(%rcx)
	movb	108(%rax), %dl
	movb	%dl, 108(%rcx)
	movb	109(%rax), %dl
	movb	%dl, 109(%rcx)
	movb	110(%rax), %dl
	movb	%dl, 110(%rcx)
	movb	111(%rax), %dl
	movb	%dl, 111(%rcx)
	movb	112(%rax), %dl
	movb	%dl, 112(%rcx)
	movb	113(%rax), %dl
	movb	%dl, 113(%rcx)
	movb	114(%rax), %dl
	movb	%dl, 114(%rcx)
	movb	115(%rax), %dl
	movb	%dl, 115(%rcx)
	movb	116(%rax), %dl
	movb	%dl, 116(%rcx)
	movb	117(%rax), %dl
	movb	%dl, 117(%rcx)
	movb	118(%rax), %dl
	movb	%dl, 118(%rcx)
	movb	119(%rax), %dl
	movb	%dl, 119(%rcx)
	movb	120(%rax), %dl
	movb	%dl, 120(%rcx)
	movb	121(%rax), %dl
	movb	%dl, 121(%rcx)
	movb	122(%rax), %dl
	movb	%dl, 122(%rcx)
	movb	123(%rax), %dl
	movb	%dl, 123(%rcx)
	movb	124(%rax), %dl
	movb	%dl, 124(%rcx)
	movb	125(%rax), %dl
	movb	%dl, 125(%rcx)
	movb	126(%rax), %dl
	movb	%dl, 126(%rcx)
	movb	127(%rax), %dl
	movb	%dl, 127(%rcx)
	movb	128(%rax), %dl
	movb	%dl, 128(%rcx)
	movb	129(%rax), %dl
	movb	%dl, 129(%rcx)
	movb	130(%rax), %dl
	movb	%dl, 130(%rcx)
	movb	131(%rax), %dl
	movb	%dl, 131(%rcx)
	movb	132(%rax), %dl
	movb	%dl, 132(%rcx)
	movb	133(%rax), %dl
	movb	%dl, 133(%rcx)
	movb	134(%rax), %dl
	movb	%dl, 134(%rcx)
	movb	135(%rax), %dl
	movb	%dl, 135(%rcx)
	movb	136(%rax), %dl
	movb	%dl, 136(%rcx)
	movb	137(%rax), %dl
	movb	%dl, 137(%rcx)
	movb	138(%rax), %dl
	movb	%dl, 138(%rcx)
	movb	139(%rax), %dl
	movb	%dl, 139(%rcx)
	movb	140(%rax), %dl
	movb	%dl, 140(%rcx)
	movb	141(%rax), %dl
	movb	%dl, 141(%rcx)
	movb	142(%rax), %dl
	movb	%dl, 142(%rcx)
	movb	143(%rax), %dl
	movb	%dl, 143(%rcx)
	movb	144(%rax), %dl
	movb	%dl, 144(%rcx)
	movb	145(%rax), %dl
	movb	%dl, 145(%rcx)
	movb	146(%rax), %dl
	movb	%dl, 146(%rcx)
	movb	147(%rax), %dl
	movb	%dl, 147(%rcx)
	movb	148(%rax), %dl
	movb	%dl, 148(%rcx)
	movb	149(%rax), %dl
	movb	%dl, 149(%rcx)
	movb	150(%rax), %dl
	movb	%dl, 150(%rcx)
	movb	151(%rax), %dl
	movb	%dl, 151(%rcx)
	movb	152(%rax), %dl
	movb	%dl, 152(%rcx)
	movb	153(%rax), %dl
	movb	%dl, 153(%rcx)
	movb	154(%rax), %dl
	movb	%dl, 154(%rcx)
	movb	155(%rax), %dl
	movb	%dl, 155(%rcx)
	movb	156(%rax), %dl
	movb	%dl, 156(%rcx)
	movb	157(%rax), %dl
	movb	%dl, 157(%rcx)
	movb	158(%rax), %dl
	movb	%dl, 158(%rcx)
	movb	159(%rax), %dl
	movb	%dl, 159(%rcx)
	movb	160(%rax), %dl
	movb	%dl, 160(%rcx)
	movb	161(%rax), %dl
	movb	%dl, 161(%rcx)
	movb	162(%rax), %dl
	movb	%dl, 162(%rcx)
	movb	163(%rax), %dl
	movb	%dl, 163(%rcx)
	movb	164(%rax), %dl
	movb	%dl, 164(%rcx)
	movb	165(%rax), %dl
	movb	%dl, 165(%rcx)
	movb	166(%rax), %dl
	movb	%dl, 166(%rcx)
	movb	167(%rax), %al
	movb	%al, 167(%rcx)
	ret
L_shake128_absorb34$1:
	xorq	%rdx, %rdx
	movq	%rdx, (%rax)
	movq	%rdx, 8(%rax)
	movq	%rdx, 16(%rax)
	movq	%rdx, 24(%rax)
	movq	%rdx, 32(%rax)
	movq	%rdx, 40(%rax)
	movq	%rdx, 48(%rax)
	movq	%rdx, 56(%rax)
	movq	%rdx, 64(%rax)
	movq	%rdx, 72(%rax)
	movq	%rdx, 80(%rax)
	movq	%rdx, 88(%rax)
	movq	%rdx, 96(%rax)
	movq	%rdx, 104(%rax)
	movq	%rdx, 112(%rax)
	movq	%rdx, 120(%rax)
	movq	%rdx, 128(%rax)
	movq	%rdx, 136(%rax)
	movq	%rdx, 144(%rax)
	movq	%rdx, 152(%rax)
	movq	%rdx, 160(%rax)
	movq	%rdx, 168(%rax)
	movq	%rdx, 176(%rax)
	movq	%rdx, 184(%rax)
	movq	%rdx, 192(%rax)
	movb	(%rcx), %dl
	xorb	%dl, (%rax)
	movb	1(%rcx), %dl
	xorb	%dl, 1(%rax)
	movb	2(%rcx), %dl
	xorb	%dl, 2(%rax)
	movb	3(%rcx), %dl
	xorb	%dl, 3(%rax)
	movb	4(%rcx), %dl
	xorb	%dl, 4(%rax)
	movb	5(%rcx), %dl
	xorb	%dl, 5(%rax)
	movb	6(%rcx), %dl
	xorb	%dl, 6(%rax)
	movb	7(%rcx), %dl
	xorb	%dl, 7(%rax)
	movb	8(%rcx), %dl
	xorb	%dl, 8(%rax)
	movb	9(%rcx), %dl
	xorb	%dl, 9(%rax)
	movb	10(%rcx), %dl
	xorb	%dl, 10(%rax)
	movb	11(%rcx), %dl
	xorb	%dl, 11(%rax)
	movb	12(%rcx), %dl
	xorb	%dl, 12(%rax)
	movb	13(%rcx), %dl
	xorb	%dl, 13(%rax)
	movb	14(%rcx), %dl
	xorb	%dl, 14(%rax)
	movb	15(%rcx), %dl
	xorb	%dl, 15(%rax)
	movb	16(%rcx), %dl
	xorb	%dl, 16(%rax)
	movb	17(%rcx), %dl
	xorb	%dl, 17(%rax)
	movb	18(%rcx), %dl
	xorb	%dl, 18(%rax)
	movb	19(%rcx), %dl
	xorb	%dl, 19(%rax)
	movb	20(%rcx), %dl
	xorb	%dl, 20(%rax)
	movb	21(%rcx), %dl
	xorb	%dl, 21(%rax)
	movb	22(%rcx), %dl
	xorb	%dl, 22(%rax)
	movb	23(%rcx), %dl
	xorb	%dl, 23(%rax)
	movb	24(%rcx), %dl
	xorb	%dl, 24(%rax)
	movb	25(%rcx), %dl
	xorb	%dl, 25(%rax)
	movb	26(%rcx), %dl
	xorb	%dl, 26(%rax)
	movb	27(%rcx), %dl
	xorb	%dl, 27(%rax)
	movb	28(%rcx), %dl
	xorb	%dl, 28(%rax)
	movb	29(%rcx), %dl
	xorb	%dl, 29(%rax)
	movb	30(%rcx), %dl
	xorb	%dl, 30(%rax)
	movb	31(%rcx), %dl
	xorb	%dl, 31(%rax)
	movb	32(%rcx), %dl
	xorb	%dl, 32(%rax)
	movb	33(%rcx), %cl
	xorb	%cl, 33(%rax)
	xorb	$31, 34(%rax)
	xorb	$-128, 167(%rax)
	ret
L_sha3_512_32$1:
	movq	%rax, 8(%rsp)
	leaq	16(%rsp), %rax
	xorq	%rdx, %rdx
	movq	%rdx, (%rax)
	movq	%rdx, 8(%rax)
	movq	%rdx, 16(%rax)
	movq	%rdx, 24(%rax)
	movq	%rdx, 32(%rax)
	movq	%rdx, 40(%rax)
	movq	%rdx, 48(%rax)
	movq	%rdx, 56(%rax)
	movq	%rdx, 64(%rax)
	movq	%rdx, 72(%rax)
	movq	%rdx, 80(%rax)
	movq	%rdx, 88(%rax)
	movq	%rdx, 96(%rax)
	movq	%rdx, 104(%rax)
	movq	%rdx, 112(%rax)
	movq	%rdx, 120(%rax)
	movq	%rdx, 128(%rax)
	movq	%rdx, 136(%rax)
	movq	%rdx, 144(%rax)
	movq	%rdx, 152(%rax)
	movq	%rdx, 160(%rax)
	movq	%rdx, 168(%rax)
	movq	%rdx, 176(%rax)
	movq	%rdx, 184(%rax)
	movq	%rdx, 192(%rax)
	movb	(%rcx), %dl
	xorb	%dl, (%rax)
	movb	1(%rcx), %dl
	xorb	%dl, 1(%rax)
	movb	2(%rcx), %dl
	xorb	%dl, 2(%rax)
	movb	3(%rcx), %dl
	xorb	%dl, 3(%rax)
	movb	4(%rcx), %dl
	xorb	%dl, 4(%rax)
	movb	5(%rcx), %dl
	xorb	%dl, 5(%rax)
	movb	6(%rcx), %dl
	xorb	%dl, 6(%rax)
	movb	7(%rcx), %dl
	xorb	%dl, 7(%rax)
	movb	8(%rcx), %dl
	xorb	%dl, 8(%rax)
	movb	9(%rcx), %dl
	xorb	%dl, 9(%rax)
	movb	10(%rcx), %dl
	xorb	%dl, 10(%rax)
	movb	11(%rcx), %dl
	xorb	%dl, 11(%rax)
	movb	12(%rcx), %dl
	xorb	%dl, 12(%rax)
	movb	13(%rcx), %dl
	xorb	%dl, 13(%rax)
	movb	14(%rcx), %dl
	xorb	%dl, 14(%rax)
	movb	15(%rcx), %dl
	xorb	%dl, 15(%rax)
	movb	16(%rcx), %dl
	xorb	%dl, 16(%rax)
	movb	17(%rcx), %dl
	xorb	%dl, 17(%rax)
	movb	18(%rcx), %dl
	xorb	%dl, 18(%rax)
	movb	19(%rcx), %dl
	xorb	%dl, 19(%rax)
	movb	20(%rcx), %dl
	xorb	%dl, 20(%rax)
	movb	21(%rcx), %dl
	xorb	%dl, 21(%rax)
	movb	22(%rcx), %dl
	xorb	%dl, 22(%rax)
	movb	23(%rcx), %dl
	xorb	%dl, 23(%rax)
	movb	24(%rcx), %dl
	xorb	%dl, 24(%rax)
	movb	25(%rcx), %dl
	xorb	%dl, 25(%rax)
	movb	26(%rcx), %dl
	xorb	%dl, 26(%rax)
	movb	27(%rcx), %dl
	xorb	%dl, 27(%rax)
	movb	28(%rcx), %dl
	xorb	%dl, 28(%rax)
	movb	29(%rcx), %dl
	xorb	%dl, 29(%rax)
	movb	30(%rcx), %dl
	xorb	%dl, 30(%rax)
	movb	31(%rcx), %cl
	xorb	%cl, 31(%rax)
	xorb	$6, 32(%rax)
	xorb	$-128, 71(%rax)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_sha3_512_32$2:
	leaq	224(%rsp), %rsp
	movq	8(%rsp), %rcx
	movb	(%rax), %dl
	movb	%dl, (%rcx)
	movb	1(%rax), %dl
	movb	%dl, 1(%rcx)
	movb	2(%rax), %dl
	movb	%dl, 2(%rcx)
	movb	3(%rax), %dl
	movb	%dl, 3(%rcx)
	movb	4(%rax), %dl
	movb	%dl, 4(%rcx)
	movb	5(%rax), %dl
	movb	%dl, 5(%rcx)
	movb	6(%rax), %dl
	movb	%dl, 6(%rcx)
	movb	7(%rax), %dl
	movb	%dl, 7(%rcx)
	movb	8(%rax), %dl
	movb	%dl, 8(%rcx)
	movb	9(%rax), %dl
	movb	%dl, 9(%rcx)
	movb	10(%rax), %dl
	movb	%dl, 10(%rcx)
	movb	11(%rax), %dl
	movb	%dl, 11(%rcx)
	movb	12(%rax), %dl
	movb	%dl, 12(%rcx)
	movb	13(%rax), %dl
	movb	%dl, 13(%rcx)
	movb	14(%rax), %dl
	movb	%dl, 14(%rcx)
	movb	15(%rax), %dl
	movb	%dl, 15(%rcx)
	movb	16(%rax), %dl
	movb	%dl, 16(%rcx)
	movb	17(%rax), %dl
	movb	%dl, 17(%rcx)
	movb	18(%rax), %dl
	movb	%dl, 18(%rcx)
	movb	19(%rax), %dl
	movb	%dl, 19(%rcx)
	movb	20(%rax), %dl
	movb	%dl, 20(%rcx)
	movb	21(%rax), %dl
	movb	%dl, 21(%rcx)
	movb	22(%rax), %dl
	movb	%dl, 22(%rcx)
	movb	23(%rax), %dl
	movb	%dl, 23(%rcx)
	movb	24(%rax), %dl
	movb	%dl, 24(%rcx)
	movb	25(%rax), %dl
	movb	%dl, 25(%rcx)
	movb	26(%rax), %dl
	movb	%dl, 26(%rcx)
	movb	27(%rax), %dl
	movb	%dl, 27(%rcx)
	movb	28(%rax), %dl
	movb	%dl, 28(%rcx)
	movb	29(%rax), %dl
	movb	%dl, 29(%rcx)
	movb	30(%rax), %dl
	movb	%dl, 30(%rcx)
	movb	31(%rax), %dl
	movb	%dl, 31(%rcx)
	movb	32(%rax), %dl
	movb	%dl, 32(%rcx)
	movb	33(%rax), %dl
	movb	%dl, 33(%rcx)
	movb	34(%rax), %dl
	movb	%dl, 34(%rcx)
	movb	35(%rax), %dl
	movb	%dl, 35(%rcx)
	movb	36(%rax), %dl
	movb	%dl, 36(%rcx)
	movb	37(%rax), %dl
	movb	%dl, 37(%rcx)
	movb	38(%rax), %dl
	movb	%dl, 38(%rcx)
	movb	39(%rax), %dl
	movb	%dl, 39(%rcx)
	movb	40(%rax), %dl
	movb	%dl, 40(%rcx)
	movb	41(%rax), %dl
	movb	%dl, 41(%rcx)
	movb	42(%rax), %dl
	movb	%dl, 42(%rcx)
	movb	43(%rax), %dl
	movb	%dl, 43(%rcx)
	movb	44(%rax), %dl
	movb	%dl, 44(%rcx)
	movb	45(%rax), %dl
	movb	%dl, 45(%rcx)
	movb	46(%rax), %dl
	movb	%dl, 46(%rcx)
	movb	47(%rax), %dl
	movb	%dl, 47(%rcx)
	movb	48(%rax), %dl
	movb	%dl, 48(%rcx)
	movb	49(%rax), %dl
	movb	%dl, 49(%rcx)
	movb	50(%rax), %dl
	movb	%dl, 50(%rcx)
	movb	51(%rax), %dl
	movb	%dl, 51(%rcx)
	movb	52(%rax), %dl
	movb	%dl, 52(%rcx)
	movb	53(%rax), %dl
	movb	%dl, 53(%rcx)
	movb	54(%rax), %dl
	movb	%dl, 54(%rcx)
	movb	55(%rax), %dl
	movb	%dl, 55(%rcx)
	movb	56(%rax), %dl
	movb	%dl, 56(%rcx)
	movb	57(%rax), %dl
	movb	%dl, 57(%rcx)
	movb	58(%rax), %dl
	movb	%dl, 58(%rcx)
	movb	59(%rax), %dl
	movb	%dl, 59(%rcx)
	movb	60(%rax), %dl
	movb	%dl, 60(%rcx)
	movb	61(%rax), %dl
	movb	%dl, 61(%rcx)
	movb	62(%rax), %dl
	movb	%dl, 62(%rcx)
	movb	63(%rax), %al
	movb	%al, 63(%rcx)
	ret
L_shake256_192_33$1:
	movq	%rax, 8(%rsp)
	leaq	16(%rsp), %rax
	xorq	%rdx, %rdx
	movq	%rdx, (%rax)
	movq	%rdx, 8(%rax)
	movq	%rdx, 16(%rax)
	movq	%rdx, 24(%rax)
	movq	%rdx, 32(%rax)
	movq	%rdx, 40(%rax)
	movq	%rdx, 48(%rax)
	movq	%rdx, 56(%rax)
	movq	%rdx, 64(%rax)
	movq	%rdx, 72(%rax)
	movq	%rdx, 80(%rax)
	movq	%rdx, 88(%rax)
	movq	%rdx, 96(%rax)
	movq	%rdx, 104(%rax)
	movq	%rdx, 112(%rax)
	movq	%rdx, 120(%rax)
	movq	%rdx, 128(%rax)
	movq	%rdx, 136(%rax)
	movq	%rdx, 144(%rax)
	movq	%rdx, 152(%rax)
	movq	%rdx, 160(%rax)
	movq	%rdx, 168(%rax)
	movq	%rdx, 176(%rax)
	movq	%rdx, 184(%rax)
	movq	%rdx, 192(%rax)
	movb	(%rcx), %dl
	xorb	%dl, (%rax)
	movb	1(%rcx), %dl
	xorb	%dl, 1(%rax)
	movb	2(%rcx), %dl
	xorb	%dl, 2(%rax)
	movb	3(%rcx), %dl
	xorb	%dl, 3(%rax)
	movb	4(%rcx), %dl
	xorb	%dl, 4(%rax)
	movb	5(%rcx), %dl
	xorb	%dl, 5(%rax)
	movb	6(%rcx), %dl
	xorb	%dl, 6(%rax)
	movb	7(%rcx), %dl
	xorb	%dl, 7(%rax)
	movb	8(%rcx), %dl
	xorb	%dl, 8(%rax)
	movb	9(%rcx), %dl
	xorb	%dl, 9(%rax)
	movb	10(%rcx), %dl
	xorb	%dl, 10(%rax)
	movb	11(%rcx), %dl
	xorb	%dl, 11(%rax)
	movb	12(%rcx), %dl
	xorb	%dl, 12(%rax)
	movb	13(%rcx), %dl
	xorb	%dl, 13(%rax)
	movb	14(%rcx), %dl
	xorb	%dl, 14(%rax)
	movb	15(%rcx), %dl
	xorb	%dl, 15(%rax)
	movb	16(%rcx), %dl
	xorb	%dl, 16(%rax)
	movb	17(%rcx), %dl
	xorb	%dl, 17(%rax)
	movb	18(%rcx), %dl
	xorb	%dl, 18(%rax)
	movb	19(%rcx), %dl
	xorb	%dl, 19(%rax)
	movb	20(%rcx), %dl
	xorb	%dl, 20(%rax)
	movb	21(%rcx), %dl
	xorb	%dl, 21(%rax)
	movb	22(%rcx), %dl
	xorb	%dl, 22(%rax)
	movb	23(%rcx), %dl
	xorb	%dl, 23(%rax)
	movb	24(%rcx), %dl
	xorb	%dl, 24(%rax)
	movb	25(%rcx), %dl
	xorb	%dl, 25(%rax)
	movb	26(%rcx), %dl
	xorb	%dl, 26(%rax)
	movb	27(%rcx), %dl
	xorb	%dl, 27(%rax)
	movb	28(%rcx), %dl
	xorb	%dl, 28(%rax)
	movb	29(%rcx), %dl
	xorb	%dl, 29(%rax)
	movb	30(%rcx), %dl
	xorb	%dl, 30(%rax)
	movb	31(%rcx), %dl
	xorb	%dl, 31(%rax)
	movb	32(%rcx), %cl
	xorb	%cl, 32(%rax)
	xorb	$31, 33(%rax)
	xorb	$-128, 135(%rax)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_shake256_192_33$3:
	leaq	224(%rsp), %rsp
	movq	8(%rsp), %rcx
	movb	(%rax), %dl
	movb	%dl, (%rcx)
	movb	1(%rax), %dl
	movb	%dl, 1(%rcx)
	movb	2(%rax), %dl
	movb	%dl, 2(%rcx)
	movb	3(%rax), %dl
	movb	%dl, 3(%rcx)
	movb	4(%rax), %dl
	movb	%dl, 4(%rcx)
	movb	5(%rax), %dl
	movb	%dl, 5(%rcx)
	movb	6(%rax), %dl
	movb	%dl, 6(%rcx)
	movb	7(%rax), %dl
	movb	%dl, 7(%rcx)
	movb	8(%rax), %dl
	movb	%dl, 8(%rcx)
	movb	9(%rax), %dl
	movb	%dl, 9(%rcx)
	movb	10(%rax), %dl
	movb	%dl, 10(%rcx)
	movb	11(%rax), %dl
	movb	%dl, 11(%rcx)
	movb	12(%rax), %dl
	movb	%dl, 12(%rcx)
	movb	13(%rax), %dl
	movb	%dl, 13(%rcx)
	movb	14(%rax), %dl
	movb	%dl, 14(%rcx)
	movb	15(%rax), %dl
	movb	%dl, 15(%rcx)
	movb	16(%rax), %dl
	movb	%dl, 16(%rcx)
	movb	17(%rax), %dl
	movb	%dl, 17(%rcx)
	movb	18(%rax), %dl
	movb	%dl, 18(%rcx)
	movb	19(%rax), %dl
	movb	%dl, 19(%rcx)
	movb	20(%rax), %dl
	movb	%dl, 20(%rcx)
	movb	21(%rax), %dl
	movb	%dl, 21(%rcx)
	movb	22(%rax), %dl
	movb	%dl, 22(%rcx)
	movb	23(%rax), %dl
	movb	%dl, 23(%rcx)
	movb	24(%rax), %dl
	movb	%dl, 24(%rcx)
	movb	25(%rax), %dl
	movb	%dl, 25(%rcx)
	movb	26(%rax), %dl
	movb	%dl, 26(%rcx)
	movb	27(%rax), %dl
	movb	%dl, 27(%rcx)
	movb	28(%rax), %dl
	movb	%dl, 28(%rcx)
	movb	29(%rax), %dl
	movb	%dl, 29(%rcx)
	movb	30(%rax), %dl
	movb	%dl, 30(%rcx)
	movb	31(%rax), %dl
	movb	%dl, 31(%rcx)
	movb	32(%rax), %dl
	movb	%dl, 32(%rcx)
	movb	33(%rax), %dl
	movb	%dl, 33(%rcx)
	movb	34(%rax), %dl
	movb	%dl, 34(%rcx)
	movb	35(%rax), %dl
	movb	%dl, 35(%rcx)
	movb	36(%rax), %dl
	movb	%dl, 36(%rcx)
	movb	37(%rax), %dl
	movb	%dl, 37(%rcx)
	movb	38(%rax), %dl
	movb	%dl, 38(%rcx)
	movb	39(%rax), %dl
	movb	%dl, 39(%rcx)
	movb	40(%rax), %dl
	movb	%dl, 40(%rcx)
	movb	41(%rax), %dl
	movb	%dl, 41(%rcx)
	movb	42(%rax), %dl
	movb	%dl, 42(%rcx)
	movb	43(%rax), %dl
	movb	%dl, 43(%rcx)
	movb	44(%rax), %dl
	movb	%dl, 44(%rcx)
	movb	45(%rax), %dl
	movb	%dl, 45(%rcx)
	movb	46(%rax), %dl
	movb	%dl, 46(%rcx)
	movb	47(%rax), %dl
	movb	%dl, 47(%rcx)
	movb	48(%rax), %dl
	movb	%dl, 48(%rcx)
	movb	49(%rax), %dl
	movb	%dl, 49(%rcx)
	movb	50(%rax), %dl
	movb	%dl, 50(%rcx)
	movb	51(%rax), %dl
	movb	%dl, 51(%rcx)
	movb	52(%rax), %dl
	movb	%dl, 52(%rcx)
	movb	53(%rax), %dl
	movb	%dl, 53(%rcx)
	movb	54(%rax), %dl
	movb	%dl, 54(%rcx)
	movb	55(%rax), %dl
	movb	%dl, 55(%rcx)
	movb	56(%rax), %dl
	movb	%dl, 56(%rcx)
	movb	57(%rax), %dl
	movb	%dl, 57(%rcx)
	movb	58(%rax), %dl
	movb	%dl, 58(%rcx)
	movb	59(%rax), %dl
	movb	%dl, 59(%rcx)
	movb	60(%rax), %dl
	movb	%dl, 60(%rcx)
	movb	61(%rax), %dl
	movb	%dl, 61(%rcx)
	movb	62(%rax), %dl
	movb	%dl, 62(%rcx)
	movb	63(%rax), %dl
	movb	%dl, 63(%rcx)
	movb	64(%rax), %dl
	movb	%dl, 64(%rcx)
	movb	65(%rax), %dl
	movb	%dl, 65(%rcx)
	movb	66(%rax), %dl
	movb	%dl, 66(%rcx)
	movb	67(%rax), %dl
	movb	%dl, 67(%rcx)
	movb	68(%rax), %dl
	movb	%dl, 68(%rcx)
	movb	69(%rax), %dl
	movb	%dl, 69(%rcx)
	movb	70(%rax), %dl
	movb	%dl, 70(%rcx)
	movb	71(%rax), %dl
	movb	%dl, 71(%rcx)
	movb	72(%rax), %dl
	movb	%dl, 72(%rcx)
	movb	73(%rax), %dl
	movb	%dl, 73(%rcx)
	movb	74(%rax), %dl
	movb	%dl, 74(%rcx)
	movb	75(%rax), %dl
	movb	%dl, 75(%rcx)
	movb	76(%rax), %dl
	movb	%dl, 76(%rcx)
	movb	77(%rax), %dl
	movb	%dl, 77(%rcx)
	movb	78(%rax), %dl
	movb	%dl, 78(%rcx)
	movb	79(%rax), %dl
	movb	%dl, 79(%rcx)
	movb	80(%rax), %dl
	movb	%dl, 80(%rcx)
	movb	81(%rax), %dl
	movb	%dl, 81(%rcx)
	movb	82(%rax), %dl
	movb	%dl, 82(%rcx)
	movb	83(%rax), %dl
	movb	%dl, 83(%rcx)
	movb	84(%rax), %dl
	movb	%dl, 84(%rcx)
	movb	85(%rax), %dl
	movb	%dl, 85(%rcx)
	movb	86(%rax), %dl
	movb	%dl, 86(%rcx)
	movb	87(%rax), %dl
	movb	%dl, 87(%rcx)
	movb	88(%rax), %dl
	movb	%dl, 88(%rcx)
	movb	89(%rax), %dl
	movb	%dl, 89(%rcx)
	movb	90(%rax), %dl
	movb	%dl, 90(%rcx)
	movb	91(%rax), %dl
	movb	%dl, 91(%rcx)
	movb	92(%rax), %dl
	movb	%dl, 92(%rcx)
	movb	93(%rax), %dl
	movb	%dl, 93(%rcx)
	movb	94(%rax), %dl
	movb	%dl, 94(%rcx)
	movb	95(%rax), %dl
	movb	%dl, 95(%rcx)
	movb	96(%rax), %dl
	movb	%dl, 96(%rcx)
	movb	97(%rax), %dl
	movb	%dl, 97(%rcx)
	movb	98(%rax), %dl
	movb	%dl, 98(%rcx)
	movb	99(%rax), %dl
	movb	%dl, 99(%rcx)
	movb	100(%rax), %dl
	movb	%dl, 100(%rcx)
	movb	101(%rax), %dl
	movb	%dl, 101(%rcx)
	movb	102(%rax), %dl
	movb	%dl, 102(%rcx)
	movb	103(%rax), %dl
	movb	%dl, 103(%rcx)
	movb	104(%rax), %dl
	movb	%dl, 104(%rcx)
	movb	105(%rax), %dl
	movb	%dl, 105(%rcx)
	movb	106(%rax), %dl
	movb	%dl, 106(%rcx)
	movb	107(%rax), %dl
	movb	%dl, 107(%rcx)
	movb	108(%rax), %dl
	movb	%dl, 108(%rcx)
	movb	109(%rax), %dl
	movb	%dl, 109(%rcx)
	movb	110(%rax), %dl
	movb	%dl, 110(%rcx)
	movb	111(%rax), %dl
	movb	%dl, 111(%rcx)
	movb	112(%rax), %dl
	movb	%dl, 112(%rcx)
	movb	113(%rax), %dl
	movb	%dl, 113(%rcx)
	movb	114(%rax), %dl
	movb	%dl, 114(%rcx)
	movb	115(%rax), %dl
	movb	%dl, 115(%rcx)
	movb	116(%rax), %dl
	movb	%dl, 116(%rcx)
	movb	117(%rax), %dl
	movb	%dl, 117(%rcx)
	movb	118(%rax), %dl
	movb	%dl, 118(%rcx)
	movb	119(%rax), %dl
	movb	%dl, 119(%rcx)
	movb	120(%rax), %dl
	movb	%dl, 120(%rcx)
	movb	121(%rax), %dl
	movb	%dl, 121(%rcx)
	movb	122(%rax), %dl
	movb	%dl, 122(%rcx)
	movb	123(%rax), %dl
	movb	%dl, 123(%rcx)
	movb	124(%rax), %dl
	movb	%dl, 124(%rcx)
	movb	125(%rax), %dl
	movb	%dl, 125(%rcx)
	movb	126(%rax), %dl
	movb	%dl, 126(%rcx)
	movb	127(%rax), %dl
	movb	%dl, 127(%rcx)
	movb	128(%rax), %dl
	movb	%dl, 128(%rcx)
	movb	129(%rax), %dl
	movb	%dl, 129(%rcx)
	movb	130(%rax), %dl
	movb	%dl, 130(%rcx)
	movb	131(%rax), %dl
	movb	%dl, 131(%rcx)
	movb	132(%rax), %dl
	movb	%dl, 132(%rcx)
	movb	133(%rax), %dl
	movb	%dl, 133(%rcx)
	movb	134(%rax), %dl
	movb	%dl, 134(%rcx)
	movb	135(%rax), %dl
	movb	%dl, 135(%rcx)
	movq	%rcx, 8(%rsp)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_shake256_192_33$2:
	leaq	224(%rsp), %rsp
	movq	8(%rsp), %rcx
	movb	(%rax), %dl
	movb	%dl, 136(%rcx)
	movb	1(%rax), %dl
	movb	%dl, 137(%rcx)
	movb	2(%rax), %dl
	movb	%dl, 138(%rcx)
	movb	3(%rax), %dl
	movb	%dl, 139(%rcx)
	movb	4(%rax), %dl
	movb	%dl, 140(%rcx)
	movb	5(%rax), %dl
	movb	%dl, 141(%rcx)
	movb	6(%rax), %dl
	movb	%dl, 142(%rcx)
	movb	7(%rax), %dl
	movb	%dl, 143(%rcx)
	movb	8(%rax), %dl
	movb	%dl, 144(%rcx)
	movb	9(%rax), %dl
	movb	%dl, 145(%rcx)
	movb	10(%rax), %dl
	movb	%dl, 146(%rcx)
	movb	11(%rax), %dl
	movb	%dl, 147(%rcx)
	movb	12(%rax), %dl
	movb	%dl, 148(%rcx)
	movb	13(%rax), %dl
	movb	%dl, 149(%rcx)
	movb	14(%rax), %dl
	movb	%dl, 150(%rcx)
	movb	15(%rax), %dl
	movb	%dl, 151(%rcx)
	movb	16(%rax), %dl
	movb	%dl, 152(%rcx)
	movb	17(%rax), %dl
	movb	%dl, 153(%rcx)
	movb	18(%rax), %dl
	movb	%dl, 154(%rcx)
	movb	19(%rax), %dl
	movb	%dl, 155(%rcx)
	movb	20(%rax), %dl
	movb	%dl, 156(%rcx)
	movb	21(%rax), %dl
	movb	%dl, 157(%rcx)
	movb	22(%rax), %dl
	movb	%dl, 158(%rcx)
	movb	23(%rax), %dl
	movb	%dl, 159(%rcx)
	movb	24(%rax), %dl
	movb	%dl, 160(%rcx)
	movb	25(%rax), %dl
	movb	%dl, 161(%rcx)
	movb	26(%rax), %dl
	movb	%dl, 162(%rcx)
	movb	27(%rax), %dl
	movb	%dl, 163(%rcx)
	movb	28(%rax), %dl
	movb	%dl, 164(%rcx)
	movb	29(%rax), %dl
	movb	%dl, 165(%rcx)
	movb	30(%rax), %dl
	movb	%dl, 166(%rcx)
	movb	31(%rax), %dl
	movb	%dl, 167(%rcx)
	movb	32(%rax), %dl
	movb	%dl, 168(%rcx)
	movb	33(%rax), %dl
	movb	%dl, 169(%rcx)
	movb	34(%rax), %dl
	movb	%dl, 170(%rcx)
	movb	35(%rax), %dl
	movb	%dl, 171(%rcx)
	movb	36(%rax), %dl
	movb	%dl, 172(%rcx)
	movb	37(%rax), %dl
	movb	%dl, 173(%rcx)
	movb	38(%rax), %dl
	movb	%dl, 174(%rcx)
	movb	39(%rax), %dl
	movb	%dl, 175(%rcx)
	movb	40(%rax), %dl
	movb	%dl, 176(%rcx)
	movb	41(%rax), %dl
	movb	%dl, 177(%rcx)
	movb	42(%rax), %dl
	movb	%dl, 178(%rcx)
	movb	43(%rax), %dl
	movb	%dl, 179(%rcx)
	movb	44(%rax), %dl
	movb	%dl, 180(%rcx)
	movb	45(%rax), %dl
	movb	%dl, 181(%rcx)
	movb	46(%rax), %dl
	movb	%dl, 182(%rcx)
	movb	47(%rax), %dl
	movb	%dl, 183(%rcx)
	movb	48(%rax), %dl
	movb	%dl, 184(%rcx)
	movb	49(%rax), %dl
	movb	%dl, 185(%rcx)
	movb	50(%rax), %dl
	movb	%dl, 186(%rcx)
	movb	51(%rax), %dl
	movb	%dl, 187(%rcx)
	movb	52(%rax), %dl
	movb	%dl, 188(%rcx)
	movb	53(%rax), %dl
	movb	%dl, 189(%rcx)
	movb	54(%rax), %dl
	movb	%dl, 190(%rcx)
	movb	55(%rax), %al
	movb	%al, 191(%rcx)
	ret
L_shake256_128_33$1:
	movq	%rax, 8(%rsp)
	leaq	16(%rsp), %rax
	xorq	%rdx, %rdx
	movq	%rdx, (%rax)
	movq	%rdx, 8(%rax)
	movq	%rdx, 16(%rax)
	movq	%rdx, 24(%rax)
	movq	%rdx, 32(%rax)
	movq	%rdx, 40(%rax)
	movq	%rdx, 48(%rax)
	movq	%rdx, 56(%rax)
	movq	%rdx, 64(%rax)
	movq	%rdx, 72(%rax)
	movq	%rdx, 80(%rax)
	movq	%rdx, 88(%rax)
	movq	%rdx, 96(%rax)
	movq	%rdx, 104(%rax)
	movq	%rdx, 112(%rax)
	movq	%rdx, 120(%rax)
	movq	%rdx, 128(%rax)
	movq	%rdx, 136(%rax)
	movq	%rdx, 144(%rax)
	movq	%rdx, 152(%rax)
	movq	%rdx, 160(%rax)
	movq	%rdx, 168(%rax)
	movq	%rdx, 176(%rax)
	movq	%rdx, 184(%rax)
	movq	%rdx, 192(%rax)
	movb	(%rcx), %dl
	xorb	%dl, (%rax)
	movb	1(%rcx), %dl
	xorb	%dl, 1(%rax)
	movb	2(%rcx), %dl
	xorb	%dl, 2(%rax)
	movb	3(%rcx), %dl
	xorb	%dl, 3(%rax)
	movb	4(%rcx), %dl
	xorb	%dl, 4(%rax)
	movb	5(%rcx), %dl
	xorb	%dl, 5(%rax)
	movb	6(%rcx), %dl
	xorb	%dl, 6(%rax)
	movb	7(%rcx), %dl
	xorb	%dl, 7(%rax)
	movb	8(%rcx), %dl
	xorb	%dl, 8(%rax)
	movb	9(%rcx), %dl
	xorb	%dl, 9(%rax)
	movb	10(%rcx), %dl
	xorb	%dl, 10(%rax)
	movb	11(%rcx), %dl
	xorb	%dl, 11(%rax)
	movb	12(%rcx), %dl
	xorb	%dl, 12(%rax)
	movb	13(%rcx), %dl
	xorb	%dl, 13(%rax)
	movb	14(%rcx), %dl
	xorb	%dl, 14(%rax)
	movb	15(%rcx), %dl
	xorb	%dl, 15(%rax)
	movb	16(%rcx), %dl
	xorb	%dl, 16(%rax)
	movb	17(%rcx), %dl
	xorb	%dl, 17(%rax)
	movb	18(%rcx), %dl
	xorb	%dl, 18(%rax)
	movb	19(%rcx), %dl
	xorb	%dl, 19(%rax)
	movb	20(%rcx), %dl
	xorb	%dl, 20(%rax)
	movb	21(%rcx), %dl
	xorb	%dl, 21(%rax)
	movb	22(%rcx), %dl
	xorb	%dl, 22(%rax)
	movb	23(%rcx), %dl
	xorb	%dl, 23(%rax)
	movb	24(%rcx), %dl
	xorb	%dl, 24(%rax)
	movb	25(%rcx), %dl
	xorb	%dl, 25(%rax)
	movb	26(%rcx), %dl
	xorb	%dl, 26(%rax)
	movb	27(%rcx), %dl
	xorb	%dl, 27(%rax)
	movb	28(%rcx), %dl
	xorb	%dl, 28(%rax)
	movb	29(%rcx), %dl
	xorb	%dl, 29(%rax)
	movb	30(%rcx), %dl
	xorb	%dl, 30(%rax)
	movb	31(%rcx), %dl
	xorb	%dl, 31(%rax)
	movb	32(%rcx), %cl
	xorb	%cl, 32(%rax)
	xorb	$31, 33(%rax)
	xorb	$-128, 135(%rax)
	leaq	-224(%rsp), %rsp
	call	L_keccakf1600_ref1$1
L_shake256_128_33$2:
	leaq	224(%rsp), %rsp
	movq	8(%rsp), %rcx
	movb	(%rax), %dl
	movb	%dl, (%rcx)
	movb	1(%rax), %dl
	movb	%dl, 1(%rcx)
	movb	2(%rax), %dl
	movb	%dl, 2(%rcx)
	movb	3(%rax), %dl
	movb	%dl, 3(%rcx)
	movb	4(%rax), %dl
	movb	%dl, 4(%rcx)
	movb	5(%rax), %dl
	movb	%dl, 5(%rcx)
	movb	6(%rax), %dl
	movb	%dl, 6(%rcx)
	movb	7(%rax), %dl
	movb	%dl, 7(%rcx)
	movb	8(%rax), %dl
	movb	%dl, 8(%rcx)
	movb	9(%rax), %dl
	movb	%dl, 9(%rcx)
	movb	10(%rax), %dl
	movb	%dl, 10(%rcx)
	movb	11(%rax), %dl
	movb	%dl, 11(%rcx)
	movb	12(%rax), %dl
	movb	%dl, 12(%rcx)
	movb	13(%rax), %dl
	movb	%dl, 13(%rcx)
	movb	14(%rax), %dl
	movb	%dl, 14(%rcx)
	movb	15(%rax), %dl
	movb	%dl, 15(%rcx)
	movb	16(%rax), %dl
	movb	%dl, 16(%rcx)
	movb	17(%rax), %dl
	movb	%dl, 17(%rcx)
	movb	18(%rax), %dl
	movb	%dl, 18(%rcx)
	movb	19(%rax), %dl
	movb	%dl, 19(%rcx)
	movb	20(%rax), %dl
	movb	%dl, 20(%rcx)
	movb	21(%rax), %dl
	movb	%dl, 21(%rcx)
	movb	22(%rax), %dl
	movb	%dl, 22(%rcx)
	movb	23(%rax), %dl
	movb	%dl, 23(%rcx)
	movb	24(%rax), %dl
	movb	%dl, 24(%rcx)
	movb	25(%rax), %dl
	movb	%dl, 25(%rcx)
	movb	26(%rax), %dl
	movb	%dl, 26(%rcx)
	movb	27(%rax), %dl
	movb	%dl, 27(%rcx)
	movb	28(%rax), %dl
	movb	%dl, 28(%rcx)
	movb	29(%rax), %dl
	movb	%dl, 29(%rcx)
	movb	30(%rax), %dl
	movb	%dl, 30(%rcx)
	movb	31(%rax), %dl
	movb	%dl, 31(%rcx)
	movb	32(%rax), %dl
	movb	%dl, 32(%rcx)
	movb	33(%rax), %dl
	movb	%dl, 33(%rcx)
	movb	34(%rax), %dl
	movb	%dl, 34(%rcx)
	movb	35(%rax), %dl
	movb	%dl, 35(%rcx)
	movb	36(%rax), %dl
	movb	%dl, 36(%rcx)
	movb	37(%rax), %dl
	movb	%dl, 37(%rcx)
	movb	38(%rax), %dl
	movb	%dl, 38(%rcx)
	movb	39(%rax), %dl
	movb	%dl, 39(%rcx)
	movb	40(%rax), %dl
	movb	%dl, 40(%rcx)
	movb	41(%rax), %dl
	movb	%dl, 41(%rcx)
	movb	42(%rax), %dl
	movb	%dl, 42(%rcx)
	movb	43(%rax), %dl
	movb	%dl, 43(%rcx)
	movb	44(%rax), %dl
	movb	%dl, 44(%rcx)
	movb	45(%rax), %dl
	movb	%dl, 45(%rcx)
	movb	46(%rax), %dl
	movb	%dl, 46(%rcx)
	movb	47(%rax), %dl
	movb	%dl, 47(%rcx)
	movb	48(%rax), %dl
	movb	%dl, 48(%rcx)
	movb	49(%rax), %dl
	movb	%dl, 49(%rcx)
	movb	50(%rax), %dl
	movb	%dl, 50(%rcx)
	movb	51(%rax), %dl
	movb	%dl, 51(%rcx)
	movb	52(%rax), %dl
	movb	%dl, 52(%rcx)
	movb	53(%rax), %dl
	movb	%dl, 53(%rcx)
	movb	54(%rax), %dl
	movb	%dl, 54(%rcx)
	movb	55(%rax), %dl
	movb	%dl, 55(%rcx)
	movb	56(%rax), %dl
	movb	%dl, 56(%rcx)
	movb	57(%rax), %dl
	movb	%dl, 57(%rcx)
	movb	58(%rax), %dl
	movb	%dl, 58(%rcx)
	movb	59(%rax), %dl
	movb	%dl, 59(%rcx)
	movb	60(%rax), %dl
	movb	%dl, 60(%rcx)
	movb	61(%rax), %dl
	movb	%dl, 61(%rcx)
	movb	62(%rax), %dl
	movb	%dl, 62(%rcx)
	movb	63(%rax), %dl
	movb	%dl, 63(%rcx)
	movb	64(%rax), %dl
	movb	%dl, 64(%rcx)
	movb	65(%rax), %dl
	movb	%dl, 65(%rcx)
	movb	66(%rax), %dl
	movb	%dl, 66(%rcx)
	movb	67(%rax), %dl
	movb	%dl, 67(%rcx)
	movb	68(%rax), %dl
	movb	%dl, 68(%rcx)
	movb	69(%rax), %dl
	movb	%dl, 69(%rcx)
	movb	70(%rax), %dl
	movb	%dl, 70(%rcx)
	movb	71(%rax), %dl
	movb	%dl, 71(%rcx)
	movb	72(%rax), %dl
	movb	%dl, 72(%rcx)
	movb	73(%rax), %dl
	movb	%dl, 73(%rcx)
	movb	74(%rax), %dl
	movb	%dl, 74(%rcx)
	movb	75(%rax), %dl
	movb	%dl, 75(%rcx)
	movb	76(%rax), %dl
	movb	%dl, 76(%rcx)
	movb	77(%rax), %dl
	movb	%dl, 77(%rcx)
	movb	78(%rax), %dl
	movb	%dl, 78(%rcx)
	movb	79(%rax), %dl
	movb	%dl, 79(%rcx)
	movb	80(%rax), %dl
	movb	%dl, 80(%rcx)
	movb	81(%rax), %dl
	movb	%dl, 81(%rcx)
	movb	82(%rax), %dl
	movb	%dl, 82(%rcx)
	movb	83(%rax), %dl
	movb	%dl, 83(%rcx)
	movb	84(%rax), %dl
	movb	%dl, 84(%rcx)
	movb	85(%rax), %dl
	movb	%dl, 85(%rcx)
	movb	86(%rax), %dl
	movb	%dl, 86(%rcx)
	movb	87(%rax), %dl
	movb	%dl, 87(%rcx)
	movb	88(%rax), %dl
	movb	%dl, 88(%rcx)
	movb	89(%rax), %dl
	movb	%dl, 89(%rcx)
	movb	90(%rax), %dl
	movb	%dl, 90(%rcx)
	movb	91(%rax), %dl
	movb	%dl, 91(%rcx)
	movb	92(%rax), %dl
	movb	%dl, 92(%rcx)
	movb	93(%rax), %dl
	movb	%dl, 93(%rcx)
	movb	94(%rax), %dl
	movb	%dl, 94(%rcx)
	movb	95(%rax), %dl
	movb	%dl, 95(%rcx)
	movb	96(%rax), %dl
	movb	%dl, 96(%rcx)
	movb	97(%rax), %dl
	movb	%dl, 97(%rcx)
	movb	98(%rax), %dl
	movb	%dl, 98(%rcx)
	movb	99(%rax), %dl
	movb	%dl, 99(%rcx)
	movb	100(%rax), %dl
	movb	%dl, 100(%rcx)
	movb	101(%rax), %dl
	movb	%dl, 101(%rcx)
	movb	102(%rax), %dl
	movb	%dl, 102(%rcx)
	movb	103(%rax), %dl
	movb	%dl, 103(%rcx)
	movb	104(%rax), %dl
	movb	%dl, 104(%rcx)
	movb	105(%rax), %dl
	movb	%dl, 105(%rcx)
	movb	106(%rax), %dl
	movb	%dl, 106(%rcx)
	movb	107(%rax), %dl
	movb	%dl, 107(%rcx)
	movb	108(%rax), %dl
	movb	%dl, 108(%rcx)
	movb	109(%rax), %dl
	movb	%dl, 109(%rcx)
	movb	110(%rax), %dl
	movb	%dl, 110(%rcx)
	movb	111(%rax), %dl
	movb	%dl, 111(%rcx)
	movb	112(%rax), %dl
	movb	%dl, 112(%rcx)
	movb	113(%rax), %dl
	movb	%dl, 113(%rcx)
	movb	114(%rax), %dl
	movb	%dl, 114(%rcx)
	movb	115(%rax), %dl
	movb	%dl, 115(%rcx)
	movb	116(%rax), %dl
	movb	%dl, 116(%rcx)
	movb	117(%rax), %dl
	movb	%dl, 117(%rcx)
	movb	118(%rax), %dl
	movb	%dl, 118(%rcx)
	movb	119(%rax), %dl
	movb	%dl, 119(%rcx)
	movb	120(%rax), %dl
	movb	%dl, 120(%rcx)
	movb	121(%rax), %dl
	movb	%dl, 121(%rcx)
	movb	122(%rax), %dl
	movb	%dl, 122(%rcx)
	movb	123(%rax), %dl
	movb	%dl, 123(%rcx)
	movb	124(%rax), %dl
	movb	%dl, 124(%rcx)
	movb	125(%rax), %dl
	movb	%dl, 125(%rcx)
	movb	126(%rax), %dl
	movb	%dl, 126(%rcx)
	movb	127(%rax), %al
	movb	%al, 127(%rcx)
	ret
L_keccakf1600_ref1$1:
	leaq	glob_data + 0(%rip), %rcx
	movq	%rcx, 8(%rsp)
	leaq	32(%rsp), %rcx
	movq	$0, %r10
	jmp 	L_keccakf1600_ref1$2
L_keccakf1600_ref1$3:
	movq	%r10, 16(%rsp)
	movq	8(%rsp), %rdx
	movq	(%rdx,%r10,8), %rdx
	movq	%rdx, 24(%rsp)
	movq	(%rax), %r9
	movq	8(%rax), %r8
	movq	16(%rax), %r11
	movq	24(%rax), %rbx
	movq	32(%rax), %rbp
	xorq	40(%rax), %r9
	xorq	48(%rax), %r8
	xorq	56(%rax), %r11
	xorq	64(%rax), %rbx
	xorq	72(%rax), %rbp
	xorq	80(%rax), %r9
	xorq	88(%rax), %r8
	xorq	96(%rax), %r11
	xorq	104(%rax), %rbx
	xorq	112(%rax), %rbp
	xorq	120(%rax), %r9
	xorq	128(%rax), %r8
	xorq	136(%rax), %r11
	xorq	144(%rax), %rbx
	xorq	152(%rax), %rbp
	xorq	160(%rax), %r9
	xorq	168(%rax), %r8
	xorq	176(%rax), %r11
	xorq	184(%rax), %rbx
	xorq	192(%rax), %rbp
	movq	%r8, %rdx
	rolq	$1, %rdx
	xorq	%rbp, %rdx
	movq	%r11, %rsi
	rolq	$1, %rsi
	xorq	%r9, %rsi
	movq	%rbx, %rdi
	rolq	$1, %rdi
	xorq	%r8, %rdi
	movq	%rbp, %r8
	rolq	$1, %r8
	xorq	%r11, %r8
	rolq	$1, %r9
	xorq	%rbx, %r9
	movq	(%rax), %r11
	xorq	%rdx, %r11
	movq	48(%rax), %rbx
	xorq	%rsi, %rbx
	rolq	$44, %rbx
	movq	96(%rax), %rbp
	xorq	%rdi, %rbp
	rolq	$43, %rbp
	movq	144(%rax), %r12
	xorq	%r8, %r12
	rolq	$21, %r12
	movq	192(%rax), %r13
	xorq	%r9, %r13
	rolq	$14, %r13
	movq	%rbx, %r14
	notq	%r14
	andq	%rbp, %r14
	xorq	%r11, %r14
	xorq	24(%rsp), %r14
	movq	%r14, (%rcx)
	movq	%rbp, %r14
	notq	%r14
	andq	%r12, %r14
	xorq	%rbx, %r14
	movq	%r14, 8(%rcx)
	movq	%r12, %r14
	notq	%r14
	andq	%r13, %r14
	xorq	%rbp, %r14
	movq	%r14, 16(%rcx)
	movq	%r13, %rbp
	notq	%rbp
	andq	%r11, %rbp
	xorq	%r12, %rbp
	movq	%rbp, 24(%rcx)
	notq	%r11
	andq	%rbx, %r11
	xorq	%r13, %r11
	movq	%r11, 32(%rcx)
	movq	24(%rax), %r11
	xorq	%r8, %r11
	rolq	$28, %r11
	movq	72(%rax), %rbx
	xorq	%r9, %rbx
	rolq	$20, %rbx
	movq	80(%rax), %rbp
	xorq	%rdx, %rbp
	rolq	$3, %rbp
	movq	128(%rax), %r12
	xorq	%rsi, %r12
	rolq	$45, %r12
	movq	176(%rax), %r13
	xorq	%rdi, %r13
	rolq	$61, %r13
	movq	%rbx, %r14
	notq	%r14
	andq	%rbp, %r14
	xorq	%r11, %r14
	movq	%r14, 40(%rcx)
	movq	%rbp, %r14
	notq	%r14
	andq	%r12, %r14
	xorq	%rbx, %r14
	movq	%r14, 48(%rcx)
	movq	%r12, %r14
	notq	%r14
	andq	%r13, %r14
	xorq	%rbp, %r14
	movq	%r14, 56(%rcx)
	movq	%r13, %rbp
	notq	%rbp
	andq	%r11, %rbp
	xorq	%r12, %rbp
	movq	%rbp, 64(%rcx)
	notq	%r11
	andq	%rbx, %r11
	xorq	%r13, %r11
	movq	%r11, 72(%rcx)
	movq	8(%rax), %r11
	xorq	%rsi, %r11
	rolq	$1, %r11
	movq	56(%rax), %rbx
	xorq	%rdi, %rbx
	rolq	$6, %rbx
	movq	104(%rax), %rbp
	xorq	%r8, %rbp
	rolq	$25, %rbp
	movq	152(%rax), %r12
	xorq	%r9, %r12
	rolq	$8, %r12
	movq	160(%rax), %r13
	xorq	%rdx, %r13
	rolq	$18, %r13
	movq	%rbx, %r14
	notq	%r14
	andq	%rbp, %r14
	xorq	%r11, %r14
	movq	%r14, 80(%rcx)
	movq	%rbp, %r14
	notq	%r14
	andq	%r12, %r14
	xorq	%rbx, %r14
	movq	%r14, 88(%rcx)
	movq	%r12, %r14
	notq	%r14
	andq	%r13, %r14
	xorq	%rbp, %r14
	movq	%r14, 96(%rcx)
	movq	%r13, %rbp
	notq	%rbp
	andq	%r11, %rbp
	xorq	%r12, %rbp
	movq	%rbp, 104(%rcx)
	notq	%r11
	andq	%rbx, %r11
	xorq	%r13, %r11
	movq	%r11, 112(%rcx)
	movq	32(%rax), %r11
	xorq	%r9, %r11
	rolq	$27, %r11
	movq	40(%rax), %rbx
	xorq	%rdx, %rbx
	rolq	$36, %rbx
	movq	88(%rax), %rbp
	xorq	%rsi, %rbp
	rolq	$10, %rbp
	movq	136(%rax), %r12
	xorq	%rdi, %r12
	rolq	$15, %r12
	movq	184(%rax), %r13
	xorq	%r8, %r13
	rolq	$56, %r13
	movq	%rbx, %r14
	notq	%r14
	andq	%rbp, %r14
	xorq	%r11, %r14
	movq	%r14, 120(%rcx)
	movq	%rbp, %r14
	notq	%r14
	andq	%r12, %r14
	xorq	%rbx, %r14
	movq	%r14, 128(%rcx)
	movq	%r12, %r14
	notq	%r14
	andq	%r13, %r14
	xorq	%rbp, %r14
	movq	%r14, 136(%rcx)
	movq	%r13, %rbp
	notq	%rbp
	andq	%r11, %rbp
	xorq	%r12, %rbp
	movq	%rbp, 144(%rcx)
	notq	%r11
	andq	%rbx, %r11
	xorq	%r13, %r11
	movq	%r11, 152(%rcx)
	movq	16(%rax), %r11
	xorq	%rdi, %r11
	rolq	$62, %r11
	movq	64(%rax), %rdi
	xorq	%r8, %rdi
	rolq	$55, %rdi
	movq	112(%rax), %r8
	xorq	%r9, %r8
	rolq	$39, %r8
	movq	120(%rax), %r9
	xorq	%rdx, %r9
	rolq	$41, %r9
	movq	168(%rax), %rdx
	xorq	%rsi, %rdx
	rolq	$2, %rdx
	movq	%rdi, %rsi
	notq	%rsi
	andq	%r8, %rsi
	xorq	%r11, %rsi
	movq	%rsi, 160(%rcx)
	movq	%r8, %rsi
	notq	%rsi
	andq	%r9, %rsi
	xorq	%rdi, %rsi
	movq	%rsi, 168(%rcx)
	movq	%r9, %rsi
	notq	%rsi
	andq	%rdx, %rsi
	xorq	%r8, %rsi
	movq	%rsi, 176(%rcx)
	movq	%rdx, %rsi
	notq	%rsi
	andq	%r11, %rsi
	xorq	%r9, %rsi
	movq	%rsi, 184(%rcx)
	notq	%r11
	andq	%rdi, %r11
	xorq	%rdx, %r11
	movq	%r11, 192(%rcx)
	movq	8(%rsp), %rdx
	movq	8(%rdx,%r10,8), %rdx
	movq	%rdx, 24(%rsp)
	movq	(%rcx), %r9
	movq	8(%rcx), %r8
	movq	16(%rcx), %r10
	movq	24(%rcx), %r11
	movq	32(%rcx), %rbx
	xorq	40(%rcx), %r9
	xorq	48(%rcx), %r8
	xorq	56(%rcx), %r10
	xorq	64(%rcx), %r11
	xorq	72(%rcx), %rbx
	xorq	80(%rcx), %r9
	xorq	88(%rcx), %r8
	xorq	96(%rcx), %r10
	xorq	104(%rcx), %r11
	xorq	112(%rcx), %rbx
	xorq	120(%rcx), %r9
	xorq	128(%rcx), %r8
	xorq	136(%rcx), %r10
	xorq	144(%rcx), %r11
	xorq	152(%rcx), %rbx
	xorq	160(%rcx), %r9
	xorq	168(%rcx), %r8
	xorq	176(%rcx), %r10
	xorq	184(%rcx), %r11
	xorq	192(%rcx), %rbx
	movq	%r8, %rdx
	rolq	$1, %rdx
	xorq	%rbx, %rdx
	movq	%r10, %rsi
	rolq	$1, %rsi
	xorq	%r9, %rsi
	movq	%r11, %rdi
	rolq	$1, %rdi
	xorq	%r8, %rdi
	movq	%rbx, %r8
	rolq	$1, %r8
	xorq	%r10, %r8
	rolq	$1, %r9
	xorq	%r11, %r9
	movq	(%rcx), %r10
	xorq	%rdx, %r10
	movq	48(%rcx), %r11
	xorq	%rsi, %r11
	rolq	$44, %r11
	movq	96(%rcx), %r12
	xorq	%rdi, %r12
	rolq	$43, %r12
	movq	144(%rcx), %rbx
	xorq	%r8, %rbx
	rolq	$21, %rbx
	movq	192(%rcx), %rbp
	xorq	%r9, %rbp
	rolq	$14, %rbp
	movq	%r11, %r13
	notq	%r13
	andq	%r12, %r13
	xorq	%r10, %r13
	xorq	24(%rsp), %r13
	movq	%r13, (%rax)
	movq	%r12, %r13
	notq	%r13
	andq	%rbx, %r13
	xorq	%r11, %r13
	movq	%r13, 8(%rax)
	movq	%rbx, %r13
	notq	%r13
	andq	%rbp, %r13
	xorq	%r12, %r13
	movq	%r13, 16(%rax)
	movq	%rbp, %r12
	notq	%r12
	andq	%r10, %r12
	xorq	%rbx, %r12
	movq	%r12, 24(%rax)
	notq	%r10
	andq	%r11, %r10
	xorq	%rbp, %r10
	movq	%r10, 32(%rax)
	movq	24(%rcx), %r10
	xorq	%r8, %r10
	rolq	$28, %r10
	movq	72(%rcx), %r11
	xorq	%r9, %r11
	rolq	$20, %r11
	movq	80(%rcx), %r12
	xorq	%rdx, %r12
	rolq	$3, %r12
	movq	128(%rcx), %rbx
	xorq	%rsi, %rbx
	rolq	$45, %rbx
	movq	176(%rcx), %rbp
	xorq	%rdi, %rbp
	rolq	$61, %rbp
	movq	%r11, %r13
	notq	%r13
	andq	%r12, %r13
	xorq	%r10, %r13
	movq	%r13, 40(%rax)
	movq	%r12, %r13
	notq	%r13
	andq	%rbx, %r13
	xorq	%r11, %r13
	movq	%r13, 48(%rax)
	movq	%rbx, %r13
	notq	%r13
	andq	%rbp, %r13
	xorq	%r12, %r13
	movq	%r13, 56(%rax)
	movq	%rbp, %r12
	notq	%r12
	andq	%r10, %r12
	xorq	%rbx, %r12
	movq	%r12, 64(%rax)
	notq	%r10
	andq	%r11, %r10
	xorq	%rbp, %r10
	movq	%r10, 72(%rax)
	movq	8(%rcx), %r10
	xorq	%rsi, %r10
	rolq	$1, %r10
	movq	56(%rcx), %r11
	xorq	%rdi, %r11
	rolq	$6, %r11
	movq	104(%rcx), %r12
	xorq	%r8, %r12
	rolq	$25, %r12
	movq	152(%rcx), %rbx
	xorq	%r9, %rbx
	rolq	$8, %rbx
	movq	160(%rcx), %rbp
	xorq	%rdx, %rbp
	rolq	$18, %rbp
	movq	%r11, %r13
	notq	%r13
	andq	%r12, %r13
	xorq	%r10, %r13
	movq	%r13, 80(%rax)
	movq	%r12, %r13
	notq	%r13
	andq	%rbx, %r13
	xorq	%r11, %r13
	movq	%r13, 88(%rax)
	movq	%rbx, %r13
	notq	%r13
	andq	%rbp, %r13
	xorq	%r12, %r13
	movq	%r13, 96(%rax)
	movq	%rbp, %r12
	notq	%r12
	andq	%r10, %r12
	xorq	%rbx, %r12
	movq	%r12, 104(%rax)
	notq	%r10
	andq	%r11, %r10
	xorq	%rbp, %r10
	movq	%r10, 112(%rax)
	movq	32(%rcx), %r10
	xorq	%r9, %r10
	rolq	$27, %r10
	movq	40(%rcx), %r11
	xorq	%rdx, %r11
	rolq	$36, %r11
	movq	88(%rcx), %r12
	xorq	%rsi, %r12
	rolq	$10, %r12
	movq	136(%rcx), %rbx
	xorq	%rdi, %rbx
	rolq	$15, %rbx
	movq	184(%rcx), %rbp
	xorq	%r8, %rbp
	rolq	$56, %rbp
	movq	%r11, %r13
	notq	%r13
	andq	%r12, %r13
	xorq	%r10, %r13
	movq	%r13, 120(%rax)
	movq	%r12, %r13
	notq	%r13
	andq	%rbx, %r13
	xorq	%r11, %r13
	movq	%r13, 128(%rax)
	movq	%rbx, %r13
	notq	%r13
	andq	%rbp, %r13
	xorq	%r12, %r13
	movq	%r13, 136(%rax)
	movq	%rbp, %r12
	notq	%r12
	andq	%r10, %r12
	xorq	%rbx, %r12
	movq	%r12, 144(%rax)
	notq	%r10
	andq	%r11, %r10
	xorq	%rbp, %r10
	movq	%r10, 152(%rax)
	movq	16(%rcx), %r10
	xorq	%rdi, %r10
	rolq	$62, %r10
	movq	64(%rcx), %rdi
	xorq	%r8, %rdi
	rolq	$55, %rdi
	movq	112(%rcx), %r8
	xorq	%r9, %r8
	rolq	$39, %r8
	movq	120(%rcx), %r9
	xorq	%rdx, %r9
	rolq	$41, %r9
	movq	168(%rcx), %rdx
	xorq	%rsi, %rdx
	rolq	$2, %rdx
	movq	%rdi, %rsi
	notq	%rsi
	andq	%r8, %rsi
	xorq	%r10, %rsi
	movq	%rsi, 160(%rax)
	movq	%r8, %rsi
	notq	%rsi
	andq	%r9, %rsi
	xorq	%rdi, %rsi
	movq	%rsi, 168(%rax)
	movq	%r9, %rsi
	notq	%rsi
	andq	%rdx, %rsi
	xorq	%r8, %rsi
	movq	%rsi, 176(%rax)
	movq	%rdx, %rsi
	notq	%rsi
	andq	%r10, %rsi
	xorq	%r9, %rsi
	movq	%rsi, 184(%rax)
	notq	%r10
	andq	%rdi, %r10
	xorq	%rdx, %r10
	movq	%r10, 192(%rax)
	movq	16(%rsp), %r10
	addq	$2, %r10
L_keccakf1600_ref1$2:
	cmpq	$23, %r10
	jb  	L_keccakf1600_ref1$3
	ret
	.data
	.p2align	5
_glob_data:
glob_data:
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -126
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -118
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -117
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -127
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 9
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -118
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -120
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 9
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 10
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -117
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -117
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -119
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 3
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 2
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 10
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 10
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -127
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -128
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 1
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 8
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -91
      .byte 6
      .byte 15
      .byte 7
      .byte -76
      .byte 5
      .byte 67
      .byte 9
      .byte 34
      .byte 9
      .byte 29
      .byte 9
      .byte 52
      .byte 1
      .byte 108
      .byte 0
      .byte 35
      .byte 11
      .byte 102
      .byte 3
      .byte 86
      .byte 3
      .byte -26
      .byte 5
      .byte -25
      .byte 9
      .byte -2
      .byte 4
      .byte -6
      .byte 5
      .byte -95
      .byte 4
      .byte 123
      .byte 6
      .byte -93
      .byte 4
      .byte 37
      .byte 12
      .byte 106
      .byte 3
      .byte 55
      .byte 5
      .byte 63
      .byte 8
      .byte -120
      .byte 0
      .byte -65
      .byte 4
      .byte -127
      .byte 11
      .byte -71
      .byte 5
      .byte 5
      .byte 5
      .byte -41
      .byte 7
      .byte -97
      .byte 10
      .byte -90
      .byte 10
      .byte -72
      .byte 8
      .byte -48
      .byte 9
      .byte 75
      .byte 0
      .byte -100
      .byte 0
      .byte -72
      .byte 11
      .byte 95
      .byte 11
      .byte -92
      .byte 11
      .byte 104
      .byte 3
      .byte 125
      .byte 10
      .byte 54
      .byte 6
      .byte -94
      .byte 8
      .byte 90
      .byte 2
      .byte 54
      .byte 7
      .byte 9
      .byte 3
      .byte -109
      .byte 0
      .byte 122
      .byte 8
      .byte -9
      .byte 9
      .byte -10
      .byte 0
      .byte -116
      .byte 6
      .byte -37
      .byte 6
      .byte -52
      .byte 1
      .byte 35
      .byte 1
      .byte -21
      .byte 0
      .byte 80
      .byte 12
      .byte -74
      .byte 10
      .byte 91
      .byte 11
      .byte -104
      .byte 12
      .byte -13
      .byte 6
      .byte -102
      .byte 9
      .byte -29
      .byte 4
      .byte -74
      .byte 9
      .byte -42
      .byte 10
      .byte 83
      .byte 11
      .byte 79
      .byte 4
      .byte -5
      .byte 4
      .byte 92
      .byte 10
      .byte 41
      .byte 4
      .byte 65
      .byte 11
      .byte -43
      .byte 2
      .byte -28
      .byte 5
      .byte 64
      .byte 9
      .byte -114
      .byte 1
      .byte -73
      .byte 3
      .byte -9
      .byte 0
      .byte -115
      .byte 5
      .byte -106
      .byte 12
      .byte -61
      .byte 9
      .byte 15
      .byte 1
      .byte 90
      .byte 0
      .byte 85
      .byte 3
      .byte 68
      .byte 7
      .byte -125
      .byte 12
      .byte -118
      .byte 4
      .byte 82
      .byte 6
      .byte -102
      .byte 2
      .byte 64
      .byte 1
      .byte 8
      .byte 0
      .byte -3
      .byte 10
      .byte 8
      .byte 6
      .byte 26
      .byte 1
      .byte 46
      .byte 7
      .byte 13
      .byte 5
      .byte 10
      .byte 9
      .byte 40
      .byte 2
      .byte 117
      .byte 10
      .byte 58
      .byte 8
      .byte 35
      .byte 6
      .byte -51
      .byte 0
      .byte 102
      .byte 11
      .byte 6
      .byte 6
      .byte -95
      .byte 10
      .byte 37
      .byte 10
      .byte 8
      .byte 9
      .byte -87
      .byte 2
      .byte -126
      .byte 0
      .byte 66
      .byte 6
      .byte 79
      .byte 7
      .byte 61
      .byte 3
      .byte -126
      .byte 11
      .byte -7
      .byte 11
      .byte 45
      .byte 5
      .byte -60
      .byte 10
      .byte 69
      .byte 7
      .byte -62
      .byte 5
      .byte -78
      .byte 4
      .byte 63
      .byte 9
      .byte 75
      .byte 12
      .byte -40
      .byte 6
      .byte -109
      .byte 10
      .byte -85
      .byte 0
      .byte 55
      .byte 12
      .byte -30
      .byte 11
      .byte 115
      .byte 7
      .byte 44
      .byte 7
      .byte -19
      .byte 5
      .byte 103
      .byte 1
      .byte -10
      .byte 2
      .byte -95
      .byte 5
      .byte -19
      .byte 8
      .byte 11
      .byte 10
      .byte -102
      .byte 11
      .byte 20
      .byte 7
      .byte -43
      .byte 5
      .byte -114
      .byte 5
      .byte 31
      .byte 1
      .byte -54
      .byte 0
      .byte 86
      .byte 12
      .byte 110
      .byte 2
      .byte 41
      .byte 6
      .byte -74
      .byte 0
      .byte -62
      .byte 3
      .byte 79
      .byte 8
      .byte 63
      .byte 7
      .byte -68
      .byte 5
      .byte 61
      .byte 2
      .byte -44
      .byte 7
      .byte 8
      .byte 1
      .byte 127
      .byte 1
      .byte -60
      .byte 9
      .byte -78
      .byte 5
      .byte -65
      .byte 6
      .byte 127
      .byte 12
      .byte 88
      .byte 10
      .byte -7
      .byte 3
      .byte -36
      .byte 2
      .byte 96
      .byte 2
      .byte -5
      .byte 6
      .byte -101
      .byte 1
      .byte 52
      .byte 12
      .byte -34
      .byte 6
      .byte -57
      .byte 4
      .byte -116
      .byte 2
      .byte -39
      .byte 10
      .byte -9
      .byte 3
      .byte -12
      .byte 7
      .byte -45
      .byte 5
      .byte -25
      .byte 11
      .byte -7
      .byte 6
      .byte 4
      .byte 2
      .byte -7
      .byte 12
      .byte -63
      .byte 11
      .byte 103
      .byte 10
      .byte -81
      .byte 6
      .byte 119
      .byte 8
      .byte 126
      .byte 0
      .byte -67
      .byte 5
      .byte -84
      .byte 9
      .byte -89
      .byte 12
      .byte -14
      .byte 11
      .byte 62
      .byte 3
      .byte 107
      .byte 0
      .byte 116
      .byte 7
      .byte 10
      .byte 12
      .byte 74
      .byte 9
      .byte 115
      .byte 11
      .byte -63
      .byte 3
      .byte 29
      .byte 7
      .byte 44
      .byte 10
      .byte -64
      .byte 1
      .byte -40
      .byte 8
      .byte -91
      .byte 2
      .byte 6
      .byte 8
      .byte -78
      .byte 8
      .byte -82
      .byte 1
      .byte 43
      .byte 2
      .byte 75
      .byte 3
      .byte 30
      .byte 8
      .byte 103
      .byte 3
      .byte 14
      .byte 6
      .byte 105
      .byte 0
      .byte -90
      .byte 1
      .byte 75
      .byte 2
      .byte -79
      .byte 0
      .byte 22
      .byte 12
      .byte -34
      .byte 11
      .byte 53
      .byte 11
      .byte 38
      .byte 6
      .byte 117
      .byte 6
      .byte 11
      .byte 12
      .byte 10
      .byte 3
      .byte -121
      .byte 4
      .byte 110
      .byte 12
      .byte -8
      .byte 9
      .byte -53
      .byte 5
      .byte -89
      .byte 10
      .byte 95
      .byte 4
      .byte -53
      .byte 6
      .byte -124
      .byte 2
      .byte -103
      .byte 9
      .byte 93
      .byte 1
      .byte -94
      .byte 1
      .byte 73
      .byte 1
      .byte 101
      .byte 12
      .byte -74
      .byte 12
      .byte 49
      .byte 3
      .byte 73
      .byte 4
      .byte 91
      .byte 2
      .byte 98
      .byte 2
      .byte 42
      .byte 5
      .byte -4
      .byte 7
      .byte 72
      .byte 7
      .byte -128
      .byte 1
      .byte 66
      .byte 8
      .byte 121
      .byte 12
      .byte -62
      .byte 4
      .byte -54
      .byte 7
      .byte -105
      .byte 9
      .byte -36
      .byte 0
      .byte 94
      .byte 8
      .byte -122
      .byte 6
      .byte 96
      .byte 8
      .byte 7
      .byte 7
      .byte 3
      .byte 8
      .byte 26
      .byte 3
      .byte 27
      .byte 7
      .byte -85
      .byte 9
      .byte -101
      .byte 9
      .byte -34
      .byte 1
      .byte -107
      .byte 12
      .byte -51
      .byte 11
      .byte -28
      .byte 3
      .byte -33
      .byte 3
      .byte -66
      .byte 3
      .byte 77
      .byte 7
      .byte -14
      .byte 5
      .byte 92
      .byte 6
