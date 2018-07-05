.PHONY:

all: run_cifar10

run_cifar10: keras_model.cc example_main.cc dumped.nnet
	g++ -g -Wall -O0 -std=c++11 keras_model.cc example_main.cc -o $@

dumped.nnet: ../keras2cpp/dump_to_simple_cpp.py cifar10_cnn_model_keras1.json cifar10_cnn_model_keras1.h5
	python2.7 $< -a cifar10_cnn_model_keras1.json -w cifar10_cnn_model_keras1.h5 -o $@
