<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* -----------------------------------------------------------------------
 *
 *   Copyright 2009 Intel Corporation; author H. Peter Anvin
 *
<<<<<<< HEAD
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2 or (at your
 *   option) any later version; incorporated herein by reference.
 *
=======
>>>>>>> upstream/android-13
 * ----------------------------------------------------------------------- */

/*
 * Simple helper function for initializing a register set.
 *
 * Note that this sets EFLAGS_CF in the input register set; this
 * makes it easier to catch functions which do nothing but don't
 * explicitly set CF.
 */

#include "boot.h"
#include "string.h"

void initregs(struct biosregs *reg)
{
<<<<<<< HEAD
	memset(reg, 0, sizeof *reg);
=======
	memset(reg, 0, sizeof(*reg));
>>>>>>> upstream/android-13
	reg->eflags |= X86_EFLAGS_CF;
	reg->ds = ds();
	reg->es = ds();
	reg->fs = fs();
	reg->gs = gs();
}
