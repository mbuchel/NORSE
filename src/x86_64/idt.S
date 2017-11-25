# Intel is easier for this section then AT&T
.intel_syntax noprefix

.section .text

# idt_load - loads the interrupt descriptor table
.global idt_load
.type idt_load, @function
idt_load:
	mov rax, [rsp]
	lidt [rdi]
	mov rax, [rsp]
	sti
	ret

# disable_interrupts - disables interrupts
.global disable_interrupts
.type disable_interrupts, @function
disable_interrupts:
	cli
	ret

# enable_interrupts - enables interrupts
.global enable_interrupts
.type enable_interrupts, @function
enable_interrupts:
	sti
	ret

# serve_interrupt - serves interrupt
.global serve_interrupt
.type serve_interrupt, @function
serve_interrupt:
	pushf
	mov ax, ds
	push rax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	# using a common interrupt handler by setting all interrupts into a function array in idt_load
	.extern common_interrupt_handler
	call common_interrupt_handler

	pop rax

	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popf
	add rsp, 8
	sti
	iret

.macro interrupt number
	.global isr\number
	.type isr\number, @function
	isr\number:
		cli
		push \number
		jmp serve_interrupt
.endm

.macro no_error_code_interrupt number
	.global isr\number
	.type isr\number, @function
	isr\number:
		cli
		push 0
		push \number
		jmp serve_interrupt
.endm

no_error_code_interrupt 0
no_error_code_interrupt 1
no_error_code_interrupt 2
no_error_code_interrupt 3
no_error_code_interrupt 4
no_error_code_interrupt 5
no_error_code_interrupt 6
no_error_code_interrupt 7
interrupt 8
no_error_code_interrupt 9
interrupt 10
interrupt 11
interrupt 12
interrupt 13
interrupt 14
no_error_code_interrupt 15
no_error_code_interrupt 16
interrupt 17
no_error_code_interrupt 18
no_error_code_interrupt 19
no_error_code_interrupt 20
no_error_code_interrupt 21
no_error_code_interrupt 22
no_error_code_interrupt 23
no_error_code_interrupt 24
no_error_code_interrupt 25
no_error_code_interrupt 26
no_error_code_interrupt 27
no_error_code_interrupt 28
no_error_code_interrupt 29
interrupt 30
no_error_code_interrupt 31
no_error_code_interrupt 32
no_error_code_interrupt 33
no_error_code_interrupt 34
