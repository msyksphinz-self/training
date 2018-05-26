[BITS 32]

GLOBAL	api_fclose

[SECTION .text]

api_fclose: ; void api_fclose (int fhandle);
    mov     ebx,22
    mov     eax,[esp+4]
    int     0x40
    ret
    
