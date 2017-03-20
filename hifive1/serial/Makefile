TARGET = serial

# ARCH = riscv32-unknown-elf-
SIFIVE_PATH=/home/msyksphinz/.arduino15/packages/sifive
TOOL_PATH=$(SIFIVE_PATH)/tools/riscv32-unknown-elf-gcc/3f7b3696217548bc31aeccf9a0c89bdfa4e16a8f/

ARCH = $(TOOL_PATH)bin/riscv32-unknown-elf-
CC  = $(ARCH)gcc
CXX = $(ARCH)g++
AR  = $(ARCH)ar

OBJ_FILES = main.o start.o entry.o init.o wiring.o fe300prci_driver.o

PACKAGE_BASE = $(SIFIVE_PATH)/hardware/riscv/1.0.2
LINKER = $(PACKAGE_BASE)/freedom-e-sdk/bsp/env/freedom-e300-hifive1/link.lds

INC_PATH += $(PACKAGE_BASE)/system/include
INC_PATH += $(PACKAGE_BASE)/freedom-e-sdk/bsp/drivers/
INC_PATH += $(PACKAGE_BASE)/freedom-e-sdk/bsp/env/freedom-e300-hifive1/
INC_PATH += $(PACKAGE_BASE)/freedom-e-sdk/bsp/env/
INC_PATH += $(PACKAGE_BASE)/freedom-e-sdk/bsp/include/
INC_PATH += $(PACKAGE_BASE)/freedom-e-sdk/bsp/include/sifive/devices/
INC_PATH += $(PACKAGE_BASE)/variants/standard/
INC_PATH += $(PACKAGE_BASE)/cores/arduino/

LIB_PATH += $(TOOL_PATH)riscv32-unknown-elf/lib32/ilp32/

DEF_PARAM += ARDUINO=10801
DEF_PARAM += F_CPU=256000000LL
DEF_PARAM += FREEDOM_E300_HIFIVE1

INCLUDE = $(addprefix -I, $(INC_PATH))
DEFINE  = $(addprefix -D, $(DEF_PARAM))

ASFLAGS += -march=rv32imac $(INCLUDE) $(DEFINE)
ASFLAGS += -O2

CFLAGS += -O2
CFLAGS += -march=rv32imac
CFLAGS += -fpeel-loops
CFLAGS += -ffreestanding
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fpermissive
CFLAGS += -Wall
CFLAGS += -fno-rtti
CFLAGS += -fno-exceptions
CFLAGS += -include sys/cdefs.h
CFLAGS += -g

ASFLAGS += -march=rv32imac
ASFLAGS += -g

LFLAGS1 += -nostartfiles
LFLAGS1 += -Wl,-N
LFLAGS1 += -Wl,--gc-sections
LFLAGS1 += -Wl,--wrap=malloc
LFLAGS1 += -Wl,--wrap=free
LFLAGS1 += -Wl,--wrap=sbrk
LFLAGS2 += -nostdlib
LFLAGS2 += -Wl,--start-group
LFLAGS2 += -lm
LFLAGS2 += -lstdc++
LFLAGS2 += -lc
LFLAGS2 += -lgloss
LFLAGS2 += -Wl,--end-group
LFLAGS2 += -lgcc

all: $(TARGET)

%.o: %.cpp
	$(CXX) -c $(INCLUDE) $(DEFINE) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CXX) -c $(INCLUDE) $(DEFINE) $(CFLAGS) $< -o $@

%.o: %.S
	$(CC) -c $(INCLUDE) $(DEFINE) $(ASFLAGS) $< -o $@

wiring.o:wiring.c
	$(CC) -c $(INCLUDE) $(DEFINE) $(CFLAGS) $^ -o $@

fe300prci_driver.o:fe300prci_driver.c
	$(CC) -c $(INCLUDE) $(DEFINE) $(CFLAGS) $^ -o $@


$(TARGET): $(TARGET).o $(OBJ_FILES)
	$(CXX) -T$(LINKER) $(LFLAGS1) $^ $(LFLAGS2) -o $@

PATH := $(SIFIVE_PATH)/tools/openocd/9bab0782d313679bb0bfb634e6e87c757b8d5503/bin/:$(PATH)

write:
	$(PACKAGE_BASE)/freedom-e-sdk/bsp/tools/openocd_upload.sh \
	serial \
	$(PACKAGE_BASE)/freedom-e-sdk/bsp/env/freedom-e300-hifive1/openocd.cfg

clean:
	rm -rf *.o *.a $(TARGET) openocd_upload.log
