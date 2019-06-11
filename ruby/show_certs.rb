require 'openssl'

Dir.glob('*.crt').sort.each do |crt|
  x509 = OpenSSL::X509::Certificate.new(File.open(crt))
  puts %W(#{x509.serial.to_i},#{x509.subject},#{x509.not_before},#{x509.not_after})
end
