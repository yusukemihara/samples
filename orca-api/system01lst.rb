#!/usr/bin/ruby

require 'uri'
require 'net/http'

Net::HTTP.version_1_2
HOST = "localhost"
PORT = "8000"
USER = "ormaster"
PASSWD = "ormaster"
ID = ARGV[0]

req = Net::HTTP::Get.new("/api01r/system01lst?class=01")

req.basic_auth(USER, PASSWD)

Net::HTTP.start(HOST, PORT) { |http|
  res = http.request(req)
  puts "#{res.code} #{res.body.size}"
}
