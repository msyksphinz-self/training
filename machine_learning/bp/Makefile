GPERF := off

DEFINES :=
COPTS :=

ifeq ($(GPERF), on)
	DEFINES += GPERF
	COPTS += -fno-inline
	COPTS += -g3
	LOPTS += -lprofiler
endif

DEFINES += FIXMATH_NO_OVERFLOW

COPTS += -std=gnu11
COPTS += -O3
COPTS += -g
COPTS += $(addprefix -D, $(DEFINES))

LOPTS += -lm

all: train_data t10k_data run

run: train_twolayernet_run train_twolayernet_float_run train_twolayernet_fix16_run

train_data: train-images-idx3-ubyte train-labels-idx1-ubyte
t10k_data: t10k-images-idx3-ubyte t10k-labels-idx1-ubyte

train_twolayernet_run       : train_twolayernet
	./$^ > $@
train_twolayernet_float_run	: train_twolayernet_float
	./$^ > $@
train_twolayernet_fix16_run	: train_twolayernet_fix16
	./$^ > $@

train_twolayernet: train_twolayernet.c
	gcc $(COPTS) $^ -o $@ $(LOPTS)

train_twolayernet_float: train_twolayernet_float.c
	gcc $(COPTS) $^ -o $@ $(LOPTS)

train_twolayernet_fix16: train_twolayernet_fix16.c
	gcc $(COPTS) $^ -o $@ -L./libfixmath/libfixmath -I./libfixmath/libfixmath -lfixmath $(LOPTS)

%-ubyte: %-ubyte.gz
	gunzip $^

%.gz:
	wget "http://yann.lecun.com/exdb/mnist/$@"

prof: train_twolayernet.prof train_twolayernet_float.prof train_twolayernet_fix16.prof

train_twolayernet.prof: train_twolayernet_run
	google-pprof --callgrind train_twolayernet $@ > train_twolayernet.cg
train_twolayernet_float.prof: train_twolayernet_float_run
	google-pprof --callgrind train_twolayernet_float $@ > train_twolayernet_float.cg
train_twolayernet_fix16.prof: train_twolayernet_fix16_run
	google-pprof --callgrind train_twolayernet_fix16 $@ > train_twolayernet_fix16.cg

clean:
	rm -rf train_twolayernet train_twolayernet_float train_twolayernet_fix16
	rm -rf train_twolayernet_run train_twolayernet_float_run train_twolayernet_fix16_run
	rm -rf *.prof *.cg
