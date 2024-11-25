#!/bin/bash
<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0+
>>>>>>> upstream/android-13
#
# Run a kvm-based test of the specified tree on the specified configs.
# Fully automated run and error checking, no graphics console.
#
# Execute this in the source tree.  Do not run it as a background task
# because qemu does not seem to like that much.
#
<<<<<<< HEAD
# Usage: kvm-test-1-run.sh config builddir resdir seconds qemu-args boot_args
=======
# Usage: kvm-test-1-run.sh config resdir seconds qemu-args boot_args_in
>>>>>>> upstream/android-13
#
# qemu-args defaults to "-enable-kvm -nographic", along with arguments
#			specifying the number of CPUs and other options
#			generated from the underlying CPU architecture.
<<<<<<< HEAD
# boot_args defaults to value returned by the per_version_boot_params
#			shell function.
#
# Anything you specify for either qemu-args or boot_args is appended to
=======
# boot_args_in defaults to value returned by the per_version_boot_params
#			shell function.
#
# Anything you specify for either qemu-args or boot_args_in is appended to
>>>>>>> upstream/android-13
# the default values.  The "-smp" value is deduced from the contents of
# the config fragment.
#
# More sophisticated argument parsing is clearly needed.
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
# Copyright (C) IBM Corporation, 2011
#
# Authors: Paul E. McKenney <paulmck@linux.ibm.com>
>>>>>>> upstream/android-13

T=${TMPDIR-/tmp}/kvm-test-1-run.sh.$$
trap 'rm -rf $T' 0
mkdir $T

. functions.sh
. $CONFIGFRAG/ver_functions.sh

config_template=${1}
config_dir=`echo $config_template | sed -e 's,/[^/]*$,,'`
title=`echo $config_template | sed -e 's/^.*\///'`
<<<<<<< HEAD
builddir=${2}
if test -z "$builddir" -o ! -d "$builddir" -o ! -w "$builddir"
then
	echo "kvm-test-1-run.sh :$builddir: Not a writable directory, cannot build into it"
	exit 1
fi
resdir=${3}
=======
resdir=${2}
>>>>>>> upstream/android-13
if test -z "$resdir" -o ! -d "$resdir" -o ! -w "$resdir"
then
	echo "kvm-test-1-run.sh :$resdir: Not a writable directory, cannot store results into it"
	exit 1
fi
<<<<<<< HEAD
echo ' ---' `date`: Starting build
echo ' ---' Kconfig fragment at: $config_template >> $resdir/log
touch $resdir/ConfigFragment.input $resdir/ConfigFragment
if test -r "$config_dir/CFcommon"
then
	echo " --- $config_dir/CFcommon" >> $resdir/ConfigFragment.input
	cat < $config_dir/CFcommon >> $resdir/ConfigFragment.input
	config_override.sh $config_dir/CFcommon $config_template > $T/Kc1
	grep '#CHECK#' $config_dir/CFcommon >> $resdir/ConfigFragment
else
	cp $config_template $T/Kc1
fi
echo " --- $config_template" >> $resdir/ConfigFragment.input
cat $config_template >> $resdir/ConfigFragment.input
grep '#CHECK#' $config_template >> $resdir/ConfigFragment
if test -n "$TORTURE_KCONFIG_ARG"
then
	echo $TORTURE_KCONFIG_ARG | tr -s " " "\012" > $T/cmdline
	echo " --- --kconfig argument" >> $resdir/ConfigFragment.input
	cat $T/cmdline >> $resdir/ConfigFragment.input
	config_override.sh $T/Kc1 $T/cmdline > $T/Kc2
	# Note that "#CHECK#" is not permitted on commandline.
else
	cp $T/Kc1 $T/Kc2
fi
cat $T/Kc2 >> $resdir/ConfigFragment

base_resdir=`echo $resdir | sed -e 's/\.[0-9]\+$//'`
if test "$base_resdir" != "$resdir" -a -f $base_resdir/bzImage -a -f $base_resdir/vmlinux
=======
echo ' ---' `date`: Starting build, PID $$
echo ' ---' Kconfig fragment at: $config_template >> $resdir/log
touch $resdir/ConfigFragment.input

# Combine additional Kconfig options into an existing set such that
# newer options win.  The first argument is the Kconfig source ID, the
# second the to-be-updated file within $T, and the third and final the
# list of additional Kconfig options.  Note that a $2.tmp file is
# created when doing the update.
config_override_param () {
	if test -n "$3"
	then
		echo $3 | sed -e 's/^ *//' -e 's/ *$//' | tr -s " " "\012" > $T/Kconfig_args
		echo " --- $1" >> $resdir/ConfigFragment.input
		cat $T/Kconfig_args >> $resdir/ConfigFragment.input
		config_override.sh $T/$2 $T/Kconfig_args > $T/$2.tmp
		mv $T/$2.tmp $T/$2
		# Note that "#CHECK#" is not permitted on commandline.
	fi
}

echo > $T/KcList
config_override_param "$config_dir/CFcommon" KcList "`cat $config_dir/CFcommon 2> /dev/null`"
config_override_param "$config_template" KcList "`cat $config_template 2> /dev/null`"
config_override_param "--gdb options" KcList "$TORTURE_KCONFIG_GDB_ARG"
config_override_param "--kasan options" KcList "$TORTURE_KCONFIG_KASAN_ARG"
config_override_param "--kcsan options" KcList "$TORTURE_KCONFIG_KCSAN_ARG"
config_override_param "--kconfig argument" KcList "$TORTURE_KCONFIG_ARG"
cp $T/KcList $resdir/ConfigFragment

base_resdir=`echo $resdir | sed -e 's/\.[0-9]\+$//'`
if test "$base_resdir" != "$resdir" && test -f $base_resdir/bzImage && test -f $base_resdir/vmlinux
>>>>>>> upstream/android-13
then
	# Rerunning previous test, so use that test's kernel.
	QEMU="`identify_qemu $base_resdir/vmlinux`"
	BOOT_IMAGE="`identify_boot_image $QEMU`"
	KERNEL=$base_resdir/${BOOT_IMAGE##*/} # use the last component of ${BOOT_IMAGE}
	ln -s $base_resdir/Make*.out $resdir  # for kvm-recheck.sh
	ln -s $base_resdir/.config $resdir  # for kvm-recheck.sh
	# Arch-independent indicator
	touch $resdir/builtkernel
<<<<<<< HEAD
elif kvm-build.sh $T/Kc2 $builddir $resdir
then
	# Had to build a kernel for this test.
	QEMU="`identify_qemu $builddir/vmlinux`"
	BOOT_IMAGE="`identify_boot_image $QEMU`"
	cp $builddir/vmlinux $resdir
	cp $builddir/.config $resdir
	cp $builddir/Module.symvers $resdir > /dev/null || :
	cp $builddir/System.map $resdir > /dev/null || :
	if test -n "$BOOT_IMAGE"
	then
		cp $builddir/$BOOT_IMAGE $resdir
=======
elif test "$base_resdir" != "$resdir"
then
	# Rerunning previous test for which build failed
	ln -s $base_resdir/Make*.out $resdir  # for kvm-recheck.sh
	ln -s $base_resdir/.config $resdir  # for kvm-recheck.sh
	echo Initial build failed, not running KVM, see $resdir.
	if test -f $resdir/build.wait
	then
		mv $resdir/build.wait $resdir/build.ready
	fi
	exit 1
elif kvm-build.sh $T/KcList $resdir
then
	# Had to build a kernel for this test.
	QEMU="`identify_qemu vmlinux`"
	BOOT_IMAGE="`identify_boot_image $QEMU`"
	cp vmlinux $resdir
	cp .config $resdir
	cp Module.symvers $resdir > /dev/null || :
	cp System.map $resdir > /dev/null || :
	if test -n "$BOOT_IMAGE"
	then
		cp $BOOT_IMAGE $resdir
>>>>>>> upstream/android-13
		KERNEL=$resdir/${BOOT_IMAGE##*/}
		# Arch-independent indicator
		touch $resdir/builtkernel
	else
		echo No identifiable boot image, not running KVM, see $resdir.
		echo Do the torture scripts know about your architecture?
	fi
	parse-build.sh $resdir/Make.out $title
else
	# Build failed.
<<<<<<< HEAD
	cp $builddir/Make*.out $resdir
	cp $builddir/.config $resdir || :
	echo Build failed, not running KVM, see $resdir.
	if test -f $builddir.wait
	then
		mv $builddir.wait $builddir.ready
	fi
	exit 1
fi
if test -f $builddir.wait
then
	mv $builddir.wait $builddir.ready
fi
while test -f $builddir.ready
do
	sleep 1
done
seconds=$4
qemu_args=$5
boot_args=$6

cd $KVM
kstarttime=`awk 'BEGIN { print systime() }' < /dev/null`
=======
	cp .config $resdir || :
	echo Build failed, not running KVM, see $resdir.
	if test -f $resdir/build.wait
	then
		mv $resdir/build.wait $resdir/build.ready
	fi
	exit 1
fi
if test -f $resdir/build.wait
then
	mv $resdir/build.wait $resdir/build.ready
fi
while test -f $resdir/build.ready
do
	sleep 1
done
seconds=$3
qemu_args=$4
boot_args_in=$5

>>>>>>> upstream/android-13
if test -z "$TORTURE_BUILDONLY"
then
	echo ' ---' `date`: Starting kernel
fi

# Generate -smp qemu argument.
qemu_args="-enable-kvm -nographic $qemu_args"
cpu_count=`configNR_CPUS.sh $resdir/ConfigFragment`
<<<<<<< HEAD
cpu_count=`configfrag_boot_cpus "$boot_args" "$config_template" "$cpu_count"`
vcpus=`identify_qemu_vcpus`
if test $cpu_count -gt $vcpus
then
	echo CPU count limited from $cpu_count to $vcpus | tee -a $resdir/Warnings
	cpu_count=$vcpus
fi
qemu_args="`specify_qemu_cpus "$QEMU" "$qemu_args" "$cpu_count"`"
=======
cpu_count=`configfrag_boot_cpus "$boot_args_in" "$config_template" "$cpu_count"`
if test "$cpu_count" -gt "$TORTURE_ALLOTED_CPUS"
then
	echo CPU count limited from $cpu_count to $TORTURE_ALLOTED_CPUS | tee -a $resdir/Warnings
	cpu_count=$TORTURE_ALLOTED_CPUS
fi
qemu_args="`specify_qemu_cpus "$QEMU" "$qemu_args" "$cpu_count"`"
qemu_args="`specify_qemu_net "$qemu_args"`"
>>>>>>> upstream/android-13

# Generate architecture-specific and interaction-specific qemu arguments
qemu_args="$qemu_args `identify_qemu_args "$QEMU" "$resdir/console.log"`"

# Generate qemu -append arguments
qemu_append="`identify_qemu_append "$QEMU"`"

# Pull in Kconfig-fragment boot parameters
<<<<<<< HEAD
boot_args="`configfrag_boot_params "$boot_args" "$config_template"`"
# Generate kernel-version-specific boot parameters
boot_args="`per_version_boot_params "$boot_args" $resdir/.config $seconds`"
=======
boot_args="`configfrag_boot_params "$boot_args_in" "$config_template"`"
# Generate kernel-version-specific boot parameters
boot_args="`per_version_boot_params "$boot_args" $resdir/.config $seconds`"
if test -n "$TORTURE_BOOT_GDB_ARG"
then
	boot_args="$boot_args $TORTURE_BOOT_GDB_ARG"
fi

# Give bare-metal advice
modprobe_args="`echo $boot_args | tr -s ' ' '\012' | grep "^$TORTURE_MOD\." | sed -e "s/$TORTURE_MOD\.//g"`"
kboot_args="`echo $boot_args | tr -s ' ' '\012' | grep -v "^$TORTURE_MOD\."`"
testid_txt="`dirname $resdir`/testid.txt"
touch $resdir/bare-metal
echo To run this scenario on bare metal: >> $resdir/bare-metal
echo >> $resdir/bare-metal
echo " 1." Set your bare-metal build tree to the state shown in this file: >> $resdir/bare-metal
echo "   " $testid_txt >> $resdir/bare-metal
echo " 2." Update your bare-metal build tree"'"s .config based on this file: >> $resdir/bare-metal
echo "   " $resdir/ConfigFragment >> $resdir/bare-metal
echo " 3." Make the bare-metal kernel"'"s build system aware of your .config updates: >> $resdir/bare-metal
echo "   " $ 'yes "" | make oldconfig' >> $resdir/bare-metal
echo " 4." Build your bare-metal kernel. >> $resdir/bare-metal
echo " 5." Boot your bare-metal kernel with the following parameters: >> $resdir/bare-metal
echo "   " $kboot_args >> $resdir/bare-metal
echo " 6." Start the test with the following command: >> $resdir/bare-metal
echo "   " $ modprobe $TORTURE_MOD $modprobe_args >> $resdir/bare-metal
echo " 7." After some time, end the test with the following command: >> $resdir/bare-metal
echo "   " $ rmmod $TORTURE_MOD >> $resdir/bare-metal
echo " 8." Copy your bare-metal kernel"'"s .config file, overwriting this file: >> $resdir/bare-metal
echo "   " $resdir/.config >> $resdir/bare-metal
echo " 9." Copy the console output from just before the modprobe to just after >> $resdir/bare-metal
echo "   " the rmmod into this file: >> $resdir/bare-metal
echo "   " $resdir/console.log >> $resdir/bare-metal
echo "10." Check for runtime errors using the following command: >> $resdir/bare-metal
echo "   " $ tools/testing/selftests/rcutorture/bin/kvm-recheck.sh `dirname $resdir` >> $resdir/bare-metal
echo >> $resdir/bare-metal
echo Some of the above steps may be skipped if you build your bare-metal >> $resdir/bare-metal
echo kernel here: `head -n 1 $testid_txt | sed -e 's/^Build directory: //'`  >> $resdir/bare-metal

echo $QEMU $qemu_args -m $TORTURE_QEMU_MEM -kernel $KERNEL -append \"$qemu_append $boot_args\" $TORTURE_QEMU_GDB_ARG > $resdir/qemu-cmd
echo "# TORTURE_SHUTDOWN_GRACE=$TORTURE_SHUTDOWN_GRACE" >> $resdir/qemu-cmd
echo "# seconds=$seconds" >> $resdir/qemu-cmd
echo "# TORTURE_KCONFIG_GDB_ARG=\"$TORTURE_KCONFIG_GDB_ARG\"" >> $resdir/qemu-cmd
echo "# TORTURE_JITTER_START=\"$TORTURE_JITTER_START\"" >> $resdir/qemu-cmd
echo "# TORTURE_JITTER_STOP=\"$TORTURE_JITTER_STOP\"" >> $resdir/qemu-cmd
echo "# TORTURE_TRUST_MAKE=\"$TORTURE_TRUST_MAKE\"; export TORTURE_TRUST_MAKE" >> $resdir/qemu-cmd
echo "# TORTURE_CPU_COUNT=$cpu_count" >> $resdir/qemu-cmd
>>>>>>> upstream/android-13

if test -n "$TORTURE_BUILDONLY"
then
	echo Build-only run specified, boot/test omitted.
	touch $resdir/buildonly
	exit 0
fi
<<<<<<< HEAD
echo "NOTE: $QEMU either did not run or was interactive" > $resdir/console.log
echo $QEMU $qemu_args -m $TORTURE_QEMU_MEM -kernel $KERNEL -append \"$qemu_append $boot_args\" > $resdir/qemu-cmd
( $QEMU $qemu_args -m $TORTURE_QEMU_MEM -kernel $KERNEL -append "$qemu_append $boot_args"& echo $! > $resdir/qemu_pid; wait `cat  $resdir/qemu_pid`; echo $? > $resdir/qemu-retval ) &
commandcompleted=0
sleep 10 # Give qemu's pid a chance to reach the file
if test -s "$resdir/qemu_pid"
then
	qemu_pid=`cat "$resdir/qemu_pid"`
	echo Monitoring qemu job at pid $qemu_pid
else
	qemu_pid=""
	echo Monitoring qemu job at yet-as-unknown pid
fi
while :
do
	if test -z "$qemu_pid" -a -s "$resdir/qemu_pid"
	then
		qemu_pid=`cat "$resdir/qemu_pid"`
	fi
	kruntime=`awk 'BEGIN { print systime() - '"$kstarttime"' }' < /dev/null`
	if test -z "$qemu_pid" || kill -0 "$qemu_pid" > /dev/null 2>&1
	then
		if test $kruntime -ge $seconds
		then
			break;
		fi
		sleep 1
	else
		commandcompleted=1
		if test $kruntime -lt $seconds
		then
			echo Completed in $kruntime vs. $seconds >> $resdir/Warnings 2>&1
			grep "^(qemu) qemu:" $resdir/kvm-test-1-run.sh.out >> $resdir/Warnings 2>&1
			killpid="`sed -n "s/^(qemu) qemu: terminating on signal [0-9]* from pid \([0-9]*\).*$/\1/p" $resdir/Warnings`"
			if test -n "$killpid"
			then
				echo "ps -fp $killpid" >> $resdir/Warnings 2>&1
				ps -fp $killpid >> $resdir/Warnings 2>&1
			fi
		else
			echo ' ---' `date`: "Kernel done"
		fi
		break
	fi
done
if test -z "$qemu_pid" -a -s "$resdir/qemu_pid"
then
	qemu_pid=`cat "$resdir/qemu_pid"`
fi
if test $commandcompleted -eq 0 -a -n "$qemu_pid"
then
	echo Grace period for qemu job at pid $qemu_pid
	oldline="`tail $resdir/console.log`"
	while :
	do
		kruntime=`awk 'BEGIN { print systime() - '"$kstarttime"' }' < /dev/null`
		if kill -0 $qemu_pid > /dev/null 2>&1
		then
			:
		else
			break
		fi
		must_continue=no
		newline="`tail $resdir/console.log`"
		if test "$newline" != "$oldline" && echo $newline | grep -q ' [0-9]\+us : '
		then
			must_continue=yes
		fi
		last_ts="`tail $resdir/console.log | grep '^\[ *[0-9]\+\.[0-9]\+]' | tail -1 | sed -e 's/^\[ *//' -e 's/\..*$//'`"
		if test -z "last_ts"
		then
			last_ts=0
		fi
		if test "$newline" != "$oldline" -a "$last_ts" -lt $((seconds + $TORTURE_SHUTDOWN_GRACE))
		then
			must_continue=yes
		fi
		if test $must_continue = no -a $kruntime -ge $((seconds + $TORTURE_SHUTDOWN_GRACE))
		then
			echo "!!! PID $qemu_pid hung at $kruntime vs. $seconds seconds" >> $resdir/Warnings 2>&1
			kill -KILL $qemu_pid
			break
		fi
		oldline=$newline
		sleep 10
	done
elif test -z "$qemu_pid"
then
	echo Unknown PID, cannot kill qemu command
fi

=======

kvm-test-1-run-qemu.sh $resdir
>>>>>>> upstream/android-13
parse-console.sh $resdir/console.log $title
