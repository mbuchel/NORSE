ENTRY(_start)

SECTIONS
{
	. = 0xFFFFFFFF80000000;

	.text : AT(ADDR(.text) - 0xFFFFFFFF80000000) {
		__kernel_start = .;
		__text_start = .;
		*(.text)
		*(.rodata*)
		. = ALIGN(4096);
	}

	.text : AT(1M) {
		__kernel_start = .;
		__text_start = .;
		*(.text)
	}

	.data BLOCK(4K) : ALIGN(4K) {
		__text_end = .;
		__data_start = .;
		*(.data)
		PROVIDE(kernel_symbol_table_start = .);
		PROVIDE(kernel_symbol_table_end = .);
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		__data_end = .;
		__rodata_start = .;
		*(.rodata)
	}

	.bss BLOCK(4K) : ALIGN(4K) {
		__bss_start = .;
		__rodata_end = .;
		*(.bss)
		*(.COMMON)
	}

	. = ALIGN(4K);
	__bss_end = .;
	__kernel_end = .;
}
