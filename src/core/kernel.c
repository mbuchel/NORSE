#include <norse/cpu.h>

/*
 * kernel_main - main function
 * @multiboot_table - multiboot table
 */
void kernel_main(void *multiboot)
{
	init_interrupts();
}
