NASM = nasm
QEMU = qemu-system-i386.exe
CRTIMG = CrtImg.exe
GCC = i686-elf-gcc.exe

all: os.img

boot.bin: boot.asm
	$(NASM) -f bin boot.asm -o boot.bin

os.img: boot.bin
	$(CRTIMG) len:1440 size:2 boot.bin fill:00 name:os.img

run: os.img
	$(QEMU) -fda os.img
	
clean:
	del *.bin
	del *.img
	del *.o