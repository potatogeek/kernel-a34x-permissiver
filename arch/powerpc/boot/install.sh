#!/bin/sh
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright (C) 1995 by Linus Torvalds
#
# Blatantly stolen from in arch/i386/boot/install.sh by Dave Hansen 
#
# "make install" script for ppc64 architecture
#
# Arguments:
#   $1 - kernel version
#   $2 - kernel image file
#   $3 - kernel map file
#   $4 - default install path (blank if root directory)
<<<<<<< HEAD
#   $5 and more - kernel boot files; zImage*, uImage, cuImage.*, etc.
=======
>>>>>>> upstream/android-13
#

# Bail with error code if anything goes wrong
set -e

<<<<<<< HEAD
=======
verify () {
	if [ ! -f "$1" ]; then
		echo ""                                                   1>&2
		echo " *** Missing file: $1"                              1>&2
		echo ' *** You need to run "make" before "make install".' 1>&2
		echo ""                                                   1>&2
		exit 1
	fi
}

# Make sure the files actually exist
verify "$2"
verify "$3"

>>>>>>> upstream/android-13
# User may have a custom install script

if [ -x ~/bin/${INSTALLKERNEL} ]; then exec ~/bin/${INSTALLKERNEL} "$@"; fi
if [ -x /sbin/${INSTALLKERNEL} ]; then exec /sbin/${INSTALLKERNEL} "$@"; fi

# Default install

# this should work for both the pSeries zImage and the iSeries vmlinux.sm
image_name=`basename $2`

if [ -f $4/$image_name ]; then
	mv $4/$image_name $4/$image_name.old
fi

if [ -f $4/System.map ]; then
	mv $4/System.map $4/System.old
fi

cat $2 > $4/$image_name
cp $3 $4/System.map
<<<<<<< HEAD

# Copy all the bootable image files
path=$4
shift 4
while [ $# -ne 0 ]; do
	image_name=`basename $1`
	if [ -f $path/$image_name ]; then
		mv $path/$image_name $path/$image_name.old
	fi
	cat $1 > $path/$image_name
	shift
done;
=======
>>>>>>> upstream/android-13
