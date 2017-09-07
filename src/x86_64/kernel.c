#include <norse/multiboot.h>
#include <norse/terminal.h>

void kmain(struct multiboot *boot_ptr)
{
	uint32_t initrd_location;

	clear_screen();
	print_intro();
}
