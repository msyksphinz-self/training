# i686-unknown-linux-gnu-ld -v -nostdlib -Tdata=0x00310000 init_os.o -T kernel.ld -o kernel.bin

TARGET = haribote

default:
	$(MAKE) $(TARGET).img

run: $(TARGET).img
	/usr/bin/qemu-system-i386 -m 32M -fda $^

asm/ipl.bin: 
	$(MAKE) -C asm $(notdir $@)

asm/secondboot.bin:
	$(MAKE) -C asm $(notdir $@)

asm/naskfunc.o:
	$(MAKE) -C asm $(notdir $@)

$(TARGET).sys: asm/secondboot.bin $(TARGET).bin
	cat $^ > $@

$(TARGET).bin: target/release/libharibote.a asm/naskfunc.o
	ld -v -nostdlib $^ -T kernel.ld -o $(TARGET).bin

target/release/libharibote.a: Makefile
	cargo build --release

$(TARGET).img: asm/ipl.bin $(TARGET).sys 
	mformat -f 1440 -C -B asm/ipl.bin -i $(TARGET).img ::
	mcopy $(TARGET).sys -i $(TARGET).img ::

clean:
	$(RM) -rf $(TARGET).sys $(TARGET).bin $(TARGET).img
	$(MAKE) -C asm clean

