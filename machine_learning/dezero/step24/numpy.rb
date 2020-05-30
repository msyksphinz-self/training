#!/usr/bin/ruby

require 'pycall'
sys = PyCall.import_module('sys')
p sys.path

require 'numpy'
np = Numpy
x = np.asarray([1,2,3,4,5])
y = np.array([10,20,30,40,50])
z = x * y
p x.sum()
p z
