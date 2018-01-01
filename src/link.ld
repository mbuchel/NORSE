ENTRY(_start)

SECTIONS {
	. = 0x000100000;
	
	.text : AT(ADDR(.text) - 1M) {
		_code = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}

	.data : AT(ADDR(.data) - 1M) {
		_data = .;
		*(.data)
		. = ALIGN(4096);
	}

	.bss : AT(ADDR(.bss) - 1M) {
		_bss = .;
		*(.bss)
		*(COMMON)
		. = ALIGN(4096);
	}

	_end = .;

	/DISCARD/ : {
		*(.comment)
	}
}
