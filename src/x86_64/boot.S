#include <norse/boot.h>

.section .text
	# Multiboot header
	.align ALIGN
	.long MAGIC
	.long FLAGS
	.long CHECK

.section .bss, "aw", @nobits
	.align SMALL_PAGE
init_pl4:
	.skip SMALL_PAGE
init_pl3:
	.skip SMALL_PAGE
init_pl2:
	.skip SMALL_PAGE
init_pl1_a:
	.skip SMALL_PAGE
init_pl1_b:
	.skip SMALL_PAGE
frac_pl3:
	.skip SMALL_PAGE
frac_pl2:
	.skip SMALL_PAGE
frac_pl1:
	.skip SMALL_PAGE
physpm13:
	.skip SMALL_PAGE

.global slab_higher_half
slab_higher_half:
	.skip SMALL_PAGE

.global slab_lower_half
slab_lower_half:
	.skip SMALL_PAGE

.global init_physical_allocator_vector
init_physical_allocator_vector:
	.skip SMALL_PAGE

init_stack:
	.skip (16 * KB)
_init_stack_top:

.section .text

.global _start
.type _start, @function
.code32
_start:
	# Initialize stack pointer
	movl $_init_stack_top, %esp

	# Install kernel into TSS
	movl %esp, TSS + 4
	movl $0, TSS + 8

	# Install TSS into GDT
	movl $TSS, %ecx
	movw %cx, GDT + TSS_SEG + 2
	shrl $16, %ecx
	movb %cl, GDT + TSS_SEG + 4
	shrl $8, %ecx
	movb %cl, GDT + TSS_SEG + 7
	movl $0, GDT + TSS_SEG + 8

	# Multiboot information in various registers
	pushl $0
	pushl %ebx

	movl $init_pl4, %edi
	movl %edi, %cr3

	# Page map Level 4
	movl $(init_pl3 + (PAGE_USER | PAGE_WRITE | PAGE_PRES)), init_pl4

	# Page directory pointer table
	movl $(init_pl2 + (PAGE_USER | PAGE_WRITE | PAGE_PRES)), init_pl3

	# Page directory
	movl $(init_pl1_a + (PAGE_WRITE | PAGE_PRES)), init_pl2
	movl $(init_pl1_b + (PAGE_WRITE | PAGE_PRES)), init_pl2 + 8

	# Page table (identity map first 4 MiB, except NULL), insecure code
	movl $(init_pl1_a + 8), %edi
	movl $(0x1000 | PAGE_PRES | PAGE_WRITE), %esi
	movl $1023, %ecx
1:
	movl %esi, (%edi)
	addl $SMALL_PAGE, %esi
	addl $8, %edi
	loop 1b

	# Fractal mapping
	movl $(init_pl4 + (PAGE_WRITE | PAGE_PRES)), init_pl4 + 511 * 8
	movl $(frac_pl3 + (PAGE_WRITE | PAGE_PRES)), init_pl4 + 510 * 8
	movl $(init_pl4 + (PAGE_WRITE | PAGE_PRES)), frac_pl3 + 511 * 8
	movl $(frac_pl2 + (PAGE_WRITE | PAGE_PRES)), frac_pl3 + 510 * 8
	movl $(init_pl4 + (PAGE_WRITE | PAGE_PRES)), frac_pl2 + 511 * 8
	movl $(frac_pl1 + (PAGE_WRITE | PAGE_PRES)), frac_pl2 + 510 * 8
	movl $(init_pl4 + (PAGE_WRITE | PAGE_PRES)), frac_pl1 + 511 * 8

	# Enable PAE
	movl %cr4, %eax
	orl $CR4_PAE, %eax
	movl %eax, %cr4

	# Enable long mode and no execute bit
	movl $IA32_EFER_MSR, %ecx
	rdmsr
	orl $0x900, %eax
	wrmsr

	# Enable paging and enter long mode
	movl %cr0, %eax
	orl $(CR0_PAGING | CR0_WRITE_PROTECT), %eax
	movl %eax, %cr0

paging:
	# Load GDT register
	subl $6, %esp
	movw (GDT_SIZE - 1), %cx
	movw %cx, 0(%esp)
	movl $GDT, %ecx
	movl %ecx, 2(%esp)
	lgdt 0(%esp)
	addl $6, %esp

	# Switch to 64-bit code seg
	ljmp $CODE_SEG, $long_mode

.code64
long_mode:
	# Clear upper 32 bits of stack
	mov %esp, %esp

	# Load kernel data seg
	movw $DATA_SEG, %cx
	movw %cx, %es
	movw %cx, %ss

	# Move TSS register to TSS
	movw $(TSS_SEG | RPL), %cx
	ltr %cx

	# Switch to thread local fs, and gs
	movw $(USER_DATA_SEG | RPL), %cx
	movw %cx, %fs
	movw %cx, %gs

	# Multiboot information
	pop %rdi
	call kernel_main

	jmp halt

.global halt
.type halt, @function
halt:
	cli
	hlt
	jmp halt
