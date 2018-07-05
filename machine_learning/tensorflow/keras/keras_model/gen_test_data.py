from keras.models import load_model
from keras.utils import np_utils
from keras.datasets import cifar10
import numpy as np

(x_train, y_train), (x_test, y_test) = cifar10.load_data()

x_test  = x_test.astype('float32')
x_test  /= 255

f = open("cifar10_test_data.txt", 'w')

f.write("%d %d %d\n" % (x_test[0].shape[0], x_test[0].shape[1], x_test[0].shape[2]))

for y in range(x_test[0].shape[0]):
    for x in range(x_test[0].shape[1]):
        f.write("[ ")
        for ch in range(x_test[0].shape[2]):
            f.write("%f " % x_test[0][y,x,ch])
        f.write("]\n")

f.close()
