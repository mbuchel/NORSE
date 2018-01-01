#include <norse/cpu.h>
#include <norse/types.h>
#include <norse/panic.h>
#include <norse/boot.h>
#include <norse/lock.h>

#include <arch/ports.h>

#define IDT_SIZE 256

/*
 * Locations of PIC ports on x86_64
 */
static const uint8_t PIC_MASTER_CTRL		= 0x20;
static const uint8_t PIC_MASTER_MASK		= 0x21;
static const uint8_t PIC_SLAVE_CTRL		= 0xA0;
static const uint8_t PIC_SLAVE_MASK		= 0xA1;

/*
 * idt_entry - idt entry structure
 * @base_lo - lower half of the pointer to the code
 * @base_hi - higher half of the pointer to the code
 * @sel - gdt segment that causes this interrupt call
 * @always0 - reserved
 * @flags - present flags and privilege level
 *
 * NOTE: more info in volume 3, chapter 6 intel manual
 */
static struct idt_entry {
	uint16_t base_lo;
	uint16_t sel;
	uint8_t always0;
	uint8_t flags;
	uint16_t base_hi;
} __attribute__((packed)) idt[IDT_SIZE] = {{0}};

/*
 * idt_ptr - pointer to the idt
 * @limit - size of whole idt
 * @base - pointer to start address of idt
 */
static struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idtp;

extern void idt_load(struct idt_ptr);

extern void serve_interrupt();

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();

// Array of interrupts
static isr_f INTERRUPT_DISPATCH[IDT_SIZE] = {
	isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
	isr8, isr9, isr10, isr11, isr12, isr13, isr14,
	isr15, isr16, isr17, isr18, isr19, isr20, isr21,
	isr21, isr22, isr23, isr24, isr25, isr26, isr27,
	isr28, isr29, isr30, isr31, isr32, isr33, isr34
};

/*
 * idt_set - sets an entry in the INTERRUPT_DISPATCH
 * @num - number of interrupt
 * @base - pointer to base
 * @sel - gdt condition for interrupt
 * @flags - flags that trigger
 */
static void idt_set(uint8_t num, uintptr_t base, uint16_t sel, uint8_t flags)
{
	idt[num].base_lo = base & 0xFFFF;
	idt[num].base_hi = (base >> 0x10) & 0xFFFF;
	idt[num].always0 = 0;
	idt[num].sel = sel;
	idt[num].flags = flags;
}

/*
 * init_interrupts - inits the interrupt table
 *
 * NOTE: there are 2 PICs a master and slave and
 * each has a dedicated I/O port, interrupts 0-31
 * are CPU exceptions. The main interrupts I am
 * using are 32 (programmable timer) and 33 (keyboard).
 */
void init_interrupts()
{
	idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
	idtp.base = (uintptr_t) &idt;

	for (uint16_t i = 0; INTERRUPT_DISPATCH[i] != NULL; ++i)
		idt_set(i, (uintptr_t) INTERRUPT_DISPATCH[i], 0x08, 0x8E);

	// ICW1 - begin init
	write_port(PIC_MASTER_CTRL, 0x11);
	write_port(PIC_SLAVE_CTRL, 0x11);

	// Remap beyond 0x20 because first 32 are cpu exceptions
	write_port(PIC_MASTER_MASK, 0x21);
	write_port(PIC_SLAVE_CTRL, 0x28);

	// ICW3 - setup cascading
	write_port(PIC_MASTER_MASK, 0x00);
	write_port(PIC_SLAVE_MASK, 0x00);

	// ICW4 - environment info
	write_port(PIC_MASTER_MASK, 0x01);
	write_port(PIC_SLAVE_MASK, 0x01);

	// mask interrupts
	write_port(PIC_MASTER_MASK, 0xFF);
	write_port(PIC_SLAVE_MASK, 0xFF);

	idt_load(idtp);
}

/*
 * common_interrupt_handler - generic handler for all interrupts
 * @r - cpu state
 */
void common_interrupt_handler(struct cpu_state r)
{
	if (r.int_no < IDT_SIZE && INTERRUPT_DISPATCH[r.int_no] != NULL)
		INTERRUPT_DISPATCH[r.int_no](&r);
}

/*
 * interrupt_register_handler - registers an interrupt
 * @irq_num - number of interrupt
 * @handler - handler to set
 */
void interrupt_register_handler(size_t irq_num, isr_f handler)
{
	if (irq_num < IDT_SIZE && INTERRUPT_DISPATCH[irq_num] == NULL)
		INTERRUPT_DISPATCH[irq_num] = handler;
}

/*
 * interrupt_unregister_handler - unregisters an interrupt
 * @irq_num - number of interrupt
 */
void interrupt_unregister_handler(size_t irq_num)
{
	if (irq_num < IDT_SIZE && INTERRUPT_DISPATCH[irq_num] == NULL)
		INTERRUPT_DISPATCH[irq_num] = NULL;
}
