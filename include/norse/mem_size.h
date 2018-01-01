#ifndef MEMORY_SIZE_H
#define MEMORY_SIZE_H

// Normal sizes
#define KB 0x400
#define MB 0x100000
#define GB 0x40000000

// Page sizes
#define SMALL_PAGE 0x1000
#define MEDIUM_PAGE 2*MB
#define LARGE_PAGE GB

// Kernel space start
#define KERN_START 0xFFFFFFFF80000000

// For fractals
#define FRACTAL_PAGE_TABLE_INDEX 0x100

// Helper macros
#if defined(__ASM__)
#define MEM_PRES (1 << 0)
#define MEM_WRITE (1 << 1)
#define MEM_USER (1 << 2)
#define MEM_NX (1 << 63)
#define MEM_EX_MASK (0x7FFFFFFFFFFFFFFF)
#define MEM_WRITE_MASK (0xFFFFFFFFFFFFFFFD)
#define MEM_PERM_MASK (0x8000000000000FFF)

#define virt_to_phys(x) (x - KERN_START)
#define phys_to_virt(x) (x + KERN_START)
#elif defined(__C__)
#include <stdint.h>

#define virt_to_phys(x) ((uint64_t)x - KERN_START)
#define phys_to_virt(x) ((void*)(x + KERN_START)
#endif

#endif
