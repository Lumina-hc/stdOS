@echo off
setlocal
set BUILD=build
if "%~1"=="clean" goto clean
if "%~1"=="run" goto run
if "%~1"=="all" goto build
if "%~1"=="" goto build
echo usage: make.bat [all^|run^|clean]
exit /b 1

:build
if not exist %BUILD% mkdir %BUILD%
nasm -f bin boot\mbr.asm -o %BUILD%\mbr.bin || exit /b 1
nasm -f elf32 src\entry.asm -o %BUILD%\entry.o || exit /b 1
i686-elf-gcc -ffreestanding -fno-pic -fno-stack-protector -fno-builtin -fno-pie -O2 -Wall -Wextra -c src\kmain.c -o %BUILD%\kmain.o || exit /b 1
i686-elf-ld -T link\kernel.ld -o %BUILD%\kernel.elf %BUILD%\entry.o %BUILD%\kmain.o || exit /b 1
i686-elf-objcopy -O binary %BUILD%\kernel.elf %BUILD%\kernel.bin || exit /b 1
CrtImg.exe len:1440 size:2 build\mbr.bin,build\kernel.bin fill:00 name:os.img
echo build ok
exit /b 0

:run
call :build || exit /b 1
qemu-system-i386 -drive format=raw,file=os.img,file.locking=off
exit /b %errorlevel%

:clean
if exist %BUILD% rmdir /s /q %BUILD%
if exist os.img del os.img
echo cleaned
exit /b 0
