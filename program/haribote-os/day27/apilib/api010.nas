GLOBAL  api_free

[SECTION .text]

api_free: ; void api_free (char *addr, int size);
    push    ebx
    mov     edx, 10
    mov     ebx, [cs:0x0020]
    mov     eax, [esp + 8]
    mov     ecx, [esp + 12]
    int     0x40
    pop     ebx
    ret

