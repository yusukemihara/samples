#!/usr/bin/ruby

a = [0,1,2]
b = Array.new(a)
c = a
a[0] = 5
p a
p b
p c
