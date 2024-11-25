#!/bin/bash
# SPDX-License-Identifier: GPL-2.0

ROUTER_NUM_NETIFS=4
<<<<<<< HEAD
=======
: ${TIMEOUT:=20000} # ms
>>>>>>> upstream/android-13

router_h1_create()
{
	simple_if_init $h1 192.0.1.1/24
<<<<<<< HEAD
	ip route add 193.0.0.0/8 via 192.0.1.2 dev $h1
=======
>>>>>>> upstream/android-13
}

router_h1_destroy()
{
<<<<<<< HEAD
	ip route del 193.0.0.0/8 via 192.0.1.2 dev $h1
=======
>>>>>>> upstream/android-13
	simple_if_fini $h1 192.0.1.1/24
}

router_h2_create()
{
	simple_if_init $h2 192.0.2.1/24
	tc qdisc add dev $h2 handle ffff: ingress
}

router_h2_destroy()
{
	tc qdisc del dev $h2 handle ffff: ingress
	simple_if_fini $h2 192.0.2.1/24
}

router_create()
{
	ip link set dev $rp1 up
	ip link set dev $rp2 up

	ip address add 192.0.1.2/24 dev $rp1
	ip address add 192.0.2.2/24 dev $rp2
}

router_destroy()
{
	ip address del 192.0.2.2/24 dev $rp2
	ip address del 192.0.1.2/24 dev $rp1

	ip link set dev $rp2 down
	ip link set dev $rp1 down
}

router_setup_prepare()
{
	h1=${NETIFS[p1]}
	rp1=${NETIFS[p2]}

	rp2=${NETIFS[p3]}
	h2=${NETIFS[p4]}

	h1mac=$(mac_get $h1)
	rp1mac=$(mac_get $rp1)

	vrf_prepare

	router_h1_create
	router_h2_create

	router_create
}

<<<<<<< HEAD
router_offload_validate()
{
	local route_count=$1
	local offloaded_count

	offloaded_count=$(ip route | grep -o 'offload' | wc -l)
	[[ $offloaded_count -ge $route_count ]]
=======
wait_for_routes()
{
	local t0=$1; shift
	local route_count=$1; shift

	local t1=$(ip route | grep 'offload' | grep -v 'offload_failed' | wc -l)
	local delta=$((t1 - t0))
	echo $delta
	[[ $delta -ge $route_count ]]
>>>>>>> upstream/android-13
}

router_routes_create()
{
	local route_count=$1
	local count=0

	ROUTE_FILE="$(mktemp)"

	for i in {0..255}
	do
		for j in {0..255}
		do
			for k in {0..255}
			do
				if [[ $count -eq $route_count ]]; then
					break 3
				fi

<<<<<<< HEAD
				echo route add 193.${i}.${j}.${k}/32 via \
				       192.0.2.1 dev $rp2  >> $ROUTE_FILE
=======
				echo route add 193.${i}.${j}.${k}/32 dev $rp2 \
					>> $ROUTE_FILE
>>>>>>> upstream/android-13
				((count++))
			done
		done
	done

	ip -b $ROUTE_FILE &> /dev/null
}

router_routes_destroy()
{
	if [[ -v ROUTE_FILE ]]; then
		rm -f $ROUTE_FILE
	fi
}

router_test()
{
	local route_count=$1
	local should_fail=$2
<<<<<<< HEAD
	local count=0

	RET=0

	router_routes_create $route_count

	router_offload_validate $route_count
	check_err_fail $should_fail $? "Offload of $route_count routes"
=======
	local delta

	RET=0

	local t0=$(ip route | grep -o 'offload' | wc -l)
	router_routes_create $route_count
	delta=$(busywait "$TIMEOUT" wait_for_routes $t0 $route_count)

	check_err_fail $should_fail $? "Offload routes: Expected $route_count, got $delta."
>>>>>>> upstream/android-13
	if [[ $RET -ne 0 ]] || [[ $should_fail -eq 1 ]]; then
		return
	fi

<<<<<<< HEAD
	tc filter add dev $h2 ingress protocol ip pref 1 flower \
		skip_sw dst_ip 193.0.0.0/8 action drop

	for i in {0..255}
	do
		for j in {0..255}
		do
			for k in {0..255}
			do
				if [[ $count -eq $route_count ]]; then
					break 3
				fi

				$MZ $h1 -c 1 -p 64 -a $h1mac -b $rp1mac \
					-A 192.0.1.1 -B 193.${i}.${j}.${k} \
					-t ip -q
				((count++))
			done
		done
	done

	tc_check_packets "dev $h2 ingress" 1 $route_count
	check_err $? "Offload mismatch"

	tc filter del dev $h2 ingress protocol ip pref 1 flower \
		skip_sw dst_ip 193.0.0.0/8 action drop

=======
>>>>>>> upstream/android-13
	router_routes_destroy
}

router_cleanup()
{
	pre_cleanup

	router_routes_destroy
	router_destroy

	router_h2_destroy
	router_h1_destroy

	vrf_cleanup
}
