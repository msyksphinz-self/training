TARGET = haribote

TOOLPATH=/home/msyksphinz/work/haribote_os/z_tools/
MAKEFONT = $(TOOLPATH)makefont

.PHONY:	run 

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
OBJ_FILES += mtask.o
OBJ_FILES += console.o
OBJ_FILES += file.o

HRB_FILES =
HRB_FILES += hello.hrb
HRB_FILES += hello2.hrb
HRB_FILES += hello3.hrb
HRB_FILES += hello4.hrb
HRB_FILES += hello5.hrb
HRB_FILES += a.hrb
HRB_FILES += crack1.hrb
HRB_FILES += crack2.hrb
HRB_FILES += bug1.hrb
HRB_FILES += bug2.hrb
HRB_FILES += bug3.hrb


$(TARGET).img: ipl10.bin $(TARGET).sys $(HRB_FILES)
	mformat -f 1440 -C -B ipl10.bin -i $(TARGET).img ::
	mcopy $(TARGET).sys -i $(TARGET).img ::
	mcopy ipl10.nas  -i $(TARGET).img ::
	mcopy hello.hrb  -i $(TARGET).img ::
	mcopy hello2.hrb -i $(TARGET).img ::
	mcopy hello3.hrb -i $(TARGET).img ::
	mcopy hello4.hrb hello5.hrb -i $(TARGET).img ::
	mcopy a.hrb      -i $(TARGET).img ::
	mcopy crack1.hrb -i $(TARGET).img ::
	mcopy crack2.hrb -i $(TARGET).img ::
	mcopy bug1.hrb   -i $(TARGET).img ::
	mcopy bug2.hrb   -i $(TARGET).img ::
	mcopy bug3.hrb   -i $(TARGET).img ::
	mcopy Makefile   -i $(TARGET).img ::

%.o:%.c Makefile
	gcc -Wall -m32 -O3 -c -fno-pic -nostdlib -fno-builtin -o $@ $<
	objdump -D $@ > $@.dmp

%.bin: %.o
	objcopy -O binary $^ $@ 

hello.hrb: hello.nas
	nasm -felf32 $^ -o tmp.o -l $@.lst
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ tmp.o

hello2.hrb: hello2.nas 
	nasm -felf32 $^ -o tmp.o -l $@.lst
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ tmp.o

%.hrb: %.o a_nask.o
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ $^
%.hrb: %.nas
	nasm -felf32 $^ -o tmp.o -l $@.lst
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ tmp.o

a.hrb: a.o a_nask.o
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ $^

a_nask.o: a_nask.nas
	nasm -felf32 $^ -o $@ -l $^.lst

crack1.hrb: crack1.o a_nask.o
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ $^

crack2.hrb: crack2.nas
	nasm -felf32 $^ -o tmp.o -l $^.lst
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ tmp.o

bug1.hrb: bug1.o a_nask.o
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ $^

bug2.hrb: bug2.o a_nask.o
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ $^

bug3.hrb: bug3.o a_nask.o
	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ $^

asmhead.o: asmhead.nas
	nasm $^ -o $@ -l $^.lst

naskfunc.o: naskfunc.nas
	nasm -felf32 $^ -o $@ -l $^.lst

$(TARGET).bin: $(OBJ_FILES)
	ld -Map bootpack.map -m elf_i386 -e HariMain -n -Thrb.ld -static -o $(TARGET).bin $^
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

run: $(TARGET).img
	qemu-system-i386 -m 32M -fda $^

clean:
	rm -rf *.sys $(TARGET).img *.o *.bin *.lst *.name *~ *.dmp *.hrb

