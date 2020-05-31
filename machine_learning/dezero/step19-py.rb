#!/usr/bin/env ruby

require 'pycall'
require 'numpy'
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

  attr_accessor :data, :grad, :creator, :generation
end

class Function
  def call(*inputs)
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
    y = x ** 2
    return y
  end

  def backward(gy)
    x = @inputs[0].data
    gx = 2 * x * gy
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
    y = x0 + x1
    return y
  end
  def backward(gy)
    return [gy, gy]
  end
end

def add(x0, x1)
  return Add.new().call(x0, x1)
end


x0 = Variable.new([1.0])
x1 = Variable.new([1.0])
t = add(x0, x1)
y = add(x0, t)
y.backward()

puts(y.grad, t.grad)
puts(x0.grad, x1.grad)

begin
  $enable_backprop = false
  x = Variable.new(100.times.map{100.times.map{100.times.map{1.0}}})
  y = square(square(square(x)))
  y.backward()
end

begin
  $enable_backprop = true
  x = Variable.new(100.times.map{100.times.map{100.times.map{1.0}}})
  y = square(square(square(x)))
  y.backward()
end
