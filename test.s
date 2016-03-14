	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 10
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
Ltmp3:
	.cfi_offset %rbx, -24
	movq	%rsi, %rbx
	leaq	L_str(%rip), %rdi
	callq	_puts
	movq	(%rbx), %rcx
	movzbl	(%rcx), %edx
	xorl	%eax, %eax
	cmpl	$8, %edx
	je	LBB0_5
## BB#1:                                ## %if.then
	movzbl	1(%rcx), %edx
	cmpl	$21, %edx
	je	LBB0_5
## BB#2:                                ## %if.then12
	movzbl	2(%rcx), %edx
	cmpl	$44, %edx
	jne	LBB0_5
## BB#3:                                ## %if.then20
	movzbl	3(%rcx), %ecx
	cmpl	$32, %ecx
	jne	LBB0_5
## BB#4:                                ## %if.then28
	leaq	L_str2(%rip), %rdi
	callq	_puts
	movl	$1, %eax
LBB0_5:                                 ## %return
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_str:                                  ## @str
	.asciz	"HELLO WORLD"

L_str2:                                 ## @str2
	.asciz	"it was just.."


.subsections_via_symbols
