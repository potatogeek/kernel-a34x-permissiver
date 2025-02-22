<<<<<<< HEAD
/*
 * Copyright 2014  Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright 2014  Google, Inc.
>>>>>>> upstream/android-13
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "internal.h"
<<<<<<< HEAD
#ifdef CONFIG_SEC_EXT
#include <linux/sec_ext.h>
#endif
=======
>>>>>>> upstream/android-13

static DEFINE_MUTEX(pmsg_lock);

static ssize_t write_pmsg(struct file *file, const char __user *buf,
			  size_t count, loff_t *ppos)
{
	struct pstore_record record;
	int ret;
<<<<<<< HEAD
#ifdef CONFIG_SEC_EXT
	char sec_buf[256];
	size_t sec_count = 0;
#endif
=======
>>>>>>> upstream/android-13

	if (!count)
		return 0;

	pstore_record_init(&record, psinfo);
	record.type = PSTORE_TYPE_PMSG;
	record.size = count;

	/* check outside lock, page in any data. write_user also checks */
<<<<<<< HEAD
	if (!access_ok(VERIFY_READ, buf, count))
		return -EFAULT;

	mutex_lock(&pmsg_lock);
#ifdef CONFIG_SEC_EXT
	if (count > 256)
		sec_count = 256;
	else
		sec_count = count;

	__copy_from_user(sec_buf, buf, sec_count);
#endif /* CONFIG_SEC_EXT */
=======
	if (!access_ok(buf, count))
		return -EFAULT;

	mutex_lock(&pmsg_lock);
>>>>>>> upstream/android-13
	ret = psinfo->write_user(&record, buf);
	mutex_unlock(&pmsg_lock);
	return ret ? ret : count;
}

static const struct file_operations pmsg_fops = {
	.owner		= THIS_MODULE,
	.llseek		= noop_llseek,
	.write		= write_pmsg,
};

static struct class *pmsg_class;
static int pmsg_major;
#define PMSG_NAME "pmsg"
#undef pr_fmt
#define pr_fmt(fmt) PMSG_NAME ": " fmt

static char *pmsg_devnode(struct device *dev, umode_t *mode)
{
	if (mode)
		*mode = 0220;
	return NULL;
}

void pstore_register_pmsg(void)
{
	struct device *pmsg_device;

	pmsg_major = register_chrdev(0, PMSG_NAME, &pmsg_fops);
	if (pmsg_major < 0) {
		pr_err("register_chrdev failed\n");
		goto err;
	}

	pmsg_class = class_create(THIS_MODULE, PMSG_NAME);
	if (IS_ERR(pmsg_class)) {
		pr_err("device class file already in use\n");
		goto err_class;
	}
	pmsg_class->devnode = pmsg_devnode;

	pmsg_device = device_create(pmsg_class, NULL, MKDEV(pmsg_major, 0),
					NULL, "%s%d", PMSG_NAME, 0);
	if (IS_ERR(pmsg_device)) {
		pr_err("failed to create device\n");
		goto err_device;
	}
	return;

err_device:
	class_destroy(pmsg_class);
err_class:
	unregister_chrdev(pmsg_major, PMSG_NAME);
err:
	return;
}

void pstore_unregister_pmsg(void)
{
	device_destroy(pmsg_class, MKDEV(pmsg_major, 0));
	class_destroy(pmsg_class);
	unregister_chrdev(pmsg_major, PMSG_NAME);
}
