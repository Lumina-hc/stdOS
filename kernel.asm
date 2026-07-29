[bits 32]
org 0x8000

mov ax,0x10
mov ds,ax
mov es,ax
mov ss,ax
mov esp,0x90000

call clear

mov esi,banner
call print

shell:
mov esi,prompt
call print

mov edi,buffer
call read_line

call command

jmp shell

video equ 0xb8000

cursor:
dd 0

%include "kernelAsm/basicFunc.asm"

%include "kernelAsm/key.asm"

%include "kernelAsm/command.asm"
