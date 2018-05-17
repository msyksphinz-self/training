[SECTION .text]

GLOBAL  api_end

api_end:  ; void api_end (void);
    mov     edx, 4
    int     0x40


