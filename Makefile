NASM = nasm
QEMU = qemu-system-i386.exe
CRTIMG = CrtImg.exe
GCC = i686-elf-gcc.exe

all: os.img

boot.bin: boot.asm
	$(NASM) -f bin boot.asm -o boot.bin

kernel.bin: kernel.asm
	$(NASM) -f bin kernel.asm -o kernel.bin

stdos.img: boot.bin kernel.bin
	cmd /c copy /b boot.bin+kernel.bin stdos.img

run: stdos.img
	$(QEMU) -fda stdos.img
	
clean:
	