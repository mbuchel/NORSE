#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t	VGA_WIDTH	=	0x50;
static const size_t	VGA_HEIGHT	=	0x19;
static uint16_t* const	VGA_MEMORY	=	(uint16_t*) 0xB8000;

static size_t		term_row;
static size_t		term_column;
static uint8_t		term_color;
static uint16_t*	term_buffer;

// Clear the terminal screen
void term_init()
{
	size_t x;
	size_t y;

	term_row = 0;
	term_column = 0;

	term_color = vga_entry_color(RED, BLACK);
	term_buffer = VGA_MEMORY;

	for (y = 0; y < VGA_HEIGHT; ++y) {
		for (x = 0; x < VGA_WIDTH; ++x) {
			const size_t i = y * VGA_WIDTH + x;
			term_buffer[i] = vga_entry(' ', term_color);
		}
	}
}

// Sets the terminal color
void term_setcolor(uint8_t color)
{
	term_color = color;
}

// Puts a single character to the screen
void term_putchar(char c)
{
	const size_t i = y * VGA_WIDTH + x;
	term_buffer[i] = vga_entry((unsigned char) c, color);

	if (++term_column == VGA_WIDTH) {
		term_column = 0;
		if (++term_row == VGA_HEIGHT)
			term_row = 0;
	}
}

// Writes a string to the screen
void term_write(const char* data)
{
	size_t i;

	for (i = 0; data[i]; ++i)
		term_putchar(data[i]);
}
