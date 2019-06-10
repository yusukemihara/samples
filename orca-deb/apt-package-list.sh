#!/bin/sh

w3m -dump -cols 300 $1 | \
ruby -ne '$_.scan(%r|\APackage: ([\S]+)|){|a| print "#{$1} "}'
echo
