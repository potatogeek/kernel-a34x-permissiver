#!/bin/bash
<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13

# udelay() test script
#
# Test is executed by writing and reading to /sys/kernel/debug/udelay_test
# and exercises a variety of delays to ensure that udelay() is delaying
# at least as long as requested (as compared to ktime).
#
# Copyright (C) 2014 Google, Inc.
#
<<<<<<< HEAD
# This software is licensed under the terms of the GNU General Public
# License version 2, as published by the Free Software Foundation, and
# may be copied, distributed, and modified under those terms.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
=======
>>>>>>> upstream/android-13

MODULE_NAME=udelay_test
UDELAY_PATH=/sys/kernel/debug/udelay_test

setup()
{
	/sbin/modprobe -q $MODULE_NAME
	tmp_file=`mktemp`
}

test_one()
{
	delay=$1
	echo $delay > $UDELAY_PATH
	tee -a $tmp_file < $UDELAY_PATH
}

cleanup()
{
	if [ -f $tmp_file ]; then
		rm $tmp_file
	fi
	/sbin/modprobe -q -r $MODULE_NAME
}

trap cleanup EXIT
setup

# Delay for a variety of times.
# 1..200, 200..500 (by 10), 500..2000 (by 100)
for (( delay = 1; delay < 200; delay += 1 )); do
	test_one $delay
done
for (( delay = 200; delay < 500; delay += 10 )); do
	test_one $delay
done
for (( delay = 500; delay <= 2000; delay += 100 )); do
	test_one $delay
done

# Search for failures
count=`grep -c FAIL $tmp_file`
if [ $? -eq "0" ]; then
	echo "ERROR: $count delays failed to delay long enough"
	retcode=1
fi

exit $retcode
