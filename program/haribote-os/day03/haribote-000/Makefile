run: helloos.img
	qemu-system-i386 -fda $^

helloos.img: helloos.nas
	nasm $^ -o $@ 

clean:
	rm -rf *.sys *.img *.o *.bin *.lst *.name *~
