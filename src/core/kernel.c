#include <device/keyboard.h>
#include <device/vga.h>
#include <norse/cpu.h>
#include <norse/file.h>

/*
 * kernel_main - main function
 * @multiboot_table - multiboot table
 */
void kernel_main(void *multiboot __attribute__((unused)))
{
	init_interrupts();
	file_system_init();
	//vga_init();
	//keyboard_init();

	while (1);
}
