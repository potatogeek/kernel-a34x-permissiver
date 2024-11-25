#!/bin/bash
# SPDX-License-Identifier: GPL-2.0
#
# Kselftest Install
# Install kselftest tests
# Author: Shuah Khan <shuahkh@osg.samsung.com>
# Copyright (C) 2015 Samsung Electronics Co., Ltd.

<<<<<<< HEAD
install_loc=`pwd`

main()
{
	if [ $(basename $install_loc) !=  "selftests" ]; then
		echo "$0: Please run it in selftests directory ..."
		exit 1;
	fi
	if [ "$#" -eq 0 ]; then
		echo "$0: Installing in default location - $install_loc ..."
=======
main()
{
	base_dir=`pwd`
	install_dir="$base_dir"/kselftest_install

	# Make sure we're in the selftests top-level directory.
	if [ $(basename "$base_dir") !=  "selftests" ]; then
		echo "$0: Please run it in selftests directory ..."
		exit 1;
	fi

	# Only allow installation into an existing location.
	if [ "$#" -eq 0 ]; then
		echo "$0: Installing in default location - $install_dir ..."
>>>>>>> upstream/android-13
	elif [ ! -d "$1" ]; then
		echo "$0: $1 doesn't exist!!"
		exit 1;
	else
<<<<<<< HEAD
		install_loc=$1
		echo "$0: Installing in specified location - $install_loc ..."
	fi

	install_dir=$install_loc/kselftest

# Create install directory
	mkdir -p $install_dir
# Build tests
	INSTALL_PATH=$install_dir make install
=======
		install_dir="$1"
		echo "$0: Installing in specified location - $install_dir ..."
	fi

	# Build tests
	KSFT_INSTALL_PATH="$install_dir" make install
>>>>>>> upstream/android-13
}

main "$@"
