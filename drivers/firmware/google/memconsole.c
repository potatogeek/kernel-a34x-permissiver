<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * memconsole.c
 *
 * Architecture-independent parts of the memory based BIOS console.
 *
 * Copyright 2017 Google Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2.0 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
=======
 */

>>>>>>> upstream/android-13
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/module.h>

#include "memconsole.h"

<<<<<<< HEAD
static ssize_t (*memconsole_read_func)(char *, loff_t, size_t);

=======
>>>>>>> upstream/android-13
static ssize_t memconsole_read(struct file *filp, struct kobject *kobp,
			       struct bin_attribute *bin_attr, char *buf,
			       loff_t pos, size_t count)
{
<<<<<<< HEAD
	if (WARN_ON_ONCE(!memconsole_read_func))
		return -EIO;
=======
	ssize_t (*memconsole_read_func)(char *, loff_t, size_t);

	memconsole_read_func = bin_attr->private;
	if (WARN_ON_ONCE(!memconsole_read_func))
		return -EIO;

>>>>>>> upstream/android-13
	return memconsole_read_func(buf, pos, count);
}

static struct bin_attribute memconsole_bin_attr = {
	.attr = {.name = "log", .mode = 0444},
	.read = memconsole_read,
};

void memconsole_setup(ssize_t (*read_func)(char *, loff_t, size_t))
{
<<<<<<< HEAD
	memconsole_read_func = read_func;
=======
	memconsole_bin_attr.private = read_func;
>>>>>>> upstream/android-13
}
EXPORT_SYMBOL(memconsole_setup);

int memconsole_sysfs_init(void)
{
	return sysfs_create_bin_file(firmware_kobj, &memconsole_bin_attr);
}
EXPORT_SYMBOL(memconsole_sysfs_init);

void memconsole_exit(void)
{
	sysfs_remove_bin_file(firmware_kobj, &memconsole_bin_attr);
}
EXPORT_SYMBOL(memconsole_exit);

MODULE_AUTHOR("Google, Inc.");
MODULE_LICENSE("GPL");
