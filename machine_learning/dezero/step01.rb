#!/usr/bin/env ruby

class Variable
  def initialize(data)
    @data = data
  end

  attr_accessor :data
end

data = [1.0]
x = Variable.new(data)
puts(x.data)

x.data = [2.0]
puts(x.data)
