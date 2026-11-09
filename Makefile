QEMU    := D:/programs/qemu/qemu-system-i386.exe
NASM    := nasm
CC      := i686-elf-gcc
LD      := i686-elf-ld
OBJCOPY := i686-elf-objcopy

CFLAGS  := -ffreestanding -fno-pic -fno-stack-protector -fno-builtin -fno-pie -O2 -Wall -Wextra

BUILD   := build

all: os.img

$(BUILD):
	-mkdir $(BUILD)

$(BUILD)/mbr.bin: boot/mbr.asm | $(BUILD)
	$(NASM) -f bin $< -o $@

$(BUILD)/entry.o: src/entry.asm | $(BUILD)
	$(NASM) -f elf32 $< -o $@

$(BUILD)/kmain.o: src/kmain.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.elf: $(BUILD)/entry.o $(BUILD)/kmain.o link/kernel.ld
	$(LD) -T link/kernel.ld -o $@ $(BUILD)/entry.o $(BUILD)/kmain.o

$(BUILD)/kernel.bin: $(BUILD)/kernel.elf
	$(OBJCOPY) -O binary $< $@

os.img: $(BUILD)/mbr.bin $(BUILD)/kernel.bin
	cat $(BUILD)/mbr.bin $(BUILD)/kernel.bin > os.img

run: os.img
	$(QEMU) -drive format=raw,file=os.img

clean:
	rm -rf $(BUILD) os.img

.PHONY: all run clean
