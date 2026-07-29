nasm kernel.asm -f bin -o kernelAsm.bin

nasm boot.asm -f bin -o boot.bin 

cmd /c copy /b boot.bin+kernelAsm.bin stdos.img

qemu-system-i386 -fda stdos.img