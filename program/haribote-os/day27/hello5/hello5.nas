[BITS 32]
	  GLOBAL	HariMain

HariMain:
	mov		edx, 2
	mov		ebx, msg
	int		0x40
	mov		edx, 4
	int		0x40

[SECTION	.data]
msg:
	db		"hello, world", 0x0a, 0
	