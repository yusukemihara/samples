#!/bin/sh 
set -e

LANG=C
COMMAND=glclient

ps ax | \
grep ${COMMAND} | \
grep -v grep | \
awk '{print $1}' | \
xargs ruby -e 'ARGV.each{|i| print `gdb -p #{i} -x gdb.txt` }'
