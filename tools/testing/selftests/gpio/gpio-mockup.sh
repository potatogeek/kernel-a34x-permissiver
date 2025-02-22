<<<<<<< HEAD
#!/bin/bash
# SPDX-License-Identifier: GPL-2.0

#exit status
#1: Internal error
#2: sysfs/debugfs not mount
#3: insert module fail when gpio-mockup is a module.
#4: Skip test including run as non-root user.
#5: other reason.

SYSFS=
GPIO_SYSFS=
GPIO_DRV_SYSFS=
DEBUGFS=
GPIO_DEBUGFS=
dev_type=
module=

# Kselftest framework requirement - SKIP code is 4.
=======
#!/bin/bash -efu
# SPDX-License-Identifier: GPL-2.0

#exit status
#0: success
#1: fail
#4: skip test - including run as non-root user

BASE=${0%/*}
DEBUGFS=
GPIO_DEBUGFS=
dev_type="cdev"
module="gpio-mockup"
verbose=
full_test=
random=
uapi_opt=
active_opt=
bias_opt=
line_set_pid=

# Kselftest return codes
ksft_fail=1
>>>>>>> upstream/android-13
ksft_skip=4

usage()
{
	echo "Usage:"
<<<<<<< HEAD
	echo "$0 [-f] [-m name] [-t type]"
	echo "-f:  full test. It maybe conflict with existence gpio device."
	echo "-m:  module name, default name is gpio-mockup. It could also test"
	echo "     other gpio device."
	echo "-t:  interface type: chardev(char device) and sysfs(being"
	echo "     deprecated). The first one is default"
	echo ""
	echo "$0 -h"
	echo "This usage"
=======
	echo "$0 [-frv] [-t type]"
	echo "-f:  full test (minimal set run by default)"
	echo "-r:  test random lines as well as fence posts"
	echo "-t:  interface type:"
	echo "      cdev (character device ABI) - default"
	echo "      cdev_v1 (deprecated character device ABI)"
	echo "      sysfs (deprecated SYSFS ABI)"
	echo "-v:  verbose progress reporting"
	exit $ksft_fail
}

skip()
{
	echo "$*" >&2
	echo "GPIO $module test SKIP"
	exit $ksft_skip
>>>>>>> upstream/android-13
}

prerequisite()
{
<<<<<<< HEAD
	msg="skip all tests:"
	if [ $UID != 0 ]; then
		echo $msg must be run as root >&2
		exit $ksft_skip
	fi
	SYSFS=`mount -t sysfs | head -1 | awk '{ print $3 }'`
	if [ ! -d "$SYSFS" ]; then
		echo $msg sysfs is not mounted >&2
		exit 2
	fi
	GPIO_SYSFS=`echo $SYSFS/class/gpio`
	GPIO_DRV_SYSFS=`echo $SYSFS/devices/platform/$module/gpio`
	DEBUGFS=`mount -t debugfs | head -1 | awk '{ print $3 }'`
	if [ ! -d "$DEBUGFS" ]; then
		echo $msg debugfs is not mounted >&2
		exit 2
	fi
	GPIO_DEBUGFS=`echo $DEBUGFS/gpio`
	source gpio-mockup-sysfs.sh
}

try_insert_module()
{
	if [ -d "$GPIO_DRV_SYSFS" ]; then
		echo "$GPIO_DRV_SYSFS exist. Skip insert module"
	else
		modprobe -q $module $1
		if [ X$? != X0 ]; then
			echo $msg insmod $module failed >&2
			exit 3
		fi
	fi
=======
	[ $(id -u) -eq 0 ] || skip "must be run as root"

	DEBUGFS=$(grep -w debugfs /proc/mounts | cut -f2 -d' ')
	[ -d "$DEBUGFS" ] || skip "debugfs is not mounted"

	GPIO_DEBUGFS=$DEBUGFS/$module
>>>>>>> upstream/android-13
}

remove_module()
{
	modprobe -r -q $module
}

<<<<<<< HEAD
die()
{
	remove_module
	exit 5
}

test_chips()
{
	if [ X$dev_type = Xsysfs ]; then
		echo "WARNING: sysfs ABI of gpio is going to deprecated."
		test_chips_sysfs $*
	else
		$BASE/gpio-mockup-chardev $*
	fi
}

gpio_test()
{
	param=$1
	valid=$2

	if [ X"$param" = X ]; then
		die
	fi
	try_insert_module "gpio_mockup_ranges=$param"
	echo -n "GPIO $module test with ranges: <"
	echo "$param>: "
	printf "%-10s %s\n" $param
	test_chips $module $valid
	remove_module
}

BASE=`dirname $0`

dev_type=
TEMP=`getopt -o fhm:t: -n '$0' -- "$@"`

if [ "$?" != "0" ]; then
        echo "Parameter process failed, Terminating..." >&2
        exit 1
fi

# Note the quotes around `$TEMP': they are essential!
eval set -- "$TEMP"

while true; do
	case $1 in
	-f)
		full_test=true
		shift
		;;
	-h)
		usage
		exit
		;;
	-m)
		module=$2
		shift 2
		;;
	-t)
		dev_type=$2
		shift 2
		;;
	--)
		shift
		break
		;;
	*)
		echo "Internal error!"
		exit 1
		;;
	esac
done

if [ X"$module" = X ]; then
	module="gpio-mockup"
fi

if [ X$dev_type != Xsysfs ]; then
	dev_type="chardev"
fi

prerequisite

echo "1.  Test dynamic allocation of gpio successful means insert gpiochip and"
echo "    manipulate gpio pin successful"
gpio_test "-1,32" true
gpio_test "-1,32,-1,32" true
gpio_test "-1,32,-1,32,-1,32" true
if [ X$full_test = Xtrue ]; then
	gpio_test "-1,32,32,64" true
	gpio_test "-1,32,40,64,-1,5" true
	gpio_test "-1,32,32,64,-1,32" true
	gpio_test "0,32,32,64,-1,32,-1,32" true
	gpio_test "-1,32,-1,32,0,32,32,64" true
	echo "2.  Do basic test: successful means insert gpiochip and"
	echo "    manipulate gpio pin successful"
	gpio_test "0,32" true
	gpio_test "0,32,32,64" true
	gpio_test "0,32,40,64,64,96" true
fi
echo "3.  Error test: successful means insert gpiochip failed"
echo "3.1 Test number of gpio overflow"
#Currently: The max number of gpio(1024) is defined in arm architecture.
gpio_test "-1,32,-1,1024" false
if [ X$full_test = Xtrue ]; then
	echo "3.2 Test zero line of gpio"
	gpio_test "0,0" false
	echo "3.3 Test range overlap"
	echo "3.3.1 Test corner case"
	gpio_test "0,32,0,1" false
	gpio_test "0,32,32,64,32,40" false
	gpio_test "0,32,35,64,35,45" false
	gpio_test "0,32,31,32" false
	gpio_test "0,32,32,64,36,37" false
	gpio_test "0,32,35,64,34,36" false
	echo "3.3.2 Test inserting invalid second gpiochip"
	gpio_test "0,32,30,35" false
	gpio_test "0,32,1,5" false
	gpio_test "10,32,9,14" false
	gpio_test "10,32,30,35" false
	echo "3.3.3 Test others"
	gpio_test "0,32,40,56,39,45" false
	gpio_test "0,32,40,56,30,33" false
	gpio_test "0,32,40,56,30,41" false
	gpio_test "0,32,40,56,20,21" false
fi

echo GPIO test PASS

=======
cleanup()
{
	set +e
	release_line
	remove_module
	jobs -p | xargs -r kill > /dev/null 2>&1
}

fail()
{
	echo "test failed: $*" >&2
	echo "GPIO $module test FAIL"
	exit $ksft_fail
}

try_insert_module()
{
	modprobe -q $module "$1" || fail "insert $module failed with error $?"
}

log()
{
	[ -z "$verbose" ] || echo "$*"
}

# The following line helpers, release_Line, get_line and set_line, all
# make use of the global $chip and $offset variables.
#
# This implementation drives the GPIO character device (cdev) uAPI.
# Other implementations may override these to test different uAPIs.

# Release any resources related to the line
release_line()
{
	[ "$line_set_pid" ] && kill $line_set_pid && wait $line_set_pid || true
	line_set_pid=
}

# Read the current value of the line
get_line()
{
	release_line

	local cdev_opts=${uapi_opt}${active_opt}
	$BASE/gpio-mockup-cdev $cdev_opts /dev/$chip $offset
	echo $?
}

# Set the state of the line
#
# Changes to line configuration are provided as parameters.
# The line is assumed to be an output if the line value 0 or 1 is
# specified, else an input.
set_line()
{
	local val=

	release_line

	# parse config options...
	for option in $*; do
		case $option in
		active-low)
			active_opt="-l "
			;;
		active-high)
			active_opt=
			;;
		bias-none)
			bias_opt=
			;;
		pull-down)
			bias_opt="-bpull-down "
			;;
		pull-up)
			bias_opt="-bpull-up "
			;;
		0)
			val=0
			;;
		1)
			val=1
			;;
		esac
	done

	local cdev_opts=${uapi_opt}${active_opt}
	if [ "$val" ]; then
		$BASE/gpio-mockup-cdev $cdev_opts -s$val /dev/$chip $offset &
		# failure to set is detected by reading mockup and toggling values
		line_set_pid=$!
		# allow for gpio-mockup-cdev to launch and request line
		# (there is limited value in checking if line has been requested)
		sleep 0.01
	elif [ "$bias_opt" ]; then
		cdev_opts=${cdev_opts}${bias_opt}
		$BASE/gpio-mockup-cdev $cdev_opts /dev/$chip $offset || true
	fi
}

assert_line()
{
	local val
	# don't need any retry here as set_mock allows for propagation
	val=$(get_line)
	[ "$val" = "$1" ] || fail "line value is ${val:-empty} when $1 was expected"
}

# The following mockup helpers all make use of the $mock_line
assert_mock()
{
	local backoff_wait=10
	local retry=0
	local val
	# retry allows for set propagation from uAPI to mockup
	while true; do
		val=$(< $mock_line)
		[ "$val" = "$1" ] && break
		retry=$((retry + 1))
		[ $retry -lt 5 ] || fail "mockup $mock_line value ${val:-empty} when $1 expected"
		sleep $(printf "%0.2f" $((backoff_wait))e-3)
		backoff_wait=$((backoff_wait * 2))
	done
}

set_mock()
{
	echo "$1" > $mock_line
	# allow for set propagation - so we won't be in a race with set_line
	assert_mock "$1"
}

# test the functionality of a line
#
# The line is set from the mockup side and is read from the userspace side
# (input), and is set from the userspace side and is read from the mockup side
# (output).
#
# Setting the mockup pull using the userspace interface bias settings is
# tested where supported by the userspace interface (cdev).
test_line()
{
	chip=$1
	offset=$2
	log "test_line $chip $offset"
	mock_line=$GPIO_DEBUGFS/$chip/$offset
	[ -e "$mock_line" ] || fail "missing line $chip:$offset"

	# test input active-high
	set_mock 1
	set_line input active-high
	assert_line 1
	set_mock 0
	assert_line 0
	set_mock 1
	assert_line 1

	if [ "$full_test" ]; then
		if [ "$dev_type" != "sysfs" ]; then
			# test pulls
			set_mock 0
			set_line input pull-up
			assert_line 1
			set_mock 0
			assert_line 0

			set_mock 1
			set_line input pull-down
			assert_line 0
			set_mock 1
			assert_line 1

			set_line bias-none
		fi

		# test input active-low
		set_mock 0
		set_line active-low
		assert_line 1
		set_mock 1
		assert_line 0
		set_mock 0
		assert_line 1

		# test output active-high
		set_mock 1
		set_line active-high 0
		assert_mock 0
		set_line 1
		assert_mock 1
		set_line 0
		assert_mock 0
	fi

	# test output active-low
	set_mock 0
	set_line active-low 0
	assert_mock 1
	set_line 1
	assert_mock 0
	set_line 0
	assert_mock 1

	release_line
}

test_no_line()
{
	log test_no_line "$*"
	[ ! -e "$GPIO_DEBUGFS/$1/$2" ] || fail "unexpected line $1:$2"
}

# Load the module and check that the expected number of gpiochips, with the
# expected number of lines, are created and are functional.
#
# $1 is the gpio_mockup_ranges parameter for the module
# The remaining parameters are the number of lines, n, expected for each of
# the gpiochips expected to be created.
#
# For each gpiochip the fence post lines, 0 and n-1, are tested, and the
# line on the far side of the fence post, n, is tested to not exist.
#
# If the $random flag is set then a random line in the middle of the
# gpiochip is tested as well.
insmod_test()
{
	local ranges=
	local gc=
	local width=

	[ "${1:-}" ] || fail "missing ranges"
	ranges=$1 ; shift
	try_insert_module "gpio_mockup_ranges=$ranges"
	log "GPIO $module test with ranges: <$ranges>:"
	# e.g. /sys/kernel/debug/gpio-mockup/gpiochip1
	gpiochip=$(find "$DEBUGFS/$module/" -name gpiochip* -type d | sort)
	for chip in $gpiochip; do
		gc=${chip##*/}
		[ "${1:-}" ] || fail "unexpected chip - $gc"
		width=$1 ; shift
		test_line $gc 0
		if [ "$random" -a $width -gt 2 ]; then
			test_line $gc $((RANDOM % ($width - 2) + 1))
		fi
		test_line $gc $(($width - 1))
		test_no_line $gc $width
	done
	[ "${1:-}" ] && fail "missing expected chip of width $1"
	remove_module || fail "failed to remove module with error $?"
}

while getopts ":frvt:" opt; do
	case $opt in
	f)
		full_test=true
		;;
	r)
		random=true
		;;
	t)
		dev_type=$OPTARG
		;;
	v)
		verbose=true
		;;
	*)
		usage
		;;
	esac
done
shift $((OPTIND - 1))

[ "${1:-}" ] && fail "unknown argument '$1'"

prerequisite

trap 'exit $ksft_fail' SIGTERM SIGINT
trap cleanup EXIT

case "$dev_type" in
sysfs)
	source $BASE/gpio-mockup-sysfs.sh
	echo "WARNING: gpio sysfs ABI is deprecated."
	;;
cdev_v1)
	echo "WARNING: gpio cdev ABI v1 is deprecated."
	uapi_opt="-u1 "
	;;
cdev)
	;;
*)
	fail "unknown interface type: $dev_type"
	;;
esac

remove_module || fail "can't remove existing $module module"

# manual gpio allocation tests fail if a physical chip already exists
[ "$full_test" -a -e "/dev/gpiochip0" ] && skip "full tests conflict with gpiochip0"

echo "1.  Module load tests"
echo "1.1.  dynamic allocation of gpio"
insmod_test "-1,32" 32
insmod_test "-1,23,-1,32" 23 32
insmod_test "-1,23,-1,26,-1,32" 23 26 32
if [ "$full_test" ]; then
	echo "1.2.  manual allocation of gpio"
	insmod_test "0,32" 32
	insmod_test "0,32,32,60" 32 28
	insmod_test "0,32,40,64,64,96" 32 24 32
	echo "1.3.  dynamic and manual allocation of gpio"
	insmod_test "-1,32,32,62" 32 30
	insmod_test "-1,22,-1,23,0,24,32,64" 22 23 24 32
	insmod_test "-1,32,32,60,-1,29" 32 28 29
	insmod_test "-1,32,40,64,-1,5" 32 24 5
	insmod_test "0,32,32,44,-1,22,-1,31" 32 12 22 31
fi
echo "2.  Module load error tests"
echo "2.1 gpio overflow"
# Currently: The max number of gpio(1024) is defined in arm architecture.
insmod_test "-1,1024"
if [ "$full_test" ]; then
	echo "2.2 no lines defined"
	insmod_test "0,0"
	echo "2.3 ignore range overlap"
	insmod_test "0,32,0,1" 32
	insmod_test "0,32,1,5" 32
	insmod_test "0,32,30,35" 32
	insmod_test "0,32,31,32" 32
	insmod_test "10,32,30,35" 22
	insmod_test "10,32,9,14" 22
	insmod_test "0,32,20,21,40,56" 32 16
	insmod_test "0,32,32,64,32,40" 32 32
	insmod_test "0,32,32,64,36,37" 32 32
	insmod_test "0,32,35,64,34,36" 32 29
	insmod_test "0,30,35,64,35,45" 30 29
	insmod_test "0,32,40,56,30,33" 32 16
	insmod_test "0,32,40,56,30,41" 32 16
	insmod_test "0,32,40,56,39,45" 32 16
fi

echo "GPIO $module test PASS"
>>>>>>> upstream/android-13
