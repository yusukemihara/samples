#/bin/sh

sleep 10

WFCPID=`ps ax | grep /sbin/wfc | grep -v grep | awk '{print $1}'`
DATE=`date -Iseconds | sed -e 's/+/-/g' -e 's/ /_/g' -e 's/:/./g'`
LOG=/tmp/$DATE.wfc.log.txt
CORE=/tmp/$DATE.wfc.core
logger $DATE
cat <<- EOS > /tmp/gdbwfc.txt
c
bt
thread apply all bt
quit
EOS
gdb -x /tmp/gdbwfc.txt -p $WFCPID  >$LOG 2>&1
