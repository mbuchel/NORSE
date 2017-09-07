// Currently a 32 bit os, felt that it will be easier to make that then port

.set MAGIC_NUM, 0x1BADB002
.set MULTIFLAG, (1 << 0) | (1 << 1) // Load modules on page boundaries and provide memory map
.set CHECK_SUM, - (MAGIC_NUM + MULTIFLAG)

.code32
.section .multiboot
	.align 0x04
	.long MAGIC_NUM
	.long MULTIFLAG
	.long CHECK_SUM
.section .bss
	.align 0x10 // For C stack
	stack_bottom:
		.skip 1024
	stack_top:

.section .text
.global bootstrap
.type bootstrap, @function
.extern kmain
bootstrap:
	mov $stack_top, %esp
	push %ebx
	cli
	call kmain

// In case there is a mistake and we need to stop program
.global PARANIOA
.type PARANIOA, @function
PARANIOA:
	cli
	hlt
	jmp PARANIOA
