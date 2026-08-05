@echo off
nasm -f bin boot.asm -o boot.bin
nasm -f elf32 kernel.asm -o khead.o
nasm -f elf32 kernelClang\CFunc.asm -o cfunc.o
nasm -f elf32 kernelAsm\basicFunc.asm -o basic.o
nasm -f elf32 kernelAsm\key.asm -o key.o
nasm -f elf32 kernelAsm\global.asm -o global.o
nasm -f elf32 kernelAsm\idt.asm -o idt.o

i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernel.c -o kernel.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\std.c -o std.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\diskio.c -o diskio.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\fat16.c -o fat16.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\memory.c -o mem.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\pading.c -o pd.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\irqint.c -o int.o

i686-elf-ld -T linker.ld -o kernel.elf khead.o kernel.o cfunc.o basic.o key.o global.o idt.o std.o diskio.o fat16.o int.o mem.o pd.o
i686-elf-objcopy -O binary kernel.elf kernel.bin

if %errorlevel% neq 0 exit /b %errorlevel%

powershell -NoProfile -ExecutionPolicy Bypass -File mkimg.ps1

del boot.bin *.o *.elf

qemu-system-i386 -drive file=stdos.img,format=raw,if=ide
