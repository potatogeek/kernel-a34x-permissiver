#!/bin/bash
<<<<<<< HEAD
# Copyright (c) 2015 Oracle and/or its affiliates. All Rights Reserved.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it would be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
=======
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (c) 2015 Oracle and/or its affiliates. All Rights Reserved.
#
>>>>>>> upstream/android-13
# Test checks that we can create swap zram device.
#
# Author: Alexey Kodanev <alexey.kodanev@oracle.com>
# Modified: Naresh Kamboju <naresh.kamboju@linaro.org>

TCID="zram02"
ERR_CODE=0

. ./zram_lib.sh

# Test will create the following number of zram devices:
dev_num=1
# This is a list of parameters for zram devices.
# Number of items must be equal to 'dev_num' parameter.
zram_max_streams="2"

# The zram sysfs node 'disksize' value can be either in bytes,
# or you can use mem suffixes. But in some old kernels, mem
# suffixes are not supported, for example, in RHEL6.6GA's kernel
# layer, it uses strict_strtoull() to parse disksize which does
# not support mem suffixes, in some newer kernels, they use
# memparse() which supports mem suffixes. So here we just use
# bytes to make sure everything works correctly.
zram_sizes="1048576" # 1M
zram_mem_limits="1M"

check_prereqs
zram_load
zram_max_streams
zram_set_disksizes
zram_set_memlimit
zram_makeswap
zram_swapoff
zram_cleanup
<<<<<<< HEAD
zram_unload
=======
>>>>>>> upstream/android-13

if [ $ERR_CODE -ne 0 ]; then
	echo "$TCID : [FAIL]"
else
	echo "$TCID : [PASS]"
fi
