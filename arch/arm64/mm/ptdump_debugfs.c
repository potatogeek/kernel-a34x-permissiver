// SPDX-License-Identifier: GPL-2.0
#include <linux/debugfs.h>
<<<<<<< HEAD
=======
#include <linux/memory_hotplug.h>
>>>>>>> upstream/android-13
#include <linux/seq_file.h>

#include <asm/ptdump.h>

static int ptdump_show(struct seq_file *m, void *v)
{
	struct ptdump_info *info = m->private;
<<<<<<< HEAD
	ptdump_walk_pgd(m, info);
=======

	get_online_mems();
	ptdump_walk(m, info);
	put_online_mems();
>>>>>>> upstream/android-13
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(ptdump);

<<<<<<< HEAD
int ptdump_debugfs_register(struct ptdump_info *info, const char *name)
{
	struct dentry *pe;
	pe = debugfs_create_file(name, 0400, NULL, info, &ptdump_fops);
	return pe ? 0 : -ENOMEM;

=======
void __init ptdump_debugfs_register(struct ptdump_info *info, const char *name)
{
	debugfs_create_file(name, 0400, NULL, info, &ptdump_fops);
>>>>>>> upstream/android-13
}
