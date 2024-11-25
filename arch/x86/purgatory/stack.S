<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> upstream/android-13
/*
 * purgatory:  stack
 *
 * Copyright (C) 2014 Red Hat Inc.
<<<<<<< HEAD
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 */

=======
 */

#include <linux/linkage.h>

>>>>>>> upstream/android-13
	/* A stack for the loaded kernel.
	 * Separate and in the data section so it can be prepopulated.
	 */
	.data
	.balign 4096
<<<<<<< HEAD
	.globl stack, stack_end

stack:
	.skip 4096
stack_end:
=======

SYM_DATA_START(stack)
	.skip 4096
SYM_DATA_END_LABEL(stack, SYM_L_GLOBAL, stack_end)
>>>>>>> upstream/android-13
