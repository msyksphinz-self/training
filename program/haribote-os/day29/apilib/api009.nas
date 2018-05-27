GLOBAL  api_malloc

[SECTION .text]

api_malloc: ; char *api_malloc (int size);
    push    ebx
    mov     edx, 9
    mov     ebx, [cs:0x0020]
    mov     ecx, [esp+8]
    int     0x40
    pop     ebx
    ret

