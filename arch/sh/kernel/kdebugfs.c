// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/debugfs.h>

struct dentry *arch_debugfs_dir;
EXPORT_SYMBOL(arch_debugfs_dir);

static int __init arch_kdebugfs_init(void)
{
	arch_debugfs_dir = debugfs_create_dir("sh", NULL);
<<<<<<< HEAD
	if (!arch_debugfs_dir)
		return -ENOMEM;

=======
>>>>>>> upstream/android-13
	return 0;
}
arch_initcall(arch_kdebugfs_init);
