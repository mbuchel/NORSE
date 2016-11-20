ENTRY(bootstrap)

SECTIONS
{
	/*
	 * Convention for kernels to be loaded
	 */
	. = 1M;

	/*
	 * Multiboot header
	 */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

	/*
	 * read only data
	 */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/*
	 * read-write data
	 */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/*
	 * stack data read-write uninitialized data
	 */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
