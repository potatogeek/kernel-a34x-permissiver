<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* no-block.c: implementation of routines required for non-BLOCK configuration
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> upstream/android-13
 */

#include <linux/kernel.h>
#include <linux/fs.h>

static int no_blkdev_open(struct inode * inode, struct file * filp)
{
	return -ENODEV;
}

const struct file_operations def_blk_fops = {
	.open		= no_blkdev_open,
	.llseek		= noop_llseek,
};
