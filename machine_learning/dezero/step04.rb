#!/usr/bin/env ruby

class Variable
  def initialize(data)
    @data = data
  end

  attr_accessor :data
end

class Function
  def call(input)
    x = input.data
    y = forward(x)
    output = Variable.new(y)
    return output
  end

  def forward(x)
    raise NotImplementedError
  end
  def Function(x)
    raise NotImplementedError
  end
end

class Square < Function
  def forward(x)
    return x.map{|i| i ** 2}
  end
end


class Exp < Function
  def forward(x)
    return x.map{|i| Math.exp(i)}
  end
end


def numerical_diff(f, x, eps=1e-4)
  x0 = Variable.new(x.data.map{|i| i - eps})
  x1 = Variable.new(x.data.map{|i| i + eps})
  y0 = f.call(x0)
  y1 = f.call(x1)
  return (y1.data.zip(y0.data).map{|i1, i0| i1 - i0}).map{|i| i / (2 * eps)}
end


f = Square.new()
x = Variable.new([2.0])
dy = numerical_diff(f, x)

puts(dy)

def f_func(x)
  a = Square.new()
  b = Exp.new()
  c = Square.new()
  return c.call(b.call(a.call(x)))
end
x = Variable.new([0.5])
dy = numerical_diff(method(:f_func), x)

puts dy
