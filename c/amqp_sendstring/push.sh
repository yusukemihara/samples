#!/bin/bash
./amqp_sendstring localhost 5672 amq.topic tenant.1.accept '{"event":"print001","time":"2016-05-30T16:51:08+09:00","body":[{"Report_ID":"hoge","Custom_ID":"test","Data_ID":"hoge"}]}'
