#ifndef PANIC_NORSE_H
#define PANIC_NORSE_H

#include <norse/cpu.h>

#define panic() {disable_interrupts(); halt();}

#endif
