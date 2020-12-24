#!/bin/bash

# API URL
URL="http://localhost:8000/api01rv2/patientlst2v2?class=01"

# API USER:PASSWORD
USERPASS="ormaster:ormaster"

# 最大同時リクエスト数
MAX=100

# 1回の同時リクエスト数
CONC=5

# 実行時間(秒)
LIMIT_SEC=600

# 内部変数 経過時間
ELAPSED=0

# 内部変数 リクエスト数
N_REQ=0

# リクエストXML
cat <<EOF >req.xml
<data>
        <patientlst2req type="record">
                <Patient_ID_Information type="array">
                        <Patient_ID_Information_child type="record">
                                <Patient_ID type="string">1</Patient_ID>
                        </Patient_ID_Information_child>
                </Patient_ID_Information>
        </patientlst2req>
</data>
EOF

epoch() {
  date +%s
}
ST=$(epoch)

_request() {
  curl --basic -u $USERPASS -X POST -d @req.xml $URL >/dev/null 2>&1 &
  if [ $? != 0 ]
  then
    echo "---- error"
    exit 1
  fi
}

request() {
  for i in `seq $CONC`
  do
    _request
  done
}

count_request() {
  N_REQ=`ps aux | grep curl | wc -l`
}

calc_elapsed() {
  local NOW=$(epoch)
  ELAPSED=`expr $NOW - $ST`
}

# main
echo "----start"
while [ $ELAPSED -lt $LIMIT_SEC ]
do
  count_request
  if [ $N_REQ -lt $MAX ];then request; fi
  echo "elapsed:$ELAPSED n_req:$N_REQ"
  calc_elapsed
done
echo "----end"
