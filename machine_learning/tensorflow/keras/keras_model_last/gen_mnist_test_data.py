#!/usr/bin/python2.7

import keras
from keras.models import load_model
from keras.utils import np_utils
from keras.datasets import mnist
import numpy as np
import os

keras.backend.set_image_data_format("channels_last")

(x_train, y_train), (x_test, y_test) = mnist.load_data()

x_test  = x_test.astype('float32')
x_test  /= 255

if not os.path.exists("test_data"):
    os.mkdir("test_data")

for test_idx in range(100):
    f = open("test_data/mnist_test_data_%02d.txt" % test_idx, 'w')
    f.write("%d %d 1\n" % (x_test[test_idx].shape[0], x_test[test_idx].shape[1]))
    for y in range(x_test[test_idx].shape[0]):
        for x in range(x_test[test_idx].shape[1]):
            f.write("[ ")
            f.write("%f " % x_test[test_idx][y,x])
            f.write("]\n")
    f.close()
