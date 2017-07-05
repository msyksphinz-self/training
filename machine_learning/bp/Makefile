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
	gcc -std=gnu11 -O3 -g $^ -o $@ -lm

train_twolayernet_float: train_twolayernet_float.c
	gcc -std=gnu11 -O3 -g $^ -o $@ -lm

train_twolayernet_fix16: train_twolayernet_fix16.c
	gcc -std=gnu11 -O3 -g $^ -o $@ -lm -L./libfixmath/libfixmath -I./libfixmath/libfixmath -lfixmath

%-ubyte: %-ubyte.gz
	gunzip $^

%.gz:
	wget "http://yann.lecun.com/exdb/mnist/$@"

clean:
	rm -rf train_twolayernet train_twolayernet_float train_twolayernet_fix16
	rm -rf train_twolayernet_run train_twolayernet_float_run train_twolayernet_fix16_run
