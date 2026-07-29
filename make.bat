nasm -f bin boot.asm -o boot.bin
nasm -f elf32 kernel.asm -o kernelAsm.o
nasm -f elf32 kernelClang/CFunc.asm -o cfunc.o
nasm -f elf32 global.asm -o global.o

i686-elf-gcc -m32 -ffreestanding -nostdlib -c kernel.c -o kernel.o
i686-elf-ld -T linker.ld -o kernel.elf kernelAsm.o kernel.o cfunc.o global.o
i686-elf-objcopy -O binary kernel.elf kernel.bin

copy /b boot.bin+kernel.bin stdos.img

qemu-system-i386 -fda stdos.img

.\clean.bat