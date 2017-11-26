# Naming to help me
VERSION := 0.0.1
NAME := SHELL_MANIA

# Currently this is just x86 not x86_64
PREFIX := x86_64-redhat-linux
ARCH := x86_64

# Kernel name
KERNEL := bin/norse-$(NAME).elf

# Special stuff
DEFS := -DNAME="\"$(NAME)\"" -DVERSION="\"$(VERSION)\""
WARN := -Wall -Wextra -Wno-pointer-to-int-cast #-Werror
EXTR := -O0 -ffreestanding -lgcc

# Normal C stuff
AS := $(PREFIX)-gcc
ASFLAGS := -c -I include/ -D __ASM__ $(DEFS) $(WARN) $(EXTR)

CC := $(PREFIX)-gcc
CFLAGS := -std=gnu99 -c -I include/ -D __C__ $(DEFS) $(WARN) $(EXTR)

LD := $(PREFIX)-gcc
LDFLAGS := -nostdlib -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Wl,--build-id=none $(WARN) $(EXTR)

ELF_COPY := objcopy
ELF_FLAGS := -O elf32-i386

# Stuff for os
GRUB_MKRESCUE := grub2-mkrescue

# Initrd stuff
GENINITRD := initrd
INITRD_RM := ./make_initrd ./make_initrd.c ./initrd.img
INITRD_CT := $(filter-out $(INITRD_RM), $(shell cd $(GENINITRD);find -type f))
GENINITRD_AGRS := $(foreach file, $(INITRD_CT), $(patsubst ./%,%,$(file)) $(patsubst ./%,%,$(file)))
INITRD := initrd.img

OBJS := build/$(ARCH)/boot.S.o build/$(ARCH)/paging.S.o build/$(ARCH)/idt.S.o build/$(ARCH)/lock.S.o build/$(ARCH)/gdt.c.o build/$(ARCH)/idt.c.o
OBJS += build/core/kernel.c.o

all: $(KERNEL)

init:
	@mkdir -p build/$(ARCH)/
	@mkdir -p build/core/
	@mkdir -p build/libc/
	@mkdir -p bin/

build/$(ARCH)/%.S.o: src/$(ARCH)/%.S
	@echo "[AS] Compiling $?"
	@$(AS) $(ASFLAGS) $? -o $@

build/$(ARCH)/%.c.o: src/$(ARCH)/%.c
	@echo "[CC] Compiling $?"
	@$(CC) $(CFLAGS) $? -o $@

build/core/%.S.o: src/core/%.S
	@echo "[AS] Compiling $?"
	@$(AS) $(ASFLAGS) $? -o $@

build/core/%.c.o: src/core/%.c
	@echo "[CC] Compiling $?"
	@$(CC) $(CFLAGS) $? -o $@

build/libc/%.S.o: src/libc/%.S
	@echo "[AS] Compiling $?"
	@$(AS) $(ASFLAGS) $? -o $@

build/libc/%.c.o: src/libc/%.c
	@echo "[CC] Compiling $?"
	@$(CC) $(CFLAGS) $? -o $@

$(KERNEL): $(OBJS)
	@echo "[LD] Linking Kernel: $(VERSION), Name: $(NAME)"
	@$(LD) -T src/$(ARCH)/link.ld $(OBJS) $(LDFLAGS) -o $(KERNEL)64
	@echo "Compiled Kernel Version: $(VERSION), Name: $(NAME)"

iso:
	@make all
	@mkdir -p build/iso/boot/grub
	@$(CC) $(GENINITRD)/make_initrd.c -o $(GENINITRD)/make_initrd
	@cd $(GENINITRD);./make_initrd $(GENINITRD_ARGS)
	@cp $(GENINITRD)/$(INITRD) build/iso/boot
	@$(ELF_COPY) $(KERNEL)64 $(ELF_FLAGS) $(KERNEL)32
	@cp $(KERNEL)32 build/iso/boot/norse.elf
	@cp grub.cfg build/iso/boot/grub/grub.cfg
	@cd build && $(GRUB_MKRESCUE) -o ../bin/norse-$(NAME).iso iso
	@echo "Made iso file"

debug:
	CFLAGS += -g -fsanitize=undefined
	ASFLAGS += -g
	all

clean:
	@rm -rf build/* bin/*
	@echo "Made clean"
	@make -s init
