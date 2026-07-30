nasm -f bin boot.asm -o boot.bin
nasm -f elf32 kernel.asm -o khead.o
nasm -f elf32 kernelClang/CFunc.asm -o cfunc.o
nasm -f elf32 kernelAsm/global.asm -o global.o

i686-elf-gcc -m32 -ffreestanding -nostdlib -c kernel.c -o kernel.o
i686-elf-ld -T linker.ld -o kernel.elf khead.o kernel.o cfunc.o global.o
i686-elf-objcopy -O binary kernel.elf kernel.bin

copy /b boot.bin+kernel.bin stdos.img

qemu-system-i386 -fda stdos.img

.\clean.bat