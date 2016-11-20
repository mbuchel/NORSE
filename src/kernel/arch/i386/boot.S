# boot straping and multiboot file
	.set ALIGN,1<<0
	.set MEMINFO,1<<1
	.set FLAGS,ALIGN|MEMINFO
	.set MAGIC,0x1BADB002
	.set CHECKSUM,-(MAGIC + FLAGS)

	.section .multiboot
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM

# Stack init
	.section .bss
	.align 16
	stack_bottom:
	.skip 16384
	stack_top:

	.section .text
	.global bootstrap
	.type bootstrap,@function
bootstrap:
	movl $stack_top,%esp

	call _init

	call kernel_main

# Paranoia it leaves the main kernel
	cli
1:	hlt
	jmp 1b

	.size bootstrap,.-bootstrap
