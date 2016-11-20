#include <stdio.h>
#include <kernel/tty.h>

void kernel_main()
{
	term_init();
	printf("Hello\n");
}
