#!/bin/sh
<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
#
>>>>>>> upstream/android-13
#  merge_config.sh - Takes a list of config fragment values, and merges
#  them one by one. Provides warnings on overridden values, and specified
#  values that did not make it to the resulting .config file (due to missed
#  dependencies or config symbol removal).
#
#  Portions reused from kconf_check and generate_cfg:
#  http://git.yoctoproject.org/cgit/cgit.cgi/yocto-kernel-tools/tree/tools/kconf_check
#  http://git.yoctoproject.org/cgit/cgit.cgi/yocto-kernel-tools/tree/tools/generate_cfg
#
#  Copyright (c) 2009-2010 Wind River Systems, Inc.
#  Copyright 2011 Linaro
<<<<<<< HEAD
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License version 2 as
#  published by the Free Software Foundation.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#  See the GNU General Public License for more details.

clean_up() {
	rm -f $TMP_FILE
	exit
}
trap clean_up HUP INT TERM
=======

set -e

clean_up() {
	rm -f $TMP_FILE
	rm -f $MERGE_FILE
}
>>>>>>> upstream/android-13

usage() {
	echo "Usage: $0 [OPTIONS] [CONFIG [...]]"
	echo "  -h    display this help text"
	echo "  -m    only merge the fragments, do not execute the make command"
	echo "  -n    use allnoconfig instead of alldefconfig"
	echo "  -r    list redundant entries when merging fragments"
<<<<<<< HEAD
	echo "  -O    dir to put generated output files.  Consider setting \$KCONFIG_CONFIG instead."
=======
	echo "  -y    make builtin have precedence over modules"
	echo "  -O    dir to put generated output files.  Consider setting \$KCONFIG_CONFIG instead."
	echo "  -s    strict mode. Fail if the fragment redefines any value."
	echo
	echo "Used prefix: '$CONFIG_PREFIX'. You can redefine it with \$CONFIG_ environment variable."
>>>>>>> upstream/android-13
}

RUNMAKE=true
ALLTARGET=alldefconfig
WARNREDUN=false
<<<<<<< HEAD
OUTPUT=.
=======
BUILTIN=false
OUTPUT=.
STRICT=false
CONFIG_PREFIX=${CONFIG_-CONFIG_}
>>>>>>> upstream/android-13

while true; do
	case $1 in
	"-n")
		ALLTARGET=allnoconfig
		shift
		continue
		;;
	"-m")
		RUNMAKE=false
		shift
		continue
		;;
	"-h")
		usage
		exit
		;;
	"-r")
		WARNREDUN=true
		shift
		continue
		;;
<<<<<<< HEAD
=======
	"-y")
		BUILTIN=true
		shift
		continue
		;;
>>>>>>> upstream/android-13
	"-O")
		if [ -d $2 ];then
			OUTPUT=$(echo $2 | sed 's/\/*$//')
		else
			echo "output directory $2 does not exist" 1>&2
			exit 1
		fi
		shift 2
		continue
		;;
<<<<<<< HEAD
=======
	"-s")
		STRICT=true
		shift
		continue
		;;
>>>>>>> upstream/android-13
	*)
		break
		;;
	esac
done

if [ "$#" -lt 1 ] ; then
	usage
	exit
fi

if [ -z "$KCONFIG_CONFIG" ]; then
	if [ "$OUTPUT" != . ]; then
		KCONFIG_CONFIG=$(readlink -m -- "$OUTPUT/.config")
	else
		KCONFIG_CONFIG=.config
	fi
fi

INITFILE=$1
shift;

if [ ! -r "$INITFILE" ]; then
	echo "The base file '$INITFILE' does not exist.  Exit." >&2
	exit 1
fi

MERGE_LIST=$*
<<<<<<< HEAD
SED_CONFIG_EXP="s/^\(# \)\{0,1\}\(CONFIG_[a-zA-Z0-9_]*\)[= ].*/\2/p"
TMP_FILE=$(mktemp ./.tmp.config.XXXXXXXXXX)

echo "Using $INITFILE as base"
cat $INITFILE > $TMP_FILE

# Merge files, printing warnings on overridden values
for MERGE_FILE in $MERGE_LIST ; do
	echo "Merging $MERGE_FILE"
	if [ ! -r "$MERGE_FILE" ]; then
		echo "The merge file '$MERGE_FILE' does not exist.  Exit." >&2
		exit 1
	fi
	CFG_LIST=$(sed -n "$SED_CONFIG_EXP" $MERGE_FILE)
=======
SED_CONFIG_EXP1="s/^\(${CONFIG_PREFIX}[a-zA-Z0-9_]*\)=.*/\1/p"
SED_CONFIG_EXP2="s/^# \(${CONFIG_PREFIX}[a-zA-Z0-9_]*\) is not set$/\1/p"

TMP_FILE=$(mktemp ./.tmp.config.XXXXXXXXXX)
MERGE_FILE=$(mktemp ./.merge_tmp.config.XXXXXXXXXX)

echo "Using $INITFILE as base"

trap clean_up EXIT

cat $INITFILE > $TMP_FILE

# Merge files, printing warnings on overridden values
for ORIG_MERGE_FILE in $MERGE_LIST ; do
	echo "Merging $ORIG_MERGE_FILE"
	if [ ! -r "$ORIG_MERGE_FILE" ]; then
		echo "The merge file '$ORIG_MERGE_FILE' does not exist.  Exit." >&2
		exit 1
	fi
	cat $ORIG_MERGE_FILE > $MERGE_FILE
	CFG_LIST=$(sed -n -e "$SED_CONFIG_EXP1" -e "$SED_CONFIG_EXP2" $MERGE_FILE)
>>>>>>> upstream/android-13

	for CFG in $CFG_LIST ; do
		grep -q -w $CFG $TMP_FILE || continue
		PREV_VAL=$(grep -w $CFG $TMP_FILE)
		NEW_VAL=$(grep -w $CFG $MERGE_FILE)
<<<<<<< HEAD
		if [ "x$PREV_VAL" != "x$NEW_VAL" ] ; then
			echo Value of $CFG is redefined by fragment $MERGE_FILE:
			echo Previous  value: $PREV_VAL
			echo New value:       $NEW_VAL
			echo
		elif [ "$WARNREDUN" = "true" ]; then
			echo Value of $CFG is redundant by fragment $MERGE_FILE:
		fi
		sed -i "/$CFG[ =]/d" $TMP_FILE
=======
		BUILTIN_FLAG=false
		if [ "$BUILTIN" = "true" ] && [ "${NEW_VAL#CONFIG_*=}" = "m" ] && [ "${PREV_VAL#CONFIG_*=}" = "y" ]; then
			echo Previous  value: $PREV_VAL
			echo New value:       $NEW_VAL
			echo -y passed, will not demote y to m
			echo
			BUILTIN_FLAG=true
		elif [ "x$PREV_VAL" != "x$NEW_VAL" ] ; then
			echo Value of $CFG is redefined by fragment $ORIG_MERGE_FILE:
			echo Previous  value: $PREV_VAL
			echo New value:       $NEW_VAL
			echo
			if [ "$STRICT" = "true" ]; then
				STRICT_MODE_VIOLATED=true
			fi
		elif [ "$WARNREDUN" = "true" ]; then
			echo Value of $CFG is redundant by fragment $ORIG_MERGE_FILE:
		fi
		if [ "$BUILTIN_FLAG" = "false" ]; then
			sed -i "/$CFG[ =]/d" $TMP_FILE
		else
			sed -i "/$CFG[ =]/d" $MERGE_FILE
		fi
>>>>>>> upstream/android-13
	done
	cat $MERGE_FILE >> $TMP_FILE
done

<<<<<<< HEAD
=======
if [ "$STRICT_MODE_VIOLATED" = "true" ]; then
	echo "The fragment redefined a value and strict mode had been passed."
	exit 1
fi

>>>>>>> upstream/android-13
if [ "$RUNMAKE" = "false" ]; then
	cp -T -- "$TMP_FILE" "$KCONFIG_CONFIG"
	echo "#"
	echo "# merged configuration written to $KCONFIG_CONFIG (needs make)"
	echo "#"
<<<<<<< HEAD
	clean_up
=======
>>>>>>> upstream/android-13
	exit
fi

# If we have an output dir, setup the O= argument, otherwise leave
# it blank, since O=. will create an unnecessary ./source softlink
OUTPUT_ARG=""
if [ "$OUTPUT" != "." ] ; then
	OUTPUT_ARG="O=$OUTPUT"
fi


# Use the merged file as the starting point for:
# alldefconfig: Fills in any missing symbols with Kconfig default
# allnoconfig: Fills in any missing symbols with # CONFIG_* is not set
make KCONFIG_ALLCONFIG=$TMP_FILE $OUTPUT_ARG $ALLTARGET


# Check all specified config values took (might have missed-dependency issues)
<<<<<<< HEAD
for CFG in $(sed -n "$SED_CONFIG_EXP" $TMP_FILE); do

	REQUESTED_VAL=$(grep -w -e "$CFG" $TMP_FILE)
	ACTUAL_VAL=$(grep -w -e "$CFG" "$KCONFIG_CONFIG")
=======
for CFG in $(sed -n -e "$SED_CONFIG_EXP1" -e "$SED_CONFIG_EXP2" $TMP_FILE); do

	REQUESTED_VAL=$(grep -w -e "$CFG" $TMP_FILE)
	ACTUAL_VAL=$(grep -w -e "$CFG" "$KCONFIG_CONFIG" || true)
>>>>>>> upstream/android-13
	if [ "x$REQUESTED_VAL" != "x$ACTUAL_VAL" ] ; then
		echo "Value requested for $CFG not in final .config"
		echo "Requested value:  $REQUESTED_VAL"
		echo "Actual value:     $ACTUAL_VAL"
		echo ""
	fi
done
<<<<<<< HEAD

clean_up
=======
>>>>>>> upstream/android-13
