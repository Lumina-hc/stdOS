[bits 16]
org 0x8000

start:
    cli
    in al,0x92
    or al,2
    out 0x92,al
    lgdt [gdt_desc]
    mov eax,cr0
    or eax,1
    mov cr0,eax
    jmp 0x08: protected

%include "kernelAsm/gdt.asm"

[bits 32]

protected:
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