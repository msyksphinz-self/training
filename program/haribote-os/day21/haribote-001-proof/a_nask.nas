; [FORMAT "WCOFF"]
; [INSTRSET "i486p"]
[BITS 32]
; [FILE "a_nask.nas"]

GLOBAL  api_putchar

[SECTION .text]

api_putchar:   ; void api_putchar (int c);
    mov        edx, 1
    mov        al,[esp+4]   ; c
    int        0x40
    ret