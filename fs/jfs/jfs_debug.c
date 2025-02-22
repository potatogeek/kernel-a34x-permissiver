<<<<<<< HEAD
/*
 *   Copyright (C) International Business Machines Corp., 2000-2004
 *   Portions Copyright (C) Christoph Hellwig, 2001-2002
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *   Copyright (C) International Business Machines Corp., 2000-2004
 *   Portions Copyright (C) Christoph Hellwig, 2001-2002
>>>>>>> upstream/android-13
 */

#include <linux/fs.h>
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include "jfs_incore.h"
#include "jfs_filsys.h"
#include "jfs_debug.h"

#ifdef PROC_FS_JFS /* see jfs_debug.h */

#ifdef CONFIG_JFS_DEBUG
static int jfs_loglevel_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", jfsloglevel);
	return 0;
}

static int jfs_loglevel_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, jfs_loglevel_proc_show, NULL);
}

static ssize_t jfs_loglevel_proc_write(struct file *file,
		const char __user *buffer, size_t count, loff_t *ppos)
{
	char c;

	if (get_user(c, buffer))
		return -EFAULT;

	/* yes, I know this is an ASCIIism.  --hch */
	if (c < '0' || c > '9')
		return -EINVAL;
	jfsloglevel = c - '0';
	return count;
}

<<<<<<< HEAD
static const struct file_operations jfs_loglevel_proc_fops = {
	.open		= jfs_loglevel_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= jfs_loglevel_proc_write,
=======
static const struct proc_ops jfs_loglevel_proc_ops = {
	.proc_open	= jfs_loglevel_proc_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
	.proc_write	= jfs_loglevel_proc_write,
>>>>>>> upstream/android-13
};
#endif

void jfs_proc_init(void)
{
	struct proc_dir_entry *base;

	base = proc_mkdir("fs/jfs", NULL);
	if (!base)
		return;

#ifdef CONFIG_JFS_STATISTICS
	proc_create_single("lmstats", 0, base, jfs_lmstats_proc_show);
	proc_create_single("txstats", 0, base, jfs_txstats_proc_show);
	proc_create_single("xtstat", 0, base, jfs_xtstat_proc_show);
	proc_create_single("mpstat", 0, base, jfs_mpstat_proc_show);
#endif
#ifdef CONFIG_JFS_DEBUG
	proc_create_single("TxAnchor", 0, base, jfs_txanchor_proc_show);
<<<<<<< HEAD
	proc_create("loglevel", 0, base, &jfs_loglevel_proc_fops);
=======
	proc_create("loglevel", 0, base, &jfs_loglevel_proc_ops);
>>>>>>> upstream/android-13
#endif
}

void jfs_proc_clean(void)
{
	remove_proc_subtree("fs/jfs", NULL);
}

#endif /* PROC_FS_JFS */
