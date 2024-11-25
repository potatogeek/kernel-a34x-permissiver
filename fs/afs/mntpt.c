<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> upstream/android-13
/* mountpoint management
 *
 * Copyright (C) 2002 Red Hat, Inc. All Rights Reserved.
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
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pagemap.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/gfp.h>
<<<<<<< HEAD
=======
#include <linux/fs_context.h>
>>>>>>> upstream/android-13
#include "internal.h"


static struct dentry *afs_mntpt_lookup(struct inode *dir,
				       struct dentry *dentry,
				       unsigned int flags);
static int afs_mntpt_open(struct inode *inode, struct file *file);
static void afs_mntpt_expiry_timed_out(struct work_struct *work);

const struct file_operations afs_mntpt_file_operations = {
	.open		= afs_mntpt_open,
	.llseek		= noop_llseek,
};

const struct inode_operations afs_mntpt_inode_operations = {
	.lookup		= afs_mntpt_lookup,
	.readlink	= page_readlink,
	.getattr	= afs_getattr,
<<<<<<< HEAD
	.listxattr	= afs_listxattr,
=======
>>>>>>> upstream/android-13
};

const struct inode_operations afs_autocell_inode_operations = {
	.getattr	= afs_getattr,
};

static LIST_HEAD(afs_vfsmounts);
static DECLARE_DELAYED_WORK(afs_mntpt_expiry_timer, afs_mntpt_expiry_timed_out);

static unsigned long afs_mntpt_expiry_timeout = 10 * 60;

<<<<<<< HEAD
=======
static const char afs_root_volume[] = "root.cell";

>>>>>>> upstream/android-13
/*
 * no valid lookup procedure on this sort of dir
 */
static struct dentry *afs_mntpt_lookup(struct inode *dir,
				       struct dentry *dentry,
				       unsigned int flags)
{
	_enter("%p,%p{%pd2}", dir, dentry, dentry);
	return ERR_PTR(-EREMOTE);
}

/*
 * no valid open procedure on this sort of dir
 */
static int afs_mntpt_open(struct inode *inode, struct file *file)
{
	_enter("%p,%p{%pD2}", inode, file, file);
	return -EREMOTE;
}

/*
<<<<<<< HEAD
=======
 * Set the parameters for the proposed superblock.
 */
static int afs_mntpt_set_params(struct fs_context *fc, struct dentry *mntpt)
{
	struct afs_fs_context *ctx = fc->fs_private;
	struct afs_super_info *src_as = AFS_FS_S(mntpt->d_sb);
	struct afs_vnode *vnode = AFS_FS_I(d_inode(mntpt));
	struct afs_cell *cell;
	const char *p;
	int ret;

	if (fc->net_ns != src_as->net_ns) {
		put_net(fc->net_ns);
		fc->net_ns = get_net(src_as->net_ns);
	}

	if (src_as->volume && src_as->volume->type == AFSVL_RWVOL) {
		ctx->type = AFSVL_RWVOL;
		ctx->force = true;
	}
	if (ctx->cell) {
		afs_unuse_cell(ctx->net, ctx->cell, afs_cell_trace_unuse_mntpt);
		ctx->cell = NULL;
	}
	if (test_bit(AFS_VNODE_PSEUDODIR, &vnode->flags)) {
		/* if the directory is a pseudo directory, use the d_name */
		unsigned size = mntpt->d_name.len;

		if (size < 2)
			return -ENOENT;

		p = mntpt->d_name.name;
		if (mntpt->d_name.name[0] == '.') {
			size--;
			p++;
			ctx->type = AFSVL_RWVOL;
			ctx->force = true;
		}
		if (size > AFS_MAXCELLNAME)
			return -ENAMETOOLONG;

		cell = afs_lookup_cell(ctx->net, p, size, NULL, false);
		if (IS_ERR(cell)) {
			pr_err("kAFS: unable to lookup cell '%pd'\n", mntpt);
			return PTR_ERR(cell);
		}
		ctx->cell = cell;

		ctx->volname = afs_root_volume;
		ctx->volnamesz = sizeof(afs_root_volume) - 1;
	} else {
		/* read the contents of the AFS special symlink */
		struct page *page;
		loff_t size = i_size_read(d_inode(mntpt));
		char *buf;

		if (src_as->cell)
			ctx->cell = afs_use_cell(src_as->cell, afs_cell_trace_use_mntpt);

		if (size < 2 || size > PAGE_SIZE - 1)
			return -EINVAL;

		page = read_mapping_page(d_inode(mntpt)->i_mapping, 0, NULL);
		if (IS_ERR(page))
			return PTR_ERR(page);

		if (PageError(page)) {
			ret = afs_bad(AFS_FS_I(d_inode(mntpt)), afs_file_error_mntpt);
			put_page(page);
			return ret;
		}

		buf = kmap(page);
		ret = -EINVAL;
		if (buf[size - 1] == '.')
			ret = vfs_parse_fs_string(fc, "source", buf, size - 1);
		kunmap(page);
		put_page(page);
		if (ret < 0)
			return ret;
	}

	return 0;
}

/*
>>>>>>> upstream/android-13
 * create a vfsmount to be automounted
 */
static struct vfsmount *afs_mntpt_do_automount(struct dentry *mntpt)
{
<<<<<<< HEAD
	struct afs_super_info *as;
	struct vfsmount *mnt;
	struct afs_vnode *vnode;
	struct page *page;
	char *devname, *options;
	bool rwpath = false;
	int ret;

	_enter("{%pd}", mntpt);

	BUG_ON(!d_inode(mntpt));

	ret = -ENOMEM;
	devname = (char *) get_zeroed_page(GFP_KERNEL);
	if (!devname)
		goto error_no_devname;

	options = (char *) get_zeroed_page(GFP_KERNEL);
	if (!options)
		goto error_no_options;

	vnode = AFS_FS_I(d_inode(mntpt));
	if (test_bit(AFS_VNODE_PSEUDODIR, &vnode->flags)) {
		/* if the directory is a pseudo directory, use the d_name */
		static const char afs_root_cell[] = ":root.cell.";
		unsigned size = mntpt->d_name.len;

		ret = -ENOENT;
		if (size < 2 || size > AFS_MAXCELLNAME)
			goto error_no_page;

		if (mntpt->d_name.name[0] == '.') {
			devname[0] = '%';
			memcpy(devname + 1, mntpt->d_name.name + 1, size - 1);
			memcpy(devname + size, afs_root_cell,
			       sizeof(afs_root_cell));
			rwpath = true;
		} else {
			devname[0] = '#';
			memcpy(devname + 1, mntpt->d_name.name, size);
			memcpy(devname + size + 1, afs_root_cell,
			       sizeof(afs_root_cell));
		}
	} else {
		/* read the contents of the AFS special symlink */
		loff_t size = i_size_read(d_inode(mntpt));
		char *buf;

		ret = -EINVAL;
		if (size > PAGE_SIZE - 1)
			goto error_no_page;

		page = read_mapping_page(d_inode(mntpt)->i_mapping, 0, NULL);
		if (IS_ERR(page)) {
			ret = PTR_ERR(page);
			goto error_no_page;
		}

		ret = -EIO;
		if (PageError(page))
			goto error;

		buf = kmap_atomic(page);
		memcpy(devname, buf, size);
		kunmap_atomic(buf);
		put_page(page);
		page = NULL;
	}

	/* work out what options we want */
	as = AFS_FS_S(mntpt->d_sb);
	if (as->cell) {
		memcpy(options, "cell=", 5);
		strcpy(options + 5, as->cell->name);
		if ((as->volume && as->volume->type == AFSVL_RWVOL) || rwpath)
			strcat(options, ",rwpath");
	}

	/* try and do the mount */
	_debug("--- attempting mount %s -o %s ---", devname, options);
	mnt = vfs_submount(mntpt, &afs_fs_type, devname, options);
	_debug("--- mount result %p ---", mnt);

	free_page((unsigned long) devname);
	free_page((unsigned long) options);
	_leave(" = %p", mnt);
	return mnt;

error:
	put_page(page);
error_no_page:
	free_page((unsigned long) options);
error_no_options:
	free_page((unsigned long) devname);
error_no_devname:
	_leave(" = %d", ret);
	return ERR_PTR(ret);
=======
	struct fs_context *fc;
	struct vfsmount *mnt;
	int ret;

	BUG_ON(!d_inode(mntpt));

	fc = fs_context_for_submount(&afs_fs_type, mntpt);
	if (IS_ERR(fc))
		return ERR_CAST(fc);

	ret = afs_mntpt_set_params(fc, mntpt);
	if (!ret)
		mnt = fc_mount(fc);
	else
		mnt = ERR_PTR(ret);

	put_fs_context(fc);
	return mnt;
>>>>>>> upstream/android-13
}

/*
 * handle an automount point
 */
struct vfsmount *afs_d_automount(struct path *path)
{
	struct vfsmount *newmnt;

	_enter("{%pd}", path->dentry);

	newmnt = afs_mntpt_do_automount(path->dentry);
	if (IS_ERR(newmnt))
		return newmnt;

	mntget(newmnt); /* prevent immediate expiration */
	mnt_set_expiry(newmnt, &afs_vfsmounts);
	queue_delayed_work(afs_wq, &afs_mntpt_expiry_timer,
			   afs_mntpt_expiry_timeout * HZ);
	_leave(" = %p", newmnt);
	return newmnt;
}

/*
 * handle mountpoint expiry timer going off
 */
static void afs_mntpt_expiry_timed_out(struct work_struct *work)
{
	_enter("");

	if (!list_empty(&afs_vfsmounts)) {
		mark_mounts_for_expiry(&afs_vfsmounts);
		queue_delayed_work(afs_wq, &afs_mntpt_expiry_timer,
				   afs_mntpt_expiry_timeout * HZ);
	}

	_leave("");
}

/*
 * kill the AFS mountpoint timer if it's still running
 */
void afs_mntpt_kill_timer(void)
{
	_enter("");

	ASSERT(list_empty(&afs_vfsmounts));
	cancel_delayed_work_sync(&afs_mntpt_expiry_timer);
}
