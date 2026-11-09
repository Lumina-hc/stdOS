BITS 16
ORG 0x7C00

KERNEL_LBA  equ 1
KERNEL_SECS equ 64
KERNEL_LOAD equ 0x8000

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl

    mov si, dap
    mov ah, 0x42
    int 0x13
    jc disk_fail

    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:enter_pm

disk_fail:
    mov al, 'D'
    mov ah, 0x0E
    mov bx, 0x0007
    int 0x10
halt16:
    hlt
    jmp halt16

BITS 32
enter_pm:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x7C00
    jmp KERNEL_LOAD

align 8
gdt:
    dq 0
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:

gdt_desc:
    dw gdt_end - gdt - 1
    dd gdt

dap:
    db 0x10, 0
    dw KERNEL_SECS
    dw KERNEL_LOAD, 0
    dd KERNEL_LBA
    dd 0

boot_drive db 0

times 510-($-$$) db 0
dw 0xAA55
