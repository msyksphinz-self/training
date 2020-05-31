#!/usr/bin/env ruby

require './dezero/core_simple.rb'
require './dezero/util.rb'

require 'numpy'
np = Numpy

begin
  x = Variable.new(np.random.randn(2, 3))
  x.name = 'x'
  puts(_dot_var(x))
  puts(_dot_var(x, verbose=true))
end

begin
  x0 = Variable.new(np.array(1.0))
  x1 = Variable.new(np.array(1.0))
  y = x0 + x1
  txt = _dot_func(y.creator)
  puts(txt)
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

  x.name = 'x'
  y.name = 'y'
  z.name = 'z'
  plot_dot_graph(z, verbose=false, to_file='goldstein.png')
end
