bits 16
org 0x7c00

jmp short start
nop

bpb_oem          db "STDOS   "
bpb_bytes        dw 512
bpb_sec_per_clu  db 4
bpb_reserved     dw 64
bpb_fats         db 2
bpb_root_entries dw 512
bpb_sectors      dw 0
bpb_media        db 0xF8
bpb_fat_size     dw 128
bpb_sec_per_trk  dw 63
bpb_heads        dw 16
bpb_hidden       dd 0
bpb_large_sec    dd 131072
bs_drive         db 0x80
bs_reserved      db 0
bs_bootsig       db 0x29
bs_volid         dd 0
bs_vol           db "STDOS      "
bs_fstype        db "FAT16   "

KERNEL_LBA equ 1
KERNEL_SEC equ 63

start:
    cli
    xor ax,ax
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov sp,0x7c00
    sti

    mov [bs_drive],dl

    mov dword [dap_lba],KERNEL_LBA
    mov word [dap_cnt],KERNEL_SEC
    mov word [dap_off],0
    mov word [dap_seg],0x0800

    mov ah,0x42
    mov si,dap
    int 0x13
    jc error

    in al,0x92
    or al,2
    out 0x92,al

    cli
    lgdt [gdt_desc]

    mov eax,cr0
    or eax,1
    mov cr0,eax

    jmp 0x08:0x8000

error:
    mov si,err_msg
.l:
    lodsb
    cmp al,0
    je .s
    mov ah,0x0e
    int 0x10
    jmp .l
.s:
    jmp .s

err_msg db "Disk Error",0

dap:
    db 0x10
    db 0
dap_cnt:
    dw KERNEL_SEC
dap_off:
    dw 0
dap_seg:
    dw 0x0800
dap_lba:
    dq 1

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
    dw gdt_end-gdt-1
    dd gdt

times 510-($-$$) db 0
dw 0xaa55
