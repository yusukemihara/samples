#!/usr/bin/ruby

require 'yaml'
require 'pp'
require 'open-uri'

DIST_YAML = <<EOF
xenial5.1-ginbee: http://ftp01.orca.med.or.jp/pub/debian/dists/xenial5.1-ginbee/jma/binary-amd64/Packages
xenial5.0-mihara: http://wwwold.orca.med.or.jp/~mihara/debian/dists/xenial5.0/jma/binary-amd64/Packages
xenial-common-devel: http://ftp01.orca.med.or.jp/pub/debian/dists/xenial-common-devel/jma/binary-amd64/Packages
xenial-common: http://ftp01.orca.med.or.jp/pub/debian/dists/xenial-common/jma/binary-amd64/Packages
xenial5.0: http://ftp01.orca.med.or.jp/pub/debian/dists/xenial5.0/jma/binary-amd64/Packages
xenial5.0-devel: http://ftp01.orca.med.or.jp/pub/debian/dists/xenial5.0-devel/jma/binary-amd64/Packages
trusty5.0-devel: http://ftp01.orca.med.or.jp/pub/debian/dists/trusty5.0-devel/jma/binary-amd64/Packages
trusty5.0: http://ftp01.orca.med.or.jp/pub/debian/dists/trusty5.0/jma/binary-amd64/Packages
trusty5.1-ginbee: http://ftp01.orca.med.or.jp/pub/debian/dists/trusty5.1-ginbee/jma/binary-amd64/Packages
trusty4.8-ginbee: http://ftp01.orca.med.or.jp/pub/debian/dists/trusty4.8-ginbee/jma/binary-amd64/Packages
precise4.8-ginbee: http://ftp01.orca.med.or.jp/pub/debian/dists/precise4.8-ginbee/jma/binary-amd64/Packages
trusty4.9-devel: http://ftp01.orca.med.or.jp/pub/debian/dists/trusty4.9-devel/jma/binary-amd64/Packages
trusty-common-devel: http://ftp01.orca.med.or.jp/pub/debian/dists/trusty-common-devel/jma/binary-amd64/Packages
trusty-common: http://ftp01.orca.med.or.jp/pub/debian/dists/trusty-common/jma/binary-amd64/Packages
trusty4.8: http://ftp01.orca.med.or.jp/pub/debian/dists/trusty4.8/jma/binary-amd64/Packages
precise4.9-devel: http://ftp01.orca.med.or.jp/pub/debian/dists/precise4.9-devel/jma/binary-amd64/Packages
precise4.8: http://ftp01.orca.med.or.jp/pub/debian/dists/precise4.8/jma/binary-amd64/Packages
precise-common-devel: http://ftp01.orca.med.or.jp/pub/debian/dists/precise-common-devel/jma/binary-amd64/Packages
precise-common: http://ftp01.orca.med.or.jp/pub/debian/dists/precise-common/jma/binary-amd64/Packages
02_trusty4.9-devel: http://ftp02.orca.med.or.jp/pub/debian/dists/trusty4.9-devel/jma/binary-amd64/Packages
02_trusty-common-devel: http://ftp02.orca.med.or.jp/pub/debian/dists/trusty-common-devel/jma/binary-amd64/Packages
02_trusty-common: http://ftp02.orca.med.or.jp/pub/debian/dists/trusty-common/jma/binary-amd64/Packages
02_trusty4.8: http://ftp02.orca.med.or.jp/pub/debian/dists/trusty4.8/jma/binary-amd64/Packages
02_precise4.9-devel: http://ftp02.orca.med.or.jp/pub/debian/dists/precise4.9-devel/jma/binary-amd64/Packages
02_precise4.8: http://ftp02.orca.med.or.jp/pub/debian/dists/precise4.8/jma/binary-amd64/Packages
02_precise-common-devel: http://ftp02.orca.med.or.jp/pub/debian/dists/precise-common-devel/jma/binary-amd64/Packages
02_precise-common: http://ftp02.orca.med.or.jp/pub/debian/dists/precise-common/jma/binary-amd64/Packages
EOF

DIST = YAML.load(DIST_YAML)
info = []
mtime = nil

if ARGV.size < 1
  puts "#{$0} <dist>"
  puts "---- dists"
  pp DIST.keys
  exit 0
else
  dist = ARGV[0]
end


info = []
mtime = nil
open(DIST[dist]) do |f|
  f.each do |line|
    line.scan(%r|\APackage: ([\S]+)|){ info << [$1,""]}
    line.scan(%r|\AVersion: ([\S]+)|){ info[-1][1] = $1}
    #line.scan(%r|\ASize: ([\S]+)|){ info[-1][2] << $1}
    #line.scan(%r|\AMD5sum: ([\S]+)|){ info[-1][3] << $1}
  end
  mtime = f.last_modified
end

info.sort! do |a,b|
  a[0] <=> b[0]
end

puts mtime
#pp info
info.each do |k,v|
  puts "#{k}_#{v}"
end
