	.att_syntax
	.text
	.p2align	5
	.globl	_jade_kem_kyber_kyber768_amd64_avx2_dec
	.globl	jade_kem_kyber_kyber768_amd64_avx2_dec
	.globl	_jade_kem_kyber_kyber768_amd64_avx2_enc
	.globl	jade_kem_kyber_kyber768_amd64_avx2_enc
	.globl	_jade_kem_kyber_kyber768_amd64_avx2_enc_derand
	.globl	jade_kem_kyber_kyber768_amd64_avx2_enc_derand
	.globl	_jade_kem_kyber_kyber768_amd64_avx2_keypair
	.globl	jade_kem_kyber_kyber768_amd64_avx2_keypair
	.globl	_jade_kem_kyber_kyber768_amd64_avx2_keypair_derand
	.globl	jade_kem_kyber_kyber768_amd64_avx2_keypair_derand
_jade_kem_kyber_kyber768_amd64_avx2_dec:
jade_kem_kyber_kyber768_amd64_avx2_dec:
	movq	%rsp, %rax
	leaq	-17192(%rsp), %rsp
	andq	$-32, %rsp
	movq	%rax, 17184(%rsp)
	movq	%r12, 17160(%rsp)
	movq	%rbp, 17168(%rsp)
	movq	%rbx, 17176(%rsp)
	movq	%rdi, 17120(%rsp)
	movq	%rsi, 17128(%rsp)
	leaq	32(%rsp), %rax
	vpbroadcastd	glob_data + 6404(%rip), %ymm0
	vmovdqu	glob_data + 64(%rip), %ymm1
	vpbroadcastq	glob_data + 6192(%rip), %ymm2
	vpbroadcastd	glob_data + 6400(%rip), %ymm3
	vmovdqu	(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6368(%rsp)
	vmovdqu	20(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6400(%rsp)
	vmovdqu	40(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6432(%rsp)
	vmovdqu	60(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6464(%rsp)
	vmovdqu	80(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6496(%rsp)
	vmovdqu	100(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6528(%rsp)
	vmovdqu	120(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6560(%rsp)
	vmovdqu	140(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6592(%rsp)
	vmovdqu	160(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6624(%rsp)
	vmovdqu	180(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6656(%rsp)
	vmovdqu	200(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6688(%rsp)
	vmovdqu	220(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6720(%rsp)
	vmovdqu	240(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6752(%rsp)
	vmovdqu	260(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6784(%rsp)
	vmovdqu	280(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6816(%rsp)
	vmovdqu	300(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6848(%rsp)
	vmovdqu	320(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6880(%rsp)
	vmovdqu	340(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6912(%rsp)
	vmovdqu	360(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6944(%rsp)
	vmovdqu	380(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 6976(%rsp)
	vmovdqu	400(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7008(%rsp)
	vmovdqu	420(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7040(%rsp)
	vmovdqu	440(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7072(%rsp)
	vmovdqu	460(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7104(%rsp)
	vmovdqu	480(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7136(%rsp)
	vmovdqu	500(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7168(%rsp)
	vmovdqu	520(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7200(%rsp)
	vmovdqu	540(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7232(%rsp)
	vmovdqu	560(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7264(%rsp)
	vmovdqu	580(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7296(%rsp)
	vmovdqu	600(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7328(%rsp)
	vmovdqu	620(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7360(%rsp)
	vmovdqu	640(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7392(%rsp)
	vmovdqu	660(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7424(%rsp)
	vmovdqu	680(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7456(%rsp)
	vmovdqu	700(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7488(%rsp)
	vmovdqu	720(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7520(%rsp)
	vmovdqu	740(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7552(%rsp)
	vmovdqu	760(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7584(%rsp)
	vmovdqu	780(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7616(%rsp)
	vmovdqu	800(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7648(%rsp)
	vmovdqu	820(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7680(%rsp)
	vmovdqu	840(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7712(%rsp)
	vmovdqu	860(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7744(%rsp)
	vmovdqu	880(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7776(%rsp)
	vmovdqu	900(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7808(%rsp)
	vmovdqu	920(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpsllvd	%ymm2, %ymm4, %ymm4
	vpsrlw	$1, %ymm4, %ymm4
	vpand	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 7840(%rsp)
	vmovdqu	940(%rsi), %ymm4
	vpermq	$-108, %ymm4, %ymm4
	vpshufb	%ymm1, %ymm4, %ymm1
	vpsllvd	%ymm2, %ymm1, %ymm1
	vpsrlw	$1, %ymm1, %ymm1
	vpand	%ymm3, %ymm1, %ymm1
	vpmulhrsw	%ymm0, %ymm1, %ymm0
	vmovdqu	%ymm0, 7872(%rsp)
	addq	$960, %rsi
	leaq	2432(%rsp), %rcx
	leaq	-24(%rsp), %rsp
	call	L_poly_decompress$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$145:
	leaq	24(%rsp), %rsp
	movq	%rdx, %rsi
	leaq	7904(%rsp), %r8
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$144:
	addq	$384, %rsi
	leaq	8416(%rsp), %r8
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$143:
	addq	$384, %rsi
	leaq	8928(%rsp), %r8
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$142:
	leaq	6368(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$141:
	leaq	6880(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$140:
	leaq	7392(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$139:
	leaq	2944(%rsp), %rcx
	leaq	7904(%rsp), %rsi
	leaq	6368(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$138:
	leaq	3456(%rsp), %rcx
	leaq	8416(%rsp), %rsi
	leaq	6880(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$137:
	leaq	2944(%rsp), %rcx
	leaq	3456(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$136:
	leaq	3456(%rsp), %rcx
	leaq	8928(%rsp), %rsi
	leaq	7392(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$135:
	leaq	2944(%rsp), %rcx
	leaq	3456(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$134:
	leaq	2944(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$133:
	leaq	3456(%rsp), %rcx
	leaq	2432(%rsp), %rsi
	leaq	2944(%rsp), %rdi
	call	L_poly_sub$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$132:
	leaq	3456(%rsp), %rcx
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rcx)
	vmovdqu	32(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rcx)
	vmovdqu	64(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rcx)
	vmovdqu	96(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rcx)
	vmovdqu	128(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rcx)
	vmovdqu	160(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rcx)
	vmovdqu	192(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rcx)
	vmovdqu	224(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rcx)
	vmovdqu	256(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rcx)
	vmovdqu	288(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rcx)
	vmovdqu	320(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rcx)
	vmovdqu	352(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rcx)
	vmovdqu	384(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rcx)
	vmovdqu	416(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rcx)
	vmovdqu	448(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rcx)
	vmovdqu	480(%rcx), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rcx)
	leaq	3456(%rsp), %rcx
	call	L_poly_tomsg_1$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$131:
	leaq	32(%rdx), %rax
	addq	$2304, %rax
	movq	(%rax), %rcx
	movq	%rcx, 64(%rsp)
	movq	8(%rax), %rcx
	movq	%rcx, 72(%rsp)
	movq	16(%rax), %rcx
	movq	%rcx, 80(%rsp)
	movq	24(%rax), %rax
	movq	%rax, 88(%rsp)
	movq	%rdx, 17136(%rsp)
	leaq	96(%rsp), %rax
	leaq	32(%rsp), %rcx
	leaq	-248(%rsp), %rsp
	call	L_sha3_512_64$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$130:
	leaq	248(%rsp), %rsp
	movq	17136(%rsp), %rax
	addq	$1152, %rax
	leaq	5280(%rsp), %rcx
	leaq	32(%rsp), %rdi
	leaq	128(%rsp), %rdx
	movq	%rcx, 17144(%rsp)
	movq	%rax, %rsi
	leaq	6368(%rsp), %r8
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$129:
	addq	$384, %rsi
	leaq	6880(%rsp), %r8
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$128:
	addq	$384, %rsi
	leaq	7392(%rsp), %r8
	call	L_poly_frombytes$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$127:
	movq	$0, %rcx
	addq	$1152, %rax
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$125
Ljade_kem_kyber_kyber768_amd64_avx2_dec$126:
	movq	(%rax), %rsi
	movq	%rsi, (%rsp,%rcx,8)
	addq	$8, %rax
	incq	%rcx
Ljade_kem_kyber_kyber768_amd64_avx2_dec$125:
	cmpq	$4, %rcx
	jb  	Ljade_kem_kyber_kyber768_amd64_avx2_dec$126
	leaq	3456(%rsp), %rax
	call	L_poly_frommsg_1$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$124:
	movq	%rdx, 17152(%rsp)
	vmovdqu	(%rsp), %ymm0
	vmovdqu	%ymm0, 384(%rsp)
	vmovdqu	%ymm0, 896(%rsp)
	vmovdqu	%ymm0, 1408(%rsp)
	vmovdqu	%ymm0, 1920(%rsp)
	vmovdqu	%ymm0, (%rsp)
	movb	$0, 416(%rsp)
	movb	$0, 417(%rsp)
	movb	$0, 928(%rsp)
	movb	$1, 929(%rsp)
	movb	$0, 1440(%rsp)
	movb	$2, 1441(%rsp)
	movb	$1, 1952(%rsp)
	movb	$0, 1953(%rsp)
	leaq	4480(%rsp), %rax
	leaq	384(%rsp), %rcx
	leaq	896(%rsp), %rdx
	leaq	1408(%rsp), %rsi
	leaq	1920(%rsp), %rdi
	call	L_shake128_absorb4x_34$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$123:
	leaq	4480(%rsp), %rax
	leaq	384(%rsp), %rdi
	leaq	896(%rsp), %r8
	leaq	1408(%rsp), %r9
	leaq	1920(%rsp), %r10
	movq	%rdi, %r11
	movq	%r8, %rbx
	movq	%r9, %rbp
	movq	%r10, %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$122:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	168(%rdi), %r11
	leaq	168(%r8), %rbx
	leaq	168(%r9), %rbp
	leaq	168(%r10), %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$121:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	336(%rdi), %rdi
	leaq	336(%r8), %r8
	leaq	336(%r9), %r9
	leaq	336(%r10), %r10
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$120:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%rdi)
	vmovhpd	%xmm1, (%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%r9)
	vmovhpd	%xmm0, (%r10)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%rdi)
	vmovhpd	%xmm1, 8(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%r9)
	vmovhpd	%xmm0, 8(%r10)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%rdi)
	vmovhpd	%xmm1, 16(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%r9)
	vmovhpd	%xmm0, 16(%r10)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%rdi)
	vmovhpd	%xmm1, 24(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%r9)
	vmovhpd	%xmm0, 24(%r10)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%rdi)
	vmovhpd	%xmm1, 32(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%r9)
	vmovhpd	%xmm0, 32(%r10)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%rdi)
	vmovhpd	%xmm1, 40(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%r9)
	vmovhpd	%xmm0, 40(%r10)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%rdi)
	vmovhpd	%xmm1, 48(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%r9)
	vmovhpd	%xmm0, 48(%r10)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%rdi)
	vmovhpd	%xmm1, 56(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%r9)
	vmovhpd	%xmm0, 56(%r10)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%rdi)
	vmovhpd	%xmm1, 64(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%r9)
	vmovhpd	%xmm0, 64(%r10)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%rdi)
	vmovhpd	%xmm1, 72(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%r9)
	vmovhpd	%xmm0, 72(%r10)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%rdi)
	vmovhpd	%xmm1, 80(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%r9)
	vmovhpd	%xmm0, 80(%r10)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%rdi)
	vmovhpd	%xmm1, 88(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%r9)
	vmovhpd	%xmm0, 88(%r10)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%rdi)
	vmovhpd	%xmm1, 96(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%r9)
	vmovhpd	%xmm0, 96(%r10)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%rdi)
	vmovhpd	%xmm1, 104(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%r9)
	vmovhpd	%xmm0, 104(%r10)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%rdi)
	vmovhpd	%xmm1, 112(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%r9)
	vmovhpd	%xmm0, 112(%r10)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%rdi)
	vmovhpd	%xmm1, 120(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%r9)
	vmovhpd	%xmm0, 120(%r10)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%rdi)
	vmovhpd	%xmm1, 128(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%r9)
	vmovhpd	%xmm0, 128(%r10)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%rdi)
	vmovhpd	%xmm1, 136(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%r9)
	vmovhpd	%xmm0, 136(%r10)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%rdi)
	vmovhpd	%xmm1, 144(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%r9)
	vmovhpd	%xmm0, 144(%r10)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%rdi)
	vmovhpd	%xmm1, 152(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%r9)
	vmovhpd	%xmm0, 152(%r10)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%rdi)
	vmovhpd	%xmm1, 160(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%r9)
	vmovhpd	%xmm0, 160(%r10)
	leaq	12512(%rsp), %r11
	leaq	384(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$119:
	movq	%r12, %rdi
	leaq	13024(%rsp), %r11
	leaq	896(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$118:
	movq	%r12, %r9
	leaq	13536(%rsp), %r11
	leaq	1408(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$117:
	movq	%r12, %r8
	leaq	14048(%rsp), %r11
	leaq	1920(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$116:
	cmpq	$255, %rdi
	setbe	%al
	cmpq	$255, %r9
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r8
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$93
Ljade_kem_kyber_kyber768_amd64_avx2_dec$94:
	leaq	4480(%rsp), %rax
	leaq	384(%rsp), %r10
	leaq	896(%rsp), %r11
	leaq	1408(%rsp), %rbx
	leaq	1920(%rsp), %rbp
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$115:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r10)
	vmovhpd	%xmm1, (%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbx)
	vmovhpd	%xmm0, (%rbp)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r10)
	vmovhpd	%xmm1, 8(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbx)
	vmovhpd	%xmm0, 8(%rbp)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r10)
	vmovhpd	%xmm1, 16(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbx)
	vmovhpd	%xmm0, 16(%rbp)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r10)
	vmovhpd	%xmm1, 24(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbx)
	vmovhpd	%xmm0, 24(%rbp)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r10)
	vmovhpd	%xmm1, 32(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbx)
	vmovhpd	%xmm0, 32(%rbp)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r10)
	vmovhpd	%xmm1, 40(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbx)
	vmovhpd	%xmm0, 40(%rbp)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r10)
	vmovhpd	%xmm1, 48(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbx)
	vmovhpd	%xmm0, 48(%rbp)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r10)
	vmovhpd	%xmm1, 56(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbx)
	vmovhpd	%xmm0, 56(%rbp)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r10)
	vmovhpd	%xmm1, 64(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbx)
	vmovhpd	%xmm0, 64(%rbp)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r10)
	vmovhpd	%xmm1, 72(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbx)
	vmovhpd	%xmm0, 72(%rbp)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r10)
	vmovhpd	%xmm1, 80(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbx)
	vmovhpd	%xmm0, 80(%rbp)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r10)
	vmovhpd	%xmm1, 88(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbx)
	vmovhpd	%xmm0, 88(%rbp)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r10)
	vmovhpd	%xmm1, 96(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbx)
	vmovhpd	%xmm0, 96(%rbp)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r10)
	vmovhpd	%xmm1, 104(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbx)
	vmovhpd	%xmm0, 104(%rbp)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r10)
	vmovhpd	%xmm1, 112(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbx)
	vmovhpd	%xmm0, 112(%rbp)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r10)
	vmovhpd	%xmm1, 120(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbx)
	vmovhpd	%xmm0, 120(%rbp)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r10)
	vmovhpd	%xmm1, 128(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbx)
	vmovhpd	%xmm0, 128(%rbp)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r10)
	vmovhpd	%xmm1, 136(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbx)
	vmovhpd	%xmm0, 136(%rbp)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r10)
	vmovhpd	%xmm1, 144(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbx)
	vmovhpd	%xmm0, 144(%rbp)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r10)
	vmovhpd	%xmm1, 152(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbx)
	vmovhpd	%xmm0, 152(%rbp)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r10)
	vmovhpd	%xmm1, 160(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbx)
	vmovhpd	%xmm0, 160(%rbp)
	leaq	12512(%rsp), %rax
	leaq	384(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$110
Ljade_kem_kyber_kyber768_amd64_avx2_dec$111:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$114
	movw	%si, (%rax,%rdi,2)
	incq	%rdi
Ljade_kem_kyber_kyber768_amd64_avx2_dec$114:
	cmpq	$256, %rdi
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$112
	cmpw	$3329, %r11w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$112
	movw	%r11w, (%rax,%rdi,2)
	incq	%rdi
Ljade_kem_kyber_kyber768_amd64_avx2_dec$113:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$112:
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$110:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$111
	leaq	13024(%rsp), %rax
	leaq	896(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$105
Ljade_kem_kyber_kyber768_amd64_avx2_dec$106:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$109
	movw	%si, (%rax,%r9,2)
	incq	%r9
Ljade_kem_kyber_kyber768_amd64_avx2_dec$109:
	cmpq	$256, %r9
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$107
	cmpw	$3329, %r11w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$107
	movw	%r11w, (%rax,%r9,2)
	incq	%r9
Ljade_kem_kyber_kyber768_amd64_avx2_dec$108:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$107:
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$105:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$106
	leaq	13536(%rsp), %rax
	leaq	1408(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$100
Ljade_kem_kyber_kyber768_amd64_avx2_dec$101:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$104
	movw	%si, (%rax,%r8,2)
	incq	%r8
Ljade_kem_kyber_kyber768_amd64_avx2_dec$104:
	cmpq	$256, %r8
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$102
	cmpw	$3329, %r11w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$102
	movw	%r11w, (%rax,%r8,2)
	incq	%r8
Ljade_kem_kyber_kyber768_amd64_avx2_dec$103:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$102:
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$100:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$101
	leaq	14048(%rsp), %rax
	leaq	1920(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$95
Ljade_kem_kyber_kyber768_amd64_avx2_dec$96:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$99
	movw	%si, (%rax,%r12,2)
	incq	%r12
Ljade_kem_kyber_kyber768_amd64_avx2_dec$99:
	cmpq	$256, %r12
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$97
	cmpw	$3329, %r11w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$97
	movw	%r11w, (%rax,%r12,2)
	incq	%r12
Ljade_kem_kyber_kyber768_amd64_avx2_dec$98:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$97:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$95:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$96
	cmpq	$255, %rdi
	setbe	%al
	cmpq	$255, %r9
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r8
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
Ljade_kem_kyber_kyber768_amd64_avx2_dec$93:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$94
	vmovdqu	(%rsp), %ymm0
	vmovdqu	%ymm0, 384(%rsp)
	vmovdqu	%ymm0, 896(%rsp)
	vmovdqu	%ymm0, 1408(%rsp)
	vmovdqu	%ymm0, 1920(%rsp)
	vmovdqu	%ymm0, (%rsp)
	movb	$1, 416(%rsp)
	movb	$1, 417(%rsp)
	movb	$1, 928(%rsp)
	movb	$2, 929(%rsp)
	movb	$2, 1440(%rsp)
	movb	$0, 1441(%rsp)
	movb	$2, 1952(%rsp)
	movb	$1, 1953(%rsp)
	leaq	4480(%rsp), %rax
	leaq	384(%rsp), %rcx
	leaq	896(%rsp), %rdx
	leaq	1408(%rsp), %rsi
	leaq	1920(%rsp), %rdi
	call	L_shake128_absorb4x_34$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$92:
	leaq	4480(%rsp), %rax
	leaq	384(%rsp), %rdi
	leaq	896(%rsp), %r8
	leaq	1408(%rsp), %r9
	leaq	1920(%rsp), %r10
	movq	%rdi, %r11
	movq	%r8, %rbx
	movq	%r9, %rbp
	movq	%r10, %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$91:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	168(%rdi), %r11
	leaq	168(%r8), %rbx
	leaq	168(%r9), %rbp
	leaq	168(%r10), %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$90:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	336(%rdi), %rdi
	leaq	336(%r8), %r8
	leaq	336(%r9), %r9
	leaq	336(%r10), %r10
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$89:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%rdi)
	vmovhpd	%xmm1, (%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%r9)
	vmovhpd	%xmm0, (%r10)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%rdi)
	vmovhpd	%xmm1, 8(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%r9)
	vmovhpd	%xmm0, 8(%r10)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%rdi)
	vmovhpd	%xmm1, 16(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%r9)
	vmovhpd	%xmm0, 16(%r10)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%rdi)
	vmovhpd	%xmm1, 24(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%r9)
	vmovhpd	%xmm0, 24(%r10)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%rdi)
	vmovhpd	%xmm1, 32(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%r9)
	vmovhpd	%xmm0, 32(%r10)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%rdi)
	vmovhpd	%xmm1, 40(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%r9)
	vmovhpd	%xmm0, 40(%r10)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%rdi)
	vmovhpd	%xmm1, 48(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%r9)
	vmovhpd	%xmm0, 48(%r10)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%rdi)
	vmovhpd	%xmm1, 56(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%r9)
	vmovhpd	%xmm0, 56(%r10)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%rdi)
	vmovhpd	%xmm1, 64(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%r9)
	vmovhpd	%xmm0, 64(%r10)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%rdi)
	vmovhpd	%xmm1, 72(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%r9)
	vmovhpd	%xmm0, 72(%r10)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%rdi)
	vmovhpd	%xmm1, 80(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%r9)
	vmovhpd	%xmm0, 80(%r10)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%rdi)
	vmovhpd	%xmm1, 88(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%r9)
	vmovhpd	%xmm0, 88(%r10)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%rdi)
	vmovhpd	%xmm1, 96(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%r9)
	vmovhpd	%xmm0, 96(%r10)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%rdi)
	vmovhpd	%xmm1, 104(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%r9)
	vmovhpd	%xmm0, 104(%r10)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%rdi)
	vmovhpd	%xmm1, 112(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%r9)
	vmovhpd	%xmm0, 112(%r10)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%rdi)
	vmovhpd	%xmm1, 120(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%r9)
	vmovhpd	%xmm0, 120(%r10)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%rdi)
	vmovhpd	%xmm1, 128(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%r9)
	vmovhpd	%xmm0, 128(%r10)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%rdi)
	vmovhpd	%xmm1, 136(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%r9)
	vmovhpd	%xmm0, 136(%r10)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%rdi)
	vmovhpd	%xmm1, 144(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%r9)
	vmovhpd	%xmm0, 144(%r10)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%rdi)
	vmovhpd	%xmm1, 152(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%r9)
	vmovhpd	%xmm0, 152(%r10)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%rdi)
	vmovhpd	%xmm1, 160(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%r9)
	vmovhpd	%xmm0, 160(%r10)
	leaq	14560(%rsp), %r11
	leaq	384(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$88:
	movq	%r12, %rdi
	leaq	15072(%rsp), %r11
	leaq	896(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$87:
	movq	%r12, %r8
	leaq	15584(%rsp), %r11
	leaq	1408(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$86:
	movq	%r12, %r9
	leaq	16096(%rsp), %r11
	leaq	1920(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$85:
	cmpq	$255, %rdi
	setbe	%al
	cmpq	$255, %r8
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r9
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$62
Ljade_kem_kyber_kyber768_amd64_avx2_dec$63:
	leaq	4480(%rsp), %rax
	leaq	384(%rsp), %r10
	leaq	896(%rsp), %r11
	leaq	1408(%rsp), %rbx
	leaq	1920(%rsp), %rbp
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$84:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r10)
	vmovhpd	%xmm1, (%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbx)
	vmovhpd	%xmm0, (%rbp)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r10)
	vmovhpd	%xmm1, 8(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbx)
	vmovhpd	%xmm0, 8(%rbp)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r10)
	vmovhpd	%xmm1, 16(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbx)
	vmovhpd	%xmm0, 16(%rbp)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r10)
	vmovhpd	%xmm1, 24(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbx)
	vmovhpd	%xmm0, 24(%rbp)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r10)
	vmovhpd	%xmm1, 32(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbx)
	vmovhpd	%xmm0, 32(%rbp)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r10)
	vmovhpd	%xmm1, 40(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbx)
	vmovhpd	%xmm0, 40(%rbp)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r10)
	vmovhpd	%xmm1, 48(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbx)
	vmovhpd	%xmm0, 48(%rbp)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r10)
	vmovhpd	%xmm1, 56(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbx)
	vmovhpd	%xmm0, 56(%rbp)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r10)
	vmovhpd	%xmm1, 64(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbx)
	vmovhpd	%xmm0, 64(%rbp)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r10)
	vmovhpd	%xmm1, 72(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbx)
	vmovhpd	%xmm0, 72(%rbp)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r10)
	vmovhpd	%xmm1, 80(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbx)
	vmovhpd	%xmm0, 80(%rbp)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r10)
	vmovhpd	%xmm1, 88(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbx)
	vmovhpd	%xmm0, 88(%rbp)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r10)
	vmovhpd	%xmm1, 96(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbx)
	vmovhpd	%xmm0, 96(%rbp)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r10)
	vmovhpd	%xmm1, 104(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbx)
	vmovhpd	%xmm0, 104(%rbp)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r10)
	vmovhpd	%xmm1, 112(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbx)
	vmovhpd	%xmm0, 112(%rbp)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r10)
	vmovhpd	%xmm1, 120(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbx)
	vmovhpd	%xmm0, 120(%rbp)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r10)
	vmovhpd	%xmm1, 128(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbx)
	vmovhpd	%xmm0, 128(%rbp)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r10)
	vmovhpd	%xmm1, 136(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbx)
	vmovhpd	%xmm0, 136(%rbp)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r10)
	vmovhpd	%xmm1, 144(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbx)
	vmovhpd	%xmm0, 144(%rbp)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r10)
	vmovhpd	%xmm1, 152(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbx)
	vmovhpd	%xmm0, 152(%rbp)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r10)
	vmovhpd	%xmm1, 160(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbx)
	vmovhpd	%xmm0, 160(%rbp)
	leaq	14560(%rsp), %rax
	leaq	384(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$79
Ljade_kem_kyber_kyber768_amd64_avx2_dec$80:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$83
	movw	%si, (%rax,%rdi,2)
	incq	%rdi
Ljade_kem_kyber_kyber768_amd64_avx2_dec$83:
	cmpq	$256, %rdi
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$81
	cmpw	$3329, %r11w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$81
	movw	%r11w, (%rax,%rdi,2)
	incq	%rdi
Ljade_kem_kyber_kyber768_amd64_avx2_dec$82:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$81:
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$79:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$80
	leaq	15072(%rsp), %rax
	leaq	896(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$74
Ljade_kem_kyber_kyber768_amd64_avx2_dec$75:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$78
	movw	%si, (%rax,%r8,2)
	incq	%r8
Ljade_kem_kyber_kyber768_amd64_avx2_dec$78:
	cmpq	$256, %r8
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$76
	cmpw	$3329, %r11w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$76
	movw	%r11w, (%rax,%r8,2)
	incq	%r8
Ljade_kem_kyber_kyber768_amd64_avx2_dec$77:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$76:
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$74:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$75
	leaq	15584(%rsp), %rax
	leaq	1408(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$69
Ljade_kem_kyber_kyber768_amd64_avx2_dec$70:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$73
	movw	%si, (%rax,%r9,2)
	incq	%r9
Ljade_kem_kyber_kyber768_amd64_avx2_dec$73:
	cmpq	$256, %r9
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$71
	cmpw	$3329, %r11w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$71
	movw	%r11w, (%rax,%r9,2)
	incq	%r9
Ljade_kem_kyber_kyber768_amd64_avx2_dec$72:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$71:
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$69:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$70
	leaq	16096(%rsp), %rax
	leaq	1920(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$64
Ljade_kem_kyber_kyber768_amd64_avx2_dec$65:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$68
	movw	%si, (%rax,%r12,2)
	incq	%r12
Ljade_kem_kyber_kyber768_amd64_avx2_dec$68:
	cmpq	$256, %r12
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$66
	cmpw	$3329, %r11w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$66
	movw	%r11w, (%rax,%r12,2)
	incq	%r12
Ljade_kem_kyber_kyber768_amd64_avx2_dec$67:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$66:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$64:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$65
	cmpq	$255, %rdi
	setbe	%al
	cmpq	$255, %r8
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r9
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
Ljade_kem_kyber_kyber768_amd64_avx2_dec$62:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$63
	vmovdqu	(%rsp), %ymm0
	vmovdqu	%ymm0, 384(%rsp)
	movb	$2, 416(%rsp)
	movb	$2, 417(%rsp)
	leaq	384(%rsp), %rax
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vmovdqu	%ymm0, 256(%rsp)
	vmovdqu	%ymm0, 288(%rsp)
	vmovdqu	%ymm0, 320(%rsp)
	vmovdqu	%ymm0, 352(%rsp)
	vpbroadcastq	(%rax), %ymm5
	movq	8(%rax), %rcx
	movq	glob_data + 6208(%rip), %rdx
	movq	%rcx, 160(%rsp,%rdx,8)
	movq	16(%rax), %rcx
	movq	glob_data + 6216(%rip), %rdx
	movq	%rcx, 160(%rsp,%rdx,8)
	movq	24(%rax), %rcx
	movq	glob_data + 6224(%rip), %rdx
	movq	%rcx, 160(%rsp,%rdx,8)
	movb	32(%rax), %cl
	movq	glob_data + 6232(%rip), %rdx
	shlq	$3, %rdx
	movb	%cl, 160(%rsp,%rdx)
	movb	33(%rax), %al
	incq	%rdx
	movb	%al, 160(%rsp,%rdx)
	incq	%rdx
	movb	$31, 160(%rsp,%rdx)
	movq	glob_data + 6360(%rip), %rax
	shlq	$3, %rax
	movq	$167, %rcx
	andq	$7, %rcx
	addq	%rcx, %rax
	movb	$-128, 160(%rsp,%rax)
	vmovdqu	192(%rsp), %ymm0
	vmovdqu	224(%rsp), %ymm1
	vmovdqu	256(%rsp), %ymm2
	vmovdqu	288(%rsp), %ymm6
	vmovdqu	320(%rsp), %ymm3
	vmovdqu	352(%rsp), %ymm4
	leaq	384(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
Ljade_kem_kyber_kyber768_amd64_avx2_dec$61:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$61
	vmovdqu	%ymm5, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm1, 224(%rsp)
	vmovdqu	%ymm2, 256(%rsp)
	vmovdqu	%ymm6, 288(%rsp)
	vmovdqu	%ymm3, 320(%rsp)
	vmovdqu	%ymm4, 352(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	552(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
Ljade_kem_kyber_kyber768_amd64_avx2_dec$60:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$60
	vmovdqu	%ymm5, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm1, 224(%rsp)
	vmovdqu	%ymm2, 256(%rsp)
	vmovdqu	%ymm6, 288(%rsp)
	vmovdqu	%ymm3, 320(%rsp)
	vmovdqu	%ymm4, 352(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	720(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
Ljade_kem_kyber_kyber768_amd64_avx2_dec$59:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$59
	vmovdqu	%ymm5, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm1, 224(%rsp)
	vmovdqu	%ymm2, 256(%rsp)
	vmovdqu	%ymm6, 288(%rsp)
	vmovdqu	%ymm3, 320(%rsp)
	vmovdqu	%ymm4, 352(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	vmovdqu	%ymm5, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm1, 224(%rsp)
	vmovdqu	%ymm2, 256(%rsp)
	vmovdqu	%ymm6, 288(%rsp)
	vmovdqu	%ymm3, 320(%rsp)
	vmovdqu	%ymm4, 352(%rsp)
	leaq	16608(%rsp), %r11
	leaq	384(%rsp), %rbx
	call	L_rej_uniform_avx$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$58:
	cmpq	$255, %r12
	setbe	%al
	vmovdqu	160(%rsp), %ymm6
	vmovdqu	192(%rsp), %ymm0
	vmovdqu	224(%rsp), %ymm1
	vmovdqu	256(%rsp), %ymm2
	vmovdqu	288(%rsp), %ymm3
	vmovdqu	320(%rsp), %ymm4
	vmovdqu	352(%rsp), %ymm5
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$50
Ljade_kem_kyber_kyber768_amd64_avx2_dec$51:
	leaq	384(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
Ljade_kem_kyber_kyber768_amd64_avx2_dec$57:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm4, %ymm7
	vpxor	%ymm5, %ymm3, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm6, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm6, %ymm6
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	96(%rsi), %ymm3, %ymm7
	vpsrlvq	96(%rdi), %ymm3, %ymm3
	vpor	%ymm7, %ymm3, %ymm10
	vpxor	%ymm9, %ymm4, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm4
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm4, %ymm3, %ymm4
	vpxor	%ymm9, %ymm5, %ymm5
	vpermq	$-115, %ymm1, %ymm3
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm5, %ymm1
	vpsrlvq	160(%rdi), %ymm5, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm4, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm4
	vpblendd	$12, %ymm7, %ymm3, %ymm5
	vpblendd	$12, %ymm3, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm4, %ymm4
	vpblendd	$48, %ymm10, %ymm5, %ymm5
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm9, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm4, %ymm1, %ymm1
	vpandn	%ymm11, %ymm5, %ymm5
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm3, %ymm12
	vpxor	%ymm3, %ymm1, %ymm4
	vpblendd	$48, %ymm3, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm6, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm6, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm3, %ymm13, %ymm13
	vpblendd	$-64, %ymm3, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm4, %ymm2
	vpermq	$-115, %ymm5, %ymm4
	vpermq	$114, %ymm12, %ymm5
	vpblendd	$12, %ymm3, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm3, %ymm9, %ymm3
	vpandn	%ymm3, %ymm0, %ymm3
	vpxor	%ymm13, %ymm6, %ymm6
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm3, %ymm3
	vpxor	(%rcx,%rdx), %ymm6, %ymm6
	addq	$32, %rdx
	decq	%r8
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$57
	vmovdqu	%ymm6, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm1, 224(%rsp)
	vmovdqu	%ymm2, 256(%rsp)
	vmovdqu	%ymm3, 288(%rsp)
	vmovdqu	%ymm4, 320(%rsp)
	vmovdqu	%ymm5, 352(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	160(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	16608(%rsp), %rax
	leaq	384(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%dil
	testb	%dil, %sil
	jmp 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$52
Ljade_kem_kyber_kyber768_amd64_avx2_dec$53:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %di
	movw	%di, %r8w
	shrw	$4, %r8w
	andw	$15, %di
	shlw	$8, %di
	orw 	%di, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %di
	shlw	$4, %di
	orw 	%di, %r8w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$56
	movw	%si, (%rax,%r12,2)
	incq	%r12
Ljade_kem_kyber_kyber768_amd64_avx2_dec$56:
	cmpq	$256, %r12
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$54
	cmpw	$3329, %r8w
	jnb 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$54
	movw	%r8w, (%rax,%r12,2)
	incq	%r12
Ljade_kem_kyber_kyber768_amd64_avx2_dec$55:
Ljade_kem_kyber_kyber768_amd64_avx2_dec$54:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%dil
	testb	%dil, %sil
Ljade_kem_kyber_kyber768_amd64_avx2_dec$52:
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$53
	cmpq	$255, %r12
	setbe	%al
Ljade_kem_kyber_kyber768_amd64_avx2_dec$50:
	cmpb	$0, %al
	jne 	Ljade_kem_kyber_kyber768_amd64_avx2_dec$51
	leaq	12512(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$49:
	leaq	13024(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$48:
	leaq	13536(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$47:
	leaq	14048(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$46:
	leaq	14560(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$45:
	leaq	15072(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$44:
	leaq	15584(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$43:
	leaq	16096(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$42:
	leaq	16608(%rsp), %rax
	call	L_nttunpack$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$41:
	movq	17152(%rsp), %r10
	movb	$0, %cl
	leaq	9440(%rsp), %rax
	leaq	9952(%rsp), %rdi
	leaq	10464(%rsp), %r8
	leaq	7904(%rsp), %r9
	leaq	-1496(%rsp), %rsp
	call	L_poly_getnoise_eta1_4x$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$40:
	leaq	1496(%rsp), %rsp
	movb	$4, %cl
	leaq	8416(%rsp), %rax
	leaq	8928(%rsp), %rdi
	leaq	2944(%rsp), %r8
	leaq	10976(%rsp), %r9
	leaq	-1496(%rsp), %rsp
	call	L_poly_getnoise_eta1_4x$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$39:
	leaq	1496(%rsp), %rsp
	leaq	9440(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$38:
	leaq	9952(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$37:
	leaq	10464(%rsp), %rcx
	call	L_poly_ntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$36:
	leaq	10976(%rsp), %rcx
	leaq	12512(%rsp), %rsi
	leaq	9440(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$35:
	leaq	2432(%rsp), %rcx
	leaq	13024(%rsp), %rsi
	leaq	9952(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$34:
	leaq	10976(%rsp), %rcx
	leaq	2432(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$33:
	leaq	2432(%rsp), %rcx
	leaq	13536(%rsp), %rsi
	leaq	10464(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$32:
	leaq	10976(%rsp), %rcx
	leaq	2432(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$31:
	leaq	11488(%rsp), %rcx
	leaq	14048(%rsp), %rsi
	leaq	9440(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$30:
	leaq	2432(%rsp), %rcx
	leaq	14560(%rsp), %rsi
	leaq	9952(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$29:
	leaq	11488(%rsp), %rcx
	leaq	2432(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$28:
	leaq	2432(%rsp), %rcx
	leaq	15072(%rsp), %rsi
	leaq	10464(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$27:
	leaq	11488(%rsp), %rcx
	leaq	2432(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$26:
	leaq	12000(%rsp), %rcx
	leaq	15584(%rsp), %rsi
	leaq	9440(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$25:
	leaq	2432(%rsp), %rcx
	leaq	16096(%rsp), %rsi
	leaq	9952(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$24:
	leaq	12000(%rsp), %rcx
	leaq	2432(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$23:
	leaq	2432(%rsp), %rcx
	leaq	16608(%rsp), %rsi
	leaq	10464(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$22:
	leaq	12000(%rsp), %rcx
	leaq	2432(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$21:
	leaq	2432(%rsp), %rcx
	leaq	6368(%rsp), %rsi
	leaq	9440(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$20:
	leaq	3968(%rsp), %rcx
	leaq	6880(%rsp), %rsi
	leaq	9952(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$19:
	leaq	2432(%rsp), %rcx
	leaq	3968(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$18:
	leaq	3968(%rsp), %rcx
	leaq	7392(%rsp), %rsi
	leaq	10464(%rsp), %rdi
	call	L_poly_basemul$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$17:
	leaq	2432(%rsp), %rcx
	leaq	3968(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$16:
	leaq	10976(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$15:
	leaq	11488(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$14:
	leaq	12000(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$13:
	leaq	2432(%rsp), %rcx
	call	L_poly_invntt$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$12:
	leaq	10976(%rsp), %rcx
	leaq	7904(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$11:
	leaq	11488(%rsp), %rcx
	leaq	8416(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$10:
	leaq	12000(%rsp), %rcx
	leaq	8928(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$9:
	leaq	2432(%rsp), %rcx
	leaq	2944(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$8:
	leaq	2432(%rsp), %rcx
	leaq	3456(%rsp), %rsi
	call	L_poly_add2$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$7:
	leaq	10976(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	leaq	11488(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	leaq	12000(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	leaq	2432(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	movq	17144(%rsp), %rax
	movq	%rax, %rdx
	leaq	10976(%rsp), %rcx
	call	L_poly_csubq$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$6:
	leaq	11488(%rsp), %rcx
	call	L_poly_csubq$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$5:
	leaq	12000(%rsp), %rcx
	call	L_poly_csubq$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$4:
	leaq	glob_data + 384(%rip), %rcx
	vmovdqu	(%rcx), %ymm0
	vpsllw	$3, %ymm0, %ymm1
	vpbroadcastw	glob_data + 6422(%rip), %ymm2
	vpbroadcastw	glob_data + 6420(%rip), %ymm3
	vpbroadcastw	glob_data + 6418(%rip), %ymm4
	vpbroadcastq	glob_data + 6184(%rip), %ymm5
	vpbroadcastq	glob_data + 6176(%rip), %ymm6
	vmovdqu	glob_data + 32(%rip), %ymm7
	vmovdqu	10976(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, (%rdx)
	vpextrd	$0, %xmm8, 16(%rdx)
	vmovdqu	11008(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 20(%rdx)
	vpextrd	$0, %xmm8, 36(%rdx)
	vmovdqu	11040(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 40(%rdx)
	vpextrd	$0, %xmm8, 56(%rdx)
	vmovdqu	11072(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 60(%rdx)
	vpextrd	$0, %xmm8, 76(%rdx)
	vmovdqu	11104(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 80(%rdx)
	vpextrd	$0, %xmm8, 96(%rdx)
	vmovdqu	11136(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 100(%rdx)
	vpextrd	$0, %xmm8, 116(%rdx)
	vmovdqu	11168(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 120(%rdx)
	vpextrd	$0, %xmm8, 136(%rdx)
	vmovdqu	11200(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 140(%rdx)
	vpextrd	$0, %xmm8, 156(%rdx)
	vmovdqu	11232(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 160(%rdx)
	vpextrd	$0, %xmm8, 176(%rdx)
	vmovdqu	11264(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 180(%rdx)
	vpextrd	$0, %xmm8, 196(%rdx)
	vmovdqu	11296(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 200(%rdx)
	vpextrd	$0, %xmm8, 216(%rdx)
	vmovdqu	11328(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 220(%rdx)
	vpextrd	$0, %xmm8, 236(%rdx)
	vmovdqu	11360(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 240(%rdx)
	vpextrd	$0, %xmm8, 256(%rdx)
	vmovdqu	11392(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 260(%rdx)
	vpextrd	$0, %xmm8, 276(%rdx)
	vmovdqu	11424(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 280(%rdx)
	vpextrd	$0, %xmm8, 296(%rdx)
	vmovdqu	11456(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 300(%rdx)
	vpextrd	$0, %xmm8, 316(%rdx)
	vmovdqu	11488(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 320(%rdx)
	vpextrd	$0, %xmm8, 336(%rdx)
	vmovdqu	11520(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 340(%rdx)
	vpextrd	$0, %xmm8, 356(%rdx)
	vmovdqu	11552(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 360(%rdx)
	vpextrd	$0, %xmm8, 376(%rdx)
	vmovdqu	11584(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 380(%rdx)
	vpextrd	$0, %xmm8, 396(%rdx)
	vmovdqu	11616(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 400(%rdx)
	vpextrd	$0, %xmm8, 416(%rdx)
	vmovdqu	11648(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 420(%rdx)
	vpextrd	$0, %xmm8, 436(%rdx)
	vmovdqu	11680(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 440(%rdx)
	vpextrd	$0, %xmm8, 456(%rdx)
	vmovdqu	11712(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 460(%rdx)
	vpextrd	$0, %xmm8, 476(%rdx)
	vmovdqu	11744(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 480(%rdx)
	vpextrd	$0, %xmm8, 496(%rdx)
	vmovdqu	11776(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 500(%rdx)
	vpextrd	$0, %xmm8, 516(%rdx)
	vmovdqu	11808(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 520(%rdx)
	vpextrd	$0, %xmm8, 536(%rdx)
	vmovdqu	11840(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 540(%rdx)
	vpextrd	$0, %xmm8, 556(%rdx)
	vmovdqu	11872(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 560(%rdx)
	vpextrd	$0, %xmm8, 576(%rdx)
	vmovdqu	11904(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 580(%rdx)
	vpextrd	$0, %xmm8, 596(%rdx)
	vmovdqu	11936(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 600(%rdx)
	vpextrd	$0, %xmm8, 616(%rdx)
	vmovdqu	11968(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 620(%rdx)
	vpextrd	$0, %xmm8, 636(%rdx)
	vmovdqu	12000(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 640(%rdx)
	vpextrd	$0, %xmm8, 656(%rdx)
	vmovdqu	12032(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 660(%rdx)
	vpextrd	$0, %xmm8, 676(%rdx)
	vmovdqu	12064(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 680(%rdx)
	vpextrd	$0, %xmm8, 696(%rdx)
	vmovdqu	12096(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 700(%rdx)
	vpextrd	$0, %xmm8, 716(%rdx)
	vmovdqu	12128(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 720(%rdx)
	vpextrd	$0, %xmm8, 736(%rdx)
	vmovdqu	12160(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 740(%rdx)
	vpextrd	$0, %xmm8, 756(%rdx)
	vmovdqu	12192(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 760(%rdx)
	vpextrd	$0, %xmm8, 776(%rdx)
	vmovdqu	12224(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 780(%rdx)
	vpextrd	$0, %xmm8, 796(%rdx)
	vmovdqu	12256(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 800(%rdx)
	vpextrd	$0, %xmm8, 816(%rdx)
	vmovdqu	12288(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 820(%rdx)
	vpextrd	$0, %xmm8, 836(%rdx)
	vmovdqu	12320(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 840(%rdx)
	vpextrd	$0, %xmm8, 856(%rdx)
	vmovdqu	12352(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 860(%rdx)
	vpextrd	$0, %xmm8, 876(%rdx)
	vmovdqu	12384(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 880(%rdx)
	vpextrd	$0, %xmm8, 896(%rdx)
	vmovdqu	12416(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 900(%rdx)
	vpextrd	$0, %xmm8, 916(%rdx)
	vmovdqu	12448(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 920(%rdx)
	vpextrd	$0, %xmm8, 936(%rdx)
	vmovdqu	12480(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm1
	vpaddw	%ymm2, %ymm8, %ymm2
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm0
	vpsubw	%ymm2, %ymm1, %ymm2
	vpandn	%ymm2, %ymm1, %ymm1
	vpsrlw	$15, %ymm1, %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vpmulhrsw	%ymm3, %ymm0, %ymm0
	vpand	%ymm4, %ymm0, %ymm0
	vpmaddwd	%ymm5, %ymm0, %ymm0
	vpsllvd	%ymm6, %ymm0, %ymm0
	vpsrlq	$12, %ymm0, %ymm0
	vpshufb	%ymm7, %ymm0, %ymm0
	vmovdqu	%xmm0, %xmm1
	vextracti128	$1, %ymm0, %xmm0
	vpblendw	$-32, %xmm0, %xmm1, %xmm1
	vmovdqu	%xmm1, 940(%rdx)
	vpextrd	$0, %xmm0, 956(%rdx)
	leaq	960(%rax), %rax
	leaq	2432(%rsp), %rcx
	call	L_poly_compress_1$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$3:
	movq	17128(%rsp), %r8
	leaq	5280(%rsp), %rax
	movq	$0, %rcx
	movq	$1, %rdx
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	(%rax), %ymm1
	vmovdqu	(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	32(%rax), %ymm1
	vmovdqu	32(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	64(%rax), %ymm1
	vmovdqu	64(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	96(%rax), %ymm1
	vmovdqu	96(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	128(%rax), %ymm1
	vmovdqu	128(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	160(%rax), %ymm1
	vmovdqu	160(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	192(%rax), %ymm1
	vmovdqu	192(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	224(%rax), %ymm1
	vmovdqu	224(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	256(%rax), %ymm1
	vmovdqu	256(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	288(%rax), %ymm1
	vmovdqu	288(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	320(%rax), %ymm1
	vmovdqu	320(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	352(%rax), %ymm1
	vmovdqu	352(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	384(%rax), %ymm1
	vmovdqu	384(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	416(%rax), %ymm1
	vmovdqu	416(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	448(%rax), %ymm1
	vmovdqu	448(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	480(%rax), %ymm1
	vmovdqu	480(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	512(%rax), %ymm1
	vmovdqu	512(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	544(%rax), %ymm1
	vmovdqu	544(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	576(%rax), %ymm1
	vmovdqu	576(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	608(%rax), %ymm1
	vmovdqu	608(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	640(%rax), %ymm1
	vmovdqu	640(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	672(%rax), %ymm1
	vmovdqu	672(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	704(%rax), %ymm1
	vmovdqu	704(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	736(%rax), %ymm1
	vmovdqu	736(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	768(%rax), %ymm1
	vmovdqu	768(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	800(%rax), %ymm1
	vmovdqu	800(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	832(%rax), %ymm1
	vmovdqu	832(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	864(%rax), %ymm1
	vmovdqu	864(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	896(%rax), %ymm1
	vmovdqu	896(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	928(%rax), %ymm1
	vmovdqu	928(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	960(%rax), %ymm1
	vmovdqu	960(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	992(%rax), %ymm1
	vmovdqu	992(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	1024(%rax), %ymm1
	vmovdqu	1024(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vmovdqu	1056(%rax), %ymm1
	vmovdqu	1056(%r8), %ymm2
	vpxor	%ymm2, %ymm1, %ymm1
	vpor	%ymm1, %ymm0, %ymm0
	vptest	%ymm0, %ymm0
	cmovne	%rdx, %rcx
	negq	%rcx
	shrq	$63, %rcx
	movq	17136(%rsp), %rax
	addq	$64, %rax
	addq	$2304, %rax
	leaq	96(%rsp), %rdx
	negq	%rcx
	movq	%rcx, 17136(%rsp)
	vpbroadcastq	17136(%rsp), %ymm0
	vmovdqu	(%rdx), %ymm1
	vmovdqu	(%rax), %ymm2
	vpblendvb	%ymm0, %ymm2, %ymm1, %ymm0
	vmovdqu	%ymm0, (%rdx)
	movq	$1088, %rdi
	leaq	128(%rsp), %rax
	leaq	-248(%rsp), %rsp
	call	L_sha3_256$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$2:
	leaq	248(%rsp), %rsp
	movq	17120(%rsp), %rdx
	movq	$32, %rcx
	leaq	96(%rsp), %rax
	leaq	-248(%rsp), %rsp
	call	L_shake256_64$1
Ljade_kem_kyber_kyber768_amd64_avx2_dec$1:
	leaq	248(%rsp), %rsp
	xorq	%rax, %rax
	movq	17160(%rsp), %r12
	movq	17168(%rsp), %rbp
	movq	17176(%rsp), %rbx
	movq	17184(%rsp), %rsp
	ret
_jade_kem_kyber_kyber768_amd64_avx2_enc:
jade_kem_kyber_kyber768_amd64_avx2_enc:
	movq	%rsp, %rax
	leaq	-64(%rsp), %rsp
	andq	$-32, %rsp
	movq	%rax, 56(%rsp)
	movq	%r12, 32(%rsp)
	movq	%rbp, 40(%rsp)
	movq	%rbx, 48(%rsp)
	movq	%rdi, %rbx
	movq	%rsi, %rbp
	movq	%rdx, %r12
	movq	%rsp, %rdi
	movq	$32, %rsi
	call	__jasmin_syscall_randombytes__
	movq	%rsp, %rcx
	leaq	-16088(%rsp), %rsp
	call	L__crypto_kem_enc_derand_jazz$1
Ljade_kem_kyber_kyber768_amd64_avx2_enc$1:
	leaq	16088(%rsp), %rsp
	xorq	%rax, %rax
	movq	32(%rsp), %r12
	movq	40(%rsp), %rbp
	movq	48(%rsp), %rbx
	movq	56(%rsp), %rsp
	ret
_jade_kem_kyber_kyber768_amd64_avx2_enc_derand:
jade_kem_kyber_kyber768_amd64_avx2_enc_derand:
	movq	%rsp, %rax
	leaq	-64(%rsp), %rsp
	andq	$-32, %rsp
	movq	%rax, 56(%rsp)
	movq	%r12, 32(%rsp)
	movq	%rbp, 40(%rsp)
	movq	%rbx, 48(%rsp)
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
	leaq	-16088(%rsp), %rsp
	call	L__crypto_kem_enc_derand_jazz$1
Ljade_kem_kyber_kyber768_amd64_avx2_enc_derand$1:
	leaq	16088(%rsp), %rsp
	xorq	%rax, %rax
	movq	32(%rsp), %r12
	movq	40(%rsp), %rbp
	movq	48(%rsp), %rbx
	movq	56(%rsp), %rsp
	ret
_jade_kem_kyber_kyber768_amd64_avx2_keypair:
jade_kem_kyber_kyber768_amd64_avx2_keypair:
	movq	%rsp, %rax
	leaq	-96(%rsp), %rsp
	andq	$-32, %rsp
	movq	%rax, 88(%rsp)
	movq	%r12, 64(%rsp)
	movq	%rbp, 72(%rsp)
	movq	%rbx, 80(%rsp)
	movq	%rdi, %rbp
	movq	%rsi, %rbx
	movq	%rsp, %rdi
	movq	$64, %rsi
	call	__jasmin_syscall_randombytes__
	movq	%rsp, %rax
	leaq	-13048(%rsp), %rsp
	call	L__crypto_kem_keypair_derand_jazz$1
Ljade_kem_kyber_kyber768_amd64_avx2_keypair$1:
	leaq	13048(%rsp), %rsp
	xorq	%rax, %rax
	movq	64(%rsp), %r12
	movq	72(%rsp), %rbp
	movq	80(%rsp), %rbx
	movq	88(%rsp), %rsp
	ret
_jade_kem_kyber_kyber768_amd64_avx2_keypair_derand:
jade_kem_kyber_kyber768_amd64_avx2_keypair_derand:
	movq	%rsp, %rax
	leaq	-96(%rsp), %rsp
	andq	$-32, %rsp
	movq	%rax, 88(%rsp)
	movq	%r12, 64(%rsp)
	movq	%rbp, 72(%rsp)
	movq	%rbx, 80(%rsp)
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
	leaq	-13048(%rsp), %rsp
	call	L__crypto_kem_keypair_derand_jazz$1
Ljade_kem_kyber_kyber768_amd64_avx2_keypair_derand$1:
	leaq	13048(%rsp), %rsp
	xorq	%rax, %rax
	movq	64(%rsp), %r12
	movq	72(%rsp), %rbp
	movq	80(%rsp), %rbx
	movq	88(%rsp), %rsp
	ret
L__crypto_kem_enc_derand_jazz$1:
	movq	%r12, 16064(%rsp)
	movq	%rbx, 16072(%rsp)
	movq	%rbp, 16080(%rsp)
	leaq	64(%rsp), %rax
	leaq	-248(%rsp), %rsp
	call	L_sha3_256_32$1
L__crypto_kem_enc_derand_jazz$133:
	leaq	248(%rsp), %rsp
	movq	16064(%rsp), %r8
	movq	$1184, %rdi
	leaq	96(%rsp), %rax
	leaq	-248(%rsp), %rsp
	call	L_sha3_256$1
L__crypto_kem_enc_derand_jazz$132:
	leaq	248(%rsp), %rsp
	leaq	128(%rsp), %rax
	leaq	64(%rsp), %rcx
	leaq	-248(%rsp), %rsp
	call	L_sha3_512_64$1
L__crypto_kem_enc_derand_jazz$131:
	leaq	248(%rsp), %rsp
	movq	16064(%rsp), %rax
	leaq	64(%rsp), %rdi
	leaq	160(%rsp), %rcx
	movq	%rax, %rsi
	leaq	5312(%rsp), %r8
	call	L_poly_frombytes$1
L__crypto_kem_enc_derand_jazz$130:
	addq	$384, %rsi
	leaq	5824(%rsp), %r8
	call	L_poly_frombytes$1
L__crypto_kem_enc_derand_jazz$129:
	addq	$384, %rsi
	leaq	6336(%rsp), %r8
	call	L_poly_frombytes$1
L__crypto_kem_enc_derand_jazz$128:
	movq	$0, %rdx
	addq	$1152, %rax
	jmp 	L__crypto_kem_enc_derand_jazz$126
L__crypto_kem_enc_derand_jazz$127:
	movq	(%rax), %rsi
	movq	%rsi, 32(%rsp,%rdx,8)
	addq	$8, %rax
	incq	%rdx
L__crypto_kem_enc_derand_jazz$126:
	cmpq	$4, %rdx
	jb  	L__crypto_kem_enc_derand_jazz$127
	leaq	2464(%rsp), %rax
	call	L_poly_frommsg_1$1
L__crypto_kem_enc_derand_jazz$125:
	movq	%rcx, 16064(%rsp)
	vmovdqu	32(%rsp), %ymm0
	vmovdqu	%ymm0, 416(%rsp)
	vmovdqu	%ymm0, 928(%rsp)
	vmovdqu	%ymm0, 1440(%rsp)
	vmovdqu	%ymm0, 1952(%rsp)
	vmovdqu	%ymm0, 32(%rsp)
	movb	$0, 448(%rsp)
	movb	$0, 449(%rsp)
	movb	$0, 960(%rsp)
	movb	$1, 961(%rsp)
	movb	$0, 1472(%rsp)
	movb	$2, 1473(%rsp)
	movb	$1, 1984(%rsp)
	movb	$0, 1985(%rsp)
	leaq	4512(%rsp), %rax
	leaq	416(%rsp), %rcx
	leaq	928(%rsp), %rdx
	leaq	1440(%rsp), %rsi
	leaq	1952(%rsp), %rdi
	call	L_shake128_absorb4x_34$1
L__crypto_kem_enc_derand_jazz$124:
	leaq	4512(%rsp), %rax
	leaq	416(%rsp), %rdi
	leaq	928(%rsp), %r8
	leaq	1440(%rsp), %r9
	leaq	1952(%rsp), %r10
	movq	%rdi, %r11
	movq	%r8, %rbx
	movq	%r9, %rbp
	movq	%r10, %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_enc_derand_jazz$123:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	168(%rdi), %r11
	leaq	168(%r8), %rbx
	leaq	168(%r9), %rbp
	leaq	168(%r10), %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_enc_derand_jazz$122:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	336(%rdi), %rdi
	leaq	336(%r8), %r8
	leaq	336(%r9), %r9
	leaq	336(%r10), %r10
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_enc_derand_jazz$121:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%rdi)
	vmovhpd	%xmm1, (%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%r9)
	vmovhpd	%xmm0, (%r10)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%rdi)
	vmovhpd	%xmm1, 8(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%r9)
	vmovhpd	%xmm0, 8(%r10)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%rdi)
	vmovhpd	%xmm1, 16(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%r9)
	vmovhpd	%xmm0, 16(%r10)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%rdi)
	vmovhpd	%xmm1, 24(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%r9)
	vmovhpd	%xmm0, 24(%r10)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%rdi)
	vmovhpd	%xmm1, 32(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%r9)
	vmovhpd	%xmm0, 32(%r10)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%rdi)
	vmovhpd	%xmm1, 40(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%r9)
	vmovhpd	%xmm0, 40(%r10)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%rdi)
	vmovhpd	%xmm1, 48(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%r9)
	vmovhpd	%xmm0, 48(%r10)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%rdi)
	vmovhpd	%xmm1, 56(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%r9)
	vmovhpd	%xmm0, 56(%r10)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%rdi)
	vmovhpd	%xmm1, 64(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%r9)
	vmovhpd	%xmm0, 64(%r10)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%rdi)
	vmovhpd	%xmm1, 72(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%r9)
	vmovhpd	%xmm0, 72(%r10)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%rdi)
	vmovhpd	%xmm1, 80(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%r9)
	vmovhpd	%xmm0, 80(%r10)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%rdi)
	vmovhpd	%xmm1, 88(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%r9)
	vmovhpd	%xmm0, 88(%r10)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%rdi)
	vmovhpd	%xmm1, 96(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%r9)
	vmovhpd	%xmm0, 96(%r10)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%rdi)
	vmovhpd	%xmm1, 104(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%r9)
	vmovhpd	%xmm0, 104(%r10)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%rdi)
	vmovhpd	%xmm1, 112(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%r9)
	vmovhpd	%xmm0, 112(%r10)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%rdi)
	vmovhpd	%xmm1, 120(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%r9)
	vmovhpd	%xmm0, 120(%r10)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%rdi)
	vmovhpd	%xmm1, 128(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%r9)
	vmovhpd	%xmm0, 128(%r10)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%rdi)
	vmovhpd	%xmm1, 136(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%r9)
	vmovhpd	%xmm0, 136(%r10)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%rdi)
	vmovhpd	%xmm1, 144(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%r9)
	vmovhpd	%xmm0, 144(%r10)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%rdi)
	vmovhpd	%xmm1, 152(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%r9)
	vmovhpd	%xmm0, 152(%r10)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%rdi)
	vmovhpd	%xmm1, 160(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%r9)
	vmovhpd	%xmm0, 160(%r10)
	leaq	11456(%rsp), %r11
	leaq	416(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$120:
	movq	%r12, %r9
	leaq	11968(%rsp), %r11
	leaq	928(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$119:
	movq	%r12, %rdi
	leaq	12480(%rsp), %r11
	leaq	1440(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$118:
	movq	%r12, %r8
	leaq	12992(%rsp), %r11
	leaq	1952(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$117:
	cmpq	$255, %r9
	setbe	%al
	cmpq	$255, %rdi
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r8
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
	jmp 	L__crypto_kem_enc_derand_jazz$94
L__crypto_kem_enc_derand_jazz$95:
	leaq	4512(%rsp), %rax
	leaq	416(%rsp), %r10
	leaq	928(%rsp), %r11
	leaq	1440(%rsp), %rbx
	leaq	1952(%rsp), %rbp
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_enc_derand_jazz$116:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r10)
	vmovhpd	%xmm1, (%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbx)
	vmovhpd	%xmm0, (%rbp)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r10)
	vmovhpd	%xmm1, 8(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbx)
	vmovhpd	%xmm0, 8(%rbp)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r10)
	vmovhpd	%xmm1, 16(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbx)
	vmovhpd	%xmm0, 16(%rbp)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r10)
	vmovhpd	%xmm1, 24(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbx)
	vmovhpd	%xmm0, 24(%rbp)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r10)
	vmovhpd	%xmm1, 32(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbx)
	vmovhpd	%xmm0, 32(%rbp)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r10)
	vmovhpd	%xmm1, 40(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbx)
	vmovhpd	%xmm0, 40(%rbp)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r10)
	vmovhpd	%xmm1, 48(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbx)
	vmovhpd	%xmm0, 48(%rbp)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r10)
	vmovhpd	%xmm1, 56(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbx)
	vmovhpd	%xmm0, 56(%rbp)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r10)
	vmovhpd	%xmm1, 64(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbx)
	vmovhpd	%xmm0, 64(%rbp)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r10)
	vmovhpd	%xmm1, 72(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbx)
	vmovhpd	%xmm0, 72(%rbp)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r10)
	vmovhpd	%xmm1, 80(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbx)
	vmovhpd	%xmm0, 80(%rbp)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r10)
	vmovhpd	%xmm1, 88(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbx)
	vmovhpd	%xmm0, 88(%rbp)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r10)
	vmovhpd	%xmm1, 96(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbx)
	vmovhpd	%xmm0, 96(%rbp)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r10)
	vmovhpd	%xmm1, 104(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbx)
	vmovhpd	%xmm0, 104(%rbp)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r10)
	vmovhpd	%xmm1, 112(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbx)
	vmovhpd	%xmm0, 112(%rbp)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r10)
	vmovhpd	%xmm1, 120(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbx)
	vmovhpd	%xmm0, 120(%rbp)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r10)
	vmovhpd	%xmm1, 128(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbx)
	vmovhpd	%xmm0, 128(%rbp)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r10)
	vmovhpd	%xmm1, 136(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbx)
	vmovhpd	%xmm0, 136(%rbp)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r10)
	vmovhpd	%xmm1, 144(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbx)
	vmovhpd	%xmm0, 144(%rbp)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r10)
	vmovhpd	%xmm1, 152(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbx)
	vmovhpd	%xmm0, 152(%rbp)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r10)
	vmovhpd	%xmm1, 160(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbx)
	vmovhpd	%xmm0, 160(%rbp)
	leaq	11456(%rsp), %rax
	leaq	416(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$111
L__crypto_kem_enc_derand_jazz$112:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$115
	movw	%si, (%rax,%r9,2)
	incq	%r9
L__crypto_kem_enc_derand_jazz$115:
	cmpq	$256, %r9
	jnb 	L__crypto_kem_enc_derand_jazz$113
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_enc_derand_jazz$113
	movw	%r11w, (%rax,%r9,2)
	incq	%r9
L__crypto_kem_enc_derand_jazz$114:
L__crypto_kem_enc_derand_jazz$113:
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_enc_derand_jazz$111:
	jne 	L__crypto_kem_enc_derand_jazz$112
	leaq	11968(%rsp), %rax
	leaq	928(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$106
L__crypto_kem_enc_derand_jazz$107:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$110
	movw	%si, (%rax,%rdi,2)
	incq	%rdi
L__crypto_kem_enc_derand_jazz$110:
	cmpq	$256, %rdi
	jnb 	L__crypto_kem_enc_derand_jazz$108
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_enc_derand_jazz$108
	movw	%r11w, (%rax,%rdi,2)
	incq	%rdi
L__crypto_kem_enc_derand_jazz$109:
L__crypto_kem_enc_derand_jazz$108:
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_enc_derand_jazz$106:
	jne 	L__crypto_kem_enc_derand_jazz$107
	leaq	12480(%rsp), %rax
	leaq	1440(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$101
L__crypto_kem_enc_derand_jazz$102:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$105
	movw	%si, (%rax,%r8,2)
	incq	%r8
L__crypto_kem_enc_derand_jazz$105:
	cmpq	$256, %r8
	jnb 	L__crypto_kem_enc_derand_jazz$103
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_enc_derand_jazz$103
	movw	%r11w, (%rax,%r8,2)
	incq	%r8
L__crypto_kem_enc_derand_jazz$104:
L__crypto_kem_enc_derand_jazz$103:
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_enc_derand_jazz$101:
	jne 	L__crypto_kem_enc_derand_jazz$102
	leaq	12992(%rsp), %rax
	leaq	1952(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$96
L__crypto_kem_enc_derand_jazz$97:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$100
	movw	%si, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_enc_derand_jazz$100:
	cmpq	$256, %r12
	jnb 	L__crypto_kem_enc_derand_jazz$98
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_enc_derand_jazz$98
	movw	%r11w, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_enc_derand_jazz$99:
L__crypto_kem_enc_derand_jazz$98:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_enc_derand_jazz$96:
	jne 	L__crypto_kem_enc_derand_jazz$97
	cmpq	$255, %r9
	setbe	%al
	cmpq	$255, %rdi
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r8
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
L__crypto_kem_enc_derand_jazz$94:
	jne 	L__crypto_kem_enc_derand_jazz$95
	vmovdqu	32(%rsp), %ymm0
	vmovdqu	%ymm0, 416(%rsp)
	vmovdqu	%ymm0, 928(%rsp)
	vmovdqu	%ymm0, 1440(%rsp)
	vmovdqu	%ymm0, 1952(%rsp)
	vmovdqu	%ymm0, 32(%rsp)
	movb	$1, 448(%rsp)
	movb	$1, 449(%rsp)
	movb	$1, 960(%rsp)
	movb	$2, 961(%rsp)
	movb	$2, 1472(%rsp)
	movb	$0, 1473(%rsp)
	movb	$2, 1984(%rsp)
	movb	$1, 1985(%rsp)
	leaq	4512(%rsp), %rax
	leaq	416(%rsp), %rcx
	leaq	928(%rsp), %rdx
	leaq	1440(%rsp), %rsi
	leaq	1952(%rsp), %rdi
	call	L_shake128_absorb4x_34$1
L__crypto_kem_enc_derand_jazz$93:
	leaq	4512(%rsp), %rax
	leaq	416(%rsp), %rdi
	leaq	928(%rsp), %r8
	leaq	1440(%rsp), %r9
	leaq	1952(%rsp), %r10
	movq	%rdi, %r11
	movq	%r8, %rbx
	movq	%r9, %rbp
	movq	%r10, %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_enc_derand_jazz$92:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	168(%rdi), %r11
	leaq	168(%r8), %rbx
	leaq	168(%r9), %rbp
	leaq	168(%r10), %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_enc_derand_jazz$91:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	336(%rdi), %rdi
	leaq	336(%r8), %r8
	leaq	336(%r9), %r9
	leaq	336(%r10), %r10
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_enc_derand_jazz$90:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%rdi)
	vmovhpd	%xmm1, (%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%r9)
	vmovhpd	%xmm0, (%r10)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%rdi)
	vmovhpd	%xmm1, 8(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%r9)
	vmovhpd	%xmm0, 8(%r10)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%rdi)
	vmovhpd	%xmm1, 16(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%r9)
	vmovhpd	%xmm0, 16(%r10)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%rdi)
	vmovhpd	%xmm1, 24(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%r9)
	vmovhpd	%xmm0, 24(%r10)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%rdi)
	vmovhpd	%xmm1, 32(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%r9)
	vmovhpd	%xmm0, 32(%r10)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%rdi)
	vmovhpd	%xmm1, 40(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%r9)
	vmovhpd	%xmm0, 40(%r10)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%rdi)
	vmovhpd	%xmm1, 48(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%r9)
	vmovhpd	%xmm0, 48(%r10)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%rdi)
	vmovhpd	%xmm1, 56(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%r9)
	vmovhpd	%xmm0, 56(%r10)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%rdi)
	vmovhpd	%xmm1, 64(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%r9)
	vmovhpd	%xmm0, 64(%r10)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%rdi)
	vmovhpd	%xmm1, 72(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%r9)
	vmovhpd	%xmm0, 72(%r10)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%rdi)
	vmovhpd	%xmm1, 80(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%r9)
	vmovhpd	%xmm0, 80(%r10)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%rdi)
	vmovhpd	%xmm1, 88(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%r9)
	vmovhpd	%xmm0, 88(%r10)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%rdi)
	vmovhpd	%xmm1, 96(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%r9)
	vmovhpd	%xmm0, 96(%r10)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%rdi)
	vmovhpd	%xmm1, 104(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%r9)
	vmovhpd	%xmm0, 104(%r10)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%rdi)
	vmovhpd	%xmm1, 112(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%r9)
	vmovhpd	%xmm0, 112(%r10)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%rdi)
	vmovhpd	%xmm1, 120(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%r9)
	vmovhpd	%xmm0, 120(%r10)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%rdi)
	vmovhpd	%xmm1, 128(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%r9)
	vmovhpd	%xmm0, 128(%r10)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%rdi)
	vmovhpd	%xmm1, 136(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%r9)
	vmovhpd	%xmm0, 136(%r10)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%rdi)
	vmovhpd	%xmm1, 144(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%r9)
	vmovhpd	%xmm0, 144(%r10)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%rdi)
	vmovhpd	%xmm1, 152(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%r9)
	vmovhpd	%xmm0, 152(%r10)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%rdi)
	vmovhpd	%xmm1, 160(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%r9)
	vmovhpd	%xmm0, 160(%r10)
	leaq	13504(%rsp), %r11
	leaq	416(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$89:
	movq	%r12, %rdi
	leaq	14016(%rsp), %r11
	leaq	928(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$88:
	movq	%r12, %r8
	leaq	14528(%rsp), %r11
	leaq	1440(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$87:
	movq	%r12, %r9
	leaq	15040(%rsp), %r11
	leaq	1952(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$86:
	cmpq	$255, %rdi
	setbe	%al
	cmpq	$255, %r8
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r9
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
	jmp 	L__crypto_kem_enc_derand_jazz$63
L__crypto_kem_enc_derand_jazz$64:
	leaq	4512(%rsp), %rax
	leaq	416(%rsp), %r10
	leaq	928(%rsp), %r11
	leaq	1440(%rsp), %rbx
	leaq	1952(%rsp), %rbp
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_enc_derand_jazz$85:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r10)
	vmovhpd	%xmm1, (%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbx)
	vmovhpd	%xmm0, (%rbp)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r10)
	vmovhpd	%xmm1, 8(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbx)
	vmovhpd	%xmm0, 8(%rbp)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r10)
	vmovhpd	%xmm1, 16(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbx)
	vmovhpd	%xmm0, 16(%rbp)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r10)
	vmovhpd	%xmm1, 24(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbx)
	vmovhpd	%xmm0, 24(%rbp)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r10)
	vmovhpd	%xmm1, 32(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbx)
	vmovhpd	%xmm0, 32(%rbp)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r10)
	vmovhpd	%xmm1, 40(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbx)
	vmovhpd	%xmm0, 40(%rbp)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r10)
	vmovhpd	%xmm1, 48(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbx)
	vmovhpd	%xmm0, 48(%rbp)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r10)
	vmovhpd	%xmm1, 56(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbx)
	vmovhpd	%xmm0, 56(%rbp)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r10)
	vmovhpd	%xmm1, 64(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbx)
	vmovhpd	%xmm0, 64(%rbp)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r10)
	vmovhpd	%xmm1, 72(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbx)
	vmovhpd	%xmm0, 72(%rbp)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r10)
	vmovhpd	%xmm1, 80(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbx)
	vmovhpd	%xmm0, 80(%rbp)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r10)
	vmovhpd	%xmm1, 88(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbx)
	vmovhpd	%xmm0, 88(%rbp)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r10)
	vmovhpd	%xmm1, 96(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbx)
	vmovhpd	%xmm0, 96(%rbp)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r10)
	vmovhpd	%xmm1, 104(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbx)
	vmovhpd	%xmm0, 104(%rbp)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r10)
	vmovhpd	%xmm1, 112(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbx)
	vmovhpd	%xmm0, 112(%rbp)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r10)
	vmovhpd	%xmm1, 120(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbx)
	vmovhpd	%xmm0, 120(%rbp)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r10)
	vmovhpd	%xmm1, 128(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbx)
	vmovhpd	%xmm0, 128(%rbp)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r10)
	vmovhpd	%xmm1, 136(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbx)
	vmovhpd	%xmm0, 136(%rbp)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r10)
	vmovhpd	%xmm1, 144(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbx)
	vmovhpd	%xmm0, 144(%rbp)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r10)
	vmovhpd	%xmm1, 152(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbx)
	vmovhpd	%xmm0, 152(%rbp)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r10)
	vmovhpd	%xmm1, 160(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbx)
	vmovhpd	%xmm0, 160(%rbp)
	leaq	13504(%rsp), %rax
	leaq	416(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$80
L__crypto_kem_enc_derand_jazz$81:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$84
	movw	%si, (%rax,%rdi,2)
	incq	%rdi
L__crypto_kem_enc_derand_jazz$84:
	cmpq	$256, %rdi
	jnb 	L__crypto_kem_enc_derand_jazz$82
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_enc_derand_jazz$82
	movw	%r11w, (%rax,%rdi,2)
	incq	%rdi
L__crypto_kem_enc_derand_jazz$83:
L__crypto_kem_enc_derand_jazz$82:
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_enc_derand_jazz$80:
	jne 	L__crypto_kem_enc_derand_jazz$81
	leaq	14016(%rsp), %rax
	leaq	928(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$75
L__crypto_kem_enc_derand_jazz$76:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$79
	movw	%si, (%rax,%r8,2)
	incq	%r8
L__crypto_kem_enc_derand_jazz$79:
	cmpq	$256, %r8
	jnb 	L__crypto_kem_enc_derand_jazz$77
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_enc_derand_jazz$77
	movw	%r11w, (%rax,%r8,2)
	incq	%r8
L__crypto_kem_enc_derand_jazz$78:
L__crypto_kem_enc_derand_jazz$77:
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_enc_derand_jazz$75:
	jne 	L__crypto_kem_enc_derand_jazz$76
	leaq	14528(%rsp), %rax
	leaq	1440(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$70
L__crypto_kem_enc_derand_jazz$71:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$74
	movw	%si, (%rax,%r9,2)
	incq	%r9
L__crypto_kem_enc_derand_jazz$74:
	cmpq	$256, %r9
	jnb 	L__crypto_kem_enc_derand_jazz$72
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_enc_derand_jazz$72
	movw	%r11w, (%rax,%r9,2)
	incq	%r9
L__crypto_kem_enc_derand_jazz$73:
L__crypto_kem_enc_derand_jazz$72:
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_enc_derand_jazz$70:
	jne 	L__crypto_kem_enc_derand_jazz$71
	leaq	15040(%rsp), %rax
	leaq	1952(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$65
L__crypto_kem_enc_derand_jazz$66:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$69
	movw	%si, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_enc_derand_jazz$69:
	cmpq	$256, %r12
	jnb 	L__crypto_kem_enc_derand_jazz$67
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_enc_derand_jazz$67
	movw	%r11w, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_enc_derand_jazz$68:
L__crypto_kem_enc_derand_jazz$67:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_enc_derand_jazz$65:
	jne 	L__crypto_kem_enc_derand_jazz$66
	cmpq	$255, %rdi
	setbe	%al
	cmpq	$255, %r8
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r9
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
L__crypto_kem_enc_derand_jazz$63:
	jne 	L__crypto_kem_enc_derand_jazz$64
	vmovdqu	32(%rsp), %ymm0
	vmovdqu	%ymm0, 416(%rsp)
	movb	$2, 448(%rsp)
	movb	$2, 449(%rsp)
	leaq	416(%rsp), %rax
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vmovdqu	%ymm0, 256(%rsp)
	vmovdqu	%ymm0, 288(%rsp)
	vmovdqu	%ymm0, 320(%rsp)
	vmovdqu	%ymm0, 352(%rsp)
	vmovdqu	%ymm0, 384(%rsp)
	vpbroadcastq	(%rax), %ymm5
	movq	8(%rax), %rcx
	movq	glob_data + 6208(%rip), %rdx
	movq	%rcx, 192(%rsp,%rdx,8)
	movq	16(%rax), %rcx
	movq	glob_data + 6216(%rip), %rdx
	movq	%rcx, 192(%rsp,%rdx,8)
	movq	24(%rax), %rcx
	movq	glob_data + 6224(%rip), %rdx
	movq	%rcx, 192(%rsp,%rdx,8)
	movb	32(%rax), %cl
	movq	glob_data + 6232(%rip), %rdx
	shlq	$3, %rdx
	movb	%cl, 192(%rsp,%rdx)
	movb	33(%rax), %al
	incq	%rdx
	movb	%al, 192(%rsp,%rdx)
	incq	%rdx
	movb	$31, 192(%rsp,%rdx)
	movq	glob_data + 6360(%rip), %rax
	shlq	$3, %rax
	movq	$167, %rcx
	andq	$7, %rcx
	addq	%rcx, %rax
	movb	$-128, 192(%rsp,%rax)
	vmovdqu	224(%rsp), %ymm0
	vmovdqu	256(%rsp), %ymm1
	vmovdqu	288(%rsp), %ymm2
	vmovdqu	320(%rsp), %ymm6
	vmovdqu	352(%rsp), %ymm3
	vmovdqu	384(%rsp), %ymm4
	leaq	416(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L__crypto_kem_enc_derand_jazz$62:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	L__crypto_kem_enc_derand_jazz$62
	vmovdqu	%ymm5, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vmovdqu	%ymm1, 256(%rsp)
	vmovdqu	%ymm2, 288(%rsp)
	vmovdqu	%ymm6, 320(%rsp)
	vmovdqu	%ymm3, 352(%rsp)
	vmovdqu	%ymm4, 384(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	584(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L__crypto_kem_enc_derand_jazz$61:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	L__crypto_kem_enc_derand_jazz$61
	vmovdqu	%ymm5, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vmovdqu	%ymm1, 256(%rsp)
	vmovdqu	%ymm2, 288(%rsp)
	vmovdqu	%ymm6, 320(%rsp)
	vmovdqu	%ymm3, 352(%rsp)
	vmovdqu	%ymm4, 384(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	752(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L__crypto_kem_enc_derand_jazz$60:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	L__crypto_kem_enc_derand_jazz$60
	vmovdqu	%ymm5, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vmovdqu	%ymm1, 256(%rsp)
	vmovdqu	%ymm2, 288(%rsp)
	vmovdqu	%ymm6, 320(%rsp)
	vmovdqu	%ymm3, 352(%rsp)
	vmovdqu	%ymm4, 384(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	vmovdqu	%ymm5, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vmovdqu	%ymm1, 256(%rsp)
	vmovdqu	%ymm2, 288(%rsp)
	vmovdqu	%ymm6, 320(%rsp)
	vmovdqu	%ymm3, 352(%rsp)
	vmovdqu	%ymm4, 384(%rsp)
	leaq	15552(%rsp), %r11
	leaq	416(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_enc_derand_jazz$59:
	cmpq	$255, %r12
	setbe	%al
	vmovdqu	192(%rsp), %ymm6
	vmovdqu	224(%rsp), %ymm0
	vmovdqu	256(%rsp), %ymm1
	vmovdqu	288(%rsp), %ymm2
	vmovdqu	320(%rsp), %ymm3
	vmovdqu	352(%rsp), %ymm4
	vmovdqu	384(%rsp), %ymm5
	jmp 	L__crypto_kem_enc_derand_jazz$51
L__crypto_kem_enc_derand_jazz$52:
	leaq	416(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L__crypto_kem_enc_derand_jazz$58:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm4, %ymm7
	vpxor	%ymm5, %ymm3, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm6, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm6, %ymm6
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	96(%rsi), %ymm3, %ymm7
	vpsrlvq	96(%rdi), %ymm3, %ymm3
	vpor	%ymm7, %ymm3, %ymm10
	vpxor	%ymm9, %ymm4, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm4
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm4, %ymm3, %ymm4
	vpxor	%ymm9, %ymm5, %ymm5
	vpermq	$-115, %ymm1, %ymm3
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm5, %ymm1
	vpsrlvq	160(%rdi), %ymm5, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm4, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm4
	vpblendd	$12, %ymm7, %ymm3, %ymm5
	vpblendd	$12, %ymm3, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm4, %ymm4
	vpblendd	$48, %ymm10, %ymm5, %ymm5
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm9, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm4, %ymm1, %ymm1
	vpandn	%ymm11, %ymm5, %ymm5
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm3, %ymm12
	vpxor	%ymm3, %ymm1, %ymm4
	vpblendd	$48, %ymm3, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm6, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm6, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm3, %ymm13, %ymm13
	vpblendd	$-64, %ymm3, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm4, %ymm2
	vpermq	$-115, %ymm5, %ymm4
	vpermq	$114, %ymm12, %ymm5
	vpblendd	$12, %ymm3, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm3, %ymm9, %ymm3
	vpandn	%ymm3, %ymm0, %ymm3
	vpxor	%ymm13, %ymm6, %ymm6
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm3, %ymm3
	vpxor	(%rcx,%rdx), %ymm6, %ymm6
	addq	$32, %rdx
	decq	%r8
	jne 	L__crypto_kem_enc_derand_jazz$58
	vmovdqu	%ymm6, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vmovdqu	%ymm1, 256(%rsp)
	vmovdqu	%ymm2, 288(%rsp)
	vmovdqu	%ymm3, 320(%rsp)
	vmovdqu	%ymm4, 352(%rsp)
	vmovdqu	%ymm5, 384(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	192(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	15552(%rsp), %rax
	leaq	416(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%dil
	testb	%dil, %sil
	jmp 	L__crypto_kem_enc_derand_jazz$53
L__crypto_kem_enc_derand_jazz$54:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %di
	movw	%di, %r8w
	shrw	$4, %r8w
	andw	$15, %di
	shlw	$8, %di
	orw 	%di, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %di
	shlw	$4, %di
	orw 	%di, %r8w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_enc_derand_jazz$57
	movw	%si, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_enc_derand_jazz$57:
	cmpq	$256, %r12
	jnb 	L__crypto_kem_enc_derand_jazz$55
	cmpw	$3329, %r8w
	jnb 	L__crypto_kem_enc_derand_jazz$55
	movw	%r8w, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_enc_derand_jazz$56:
L__crypto_kem_enc_derand_jazz$55:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%dil
	testb	%dil, %sil
L__crypto_kem_enc_derand_jazz$53:
	jne 	L__crypto_kem_enc_derand_jazz$54
	cmpq	$255, %r12
	setbe	%al
L__crypto_kem_enc_derand_jazz$51:
	cmpb	$0, %al
	jne 	L__crypto_kem_enc_derand_jazz$52
	leaq	11456(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$50:
	leaq	11968(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$49:
	leaq	12480(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$48:
	leaq	12992(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$47:
	leaq	13504(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$46:
	leaq	14016(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$45:
	leaq	14528(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$44:
	leaq	15040(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$43:
	leaq	15552(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_enc_derand_jazz$42:
	movq	16064(%rsp), %r10
	movb	$0, %cl
	leaq	8384(%rsp), %rax
	leaq	8896(%rsp), %rdi
	leaq	9408(%rsp), %r8
	leaq	6848(%rsp), %r9
	leaq	-1496(%rsp), %rsp
	call	L_poly_getnoise_eta1_4x$1
L__crypto_kem_enc_derand_jazz$41:
	leaq	1496(%rsp), %rsp
	movb	$4, %cl
	leaq	7360(%rsp), %rax
	leaq	7872(%rsp), %rdi
	leaq	2976(%rsp), %r8
	leaq	9920(%rsp), %r9
	leaq	-1496(%rsp), %rsp
	call	L_poly_getnoise_eta1_4x$1
L__crypto_kem_enc_derand_jazz$40:
	leaq	1496(%rsp), %rsp
	leaq	8384(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_enc_derand_jazz$39:
	leaq	8896(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_enc_derand_jazz$38:
	leaq	9408(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_enc_derand_jazz$37:
	leaq	9920(%rsp), %rcx
	leaq	11456(%rsp), %rsi
	leaq	8384(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$36:
	leaq	3488(%rsp), %rcx
	leaq	11968(%rsp), %rsi
	leaq	8896(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$35:
	leaq	9920(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$34:
	leaq	3488(%rsp), %rcx
	leaq	12480(%rsp), %rsi
	leaq	9408(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$33:
	leaq	9920(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$32:
	leaq	10432(%rsp), %rcx
	leaq	12992(%rsp), %rsi
	leaq	8384(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$31:
	leaq	3488(%rsp), %rcx
	leaq	13504(%rsp), %rsi
	leaq	8896(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$30:
	leaq	10432(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$29:
	leaq	3488(%rsp), %rcx
	leaq	14016(%rsp), %rsi
	leaq	9408(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$28:
	leaq	10432(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$27:
	leaq	10944(%rsp), %rcx
	leaq	14528(%rsp), %rsi
	leaq	8384(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$26:
	leaq	3488(%rsp), %rcx
	leaq	15040(%rsp), %rsi
	leaq	8896(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$25:
	leaq	10944(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$24:
	leaq	3488(%rsp), %rcx
	leaq	15552(%rsp), %rsi
	leaq	9408(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$23:
	leaq	10944(%rsp), %rcx
	leaq	3488(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$22:
	leaq	3488(%rsp), %rcx
	leaq	5312(%rsp), %rsi
	leaq	8384(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$21:
	leaq	4000(%rsp), %rcx
	leaq	5824(%rsp), %rsi
	leaq	8896(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$20:
	leaq	3488(%rsp), %rcx
	leaq	4000(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$19:
	leaq	4000(%rsp), %rcx
	leaq	6336(%rsp), %rsi
	leaq	9408(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_enc_derand_jazz$18:
	leaq	3488(%rsp), %rcx
	leaq	4000(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$17:
	leaq	9920(%rsp), %rcx
	call	L_poly_invntt$1
L__crypto_kem_enc_derand_jazz$16:
	leaq	10432(%rsp), %rcx
	call	L_poly_invntt$1
L__crypto_kem_enc_derand_jazz$15:
	leaq	10944(%rsp), %rcx
	call	L_poly_invntt$1
L__crypto_kem_enc_derand_jazz$14:
	leaq	3488(%rsp), %rcx
	call	L_poly_invntt$1
L__crypto_kem_enc_derand_jazz$13:
	leaq	9920(%rsp), %rcx
	leaq	6848(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$12:
	leaq	10432(%rsp), %rcx
	leaq	7360(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$11:
	leaq	10944(%rsp), %rcx
	leaq	7872(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$10:
	leaq	3488(%rsp), %rcx
	leaq	2976(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$9:
	leaq	3488(%rsp), %rcx
	leaq	2464(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_enc_derand_jazz$8:
	leaq	9920(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	leaq	10432(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	leaq	10944(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	leaq	3488(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	movq	16072(%rsp), %rax
	leaq	9920(%rsp), %rcx
	call	L_poly_csubq$1
L__crypto_kem_enc_derand_jazz$7:
	leaq	10432(%rsp), %rcx
	call	L_poly_csubq$1
L__crypto_kem_enc_derand_jazz$6:
	leaq	10944(%rsp), %rcx
	call	L_poly_csubq$1
L__crypto_kem_enc_derand_jazz$5:
	leaq	glob_data + 384(%rip), %rcx
	vmovdqu	(%rcx), %ymm0
	vpsllw	$3, %ymm0, %ymm1
	vpbroadcastw	glob_data + 6422(%rip), %ymm2
	vpbroadcastw	glob_data + 6420(%rip), %ymm3
	vpbroadcastw	glob_data + 6418(%rip), %ymm4
	vpbroadcastq	glob_data + 6184(%rip), %ymm5
	vpbroadcastq	glob_data + 6176(%rip), %ymm6
	vmovdqu	glob_data + 32(%rip), %ymm7
	vmovdqu	9920(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, (%rax)
	vpextrd	$0, %xmm8, 16(%rax)
	vmovdqu	9952(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 20(%rax)
	vpextrd	$0, %xmm8, 36(%rax)
	vmovdqu	9984(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 40(%rax)
	vpextrd	$0, %xmm8, 56(%rax)
	vmovdqu	10016(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 60(%rax)
	vpextrd	$0, %xmm8, 76(%rax)
	vmovdqu	10048(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 80(%rax)
	vpextrd	$0, %xmm8, 96(%rax)
	vmovdqu	10080(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 100(%rax)
	vpextrd	$0, %xmm8, 116(%rax)
	vmovdqu	10112(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 120(%rax)
	vpextrd	$0, %xmm8, 136(%rax)
	vmovdqu	10144(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 140(%rax)
	vpextrd	$0, %xmm8, 156(%rax)
	vmovdqu	10176(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 160(%rax)
	vpextrd	$0, %xmm8, 176(%rax)
	vmovdqu	10208(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 180(%rax)
	vpextrd	$0, %xmm8, 196(%rax)
	vmovdqu	10240(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 200(%rax)
	vpextrd	$0, %xmm8, 216(%rax)
	vmovdqu	10272(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 220(%rax)
	vpextrd	$0, %xmm8, 236(%rax)
	vmovdqu	10304(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 240(%rax)
	vpextrd	$0, %xmm8, 256(%rax)
	vmovdqu	10336(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 260(%rax)
	vpextrd	$0, %xmm8, 276(%rax)
	vmovdqu	10368(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 280(%rax)
	vpextrd	$0, %xmm8, 296(%rax)
	vmovdqu	10400(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 300(%rax)
	vpextrd	$0, %xmm8, 316(%rax)
	vmovdqu	10432(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 320(%rax)
	vpextrd	$0, %xmm8, 336(%rax)
	vmovdqu	10464(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 340(%rax)
	vpextrd	$0, %xmm8, 356(%rax)
	vmovdqu	10496(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 360(%rax)
	vpextrd	$0, %xmm8, 376(%rax)
	vmovdqu	10528(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 380(%rax)
	vpextrd	$0, %xmm8, 396(%rax)
	vmovdqu	10560(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 400(%rax)
	vpextrd	$0, %xmm8, 416(%rax)
	vmovdqu	10592(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 420(%rax)
	vpextrd	$0, %xmm8, 436(%rax)
	vmovdqu	10624(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 440(%rax)
	vpextrd	$0, %xmm8, 456(%rax)
	vmovdqu	10656(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 460(%rax)
	vpextrd	$0, %xmm8, 476(%rax)
	vmovdqu	10688(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 480(%rax)
	vpextrd	$0, %xmm8, 496(%rax)
	vmovdqu	10720(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 500(%rax)
	vpextrd	$0, %xmm8, 516(%rax)
	vmovdqu	10752(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 520(%rax)
	vpextrd	$0, %xmm8, 536(%rax)
	vmovdqu	10784(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 540(%rax)
	vpextrd	$0, %xmm8, 556(%rax)
	vmovdqu	10816(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 560(%rax)
	vpextrd	$0, %xmm8, 576(%rax)
	vmovdqu	10848(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 580(%rax)
	vpextrd	$0, %xmm8, 596(%rax)
	vmovdqu	10880(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 600(%rax)
	vpextrd	$0, %xmm8, 616(%rax)
	vmovdqu	10912(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 620(%rax)
	vpextrd	$0, %xmm8, 636(%rax)
	vmovdqu	10944(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 640(%rax)
	vpextrd	$0, %xmm8, 656(%rax)
	vmovdqu	10976(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 660(%rax)
	vpextrd	$0, %xmm8, 676(%rax)
	vmovdqu	11008(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 680(%rax)
	vpextrd	$0, %xmm8, 696(%rax)
	vmovdqu	11040(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 700(%rax)
	vpextrd	$0, %xmm8, 716(%rax)
	vmovdqu	11072(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 720(%rax)
	vpextrd	$0, %xmm8, 736(%rax)
	vmovdqu	11104(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 740(%rax)
	vpextrd	$0, %xmm8, 756(%rax)
	vmovdqu	11136(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 760(%rax)
	vpextrd	$0, %xmm8, 776(%rax)
	vmovdqu	11168(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 780(%rax)
	vpextrd	$0, %xmm8, 796(%rax)
	vmovdqu	11200(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 800(%rax)
	vpextrd	$0, %xmm8, 816(%rax)
	vmovdqu	11232(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 820(%rax)
	vpextrd	$0, %xmm8, 836(%rax)
	vmovdqu	11264(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 840(%rax)
	vpextrd	$0, %xmm8, 856(%rax)
	vmovdqu	11296(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 860(%rax)
	vpextrd	$0, %xmm8, 876(%rax)
	vmovdqu	11328(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 880(%rax)
	vpextrd	$0, %xmm8, 896(%rax)
	vmovdqu	11360(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 900(%rax)
	vpextrd	$0, %xmm8, 916(%rax)
	vmovdqu	11392(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm9
	vpaddw	%ymm2, %ymm8, %ymm10
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm9, %ymm10
	vpandn	%ymm10, %ymm9, %ymm9
	vpsrlw	$15, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm8, %ymm8
	vpmulhrsw	%ymm3, %ymm8, %ymm8
	vpand	%ymm4, %ymm8, %ymm8
	vpmaddwd	%ymm5, %ymm8, %ymm8
	vpsllvd	%ymm6, %ymm8, %ymm8
	vpsrlq	$12, %ymm8, %ymm8
	vpshufb	%ymm7, %ymm8, %ymm8
	vmovdqu	%xmm8, %xmm9
	vextracti128	$1, %ymm8, %xmm8
	vpblendw	$-32, %xmm8, %xmm9, %xmm9
	vmovdqu	%xmm9, 920(%rax)
	vpextrd	$0, %xmm8, 936(%rax)
	vmovdqu	11424(%rsp), %ymm8
	vpmullw	%ymm1, %ymm8, %ymm1
	vpaddw	%ymm2, %ymm8, %ymm2
	vpsllw	$3, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm8, %ymm0
	vpsubw	%ymm2, %ymm1, %ymm2
	vpandn	%ymm2, %ymm1, %ymm1
	vpsrlw	$15, %ymm1, %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vpmulhrsw	%ymm3, %ymm0, %ymm0
	vpand	%ymm4, %ymm0, %ymm0
	vpmaddwd	%ymm5, %ymm0, %ymm0
	vpsllvd	%ymm6, %ymm0, %ymm0
	vpsrlq	$12, %ymm0, %ymm0
	vpshufb	%ymm7, %ymm0, %ymm0
	vmovdqu	%xmm0, %xmm1
	vextracti128	$1, %ymm0, %xmm0
	vpblendw	$-32, %xmm0, %xmm1, %xmm1
	vmovdqu	%xmm1, 940(%rax)
	vpextrd	$0, %xmm0, 956(%rax)
	addq	$960, %rax
	leaq	3488(%rsp), %rcx
	call	L_poly_compress$1
L__crypto_kem_enc_derand_jazz$4:
	movq	16072(%rsp), %r8
	movq	$1088, %rdi
	leaq	160(%rsp), %rax
	leaq	-248(%rsp), %rsp
	call	L_sha3_256$1
L__crypto_kem_enc_derand_jazz$3:
	leaq	248(%rsp), %rsp
	movq	16080(%rsp), %rdx
	movq	$32, %rcx
	leaq	128(%rsp), %rax
	leaq	-248(%rsp), %rsp
	call	L_shake256_64$1
L__crypto_kem_enc_derand_jazz$2:
	leaq	248(%rsp), %rsp
	ret
L__crypto_kem_keypair_derand_jazz$1:
	movq	%rax, 12928(%rsp)
	movq	%rbp, 12936(%rsp)
	movq	%rbx, 12944(%rsp)
	movq	%rax, %rcx
	movq	%rbp, 12952(%rsp)
	movq	%rbx, 12960(%rsp)
	leaq	12968(%rsp), %rax
	leaq	-248(%rsp), %rsp
	call	L_sha3_512_32$1
L__crypto_kem_keypair_derand_jazz$121:
	leaq	248(%rsp), %rsp
	movq	12968(%rsp), %rax
	movq	%rax, 64(%rsp)
	movq	13000(%rsp), %rax
	movq	%rax, 32(%rsp)
	movq	12976(%rsp), %rax
	movq	%rax, 72(%rsp)
	movq	13008(%rsp), %rax
	movq	%rax, 40(%rsp)
	movq	12984(%rsp), %rax
	movq	%rax, 80(%rsp)
	movq	13016(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	12992(%rsp), %rax
	movq	%rax, 88(%rsp)
	movq	13024(%rsp), %rax
	movq	%rax, 56(%rsp)
	vmovdqu	64(%rsp), %ymm0
	vmovdqu	%ymm0, 352(%rsp)
	vmovdqu	%ymm0, 864(%rsp)
	vmovdqu	%ymm0, 1376(%rsp)
	vmovdqu	%ymm0, 1888(%rsp)
	vmovdqu	%ymm0, 96(%rsp)
	movb	$0, 384(%rsp)
	movb	$0, 385(%rsp)
	movb	$1, 896(%rsp)
	movb	$0, 897(%rsp)
	movb	$2, 1408(%rsp)
	movb	$0, 1409(%rsp)
	movb	$0, 1920(%rsp)
	movb	$1, 1921(%rsp)
	leaq	2912(%rsp), %rax
	leaq	352(%rsp), %rcx
	leaq	864(%rsp), %rdx
	leaq	1376(%rsp), %rsi
	leaq	1888(%rsp), %rdi
	call	L_shake128_absorb4x_34$1
L__crypto_kem_keypair_derand_jazz$120:
	leaq	2912(%rsp), %rax
	leaq	352(%rsp), %rdi
	leaq	864(%rsp), %r8
	leaq	1376(%rsp), %r9
	leaq	1888(%rsp), %r10
	movq	%rdi, %r11
	movq	%r8, %rbx
	movq	%r9, %rbp
	movq	%r10, %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_keypair_derand_jazz$119:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	168(%rdi), %r11
	leaq	168(%r8), %rbx
	leaq	168(%r9), %rbp
	leaq	168(%r10), %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_keypair_derand_jazz$118:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	336(%rdi), %rdi
	leaq	336(%r8), %r8
	leaq	336(%r9), %r9
	leaq	336(%r10), %r10
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_keypair_derand_jazz$117:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%rdi)
	vmovhpd	%xmm1, (%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%r9)
	vmovhpd	%xmm0, (%r10)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%rdi)
	vmovhpd	%xmm1, 8(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%r9)
	vmovhpd	%xmm0, 8(%r10)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%rdi)
	vmovhpd	%xmm1, 16(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%r9)
	vmovhpd	%xmm0, 16(%r10)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%rdi)
	vmovhpd	%xmm1, 24(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%r9)
	vmovhpd	%xmm0, 24(%r10)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%rdi)
	vmovhpd	%xmm1, 32(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%r9)
	vmovhpd	%xmm0, 32(%r10)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%rdi)
	vmovhpd	%xmm1, 40(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%r9)
	vmovhpd	%xmm0, 40(%r10)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%rdi)
	vmovhpd	%xmm1, 48(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%r9)
	vmovhpd	%xmm0, 48(%r10)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%rdi)
	vmovhpd	%xmm1, 56(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%r9)
	vmovhpd	%xmm0, 56(%r10)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%rdi)
	vmovhpd	%xmm1, 64(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%r9)
	vmovhpd	%xmm0, 64(%r10)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%rdi)
	vmovhpd	%xmm1, 72(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%r9)
	vmovhpd	%xmm0, 72(%r10)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%rdi)
	vmovhpd	%xmm1, 80(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%r9)
	vmovhpd	%xmm0, 80(%r10)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%rdi)
	vmovhpd	%xmm1, 88(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%r9)
	vmovhpd	%xmm0, 88(%r10)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%rdi)
	vmovhpd	%xmm1, 96(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%r9)
	vmovhpd	%xmm0, 96(%r10)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%rdi)
	vmovhpd	%xmm1, 104(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%r9)
	vmovhpd	%xmm0, 104(%r10)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%rdi)
	vmovhpd	%xmm1, 112(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%r9)
	vmovhpd	%xmm0, 112(%r10)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%rdi)
	vmovhpd	%xmm1, 120(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%r9)
	vmovhpd	%xmm0, 120(%r10)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%rdi)
	vmovhpd	%xmm1, 128(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%r9)
	vmovhpd	%xmm0, 128(%r10)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%rdi)
	vmovhpd	%xmm1, 136(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%r9)
	vmovhpd	%xmm0, 136(%r10)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%rdi)
	vmovhpd	%xmm1, 144(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%r9)
	vmovhpd	%xmm0, 144(%r10)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%rdi)
	vmovhpd	%xmm1, 152(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%r9)
	vmovhpd	%xmm0, 152(%r10)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%rdi)
	vmovhpd	%xmm1, 160(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%r9)
	vmovhpd	%xmm0, 160(%r10)
	leaq	8320(%rsp), %r11
	leaq	352(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$116:
	movq	%r12, %r9
	leaq	8832(%rsp), %r11
	leaq	864(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$115:
	movq	%r12, %rdi
	leaq	9344(%rsp), %r11
	leaq	1376(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$114:
	movq	%r12, %r8
	leaq	9856(%rsp), %r11
	leaq	1888(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$113:
	cmpq	$255, %r9
	setbe	%al
	cmpq	$255, %rdi
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r8
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
	jmp 	L__crypto_kem_keypair_derand_jazz$90
L__crypto_kem_keypair_derand_jazz$91:
	leaq	2912(%rsp), %rax
	leaq	352(%rsp), %r10
	leaq	864(%rsp), %r11
	leaq	1376(%rsp), %rbx
	leaq	1888(%rsp), %rbp
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_keypair_derand_jazz$112:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r10)
	vmovhpd	%xmm1, (%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbx)
	vmovhpd	%xmm0, (%rbp)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r10)
	vmovhpd	%xmm1, 8(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbx)
	vmovhpd	%xmm0, 8(%rbp)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r10)
	vmovhpd	%xmm1, 16(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbx)
	vmovhpd	%xmm0, 16(%rbp)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r10)
	vmovhpd	%xmm1, 24(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbx)
	vmovhpd	%xmm0, 24(%rbp)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r10)
	vmovhpd	%xmm1, 32(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbx)
	vmovhpd	%xmm0, 32(%rbp)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r10)
	vmovhpd	%xmm1, 40(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbx)
	vmovhpd	%xmm0, 40(%rbp)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r10)
	vmovhpd	%xmm1, 48(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbx)
	vmovhpd	%xmm0, 48(%rbp)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r10)
	vmovhpd	%xmm1, 56(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbx)
	vmovhpd	%xmm0, 56(%rbp)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r10)
	vmovhpd	%xmm1, 64(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbx)
	vmovhpd	%xmm0, 64(%rbp)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r10)
	vmovhpd	%xmm1, 72(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbx)
	vmovhpd	%xmm0, 72(%rbp)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r10)
	vmovhpd	%xmm1, 80(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbx)
	vmovhpd	%xmm0, 80(%rbp)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r10)
	vmovhpd	%xmm1, 88(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbx)
	vmovhpd	%xmm0, 88(%rbp)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r10)
	vmovhpd	%xmm1, 96(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbx)
	vmovhpd	%xmm0, 96(%rbp)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r10)
	vmovhpd	%xmm1, 104(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbx)
	vmovhpd	%xmm0, 104(%rbp)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r10)
	vmovhpd	%xmm1, 112(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbx)
	vmovhpd	%xmm0, 112(%rbp)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r10)
	vmovhpd	%xmm1, 120(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbx)
	vmovhpd	%xmm0, 120(%rbp)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r10)
	vmovhpd	%xmm1, 128(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbx)
	vmovhpd	%xmm0, 128(%rbp)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r10)
	vmovhpd	%xmm1, 136(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbx)
	vmovhpd	%xmm0, 136(%rbp)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r10)
	vmovhpd	%xmm1, 144(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbx)
	vmovhpd	%xmm0, 144(%rbp)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r10)
	vmovhpd	%xmm1, 152(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbx)
	vmovhpd	%xmm0, 152(%rbp)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r10)
	vmovhpd	%xmm1, 160(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbx)
	vmovhpd	%xmm0, 160(%rbp)
	leaq	8320(%rsp), %rax
	leaq	352(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$107
L__crypto_kem_keypair_derand_jazz$108:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$111
	movw	%si, (%rax,%r9,2)
	incq	%r9
L__crypto_kem_keypair_derand_jazz$111:
	cmpq	$256, %r9
	jnb 	L__crypto_kem_keypair_derand_jazz$109
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_keypair_derand_jazz$109
	movw	%r11w, (%rax,%r9,2)
	incq	%r9
L__crypto_kem_keypair_derand_jazz$110:
L__crypto_kem_keypair_derand_jazz$109:
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_keypair_derand_jazz$107:
	jne 	L__crypto_kem_keypair_derand_jazz$108
	leaq	8832(%rsp), %rax
	leaq	864(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$102
L__crypto_kem_keypair_derand_jazz$103:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$106
	movw	%si, (%rax,%rdi,2)
	incq	%rdi
L__crypto_kem_keypair_derand_jazz$106:
	cmpq	$256, %rdi
	jnb 	L__crypto_kem_keypair_derand_jazz$104
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_keypair_derand_jazz$104
	movw	%r11w, (%rax,%rdi,2)
	incq	%rdi
L__crypto_kem_keypair_derand_jazz$105:
L__crypto_kem_keypair_derand_jazz$104:
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_keypair_derand_jazz$102:
	jne 	L__crypto_kem_keypair_derand_jazz$103
	leaq	9344(%rsp), %rax
	leaq	1376(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$97
L__crypto_kem_keypair_derand_jazz$98:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$101
	movw	%si, (%rax,%r8,2)
	incq	%r8
L__crypto_kem_keypair_derand_jazz$101:
	cmpq	$256, %r8
	jnb 	L__crypto_kem_keypair_derand_jazz$99
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_keypair_derand_jazz$99
	movw	%r11w, (%rax,%r8,2)
	incq	%r8
L__crypto_kem_keypair_derand_jazz$100:
L__crypto_kem_keypair_derand_jazz$99:
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_keypair_derand_jazz$97:
	jne 	L__crypto_kem_keypair_derand_jazz$98
	leaq	9856(%rsp), %rax
	leaq	1888(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$92
L__crypto_kem_keypair_derand_jazz$93:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$96
	movw	%si, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_keypair_derand_jazz$96:
	cmpq	$256, %r12
	jnb 	L__crypto_kem_keypair_derand_jazz$94
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_keypair_derand_jazz$94
	movw	%r11w, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_keypair_derand_jazz$95:
L__crypto_kem_keypair_derand_jazz$94:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_keypair_derand_jazz$92:
	jne 	L__crypto_kem_keypair_derand_jazz$93
	cmpq	$255, %r9
	setbe	%al
	cmpq	$255, %rdi
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r8
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
L__crypto_kem_keypair_derand_jazz$90:
	jne 	L__crypto_kem_keypair_derand_jazz$91
	vmovdqu	96(%rsp), %ymm0
	vmovdqu	%ymm0, 352(%rsp)
	vmovdqu	%ymm0, 864(%rsp)
	vmovdqu	%ymm0, 1376(%rsp)
	vmovdqu	%ymm0, 1888(%rsp)
	vmovdqu	%ymm0, 96(%rsp)
	movb	$1, 384(%rsp)
	movb	$1, 385(%rsp)
	movb	$2, 896(%rsp)
	movb	$1, 897(%rsp)
	movb	$0, 1408(%rsp)
	movb	$2, 1409(%rsp)
	movb	$1, 1920(%rsp)
	movb	$2, 1921(%rsp)
	leaq	2912(%rsp), %rax
	leaq	352(%rsp), %rcx
	leaq	864(%rsp), %rdx
	leaq	1376(%rsp), %rsi
	leaq	1888(%rsp), %rdi
	call	L_shake128_absorb4x_34$1
L__crypto_kem_keypair_derand_jazz$89:
	leaq	2912(%rsp), %rax
	leaq	352(%rsp), %rdi
	leaq	864(%rsp), %r8
	leaq	1376(%rsp), %r9
	leaq	1888(%rsp), %r10
	movq	%rdi, %r11
	movq	%r8, %rbx
	movq	%r9, %rbp
	movq	%r10, %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_keypair_derand_jazz$88:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	168(%rdi), %r11
	leaq	168(%r8), %rbx
	leaq	168(%r9), %rbp
	leaq	168(%r10), %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_keypair_derand_jazz$87:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r11)
	vmovhpd	%xmm1, (%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbp)
	vmovhpd	%xmm0, (%r12)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r11)
	vmovhpd	%xmm1, 8(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbp)
	vmovhpd	%xmm0, 8(%r12)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r11)
	vmovhpd	%xmm1, 16(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbp)
	vmovhpd	%xmm0, 16(%r12)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r11)
	vmovhpd	%xmm1, 24(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbp)
	vmovhpd	%xmm0, 24(%r12)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r11)
	vmovhpd	%xmm1, 32(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbp)
	vmovhpd	%xmm0, 32(%r12)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r11)
	vmovhpd	%xmm1, 40(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbp)
	vmovhpd	%xmm0, 40(%r12)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r11)
	vmovhpd	%xmm1, 48(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbp)
	vmovhpd	%xmm0, 48(%r12)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r11)
	vmovhpd	%xmm1, 56(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbp)
	vmovhpd	%xmm0, 56(%r12)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r11)
	vmovhpd	%xmm1, 64(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbp)
	vmovhpd	%xmm0, 64(%r12)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r11)
	vmovhpd	%xmm1, 72(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbp)
	vmovhpd	%xmm0, 72(%r12)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r11)
	vmovhpd	%xmm1, 80(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbp)
	vmovhpd	%xmm0, 80(%r12)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r11)
	vmovhpd	%xmm1, 88(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbp)
	vmovhpd	%xmm0, 88(%r12)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r11)
	vmovhpd	%xmm1, 96(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbp)
	vmovhpd	%xmm0, 96(%r12)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r11)
	vmovhpd	%xmm1, 104(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbp)
	vmovhpd	%xmm0, 104(%r12)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r11)
	vmovhpd	%xmm1, 112(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbp)
	vmovhpd	%xmm0, 112(%r12)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r11)
	vmovhpd	%xmm1, 120(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbp)
	vmovhpd	%xmm0, 120(%r12)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r11)
	vmovhpd	%xmm1, 128(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbp)
	vmovhpd	%xmm0, 128(%r12)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r11)
	vmovhpd	%xmm1, 136(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbp)
	vmovhpd	%xmm0, 136(%r12)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r11)
	vmovhpd	%xmm1, 144(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbp)
	vmovhpd	%xmm0, 144(%r12)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r11)
	vmovhpd	%xmm1, 152(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbp)
	vmovhpd	%xmm0, 152(%r12)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r11)
	vmovhpd	%xmm1, 160(%rbx)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbp)
	vmovhpd	%xmm0, 160(%r12)
	leaq	336(%rdi), %rdi
	leaq	336(%r8), %r8
	leaq	336(%r9), %r9
	leaq	336(%r10), %r10
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_keypair_derand_jazz$86:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%rdi)
	vmovhpd	%xmm1, (%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%r9)
	vmovhpd	%xmm0, (%r10)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%rdi)
	vmovhpd	%xmm1, 8(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%r9)
	vmovhpd	%xmm0, 8(%r10)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%rdi)
	vmovhpd	%xmm1, 16(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%r9)
	vmovhpd	%xmm0, 16(%r10)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%rdi)
	vmovhpd	%xmm1, 24(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%r9)
	vmovhpd	%xmm0, 24(%r10)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%rdi)
	vmovhpd	%xmm1, 32(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%r9)
	vmovhpd	%xmm0, 32(%r10)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%rdi)
	vmovhpd	%xmm1, 40(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%r9)
	vmovhpd	%xmm0, 40(%r10)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%rdi)
	vmovhpd	%xmm1, 48(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%r9)
	vmovhpd	%xmm0, 48(%r10)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%rdi)
	vmovhpd	%xmm1, 56(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%r9)
	vmovhpd	%xmm0, 56(%r10)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%rdi)
	vmovhpd	%xmm1, 64(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%r9)
	vmovhpd	%xmm0, 64(%r10)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%rdi)
	vmovhpd	%xmm1, 72(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%r9)
	vmovhpd	%xmm0, 72(%r10)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%rdi)
	vmovhpd	%xmm1, 80(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%r9)
	vmovhpd	%xmm0, 80(%r10)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%rdi)
	vmovhpd	%xmm1, 88(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%r9)
	vmovhpd	%xmm0, 88(%r10)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%rdi)
	vmovhpd	%xmm1, 96(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%r9)
	vmovhpd	%xmm0, 96(%r10)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%rdi)
	vmovhpd	%xmm1, 104(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%r9)
	vmovhpd	%xmm0, 104(%r10)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%rdi)
	vmovhpd	%xmm1, 112(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%r9)
	vmovhpd	%xmm0, 112(%r10)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%rdi)
	vmovhpd	%xmm1, 120(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%r9)
	vmovhpd	%xmm0, 120(%r10)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%rdi)
	vmovhpd	%xmm1, 128(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%r9)
	vmovhpd	%xmm0, 128(%r10)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%rdi)
	vmovhpd	%xmm1, 136(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%r9)
	vmovhpd	%xmm0, 136(%r10)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%rdi)
	vmovhpd	%xmm1, 144(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%r9)
	vmovhpd	%xmm0, 144(%r10)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%rdi)
	vmovhpd	%xmm1, 152(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%r9)
	vmovhpd	%xmm0, 152(%r10)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%rdi)
	vmovhpd	%xmm1, 160(%r8)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%r9)
	vmovhpd	%xmm0, 160(%r10)
	leaq	10368(%rsp), %r11
	leaq	352(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$85:
	movq	%r12, %rdi
	leaq	10880(%rsp), %r11
	leaq	864(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$84:
	movq	%r12, %r9
	leaq	11392(%rsp), %r11
	leaq	1376(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$83:
	movq	%r12, %r8
	leaq	11904(%rsp), %r11
	leaq	1888(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$82:
	cmpq	$255, %rdi
	setbe	%al
	cmpq	$255, %r9
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r8
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
	jmp 	L__crypto_kem_keypair_derand_jazz$59
L__crypto_kem_keypair_derand_jazz$60:
	leaq	2912(%rsp), %rax
	leaq	352(%rsp), %r10
	leaq	864(%rsp), %r11
	leaq	1376(%rsp), %rbx
	leaq	1888(%rsp), %rbp
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L__crypto_kem_keypair_derand_jazz$81:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%r10)
	vmovhpd	%xmm1, (%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%rbx)
	vmovhpd	%xmm0, (%rbp)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%r10)
	vmovhpd	%xmm1, 8(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%rbx)
	vmovhpd	%xmm0, 8(%rbp)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%r10)
	vmovhpd	%xmm1, 16(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%rbx)
	vmovhpd	%xmm0, 16(%rbp)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%r10)
	vmovhpd	%xmm1, 24(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%rbx)
	vmovhpd	%xmm0, 24(%rbp)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%r10)
	vmovhpd	%xmm1, 32(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%rbx)
	vmovhpd	%xmm0, 32(%rbp)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%r10)
	vmovhpd	%xmm1, 40(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%rbx)
	vmovhpd	%xmm0, 40(%rbp)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%r10)
	vmovhpd	%xmm1, 48(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%rbx)
	vmovhpd	%xmm0, 48(%rbp)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%r10)
	vmovhpd	%xmm1, 56(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%rbx)
	vmovhpd	%xmm0, 56(%rbp)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%r10)
	vmovhpd	%xmm1, 64(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%rbx)
	vmovhpd	%xmm0, 64(%rbp)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%r10)
	vmovhpd	%xmm1, 72(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%rbx)
	vmovhpd	%xmm0, 72(%rbp)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%r10)
	vmovhpd	%xmm1, 80(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%rbx)
	vmovhpd	%xmm0, 80(%rbp)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%r10)
	vmovhpd	%xmm1, 88(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%rbx)
	vmovhpd	%xmm0, 88(%rbp)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%r10)
	vmovhpd	%xmm1, 96(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%rbx)
	vmovhpd	%xmm0, 96(%rbp)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%r10)
	vmovhpd	%xmm1, 104(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%rbx)
	vmovhpd	%xmm0, 104(%rbp)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%r10)
	vmovhpd	%xmm1, 112(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%rbx)
	vmovhpd	%xmm0, 112(%rbp)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%r10)
	vmovhpd	%xmm1, 120(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%rbx)
	vmovhpd	%xmm0, 120(%rbp)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%r10)
	vmovhpd	%xmm1, 128(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%rbx)
	vmovhpd	%xmm0, 128(%rbp)
	vmovdqu	544(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 136(%r10)
	vmovhpd	%xmm1, 136(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 136(%rbx)
	vmovhpd	%xmm0, 136(%rbp)
	vmovdqu	576(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 144(%r10)
	vmovhpd	%xmm1, 144(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 144(%rbx)
	vmovhpd	%xmm0, 144(%rbp)
	vmovdqu	608(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 152(%r10)
	vmovhpd	%xmm1, 152(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 152(%rbx)
	vmovhpd	%xmm0, 152(%rbp)
	vmovdqu	640(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 160(%r10)
	vmovhpd	%xmm1, 160(%r11)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 160(%rbx)
	vmovhpd	%xmm0, 160(%rbp)
	leaq	10368(%rsp), %rax
	leaq	352(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$76
L__crypto_kem_keypair_derand_jazz$77:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$80
	movw	%si, (%rax,%rdi,2)
	incq	%rdi
L__crypto_kem_keypair_derand_jazz$80:
	cmpq	$256, %rdi
	jnb 	L__crypto_kem_keypair_derand_jazz$78
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_keypair_derand_jazz$78
	movw	%r11w, (%rax,%rdi,2)
	incq	%rdi
L__crypto_kem_keypair_derand_jazz$79:
L__crypto_kem_keypair_derand_jazz$78:
	cmpq	$255, %rdi
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_keypair_derand_jazz$76:
	jne 	L__crypto_kem_keypair_derand_jazz$77
	leaq	10880(%rsp), %rax
	leaq	864(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$71
L__crypto_kem_keypair_derand_jazz$72:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$75
	movw	%si, (%rax,%r9,2)
	incq	%r9
L__crypto_kem_keypair_derand_jazz$75:
	cmpq	$256, %r9
	jnb 	L__crypto_kem_keypair_derand_jazz$73
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_keypair_derand_jazz$73
	movw	%r11w, (%rax,%r9,2)
	incq	%r9
L__crypto_kem_keypair_derand_jazz$74:
L__crypto_kem_keypair_derand_jazz$73:
	cmpq	$255, %r9
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_keypair_derand_jazz$71:
	jne 	L__crypto_kem_keypair_derand_jazz$72
	leaq	11392(%rsp), %rax
	leaq	1376(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$66
L__crypto_kem_keypair_derand_jazz$67:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$70
	movw	%si, (%rax,%r8,2)
	incq	%r8
L__crypto_kem_keypair_derand_jazz$70:
	cmpq	$256, %r8
	jnb 	L__crypto_kem_keypair_derand_jazz$68
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_keypair_derand_jazz$68
	movw	%r11w, (%rax,%r8,2)
	incq	%r8
L__crypto_kem_keypair_derand_jazz$69:
L__crypto_kem_keypair_derand_jazz$68:
	cmpq	$255, %r8
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_keypair_derand_jazz$66:
	jne 	L__crypto_kem_keypair_derand_jazz$67
	leaq	11904(%rsp), %rax
	leaq	1888(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$61
L__crypto_kem_keypair_derand_jazz$62:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	movw	%r10w, %r11w
	shrw	$4, %r11w
	andw	$15, %r10w
	shlw	$8, %r10w
	orw 	%r10w, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %r10w
	shlw	$4, %r10w
	orw 	%r10w, %r11w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$65
	movw	%si, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_keypair_derand_jazz$65:
	cmpq	$256, %r12
	jnb 	L__crypto_kem_keypair_derand_jazz$63
	cmpw	$3329, %r11w
	jnb 	L__crypto_kem_keypair_derand_jazz$63
	movw	%r11w, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_keypair_derand_jazz$64:
L__crypto_kem_keypair_derand_jazz$63:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%r10b
	testb	%r10b, %sil
L__crypto_kem_keypair_derand_jazz$61:
	jne 	L__crypto_kem_keypair_derand_jazz$62
	cmpq	$255, %rdi
	setbe	%al
	cmpq	$255, %r9
	setbe	%cl
	orb 	%cl, %al
	cmpq	$255, %r8
	setbe	%cl
	cmpq	$255, %r12
	setbe	%dl
	orb 	%dl, %cl
	orb 	%al, %cl
L__crypto_kem_keypair_derand_jazz$59:
	jne 	L__crypto_kem_keypair_derand_jazz$60
	vmovdqu	96(%rsp), %ymm0
	vmovdqu	%ymm0, 352(%rsp)
	movb	$2, 384(%rsp)
	movb	$2, 385(%rsp)
	leaq	352(%rsp), %rax
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vmovdqu	%ymm0, 256(%rsp)
	vmovdqu	%ymm0, 288(%rsp)
	vmovdqu	%ymm0, 320(%rsp)
	vpbroadcastq	(%rax), %ymm5
	movq	8(%rax), %rcx
	movq	glob_data + 6208(%rip), %rdx
	movq	%rcx, 128(%rsp,%rdx,8)
	movq	16(%rax), %rcx
	movq	glob_data + 6216(%rip), %rdx
	movq	%rcx, 128(%rsp,%rdx,8)
	movq	24(%rax), %rcx
	movq	glob_data + 6224(%rip), %rdx
	movq	%rcx, 128(%rsp,%rdx,8)
	movb	32(%rax), %cl
	movq	glob_data + 6232(%rip), %rdx
	shlq	$3, %rdx
	movb	%cl, 128(%rsp,%rdx)
	movb	33(%rax), %al
	incq	%rdx
	movb	%al, 128(%rsp,%rdx)
	incq	%rdx
	movb	$31, 128(%rsp,%rdx)
	movq	glob_data + 6360(%rip), %rax
	shlq	$3, %rax
	movq	$167, %rcx
	andq	$7, %rcx
	addq	%rcx, %rax
	movb	$-128, 128(%rsp,%rax)
	vmovdqu	160(%rsp), %ymm0
	vmovdqu	192(%rsp), %ymm1
	vmovdqu	224(%rsp), %ymm2
	vmovdqu	256(%rsp), %ymm6
	vmovdqu	288(%rsp), %ymm3
	vmovdqu	320(%rsp), %ymm4
	leaq	352(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L__crypto_kem_keypair_derand_jazz$58:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	L__crypto_kem_keypair_derand_jazz$58
	vmovdqu	%ymm5, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm1, 192(%rsp)
	vmovdqu	%ymm2, 224(%rsp)
	vmovdqu	%ymm6, 256(%rsp)
	vmovdqu	%ymm3, 288(%rsp)
	vmovdqu	%ymm4, 320(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	520(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L__crypto_kem_keypair_derand_jazz$57:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	L__crypto_kem_keypair_derand_jazz$57
	vmovdqu	%ymm5, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm1, 192(%rsp)
	vmovdqu	%ymm2, 224(%rsp)
	vmovdqu	%ymm6, 256(%rsp)
	vmovdqu	%ymm3, 288(%rsp)
	vmovdqu	%ymm4, 320(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	688(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L__crypto_kem_keypair_derand_jazz$56:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm3, %ymm7
	vpxor	%ymm4, %ymm6, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm5, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm5, %ymm5
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%rsi), %ymm6, %ymm7
	vpsrlvq	96(%rdi), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm6
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm4, %ymm4
	vpermq	$-115, %ymm1, %ymm6
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm4, %ymm1
	vpsrlvq	160(%rdi), %ymm4, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm3, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm6, %ymm4
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm3, %ymm3
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm3, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm4
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm6, %ymm12
	vpxor	%ymm6, %ymm1, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm5, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm3, %ymm2
	vpermq	$-115, %ymm4, %ymm3
	vpermq	$114, %ymm12, %ymm4
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm0, %ymm6
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rcx,%rdx), %ymm5, %ymm5
	addq	$32, %rdx
	decq	%r8
	jne 	L__crypto_kem_keypair_derand_jazz$56
	vmovdqu	%ymm5, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm1, 192(%rsp)
	vmovdqu	%ymm2, 224(%rsp)
	vmovdqu	%ymm6, 256(%rsp)
	vmovdqu	%ymm3, 288(%rsp)
	vmovdqu	%ymm4, 320(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	vmovdqu	%ymm5, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm1, 192(%rsp)
	vmovdqu	%ymm2, 224(%rsp)
	vmovdqu	%ymm6, 256(%rsp)
	vmovdqu	%ymm3, 288(%rsp)
	vmovdqu	%ymm4, 320(%rsp)
	leaq	12416(%rsp), %r11
	leaq	352(%rsp), %rbx
	call	L_rej_uniform_avx$1
L__crypto_kem_keypair_derand_jazz$55:
	cmpq	$255, %r12
	setbe	%al
	vmovdqu	128(%rsp), %ymm6
	vmovdqu	160(%rsp), %ymm0
	vmovdqu	192(%rsp), %ymm1
	vmovdqu	224(%rsp), %ymm2
	vmovdqu	256(%rsp), %ymm3
	vmovdqu	288(%rsp), %ymm4
	vmovdqu	320(%rsp), %ymm5
	jmp 	L__crypto_kem_keypair_derand_jazz$47
L__crypto_kem_keypair_derand_jazz$48:
	leaq	352(%rsp), %rax
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L__crypto_kem_keypair_derand_jazz$54:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm4, %ymm7
	vpxor	%ymm5, %ymm3, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm6, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm6, %ymm6
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	96(%rsi), %ymm3, %ymm7
	vpsrlvq	96(%rdi), %ymm3, %ymm3
	vpor	%ymm7, %ymm3, %ymm10
	vpxor	%ymm9, %ymm4, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm4
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm4, %ymm3, %ymm4
	vpxor	%ymm9, %ymm5, %ymm5
	vpermq	$-115, %ymm1, %ymm3
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm5, %ymm1
	vpsrlvq	160(%rdi), %ymm5, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm4, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm4
	vpblendd	$12, %ymm7, %ymm3, %ymm5
	vpblendd	$12, %ymm3, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm4, %ymm4
	vpblendd	$48, %ymm10, %ymm5, %ymm5
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm9, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm4, %ymm1, %ymm1
	vpandn	%ymm11, %ymm5, %ymm5
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm3, %ymm12
	vpxor	%ymm3, %ymm1, %ymm4
	vpblendd	$48, %ymm3, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm6, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm6, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm3, %ymm13, %ymm13
	vpblendd	$-64, %ymm3, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm4, %ymm2
	vpermq	$-115, %ymm5, %ymm4
	vpermq	$114, %ymm12, %ymm5
	vpblendd	$12, %ymm3, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm3, %ymm9, %ymm3
	vpandn	%ymm3, %ymm0, %ymm3
	vpxor	%ymm13, %ymm6, %ymm6
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm3, %ymm3
	vpxor	(%rcx,%rdx), %ymm6, %ymm6
	addq	$32, %rdx
	decq	%r8
	jne 	L__crypto_kem_keypair_derand_jazz$54
	vmovdqu	%ymm6, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm1, 192(%rsp)
	vmovdqu	%ymm2, 224(%rsp)
	vmovdqu	%ymm3, 256(%rsp)
	vmovdqu	%ymm4, 288(%rsp)
	vmovdqu	%ymm5, 320(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	movq	glob_data + 6264(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 64(%rax)
	movq	glob_data + 6272(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 72(%rax)
	movq	glob_data + 6280(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 80(%rax)
	movq	glob_data + 6288(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 88(%rax)
	movq	glob_data + 6296(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 96(%rax)
	movq	glob_data + 6304(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 104(%rax)
	movq	glob_data + 6312(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 112(%rax)
	movq	glob_data + 6320(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 120(%rax)
	movq	glob_data + 6328(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 128(%rax)
	movq	glob_data + 6336(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 136(%rax)
	movq	glob_data + 6344(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 144(%rax)
	movq	glob_data + 6352(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 152(%rax)
	movq	glob_data + 6360(%rip), %rcx
	movq	128(%rsp,%rcx,8), %rcx
	movq	%rcx, 160(%rax)
	leaq	12416(%rsp), %rax
	leaq	352(%rsp), %rcx
	movq	$0, %rdx
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%dil
	testb	%dil, %sil
	jmp 	L__crypto_kem_keypair_derand_jazz$49
L__crypto_kem_keypair_derand_jazz$50:
	movzbw	(%rcx,%rdx), %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %di
	movw	%di, %r8w
	shrw	$4, %r8w
	andw	$15, %di
	shlw	$8, %di
	orw 	%di, %si
	incq	%rdx
	movzbw	(%rcx,%rdx), %di
	shlw	$4, %di
	orw 	%di, %r8w
	incq	%rdx
	cmpw	$3329, %si
	jnb 	L__crypto_kem_keypair_derand_jazz$53
	movw	%si, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_keypair_derand_jazz$53:
	cmpq	$256, %r12
	jnb 	L__crypto_kem_keypair_derand_jazz$51
	cmpw	$3329, %r8w
	jnb 	L__crypto_kem_keypair_derand_jazz$51
	movw	%r8w, (%rax,%r12,2)
	incq	%r12
L__crypto_kem_keypair_derand_jazz$52:
L__crypto_kem_keypair_derand_jazz$51:
	cmpq	$255, %r12
	setbe	%sil
	cmpq	$165, %rdx
	setbe	%dil
	testb	%dil, %sil
L__crypto_kem_keypair_derand_jazz$49:
	jne 	L__crypto_kem_keypair_derand_jazz$50
	cmpq	$255, %r12
	setbe	%al
L__crypto_kem_keypair_derand_jazz$47:
	cmpb	$0, %al
	jne 	L__crypto_kem_keypair_derand_jazz$48
	leaq	8320(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$46:
	leaq	8832(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$45:
	leaq	9344(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$44:
	leaq	9856(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$43:
	leaq	10368(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$42:
	leaq	10880(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$41:
	leaq	11392(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$40:
	leaq	11904(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$39:
	leaq	12416(%rsp), %rax
	call	L_nttunpack$1
L__crypto_kem_keypair_derand_jazz$38:
	movb	$0, %cl
	leaq	5248(%rsp), %rax
	leaq	5760(%rsp), %rdi
	leaq	6272(%rsp), %r8
	leaq	3712(%rsp), %r9
	leaq	32(%rsp), %r10
	leaq	-1496(%rsp), %rsp
	call	L_poly_getnoise_eta1_4x$1
L__crypto_kem_keypair_derand_jazz$37:
	leaq	1496(%rsp), %rsp
	movb	$4, %cl
	leaq	4224(%rsp), %rax
	leaq	4736(%rsp), %rdi
	leaq	6784(%rsp), %r8
	leaq	7296(%rsp), %r9
	leaq	32(%rsp), %r10
	leaq	-1496(%rsp), %rsp
	call	L_poly_getnoise_eta1_4x$1
L__crypto_kem_keypair_derand_jazz$36:
	leaq	1496(%rsp), %rsp
	leaq	5248(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$35:
	leaq	5760(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$34:
	leaq	6272(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$33:
	leaq	3712(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$32:
	leaq	4224(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$31:
	leaq	4736(%rsp), %rcx
	call	L_poly_ntt$1
L__crypto_kem_keypair_derand_jazz$30:
	leaq	6784(%rsp), %rcx
	leaq	8320(%rsp), %rsi
	leaq	5248(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$29:
	leaq	2400(%rsp), %rcx
	leaq	8832(%rsp), %rsi
	leaq	5760(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$28:
	leaq	6784(%rsp), %rcx
	leaq	2400(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$27:
	leaq	2400(%rsp), %rcx
	leaq	9344(%rsp), %rsi
	leaq	6272(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$26:
	leaq	6784(%rsp), %rcx
	leaq	2400(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$25:
	leaq	6784(%rsp), %rax
	call	L_poly_frommont$1
L__crypto_kem_keypair_derand_jazz$24:
	leaq	7296(%rsp), %rcx
	leaq	9856(%rsp), %rsi
	leaq	5248(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$23:
	leaq	2400(%rsp), %rcx
	leaq	10368(%rsp), %rsi
	leaq	5760(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$22:
	leaq	7296(%rsp), %rcx
	leaq	2400(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$21:
	leaq	2400(%rsp), %rcx
	leaq	10880(%rsp), %rsi
	leaq	6272(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$20:
	leaq	7296(%rsp), %rcx
	leaq	2400(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$19:
	leaq	7296(%rsp), %rax
	call	L_poly_frommont$1
L__crypto_kem_keypair_derand_jazz$18:
	leaq	7808(%rsp), %rcx
	leaq	11392(%rsp), %rsi
	leaq	5248(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$17:
	leaq	2400(%rsp), %rcx
	leaq	11904(%rsp), %rsi
	leaq	5760(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$16:
	leaq	7808(%rsp), %rcx
	leaq	2400(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$15:
	leaq	2400(%rsp), %rcx
	leaq	12416(%rsp), %rsi
	leaq	6272(%rsp), %rdi
	call	L_poly_basemul$1
L__crypto_kem_keypair_derand_jazz$14:
	leaq	7808(%rsp), %rcx
	leaq	2400(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$13:
	leaq	7808(%rsp), %rax
	call	L_poly_frommont$1
L__crypto_kem_keypair_derand_jazz$12:
	leaq	6784(%rsp), %rcx
	leaq	3712(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$11:
	leaq	7296(%rsp), %rcx
	leaq	4224(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$10:
	leaq	7808(%rsp), %rcx
	leaq	4736(%rsp), %rsi
	call	L_poly_add2$1
L__crypto_kem_keypair_derand_jazz$9:
	leaq	6784(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	leaq	7296(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	leaq	7808(%rsp), %rax
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, (%rax)
	vmovdqu	32(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vmovdqu	64(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vmovdqu	96(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vmovdqu	128(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	160(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 160(%rax)
	vmovdqu	192(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 192(%rax)
	vmovdqu	224(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 256(%rax)
	vmovdqu	288(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	320(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 320(%rax)
	vmovdqu	352(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 352(%rax)
	vmovdqu	384(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 384(%rax)
	vmovdqu	416(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 416(%rax)
	vmovdqu	448(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	480(%rax), %ymm2
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	movq	12952(%rsp), %rax
	movq	12960(%rsp), %rcx
	leaq	5248(%rsp), %rdx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$8:
	addq	$384, %rcx
	leaq	5760(%rsp), %rdx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$7:
	addq	$384, %rcx
	leaq	6272(%rsp), %rdx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$6:
	movq	%rax, %rcx
	leaq	6784(%rsp), %rdx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$5:
	addq	$384, %rcx
	leaq	7296(%rsp), %rdx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$4:
	addq	$384, %rcx
	leaq	7808(%rsp), %rdx
	call	L_poly_tobytes$1
L__crypto_kem_keypair_derand_jazz$3:
	addq	$1152, %rax
	movq	64(%rsp), %rcx
	movq	%rcx, (%rax)
	addq	$8, %rax
	movq	72(%rsp), %rcx
	movq	%rcx, (%rax)
	addq	$8, %rax
	movq	80(%rsp), %rcx
	movq	%rcx, (%rax)
	addq	$8, %rax
	movq	88(%rsp), %rcx
	movq	%rcx, (%rax)
	movq	12944(%rsp), %rax
	addq	$1152, %rax
	movq	12936(%rsp), %rcx
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
	movq	792(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	800(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	808(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	816(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	824(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	832(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	840(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	848(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	856(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	864(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	872(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	880(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	888(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	896(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	904(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	912(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	920(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	928(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	936(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	944(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	952(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	960(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	968(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	976(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	984(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	992(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1000(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1008(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1016(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1024(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1032(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1040(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1048(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1056(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1064(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1072(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1080(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1088(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1096(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1104(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1112(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1120(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1128(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1136(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1144(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1152(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1160(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1168(%rcx), %rdx
	movq	%rdx, (%rax)
	addq	$8, %rax
	movq	1176(%rcx), %rcx
	movq	%rcx, (%rax)
	addq	$8, %rax
	movq	%rax, 12944(%rsp)
	movq	12936(%rsp), %r8
	movq	$1184, %rdi
	leaq	64(%rsp), %rax
	leaq	-248(%rsp), %rsp
	call	L_sha3_256$1
L__crypto_kem_keypair_derand_jazz$2:
	leaq	248(%rsp), %rsp
	movq	12944(%rsp), %rax
	movq	12928(%rsp), %rcx
	leaq	64(%rsp), %rdx
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
L_rej_uniform_avx$1:
	leaq	glob_data + 4128(%rip), %r10
	vmovdqu	glob_data + 448(%rip), %ymm0
	movq	$0, %r12
	movq	$0, %rbp
	vpbroadcastb	glob_data + 6430(%rip), %ymm1
	vpbroadcastw	glob_data + 6416(%rip), %ymm2
	vmovdqu	glob_data + 0(%rip), %ymm3
	cmpq	$224, %r12
	setbe	%al
	cmpq	$448, %rbp
	setbe	%cl
	testb	%cl, %al
	jmp 	L_rej_uniform_avx$9
L_rej_uniform_avx$10:
	vpermq	$-108, (%rbx,%rbp), %ymm4
	vpermq	$-108, 24(%rbx,%rbp), %ymm5
	vpshufb	%ymm3, %ymm4, %ymm4
	vpshufb	%ymm3, %ymm5, %ymm5
	vpsrlw	$4, %ymm4, %ymm6
	vpsrlw	$4, %ymm5, %ymm7
	vpblendw	$-86, %ymm6, %ymm4, %ymm4
	vpblendw	$-86, %ymm7, %ymm5, %ymm5
	vpand	%ymm2, %ymm4, %ymm4
	vpand	%ymm2, %ymm5, %ymm5
	vpcmpgtw	%ymm4, %ymm0, %ymm6
	vpcmpgtw	%ymm5, %ymm0, %ymm7
	vpacksswb	%ymm7, %ymm6, %ymm6
	vpmovmskb	%ymm6, %rax
	movq	%rax, %rcx
	andq	$255, %rcx
	vmovq	(%r10,%rcx,8), %xmm6
	movq	%rax, %rdx
	shrq	$16, %rdx
	andq	$255, %rdx
	vmovq	(%r10,%rdx,8), %xmm7
	movq	%rax, %rsi
	shrq	$8, %rsi
	andq	$255, %rsi
	vmovq	(%r10,%rsi,8), %xmm8
	shrq	$24, %rax
	andq	$255, %rax
	vmovq	(%r10,%rax,8), %xmm9
	vinserti128	$1, %xmm7, %ymm6, %ymm6
	popcnt	%rcx, %rcx
	popcnt	%rdx, %rdx
	addq	%r12, %rcx
	vinserti128	$1, %xmm9, %ymm8, %ymm7
	addq	%rcx, %rdx
	popcnt	%rsi, %rsi
	addq	%rdx, %rsi
	popcnt	%rax, %rax
	addq	%rsi, %rax
	vpaddb	%ymm1, %ymm6, %ymm8
	vpunpcklbw	%ymm8, %ymm6, %ymm6
	vpaddb	%ymm1, %ymm7, %ymm8
	vpunpcklbw	%ymm8, %ymm7, %ymm7
	vpshufb	%ymm6, %ymm4, %ymm4
	vpshufb	%ymm7, %ymm5, %ymm5
	vmovdqu	%xmm4, (%r11,%r12,2)
	vextracti128	$1, %ymm4, (%r11,%rcx,2)
	vmovdqu	%xmm5, (%r11,%rdx,2)
	vextracti128	$1, %ymm5, (%r11,%rsi,2)
	movq	%rax, %r12
	cmpq	$224, %r12
	setbe	%al
	addq	$48, %rbp
	cmpq	$448, %rbp
	setbe	%cl
	testb	%cl, %al
L_rej_uniform_avx$9:
	jne 	L_rej_uniform_avx$10
	cmpq	$248, %r12
	setbe	%al
	cmpq	$488, %rbp
	setbe	%cl
	testb	%cl, %al
	movq	$21845, %rax
	jmp 	L_rej_uniform_avx$7
L_rej_uniform_avx$8:
	vmovdqu	(%rbx,%rbp), %xmm4
	vpshufb	%xmm3, %xmm4, %xmm4
	vpsrlw	$4, %xmm4, %xmm5
	vpblendw	$-86, %xmm5, %xmm4, %xmm4
	vpand	%xmm2, %xmm4, %xmm4
	vpcmpgtw	%xmm4, %xmm0, %xmm5
	vpmovmskb	%xmm5, %rcx
	pext	%rax, %rcx, %rcx
	vmovq	(%r10,%rcx,8), %xmm5
	popcnt	%rcx, %rcx
	vpaddb	%xmm1, %xmm5, %xmm6
	vpunpcklbw	%xmm6, %xmm5, %xmm5
	vpshufb	%xmm5, %xmm4, %xmm4
	vmovdqu	%xmm4, (%r11,%r12,2)
	addq	%rcx, %r12
	addq	$12, %rbp
	cmpq	$248, %r12
	setbe	%cl
	cmpq	$488, %rbp
	setbe	%dl
	testb	%dl, %cl
L_rej_uniform_avx$7:
	jne 	L_rej_uniform_avx$8
	cmpq	$255, %r12
	setbe	%al
	cmpq	$501, %rbp
	setbe	%cl
	testb	%cl, %al
	jmp 	L_rej_uniform_avx$2
L_rej_uniform_avx$3:
	movzbw	(%rbx,%rbp), %ax
	incq	%rbp
	movzbw	(%rbx,%rbp), %dx
	incq	%rbp
	movw	%dx, %cx
	shlw	$8, %dx
	orw 	%dx, %ax
	andw	$4095, %ax
	shrw	$4, %cx
	movzbw	(%rbx,%rbp), %dx
	incq	%rbp
	shlw	$4, %dx
	orw 	%dx, %cx
	cmpw	$3329, %ax
	jnb 	L_rej_uniform_avx$6
	movw	%ax, (%r11,%r12,2)
	incq	%r12
L_rej_uniform_avx$6:
	cmpw	$3329, %cx
	jnb 	L_rej_uniform_avx$4
	cmpq	$256, %r12
	jnb 	L_rej_uniform_avx$4
	movw	%cx, (%r11,%r12,2)
	incq	%r12
L_rej_uniform_avx$5:
L_rej_uniform_avx$4:
	cmpq	$255, %r12
	setbe	%al
	cmpq	$501, %rbp
	setbe	%cl
	testb	%cl, %al
L_rej_uniform_avx$2:
	jne 	L_rej_uniform_avx$3
	ret
L_poly_tomsg_1$1:
	call	L_poly_csubq$1
L_poly_tomsg_1$2:
	leaq	glob_data + 224(%rip), %rsi
	vmovdqu	(%rsi), %ymm0
	leaq	glob_data + 192(%rip), %rsi
	vmovdqu	(%rsi), %ymm1
	vmovdqu	(%rcx), %ymm2
	vmovdqu	32(%rcx), %ymm3
	vpsubw	%ymm2, %ymm0, %ymm2
	vpsubw	%ymm3, %ymm0, %ymm3
	vpsraw	$15, %ymm2, %ymm4
	vpsraw	$15, %ymm3, %ymm5
	vpxor	%ymm4, %ymm2, %ymm2
	vpxor	%ymm5, %ymm3, %ymm3
	vpsubw	%ymm1, %ymm2, %ymm2
	vpsubw	%ymm1, %ymm3, %ymm3
	vpacksswb	%ymm3, %ymm2, %ymm2
	vpermq	$-40, %ymm2, %ymm2
	vpmovmskb	%ymm2, %esi
	movl	%esi, (%rax)
	vmovdqu	64(%rcx), %ymm2
	vmovdqu	96(%rcx), %ymm3
	vpsubw	%ymm2, %ymm0, %ymm2
	vpsubw	%ymm3, %ymm0, %ymm3
	vpsraw	$15, %ymm2, %ymm4
	vpsraw	$15, %ymm3, %ymm5
	vpxor	%ymm4, %ymm2, %ymm2
	vpxor	%ymm5, %ymm3, %ymm3
	vpsubw	%ymm1, %ymm2, %ymm2
	vpsubw	%ymm1, %ymm3, %ymm3
	vpacksswb	%ymm3, %ymm2, %ymm2
	vpermq	$-40, %ymm2, %ymm2
	vpmovmskb	%ymm2, %esi
	movl	%esi, 4(%rax)
	vmovdqu	128(%rcx), %ymm2
	vmovdqu	160(%rcx), %ymm3
	vpsubw	%ymm2, %ymm0, %ymm2
	vpsubw	%ymm3, %ymm0, %ymm3
	vpsraw	$15, %ymm2, %ymm4
	vpsraw	$15, %ymm3, %ymm5
	vpxor	%ymm4, %ymm2, %ymm2
	vpxor	%ymm5, %ymm3, %ymm3
	vpsubw	%ymm1, %ymm2, %ymm2
	vpsubw	%ymm1, %ymm3, %ymm3
	vpacksswb	%ymm3, %ymm2, %ymm2
	vpermq	$-40, %ymm2, %ymm2
	vpmovmskb	%ymm2, %esi
	movl	%esi, 8(%rax)
	vmovdqu	192(%rcx), %ymm2
	vmovdqu	224(%rcx), %ymm3
	vpsubw	%ymm2, %ymm0, %ymm2
	vpsubw	%ymm3, %ymm0, %ymm3
	vpsraw	$15, %ymm2, %ymm4
	vpsraw	$15, %ymm3, %ymm5
	vpxor	%ymm4, %ymm2, %ymm2
	vpxor	%ymm5, %ymm3, %ymm3
	vpsubw	%ymm1, %ymm2, %ymm2
	vpsubw	%ymm1, %ymm3, %ymm3
	vpacksswb	%ymm3, %ymm2, %ymm2
	vpermq	$-40, %ymm2, %ymm2
	vpmovmskb	%ymm2, %esi
	movl	%esi, 12(%rax)
	vmovdqu	256(%rcx), %ymm2
	vmovdqu	288(%rcx), %ymm3
	vpsubw	%ymm2, %ymm0, %ymm2
	vpsubw	%ymm3, %ymm0, %ymm3
	vpsraw	$15, %ymm2, %ymm4
	vpsraw	$15, %ymm3, %ymm5
	vpxor	%ymm4, %ymm2, %ymm2
	vpxor	%ymm5, %ymm3, %ymm3
	vpsubw	%ymm1, %ymm2, %ymm2
	vpsubw	%ymm1, %ymm3, %ymm3
	vpacksswb	%ymm3, %ymm2, %ymm2
	vpermq	$-40, %ymm2, %ymm2
	vpmovmskb	%ymm2, %esi
	movl	%esi, 16(%rax)
	vmovdqu	320(%rcx), %ymm2
	vmovdqu	352(%rcx), %ymm3
	vpsubw	%ymm2, %ymm0, %ymm2
	vpsubw	%ymm3, %ymm0, %ymm3
	vpsraw	$15, %ymm2, %ymm4
	vpsraw	$15, %ymm3, %ymm5
	vpxor	%ymm4, %ymm2, %ymm2
	vpxor	%ymm5, %ymm3, %ymm3
	vpsubw	%ymm1, %ymm2, %ymm2
	vpsubw	%ymm1, %ymm3, %ymm3
	vpacksswb	%ymm3, %ymm2, %ymm2
	vpermq	$-40, %ymm2, %ymm2
	vpmovmskb	%ymm2, %esi
	movl	%esi, 20(%rax)
	vmovdqu	384(%rcx), %ymm2
	vmovdqu	416(%rcx), %ymm3
	vpsubw	%ymm2, %ymm0, %ymm2
	vpsubw	%ymm3, %ymm0, %ymm3
	vpsraw	$15, %ymm2, %ymm4
	vpsraw	$15, %ymm3, %ymm5
	vpxor	%ymm4, %ymm2, %ymm2
	vpxor	%ymm5, %ymm3, %ymm3
	vpsubw	%ymm1, %ymm2, %ymm2
	vpsubw	%ymm1, %ymm3, %ymm3
	vpacksswb	%ymm3, %ymm2, %ymm2
	vpermq	$-40, %ymm2, %ymm2
	vpmovmskb	%ymm2, %esi
	movl	%esi, 24(%rax)
	vmovdqu	448(%rcx), %ymm2
	vmovdqu	480(%rcx), %ymm3
	vpsubw	%ymm2, %ymm0, %ymm2
	vpsubw	%ymm3, %ymm0, %ymm0
	vpsraw	$15, %ymm2, %ymm3
	vpsraw	$15, %ymm0, %ymm4
	vpxor	%ymm3, %ymm2, %ymm2
	vpxor	%ymm4, %ymm0, %ymm0
	vpsubw	%ymm1, %ymm2, %ymm2
	vpsubw	%ymm1, %ymm0, %ymm0
	vpacksswb	%ymm0, %ymm2, %ymm0
	vpermq	$-40, %ymm0, %ymm0
	vpmovmskb	%ymm0, %ecx
	movl	%ecx, 28(%rax)
	ret
L_poly_tobytes$1:
	leaq	glob_data + 448(%rip), %rsi
	vmovdqu	(%rsi), %ymm0
	vmovdqu	(%rdx), %ymm7
	vmovdqu	32(%rdx), %ymm8
	vmovdqu	64(%rdx), %ymm1
	vmovdqu	96(%rdx), %ymm2
	vmovdqu	128(%rdx), %ymm3
	vmovdqu	160(%rdx), %ymm4
	vmovdqu	192(%rdx), %ymm5
	vmovdqu	224(%rdx), %ymm6
	vpsubw	%ymm0, %ymm7, %ymm7
	vpsraw	$15, %ymm7, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm7, %ymm9, %ymm7
	vpsubw	%ymm0, %ymm8, %ymm8
	vpsraw	$15, %ymm8, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm8, %ymm9, %ymm8
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm1, %ymm9, %ymm1
	vpsubw	%ymm0, %ymm2, %ymm2
	vpsraw	$15, %ymm2, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm2, %ymm9, %ymm2
	vpsubw	%ymm0, %ymm3, %ymm3
	vpsraw	$15, %ymm3, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm3, %ymm9, %ymm3
	vpsubw	%ymm0, %ymm4, %ymm4
	vpsraw	$15, %ymm4, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm4, %ymm9, %ymm4
	vpsubw	%ymm0, %ymm5, %ymm5
	vpsraw	$15, %ymm5, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm5, %ymm9, %ymm5
	vpsubw	%ymm0, %ymm6, %ymm6
	vpsraw	$15, %ymm6, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm6, %ymm9, %ymm6
	vpsllw	$12, %ymm8, %ymm9
	vpor	%ymm7, %ymm9, %ymm7
	vpsrlw	$4, %ymm8, %ymm8
	vpsllw	$8, %ymm1, %ymm9
	vpor	%ymm9, %ymm8, %ymm8
	vpsrlw	$8, %ymm1, %ymm1
	vpsllw	$4, %ymm2, %ymm2
	vpor	%ymm2, %ymm1, %ymm1
	vpsllw	$12, %ymm4, %ymm2
	vpor	%ymm3, %ymm2, %ymm2
	vpsrlw	$4, %ymm4, %ymm3
	vpsllw	$8, %ymm5, %ymm4
	vpor	%ymm4, %ymm3, %ymm3
	vpsrlw	$8, %ymm5, %ymm4
	vpsllw	$4, %ymm6, %ymm5
	vpor	%ymm5, %ymm4, %ymm4
	vpslld	$16, %ymm8, %ymm5
	vpblendw	$-86, %ymm5, %ymm7, %ymm5
	vpsrld	$16, %ymm7, %ymm6
	vpblendw	$-86, %ymm8, %ymm6, %ymm6
	vpslld	$16, %ymm2, %ymm7
	vpblendw	$-86, %ymm7, %ymm1, %ymm7
	vpsrld	$16, %ymm1, %ymm1
	vpblendw	$-86, %ymm2, %ymm1, %ymm1
	vpslld	$16, %ymm4, %ymm2
	vpblendw	$-86, %ymm2, %ymm3, %ymm8
	vpsrld	$16, %ymm3, %ymm2
	vpblendw	$-86, %ymm4, %ymm2, %ymm2
	vmovsldup	%ymm7, %ymm3
	vpblendd	$-86, %ymm3, %ymm5, %ymm3
	vpsrlq	$32, %ymm5, %ymm4
	vpblendd	$-86, %ymm7, %ymm4, %ymm4
	vmovsldup	%ymm6, %ymm5
	vpblendd	$-86, %ymm5, %ymm8, %ymm5
	vpsrlq	$32, %ymm8, %ymm7
	vpblendd	$-86, %ymm6, %ymm7, %ymm6
	vmovsldup	%ymm2, %ymm7
	vpblendd	$-86, %ymm7, %ymm1, %ymm7
	vpsrlq	$32, %ymm1, %ymm1
	vpblendd	$-86, %ymm2, %ymm1, %ymm1
	vpunpcklqdq	%ymm5, %ymm3, %ymm2
	vpunpckhqdq	%ymm5, %ymm3, %ymm3
	vpunpcklqdq	%ymm4, %ymm7, %ymm5
	vpunpckhqdq	%ymm4, %ymm7, %ymm4
	vpunpcklqdq	%ymm1, %ymm6, %ymm7
	vpunpckhqdq	%ymm1, %ymm6, %ymm1
	vperm2i128	$32, %ymm5, %ymm2, %ymm6
	vperm2i128	$49, %ymm5, %ymm2, %ymm2
	vperm2i128	$32, %ymm3, %ymm7, %ymm5
	vperm2i128	$49, %ymm3, %ymm7, %ymm3
	vperm2i128	$32, %ymm1, %ymm4, %ymm7
	vperm2i128	$49, %ymm1, %ymm4, %ymm1
	vmovdqu	%ymm6, (%rcx)
	vmovdqu	%ymm5, 32(%rcx)
	vmovdqu	%ymm7, 64(%rcx)
	vmovdqu	%ymm2, 96(%rcx)
	vmovdqu	%ymm3, 128(%rcx)
	vmovdqu	%ymm1, 160(%rcx)
	vmovdqu	256(%rdx), %ymm7
	vmovdqu	288(%rdx), %ymm8
	vmovdqu	320(%rdx), %ymm1
	vmovdqu	352(%rdx), %ymm2
	vmovdqu	384(%rdx), %ymm3
	vmovdqu	416(%rdx), %ymm4
	vmovdqu	448(%rdx), %ymm5
	vmovdqu	480(%rdx), %ymm6
	vpsubw	%ymm0, %ymm7, %ymm7
	vpsraw	$15, %ymm7, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm7, %ymm9, %ymm7
	vpsubw	%ymm0, %ymm8, %ymm8
	vpsraw	$15, %ymm8, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm8, %ymm9, %ymm8
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm1, %ymm9, %ymm1
	vpsubw	%ymm0, %ymm2, %ymm2
	vpsraw	$15, %ymm2, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm2, %ymm9, %ymm2
	vpsubw	%ymm0, %ymm3, %ymm3
	vpsraw	$15, %ymm3, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm3, %ymm9, %ymm3
	vpsubw	%ymm0, %ymm4, %ymm4
	vpsraw	$15, %ymm4, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm4, %ymm9, %ymm4
	vpsubw	%ymm0, %ymm5, %ymm5
	vpsraw	$15, %ymm5, %ymm9
	vpand	%ymm0, %ymm9, %ymm9
	vpaddw	%ymm5, %ymm9, %ymm5
	vpsubw	%ymm0, %ymm6, %ymm6
	vpsraw	$15, %ymm6, %ymm9
	vpand	%ymm0, %ymm9, %ymm0
	vpaddw	%ymm6, %ymm0, %ymm0
	vpsllw	$12, %ymm8, %ymm6
	vpor	%ymm7, %ymm6, %ymm6
	vpsrlw	$4, %ymm8, %ymm7
	vpsllw	$8, %ymm1, %ymm8
	vpor	%ymm8, %ymm7, %ymm7
	vpsrlw	$8, %ymm1, %ymm1
	vpsllw	$4, %ymm2, %ymm2
	vpor	%ymm2, %ymm1, %ymm1
	vpsllw	$12, %ymm4, %ymm2
	vpor	%ymm3, %ymm2, %ymm2
	vpsrlw	$4, %ymm4, %ymm3
	vpsllw	$8, %ymm5, %ymm4
	vpor	%ymm4, %ymm3, %ymm3
	vpsrlw	$8, %ymm5, %ymm4
	vpsllw	$4, %ymm0, %ymm0
	vpor	%ymm0, %ymm4, %ymm0
	vpslld	$16, %ymm7, %ymm4
	vpblendw	$-86, %ymm4, %ymm6, %ymm4
	vpsrld	$16, %ymm6, %ymm5
	vpblendw	$-86, %ymm7, %ymm5, %ymm5
	vpslld	$16, %ymm2, %ymm6
	vpblendw	$-86, %ymm6, %ymm1, %ymm6
	vpsrld	$16, %ymm1, %ymm1
	vpblendw	$-86, %ymm2, %ymm1, %ymm1
	vpslld	$16, %ymm0, %ymm2
	vpblendw	$-86, %ymm2, %ymm3, %ymm7
	vpsrld	$16, %ymm3, %ymm2
	vpblendw	$-86, %ymm0, %ymm2, %ymm0
	vmovsldup	%ymm6, %ymm2
	vpblendd	$-86, %ymm2, %ymm4, %ymm2
	vpsrlq	$32, %ymm4, %ymm3
	vpblendd	$-86, %ymm6, %ymm3, %ymm3
	vmovsldup	%ymm5, %ymm4
	vpblendd	$-86, %ymm4, %ymm7, %ymm4
	vpsrlq	$32, %ymm7, %ymm6
	vpblendd	$-86, %ymm5, %ymm6, %ymm5
	vmovsldup	%ymm0, %ymm6
	vpblendd	$-86, %ymm6, %ymm1, %ymm6
	vpsrlq	$32, %ymm1, %ymm1
	vpblendd	$-86, %ymm0, %ymm1, %ymm0
	vpunpcklqdq	%ymm4, %ymm2, %ymm1
	vpunpckhqdq	%ymm4, %ymm2, %ymm2
	vpunpcklqdq	%ymm3, %ymm6, %ymm4
	vpunpckhqdq	%ymm3, %ymm6, %ymm3
	vpunpcklqdq	%ymm0, %ymm5, %ymm6
	vpunpckhqdq	%ymm0, %ymm5, %ymm0
	vperm2i128	$32, %ymm4, %ymm1, %ymm5
	vperm2i128	$49, %ymm4, %ymm1, %ymm1
	vperm2i128	$32, %ymm2, %ymm6, %ymm4
	vperm2i128	$49, %ymm2, %ymm6, %ymm2
	vperm2i128	$32, %ymm0, %ymm3, %ymm6
	vperm2i128	$49, %ymm0, %ymm3, %ymm0
	vmovdqu	%ymm5, 192(%rcx)
	vmovdqu	%ymm4, 224(%rcx)
	vmovdqu	%ymm6, 256(%rcx)
	vmovdqu	%ymm1, 288(%rcx)
	vmovdqu	%ymm2, 320(%rcx)
	vmovdqu	%ymm0, 352(%rcx)
	ret
L_poly_sub$1:
	vmovdqu	(%rsi), %ymm0
	vmovdqu	(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, (%rcx)
	vmovdqu	32(%rsi), %ymm0
	vmovdqu	32(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 32(%rcx)
	vmovdqu	64(%rsi), %ymm0
	vmovdqu	64(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 64(%rcx)
	vmovdqu	96(%rsi), %ymm0
	vmovdqu	96(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 96(%rcx)
	vmovdqu	128(%rsi), %ymm0
	vmovdqu	128(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 128(%rcx)
	vmovdqu	160(%rsi), %ymm0
	vmovdqu	160(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 160(%rcx)
	vmovdqu	192(%rsi), %ymm0
	vmovdqu	192(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 192(%rcx)
	vmovdqu	224(%rsi), %ymm0
	vmovdqu	224(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 224(%rcx)
	vmovdqu	256(%rsi), %ymm0
	vmovdqu	256(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 256(%rcx)
	vmovdqu	288(%rsi), %ymm0
	vmovdqu	288(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 288(%rcx)
	vmovdqu	320(%rsi), %ymm0
	vmovdqu	320(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 320(%rcx)
	vmovdqu	352(%rsi), %ymm0
	vmovdqu	352(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 352(%rcx)
	vmovdqu	384(%rsi), %ymm0
	vmovdqu	384(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 384(%rcx)
	vmovdqu	416(%rsi), %ymm0
	vmovdqu	416(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 416(%rcx)
	vmovdqu	448(%rsi), %ymm0
	vmovdqu	448(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 448(%rcx)
	vmovdqu	480(%rsi), %ymm0
	vmovdqu	480(%rdi), %ymm1
	vpsubw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 480(%rcx)
	ret
L_poly_ntt$1:
	leaq	glob_data + 1280(%rip), %rsi
	vmovdqu	glob_data + 448(%rip), %ymm0
	vpbroadcastd	(%rsi), %ymm1
	vpbroadcastd	4(%rsi), %ymm2
	vmovdqu	(%rcx), %ymm3
	vmovdqu	32(%rcx), %ymm14
	vmovdqu	64(%rcx), %ymm4
	vmovdqu	96(%rcx), %ymm5
	vmovdqu	256(%rcx), %ymm6
	vmovdqu	288(%rcx), %ymm7
	vmovdqu	320(%rcx), %ymm12
	vmovdqu	352(%rcx), %ymm8
	vpmullw	%ymm6, %ymm1, %ymm9
	vpmulhw	%ymm6, %ymm2, %ymm6
	vpmullw	%ymm7, %ymm1, %ymm10
	vpmulhw	%ymm7, %ymm2, %ymm15
	vpmullw	%ymm12, %ymm1, %ymm11
	vpmulhw	%ymm12, %ymm2, %ymm7
	vpmullw	%ymm8, %ymm1, %ymm12
	vpmulhw	%ymm8, %ymm2, %ymm8
	vpmulhw	%ymm0, %ymm9, %ymm9
	vpmulhw	%ymm0, %ymm10, %ymm10
	vpmulhw	%ymm0, %ymm11, %ymm11
	vpmulhw	%ymm0, %ymm12, %ymm12
	vpsubw	%ymm15, %ymm14, %ymm13
	vpaddw	%ymm14, %ymm15, %ymm14
	vpsubw	%ymm6, %ymm3, %ymm15
	vpaddw	%ymm3, %ymm6, %ymm3
	vpsubw	%ymm8, %ymm5, %ymm6
	vpaddw	%ymm5, %ymm8, %ymm5
	vpsubw	%ymm7, %ymm4, %ymm8
	vpaddw	%ymm4, %ymm7, %ymm4
	vpaddw	%ymm15, %ymm9, %ymm7
	vpsubw	%ymm9, %ymm3, %ymm3
	vpaddw	%ymm13, %ymm10, %ymm9
	vpsubw	%ymm10, %ymm14, %ymm10
	vpaddw	%ymm8, %ymm11, %ymm8
	vpsubw	%ymm11, %ymm4, %ymm4
	vpaddw	%ymm6, %ymm12, %ymm6
	vpsubw	%ymm12, %ymm5, %ymm5
	vmovdqu	%ymm3, (%rcx)
	vmovdqu	%ymm10, 32(%rcx)
	vmovdqu	%ymm4, 64(%rcx)
	vmovdqu	%ymm5, 96(%rcx)
	vmovdqu	%ymm7, 256(%rcx)
	vmovdqu	%ymm9, 288(%rcx)
	vmovdqu	%ymm8, 320(%rcx)
	vmovdqu	%ymm6, 352(%rcx)
	vmovdqu	128(%rcx), %ymm3
	vmovdqu	160(%rcx), %ymm12
	vmovdqu	192(%rcx), %ymm4
	vmovdqu	224(%rcx), %ymm5
	vmovdqu	384(%rcx), %ymm6
	vmovdqu	416(%rcx), %ymm7
	vmovdqu	448(%rcx), %ymm10
	vmovdqu	480(%rcx), %ymm11
	vpmullw	%ymm6, %ymm1, %ymm14
	vpmulhw	%ymm6, %ymm2, %ymm6
	vpmullw	%ymm7, %ymm1, %ymm8
	vpmulhw	%ymm7, %ymm2, %ymm13
	vpmullw	%ymm10, %ymm1, %ymm9
	vpmulhw	%ymm10, %ymm2, %ymm7
	vpmullw	%ymm11, %ymm1, %ymm10
	vpmulhw	%ymm11, %ymm2, %ymm1
	vpmulhw	%ymm0, %ymm14, %ymm2
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpmulhw	%ymm0, %ymm9, %ymm9
	vpmulhw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpsubw	%ymm6, %ymm3, %ymm13
	vpaddw	%ymm3, %ymm6, %ymm3
	vpsubw	%ymm1, %ymm5, %ymm6
	vpaddw	%ymm5, %ymm1, %ymm1
	vpsubw	%ymm7, %ymm4, %ymm5
	vpaddw	%ymm4, %ymm7, %ymm4
	vpaddw	%ymm13, %ymm2, %ymm7
	vpsubw	%ymm2, %ymm3, %ymm2
	vpaddw	%ymm11, %ymm8, %ymm3
	vpsubw	%ymm8, %ymm12, %ymm8
	vpaddw	%ymm5, %ymm9, %ymm5
	vpsubw	%ymm9, %ymm4, %ymm9
	vpaddw	%ymm6, %ymm10, %ymm4
	vpsubw	%ymm10, %ymm1, %ymm14
	vmovdqu	%ymm7, 384(%rcx)
	vmovdqu	%ymm3, 416(%rcx)
	vmovdqu	%ymm5, 448(%rcx)
	vmovdqu	%ymm4, 480(%rcx)
	vpbroadcastd	8(%rsi), %ymm1
	vpbroadcastd	12(%rsi), %ymm4
	vmovdqu	%ymm8, %ymm10
	vmovdqu	%ymm9, %ymm3
	vmovdqu	(%rcx), %ymm6
	vmovdqu	32(%rcx), %ymm5
	vmovdqu	64(%rcx), %ymm9
	vmovdqu	96(%rcx), %ymm7
	vpmullw	%ymm2, %ymm1, %ymm11
	vpmulhw	%ymm2, %ymm4, %ymm8
	vpmullw	%ymm10, %ymm1, %ymm13
	vpmulhw	%ymm10, %ymm4, %ymm12
	vpmullw	%ymm3, %ymm1, %ymm2
	vpmulhw	%ymm3, %ymm4, %ymm10
	vpmullw	%ymm14, %ymm1, %ymm3
	vpmulhw	%ymm14, %ymm4, %ymm14
	vpmulhw	%ymm0, %ymm11, %ymm11
	vpmulhw	%ymm0, %ymm13, %ymm1
	vpmulhw	%ymm0, %ymm2, %ymm2
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm12, %ymm5, %ymm4
	vpaddw	%ymm5, %ymm12, %ymm5
	vpsubw	%ymm8, %ymm6, %ymm12
	vpaddw	%ymm6, %ymm8, %ymm13
	vpsubw	%ymm14, %ymm7, %ymm6
	vpaddw	%ymm7, %ymm14, %ymm7
	vpsubw	%ymm10, %ymm9, %ymm8
	vpaddw	%ymm9, %ymm10, %ymm9
	vpaddw	%ymm12, %ymm11, %ymm10
	vpsubw	%ymm11, %ymm13, %ymm12
	vpaddw	%ymm4, %ymm1, %ymm4
	vpsubw	%ymm1, %ymm5, %ymm5
	vpaddw	%ymm8, %ymm2, %ymm8
	vpsubw	%ymm2, %ymm9, %ymm2
	vpaddw	%ymm6, %ymm3, %ymm13
	vpsubw	%ymm3, %ymm7, %ymm7
	vmovdqu	16(%rsi), %ymm3
	vmovdqu	48(%rsi), %ymm6
	vperm2i128	$32, %ymm10, %ymm12, %ymm11
	vperm2i128	$49, %ymm10, %ymm12, %ymm9
	vperm2i128	$32, %ymm4, %ymm5, %ymm1
	vperm2i128	$49, %ymm4, %ymm5, %ymm12
	vperm2i128	$32, %ymm8, %ymm2, %ymm5
	vperm2i128	$49, %ymm8, %ymm2, %ymm8
	vperm2i128	$32, %ymm13, %ymm7, %ymm2
	vperm2i128	$49, %ymm13, %ymm7, %ymm13
	vpmullw	%ymm5, %ymm3, %ymm4
	vpmulhw	%ymm5, %ymm6, %ymm10
	vpmullw	%ymm8, %ymm3, %ymm14
	vpmulhw	%ymm8, %ymm6, %ymm8
	vpmullw	%ymm2, %ymm3, %ymm5
	vpmulhw	%ymm2, %ymm6, %ymm2
	vpmullw	%ymm13, %ymm3, %ymm7
	vpmulhw	%ymm13, %ymm6, %ymm13
	vpmulhw	%ymm0, %ymm4, %ymm3
	vpmulhw	%ymm0, %ymm14, %ymm4
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm7, %ymm6
	vpsubw	%ymm8, %ymm9, %ymm7
	vpaddw	%ymm9, %ymm8, %ymm8
	vpsubw	%ymm10, %ymm11, %ymm9
	vpaddw	%ymm11, %ymm10, %ymm10
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpsubw	%ymm2, %ymm1, %ymm13
	vpaddw	%ymm1, %ymm2, %ymm1
	vpaddw	%ymm9, %ymm3, %ymm2
	vpsubw	%ymm3, %ymm10, %ymm9
	vpaddw	%ymm7, %ymm4, %ymm3
	vpsubw	%ymm4, %ymm8, %ymm4
	vpaddw	%ymm13, %ymm5, %ymm7
	vpsubw	%ymm5, %ymm1, %ymm13
	vpaddw	%ymm11, %ymm6, %ymm11
	vpsubw	%ymm6, %ymm12, %ymm14
	vmovdqu	80(%rsi), %ymm1
	vmovdqu	112(%rsi), %ymm10
	vpunpcklqdq	%ymm2, %ymm9, %ymm5
	vpunpckhqdq	%ymm2, %ymm9, %ymm12
	vpunpcklqdq	%ymm3, %ymm4, %ymm8
	vpunpckhqdq	%ymm3, %ymm4, %ymm6
	vpunpcklqdq	%ymm7, %ymm13, %ymm2
	vpunpckhqdq	%ymm7, %ymm13, %ymm4
	vpunpcklqdq	%ymm11, %ymm14, %ymm3
	vpunpckhqdq	%ymm11, %ymm14, %ymm7
	vpmullw	%ymm2, %ymm1, %ymm11
	vpmulhw	%ymm2, %ymm10, %ymm13
	vpmullw	%ymm4, %ymm1, %ymm14
	vpmulhw	%ymm4, %ymm10, %ymm4
	vpmullw	%ymm3, %ymm1, %ymm2
	vpmulhw	%ymm3, %ymm10, %ymm9
	vpmullw	%ymm7, %ymm1, %ymm3
	vpmulhw	%ymm7, %ymm10, %ymm7
	vpmulhw	%ymm0, %ymm11, %ymm10
	vpmulhw	%ymm0, %ymm14, %ymm1
	vpmulhw	%ymm0, %ymm2, %ymm2
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm4, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm4, %ymm4
	vpsubw	%ymm13, %ymm5, %ymm12
	vpaddw	%ymm5, %ymm13, %ymm13
	vpsubw	%ymm7, %ymm6, %ymm5
	vpaddw	%ymm6, %ymm7, %ymm6
	vpsubw	%ymm9, %ymm8, %ymm7
	vpaddw	%ymm8, %ymm9, %ymm8
	vpaddw	%ymm12, %ymm10, %ymm9
	vpsubw	%ymm10, %ymm13, %ymm12
	vpaddw	%ymm11, %ymm1, %ymm10
	vpsubw	%ymm1, %ymm4, %ymm4
	vpaddw	%ymm7, %ymm2, %ymm7
	vpsubw	%ymm2, %ymm8, %ymm2
	vpaddw	%ymm5, %ymm3, %ymm13
	vpsubw	%ymm3, %ymm6, %ymm6
	vmovdqu	144(%rsi), %ymm3
	vmovdqu	176(%rsi), %ymm5
	vmovsldup	%ymm9, %ymm1
	vpblendd	$-86, %ymm1, %ymm12, %ymm11
	vpsrlq	$32, %ymm12, %ymm1
	vpblendd	$-86, %ymm9, %ymm1, %ymm8
	vmovsldup	%ymm10, %ymm1
	vpblendd	$-86, %ymm1, %ymm4, %ymm1
	vpsrlq	$32, %ymm4, %ymm4
	vpblendd	$-86, %ymm10, %ymm4, %ymm12
	vmovsldup	%ymm7, %ymm4
	vpblendd	$-86, %ymm4, %ymm2, %ymm9
	vpsrlq	$32, %ymm2, %ymm2
	vpblendd	$-86, %ymm7, %ymm2, %ymm7
	vmovsldup	%ymm13, %ymm2
	vpblendd	$-86, %ymm2, %ymm6, %ymm2
	vpsrlq	$32, %ymm6, %ymm4
	vpblendd	$-86, %ymm13, %ymm4, %ymm13
	vpmullw	%ymm9, %ymm3, %ymm4
	vpmulhw	%ymm9, %ymm5, %ymm10
	vpmullw	%ymm7, %ymm3, %ymm14
	vpmulhw	%ymm7, %ymm5, %ymm9
	vpmullw	%ymm2, %ymm3, %ymm6
	vpmulhw	%ymm2, %ymm5, %ymm2
	vpmullw	%ymm13, %ymm3, %ymm7
	vpmulhw	%ymm13, %ymm5, %ymm13
	vpmulhw	%ymm0, %ymm4, %ymm3
	vpmulhw	%ymm0, %ymm14, %ymm4
	vpmulhw	%ymm0, %ymm6, %ymm5
	vpmulhw	%ymm0, %ymm7, %ymm6
	vpsubw	%ymm9, %ymm8, %ymm7
	vpaddw	%ymm8, %ymm9, %ymm8
	vpsubw	%ymm10, %ymm11, %ymm9
	vpaddw	%ymm11, %ymm10, %ymm10
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpsubw	%ymm2, %ymm1, %ymm13
	vpaddw	%ymm1, %ymm2, %ymm1
	vpaddw	%ymm9, %ymm3, %ymm2
	vpsubw	%ymm3, %ymm10, %ymm9
	vpaddw	%ymm7, %ymm4, %ymm3
	vpsubw	%ymm4, %ymm8, %ymm4
	vpaddw	%ymm13, %ymm5, %ymm7
	vpsubw	%ymm5, %ymm1, %ymm13
	vpaddw	%ymm11, %ymm6, %ymm11
	vpsubw	%ymm6, %ymm12, %ymm14
	vmovdqu	208(%rsi), %ymm5
	vmovdqu	240(%rsi), %ymm6
	vpslld	$16, %ymm2, %ymm1
	vpblendw	$-86, %ymm1, %ymm9, %ymm10
	vpsrld	$16, %ymm9, %ymm1
	vpblendw	$-86, %ymm2, %ymm1, %ymm8
	vpslld	$16, %ymm3, %ymm1
	vpblendw	$-86, %ymm1, %ymm4, %ymm1
	vpsrld	$16, %ymm4, %ymm2
	vpblendw	$-86, %ymm3, %ymm2, %ymm12
	vpslld	$16, %ymm7, %ymm2
	vpblendw	$-86, %ymm2, %ymm13, %ymm4
	vpsrld	$16, %ymm13, %ymm2
	vpblendw	$-86, %ymm7, %ymm2, %ymm7
	vpslld	$16, %ymm11, %ymm2
	vpblendw	$-86, %ymm2, %ymm14, %ymm2
	vpsrld	$16, %ymm14, %ymm3
	vpblendw	$-86, %ymm11, %ymm3, %ymm13
	vpmullw	%ymm4, %ymm5, %ymm3
	vpmulhw	%ymm4, %ymm6, %ymm11
	vpmullw	%ymm7, %ymm5, %ymm4
	vpmulhw	%ymm7, %ymm6, %ymm9
	vpmullw	%ymm2, %ymm5, %ymm7
	vpmulhw	%ymm2, %ymm6, %ymm2
	vpmullw	%ymm13, %ymm5, %ymm14
	vpmulhw	%ymm13, %ymm6, %ymm13
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm4, %ymm4
	vpmulhw	%ymm0, %ymm7, %ymm5
	vpmulhw	%ymm0, %ymm14, %ymm6
	vpsubw	%ymm9, %ymm8, %ymm7
	vpaddw	%ymm8, %ymm9, %ymm8
	vpsubw	%ymm11, %ymm10, %ymm9
	vpaddw	%ymm10, %ymm11, %ymm10
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpsubw	%ymm2, %ymm1, %ymm13
	vpaddw	%ymm1, %ymm2, %ymm1
	vpaddw	%ymm9, %ymm3, %ymm2
	vpsubw	%ymm3, %ymm10, %ymm3
	vpaddw	%ymm7, %ymm4, %ymm7
	vpsubw	%ymm4, %ymm8, %ymm4
	vpaddw	%ymm13, %ymm5, %ymm8
	vpsubw	%ymm5, %ymm1, %ymm1
	vpaddw	%ymm11, %ymm6, %ymm5
	vpsubw	%ymm6, %ymm12, %ymm10
	vmovdqu	272(%rsi), %ymm9
	vmovdqu	304(%rsi), %ymm13
	vmovdqu	336(%rsi), %ymm11
	vmovdqu	368(%rsi), %ymm6
	vpmullw	%ymm1, %ymm9, %ymm14
	vpmulhw	%ymm1, %ymm13, %ymm12
	vpmullw	%ymm10, %ymm9, %ymm9
	vpmulhw	%ymm10, %ymm13, %ymm13
	vpmullw	%ymm8, %ymm11, %ymm10
	vpmulhw	%ymm8, %ymm6, %ymm1
	vpmullw	%ymm5, %ymm11, %ymm11
	vpmulhw	%ymm5, %ymm6, %ymm5
	vpmulhw	%ymm0, %ymm14, %ymm6
	vpmulhw	%ymm0, %ymm9, %ymm8
	vpmulhw	%ymm0, %ymm10, %ymm9
	vpmulhw	%ymm0, %ymm11, %ymm10
	vpsubw	%ymm13, %ymm4, %ymm11
	vpaddw	%ymm4, %ymm13, %ymm4
	vpsubw	%ymm12, %ymm3, %ymm13
	vpaddw	%ymm3, %ymm12, %ymm3
	vpsubw	%ymm5, %ymm7, %ymm12
	vpaddw	%ymm7, %ymm5, %ymm5
	vpsubw	%ymm1, %ymm2, %ymm7
	vpaddw	%ymm2, %ymm1, %ymm1
	vpaddw	%ymm13, %ymm6, %ymm2
	vpsubw	%ymm6, %ymm3, %ymm3
	vpaddw	%ymm11, %ymm8, %ymm6
	vpsubw	%ymm8, %ymm4, %ymm4
	vpaddw	%ymm7, %ymm9, %ymm7
	vpsubw	%ymm9, %ymm1, %ymm8
	vpaddw	%ymm12, %ymm10, %ymm1
	vpsubw	%ymm10, %ymm5, %ymm5
	vmovdqu	glob_data + 384(%rip), %ymm9
	vpmulhw	%ymm9, %ymm3, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm3, %ymm3
	vpmulhw	%ymm9, %ymm4, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm4, %ymm4
	vpmulhw	%ymm9, %ymm8, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm8, %ymm8
	vpmulhw	%ymm9, %ymm5, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm5, %ymm5
	vpmulhw	%ymm9, %ymm2, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm2, %ymm2
	vpmulhw	%ymm9, %ymm6, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm6, %ymm6
	vpmulhw	%ymm9, %ymm7, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm7, %ymm7
	vpmulhw	%ymm9, %ymm1, %ymm9
	vpsraw	$10, %ymm9, %ymm9
	vpmullw	%ymm0, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm1, %ymm1
	vmovdqu	%ymm3, (%rcx)
	vmovdqu	%ymm4, 32(%rcx)
	vmovdqu	%ymm2, 64(%rcx)
	vmovdqu	%ymm6, 96(%rcx)
	vmovdqu	%ymm8, 128(%rcx)
	vmovdqu	%ymm5, 160(%rcx)
	vmovdqu	%ymm7, 192(%rcx)
	vmovdqu	%ymm1, 224(%rcx)
	vpbroadcastd	400(%rsi), %ymm5
	vpbroadcastd	404(%rsi), %ymm8
	vmovdqu	384(%rcx), %ymm1
	vmovdqu	416(%rcx), %ymm2
	vmovdqu	448(%rcx), %ymm10
	vmovdqu	480(%rcx), %ymm9
	vmovdqu	256(%rcx), %ymm3
	vmovdqu	288(%rcx), %ymm7
	vmovdqu	320(%rcx), %ymm6
	vmovdqu	352(%rcx), %ymm4
	vpmullw	%ymm1, %ymm5, %ymm14
	vpmulhw	%ymm1, %ymm8, %ymm12
	vpmullw	%ymm2, %ymm5, %ymm1
	vpmulhw	%ymm2, %ymm8, %ymm11
	vpmullw	%ymm10, %ymm5, %ymm2
	vpmulhw	%ymm10, %ymm8, %ymm13
	vpmullw	%ymm9, %ymm5, %ymm10
	vpmulhw	%ymm9, %ymm8, %ymm5
	vpmulhw	%ymm0, %ymm14, %ymm8
	vpmulhw	%ymm0, %ymm1, %ymm9
	vpmulhw	%ymm0, %ymm2, %ymm1
	vpmulhw	%ymm0, %ymm10, %ymm2
	vpsubw	%ymm11, %ymm7, %ymm10
	vpaddw	%ymm7, %ymm11, %ymm11
	vpsubw	%ymm12, %ymm3, %ymm7
	vpaddw	%ymm3, %ymm12, %ymm12
	vpsubw	%ymm5, %ymm4, %ymm3
	vpaddw	%ymm4, %ymm5, %ymm4
	vpsubw	%ymm13, %ymm6, %ymm5
	vpaddw	%ymm6, %ymm13, %ymm6
	vpaddw	%ymm7, %ymm8, %ymm7
	vpsubw	%ymm8, %ymm12, %ymm12
	vpaddw	%ymm10, %ymm9, %ymm8
	vpsubw	%ymm9, %ymm11, %ymm9
	vpaddw	%ymm5, %ymm1, %ymm5
	vpsubw	%ymm1, %ymm6, %ymm6
	vpaddw	%ymm3, %ymm2, %ymm13
	vpsubw	%ymm2, %ymm4, %ymm2
	vmovdqu	408(%rsi), %ymm3
	vmovdqu	440(%rsi), %ymm4
	vperm2i128	$32, %ymm7, %ymm12, %ymm10
	vperm2i128	$49, %ymm7, %ymm12, %ymm11
	vperm2i128	$32, %ymm8, %ymm9, %ymm1
	vperm2i128	$49, %ymm8, %ymm9, %ymm12
	vperm2i128	$32, %ymm5, %ymm6, %ymm8
	vperm2i128	$49, %ymm5, %ymm6, %ymm5
	vperm2i128	$32, %ymm13, %ymm2, %ymm6
	vperm2i128	$49, %ymm13, %ymm2, %ymm13
	vpmullw	%ymm8, %ymm3, %ymm7
	vpmulhw	%ymm8, %ymm4, %ymm14
	vpmullw	%ymm5, %ymm3, %ymm9
	vpmulhw	%ymm5, %ymm4, %ymm8
	vpmullw	%ymm6, %ymm3, %ymm5
	vpmulhw	%ymm6, %ymm4, %ymm2
	vpmullw	%ymm13, %ymm3, %ymm6
	vpmulhw	%ymm13, %ymm4, %ymm13
	vpmulhw	%ymm0, %ymm7, %ymm3
	vpmulhw	%ymm0, %ymm9, %ymm4
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm8, %ymm11, %ymm7
	vpaddw	%ymm11, %ymm8, %ymm8
	vpsubw	%ymm14, %ymm10, %ymm9
	vpaddw	%ymm10, %ymm14, %ymm10
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpsubw	%ymm2, %ymm1, %ymm13
	vpaddw	%ymm1, %ymm2, %ymm1
	vpaddw	%ymm9, %ymm3, %ymm2
	vpsubw	%ymm3, %ymm10, %ymm9
	vpaddw	%ymm7, %ymm4, %ymm3
	vpsubw	%ymm4, %ymm8, %ymm4
	vpaddw	%ymm13, %ymm5, %ymm7
	vpsubw	%ymm5, %ymm1, %ymm13
	vpaddw	%ymm11, %ymm6, %ymm11
	vpsubw	%ymm6, %ymm12, %ymm14
	vmovdqu	472(%rsi), %ymm1
	vmovdqu	504(%rsi), %ymm10
	vpunpcklqdq	%ymm2, %ymm9, %ymm5
	vpunpckhqdq	%ymm2, %ymm9, %ymm12
	vpunpcklqdq	%ymm3, %ymm4, %ymm8
	vpunpckhqdq	%ymm3, %ymm4, %ymm6
	vpunpcklqdq	%ymm7, %ymm13, %ymm2
	vpunpckhqdq	%ymm7, %ymm13, %ymm4
	vpunpcklqdq	%ymm11, %ymm14, %ymm3
	vpunpckhqdq	%ymm11, %ymm14, %ymm7
	vpmullw	%ymm2, %ymm1, %ymm11
	vpmulhw	%ymm2, %ymm10, %ymm13
	vpmullw	%ymm4, %ymm1, %ymm14
	vpmulhw	%ymm4, %ymm10, %ymm4
	vpmullw	%ymm3, %ymm1, %ymm2
	vpmulhw	%ymm3, %ymm10, %ymm9
	vpmullw	%ymm7, %ymm1, %ymm3
	vpmulhw	%ymm7, %ymm10, %ymm7
	vpmulhw	%ymm0, %ymm11, %ymm10
	vpmulhw	%ymm0, %ymm14, %ymm1
	vpmulhw	%ymm0, %ymm2, %ymm2
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm4, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm4, %ymm4
	vpsubw	%ymm13, %ymm5, %ymm12
	vpaddw	%ymm5, %ymm13, %ymm13
	vpsubw	%ymm7, %ymm6, %ymm5
	vpaddw	%ymm6, %ymm7, %ymm6
	vpsubw	%ymm9, %ymm8, %ymm7
	vpaddw	%ymm8, %ymm9, %ymm8
	vpaddw	%ymm12, %ymm10, %ymm9
	vpsubw	%ymm10, %ymm13, %ymm12
	vpaddw	%ymm11, %ymm1, %ymm10
	vpsubw	%ymm1, %ymm4, %ymm4
	vpaddw	%ymm7, %ymm2, %ymm7
	vpsubw	%ymm2, %ymm8, %ymm2
	vpaddw	%ymm5, %ymm3, %ymm13
	vpsubw	%ymm3, %ymm6, %ymm6
	vmovdqu	536(%rsi), %ymm3
	vmovdqu	568(%rsi), %ymm5
	vmovsldup	%ymm9, %ymm1
	vpblendd	$-86, %ymm1, %ymm12, %ymm11
	vpsrlq	$32, %ymm12, %ymm1
	vpblendd	$-86, %ymm9, %ymm1, %ymm8
	vmovsldup	%ymm10, %ymm1
	vpblendd	$-86, %ymm1, %ymm4, %ymm1
	vpsrlq	$32, %ymm4, %ymm4
	vpblendd	$-86, %ymm10, %ymm4, %ymm12
	vmovsldup	%ymm7, %ymm4
	vpblendd	$-86, %ymm4, %ymm2, %ymm9
	vpsrlq	$32, %ymm2, %ymm2
	vpblendd	$-86, %ymm7, %ymm2, %ymm7
	vmovsldup	%ymm13, %ymm2
	vpblendd	$-86, %ymm2, %ymm6, %ymm2
	vpsrlq	$32, %ymm6, %ymm4
	vpblendd	$-86, %ymm13, %ymm4, %ymm13
	vpmullw	%ymm9, %ymm3, %ymm4
	vpmulhw	%ymm9, %ymm5, %ymm10
	vpmullw	%ymm7, %ymm3, %ymm14
	vpmulhw	%ymm7, %ymm5, %ymm9
	vpmullw	%ymm2, %ymm3, %ymm6
	vpmulhw	%ymm2, %ymm5, %ymm2
	vpmullw	%ymm13, %ymm3, %ymm7
	vpmulhw	%ymm13, %ymm5, %ymm13
	vpmulhw	%ymm0, %ymm4, %ymm3
	vpmulhw	%ymm0, %ymm14, %ymm4
	vpmulhw	%ymm0, %ymm6, %ymm5
	vpmulhw	%ymm0, %ymm7, %ymm6
	vpsubw	%ymm9, %ymm8, %ymm7
	vpaddw	%ymm8, %ymm9, %ymm8
	vpsubw	%ymm10, %ymm11, %ymm9
	vpaddw	%ymm11, %ymm10, %ymm10
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpsubw	%ymm2, %ymm1, %ymm13
	vpaddw	%ymm1, %ymm2, %ymm1
	vpaddw	%ymm9, %ymm3, %ymm2
	vpsubw	%ymm3, %ymm10, %ymm9
	vpaddw	%ymm7, %ymm4, %ymm3
	vpsubw	%ymm4, %ymm8, %ymm4
	vpaddw	%ymm13, %ymm5, %ymm7
	vpsubw	%ymm5, %ymm1, %ymm13
	vpaddw	%ymm11, %ymm6, %ymm11
	vpsubw	%ymm6, %ymm12, %ymm14
	vmovdqu	600(%rsi), %ymm5
	vmovdqu	632(%rsi), %ymm6
	vpslld	$16, %ymm2, %ymm1
	vpblendw	$-86, %ymm1, %ymm9, %ymm10
	vpsrld	$16, %ymm9, %ymm1
	vpblendw	$-86, %ymm2, %ymm1, %ymm8
	vpslld	$16, %ymm3, %ymm1
	vpblendw	$-86, %ymm1, %ymm4, %ymm1
	vpsrld	$16, %ymm4, %ymm2
	vpblendw	$-86, %ymm3, %ymm2, %ymm12
	vpslld	$16, %ymm7, %ymm2
	vpblendw	$-86, %ymm2, %ymm13, %ymm4
	vpsrld	$16, %ymm13, %ymm2
	vpblendw	$-86, %ymm7, %ymm2, %ymm7
	vpslld	$16, %ymm11, %ymm2
	vpblendw	$-86, %ymm2, %ymm14, %ymm2
	vpsrld	$16, %ymm14, %ymm3
	vpblendw	$-86, %ymm11, %ymm3, %ymm13
	vpmullw	%ymm4, %ymm5, %ymm3
	vpmulhw	%ymm4, %ymm6, %ymm11
	vpmullw	%ymm7, %ymm5, %ymm4
	vpmulhw	%ymm7, %ymm6, %ymm9
	vpmullw	%ymm2, %ymm5, %ymm7
	vpmulhw	%ymm2, %ymm6, %ymm2
	vpmullw	%ymm13, %ymm5, %ymm14
	vpmulhw	%ymm13, %ymm6, %ymm13
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm4, %ymm4
	vpmulhw	%ymm0, %ymm7, %ymm5
	vpmulhw	%ymm0, %ymm14, %ymm6
	vpsubw	%ymm9, %ymm8, %ymm7
	vpaddw	%ymm8, %ymm9, %ymm8
	vpsubw	%ymm11, %ymm10, %ymm9
	vpaddw	%ymm10, %ymm11, %ymm10
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpsubw	%ymm2, %ymm1, %ymm13
	vpaddw	%ymm1, %ymm2, %ymm1
	vpaddw	%ymm9, %ymm3, %ymm2
	vpsubw	%ymm3, %ymm10, %ymm3
	vpaddw	%ymm7, %ymm4, %ymm7
	vpsubw	%ymm4, %ymm8, %ymm4
	vpaddw	%ymm13, %ymm5, %ymm8
	vpsubw	%ymm5, %ymm1, %ymm1
	vpaddw	%ymm11, %ymm6, %ymm5
	vpsubw	%ymm6, %ymm12, %ymm10
	vmovdqu	664(%rsi), %ymm9
	vmovdqu	696(%rsi), %ymm13
	vmovdqu	728(%rsi), %ymm11
	vmovdqu	760(%rsi), %ymm6
	vpmullw	%ymm1, %ymm9, %ymm14
	vpmulhw	%ymm1, %ymm13, %ymm12
	vpmullw	%ymm10, %ymm9, %ymm9
	vpmulhw	%ymm10, %ymm13, %ymm13
	vpmullw	%ymm8, %ymm11, %ymm10
	vpmulhw	%ymm8, %ymm6, %ymm1
	vpmullw	%ymm5, %ymm11, %ymm11
	vpmulhw	%ymm5, %ymm6, %ymm5
	vpmulhw	%ymm0, %ymm14, %ymm6
	vpmulhw	%ymm0, %ymm9, %ymm8
	vpmulhw	%ymm0, %ymm10, %ymm9
	vpmulhw	%ymm0, %ymm11, %ymm10
	vpsubw	%ymm13, %ymm4, %ymm11
	vpaddw	%ymm4, %ymm13, %ymm4
	vpsubw	%ymm12, %ymm3, %ymm13
	vpaddw	%ymm3, %ymm12, %ymm3
	vpsubw	%ymm5, %ymm7, %ymm12
	vpaddw	%ymm7, %ymm5, %ymm5
	vpsubw	%ymm1, %ymm2, %ymm7
	vpaddw	%ymm2, %ymm1, %ymm1
	vpaddw	%ymm13, %ymm6, %ymm2
	vpsubw	%ymm6, %ymm3, %ymm3
	vpaddw	%ymm11, %ymm8, %ymm6
	vpsubw	%ymm8, %ymm4, %ymm4
	vpaddw	%ymm7, %ymm9, %ymm7
	vpsubw	%ymm9, %ymm1, %ymm8
	vpaddw	%ymm12, %ymm10, %ymm1
	vpsubw	%ymm10, %ymm5, %ymm5
	vmovdqu	glob_data + 384(%rip), %ymm9
	vpmulhw	%ymm9, %ymm3, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm3, %ymm3
	vpmulhw	%ymm9, %ymm4, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm4, %ymm4
	vpmulhw	%ymm9, %ymm8, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm8, %ymm8
	vpmulhw	%ymm9, %ymm5, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm5, %ymm5
	vpmulhw	%ymm9, %ymm2, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm2, %ymm2
	vpmulhw	%ymm9, %ymm6, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm6, %ymm6
	vpmulhw	%ymm9, %ymm7, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm7, %ymm7
	vpmulhw	%ymm9, %ymm1, %ymm9
	vpsraw	$10, %ymm9, %ymm9
	vpmullw	%ymm0, %ymm9, %ymm0
	vpsubw	%ymm0, %ymm1, %ymm0
	vmovdqu	%ymm3, 256(%rcx)
	vmovdqu	%ymm4, 288(%rcx)
	vmovdqu	%ymm2, 320(%rcx)
	vmovdqu	%ymm6, 352(%rcx)
	vmovdqu	%ymm8, 384(%rcx)
	vmovdqu	%ymm5, 416(%rcx)
	vmovdqu	%ymm7, 448(%rcx)
	vmovdqu	%ymm0, 480(%rcx)
	ret
L_poly_invntt$1:
	leaq	glob_data + 480(%rip), %rsi
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	(%rsi), %ymm12
	vmovdqu	64(%rsi), %ymm1
	vmovdqu	32(%rsi), %ymm2
	vmovdqu	96(%rsi), %ymm5
	vmovdqu	(%rcx), %ymm3
	vmovdqu	32(%rcx), %ymm4
	vmovdqu	64(%rcx), %ymm15
	vmovdqu	96(%rcx), %ymm11
	vmovdqu	128(%rcx), %ymm7
	vmovdqu	160(%rcx), %ymm8
	vmovdqu	192(%rcx), %ymm13
	vmovdqu	224(%rcx), %ymm14
	vpsubw	%ymm15, %ymm3, %ymm6
	vpsubw	%ymm11, %ymm4, %ymm9
	vpsubw	%ymm13, %ymm7, %ymm10
	vpaddw	%ymm3, %ymm15, %ymm3
	vpaddw	%ymm4, %ymm11, %ymm4
	vpmullw	%ymm6, %ymm12, %ymm11
	vpaddw	%ymm7, %ymm13, %ymm7
	vpmullw	%ymm9, %ymm12, %ymm12
	vpsubw	%ymm14, %ymm8, %ymm13
	vpaddw	%ymm8, %ymm14, %ymm8
	vpmullw	%ymm10, %ymm1, %ymm14
	vpmullw	%ymm13, %ymm1, %ymm1
	vpmulhw	%ymm6, %ymm2, %ymm6
	vpmulhw	%ymm9, %ymm2, %ymm2
	vpmulhw	%ymm10, %ymm5, %ymm9
	vpmulhw	%ymm13, %ymm5, %ymm5
	vpmulhw	%ymm11, %ymm0, %ymm10
	vpmulhw	%ymm12, %ymm0, %ymm11
	vpmulhw	%ymm14, %ymm0, %ymm12
	vpmulhw	%ymm1, %ymm0, %ymm1
	vpsubw	%ymm10, %ymm6, %ymm6
	vpsubw	%ymm11, %ymm2, %ymm11
	vpsubw	%ymm12, %ymm9, %ymm9
	vpsubw	%ymm1, %ymm5, %ymm13
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	128(%rsi), %ymm2
	vmovdqu	160(%rsi), %ymm5
	vpmulhw	%ymm1, %ymm3, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm3, %ymm10
	vpmulhw	%ymm1, %ymm4, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm12
	vpmulhw	%ymm1, %ymm7, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm7, %ymm7
	vpmulhw	%ymm1, %ymm8, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm8, %ymm8
	vpsubw	%ymm7, %ymm10, %ymm15
	vpsubw	%ymm8, %ymm12, %ymm3
	vpsubw	%ymm9, %ymm6, %ymm4
	vpaddw	%ymm10, %ymm7, %ymm7
	vpaddw	%ymm12, %ymm8, %ymm8
	vpmullw	%ymm15, %ymm2, %ymm10
	vpaddw	%ymm6, %ymm9, %ymm9
	vpmullw	%ymm3, %ymm2, %ymm6
	vpsubw	%ymm13, %ymm11, %ymm12
	vpaddw	%ymm11, %ymm13, %ymm11
	vpmullw	%ymm4, %ymm2, %ymm13
	vpmullw	%ymm12, %ymm2, %ymm14
	vpmulhw	%ymm15, %ymm5, %ymm2
	vpmulhw	%ymm3, %ymm5, %ymm3
	vpmulhw	%ymm4, %ymm5, %ymm4
	vpmulhw	%ymm12, %ymm5, %ymm5
	vpmulhw	%ymm10, %ymm0, %ymm10
	vpmulhw	%ymm6, %ymm0, %ymm12
	vpmulhw	%ymm13, %ymm0, %ymm13
	vpmulhw	%ymm14, %ymm0, %ymm6
	vpsubw	%ymm10, %ymm2, %ymm2
	vpsubw	%ymm12, %ymm3, %ymm3
	vpsubw	%ymm13, %ymm4, %ymm4
	vpsubw	%ymm6, %ymm5, %ymm5
	vpslld	$16, %ymm8, %ymm6
	vpblendw	$-86, %ymm6, %ymm7, %ymm6
	vpsrld	$16, %ymm7, %ymm7
	vpblendw	$-86, %ymm8, %ymm7, %ymm7
	vpslld	$16, %ymm11, %ymm8
	vpblendw	$-86, %ymm8, %ymm9, %ymm10
	vpsrld	$16, %ymm9, %ymm8
	vpblendw	$-86, %ymm11, %ymm8, %ymm11
	vpslld	$16, %ymm3, %ymm8
	vpblendw	$-86, %ymm8, %ymm2, %ymm8
	vpsrld	$16, %ymm2, %ymm2
	vpblendw	$-86, %ymm3, %ymm2, %ymm9
	vpslld	$16, %ymm5, %ymm2
	vpblendw	$-86, %ymm2, %ymm4, %ymm12
	vpsrld	$16, %ymm4, %ymm2
	vpblendw	$-86, %ymm5, %ymm2, %ymm13
	vmovdqu	192(%rsi), %ymm2
	vmovdqu	224(%rsi), %ymm5
	vpsubw	%ymm7, %ymm6, %ymm15
	vpsubw	%ymm11, %ymm10, %ymm3
	vpsubw	%ymm9, %ymm8, %ymm4
	vpaddw	%ymm6, %ymm7, %ymm6
	vpaddw	%ymm10, %ymm11, %ymm7
	vpmullw	%ymm15, %ymm2, %ymm10
	vpaddw	%ymm8, %ymm9, %ymm8
	vpmullw	%ymm3, %ymm2, %ymm9
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpmullw	%ymm4, %ymm2, %ymm13
	vpmullw	%ymm11, %ymm2, %ymm14
	vpmulhw	%ymm15, %ymm5, %ymm2
	vpmulhw	%ymm3, %ymm5, %ymm3
	vpmulhw	%ymm4, %ymm5, %ymm4
	vpmulhw	%ymm11, %ymm5, %ymm5
	vpmulhw	%ymm10, %ymm0, %ymm10
	vpmulhw	%ymm9, %ymm0, %ymm11
	vpmulhw	%ymm13, %ymm0, %ymm13
	vpmulhw	%ymm14, %ymm0, %ymm9
	vpsubw	%ymm10, %ymm2, %ymm2
	vpsubw	%ymm11, %ymm3, %ymm3
	vpsubw	%ymm13, %ymm4, %ymm4
	vpsubw	%ymm9, %ymm5, %ymm5
	vpmulhw	%ymm1, %ymm6, %ymm9
	vpsraw	$10, %ymm9, %ymm9
	vpmullw	%ymm0, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm6, %ymm6
	vmovsldup	%ymm7, %ymm9
	vpblendd	$-86, %ymm9, %ymm6, %ymm9
	vpsrlq	$32, %ymm6, %ymm6
	vpblendd	$-86, %ymm7, %ymm6, %ymm10
	vmovsldup	%ymm12, %ymm6
	vpblendd	$-86, %ymm6, %ymm8, %ymm11
	vpsrlq	$32, %ymm8, %ymm6
	vpblendd	$-86, %ymm12, %ymm6, %ymm14
	vmovsldup	%ymm3, %ymm6
	vpblendd	$-86, %ymm6, %ymm2, %ymm12
	vpsrlq	$32, %ymm2, %ymm2
	vpblendd	$-86, %ymm3, %ymm2, %ymm13
	vmovsldup	%ymm5, %ymm2
	vpblendd	$-86, %ymm2, %ymm4, %ymm2
	vpsrlq	$32, %ymm4, %ymm3
	vpblendd	$-86, %ymm5, %ymm3, %ymm3
	vmovdqu	256(%rsi), %ymm4
	vmovdqu	288(%rsi), %ymm5
	vpsubw	%ymm10, %ymm9, %ymm6
	vpsubw	%ymm14, %ymm11, %ymm7
	vpsubw	%ymm13, %ymm12, %ymm8
	vpaddw	%ymm9, %ymm10, %ymm9
	vpaddw	%ymm11, %ymm14, %ymm10
	vpmullw	%ymm6, %ymm4, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpmullw	%ymm7, %ymm4, %ymm13
	vpsubw	%ymm3, %ymm2, %ymm14
	vpaddw	%ymm2, %ymm3, %ymm2
	vpmullw	%ymm8, %ymm4, %ymm3
	vpmullw	%ymm14, %ymm4, %ymm4
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpmulhw	%ymm7, %ymm5, %ymm7
	vpmulhw	%ymm8, %ymm5, %ymm8
	vpmulhw	%ymm14, %ymm5, %ymm5
	vpmulhw	%ymm11, %ymm0, %ymm11
	vpmulhw	%ymm13, %ymm0, %ymm13
	vpmulhw	%ymm3, %ymm0, %ymm3
	vpmulhw	%ymm4, %ymm0, %ymm4
	vpsubw	%ymm11, %ymm6, %ymm6
	vpsubw	%ymm13, %ymm7, %ymm7
	vpsubw	%ymm3, %ymm8, %ymm3
	vpsubw	%ymm4, %ymm5, %ymm4
	vpmulhw	%ymm1, %ymm9, %ymm5
	vpsraw	$10, %ymm5, %ymm5
	vpmullw	%ymm0, %ymm5, %ymm5
	vpsubw	%ymm5, %ymm9, %ymm8
	vpunpcklqdq	%ymm10, %ymm8, %ymm5
	vpunpckhqdq	%ymm10, %ymm8, %ymm8
	vpunpcklqdq	%ymm2, %ymm12, %ymm9
	vpunpckhqdq	%ymm2, %ymm12, %ymm2
	vpunpcklqdq	%ymm7, %ymm6, %ymm10
	vpunpckhqdq	%ymm7, %ymm6, %ymm6
	vpunpcklqdq	%ymm4, %ymm3, %ymm7
	vpunpckhqdq	%ymm4, %ymm3, %ymm3
	vmovdqu	320(%rsi), %ymm4
	vmovdqu	352(%rsi), %ymm11
	vpsubw	%ymm8, %ymm5, %ymm12
	vpsubw	%ymm2, %ymm9, %ymm13
	vpsubw	%ymm6, %ymm10, %ymm14
	vpaddw	%ymm5, %ymm8, %ymm5
	vpaddw	%ymm9, %ymm2, %ymm2
	vpmullw	%ymm12, %ymm4, %ymm8
	vpaddw	%ymm10, %ymm6, %ymm6
	vpmullw	%ymm13, %ymm4, %ymm9
	vpsubw	%ymm3, %ymm7, %ymm10
	vpaddw	%ymm7, %ymm3, %ymm3
	vpmullw	%ymm14, %ymm4, %ymm7
	vpmullw	%ymm10, %ymm4, %ymm4
	vpmulhw	%ymm12, %ymm11, %ymm12
	vpmulhw	%ymm13, %ymm11, %ymm13
	vpmulhw	%ymm14, %ymm11, %ymm14
	vpmulhw	%ymm10, %ymm11, %ymm10
	vpmulhw	%ymm8, %ymm0, %ymm8
	vpmulhw	%ymm9, %ymm0, %ymm9
	vpmulhw	%ymm7, %ymm0, %ymm7
	vpmulhw	%ymm4, %ymm0, %ymm4
	vpsubw	%ymm8, %ymm12, %ymm8
	vpsubw	%ymm9, %ymm13, %ymm9
	vpsubw	%ymm7, %ymm14, %ymm7
	vpsubw	%ymm4, %ymm10, %ymm4
	vpmulhw	%ymm1, %ymm5, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm5, %ymm10
	vperm2i128	$32, %ymm2, %ymm10, %ymm5
	vperm2i128	$49, %ymm2, %ymm10, %ymm2
	vperm2i128	$32, %ymm3, %ymm6, %ymm10
	vperm2i128	$49, %ymm3, %ymm6, %ymm3
	vperm2i128	$32, %ymm9, %ymm8, %ymm6
	vperm2i128	$49, %ymm9, %ymm8, %ymm8
	vperm2i128	$32, %ymm4, %ymm7, %ymm9
	vperm2i128	$49, %ymm4, %ymm7, %ymm4
	vpbroadcastd	384(%rsi), %ymm7
	vpbroadcastd	388(%rsi), %ymm11
	vpsubw	%ymm2, %ymm5, %ymm12
	vpsubw	%ymm3, %ymm10, %ymm13
	vpsubw	%ymm8, %ymm6, %ymm14
	vpaddw	%ymm5, %ymm2, %ymm2
	vpaddw	%ymm10, %ymm3, %ymm3
	vpmullw	%ymm12, %ymm7, %ymm5
	vpaddw	%ymm6, %ymm8, %ymm6
	vpmullw	%ymm13, %ymm7, %ymm8
	vpsubw	%ymm4, %ymm9, %ymm10
	vpaddw	%ymm9, %ymm4, %ymm4
	vpmullw	%ymm14, %ymm7, %ymm9
	vpmullw	%ymm10, %ymm7, %ymm7
	vpmulhw	%ymm12, %ymm11, %ymm12
	vpmulhw	%ymm13, %ymm11, %ymm13
	vpmulhw	%ymm14, %ymm11, %ymm14
	vpmulhw	%ymm10, %ymm11, %ymm10
	vpmulhw	%ymm5, %ymm0, %ymm5
	vpmulhw	%ymm8, %ymm0, %ymm8
	vpmulhw	%ymm9, %ymm0, %ymm9
	vpmulhw	%ymm7, %ymm0, %ymm7
	vpsubw	%ymm5, %ymm12, %ymm5
	vpsubw	%ymm8, %ymm13, %ymm8
	vpsubw	%ymm9, %ymm14, %ymm9
	vpsubw	%ymm7, %ymm10, %ymm7
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm1
	vpsubw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, (%rcx)
	vmovdqu	%ymm3, 32(%rcx)
	vmovdqu	%ymm6, 64(%rcx)
	vmovdqu	%ymm4, 96(%rcx)
	vmovdqu	%ymm5, 128(%rcx)
	vmovdqu	%ymm8, 160(%rcx)
	vmovdqu	%ymm9, 192(%rcx)
	vmovdqu	%ymm7, 224(%rcx)
	vmovdqu	392(%rsi), %ymm12
	vmovdqu	456(%rsi), %ymm1
	vmovdqu	424(%rsi), %ymm2
	vmovdqu	488(%rsi), %ymm5
	vmovdqu	256(%rcx), %ymm3
	vmovdqu	288(%rcx), %ymm4
	vmovdqu	320(%rcx), %ymm15
	vmovdqu	352(%rcx), %ymm11
	vmovdqu	384(%rcx), %ymm7
	vmovdqu	416(%rcx), %ymm8
	vmovdqu	448(%rcx), %ymm13
	vmovdqu	480(%rcx), %ymm14
	vpsubw	%ymm15, %ymm3, %ymm6
	vpsubw	%ymm11, %ymm4, %ymm9
	vpsubw	%ymm13, %ymm7, %ymm10
	vpaddw	%ymm3, %ymm15, %ymm3
	vpaddw	%ymm4, %ymm11, %ymm4
	vpmullw	%ymm6, %ymm12, %ymm11
	vpaddw	%ymm7, %ymm13, %ymm7
	vpmullw	%ymm9, %ymm12, %ymm12
	vpsubw	%ymm14, %ymm8, %ymm13
	vpaddw	%ymm8, %ymm14, %ymm8
	vpmullw	%ymm10, %ymm1, %ymm14
	vpmullw	%ymm13, %ymm1, %ymm1
	vpmulhw	%ymm6, %ymm2, %ymm6
	vpmulhw	%ymm9, %ymm2, %ymm2
	vpmulhw	%ymm10, %ymm5, %ymm9
	vpmulhw	%ymm13, %ymm5, %ymm5
	vpmulhw	%ymm11, %ymm0, %ymm10
	vpmulhw	%ymm12, %ymm0, %ymm11
	vpmulhw	%ymm14, %ymm0, %ymm12
	vpmulhw	%ymm1, %ymm0, %ymm1
	vpsubw	%ymm10, %ymm6, %ymm6
	vpsubw	%ymm11, %ymm2, %ymm11
	vpsubw	%ymm12, %ymm9, %ymm9
	vpsubw	%ymm1, %ymm5, %ymm13
	vmovdqu	glob_data + 384(%rip), %ymm1
	vmovdqu	520(%rsi), %ymm2
	vmovdqu	552(%rsi), %ymm5
	vpmulhw	%ymm1, %ymm3, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm3, %ymm10
	vpmulhw	%ymm1, %ymm4, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm12
	vpmulhw	%ymm1, %ymm7, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm7, %ymm7
	vpmulhw	%ymm1, %ymm8, %ymm3
	vpsraw	$10, %ymm3, %ymm3
	vpmullw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm8, %ymm8
	vpsubw	%ymm7, %ymm10, %ymm15
	vpsubw	%ymm8, %ymm12, %ymm3
	vpsubw	%ymm9, %ymm6, %ymm4
	vpaddw	%ymm10, %ymm7, %ymm7
	vpaddw	%ymm12, %ymm8, %ymm8
	vpmullw	%ymm15, %ymm2, %ymm10
	vpaddw	%ymm6, %ymm9, %ymm9
	vpmullw	%ymm3, %ymm2, %ymm6
	vpsubw	%ymm13, %ymm11, %ymm12
	vpaddw	%ymm11, %ymm13, %ymm11
	vpmullw	%ymm4, %ymm2, %ymm13
	vpmullw	%ymm12, %ymm2, %ymm14
	vpmulhw	%ymm15, %ymm5, %ymm2
	vpmulhw	%ymm3, %ymm5, %ymm3
	vpmulhw	%ymm4, %ymm5, %ymm4
	vpmulhw	%ymm12, %ymm5, %ymm5
	vpmulhw	%ymm10, %ymm0, %ymm10
	vpmulhw	%ymm6, %ymm0, %ymm12
	vpmulhw	%ymm13, %ymm0, %ymm13
	vpmulhw	%ymm14, %ymm0, %ymm6
	vpsubw	%ymm10, %ymm2, %ymm2
	vpsubw	%ymm12, %ymm3, %ymm3
	vpsubw	%ymm13, %ymm4, %ymm4
	vpsubw	%ymm6, %ymm5, %ymm5
	vpslld	$16, %ymm8, %ymm6
	vpblendw	$-86, %ymm6, %ymm7, %ymm6
	vpsrld	$16, %ymm7, %ymm7
	vpblendw	$-86, %ymm8, %ymm7, %ymm7
	vpslld	$16, %ymm11, %ymm8
	vpblendw	$-86, %ymm8, %ymm9, %ymm10
	vpsrld	$16, %ymm9, %ymm8
	vpblendw	$-86, %ymm11, %ymm8, %ymm11
	vpslld	$16, %ymm3, %ymm8
	vpblendw	$-86, %ymm8, %ymm2, %ymm8
	vpsrld	$16, %ymm2, %ymm2
	vpblendw	$-86, %ymm3, %ymm2, %ymm9
	vpslld	$16, %ymm5, %ymm2
	vpblendw	$-86, %ymm2, %ymm4, %ymm12
	vpsrld	$16, %ymm4, %ymm2
	vpblendw	$-86, %ymm5, %ymm2, %ymm13
	vmovdqu	584(%rsi), %ymm2
	vmovdqu	616(%rsi), %ymm5
	vpsubw	%ymm7, %ymm6, %ymm15
	vpsubw	%ymm11, %ymm10, %ymm3
	vpsubw	%ymm9, %ymm8, %ymm4
	vpaddw	%ymm6, %ymm7, %ymm6
	vpaddw	%ymm10, %ymm11, %ymm7
	vpmullw	%ymm15, %ymm2, %ymm10
	vpaddw	%ymm8, %ymm9, %ymm8
	vpmullw	%ymm3, %ymm2, %ymm9
	vpsubw	%ymm13, %ymm12, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpmullw	%ymm4, %ymm2, %ymm13
	vpmullw	%ymm11, %ymm2, %ymm14
	vpmulhw	%ymm15, %ymm5, %ymm2
	vpmulhw	%ymm3, %ymm5, %ymm3
	vpmulhw	%ymm4, %ymm5, %ymm4
	vpmulhw	%ymm11, %ymm5, %ymm5
	vpmulhw	%ymm10, %ymm0, %ymm10
	vpmulhw	%ymm9, %ymm0, %ymm11
	vpmulhw	%ymm13, %ymm0, %ymm13
	vpmulhw	%ymm14, %ymm0, %ymm9
	vpsubw	%ymm10, %ymm2, %ymm2
	vpsubw	%ymm11, %ymm3, %ymm3
	vpsubw	%ymm13, %ymm4, %ymm4
	vpsubw	%ymm9, %ymm5, %ymm5
	vpmulhw	%ymm1, %ymm6, %ymm9
	vpsraw	$10, %ymm9, %ymm9
	vpmullw	%ymm0, %ymm9, %ymm9
	vpsubw	%ymm9, %ymm6, %ymm6
	vmovsldup	%ymm7, %ymm9
	vpblendd	$-86, %ymm9, %ymm6, %ymm9
	vpsrlq	$32, %ymm6, %ymm6
	vpblendd	$-86, %ymm7, %ymm6, %ymm10
	vmovsldup	%ymm12, %ymm6
	vpblendd	$-86, %ymm6, %ymm8, %ymm11
	vpsrlq	$32, %ymm8, %ymm6
	vpblendd	$-86, %ymm12, %ymm6, %ymm14
	vmovsldup	%ymm3, %ymm6
	vpblendd	$-86, %ymm6, %ymm2, %ymm12
	vpsrlq	$32, %ymm2, %ymm2
	vpblendd	$-86, %ymm3, %ymm2, %ymm13
	vmovsldup	%ymm5, %ymm2
	vpblendd	$-86, %ymm2, %ymm4, %ymm2
	vpsrlq	$32, %ymm4, %ymm3
	vpblendd	$-86, %ymm5, %ymm3, %ymm3
	vmovdqu	648(%rsi), %ymm4
	vmovdqu	680(%rsi), %ymm5
	vpsubw	%ymm10, %ymm9, %ymm6
	vpsubw	%ymm14, %ymm11, %ymm7
	vpsubw	%ymm13, %ymm12, %ymm8
	vpaddw	%ymm9, %ymm10, %ymm9
	vpaddw	%ymm11, %ymm14, %ymm10
	vpmullw	%ymm6, %ymm4, %ymm11
	vpaddw	%ymm12, %ymm13, %ymm12
	vpmullw	%ymm7, %ymm4, %ymm13
	vpsubw	%ymm3, %ymm2, %ymm14
	vpaddw	%ymm2, %ymm3, %ymm2
	vpmullw	%ymm8, %ymm4, %ymm3
	vpmullw	%ymm14, %ymm4, %ymm4
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpmulhw	%ymm7, %ymm5, %ymm7
	vpmulhw	%ymm8, %ymm5, %ymm8
	vpmulhw	%ymm14, %ymm5, %ymm5
	vpmulhw	%ymm11, %ymm0, %ymm11
	vpmulhw	%ymm13, %ymm0, %ymm13
	vpmulhw	%ymm3, %ymm0, %ymm3
	vpmulhw	%ymm4, %ymm0, %ymm4
	vpsubw	%ymm11, %ymm6, %ymm6
	vpsubw	%ymm13, %ymm7, %ymm7
	vpsubw	%ymm3, %ymm8, %ymm3
	vpsubw	%ymm4, %ymm5, %ymm4
	vpmulhw	%ymm1, %ymm9, %ymm5
	vpsraw	$10, %ymm5, %ymm5
	vpmullw	%ymm0, %ymm5, %ymm5
	vpsubw	%ymm5, %ymm9, %ymm8
	vpunpcklqdq	%ymm10, %ymm8, %ymm5
	vpunpckhqdq	%ymm10, %ymm8, %ymm8
	vpunpcklqdq	%ymm2, %ymm12, %ymm9
	vpunpckhqdq	%ymm2, %ymm12, %ymm2
	vpunpcklqdq	%ymm7, %ymm6, %ymm10
	vpunpckhqdq	%ymm7, %ymm6, %ymm6
	vpunpcklqdq	%ymm4, %ymm3, %ymm7
	vpunpckhqdq	%ymm4, %ymm3, %ymm3
	vmovdqu	712(%rsi), %ymm4
	vmovdqu	744(%rsi), %ymm11
	vpsubw	%ymm8, %ymm5, %ymm12
	vpsubw	%ymm2, %ymm9, %ymm13
	vpsubw	%ymm6, %ymm10, %ymm14
	vpaddw	%ymm5, %ymm8, %ymm5
	vpaddw	%ymm9, %ymm2, %ymm2
	vpmullw	%ymm12, %ymm4, %ymm8
	vpaddw	%ymm10, %ymm6, %ymm6
	vpmullw	%ymm13, %ymm4, %ymm9
	vpsubw	%ymm3, %ymm7, %ymm10
	vpaddw	%ymm7, %ymm3, %ymm3
	vpmullw	%ymm14, %ymm4, %ymm7
	vpmullw	%ymm10, %ymm4, %ymm4
	vpmulhw	%ymm12, %ymm11, %ymm12
	vpmulhw	%ymm13, %ymm11, %ymm13
	vpmulhw	%ymm14, %ymm11, %ymm14
	vpmulhw	%ymm10, %ymm11, %ymm10
	vpmulhw	%ymm8, %ymm0, %ymm8
	vpmulhw	%ymm9, %ymm0, %ymm9
	vpmulhw	%ymm7, %ymm0, %ymm7
	vpmulhw	%ymm4, %ymm0, %ymm4
	vpsubw	%ymm8, %ymm12, %ymm8
	vpsubw	%ymm9, %ymm13, %ymm9
	vpsubw	%ymm7, %ymm14, %ymm7
	vpsubw	%ymm4, %ymm10, %ymm4
	vpmulhw	%ymm1, %ymm5, %ymm10
	vpsraw	$10, %ymm10, %ymm10
	vpmullw	%ymm0, %ymm10, %ymm10
	vpsubw	%ymm10, %ymm5, %ymm10
	vperm2i128	$32, %ymm2, %ymm10, %ymm5
	vperm2i128	$49, %ymm2, %ymm10, %ymm2
	vperm2i128	$32, %ymm3, %ymm6, %ymm10
	vperm2i128	$49, %ymm3, %ymm6, %ymm3
	vperm2i128	$32, %ymm9, %ymm8, %ymm6
	vperm2i128	$49, %ymm9, %ymm8, %ymm8
	vperm2i128	$32, %ymm4, %ymm7, %ymm9
	vperm2i128	$49, %ymm4, %ymm7, %ymm4
	vpbroadcastd	776(%rsi), %ymm7
	vpbroadcastd	780(%rsi), %ymm11
	vpsubw	%ymm2, %ymm5, %ymm12
	vpsubw	%ymm3, %ymm10, %ymm13
	vpsubw	%ymm8, %ymm6, %ymm14
	vpaddw	%ymm5, %ymm2, %ymm2
	vpaddw	%ymm10, %ymm3, %ymm3
	vpmullw	%ymm12, %ymm7, %ymm5
	vpaddw	%ymm6, %ymm8, %ymm6
	vpmullw	%ymm13, %ymm7, %ymm8
	vpsubw	%ymm4, %ymm9, %ymm10
	vpaddw	%ymm9, %ymm4, %ymm4
	vpmullw	%ymm14, %ymm7, %ymm9
	vpmullw	%ymm10, %ymm7, %ymm7
	vpmulhw	%ymm12, %ymm11, %ymm12
	vpmulhw	%ymm13, %ymm11, %ymm13
	vpmulhw	%ymm14, %ymm11, %ymm14
	vpmulhw	%ymm10, %ymm11, %ymm10
	vpmulhw	%ymm5, %ymm0, %ymm5
	vpmulhw	%ymm8, %ymm0, %ymm8
	vpmulhw	%ymm9, %ymm0, %ymm9
	vpmulhw	%ymm7, %ymm0, %ymm7
	vpsubw	%ymm5, %ymm12, %ymm5
	vpsubw	%ymm8, %ymm13, %ymm8
	vpsubw	%ymm9, %ymm14, %ymm9
	vpsubw	%ymm7, %ymm10, %ymm7
	vpmulhw	%ymm1, %ymm2, %ymm1
	vpsraw	$10, %ymm1, %ymm1
	vpmullw	%ymm0, %ymm1, %ymm1
	vpsubw	%ymm1, %ymm2, %ymm10
	vmovdqu	%ymm5, 384(%rcx)
	vmovdqu	%ymm8, 416(%rcx)
	vmovdqu	%ymm9, 448(%rcx)
	vmovdqu	%ymm7, 480(%rcx)
	vpbroadcastd	784(%rsi), %ymm1
	vpbroadcastd	788(%rsi), %ymm2
	vmovdqu	%ymm4, %ymm7
	vmovdqu	%ymm3, %ymm4
	vmovdqu	%ymm10, %ymm3
	vmovdqu	(%rcx), %ymm9
	vmovdqu	32(%rcx), %ymm13
	vmovdqu	64(%rcx), %ymm5
	vmovdqu	96(%rcx), %ymm8
	vpsubw	%ymm3, %ymm9, %ymm10
	vpsubw	%ymm4, %ymm13, %ymm11
	vpsubw	%ymm6, %ymm5, %ymm12
	vpaddw	%ymm9, %ymm3, %ymm3
	vpaddw	%ymm13, %ymm4, %ymm4
	vpmullw	%ymm10, %ymm1, %ymm13
	vpaddw	%ymm5, %ymm6, %ymm5
	vpmullw	%ymm11, %ymm1, %ymm6
	vpsubw	%ymm7, %ymm8, %ymm14
	vpaddw	%ymm8, %ymm7, %ymm7
	vpmullw	%ymm12, %ymm1, %ymm8
	vpmullw	%ymm14, %ymm1, %ymm9
	vpmulhw	%ymm10, %ymm2, %ymm10
	vpmulhw	%ymm11, %ymm2, %ymm11
	vpmulhw	%ymm12, %ymm2, %ymm12
	vpmulhw	%ymm14, %ymm2, %ymm14
	vpmulhw	%ymm13, %ymm0, %ymm13
	vpmulhw	%ymm6, %ymm0, %ymm6
	vpmulhw	%ymm8, %ymm0, %ymm8
	vpmulhw	%ymm9, %ymm0, %ymm9
	vpsubw	%ymm13, %ymm10, %ymm10
	vpsubw	%ymm6, %ymm11, %ymm6
	vpsubw	%ymm8, %ymm12, %ymm11
	vpsubw	%ymm9, %ymm14, %ymm12
	vmovdqu	glob_data + 320(%rip), %ymm8
	vmovdqu	glob_data + 352(%rip), %ymm9
	vmovdqu	%ymm10, 256(%rcx)
	vmovdqu	%ymm6, 288(%rcx)
	vmovdqu	%ymm11, 320(%rcx)
	vmovdqu	%ymm12, 352(%rcx)
	vpmullw	%ymm3, %ymm8, %ymm6
	vpmulhw	%ymm3, %ymm9, %ymm3
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm6, %ymm3, %ymm3
	vpmullw	%ymm4, %ymm8, %ymm6
	vpmulhw	%ymm4, %ymm9, %ymm4
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm6, %ymm4, %ymm4
	vpmullw	%ymm5, %ymm8, %ymm6
	vpmulhw	%ymm5, %ymm9, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm6, %ymm5, %ymm5
	vpmullw	%ymm7, %ymm8, %ymm6
	vpmulhw	%ymm7, %ymm9, %ymm7
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm6, %ymm7, %ymm6
	vmovdqu	%ymm3, (%rcx)
	vmovdqu	%ymm4, 32(%rcx)
	vmovdqu	%ymm5, 64(%rcx)
	vmovdqu	%ymm6, 96(%rcx)
	vmovdqu	384(%rcx), %ymm4
	vmovdqu	416(%rcx), %ymm5
	vmovdqu	448(%rcx), %ymm7
	vmovdqu	480(%rcx), %ymm10
	vmovdqu	128(%rcx), %ymm6
	vmovdqu	160(%rcx), %ymm9
	vmovdqu	192(%rcx), %ymm8
	vmovdqu	224(%rcx), %ymm11
	vpsubw	%ymm4, %ymm6, %ymm12
	vpsubw	%ymm5, %ymm9, %ymm13
	vpsubw	%ymm7, %ymm8, %ymm3
	vpaddw	%ymm6, %ymm4, %ymm4
	vpaddw	%ymm9, %ymm5, %ymm5
	vpmullw	%ymm12, %ymm1, %ymm6
	vpaddw	%ymm8, %ymm7, %ymm7
	vpmullw	%ymm13, %ymm1, %ymm8
	vpsubw	%ymm10, %ymm11, %ymm9
	vpaddw	%ymm11, %ymm10, %ymm10
	vpmullw	%ymm3, %ymm1, %ymm11
	vpmullw	%ymm9, %ymm1, %ymm1
	vpmulhw	%ymm12, %ymm2, %ymm12
	vpmulhw	%ymm13, %ymm2, %ymm13
	vpmulhw	%ymm3, %ymm2, %ymm3
	vpmulhw	%ymm9, %ymm2, %ymm2
	vpmulhw	%ymm6, %ymm0, %ymm6
	vpmulhw	%ymm8, %ymm0, %ymm8
	vpmulhw	%ymm11, %ymm0, %ymm9
	vpmulhw	%ymm1, %ymm0, %ymm11
	vpsubw	%ymm6, %ymm12, %ymm1
	vpsubw	%ymm8, %ymm13, %ymm6
	vpsubw	%ymm9, %ymm3, %ymm8
	vpsubw	%ymm11, %ymm2, %ymm9
	vmovdqu	glob_data + 320(%rip), %ymm2
	vmovdqu	glob_data + 352(%rip), %ymm3
	vmovdqu	%ymm1, 384(%rcx)
	vmovdqu	%ymm6, 416(%rcx)
	vmovdqu	%ymm8, 448(%rcx)
	vmovdqu	%ymm9, 480(%rcx)
	vpmullw	%ymm4, %ymm2, %ymm1
	vpmulhw	%ymm4, %ymm3, %ymm4
	vpmulhw	%ymm0, %ymm1, %ymm1
	vpsubw	%ymm1, %ymm4, %ymm1
	vpmullw	%ymm5, %ymm2, %ymm4
	vpmulhw	%ymm5, %ymm3, %ymm5
	vpmulhw	%ymm0, %ymm4, %ymm4
	vpsubw	%ymm4, %ymm5, %ymm4
	vpmullw	%ymm7, %ymm2, %ymm5
	vpmulhw	%ymm7, %ymm3, %ymm6
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpsubw	%ymm5, %ymm6, %ymm5
	vpmullw	%ymm10, %ymm2, %ymm2
	vpmulhw	%ymm10, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm2, %ymm0
	vpsubw	%ymm0, %ymm3, %ymm0
	vmovdqu	%ymm1, 128(%rcx)
	vmovdqu	%ymm4, 160(%rcx)
	vmovdqu	%ymm5, 192(%rcx)
	vmovdqu	%ymm0, 224(%rcx)
	ret
L_poly_getnoise_eta1_4x$1:
	movq	%rax, 1472(%rsp)
	vmovdqu	(%r10), %ymm0
	vmovdqu	%ymm0, 32(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm0, 352(%rsp)
	vmovdqu	%ymm0, 512(%rsp)
	movb	%cl, 64(%rsp)
	incb	%cl
	movb	%cl, 224(%rsp)
	incb	%cl
	movb	%cl, 384(%rsp)
	incb	%cl
	movb	%cl, 544(%rsp)
	leaq	672(%rsp), %rax
	leaq	32(%rsp), %rcx
	leaq	192(%rsp), %rdx
	leaq	352(%rsp), %rsi
	leaq	512(%rsp), %r11
	call	L_shake256_absorb4x_33$1
L_poly_getnoise_eta1_4x$3:
	leaq	672(%rsp), %rax
	leaq	32(%rsp), %rcx
	leaq	192(%rsp), %rdx
	leaq	352(%rsp), %rsi
	leaq	512(%rsp), %r11
	movq	%rcx, %rbx
	movq	%rdx, %rbp
	movq	%rsi, %r12
	leaq	-824(%rsp), %rsp
	call	L_keccakf1600_4x_avx2$1
L_poly_getnoise_eta1_4x$2:
	leaq	824(%rsp), %rsp
	vmovdqu	(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, (%rbx)
	vmovhpd	%xmm1, (%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, (%r12)
	vmovhpd	%xmm0, (%r11)
	vmovdqu	32(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 8(%rbx)
	vmovhpd	%xmm1, 8(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 8(%r12)
	vmovhpd	%xmm0, 8(%r11)
	vmovdqu	64(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 16(%rbx)
	vmovhpd	%xmm1, 16(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 16(%r12)
	vmovhpd	%xmm0, 16(%r11)
	vmovdqu	96(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 24(%rbx)
	vmovhpd	%xmm1, 24(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 24(%r12)
	vmovhpd	%xmm0, 24(%r11)
	vmovdqu	128(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 32(%rbx)
	vmovhpd	%xmm1, 32(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 32(%r12)
	vmovhpd	%xmm0, 32(%r11)
	vmovdqu	160(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 40(%rbx)
	vmovhpd	%xmm1, 40(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 40(%r12)
	vmovhpd	%xmm0, 40(%r11)
	vmovdqu	192(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 48(%rbx)
	vmovhpd	%xmm1, 48(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 48(%r12)
	vmovhpd	%xmm0, 48(%r11)
	vmovdqu	224(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 56(%rbx)
	vmovhpd	%xmm1, 56(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 56(%r12)
	vmovhpd	%xmm0, 56(%r11)
	vmovdqu	256(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 64(%rbx)
	vmovhpd	%xmm1, 64(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 64(%r12)
	vmovhpd	%xmm0, 64(%r11)
	vmovdqu	288(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 72(%rbx)
	vmovhpd	%xmm1, 72(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 72(%r12)
	vmovhpd	%xmm0, 72(%r11)
	vmovdqu	320(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 80(%rbx)
	vmovhpd	%xmm1, 80(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 80(%r12)
	vmovhpd	%xmm0, 80(%r11)
	vmovdqu	352(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 88(%rbx)
	vmovhpd	%xmm1, 88(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 88(%r12)
	vmovhpd	%xmm0, 88(%r11)
	vmovdqu	384(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 96(%rbx)
	vmovhpd	%xmm1, 96(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 96(%r12)
	vmovhpd	%xmm0, 96(%r11)
	vmovdqu	416(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 104(%rbx)
	vmovhpd	%xmm1, 104(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 104(%r12)
	vmovhpd	%xmm0, 104(%r11)
	vmovdqu	448(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 112(%rbx)
	vmovhpd	%xmm1, 112(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 112(%r12)
	vmovhpd	%xmm0, 112(%r11)
	vmovdqu	480(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 120(%rbx)
	vmovhpd	%xmm1, 120(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 120(%r12)
	vmovhpd	%xmm0, 120(%r11)
	vmovdqu	512(%rax), %ymm0
	vmovdqu	%xmm0, %xmm1
	vmovlpd	%xmm1, 128(%rbx)
	vmovhpd	%xmm1, 128(%rbp)
	vextracti128	$1, %ymm0, %xmm0
	vmovlpd	%xmm0, 128(%r12)
	vmovhpd	%xmm0, 128(%r11)
	movq	1472(%rsp), %rax
	leaq	32(%rsp), %rcx
	movl	$1431655765, 1480(%rsp)
	movl	$858993459, 1484(%rsp)
	movl	$50529027, 1488(%rsp)
	movl	$252645135, 1492(%rsp)
	vpbroadcastd	1480(%rsp), %ymm0
	vpbroadcastd	1484(%rsp), %ymm1
	vpbroadcastd	1488(%rsp), %ymm2
	vpbroadcastd	1492(%rsp), %ymm3
	vmovdqu	(%rcx), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, (%rax)
	vmovdqu	%ymm7, 32(%rax)
	vmovdqu	%ymm6, 64(%rax)
	vmovdqu	%ymm4, 96(%rax)
	vmovdqu	32(%rcx), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, 128(%rax)
	vmovdqu	%ymm7, 160(%rax)
	vmovdqu	%ymm6, 192(%rax)
	vmovdqu	%ymm4, 224(%rax)
	vmovdqu	64(%rcx), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, 256(%rax)
	vmovdqu	%ymm7, 288(%rax)
	vmovdqu	%ymm6, 320(%rax)
	vmovdqu	%ymm4, 352(%rax)
	vmovdqu	96(%rcx), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm0
	vpaddb	%ymm0, %ymm4, %ymm0
	vpsrlw	$2, %ymm0, %ymm4
	vpand	%ymm0, %ymm1, %ymm0
	vpand	%ymm4, %ymm1, %ymm4
	vpaddb	%ymm1, %ymm0, %ymm0
	vpsubb	%ymm4, %ymm0, %ymm0
	vpsrlw	$4, %ymm0, %ymm1
	vpand	%ymm0, %ymm3, %ymm0
	vpand	%ymm1, %ymm3, %ymm1
	vpsubb	%ymm2, %ymm0, %ymm0
	vpsubb	%ymm2, %ymm1, %ymm1
	vpunpcklbw	%ymm1, %ymm0, %ymm2
	vpunpckhbw	%ymm1, %ymm0, %ymm0
	vmovdqu	%xmm2, %xmm1
	vpmovsxbw	%xmm1, %ymm1
	vextracti128	$1, %ymm2, %xmm2
	vpmovsxbw	%xmm2, %ymm2
	vmovdqu	%xmm0, %xmm3
	vpmovsxbw	%xmm3, %ymm3
	vextracti128	$1, %ymm0, %xmm0
	vpmovsxbw	%xmm0, %ymm0
	vmovdqu	%ymm1, 384(%rax)
	vmovdqu	%ymm3, 416(%rax)
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	%ymm0, 480(%rax)
	leaq	192(%rsp), %rax
	movl	$1431655765, 1492(%rsp)
	movl	$858993459, 1488(%rsp)
	movl	$50529027, 1484(%rsp)
	movl	$252645135, 1480(%rsp)
	vpbroadcastd	1492(%rsp), %ymm0
	vpbroadcastd	1488(%rsp), %ymm1
	vpbroadcastd	1484(%rsp), %ymm2
	vpbroadcastd	1480(%rsp), %ymm3
	vmovdqu	(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, (%rdi)
	vmovdqu	%ymm7, 32(%rdi)
	vmovdqu	%ymm6, 64(%rdi)
	vmovdqu	%ymm4, 96(%rdi)
	vmovdqu	32(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, 128(%rdi)
	vmovdqu	%ymm7, 160(%rdi)
	vmovdqu	%ymm6, 192(%rdi)
	vmovdqu	%ymm4, 224(%rdi)
	vmovdqu	64(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, 256(%rdi)
	vmovdqu	%ymm7, 288(%rdi)
	vmovdqu	%ymm6, 320(%rdi)
	vmovdqu	%ymm4, 352(%rdi)
	vmovdqu	96(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm0
	vpaddb	%ymm0, %ymm4, %ymm0
	vpsrlw	$2, %ymm0, %ymm4
	vpand	%ymm0, %ymm1, %ymm0
	vpand	%ymm4, %ymm1, %ymm4
	vpaddb	%ymm1, %ymm0, %ymm0
	vpsubb	%ymm4, %ymm0, %ymm0
	vpsrlw	$4, %ymm0, %ymm1
	vpand	%ymm0, %ymm3, %ymm0
	vpand	%ymm1, %ymm3, %ymm1
	vpsubb	%ymm2, %ymm0, %ymm0
	vpsubb	%ymm2, %ymm1, %ymm1
	vpunpcklbw	%ymm1, %ymm0, %ymm2
	vpunpckhbw	%ymm1, %ymm0, %ymm0
	vmovdqu	%xmm2, %xmm1
	vpmovsxbw	%xmm1, %ymm1
	vextracti128	$1, %ymm2, %xmm2
	vpmovsxbw	%xmm2, %ymm2
	vmovdqu	%xmm0, %xmm3
	vpmovsxbw	%xmm3, %ymm3
	vextracti128	$1, %ymm0, %xmm0
	vpmovsxbw	%xmm0, %ymm0
	vmovdqu	%ymm1, 384(%rdi)
	vmovdqu	%ymm3, 416(%rdi)
	vmovdqu	%ymm2, 448(%rdi)
	vmovdqu	%ymm0, 480(%rdi)
	leaq	352(%rsp), %rax
	movl	$1431655765, 1480(%rsp)
	movl	$858993459, 1484(%rsp)
	movl	$50529027, 1488(%rsp)
	movl	$252645135, 1492(%rsp)
	vpbroadcastd	1480(%rsp), %ymm0
	vpbroadcastd	1484(%rsp), %ymm1
	vpbroadcastd	1488(%rsp), %ymm2
	vpbroadcastd	1492(%rsp), %ymm3
	vmovdqu	(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, (%r8)
	vmovdqu	%ymm7, 32(%r8)
	vmovdqu	%ymm6, 64(%r8)
	vmovdqu	%ymm4, 96(%r8)
	vmovdqu	32(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, 128(%r8)
	vmovdqu	%ymm7, 160(%r8)
	vmovdqu	%ymm6, 192(%r8)
	vmovdqu	%ymm4, 224(%r8)
	vmovdqu	64(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, 256(%r8)
	vmovdqu	%ymm7, 288(%r8)
	vmovdqu	%ymm6, 320(%r8)
	vmovdqu	%ymm4, 352(%r8)
	vmovdqu	96(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm0
	vpaddb	%ymm0, %ymm4, %ymm0
	vpsrlw	$2, %ymm0, %ymm4
	vpand	%ymm0, %ymm1, %ymm0
	vpand	%ymm4, %ymm1, %ymm4
	vpaddb	%ymm1, %ymm0, %ymm0
	vpsubb	%ymm4, %ymm0, %ymm0
	vpsrlw	$4, %ymm0, %ymm1
	vpand	%ymm0, %ymm3, %ymm0
	vpand	%ymm1, %ymm3, %ymm1
	vpsubb	%ymm2, %ymm0, %ymm0
	vpsubb	%ymm2, %ymm1, %ymm1
	vpunpcklbw	%ymm1, %ymm0, %ymm2
	vpunpckhbw	%ymm1, %ymm0, %ymm0
	vmovdqu	%xmm2, %xmm1
	vpmovsxbw	%xmm1, %ymm1
	vextracti128	$1, %ymm2, %xmm2
	vpmovsxbw	%xmm2, %ymm2
	vmovdqu	%xmm0, %xmm3
	vpmovsxbw	%xmm3, %ymm3
	vextracti128	$1, %ymm0, %xmm0
	vpmovsxbw	%xmm0, %ymm0
	vmovdqu	%ymm1, 384(%r8)
	vmovdqu	%ymm3, 416(%r8)
	vmovdqu	%ymm2, 448(%r8)
	vmovdqu	%ymm0, 480(%r8)
	leaq	512(%rsp), %rax
	movl	$1431655765, 1492(%rsp)
	movl	$858993459, 1488(%rsp)
	movl	$50529027, 1484(%rsp)
	movl	$252645135, 1480(%rsp)
	vpbroadcastd	1492(%rsp), %ymm0
	vpbroadcastd	1488(%rsp), %ymm1
	vpbroadcastd	1484(%rsp), %ymm2
	vpbroadcastd	1480(%rsp), %ymm3
	vmovdqu	(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, (%r9)
	vmovdqu	%ymm7, 32(%r9)
	vmovdqu	%ymm6, 64(%r9)
	vmovdqu	%ymm4, 96(%r9)
	vmovdqu	32(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, 128(%r9)
	vmovdqu	%ymm7, 160(%r9)
	vmovdqu	%ymm6, 192(%r9)
	vmovdqu	%ymm4, 224(%r9)
	vmovdqu	64(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm5
	vpaddb	%ymm5, %ymm4, %ymm4
	vpsrlw	$2, %ymm4, %ymm5
	vpand	%ymm4, %ymm1, %ymm4
	vpand	%ymm5, %ymm1, %ymm5
	vpaddb	%ymm1, %ymm4, %ymm4
	vpsubb	%ymm5, %ymm4, %ymm4
	vpsrlw	$4, %ymm4, %ymm5
	vpand	%ymm4, %ymm3, %ymm4
	vpand	%ymm5, %ymm3, %ymm5
	vpsubb	%ymm2, %ymm4, %ymm4
	vpsubb	%ymm2, %ymm5, %ymm5
	vpunpcklbw	%ymm5, %ymm4, %ymm6
	vpunpckhbw	%ymm5, %ymm4, %ymm4
	vmovdqu	%xmm6, %xmm5
	vpmovsxbw	%xmm5, %ymm5
	vextracti128	$1, %ymm6, %xmm6
	vpmovsxbw	%xmm6, %ymm6
	vmovdqu	%xmm4, %xmm7
	vpmovsxbw	%xmm7, %ymm7
	vextracti128	$1, %ymm4, %xmm4
	vpmovsxbw	%xmm4, %ymm4
	vmovdqu	%ymm5, 256(%r9)
	vmovdqu	%ymm7, 288(%r9)
	vmovdqu	%ymm6, 320(%r9)
	vmovdqu	%ymm4, 352(%r9)
	vmovdqu	96(%rax), %ymm4
	vpsrlw	$1, %ymm4, %ymm5
	vpand	%ymm4, %ymm0, %ymm4
	vpand	%ymm5, %ymm0, %ymm0
	vpaddb	%ymm0, %ymm4, %ymm0
	vpsrlw	$2, %ymm0, %ymm4
	vpand	%ymm0, %ymm1, %ymm0
	vpand	%ymm4, %ymm1, %ymm4
	vpaddb	%ymm1, %ymm0, %ymm0
	vpsubb	%ymm4, %ymm0, %ymm0
	vpsrlw	$4, %ymm0, %ymm1
	vpand	%ymm0, %ymm3, %ymm0
	vpand	%ymm1, %ymm3, %ymm1
	vpsubb	%ymm2, %ymm0, %ymm0
	vpsubb	%ymm2, %ymm1, %ymm1
	vpunpcklbw	%ymm1, %ymm0, %ymm2
	vpunpckhbw	%ymm1, %ymm0, %ymm0
	vmovdqu	%xmm2, %xmm1
	vpmovsxbw	%xmm1, %ymm1
	vextracti128	$1, %ymm2, %xmm2
	vpmovsxbw	%xmm2, %ymm2
	vmovdqu	%xmm0, %xmm3
	vpmovsxbw	%xmm3, %ymm3
	vextracti128	$1, %ymm0, %xmm0
	vpmovsxbw	%xmm0, %ymm0
	vmovdqu	%ymm1, 384(%r9)
	vmovdqu	%ymm3, 416(%r9)
	vmovdqu	%ymm2, 448(%r9)
	vmovdqu	%ymm0, 480(%r9)
	ret
L_poly_frommsg_1$1:
	leaq	glob_data + 256(%rip), %rsi
	vmovdqu	(%rsi), %ymm0
	vbroadcasti128	glob_data + 4112(%rip), %ymm1
	vbroadcasti128	glob_data + 4096(%rip), %ymm2
	vmovdqu	(%rdi), %ymm3
	vpshufd	$0, %ymm3, %ymm4
	vpsllvd	%ymm1, %ymm4, %ymm4
	vpshufb	%ymm2, %ymm4, %ymm4
	vpsllw	$12, %ymm4, %ymm5
	vpsllw	$8, %ymm4, %ymm6
	vpsllw	$4, %ymm4, %ymm7
	vpsraw	$15, %ymm5, %ymm5
	vpsraw	$15, %ymm6, %ymm6
	vpsraw	$15, %ymm7, %ymm7
	vpsraw	$15, %ymm4, %ymm4
	vpand	%ymm0, %ymm5, %ymm5
	vpand	%ymm0, %ymm6, %ymm6
	vpand	%ymm0, %ymm7, %ymm7
	vpand	%ymm0, %ymm4, %ymm4
	vpunpcklqdq	%ymm6, %ymm5, %ymm8
	vpunpckhqdq	%ymm6, %ymm5, %ymm5
	vpunpcklqdq	%ymm4, %ymm7, %ymm6
	vpunpckhqdq	%ymm4, %ymm7, %ymm4
	vperm2i128	$32, %ymm6, %ymm8, %ymm7
	vperm2i128	$49, %ymm6, %ymm8, %ymm6
	vperm2i128	$32, %ymm4, %ymm5, %ymm8
	vperm2i128	$49, %ymm4, %ymm5, %ymm4
	vmovdqu	%ymm7, (%rax)
	vmovdqu	%ymm8, 32(%rax)
	vmovdqu	%ymm6, 256(%rax)
	vmovdqu	%ymm4, 288(%rax)
	vpshufd	$85, %ymm3, %ymm4
	vpsllvd	%ymm1, %ymm4, %ymm4
	vpshufb	%ymm2, %ymm4, %ymm4
	vpsllw	$12, %ymm4, %ymm5
	vpsllw	$8, %ymm4, %ymm6
	vpsllw	$4, %ymm4, %ymm7
	vpsraw	$15, %ymm5, %ymm5
	vpsraw	$15, %ymm6, %ymm6
	vpsraw	$15, %ymm7, %ymm7
	vpsraw	$15, %ymm4, %ymm4
	vpand	%ymm0, %ymm5, %ymm5
	vpand	%ymm0, %ymm6, %ymm6
	vpand	%ymm0, %ymm7, %ymm7
	vpand	%ymm0, %ymm4, %ymm4
	vpunpcklqdq	%ymm6, %ymm5, %ymm8
	vpunpckhqdq	%ymm6, %ymm5, %ymm5
	vpunpcklqdq	%ymm4, %ymm7, %ymm6
	vpunpckhqdq	%ymm4, %ymm7, %ymm4
	vperm2i128	$32, %ymm6, %ymm8, %ymm7
	vperm2i128	$49, %ymm6, %ymm8, %ymm6
	vperm2i128	$32, %ymm4, %ymm5, %ymm8
	vperm2i128	$49, %ymm4, %ymm5, %ymm4
	vmovdqu	%ymm7, 64(%rax)
	vmovdqu	%ymm8, 96(%rax)
	vmovdqu	%ymm6, 320(%rax)
	vmovdqu	%ymm4, 352(%rax)
	vpshufd	$-86, %ymm3, %ymm4
	vpsllvd	%ymm1, %ymm4, %ymm4
	vpshufb	%ymm2, %ymm4, %ymm4
	vpsllw	$12, %ymm4, %ymm5
	vpsllw	$8, %ymm4, %ymm6
	vpsllw	$4, %ymm4, %ymm7
	vpsraw	$15, %ymm5, %ymm5
	vpsraw	$15, %ymm6, %ymm6
	vpsraw	$15, %ymm7, %ymm7
	vpsraw	$15, %ymm4, %ymm4
	vpand	%ymm0, %ymm5, %ymm5
	vpand	%ymm0, %ymm6, %ymm6
	vpand	%ymm0, %ymm7, %ymm7
	vpand	%ymm0, %ymm4, %ymm4
	vpunpcklqdq	%ymm6, %ymm5, %ymm8
	vpunpckhqdq	%ymm6, %ymm5, %ymm5
	vpunpcklqdq	%ymm4, %ymm7, %ymm6
	vpunpckhqdq	%ymm4, %ymm7, %ymm4
	vperm2i128	$32, %ymm6, %ymm8, %ymm7
	vperm2i128	$49, %ymm6, %ymm8, %ymm6
	vperm2i128	$32, %ymm4, %ymm5, %ymm8
	vperm2i128	$49, %ymm4, %ymm5, %ymm4
	vmovdqu	%ymm7, 128(%rax)
	vmovdqu	%ymm8, 160(%rax)
	vmovdqu	%ymm6, 384(%rax)
	vmovdqu	%ymm4, 416(%rax)
	vpshufd	$-1, %ymm3, %ymm3
	vpsllvd	%ymm1, %ymm3, %ymm1
	vpshufb	%ymm2, %ymm1, %ymm1
	vpsllw	$12, %ymm1, %ymm2
	vpsllw	$8, %ymm1, %ymm3
	vpsllw	$4, %ymm1, %ymm4
	vpsraw	$15, %ymm2, %ymm2
	vpsraw	$15, %ymm3, %ymm3
	vpsraw	$15, %ymm4, %ymm4
	vpsraw	$15, %ymm1, %ymm1
	vpand	%ymm0, %ymm2, %ymm2
	vpand	%ymm0, %ymm3, %ymm3
	vpand	%ymm0, %ymm4, %ymm4
	vpand	%ymm0, %ymm1, %ymm0
	vpunpcklqdq	%ymm3, %ymm2, %ymm1
	vpunpckhqdq	%ymm3, %ymm2, %ymm2
	vpunpcklqdq	%ymm0, %ymm4, %ymm3
	vpunpckhqdq	%ymm0, %ymm4, %ymm0
	vperm2i128	$32, %ymm3, %ymm1, %ymm4
	vperm2i128	$49, %ymm3, %ymm1, %ymm1
	vperm2i128	$32, %ymm0, %ymm2, %ymm3
	vperm2i128	$49, %ymm0, %ymm2, %ymm0
	vmovdqu	%ymm4, 192(%rax)
	vmovdqu	%ymm3, 224(%rax)
	vmovdqu	%ymm1, 448(%rax)
	vmovdqu	%ymm0, 480(%rax)
	ret
L_poly_frommont$1:
	leaq	glob_data + 448(%rip), %rcx
	vmovdqu	(%rcx), %ymm0
	leaq	glob_data + 416(%rip), %rcx
	vmovdqu	(%rcx), %ymm1
	leaq	glob_data + 160(%rip), %rcx
	vmovdqu	(%rcx), %ymm2
	vmovdqu	(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, (%rax)
	vmovdqu	32(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 32(%rax)
	vmovdqu	64(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 64(%rax)
	vmovdqu	96(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 96(%rax)
	vmovdqu	128(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 128(%rax)
	vmovdqu	160(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 160(%rax)
	vmovdqu	192(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 192(%rax)
	vmovdqu	224(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 224(%rax)
	vmovdqu	256(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 256(%rax)
	vmovdqu	288(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 288(%rax)
	vmovdqu	320(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 320(%rax)
	vmovdqu	352(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 352(%rax)
	vmovdqu	384(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 384(%rax)
	vmovdqu	416(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 416(%rax)
	vmovdqu	448(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm3
	vpmullw	%ymm1, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm4, %ymm3
	vmovdqu	%ymm3, 448(%rax)
	vmovdqu	480(%rax), %ymm3
	vpmulhw	%ymm2, %ymm3, %ymm4
	vpmullw	%ymm2, %ymm3, %ymm2
	vpmullw	%ymm1, %ymm2, %ymm1
	vpmulhw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm4, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	ret
L_poly_frombytes$1:
	leaq	glob_data + 288(%rip), %r9
	vmovdqu	(%r9), %ymm0
	vmovdqu	(%rsi), %ymm1
	vmovdqu	32(%rsi), %ymm2
	vmovdqu	64(%rsi), %ymm3
	vmovdqu	96(%rsi), %ymm4
	vmovdqu	128(%rsi), %ymm5
	vmovdqu	160(%rsi), %ymm6
	vperm2i128	$32, %ymm4, %ymm1, %ymm7
	vperm2i128	$49, %ymm4, %ymm1, %ymm4
	vperm2i128	$32, %ymm5, %ymm2, %ymm8
	vperm2i128	$49, %ymm5, %ymm2, %ymm1
	vperm2i128	$32, %ymm6, %ymm3, %ymm5
	vperm2i128	$49, %ymm6, %ymm3, %ymm3
	vpunpcklqdq	%ymm1, %ymm7, %ymm6
	vpunpckhqdq	%ymm1, %ymm7, %ymm1
	vpunpcklqdq	%ymm5, %ymm4, %ymm2
	vpunpckhqdq	%ymm5, %ymm4, %ymm5
	vpunpcklqdq	%ymm3, %ymm8, %ymm7
	vpunpckhqdq	%ymm3, %ymm8, %ymm3
	vmovsldup	%ymm5, %ymm4
	vpblendd	$-86, %ymm4, %ymm6, %ymm4
	vpsrlq	$32, %ymm6, %ymm6
	vpblendd	$-86, %ymm5, %ymm6, %ymm5
	vmovsldup	%ymm7, %ymm6
	vpblendd	$-86, %ymm6, %ymm1, %ymm6
	vpsrlq	$32, %ymm1, %ymm1
	vpblendd	$-86, %ymm7, %ymm1, %ymm7
	vmovsldup	%ymm3, %ymm1
	vpblendd	$-86, %ymm1, %ymm2, %ymm1
	vpsrlq	$32, %ymm2, %ymm2
	vpblendd	$-86, %ymm3, %ymm2, %ymm2
	vpslld	$16, %ymm7, %ymm3
	vpblendw	$-86, %ymm3, %ymm4, %ymm3
	vpsrld	$16, %ymm4, %ymm4
	vpblendw	$-86, %ymm7, %ymm4, %ymm4
	vpslld	$16, %ymm1, %ymm7
	vpblendw	$-86, %ymm7, %ymm5, %ymm7
	vpsrld	$16, %ymm5, %ymm5
	vpblendw	$-86, %ymm1, %ymm5, %ymm1
	vpslld	$16, %ymm2, %ymm5
	vpblendw	$-86, %ymm5, %ymm6, %ymm5
	vpsrld	$16, %ymm6, %ymm6
	vpblendw	$-86, %ymm2, %ymm6, %ymm2
	vpsrlw	$12, %ymm3, %ymm6
	vpsllw	$4, %ymm4, %ymm8
	vpor	%ymm8, %ymm6, %ymm6
	vpand	%ymm3, %ymm0, %ymm3
	vpand	%ymm6, %ymm0, %ymm6
	vpsrlw	$8, %ymm4, %ymm4
	vpsllw	$8, %ymm7, %ymm8
	vpor	%ymm8, %ymm4, %ymm4
	vpand	%ymm4, %ymm0, %ymm4
	vpsrlw	$4, %ymm7, %ymm7
	vpand	%ymm7, %ymm0, %ymm7
	vpsrlw	$12, %ymm1, %ymm8
	vpsllw	$4, %ymm5, %ymm9
	vpor	%ymm9, %ymm8, %ymm8
	vpand	%ymm1, %ymm0, %ymm1
	vpand	%ymm8, %ymm0, %ymm8
	vpsrlw	$8, %ymm5, %ymm5
	vpsllw	$8, %ymm2, %ymm9
	vpor	%ymm9, %ymm5, %ymm5
	vpand	%ymm5, %ymm0, %ymm5
	vpsrlw	$4, %ymm2, %ymm2
	vpand	%ymm2, %ymm0, %ymm2
	vmovdqu	%ymm3, (%r8)
	vmovdqu	%ymm6, 32(%r8)
	vmovdqu	%ymm4, 64(%r8)
	vmovdqu	%ymm7, 96(%r8)
	vmovdqu	%ymm1, 128(%r8)
	vmovdqu	%ymm8, 160(%r8)
	vmovdqu	%ymm5, 192(%r8)
	vmovdqu	%ymm2, 224(%r8)
	vmovdqu	192(%rsi), %ymm1
	vmovdqu	224(%rsi), %ymm2
	vmovdqu	256(%rsi), %ymm3
	vmovdqu	288(%rsi), %ymm4
	vmovdqu	320(%rsi), %ymm5
	vmovdqu	352(%rsi), %ymm6
	vperm2i128	$32, %ymm4, %ymm1, %ymm7
	vperm2i128	$49, %ymm4, %ymm1, %ymm4
	vperm2i128	$32, %ymm5, %ymm2, %ymm8
	vperm2i128	$49, %ymm5, %ymm2, %ymm1
	vperm2i128	$32, %ymm6, %ymm3, %ymm5
	vperm2i128	$49, %ymm6, %ymm3, %ymm3
	vpunpcklqdq	%ymm1, %ymm7, %ymm6
	vpunpckhqdq	%ymm1, %ymm7, %ymm1
	vpunpcklqdq	%ymm5, %ymm4, %ymm2
	vpunpckhqdq	%ymm5, %ymm4, %ymm5
	vpunpcklqdq	%ymm3, %ymm8, %ymm7
	vpunpckhqdq	%ymm3, %ymm8, %ymm3
	vmovsldup	%ymm5, %ymm4
	vpblendd	$-86, %ymm4, %ymm6, %ymm4
	vpsrlq	$32, %ymm6, %ymm6
	vpblendd	$-86, %ymm5, %ymm6, %ymm5
	vmovsldup	%ymm7, %ymm6
	vpblendd	$-86, %ymm6, %ymm1, %ymm6
	vpsrlq	$32, %ymm1, %ymm1
	vpblendd	$-86, %ymm7, %ymm1, %ymm7
	vmovsldup	%ymm3, %ymm1
	vpblendd	$-86, %ymm1, %ymm2, %ymm1
	vpsrlq	$32, %ymm2, %ymm2
	vpblendd	$-86, %ymm3, %ymm2, %ymm2
	vpslld	$16, %ymm7, %ymm3
	vpblendw	$-86, %ymm3, %ymm4, %ymm3
	vpsrld	$16, %ymm4, %ymm4
	vpblendw	$-86, %ymm7, %ymm4, %ymm4
	vpslld	$16, %ymm1, %ymm7
	vpblendw	$-86, %ymm7, %ymm5, %ymm7
	vpsrld	$16, %ymm5, %ymm5
	vpblendw	$-86, %ymm1, %ymm5, %ymm1
	vpslld	$16, %ymm2, %ymm5
	vpblendw	$-86, %ymm5, %ymm6, %ymm5
	vpsrld	$16, %ymm6, %ymm6
	vpblendw	$-86, %ymm2, %ymm6, %ymm2
	vpsrlw	$12, %ymm3, %ymm6
	vpsllw	$4, %ymm4, %ymm8
	vpor	%ymm8, %ymm6, %ymm6
	vpand	%ymm3, %ymm0, %ymm3
	vpand	%ymm6, %ymm0, %ymm6
	vpsrlw	$8, %ymm4, %ymm4
	vpsllw	$8, %ymm7, %ymm8
	vpor	%ymm8, %ymm4, %ymm4
	vpand	%ymm4, %ymm0, %ymm4
	vpsrlw	$4, %ymm7, %ymm7
	vpand	%ymm7, %ymm0, %ymm7
	vpsrlw	$12, %ymm1, %ymm8
	vpsllw	$4, %ymm5, %ymm9
	vpor	%ymm9, %ymm8, %ymm8
	vpand	%ymm1, %ymm0, %ymm1
	vpand	%ymm8, %ymm0, %ymm8
	vpsrlw	$8, %ymm5, %ymm5
	vpsllw	$8, %ymm2, %ymm9
	vpor	%ymm9, %ymm5, %ymm5
	vpand	%ymm5, %ymm0, %ymm5
	vpsrlw	$4, %ymm2, %ymm2
	vpand	%ymm2, %ymm0, %ymm0
	vmovdqu	%ymm3, 256(%r8)
	vmovdqu	%ymm6, 288(%r8)
	vmovdqu	%ymm4, 320(%r8)
	vmovdqu	%ymm7, 352(%r8)
	vmovdqu	%ymm1, 384(%r8)
	vmovdqu	%ymm8, 416(%r8)
	vmovdqu	%ymm5, 448(%r8)
	vmovdqu	%ymm0, 480(%r8)
	ret
L_poly_decompress$1:
	leaq	glob_data + 448(%rip), %rdi
	vmovdqu	(%rdi), %ymm0
	leaq	glob_data + 96(%rip), %rdi
	vmovdqu	(%rdi), %ymm1
	vpbroadcastd	glob_data + 6412(%rip), %ymm2
	vpbroadcastd	glob_data + 6408(%rip), %ymm3
	vpxor	%ymm4, %ymm4, %ymm4
	movq	(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, (%rcx)
	movq	8(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 32(%rcx)
	movq	16(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 64(%rcx)
	movq	24(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 96(%rcx)
	movq	32(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 128(%rcx)
	movq	40(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 160(%rcx)
	movq	48(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 192(%rcx)
	movq	56(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 224(%rcx)
	movq	64(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 256(%rcx)
	movq	72(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 288(%rcx)
	movq	80(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 320(%rcx)
	movq	88(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 352(%rcx)
	movq	96(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 384(%rcx)
	movq	104(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 416(%rcx)
	movq	112(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm4
	vpand	%ymm2, %ymm4, %ymm4
	vpmullw	%ymm3, %ymm4, %ymm4
	vpmulhrsw	%ymm0, %ymm4, %ymm4
	vmovdqu	%ymm4, 448(%rcx)
	movq	120(%rsi), %xmm4
	vmovdqu	%xmm4, 16(%rsp)
	vbroadcasti128	16(%rsp), %ymm4
	vpshufb	%ymm1, %ymm4, %ymm1
	vpand	%ymm2, %ymm1, %ymm1
	vpmullw	%ymm3, %ymm1, %ymm1
	vpmulhrsw	%ymm0, %ymm1, %ymm0
	vmovdqu	%ymm0, 480(%rcx)
	ret
L_poly_compress_1$1:
	call	L_poly_csubq$1
L_poly_compress_1$2:
	leaq	glob_data + 384(%rip), %rdx
	vmovdqu	(%rdx), %ymm0
	vpbroadcastw	glob_data + 6428(%rip), %ymm1
	vpbroadcastw	glob_data + 6426(%rip), %ymm2
	vpbroadcastw	glob_data + 6424(%rip), %ymm3
	vmovdqu	glob_data + 128(%rip), %ymm4
	vmovdqu	(%rcx), %ymm5
	vmovdqu	32(%rcx), %ymm6
	vmovdqu	64(%rcx), %ymm7
	vmovdqu	96(%rcx), %ymm8
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpmulhw	%ymm0, %ymm7, %ymm7
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpmulhrsw	%ymm1, %ymm5, %ymm5
	vpmulhrsw	%ymm1, %ymm6, %ymm6
	vpmulhrsw	%ymm1, %ymm7, %ymm7
	vpmulhrsw	%ymm1, %ymm8, %ymm8
	vpand	%ymm2, %ymm5, %ymm5
	vpand	%ymm2, %ymm6, %ymm6
	vpand	%ymm2, %ymm7, %ymm7
	vpand	%ymm2, %ymm8, %ymm8
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpackuswb	%ymm8, %ymm7, %ymm6
	vpmaddubsw	%ymm3, %ymm5, %ymm5
	vpmaddubsw	%ymm3, %ymm6, %ymm6
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpermd	%ymm5, %ymm4, %ymm5
	vmovdqu	%ymm5, (%rax)
	vmovdqu	128(%rcx), %ymm5
	vmovdqu	160(%rcx), %ymm6
	vmovdqu	192(%rcx), %ymm7
	vmovdqu	224(%rcx), %ymm8
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpmulhw	%ymm0, %ymm7, %ymm7
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpmulhrsw	%ymm1, %ymm5, %ymm5
	vpmulhrsw	%ymm1, %ymm6, %ymm6
	vpmulhrsw	%ymm1, %ymm7, %ymm7
	vpmulhrsw	%ymm1, %ymm8, %ymm8
	vpand	%ymm2, %ymm5, %ymm5
	vpand	%ymm2, %ymm6, %ymm6
	vpand	%ymm2, %ymm7, %ymm7
	vpand	%ymm2, %ymm8, %ymm8
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpackuswb	%ymm8, %ymm7, %ymm6
	vpmaddubsw	%ymm3, %ymm5, %ymm5
	vpmaddubsw	%ymm3, %ymm6, %ymm6
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpermd	%ymm5, %ymm4, %ymm5
	vmovdqu	%ymm5, 32(%rax)
	vmovdqu	256(%rcx), %ymm5
	vmovdqu	288(%rcx), %ymm6
	vmovdqu	320(%rcx), %ymm7
	vmovdqu	352(%rcx), %ymm8
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpmulhw	%ymm0, %ymm7, %ymm7
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpmulhrsw	%ymm1, %ymm5, %ymm5
	vpmulhrsw	%ymm1, %ymm6, %ymm6
	vpmulhrsw	%ymm1, %ymm7, %ymm7
	vpmulhrsw	%ymm1, %ymm8, %ymm8
	vpand	%ymm2, %ymm5, %ymm5
	vpand	%ymm2, %ymm6, %ymm6
	vpand	%ymm2, %ymm7, %ymm7
	vpand	%ymm2, %ymm8, %ymm8
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpackuswb	%ymm8, %ymm7, %ymm6
	vpmaddubsw	%ymm3, %ymm5, %ymm5
	vpmaddubsw	%ymm3, %ymm6, %ymm6
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpermd	%ymm5, %ymm4, %ymm5
	vmovdqu	%ymm5, 64(%rax)
	vmovdqu	384(%rcx), %ymm5
	vmovdqu	416(%rcx), %ymm6
	vmovdqu	448(%rcx), %ymm7
	vmovdqu	480(%rcx), %ymm8
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpmulhw	%ymm0, %ymm7, %ymm7
	vpmulhw	%ymm0, %ymm8, %ymm0
	vpmulhrsw	%ymm1, %ymm5, %ymm5
	vpmulhrsw	%ymm1, %ymm6, %ymm6
	vpmulhrsw	%ymm1, %ymm7, %ymm7
	vpmulhrsw	%ymm1, %ymm0, %ymm0
	vpand	%ymm2, %ymm5, %ymm1
	vpand	%ymm2, %ymm6, %ymm5
	vpand	%ymm2, %ymm7, %ymm6
	vpand	%ymm2, %ymm0, %ymm0
	vpackuswb	%ymm5, %ymm1, %ymm1
	vpackuswb	%ymm0, %ymm6, %ymm0
	vpmaddubsw	%ymm3, %ymm1, %ymm1
	vpmaddubsw	%ymm3, %ymm0, %ymm0
	vpackuswb	%ymm0, %ymm1, %ymm0
	vpermd	%ymm0, %ymm4, %ymm0
	vmovdqu	%ymm0, 96(%rax)
	ret
L_poly_compress$1:
	call	L_poly_csubq$1
L_poly_compress$2:
	leaq	glob_data + 384(%rip), %rdx
	vmovdqu	(%rdx), %ymm0
	vpbroadcastw	glob_data + 6428(%rip), %ymm1
	vpbroadcastw	glob_data + 6426(%rip), %ymm2
	vpbroadcastw	glob_data + 6424(%rip), %ymm3
	vmovdqu	glob_data + 128(%rip), %ymm4
	vmovdqu	(%rcx), %ymm5
	vmovdqu	32(%rcx), %ymm6
	vmovdqu	64(%rcx), %ymm7
	vmovdqu	96(%rcx), %ymm8
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpmulhw	%ymm0, %ymm7, %ymm7
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpmulhrsw	%ymm1, %ymm5, %ymm5
	vpmulhrsw	%ymm1, %ymm6, %ymm6
	vpmulhrsw	%ymm1, %ymm7, %ymm7
	vpmulhrsw	%ymm1, %ymm8, %ymm8
	vpand	%ymm2, %ymm5, %ymm5
	vpand	%ymm2, %ymm6, %ymm6
	vpand	%ymm2, %ymm7, %ymm7
	vpand	%ymm2, %ymm8, %ymm8
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpackuswb	%ymm8, %ymm7, %ymm6
	vpmaddubsw	%ymm3, %ymm5, %ymm5
	vpmaddubsw	%ymm3, %ymm6, %ymm6
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpermd	%ymm5, %ymm4, %ymm5
	vmovdqu	%ymm5, (%rax)
	vmovdqu	128(%rcx), %ymm5
	vmovdqu	160(%rcx), %ymm6
	vmovdqu	192(%rcx), %ymm7
	vmovdqu	224(%rcx), %ymm8
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpmulhw	%ymm0, %ymm7, %ymm7
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpmulhrsw	%ymm1, %ymm5, %ymm5
	vpmulhrsw	%ymm1, %ymm6, %ymm6
	vpmulhrsw	%ymm1, %ymm7, %ymm7
	vpmulhrsw	%ymm1, %ymm8, %ymm8
	vpand	%ymm2, %ymm5, %ymm5
	vpand	%ymm2, %ymm6, %ymm6
	vpand	%ymm2, %ymm7, %ymm7
	vpand	%ymm2, %ymm8, %ymm8
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpackuswb	%ymm8, %ymm7, %ymm6
	vpmaddubsw	%ymm3, %ymm5, %ymm5
	vpmaddubsw	%ymm3, %ymm6, %ymm6
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpermd	%ymm5, %ymm4, %ymm5
	vmovdqu	%ymm5, 32(%rax)
	vmovdqu	256(%rcx), %ymm5
	vmovdqu	288(%rcx), %ymm6
	vmovdqu	320(%rcx), %ymm7
	vmovdqu	352(%rcx), %ymm8
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpmulhw	%ymm0, %ymm7, %ymm7
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpmulhrsw	%ymm1, %ymm5, %ymm5
	vpmulhrsw	%ymm1, %ymm6, %ymm6
	vpmulhrsw	%ymm1, %ymm7, %ymm7
	vpmulhrsw	%ymm1, %ymm8, %ymm8
	vpand	%ymm2, %ymm5, %ymm5
	vpand	%ymm2, %ymm6, %ymm6
	vpand	%ymm2, %ymm7, %ymm7
	vpand	%ymm2, %ymm8, %ymm8
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpackuswb	%ymm8, %ymm7, %ymm6
	vpmaddubsw	%ymm3, %ymm5, %ymm5
	vpmaddubsw	%ymm3, %ymm6, %ymm6
	vpackuswb	%ymm6, %ymm5, %ymm5
	vpermd	%ymm5, %ymm4, %ymm5
	vmovdqu	%ymm5, 64(%rax)
	vmovdqu	384(%rcx), %ymm5
	vmovdqu	416(%rcx), %ymm6
	vmovdqu	448(%rcx), %ymm7
	vmovdqu	480(%rcx), %ymm8
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpmulhw	%ymm0, %ymm7, %ymm7
	vpmulhw	%ymm0, %ymm8, %ymm0
	vpmulhrsw	%ymm1, %ymm5, %ymm5
	vpmulhrsw	%ymm1, %ymm6, %ymm6
	vpmulhrsw	%ymm1, %ymm7, %ymm7
	vpmulhrsw	%ymm1, %ymm0, %ymm0
	vpand	%ymm2, %ymm5, %ymm1
	vpand	%ymm2, %ymm6, %ymm5
	vpand	%ymm2, %ymm7, %ymm6
	vpand	%ymm2, %ymm0, %ymm0
	vpackuswb	%ymm5, %ymm1, %ymm1
	vpackuswb	%ymm0, %ymm6, %ymm0
	vpmaddubsw	%ymm3, %ymm1, %ymm1
	vpmaddubsw	%ymm3, %ymm0, %ymm0
	vpackuswb	%ymm0, %ymm1, %ymm0
	vpermd	%ymm0, %ymm4, %ymm0
	vmovdqu	%ymm0, 96(%rax)
	ret
L_poly_basemul$1:
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	glob_data + 416(%rip), %ymm1
	vmovdqu	glob_data + 1552(%rip), %ymm2
	vmovdqu	glob_data + 1584(%rip), %ymm3
	vmovdqu	(%rsi), %ymm4
	vmovdqu	32(%rsi), %ymm5
	vmovdqu	(%rdi), %ymm6
	vmovdqu	32(%rdi), %ymm7
	vpmullw	%ymm5, %ymm2, %ymm8
	vpmulhw	%ymm5, %ymm3, %ymm9
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm8, %ymm9, %ymm8
	vpmullw	%ymm6, %ymm4, %ymm10
	vpmulhw	%ymm6, %ymm4, %ymm11
	vpunpcklwd	%ymm11, %ymm10, %ymm9
	vpunpckhwd	%ymm11, %ymm10, %ymm10
	vpmullw	%ymm7, %ymm4, %ymm12
	vpmulhw	%ymm7, %ymm4, %ymm4
	vpunpcklwd	%ymm4, %ymm12, %ymm11
	vpunpckhwd	%ymm4, %ymm12, %ymm4
	vpmullw	%ymm6, %ymm5, %ymm12
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpunpcklwd	%ymm6, %ymm12, %ymm5
	vpunpckhwd	%ymm6, %ymm12, %ymm6
	vpmullw	%ymm7, %ymm8, %ymm12
	vpmulhw	%ymm7, %ymm8, %ymm7
	vpunpcklwd	%ymm7, %ymm12, %ymm8
	vpunpckhwd	%ymm7, %ymm12, %ymm12
	vpaddd	%ymm8, %ymm9, %ymm7
	vpaddd	%ymm12, %ymm10, %ymm8
	vpaddd	%ymm11, %ymm5, %ymm5
	vpaddd	%ymm4, %ymm6, %ymm4
	vpxor	%ymm9, %ymm9, %ymm9
	vpblendw	$-86, %ymm9, %ymm7, %ymm6
	vpblendw	$-86, %ymm9, %ymm8, %ymm10
	vpackusdw	%ymm10, %ymm6, %ymm6
	vpsrld	$16, %ymm7, %ymm7
	vpsrld	$16, %ymm8, %ymm8
	vpackusdw	%ymm8, %ymm7, %ymm7
	vpblendw	$-86, %ymm9, %ymm5, %ymm8
	vpblendw	$-86, %ymm9, %ymm4, %ymm9
	vpackusdw	%ymm9, %ymm8, %ymm8
	vpsrld	$16, %ymm5, %ymm5
	vpsrld	$16, %ymm4, %ymm4
	vpackusdw	%ymm4, %ymm5, %ymm4
	vpmullw	%ymm1, %ymm6, %ymm5
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpsubw	%ymm5, %ymm7, %ymm5
	vpmullw	%ymm1, %ymm8, %ymm6
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm6, %ymm4, %ymm4
	vmovdqu	%ymm5, (%rcx)
	vmovdqu	%ymm4, 32(%rcx)
	vmovdqu	64(%rsi), %ymm4
	vmovdqu	96(%rsi), %ymm5
	vmovdqu	64(%rdi), %ymm6
	vmovdqu	96(%rdi), %ymm7
	vpmullw	%ymm5, %ymm2, %ymm2
	vpmulhw	%ymm5, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm2, %ymm2
	vpsubw	%ymm2, %ymm3, %ymm2
	vpmullw	%ymm6, %ymm4, %ymm8
	vpmulhw	%ymm6, %ymm4, %ymm9
	vpunpcklwd	%ymm9, %ymm8, %ymm3
	vpunpckhwd	%ymm9, %ymm8, %ymm8
	vpmullw	%ymm7, %ymm4, %ymm9
	vpmulhw	%ymm7, %ymm4, %ymm10
	vpunpcklwd	%ymm10, %ymm9, %ymm4
	vpunpckhwd	%ymm10, %ymm9, %ymm9
	vpmullw	%ymm6, %ymm5, %ymm10
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpunpcklwd	%ymm6, %ymm10, %ymm5
	vpunpckhwd	%ymm6, %ymm10, %ymm6
	vpmullw	%ymm7, %ymm2, %ymm10
	vpmulhw	%ymm7, %ymm2, %ymm2
	vpunpcklwd	%ymm2, %ymm10, %ymm7
	vpunpckhwd	%ymm2, %ymm10, %ymm2
	vpsubd	%ymm7, %ymm3, %ymm7
	vpsubd	%ymm2, %ymm8, %ymm8
	vpaddd	%ymm4, %ymm5, %ymm2
	vpaddd	%ymm9, %ymm6, %ymm3
	vpxor	%ymm5, %ymm5, %ymm5
	vpblendw	$-86, %ymm5, %ymm7, %ymm4
	vpblendw	$-86, %ymm5, %ymm8, %ymm6
	vpackusdw	%ymm6, %ymm4, %ymm4
	vpsrld	$16, %ymm7, %ymm6
	vpsrld	$16, %ymm8, %ymm7
	vpackusdw	%ymm7, %ymm6, %ymm6
	vpblendw	$-86, %ymm5, %ymm2, %ymm7
	vpblendw	$-86, %ymm5, %ymm3, %ymm5
	vpackusdw	%ymm5, %ymm7, %ymm5
	vpsrld	$16, %ymm2, %ymm2
	vpsrld	$16, %ymm3, %ymm3
	vpackusdw	%ymm3, %ymm2, %ymm2
	vpmullw	%ymm1, %ymm4, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm6, %ymm3
	vpmullw	%ymm1, %ymm5, %ymm4
	vpmulhw	%ymm0, %ymm4, %ymm4
	vpsubw	%ymm4, %ymm2, %ymm2
	vmovdqu	%ymm3, 64(%rcx)
	vmovdqu	%ymm2, 96(%rcx)
	vmovdqu	glob_data + 1616(%rip), %ymm2
	vmovdqu	glob_data + 1648(%rip), %ymm3
	vmovdqu	128(%rsi), %ymm4
	vmovdqu	160(%rsi), %ymm5
	vmovdqu	128(%rdi), %ymm6
	vmovdqu	160(%rdi), %ymm7
	vpmullw	%ymm5, %ymm2, %ymm8
	vpmulhw	%ymm5, %ymm3, %ymm9
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm8, %ymm9, %ymm8
	vpmullw	%ymm6, %ymm4, %ymm10
	vpmulhw	%ymm6, %ymm4, %ymm11
	vpunpcklwd	%ymm11, %ymm10, %ymm9
	vpunpckhwd	%ymm11, %ymm10, %ymm10
	vpmullw	%ymm7, %ymm4, %ymm12
	vpmulhw	%ymm7, %ymm4, %ymm4
	vpunpcklwd	%ymm4, %ymm12, %ymm11
	vpunpckhwd	%ymm4, %ymm12, %ymm4
	vpmullw	%ymm6, %ymm5, %ymm12
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpunpcklwd	%ymm6, %ymm12, %ymm5
	vpunpckhwd	%ymm6, %ymm12, %ymm6
	vpmullw	%ymm7, %ymm8, %ymm12
	vpmulhw	%ymm7, %ymm8, %ymm7
	vpunpcklwd	%ymm7, %ymm12, %ymm8
	vpunpckhwd	%ymm7, %ymm12, %ymm12
	vpaddd	%ymm8, %ymm9, %ymm7
	vpaddd	%ymm12, %ymm10, %ymm8
	vpaddd	%ymm11, %ymm5, %ymm5
	vpaddd	%ymm4, %ymm6, %ymm4
	vpxor	%ymm9, %ymm9, %ymm9
	vpblendw	$-86, %ymm9, %ymm7, %ymm6
	vpblendw	$-86, %ymm9, %ymm8, %ymm10
	vpackusdw	%ymm10, %ymm6, %ymm6
	vpsrld	$16, %ymm7, %ymm7
	vpsrld	$16, %ymm8, %ymm8
	vpackusdw	%ymm8, %ymm7, %ymm7
	vpblendw	$-86, %ymm9, %ymm5, %ymm8
	vpblendw	$-86, %ymm9, %ymm4, %ymm9
	vpackusdw	%ymm9, %ymm8, %ymm8
	vpsrld	$16, %ymm5, %ymm5
	vpsrld	$16, %ymm4, %ymm4
	vpackusdw	%ymm4, %ymm5, %ymm4
	vpmullw	%ymm1, %ymm6, %ymm5
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpsubw	%ymm5, %ymm7, %ymm5
	vpmullw	%ymm1, %ymm8, %ymm6
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm6, %ymm4, %ymm4
	vmovdqu	%ymm5, 128(%rcx)
	vmovdqu	%ymm4, 160(%rcx)
	vmovdqu	192(%rsi), %ymm4
	vmovdqu	224(%rsi), %ymm5
	vmovdqu	192(%rdi), %ymm6
	vmovdqu	224(%rdi), %ymm7
	vpmullw	%ymm5, %ymm2, %ymm2
	vpmulhw	%ymm5, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm2, %ymm2
	vpsubw	%ymm2, %ymm3, %ymm2
	vpmullw	%ymm6, %ymm4, %ymm8
	vpmulhw	%ymm6, %ymm4, %ymm9
	vpunpcklwd	%ymm9, %ymm8, %ymm3
	vpunpckhwd	%ymm9, %ymm8, %ymm8
	vpmullw	%ymm7, %ymm4, %ymm9
	vpmulhw	%ymm7, %ymm4, %ymm10
	vpunpcklwd	%ymm10, %ymm9, %ymm4
	vpunpckhwd	%ymm10, %ymm9, %ymm9
	vpmullw	%ymm6, %ymm5, %ymm10
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpunpcklwd	%ymm6, %ymm10, %ymm5
	vpunpckhwd	%ymm6, %ymm10, %ymm6
	vpmullw	%ymm7, %ymm2, %ymm10
	vpmulhw	%ymm7, %ymm2, %ymm2
	vpunpcklwd	%ymm2, %ymm10, %ymm7
	vpunpckhwd	%ymm2, %ymm10, %ymm2
	vpsubd	%ymm7, %ymm3, %ymm7
	vpsubd	%ymm2, %ymm8, %ymm8
	vpaddd	%ymm4, %ymm5, %ymm2
	vpaddd	%ymm9, %ymm6, %ymm3
	vpxor	%ymm5, %ymm5, %ymm5
	vpblendw	$-86, %ymm5, %ymm7, %ymm4
	vpblendw	$-86, %ymm5, %ymm8, %ymm6
	vpackusdw	%ymm6, %ymm4, %ymm4
	vpsrld	$16, %ymm7, %ymm6
	vpsrld	$16, %ymm8, %ymm7
	vpackusdw	%ymm7, %ymm6, %ymm6
	vpblendw	$-86, %ymm5, %ymm2, %ymm7
	vpblendw	$-86, %ymm5, %ymm3, %ymm5
	vpackusdw	%ymm5, %ymm7, %ymm5
	vpsrld	$16, %ymm2, %ymm2
	vpsrld	$16, %ymm3, %ymm3
	vpackusdw	%ymm3, %ymm2, %ymm2
	vpmullw	%ymm1, %ymm4, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm6, %ymm3
	vpmullw	%ymm1, %ymm5, %ymm4
	vpmulhw	%ymm0, %ymm4, %ymm4
	vpsubw	%ymm4, %ymm2, %ymm2
	vmovdqu	%ymm3, 192(%rcx)
	vmovdqu	%ymm2, 224(%rcx)
	vmovdqu	glob_data + 1944(%rip), %ymm2
	vmovdqu	glob_data + 1976(%rip), %ymm3
	vmovdqu	256(%rsi), %ymm4
	vmovdqu	288(%rsi), %ymm5
	vmovdqu	256(%rdi), %ymm6
	vmovdqu	288(%rdi), %ymm7
	vpmullw	%ymm5, %ymm2, %ymm8
	vpmulhw	%ymm5, %ymm3, %ymm9
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm8, %ymm9, %ymm8
	vpmullw	%ymm6, %ymm4, %ymm10
	vpmulhw	%ymm6, %ymm4, %ymm11
	vpunpcklwd	%ymm11, %ymm10, %ymm9
	vpunpckhwd	%ymm11, %ymm10, %ymm10
	vpmullw	%ymm7, %ymm4, %ymm12
	vpmulhw	%ymm7, %ymm4, %ymm4
	vpunpcklwd	%ymm4, %ymm12, %ymm11
	vpunpckhwd	%ymm4, %ymm12, %ymm4
	vpmullw	%ymm6, %ymm5, %ymm12
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpunpcklwd	%ymm6, %ymm12, %ymm5
	vpunpckhwd	%ymm6, %ymm12, %ymm6
	vpmullw	%ymm7, %ymm8, %ymm12
	vpmulhw	%ymm7, %ymm8, %ymm7
	vpunpcklwd	%ymm7, %ymm12, %ymm8
	vpunpckhwd	%ymm7, %ymm12, %ymm12
	vpaddd	%ymm8, %ymm9, %ymm7
	vpaddd	%ymm12, %ymm10, %ymm8
	vpaddd	%ymm11, %ymm5, %ymm5
	vpaddd	%ymm4, %ymm6, %ymm4
	vpxor	%ymm9, %ymm9, %ymm9
	vpblendw	$-86, %ymm9, %ymm7, %ymm6
	vpblendw	$-86, %ymm9, %ymm8, %ymm10
	vpackusdw	%ymm10, %ymm6, %ymm6
	vpsrld	$16, %ymm7, %ymm7
	vpsrld	$16, %ymm8, %ymm8
	vpackusdw	%ymm8, %ymm7, %ymm7
	vpblendw	$-86, %ymm9, %ymm5, %ymm8
	vpblendw	$-86, %ymm9, %ymm4, %ymm9
	vpackusdw	%ymm9, %ymm8, %ymm8
	vpsrld	$16, %ymm5, %ymm5
	vpsrld	$16, %ymm4, %ymm4
	vpackusdw	%ymm4, %ymm5, %ymm4
	vpmullw	%ymm1, %ymm6, %ymm5
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpsubw	%ymm5, %ymm7, %ymm5
	vpmullw	%ymm1, %ymm8, %ymm6
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm6, %ymm4, %ymm4
	vmovdqu	%ymm5, 256(%rcx)
	vmovdqu	%ymm4, 288(%rcx)
	vmovdqu	320(%rsi), %ymm4
	vmovdqu	352(%rsi), %ymm5
	vmovdqu	320(%rdi), %ymm6
	vmovdqu	352(%rdi), %ymm7
	vpmullw	%ymm5, %ymm2, %ymm2
	vpmulhw	%ymm5, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm2, %ymm2
	vpsubw	%ymm2, %ymm3, %ymm2
	vpmullw	%ymm6, %ymm4, %ymm8
	vpmulhw	%ymm6, %ymm4, %ymm9
	vpunpcklwd	%ymm9, %ymm8, %ymm3
	vpunpckhwd	%ymm9, %ymm8, %ymm8
	vpmullw	%ymm7, %ymm4, %ymm9
	vpmulhw	%ymm7, %ymm4, %ymm10
	vpunpcklwd	%ymm10, %ymm9, %ymm4
	vpunpckhwd	%ymm10, %ymm9, %ymm9
	vpmullw	%ymm6, %ymm5, %ymm10
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpunpcklwd	%ymm6, %ymm10, %ymm5
	vpunpckhwd	%ymm6, %ymm10, %ymm6
	vpmullw	%ymm7, %ymm2, %ymm10
	vpmulhw	%ymm7, %ymm2, %ymm2
	vpunpcklwd	%ymm2, %ymm10, %ymm7
	vpunpckhwd	%ymm2, %ymm10, %ymm2
	vpsubd	%ymm7, %ymm3, %ymm7
	vpsubd	%ymm2, %ymm8, %ymm8
	vpaddd	%ymm4, %ymm5, %ymm2
	vpaddd	%ymm9, %ymm6, %ymm3
	vpxor	%ymm5, %ymm5, %ymm5
	vpblendw	$-86, %ymm5, %ymm7, %ymm4
	vpblendw	$-86, %ymm5, %ymm8, %ymm6
	vpackusdw	%ymm6, %ymm4, %ymm4
	vpsrld	$16, %ymm7, %ymm6
	vpsrld	$16, %ymm8, %ymm7
	vpackusdw	%ymm7, %ymm6, %ymm6
	vpblendw	$-86, %ymm5, %ymm2, %ymm7
	vpblendw	$-86, %ymm5, %ymm3, %ymm5
	vpackusdw	%ymm5, %ymm7, %ymm5
	vpsrld	$16, %ymm2, %ymm2
	vpsrld	$16, %ymm3, %ymm3
	vpackusdw	%ymm3, %ymm2, %ymm2
	vpmullw	%ymm1, %ymm4, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm6, %ymm3
	vpmullw	%ymm1, %ymm5, %ymm4
	vpmulhw	%ymm0, %ymm4, %ymm4
	vpsubw	%ymm4, %ymm2, %ymm2
	vmovdqu	%ymm3, 320(%rcx)
	vmovdqu	%ymm2, 352(%rcx)
	vmovdqu	glob_data + 2008(%rip), %ymm2
	vmovdqu	glob_data + 2040(%rip), %ymm3
	vmovdqu	384(%rsi), %ymm4
	vmovdqu	416(%rsi), %ymm5
	vmovdqu	384(%rdi), %ymm6
	vmovdqu	416(%rdi), %ymm7
	vpmullw	%ymm5, %ymm2, %ymm8
	vpmulhw	%ymm5, %ymm3, %ymm9
	vpmulhw	%ymm0, %ymm8, %ymm8
	vpsubw	%ymm8, %ymm9, %ymm8
	vpmullw	%ymm6, %ymm4, %ymm10
	vpmulhw	%ymm6, %ymm4, %ymm11
	vpunpcklwd	%ymm11, %ymm10, %ymm9
	vpunpckhwd	%ymm11, %ymm10, %ymm10
	vpmullw	%ymm7, %ymm4, %ymm12
	vpmulhw	%ymm7, %ymm4, %ymm4
	vpunpcklwd	%ymm4, %ymm12, %ymm11
	vpunpckhwd	%ymm4, %ymm12, %ymm4
	vpmullw	%ymm6, %ymm5, %ymm12
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpunpcklwd	%ymm6, %ymm12, %ymm5
	vpunpckhwd	%ymm6, %ymm12, %ymm6
	vpmullw	%ymm7, %ymm8, %ymm12
	vpmulhw	%ymm7, %ymm8, %ymm7
	vpunpcklwd	%ymm7, %ymm12, %ymm8
	vpunpckhwd	%ymm7, %ymm12, %ymm12
	vpaddd	%ymm8, %ymm9, %ymm7
	vpaddd	%ymm12, %ymm10, %ymm8
	vpaddd	%ymm11, %ymm5, %ymm5
	vpaddd	%ymm4, %ymm6, %ymm4
	vpxor	%ymm9, %ymm9, %ymm9
	vpblendw	$-86, %ymm9, %ymm7, %ymm6
	vpblendw	$-86, %ymm9, %ymm8, %ymm10
	vpackusdw	%ymm10, %ymm6, %ymm6
	vpsrld	$16, %ymm7, %ymm7
	vpsrld	$16, %ymm8, %ymm8
	vpackusdw	%ymm8, %ymm7, %ymm7
	vpblendw	$-86, %ymm9, %ymm5, %ymm8
	vpblendw	$-86, %ymm9, %ymm4, %ymm9
	vpackusdw	%ymm9, %ymm8, %ymm8
	vpsrld	$16, %ymm5, %ymm5
	vpsrld	$16, %ymm4, %ymm4
	vpackusdw	%ymm4, %ymm5, %ymm4
	vpmullw	%ymm1, %ymm6, %ymm5
	vpmulhw	%ymm0, %ymm5, %ymm5
	vpsubw	%ymm5, %ymm7, %ymm5
	vpmullw	%ymm1, %ymm8, %ymm6
	vpmulhw	%ymm0, %ymm6, %ymm6
	vpsubw	%ymm6, %ymm4, %ymm4
	vmovdqu	%ymm5, 384(%rcx)
	vmovdqu	%ymm4, 416(%rcx)
	vmovdqu	448(%rsi), %ymm4
	vmovdqu	480(%rsi), %ymm5
	vmovdqu	448(%rdi), %ymm6
	vmovdqu	480(%rdi), %ymm7
	vpmullw	%ymm5, %ymm2, %ymm2
	vpmulhw	%ymm5, %ymm3, %ymm3
	vpmulhw	%ymm0, %ymm2, %ymm2
	vpsubw	%ymm2, %ymm3, %ymm2
	vpmullw	%ymm6, %ymm4, %ymm8
	vpmulhw	%ymm6, %ymm4, %ymm9
	vpunpcklwd	%ymm9, %ymm8, %ymm3
	vpunpckhwd	%ymm9, %ymm8, %ymm8
	vpmullw	%ymm7, %ymm4, %ymm9
	vpmulhw	%ymm7, %ymm4, %ymm10
	vpunpcklwd	%ymm10, %ymm9, %ymm4
	vpunpckhwd	%ymm10, %ymm9, %ymm9
	vpmullw	%ymm6, %ymm5, %ymm10
	vpmulhw	%ymm6, %ymm5, %ymm6
	vpunpcklwd	%ymm6, %ymm10, %ymm5
	vpunpckhwd	%ymm6, %ymm10, %ymm6
	vpmullw	%ymm7, %ymm2, %ymm10
	vpmulhw	%ymm7, %ymm2, %ymm2
	vpunpcklwd	%ymm2, %ymm10, %ymm7
	vpunpckhwd	%ymm2, %ymm10, %ymm2
	vpsubd	%ymm7, %ymm3, %ymm7
	vpsubd	%ymm2, %ymm8, %ymm8
	vpaddd	%ymm4, %ymm5, %ymm2
	vpaddd	%ymm9, %ymm6, %ymm3
	vpxor	%ymm5, %ymm5, %ymm5
	vpblendw	$-86, %ymm5, %ymm7, %ymm4
	vpblendw	$-86, %ymm5, %ymm8, %ymm6
	vpackusdw	%ymm6, %ymm4, %ymm4
	vpsrld	$16, %ymm7, %ymm6
	vpsrld	$16, %ymm8, %ymm7
	vpackusdw	%ymm7, %ymm6, %ymm6
	vpblendw	$-86, %ymm5, %ymm2, %ymm7
	vpblendw	$-86, %ymm5, %ymm3, %ymm5
	vpackusdw	%ymm5, %ymm7, %ymm5
	vpsrld	$16, %ymm2, %ymm2
	vpsrld	$16, %ymm3, %ymm3
	vpackusdw	%ymm3, %ymm2, %ymm2
	vpmullw	%ymm1, %ymm4, %ymm3
	vpmulhw	%ymm0, %ymm3, %ymm3
	vpsubw	%ymm3, %ymm6, %ymm3
	vpmullw	%ymm1, %ymm5, %ymm1
	vpmulhw	%ymm0, %ymm1, %ymm0
	vpsubw	%ymm0, %ymm2, %ymm0
	vmovdqu	%ymm3, 448(%rcx)
	vmovdqu	%ymm0, 480(%rcx)
	ret
L_poly_csubq$1:
	vmovdqu	glob_data + 448(%rip), %ymm0
	vmovdqu	(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, (%rcx)
	vmovdqu	32(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 32(%rcx)
	vmovdqu	64(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 64(%rcx)
	vmovdqu	96(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 96(%rcx)
	vmovdqu	128(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 128(%rcx)
	vmovdqu	160(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 160(%rcx)
	vmovdqu	192(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 192(%rcx)
	vmovdqu	224(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 224(%rcx)
	vmovdqu	256(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 256(%rcx)
	vmovdqu	288(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 288(%rcx)
	vmovdqu	320(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 320(%rcx)
	vmovdqu	352(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 352(%rcx)
	vmovdqu	384(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 384(%rcx)
	vmovdqu	416(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 416(%rcx)
	vmovdqu	448(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm2
	vpaddw	%ymm1, %ymm2, %ymm1
	vmovdqu	%ymm1, 448(%rcx)
	vmovdqu	480(%rcx), %ymm1
	vpsubw	%ymm0, %ymm1, %ymm1
	vpsraw	$15, %ymm1, %ymm2
	vpand	%ymm0, %ymm2, %ymm0
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 480(%rcx)
	ret
L_poly_add2$1:
	vmovdqu	(%rcx), %ymm0
	vmovdqu	(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, (%rcx)
	vmovdqu	32(%rcx), %ymm0
	vmovdqu	32(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 32(%rcx)
	vmovdqu	64(%rcx), %ymm0
	vmovdqu	64(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 64(%rcx)
	vmovdqu	96(%rcx), %ymm0
	vmovdqu	96(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 96(%rcx)
	vmovdqu	128(%rcx), %ymm0
	vmovdqu	128(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 128(%rcx)
	vmovdqu	160(%rcx), %ymm0
	vmovdqu	160(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 160(%rcx)
	vmovdqu	192(%rcx), %ymm0
	vmovdqu	192(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 192(%rcx)
	vmovdqu	224(%rcx), %ymm0
	vmovdqu	224(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 224(%rcx)
	vmovdqu	256(%rcx), %ymm0
	vmovdqu	256(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 256(%rcx)
	vmovdqu	288(%rcx), %ymm0
	vmovdqu	288(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 288(%rcx)
	vmovdqu	320(%rcx), %ymm0
	vmovdqu	320(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 320(%rcx)
	vmovdqu	352(%rcx), %ymm0
	vmovdqu	352(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 352(%rcx)
	vmovdqu	384(%rcx), %ymm0
	vmovdqu	384(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 384(%rcx)
	vmovdqu	416(%rcx), %ymm0
	vmovdqu	416(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 416(%rcx)
	vmovdqu	448(%rcx), %ymm0
	vmovdqu	448(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 448(%rcx)
	vmovdqu	480(%rcx), %ymm0
	vmovdqu	480(%rsi), %ymm1
	vpaddw	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 480(%rcx)
	ret
L_nttunpack$1:
	vmovdqu	(%rax), %ymm1
	vmovdqu	32(%rax), %ymm3
	vmovdqu	64(%rax), %ymm4
	vmovdqu	96(%rax), %ymm5
	vmovdqu	128(%rax), %ymm2
	vmovdqu	160(%rax), %ymm6
	vmovdqu	192(%rax), %ymm7
	vmovdqu	224(%rax), %ymm8
	vperm2i128	$32, %ymm2, %ymm1, %ymm0
	vperm2i128	$49, %ymm2, %ymm1, %ymm1
	vperm2i128	$32, %ymm6, %ymm3, %ymm2
	vperm2i128	$49, %ymm6, %ymm3, %ymm3
	vperm2i128	$32, %ymm7, %ymm4, %ymm6
	vperm2i128	$49, %ymm7, %ymm4, %ymm7
	vperm2i128	$32, %ymm8, %ymm5, %ymm9
	vperm2i128	$49, %ymm8, %ymm5, %ymm10
	vpunpcklqdq	%ymm6, %ymm0, %ymm4
	vpunpckhqdq	%ymm6, %ymm0, %ymm5
	vpunpcklqdq	%ymm7, %ymm1, %ymm6
	vpunpckhqdq	%ymm7, %ymm1, %ymm7
	vpunpcklqdq	%ymm9, %ymm2, %ymm0
	vpunpckhqdq	%ymm9, %ymm2, %ymm2
	vpunpcklqdq	%ymm10, %ymm3, %ymm8
	vpunpckhqdq	%ymm10, %ymm3, %ymm3
	vmovsldup	%ymm0, %ymm1
	vpblendd	$-86, %ymm1, %ymm4, %ymm9
	vpsrlq	$32, %ymm4, %ymm1
	vpblendd	$-86, %ymm0, %ymm1, %ymm0
	vmovsldup	%ymm2, %ymm1
	vpblendd	$-86, %ymm1, %ymm5, %ymm1
	vpsrlq	$32, %ymm5, %ymm4
	vpblendd	$-86, %ymm2, %ymm4, %ymm2
	vmovsldup	%ymm8, %ymm4
	vpblendd	$-86, %ymm4, %ymm6, %ymm4
	vpsrlq	$32, %ymm6, %ymm5
	vpblendd	$-86, %ymm8, %ymm5, %ymm5
	vmovsldup	%ymm3, %ymm6
	vpblendd	$-86, %ymm6, %ymm7, %ymm6
	vpsrlq	$32, %ymm7, %ymm7
	vpblendd	$-86, %ymm3, %ymm7, %ymm3
	vpslld	$16, %ymm4, %ymm7
	vpblendw	$-86, %ymm7, %ymm9, %ymm7
	vpsrld	$16, %ymm9, %ymm8
	vpblendw	$-86, %ymm4, %ymm8, %ymm4
	vpslld	$16, %ymm5, %ymm8
	vpblendw	$-86, %ymm8, %ymm0, %ymm8
	vpsrld	$16, %ymm0, %ymm0
	vpblendw	$-86, %ymm5, %ymm0, %ymm0
	vpslld	$16, %ymm6, %ymm5
	vpblendw	$-86, %ymm5, %ymm1, %ymm5
	vpsrld	$16, %ymm1, %ymm1
	vpblendw	$-86, %ymm6, %ymm1, %ymm1
	vpslld	$16, %ymm3, %ymm6
	vpblendw	$-86, %ymm6, %ymm2, %ymm6
	vpsrld	$16, %ymm2, %ymm2
	vpblendw	$-86, %ymm3, %ymm2, %ymm2
	vmovdqu	%ymm7, (%rax)
	vmovdqu	%ymm4, 32(%rax)
	vmovdqu	%ymm8, 64(%rax)
	vmovdqu	%ymm0, 96(%rax)
	vmovdqu	%ymm5, 128(%rax)
	vmovdqu	%ymm1, 160(%rax)
	vmovdqu	%ymm6, 192(%rax)
	vmovdqu	%ymm2, 224(%rax)
	vmovdqu	256(%rax), %ymm1
	vmovdqu	288(%rax), %ymm3
	vmovdqu	320(%rax), %ymm4
	vmovdqu	352(%rax), %ymm5
	vmovdqu	384(%rax), %ymm2
	vmovdqu	416(%rax), %ymm6
	vmovdqu	448(%rax), %ymm7
	vmovdqu	480(%rax), %ymm8
	vperm2i128	$32, %ymm2, %ymm1, %ymm0
	vperm2i128	$49, %ymm2, %ymm1, %ymm1
	vperm2i128	$32, %ymm6, %ymm3, %ymm2
	vperm2i128	$49, %ymm6, %ymm3, %ymm3
	vperm2i128	$32, %ymm7, %ymm4, %ymm6
	vperm2i128	$49, %ymm7, %ymm4, %ymm7
	vperm2i128	$32, %ymm8, %ymm5, %ymm9
	vperm2i128	$49, %ymm8, %ymm5, %ymm10
	vpunpcklqdq	%ymm6, %ymm0, %ymm4
	vpunpckhqdq	%ymm6, %ymm0, %ymm5
	vpunpcklqdq	%ymm7, %ymm1, %ymm6
	vpunpckhqdq	%ymm7, %ymm1, %ymm7
	vpunpcklqdq	%ymm9, %ymm2, %ymm0
	vpunpckhqdq	%ymm9, %ymm2, %ymm2
	vpunpcklqdq	%ymm10, %ymm3, %ymm8
	vpunpckhqdq	%ymm10, %ymm3, %ymm3
	vmovsldup	%ymm0, %ymm1
	vpblendd	$-86, %ymm1, %ymm4, %ymm9
	vpsrlq	$32, %ymm4, %ymm1
	vpblendd	$-86, %ymm0, %ymm1, %ymm0
	vmovsldup	%ymm2, %ymm1
	vpblendd	$-86, %ymm1, %ymm5, %ymm1
	vpsrlq	$32, %ymm5, %ymm4
	vpblendd	$-86, %ymm2, %ymm4, %ymm2
	vmovsldup	%ymm8, %ymm4
	vpblendd	$-86, %ymm4, %ymm6, %ymm4
	vpsrlq	$32, %ymm6, %ymm5
	vpblendd	$-86, %ymm8, %ymm5, %ymm5
	vmovsldup	%ymm3, %ymm6
	vpblendd	$-86, %ymm6, %ymm7, %ymm6
	vpsrlq	$32, %ymm7, %ymm7
	vpblendd	$-86, %ymm3, %ymm7, %ymm3
	vpslld	$16, %ymm4, %ymm7
	vpblendw	$-86, %ymm7, %ymm9, %ymm7
	vpsrld	$16, %ymm9, %ymm8
	vpblendw	$-86, %ymm4, %ymm8, %ymm4
	vpslld	$16, %ymm5, %ymm8
	vpblendw	$-86, %ymm8, %ymm0, %ymm8
	vpsrld	$16, %ymm0, %ymm0
	vpblendw	$-86, %ymm5, %ymm0, %ymm0
	vpslld	$16, %ymm6, %ymm5
	vpblendw	$-86, %ymm5, %ymm1, %ymm5
	vpsrld	$16, %ymm1, %ymm1
	vpblendw	$-86, %ymm6, %ymm1, %ymm1
	vpslld	$16, %ymm3, %ymm6
	vpblendw	$-86, %ymm6, %ymm2, %ymm6
	vpsrld	$16, %ymm2, %ymm2
	vpblendw	$-86, %ymm3, %ymm2, %ymm2
	vmovdqu	%ymm7, 256(%rax)
	vmovdqu	%ymm4, 288(%rax)
	vmovdqu	%ymm8, 320(%rax)
	vmovdqu	%ymm0, 352(%rax)
	vmovdqu	%ymm5, 384(%rax)
	vmovdqu	%ymm1, 416(%rax)
	vmovdqu	%ymm6, 448(%rax)
	vmovdqu	%ymm2, 480(%rax)
	ret
L_shake256_absorb4x_33$1:
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, (%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 32(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 64(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 96(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 128(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 160(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 192(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 224(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 256(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 288(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 320(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 352(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 384(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 416(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 448(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 512(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 544(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 576(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 608(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 640(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 672(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 704(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 736(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 768(%rax)
	movq	(%rcx), %rbx
	xorq	%rbx, (%rax)
	movq	(%rdx), %rbx
	xorq	%rbx, 8(%rax)
	movq	(%rsi), %rbx
	xorq	%rbx, 16(%rax)
	movq	(%r11), %rbx
	xorq	%rbx, 24(%rax)
	movq	8(%rcx), %rbx
	xorq	%rbx, 32(%rax)
	movq	8(%rdx), %rbx
	xorq	%rbx, 40(%rax)
	movq	8(%rsi), %rbx
	xorq	%rbx, 48(%rax)
	movq	8(%r11), %rbx
	xorq	%rbx, 56(%rax)
	movq	16(%rcx), %rbx
	xorq	%rbx, 64(%rax)
	movq	16(%rdx), %rbx
	xorq	%rbx, 72(%rax)
	movq	16(%rsi), %rbx
	xorq	%rbx, 80(%rax)
	movq	16(%r11), %rbx
	xorq	%rbx, 88(%rax)
	movq	24(%rcx), %rbx
	xorq	%rbx, 96(%rax)
	movq	24(%rdx), %rbx
	xorq	%rbx, 104(%rax)
	movq	24(%rsi), %rbx
	xorq	%rbx, 112(%rax)
	movq	24(%r11), %rbx
	xorq	%rbx, 120(%rax)
	movb	32(%rcx), %cl
	xorb	%cl, 128(%rax)
	xorb	$31, 129(%rax)
	movb	32(%rdx), %cl
	xorb	%cl, 136(%rax)
	xorb	$31, 137(%rax)
	movb	32(%rsi), %cl
	xorb	%cl, 144(%rax)
	xorb	$31, 145(%rax)
	movb	32(%r11), %cl
	xorb	%cl, 152(%rax)
	xorb	$31, 153(%rax)
	vmovdqu	glob_data + 2080(%rip), %ymm0
	vmovdqu	512(%rax), %ymm1
	vpxor	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 512(%rax)
	ret
L_shake128_absorb4x_34$1:
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, (%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 32(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 64(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 96(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 128(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 160(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 192(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 224(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 256(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 288(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 320(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 352(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 384(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 416(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 448(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 480(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 512(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 544(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 576(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 608(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 640(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 672(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 704(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 736(%rax)
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 768(%rax)
	movq	(%rcx), %r8
	xorq	%r8, (%rax)
	movq	(%rdx), %r8
	xorq	%r8, 8(%rax)
	movq	(%rsi), %r8
	xorq	%r8, 16(%rax)
	movq	(%rdi), %r8
	xorq	%r8, 24(%rax)
	movq	8(%rcx), %r8
	xorq	%r8, 32(%rax)
	movq	8(%rdx), %r8
	xorq	%r8, 40(%rax)
	movq	8(%rsi), %r8
	xorq	%r8, 48(%rax)
	movq	8(%rdi), %r8
	xorq	%r8, 56(%rax)
	movq	16(%rcx), %r8
	xorq	%r8, 64(%rax)
	movq	16(%rdx), %r8
	xorq	%r8, 72(%rax)
	movq	16(%rsi), %r8
	xorq	%r8, 80(%rax)
	movq	16(%rdi), %r8
	xorq	%r8, 88(%rax)
	movq	24(%rcx), %r8
	xorq	%r8, 96(%rax)
	movq	24(%rdx), %r8
	xorq	%r8, 104(%rax)
	movq	24(%rsi), %r8
	xorq	%r8, 112(%rax)
	movq	24(%rdi), %r8
	xorq	%r8, 120(%rax)
	movw	32(%rcx), %cx
	xorw	%cx, 128(%rax)
	xorb	$31, 130(%rax)
	movw	32(%rdx), %cx
	xorw	%cx, 136(%rax)
	xorb	$31, 138(%rax)
	movw	32(%rsi), %cx
	xorw	%cx, 144(%rax)
	xorb	$31, 146(%rax)
	movw	32(%rdi), %cx
	xorw	%cx, 152(%rax)
	xorb	$31, 154(%rax)
	vmovdqu	glob_data + 2080(%rip), %ymm0
	vmovdqu	640(%rax), %ymm1
	vpxor	%ymm1, %ymm0, %ymm0
	vmovdqu	%ymm0, 640(%rax)
	ret
L_keccakf1600_4x_avx2$1:
	leaq	32(%rsp), %rcx
	leaq	glob_data + 2176(%rip), %rdx
	vmovdqu	glob_data + 2112(%rip), %ymm0
	vmovdqu	glob_data + 2144(%rip), %ymm1
	movq	$0, %rsi
L_keccakf1600_4x_avx2$2:
	vmovdqu	(%rdx,%rsi), %ymm2
	vmovdqu	(%rax), %ymm3
	vmovdqu	32(%rax), %ymm4
	vmovdqu	64(%rax), %ymm5
	vmovdqu	96(%rax), %ymm6
	vmovdqu	128(%rax), %ymm7
	vpxor	160(%rax), %ymm3, %ymm3
	vpxor	192(%rax), %ymm4, %ymm4
	vpxor	224(%rax), %ymm5, %ymm5
	vpxor	256(%rax), %ymm6, %ymm6
	vpxor	288(%rax), %ymm7, %ymm7
	vpxor	320(%rax), %ymm3, %ymm3
	vpxor	352(%rax), %ymm4, %ymm4
	vpxor	384(%rax), %ymm5, %ymm5
	vpxor	416(%rax), %ymm6, %ymm6
	vpxor	448(%rax), %ymm7, %ymm7
	vpxor	480(%rax), %ymm3, %ymm3
	vpxor	512(%rax), %ymm4, %ymm4
	vpxor	544(%rax), %ymm5, %ymm8
	vpxor	576(%rax), %ymm6, %ymm9
	vpxor	608(%rax), %ymm7, %ymm10
	vpxor	640(%rax), %ymm3, %ymm7
	vpxor	672(%rax), %ymm4, %ymm5
	vpxor	704(%rax), %ymm8, %ymm6
	vpxor	736(%rax), %ymm9, %ymm8
	vpxor	768(%rax), %ymm10, %ymm9
	vmovdqu	%ymm5, %ymm3
	vpsllq	$1, %ymm3, %ymm4
	vpsrlq	$63, %ymm3, %ymm3
	vpor	%ymm4, %ymm3, %ymm3
	vpxor	%ymm9, %ymm3, %ymm3
	vmovdqu	%ymm6, %ymm4
	vpsllq	$1, %ymm4, %ymm10
	vpsrlq	$63, %ymm4, %ymm4
	vpor	%ymm10, %ymm4, %ymm4
	vpxor	%ymm7, %ymm4, %ymm4
	vmovdqu	%ymm8, %ymm10
	vpsllq	$1, %ymm10, %ymm11
	vpsrlq	$63, %ymm10, %ymm10
	vpor	%ymm11, %ymm10, %ymm10
	vpxor	%ymm5, %ymm10, %ymm5
	vpsllq	$1, %ymm9, %ymm10
	vpsrlq	$63, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vpxor	%ymm6, %ymm9, %ymm6
	vpsllq	$1, %ymm7, %ymm9
	vpsrlq	$63, %ymm7, %ymm7
	vpor	%ymm9, %ymm7, %ymm7
	vpxor	%ymm8, %ymm7, %ymm7
	vmovdqu	(%rax), %ymm8
	vpxor	%ymm3, %ymm8, %ymm8
	vmovdqu	192(%rax), %ymm9
	vpxor	%ymm4, %ymm9, %ymm9
	vpsllq	$44, %ymm9, %ymm10
	vpsrlq	$20, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vmovdqu	384(%rax), %ymm10
	vpxor	%ymm5, %ymm10, %ymm10
	vpsllq	$43, %ymm10, %ymm11
	vpsrlq	$21, %ymm10, %ymm10
	vpor	%ymm11, %ymm10, %ymm10
	vmovdqu	576(%rax), %ymm11
	vpxor	%ymm6, %ymm11, %ymm11
	vpsllq	$21, %ymm11, %ymm12
	vpsrlq	$43, %ymm11, %ymm11
	vpor	%ymm12, %ymm11, %ymm11
	vmovdqu	768(%rax), %ymm12
	vpxor	%ymm7, %ymm12, %ymm12
	vpsllq	$14, %ymm12, %ymm13
	vpsrlq	$50, %ymm12, %ymm12
	vpor	%ymm13, %ymm12, %ymm12
	vpandn	%ymm10, %ymm9, %ymm13
	vpxor	%ymm8, %ymm13, %ymm13
	vpxor	%ymm2, %ymm13, %ymm2
	vmovdqu	%ymm2, (%rcx)
	vpandn	%ymm11, %ymm10, %ymm2
	vpxor	%ymm9, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rcx)
	vpandn	%ymm12, %ymm11, %ymm2
	vpxor	%ymm10, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rcx)
	vpandn	%ymm8, %ymm12, %ymm2
	vpxor	%ymm11, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rcx)
	vpandn	%ymm9, %ymm8, %ymm2
	vpxor	%ymm12, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rcx)
	vmovdqu	96(%rax), %ymm2
	vpxor	%ymm6, %ymm2, %ymm2
	vpsllq	$28, %ymm2, %ymm8
	vpsrlq	$36, %ymm2, %ymm2
	vpor	%ymm8, %ymm2, %ymm2
	vmovdqu	288(%rax), %ymm8
	vpxor	%ymm7, %ymm8, %ymm8
	vpsllq	$20, %ymm8, %ymm9
	vpsrlq	$44, %ymm8, %ymm8
	vpor	%ymm9, %ymm8, %ymm8
	vmovdqu	320(%rax), %ymm9
	vpxor	%ymm3, %ymm9, %ymm9
	vpsllq	$3, %ymm9, %ymm10
	vpsrlq	$61, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vmovdqu	512(%rax), %ymm10
	vpxor	%ymm4, %ymm10, %ymm10
	vpsllq	$45, %ymm10, %ymm11
	vpsrlq	$19, %ymm10, %ymm10
	vpor	%ymm11, %ymm10, %ymm10
	vmovdqu	704(%rax), %ymm11
	vpxor	%ymm5, %ymm11, %ymm11
	vpsllq	$61, %ymm11, %ymm12
	vpsrlq	$3, %ymm11, %ymm11
	vpor	%ymm12, %ymm11, %ymm11
	vpandn	%ymm9, %ymm8, %ymm12
	vpxor	%ymm2, %ymm12, %ymm12
	vmovdqu	%ymm12, 160(%rcx)
	vpandn	%ymm10, %ymm9, %ymm12
	vpxor	%ymm8, %ymm12, %ymm12
	vmovdqu	%ymm12, 192(%rcx)
	vpandn	%ymm11, %ymm10, %ymm12
	vpxor	%ymm9, %ymm12, %ymm9
	vmovdqu	%ymm9, 224(%rcx)
	vpandn	%ymm2, %ymm11, %ymm9
	vpxor	%ymm10, %ymm9, %ymm9
	vmovdqu	%ymm9, 256(%rcx)
	vpandn	%ymm8, %ymm2, %ymm2
	vpxor	%ymm11, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rcx)
	vmovdqu	32(%rax), %ymm2
	vpxor	%ymm4, %ymm2, %ymm2
	vpsllq	$1, %ymm2, %ymm8
	vpsrlq	$63, %ymm2, %ymm2
	vpor	%ymm8, %ymm2, %ymm2
	vmovdqu	224(%rax), %ymm8
	vpxor	%ymm5, %ymm8, %ymm8
	vpsllq	$6, %ymm8, %ymm9
	vpsrlq	$58, %ymm8, %ymm8
	vpor	%ymm9, %ymm8, %ymm8
	vmovdqu	416(%rax), %ymm9
	vpxor	%ymm6, %ymm9, %ymm9
	vpsllq	$25, %ymm9, %ymm10
	vpsrlq	$39, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vmovdqu	608(%rax), %ymm10
	vpxor	%ymm7, %ymm10, %ymm10
	vpshufb	%ymm0, %ymm10, %ymm10
	vmovdqu	640(%rax), %ymm11
	vpxor	%ymm3, %ymm11, %ymm11
	vpsllq	$18, %ymm11, %ymm12
	vpsrlq	$46, %ymm11, %ymm11
	vpor	%ymm12, %ymm11, %ymm11
	vpandn	%ymm9, %ymm8, %ymm12
	vpxor	%ymm2, %ymm12, %ymm12
	vmovdqu	%ymm12, 320(%rcx)
	vpandn	%ymm10, %ymm9, %ymm12
	vpxor	%ymm8, %ymm12, %ymm12
	vmovdqu	%ymm12, 352(%rcx)
	vpandn	%ymm11, %ymm10, %ymm12
	vpxor	%ymm9, %ymm12, %ymm9
	vmovdqu	%ymm9, 384(%rcx)
	vpandn	%ymm2, %ymm11, %ymm9
	vpxor	%ymm10, %ymm9, %ymm9
	vmovdqu	%ymm9, 416(%rcx)
	vpandn	%ymm8, %ymm2, %ymm2
	vpxor	%ymm11, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rcx)
	vmovdqu	128(%rax), %ymm2
	vpxor	%ymm7, %ymm2, %ymm2
	vpsllq	$27, %ymm2, %ymm8
	vpsrlq	$37, %ymm2, %ymm2
	vpor	%ymm8, %ymm2, %ymm2
	vmovdqu	160(%rax), %ymm8
	vpxor	%ymm3, %ymm8, %ymm8
	vpsllq	$36, %ymm8, %ymm9
	vpsrlq	$28, %ymm8, %ymm8
	vpor	%ymm9, %ymm8, %ymm8
	vmovdqu	352(%rax), %ymm9
	vpxor	%ymm4, %ymm9, %ymm9
	vpsllq	$10, %ymm9, %ymm10
	vpsrlq	$54, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vmovdqu	544(%rax), %ymm10
	vpxor	%ymm5, %ymm10, %ymm10
	vpsllq	$15, %ymm10, %ymm11
	vpsrlq	$49, %ymm10, %ymm10
	vpor	%ymm11, %ymm10, %ymm10
	vmovdqu	736(%rax), %ymm11
	vpxor	%ymm6, %ymm11, %ymm11
	vpshufb	%ymm1, %ymm11, %ymm11
	vpandn	%ymm9, %ymm8, %ymm12
	vpxor	%ymm2, %ymm12, %ymm12
	vmovdqu	%ymm12, 480(%rcx)
	vpandn	%ymm10, %ymm9, %ymm12
	vpxor	%ymm8, %ymm12, %ymm12
	vmovdqu	%ymm12, 512(%rcx)
	vpandn	%ymm11, %ymm10, %ymm12
	vpxor	%ymm9, %ymm12, %ymm9
	vmovdqu	%ymm9, 544(%rcx)
	vpandn	%ymm2, %ymm11, %ymm9
	vpxor	%ymm10, %ymm9, %ymm9
	vmovdqu	%ymm9, 576(%rcx)
	vpandn	%ymm8, %ymm2, %ymm2
	vpxor	%ymm11, %ymm2, %ymm2
	vmovdqu	%ymm2, 608(%rcx)
	vmovdqu	64(%rax), %ymm2
	vpxor	%ymm5, %ymm2, %ymm2
	vpsllq	$62, %ymm2, %ymm5
	vpsrlq	$2, %ymm2, %ymm2
	vpor	%ymm5, %ymm2, %ymm2
	vmovdqu	256(%rax), %ymm5
	vpxor	%ymm6, %ymm5, %ymm5
	vpsllq	$55, %ymm5, %ymm6
	vpsrlq	$9, %ymm5, %ymm5
	vpor	%ymm6, %ymm5, %ymm5
	vmovdqu	448(%rax), %ymm6
	vpxor	%ymm7, %ymm6, %ymm6
	vpsllq	$39, %ymm6, %ymm7
	vpsrlq	$25, %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm6
	vmovdqu	480(%rax), %ymm7
	vpxor	%ymm3, %ymm7, %ymm3
	vpsllq	$41, %ymm3, %ymm7
	vpsrlq	$23, %ymm3, %ymm3
	vpor	%ymm7, %ymm3, %ymm3
	vmovdqu	672(%rax), %ymm7
	vpxor	%ymm4, %ymm7, %ymm4
	vpsllq	$2, %ymm4, %ymm7
	vpsrlq	$62, %ymm4, %ymm4
	vpor	%ymm7, %ymm4, %ymm4
	vpandn	%ymm6, %ymm5, %ymm7
	vpxor	%ymm2, %ymm7, %ymm7
	vmovdqu	%ymm7, 640(%rcx)
	vpandn	%ymm3, %ymm6, %ymm7
	vpxor	%ymm5, %ymm7, %ymm7
	vmovdqu	%ymm7, 672(%rcx)
	vpandn	%ymm4, %ymm3, %ymm7
	vpxor	%ymm6, %ymm7, %ymm6
	vmovdqu	%ymm6, 704(%rcx)
	vpandn	%ymm2, %ymm4, %ymm6
	vpxor	%ymm3, %ymm6, %ymm3
	vmovdqu	%ymm3, 736(%rcx)
	vpandn	%ymm5, %ymm2, %ymm2
	vpxor	%ymm4, %ymm2, %ymm2
	vmovdqu	%ymm2, 768(%rcx)
	vmovdqu	32(%rdx,%rsi), %ymm2
	vmovdqu	(%rcx), %ymm3
	vmovdqu	32(%rcx), %ymm4
	vmovdqu	64(%rcx), %ymm5
	vmovdqu	96(%rcx), %ymm6
	vmovdqu	128(%rcx), %ymm7
	vpxor	160(%rcx), %ymm3, %ymm3
	vpxor	192(%rcx), %ymm4, %ymm4
	vpxor	224(%rcx), %ymm5, %ymm5
	vpxor	256(%rcx), %ymm6, %ymm6
	vpxor	288(%rcx), %ymm7, %ymm7
	vpxor	320(%rcx), %ymm3, %ymm3
	vpxor	352(%rcx), %ymm4, %ymm4
	vpxor	384(%rcx), %ymm5, %ymm5
	vpxor	416(%rcx), %ymm6, %ymm6
	vpxor	448(%rcx), %ymm7, %ymm7
	vpxor	480(%rcx), %ymm3, %ymm3
	vpxor	512(%rcx), %ymm4, %ymm4
	vpxor	544(%rcx), %ymm5, %ymm8
	vpxor	576(%rcx), %ymm6, %ymm9
	vpxor	608(%rcx), %ymm7, %ymm10
	vpxor	640(%rcx), %ymm3, %ymm7
	vpxor	672(%rcx), %ymm4, %ymm5
	vpxor	704(%rcx), %ymm8, %ymm6
	vpxor	736(%rcx), %ymm9, %ymm8
	vpxor	768(%rcx), %ymm10, %ymm9
	vmovdqu	%ymm5, %ymm3
	vpsllq	$1, %ymm3, %ymm4
	vpsrlq	$63, %ymm3, %ymm3
	vpor	%ymm4, %ymm3, %ymm3
	vpxor	%ymm9, %ymm3, %ymm3
	vmovdqu	%ymm6, %ymm4
	vpsllq	$1, %ymm4, %ymm10
	vpsrlq	$63, %ymm4, %ymm4
	vpor	%ymm10, %ymm4, %ymm4
	vpxor	%ymm7, %ymm4, %ymm4
	vmovdqu	%ymm8, %ymm10
	vpsllq	$1, %ymm10, %ymm11
	vpsrlq	$63, %ymm10, %ymm10
	vpor	%ymm11, %ymm10, %ymm10
	vpxor	%ymm5, %ymm10, %ymm5
	vpsllq	$1, %ymm9, %ymm10
	vpsrlq	$63, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vpxor	%ymm6, %ymm9, %ymm6
	vpsllq	$1, %ymm7, %ymm9
	vpsrlq	$63, %ymm7, %ymm7
	vpor	%ymm9, %ymm7, %ymm7
	vpxor	%ymm8, %ymm7, %ymm7
	vmovdqu	(%rcx), %ymm8
	vpxor	%ymm3, %ymm8, %ymm8
	vmovdqu	192(%rcx), %ymm9
	vpxor	%ymm4, %ymm9, %ymm9
	vpsllq	$44, %ymm9, %ymm10
	vpsrlq	$20, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vmovdqu	384(%rcx), %ymm10
	vpxor	%ymm5, %ymm10, %ymm10
	vpsllq	$43, %ymm10, %ymm11
	vpsrlq	$21, %ymm10, %ymm10
	vpor	%ymm11, %ymm10, %ymm10
	vmovdqu	576(%rcx), %ymm11
	vpxor	%ymm6, %ymm11, %ymm11
	vpsllq	$21, %ymm11, %ymm12
	vpsrlq	$43, %ymm11, %ymm11
	vpor	%ymm12, %ymm11, %ymm11
	vmovdqu	768(%rcx), %ymm12
	vpxor	%ymm7, %ymm12, %ymm12
	vpsllq	$14, %ymm12, %ymm13
	vpsrlq	$50, %ymm12, %ymm12
	vpor	%ymm13, %ymm12, %ymm12
	vpandn	%ymm10, %ymm9, %ymm13
	vpxor	%ymm8, %ymm13, %ymm13
	vpxor	%ymm2, %ymm13, %ymm2
	vmovdqu	%ymm2, (%rax)
	vpandn	%ymm11, %ymm10, %ymm2
	vpxor	%ymm9, %ymm2, %ymm2
	vmovdqu	%ymm2, 32(%rax)
	vpandn	%ymm12, %ymm11, %ymm2
	vpxor	%ymm10, %ymm2, %ymm2
	vmovdqu	%ymm2, 64(%rax)
	vpandn	%ymm8, %ymm12, %ymm2
	vpxor	%ymm11, %ymm2, %ymm2
	vmovdqu	%ymm2, 96(%rax)
	vpandn	%ymm9, %ymm8, %ymm2
	vpxor	%ymm12, %ymm2, %ymm2
	vmovdqu	%ymm2, 128(%rax)
	vmovdqu	96(%rcx), %ymm2
	vpxor	%ymm6, %ymm2, %ymm2
	vpsllq	$28, %ymm2, %ymm8
	vpsrlq	$36, %ymm2, %ymm2
	vpor	%ymm8, %ymm2, %ymm2
	vmovdqu	288(%rcx), %ymm8
	vpxor	%ymm7, %ymm8, %ymm8
	vpsllq	$20, %ymm8, %ymm9
	vpsrlq	$44, %ymm8, %ymm8
	vpor	%ymm9, %ymm8, %ymm8
	vmovdqu	320(%rcx), %ymm9
	vpxor	%ymm3, %ymm9, %ymm9
	vpsllq	$3, %ymm9, %ymm10
	vpsrlq	$61, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vmovdqu	512(%rcx), %ymm10
	vpxor	%ymm4, %ymm10, %ymm10
	vpsllq	$45, %ymm10, %ymm11
	vpsrlq	$19, %ymm10, %ymm10
	vpor	%ymm11, %ymm10, %ymm10
	vmovdqu	704(%rcx), %ymm11
	vpxor	%ymm5, %ymm11, %ymm11
	vpsllq	$61, %ymm11, %ymm12
	vpsrlq	$3, %ymm11, %ymm11
	vpor	%ymm12, %ymm11, %ymm11
	vpandn	%ymm9, %ymm8, %ymm12
	vpxor	%ymm2, %ymm12, %ymm12
	vmovdqu	%ymm12, 160(%rax)
	vpandn	%ymm10, %ymm9, %ymm12
	vpxor	%ymm8, %ymm12, %ymm12
	vmovdqu	%ymm12, 192(%rax)
	vpandn	%ymm11, %ymm10, %ymm12
	vpxor	%ymm9, %ymm12, %ymm9
	vmovdqu	%ymm9, 224(%rax)
	vpandn	%ymm2, %ymm11, %ymm9
	vpxor	%ymm10, %ymm9, %ymm9
	vmovdqu	%ymm9, 256(%rax)
	vpandn	%ymm8, %ymm2, %ymm2
	vpxor	%ymm11, %ymm2, %ymm2
	vmovdqu	%ymm2, 288(%rax)
	vmovdqu	32(%rcx), %ymm2
	vpxor	%ymm4, %ymm2, %ymm2
	vpsllq	$1, %ymm2, %ymm8
	vpsrlq	$63, %ymm2, %ymm2
	vpor	%ymm8, %ymm2, %ymm2
	vmovdqu	224(%rcx), %ymm8
	vpxor	%ymm5, %ymm8, %ymm8
	vpsllq	$6, %ymm8, %ymm9
	vpsrlq	$58, %ymm8, %ymm8
	vpor	%ymm9, %ymm8, %ymm8
	vmovdqu	416(%rcx), %ymm9
	vpxor	%ymm6, %ymm9, %ymm9
	vpsllq	$25, %ymm9, %ymm10
	vpsrlq	$39, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vmovdqu	608(%rcx), %ymm10
	vpxor	%ymm7, %ymm10, %ymm10
	vpshufb	%ymm0, %ymm10, %ymm10
	vmovdqu	640(%rcx), %ymm11
	vpxor	%ymm3, %ymm11, %ymm11
	vpsllq	$18, %ymm11, %ymm12
	vpsrlq	$46, %ymm11, %ymm11
	vpor	%ymm12, %ymm11, %ymm11
	vpandn	%ymm9, %ymm8, %ymm12
	vpxor	%ymm2, %ymm12, %ymm12
	vmovdqu	%ymm12, 320(%rax)
	vpandn	%ymm10, %ymm9, %ymm12
	vpxor	%ymm8, %ymm12, %ymm12
	vmovdqu	%ymm12, 352(%rax)
	vpandn	%ymm11, %ymm10, %ymm12
	vpxor	%ymm9, %ymm12, %ymm9
	vmovdqu	%ymm9, 384(%rax)
	vpandn	%ymm2, %ymm11, %ymm9
	vpxor	%ymm10, %ymm9, %ymm9
	vmovdqu	%ymm9, 416(%rax)
	vpandn	%ymm8, %ymm2, %ymm2
	vpxor	%ymm11, %ymm2, %ymm2
	vmovdqu	%ymm2, 448(%rax)
	vmovdqu	128(%rcx), %ymm2
	vpxor	%ymm7, %ymm2, %ymm2
	vpsllq	$27, %ymm2, %ymm8
	vpsrlq	$37, %ymm2, %ymm2
	vpor	%ymm8, %ymm2, %ymm2
	vmovdqu	160(%rcx), %ymm8
	vpxor	%ymm3, %ymm8, %ymm8
	vpsllq	$36, %ymm8, %ymm9
	vpsrlq	$28, %ymm8, %ymm8
	vpor	%ymm9, %ymm8, %ymm8
	vmovdqu	352(%rcx), %ymm9
	vpxor	%ymm4, %ymm9, %ymm9
	vpsllq	$10, %ymm9, %ymm10
	vpsrlq	$54, %ymm9, %ymm9
	vpor	%ymm10, %ymm9, %ymm9
	vmovdqu	544(%rcx), %ymm10
	vpxor	%ymm5, %ymm10, %ymm10
	vpsllq	$15, %ymm10, %ymm11
	vpsrlq	$49, %ymm10, %ymm10
	vpor	%ymm11, %ymm10, %ymm10
	vmovdqu	736(%rcx), %ymm11
	vpxor	%ymm6, %ymm11, %ymm11
	vpshufb	%ymm1, %ymm11, %ymm11
	vpandn	%ymm9, %ymm8, %ymm12
	vpxor	%ymm2, %ymm12, %ymm12
	vmovdqu	%ymm12, 480(%rax)
	vpandn	%ymm10, %ymm9, %ymm12
	vpxor	%ymm8, %ymm12, %ymm12
	vmovdqu	%ymm12, 512(%rax)
	vpandn	%ymm11, %ymm10, %ymm12
	vpxor	%ymm9, %ymm12, %ymm9
	vmovdqu	%ymm9, 544(%rax)
	vpandn	%ymm2, %ymm11, %ymm9
	vpxor	%ymm10, %ymm9, %ymm9
	vmovdqu	%ymm9, 576(%rax)
	vpandn	%ymm8, %ymm2, %ymm2
	vpxor	%ymm11, %ymm2, %ymm2
	vmovdqu	%ymm2, 608(%rax)
	vmovdqu	64(%rcx), %ymm2
	vpxor	%ymm5, %ymm2, %ymm2
	vpsllq	$62, %ymm2, %ymm5
	vpsrlq	$2, %ymm2, %ymm2
	vpor	%ymm5, %ymm2, %ymm2
	vmovdqu	256(%rcx), %ymm5
	vpxor	%ymm6, %ymm5, %ymm5
	vpsllq	$55, %ymm5, %ymm6
	vpsrlq	$9, %ymm5, %ymm5
	vpor	%ymm6, %ymm5, %ymm5
	vmovdqu	448(%rcx), %ymm6
	vpxor	%ymm7, %ymm6, %ymm6
	vpsllq	$39, %ymm6, %ymm7
	vpsrlq	$25, %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm6
	vmovdqu	480(%rcx), %ymm7
	vpxor	%ymm3, %ymm7, %ymm3
	vpsllq	$41, %ymm3, %ymm7
	vpsrlq	$23, %ymm3, %ymm3
	vpor	%ymm7, %ymm3, %ymm3
	vmovdqu	672(%rcx), %ymm7
	vpxor	%ymm4, %ymm7, %ymm4
	vpsllq	$2, %ymm4, %ymm7
	vpsrlq	$62, %ymm4, %ymm4
	vpor	%ymm7, %ymm4, %ymm4
	vpandn	%ymm6, %ymm5, %ymm7
	vpxor	%ymm2, %ymm7, %ymm7
	vmovdqu	%ymm7, 640(%rax)
	vpandn	%ymm3, %ymm6, %ymm7
	vpxor	%ymm5, %ymm7, %ymm7
	vmovdqu	%ymm7, 672(%rax)
	vpandn	%ymm4, %ymm3, %ymm7
	vpxor	%ymm6, %ymm7, %ymm6
	vmovdqu	%ymm6, 704(%rax)
	vpandn	%ymm2, %ymm4, %ymm6
	vpxor	%ymm3, %ymm6, %ymm3
	vmovdqu	%ymm3, 736(%rax)
	vpandn	%ymm5, %ymm2, %ymm2
	vpxor	%ymm4, %ymm2, %ymm2
	vmovdqu	%ymm2, 768(%rax)
	addq	$64, %rsi
	cmpq	$768, %rsi
	jb  	L_keccakf1600_4x_avx2$2
	ret
L_sha3_512_32$1:
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 32(%rsp)
	vmovdqu	%ymm0, 64(%rsp)
	vmovdqu	%ymm0, 96(%rsp)
	vmovdqu	%ymm0, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vpbroadcastq	(%rcx), %ymm6
	movq	8(%rcx), %rdx
	movq	glob_data + 6208(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	16(%rcx), %rdx
	movq	glob_data + 6216(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	24(%rcx), %rcx
	movq	glob_data + 6224(%rip), %rdx
	movq	%rcx, 32(%rsp,%rdx,8)
	movq	glob_data + 6232(%rip), %rcx
	shlq	$3, %rcx
	movb	$6, 32(%rsp,%rcx)
	movq	glob_data + 6264(%rip), %rcx
	shlq	$3, %rcx
	movq	$71, %rdx
	andq	$7, %rdx
	addq	%rdx, %rcx
	xorb	$-128, 32(%rsp,%rcx)
	vmovdqu	64(%rsp), %ymm0
	vmovdqu	96(%rsp), %ymm1
	vmovdqu	128(%rsp), %ymm2
	vmovdqu	160(%rsp), %ymm3
	vmovdqu	192(%rsp), %ymm4
	vmovdqu	224(%rsp), %ymm5
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L_sha3_512_32$2:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm4, %ymm7
	vpxor	%ymm5, %ymm3, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm6, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm6, %ymm6
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	96(%rsi), %ymm3, %ymm7
	vpsrlvq	96(%rdi), %ymm3, %ymm3
	vpor	%ymm7, %ymm3, %ymm10
	vpxor	%ymm9, %ymm4, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm4
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm4, %ymm3, %ymm4
	vpxor	%ymm9, %ymm5, %ymm5
	vpermq	$-115, %ymm1, %ymm3
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm5, %ymm1
	vpsrlvq	160(%rdi), %ymm5, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm4, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm4
	vpblendd	$12, %ymm7, %ymm3, %ymm5
	vpblendd	$12, %ymm3, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm4, %ymm4
	vpblendd	$48, %ymm10, %ymm5, %ymm5
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm9, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm4, %ymm1, %ymm1
	vpandn	%ymm11, %ymm5, %ymm5
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm3, %ymm12
	vpxor	%ymm3, %ymm1, %ymm4
	vpblendd	$48, %ymm3, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm6, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm6, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm3, %ymm13, %ymm13
	vpblendd	$-64, %ymm3, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm4, %ymm2
	vpermq	$-115, %ymm5, %ymm4
	vpermq	$114, %ymm12, %ymm5
	vpblendd	$12, %ymm3, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm3, %ymm9, %ymm3
	vpandn	%ymm3, %ymm0, %ymm3
	vpxor	%ymm13, %ymm6, %ymm6
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm3, %ymm3
	vpxor	(%rcx,%rdx), %ymm6, %ymm6
	addq	$32, %rdx
	decq	%r8
	jne 	L_sha3_512_32$2
	vmovdqu	%ymm6, 32(%rsp)
	vmovdqu	%ymm0, 64(%rsp)
	vmovdqu	%ymm1, 96(%rsp)
	vmovdqu	%ymm2, 128(%rsp)
	vmovdqu	%ymm3, 160(%rsp)
	vmovdqu	%ymm4, 192(%rsp)
	vmovdqu	%ymm5, 224(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	ret
L_sha3_512_64$1:
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 32(%rsp)
	vmovdqu	%ymm0, 64(%rsp)
	vmovdqu	%ymm0, 96(%rsp)
	vmovdqu	%ymm0, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vpbroadcastq	(%rcx), %ymm6
	movq	8(%rcx), %rdx
	movq	glob_data + 6208(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	16(%rcx), %rdx
	movq	glob_data + 6216(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	24(%rcx), %rdx
	movq	glob_data + 6224(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	32(%rcx), %rdx
	movq	glob_data + 6232(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	40(%rcx), %rdx
	movq	glob_data + 6240(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	48(%rcx), %rdx
	movq	glob_data + 6248(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	56(%rcx), %rcx
	movq	glob_data + 6256(%rip), %rdx
	movq	%rcx, 32(%rsp,%rdx,8)
	movq	glob_data + 6264(%rip), %rcx
	shlq	$3, %rcx
	movb	$6, 32(%rsp,%rcx)
	movq	glob_data + 6264(%rip), %rcx
	shlq	$3, %rcx
	movq	$71, %rdx
	andq	$7, %rdx
	addq	%rdx, %rcx
	xorb	$-128, 32(%rsp,%rcx)
	vmovdqu	64(%rsp), %ymm0
	vmovdqu	96(%rsp), %ymm1
	vmovdqu	128(%rsp), %ymm2
	vmovdqu	160(%rsp), %ymm3
	vmovdqu	192(%rsp), %ymm4
	vmovdqu	224(%rsp), %ymm5
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L_sha3_512_64$2:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm4, %ymm7
	vpxor	%ymm5, %ymm3, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm6, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm6, %ymm6
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	96(%rsi), %ymm3, %ymm7
	vpsrlvq	96(%rdi), %ymm3, %ymm3
	vpor	%ymm7, %ymm3, %ymm10
	vpxor	%ymm9, %ymm4, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm4
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm4, %ymm3, %ymm4
	vpxor	%ymm9, %ymm5, %ymm5
	vpermq	$-115, %ymm1, %ymm3
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm5, %ymm1
	vpsrlvq	160(%rdi), %ymm5, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm4, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm4
	vpblendd	$12, %ymm7, %ymm3, %ymm5
	vpblendd	$12, %ymm3, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm4, %ymm4
	vpblendd	$48, %ymm10, %ymm5, %ymm5
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm9, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm4, %ymm1, %ymm1
	vpandn	%ymm11, %ymm5, %ymm5
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm3, %ymm12
	vpxor	%ymm3, %ymm1, %ymm4
	vpblendd	$48, %ymm3, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm6, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm6, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm3, %ymm13, %ymm13
	vpblendd	$-64, %ymm3, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm4, %ymm2
	vpermq	$-115, %ymm5, %ymm4
	vpermq	$114, %ymm12, %ymm5
	vpblendd	$12, %ymm3, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm3, %ymm9, %ymm3
	vpandn	%ymm3, %ymm0, %ymm3
	vpxor	%ymm13, %ymm6, %ymm6
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm3, %ymm3
	vpxor	(%rcx,%rdx), %ymm6, %ymm6
	addq	$32, %rdx
	decq	%r8
	jne 	L_sha3_512_64$2
	vmovdqu	%ymm6, 32(%rsp)
	vmovdqu	%ymm0, 64(%rsp)
	vmovdqu	%ymm1, 96(%rsp)
	vmovdqu	%ymm2, 128(%rsp)
	vmovdqu	%ymm3, 160(%rsp)
	vmovdqu	%ymm4, 192(%rsp)
	vmovdqu	%ymm5, 224(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	movq	glob_data + 6232(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 32(%rax)
	movq	glob_data + 6240(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 40(%rax)
	movq	glob_data + 6248(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 48(%rax)
	movq	glob_data + 6256(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 56(%rax)
	ret
L_shake256_64$1:
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 32(%rsp)
	vmovdqu	%ymm0, 64(%rsp)
	vmovdqu	%ymm0, 96(%rsp)
	vmovdqu	%ymm0, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vpbroadcastq	(%rax), %ymm4
	movq	glob_data + 6208(%rip), %rsi
	movq	8(%rax), %rdi
	movq	%rdi, 32(%rsp,%rsi,8)
	movq	glob_data + 6216(%rip), %rsi
	movq	16(%rax), %rdi
	movq	%rdi, 32(%rsp,%rsi,8)
	movq	glob_data + 6224(%rip), %rsi
	movq	24(%rax), %rdi
	movq	%rdi, 32(%rsp,%rsi,8)
	movq	glob_data + 6232(%rip), %rsi
	movq	32(%rax), %rdi
	movq	%rdi, 32(%rsp,%rsi,8)
	movq	glob_data + 6240(%rip), %rsi
	movq	40(%rax), %rdi
	movq	%rdi, 32(%rsp,%rsi,8)
	movq	glob_data + 6248(%rip), %rsi
	movq	48(%rax), %rdi
	movq	%rdi, 32(%rsp,%rsi,8)
	movq	glob_data + 6256(%rip), %rsi
	movq	56(%rax), %rax
	movq	%rax, 32(%rsp,%rsi,8)
	movq	glob_data + 6264(%rip), %rax
	shlq	$3, %rax
	movb	$31, 32(%rsp,%rax)
	movq	glob_data + 6328(%rip), %rax
	shlq	$3, %rax
	movq	$135, %rsi
	andq	$7, %rsi
	addq	%rsi, %rax
	xorb	$-128, 32(%rsp,%rax)
	vmovdqu	64(%rsp), %ymm5
	vmovdqu	96(%rsp), %ymm0
	vmovdqu	128(%rsp), %ymm1
	vmovdqu	160(%rsp), %ymm6
	vmovdqu	192(%rsp), %ymm2
	vmovdqu	224(%rsp), %ymm3
	movq	$136, %rsi
	leaq	glob_data + 6200(%rip), %rax
	jmp 	L_shake256_64$7
L_shake256_64$8:
	leaq	glob_data + 3328(%rip), %rdi
	movq	$0, %r8
	leaq	glob_data + 3136(%rip), %r9
	leaq	glob_data + 2944(%rip), %r10
	movq	$24, %r11
L_shake256_64$11:
	vpshufd	$78, %ymm0, %ymm8
	vpxor	%ymm1, %ymm2, %ymm7
	vpxor	%ymm3, %ymm6, %ymm9
	vpxor	%ymm5, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm0, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm4, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm0, %ymm0
	vpxor	%ymm11, %ymm4, %ymm4
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%r9), %ymm0, %ymm7
	vpsrlvq	(%r10), %ymm0, %ymm0
	vpor	%ymm7, %ymm0, %ymm0
	vpxor	%ymm9, %ymm1, %ymm1
	vpsllvq	64(%r9), %ymm1, %ymm7
	vpsrlvq	64(%r10), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%r9), %ymm6, %ymm7
	vpsrlvq	96(%r10), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	128(%r9), %ymm2, %ymm6
	vpsrlvq	128(%r10), %ymm2, %ymm2
	vpor	%ymm6, %ymm2, %ymm2
	vpxor	%ymm9, %ymm3, %ymm3
	vpermq	$-115, %ymm0, %ymm6
	vpermq	$-115, %ymm1, %ymm7
	vpsllvq	160(%r9), %ymm3, %ymm0
	vpsrlvq	160(%r10), %ymm3, %ymm1
	vpor	%ymm0, %ymm1, %ymm8
	vpxor	%ymm9, %ymm5, %ymm0
	vpermq	$27, %ymm10, %ymm5
	vpermq	$114, %ymm2, %ymm9
	vpsllvq	32(%r9), %ymm0, %ymm1
	vpsrlvq	32(%r10), %ymm0, %ymm0
	vpor	%ymm1, %ymm0, %ymm10
	vpsrldq	$8, %ymm8, %ymm0
	vpandn	%ymm0, %ymm8, %ymm1
	vpblendd	$12, %ymm9, %ymm10, %ymm0
	vpblendd	$12, %ymm10, %ymm7, %ymm2
	vpblendd	$12, %ymm7, %ymm6, %ymm3
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm0, %ymm0
	vpblendd	$48, %ymm5, %ymm2, %ymm2
	vpblendd	$48, %ymm10, %ymm3, %ymm3
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm5, %ymm0, %ymm0
	vpblendd	$-64, %ymm9, %ymm2, %ymm2
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm2, %ymm0, %ymm0
	vpandn	%ymm11, %ymm3, %ymm3
	vpblendd	$12, %ymm10, %ymm5, %ymm11
	vpblendd	$12, %ymm5, %ymm6, %ymm12
	vpxor	%ymm6, %ymm0, %ymm2
	vpblendd	$48, %ymm6, %ymm11, %ymm0
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm5, %ymm3, %ymm3
	vpblendd	$-64, %ymm7, %ymm0, %ymm0
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm0, %ymm0
	vpxor	%ymm9, %ymm0, %ymm12
	vpermq	$30, %ymm8, %ymm0
	vpblendd	$48, %ymm4, %ymm0, %ymm0
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm4, %ymm11, %ymm11
	vpandn	%ymm0, %ymm11, %ymm11
	vpblendd	$12, %ymm5, %ymm7, %ymm0
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm0, %ymm0
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm0, %ymm0
	vpblendd	$-64, %ymm5, %ymm13, %ymm13
	vpandn	%ymm13, %ymm0, %ymm0
	vpxor	%ymm10, %ymm0, %ymm0
	vpermq	$0, %ymm1, %ymm13
	vpermq	$27, %ymm2, %ymm1
	vpermq	$-115, %ymm3, %ymm2
	vpermq	$114, %ymm12, %ymm3
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm5, %ymm9
	vpblendd	$48, %ymm5, %ymm12, %ymm5
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm5, %ymm5
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm5, %ymm6
	vpxor	%ymm13, %ymm4, %ymm4
	vpxor	%ymm8, %ymm11, %ymm5
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rdi,%r8), %ymm4, %ymm4
	addq	$32, %r8
	decq	%r11
	jne 	L_shake256_64$11
	vmovdqu	%ymm4, 32(%rsp)
	vmovdqu	%ymm5, 64(%rsp)
	vmovdqu	%ymm0, 96(%rsp)
	vmovdqu	%ymm1, 128(%rsp)
	vmovdqu	%ymm6, 160(%rsp)
	vmovdqu	%ymm2, 192(%rsp)
	vmovdqu	%ymm3, 224(%rsp)
	movq	%rsi, %rdi
	shrq	$3, %rdi
	movq	$0, %r8
	jmp 	L_shake256_64$9
L_shake256_64$10:
	movq	(%rax,%r8,8), %r9
	movq	32(%rsp,%r9,8), %r9
	movq	%r9, (%rdx,%r8,8)
	incq	%r8
L_shake256_64$9:
	cmpq	%rdi, %r8
	jb  	L_shake256_64$10
	addq	%rsi, %rdx
	subq	%rsi, %rcx
L_shake256_64$7:
	cmpq	%rsi, %rcx
	jnbe	L_shake256_64$8
	leaq	glob_data + 3328(%rip), %rsi
	movq	$0, %rdi
	leaq	glob_data + 3136(%rip), %r8
	leaq	glob_data + 2944(%rip), %r9
	movq	$24, %r10
L_shake256_64$6:
	vpshufd	$78, %ymm0, %ymm8
	vpxor	%ymm1, %ymm2, %ymm7
	vpxor	%ymm3, %ymm6, %ymm9
	vpxor	%ymm5, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm0, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm4, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm0, %ymm0
	vpxor	%ymm11, %ymm4, %ymm4
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%r8), %ymm0, %ymm7
	vpsrlvq	(%r9), %ymm0, %ymm0
	vpor	%ymm7, %ymm0, %ymm0
	vpxor	%ymm9, %ymm1, %ymm1
	vpsllvq	64(%r8), %ymm1, %ymm7
	vpsrlvq	64(%r9), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm6, %ymm6
	vpsllvq	96(%r8), %ymm6, %ymm7
	vpsrlvq	96(%r9), %ymm6, %ymm6
	vpor	%ymm7, %ymm6, %ymm10
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	128(%r8), %ymm2, %ymm6
	vpsrlvq	128(%r9), %ymm2, %ymm2
	vpor	%ymm6, %ymm2, %ymm2
	vpxor	%ymm9, %ymm3, %ymm3
	vpermq	$-115, %ymm0, %ymm6
	vpermq	$-115, %ymm1, %ymm7
	vpsllvq	160(%r8), %ymm3, %ymm0
	vpsrlvq	160(%r9), %ymm3, %ymm1
	vpor	%ymm0, %ymm1, %ymm8
	vpxor	%ymm9, %ymm5, %ymm0
	vpermq	$27, %ymm10, %ymm5
	vpermq	$114, %ymm2, %ymm9
	vpsllvq	32(%r8), %ymm0, %ymm1
	vpsrlvq	32(%r9), %ymm0, %ymm0
	vpor	%ymm1, %ymm0, %ymm10
	vpsrldq	$8, %ymm8, %ymm0
	vpandn	%ymm0, %ymm8, %ymm1
	vpblendd	$12, %ymm9, %ymm10, %ymm0
	vpblendd	$12, %ymm10, %ymm7, %ymm2
	vpblendd	$12, %ymm7, %ymm6, %ymm3
	vpblendd	$12, %ymm6, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm0, %ymm0
	vpblendd	$48, %ymm5, %ymm2, %ymm2
	vpblendd	$48, %ymm10, %ymm3, %ymm3
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm5, %ymm0, %ymm0
	vpblendd	$-64, %ymm9, %ymm2, %ymm2
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm2, %ymm0, %ymm0
	vpandn	%ymm11, %ymm3, %ymm3
	vpblendd	$12, %ymm10, %ymm5, %ymm11
	vpblendd	$12, %ymm5, %ymm6, %ymm12
	vpxor	%ymm6, %ymm0, %ymm2
	vpblendd	$48, %ymm6, %ymm11, %ymm0
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm5, %ymm3, %ymm3
	vpblendd	$-64, %ymm7, %ymm0, %ymm0
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm0, %ymm0
	vpxor	%ymm9, %ymm0, %ymm12
	vpermq	$30, %ymm8, %ymm0
	vpblendd	$48, %ymm4, %ymm0, %ymm0
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm4, %ymm11, %ymm11
	vpandn	%ymm0, %ymm11, %ymm11
	vpblendd	$12, %ymm5, %ymm7, %ymm0
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm0, %ymm0
	vpblendd	$48, %ymm6, %ymm13, %ymm13
	vpblendd	$-64, %ymm6, %ymm0, %ymm0
	vpblendd	$-64, %ymm5, %ymm13, %ymm13
	vpandn	%ymm13, %ymm0, %ymm0
	vpxor	%ymm10, %ymm0, %ymm0
	vpermq	$0, %ymm1, %ymm13
	vpermq	$27, %ymm2, %ymm1
	vpermq	$-115, %ymm3, %ymm2
	vpermq	$114, %ymm12, %ymm3
	vpblendd	$12, %ymm6, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm5, %ymm9
	vpblendd	$48, %ymm5, %ymm12, %ymm5
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm5, %ymm5
	vpblendd	$-64, %ymm6, %ymm9, %ymm6
	vpandn	%ymm6, %ymm5, %ymm6
	vpxor	%ymm13, %ymm4, %ymm4
	vpxor	%ymm8, %ymm11, %ymm5
	vpxor	%ymm7, %ymm6, %ymm6
	vpxor	(%rsi,%rdi), %ymm4, %ymm4
	addq	$32, %rdi
	decq	%r10
	jne 	L_shake256_64$6
	vmovdqu	%ymm4, 32(%rsp)
	vmovdqu	%ymm5, 64(%rsp)
	vmovdqu	%ymm0, 96(%rsp)
	vmovdqu	%ymm1, 128(%rsp)
	vmovdqu	%ymm6, 160(%rsp)
	vmovdqu	%ymm2, 192(%rsp)
	vmovdqu	%ymm3, 224(%rsp)
	movq	%rcx, %rsi
	shrq	$3, %rsi
	movq	$0, %rdi
	jmp 	L_shake256_64$4
L_shake256_64$5:
	movq	(%rax,%rdi,8), %r8
	movq	32(%rsp,%r8,8), %r8
	movq	%r8, (%rdx,%rdi,8)
	incq	%rdi
L_shake256_64$4:
	cmpq	%rsi, %rdi
	jb  	L_shake256_64$5
	movq	(%rax,%rdi,8), %rax
	shlq	$3, %rdi
	shlq	$3, %rax
	jmp 	L_shake256_64$2
L_shake256_64$3:
	movb	32(%rsp,%rax), %sil
	movb	%sil, (%rdx,%rdi)
	incq	%rdi
	incq	%rax
L_shake256_64$2:
	cmpq	%rcx, %rdi
	jb  	L_shake256_64$3
	ret
L_sha3_256_32$1:
	vpxor	%ymm0, %ymm0, %ymm0
	vmovdqu	%ymm0, 32(%rsp)
	vmovdqu	%ymm0, 64(%rsp)
	vmovdqu	%ymm0, 96(%rsp)
	vmovdqu	%ymm0, 128(%rsp)
	vmovdqu	%ymm0, 160(%rsp)
	vmovdqu	%ymm0, 192(%rsp)
	vmovdqu	%ymm0, 224(%rsp)
	vpbroadcastq	(%rcx), %ymm6
	movq	8(%rcx), %rdx
	movq	glob_data + 6208(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	16(%rcx), %rdx
	movq	glob_data + 6216(%rip), %rsi
	movq	%rdx, 32(%rsp,%rsi,8)
	movq	24(%rcx), %rcx
	movq	glob_data + 6224(%rip), %rdx
	movq	%rcx, 32(%rsp,%rdx,8)
	movq	glob_data + 6232(%rip), %rcx
	shlq	$3, %rcx
	movb	$6, 32(%rsp,%rcx)
	movq	glob_data + 6328(%rip), %rcx
	shlq	$3, %rcx
	movq	$135, %rdx
	andq	$7, %rdx
	addq	%rdx, %rcx
	xorb	$-128, 32(%rsp,%rcx)
	vmovdqu	64(%rsp), %ymm0
	vmovdqu	96(%rsp), %ymm1
	vmovdqu	128(%rsp), %ymm2
	vmovdqu	160(%rsp), %ymm3
	vmovdqu	192(%rsp), %ymm4
	vmovdqu	224(%rsp), %ymm5
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L_sha3_256_32$2:
	vpshufd	$78, %ymm1, %ymm8
	vpxor	%ymm2, %ymm4, %ymm7
	vpxor	%ymm5, %ymm3, %ymm9
	vpxor	%ymm0, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm1, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm6, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm1, %ymm1
	vpxor	%ymm11, %ymm6, %ymm6
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%rsi), %ymm1, %ymm7
	vpsrlvq	(%rdi), %ymm1, %ymm1
	vpor	%ymm7, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpsllvq	64(%rsi), %ymm2, %ymm7
	vpsrlvq	64(%rdi), %ymm2, %ymm2
	vpor	%ymm7, %ymm2, %ymm2
	vpxor	%ymm9, %ymm3, %ymm3
	vpsllvq	96(%rsi), %ymm3, %ymm7
	vpsrlvq	96(%rdi), %ymm3, %ymm3
	vpor	%ymm7, %ymm3, %ymm10
	vpxor	%ymm9, %ymm4, %ymm3
	vpsllvq	128(%rsi), %ymm3, %ymm4
	vpsrlvq	128(%rdi), %ymm3, %ymm3
	vpor	%ymm4, %ymm3, %ymm4
	vpxor	%ymm9, %ymm5, %ymm5
	vpermq	$-115, %ymm1, %ymm3
	vpermq	$-115, %ymm2, %ymm7
	vpsllvq	160(%rsi), %ymm5, %ymm1
	vpsrlvq	160(%rdi), %ymm5, %ymm2
	vpor	%ymm1, %ymm2, %ymm8
	vpxor	%ymm9, %ymm0, %ymm1
	vpermq	$27, %ymm10, %ymm0
	vpermq	$114, %ymm4, %ymm9
	vpsllvq	32(%rsi), %ymm1, %ymm2
	vpsrlvq	32(%rdi), %ymm1, %ymm1
	vpor	%ymm2, %ymm1, %ymm10
	vpsrldq	$8, %ymm8, %ymm1
	vpandn	%ymm1, %ymm8, %ymm2
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm4
	vpblendd	$12, %ymm7, %ymm3, %ymm5
	vpblendd	$12, %ymm3, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm0, %ymm4, %ymm4
	vpblendd	$48, %ymm10, %ymm5, %ymm5
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm0, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm9, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm4, %ymm1, %ymm1
	vpandn	%ymm11, %ymm5, %ymm5
	vpblendd	$12, %ymm10, %ymm0, %ymm11
	vpblendd	$12, %ymm0, %ymm3, %ymm12
	vpxor	%ymm3, %ymm1, %ymm4
	vpblendd	$48, %ymm3, %ymm11, %ymm1
	vpblendd	$48, %ymm7, %ymm12, %ymm11
	vpxor	%ymm0, %ymm5, %ymm5
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm1, %ymm1
	vpxor	%ymm9, %ymm1, %ymm12
	vpermq	$30, %ymm8, %ymm1
	vpblendd	$48, %ymm6, %ymm1, %ymm1
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm6, %ymm11, %ymm11
	vpandn	%ymm1, %ymm11, %ymm11
	vpblendd	$12, %ymm0, %ymm7, %ymm1
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm1, %ymm1
	vpblendd	$48, %ymm3, %ymm13, %ymm13
	vpblendd	$-64, %ymm3, %ymm1, %ymm1
	vpblendd	$-64, %ymm0, %ymm13, %ymm13
	vpandn	%ymm13, %ymm1, %ymm1
	vpxor	%ymm10, %ymm1, %ymm1
	vpermq	$0, %ymm2, %ymm13
	vpermq	$27, %ymm4, %ymm2
	vpermq	$-115, %ymm5, %ymm4
	vpermq	$114, %ymm12, %ymm5
	vpblendd	$12, %ymm3, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm0, %ymm9
	vpblendd	$48, %ymm0, %ymm12, %ymm0
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm0, %ymm0
	vpblendd	$-64, %ymm3, %ymm9, %ymm3
	vpandn	%ymm3, %ymm0, %ymm3
	vpxor	%ymm13, %ymm6, %ymm6
	vpxor	%ymm8, %ymm11, %ymm0
	vpxor	%ymm7, %ymm3, %ymm3
	vpxor	(%rcx,%rdx), %ymm6, %ymm6
	addq	$32, %rdx
	decq	%r8
	jne 	L_sha3_256_32$2
	vmovdqu	%ymm6, 32(%rsp)
	vmovdqu	%ymm0, 64(%rsp)
	vmovdqu	%ymm1, 96(%rsp)
	vmovdqu	%ymm2, 128(%rsp)
	vmovdqu	%ymm3, 160(%rsp)
	vmovdqu	%ymm4, 192(%rsp)
	vmovdqu	%ymm5, 224(%rsp)
	movq	glob_data + 6200(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, (%rax)
	movq	glob_data + 6208(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 8(%rax)
	movq	glob_data + 6216(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 16(%rax)
	movq	glob_data + 6224(%rip), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	ret
L_sha3_256$1:
	vpxor	%ymm6, %ymm6, %ymm6
	vpxor	%ymm3, %ymm3, %ymm3
	vpxor	%ymm4, %ymm4, %ymm4
	vpxor	%ymm0, %ymm0, %ymm0
	vpxor	%ymm5, %ymm5, %ymm5
	vpxor	%ymm1, %ymm1, %ymm1
	vpxor	%ymm2, %ymm2, %ymm2
	movb	$6, %cl
	movq	$136, %rdx
	leaq	glob_data + 6200(%rip), %rsi
	vpxor	%ymm7, %ymm7, %ymm7
	vmovdqu	%ymm7, 32(%rsp)
	vmovdqu	%ymm7, 64(%rsp)
	vmovdqu	%ymm7, 96(%rsp)
	vmovdqu	%ymm7, 128(%rsp)
	vmovdqu	%ymm7, 160(%rsp)
	vmovdqu	%ymm7, 192(%rsp)
	vmovdqu	%ymm7, 224(%rsp)
	jmp 	L_sha3_256$7
L_sha3_256$8:
	movq	%rdx, %r9
	shrq	$3, %r9
	movq	$0, %r10
	jmp 	L_sha3_256$10
L_sha3_256$11:
	movq	(%r8,%r10,8), %r11
	movq	(%rsi,%r10,8), %rbx
	movq	%r11, 32(%rsp,%rbx,8)
	incq	%r10
L_sha3_256$10:
	cmpq	%r9, %r10
	jb  	L_sha3_256$11
	movq	32(%rsp), %r9
	movq	%r9, 40(%rsp)
	movq	%r9, 48(%rsp)
	movq	%r9, 56(%rsp)
	vpxor	32(%rsp), %ymm6, %ymm6
	vpxor	64(%rsp), %ymm3, %ymm3
	vpxor	96(%rsp), %ymm4, %ymm4
	vpxor	128(%rsp), %ymm0, %ymm0
	vpxor	160(%rsp), %ymm5, %ymm5
	vpxor	192(%rsp), %ymm1, %ymm1
	vpxor	224(%rsp), %ymm2, %ymm2
	addq	%rdx, %r8
	subq	%rdx, %rdi
	leaq	glob_data + 3328(%rip), %r9
	movq	$0, %r10
	leaq	glob_data + 3136(%rip), %r11
	leaq	glob_data + 2944(%rip), %rbx
	movq	$24, %rbp
L_sha3_256$9:
	vpshufd	$78, %ymm4, %ymm8
	vpxor	%ymm0, %ymm1, %ymm7
	vpxor	%ymm2, %ymm5, %ymm9
	vpxor	%ymm3, %ymm7, %ymm7
	vpxor	%ymm9, %ymm7, %ymm10
	vpermq	$-109, %ymm10, %ymm7
	vpxor	%ymm4, %ymm8, %ymm8
	vpermq	$78, %ymm8, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm7, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm6, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm8
	vpsrlq	$63, %ymm8, %ymm9
	vpaddq	%ymm8, %ymm8, %ymm12
	vpor	%ymm9, %ymm12, %ymm9
	vpxor	%ymm11, %ymm4, %ymm4
	vpxor	%ymm11, %ymm6, %ymm6
	vpblendd	$-64, %ymm9, %ymm10, %ymm9
	vpblendd	$3, %ymm8, %ymm7, %ymm7
	vpxor	%ymm7, %ymm9, %ymm9
	vpsllvq	(%r11), %ymm4, %ymm7
	vpsrlvq	(%rbx), %ymm4, %ymm4
	vpor	%ymm7, %ymm4, %ymm4
	vpxor	%ymm9, %ymm0, %ymm0
	vpsllvq	64(%r11), %ymm0, %ymm7
	vpsrlvq	64(%rbx), %ymm0, %ymm0
	vpor	%ymm7, %ymm0, %ymm0
	vpxor	%ymm9, %ymm5, %ymm5
	vpsllvq	96(%r11), %ymm5, %ymm7
	vpsrlvq	96(%rbx), %ymm5, %ymm5
	vpor	%ymm7, %ymm5, %ymm10
	vpxor	%ymm9, %ymm1, %ymm1
	vpsllvq	128(%r11), %ymm1, %ymm5
	vpsrlvq	128(%rbx), %ymm1, %ymm1
	vpor	%ymm5, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpermq	$-115, %ymm4, %ymm5
	vpermq	$-115, %ymm0, %ymm7
	vpsllvq	160(%r11), %ymm2, %ymm0
	vpsrlvq	160(%rbx), %ymm2, %ymm2
	vpor	%ymm0, %ymm2, %ymm8
	vpxor	%ymm9, %ymm3, %ymm0
	vpermq	$27, %ymm10, %ymm3
	vpermq	$114, %ymm1, %ymm9
	vpsllvq	32(%r11), %ymm0, %ymm1
	vpsrlvq	32(%rbx), %ymm0, %ymm0
	vpor	%ymm1, %ymm0, %ymm10
	vpsrldq	$8, %ymm8, %ymm0
	vpandn	%ymm0, %ymm8, %ymm0
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm7, %ymm2
	vpblendd	$12, %ymm7, %ymm5, %ymm4
	vpblendd	$12, %ymm5, %ymm10, %ymm11
	vpblendd	$48, %ymm7, %ymm1, %ymm1
	vpblendd	$48, %ymm3, %ymm2, %ymm2
	vpblendd	$48, %ymm10, %ymm4, %ymm4
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm3, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm2, %ymm2
	vpblendd	$-64, %ymm9, %ymm4, %ymm4
	vpblendd	$-64, %ymm7, %ymm11, %ymm11
	vpandn	%ymm2, %ymm1, %ymm1
	vpandn	%ymm11, %ymm4, %ymm2
	vpblendd	$12, %ymm10, %ymm3, %ymm4
	vpblendd	$12, %ymm3, %ymm5, %ymm11
	vpxor	%ymm5, %ymm1, %ymm1
	vpblendd	$48, %ymm5, %ymm4, %ymm4
	vpblendd	$48, %ymm7, %ymm11, %ymm11
	vpxor	%ymm3, %ymm2, %ymm2
	vpblendd	$-64, %ymm7, %ymm4, %ymm4
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm4, %ymm4
	vpxor	%ymm9, %ymm4, %ymm12
	vpermq	$30, %ymm8, %ymm4
	vpblendd	$48, %ymm6, %ymm4, %ymm4
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm6, %ymm11, %ymm11
	vpandn	%ymm4, %ymm11, %ymm11
	vpblendd	$12, %ymm3, %ymm7, %ymm4
	vpblendd	$12, %ymm7, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm4, %ymm4
	vpblendd	$48, %ymm5, %ymm13, %ymm13
	vpblendd	$-64, %ymm5, %ymm4, %ymm4
	vpblendd	$-64, %ymm3, %ymm13, %ymm13
	vpandn	%ymm13, %ymm4, %ymm4
	vpxor	%ymm10, %ymm4, %ymm4
	vpermq	$0, %ymm0, %ymm13
	vpermq	$27, %ymm1, %ymm0
	vpermq	$-115, %ymm2, %ymm1
	vpermq	$114, %ymm12, %ymm2
	vpblendd	$12, %ymm5, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm3, %ymm9
	vpblendd	$48, %ymm3, %ymm12, %ymm3
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm3, %ymm3
	vpblendd	$-64, %ymm5, %ymm9, %ymm5
	vpandn	%ymm5, %ymm3, %ymm5
	vpxor	%ymm13, %ymm6, %ymm6
	vpxor	%ymm8, %ymm11, %ymm3
	vpxor	%ymm7, %ymm5, %ymm5
	vpxor	(%r9,%r10), %ymm6, %ymm6
	addq	$32, %r10
	decq	%rbp
	jne 	L_sha3_256$9
L_sha3_256$7:
	cmpq	%rdx, %rdi
	jnb 	L_sha3_256$8
	vpxor	%ymm7, %ymm7, %ymm7
	vmovdqu	%ymm7, 32(%rsp)
	vmovdqu	%ymm7, 64(%rsp)
	vmovdqu	%ymm7, 96(%rsp)
	vmovdqu	%ymm7, 128(%rsp)
	vmovdqu	%ymm7, 160(%rsp)
	vmovdqu	%ymm7, 192(%rsp)
	vmovdqu	%ymm7, 224(%rsp)
	movq	%rdi, %r9
	shrq	$3, %r9
	movq	$0, %r10
	jmp 	L_sha3_256$5
L_sha3_256$6:
	movq	(%r8,%r10,8), %r11
	movq	(%rsi,%r10,8), %rbx
	movq	%r11, 32(%rsp,%rbx,8)
	incq	%r10
L_sha3_256$5:
	cmpq	%r9, %r10
	jb  	L_sha3_256$6
	movq	(%rsi,%r10,8), %r9
	shlq	$3, %r9
	shlq	$3, %r10
	jmp 	L_sha3_256$3
L_sha3_256$4:
	movb	(%r8,%r10), %r11b
	movb	%r11b, 32(%rsp,%r9)
	incq	%r10
	incq	%r9
L_sha3_256$3:
	cmpq	%rdi, %r10
	jb  	L_sha3_256$4
	movb	%cl, 32(%rsp,%r9)
	movq	%rdx, %rcx
	addq	$-1, %rcx
	shrq	$3, %rcx
	movq	(%rsi,%rcx,8), %rcx
	shlq	$3, %rcx
	addq	$-1, %rdx
	andq	$7, %rdx
	addq	%rdx, %rcx
	xorb	$-128, 32(%rsp,%rcx)
	movq	32(%rsp), %rcx
	movq	%rcx, 40(%rsp)
	movq	%rcx, 48(%rsp)
	movq	%rcx, 56(%rsp)
	vpxor	32(%rsp), %ymm6, %ymm6
	vpxor	64(%rsp), %ymm3, %ymm7
	vpxor	96(%rsp), %ymm4, %ymm3
	vpxor	128(%rsp), %ymm0, %ymm0
	vpxor	160(%rsp), %ymm5, %ymm4
	vpxor	192(%rsp), %ymm1, %ymm1
	vpxor	224(%rsp), %ymm2, %ymm2
	leaq	glob_data + 3328(%rip), %rcx
	movq	$0, %rdx
	leaq	glob_data + 3136(%rip), %rsi
	leaq	glob_data + 2944(%rip), %rdi
	movq	$24, %r8
L_sha3_256$2:
	vpshufd	$78, %ymm3, %ymm5
	vpxor	%ymm0, %ymm1, %ymm8
	vpxor	%ymm2, %ymm4, %ymm9
	vpxor	%ymm7, %ymm8, %ymm8
	vpxor	%ymm9, %ymm8, %ymm10
	vpermq	$-109, %ymm10, %ymm8
	vpxor	%ymm3, %ymm5, %ymm5
	vpermq	$78, %ymm5, %ymm9
	vpsrlq	$63, %ymm10, %ymm11
	vpaddq	%ymm10, %ymm10, %ymm10
	vpor	%ymm10, %ymm11, %ymm11
	vpermq	$57, %ymm11, %ymm10
	vpxor	%ymm8, %ymm11, %ymm11
	vpermq	$0, %ymm11, %ymm11
	vpxor	%ymm6, %ymm5, %ymm5
	vpxor	%ymm9, %ymm5, %ymm9
	vpsrlq	$63, %ymm9, %ymm5
	vpaddq	%ymm9, %ymm9, %ymm12
	vpor	%ymm5, %ymm12, %ymm12
	vpxor	%ymm11, %ymm3, %ymm3
	vpxor	%ymm11, %ymm6, %ymm5
	vpblendd	$-64, %ymm12, %ymm10, %ymm6
	vpblendd	$3, %ymm9, %ymm8, %ymm8
	vpxor	%ymm8, %ymm6, %ymm9
	vpsllvq	(%rsi), %ymm3, %ymm6
	vpsrlvq	(%rdi), %ymm3, %ymm3
	vpor	%ymm6, %ymm3, %ymm3
	vpxor	%ymm9, %ymm0, %ymm0
	vpsllvq	64(%rsi), %ymm0, %ymm6
	vpsrlvq	64(%rdi), %ymm0, %ymm0
	vpor	%ymm6, %ymm0, %ymm0
	vpxor	%ymm9, %ymm4, %ymm4
	vpsllvq	96(%rsi), %ymm4, %ymm6
	vpsrlvq	96(%rdi), %ymm4, %ymm4
	vpor	%ymm6, %ymm4, %ymm10
	vpxor	%ymm9, %ymm1, %ymm1
	vpsllvq	128(%rsi), %ymm1, %ymm4
	vpsrlvq	128(%rdi), %ymm1, %ymm1
	vpor	%ymm4, %ymm1, %ymm1
	vpxor	%ymm9, %ymm2, %ymm2
	vpermq	$-115, %ymm3, %ymm4
	vpermq	$-115, %ymm0, %ymm6
	vpsllvq	160(%rsi), %ymm2, %ymm0
	vpsrlvq	160(%rdi), %ymm2, %ymm2
	vpor	%ymm0, %ymm2, %ymm8
	vpxor	%ymm9, %ymm7, %ymm0
	vpermq	$27, %ymm10, %ymm7
	vpermq	$114, %ymm1, %ymm9
	vpsllvq	32(%rsi), %ymm0, %ymm1
	vpsrlvq	32(%rdi), %ymm0, %ymm0
	vpor	%ymm1, %ymm0, %ymm10
	vpsrldq	$8, %ymm8, %ymm0
	vpandn	%ymm0, %ymm8, %ymm0
	vpblendd	$12, %ymm9, %ymm10, %ymm1
	vpblendd	$12, %ymm10, %ymm6, %ymm2
	vpblendd	$12, %ymm6, %ymm4, %ymm3
	vpblendd	$12, %ymm4, %ymm10, %ymm11
	vpblendd	$48, %ymm6, %ymm1, %ymm1
	vpblendd	$48, %ymm7, %ymm2, %ymm2
	vpblendd	$48, %ymm10, %ymm3, %ymm3
	vpblendd	$48, %ymm9, %ymm11, %ymm11
	vpblendd	$-64, %ymm7, %ymm1, %ymm1
	vpblendd	$-64, %ymm9, %ymm2, %ymm2
	vpblendd	$-64, %ymm9, %ymm3, %ymm3
	vpblendd	$-64, %ymm6, %ymm11, %ymm11
	vpandn	%ymm2, %ymm1, %ymm1
	vpandn	%ymm11, %ymm3, %ymm2
	vpblendd	$12, %ymm10, %ymm7, %ymm3
	vpblendd	$12, %ymm7, %ymm4, %ymm11
	vpxor	%ymm4, %ymm1, %ymm1
	vpblendd	$48, %ymm4, %ymm3, %ymm3
	vpblendd	$48, %ymm6, %ymm11, %ymm11
	vpxor	%ymm7, %ymm2, %ymm2
	vpblendd	$-64, %ymm6, %ymm3, %ymm3
	vpblendd	$-64, %ymm10, %ymm11, %ymm11
	vpandn	%ymm11, %ymm3, %ymm3
	vpxor	%ymm9, %ymm3, %ymm12
	vpermq	$30, %ymm8, %ymm3
	vpblendd	$48, %ymm5, %ymm3, %ymm3
	vpermq	$57, %ymm8, %ymm11
	vpblendd	$-64, %ymm5, %ymm11, %ymm11
	vpandn	%ymm3, %ymm11, %ymm11
	vpblendd	$12, %ymm7, %ymm6, %ymm3
	vpblendd	$12, %ymm6, %ymm9, %ymm13
	vpblendd	$48, %ymm9, %ymm3, %ymm3
	vpblendd	$48, %ymm4, %ymm13, %ymm13
	vpblendd	$-64, %ymm4, %ymm3, %ymm3
	vpblendd	$-64, %ymm7, %ymm13, %ymm13
	vpandn	%ymm13, %ymm3, %ymm3
	vpxor	%ymm10, %ymm3, %ymm3
	vpermq	$0, %ymm0, %ymm13
	vpermq	$27, %ymm1, %ymm0
	vpermq	$-115, %ymm2, %ymm1
	vpermq	$114, %ymm12, %ymm2
	vpblendd	$12, %ymm4, %ymm9, %ymm12
	vpblendd	$12, %ymm9, %ymm7, %ymm9
	vpblendd	$48, %ymm7, %ymm12, %ymm7
	vpblendd	$48, %ymm10, %ymm9, %ymm9
	vpblendd	$-64, %ymm10, %ymm7, %ymm7
	vpblendd	$-64, %ymm4, %ymm9, %ymm4
	vpandn	%ymm4, %ymm7, %ymm4
	vpxor	%ymm13, %ymm5, %ymm5
	vpxor	%ymm8, %ymm11, %ymm7
	vpxor	%ymm6, %ymm4, %ymm4
	vpxor	(%rcx,%rdx), %ymm5, %ymm6
	addq	$32, %rdx
	decq	%r8
	jne 	L_sha3_256$2
	vmovdqu	%ymm6, 32(%rsp)
	vmovdqu	%ymm7, 64(%rsp)
	vmovdqu	%ymm3, 96(%rsp)
	vmovdqu	%ymm0, 128(%rsp)
	vmovdqu	%ymm4, 160(%rsp)
	vmovdqu	%ymm1, 192(%rsp)
	vmovdqu	%ymm2, 224(%rsp)
	leaq	glob_data + 6200(%rip), %rcx
	movq	(%rcx), %rdx
	movq	32(%rsp,%rdx,8), %rdx
	movq	%rdx, (%rax)
	movq	8(%rcx), %rdx
	movq	32(%rsp,%rdx,8), %rdx
	movq	%rdx, 8(%rax)
	movq	16(%rcx), %rdx
	movq	32(%rsp,%rdx,8), %rdx
	movq	%rdx, 16(%rax)
	movq	24(%rcx), %rcx
	movq	32(%rsp,%rcx,8), %rcx
	movq	%rcx, 24(%rax)
	ret
	.data
	.p2align	5
_glob_data:
glob_data:
      .byte 0
      .byte 1
      .byte 1
      .byte 2
      .byte 3
      .byte 4
      .byte 4
      .byte 5
      .byte 6
      .byte 7
      .byte 7
      .byte 8
      .byte 9
      .byte 10
      .byte 10
      .byte 11
      .byte 4
      .byte 5
      .byte 5
      .byte 6
      .byte 7
      .byte 8
      .byte 8
      .byte 9
      .byte 10
      .byte 11
      .byte 11
      .byte 12
      .byte 13
      .byte 14
      .byte 14
      .byte 15
      .byte 0
      .byte 1
      .byte 2
      .byte 3
      .byte 4
      .byte 8
      .byte 9
      .byte 10
      .byte 11
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 9
      .byte 10
      .byte 11
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 1
      .byte 2
      .byte 3
      .byte 4
      .byte 8
      .byte 0
      .byte 1
      .byte 1
      .byte 2
      .byte 2
      .byte 3
      .byte 3
      .byte 4
      .byte 5
      .byte 6
      .byte 6
      .byte 7
      .byte 7
      .byte 8
      .byte 8
      .byte 9
      .byte 2
      .byte 3
      .byte 3
      .byte 4
      .byte 4
      .byte 5
      .byte 5
      .byte 6
      .byte 7
      .byte 8
      .byte 8
      .byte 9
      .byte 9
      .byte 10
      .byte 10
      .byte 11
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 1
      .byte 1
      .byte 1
      .byte 2
      .byte 2
      .byte 2
      .byte 2
      .byte 3
      .byte 3
      .byte 3
      .byte 3
      .byte 4
      .byte 4
      .byte 4
      .byte 4
      .byte 5
      .byte 5
      .byte 5
      .byte 5
      .byte 6
      .byte 6
      .byte 6
      .byte 6
      .byte 7
      .byte 7
      .byte 7
      .byte 7
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 4
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 5
      .byte 0
      .byte 0
      .byte 0
      .byte 2
      .byte 0
      .byte 0
      .byte 0
      .byte 6
      .byte 0
      .byte 0
      .byte 0
      .byte 3
      .byte 0
      .byte 0
      .byte 0
      .byte 7
      .byte 0
      .byte 0
      .byte 0
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 73
      .byte 5
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte 64
      .byte 3
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -128
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -127
      .byte 6
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -1
      .byte 15
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte -40
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -95
      .byte 5
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte -65
      .byte 78
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte -13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte 1
      .byte 13
      .byte -91
      .byte -91
      .byte -76
      .byte -31
      .byte 34
      .byte 79
      .byte 52
      .byte 93
      .byte 35
      .byte 68
      .byte 86
      .byte -91
      .byte -25
      .byte 78
      .byte -6
      .byte 83
      .byte 123
      .byte -57
      .byte 37
      .byte 43
      .byte 55
      .byte 58
      .byte -120
      .byte 24
      .byte -127
      .byte 126
      .byte 5
      .byte -60
      .byte -97
      .byte -9
      .byte -72
      .byte -80
      .byte -91
      .byte 6
      .byte -76
      .byte 5
      .byte 34
      .byte 9
      .byte 52
      .byte 1
      .byte 35
      .byte 11
      .byte 86
      .byte 3
      .byte -25
      .byte 9
      .byte -6
      .byte 5
      .byte 123
      .byte 6
      .byte 37
      .byte 12
      .byte 55
      .byte 5
      .byte -120
      .byte 0
      .byte -127
      .byte 11
      .byte 5
      .byte 5
      .byte -97
      .byte 10
      .byte -72
      .byte 8
      .byte 15
      .byte 68
      .byte 67
      .byte -94
      .byte 29
      .byte -112
      .byte 108
      .byte -124
      .byte 102
      .byte -43
      .byte -26
      .byte 87
      .byte -2
      .byte 30
      .byte -95
      .byte -41
      .byte -93
      .byte -67
      .byte 106
      .byte -95
      .byte 63
      .byte -43
      .byte -65
      .byte 81
      .byte -71
      .byte -96
      .byte -41
      .byte 28
      .byte -90
      .byte -100
      .byte -48
      .byte 121
      .byte 15
      .byte 7
      .byte 67
      .byte 9
      .byte 29
      .byte 9
      .byte 108
      .byte 0
      .byte 102
      .byte 3
      .byte -26
      .byte 5
      .byte -2
      .byte 4
      .byte -95
      .byte 4
      .byte -93
      .byte 4
      .byte 106
      .byte 3
      .byte 63
      .byte 8
      .byte -65
      .byte 4
      .byte -71
      .byte 5
      .byte -41
      .byte 7
      .byte -90
      .byte 10
      .byte -48
      .byte 9
      .byte -5
      .byte 69
      .byte 92
      .byte 94
      .byte 41
      .byte -17
      .byte 65
      .byte -66
      .byte -43
      .byte 49
      .byte -28
      .byte 113
      .byte 64
      .byte -55
      .byte -114
      .byte -53
      .byte -73
      .byte -72
      .byte -9
      .byte 117
      .byte -115
      .byte -36
      .byte -106
      .byte 110
      .byte -61
      .byte 34
      .byte 15
      .byte 62
      .byte 90
      .byte 110
      .byte 85
      .byte -78
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
      .byte 35
      .byte 63
      .byte 35
      .byte 63
      .byte -51
      .byte -105
      .byte -51
      .byte -105
      .byte 102
      .byte -35
      .byte 102
      .byte -35
      .byte 6
      .byte -72
      .byte 6
      .byte -72
      .byte -95
      .byte -35
      .byte -95
      .byte -35
      .byte 37
      .byte 41
      .byte 37
      .byte 41
      .byte 8
      .byte -95
      .byte 8
      .byte -95
      .byte -87
      .byte 109
      .byte -87
      .byte 109
      .byte 35
      .byte 6
      .byte 35
      .byte 6
      .byte -51
      .byte 0
      .byte -51
      .byte 0
      .byte 102
      .byte 11
      .byte 102
      .byte 11
      .byte 6
      .byte 6
      .byte 6
      .byte 6
      .byte -95
      .byte 10
      .byte -95
      .byte 10
      .byte 37
      .byte 10
      .byte 37
      .byte 10
      .byte 8
      .byte 9
      .byte 8
      .byte 9
      .byte -87
      .byte 2
      .byte -87
      .byte 2
      .byte 69
      .byte -122
      .byte 69
      .byte -122
      .byte 69
      .byte -122
      .byte 69
      .byte -122
      .byte -62
      .byte 43
      .byte -62
      .byte 43
      .byte -62
      .byte 43
      .byte -62
      .byte 43
      .byte -78
      .byte -6
      .byte -78
      .byte -6
      .byte -78
      .byte -6
      .byte -78
      .byte -6
      .byte 63
      .byte -42
      .byte 63
      .byte -42
      .byte 63
      .byte -42
      .byte 63
      .byte -42
      .byte 69
      .byte 7
      .byte 69
      .byte 7
      .byte 69
      .byte 7
      .byte 69
      .byte 7
      .byte -62
      .byte 5
      .byte -62
      .byte 5
      .byte -62
      .byte 5
      .byte -62
      .byte 5
      .byte -78
      .byte 4
      .byte -78
      .byte 4
      .byte -78
      .byte 4
      .byte -78
      .byte 4
      .byte 63
      .byte 9
      .byte 63
      .byte 9
      .byte 63
      .byte 9
      .byte 63
      .byte 9
      .byte 55
      .byte 65
      .byte 55
      .byte 65
      .byte 55
      .byte 65
      .byte 55
      .byte 65
      .byte 55
      .byte 65
      .byte 55
      .byte 65
      .byte 55
      .byte 65
      .byte 55
      .byte 65
      .byte -30
      .byte -111
      .byte -30
      .byte -111
      .byte -30
      .byte -111
      .byte -30
      .byte -111
      .byte -30
      .byte -111
      .byte -30
      .byte -111
      .byte -30
      .byte -111
      .byte -30
      .byte -111
      .byte 55
      .byte 12
      .byte 55
      .byte 12
      .byte 55
      .byte 12
      .byte 55
      .byte 12
      .byte 55
      .byte 12
      .byte 55
      .byte 12
      .byte 55
      .byte 12
      .byte 55
      .byte 12
      .byte -30
      .byte 11
      .byte -30
      .byte 11
      .byte -30
      .byte 11
      .byte -30
      .byte 11
      .byte -30
      .byte 11
      .byte -30
      .byte 11
      .byte -30
      .byte 11
      .byte -30
      .byte 11
      .byte -19
      .byte -4
      .byte -19
      .byte -4
      .byte -19
      .byte 5
      .byte -19
      .byte 5
      .byte 75
      .byte 49
      .byte -72
      .byte -77
      .byte -92
      .byte -73
      .byte 125
      .byte -79
      .byte -94
      .byte -50
      .byte 54
      .byte 73
      .byte -109
      .byte -119
      .byte -9
      .byte 126
      .byte -116
      .byte -22
      .byte -52
      .byte -91
      .byte -21
      .byte 17
      .byte -74
      .byte -52
      .byte -104
      .byte 84
      .byte -102
      .byte 55
      .byte -74
      .byte -53
      .byte 83
      .byte -44
      .byte 75
      .byte 0
      .byte -72
      .byte 11
      .byte -92
      .byte 11
      .byte 125
      .byte 10
      .byte -94
      .byte 8
      .byte 54
      .byte 7
      .byte -109
      .byte 0
      .byte -9
      .byte 9
      .byte -116
      .byte 6
      .byte -52
      .byte 1
      .byte -21
      .byte 0
      .byte -74
      .byte 10
      .byte -104
      .byte 12
      .byte -102
      .byte 9
      .byte -74
      .byte 9
      .byte 83
      .byte 11
      .byte -100
      .byte 20
      .byte 95
      .byte 56
      .byte 104
      .byte -69
      .byte 54
      .byte 72
      .byte 90
      .byte 112
      .byte 9
      .byte -114
      .byte 122
      .byte -42
      .byte -10
      .byte -126
      .byte -37
      .byte -25
      .byte 35
      .byte 58
      .byte 80
      .byte -4
      .byte 91
      .byte 108
      .byte -13
      .byte -81
      .byte -29
      .byte 125
      .byte -42
      .byte 44
      .byte 79
      .byte 1
      .byte -100
      .byte 0
      .byte 95
      .byte 11
      .byte 104
      .byte 3
      .byte 54
      .byte 6
      .byte 90
      .byte 2
      .byte 9
      .byte 3
      .byte 122
      .byte 8
      .byte -10
      .byte 0
      .byte -37
      .byte 6
      .byte 35
      .byte 1
      .byte 80
      .byte 12
      .byte 91
      .byte 11
      .byte -13
      .byte 6
      .byte -29
      .byte 4
      .byte -42
      .byte 10
      .byte 79
      .byte 4
      .byte 68
      .byte -109
      .byte -125
      .byte 101
      .byte -118
      .byte 2
      .byte 82
      .byte -36
      .byte -102
      .byte 48
      .byte 64
      .byte -63
      .byte 8
      .byte -104
      .byte -3
      .byte 49
      .byte 8
      .byte -98
      .byte 26
      .byte -81
      .byte 46
      .byte -79
      .byte 13
      .byte 92
      .byte 10
      .byte -121
      .byte 40
      .byte -6
      .byte 117
      .byte 25
      .byte 58
      .byte 22
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
      .byte -126
      .byte 102
      .byte -126
      .byte 102
      .byte 66
      .byte -84
      .byte 66
      .byte -84
      .byte 79
      .byte 4
      .byte 79
      .byte 4
      .byte 61
      .byte -22
      .byte 61
      .byte -22
      .byte -126
      .byte 113
      .byte -126
      .byte 113
      .byte -7
      .byte 102
      .byte -7
      .byte 102
      .byte 45
      .byte -68
      .byte 45
      .byte -68
      .byte -60
      .byte 22
      .byte -60
      .byte 22
      .byte -126
      .byte 0
      .byte -126
      .byte 0
      .byte 66
      .byte 6
      .byte 66
      .byte 6
      .byte 79
      .byte 7
      .byte 79
      .byte 7
      .byte 61
      .byte 3
      .byte 61
      .byte 3
      .byte -126
      .byte 11
      .byte -126
      .byte 11
      .byte -7
      .byte 11
      .byte -7
      .byte 11
      .byte 45
      .byte 5
      .byte 45
      .byte 5
      .byte -60
      .byte 10
      .byte -60
      .byte 10
      .byte 75
      .byte 61
      .byte 75
      .byte 61
      .byte 75
      .byte 61
      .byte 75
      .byte 61
      .byte -40
      .byte 14
      .byte -40
      .byte 14
      .byte -40
      .byte 14
      .byte -40
      .byte 14
      .byte -109
      .byte -109
      .byte -109
      .byte -109
      .byte -109
      .byte -109
      .byte -109
      .byte -109
      .byte -85
      .byte 81
      .byte -85
      .byte 81
      .byte -85
      .byte 81
      .byte -85
      .byte 81
      .byte 75
      .byte 12
      .byte 75
      .byte 12
      .byte 75
      .byte 12
      .byte 75
      .byte 12
      .byte -40
      .byte 6
      .byte -40
      .byte 6
      .byte -40
      .byte 6
      .byte -40
      .byte 6
      .byte -109
      .byte 10
      .byte -109
      .byte 10
      .byte -109
      .byte 10
      .byte -109
      .byte 10
      .byte -85
      .byte 0
      .byte -85
      .byte 0
      .byte -85
      .byte 0
      .byte -85
      .byte 0
      .byte 115
      .byte 48
      .byte 115
      .byte 48
      .byte 115
      .byte 48
      .byte 115
      .byte 48
      .byte 115
      .byte 48
      .byte 115
      .byte 48
      .byte 115
      .byte 48
      .byte 115
      .byte 48
      .byte 44
      .byte -53
      .byte 44
      .byte -53
      .byte 44
      .byte -53
      .byte 44
      .byte -53
      .byte 44
      .byte -53
      .byte 44
      .byte -53
      .byte 44
      .byte -53
      .byte 44
      .byte -53
      .byte 115
      .byte 7
      .byte 115
      .byte 7
      .byte 115
      .byte 7
      .byte 115
      .byte 7
      .byte 115
      .byte 7
      .byte 115
      .byte 7
      .byte 115
      .byte 7
      .byte 115
      .byte 7
      .byte 44
      .byte 7
      .byte 44
      .byte 7
      .byte 44
      .byte 7
      .byte 44
      .byte 7
      .byte 44
      .byte 7
      .byte 44
      .byte 7
      .byte 44
      .byte 7
      .byte 44
      .byte 7
      .byte 103
      .byte -58
      .byte 103
      .byte -58
      .byte 103
      .byte 1
      .byte 103
      .byte 1
      .byte -116
      .byte -21
      .byte -116
      .byte -21
      .byte -116
      .byte 7
      .byte -116
      .byte 7
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 11
      .byte 123
      .byte 11
      .byte 123
      .byte 11
      .byte 10
      .byte 11
      .byte 10
      .byte -102
      .byte 57
      .byte -102
      .byte 57
      .byte -102
      .byte 11
      .byte -102
      .byte 11
      .byte -43
      .byte 52
      .byte -43
      .byte 52
      .byte -43
      .byte 52
      .byte -43
      .byte 52
      .byte -43
      .byte 52
      .byte -43
      .byte 52
      .byte -43
      .byte 52
      .byte -43
      .byte 52
      .byte -114
      .byte -49
      .byte -114
      .byte -49
      .byte -114
      .byte -49
      .byte -114
      .byte -49
      .byte -114
      .byte -49
      .byte -114
      .byte -49
      .byte -114
      .byte -49
      .byte -114
      .byte -49
      .byte -43
      .byte 5
      .byte -43
      .byte 5
      .byte -43
      .byte 5
      .byte -43
      .byte 5
      .byte -43
      .byte 5
      .byte -43
      .byte 5
      .byte -43
      .byte 5
      .byte -43
      .byte 5
      .byte -114
      .byte 5
      .byte -114
      .byte 5
      .byte -114
      .byte 5
      .byte -114
      .byte 5
      .byte -114
      .byte 5
      .byte -114
      .byte 5
      .byte -114
      .byte 5
      .byte -114
      .byte 5
      .byte 86
      .byte -82
      .byte 86
      .byte -82
      .byte 86
      .byte -82
      .byte 86
      .byte -82
      .byte 110
      .byte 108
      .byte 110
      .byte 108
      .byte 110
      .byte 108
      .byte 110
      .byte 108
      .byte 41
      .byte -15
      .byte 41
      .byte -15
      .byte 41
      .byte -15
      .byte 41
      .byte -15
      .byte -74
      .byte -62
      .byte -74
      .byte -62
      .byte -74
      .byte -62
      .byte -74
      .byte -62
      .byte 86
      .byte 12
      .byte 86
      .byte 12
      .byte 86
      .byte 12
      .byte 86
      .byte 12
      .byte 110
      .byte 2
      .byte 110
      .byte 2
      .byte 110
      .byte 2
      .byte 110
      .byte 2
      .byte 41
      .byte 6
      .byte 41
      .byte 6
      .byte 41
      .byte 6
      .byte 41
      .byte 6
      .byte -74
      .byte 0
      .byte -74
      .byte 0
      .byte -74
      .byte 0
      .byte -74
      .byte 0
      .byte 61
      .byte -23
      .byte 61
      .byte -23
      .byte -44
      .byte 67
      .byte -44
      .byte 67
      .byte 8
      .byte -103
      .byte 8
      .byte -103
      .byte 127
      .byte -114
      .byte 127
      .byte -114
      .byte -60
      .byte 21
      .byte -60
      .byte 21
      .byte -78
      .byte -5
      .byte -78
      .byte -5
      .byte -65
      .byte 83
      .byte -65
      .byte 83
      .byte 127
      .byte -103
      .byte 127
      .byte -103
      .byte 61
      .byte 2
      .byte 61
      .byte 2
      .byte -44
      .byte 7
      .byte -44
      .byte 7
      .byte 8
      .byte 1
      .byte 8
      .byte 1
      .byte 127
      .byte 1
      .byte 127
      .byte 1
      .byte -60
      .byte 9
      .byte -60
      .byte 9
      .byte -78
      .byte 5
      .byte -78
      .byte 5
      .byte -65
      .byte 6
      .byte -65
      .byte 6
      .byte 127
      .byte 12
      .byte 127
      .byte 12
      .byte -57
      .byte -23
      .byte -116
      .byte -26
      .byte -39
      .byte 5
      .byte -9
      .byte 120
      .byte -12
      .byte -93
      .byte -45
      .byte 78
      .byte -25
      .byte 80
      .byte -7
      .byte 97
      .byte 4
      .byte -50
      .byte -7
      .byte 103
      .byte -63
      .byte 62
      .byte 103
      .byte -49
      .byte -81
      .byte 35
      .byte 119
      .byte -3
      .byte 126
      .byte -102
      .byte -67
      .byte 108
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
      .byte -78
      .byte -2
      .byte 43
      .byte -45
      .byte 30
      .byte -126
      .byte 14
      .byte 80
      .byte -90
      .byte -109
      .byte -79
      .byte 3
      .byte -34
      .byte -59
      .byte 38
      .byte 24
      .byte 11
      .byte 125
      .byte -121
      .byte 41
      .byte -8
      .byte 113
      .byte -89
      .byte -113
      .byte -53
      .byte -73
      .byte -103
      .byte 68
      .byte -94
      .byte -57
      .byte 101
      .byte -21
      .byte -78
      .byte 8
      .byte 43
      .byte 2
      .byte 30
      .byte 8
      .byte 14
      .byte 6
      .byte -90
      .byte 1
      .byte -79
      .byte 0
      .byte -34
      .byte 11
      .byte 38
      .byte 6
      .byte 11
      .byte 12
      .byte -121
      .byte 4
      .byte -8
      .byte 9
      .byte -89
      .byte 10
      .byte -53
      .byte 6
      .byte -103
      .byte 9
      .byte -94
      .byte 1
      .byte 101
      .byte 12
      .byte -82
      .byte 43
      .byte 75
      .byte 52
      .byte 103
      .byte -56
      .byte 105
      .byte -85
      .byte 75
      .byte 51
      .byte 22
      .byte -18
      .byte 53
      .byte 90
      .byte 117
      .byte 21
      .byte 10
      .byte -127
      .byte 110
      .byte 118
      .byte -53
      .byte -74
      .byte 95
      .byte 49
      .byte -124
      .byte 78
      .byte 93
      .byte 72
      .byte 73
      .byte 76
      .byte -74
      .byte -50
      .byte -82
      .byte 1
      .byte 75
      .byte 3
      .byte 103
      .byte 3
      .byte 105
      .byte 0
      .byte 75
      .byte 2
      .byte 22
      .byte 12
      .byte 53
      .byte 11
      .byte 117
      .byte 6
      .byte 10
      .byte 3
      .byte 110
      .byte 12
      .byte -53
      .byte 5
      .byte 95
      .byte 4
      .byte -124
      .byte 2
      .byte 93
      .byte 1
      .byte 73
      .byte 1
      .byte -74
      .byte 12
      .byte 20
      .byte 3
      .byte 20
      .byte 3
      .byte 20
      .byte 7
      .byte 20
      .byte 7
      .byte 31
      .byte 110
      .byte 31
      .byte 110
      .byte 31
      .byte 110
      .byte 31
      .byte 110
      .byte 31
      .byte 110
      .byte 31
      .byte 110
      .byte 31
      .byte 110
      .byte 31
      .byte 110
      .byte -54
      .byte -66
      .byte -54
      .byte -66
      .byte -54
      .byte -66
      .byte -54
      .byte -66
      .byte -54
      .byte -66
      .byte -54
      .byte -66
      .byte -54
      .byte -66
      .byte -54
      .byte -66
      .byte 31
      .byte 1
      .byte 31
      .byte 1
      .byte 31
      .byte 1
      .byte 31
      .byte 1
      .byte 31
      .byte 1
      .byte 31
      .byte 1
      .byte 31
      .byte 1
      .byte 31
      .byte 1
      .byte -54
      .byte 0
      .byte -54
      .byte 0
      .byte -54
      .byte 0
      .byte -54
      .byte 0
      .byte -54
      .byte 0
      .byte -54
      .byte 0
      .byte -54
      .byte 0
      .byte -54
      .byte 0
      .byte -62
      .byte 41
      .byte -62
      .byte 41
      .byte -62
      .byte 41
      .byte -62
      .byte 41
      .byte 79
      .byte 5
      .byte 79
      .byte 5
      .byte 79
      .byte 5
      .byte 79
      .byte 5
      .byte 63
      .byte -44
      .byte 63
      .byte -44
      .byte 63
      .byte -44
      .byte 63
      .byte -44
      .byte -68
      .byte 121
      .byte -68
      .byte 121
      .byte -68
      .byte 121
      .byte -68
      .byte 121
      .byte -62
      .byte 3
      .byte -62
      .byte 3
      .byte -62
      .byte 3
      .byte -62
      .byte 3
      .byte 79
      .byte 8
      .byte 79
      .byte 8
      .byte 79
      .byte 8
      .byte 79
      .byte 8
      .byte 63
      .byte 7
      .byte 63
      .byte 7
      .byte 63
      .byte 7
      .byte 63
      .byte 7
      .byte -68
      .byte 5
      .byte -68
      .byte 5
      .byte -68
      .byte 5
      .byte -68
      .byte 5
      .byte 88
      .byte -110
      .byte 88
      .byte -110
      .byte -7
      .byte 94
      .byte -7
      .byte 94
      .byte -36
      .byte -42
      .byte -36
      .byte -42
      .byte 96
      .byte 34
      .byte 96
      .byte 34
      .byte -5
      .byte 71
      .byte -5
      .byte 71
      .byte -101
      .byte 34
      .byte -101
      .byte 34
      .byte 52
      .byte 104
      .byte 52
      .byte 104
      .byte -34
      .byte -64
      .byte -34
      .byte -64
      .byte 88
      .byte 10
      .byte 88
      .byte 10
      .byte -7
      .byte 3
      .byte -7
      .byte 3
      .byte -36
      .byte 2
      .byte -36
      .byte 2
      .byte 96
      .byte 2
      .byte 96
      .byte 2
      .byte -5
      .byte 6
      .byte -5
      .byte 6
      .byte -101
      .byte 1
      .byte -101
      .byte 1
      .byte 52
      .byte 12
      .byte 52
      .byte 12
      .byte -34
      .byte 6
      .byte -34
      .byte 6
      .byte -84
      .byte 77
      .byte -89
      .byte -111
      .byte -14
      .byte -63
      .byte 62
      .byte -35
      .byte 107
      .byte -111
      .byte 116
      .byte 35
      .byte 10
      .byte -118
      .byte 74
      .byte 71
      .byte 115
      .byte 52
      .byte -63
      .byte 54
      .byte 29
      .byte -114
      .byte 44
      .byte -50
      .byte -64
      .byte 65
      .byte -40
      .byte 16
      .byte -91
      .byte -95
      .byte 6
      .byte -70
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
      .byte 49
      .byte -122
      .byte 91
      .byte 99
      .byte 42
      .byte -29
      .byte 72
      .byte 95
      .byte 66
      .byte -82
      .byte -62
      .byte 42
      .byte -105
      .byte 94
      .byte 94
      .byte 66
      .byte 96
      .byte 40
      .byte 3
      .byte -31
      .byte 27
      .byte -88
      .byte -101
      .byte 42
      .byte -107
      .byte 123
      .byte -28
      .byte 111
      .byte -66
      .byte 93
      .byte -14
      .byte -69
      .byte 49
      .byte 3
      .byte 91
      .byte 2
      .byte 42
      .byte 5
      .byte 72
      .byte 7
      .byte 66
      .byte 8
      .byte -62
      .byte 4
      .byte -105
      .byte 9
      .byte 94
      .byte 8
      .byte 96
      .byte 8
      .byte 3
      .byte 8
      .byte 27
      .byte 7
      .byte -101
      .byte 9
      .byte -107
      .byte 12
      .byte -28
      .byte 3
      .byte -66
      .byte 3
      .byte -14
      .byte 5
      .byte 73
      .byte 79
      .byte 98
      .byte 8
      .byte -4
      .byte 59
      .byte -128
      .byte -127
      .byte 121
      .byte -25
      .byte -54
      .byte -59
      .byte -36
      .byte -44
      .byte -122
      .byte 56
      .byte 7
      .byte -84
      .byte 26
      .byte -79
      .byte -85
      .byte 90
      .byte -34
      .byte -69
      .byte -51
      .byte -94
      .byte -33
      .byte -80
      .byte 77
      .byte 30
      .byte 92
      .byte 90
      .byte 73
      .byte 4
      .byte 98
      .byte 2
      .byte -4
      .byte 7
      .byte -128
      .byte 1
      .byte 121
      .byte 12
      .byte -54
      .byte 7
      .byte -36
      .byte 0
      .byte -122
      .byte 6
      .byte 7
      .byte 7
      .byte 26
      .byte 3
      .byte -85
      .byte 9
      .byte -34
      .byte 1
      .byte -51
      .byte 11
      .byte -33
      .byte 3
      .byte 77
      .byte 7
      .byte 92
      .byte 6
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 7
      .byte 0
      .byte 1
      .byte 2
      .byte 3
      .byte 4
      .byte 5
      .byte 6
      .byte 15
      .byte 8
      .byte 9
      .byte 10
      .byte 11
      .byte 12
      .byte 13
      .byte 14
      .byte 23
      .byte 16
      .byte 17
      .byte 18
      .byte 19
      .byte 20
      .byte 21
      .byte 22
      .byte 31
      .byte 24
      .byte 25
      .byte 26
      .byte 27
      .byte 28
      .byte 29
      .byte 30
      .byte 1
      .byte 2
      .byte 3
      .byte 4
      .byte 5
      .byte 6
      .byte 7
      .byte 0
      .byte 9
      .byte 10
      .byte 11
      .byte 12
      .byte 13
      .byte 14
      .byte 15
      .byte 8
      .byte 17
      .byte 18
      .byte 19
      .byte 20
      .byte 21
      .byte 22
      .byte 23
      .byte 16
      .byte 25
      .byte 26
      .byte 27
      .byte 28
      .byte 29
      .byte 30
      .byte 31
      .byte 24
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
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
      .byte -126
      .byte -128
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
      .byte -118
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -118
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
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
      .byte 0
      .byte -128
      .byte 0
      .byte -128
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
      .byte -117
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -117
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
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
      .byte 1
      .byte 0
      .byte 0
      .byte -128
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
      .byte -127
      .byte -128
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
      .byte 9
      .byte -128
      .byte 0
      .byte 0
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
      .byte -118
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -118
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
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
      .byte -120
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
      .byte 9
      .byte -128
      .byte 0
      .byte -128
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
      .byte 10
      .byte 0
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
      .byte -128
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
      .byte -117
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -117
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
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
      .byte -119
      .byte -128
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
      .byte 3
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
      .byte 2
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
      .byte -128
      .byte 0
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
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 10
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
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
      .byte 10
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 10
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
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
      .byte -127
      .byte -128
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
      .byte -128
      .byte -128
      .byte 0
      .byte 0
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
      .byte 1
      .byte 0
      .byte 0
      .byte -128
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
      .byte 8
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 8
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 8
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 61
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 46
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 28
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 23
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 63
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 2
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 36
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 37
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 19
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 58
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 8
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 25
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 54
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 3
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 9
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 56
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 62
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 49
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 39
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 44
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 20
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 21
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 43
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 50
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 3
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 18
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 36
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 41
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 62
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 28
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 27
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 45
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 6
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 56
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 39
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 10
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 61
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 55
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 8
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 2
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 15
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 25
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 20
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 44
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 43
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 21
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 14
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
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
      .byte -126
      .byte -128
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
      .byte -118
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -118
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
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
      .byte 0
      .byte -128
      .byte 0
      .byte -128
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
      .byte -117
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -117
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
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
      .byte 1
      .byte 0
      .byte 0
      .byte -128
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
      .byte -127
      .byte -128
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
      .byte 9
      .byte -128
      .byte 0
      .byte 0
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
      .byte -118
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -118
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
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
      .byte -120
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
      .byte 9
      .byte -128
      .byte 0
      .byte -128
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
      .byte 10
      .byte 0
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
      .byte -128
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
      .byte -117
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte -117
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -128
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
      .byte -119
      .byte -128
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
      .byte 3
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
      .byte 2
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
      .byte -128
      .byte 0
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
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 10
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
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
      .byte 10
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 10
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
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
      .byte -127
      .byte -128
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
      .byte -128
      .byte -128
      .byte 0
      .byte 0
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
      .byte 1
      .byte 0
      .byte 0
      .byte -128
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
      .byte 8
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 8
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 8
      .byte -128
      .byte 0
      .byte -128
      .byte 0
      .byte 0
      .byte 0
      .byte -128
      .byte 0
      .byte 1
      .byte 4
      .byte 5
      .byte 8
      .byte 9
      .byte 12
      .byte 13
      .byte 2
      .byte 3
      .byte 6
      .byte 7
      .byte 10
      .byte 11
      .byte 14
      .byte 15
      .byte 3
      .byte 0
      .byte 0
      .byte 0
      .byte 2
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte -1
      .byte -1
      .byte -1
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte -1
      .byte -1
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 12
      .byte -1
      .byte -1
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte -1
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 14
      .byte -1
      .byte -1
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte 6
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 14
      .byte -1
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 6
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 6
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 12
      .byte 14
      .byte -1
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 4
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 6
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 4
      .byte 6
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 2
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 4
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 2
      .byte 4
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte -1
      .byte 0
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 2
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 0
      .byte 2
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte -1
      .byte 0
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte -1
      .byte 0
      .byte 2
      .byte 4
      .byte 6
      .byte 8
      .byte 10
      .byte 12
      .byte 14
      .byte 12
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 1
      .byte 0
      .byte 0
      .byte 4
      .byte 1
      .byte 0
      .byte 0
      .byte 4
      .byte 4
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 4
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 5
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 6
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 7
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 10
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 24
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 13
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 18
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 23
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 8
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 16
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 25
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 22
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 15
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 11
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 12
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 21
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 26
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 19
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 9
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 20
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 17
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 14
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 27
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte 0
      .byte -8
      .byte 31
      .byte -32
      .byte 127
      .byte 4
      .byte 52
      .byte 1
      .byte 13
      .byte 0
      .byte 8
      .byte -128
      .byte 0
      .byte 15
      .byte 0
      .byte -16
      .byte 0
      .byte -1
      .byte 15
      .byte -1
      .byte 3
      .byte 0
      .byte 16
      .byte 15
      .byte 0
      .byte 1
      .byte 16
      .byte 15
      .byte 0
      .byte 0
      .byte 2
      .byte 1
      .byte 0
      .byte 1
      .byte 2
      .byte -1
      .byte 3
      .byte 4
      .byte 5
      .byte -1
      .byte 6
      .byte 7
      .byte 8
      .byte -1
      .byte 9
      .byte 10
      .byte 11
      .byte -1
      .byte 4
      .byte 5
      .byte 6
      .byte -1
      .byte 7
      .byte 8
      .byte 9
      .byte -1
      .byte 10
      .byte 11
      .byte 12
      .byte -1
      .byte 13
      .byte 14
      .byte 15
      .byte -1
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
      .byte -5
      .byte 58
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
