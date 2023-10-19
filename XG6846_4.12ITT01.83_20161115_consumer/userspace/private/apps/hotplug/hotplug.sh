#!/bin/sh
[ -n "$HOTPLUG_TYPE" ] || export HOTPLUG_TYPE="$1"
PATH=/bin:/sbin:/usr/bin:/usr/sbin
LOGNAME=root
USER=root
export PATH LOGNAME USER

[ \! -z "$HOTPLUG_TYPE" -a -d /etc/hotplug.d/$HOTPLUG_TYPE ] && {
	for script in $(ls /etc/hotplug.d/$HOTPLUG_TYPE/* 2>&-); do (
		[ -f $script ] && . $script
	); done
}
