CYLS	equ	10

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

	mov		ax, 0x0820
	mov		es, ax
	mov		ch, 0		; cylinder=0
	mov		dh, 0		; head=0
	mov		cl, 2		; sector=2
readloop:
	mov		si, 0		; register that count num of errors
retry:
	mov		ah, 0x02	; ah=0x02, read disk
	mov		al, 1		; 1 sector
	mov		bx, 0
	mov		dl, 0x00	; A drive
	int		0x13		; interrupt call
	jnc		next
	add		si, 1
	cmp		si, 5
	jae		error		; jump error if si >= 5
	mov		ah, 0x00
	mov		dl, 0x00
	int		0x13		; system call: drive reset
	jmp		retry

next:
	mov		ax, es		; proceed address 0x200
	add		ax, 0x0020
	mov		es, ax
	add		cl, 1
	cmp		cl, 18
	jbe		readloop	; jump readloop if CL<=18, 
	mov		cl, 1
	add		dh, 1
	cmp		dh, 2
	jb		readloop	; jump readloop if dh < 2
	mov		dh, 0
	add		ch, 1
	cmp		ch, CYLS
	jb		readloop    ; jump readloop if CH < CYLS


	jmp		0xc200


error:
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
	HLT
	jmp		fin

msg:
	DB		0x0a, 0x0a
	DB		"hello, world"
	DB		0x0a
	DB		0

	RESB 	0x1fe-($-$$)

	DB 		0x55, 0xaa

	DB 		0xf0, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00
	RESB 	4600
	DB	 	0xf0, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00
	RESB 	1469432
