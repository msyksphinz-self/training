#!/usr/bin/python2.7

import os
import sys
from keras.models import load_model
from keras.utils import np_utils

sys.path.append("../keras/examples")

import mnist_cnn

save_dir = os.path.join(os.getcwd(), 'saved_models')
model_name = 'keras_mnist_cnn_trained_model'

# Save model and weights
if not os.path.isdir(save_dir):
    os.makedirs(save_dir)
model_path = os.path.join(save_dir, model_name + '.h5')
mnist_cnn.model.save(model_path)
print('Saved trained model at %s ' % model_path)

weight_path = os.path.join(save_dir, model_name + '.json')
with open(weight_path, 'w') as fout:
    fout.write(mnist_cnn.model.to_json())
print('Saved trained weights at %s ' % weight_path)
