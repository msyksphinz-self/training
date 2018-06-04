all: ipl.bin secondboot.bin

%.bin: %.asm Makefile
	nasm -f bin -o $@ $< 
