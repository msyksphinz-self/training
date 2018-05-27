[BITS 32]

GLOBAL	api_fsize

[SECTION .text]

api_fsize: ; int api_fsize (int fhandle, int mode);
    mov    edx, 24
    mov    eax, [esp + 4]
    mov    ecx, [esp + 8]
    int    0x40
    ret
    