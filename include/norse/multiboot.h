/*
 * multiboot.h -- Declares the multiboot info structure.
 * Made for JamesM's tutorials <www.jamesmolloy.co.uk>
 *
 * This is because I have been having issues loading into multiboot
 * when I set up paging and try to switch to long mode, maybe because
 * I am using grub 2
 *
 * Note: I am aware of #pragma once but I am used to the ifdef way
 */
#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

const uint32_t MULTIBOOT_FLAG_MEM	= 0x0001;
const uint32_t MULTIBOOT_FLAG_DEV	= 0x0002;
const uint32_t MULTIBOOT_FLAG_CMD	= 0x0004;
const uint32_t MULTIBOOT_FLAG_MOD	= 0x0008;
const uint32_t MULTIBOOT_FLAG_AOUT	= 0x0010;
const uint32_t MULTIBOOT_FLAG_ELF	= 0x0020;
const uint32_t MULTIBOOT_FLAG_MMAP	= 0x0040;
const uint32_t MULTIBOOT_FLAG_CFG	= 0x0080;
const uint32_t MULTIBOOT_FLAG_LDR	= 0x0100;
const uint32_t MULTIBOOT_FLAG_APM	= 0x0200;
const uint32_t MULTIBOOT_FLAG_VBE	= 0x0400;

struct multiboot {
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	uint32_t boot_dev;
	uint32_t cmd;
	uint32_t mods_count;
	uint32_t mods_addr;
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
	uint32_t mmap_len;
	uint32_t mmap_addr;
	uint32_t drives_len;
	uint32_t drives_addr;
	uint32_t cfg_tables;
	uint32_t boot_ldr_name;
	uint32_t apm_table;
	uint32_t vbe_ctrl_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
} __attribute__((packed));

void PARANIOA();

#endif
