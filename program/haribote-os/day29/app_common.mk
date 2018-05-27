default : $(APP).hrb

$(APP).hrb : $(APP).o $(OBJ_LIBRARIES) ./../apilib/apilib.lib Makefile 
	ld -m elf_i386 -e HariMain -n -T../haribote/hrb.ld -static -o $@ $<  $(OBJ_LIBRARIES) ../apilib/apilib.lib

%.o: %.nas Makefile
	nasm -felf32 $< -o $@ -l $@.lst

%.o: %.c Makefile
	gcc -Wall -m32 -c -fno-pic -nostdlib -fno-builtin -I../ -I../haribote/ -o $@ $<
	objdump -D $@ > $@.dmp

clean:
	rm -rf *.sys *.o *.bin *.lst *.name *~ *.dmp *.hrb *.map
