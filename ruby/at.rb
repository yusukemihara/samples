loop do

loop do
  h = Time.now.strftime("%H").to_i
  puts h
  if 10 <= h && h <= 12
    puts "hoge"
    break
  else
    puts "sleep"
    sleep 60
  end
end

end
