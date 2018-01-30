TARGET = 
TARGET += train_twolayernet_fix16
TARGET += train_twolayernet_fix16_full
TARGET += train_twolayernet_fix16_hw_full
TARGET += train_twolayernet_fix16_hw
TARGET += train_twolayernet_fix16_debug
TARGET += train_twolayernet_fix16_hw_debug

C_SRCS := train_twolayernet_fix16.c
CFLAGS += -O3 -fno-builtin-printf
CFLAGS += -I$(abspath libfixmath/libfixmath/)
CFLAGS += -DFIXMATH_NO_OVERFLOW
CFLAGS += -DFIXMATH_NO_64BIT

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


all: $(TARGET)

$(DHRY_OBJS): %.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(DHRY_CFLAGS) -c -o $@ $<

BSP_BASE = ../../bsp

libfixmath.a:
	$(MAKE) -C ./libfixmath/libfixmath CC=$(CC) CC_FLAGS="$(CFLAGS)" LD_FLAGS="$(LDFLAGS)" RISCV_ARCH=$(RISCV_ARCH) RISCV_ABI=$(RISCV_ABI) AR=$(AR)

# IMAGE_SIZE = 4000000
IMAGE_SIZE = 4000

t10k-images-idx3-ubyte.small.o: t10k-images-idx3-ubyte
	head -c $(IMAGE_SIZE) $< > $<.small
	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv --rename-section .data=.rodata $<.small $@

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


TRAIN_FLAGS :=
TRAIN_FLAGS += -mabi=lp64
TRAIN_FLAGS += -DPREALLOCATE=1
TRAIN_FLAGS += -mcmodel=medany
TRAIN_FLAGS += -std=gnu99
TRAIN_FLAGS += -O2
TRAIN_FLAGS += -ffast-math
TRAIN_FLAGS += -fno-common
TRAIN_FLAGS += -fno-builtin-printf
TRAIN_FLAGS += -static
TRAIN_FLAGS += -nostartfiles
TRAIN_FLAGS += -nostdlib
TRAIN_FLAGS += -T ./test.ld

TRAIN_FLAGS2 += -lfixmath -L.
TRAIN_FLAGS2 += -Wl,--wrap=malloc
TRAIN_FLAGS2 += -Wl,--wrap=sbrk
TRAIN_FLAGS2 += -Wl,--wrap=free

TRAIN_SRC_FILES :=
TRAIN_SRC_FILES += train_twolayernet_fix16.c
TRAIN_SRC_FILES += syscalls.c
TRAIN_SRC_FILES += crt.S
TRAIN_SRC_FILES += wh0_init.o
TRAIN_SRC_FILES += wh1_init.o
TRAIN_SRC_FILES += wb0_init.o
TRAIN_SRC_FILES += wb1_init.o
TRAIN_SRC_FILES += sbrk.o
TRAIN_SRC_FILES += malloc.o
TRAIN_SRC_FILES += t10k-labels-idx1-ubyte.o


train_twolayernet_fix16: $(TRAIN_SRC_FILES) t10k-images-idx3-ubyte.small.o
	riscv64-unknown-elf-gcc -o $@ $(TRAIN_FLAGS) -DIMAGE_SIZE_4000 $^ $(TRAIN_FLAGS2)

train_twolayernet_fix16_full: $(TRAIN_SRC_FILES) t10k-images-idx3-ubyte.o
	riscv64-unknown-elf-gcc -o $@$(TRAIN_FLAGS) $^ $(TRAIN_FLAGS2)

train_twolayernet_fix16_hw: $(TRAIN_SRC_FILES) t10k-images-idx3-ubyte.small.o
	riscv64-unknown-elf-gcc -o $@ $(TRAIN_FLAGS) -DROCC_MATRIX16 -DIMAGE_SIZE_4000 $^ $(TRAIN_FLAGS2)

train_twolayernet_fix16_hw_full: $(TRAIN_SRC_FILES) t10k-images-idx3-ubyte.o
	riscv64-unknown-elf-gcc -o $@ $(TRAIN_FLAGS) -DROCC_MATRIX16 $^ $(TRAIN_FLAGS2)

train_twolayernet_fix16_debug: $(TRAIN_SRC_FILES) t10k-images-idx3-ubyte.small.o
	riscv64-unknown-elf-gcc -o $@ $(TRAIN_FLAGS) -DDEBUG -DIMAGE_SIZE_4000 $^ $(TRAIN_FLAGS2)

train_twolayernet_fix16_hw_debug: $(TRAIN_SRC_FILES) t10k-images-idx3-ubyte.small.o
	riscv64-unknown-elf-gcc -o $@ $(TRAIN_FLAGS) -DDEBUG -DROCC_MATRIX16 -DIMAGE_SIZE_4000 $^ $(TRAIN_FLAGS2)

%-ubyte: %-ubyte.gz
	gunzip $^

%.gz:
	wget "http://yann.lecun.com/exdb/mnist/$@"

%_init.o: %.bin
	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv --rename-section .data=.rodata $< $@
# %_init.o: %.bin
# 	riscv64-unknown-elf-objcopy -I binary -O elf64-littleriscv -B riscv $< $@

clean:
	rm -rf *.o train_twolayernet_fix16 train_twolayernet_fix16_full train_twolayernet_fix16_hw train_twolayernet_fix16_hw_full
