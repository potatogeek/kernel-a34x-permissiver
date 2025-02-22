<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * linux/fs/ceph/acl.c
 *
 * Copyright (C) 2013 Guangliang Zhao, <lucienchao@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
=======
>>>>>>> upstream/android-13
 */

#include <linux/ceph/ceph_debug.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/xattr.h>
#include <linux/posix_acl_xattr.h>
#include <linux/posix_acl.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "super.h"

static inline void ceph_set_cached_acl(struct inode *inode,
					int type, struct posix_acl *acl)
{
	struct ceph_inode_info *ci = ceph_inode(inode);

	spin_lock(&ci->i_ceph_lock);
<<<<<<< HEAD
	if (__ceph_caps_issued_mask(ci, CEPH_CAP_XATTR_SHARED, 0))
=======
	if (__ceph_caps_issued_mask_metric(ci, CEPH_CAP_XATTR_SHARED, 0))
>>>>>>> upstream/android-13
		set_cached_acl(inode, type, acl);
	else
		forget_cached_acl(inode, type);
	spin_unlock(&ci->i_ceph_lock);
}

<<<<<<< HEAD
struct posix_acl *ceph_get_acl(struct inode *inode, int type)
=======
struct posix_acl *ceph_get_acl(struct inode *inode, int type, bool rcu)
>>>>>>> upstream/android-13
{
	int size;
	unsigned int retry_cnt = 0;
	const char *name;
	char *value = NULL;
	struct posix_acl *acl;

<<<<<<< HEAD
=======
	if (rcu)
		return ERR_PTR(-ECHILD);

>>>>>>> upstream/android-13
	switch (type) {
	case ACL_TYPE_ACCESS:
		name = XATTR_NAME_POSIX_ACL_ACCESS;
		break;
	case ACL_TYPE_DEFAULT:
		name = XATTR_NAME_POSIX_ACL_DEFAULT;
		break;
	default:
		BUG();
	}

retry:
	size = __ceph_getxattr(inode, name, "", 0);
	if (size > 0) {
		value = kzalloc(size, GFP_NOFS);
		if (!value)
			return ERR_PTR(-ENOMEM);
		size = __ceph_getxattr(inode, name, value, size);
	}

	if (size == -ERANGE && retry_cnt < 10) {
		retry_cnt++;
		kfree(value);
		value = NULL;
		goto retry;
	}

	if (size > 0) {
		acl = posix_acl_from_xattr(&init_user_ns, value, size);
	} else if (size == -ENODATA || size == 0) {
		acl = NULL;
	} else {
		pr_err_ratelimited("get acl %llx.%llx failed, err=%d\n",
				   ceph_vinop(inode), size);
		acl = ERR_PTR(-EIO);
	}

	kfree(value);

	if (!IS_ERR(acl))
		ceph_set_cached_acl(inode, type, acl);

	return acl;
}

<<<<<<< HEAD
int ceph_set_acl(struct inode *inode, struct posix_acl *acl, int type)
=======
int ceph_set_acl(struct user_namespace *mnt_userns, struct inode *inode,
		 struct posix_acl *acl, int type)
>>>>>>> upstream/android-13
{
	int ret = 0, size = 0;
	const char *name = NULL;
	char *value = NULL;
	struct iattr newattrs;
	struct timespec64 old_ctime = inode->i_ctime;
	umode_t new_mode = inode->i_mode, old_mode = inode->i_mode;

<<<<<<< HEAD
=======
	if (ceph_snap(inode) != CEPH_NOSNAP) {
		ret = -EROFS;
		goto out;
	}

>>>>>>> upstream/android-13
	switch (type) {
	case ACL_TYPE_ACCESS:
		name = XATTR_NAME_POSIX_ACL_ACCESS;
		if (acl) {
<<<<<<< HEAD
			ret = posix_acl_update_mode(inode, &new_mode, &acl);
=======
			ret = posix_acl_update_mode(&init_user_ns, inode,
						    &new_mode, &acl);
>>>>>>> upstream/android-13
			if (ret)
				goto out;
		}
		break;
	case ACL_TYPE_DEFAULT:
		if (!S_ISDIR(inode->i_mode)) {
			ret = acl ? -EINVAL : 0;
			goto out;
		}
		name = XATTR_NAME_POSIX_ACL_DEFAULT;
		break;
	default:
		ret = -EINVAL;
		goto out;
	}

	if (acl) {
		size = posix_acl_xattr_size(acl->a_count);
		value = kmalloc(size, GFP_NOFS);
		if (!value) {
			ret = -ENOMEM;
			goto out;
		}

		ret = posix_acl_to_xattr(&init_user_ns, acl, value, size);
		if (ret < 0)
			goto out_free;
	}

<<<<<<< HEAD
	if (ceph_snap(inode) != CEPH_NOSNAP) {
		ret = -EROFS;
		goto out_free;
	}

=======
>>>>>>> upstream/android-13
	if (new_mode != old_mode) {
		newattrs.ia_ctime = current_time(inode);
		newattrs.ia_mode = new_mode;
		newattrs.ia_valid = ATTR_MODE | ATTR_CTIME;
		ret = __ceph_setattr(inode, &newattrs);
		if (ret)
			goto out_free;
	}

	ret = __ceph_setxattr(inode, name, value, size, 0);
	if (ret) {
		if (new_mode != old_mode) {
			newattrs.ia_ctime = old_ctime;
			newattrs.ia_mode = old_mode;
			newattrs.ia_valid = ATTR_MODE | ATTR_CTIME;
			__ceph_setattr(inode, &newattrs);
		}
		goto out_free;
	}

	ceph_set_cached_acl(inode, type, acl);

out_free:
	kfree(value);
out:
	return ret;
}

int ceph_pre_init_acls(struct inode *dir, umode_t *mode,
<<<<<<< HEAD
		       struct ceph_acls_info *info)
=======
		       struct ceph_acl_sec_ctx *as_ctx)
>>>>>>> upstream/android-13
{
	struct posix_acl *acl, *default_acl;
	size_t val_size1 = 0, val_size2 = 0;
	struct ceph_pagelist *pagelist = NULL;
	void *tmp_buf = NULL;
	int err;

	err = posix_acl_create(dir, mode, &default_acl, &acl);
	if (err)
		return err;

	if (acl) {
		err = posix_acl_equiv_mode(acl, mode);
		if (err < 0)
			goto out_err;
		if (err == 0) {
			posix_acl_release(acl);
			acl = NULL;
		}
	}

	if (!default_acl && !acl)
		return 0;

	if (acl)
		val_size1 = posix_acl_xattr_size(acl->a_count);
	if (default_acl)
		val_size2 = posix_acl_xattr_size(default_acl->a_count);

	err = -ENOMEM;
	tmp_buf = kmalloc(max(val_size1, val_size2), GFP_KERNEL);
	if (!tmp_buf)
		goto out_err;
<<<<<<< HEAD
	pagelist = kmalloc(sizeof(struct ceph_pagelist), GFP_KERNEL);
	if (!pagelist)
		goto out_err;
	ceph_pagelist_init(pagelist);
=======
	pagelist = ceph_pagelist_alloc(GFP_KERNEL);
	if (!pagelist)
		goto out_err;
>>>>>>> upstream/android-13

	err = ceph_pagelist_reserve(pagelist, PAGE_SIZE);
	if (err)
		goto out_err;

	ceph_pagelist_encode_32(pagelist, acl && default_acl ? 2 : 1);

	if (acl) {
		size_t len = strlen(XATTR_NAME_POSIX_ACL_ACCESS);
		err = ceph_pagelist_reserve(pagelist, len + val_size1 + 8);
		if (err)
			goto out_err;
		ceph_pagelist_encode_string(pagelist, XATTR_NAME_POSIX_ACL_ACCESS,
					    len);
		err = posix_acl_to_xattr(&init_user_ns, acl,
					 tmp_buf, val_size1);
		if (err < 0)
			goto out_err;
		ceph_pagelist_encode_32(pagelist, val_size1);
		ceph_pagelist_append(pagelist, tmp_buf, val_size1);
	}
	if (default_acl) {
		size_t len = strlen(XATTR_NAME_POSIX_ACL_DEFAULT);
		err = ceph_pagelist_reserve(pagelist, len + val_size2 + 8);
		if (err)
			goto out_err;
<<<<<<< HEAD
		err = ceph_pagelist_encode_string(pagelist,
						  XATTR_NAME_POSIX_ACL_DEFAULT, len);
=======
		ceph_pagelist_encode_string(pagelist,
					  XATTR_NAME_POSIX_ACL_DEFAULT, len);
>>>>>>> upstream/android-13
		err = posix_acl_to_xattr(&init_user_ns, default_acl,
					 tmp_buf, val_size2);
		if (err < 0)
			goto out_err;
		ceph_pagelist_encode_32(pagelist, val_size2);
		ceph_pagelist_append(pagelist, tmp_buf, val_size2);
	}

	kfree(tmp_buf);

<<<<<<< HEAD
	info->acl = acl;
	info->default_acl = default_acl;
	info->pagelist = pagelist;
=======
	as_ctx->acl = acl;
	as_ctx->default_acl = default_acl;
	as_ctx->pagelist = pagelist;
>>>>>>> upstream/android-13
	return 0;

out_err:
	posix_acl_release(acl);
	posix_acl_release(default_acl);
	kfree(tmp_buf);
	if (pagelist)
		ceph_pagelist_release(pagelist);
	return err;
}

<<<<<<< HEAD
void ceph_init_inode_acls(struct inode* inode, struct ceph_acls_info *info)
{
	if (!inode)
		return;
	ceph_set_cached_acl(inode, ACL_TYPE_ACCESS, info->acl);
	ceph_set_cached_acl(inode, ACL_TYPE_DEFAULT, info->default_acl);
}

void ceph_release_acls_info(struct ceph_acls_info *info)
{
	posix_acl_release(info->acl);
	posix_acl_release(info->default_acl);
	if (info->pagelist)
		ceph_pagelist_release(info->pagelist);
=======
void ceph_init_inode_acls(struct inode *inode, struct ceph_acl_sec_ctx *as_ctx)
{
	if (!inode)
		return;
	ceph_set_cached_acl(inode, ACL_TYPE_ACCESS, as_ctx->acl);
	ceph_set_cached_acl(inode, ACL_TYPE_DEFAULT, as_ctx->default_acl);
>>>>>>> upstream/android-13
}
