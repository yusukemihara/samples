#/bin/bash -ex

echo
echo "----"
for i in `seq 100`
do
  ruby system01lst.rb & 
done

echo "---- end"
