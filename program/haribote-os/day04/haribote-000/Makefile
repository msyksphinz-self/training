run: helloos.img
	qemu-system-i386 -fda $^

helloos.img: ipl10.bin haribote.sys
	echo naskfunc.sys > haribote.name
	dd if=ipl10.bin     of=helloos.img count=2880 bs=512 conv=notrunc
	dd if=haribote.name of=helloos.img count=1 bs=512 seek=19 conv=notrunc
	dd if=haribote.sys  of=helloos.img count=1 bs=512 seek=33 conv=notrunc

%.o:%.c
	gcc -Wl,-s -m32 -c -fno-pic -nostdlib -T hrb.ld -o $@ $<

bootpack.bin: naskfunc.o bootpack.o 
	ld -m elf_i386 -e HariMain $^ -o $@

%.o: %.nas
	nasm $^ -o $@ -l %.lst

haribote.sys: asmhead.o bootpack.o naskfunc.o 
	cat $^ > $@

ipl10.bin: ipl10.nas
	nasm -o $@ $^

clean:
	rm -rf *.sys *.img *.o *.bin *.lst *.name *~
