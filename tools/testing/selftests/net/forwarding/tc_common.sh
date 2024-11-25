#!/bin/bash
# SPDX-License-Identifier: GPL-2.0

CHECK_TC="yes"

<<<<<<< HEAD
=======
# Can be overridden by the configuration file. See lib.sh
TC_HIT_TIMEOUT=${TC_HIT_TIMEOUT:=1000} # ms

>>>>>>> upstream/android-13
tc_check_packets()
{
	local id=$1
	local handle=$2
	local count=$3
<<<<<<< HEAD
	local ret

	output="$(tc -j -s filter show $id)"
	# workaround the jq bug which causes jq to return 0 in case input is ""
	ret=$?
	if [[ $ret -ne 0 ]]; then
		return $ret
	fi
	echo $output | \
		jq -e ".[] \
		| select(.options.handle == $handle) \
		| select(.options.actions[0].stats.packets == $count)" \
		&> /dev/null
	return $?
=======

	busywait "$TC_HIT_TIMEOUT" until_counter_is "== $count" \
		 tc_rule_handle_stats_get "$id" "$handle" > /dev/null
}

tc_check_packets_hitting()
{
	local id=$1
	local handle=$2

	busywait "$TC_HIT_TIMEOUT" until_counter_is "> 0" \
		 tc_rule_handle_stats_get "$id" "$handle" > /dev/null
>>>>>>> upstream/android-13
}
