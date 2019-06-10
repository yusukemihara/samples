#!/usr/bin/ruby

s=[
["E3809C","EFBD9E"],
["E28892","EFBC8D"],
["E28094","EFBC8D"],
["E28096","E288A5"],
["C2A2","EFBFA0"],
["C2A3","EFBFA1"],
]
s.map!{|ss| ss.map!{|sss| [sss].pack("H*")}}

s.each_with_index{|a,i|
  r = `echo #{a[1]} |  iconv -t euc-jisx0213 -f utf8`
  puts a[1] + ":" + r.chop.unpack("H*")[0]
}
