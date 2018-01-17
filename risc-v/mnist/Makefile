TARGET = mnist

C_SRCS := train_twolayernet_fix16.c
CFLAGS += -O3 -fno-builtin-printf
CFLAGS += -I$(abspath libfixmath/libfixmath/)
CFLAGS += -DFIXMATH_NO_OVERFLOW
# CFLAGS += -DFIXMATH_NO_64BIT

LDFLAGS += -L$(abspath libfixmath/libfixmath/)
LDFLAGS += -lfixmath
# LDFLAGS += train-images-idx3-ubyte.100.o
LDFLAGS += t10k-images-idx3-ubyte.o
LDFLAGS += t10k-labels-idx1-ubyte.o
LDFLAGS += wb0_init.o
LDFLAGS += wb1_init.o
LDFLAGS += wh0_init.o
LDFLAGS += wh1_init.o

LINK_DEPS += libfixmath.a
LINK_DEPS += train_data
LINK_DEPS += t10k_data
# LINK_DEPS += train-images-idx3-ubyte.100.o
LINK_DEPS += t10k-images-idx3-ubyte.o
LINK_DEPS += t10k-labels-idx1-ubyte.o
LINK_DEPS += wb0_init.o
LINK_DEPS += wb1_init.o
LINK_DEPS += wh0_init.o
LINK_DEPS += wh1_init.o

CLEAN_OBJS += train-images-idx3-ubyte.100.o
CLEAN_OBJS += wb0_init.o
CLEAN_OBJS += wb1_init.o
CLEAN_OBJS += wh0_init.o
CLEAN_OBJS += wh1_init.o
CLEAN_OBJS += ./libfixmath/libfixmath/libfixmath.a


all: train_twolayernet_fix16 train_twolayernet_fix16_full train_twolayernet_fix16_hw

$(DHRY_OBJS): %.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(DHRY_CFLAGS) -c -o $@ $<

BSP_BASE = ../../bsp

libfixmath.a:
	$(MAKE) -C ./libfixmath/libfixmath CC=$(CC) CC_FLAGS="$(CFLAGS)" LD_FLAGS="$(LDFLAGS)" RISCV_ARCH=$(RISCV_ARCH) RISCV_ABI=$(RISCV_ABI) AR=$(AR)

# IMAGE_SIZE = 4000000
IMAGE_SIZE = 4000

train-images-idx3-ubyte.100.o: train-images-idx3-ubyte
	head -c $(IMAGE_SIZE) $< > $<.$(IMAGE_SIZE)
	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv --rename-section .data=.rodata $<.$(IMAGE_SIZE) $@

t10k-images-idx3-ubyte.$(IMAGE_SIZE).o: t10k-images-idx3-ubyte
	head -c $(IMAGE_SIZE) $< > $<.$(IMAGE_SIZE)
	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv --rename-section .data=.rodata $<.$(IMAGE_SIZE) $@

t10k-images-idx3-ubyte.o: t10k-images-idx3-ubyte
	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv --rename-section .data=.rodata $< $@

t10k-labels-idx1-ubyte.o: t10k-labels-idx1-ubyte
	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv --rename-section .data=.rodata $< $@

train_data: train-images-idx3-ubyte train-labels-idx1-ubyte
t10k_data: t10k-images-idx3-ubyte t10k-labels-idx1-ubyte

sbrk.o: sbrk.c
	riscv64-unknown-elf-gcc -mabi=lp64 -c -shared -fpic $^
malloc.o: malloc.c
	riscv64-unknown-elf-gcc -mabi=lp64 -c -shared -fpic $^


# train_twolayernet_fix16: train_twolayernet_fix16.c syscalls.c crt.S wh1_init.o wh0_init.o wb0_init.o wb1_init.o sbrk.o malloc.o t10k-labels-idx1-ubyte.o t10k-images-idx3-ubyte.o
train_twolayernet_fix16: train_twolayernet_fix16.c syscalls.c crt.S wh0_init.o wh1_init.o wb0_init.o wb1_init.o sbrk.o malloc.o t10k-labels-idx1-ubyte.o t10k-images-idx3-ubyte.4000.o
	riscv64-unknown-elf-gcc \
	-mabi=lp64 \
	-DPREALLOCATE=1 \
	-DIMAGE_SIZE_4000 \
	-mcmodel=medany \
	-std=gnu99 \
	-O2 \
	-ffast-math \
	-fno-common \
	-fno-builtin-printf \
	-static \
	-nostartfiles \
	-nostdlib \
	-T ./test.ld \
	-o $@ \
	$^ \
	-lfixmath -L. \
	-Wl,--wrap=malloc \
	-Wl,--wrap=sbrk \
	-Wl,--wrap=free

train_twolayernet_fix16_full: train_twolayernet_fix16.c syscalls.c crt.S wh0_init.o wh1_init.o wb0_init.o wb1_init.o sbrk.o malloc.o t10k-labels-idx1-ubyte.o t10k-images-idx3-ubyte.o
	riscv64-unknown-elf-gcc \
	-mabi=lp64 \
	-DPREALLOCATE=1 \
	-mcmodel=medany \
	-std=gnu99 \
	-O2 \
	-ffast-math \
	-fno-common \
	-fno-builtin-printf \
	-static \
	-nostartfiles \
	-nostdlib \
	-T ./test.ld \
	-o $@ \
	$^ \
	-lfixmath -L. \
	-Wl,--wrap=malloc \
	-Wl,--wrap=sbrk \
	-Wl,--wrap=free

train_twolayernet_fix16_hw: train_twolayernet_fix16.c syscalls.c crt.S wh0_init.o wh1_init.o wb0_init.o wb1_init.o sbrk.o malloc.o t10k-labels-idx1-ubyte.o t10k-images-idx3-ubyte.o
	riscv64-unknown-elf-gcc \
	-mabi=lp64 \
	-DPREALLOCATE=1 \
	-DROCC_MATRIX16 \
	-mcmodel=medany \
	-std=gnu99 \
	-O2 \
	-ffast-math \
	-fno-common \
	-fno-builtin-printf \
	-static \
	-nostartfiles \
	-nostdlib \
	-T ./test.ld \
	-o $@ \
	$^ \
	-lfixmath -L. \
	-Wl,--wrap=malloc \
	-Wl,--wrap=sbrk \
	-Wl,--wrap=free


%-ubyte: %-ubyte.gz
	gunzip $^

%.gz:
	wget "http://yann.lecun.com/exdb/mnist/$@"

%_init.o: %.bin
	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv --rename-section .data=.rodata $< $@
# %_init.o: %.bin
# 	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv $< $@

clean:
	rm *.o train_twolayernet_fix16
