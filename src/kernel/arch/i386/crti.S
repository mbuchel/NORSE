	.section .init
	.clobal _init
	.type _init, @function

# crtbegin.o init section
_init:
	push %edp
	movl %esp, %ebp

	.section .fini
	.global _fini
	.type _fini, @function

# crtbegin.o fini section
_fini:
	push %ebp
	movl %esp, %ebp
