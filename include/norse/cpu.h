#ifndef CPU_NORSE_H
#define CPU_NORSE_H

#include <norse/types.h>

/*
 * cpu_state - saves the current state of the cpu registers
 * @ds - pushed the segs last
 * @edi, @esi, @edp, @esp, @ebx, @edx, @ecx, @eax - all pushed by pusha
 * @int_no, @err_code - push byte # does this
 * @eip, @cs, @eflags, @useresp, @ss - automatically pushed
 */
struct cpu_state {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
};

typedef void (*isr_f)(struct cpu_state *);

void init_interrupts();
void interrupt_register_handler(size_t irq_num, isr_f handler);
void interrupt_unregister_handler(size_t irq_num);

void disable_interrupts();
void enable_interrupts();
void halt();

#endif
