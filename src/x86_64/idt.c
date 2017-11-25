#include <norse/cpu.h>
#include <norse/types.h>
#include <norse/panic.h>
#include <norse/boot.h>

#include <arch/ports.h>

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
