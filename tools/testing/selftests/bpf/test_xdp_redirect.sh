<<<<<<< HEAD
#!/bin/sh
=======
#!/bin/bash
>>>>>>> upstream/android-13
# Create 2 namespaces with two veth peers, and
# forward packets in-between using generic XDP
#
# NS1(veth11)     NS2(veth22)
#     |               |
#     |               |
#   (veth1, ------ (veth2,
#   id:111)         id:222)
#     | xdp forwarding |
#     ------------------

<<<<<<< HEAD
cleanup()
{
	if [ "$?" = "0" ]; then
		echo "selftests: test_xdp_redirect [PASS]";
	else
		echo "selftests: test_xdp_redirect [FAILED]";
	fi

	set +e
=======
ret=0

setup()
{

	local xdpmode=$1

	ip netns add ns1
	ip netns add ns2

	ip link add veth1 index 111 type veth peer name veth11 netns ns1
	ip link add veth2 index 222 type veth peer name veth22 netns ns2

	ip link set veth1 up
	ip link set veth2 up
	ip -n ns1 link set dev veth11 up
	ip -n ns2 link set dev veth22 up

	ip -n ns1 addr add 10.1.1.11/24 dev veth11
	ip -n ns2 addr add 10.1.1.22/24 dev veth22
}

cleanup()
{
>>>>>>> upstream/android-13
	ip link del veth1 2> /dev/null
	ip link del veth2 2> /dev/null
	ip netns del ns1 2> /dev/null
	ip netns del ns2 2> /dev/null
}

<<<<<<< HEAD
ip link set dev lo xdpgeneric off 2>/dev/null > /dev/null
if [ $? -ne 0 ];then
	echo "selftests: [SKIP] Could not run test without the ip xdpgeneric support"
	exit 0
fi
set -e

ip netns add ns1
ip netns add ns2

trap cleanup 0 2 3 6 9

ip link add veth1 index 111 type veth peer name veth11
ip link add veth2 index 222 type veth peer name veth22

ip link set veth11 netns ns1
ip link set veth22 netns ns2

ip link set veth1 up
ip link set veth2 up

ip netns exec ns1 ip addr add 10.1.1.11/24 dev veth11
ip netns exec ns2 ip addr add 10.1.1.22/24 dev veth22

ip netns exec ns1 ip link set dev veth11 up
ip netns exec ns2 ip link set dev veth22 up

ip link set dev veth1 xdpgeneric obj test_xdp_redirect.o sec redirect_to_222
ip link set dev veth2 xdpgeneric obj test_xdp_redirect.o sec redirect_to_111

ip netns exec ns1 ping -c 1 10.1.1.22
ip netns exec ns2 ping -c 1 10.1.1.11

exit 0
=======
test_xdp_redirect()
{
	local xdpmode=$1

	setup

	ip link set dev veth1 $xdpmode off &> /dev/null
	if [ $? -ne 0 ];then
		echo "selftests: test_xdp_redirect $xdpmode [SKIP]"
		return 0
	fi

	ip -n ns1 link set veth11 $xdpmode obj xdp_dummy.o sec xdp &> /dev/null
	ip -n ns2 link set veth22 $xdpmode obj xdp_dummy.o sec xdp &> /dev/null
	ip link set dev veth1 $xdpmode obj test_xdp_redirect.o sec redirect_to_222 &> /dev/null
	ip link set dev veth2 $xdpmode obj test_xdp_redirect.o sec redirect_to_111 &> /dev/null

	if ip netns exec ns1 ping -c 1 10.1.1.22 &> /dev/null &&
	   ip netns exec ns2 ping -c 1 10.1.1.11 &> /dev/null; then
		echo "selftests: test_xdp_redirect $xdpmode [PASS]";
	else
		ret=1
		echo "selftests: test_xdp_redirect $xdpmode [FAILED]";
	fi

	cleanup
}

set -e
trap cleanup 2 3 6 9

test_xdp_redirect xdpgeneric
test_xdp_redirect xdpdrv

exit $ret
>>>>>>> upstream/android-13
