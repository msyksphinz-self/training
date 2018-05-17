GLOBAL  api_getkey

[SECTION .text]

api_getkey: ; int api_getkey (int mode);
    mov		edx, 15
	mov		eax, [esp + 4];  mode
	int		0x40
	ret


