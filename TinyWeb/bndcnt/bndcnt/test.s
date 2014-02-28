	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp2:
	.cfi_def_cfa_offset 16
Ltmp3:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp4:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	leaq	-24(%rbp), %rax
	movabsq	$0, %rcx
	leaq	_count(%rip), %rdx
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rax, %rdi
	movq	%rcx, %rsi
	callq	_Pthread_create
	leaq	-32(%rbp), %rdi
	movabsq	$0, %rax
	leaq	_count(%rip), %rdx
	movq	%rax, %rsi
	movq	%rax, %rcx
	callq	_Pthread_create
	movabsq	$0, %rsi
	movq	-24(%rbp), %rdi
	callq	_Pthread_join
	movabsq	$0, %rsi
	movq	-32(%rbp), %rdi
	callq	_Pthread_join
	cmpl	$20000, _cnt(%rip)      ## imm = 0x4E20
	je	LBB0_2
## BB#1:
	leaq	L_.str(%rip), %rdi
	movl	_cnt(%rip), %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -36(%rbp)         ## 4-byte Spill
	jmp	LBB0_3
LBB0_2:
	leaq	L_.str1(%rip), %rdi
	movl	_cnt(%rip), %esi
	movb	$0, %al
	callq	_printf
	movl	%eax, -40(%rbp)         ## 4-byte Spill
LBB0_3:
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.cfi_endproc

	.globl	_count
	.align	4, 0x90
_count:                                 ## @count
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp7:
	.cfi_def_cfa_offset 16
Ltmp8:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp9:
	.cfi_def_cfa_register %rbp
	movq	%rdi, -16(%rbp)
	movl	$0, -20(%rbp)
## BB#1:
	cmpl	$10000, -20(%rbp)       ## imm = 0x2710
	jge	LBB1_3
## BB#2:
	movl	_cnt(%rip), %eax
	addl	$1, %eax
	movl	%eax, _cnt(%rip)
	movq	$0, -8(%rbp)
	jmp	LBB1_4
LBB1_3:
	movq	$0, -8(%rbp)
LBB1_4:
	movq	-8(%rbp), %rax
	popq	%rbp
	ret
	.cfi_endproc

	.globl	_cnt                    ## @cnt
.zerofill __DATA,__common,_cnt,4,2
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	 "BOOM! cnt = %d \n"

L_.str1:                                ## @.str1
	.asciz	 "ok cnt = %d\n"


.subsections_via_symbols
