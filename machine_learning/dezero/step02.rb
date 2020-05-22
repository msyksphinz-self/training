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
    y = x.map{|i| i ** 2}
    output = Variable.new(y)
    return output
  end

  def forward(x)
    raise NotImplementedError
  end
end

class Square < Function
  def forward(x)
    return x.map{|i| i ** 2}
  end
end


x = Variable.new([10])
f = Function.new()
y = f.call(x)
puts(y.class)
puts(y.data)

f2 = Square.new()
y2 = f2.call(x)
puts(y2.class)
puts(y2.data)
