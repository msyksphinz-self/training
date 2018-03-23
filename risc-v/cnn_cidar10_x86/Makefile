ARCH_GNU_TOOLS = x64
# ARCH_GNU_TOOLS = riscv64-unknown-elf-

TARGET = 
TARGET += cnn_cifar10
TARGET += cnn_cifar10_full
TARGET += cnn_cifar10_hw_full
TARGET += cnn_cifar10_hw
TARGET += cnn_cifar10_debug
TARGET += cnn_cifar10_hw_debug

CFLAGS += -O3 -fno-builtin-printf
CFLAGS += -I$(abspath libfixmath/libfixmath/)
CFLAGS += -DFIXMATH_NO_OVERFLOW
CFLAGS += -DFIXMATH_NO_64BIT

TRAIN_FLAGS     :=
TRAIN_SRC_FILES :=
TRAIN_FLAGS2    :=
OBJCPY_FLAGS    :=
GNU_TARGET = 

DATA_OBJECT = cifar-10-batches-bin/data_batch_1.o

ifeq ($(ARCH_GNU_TOOLS),riscv64-uknown-elf-)
	GNU_TARGET = riscv64-uknown-elf-
	CFLAGS += -mabi=lp64

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

	TRAIN_SRC_FILES += syscalls.c
	TRAIN_SRC_FILES += t10k-labels-idx1-ubyte.o

	OBJCPY_FLAGS += -O elf64-littleriscv -B riscv
endif

ifeq ($(ARCH_GNU_TOOLS), x64)
	GNU_TARGET = 
	OBJCPY_FLAGS += -O elf64-x86-64 -B i386
endif

OBJCPY_FLAGS    += -I binary
TRAIN_SRC_FILES += conv_weight.c
TRAIN_SRC_FILES += cnn_cifar10.c
TRAIN_SRC_FILES += layers.c

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

cifar-10-batches-bin/%.o: cifar-10-batches-bin/%.bin
	$(GNU_TARGET)objcopy $(OBJCPY_FLAGS) --rename-section .data=.rodata $< $@

sbrk.o: sbrk.c
	$(GNU_TARGET)gcc $(CFLAGS) -c -shared -fpic $^
malloc.o: malloc.c
	$(GNU_TARGET)gcc $(CFLAGS) -c -shared -fpic $^


cifar10_dump : cifar10_dump.c $(DATA_OBJECT)
	$(GNU_TARGET)gcc -o $@ $^ $(TRAIN_FLAGS2)

cnn_cifar10: $(TRAIN_SRC_FILES) $(DATA_OBJECT)
	$(GNU_TARGET)gcc -o $@ $(TRAIN_FLAGS) -DIMAGE_SIZE_4000 $^ $(TRAIN_FLAGS2)

cnn_cifar10_full: $(TRAIN_SRC_FILES) $(DATA_OBJECT)
	$(GNU_TARGET)gcc -o $@$(TRAIN_FLAGS) $^ $(TRAIN_FLAGS2)

cnn_cifar10_hw: $(TRAIN_SRC_FILES) $(DATA_OBJECT)
	$(GNU_TARGET)gcc -o $@ $(TRAIN_FLAGS) -DROCC_MATRIX16 -DIMAGE_SIZE_4000 $^ $(TRAIN_FLAGS2)

cnn_cifar10_hw_full: $(TRAIN_SRC_FILES) $(DATA_OBJECT)
	$(GNU_TARGET)gcc -o $@ $(TRAIN_FLAGS) -DROCC_MATRIX16 $^ $(TRAIN_FLAGS2)

cnn_cifar10_debug: $(TRAIN_SRC_FILES) $(DATA_OBJECT)
	$(GNU_TARGET)gcc -o $@ $(TRAIN_FLAGS) -DDEBUG -DIMAGE_SIZE_4000 $^ $(TRAIN_FLAGS2)

cnn_cifar10_hw_debug: $(TRAIN_SRC_FILES) $(DATA_OBJECT)
	$(GNU_TARGET)gcc -o $@ $(TRAIN_FLAGS) -DDEBUG -DROCC_MATRIX16 -DIMAGE_SIZE_4000 $^ $(TRAIN_FLAGS2)

%.bin: cifar-10-binary.tar.gz
	tar xfz $^

%.gz:
	wget "https://www.cs.toronto.edu/~kriz/$@"

%_init.o: %.bin
	$(GNU_TARGET)objcopy $(OBJCPY_FLAGS) --rename-section .data=.rodata $< $@

clean:
	rm -rf *.o $(TARGET) cifar10_dump cifar10.dump $(DATA_OJBECT) cifar-10-batches-bin cifar-10-binary.tar.gz
