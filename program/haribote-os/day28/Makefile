TARGET = haribote

TOOLPATH=/home/msyksphinz/work/haribote_os/z_tools/
MAKEFONT = $(TOOLPATH)makefont

.PHONY:	run

target_size = $(shell wc -c $(TARGET).sys | cut -f1 -d' ')
target_byte_size = $(shell expr $(target_size) / 512 + 1)

OBJ_FILES =
OBJ_FILES += haribote/bootpack.o
OBJ_FILES += haribote/graphic.o
OBJ_FILES += haribote/dsctbl.o
OBJ_FILES += haribote/int.o
OBJ_FILES += haribote/naskfunc.o
OBJ_FILES += haribote/hankaku.o
OBJ_FILES += haribote/sprintf.o
OBJ_FILES += haribote/fifo.o
OBJ_FILES += haribote/keyboard.o
OBJ_FILES += haribote/mouse.o
OBJ_FILES += haribote/memory.o
OBJ_FILES += haribote/memtest.o
OBJ_FILES += haribote/sheets.o
OBJ_FILES += haribote/timer.o
OBJ_FILES += haribote/mtask.o
OBJ_FILES += haribote/console.o
OBJ_FILES += haribote/file.o

HRB_FILES =
HRB_FILES += hello/hello.hrb
HRB_FILES += hello2/hello2.hrb
HRB_FILES += hello3/hello3.hrb
HRB_FILES += hello4/hello4.hrb
HRB_FILES += hello5/hello5.hrb
HRB_FILES += a/a.hrb
HRB_FILES += winhelo/winhelo.hrb
HRB_FILES += winhelo2/winhelo2.hrb
HRB_FILES += winhelo3/winhelo3.hrb
HRB_FILES += star1/star1.hrb
HRB_FILES += stars/stars.hrb
HRB_FILES += stars2/stars2.hrb
HRB_FILES += lines/lines.hrb
HRB_FILES += walk/walk.hrb
HRB_FILES += noodle/noodle.hrb
HRB_FILES += beepdown/beepdown.hrb
HRB_FILES += color/color.hrb
HRB_FILES += color2/color2.hrb
HRB_FILES += sosu/sosu.hrb
HRB_FILES += type/type.hrb
HRB_FILES += iroha/iroha.hrb

full:
	$(MAKE) -C haribote
	$(MAKE) -C apilib
	$(MAKE) -C hello
	$(MAKE) -C hello2
	$(MAKE) -C hello3
	$(MAKE) -C hello4
	$(MAKE) -C hello5
	$(MAKE) -C a
	$(MAKE) -C winhelo
	$(MAKE) -C winhelo2
	$(MAKE) -C winhelo3
	$(MAKE) -C star1
	$(MAKE) -C stars
	$(MAKE) -C stars2
	$(MAKE) -C lines
	$(MAKE) -C walk
	$(MAKE) -C noodle
	$(MAKE) -C beepdown
	$(MAKE) -C color
	$(MAKE) -C color2
	$(MAKE) -C sosu
	$(MAKE) -C type
	$(MAKE) -C iroha
	$(MAKE) $(TARGET).img


$(TARGET).img: haribote/ipl20.bin $(TARGET).sys $(HRB_FILES)
	mformat -f 1440 -C -B haribote/ipl20.bin -i $(TARGET).img ::
	mcopy haribote/$(TARGET).sys -i $(TARGET).img ::
	mcopy haribote/ipl20.nas  	 -i $(TARGET).img ::
	mcopy $(HRB_FILES)        	 -i $(TARGET).img ::
	mcopy Makefile            	 -i $(TARGET).img ::
	mcopy nihongo.fnt            -i $(TARGET).img ::
	mcopy euc1.txt              -i $(TARGET).img ::


apilib.lib : Makefile $(OBJS_API)
	ar rcs $@ $(OBJS_API)


%.o:%.c Makefile
	gcc -Wall -m32 -c -fno-pic -nostdlib -fno-builtin -o $@ $<
	objdump -D $@ > $@.dmp

%.o:%.nas Makefile
	nasm -felf32 $< -o $@ -l $@.lst


%.bin: %.o
	objcopy -O binary $^ $@

# %.hrb: %.o apilib.lib Makefile
# 	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ $< apilib.lib
# %.hrb: %.nas
# 	nasm -felf32 $^ -o tmp.o -l $@.lst
# 	ld -m elf_i386 -e HariMain -n -Thrb.ld -static -o $@ tmp.o


asmhead.o: asmhead.nas
	nasm $^ -o $@ -l $^.lst

naskfunc.o: naskfunc.nas
	nasm -felf32 $^ -o $@ -l $^.lst

$(TARGET).bin: $(OBJ_FILES)
	ld -Map bootpack.map -m elf_i386 -e HariMain -n -Tharibote/hrb.ld -static -o $(TARGET).bin $^
	hexdump -C $(TARGET).bin > $(TARGET).bin.dmp

hankaku.c : hankaku.txt Makefile
	./hankaku.py

$(TARGET).sys: haribote/asmhead.o $(TARGET).bin
	cat $^ > $@

diff: $(TARGET).diff haribote_reference.diff
	diff -y -W200 $^

%.diff: %.img
	hexdump -C $^ > $@

run: $(TARGET).img
	/usr/bin/qemu-system-i386 -m 32M -fda $^

clean:
	rm -rf *.sys $(TARGET).img *.o */*.o */*.bin *.bin */*.lst *.lib *.name *~ */*.dmp */*.hrb */*.map
