[BITS 32]

GLOBAL	api_fread

[SECTION .text]

api_fread: ; int api_fread (char *buf, int maxsize, int fhandle);
    push   ebx
    mov    edx, 25
    mov    eax,[esp+16]
    mov    ecx,[esp+12]
    mov    ebx,[esp+ 8]
    int    0x40
    pop    ebx
    ret

