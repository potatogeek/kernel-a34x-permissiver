<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * custom_method.c - debugfs interface for customizing ACPI control method
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>
#include <linux/acpi.h>
<<<<<<< HEAD

#include "internal.h"

#define _COMPONENT		ACPI_SYSTEM_COMPONENT
ACPI_MODULE_NAME("custom_method");
=======
#include <linux/security.h>

#include "internal.h"

>>>>>>> upstream/android-13
MODULE_LICENSE("GPL");

static struct dentry *cm_dentry;

/* /sys/kernel/debug/acpi/custom_method */

<<<<<<< HEAD
static ssize_t cm_write(struct file *file, const char __user * user_buf,
=======
static ssize_t cm_write(struct file *file, const char __user *user_buf,
>>>>>>> upstream/android-13
			size_t count, loff_t *ppos)
{
	static char *buf;
	static u32 max_size;
	static u32 uncopied_bytes;

	struct acpi_table_header table;
	acpi_status status;
<<<<<<< HEAD
=======
	int ret;

	ret = security_locked_down(LOCKDOWN_ACPI_TABLES);
	if (ret)
		return ret;
>>>>>>> upstream/android-13

	if (!(*ppos)) {
		/* parse the table header to get the table length */
		if (count <= sizeof(struct acpi_table_header))
			return -EINVAL;
		if (copy_from_user(&table, user_buf,
				   sizeof(struct acpi_table_header)))
			return -EFAULT;
		uncopied_bytes = max_size = table.length;
		/* make sure the buf is not allocated */
		kfree(buf);
		buf = kzalloc(max_size, GFP_KERNEL);
		if (!buf)
			return -ENOMEM;
	}

	if (buf == NULL)
		return -EINVAL;

	if ((*ppos > max_size) ||
	    (*ppos + count > max_size) ||
	    (*ppos + count < count) ||
	    (count > uncopied_bytes)) {
		kfree(buf);
		buf = NULL;
		return -EINVAL;
	}

	if (copy_from_user(buf + (*ppos), user_buf, count)) {
		kfree(buf);
		buf = NULL;
		return -EFAULT;
	}

	uncopied_bytes -= count;
	*ppos += count;

	if (!uncopied_bytes) {
		status = acpi_install_method(buf);
		kfree(buf);
		buf = NULL;
		if (ACPI_FAILURE(status))
			return -EINVAL;
		add_taint(TAINT_OVERRIDDEN_ACPI_TABLE, LOCKDEP_NOW_UNRELIABLE);
	}

	return count;
}

static const struct file_operations cm_fops = {
	.write = cm_write,
	.llseek = default_llseek,
};

static int __init acpi_custom_method_init(void)
{
<<<<<<< HEAD
	if (acpi_debugfs_dir == NULL)
		return -ENOENT;

	cm_dentry = debugfs_create_file("custom_method", S_IWUSR,
					acpi_debugfs_dir, NULL, &cm_fops);
	if (cm_dentry == NULL)
		return -ENODEV;

=======
	cm_dentry = debugfs_create_file("custom_method", S_IWUSR,
					acpi_debugfs_dir, NULL, &cm_fops);
>>>>>>> upstream/android-13
	return 0;
}

static void __exit acpi_custom_method_exit(void)
{
<<<<<<< HEAD
	if (cm_dentry)
		debugfs_remove(cm_dentry);
=======
	debugfs_remove(cm_dentry);
>>>>>>> upstream/android-13
}

module_init(acpi_custom_method_init);
module_exit(acpi_custom_method_exit);
