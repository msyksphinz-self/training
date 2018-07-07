import keras
from keras.models import load_model
from keras.utils import np_utils
from keras.datasets import cifar10
import numpy as np
import os

keras.backend.set_image_data_format("channels_last")

(x_train, y_train), (x_test, y_test) = cifar10.load_data()

x_test  = x_test.astype('float32')
x_test  /= 255

os.mkdir("test_data")

for test_idx in range(100):
    f = open("test_data/cifar10_test_data_%02d.txt" % test_idx, 'w')
    f.write("%d %d %d\n" % (x_test[test_idx].shape[0], x_test[test_idx].shape[1], x_test[test_idx].shape[2]))
    for y in range(x_test[test_idx].shape[0]):
        for x in range(x_test[test_idx].shape[1]):
            f.write("[ ")
            for ch in range(x_test[test_idx].shape[2]):
                f.write("%f " % x_test[test_idx][y,x,ch])
            f.write("]\n")
    f.close()
