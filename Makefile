NASM = nasm
CC   = gcc
LD   = ld

SRCDIR   = src
INCDIR   = include
BUILDDIR = build

DEP_FLAGS = -MMD -MP
INCLUDES  = -I$(INCDIR)
CFLAGS    = -m32 -mavx -ffreestanding -fno-pie -fno-builtin -nostdlib -O2 -Wall -Wextra $(INCLUDES) $(DEP_FLAGS)
LDFLAGS   = -m elf_i386 -nostdlib --oformat binary -Ttext 0x10000 -e _start

IMAGE      = $(BUILDDIR)/disk.img
BOOT_SRC   = $(SRCDIR)/boot.asm
ENTRY_SRC  = $(SRCDIR)/kernel_entry.asm
C_SRCS     = $(wildcard $(SRCDIR)/*.c)
OBJS       = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(C_SRCS))
ENTRY_OBJ  = $(BUILDDIR)/kernel_entry.o
DEPS       = $(OBJS:.o=.d)

BOOT_BIN   = $(BUILDDIR)/boot.bin
KERNEL_BIN = $(BUILDDIR)/kernel.bin

-include $(DEPS)

.PHONY: all clean directories rebuild

all: directories $(IMAGE)

rebuild: clean all

directories:
	@mkdir -p $(BUILDDIR)

$(BOOT_BIN): $(BOOT_SRC) | directories
	$(NASM) -f bin $< -o $@

$(ENTRY_OBJ): $(ENTRY_SRC) | directories
	$(NASM) -f elf32 $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | directories
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL_BIN): $(ENTRY_OBJ) $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $@
	truncate -s 1440K $@

clean:
	rm -rf $(BUILDDIR)