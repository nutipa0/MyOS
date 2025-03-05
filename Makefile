CC = gcc
ASM = nasm
LD = ld

CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -c
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

SRCDIR = .
OBJDIR = obj

SRC_C = $(wildcard $(SRCDIR)/*.c)
SRC_ASM = $(wildcard $(SRCDIR)/*.asm)

OBJ_C = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC_C))
OBJ_ASM = $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%.o, $(SRC_ASM))

all: kernel.bin

kernel.bin: $(OBJ_C) $(OBJ_ASM) obj/kernel_asm.o
	$(LD) $(LDFLAGS) -o $@ $^

# kernel.bin: $(OBJ_C) $(OBJ_ASM) obj/kernel_asm.o

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

obj/kernel_asm.o: kernel.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

# $(OBJDIR)/%.o: $(SRCDIR)/%.asm
# 	$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	rm -f $(OBJDIR)/*.o kernel.bin

.PHONY: all clean