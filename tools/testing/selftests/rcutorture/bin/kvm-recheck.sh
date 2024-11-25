#!/bin/bash
<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
#
# Given the results directories for previous KVM-based torture runs,
# check the build and console output for errors.  Given a directory
# containing results directories, this recursively checks them all.
#
# Usage: kvm-recheck.sh resdir ...
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
# Copyright (C) IBM Corporation, 2011
#
# Authors: Paul E. McKenney <paulmck@linux.vnet.ibm.com>
=======
# Returns status reflecting the success or not of the last run specified.
#
# Copyright (C) IBM Corporation, 2011
#
# Authors: Paul E. McKenney <paulmck@linux.ibm.com>

T=/tmp/kvm-recheck.sh.$$
trap 'rm -f $T' 0 2
>>>>>>> upstream/android-13

PATH=`pwd`/tools/testing/selftests/rcutorture/bin:$PATH; export PATH
. functions.sh
for rd in "$@"
do
	firsttime=1
	dirs=`find $rd -name Make.defconfig.out -print | sort | sed -e 's,/[^/]*$,,' | sort -u`
	for i in $dirs
	do
		if test -n "$firsttime"
		then
			firsttime=""
			resdir=`echo $i | sed -e 's,/$,,' -e 's,/[^/]*$,,'`
			head -1 $resdir/log
		fi
<<<<<<< HEAD
		TORTURE_SUITE="`cat $i/../TORTURE_SUITE`"
		rm -f $i/console.log.*.diags
		kvm-recheck-${TORTURE_SUITE}.sh $i
		if test -f "$i/console.log"
		then
			configcheck.sh $i/.config $i/ConfigFragment
=======
		TORTURE_SUITE="`cat $i/../torture_suite`"
		configfile=`echo $i | sed -e 's,^.*/,,'`
		rm -f $i/console.log.*.diags
		kvm-recheck-${TORTURE_SUITE}.sh $i
		if test -f "$i/qemu-retval" && test "`cat $i/qemu-retval`" -ne 0 && test "`cat $i/qemu-retval`" -ne 137
		then
			echo QEMU error, output:
			cat $i/qemu-output
		elif test -f "$i/console.log"
		then
			if test -f "$i/qemu-retval" && test "`cat $i/qemu-retval`" -eq 137
			then
				echo QEMU killed
			fi
			configcheck.sh $i/.config $i/ConfigFragment > $T 2>&1
			cat $T
>>>>>>> upstream/android-13
			if test -r $i/Make.oldconfig.err
			then
				cat $i/Make.oldconfig.err
			fi
			parse-build.sh $i/Make.out $configfile
			parse-console.sh $i/console.log $configfile
			if test -r $i/Warnings
			then
				cat $i/Warnings
			fi
		else
<<<<<<< HEAD
			if test -f "$i/qemu-cmd"
			then
				print_bug qemu failed
				echo "   $i"
			elif test -f "$i/buildonly"
=======
			if test -f "$i/buildonly"
>>>>>>> upstream/android-13
			then
				echo Build-only run, no boot/test
				configcheck.sh $i/.config $i/ConfigFragment
				parse-build.sh $i/Make.out $configfile
<<<<<<< HEAD
=======
			elif test -f "$i/qemu-cmd"
			then
				print_bug qemu failed
				echo "   $i"
>>>>>>> upstream/android-13
			else
				print_bug Build failed
				echo "   $i"
			fi
		fi
	done
<<<<<<< HEAD
done
=======
	if test -f "$rd/kcsan.sum"
	then
		if ! test -f $T
		then
			:
		elif grep -q CONFIG_KCSAN=y $T
		then
			echo "Compiler or architecture does not support KCSAN!"
			echo Did you forget to switch your compiler with '--kmake-arg CC=<cc-that-supports-kcsan>'?
		elif test -s "$rd/kcsan.sum"
		then
			echo KCSAN summary in $rd/kcsan.sum
		else
			echo Clean KCSAN run in $rd
		fi
	fi
done
EDITOR=echo kvm-find-errors.sh "${@: -1}" > $T 2>&1
builderrors="`tr ' ' '\012' < $T | grep -c '/Make.out.diags'`"
if test "$builderrors" -gt 0
then
	echo $builderrors runs with build errors.
	ret=1
fi
runerrors="`tr ' ' '\012' < $T | grep -c '/console.log.diags'`"
if test "$runerrors" -gt 0
then
	echo $runerrors runs with runtime errors.
	ret=2
fi
exit $ret
>>>>>>> upstream/android-13
