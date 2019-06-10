def strrand(chars,size)
  res = ""
  size.times do
    res << chars[rand(chars.length)]
  end
  res
end

chars = ('a'..'z').to_a + ('A'..'Z').to_a + ('0'..'9').to_a - ['l','1','I','0','O']
puts strrand(chars,16)
