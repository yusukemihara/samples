require 'pp'
require 'open-uri'

HOST = [
  'http://ftp01.orca.med.or.jp/pub/debian/dists/',
  'http://ftp02.orca.med.or.jp/pub/debian/dists/',
]
DIST = [
[
"trusty4.9-devel",
"trusty4.9-devel",
"trusty4.9-devel/jma/binary-amd64/Packages"
],
#[
#"trusty-common-devel",
#"trusty-common-devel",
#"http://ftp01.orca.med.or.jp/pub/debian/dists/trusty-common-devel/jma/binary-amd64/Packages"
#],
[
"trusty-common",
"trusty-common",
"trusty-common/jma/binary-amd64/Packages"
],
#[
#"trusty4.8-devel",
#"trusty4.8-devel",
#"http://ftp01.orca.med.or.jp/pub/debian/dists/trusty4.8-devel/jma/binary-amd64/Packages"
#],
[
"trusty4.8",
"trusty4.8",
"trusty4.8/jma/binary-amd64/Packages"
],
[
"precise4.8",
"precise4.8",
"precise4.8/jma/binary-amd64/Packages"
],
#[
#"precise4.8-devel",
#"precise4.8-devel",
#"http://ftp01.orca.med.or.jp/pub/debian/dists/precise4.8-devel/jma/binary-amd64/Packages"
#],
#[
#"precise-common-devel",
#"precise-common-devel",
#"http://ftp01.orca.med.or.jp/pub/debian/dists/precise-common-devel/jma/binary-amd64/Packages"
#],
[
"precise-common",
"precise-common",
"precise-common/jma/binary-amd64/Packages"
],
[
"precise4.7",
"precise4.7",
"precise4.7/jma/binary-amd64/Packages"
],
]

#### main

if ARGV.size < 2
  puts "#{$0} <server num> <dist>"
  puts "dists:"
  DIST.each do |d|
    puts "  #{d[1]}"
  end
  puts
  exit 1
end

n    = ARGV[0]
name = ARGV[1]

host = HOST[n.to_i]
dist = DIST.find do |d| d[0] == name end
unless dist
  puts "unknown dist"
  exit 1
end
url = host + dist[2]

info = []
open(url) do |f|
  f.each do |line|
    line.scan(%r|\APackage: ([\S]+)|){ info << [$1, "", "", ""]}
    line.scan(%r|\AVersion: ([\S]+)|){ info[-1][1] = $1}
    line.scan(%r|\ASize: ([\S]+)|){ info[-1][2] << $1}
    line.scan(%r|\AMD5sum: ([\S]+)|){ info[-1][3] << $1}
  end
  mtime = f.last_modified
end
info.sort! do |a,b|
  a[0] <=> b[0]
end
info.each do |pkg|
 puts "#{pkg[0]},  #{pkg[1]},  #{pkg[2]}" 
end
