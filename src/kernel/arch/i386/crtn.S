# crtend.o init section
	.section .init

	popl %ebp
	ret

# crtend.o fini section
	.section .fini

	popl %ebp
	ret
