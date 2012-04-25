	.file	"tt.cl"
	.section	.rodata.cst4,"aM",@progbits,4
	.align	4
.LCPI0_0:
	.long	1078530000
	.text
	.globl	test
	.align	16, 0x90
	.type	test,@function
test:
	pushl	%ebx
	pushl	%esi
	pushl	%eax
	calll	.L0$pb
.L0$pb:
	popl	%esi
.Ltmp0:
	addl	$_GLOBAL_OFFSET_TABLE_+(.Ltmp0-.L0$pb), %esi
	movl	$0, (%esp)
	movl	%esi, %ebx
	calll	get_global_id@PLT
	movl	16(%esp), %ecx
	flds	(%ecx,%eax,4)
	fld	%st(0)
	fmuls	.LCPI0_0@GOTOFF(%esi)
	faddp	%st(1)
	movl	20(%esp), %ecx
	fstps	(%ecx,%eax,4)
	addl	$4, %esp
	popl	%esi
	popl	%ebx
	ret
.Ltmp1:
	.size	test, .Ltmp1-test


	.section	".note.GNU-stack","",@progbits
