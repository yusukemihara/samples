#!/usr/bin/ruby 

File.open(ARGV[0],"r"){|io|
	io.readlines.each{ |line|
		line.chomp!
#		if %r!\A<([\dabcdef]+)> (\d+)! =~ line
#			puts "{0x#{$1},#{$2}},"
#		end
		if %r!\A<([\dabcdef]+)> <([\dabcdef]+)> (\d+)! =~ line
			puts "{0x#{$1},0x#{$2},#{$3}},"
		end
	}
}
