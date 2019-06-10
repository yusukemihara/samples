require_relative 'em-websocket-client'

EM.run do
  auth = ["user:password"].pack('m').delete("\r\n")
  conn = EventMachine::WebSocketClient.connect("ws://localhost:9202/ws",
    {"headers" => {"Authorization" => "Basic #{auth}"}})

  conn.callback do
    conn.send_msg "hello!"
    puts "hello"
  end

  conn.errback do |e|
    puts e
  end

  conn.stream do |msg|
    puts msg
  end

  conn.disconnect do
    puts "gone"
    EM::stop_event_loop
  end
end
