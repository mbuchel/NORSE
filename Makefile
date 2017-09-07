# Naming to help me
VERSION := 0.0.1
NAME := SHELL_MANIA

# Currently this is just x86 not x86_64
PREFIX := x86_64-redhat-linux
ARCH := x86_64

# Kernel name
KERNEL := build/norse-$(NAME).elf

# Normal C stuff
CC := $(PREFIX)-gcc
CFLAGS := -m32 -std=gnu99 -O3 -ffreestanding -c -Wall -Wextra -I include/ -D __C__
AS := $(PREFIX)-gcc
ASFLAGS := -m32 -O3 -ffreestanding -c -Wall -Wextra -I include/ -D __ASM__
LD := $(PREFIX)-gcc
LDFLAGS := -m32 -nostdlib -ffreestanding -O3 -Wl,--build-id=none

# Stuff for os
GRUB_MKRESCUE := grub2-mkrescue

# Initrd stuff
GENINITRD := initrd
INITRD_RM := ./make_initrd ./make_initrd.c ./initrd.img
INITRD_CT := $(filter-out $(INITRD_RM), $(shell cd $(GENINITRD);find -type f))
GENINITRD_AGRS := $(foreach file, $(INITRD_CT), $(patsubst ./%,%,$(file)) $(patsubst ./%,%,$(file)))
INITRD := initrd.img

.PHONY: all clean debug iso

all:
	@echo "Compiling Kernel Version: $(VERSION), Name: $(NAME)"
	@mkdir -p build/$(ARCH)/
	@mkdir -p build/core/
	@$(AS) -c src/$(ARCH)/*.S $(ASFLAGS)
	@$(CC) -c src/$(ARCH)/*.c $(CFLAGS)
	@mv *.o build/$(ARCH)/
#	@$(AS) -c src/core/*.S $(ASFLAGS)
	@$(CC) -c src/core/*.c $(CFLAGS)
	@mv *.o build/core/
	@$(LD) -T src/$(ARCH)/link.ld build/*/*.o $(LDFLAGS) -o $(KERNEL)
	@echo "Compiled Kernel Version: $(VERSION), Name: $(NAME)"

iso:
	@make all
	@mkdir -p build/iso/boot/grub
	@$(CC) $(GENINITRD)/make_initrd.c -o $(GENINITRD)/make_initrd
	@cd $(GENINITRD);./make_initrd $(GENINITRD_ARGS)
	@cp $(GENINITRD)/$(INITRD) build/iso/boot
	@cp $(KERNEL) build/iso/boot/norse.elf
	@cp grub.cfg build/iso/boot/grub/grub.cfg
	@cd build && $(GRUB_MKRESCUE) -o norse-$(NAME).iso iso

debug:
	CFLAGS += -g -fsanitize=undefined
	ASFLAGS += -g
	all

clean:
	@rm -rf build/*
	@echo "Made clean"
