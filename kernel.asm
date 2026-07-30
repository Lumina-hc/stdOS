[bits 32]
global _start
extern kmain

_start:
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov esp,0x90000

    call setIdt
    call clear
    call kmain

extern video
extern cursor

%include "kernelAsm/basicFunc.asm"
%include "kernelAsm/key.asm"
%include "kernelAsm/idt.asm"
