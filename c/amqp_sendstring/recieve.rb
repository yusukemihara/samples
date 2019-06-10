# coding:utf-8
require 'rubygems'
require 'faye/websocket'
require 'eventmachine'

require 'pp'
require 'securerandom'
require 'yaml'
require 'json'
require 'thread'

conf = {
:ws_server => "ws://localhost:9401/ws",
}

EM.run do
  subid = nil
  ws = Faye::WebSocket::Client.new(conf[:ws_server],nil,
         {
           :headers => {"X-GINBEE-TENANT-ID" => "1"},
           :ping    => 10
         })
  
  ws.on :open do |event|
    reqid = SecureRandom.uuid
    str = %|{"command":"subscribe","req.id":"#{reqid}","event":"*"}|
    ws.send str
  end

  ws.on :message do |event|
    puts event.data
    m = JSON.parse(event.data)
    command = m["command"]
    case command
    when "subscribed"
      subid = m["sub.id"]
    when "event"
    end
  end

  ws.on :error do |event|
    puts "error"
  end

  ws.on :close do |event|
    puts "close"
    ws = nil
  end
end
