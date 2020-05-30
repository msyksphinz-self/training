#!/usr/bin/env ruby

require "./core_simple.rb"

# def sphere(x, y)
#   z = x ** 2.0 + y ** 2.0
#   return z
# end
#
#
# x = Variable.new([1.0])
# y = Variable.new([1.0])
# z = sphere(x, y)
# z.backward()
# puts x.grad, y.grad


def matyas(x, y)
  z = x ** 2.0 - x
  return z
end


x = Variable.new([1.0])
y = Variable.new([1.0])
z = matyas(x, y)
puts "z = " + z.to_s
z.backward()
puts x.grad.to_s + ", " + y.grad.to_s
