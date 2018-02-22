	ORG		0x7c00
 	JMP		entry
	
	DB		0x90
	DB		"HELLOIPL"
	DW		512
	DB		1
	DW		1
	DB		2
	DW		224
	DW		2880
	DB		0xf0
	DW		9
	DW		18
	DW		2
	DD		0
	DD		2880
	DB		0,0,0x29
	DD		0xffffffff
	DB		"HELLO-OS   "
	DB		"FAT12   "
	RESB	18

; Main Program

entry:
	mov		ax, 0
	mov		ss, ax
	mov		sp, 0x7c00
	mov		ds, ax
	mov		es, ax

	mov		si, msg

putloop:
	mov		al, [si]
	add		si, 1
	cmp		al, 0
	je		fin
	mov		ah, 0x0e
	mov		bx, 15
	int		0x10
	jmp		putloop

fin:
	hlt
	jmp	fin

msg:
	DB		0x0a, 0x0a
	DB		"hello, world"
	DB		0x0a
	DB		0

	RESB 	0x1fe-($-$$)

	DB 		0x55, 0xaa

	DB 		0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
	RESB 	4600
	DB	 	0xf0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00
	RESB 	1469432
