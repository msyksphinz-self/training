#!/usr/bin/env python

from __future__ import print_function
import argparse
import time

import numpy as np
import six

import chainer
from chainer import computational_graph
from chainer import Link, Chain, ChainList
import chainer.functions as F
import chainer.links as L
from chainer import optimizers
from chainer import serializers

class Function3DimentionModel(chainer.FunctionSet):
    def __init__(self):
        super(Function3DimentionModel, self).__init__(
            fc1=chainer.functions.Linear( 1,16),
            fc2=chainer.functions.Linear(16,32),
            fc3=chainer.functions.Linear(32, 1),
    )

    def forward(self, x):
        h1 = self.fc1(x)
        h2 = self.fc2(h1)
        h3 = self.fc3(h2)
        return h3

    def train(self, x_data, y_data):
        x = chainer.Variable(x_data.astype(np.float32), volatile=False)
        y = chainer.Variable(y_data.astype(np.float32), volatile=False)
        h = self.forward(x)

        optimizer.zero_grads()
        # error = chainer.functions.sigmoid(h, y)
        error = F.mean_squared_error(h, y)
        error.backward()
        optimizer.update()

        # print("x: {}".format(x.data))
        # print("h: {}".format(h.data))
        # print("h_class: {}".format(h.data.argmax()))


n_epoch = 1000
n_units = 100

# Prepare dataset
print('load MNIST dataset')

with open('input_data.txt', "rU") as f:
    data = map(lambda x:x.split(","), f.read().strip().split("\n"))

N = 100000
x_train_int = np.transpose(data)[0]
y_train_int = np.transpose(data)[1]

x_train = np.array(map(np.float32, x_train_int))
y_train = np.array(map(np.float32, y_train_int))

xp = np

model = Function3DimentionModel()
optimizer = chainer.optimizers.Adam()
optimizer.setup(model)

# n_epoch = N / batchsize
batchsize = 100
n_epoch = 100

# Learning loop
for epoch in six.moves.range(1, n_epoch + 1):
    print('epoch', epoch)

    perm = np.random.permutation(N)

    for i in six.moves.range(0, N, batchsize):
        x_part = x_train[perm[i:i + batchsize]].reshape(batchsize, 1)
        y_part = y_train[perm[i:i + batchsize]].reshape(batchsize, 1)

        model.train (x_part, y_part)


sum_accuracy = 0
sum_loss = 0
for i in six.moves.range(0, 100, batchsize):
    x_batch = x_train[i:i + batchsize].reshape(batchsize, 1)
    y_batch = y_train[i:i + batchsize].reshape(batchsize, 1)

    y = model.forward(x_batch)

    for b in six.moves.range(0, batchsize):
        print (x_batch[b][0], y.data[b][0])
    
    # x_test = chainer.Variable(np.asarray(x_batch))
    # loss = F.mean_squared_error(y.data, )

    
    # print ('loss = {}'.format(loss.data / N_test))

    # print (x_batch)
    # print (y.data)
    # print (y_array)
