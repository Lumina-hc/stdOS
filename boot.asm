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
    
    jmp 0x0000:0x8000

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

times 510-($-$$) db 0
dw 0xaa55