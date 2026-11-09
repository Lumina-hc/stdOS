@echo off
if not exist build mkdir build
nasm -f bin boot\mbr.asm -o build\mbr.bin || exit /b 1
nasm -f elf32 src\entry.asm -o build\entry.o || exit /b 1
i686-elf-gcc -ffreestanding -fno-pic -fno-stack-protector -fno-builtin -fno-pie -O2 -Wall -Wextra -c src\kmain.c -o build\kmain.o || exit /b 1
i686-elf-ld -T link\kernel.ld -o build\kernel.elf build\entry.o build\kmain.o || exit /b 1
i686-elf-objcopy -O binary build\kernel.elf build\kernel.bin || exit /b 1
copy /b build\mbr.bin + build\kernel.bin os.img >nul || exit /b 1
echo build ok
