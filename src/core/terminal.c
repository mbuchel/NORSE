#include <norse/terminal.h>
#include <x86_64/system.h>

// Some constants
static const char *intro_msg	= "Welcome to NORSE\n\tVersion: 0.0.1\n\tSHELL MANIA\n";
static const int VGA_COLS	= 0x50;
static const int VGA_ROWS	= 0x19;

static uint16_t *vga_buf	= (uint16_t*) 0xB8000;

static enum background bcolor	= BBLACK;
static enum foreground fcolor	= FWHITE;

static uint8_t term_col		= 0;
static uint8_t term_row		= 0;
static uint16_t term_color	= (BBLACK | FWHITE) << 8;

/*
 * get_char_color - does the operation to get the char color
 * @c - character to be colored
 */
static inline uint16_t get_char_color(const char c)
{
	return (term_color | c);
}

/*
 * update_term_color - updates the term color variable
 */
static inline void update_term_color()
{
	term_color = (bcolor | fcolor) << 8;
}

/*
 * term_putc_private - small helper function to put a character on the screen
 * @row - row to put it on
 * @col - col to put it on
 * @c - character to place
 */
static inline void term_putc_private(const int row, const int col, const char c)
{
	vga_buf[(VGA_COLS * row) + col] = get_char_color(c);
}

/*
 * set_crz_x - sets x cruser location
 * @x - location
 */
void set_crz_x(uint8_t x)
{
	term_col = x;
	move_crz();
}

/*
 * set_crz_y - sets y cruser location
 * @y - location
 */
void set_crz_y(uint8_t y)
{
	term_row = y;
	move_crz();
}

/*
 * move_crz - moves the cruser to the new location
 */
void move_crz()
{
	uint16_t temp = (VGA_COLS * term_row) + term_col;

	outportb(0x3D4, 0xE);
	outportb(0x3D5, temp >> 8);
	outportb(0x3D4, 0xF);
	outportb(0x3D5, temp);
}


/*
 * change_background_color - changes the background color
 * @color - color to change background color to
 */
void change_background_color(enum background color)
{
	bcolor = color;
	update_term_color();
}

/*
 * change_foreground_color - changes the foreground color
 * @color - color to change foreground to
 */
void change_foreground_color(enum foreground color)
{
	fcolor = color;
	update_term_color();
}

/*
 * term_putc - safe putc for other programs to talk to term screen
 * @c - character to put
 */
void term_putc(const char c)
{
	switch (c) {
	case '\n':
		term_col = 0;
		++term_row;
		break;
	case '\t':
		term_col += 4;

		if (term_col >= VGA_COLS) {
			term_col = 0;
			++term_row;
		}
		break;
	default:
		term_putc_private(term_row, term_col++, c);
		break;
	}

	if (term_col >= VGA_COLS) {
		term_col = 0;
		++term_row;
	}

	if (term_row >= VGA_ROWS) {
		term_col = 0;
		term_row = 0;
	}
}

/*
 * term_print - print a string on the terminal
 * @str - string pointer
 */
void term_print(const char *str)
{
	for (; *str; ++str)
		term_putc(*str);
	
	move_crz();
}

/*
 * clear_screen - clears the screen
 */
void clear_screen()
{
	term_col = 0;
	term_row = 0;
	move_crz();

	for (int row = 0; row < VGA_ROWS; ++row)
		for (int col = 0; col < VGA_COLS; ++col)
			term_putc_private(row, col, 0x00);
}

/*
 * print_intro - prints the intro for the kernel
 */
void print_intro()
{
	term_print(intro_msg);
}
