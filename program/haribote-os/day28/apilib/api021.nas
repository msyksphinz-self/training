[BITS 32]

GLOBAL	api_fopen

[SECTION .text]

api_fopen: ; void api_fopen (char *fname);
    push   ebx
    mov    edx, 21
    mov    ebx, [esp+8]
    int    0x40
    pop    ebx
