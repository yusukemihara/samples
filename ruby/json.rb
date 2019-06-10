require 'pp'
require 'json'

jsonstr = <<-EOS
{"a":"1","b":2,"c":3,"d":{"aa":11,"bb":12,"cc":13}}
EOS

puts jsonstr
hash = JSON.parse(jsonstr)
pp hash
puts hash.to_json
