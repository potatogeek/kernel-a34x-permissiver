#!/bin/bash
<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
#
# Get an estimate of how CPU-hoggy to be.
#
# Usage: cpus2use.sh
#
<<<<<<< HEAD
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, you can access it online at
# http://www.gnu.org/licenses/gpl-2.0.html.
#
# Copyright (C) IBM Corporation, 2013
#
# Authors: Paul E. McKenney <paulmck@linux.vnet.ibm.com>

ncpus=`grep '^processor' /proc/cpuinfo | wc -l`
idlecpus=`mpstat | tail -1 | \
	awk -v ncpus=$ncpus '{ print ncpus * ($7 + $NF) / 100 }'`
=======
# Copyright (C) IBM Corporation, 2013
#
# Authors: Paul E. McKenney <paulmck@linux.ibm.com>

if test -n "$TORTURE_ALLOTED_CPUS"
then
	echo $TORTURE_ALLOTED_CPUS
	exit 0
fi
ncpus=`grep '^processor' /proc/cpuinfo | wc -l`
if mpstat -V > /dev/null 2>&1
then
	idlecpus=`mpstat | tail -1 | \
		awk -v ncpus=$ncpus '{ print ncpus * ($7 + $NF) / 100 }'`
else
	# No mpstat command, so use all available CPUs.
	idlecpus=$ncpus
fi
>>>>>>> upstream/android-13
awk -v ncpus=$ncpus -v idlecpus=$idlecpus < /dev/null '
BEGIN {
	cpus2use = idlecpus;
	if (cpus2use < 1)
		cpus2use = 1;
	if (cpus2use < ncpus / 10)
		cpus2use = ncpus / 10;
	if (cpus2use == int(cpus2use))
		cpus2use = int(cpus2use)
	else
		cpus2use = int(cpus2use) + 1
	print cpus2use;
}'

