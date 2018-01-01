symbol-file bin/debug.sym
target remote | qemu-system-x86_64 -S -gdb stdio -kernel bin/norse-SHELL_MANIA.elf64
