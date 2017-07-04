all: train-images-idx3-ubyte train-labels-idx1-ubyte t10k-images-idx3-ubyte t10k-labels-idx1-ubyte
	gcc -std=gnu11 -O3 -g train_twolayernet.c -o train_twolayernet -lm
	gcc -std=gnu11 -O3 -g train_twolayernet_float.c -o train_twolayernet_float -lm
	gcc -std=gnu11 -O3 -g train_twolayernet_fix16.c -o train_twolayernet_fix16 -lm

%-ubyte: %-ubyte.gz
	gunzip $^

%.gz:
	wget "http://yann.lecun.com/exdb/mnist/$@"

clean:
	rm -rf train_twolayernet
