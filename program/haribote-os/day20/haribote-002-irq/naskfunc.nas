; [FORMAT "WCOFF"]
; [INSTRSET "i486p"]
[BITS 32]
; [FILE "naskfunc.nas"]

		GLOBAL	io_hlt, io_cli, io_sti, io_stihlt
		GLOBAL	io_in8, io_in16, io_in32
		GLOBAL	io_out8, io_out16, io_out32
		GLOBAL	io_load_eflags, io_store_eflags
		GLOBAL	load_gdtr, load_idtr
		GLOBAL	load_cr0, store_cr0
		GLOBAL	asm_inthandler20, asm_inthandler21, asm_inthandler27, asm_inthandler2c
		GLOBAL	asm_cons_putchar
        GLOBAL  load_tr, farjmp, farcall
		EXTERN	inthandler20, inthandler21, inthandler27, inthandler2c
		EXTERN	cons_putchar

[SECTION .text]

io_hlt:	; void io_hlt(void);
		hlt
		ret

io_cli:	; void io_cli (void);
		cli
		ret

io_sti: ; void io_sti (void);
		sti
		ret

io_stihlt: ; void io_stihlt (void);
		sti
		hlt
		ret

io_in8: ; io_in8 (int port);
		mov		 edx,[esp+4]   ; port
		mov		 eax, 0
		in		 al, dx
		ret

io_in16: ; int io_in16 (int port);
		mov		 edx,[esp+4]   ; port
		mov		 eax, 0
		in		 ax, dx
		ret

io_in32: ; int io_in32 (int port);
		mov		 edx,[esp+4]   ; port
		in		 eax, dx
		ret

io_out8: ; void io_out8(int port, int data);
		mov		edx,[esp+4]	  ; port
		mov		al, [esp+8]	  ; data
		out		dx, al
		ret

io_out16: ; void io_out16(int port, int data);
		mov		edx,[esp+4]	  ; port
		mov		ax, [esp+8]	  ; data
		out		dx, ax
		ret

io_out32: ; void io_out32(int port, int data);
		mov		edx,[esp+4]	  ; port
		mov		eax,[esp+8]	  ; data
		out		dx, eax
		ret

io_load_eflags: ; int io_load_eflags (void);
		pushfd	; push eflags
		pop		eax
		ret

io_store_eflags: ; void io_store_eflags (int eflags);
		mov		 eax,[esp+4]
		push	 eax
		popfd	 ; pop eflags
		ret


load_gdtr:		; void load_gdtr(int limit, int addr);
		mov		ax,[esp+4]		; limit
		mov		[esp+6],AX
		lgdt	[esp+6]
		ret

load_idtr:		; void load_idtr(int limit, int addr);
		mov		ax,[esp+4]		; limit
		mov		[esp+6],AX
		lidt	[esp+6]
		ret

load_cr0:       ; int load_cr0 (void);
    mov     eax, cr0
    ret

store_cr0:      ; void store_cr0 (int cr0);
    mov     eax,[esp+4]
    mov     cr0, eax
    ret

load_tr:        ; void load_tr(int tr);
    ltr         [esp+4]
    ret

farjmp:    ; void farjmp(int eip, int es);
    jmp         far[esp+4]
    ret


asm_inthandler20:
		push	es
		push	ds
		pushad
		mov		eax,esp
		push	eax
		mov		ax,ss
		mov		ds,ax
		mov		es,ax
		call	inthandler20
		pop		eax
		popad
		pop		ds
		pop		es
		iretd


asm_inthandler21:
		push	es
		push	ds
		pushad
		mov		eax,esp
		push	eax
		mov		ax,ss
		mov		ds,ax
		mov		es,ax
		call	inthandler21
		pop		eax
		popad
		pop		ds
		pop		es
		iretd

asm_inthandler27:
		push	es
		push	ds
		pushad
		mov		eax,esp
		push	eax
		mov		ax,ss
		mov		ds,ax
		mov		es,ax
		call	inthandler27
		pop		eax
		popad
		pop		ds
		pop		es
		iretd

asm_inthandler2c:
		push	es
		push	ds
		pushad
		mov		eax,esp
		push	eax
		mov		ax,ss
		mov		ds,ax
		mov		es,ax
		call	inthandler2c
		pop		eax
		popad
		pop		ds
		pop		es
		iretd

asm_cons_putchar:   ; void asm_cons_putchar (char a)
        sti
		push	1
		and		eax, 0xff
		push	eax
		push	dword[0x0fec]
		call	cons_putchar
		add		esp, 12
		iretd

farcall:    ; void farcall (int eip, int cs)
		call  far[esp+4]
		ret
		