#ifndef CPU_NORSE_H
#define CPU_NORSE_H

#include <norse/types.h>

struct cpu_state;

typedef void (isr_f)(struct cpu_state *);

void init_interrupts();

int install_interrupt(uint8_t num, isr_f func);

extern void disable_interrupts();
extern void enable_interrupts();
extern void halt();

#endif
