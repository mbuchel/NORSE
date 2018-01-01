#include <device/vga.h>
#include <norse/types.h>

// TODO: Change font and make this into a file

enum vga_color {
	BLACK, BLUE, GREEN, CYAN, RED, MAGENTA,
	BROWN, LIGHT_GREY, DARK_GREY, LIGHT_BLUE,
	LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, LIGHT_MAGENTA,
	LIGHT_BROWN, WHITE
};

/*
 * vga_entry_color - helper function to change the foreground and background
 * @fg - foreground color
 * @bg - background color
 */
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

/*
 * vga_entry - entry for vga characters
 * @c - character
 * @color - color for the entry
 */
static inline uint16_t vga_entry(const uint8_t c, const uint8_t color)
{
	return (uint16_t) c | (uint16_t) color << 8;
}

static const size_t WIDTH = 80;
static const size_t HEIGHT = 25;

static size_t term_row;
static size_t term_col;

static uint8_t clear_entry;
static uint8_t term_color;

static volatile uint16_t *term_buf;

/*
 * vga_init - initializes the vga screen
 */
void vga_init()
{
	const char *welcome[] = {
		"Welcome to ", NAME, "\n\tVersion: ",
		VERSION, "\n\n>"
	};

	term_row = 0;
	term_col = 0;
	term_color = vga_entry_color(RED, BLACK);
	clear_entry = vga_entry(' ', term_color);
	term_buf = (uint16_t*) 0xB8000;

	for (size_t y = 0; y < HEIGHT; ++y) {
		for (size_t x = 0; x < WIDTH; ++x) {
			const size_t index = y * WIDTH + x;
			term_buf[index] = clear_entry;
		}
	}

	vga_put(welcome[0]);
	vga_put(welcome[1]);
	vga_put(welcome[2]);
	vga_put(welcome[3]);
	vga_put(welcome[4]);
}

/*
 * vga_putc - puts c on the vga screen
 * @c - c to put
 */
void vga_putc(const char c)
{
	size_t index;
	switch (c) {
	case '\b':
		index = term_col * WIDTH + term_row - 1;
		term_buf[index] = clear_entry;

		if (index == 0)
			break;

		if (term_row) {
			--term_row;
		} else {
			term_row = WIDTH - 1;
			--term_col;
		}
		break;
	case '\t':
		term_row += 8;
		
		if (term_row < WIDTH) {
			term_row = 0;
			++term_col;
		}

		break;
	case '\n':
		term_row = 0;
		++term_col;
		break;
	default:
		index = term_col * WIDTH + term_row;
		term_buf[index] = vga_entry(c, term_color);
		
		if (++term_row == WIDTH) {
			term_row = 0;
			++term_col;
		}

		break;
	};

	if (term_col == HEIGHT) {
		term_row = 0;
		term_col = 0;
	}
}

/*
 * vga_put - puts a string on the vga screen
 * @str - string to put
 */
void vga_put(const char *str)
{
	while (*str) {
		vga_putc(*str);
		++str;
	}
}
