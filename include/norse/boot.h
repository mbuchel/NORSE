#ifndef BOOT_H
#define BOOT_H

#include <norse/mem_size.h>

// Multiboot stuff
#define MAGIC			0x1BADB002
#define ALIGN			(1 << 1)
#define MINFO			(1 << 1)
#define FLAGS			(ALIGN | MINFO)
#define CHECK			-(MAGIC + FLAGS)

#define PL1_S			SMALL_PAGE
#define PL2_S			SMALL_PAGE
#define PL3_S			SMALL_PAGE
#define PL4_S			SMALL_PAGE

#define NULL_SEG		0x00
#define CODE_SEG		0x08
#define DATA_SEG		0x10
#define USER_CODE_SEG		0x18
#define USER_DATA_SEG		0x20
#define TSS_SEG			0x28
#define RPL			0x3

#define CR0_PAGING		0x80000000
#define CR0_WRITE_PROTECT	0x00010000

#define CR4_PAE			0x00000020

#define PAGE_PRES		(1 << 0)
#define PAGE_WRITE		(1 << 1)
#define PAGE_USER		(1 << 2)
#define PAGE_EXEC_DISABLE	(1 << 63)
#define PAGE_EXEC_DISABLE_HIGH	(1 << 31)

#define IA32_EFER_MSR		0xC0000080

#endif
