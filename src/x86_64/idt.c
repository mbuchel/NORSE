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

// Protects the INTERRUPT_DISPATCH
static spinlock_t idt_dispatch_lock = SPINLOCK_INIT;

// Array of interrupts
static isr_f INTERRUPT_DISPATCH[IDT_SIZE] = {0};

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

/*
 * idt_set - sets an entry in the INTERRUPT_DISPATCH
 * @num - number of interrupt
 * @base - pointer to base
 * @sel - gdt condition for interrupt
 * @flags - flags that trigger
 */
static void idt_set(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt[num].base_lo = base & 0xFFFF;
	idt[num].base_hi = (base >> 0x10) & 0xFFFF;
	idt[num].always0 = 0;
	idt[num].sel = sel;
	idt[num].flags = flags;
}

/*
 * install_interrupt - installs the interrupt function
 * @num - number for the interrupt
 * @func - function for interrupt
 *
 * returns -1 if an interrupt is already installed
 */
int install_interrupt(const uint8_t num, isr_f func)
{
	int ret = 0;

	acquire_spinlock(&idt_dispatch_lock);
	if (INTERRUPT_DISPATCH[num] != NULL)
		ret = -1;
	else
		INTERRUPT_DISPATCH[num] = func;
	release_spinlock(&idt_dispatch_lock);

	return ret;
}

/*
 * init_interrupts - inits the interrupt table
 *
 * NOTE: there are 2 PICs a master and slave and
 * each has a dedicated I/O port, interrupts 0-31
 * are CPU exceptions. The main interrupts I am
 * using are 32 (programmable timer) and 33 (keyboard).
 *
 * WARNING: there is a warning here due to cast to integer
 * the pointer for the function has to be a 32 bit number as
 * according to the intel manual volume 3, chapter 6
 */
void init_interrupts()
{
	idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
	idtp.base = (uint32_t) &idt;

	idt_set(0, (uint32_t)isr0, 0x08, 0x8e);
	idt_set(1, (uint32_t)isr1, 0x08, 0x8e);
	idt_set(2, (uint32_t)isr2, 0x08, 0x8e);
	idt_set(3, (uint32_t)isr3, 0x08, 0x8e);
	idt_set(4, (uint32_t)isr4, 0x08, 0x8e);
	idt_set(5, (uint32_t)isr5, 0x08, 0x8e);
	idt_set(6, (uint32_t)isr6, 0x08, 0x8e);
	idt_set(7, (uint32_t)isr7, 0x08, 0x8e);
	idt_set(8, (uint32_t)isr8, 0x08, 0x8e);
	idt_set(9, (uint32_t)isr9, 0x08, 0x8e);
	idt_set(10, (uint32_t)isr10, 0x08, 0x8e);
	idt_set(11, (uint32_t)isr11, 0x08, 0x8e);
	idt_set(12, (uint32_t)isr12, 0x08, 0x8e);
	idt_set(13, (uint32_t)isr13, 0x08, 0x8e);
	idt_set(14, (uint32_t)isr14, 0x08, 0x8e);
	idt_set(15, (uint32_t)isr15, 0x08, 0x8e);
	idt_set(16, (uint32_t)isr16, 0x08, 0x8e);
	idt_set(17, (uint32_t)isr17, 0x08, 0x8e);
	idt_set(18, (uint32_t)isr18, 0x08, 0x8e);
	idt_set(19, (uint32_t)isr19, 0x08, 0x8e);
	idt_set(20, (uint32_t)isr20, 0x08, 0x8e);
	idt_set(21, (uint32_t)isr21, 0x08, 0x8e);
	idt_set(22, (uint32_t)isr22, 0x08, 0x8e);
	idt_set(23, (uint32_t)isr23, 0x08, 0x8e);
	idt_set(24, (uint32_t)isr24, 0x08, 0x8e);
	idt_set(25, (uint32_t)isr25, 0x08, 0x8e);
	idt_set(26, (uint32_t)isr26, 0x08, 0x8e);
	idt_set(27, (uint32_t)isr27, 0x08, 0x8e);
	idt_set(28, (uint32_t)isr28, 0x08, 0x8e);
	idt_set(29, (uint32_t)isr29, 0x08, 0x8e);
	idt_set(30, (uint32_t)isr30, 0x08, 0x8e);
	idt_set(31, (uint32_t)isr31, 0x08, 0x8e);
	idt_set(32, (uint32_t)isr32, 0x08, 0x8e);
	idt_set(33, (uint32_t)isr33, 0x08, 0x8e);
	idt_set(34, (uint32_t)isr34, 0x08, 0x8e);

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
	if (r.int_no < IDT_SIZE && INTERRUPT_DISPATCH[r.int_no] != NULL) {
		INTERRUPT_DISPATCH[r.int_no](&r);
	}
}
