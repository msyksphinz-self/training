#!/usr/bin/ruby

def func (x)
  return -Math.sin(x) + 0.3
end

prng = Random.new(100)

for i in (0..1000-1) do
  x = Random.rand(-2.0..2.0)
  printf("%f %f\n", x, func(x))
end
