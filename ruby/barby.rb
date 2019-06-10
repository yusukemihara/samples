require 'barby'
require 'barby/outputter/rmagick_outputter'

barcode = Barby::Code39.new(ARGV[1])
File.open(ARGV[0],"w"){|f|
  f.write barcode.to_png(:height=>64,:margin=>1)
}
