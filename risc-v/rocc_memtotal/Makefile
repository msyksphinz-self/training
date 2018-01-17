TARGET = rocc_memtotal

CC=riscv64-unknown-elf-gcc

C_SRCS := rocc_memtotal.c
CFLAGS += -O2
CFLAGS += -mcmodel=medany
CFLAGS += -ffast-math
CFLAGS += -fno-common
CFLAGS += -fno-builtin-printf
CFLAGS += -std=gnu99
CFLAGS += -mabi=lp64

LDFLAGS += -O2
LDFLAGS += -T./test.ld
LDFLAGS += -mcmodel=medany
LDFLAGS += -ffast-math
LDFLAGS += -fno-common
LDFLAGS += -fno-builtin-printf
LDFLAGS += -static
LDFLAGS += -nostartfiles
LDFLAGS += -nostdlib
LDFLAGS += -std=gnu99
LDFLAGS += -Wl,--wrap=malloc
LDFLAGS += -Wl,--wrap=sbrk
LDFLAGS += -Wl,--wrap=free
LDFLAGS += -mabi=lp64

all: $(TARGET)

$(TARGET): $(TARGET).o crt.S syscalls.c
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $^
