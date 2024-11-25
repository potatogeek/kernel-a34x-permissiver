<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * init/noinitramfs.c
 *
 * Copyright (C) 2006, NXP Semiconductors, All Rights Reserved
 * Author: Jean-Paul Saman <jean-paul.saman@nxp.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
>>>>>>> upstream/android-13
 */
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/kdev_t.h>
#include <linux/syscalls.h>
<<<<<<< HEAD
=======
#include <linux/init_syscalls.h>
#include <linux/umh.h>
>>>>>>> upstream/android-13

/*
 * Create a simple rootfs that is similar to the default initramfs
 */
static int __init default_rootfs(void)
{
	int err;

<<<<<<< HEAD
	err = ksys_mkdir((const char __user __force *) "/dev", 0755);
	if (err < 0)
		goto out;

	err = ksys_mknod((const char __user __force *) "/dev/console",
			S_IFCHR | S_IRUSR | S_IWUSR,
=======
	usermodehelper_enable();
	err = init_mkdir("/dev", 0755);
	if (err < 0)
		goto out;

	err = init_mknod("/dev/console", S_IFCHR | S_IRUSR | S_IWUSR,
>>>>>>> upstream/android-13
			new_encode_dev(MKDEV(5, 1)));
	if (err < 0)
		goto out;

<<<<<<< HEAD
	err = ksys_mkdir((const char __user __force *) "/root", 0700);
=======
	err = init_mkdir("/root", 0700);
>>>>>>> upstream/android-13
	if (err < 0)
		goto out;

	return 0;

out:
	printk(KERN_WARNING "Failed to create a rootfs\n");
	return err;
}
rootfs_initcall(default_rootfs);
