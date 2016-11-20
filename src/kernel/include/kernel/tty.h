#ifndef __KERNEL_TTY_H
#define __KERNEL_TTY_H

#include <stddef.h>

void term_init();
void term_putchar(char c);
void term_write(const char* data);

#endif
