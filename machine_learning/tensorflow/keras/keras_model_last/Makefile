.PHONY: make_model pre_compile run_cifar10



all: run_cifar10_cnn run_mnist_cnn

run_cifar10_cnn: keras_model.cc example_main.cc
	g++ -g -Wall -O0 -std=c++11 $^ -o $@

dumped.nnet: ../keras2cpp/dump_to_simple_cpp.py cifar10_cnn_model.json cifar10_cnn_model.h5
	python2.7 $< -a cifar10_cnn_model.json -w cifar10_cnn_model.h5 -o $@

dumped_mnist_cnn.nnet: ../keras2cpp/dump_to_simple_cpp.py
	python2.7 ../keras2cpp/dump_to_simple_cpp.py \
		-a saved_models/keras_mnist_cnn_trained_model.json \
		-w saved_models/keras_mnist_cnn_trained_model.h5	\
		-o $@

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

run_mnist_cnn: keras_model.cc mnist_cnn_main.cc dumped_mnist_cnn.nnet
	g++ -g -Wall -O0 -std=c++11 keras_model.cc mnist_cnn_main.cc -o $@

train_mnist_cnn:
	python2.7 ./mnist_cnn_make_model.py
