all: ipl.bin secondboot.bin naskfunc.o

%.bin: %.asm Makefile
	nasm -f bin -o $@ $< 

%.o: %.nas Makefile
	nasm -felf64 -o $@ $<

clean:
	$(RM) -rf *.bin *.o

