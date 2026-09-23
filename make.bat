@echo off
nasm -f bin boot.asm -o boot.bin
nasm -f elf32 kernel.asm -o khead.o
nasm -f elf32 kernelClang\CFunc.asm -o cfunc.o
nasm -f elf32 kernelAsm\basicFunc.asm -o basic.o
nasm -f elf32 kernelAsm\key.asm -o key.o
nasm -f elf32 kernelAsm\global.asm -o global.o
nasm -f elf32 kernelAsm\idt.asm -o idt.o
nasm -f elf32 kernelAsm\tss.asm -o tss.o
nasm -f elf32 kernelAsm\syscall.asm -o sc.o

nasm -f bin userShell.asm -o usershell.bin

i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernel.c -o ckernelhead.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\std.c -o std.o

i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\disk\diskio.c -o diskio.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\disk\fat16.c -o fat16.o

i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\memory\memory.c -o mem.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\memory\task.c -o task.o

i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\irqint.c -o int.o
i686-elf-gcc -m32 -ffreestanding -nostdlib -c -std=c99 kernelClang\syscall.c -o syscall.o

i686-elf-ld -T linker.ld -o kernel.elf khead.o ckernelhead.o cfunc.o basic.o key.o global.o idt.o tss.o sc.o std.o diskio.o fat16.o int.o mem.o task.o syscall.o 
i686-elf-objcopy -O binary kernel.elf kernel.bin

if %errorlevel% neq 0 exit /b %errorlevel%

copy /b kernel.bin kernel_full.bin >nul

powershell -NoProfile -ExecutionPolicy Bypass -File mkimg.ps1

del boot.bin *.o *.elf kernel.bin usershell.bin kernel_full.bin

qemu-system-i386 -drive file=stdos.img,format=raw,if=ide
