#!/bin/bash
# SPDX-License-Identifier: GPL-2.0

<<<<<<< HEAD
NUM_NETIFS=6
source ../../../../net/forwarding/lib.sh
source ../../../../net/forwarding/tc_common.sh
=======
lib_dir=$(dirname $0)/../../../../net/forwarding

NUM_NETIFS=6
source $lib_dir/lib.sh
source $lib_dir/tc_common.sh
>>>>>>> upstream/android-13
source devlink_lib_spectrum.sh

current_test=""

cleanup()
{
	pre_cleanup
	if [ ! -z $current_test ]; then
		${current_test}_cleanup
	fi
	devlink_sp_size_kvd_to_default
}

devlink_sp_read_kvd_defaults
trap cleanup EXIT

<<<<<<< HEAD
ALL_TESTS="router tc_flower mirror_gre"
for current_test in ${TESTS:-$ALL_TESTS}; do
=======
ALL_TESTS="router tc_flower mirror_gre tc_police port"
for current_test in ${TESTS:-$ALL_TESTS}; do
	RET_FIN=0
>>>>>>> upstream/android-13
	source ${current_test}_scale.sh

	num_netifs_var=${current_test^^}_NUM_NETIFS
	num_netifs=${!num_netifs_var:-$NUM_NETIFS}

	for profile in $KVD_PROFILES; do
		RET=0
		devlink_sp_resource_kvd_profile_set $profile
		if [[ $RET -gt 0 ]]; then
			log_test "'$current_test' [$profile] setting"
			continue
		fi

		for should_fail in 0 1; do
			RET=0
			target=$(${current_test}_get_target "$should_fail")
			${current_test}_setup_prepare
			setup_wait $num_netifs
			${current_test}_test "$target" "$should_fail"
			${current_test}_cleanup
			if [[ "$should_fail" -eq 0 ]]; then
				log_test "'$current_test' [$profile] $target"
			else
				log_test "'$current_test' [$profile] overflow $target"
			fi
<<<<<<< HEAD
=======
			RET_FIN=$(( RET_FIN || RET ))
>>>>>>> upstream/android-13
		done
	done
done
current_test=""

<<<<<<< HEAD
exit "$RET"
=======
exit "$RET_FIN"
>>>>>>> upstream/android-13
