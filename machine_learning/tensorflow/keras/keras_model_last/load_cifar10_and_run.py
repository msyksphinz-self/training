from keras.models import load_model
from keras.utils import np_utils
from keras.datasets import cifar10
import numpy as np
from keras import backend as K

model = load_model('saved_models/keras_cifar10_trained_model.h5')

with open('saved_models/cifar10_cnn_model_keras1.json', 'w') as fout:
    fout.write(model.to_json())

(x_train, y_train), (x_test, y_test) = cifar10.load_data()
y_test = np_utils.to_categorical(y_test, 10)

x_train = x_train.astype('float32')
x_test  = x_test.astype('float32')
x_train /= 255
x_test  /= 255

print x_test[0]

# scores = model.evaluate(x_test, y_test)
# print('Test loss:', scores[0])
# print('Test accuracy:', scores[1])

# from keras.utils import plot_model
# plot_model(model, to_file='model.png', show_shapes=True)

for idx in range(0, 10):
    print(model.predict(np.expand_dims(x_test[idx], axis=0)))
    print(y_test[idx])

model.summary()
