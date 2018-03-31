TARGET = haribote

TOOLPATH=/home/msyksphinz/work/haribote_os/z_tools/
MAKEFONT = $(TOOLPATH)makefont

run: $(TARGET).img
	qemu-system-i386 -m 32M -fda $^

target_size = $(shell wc -c $(TARGET).sys | cut -f1 -d' ')
target_byte_size = $(shell expr $(target_size) / 512 + 1)

OBJ_FILES =
OBJ_FILES += bootpack.o
OBJ_FILES += graphic.o
OBJ_FILES += dsctbl.o
OBJ_FILES += int.o
OBJ_FILES += naskfunc.o
OBJ_FILES += hankaku.o
OBJ_FILES += sprintf.o
OBJ_FILES += fifo.o
OBJ_FILES += keyboard.o
OBJ_FILES += mouse.o
OBJ_FILES += memory.o
OBJ_FILES += memtest.o
OBJ_FILES += sheets.o
OBJ_FILES += timer.o

$(TARGET).img: ipl10.bin $(TARGET).sys
	echo $(TARGET).sys > $(TARGET).name
	dd if=ipl10.bin     of=$(TARGET).img count=2880 bs=512 conv=notrunc
	dd if=$(TARGET).name of=$(TARGET).img count=1  bs=512 seek=19 conv=notrunc
	dd if=$(TARGET).sys  of=$(TARGET).img count=$(target_byte_size) bs=512 seek=33 conv=notrunc


%.o:%.c
	gcc -m32 -O3 -c -fno-pic -nostdlib -fno-builtin -o $@ $<
	objdump -D $@ > $@.dmp

%.bin: %.oT
	objcopy -O binary $^ $@ 

asmhead.o: asmhead.nas
	nasm $^ -o $@ -l $^.lst

naskfunc.o: naskfunc.nas
	nasm -felf32 $^ -o $@ -l $^.lst

$(TARGET).bin: $(OBJ_FILES)
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $(TARGET).bin $^
	hexdump -C $(TARGET).bin > $(TARGET).bin.dmp

hankaku.c : hankaku.txt Makefile
	./hankaku.py

$(TARGET).sys: asmhead.o $(TARGET).bin
	cat $^ > $@

ipl10.bin: ipl10.nas
	nasm -fbin -o $@ $^ -l ipl10.lst

diff: $(TARGET).diff haribote_reference.diff
	diff -y -W200 $^

%.diff: %.img
	hexdump -C $^ > $@

clean:
	rm -rf *.sys $(TARGET).img *.o *.bin *.lst *.name *~ *.dmp
