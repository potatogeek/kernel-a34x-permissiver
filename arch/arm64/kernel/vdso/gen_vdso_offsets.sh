#!/bin/sh
# SPDX-License-Identifier: GPL-2.0

#
# Match symbols in the DSO that look like VDSO_*; produce a header file
# of constant offsets into the shared object.
#
# Doing this inside the Makefile will break the $(filter-out) function,
# causing Kbuild to rebuild the vdso-offsets header file every time.
#
<<<<<<< HEAD
# Author: Will Deacon <will.deacon@arm.com
=======
# Author: Will Deacon <will.deacon@arm.com>
>>>>>>> upstream/android-13
#

LC_ALL=C
sed -n -e 's/^00*/0/' -e \
<<<<<<< HEAD
's/^\([0-9a-fA-F]*\) . VDSO_\([a-zA-Z0-9_]*\)$/\#define vdso_offset_\2\t0x\1/p'
=======
's/^\([0-9a-fA-F]*\) . VDSO_\([a-zA-Z0-9_]*\)$/\#define vdso_offset_\2 0x\1/p'
>>>>>>> upstream/android-13
