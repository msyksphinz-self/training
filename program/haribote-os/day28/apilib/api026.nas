[BITS 32]

GLOBAL	api_cmdline

[SECTION .text]

api_cmdline: ; int api_cmdline (char *buf, int maxsize);
    push   ebx
    mov    edx, 26
    mov    ecx,[esp+12]  ; maxsize
    mov    ebx,[esp+ 8]  ; buf
    int    0x40
    pop    ebx
    ret


