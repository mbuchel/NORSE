#include <norse/boot.h>

.section .text
	# Multiboot header
	.align ALIGN
	.long MAGIC
	.long FLAGS
	.long CHECK

.section .bss, "aw", @nobits
	.align SMALL_PAGE

.global init_pl4
init_pl4:
	.skip SMALL_PAGE

.global init_pl3
init_pl3:
	.skip SMALL_PAGE

.global init_pl2
init_pl2:
	.skip SMALL_PAGE

.global init_pl1
init_pl1:
	.skip SMALL_PAGE

.global slab_higher_half
slab_higher_half:
	.skip SMALL_PAGE

.global init_physical_allocator_vector
init_physical_allocator_vector:
	.skip SMALL_PAGE

.global _init_stack_top
init_stack:
	.skip (16 * KB)
_init_stack_top:

.section .text

.global _start
.type _start, @function
.code32
_start:
	# Initialize stack pointer
	movl $virt_to_phys(_init_stack_top), %esp

	# Multiboot information in various registers
	pushl $0
	pushl %ebx

	movl $virt_to_phys(init_pl4), %edi
	movl %edi, %cr3

	# Page map Level 4
	movl $(virt_to_phys(init_pl3) + (PAGE_USER | PAGE_WRITE | PAGE_PRES)), virt_to_phys(init_pl4)
	movl $(virt_to_phys(init_pl3) + (PAGE_USER | PAGE_WRITE | PAGE_PRES)), virt_to_phys(init_pl4) + 511 * 8

	# Page map Level 3
	movl $(virt_to_phys(init_pl2) + (PAGE_USER | PAGE_WRITE | PAGE_PRES)), virt_to_phys(init_pl3)
	movl $(virt_to_phys(init_pl2) + (PAGE_USER | PAGE_WRITE | PAGE_PRES)), virt_to_phys(init_pl3) + 510 * 8

	# Page directory pointer table
	movl $(virt_to_phys(init_pl1) + (PAGE_WRITE | PAGE_PRES)), virt_to_phys(init_pl2)

	# Fractal mapping
	movl $(virt_to_phys(init_pl4) + (PAGE_WRITE | PAGE_PRES)), virt_to_phys(init_pl4) + FRACTAL_PAGE_TABLE_INDEX * 8
	movl $PAGE_EXEC_DISABLE_HIGH, virt_to_phys(init_pl4) + (FRACTAL_PAGE_TABLE_INDEX * 8) + 4

	# Page table (identity map first 4 MiB, except NULL), insecure code
	movl $(virt_to_phys(init_pl1) + 8), %edi
	movl $(0x1000 | PAGE_PRES | PAGE_WRITE), %esi
	movl $PAGE_EXEC_DISABLE_HIGH, %edx
	movl $0xFF, %ecx
1:
	movl %esi, (%edi)
	addl $4, %edi
	movl %edx, (%edi)
	addl $SMALL_PAGE, %esi
	addl $4, %edi
	loop 1b

	# Map kernel text section
	andl $MEM_WRITE_MASK, %esi
	movl $0, %edx
	movl $virt_to_phys(__text_end), %ecx
	subl $virt_to_phys(__text_start), %ecx
1:
	movl %esi, (%edi)
	addl $4, %edi
	movl %edx, (%edi)
	addl $SMALL_PAGE, %esi
	addl $4, %edi
	loop 1b

	# Map kernel data section
	orl $MEM_WRITE, %esi
	movl $PAGE_EXEC_DISABLE_HIGH, %edx
	movl $virt_to_phys(__data_end), %ecx
	subl $virt_to_phys(__data_start), %ecx
1:
	movl %esi, (%edi)
	addl $4, %edi
	movl %edx, (%edi)
	addl $SMALL_PAGE, %esi
	addl $4, %edi
	loop 1b

	# Map kernel bss section
	orl $MEM_WRITE, %esi
	movl $PAGE_EXEC_DISABLE_HIGH, %edx
	movl $virt_to_phys(__bss_end), %ecx
	subl $virt_to_phys(__bss_start), %ecx
1:
	movl %esi, (%edi)
	addl $4, %edi
	movl %edx, (%edi)
	addl $SMALL_PAGE, %esi
	addl $4, %edi
	loop 1b

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
	lgdt virt_to_phys(PHYS_GDT_REG)

	# Switch to 64-bit code seg
	ljmp $CODE_SEG, $virt_to_phys(long_mode)

.code64
long_mode:
	# Clear upper 32 bits of stack
	mov %esp, %esp

	# Load kernel data seg
	movw $DATA_SEG, %cx
	movw %cx, %ds
	movw %cx, %es
	movw %cx, %ss

	# Install Kernel stack
	mov $_init_stack_top, %rcx
	mov %rcx, TSS + 4

	# Finish installing TSS into GDT
	mov $TSS, %rcx
	movw %cx, GDT + TSS_SEG + 2
	shr $16, %rcx
	movb %cl, GDT + TSS_SEG + 4
	shr $8, %rcx
	movb %cl, GDT + TSS_SEG + 7
	shr $8, %rcx
	movl %ecx, GDT + TSS_SEG + 8

	# Move TSS register to TSS
	movw $(TSS_SEG | RPL), %cx
	ltr %cx

	# Move to higher half of kernel
	add $KERN_START, %rsp
	mov $higher_half, %rcx
	jmp *%rcx

higher_half:
	# Gets GDT pointer
	lgdt GDT_REG

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
