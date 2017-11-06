# Import libraries for simulation
import tensorflow as tf
import numpy as np

# Imports for visualization
import PIL.Image
from cStringIO import StringIO
from IPython.display import clear_output, Image, display
import scipy.ndimage as nd

def DisplayFractal(a, fmt='jpeg'):
    """Display an array of iteration counts as a
    colorful picture of a fractal."""
    a_cyclic = (6.28*a/20.0).reshape(list(a.shape)+[1])
    img = np.concatenate([10+20*np.cos(a_cyclic),
                          30+50*np.sin(a_cyclic),
                          155-80*np.cos(a_cyclic)], 2)
    img[a==a.max()] = 0
    a = img
    a = np.uint8(np.clip(a, 0, 255))
    f = StringIO()
    PIL.Image.fromarray(a).save(f, fmt)
    display(Image(data=f.getvalue()))

filenames = ['mandelbrot.jpg']
# Create a queue that produces the filenames to read
filename_queue = tf.train.string_input_producer(filenames)
# Create a reader for the filequeue
reader = tf.WholeFileReader()
# Read in the files
key, value = reader.read(filename_queue)

image = tf.image.decode_jpeg(value)
resized_image = tf.image.resize_images(image, 300, 300)
encode = tf.image.encode_jpeg(resized_image)

tf.initialize_all_variables().run()

encode.run()
