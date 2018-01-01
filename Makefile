# Naming to help me
VERSION := 0.0.1
NAME := SHELL_MANIA

PREFIX := x86_64-redhat-linux
ARCH := x86_64

# Kernel name
KERNEL := bin/norse-$(NAME).elf
DEBUG_SYM := bin/debug.sym

# Special stuff
DEFS := -DNAME="\"$(NAME)\"" -DVERSION="\"$(VERSION)\""
WARN := -Wall -Wextra -Werror -Wno-missing-braces
EXTR := -Og -ffreestanding -lgcc -g3 -MP -MMD -mcmodel=kernel

# Normal C stuff
AS := $(PREFIX)-gcc
ASFLAGS := -c -I include/ -D __ASM__ $(DEFS) $(WARN) $(EXTR)

CC := $(PREFIX)-gcc
CFLAGS := -std=gnu11 -c -I include/ -D __C__ $(DEFS) $(WARN) $(EXTR)

LD := $(PREFIX)-gcc
LDFLAGS := -N -nostdlib -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Wl,--build-id=none $(WARN) $(EXTR)

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

OBJS :=
DIRS :=

include src/Makefile

debug: all
	@$(ELF_COPY) --only-keep-debug $(KERNEL)64 $(DEBUG_SYM)
	@$(ELF_COPY) --strip-debug $(KERNEL)64
	@echo "Ready for debug"

all: $(DIRS) $(KERNEL)

%/:
	@mkdir -p $@

build/%.S.o: src/%.S
	@echo "[AS] Compiling $<"
	@$(AS) $(ASFLAGS) $< -o $@

build/%.c.o: src/%.c
	@echo "[CC] Compiling $<"
	@$(CC) $(CFLAGS) $< -o $@

$(KERNEL): $(OBJS)
	@echo "[LD] Linking Kernel: $(VERSION), Name: $(NAME)"
	@$(LD) -T src/link.ld $(OBJS) $(LDFLAGS) -o $(KERNEL)64
	@echo "Compiled Kernel Version: $(VERSION), Name: $(NAME)"

iso: all
	@$(CC) $(GENINITRD)/make_initrd.c -o $(GENINITRD)/make_initrd
	@cd $(GENINITRD);./make_initrd $(GENINITRD_ARGS)
	@cp $(GENINITRD)/$(INITRD) build/iso/boot
	@$(ELF_COPY) $(KERNEL)64 $(ELF_FLAGS) $(KERNEL)32
	@cp $(KERNEL)32 build/iso/boot/norse.elf
	@cp grub.cfg build/iso/boot/grub/grub.cfg
	@cd build && $(GRUB_MKRESCUE) -o ../bin/norse-$(NAME).iso iso
	@echo "Made iso file"

clean:
	@rm -rf build/ bin/
	@echo "Made clean"
