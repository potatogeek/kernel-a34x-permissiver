#!/bin/sh
<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
#
# Invoke a text editor on all console.log files for all runs with diagnostics,
# that is, on all such files having a console.log.diags counterpart.
# Note that both console.log.diags and console.log are passed to the
# editor (currently defaulting to "vi"), allowing the user to get an
# idea of what to search for in the console.log file.
#
# Usage: kvm-find-errors.sh directory
#
# The "directory" above should end with the date/time directory, for example,
# "tools/testing/selftests/rcutorture/res/2018.02.25-14:27:27".
<<<<<<< HEAD
=======
# Returns error status reflecting the success (or not) of the specified run.
#
# Copyright (C) IBM Corporation, 2018
#
# Author: Paul E. McKenney <paulmck@linux.ibm.com>
>>>>>>> upstream/android-13

rundir="${1}"
if test -z "$rundir" -o ! -d "$rundir"
then
<<<<<<< HEAD
	echo Usage: $0 directory
=======
	echo Directory "$rundir" not found.
	echo Usage: $0 directory
	exit 1
>>>>>>> upstream/android-13
fi
editor=${EDITOR-vi}

# Find builds with errors
files=
for i in ${rundir}/*/Make.out
do
	if egrep -q "error:|warning:" < $i
	then
		egrep "error:|warning:" < $i > $i.diags
		files="$files $i.diags $i"
	fi
done
if test -n "$files"
then
	$editor $files
<<<<<<< HEAD
else
	echo No build errors.
fi
if grep -q -e "--buildonly" < ${rundir}/log
then
	echo Build-only run, no console logs to check.
=======
	editorret=1
else
	echo No build errors.
fi
if grep -q -e "--build-\?only" < ${rundir}/log && ! test -f "${rundir}/remote-log"
then
	echo Build-only run, no console logs to check.
	exit $editorret
>>>>>>> upstream/android-13
fi

# Find console logs with errors
files=
for i in ${rundir}/*/console.log
do
	if test -r $i.diags
	then
		files="$files $i.diags $i"
	fi
done
if test -n "$files"
then
	$editor $files
<<<<<<< HEAD
else
	echo No errors in console logs.
=======
	exit 1
else
	echo No errors in console logs.
	if test -n "$editorret"
	then
		exit $editorret
	else
		exit 0
	fi
>>>>>>> upstream/android-13
fi
