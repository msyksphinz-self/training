import tensorflow as tf
import input_data
import numpy as np
import pandas as pd
from pandas import Series, DataFrame
from numpy.random import randint, rand

# IMG_WIDTH=28
IMG_WIDTH=28
K = 10   # types of image

# make first image generator
mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)
img_gen = tf.Variable(tf.random_uniform([K, IMG_WIDTH*IMG_WIDTH], 0.0, 1.0))

img = tf.placeholder(tf.float32, shape=(IMG_WIDTH, IMG_WIDTH))

tf.mul(tf.equal(img_gen, img), tf.not_equal(img_gen, img))

img_p = tf.reduce_sum(img_gen, 1)

      
init = tf.initialize_all_variables()
sess = tf.Session()
sess.run(init)

img_gen_run = sess.run(img_gen)

for idx in range(K):
      for y in range(IMG_WIDTH):
            for x in range(IMG_WIDTH):
                  print img_gen_run[idx][IMG_WIDTH*y+x],
            print ""


            
# for i in range(1000):
#     batch_xs, batch_ys = mnist.train.next_batch(1)
#     sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})
