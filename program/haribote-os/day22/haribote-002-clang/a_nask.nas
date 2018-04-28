;; [FORMAT "WCOFF"]
; [INSTRSET "i486p"]
[BITS 32]
; [FILE "a_nask.nas"]

GLOBAL  api_putchar
GLOBAL	api_putstr0
GLOBAL  api_end

[SECTION .text]

api_putchar:   ; void api_putchar (int c);
    mov     edx, 1
    mov     al,[esp+4]   ; c
    int     0x40
    ret

api_putstr0:   ; void api_putstr0 (char *s);
	push	ebx
	mov		edx, 2
	mov		ebx, [esp+8]  ; s
	int		0x40
	pop		ebx
	ret
	
api_end:  ; void api_end (void);
    mov     edx, 4
    int     0x40
