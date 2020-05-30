#!/usr/bin/env ruby

require 'set'
require 'weakref'

require 'pycall'
sys = PyCall.import_module('sys')
p sys.path
require 'numpy'
np = Numpy

$enable_backprop = true


def as_array(x)
  np = Numpy

  if np.isscalar(x)
    return np.array(x)
  end
  return x
end


class Variable
  def initialize(data, name = nil)
    @data = data
    @name = name
    @grad = nil
    @creator = nil
    @generation = 0
  end

  def set_creator(func)
    @creator = func
    @generation = func.generation + 1
  end

  def backward(retain_grad=false)
    np = Numpy

    if @grad == nil
      @grad = np.ones_like(@data)
    end

    funcs = Array.new()
    seen_set = Set.new

    def add_func(f, funcs, seen_set)
      if f != nil and (not seen_set.include?(f)) then
        funcs.push(f)
        seen_set.add(f)
        funcs.sort!{|a| a.generation}
      end
    end

    add_func(@creator, funcs, seen_set)

    while not funcs.empty? do
      f = funcs.pop
      gys = f.outputs.map{|output| output.grad}
      gxs = f.backward(*gys)
      if not gxs.is_a?(Array)
        gxs = [gxs]
      end

      f.inputs.zip(gxs).each{|x, gx|
        if x.grad == nil
          x.grad = gx
        else
          x.grad = x.grad + gx
        end
        if x.creator != nil then
          add_func(x.creator, funcs, seen_set)
        end
      }
      if not retain_grad then
        f.outputs.map{|y| y.grad = nil }
      end
    end
  end

  def cleargrad()
    @grad = nil
  end

  def shape()
    return @data.shape
  end

  def ndim()
    return @data.ndim
  end

  def size()
    return @data.size
  end

  def dtype()
    return @data.dtype
  end

  def to_s
    return 'variable(' + @data.to_s + ')'
  end

  def *(other)
    return mul(self, other)
  end

  def /(other)
    return div(self, other)
  end

  def +(other)
    return add(self, other)
  end

  def -(other)
    return sub(self, other)
  end

  def -@
    return neg(self)
  end

  def **(other)
    return pow(self, other)
  end

  attr_accessor :data, :grad, :creator, :generation
end

class Function
  def as_variable(obj)
    if obj.is_a?(Variable) then
      return obj
    end
    return Variable.new(obj)
  end

  def call(*inputs)
    inputs = inputs.map{|x| as_variable(x)}

    xs = inputs.map{|x| x.data}
    ys = forward(*xs)
    if not ys.is_a?(Array) then
      ys = [ys]
    end
    outputs = ys.map{|y| Variable.new(y) }

    if $enable_backprop then
      @generation = (inputs.map{|x| x.generation}).max
      outputs.each{|output|
        output.set_creator(self)
      }
    end

    @inputs = inputs
    @outputs = outputs.map{|output| WeakRef.new(output)}
    return outputs.size > 1 ? outputs : outputs[0]
  end
  def forward(xs)
    raise NotImplementedError
  end
  def backward(gys)
    raise NotImplementedError
  end

  attr_accessor :inputs, :outputs, :generation
end


class Square < Function
  def forward(x)
    y = x ** 2
    return y
  end
  def backward(gy)
    x = @inputs[0].data
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

class Add < Function
  def forward(x0, x1)
    y = x0 + x1
    return y
  end
  def backward(gy)
    return [gy, gy]
  end
end


class Sub < Function
  def forward(x0, x1)
    y = x0 - x1
    return y
  end
  def backward(gy)
    return [gy, gy * -1.0]
  end
end

def sub(x0, x1)
  return Sub.new().call(x0, x1)
end

class Mul < Function
  def forward(x0, x1)
    y = x0 * x1
    return y
  end
  def backward(gy)
    x0 = @inputs[0].data
    x1 = @inputs[1].data
    return [gy * x1, gy * x0]
  end
end


class Div < Function
  def forward(x0, x1)
    y = x0 / x1
    return y
  end
  def backward(gy)
    x0 = @inputs[0].data
    y0 = @inputs[1].data
    return [gy / x1, gy * (-x0 / x1 ** 2.0)]
  end
end

def div(x0, x1)
  return Div.new().call(x0, x1)
end


class Neg < Function
  def forward(x)
    tmp = -x
    return tmp
  end
  def backward(gy)
    return -gy
  end
end


class Pow < Function
  def initialize(c)
    @c = c
  end
  def forward(x)
    y = x ** @c
    return y
  end
  def backward(gy)
    x = @inputs[0].data
    c = @c
    gx = c * x ** (c - 1) * gy
    return gx
  end
end

def pow(x, c)
  return Pow.new(c).call(x)
end

def neg(x)
  return Neg.new().call(x)
end

def square(x)
  return Square.new().call(x)
end

def exp(x)
  return Exp.new().call(x)
end

def add(x0, x1)
  x1 = as_array(x1)
  return Add.new().call(x0, x1)
end

def mul(x0, x1)
  x1 = as_array(x1)
  return Mul.new().call(x0, x1)
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
#   y.grad = np.array(1.0)
#   y.backward()
#   puts(x.grad)
# end
#
#
# begin
#   x = Variable.new(np.array(0.5))
#   y = square(exp(square(x)))
#
#   y.grad = np.array(1.0)
#   y.backward()
#   puts(x.grad)
# end
#
#
# begin
#   x = Variable.new(np.array(0.5))
#   y = square(exp(square(x)))
#
#   y.backward()
#   puts(x.grad)
# end

# begin
#   x0 = Variable.new(np.array(2))
#   x1 = Variable.new(np.array(3))
#   f = Add.new()
#   y = f.call(x0, x1)
#   print(y[0].data)
# end


begin
  x0 = Variable.new(np.array(2))
  x1 = Variable.new(np.array(3))
  y = add(x0, x1)
  puts(y.data)
end


begin
  x = Variable.new(np.array(2.0))
  y = Variable.new(np.array(3.0))
  z = add(square(x), square(y))
  z.backward()

  puts(z.data)
  puts(x.grad)
  puts(y.grad)
end


begin
  x = Variable.new(np.array(3.0))
  z = add(add(x, x),x)
  z.backward()

  puts(x.grad)
end


begin
  x = Variable.new(np.array(3.0))
  y = add(x, x)
  y.backward()
  puts(x.grad)

  x = Variable.new(np.array(3.0))
  y = add(add(x, x),x)
  y.backward()
  puts(x.grad)
end

begin
  x = Variable.new(np.array(2.0))
  a = square(x)
  y = add(square(a), square(a))
  y.backward()

  puts(y.data)
  puts(x.grad)
end

begin
  x = Variable.new(np.array([[1, 2, 3], [4, 5, 6]]))
  puts(x.shape.to_s)
end


begin
  x = Variable.new(np.array([1, 2, 3]))
  puts(x.to_s)

  x = Variable.new(nil)
  puts(x.to_s)

  x = Variable.new(np.array([[1, 2, 3], [4, 5, 6]]))
  puts(x.to_s)
end


begin
  a = Variable.new(np.array(3.0))
  b = Variable.new(np.array(2.0))
  c = Variable.new(np.array(1.0))
  y = add(mul(a, b), c)
  y.backward()

  puts(y.to_s)
  puts(a.grad)
  puts(b.grad)
end


begin
  a = Variable.new(np.array(3.0))
  b = Variable.new(np.array(2.0))
  c = Variable.new(np.array(1.0))
  y = a * b + c
  y.backward()

  puts(y.to_s)
  puts(a.grad)
  puts(b.grad)
end


begin
  x = Variable.new(np.array(2.0))
  y = x + np.array(3.0)
  puts(y)
end

begin
  x = Variable.new(np.array(2.0))
  y = x ** 3
  y.backward()
  puts "=== Pow Test === "
  puts(y)
  puts(x.grad)
  puts "=== End Pow Test ==="
end

begin
  def sphere(x, y)
    z = x ** 2.0 + y ** 2.0
    return z
  end

  x = Variable.new(np.array(1.0))
  y = Variable.new(np.array(1.0))
  z = sphere(x, y)
  z.backward()
  puts [x.grad, y.grad].to_s
end


begin
  def matyas(x, y)
    z = ((x ** 2.0 + y ** 2.0) * 0.26) - (x * y * 0.48)
    return z
  end
  x = Variable.new(np.array(1.0))
  y = Variable.new(np.array(1.0))
  z = matyas(x, y)
  z.backward()
  puts z
  puts [x.grad, y.grad].to_s
end


begin
  def goldstein(x, y)
    z = (((x + y + 1)**2) * (x*(-14) + (x**2)*3 - y*14.0 + x*y*6.0 + (y**2)*3 + 19) + 1) *
        (((x*2 - y*3)**2) * (x*(-32.0) + (x**2)*12 + y*48.0 - x*y*36.0 + (y**2)*27 + 18) + 30)
    return z
  end
  x = Variable.new(np.array(1.0))
  y = Variable.new(np.array(1.0))
  z = goldstein(x, y)
  z.backward()
  puts z
  puts [x.grad, y.grad].to_s
end
