#!/bin/sh

cat << EOF
#!/bin/sh

RESOLV_CONF="\$RESOLV_CONF"

[ -n "\$1" ] || { echo "Error: should be called from udhcpc"; exit 1; }

NETMASK=""
[ -n "\$subnet" ] && NETMASK="netmask \$subnet"
BROADCAST="broadcast +"
[ -n "\$broadcast" ] && BROADCAST="broadcast \$broadcast"
export > /var/fyi/sys/export

case "\$1" in
	renew|bound)
		echo "Setting IP address \$ip on \$interface"
		ifconfig \$interface \$ip \$NETMASK \$BROADCAST
		if [ -n "\$router" ] ; then
			metric=0
			for i in \$router ; do
				echo "Adding router \$i"
				route add default gw \$i dev \$interface metric \$((metric++))
			done
		fi

		echo "Recreating \$RESOLV_CONF"
		[ -n "\$domain" ] && echo "search \$domain" > \$RESOLV_CONF-\$\$
		for i in \$dns ; do
			echo " Adding DNS server \$i"
			echo "nameserver \$i" >> \$RESOLV_CONF-\$\$
		done
		cp -f \$RESOLV_CONF-\$\$ \$RESOLV_CONF
		rm -f \$RESOLV_CONF-\$\$

		if [ -x /bin/xmlprov ]; then
			killall xmlprov >/dev/null 2>&1
			killall xmlprov >/dev/null 2>&1
			killall xmlprov >/dev/null 2>&1
			sleep 1
			xmlprov -m 0 &
		fi
		[ -z "\$vlanid" ] || echo \$vlanid > /var/fyi/sys/vlanid
		NAME=wan WANPROTO=dhcp IPRENEWED=1 /sbin/hotplug process &
		;;
esac

exit 0
EOF
