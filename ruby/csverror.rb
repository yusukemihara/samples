#!/usr/bin/ruby

require 'csv'

CSV.open(ARGV[0], "r"){ |row|
  p row
}
