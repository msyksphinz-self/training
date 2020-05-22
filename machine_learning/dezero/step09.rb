#!/usr/bin/env ruby

class Variable
  def initialize(data)
    if data != nil then
      if not data.is_a?(Array) then
        raise TypeError, data.class.to_s + " is not supported."
      end
    end
    @data = data
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

def square(x)
  return Square.new().call(x)
end


def exp(x)
  return Exp.new().call(x)
end

begin
  x = Variable.new([0.5])
  y = square(exp(square(x)))

  y.grad = [1.0]
  y.backward()
  puts(x.grad)
end

begin
  x = Variable.new([0.5])
  y = square(exp(square(x)))
  y.backward()
  puts(x.grad)
end

begin
  x = Variable.new(0.5)
  y = square(exp(square(x)))
  y.backward()
  puts(x.grad)
end
