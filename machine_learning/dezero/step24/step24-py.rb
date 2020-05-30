#!/usr/bin/env ruby

require 'pycall'
sys = PyCall.import_module('sys')
p sys.path
require 'numpy'
np = Numpy

def as_array(x)
  np = Numpy

  if np.isscalar(x)
    return np.array(x)
  end
  return x
end


class Variable
  def initialize(data)
    @data = data
    @grad = nil
    @creator = nil
  end

  def set_creator(func)
    @creator = func
  end

  def backward(retain_grad=false)
    np = Numpy

    if @grad == nil
      @grad = np.ones_like(@data)
    end

    funcs = [@creator]

    while not funcs.empty? do
      f = funcs.pop
      x = f.input
      y = f.output
      x.grad = f.backward(y.grad)

      if x.creator != nil
        funcs.push(x.creator)
      end
    end
  end

  attr_accessor :data, :grad, :creator
end

class Function
  def call(input)
    x = input.data
    y = forward(x)
    output = Variable.new(as_array(y))
    output.set_creator(self)
    @input = input
    @output = output
    return output
  end
  def forward(x)
    raise NotImplementedError
  end
  def backward(gy)
    raise NotImplementedError
  end

  attr_accessor :input, :output
end


class Square < Function
  def forward(x)
    y = x ** 2
    return y
  end
  def backward(gy)
    x = @input.data
    gx = 2 * x * gy
    return gx
  end
end


class Exp < Function
  def forward(x)
    np = Numpy
    y = np.exp(x)
    return y
  end
  def backward(gy)
    np = Numpy
    x = @input.data
    gx = np.exp(x) * gy
    return gx
  end
end

def square(x)
  return Square.new().call(x)
end


def exp(x)
  return Exp.new().call(x)
end


begin
  x = Variable.new(np.array(1.0))
  puts(x.data.to_s)

  x = Variable.new(np.array(2.0))
  puts(x.data.to_s)
end

# begin
#   x = Variable.new(np.array(10))
#   f = Function.new()
#   y = f.call(x)
#   puts(y.class)
#   puts(y.data)
#
#   f2 = Square.new()
#   y2 = f2.call(x)
#   puts(y2.class)
#   puts(y2.data)
# end

# begin
#   A = Square.new()
#   B = Exp.new()
#   C = Square.new()
#
#   x = Variable.new(np.array(0.5))
#   a = A.call(x)
#   b = B.call(a)
#   y = C.call(b)
#
#   puts(y.data)
#
#   y.grad = np.array(1.0)
#   b.grad = C.backward(y.grad)
#   a.grad = B.backward(b.grad)
#   x.grad = A.backward(a.grad)
#
#   puts(x.grad)
# end

begin
  A = Square.new()
  B = Exp.new()
  C = Square.new()

  x = Variable.new(np.array(0.5))
  a = A.call(x)
  b = B.call(a)
  y = C.call(b)

  y.grad = np.array(1.0)
  y.backward()
  puts(x.grad)
end


begin
  x = Variable.new(np.array(0.5))
  y = square(exp(square(x)))

  y.grad = np.array(1.0)
  y.backward()
  puts(x.grad)
end


begin
  x = Variable.new(np.array(0.5))
  y = square(exp(square(x)))

  y.backward()
  puts(x.grad)
end
