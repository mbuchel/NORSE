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

int install_interrupt(const uint8_t num, isr_f func);

extern void disable_interrupts();
extern void enable_interrupts();
extern void halt();

#endif
