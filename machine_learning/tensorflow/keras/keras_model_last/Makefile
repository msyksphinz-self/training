.PHONY: make_model pre_compile run_cifar10



all: run_cifar10

run_cifar10: keras_model.cc example_main.cc
	g++ -g -Wall -O0 -std=c++11 $^ -o $@

dumped.nnet: ../keras2cpp/dump_to_simple_cpp.py cifar10_cnn_model.json cifar10_cnn_model.h5
	python2.7 $< -a cifar10_cnn_model.json -w cifar10_cnn_model.h5 -o $@

cifar10_cnn_model.json cifar10_cnn_model.h5: pre_compile
	python2.7 ../keras/examples/cifar10_cnn.py
	python2.7 ../keras/examples/save_model.py
	python2.7 ../keras/examples/gen_test_data.py
	python2.7 ../keras/examples/write_layer_output.py

pre_compile:
	ln -sf ../../keras_model/write_layer_output.py   ../keras/examples/
	ln -sf ../../keras_model/load_cifar10_and_run.py ../keras/examples/
	ln -sf ../../keras_model/save_model.py     		 ../keras/examples/
	ln -sf ../../keras_model/gen_test_data.py  		 ../keras/examples/
