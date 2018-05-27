[BITS 32]

GLOBAL	api_fseek

[SECTION .text]

api_fseek: ; void api_fseek (int fhandle, int offset, int mode);
    push   ebx
    mov    ebx, 23
    mov    eax, [esp+ 8]
    mov    ecx, [esp+16]
    mov    ebx, [esp+12]
    int    0x40
    pop    ebx
    ret
    