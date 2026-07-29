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

    cmp al,0x0e
    je key_backspace

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
key_backspace:
    mov al,0x08
    ret

key_enter:
    mov al,13
    ret