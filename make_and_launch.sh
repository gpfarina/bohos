#!/bin/bash
origin=$(pwd)
build_dir="builds"
esci=0

while [ "$esci" = "0" ];do
	make clean && make && cd $build_dir
	umps
	cd $origin
	clear
	echo "do you want to continue? please give me more! [y/n]"
	read response
	if [ "$(echo $response|tr "A-Z" "a-z")" = "y" ];then
		esci=0
	else
		esci=1
	fi
done
exit 0
