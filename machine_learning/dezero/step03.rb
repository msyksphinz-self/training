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

A = Square.new()
B = Exp.new()
C = Square.new()

x = Variable.new([0.5])
a = A(x)
b = B(a)
y = C(b)
puts(y.data)
