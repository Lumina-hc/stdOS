    command:
    mov esi,buffer
    cmp byte[esi],0
    je command_end
    cmp dword[esi],"help"
    je cmd_help
    cmp dword[esi],"echo"
    je cmd_echo
    cmp dword[esi],"clea"
    jne .next1
    cmp byte[esi+4],"n"
    je cmd_clear
.next1:
    cmp dword[esi],"abou"
    jne .next2
    cmp byte[esi+4],"t"
    je cmd_about
.next2:
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
