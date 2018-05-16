ENTRY(_start)

SECTIONS
{
	. = 1M;

	.bootstrap :
	{
		build/x86_64/boot.S.o (.text)
	}

	. += 0xFFFFFFFF80000000;

	.text : AT(ADDR(.text) - 0xFFFFFFFF80000000) {
		__text_start = .;
		*(EXCLUDE_FILE(* build/x86_64/boot.S.o) .text)
		*(.rodata*)
		. = ALIGN(4096);
		__text_end = .;
	}

	.data : AT(ADDR(.data) - 0xFFFFFFFF80000000) {
		__data_start = .;
		*(.data)
		. = ALIGN(4096);
		__data_end = .;
	}

	/*
	 * Do not see a need for eh_frame yet
	.eh_frame : AT(ADDR(.eh_frame) - 0xFFFFFFFF80000000) {
		_ehframe = .;
		*(.eh_frame)
		. = ALIGN(4096);
	}
	*/

	.bss : AT(ADDR(.bss) - 0xFFFFFFFF80000000) {
		__bss_start = .;
		*(.bss)
		*(.COMMON)
		. = ALIGN(4096);
		__bss_end = .;
	}
}
