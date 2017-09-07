#include <x86_64/system.h>

/*
 * outportb - writes a byte to an output port
 * @port - port number
 * @data - data to write
 */
void outportb(uint16_t port, uint8_t data)
{
	__asm__ __volatile__ (
		"outb %1, %0"
		:
		: "dN"(port), "a"(data)
	);
}

/*
 * inportb - gets a byte from an input port
 * @port - port number
 */
uint8_t inportb(uint16_t port)
{
	uint8_t ret;
	__asm__ __volatile__ (
		"inb %1, %0"
		: "=a"(ret)
		: "dN"(port)
	);
	return ret;
}
