#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

int putchar(int c)
{
#if defined(__is_libk)
	term_putchar((char)c);
#else
	// TODO: Sys call
#endif
	return c;
}
