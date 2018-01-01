#include <device/vga.h>
#include <norse/cpu.h>
#include <norse/file.h>

#include <arch/ports.h>

// Keyboard definitions
#define RIGHT_SHIFT_HELD 0x36
#define RIGHT_SHIFT_RELEASE 0xB6
#define LEFT_SHIFT_HELD 0x2A
#define LEFT_SHIFT_RELEASE 0xAA
#define INPUT_AVAIL 0x1

#define STATUS_PORT 0x64
#define DATA_PORT 0x60

#define IRQ_NUM 0x21

// The following are different keyboard maps
static unsigned char map[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8',
	'9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e',
	'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
	'\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',
	'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static unsigned char shift_map[256] = {
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*',
	'(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E',
	'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B',
	'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '_',
	0, 0, 0, '=', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

struct file keyboard = {
	.type = DEV,
	.name = "keyboard",
	.permissions = 0744,
	.obj = OBJ_CLR
};

/*
 * keyboard_handle - handles keyboard input
 */
void keyboard_handle()
{
	static bool shift = false;
	uint8_t status_port;
	uint8_t key_code;
	char c;

	status_port = read_port(STATUS_PORT);

	if (status_port & INPUT_AVAIL) {
		key_code = read_port(DATA_PORT);
		
		if (key_code == RIGHT_SHIFT_HELD || key_code == LEFT_SHIFT_HELD) {
			shift = true;
			goto exit_handle;
		} else if (key_code == RIGHT_SHIFT_RELEASE || key_code == LEFT_SHIFT_RELEASE) {
			shift = false;
			goto exit_handle;
		}

		if (shift)
			c = shift_map[key_code];
		else
			c = map[key_code];

		if (c)
			vga_putc(c);

	}

exit_handle:
	return;
}

/*
 * keyboard_init - initializes the keyboard device
 */
void keyboard_init(void)
{
	file_attach_path("/dev/", &keyboard);
	interrupt_register_handler(IRQ_NUM, keyboard_handle);
}

/*
 * keyboard_fini - deletes the keyboard device
 */
void keyboard_fini()
{
	interrupt_unregister_handler(IRQ_NUM);
	file_fini(&keyboard);
}
