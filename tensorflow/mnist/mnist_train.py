import tensorflow as tf
import input_data

IMG_WIDTH=28

mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)
img_gen = tf.Variable(tf.random_uniform([IMG_WIDTH*IMG_WIDTH, 1000], 0.0, 1.0))

init = tf.initialize_all_variables()
sess = tf.Session()
sess.run(init)

batch_xs, batch_ys = mnist.train.next_batch(1)
for y in range(28):
      for x in range(28):
            if (batch_xs[0][y*28+x] != 0):
                  print "1",
            else:
                  print "0",
      print ""
      

for idx in range(10):      
      for y in range(IMG_WIDTH):
            for x in range(IMG_WIDTH):
                  print sess.run(img_gen)[idx][IMG_WIDTH*y+x],
            print ""
      


                      

      
for i in range(1000):
      batch_xs, batch_ys = mnist.train.next_batch(1)
      

#      sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})
      
