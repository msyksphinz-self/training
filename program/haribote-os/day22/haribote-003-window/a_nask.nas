;; [FORMAT "WCOFF"]
; [INSTRSET "i486p"]
[BITS 32]
; [FILE "a_nask.nas"]

GLOBAL  api_putchar
GLOBAL	api_putstr0
GLOBAL  api_end
GLOBAL  api_openwin

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

api_openwin:   ; int api_openwin (char *buf, int xsiz, int ysiz, int col_inv, char *title);
    push    edi
    push    esi
    push    ebx
    mov     edx, 5
    mov     ebx, [esp+16]    ; buf
    mov     esi, [esp+20]    ; xsiz
    mov     edi, [esp+24]    ; ysiz
    mov     eax, [esp+28]    ; col_inv
    mov     ecx, [esp+32]    ; title
    int     0x40
    pop     ebx
    pop     esi
    pop     edi
    ret

