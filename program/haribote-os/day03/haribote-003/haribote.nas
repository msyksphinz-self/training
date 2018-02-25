; haribote-nas
CLYS    equ 0x0ff0
LEDS    equ 0x0ff1
VMODE   equ 0x0ff2
SCRNX   equ 0x0ff4
SCRNY   equ 0x0ff6
VRAM    equ 0x0ff8

    ORG 0xc200

    mov al, 0x13    ; VGA graphics 320x200x8bit color
    mov ah, 0x00
    int 0x10
    mov byte [VMODE], 8
    mov word [SCRNX], 320
    mov word [SCRNX], 200
    mov dword [VRAM], 0x000a0000

; tell keyboard LED status
    mov ah, 0x02
    int 0x16    ; keyboard BIOS
    mov [LEDS],al

fin:
	hlt
	jmp	fin
