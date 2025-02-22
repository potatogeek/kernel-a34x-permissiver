#!/bin/bash
#
# This test is for basic NAT functionality: snat, dnat, redirect, masquerade.
#

# Kselftest framework requirement - SKIP code is 4.
ksft_skip=4
ret=0
<<<<<<< HEAD
=======
test_inet_nat=true

sfx=$(mktemp -u "XXXXXXXX")
ns0="ns0-$sfx"
ns1="ns1-$sfx"
ns2="ns2-$sfx"

cleanup()
{
	for i in 0 1 2; do ip netns del ns$i-"$sfx";done
}
>>>>>>> upstream/android-13

nft --version > /dev/null 2>&1
if [ $? -ne 0 ];then
	echo "SKIP: Could not run test without nft tool"
	exit $ksft_skip
fi

ip -Version > /dev/null 2>&1
if [ $? -ne 0 ];then
	echo "SKIP: Could not run test without ip tool"
	exit $ksft_skip
fi

<<<<<<< HEAD
ip netns add ns0
ip netns add ns1
ip netns add ns2

ip link add veth0 netns ns0 type veth peer name eth0 netns ns1 > /dev/null 2>&1
=======
ip netns add "$ns0"
if [ $? -ne 0 ];then
	echo "SKIP: Could not create net namespace $ns0"
	exit $ksft_skip
fi

trap cleanup EXIT

ip netns add "$ns1"
if [ $? -ne 0 ];then
	echo "SKIP: Could not create net namespace $ns1"
	exit $ksft_skip
fi

ip netns add "$ns2"
if [ $? -ne 0 ];then
	echo "SKIP: Could not create net namespace $ns2"
	exit $ksft_skip
fi

ip link add veth0 netns "$ns0" type veth peer name eth0 netns "$ns1" > /dev/null 2>&1
>>>>>>> upstream/android-13
if [ $? -ne 0 ];then
    echo "SKIP: No virtual ethernet pair device support in kernel"
    exit $ksft_skip
fi
<<<<<<< HEAD
ip link add veth1 netns ns0 type veth peer name eth0 netns ns2

ip -net ns0 link set lo up
ip -net ns0 link set veth0 up
ip -net ns0 addr add 10.0.1.1/24 dev veth0
ip -net ns0 addr add dead:1::1/64 dev veth0

ip -net ns0 link set veth1 up
ip -net ns0 addr add 10.0.2.1/24 dev veth1
ip -net ns0 addr add dead:2::1/64 dev veth1

for i in 1 2; do
  ip -net ns$i link set lo up
  ip -net ns$i link set eth0 up
  ip -net ns$i addr add 10.0.$i.99/24 dev eth0
  ip -net ns$i route add default via 10.0.$i.1
  ip -net ns$i addr add dead:$i::99/64 dev eth0
  ip -net ns$i route add default via dead:$i::1
=======
ip link add veth1 netns "$ns0" type veth peer name eth0 netns "$ns2"

ip -net "$ns0" link set lo up
ip -net "$ns0" link set veth0 up
ip -net "$ns0" addr add 10.0.1.1/24 dev veth0
ip -net "$ns0" addr add dead:1::1/64 dev veth0

ip -net "$ns0" link set veth1 up
ip -net "$ns0" addr add 10.0.2.1/24 dev veth1
ip -net "$ns0" addr add dead:2::1/64 dev veth1

for i in 1 2; do
  ip -net ns$i-$sfx link set lo up
  ip -net ns$i-$sfx link set eth0 up
  ip -net ns$i-$sfx addr add 10.0.$i.99/24 dev eth0
  ip -net ns$i-$sfx route add default via 10.0.$i.1
  ip -net ns$i-$sfx addr add dead:$i::99/64 dev eth0
  ip -net ns$i-$sfx route add default via dead:$i::1
>>>>>>> upstream/android-13
done

bad_counter()
{
	local ns=$1
	local counter=$2
	local expect=$3
<<<<<<< HEAD

	echo "ERROR: $counter counter in $ns has unexpected value (expected $expect)" 1>&2
=======
	local tag=$4

	echo "ERROR: $counter counter in $ns has unexpected value (expected $expect) at $tag" 1>&2
>>>>>>> upstream/android-13
	ip netns exec $ns nft list counter inet filter $counter 1>&2
}

check_counters()
{
	ns=$1
	local lret=0

	cnt=$(ip netns exec $ns nft list counter inet filter ns0in | grep -q "packets 1 bytes 84")
	if [ $? -ne 0 ]; then
<<<<<<< HEAD
		bad_counter $ns ns0in "packets 1 bytes 84"
=======
		bad_counter $ns ns0in "packets 1 bytes 84" "check_counters 1"
>>>>>>> upstream/android-13
		lret=1
	fi
	cnt=$(ip netns exec $ns nft list counter inet filter ns0out | grep -q "packets 1 bytes 84")
	if [ $? -ne 0 ]; then
<<<<<<< HEAD
		bad_counter $ns ns0out "packets 1 bytes 84"
=======
		bad_counter $ns ns0out "packets 1 bytes 84" "check_counters 2"
>>>>>>> upstream/android-13
		lret=1
	fi

	expect="packets 1 bytes 104"
	cnt=$(ip netns exec $ns nft list counter inet filter ns0in6 | grep -q "$expect")
	if [ $? -ne 0 ]; then
<<<<<<< HEAD
		bad_counter $ns ns0in6 "$expect"
=======
		bad_counter $ns ns0in6 "$expect" "check_counters 3"
>>>>>>> upstream/android-13
		lret=1
	fi
	cnt=$(ip netns exec $ns nft list counter inet filter ns0out6 | grep -q "$expect")
	if [ $? -ne 0 ]; then
<<<<<<< HEAD
		bad_counter $ns ns0out6 "$expect"
=======
		bad_counter $ns ns0out6 "$expect" "check_counters 4"
>>>>>>> upstream/android-13
		lret=1
	fi

	return $lret
}

check_ns0_counters()
{
	local ns=$1
	local lret=0

<<<<<<< HEAD
	cnt=$(ip netns exec ns0 nft list counter inet filter ns0in | grep -q "packets 0 bytes 0")
	if [ $? -ne 0 ]; then
		bad_counter ns0 ns0in "packets 0 bytes 0"
		lret=1
	fi

	cnt=$(ip netns exec ns0 nft list counter inet filter ns0in6 | grep -q "packets 0 bytes 0")
	if [ $? -ne 0 ]; then
		bad_counter ns0 ns0in6 "packets 0 bytes 0"
		lret=1
	fi

	cnt=$(ip netns exec ns0 nft list counter inet filter ns0out | grep -q "packets 0 bytes 0")
	if [ $? -ne 0 ]; then
		bad_counter ns0 ns0out "packets 0 bytes 0"
		lret=1
	fi
	cnt=$(ip netns exec ns0 nft list counter inet filter ns0out6 | grep -q "packets 0 bytes 0")
	if [ $? -ne 0 ]; then
		bad_counter ns0 ns0out6 "packets 0 bytes 0"
=======
	cnt=$(ip netns exec "$ns0" nft list counter inet filter ns0in | grep -q "packets 0 bytes 0")
	if [ $? -ne 0 ]; then
		bad_counter "$ns0" ns0in "packets 0 bytes 0" "check_ns0_counters 1"
		lret=1
	fi

	cnt=$(ip netns exec "$ns0" nft list counter inet filter ns0in6 | grep -q "packets 0 bytes 0")
	if [ $? -ne 0 ]; then
		bad_counter "$ns0" ns0in6 "packets 0 bytes 0"
		lret=1
	fi

	cnt=$(ip netns exec "$ns0" nft list counter inet filter ns0out | grep -q "packets 0 bytes 0")
	if [ $? -ne 0 ]; then
		bad_counter "$ns0" ns0out "packets 0 bytes 0" "check_ns0_counters 2"
		lret=1
	fi
	cnt=$(ip netns exec "$ns0" nft list counter inet filter ns0out6 | grep -q "packets 0 bytes 0")
	if [ $? -ne 0 ]; then
		bad_counter "$ns0" ns0out6 "packets 0 bytes 0" "check_ns0_counters3 "
>>>>>>> upstream/android-13
		lret=1
	fi

	for dir in "in" "out" ; do
		expect="packets 1 bytes 84"
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ${ns}${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns0 $ns$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ${ns}${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" $ns$dir "$expect" "check_ns0_counters 4"
>>>>>>> upstream/android-13
			lret=1
		fi

		expect="packets 1 bytes 104"
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ${ns}${dir}6 | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns0 $ns$dir6 "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ${ns}${dir}6 | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" $ns$dir6 "$expect" "check_ns0_counters 5"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	return $lret
}

reset_counters()
{
	for i in 0 1 2;do
<<<<<<< HEAD
		ip netns exec ns$i nft reset counters inet > /dev/null
=======
		ip netns exec ns$i-$sfx nft reset counters inet > /dev/null
>>>>>>> upstream/android-13
	done
}

test_local_dnat6()
{
<<<<<<< HEAD
	local lret=0
ip netns exec ns0 nft -f - <<EOF
table ip6 nat {
	chain output {
		type nat hook output priority 0; policy accept;
		ip6 daddr dead:1::99 dnat to dead:2::99
=======
	local family=$1
	local lret=0
	local IPF=""

	if [ $family = "inet" ];then
		IPF="ip6"
	fi

ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family nat {
	chain output {
		type nat hook output priority 0; policy accept;
		ip6 daddr dead:1::99 dnat $IPF to dead:2::99
>>>>>>> upstream/android-13
	}
}
EOF
	if [ $? -ne 0 ]; then
<<<<<<< HEAD
		echo "SKIP: Could not add add ip6 dnat hook"
=======
		echo "SKIP: Could not add add $family dnat hook"
>>>>>>> upstream/android-13
		return $ksft_skip
	fi

	# ping netns1, expect rewrite to netns2
<<<<<<< HEAD
	ip netns exec ns0 ping -q -c 1 dead:1::99 > /dev/null
=======
	ip netns exec "$ns0" ping -q -c 1 dead:1::99 > /dev/null
>>>>>>> upstream/android-13
	if [ $? -ne 0 ]; then
		lret=1
		echo "ERROR: ping6 failed"
		return $lret
	fi

	expect="packets 0 bytes 0"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns0 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns1$dir "$expect" "test_local_dnat6 1"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	expect="packets 1 bytes 104"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns0 ns2$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns2$dir "$expect" "test_local_dnat6 2"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# expect 0 count in ns1
	expect="packets 0 bytes 0"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_local_dnat6 3"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# expect 1 packet in ns2
	expect="packets 1 bytes 104"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns2 nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns0$dir "$expect" "test_local_dnat6 4"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

<<<<<<< HEAD
	test $lret -eq 0 && echo "PASS: ipv6 ping to ns1 was NATted to ns2"
	ip netns exec ns0 nft flush chain ip6 nat output
=======
	test $lret -eq 0 && echo "PASS: ipv6 ping to $ns1 was $family NATted to $ns2"
	ip netns exec "$ns0" nft flush chain ip6 nat output
>>>>>>> upstream/android-13

	return $lret
}

test_local_dnat()
{
<<<<<<< HEAD
	local lret=0
ip netns exec ns0 nft -f - <<EOF
table ip nat {
	chain output {
		type nat hook output priority 0; policy accept;
		ip daddr 10.0.1.99 dnat to 10.0.2.99
	}
}
EOF
	# ping netns1, expect rewrite to netns2
	ip netns exec ns0 ping -q -c 1 10.0.1.99 > /dev/null
=======
	local family=$1
	local lret=0
	local IPF=""

	if [ $family = "inet" ];then
		IPF="ip"
	fi

ip netns exec "$ns0" nft -f /dev/stdin <<EOF 2>/dev/null
table $family nat {
	chain output {
		type nat hook output priority 0; policy accept;
		ip daddr 10.0.1.99 dnat $IPF to 10.0.2.99
	}
}
EOF
	if [ $? -ne 0 ]; then
		if [ $family = "inet" ];then
			echo "SKIP: inet nat tests"
			test_inet_nat=false
			return $ksft_skip
		fi
		echo "SKIP: Could not add add $family dnat hook"
		return $ksft_skip
	fi

	# ping netns1, expect rewrite to netns2
	ip netns exec "$ns0" ping -q -c 1 10.0.1.99 > /dev/null
>>>>>>> upstream/android-13
	if [ $? -ne 0 ]; then
		lret=1
		echo "ERROR: ping failed"
		return $lret
	fi

	expect="packets 0 bytes 0"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns0 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns1$dir "$expect" "test_local_dnat 1"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	expect="packets 1 bytes 84"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns0 ns2$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns2$dir "$expect" "test_local_dnat 2"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# expect 0 count in ns1
	expect="packets 0 bytes 0"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_local_dnat 3"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# expect 1 packet in ns2
	expect="packets 1 bytes 84"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns2 nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns0$dir "$expect" "test_local_dnat 4"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

<<<<<<< HEAD
	test $lret -eq 0 && echo "PASS: ping to ns1 was NATted to ns2"

	ip netns exec ns0 nft flush chain ip nat output

	reset_counters
	ip netns exec ns0 ping -q -c 1 10.0.1.99 > /dev/null
=======
	test $lret -eq 0 && echo "PASS: ping to $ns1 was $family NATted to $ns2"

	ip netns exec "$ns0" nft flush chain $family nat output

	reset_counters
	ip netns exec "$ns0" ping -q -c 1 10.0.1.99 > /dev/null
>>>>>>> upstream/android-13
	if [ $? -ne 0 ]; then
		lret=1
		echo "ERROR: ping failed"
		return $lret
	fi

	expect="packets 1 bytes 84"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns1$dir "$expect" "test_local_dnat 5"
>>>>>>> upstream/android-13
			lret=1
		fi
	done
	expect="packets 0 bytes 0"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns0 ns2$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns2$dir "$expect" "test_local_dnat 6"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# expect 1 count in ns1
	expect="packets 1 bytes 84"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns0 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns0$dir "$expect" "test_local_dnat 7"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# expect 0 packet in ns2
	expect="packets 0 bytes 0"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns2 nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns2$dir "$expect"
=======
		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns0$dir "$expect" "test_local_dnat 8"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

<<<<<<< HEAD
	test $lret -eq 0 && echo "PASS: ping to ns1 OK after nat output chain flush"
=======
	test $lret -eq 0 && echo "PASS: ping to $ns1 OK after $family nat output chain flush"
>>>>>>> upstream/android-13

	return $lret
}


test_masquerade6()
{
<<<<<<< HEAD
	local lret=0

	ip netns exec ns0 sysctl net.ipv6.conf.all.forwarding=1 > /dev/null

	ip netns exec ns2 ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping ns1 from ns2 via ipv6"
=======
	local family=$1
	local natflags=$2
	local lret=0

	ip netns exec "$ns0" sysctl net.ipv6.conf.all.forwarding=1 > /dev/null

	ip netns exec "$ns2" ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 via ipv6"
>>>>>>> upstream/android-13
		return 1
		lret=1
	fi

	expect="packets 1 bytes 104"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns2$dir "$expect"
			lret=1
		fi

		cnt=$(ip netns exec ns2 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns2$dir "$expect" "test_masquerade6 1"
			lret=1
		fi

		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns1$dir "$expect" "test_masquerade6 2"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	reset_counters

# add masquerading rule
<<<<<<< HEAD
ip netns exec ns0 nft -f - <<EOF
table ip6 nat {
	chain postrouting {
		type nat hook postrouting priority 0; policy accept;
		meta oif veth0 masquerade
	}
}
EOF
	ip netns exec ns2 ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping ns1 from ns2 with active ipv6 masquerading"
=======
ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family nat {
	chain postrouting {
		type nat hook postrouting priority 0; policy accept;
		meta oif veth0 masquerade $natflags
	}
}
EOF
	if [ $? -ne 0 ]; then
		echo "SKIP: Could not add add $family masquerade hook"
		return $ksft_skip
	fi

	ip netns exec "$ns2" ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 with active $family masquerade $natflags"
>>>>>>> upstream/android-13
		lret=1
	fi

	# ns1 should have seen packets from ns0, due to masquerade
	expect="packets 1 bytes 104"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD

		cnt=$(ip netns exec ns1 nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
			lret=1
		fi

		cnt=$(ip netns exec ns2 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_masquerade6 3"
			lret=1
		fi

		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns1$dir "$expect" "test_masquerade6 4"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# ns1 should not have seen packets from ns2, due to masquerade
	expect="packets 0 bytes 0"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
			lret=1
		fi

		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_masquerade6 5"
			lret=1
		fi

		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns1$dir "$expect" "test_masquerade6 6"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

<<<<<<< HEAD
	ip netns exec ns0 nft flush chain ip6 nat postrouting
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not flush ip6 nat postrouting" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: IPv6 masquerade for ns2"
=======
	ip netns exec "$ns2" ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 with active ipv6 masquerade $natflags (attempt 2)"
		lret=1
	fi

	ip netns exec "$ns0" nft flush chain $family nat postrouting
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not flush $family nat postrouting" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: $family IPv6 masquerade $natflags for $ns2"
>>>>>>> upstream/android-13

	return $lret
}

test_masquerade()
{
<<<<<<< HEAD
	local lret=0

	ip netns exec ns0 sysctl net.ipv4.conf.veth0.forwarding=1 > /dev/null
	ip netns exec ns0 sysctl net.ipv4.conf.veth1.forwarding=1 > /dev/null

	ip netns exec ns2 ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: canot ping ns1 from ns2"
=======
	local family=$1
	local natflags=$2
	local lret=0

	ip netns exec "$ns0" sysctl net.ipv4.conf.veth0.forwarding=1 > /dev/null
	ip netns exec "$ns0" sysctl net.ipv4.conf.veth1.forwarding=1 > /dev/null

	ip netns exec "$ns2" ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from "$ns2" $natflags"
>>>>>>> upstream/android-13
		lret=1
	fi

	expect="packets 1 bytes 84"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns2$dir "$expect"
			lret=1
		fi

		cnt=$(ip netns exec ns2 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns2$dir "$expect" "test_masquerade 1"
			lret=1
		fi

		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns1$dir "$expect" "test_masquerade 2"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	reset_counters

# add masquerading rule
<<<<<<< HEAD
ip netns exec ns0 nft -f - <<EOF
table ip nat {
	chain postrouting {
		type nat hook postrouting priority 0; policy accept;
		meta oif veth0 masquerade
	}
}
EOF
	ip netns exec ns2 ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping ns1 from ns2 with active ip masquerading"
=======
ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family nat {
	chain postrouting {
		type nat hook postrouting priority 0; policy accept;
		meta oif veth0 masquerade $natflags
	}
}
EOF
	if [ $? -ne 0 ]; then
		echo "SKIP: Could not add add $family masquerade hook"
		return $ksft_skip
	fi

	ip netns exec "$ns2" ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 with active $family masquerade $natflags"
>>>>>>> upstream/android-13
		lret=1
	fi

	# ns1 should have seen packets from ns0, due to masquerade
	expect="packets 1 bytes 84"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
			lret=1
		fi

		cnt=$(ip netns exec ns2 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns0${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_masquerade 3"
			lret=1
		fi

		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns1$dir "$expect" "test_masquerade 4"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# ns1 should not have seen packets from ns2, due to masquerade
	expect="packets 0 bytes 0"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
			lret=1
		fi

		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_masquerade 5"
			lret=1
		fi

		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns1$dir "$expect" "test_masquerade 6"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

<<<<<<< HEAD
	ip netns exec ns0 nft flush chain ip nat postrouting
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not flush nat postrouting" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: IP masquerade for ns2"
=======
	ip netns exec "$ns2" ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 with active ip masquerade $natflags (attempt 2)"
		lret=1
	fi

	ip netns exec "$ns0" nft flush chain $family nat postrouting
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not flush $family nat postrouting" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: $family IP masquerade $natflags for $ns2"
>>>>>>> upstream/android-13

	return $lret
}

test_redirect6()
{
<<<<<<< HEAD
	local lret=0

	ip netns exec ns0 sysctl net.ipv6.conf.all.forwarding=1 > /dev/null

	ip netns exec ns2 ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannnot ping ns1 from ns2 via ipv6"
=======
	local family=$1
	local lret=0

	ip netns exec "$ns0" sysctl net.ipv6.conf.all.forwarding=1 > /dev/null

	ip netns exec "$ns2" ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannnot ping $ns1 from $ns2 via ipv6"
>>>>>>> upstream/android-13
		lret=1
	fi

	expect="packets 1 bytes 104"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns2$dir "$expect"
			lret=1
		fi

		cnt=$(ip netns exec ns2 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns2$dir "$expect" "test_redirect6 1"
			lret=1
		fi

		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns1$dir "$expect" "test_redirect6 2"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	reset_counters

# add redirect rule
<<<<<<< HEAD
ip netns exec ns0 nft -f - <<EOF
table ip6 nat {
=======
ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family nat {
>>>>>>> upstream/android-13
	chain prerouting {
		type nat hook prerouting priority 0; policy accept;
		meta iif veth1 meta l4proto icmpv6 ip6 saddr dead:2::99 ip6 daddr dead:1::99 redirect
	}
}
EOF
<<<<<<< HEAD
	ip netns exec ns2 ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping ns1 from ns2 with active ip6 redirect"
=======
	if [ $? -ne 0 ]; then
		echo "SKIP: Could not add add $family redirect hook"
		return $ksft_skip
	fi

	ip netns exec "$ns2" ping -q -c 1 dead:1::99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 via ipv6 with active $family redirect"
>>>>>>> upstream/android-13
		lret=1
	fi

	# ns1 should have seen no packets from ns2, due to redirection
	expect="packets 0 bytes 0"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_redirect6 3"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# ns0 should have seen packets from ns2, due to masquerade
	expect="packets 1 bytes 104"
	for dir in "in6" "out6" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_redirect6 4"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

<<<<<<< HEAD
	ip netns exec ns0 nft delete table ip6 nat
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not delete ip6 nat table" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: IPv6 redirection for ns2"
=======
	ip netns exec "$ns0" nft delete table $family nat
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not delete $family nat table" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: $family IPv6 redirection for $ns2"
>>>>>>> upstream/android-13

	return $lret
}

test_redirect()
{
<<<<<<< HEAD
	local lret=0

	ip netns exec ns0 sysctl net.ipv4.conf.veth0.forwarding=1 > /dev/null
	ip netns exec ns0 sysctl net.ipv4.conf.veth1.forwarding=1 > /dev/null

	ip netns exec ns2 ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping ns1 from ns2"
=======
	local family=$1
	local lret=0

	ip netns exec "$ns0" sysctl net.ipv4.conf.veth0.forwarding=1 > /dev/null
	ip netns exec "$ns0" sysctl net.ipv4.conf.veth1.forwarding=1 > /dev/null

	ip netns exec "$ns2" ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2"
>>>>>>> upstream/android-13
		lret=1
	fi

	expect="packets 1 bytes 84"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns2$dir "$expect"
			lret=1
		fi

		cnt=$(ip netns exec ns2 nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns2 ns1$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" $ns2$dir "$expect" "test_redirect 1"
			lret=1
		fi

		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns1$dir "$expect" "test_redirect 2"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	reset_counters

# add redirect rule
<<<<<<< HEAD
ip netns exec ns0 nft -f - <<EOF
table ip nat {
=======
ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family nat {
>>>>>>> upstream/android-13
	chain prerouting {
		type nat hook prerouting priority 0; policy accept;
		meta iif veth1 ip protocol icmp ip saddr 10.0.2.99 ip daddr 10.0.1.99 redirect
	}
}
EOF
<<<<<<< HEAD
	ip netns exec ns2 ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping ns1 from ns2 with active ip redirect"
=======
	if [ $? -ne 0 ]; then
		echo "SKIP: Could not add add $family redirect hook"
		return $ksft_skip
	fi

	ip netns exec "$ns2" ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 with active $family ip redirect"
>>>>>>> upstream/android-13
		lret=1
	fi

	# ns1 should have seen no packets from ns2, due to redirection
	expect="packets 0 bytes 0"
	for dir in "in" "out" ; do

<<<<<<< HEAD
		cnt=$(ip netns exec ns1 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_redirect 3"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

	# ns0 should have seen packets from ns2, due to masquerade
	expect="packets 1 bytes 84"
	for dir in "in" "out" ; do
<<<<<<< HEAD
		cnt=$(ip netns exec ns0 nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter ns1 ns0$dir "$expect"
=======
		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns0$dir "$expect" "test_redirect 4"
>>>>>>> upstream/android-13
			lret=1
		fi
	done

<<<<<<< HEAD
	ip netns exec ns0 nft delete table ip nat
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not delete nat table" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: IP redirection for ns2"
=======
	ip netns exec "$ns0" nft delete table $family nat
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not delete $family nat table" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: $family IP redirection for $ns2"
>>>>>>> upstream/android-13

	return $lret
}

<<<<<<< HEAD

# ip netns exec ns0 ping -c 1 -q 10.0.$i.99
for i in 0 1 2; do
ip netns exec ns$i nft -f - <<EOF
=======
# test port shadowing.
# create two listening services, one on router (ns0), one
# on client (ns2), which is masqueraded from ns1 point of view.
# ns2 sends udp packet coming from service port to ns1, on a highport.
# Later, if n1 uses same highport to connect to ns0:service, packet
# might be port-forwarded to ns2 instead.

# second argument tells if we expect the 'fake-entry' to take effect
# (CLIENT) or not (ROUTER).
test_port_shadow()
{
	local test=$1
	local expect=$2
	local daddrc="10.0.1.99"
	local daddrs="10.0.1.1"
	local result=""
	local logmsg=""

	echo ROUTER | ip netns exec "$ns0" nc -w 5 -u -l -p 1405 >/dev/null 2>&1 &
	nc_r=$!

	echo CLIENT | ip netns exec "$ns2" nc -w 5 -u -l -p 1405 >/dev/null 2>&1 &
	nc_c=$!

	# make shadow entry, from client (ns2), going to (ns1), port 41404, sport 1405.
	echo "fake-entry" | ip netns exec "$ns2" nc -w 1 -p 1405 -u "$daddrc" 41404 > /dev/null

	# ns1 tries to connect to ns0:1405.  With default settings this should connect
	# to client, it matches the conntrack entry created above.

	result=$(echo "" | ip netns exec "$ns1" nc -w 1 -p 41404 -u "$daddrs" 1405)

	if [ "$result" = "$expect" ] ;then
		echo "PASS: portshadow test $test: got reply from ${expect}${logmsg}"
	else
		echo "ERROR: portshadow test $test: got reply from \"$result\", not $expect as intended"
		ret=1
	fi

	kill $nc_r $nc_c 2>/dev/null

	# flush udp entries for next test round, if any
	ip netns exec "$ns0" conntrack -F >/dev/null 2>&1
}

# This prevents port shadow of router service via packet filter,
# packets claiming to originate from service port from internal
# network are dropped.
test_port_shadow_filter()
{
	local family=$1

ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family filter {
	chain forward {
		type filter hook forward priority 0; policy accept;
		meta iif veth1 udp sport 1405 drop
	}
}
EOF
	test_port_shadow "port-filter" "ROUTER"

	ip netns exec "$ns0" nft delete table $family filter
}

# This prevents port shadow of router service via notrack.
test_port_shadow_notrack()
{
	local family=$1

ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family raw {
	chain prerouting {
		type filter hook prerouting priority -300; policy accept;
		meta iif veth0 udp dport 1405 notrack
		udp dport 1405 notrack
	}
	chain output {
		type filter hook output priority -300; policy accept;
		udp sport 1405 notrack
	}
}
EOF
	test_port_shadow "port-notrack" "ROUTER"

	ip netns exec "$ns0" nft delete table $family raw
}

# This prevents port shadow of router service via sport remap.
test_port_shadow_pat()
{
	local family=$1

ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family pat {
	chain postrouting {
		type nat hook postrouting priority -1; policy accept;
		meta iif veth1 udp sport <= 1405 masquerade to : 1406-65535 random
	}
}
EOF
	test_port_shadow "pat" "ROUTER"

	ip netns exec "$ns0" nft delete table $family pat
}

test_port_shadowing()
{
	local family="ip"

	ip netns exec "$ns0" sysctl net.ipv4.conf.veth0.forwarding=1 > /dev/null
	ip netns exec "$ns0" sysctl net.ipv4.conf.veth1.forwarding=1 > /dev/null

	ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table $family nat {
	chain postrouting {
		type nat hook postrouting priority 0; policy accept;
		meta oif veth0 masquerade
	}
}
EOF
	if [ $? -ne 0 ]; then
		echo "SKIP: Could not add add $family masquerade hook"
		return $ksft_skip
	fi

	# test default behaviour. Packet from ns1 to ns0 is redirected to ns2.
	test_port_shadow "default" "CLIENT"

	# test packet filter based mitigation: prevent forwarding of
	# packets claiming to come from the service port.
	test_port_shadow_filter "$family"

	# test conntrack based mitigation: connections going or coming
	# from router:service bypass connection tracking.
	test_port_shadow_notrack "$family"

	# test nat based mitigation: fowarded packets coming from service port
	# are masqueraded with random highport.
	test_port_shadow_pat "$family"

	ip netns exec "$ns0" nft delete table $family nat
}

test_stateless_nat_ip()
{
	local lret=0

	ip netns exec "$ns0" sysctl net.ipv4.conf.veth0.forwarding=1 > /dev/null
	ip netns exec "$ns0" sysctl net.ipv4.conf.veth1.forwarding=1 > /dev/null

	ip netns exec "$ns2" ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 before loading stateless rules"
		return 1
	fi

ip netns exec "$ns0" nft -f /dev/stdin <<EOF
table ip stateless {
	map xlate_in {
		typeof meta iifname . ip saddr . ip daddr : ip daddr
		elements = {
			"veth1" . 10.0.2.99 . 10.0.1.99 : 10.0.2.2,
		}
	}
	map xlate_out {
		typeof meta iifname . ip saddr . ip daddr : ip daddr
		elements = {
			"veth0" . 10.0.1.99 . 10.0.2.2 : 10.0.2.99
		}
	}

	chain prerouting {
		type filter hook prerouting priority -400; policy accept;
		ip saddr set meta iifname . ip saddr . ip daddr map @xlate_in
		ip daddr set meta iifname . ip saddr . ip daddr map @xlate_out
	}
}
EOF
	if [ $? -ne 0 ]; then
		echo "SKIP: Could not add ip statless rules"
		return $ksft_skip
	fi

	reset_counters

	ip netns exec "$ns2" ping -q -c 1 10.0.1.99 > /dev/null # ping ns2->ns1
	if [ $? -ne 0 ] ; then
		echo "ERROR: cannot ping $ns1 from $ns2 with stateless rules"
		lret=1
	fi

	# ns1 should have seen packets from .2.2, due to stateless rewrite.
	expect="packets 1 bytes 84"
	cnt=$(ip netns exec "$ns1" nft list counter inet filter ns0insl | grep -q "$expect")
	if [ $? -ne 0 ]; then
		bad_counter "$ns1" ns0insl "$expect" "test_stateless 1"
		lret=1
	fi

	for dir in "in" "out" ; do
		cnt=$(ip netns exec "$ns2" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns2" ns1$dir "$expect" "test_stateless 2"
			lret=1
		fi
	done

	# ns1 should not have seen packets from ns2, due to masquerade
	expect="packets 0 bytes 0"
	for dir in "in" "out" ; do
		cnt=$(ip netns exec "$ns1" nft list counter inet filter ns2${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns1" ns0$dir "$expect" "test_stateless 3"
			lret=1
		fi

		cnt=$(ip netns exec "$ns0" nft list counter inet filter ns1${dir} | grep -q "$expect")
		if [ $? -ne 0 ]; then
			bad_counter "$ns0" ns1$dir "$expect" "test_stateless 4"
			lret=1
		fi
	done

	reset_counters

	socat -h > /dev/null 2>&1
	if [ $? -ne 0 ];then
		echo "SKIP: Could not run stateless nat frag test without socat tool"
		if [ $lret -eq 0 ]; then
			return $ksft_skip
		fi

		ip netns exec "$ns0" nft delete table ip stateless
		return $lret
	fi

	local tmpfile=$(mktemp)
	dd if=/dev/urandom of=$tmpfile bs=4096 count=1 2>/dev/null

	local outfile=$(mktemp)
	ip netns exec "$ns1" timeout 3 socat -u UDP4-RECV:4233 OPEN:$outfile < /dev/null &
	sc_r=$!

	sleep 1
	# re-do with large ping -> ip fragmentation
	ip netns exec "$ns2" timeout 3 socat - UDP4-SENDTO:"10.0.1.99:4233" < "$tmpfile" > /dev/null
	if [ $? -ne 0 ] ; then
		echo "ERROR: failed to test udp $ns1 to $ns2 with stateless ip nat" 1>&2
		lret=1
	fi

	wait

	cmp "$tmpfile" "$outfile"
	if [ $? -ne 0 ]; then
		ls -l "$tmpfile" "$outfile"
		echo "ERROR: in and output file mismatch when checking udp with stateless nat" 1>&2
		lret=1
	fi

	rm -f "$tmpfile" "$outfile"

	# ns1 should have seen packets from 2.2, due to stateless rewrite.
	expect="packets 3 bytes 4164"
	cnt=$(ip netns exec "$ns1" nft list counter inet filter ns0insl | grep -q "$expect")
	if [ $? -ne 0 ]; then
		bad_counter "$ns1" ns0insl "$expect" "test_stateless 5"
		lret=1
	fi

	ip netns exec "$ns0" nft delete table ip stateless
	if [ $? -ne 0 ]; then
		echo "ERROR: Could not delete table ip stateless" 1>&2
		lret=1
	fi

	test $lret -eq 0 && echo "PASS: IP statless for $ns2"

	return $lret
}

# ip netns exec "$ns0" ping -c 1 -q 10.0.$i.99
for i in 0 1 2; do
ip netns exec ns$i-$sfx nft -f /dev/stdin <<EOF
>>>>>>> upstream/android-13
table inet filter {
	counter ns0in {}
	counter ns1in {}
	counter ns2in {}

	counter ns0out {}
	counter ns1out {}
	counter ns2out {}

	counter ns0in6 {}
	counter ns1in6 {}
	counter ns2in6 {}

	counter ns0out6 {}
	counter ns1out6 {}
	counter ns2out6 {}

	map nsincounter {
		type ipv4_addr : counter
		elements = { 10.0.1.1 : "ns0in",
			     10.0.2.1 : "ns0in",
			     10.0.1.99 : "ns1in",
			     10.0.2.99 : "ns2in" }
	}

	map nsincounter6 {
		type ipv6_addr : counter
		elements = { dead:1::1 : "ns0in6",
			     dead:2::1 : "ns0in6",
			     dead:1::99 : "ns1in6",
			     dead:2::99 : "ns2in6" }
	}

	map nsoutcounter {
		type ipv4_addr : counter
		elements = { 10.0.1.1 : "ns0out",
			     10.0.2.1 : "ns0out",
			     10.0.1.99: "ns1out",
			     10.0.2.99: "ns2out" }
	}

	map nsoutcounter6 {
		type ipv6_addr : counter
		elements = { dead:1::1 : "ns0out6",
			     dead:2::1 : "ns0out6",
			     dead:1::99 : "ns1out6",
			     dead:2::99 : "ns2out6" }
	}

	chain input {
		type filter hook input priority 0; policy accept;
		counter name ip saddr map @nsincounter
		icmpv6 type { "echo-request", "echo-reply" } counter name ip6 saddr map @nsincounter6
	}
	chain output {
		type filter hook output priority 0; policy accept;
		counter name ip daddr map @nsoutcounter
		icmpv6 type { "echo-request", "echo-reply" } counter name ip6 daddr map @nsoutcounter6
	}
}
EOF
done

<<<<<<< HEAD
sleep 3
# test basic connectivity
for i in 1 2; do
  ip netns exec ns0 ping -c 1 -q 10.0.$i.99 > /dev/null
=======
# special case for stateless nat check, counter needs to
# be done before (input) ip defragmentation
ip netns exec ns1-$sfx nft -f /dev/stdin <<EOF
table inet filter {
	counter ns0insl {}

	chain pre {
		type filter hook prerouting priority -400; policy accept;
		ip saddr 10.0.2.2 counter name "ns0insl"
	}
}
EOF

sleep 3
# test basic connectivity
for i in 1 2; do
  ip netns exec "$ns0" ping -c 1 -q 10.0.$i.99 > /dev/null
>>>>>>> upstream/android-13
  if [ $? -ne 0 ];then
  	echo "ERROR: Could not reach other namespace(s)" 1>&2
	ret=1
  fi

<<<<<<< HEAD
  ip netns exec ns0 ping -c 1 -q dead:$i::99 > /dev/null
=======
  ip netns exec "$ns0" ping -c 1 -q dead:$i::99 > /dev/null
>>>>>>> upstream/android-13
  if [ $? -ne 0 ];then
	echo "ERROR: Could not reach other namespace(s) via ipv6" 1>&2
	ret=1
  fi
<<<<<<< HEAD
  check_counters ns$i
=======
  check_counters ns$i-$sfx
>>>>>>> upstream/android-13
  if [ $? -ne 0 ]; then
	ret=1
  fi

  check_ns0_counters ns$i
  if [ $? -ne 0 ]; then
	ret=1
  fi
  reset_counters
done

if [ $ret -eq 0 ];then
<<<<<<< HEAD
	echo "PASS: netns routing/connectivity: ns0 can reach ns1 and ns2"
fi

reset_counters
test_local_dnat
test_local_dnat6

reset_counters
test_masquerade
test_masquerade6

reset_counters
test_redirect
test_redirect6

for i in 0 1 2; do ip netns del ns$i;done
=======
	echo "PASS: netns routing/connectivity: $ns0 can reach $ns1 and $ns2"
fi

reset_counters
test_local_dnat ip
test_local_dnat6 ip6
reset_counters
$test_inet_nat && test_local_dnat inet
$test_inet_nat && test_local_dnat6 inet

for flags in "" "fully-random"; do
reset_counters
test_masquerade ip $flags
test_masquerade6 ip6 $flags
reset_counters
$test_inet_nat && test_masquerade inet $flags
$test_inet_nat && test_masquerade6 inet $flags
done

reset_counters
test_redirect ip
test_redirect6 ip6
reset_counters
$test_inet_nat && test_redirect inet
$test_inet_nat && test_redirect6 inet

test_port_shadowing
test_stateless_nat_ip

if [ $ret -ne 0 ];then
	echo -n "FAIL: "
	nft --version
fi
>>>>>>> upstream/android-13

exit $ret
