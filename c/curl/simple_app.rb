# coding: utf-8

require 'rack/request'
require 'rack/response'

class SimpleApp
  def call(env)
    req = Rack::Request.new(env)
    body = req.body.read
    STDERR.puts req.request_method
    STDERR.puts body

    res = Rack::Response.new { |r|
      r.status = 200
      r['Content-Type'] = 'application/json'
      r.write body
    }
    res.finish
  end
end

if __FILE__ == $0
  require 'rack/handler/webrick'
  Rack::Handler::WEBrick.run SimpleApp.new, :Port => 9292
end
