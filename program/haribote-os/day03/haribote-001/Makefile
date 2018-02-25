run: helloos.img
	qemu-system-i386 -fda $^

helloos.img: ipl.bin haribote.sys
	echo haribote.sys > haribote.name
	dd if=ipl.bin of=helloos.img count=1 bs=512 conv=notrunc
	dd if=haribote.name of=helloos.img count=1 bs=512 seek=19 conv=notrunc
	dd if=haribote.sys  of=helloos.img count=1 bs=512 seek=33 conv=notrunc

#	dd if=./bin/osname.bin of=./bin/fdimage/oslite.img count=1 bs=512 seek=19 conv=notrunc


haribote.sys: haribote.nas
	nasm $^ -o $@ -l haribote.lst

ipl.bin: helloos.nas
	nasm -o $@ $^

clean:
	rm -rf *.sys *.img *.o *.bin *.lst *.name *~
