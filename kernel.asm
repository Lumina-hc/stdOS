bits 16
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
    jmp 0x08:protected

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

bits 32

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

clear:
    mov edi,video
    mov ecx,2000
    mov ax,0x0720
    rep stosw
    mov dword[cursor],0
    call update_cursor
    ret

print:
    lodsb
    cmp al,0
    je print_end
    call putchar
    jmp print

print_end:
    ret

putchar:
    cmp al,13
    je newline

    mov ebx,[cursor]
    mov [video+ebx],al
    mov byte[video+ebx+1],7
    add ebx,2
    mov [cursor],ebx
    call update_cursor
    ret

newline:
    mov eax,[cursor]
    mov ebx,160
    xor edx,edx
    div ebx
    inc eax
    mul ebx
    mov [cursor],eax
    call update_cursor
    ret

update_cursor:
    mov eax,[cursor]
    shr eax,1

    mov dx,0x3d4
    mov al,0x0f
    out dx,al

    mov dx,0x3d5
    mov al,byte[eax]
    out dx,al

    mov dx,0x3d4
    mov al,0x0e
    out dx,al

    mov dx,0x3d5
    mov al,byte[eax+1]
    out dx,al

    ret

read_line:
    mov esi,edi

read_loop:
    call keyboard

    cmp al,13
    je input_done

    mov [esi],al
    inc esi

    call putchar

    jmp read_loop

input_done:
    mov byte[esi],0
    mov al,13
    call putchar
    ret

keyboard:
wait_key:
    in al,0x64
    test al,1
    jz wait_key

    in al,0x60

    test al,0x80
    jnz wait_key

    cmp al,0x39
    je key_space

    cmp al,0x1c
    je key_enter

    cmp al,0x10
    je key_q
    cmp al,0x11
    je key_w
    cmp al,0x12
    je key_e
    cmp al,0x13
    je key_r
    cmp al,0x14
    je key_t
    cmp al,0x15
    je key_y
    cmp al,0x16
    je key_u
    cmp al,0x17
    je key_i
    cmp al,0x18
    je key_o
    cmp al,0x19
    je key_p

    cmp al,0x1e
    je key_a
    cmp al,0x1f
    je key_s
    cmp al,0x20
    je key_d
    cmp al,0x21
    je key_f
    cmp al,0x22
    je key_g
    cmp al,0x23
    je key_h
    cmp al,0x24
    je key_j
    cmp al,0x25
    je key_k
    cmp al,0x26
    je key_l

    cmp al,0x2c
    je key_z
    cmp al,0x2d
    je key_x
    cmp al,0x2e
    je key_c
    cmp al,0x2f
    je key_v
    cmp al,0x30
    je key_b
    cmp al,0x31
    je key_n
    cmp al,0x32
    je key_m

    jmp wait_key

key_q:
    mov al,'q'
    ret
key_w:
    mov al,'w'
    ret
key_e:
    mov al,'e'
    ret
key_r:
    mov al,'r'
    ret
key_t:
    mov al,'t'
    ret
key_y:
    mov al,'y'
    ret
key_u:
    mov al,'u'
    ret
key_i:
    mov al,'i'
    ret
key_o:
    mov al,'o'
    ret
key_p:
    mov al,'p'
    ret
key_a:
    mov al,'a'
    ret
key_s:
    mov al,'s'
    ret
key_d:
    mov al,'d'
    ret
key_f:
    mov al,'f'
    ret
key_g:
    mov al,'g'
    ret
key_h:
    mov al,'h'
    ret
key_j:
    mov al,'j'
    ret
key_k:
    mov al,'k'
    ret
key_l:
    mov al,'l'
    ret
key_z:
    mov al,'z'
    ret
key_x:
    mov al,'x'
    ret
key_c:
    mov al,'c'
    ret
key_v:
    mov al,'v'
    ret
key_b:
    mov al,'b'
    ret
key_n:
    mov al,'n'
    ret
key_m:
    mov al,'m'
    ret
key_space:
    mov al,' '
    ret
key_enter:
    mov al,13
    ret

command:
    mov esi,buffer

    cmp byte[esi],0
    je command_end

    cmp dword[esi],"help"
    je cmd_help

    cmp dword[esi],"echo"
    je cmd_echo

    cmp dword[esi],"clea"
    je cmd_clear

    cmp dword[esi],"abou"
    je cmd_about

    mov esi,unknown_msg
    call print
    mov al,13
    call putchar

command_end:
    ret

cmd_help:
    mov esi,help_msg
    call print
    mov al,13
    call putchar
    ret

cmd_echo:
    mov esi,buffer
    add esi,5
    call print
    mov al,13
    call putchar
    ret

cmd_clear:
    call clear
    ret

cmd_about:
    mov esi,about_msg
    call print
    mov al,13
    call putchar
    ret

banner:
    db "stdOS 32bit kernel",13,0

prompt:
    db "> ",0

help_msg:
    db "help clear about echo",0

about_msg:
    db "stdOS hybrid kernel",0

unknown_msg:
    db "unknown command",0

buffer:
    times 128 db 0