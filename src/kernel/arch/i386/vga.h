#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>

// Enumeration to have easier color definitions
enum vga_color {
	BLACK		=	0,
	BLUE		=	1,
	GREEN		=	2,
	CYAN		=	3,
	RED		=	4,
	MAGENTA		=	5,
	BROWN		=	6,
	LIGHT_GREY	=	7,
	DARK_GREY	=	8,
	LIGHT_BLUE	=	9,
	LIGHT_GREEN	=	10,
	LIGHT_CYAN	=	11,
	LIGHT_RED	=	12,
	LIGHT_MAGENTA	=	13,
	LIGHT_BROWN	=	14,
	WHITE		=	15
};

// Static inline methods for easier vga manipulations
static inline uint8_t vga_entry_color(enum vga_color front, enum vga_color back)
{
	return front | back << 4;
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color)
{
	return (uint16_t) c |(uint16_t) color << 8;
}

#endif
