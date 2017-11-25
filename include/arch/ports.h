#ifndef PORTS_H
#define PORTS_H

#include <norse/types.h>

/*
 * write_port - sends a byte to a port
 * @data - byte to send
 * @port - port to send to
 */
static inline void write_port(uint8_t data, uint16_t port)
{
	__asm__ __volatile__(
		"outb %1, %0"
		:
		:"dn"(port), "a"(data)
	);
}

/*
 * write_portw - write a word to a port
 * @data - word to send
 * @port - port to send to
 */
static inline void write_portw(uint16_t data, uint16_t port)
{
	__asm__ __volatile__(
		"outw %1, %0"
		:
		:"dn"(port), "a"(data)
	);
}

/*
 * write_portl - write a double word to a port
 * @data - double word to send
 * @port - port to send to
 */
static inline void write_portl(uint32_t data, uint16_t port)
{
	__asm__ __volatile__(
		"outl %1, %0"
		:
		:"dn"(port), "a"(data)
	);
}

/*
 * read_port - reads a byte from a port
 * @port - port to read from
 */
static inline uint8_t read_port(uint16_t port)
{
	uint8_t ret;

	__asm__ __volatile__(
		"inb %1, %0"
		:"=a"(ret)
		:"dn"(port)
	);
	
	return ret;
}

/*
 * read_portw - reads a word from a port
 * @port - port to read from
 */
static inline uint16_t read_portw(uint16_t port)
{
	uint16_t ret;

	__asm__ __volatile__(
		"inw %1, %0"
		:"=a"(ret)
		:"dn"(port)
	);

	return ret;
}

/*
 * read_portl - read a double word from a port
 * @port - port to read from
 */
static inline uint32_t read_portl(uint16_t port)
{
	uint32_t ret;

	__asm__ __volatile__(
		"inl %1, %0"
		:"=a"(ret)
		:"dn"(port)
	);

	return ret;
}

#endif
