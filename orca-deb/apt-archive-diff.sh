#!/bin/sh

set -e

TMP=/tmp

make_package_list()
{
	w3m -dump -cols 300 $1 | \
	grep 'Package\|Version' | \
	ruby -e '
	STDIN.readlines.each_with_index{|line, i| 
	  line.chomp!
	  print "#{line} "
	  print "\n\n" if i % 2 == 1 }' \
	> $TMP/$2
}

if [ $# != 2 ]
then
	echo "$0 http://.../Packages http://.../Packages"
	exit 1;
fi

pname1=`echo $1 | sed 's:/:_:g'`
pname2=`echo $2 | sed 's:/:_:g'`

make_package_list $1 $pname1
make_package_list $2 $pname2

diff -u $TMP/$pname1 $TMP/$pname2
