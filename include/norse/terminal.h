#ifndef TERM_H
#define TERM_H

#include <stddef.h>
#include <stdint.h>

enum background {
	BBLACK		= 0x00,
	BBLUE		= 0x10,
	BLIGHT_GREEN	= 0x20,
	BGREY		= 0x30,
	BRED		= 0x40,
	BPURPLE		= 0x50,
	BBROWN		= 0x60,
	BLIGHT_GREY	= 0x70,
	BDARK_GREY	= 0x80,
	BLIGHT_BLUE	= 0x90,
	BGREEN		= 0xA0,
	BCYAN		= 0xB0,
	BBRIGHT_RED	= 0xC0,
	BMAGENTA	= 0xD0,
	BYELLOW		= 0xE0,
	BWHITE		= 0xF0
};

enum foreground {
	FBLACK		= 0x00,
	FBLUE		= 0x01,
	FLIGHT_GREEN	= 0x02,
	FGREY		= 0x03,
	FRED		= 0x04,
	FPURPLE		= 0x05,
	FBROWN		= 0x06,
	FLIGHT_GREY	= 0x07,
	FDARK_GREY	= 0x08,
	FLIGHT_BLUE	= 0x09,
	FGREEN		= 0x0A,
	FCYAN		= 0x0B,
	FBRIGHT_RED	= 0x0C,
	FMAGENTA	= 0x0D,
	FYELLOW		= 0x0E,
	FWHITE		= 0x0F
};

// Intro functions
void clear_screen();
void print_intro();
void change_background_color(const enum background color);
void change_foreground_color(const enum foreground color);
void move_crz();
void set_crz_x(uint8_t x);
void set_crz_y(uint8_t y);
void term_putc(const char c);
void term_print(const char *str);

#endif
