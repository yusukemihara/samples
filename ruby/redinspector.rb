require "zlib"
require "iconv"

def read_red(path)
  Zlib::GzipReader.open(path){|gz|
    return gz.read
  }
rescue Zlib::GzipFile::Error
  return File.read(path)
end

def check_red(file)
  redin = read_red(file)
  redin.gsub!(/&#([0-9]+);/){[$1.to_i.to_s(16)].pack("H*")}
  Iconv.open("utf8","euc-jp") {|i|
    i.iconv(redin)
  }
rescue Iconv::Failure => ex
  a =  ex.success.split("\n")
  b = a[-1].size
  puts "---- ERROR #{file} line:#{a.size} pos:#{b}"
end

ARGV.each{|file|
  check_red(file)
}
