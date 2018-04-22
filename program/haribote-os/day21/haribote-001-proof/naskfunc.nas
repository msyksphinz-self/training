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
		GLOBAL	asm_inthandler0d, asm_inthandler20, asm_inthandler21, asm_inthandler27, asm_inthandler2c
		GLOBAL	asm_hrb_api
        GLOBAL  load_tr, farjmp, farcall, start_app
		EXTERN	inthandler0d, inthandler20, inthandler21, inthandler27, inthandler2c
		EXTERN	cons_putchar, hrb_api

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
		PUSH	ES
		PUSH	DS
		PUSHAD
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
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSが動いているときに割り込まれたのでほぼ今までどおり
		MOV		EAX,ESP
		PUSH	SS				; 割り込まれたときのSSを保存
		PUSH	EAX				; 割り込まれたときのESPを保存
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler21
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	アプリが動いているときに割り込まれた
		MOV		EAX,1*8
		MOV		DS,AX			; とりあえずDSだけOS用にする
		MOV		ECX,[0xfe4]		; OSのESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; 割り込まれたときのSSを保存
		MOV		[ECX  ],ESP		; 割り込まれたときのESPを保存
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	inthandler21
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSをアプリ用に戻す
		MOV		ESP,ECX			; ESPもアプリ用に戻す
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSが動いているときに割り込まれたのでほぼ今までどおり
		MOV		EAX,ESP
		PUSH	SS				; 割り込まれたときのSSを保存
		PUSH	EAX				; 割り込まれたときのESPを保存
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler27
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	アプリが動いているときに割り込まれた
		MOV		EAX,1*8
		MOV		DS,AX			; とりあえずDSだけOS用にする
		MOV		ECX,[0xfe4]		; OSのESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; 割り込まれたときのSSを保存
		MOV		[ECX  ],ESP		; 割り込まれたときのESPを保存
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	inthandler27
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSをアプリ用に戻す
		MOV		ESP,ECX			; ESPもアプリ用に戻す
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		AX,SS
		CMP		AX,1*8
		JNE		.from_app
;	OSが動いているときに割り込まれたのでほぼ今までどおり
		MOV		EAX,ESP
		PUSH	SS				; 割り込まれたときのSSを保存
		PUSH	EAX				; 割り込まれたときのESPを保存
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	inthandler2c
		ADD		ESP,8
		POPAD
		POP		DS
		POP		ES
		IRETD
.from_app:
;	アプリが動いているときに割り込まれた
		MOV		EAX,1*8
		MOV		DS,AX			; とりあえずDSだけOS用にする
		MOV		ECX,[0xfe4]		; OSのESP
		ADD		ECX,-8
		MOV		[ECX+4],SS		; 割り込まれたときのSSを保存
		MOV		[ECX  ],ESP		; 割り込まれたときのESPを保存
		MOV		SS,AX
		MOV		ES,AX
		MOV		ESP,ECX
		CALL	inthandler2c
		POP		ECX
		POP		EAX
		MOV		SS,AX			; SSをアプリ用に戻す
		MOV		ESP,ECX			; ESPもアプリ用に戻す
		POPAD
		POP		DS
		POP		ES
		IRETD

asm_inthandler0d:
		STI
		PUSH	ES
		PUSH	DS
		PUSHAD
		mov		eax,esp
		push	eax
		mov		ax,ss
		mov		ds,ax
		mov		es,ax
		call	inthandler0d
		cmp		eax,0
		jne		end_app
		pop		eax
		popad
		pop		ds
		pop		es
		add		esp,4
		iretd
		

asm_cons_putchar:   ; void asm_cons_putchar (char a)
        sti
		pushad
		push	1
		and		eax, 0xff
		push	eax
		push	dword[0x0fec]
		call	cons_putchar
		add		esp, 12
		popad
		iretd

farcall:    ; void farcall (int eip, int cs)
		call  far[esp+4]
		ret

asm_hrb_api:
		push	ds
		push	es
		pushad
		pushad
		mov		ax,ss
		mov		ds,ax
		mov		es,ax

		call	hrb_api

		cmp		eax,0
		jne		end_app
		add		esp,32
		popad
		pop		es
		pop		ds
		iretd

end_app:
		mov		esp, [eax]
		popad
		ret

start_app:		; void start_app(int eip, int cs, int esp, int ds);
		PUSHAD		; 32ビットレジスタを全部保存しておく
		MOV		EAX,[ESP+36]	; アプリ用のEIP
		MOV		ECX,[ESP+40]	; アプリ用のCS
		MOV		EDX,[ESP+44]	; アプリ用のESP
		MOV		EBX,[ESP+48]	; アプリ用のDS/SS
		MOV		EBP,[ESP+52]	; アプリ用のDS/SS
		MOV		[EBP],  ESP
		MOV		[EBP+4],SS		; 
		CLI			; 切り替え中に割り込みが起きてほしくないので禁止
		MOV		ES,BX
		MOV		DS,BX
		MOV		FS,BX
		MOV		GS,BX
		MOV		ESP,EDX
		OR		ECX, 3
		OR		EBX, 3
		PUSH	EBX
		PUSH	EDX
		PUSH	ECX
		PUSH	EAX
		RETF
		
