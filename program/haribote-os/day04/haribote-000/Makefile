run: helloos.img
	qemu-system-i386 -fda $^

helloos.img: ipl10.bin haribote.sys
	echo haribote.sys > haribote.name
	dd if=ipl10.bin     of=helloos.img count=2880 bs=512 conv=notrunc
	dd if=haribote.name of=helloos.img count=1 bs=512 seek=19 conv=notrunc
	dd if=haribote.sys  of=helloos.img count=4 bs=512 seek=33 conv=notrunc


%.o:%.c
	gcc -m32 -c -fno-pic -nostdlib -o $@ $<

%.bin: %.o
	objcopy -O binary $^ $@ 

asmhead.o: asmhead.nas
	nasm $^ -o $@ -l $^.lst

naskfunc.o: naskfunc.nas
	nasm -felf32 $^ -o $@ -l $^.lst

haribote.bin: bootpack.o naskfunc.o 
	ld -m elf_i386 -e HariMain -n -Ttext 0xc200 -Thrb.ld -static -o haribote.bin $^
#	strip -O binary $@

haribote.sys: asmhead.o haribote.bin
	cat $^ > $@

ipl10.bin: ipl10.nas
	nasm -fbin -o $@ $^ -l ipl10.lst

clean:
	rm -rf *.sys helloos.img *.o *.bin *.lst *.name *~
