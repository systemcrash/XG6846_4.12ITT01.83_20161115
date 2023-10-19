#!/bin/sh

PROG="watchdog"
conf="/var/run/watchdog.conf"
pidfn="/var/run/watchdog.sh.pid"
tr69_isaction="/var/run/watchdog_restarting_tr69c.run"
tr69_isrestaringtimer="/var/run/watchdog_need_restart_tr69c.run"
tr69_isforcerestarting="/var/run/tr69_isforcerestarting.run"
xg_noipisrunning="/var/run/watchdog.noip.running"
tr69_zombieisrunning="/var/run/watchdog.zombie.running"

mysleep() {
	target=$(($(date +"%s")+$1))
	x=0
	while [ 1 ]; do
		[ ! -s $pidfn ] || . $pidfn
		[ "$MYPID" = "$FNPID" ] || exit
		now=$(date +"%s")
		[ $now -lt $target ] || break
		sleep 2
	done
}

getip() {
	ipaddr=$(ifconfig tap1 | sed -e '/inet addr:/!d' -e 's/.*inet addr://' \
		-e 's/ .*//')
}

action_lock() {
	[ ! -f "$tr69_isaction" ] || return
	echo > $tr69_isaction
}

action_unlock() {
	rm -f $tr69_isaction
}

action_tr69restarttimer() {
	echo "[$PROG] Restart Timer [$TR69_RESTARTTIMER]"
	mysleep $TR69_RESTARTTIMER
	/etc/init.d/trclient restart
	rm -f $tr69_isrestaringtimer
}

action_xgnoip() {
	mysleep $XG_NOIPRESTOREDEFAULT
	getip
	[ -z "$ipaddr" ] || {
		echo "$PROG Okay, we're gotta ip again"
		rm -f $xg_noipisrunning
		return
	}
	echo "$PROG No IP and restore default now"
	sw_upgrade -q # restore default
	reboot
}

action_xgforcereboot() {
	[ ! -s $pidfn ] || . $pidfn
	[ "$MYPID" = "$FNPID" ] || exit
	echo "$PROG Force to reboot [$TR69C_BOOTSTAMP]"
	reboot
}

action_tr69forcerestart() {
	[ ! -s $pidfn ] || . $pidfn
	[ "$MYPID" = "$FNPID" ] || exit
	[ ! -f "$tr69_isaction" ] || return
	[ ! -f "$tr69_isrestaringtimer" ] || return

	action_lock
	echo "[$PROG] Force to restart tr69c [$TR69C_BOOTSTAMP]"
	/etc/init.d/trclient restart
	action_unlock
}

condition_tr69czombie() {
	[ ! -f "$tr69_zombieisrunning" ] || return
	S=$(ps |grep tr69c|grep -v grep |awk '{print $4}')
	[ "$S" = "Z" ] || return
	echo "Zombie TR69c, I would reboot now"
	reboot
}

condition_tr69restarttimer() {
	[ "$TR69_RESTARTTIMER" != "0" ] || return
	[ ! -f "$tr69_isaction" ] || return
	[ ! -f "$tr69_isrestaringtimer" ] || return
	[ -z "$(pidof tr69c)" ] || return
	echo > $tr69_isrestaringtimer
	action_tr69restarttimer &
}

condition_xgnoip() {
	[ $XG_NOIPRESTOREDEFAULT != "0" ] || return
	[ ! -f "$xg_noipisrunning" ] || return
	getip
	[ -z "$ipaddr" ] || {
		rm -f $xg_noipisrunning
		return
	}
	echo "$PROG Uh, I'd check again, if no ipaadr after $XG_NOIPRESTOREDEFAULT secs"
	echo > $xg_noipisrunning
	action_xgnoip &
}

condition_xgntp() {
	now=$(date +"%s")
	[ $TR69_FORCERESTART -ne 0 -o $XG_FORCEREBOOT -ne 0 ] || return
	nowhour=$(date +"%H")
	diff=$(($now-$TR69C_BOOTSTAMP))
	boottime=$(sed 's/\..*//' /proc/uptime)
	# No range limit to action
	if [ $XG_FORCEREBOOT -ne 0 ]; then
		[ $boottime -le $XG_FORCEREBOOT ] || {
			action_xgforcereboot
			return
		}
	fi
	if [ $TR69_FORCERESTART -ne 0 ]; then
		[ $diff -le $TR69_FORCERESTART ] || {
			action_tr69forcerestart
			return
		}
	fi

	# Check if zombie only when tr69c running over 1 days
	[ $diff -lt 86400 ] || condition_tr69czombie

	# if NTP not work(after AD 2000), we don't care START or STOP
	[ $now -gt 946684800 ] || return
	# Range limit to action
	if [ $XG_FORCEREBOOT -ne 0 -a $nowhour -ge $XG_FORCEREBOOTSTART -a \
		$nowhour -le $XG_FORCEREBOOTSTOP ]; then
		[ $boottime -le $XG_FORCEREBOOT ] || {
			action_xgforcereboot
			return
		}
	fi
	if [ $TR69_FORCERESTART -ne 0 -a $nowhour -ge $TR69_FORCERESTARTSTART -a \
		$nowhour -le $TR69_FORCERESTARTSTOP ]; then
		[ $diff -le $TR69_FORCERESTART ] || {
			action_tr69forcerestart
			return
		}
	fi
}

pidfn="/var/run/watchdog.sh.pid"
VARPID=$(cat $pidfn 2>/dev/null)
if [ -n "$VARPID" ]; then
	REALPID=$(ps -ef | awk '{if ($2 == "'$VARPID'") print $2 }')
	if [ -n "$REALPID" ]; then
		echo "$PROG Another watchdog.sh is running"
		exit
	fi
fi
MYPID=$$
echo "FNPID=$MYPID" > $pidfn

while [ ! -s $conf ]; do
	mysleep 3
done
while [ 1 ]; do
	. $conf
	[ ! -s $pidfn ] || . $pidfn
	[ "$MYPID" = "$FNPID" ] || exit
	condition_tr69restarttimer
	condition_xgnoip
	condition_xgntp
	sleep 2
done
