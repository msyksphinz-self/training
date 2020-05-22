#!/usr/bin/env ruby

class Variable
  def initialize(data)
    @data = data
    @grad = nil
    @creator = nil
  end
  def set_creator(func)
    @creator = func
  end

  def backward()
    funcs = [@creator]
    while funcs != [] do
      f = funcs.pop
      x = f.input
      y = f.output
      x.grad = f.backward(y.grad)
      if x.creator != nil then
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
    output = Variable.new(y)
    output.set_creator(self)
    @input = input
    @output = output
    return output
  end

  def forward(x)
    raise NotImplementedError
  end
  def backward(x)
    raise NotImplementedError
  end

  attr_accessor :input, :output
end

class Square < Function
  def forward(x)
    return x.map{|i| i ** 2}
  end
  def backward(gy)
    x = @input.data
    gx = x.zip(gy).map{|i0, i1| i0 * i1 * 2.0}
    return gx
  end
end


class Exp < Function
  def forward(x)
    return x.map{|i| Math.exp(i)}
  end
  def backward(gy)
    x = @input.data
    gx = x.zip(gy).map{|i0, i1| Math.exp(i0) * i1}
    return gx
  end
end


def numerical_diff(f, x, eps=1e-4)
  x0 = Variable.new(x.data.map{|i| i - eps})
  x1 = Variable.new(x.data.map{|i| i + eps})
  y0 = f.call(x0)
  y1 = f.call(x1)
  return (y1.data.zip(y0.data).map{|i1, i0| i1 - i0}).map{|i| i / (2 * eps)}
end


A = Square.new()
B = Exp.new()
C = Square.new()

x = Variable.new([0.5])
a = A.call(x)
b = B.call(a)
y = C.call(b)

# puts(y.data)
#
# y.grad = [1.0]
# C = y.creator
# b = C.input
# b.grad = C.backward(y.grad)
#
# B = b.creator
# a = B.input
# a.grad = B.backward(b.grad)
#
# A = a.creator
# x = A.input
# x.grad = A.backward(a.grad)
#
# puts(x.grad)

y.grad = [1.0]
y.backward()
puts(x.grad)
