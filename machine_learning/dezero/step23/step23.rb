#!/usr/bin/env ruby

require "./core_simple.rb"

x0 = Variable.new([1.0])
x1 = Variable.new([1.0])
t = add(x0, x1)
y = add(x0, t)
y.backward()

x = Variable.new([1, 2, 3])
puts x

x = Variable.new(nil)
puts x

x = Variable.new([[1, 2, 3], [4, 5, 6]])
puts x


a = Variable.new([3.0])
b = Variable.new([2.0])
c = Variable.new([5.0])
y = a + b * c
puts y


x = Variable.new([2.0])
y = x + [3.0]
puts y


x = Variable.new([2.0])
y = -x
puts y


a = Variable.new([3.0])
b = Variable.new([2.0])
y = a - b
puts y

a = Variable.new([3.0])
b = Variable.new([2.0])
y = a / b
puts y


a = Variable.new([3.0])
y = a ** 3.0
puts y
