#!/bin/bash

PROC_IGMP_OUTDEV="/proc/driver/mvsw/igmp_outdev"
route -n | grep 224.0.0.0 > /dev/null
if [ "$?" != "0" ]; then
	echo "Add multicast path"
	route add -net 224.0.0.0 netmask 255.0.0.0 dev br0
fi
ps -ef | grep igmptool |grep -v grep
echo "Add multicast path"
if [ "$?" = "0" ]; then
	killall igmptool > /dev/null 2>&1
fi
echo 1 > $PROC_IGMP_OUTDEV
igmptool -r 224.0.10.1 &
sleep 3
echo 0 > $PROC_IGMP_OUTDEV

