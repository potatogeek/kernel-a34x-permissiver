<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 *  linux/fs/9p/vfs_dentry.c
 *
 * This file contians vfs dentry ops for the 9P2000 protocol.
 *
 *  Copyright (C) 2004 by Eric Van Hensbergen <ericvh@gmail.com>
 *  Copyright (C) 2002 by Ron Minnich <rminnich@lanl.gov>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
=======
>>>>>>> upstream/android-13
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/pagemap.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/inet.h>
#include <linux/namei.h>
#include <linux/idr.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <net/9p/9p.h>
#include <net/9p/client.h>

#include "v9fs.h"
#include "v9fs_vfs.h"
#include "fid.h"

/**
 * v9fs_cached_dentry_delete - called when dentry refcount equals 0
 * @dentry:  dentry in question
 *
 */
static int v9fs_cached_dentry_delete(const struct dentry *dentry)
{
	p9_debug(P9_DEBUG_VFS, " dentry: %pd (%p)\n",
		 dentry, dentry);

	/* Don't cache negative dentries */
	if (d_really_is_negative(dentry))
		return 1;
	return 0;
}

/**
 * v9fs_dentry_release - called when dentry is going to be freed
 * @dentry:  dentry that is being release
 *
 */

static void v9fs_dentry_release(struct dentry *dentry)
{
	struct hlist_node *p, *n;
	p9_debug(P9_DEBUG_VFS, " dentry: %pd (%p)\n",
		 dentry, dentry);
	hlist_for_each_safe(p, n, (struct hlist_head *)&dentry->d_fsdata)
		p9_client_clunk(hlist_entry(p, struct p9_fid, dlist));
	dentry->d_fsdata = NULL;
}

static int v9fs_lookup_revalidate(struct dentry *dentry, unsigned int flags)
{
	struct p9_fid *fid;
	struct inode *inode;
	struct v9fs_inode *v9inode;

	if (flags & LOOKUP_RCU)
		return -ECHILD;

	inode = d_inode(dentry);
	if (!inode)
		goto out_valid;

	v9inode = V9FS_I(inode);
	if (v9inode->cache_validity & V9FS_INO_INVALID_ATTR) {
		int retval;
		struct v9fs_session_info *v9ses;
		fid = v9fs_fid_lookup(dentry);
		if (IS_ERR(fid))
			return PTR_ERR(fid);

		v9ses = v9fs_inode2v9ses(inode);
		if (v9fs_proto_dotl(v9ses))
			retval = v9fs_refresh_inode_dotl(fid, inode);
		else
			retval = v9fs_refresh_inode(fid, inode);
<<<<<<< HEAD
=======
		p9_client_clunk(fid);

>>>>>>> upstream/android-13
		if (retval == -ENOENT)
			return 0;
		if (retval < 0)
			return retval;
	}
out_valid:
	return 1;
}

const struct dentry_operations v9fs_cached_dentry_operations = {
	.d_revalidate = v9fs_lookup_revalidate,
	.d_weak_revalidate = v9fs_lookup_revalidate,
	.d_delete = v9fs_cached_dentry_delete,
	.d_release = v9fs_dentry_release,
};

const struct dentry_operations v9fs_dentry_operations = {
	.d_delete = always_delete_dentry,
	.d_release = v9fs_dentry_release,
};
