#!/usr/bin/env ruby

require 'test/unit'
require 'set'
require 'weakref'

$enable_backprop = true

def fill_one(a)
  if a.is_a?(Array)
    a.map{|i| fill_one(i) }
  else
    a = 1.0
  end
end


def as_array(x)
  puts x.class
  if x.is_a?(Array) then
    return x
  end
  return [x]
end

class Variable
  def initialize(data, name=nil)
    if data != nil then
      if not data.is_a?(Array) then
        @data = [data]
        # raise TypeError, data.class.to_s + " is not supported."
      else
        @data = data
      end
    end
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
    if @grad == nil then
      @grad = fill_one(@data.clone)
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
      gys = f.outputs.map{|x| x.grad}
      # puts "gys = " + gys.to_s
      # puts "f.outputs = " + f.outputs.to_s
      gxs = f.backward(*gys)
      if not gxs.is_a?(Array) then
        gxs = [gxs]
      end
      f.inputs.zip(gxs).each{|x, gx|
        if x.grad === nil then
          x.grad = gx
        else
          tmp = (x.grad + gx)
          x.grad = [tmp.is_a?(Array) ? tmp.sum : tmp]
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

  def len
    @data.size
  end

  def to_s
    if @data == nil then
      return 'variable(None)'
    end
    return 'variable(' + @data.to_s + ')'
  end

  def *(other)
    return mul(self, other)
  end

  def +(other)
    return add(self, other)
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
    inputs = inputs.map{|x| as_variable(x) }
    xs = inputs.map{|x| x.data}
    ys = forward(*xs)
    if ys.is_a?(Array) then
      ys = [ys]
    end
    outputs = ys.map{|y| Variable.new(y) }

    if $enable_backprop then
      @generation = (inputs.map{|x| x.generation}).max
      outputs.each {|output| output.set_creator(self) }
    end

    @inputs = inputs
    @outputs = outputs.map{|output| WeakRef.new(output)}
    return outputs.size > 1 ? outputs : outputs[0]
  end

  def forward(x)
    raise NotImplementedError
  end
  def backward(x)
    raise NotImplementedError
  end

  attr_accessor :inputs, :outputs, :generation
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
    tmp = x.map{|i| _calc(i)}
    return tmp
  end

  def _backward_calc(x)
    if not x.is_a?(Array) then
      return x
    elsif x.is_a?(Array) and
      x.length == 2 and
      not x[0].is_a?(Array) then
      return x[0] * x[1] * 2.0
    else
      return x.map{|i| _backward_calc(i) }
    end
  end

  def backward(gy)
    x = @inputs[0].data
    gx = x.zip(gy).map{|i, j|
      # puts "i = " + i.to_s
      # puts "j = " + j.to_s
      _backward_calc(i) }
    return [gx]
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
  def forward(x0, x1)
    y = x0[0] + x1[0]
    return [y]
  end
  def backward(gy)
    return [gy, gy]
  end
end

def add(x0, x1)
  # x1 = as_array(x1)
  return Add.new().call(x0, x1)
end

class Mul < Function
  def forward(x0, x1)
    y = x0[0] * x1[0]
    return [y]
  end
  def backward(gy)
    x0 = @inputs[0].data
    y0 = @inputs[1].data
    return [gy * x1, gy * x0]
  end
end

def mul(x0, x1)
  return Mul.new().call(x0, x1)
end


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
