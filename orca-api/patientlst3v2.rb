require 'uri'
require 'net/http'

Net::HTTP.version_1_2

HOST = "localhost"
PORT = "8000"
USER = "ormaster"
PASSWD = "ormaster"
CONTENT_TYPE = "application/xml"


req = Net::HTTP::Post.new("/api01rv2/patientlst3v2?class=01")
# class :01 指定患者情報取得
#
#
BODY = <<EOF
<data>
        <patientlst3req type="record">
                <WholeName type="string">テスト</WholeName>
                <Birth_StartDate type="string"> </Birth_StartDate>
                <Birth_EndDate type="string"> </Birth_EndDate>
                <Sex type="string"> </Sex>
                <InOut type="string"> </InOut>
        </patientlst3req>
</data>
EOF


req.content_length = BODY.size
req.content_type = CONTENT_TYPE
req.body = BODY
req.basic_auth(USER, PASSWD)
  puts req.body

Net::HTTP.start(HOST, PORT) {|http|
  res = http.request(req)
  puts res.body
}
