#!/usr/bin/ruby


key1 = [1,1,1,1,1]
key2 = [2,2,2,2,2]

hash = {[key1[0],key1[1]] => key1 , key2[0..1] => key2}
p hash

key1[0] = 5
p hash
exit 0
p hash[[1,1]] 
p hash[[1,2]]
hash[[1,1]] = "value3"
p hash
