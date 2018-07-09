#include "keras_model.h"

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace keras;

// Step 1
// Dump keras model and input sample into text files
// python dump_to_simple_cpp.py -a example/my_nn_arch.json -w example/my_nn_weights.h5 -o example/dumped.nnet
// Step 2
// Use text files in c++ example. To compile:
// g++ keras_model.cc example_main.cc
// To execute:
// a.out

int main() {
  cout << "This is simple example with Keras neural network model loading into C++.\n"
           << "Keras model will be used in C++ for prediction only." << endl;

  KerasModel m("./dumped_mnist_cnn.nnet", ImageDataFormat_t::Last, false);

  for (int idx = 0; idx < 100; idx++) {
    std::stringstream filename;
    filename << "test_data/mnist_test_data_" << std::setfill('0') << std::setw(2) << idx << ".txt";

    DataChunk *sample = new DataChunk2D();
    sample->read_from_file(filename.str().c_str(), ImageDataFormat_t::Last);
    // std::cout << sample->get_3d().size() << std::endl;
    m.compute_output(sample);

    delete sample;
  }

  return 0;
}
