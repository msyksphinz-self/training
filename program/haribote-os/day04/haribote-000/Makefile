TARGET = haribote


run: $(TARGET).img
	qemu-system-i386 -fda $^

$(TARGET).img: ipl10.bin $(TARGET).sys
	echo $(TARGET).sys > $(TARGET).name
	dd if=ipl10.bin     of=$(TARGET).img count=2880 bs=512 conv=notrunc
	dd if=$(TARGET).name of=$(TARGET).img count=1 bs=512 seek=19 conv=notrunc
	dd if=$(TARGET).sys  of=$(TARGET).img count=4 bs=512 seek=33 conv=notrunc


%.o:%.c
	gcc -m32 -c -fno-pic -nostdlib -o $@ $<

%.bin: %.o
	objcopy -O binary $^ $@ 

asmhead.o: asmhead.nas
	nasm $^ -o $@ -l $^.lst

naskfunc.o: naskfunc.nas
	nasm -felf32 $^ -o $@ -l $^.lst

$(TARGET).bin: bootpack.o naskfunc.o 
	ld -m elf_i386 -e HariMain -n -Ttext 0xc200 -Thrb.ld -static -o $(TARGET).bin $^
#	strip -O binary $@

$(TARGET).sys: asmhead.o $(TARGET).bin
	cat $^ > $@

ipl10.bin: ipl10.nas
	nasm -fbin -o $@ $^ -l ipl10.lst

clean:
	rm -rf *.sys $(TARGET).img *.o *.bin *.lst *.name *~
