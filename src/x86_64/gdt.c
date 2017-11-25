#include <norse/types.h>

/*
 * This file is to make the gdt, which is the
 * global descriptor table, ours will have only
 * 6 entries, and we do our best to follow linux's
 * example and not use hardware multithreading
 *
 * 1. An empty entry (manual)
 * 2. Kernel mode code seg
 * 3. Kernel mode data seg
 * 4. User mode code seg
 * 5. User mode data seg
 * 6. TSS
 */

struct gdt_entry {
	uint16_t lower_limit;
	uint16_t lower_base;
	uint8_t middle_base;
	uint8_t access;
	uint8_t granularity;
	uint8_t higher_base;
};

struct gdt_entry64 {
	uint16_t lower_limit;
	uint16_t lower_base;
	uint8_t middle_base;
	uint8_t access;
	uint8_t granularity;
	uint8_t higher_base;
	uint32_t highest_base;
	uint32_t reserved0;
};

struct tss_entry {
	uint32_t reserved0;
	uint64_t stack0;
	uint64_t stack1;
	uint64_t stack2;
	uint64_t reserved1;
	uint64_t ist[7];
	uint64_t reserved2;
	uint64_t reserved3;
	uint16_t iomap_base;
} pack;

// Would probably work if I set one to 0
struct tss_entry TSS = {
	.reserved0 = 0,
	.stack0 = 0,
	.stack1 = 0,
	.stack2 = 0,
	.reserved1 = 0,
	.ist = {0, 0, 0, 0, 0, 0, 0},
	.reserved2 = 0,
	.reserved3 = 0,
	.iomap_base = 0
};

#define GRAN_64_BIT_MODE (1 << 5)
#define GRAN_4KIB_BLOCKS (1 << 7)

#define gdt_entry(base, limit, access, granularity) \
	{ \
		(limit) & 0xFFFF, \
		(uint16_t) ((base) >> 0 & 0xFFFF), \
		(uint8_t) ((base) >> 16 & 0xFF), \
		(access) & 0xFF, \
		((limit) >> 16 & 0x0F) | ((granularity) & 0xF0), \
		(uint8_t) ((base) >> 24 & 0xFF) \
	}

#define gdt_entry64(base, limit, access, granularity) \
	{ \
		(limit) & 0xFFFF, \
		(uint16_t) ((base) >> 0 & 0xFFFF), \
		(uint8_t) ((base) >> 16 & 0xFF), \
		(access) & 0xFF, \
		((limit) >> 16 & 0x0F) | ((granularity) & 0xF0), \
		(uint8_t) ((base) >> 24 & 0xFF) \
	}, \
	{ \
		(uint16_t) ((base) >> 32 & 0xFFFF), \
		(uint16_t) ((base) >> 48 & 0xFFFF), \
		0, \
		0, \
		0, \
		0 \
	}

struct gdt_entry GDT[] = {
	// NULL
	gdt_entry(0, 0, 0, 0),

	// Kernel Code
	gdt_entry(0, 0xFFFFFFFF, 0x9A, GRAN_64_BIT_MODE | GRAN_4KIB_BLOCKS),

	// Kernel Data
	gdt_entry(0, 0xFFFFFFFF, 0x92, GRAN_64_BIT_MODE | GRAN_4KIB_BLOCKS),

	// User Code
	gdt_entry(0, 0xFFFFFFFF, 0xFA, GRAN_64_BIT_MODE | GRAN_4KIB_BLOCKS),

	// User Data
	gdt_entry(0, 0xFFFFFFFF, 0xF2, GRAN_64_BIT_MODE | GRAN_4KIB_BLOCKS),

	// TSS
	gdt_entry64(0ull, sizeof(TSS) - 1, 0xE9, 0x00)
};

uint16_t GDT_SIZE = sizeof(GDT) - 1;
