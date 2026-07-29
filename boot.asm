bits 16
org 0x7c00

start:
    cli
    mov [drive],dl
    xor ax,ax
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov sp,0x7c00

    call read_kernel

    in al,0x92
    or al,2
    out 0x92,al

    lgdt [gdt_desc]

    mov eax,cr0
    or eax,1
    mov cr0,eax

    jmp 0x08:0x8000

read_kernel:
    mov ah,0x02
    mov al,15
    mov ch,0
    mov cl,2
    mov dh,0
    mov dl,[drive]
    mov bx,0x0800
    mov es,bx
    xor bx,bx
    int 0x13
    jc disk_error
    ret

disk_error:
    mov si,error_msg

error_loop:
    lodsb
    cmp al,0
    je error_stop
    mov ah,0x0e
    int 0x10
    jmp error_loop

error_stop:
    jmp error_stop

drive db 0
error_msg db "Disk Error",0

gdt:
    dq 0
    dw 0xffff
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
    dw 0xffff
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0

gdt_end:

gdt_desc:
    dw gdt_end - gdt - 1
    dd gdt

times 510-($-$$) db 0
dw 0xaa55
