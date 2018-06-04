all: init_os.o

CFLAGS = 
# CFLAGS += --target=i686-unknown-linux-gnu
CFLAGS += --crate-type=staticlib
CFLAGS += --emit=obj
CFLAGS += -C lto
CFLAGS += -C opt-level=2
CFLAGS += -C no-prepopulate-passes
# CFLAGS += -C no-stack-check
# CFLAGS += -Z verbose
# CFLAGS += -Z no-landing-pads
# CFLAGS += --extern core=libcore.rlib

%.o: %.rs Makefile
	rustc $(CFLAGS) -o $@ $< 
