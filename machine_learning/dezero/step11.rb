#!/usr/bin/env ruby

require 'test/unit'

class Variable
  def initialize(data)
    if data != nil then
      if not data.is_a?(Array) then
        @data = [data]
        # raise TypeError, data.class.to_s + " is not supported."
      else
        @data = data
      end
    end
    @grad = nil
    @creator = nil
  end
  def set_creator(func)
    @creator = func
  end

  def backward()
    if @grad == nil then
      @grad = @data.clone.fill(1.0)
    end
    funcs = [@creator]
    while funcs != [] do
      f = funcs.pop
      x = f.inputs
      y = f.outputs
      x.grad = f.backward(y.grad)
      if x.creator != nil then
        funcs.push(x.creator)
      end
    end
  end

  attr_accessor :data, :grad, :creator
end

class Function
  def call(inputs)
    xs = inputs.map{|x| x.data}
    ys = forward(xs)
    outputs = ys.map{|y| Variable.new(y) }

    outputs.each {|output|
      output.set_creator(self)
    }
    @inputs = inputs
    @outputs = outputs
    return outputs
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
  def _calc(x)
    if x.is_a?(Array) then
      return x.map{|i| _calc(i) }
    else
      return x ** 2
    end
  end
  def forward(x)
    return x.map{|i| _calc(i)}
  end
  def backward(gy)
    x = @inputs.data
    gx = x.zip(gy).map{|i0, i1| i0 * i1 * 2.0}
    return gx
  end
end


class Exp < Function
  def _calc(x)
    if x.is_a?(Array) then
      return x.map{|i| _calc(i) }
    else
      return Math.exp(x)
    end
  end
  def forward(x)
    return x.map{|i| _calc(i)}
  end
  def backward(gy)
    x = @inputs.data
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

def square(x)
  return Square.new().call(x)
end


def exp(x)
  return Exp.new().call(x)
end

class Add < Function
  def forward(xs)
    y = xs[0][0] + xs[1][0]
    return [y]
  end
end

xs = [Variable.new([2.0]), Variable.new([3.0])]
f = Add.new()
ys = f.call(xs)
y = ys[0]
puts y.data
