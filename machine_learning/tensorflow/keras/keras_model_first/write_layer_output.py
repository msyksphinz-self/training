from keras.models import load_model
from keras.utils import np_utils
from keras.datasets import cifar10
import numpy as np
from keras import backend as K
import StringIO

def dump_mdarray(fp, mdarray):
    if len(mdarray.shape) == 1:
        for elem in mdarray:
            fp.write("%f " % elem)
        fp.write("\n")
    else:
        for sub_array in mdarray:
            dump_mdarray(fp, sub_array)

(x_train, y_train), (x_test, y_test) = cifar10.load_data()
y_test = np_utils.to_categorical(y_test, 10)

x_train = x_train.astype('float32')
x_test  = x_test.astype('float32')
x_train /= 255
x_test  /= 255

model = load_model('cifar10_cnn_model.h5')

layer_output = [np.expand_dims(x_test[0], axis=0),]

layer_idx = 0
for layer in model.layers:
    get_layer_output = K.function([layer.input],
                                  [layer.output])
    layer_output = get_layer_output(layer_output)

    fp = open("layer" + str(layer_idx) + "_output.txt", "w")
    dump_mdarray(fp, layer_output[0])
    layer_idx = layer_idx + 1
    fp.close()
