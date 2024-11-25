#!/bin/bash
<<<<<<< HEAD
#
# Usage: configinit.sh config-spec-file build-output-dir results-dir
=======
# SPDX-License-Identifier: GPL-2.0+
#
# Usage: configinit.sh config-spec-file results-dir
>>>>>>> upstream/android-13
#
# Create a .config file from the spec file.  Run from the kernel source tree.
# Exits with 0 if all went well, with 1 if all went well but the config
# did not match, and some other number for other failures.
#
# The first argument is the .config specification file, which contains
# desired settings, for example, "CONFIG_NO_HZ=y".  For best results,
# this should be a full pathname.
#
<<<<<<< HEAD
# The second argument is a optional path to a build output directory,
# for example, "O=/tmp/foo".  If this argument is omitted, the .config
# file will be generated directly in the current directory.
#
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
=======
# Copyright (C) IBM Corporation, 2013
#
# Authors: Paul E. McKenney <paulmck@linux.ibm.com>
>>>>>>> upstream/android-13

T=${TMPDIR-/tmp}/configinit.sh.$$
trap 'rm -rf $T' 0
mkdir $T

# Capture config spec file.

c=$1
<<<<<<< HEAD
buildloc=$2
resdir=$3
builddir=
if echo $buildloc | grep -q '^O='
then
	builddir=`echo $buildloc | sed -e 's/^O=//'`
	if test ! -d $builddir
	then
		mkdir $builddir
	fi
else
	echo Bad build directory: \"$buildloc\"
	exit 2
fi
=======
resdir=$2
>>>>>>> upstream/android-13

sed -e 's/^\(CONFIG[0-9A-Z_]*\)=.*$/grep -v "^# \1" |/' < $c > $T/u.sh
sed -e 's/^\(CONFIG[0-9A-Z_]*=\).*$/grep -v \1 |/' < $c >> $T/u.sh
grep '^grep' < $T/u.sh > $T/upd.sh
echo "cat - $c" >> $T/upd.sh
<<<<<<< HEAD
make mrproper
make $buildloc distclean > $resdir/Make.distclean 2>&1
make $buildloc $TORTURE_DEFCONFIG > $resdir/Make.defconfig.out 2>&1
mv $builddir/.config $builddir/.config.sav
sh $T/upd.sh < $builddir/.config.sav > $builddir/.config
cp $builddir/.config $builddir/.config.new
yes '' | make $buildloc oldconfig > $resdir/Make.oldconfig.out 2> $resdir/Make.oldconfig.err

# verify new config matches specification.
configcheck.sh $builddir/.config $c
=======
if test -z "$TORTURE_TRUST_MAKE"
then
	make clean > $resdir/Make.clean 2>&1
fi
make $TORTURE_KMAKE_ARG $TORTURE_DEFCONFIG > $resdir/Make.defconfig.out 2>&1
mv .config .config.sav
sh $T/upd.sh < .config.sav > .config
cp .config .config.new
yes '' | make $TORTURE_KMAKE_ARG oldconfig > $resdir/Make.oldconfig.out 2> $resdir/Make.oldconfig.err

# verify new config matches specification.
configcheck.sh .config $c
>>>>>>> upstream/android-13

exit 0
