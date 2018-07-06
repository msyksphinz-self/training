from keras.models import load_model
from keras.utils import np_utils
from keras.datasets import cifar10

# import input_cifar
# import numpy as np

# returns a compiled model
# identical to the previous one
model = load_model('saved_models/keras_cifar10_trained_model.h5')

model.save('cifar10_cnn_model.h5') # creates a HDF5 file 'my_model.h5'
with open('cifar10_cnn_model.json', 'w') as fout:
    fout.write(model.to_json())
