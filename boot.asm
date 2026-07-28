    bits 16
    org 0x7c00

    start:
        mov ax,0
        mov ss,ax
        mov sp,0x7c00
        mov ds,ax
        mov es,ax
        call Read ; 读扇区，为内核做准备

        jmp $
    Read:
        mov ah,0x02
        mov al,15
        mov ch,0
        mov cl,2
        mov dh,0
        mov dl,0
        mov bx,0x0800
        mov es,bx
        mov bx,0

        int 0x13

        jc error

        ret
    error: jmp $

times 510-($-$$) db 0
dw 0xAA55