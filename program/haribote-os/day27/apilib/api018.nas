GLOBAL	api_settimer

[SECTION .text]

api_settimer: ; void api_settimer (int timer, int time);
	push	ebx
	mov		edx, 18
	mov		ebx, [esp +  8]
	mov		eax, [esp + 12]
	int		0x40
	pop		ebx
	ret

