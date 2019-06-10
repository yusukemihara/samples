#/bin/sh

DATE=`date -R | sed -e 's/,//g' -e 's/ /_/g' -e 's/:/./g'`
LOG=$DATE.wfc.log.txt
CORE=$DATE.wfc.core
echo $LOG
echo $CORE
exit 0
gdb -c $CORE -x /usr/local/lib/wfcdebug/cmd.txt  --args /usr/lib/panda/sbin/wfc $@ >$LOG 2>&1
