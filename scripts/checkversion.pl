#! /usr/bin/env perl
# SPDX-License-Identifier: GPL-2.0
#
<<<<<<< HEAD
# checkversion find uses of LINUX_VERSION_CODE or KERNEL_VERSION
# without including <linux/version.h>, or cases of
# including <linux/version.h> that don't need it.
# Copyright (C) 2003, Randy Dunlap <rdunlap@xenotime.net>
=======
# checkversion finds uses of all macros in <linux/version.h>
# where the source files do not #include <linux/version.h>; or cases
# of including <linux/version.h> where it is not needed.
# Copyright (C) 2003, Randy Dunlap <rdunlap@infradead.org>
>>>>>>> upstream/android-13

use strict;

$| = 1;

my $debugging;

foreach my $file (@ARGV) {
<<<<<<< HEAD
    next if $file =~ "include/linux/version\.h";
=======
    next if $file =~ "include/generated/uapi/linux/version\.h";
    next if $file =~ "usr/include/linux/version\.h";
>>>>>>> upstream/android-13
    # Open this file.
    open( my $f, '<', $file )
      or die "Can't open $file: $!\n";

    # Initialize variables.
    my ($fInComment, $fInString, $fUseVersion);
    my $iLinuxVersion = 0;

    while (<$f>) {
	# Strip comments.
	$fInComment && (s+^.*?\*/+ +o ? ($fInComment = 0) : next);
	m+/\*+o && (s+/\*.*?\*/+ +go, (s+/\*.*$+ +o && ($fInComment = 1)));

	# Pick up definitions.
	if ( m/^\s*#/o ) {
	    $iLinuxVersion      = $. if m/^\s*#\s*include\s*"linux\/version\.h"/o;
	}

	# Strip strings.
	$fInString && (s+^.*?"+ +o ? ($fInString = 0) : next);
	m+"+o && (s+".*?"+ +go, (s+".*$+ +o && ($fInString = 1)));

	# Pick up definitions.
	if ( m/^\s*#/o ) {
	    $iLinuxVersion      = $. if m/^\s*#\s*include\s*<linux\/version\.h>/o;
	}

<<<<<<< HEAD
	# Look for uses: LINUX_VERSION_CODE, KERNEL_VERSION, UTS_RELEASE
	if (($_ =~ /LINUX_VERSION_CODE/) || ($_ =~ /\WKERNEL_VERSION/)) {
=======
	# Look for uses: LINUX_VERSION_CODE, KERNEL_VERSION,
	# LINUX_VERSION_MAJOR, LINUX_VERSION_PATCHLEVEL, LINUX_VERSION_SUBLEVEL
	if (($_ =~ /LINUX_VERSION_CODE/) || ($_ =~ /\WKERNEL_VERSION/) ||
	    ($_ =~ /LINUX_VERSION_MAJOR/) || ($_ =~ /LINUX_VERSION_PATCHLEVEL/) ||
	    ($_ =~ /LINUX_VERSION_SUBLEVEL/)) {
>>>>>>> upstream/android-13
	    $fUseVersion = 1;
            last if $iLinuxVersion;
        }
    }

    # Report used version IDs without include?
    if ($fUseVersion && ! $iLinuxVersion) {
	print "$file: $.: need linux/version.h\n";
    }

    # Report superfluous includes.
    if ($iLinuxVersion && ! $fUseVersion) {
	print "$file: $iLinuxVersion linux/version.h not needed.\n";
    }

    # debug: report OK results:
    if ($debugging) {
        if ($iLinuxVersion && $fUseVersion) {
	    print "$file: version use is OK ($iLinuxVersion)\n";
        }
        if (! $iLinuxVersion && ! $fUseVersion) {
	    print "$file: version use is OK (none)\n";
        }
    }

    close($f);
}
