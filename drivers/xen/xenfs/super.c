<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  xenfs.c - a filesystem for passing info between the a domain and
 *  the hypervisor.
 *
 * 2008-10-07  Alex Zeffertt    Replaced /proc/xen/xenbus with xenfs filesystem
 *                              and /proc/xen compatibility mount point.
 *                              Turned xenfs into a loadable module.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/fs.h>
<<<<<<< HEAD
=======
#include <linux/fs_context.h>
>>>>>>> upstream/android-13
#include <linux/magic.h>

#include <xen/xen.h>
#include <xen/xenbus.h>

#include "xenfs.h"
#include "../privcmd.h"

#include <asm/xen/hypervisor.h>

MODULE_DESCRIPTION("Xen filesystem");
MODULE_LICENSE("GPL");

static ssize_t capabilities_read(struct file *file, char __user *buf,
				 size_t size, loff_t *off)
{
	char *tmp = "";

	if (xen_initial_domain())
		tmp = "control_d\n";

	return simple_read_from_buffer(buf, size, off, tmp, strlen(tmp));
}

static const struct file_operations capabilities_file_ops = {
	.read = capabilities_read,
	.llseek = default_llseek,
};

<<<<<<< HEAD
static int xenfs_fill_super(struct super_block *sb, void *data, int silent)
=======
static int xenfs_fill_super(struct super_block *sb, struct fs_context *fc)
>>>>>>> upstream/android-13
{
	static const struct tree_descr xenfs_files[] = {
		[2] = { "xenbus", &xen_xenbus_fops, S_IRUSR|S_IWUSR },
		{ "capabilities", &capabilities_file_ops, S_IRUGO },
		{ "privcmd", &xen_privcmd_fops, S_IRUSR|S_IWUSR },
		{""},
	};

	static const struct tree_descr xenfs_init_files[] = {
		[2] = { "xenbus", &xen_xenbus_fops, S_IRUSR|S_IWUSR },
		{ "capabilities", &capabilities_file_ops, S_IRUGO },
		{ "privcmd", &xen_privcmd_fops, S_IRUSR|S_IWUSR },
		{ "xsd_kva", &xsd_kva_file_ops, S_IRUSR|S_IWUSR},
		{ "xsd_port", &xsd_port_file_ops, S_IRUSR|S_IWUSR},
#ifdef CONFIG_XEN_SYMS
		{ "xensyms", &xensyms_ops, S_IRUSR},
#endif
		{""},
	};

	return simple_fill_super(sb, XENFS_SUPER_MAGIC,
			xen_initial_domain() ? xenfs_init_files : xenfs_files);
}

<<<<<<< HEAD
static struct dentry *xenfs_mount(struct file_system_type *fs_type,
				  int flags, const char *dev_name,
				  void *data)
{
	return mount_single(fs_type, flags, data, xenfs_fill_super);
=======
static int xenfs_get_tree(struct fs_context *fc)
{
	return get_tree_single(fc, xenfs_fill_super);
}

static const struct fs_context_operations xenfs_context_ops = {
	.get_tree	= xenfs_get_tree,
};

static int xenfs_init_fs_context(struct fs_context *fc)
{
	fc->ops = &xenfs_context_ops;
	return 0;
>>>>>>> upstream/android-13
}

static struct file_system_type xenfs_type = {
	.owner =	THIS_MODULE,
	.name =		"xenfs",
<<<<<<< HEAD
	.mount =	xenfs_mount,
=======
	.init_fs_context = xenfs_init_fs_context,
>>>>>>> upstream/android-13
	.kill_sb =	kill_litter_super,
};
MODULE_ALIAS_FS("xenfs");

static int __init xenfs_init(void)
{
	if (xen_domain())
		return register_filesystem(&xenfs_type);

	return 0;
}

static void __exit xenfs_exit(void)
{
	if (xen_domain())
		unregister_filesystem(&xenfs_type);
}

module_init(xenfs_init);
module_exit(xenfs_exit);

