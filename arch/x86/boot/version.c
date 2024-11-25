<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/* -*- linux-c -*- ------------------------------------------------------- *
 *
 *   Copyright (C) 1991, 1992 Linus Torvalds
 *   Copyright 2007 rPath, Inc. - All Rights Reserved
 *
<<<<<<< HEAD
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2.
 *
=======
>>>>>>> upstream/android-13
 * ----------------------------------------------------------------------- */

/*
 * Kernel version string
 */

#include "boot.h"
#include <generated/utsrelease.h>
#include <generated/compile.h>

const char kernel_version[] =
	UTS_RELEASE " (" LINUX_COMPILE_BY "@" LINUX_COMPILE_HOST ") "
	UTS_VERSION;
