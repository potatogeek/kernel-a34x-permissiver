<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> upstream/android-13
/*
 * fs/kernfs/inode.c - kernfs inode implementation
 *
 * Copyright (c) 2001-3 Patrick Mochel
 * Copyright (c) 2007 SUSE Linux Products GmbH
 * Copyright (c) 2007, 2013 Tejun Heo <tj@kernel.org>
<<<<<<< HEAD
 *
 * This file is released under the GPLv2.
=======
>>>>>>> upstream/android-13
 */

#include <linux/pagemap.h>
#include <linux/backing-dev.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/xattr.h>
#include <linux/security.h>

#include "kernfs-internal.h"

<<<<<<< HEAD
static const struct address_space_operations kernfs_aops = {
	.readpage	= simple_readpage,
	.write_begin	= simple_write_begin,
	.write_end	= simple_write_end,
};

=======
>>>>>>> upstream/android-13
static const struct inode_operations kernfs_iops = {
	.permission	= kernfs_iop_permission,
	.setattr	= kernfs_iop_setattr,
	.getattr	= kernfs_iop_getattr,
	.listxattr	= kernfs_iop_listxattr,
};

<<<<<<< HEAD
static struct kernfs_iattrs *kernfs_iattrs(struct kernfs_node *kn)
{
	static DEFINE_MUTEX(iattr_mutex);
	struct kernfs_iattrs *ret;
	struct iattr *iattrs;

	mutex_lock(&iattr_mutex);

	if (kn->iattr)
		goto out_unlock;

	kn->iattr = kzalloc(sizeof(struct kernfs_iattrs), GFP_KERNEL);
	if (!kn->iattr)
		goto out_unlock;
	iattrs = &kn->iattr->ia_iattr;

	/* assign default attributes */
	iattrs->ia_mode = kn->mode;
	iattrs->ia_uid = GLOBAL_ROOT_UID;
	iattrs->ia_gid = GLOBAL_ROOT_GID;

	ktime_get_real_ts64(&iattrs->ia_atime);
	iattrs->ia_mtime = iattrs->ia_atime;
	iattrs->ia_ctime = iattrs->ia_atime;

	simple_xattrs_init(&kn->iattr->xattrs);
=======
static struct kernfs_iattrs *__kernfs_iattrs(struct kernfs_node *kn, int alloc)
{
	static DEFINE_MUTEX(iattr_mutex);
	struct kernfs_iattrs *ret;

	mutex_lock(&iattr_mutex);

	if (kn->iattr || !alloc)
		goto out_unlock;

	kn->iattr = kmem_cache_zalloc(kernfs_iattrs_cache, GFP_KERNEL);
	if (!kn->iattr)
		goto out_unlock;

	/* assign default attributes */
	kn->iattr->ia_uid = GLOBAL_ROOT_UID;
	kn->iattr->ia_gid = GLOBAL_ROOT_GID;

	ktime_get_real_ts64(&kn->iattr->ia_atime);
	kn->iattr->ia_mtime = kn->iattr->ia_atime;
	kn->iattr->ia_ctime = kn->iattr->ia_atime;

	simple_xattrs_init(&kn->iattr->xattrs);
	atomic_set(&kn->iattr->nr_user_xattrs, 0);
	atomic_set(&kn->iattr->user_xattr_size, 0);
>>>>>>> upstream/android-13
out_unlock:
	ret = kn->iattr;
	mutex_unlock(&iattr_mutex);
	return ret;
}

<<<<<<< HEAD
int __kernfs_setattr(struct kernfs_node *kn, const struct iattr *iattr)
{
	struct kernfs_iattrs *attrs;
	struct iattr *iattrs;
=======
static struct kernfs_iattrs *kernfs_iattrs(struct kernfs_node *kn)
{
	return __kernfs_iattrs(kn, 1);
}

static struct kernfs_iattrs *kernfs_iattrs_noalloc(struct kernfs_node *kn)
{
	return __kernfs_iattrs(kn, 0);
}

int __kernfs_setattr(struct kernfs_node *kn, const struct iattr *iattr)
{
	struct kernfs_iattrs *attrs;
>>>>>>> upstream/android-13
	unsigned int ia_valid = iattr->ia_valid;

	attrs = kernfs_iattrs(kn);
	if (!attrs)
		return -ENOMEM;

<<<<<<< HEAD
	iattrs = &attrs->ia_iattr;

	if (ia_valid & ATTR_UID)
		iattrs->ia_uid = iattr->ia_uid;
	if (ia_valid & ATTR_GID)
		iattrs->ia_gid = iattr->ia_gid;
	if (ia_valid & ATTR_ATIME)
		iattrs->ia_atime = iattr->ia_atime;
	if (ia_valid & ATTR_MTIME)
		iattrs->ia_mtime = iattr->ia_mtime;
	if (ia_valid & ATTR_CTIME)
		iattrs->ia_ctime = iattr->ia_ctime;
	if (ia_valid & ATTR_MODE) {
		umode_t mode = iattr->ia_mode;
		iattrs->ia_mode = kn->mode = mode;
	}
=======
	if (ia_valid & ATTR_UID)
		attrs->ia_uid = iattr->ia_uid;
	if (ia_valid & ATTR_GID)
		attrs->ia_gid = iattr->ia_gid;
	if (ia_valid & ATTR_ATIME)
		attrs->ia_atime = iattr->ia_atime;
	if (ia_valid & ATTR_MTIME)
		attrs->ia_mtime = iattr->ia_mtime;
	if (ia_valid & ATTR_CTIME)
		attrs->ia_ctime = iattr->ia_ctime;
	if (ia_valid & ATTR_MODE)
		kn->mode = iattr->ia_mode;
>>>>>>> upstream/android-13
	return 0;
}

/**
 * kernfs_setattr - set iattr on a node
 * @kn: target node
 * @iattr: iattr to set
 *
 * Returns 0 on success, -errno on failure.
 */
int kernfs_setattr(struct kernfs_node *kn, const struct iattr *iattr)
{
	int ret;

<<<<<<< HEAD
	mutex_lock(&kernfs_mutex);
	ret = __kernfs_setattr(kn, iattr);
	mutex_unlock(&kernfs_mutex);
	return ret;
}

int kernfs_iop_setattr(struct dentry *dentry, struct iattr *iattr)
=======
	down_write(&kernfs_rwsem);
	ret = __kernfs_setattr(kn, iattr);
	up_write(&kernfs_rwsem);
	return ret;
}

int kernfs_iop_setattr(struct user_namespace *mnt_userns, struct dentry *dentry,
		       struct iattr *iattr)
>>>>>>> upstream/android-13
{
	struct inode *inode = d_inode(dentry);
	struct kernfs_node *kn = inode->i_private;
	int error;

	if (!kn)
		return -EINVAL;

<<<<<<< HEAD
	mutex_lock(&kernfs_mutex);
	error = setattr_prepare(dentry, iattr);
=======
	down_write(&kernfs_rwsem);
	error = setattr_prepare(&init_user_ns, dentry, iattr);
>>>>>>> upstream/android-13
	if (error)
		goto out;

	error = __kernfs_setattr(kn, iattr);
	if (error)
		goto out;

	/* this ignores size changes */
<<<<<<< HEAD
	setattr_copy(inode, iattr);

out:
	mutex_unlock(&kernfs_mutex);
	return error;
}

static int kernfs_node_setsecdata(struct kernfs_iattrs *attrs, void **secdata,
				  u32 *secdata_len)
{
	void *old_secdata;
	size_t old_secdata_len;

	old_secdata = attrs->ia_secdata;
	old_secdata_len = attrs->ia_secdata_len;

	attrs->ia_secdata = *secdata;
	attrs->ia_secdata_len = *secdata_len;

	*secdata = old_secdata;
	*secdata_len = old_secdata_len;
	return 0;
}

=======
	setattr_copy(&init_user_ns, inode, iattr);

out:
	up_write(&kernfs_rwsem);
	return error;
}

>>>>>>> upstream/android-13
ssize_t kernfs_iop_listxattr(struct dentry *dentry, char *buf, size_t size)
{
	struct kernfs_node *kn = kernfs_dentry_node(dentry);
	struct kernfs_iattrs *attrs;

	attrs = kernfs_iattrs(kn);
	if (!attrs)
		return -ENOMEM;

	return simple_xattr_list(d_inode(dentry), &attrs->xattrs, buf, size);
}

static inline void set_default_inode_attr(struct inode *inode, umode_t mode)
{
	inode->i_mode = mode;
	inode->i_atime = inode->i_mtime =
		inode->i_ctime = current_time(inode);
}

<<<<<<< HEAD
static inline void set_inode_attr(struct inode *inode, struct iattr *iattr)
{
	struct super_block *sb = inode->i_sb;
	inode->i_uid = iattr->ia_uid;
	inode->i_gid = iattr->ia_gid;
	inode->i_atime = timespec64_trunc(iattr->ia_atime, sb->s_time_gran);
	inode->i_mtime = timespec64_trunc(iattr->ia_mtime, sb->s_time_gran);
	inode->i_ctime = timespec64_trunc(iattr->ia_ctime, sb->s_time_gran);
=======
static inline void set_inode_attr(struct inode *inode,
				  struct kernfs_iattrs *attrs)
{
	inode->i_uid = attrs->ia_uid;
	inode->i_gid = attrs->ia_gid;
	inode->i_atime = attrs->ia_atime;
	inode->i_mtime = attrs->ia_mtime;
	inode->i_ctime = attrs->ia_ctime;
>>>>>>> upstream/android-13
}

static void kernfs_refresh_inode(struct kernfs_node *kn, struct inode *inode)
{
	struct kernfs_iattrs *attrs = kn->iattr;

	inode->i_mode = kn->mode;
<<<<<<< HEAD
	if (attrs) {
=======
	if (attrs)
>>>>>>> upstream/android-13
		/*
		 * kernfs_node has non-default attributes get them from
		 * persistent copy in kernfs_node.
		 */
<<<<<<< HEAD
		set_inode_attr(inode, &attrs->ia_iattr);
		security_inode_notifysecctx(inode, attrs->ia_secdata,
					    attrs->ia_secdata_len);
	}
=======
		set_inode_attr(inode, attrs);
>>>>>>> upstream/android-13

	if (kernfs_type(kn) == KERNFS_DIR)
		set_nlink(inode, kn->dir.subdirs + 2);
}

<<<<<<< HEAD
int kernfs_iop_getattr(const struct path *path, struct kstat *stat,
=======
int kernfs_iop_getattr(struct user_namespace *mnt_userns,
		       const struct path *path, struct kstat *stat,
>>>>>>> upstream/android-13
		       u32 request_mask, unsigned int query_flags)
{
	struct inode *inode = d_inode(path->dentry);
	struct kernfs_node *kn = inode->i_private;

<<<<<<< HEAD
	mutex_lock(&kernfs_mutex);
	kernfs_refresh_inode(kn, inode);
	mutex_unlock(&kernfs_mutex);

	generic_fillattr(inode, stat);
=======
	down_read(&kernfs_rwsem);
	spin_lock(&inode->i_lock);
	kernfs_refresh_inode(kn, inode);
	generic_fillattr(&init_user_ns, inode, stat);
	spin_unlock(&inode->i_lock);
	up_read(&kernfs_rwsem);

>>>>>>> upstream/android-13
	return 0;
}

static void kernfs_init_inode(struct kernfs_node *kn, struct inode *inode)
{
	kernfs_get(kn);
	inode->i_private = kn;
<<<<<<< HEAD
	inode->i_mapping->a_ops = &kernfs_aops;
	inode->i_op = &kernfs_iops;
	inode->i_generation = kn->id.generation;
=======
	inode->i_mapping->a_ops = &ram_aops;
	inode->i_op = &kernfs_iops;
	inode->i_generation = kernfs_gen(kn);
>>>>>>> upstream/android-13

	set_default_inode_attr(inode, kn->mode);
	kernfs_refresh_inode(kn, inode);

	/* initialize inode according to type */
	switch (kernfs_type(kn)) {
	case KERNFS_DIR:
		inode->i_op = &kernfs_dir_iops;
		inode->i_fop = &kernfs_dir_fops;
		if (kn->flags & KERNFS_EMPTY_DIR)
			make_empty_dir_inode(inode);
		break;
	case KERNFS_FILE:
		inode->i_size = kn->attr.size;
		inode->i_fop = &kernfs_file_fops;
		break;
	case KERNFS_LINK:
		inode->i_op = &kernfs_symlink_iops;
		break;
	default:
		BUG();
	}

	unlock_new_inode(inode);
}

/**
 *	kernfs_get_inode - get inode for kernfs_node
 *	@sb: super block
 *	@kn: kernfs_node to allocate inode for
 *
 *	Get inode for @kn.  If such inode doesn't exist, a new inode is
 *	allocated and basics are initialized.  New inode is returned
 *	locked.
 *
 *	LOCKING:
 *	Kernel thread context (may sleep).
 *
 *	RETURNS:
 *	Pointer to allocated inode on success, NULL on failure.
 */
struct inode *kernfs_get_inode(struct super_block *sb, struct kernfs_node *kn)
{
	struct inode *inode;

<<<<<<< HEAD
	inode = iget_locked(sb, kn->id.ino);
=======
	inode = iget_locked(sb, kernfs_ino(kn));
>>>>>>> upstream/android-13
	if (inode && (inode->i_state & I_NEW))
		kernfs_init_inode(kn, inode);

	return inode;
}

/*
 * The kernfs_node serves as both an inode and a directory entry for
 * kernfs.  To prevent the kernfs inode numbers from being freed
 * prematurely we take a reference to kernfs_node from the kernfs inode.  A
 * super_operations.evict_inode() implementation is needed to drop that
 * reference upon inode destruction.
 */
void kernfs_evict_inode(struct inode *inode)
{
	struct kernfs_node *kn = inode->i_private;

	truncate_inode_pages_final(&inode->i_data);
	clear_inode(inode);
	kernfs_put(kn);
}

<<<<<<< HEAD
int kernfs_iop_permission(struct inode *inode, int mask)
{
	struct kernfs_node *kn;
=======
int kernfs_iop_permission(struct user_namespace *mnt_userns,
			  struct inode *inode, int mask)
{
	struct kernfs_node *kn;
	int ret;
>>>>>>> upstream/android-13

	if (mask & MAY_NOT_BLOCK)
		return -ECHILD;

	kn = inode->i_private;

<<<<<<< HEAD
	mutex_lock(&kernfs_mutex);
	kernfs_refresh_inode(kn, inode);
	mutex_unlock(&kernfs_mutex);

	return generic_permission(inode, mask);
}

static int kernfs_xattr_get(const struct xattr_handler *handler,
			    struct dentry *unused, struct inode *inode,
			    const char *suffix, void *value, size_t size)
{
	const char *name = xattr_full_name(handler, suffix);
	struct kernfs_node *kn = inode->i_private;
	struct kernfs_iattrs *attrs;

	attrs = kernfs_iattrs(kn);
	if (!attrs)
		return -ENOMEM;
=======
	down_read(&kernfs_rwsem);
	spin_lock(&inode->i_lock);
	kernfs_refresh_inode(kn, inode);
	ret = generic_permission(&init_user_ns, inode, mask);
	spin_unlock(&inode->i_lock);
	up_read(&kernfs_rwsem);

	return ret;
}

int kernfs_xattr_get(struct kernfs_node *kn, const char *name,
		     void *value, size_t size)
{
	struct kernfs_iattrs *attrs = kernfs_iattrs_noalloc(kn);
	if (!attrs)
		return -ENODATA;
>>>>>>> upstream/android-13

	return simple_xattr_get(&attrs->xattrs, name, value, size);
}

<<<<<<< HEAD
static int kernfs_xattr_set(const struct xattr_handler *handler,
			    struct dentry *unused, struct inode *inode,
			    const char *suffix, const void *value,
			    size_t size, int flags)
{
	const char *name = xattr_full_name(handler, suffix);
	struct kernfs_node *kn = inode->i_private;
	struct kernfs_iattrs *attrs;

	attrs = kernfs_iattrs(kn);
	if (!attrs)
		return -ENOMEM;

	return simple_xattr_set(&attrs->xattrs, name, value, size, flags);
}

static const struct xattr_handler kernfs_trusted_xattr_handler = {
	.prefix = XATTR_TRUSTED_PREFIX,
	.get = kernfs_xattr_get,
	.set = kernfs_xattr_set,
};

static int kernfs_security_xattr_set(const struct xattr_handler *handler,
=======
int kernfs_xattr_set(struct kernfs_node *kn, const char *name,
		     const void *value, size_t size, int flags)
{
	struct kernfs_iattrs *attrs = kernfs_iattrs(kn);
	if (!attrs)
		return -ENOMEM;

	return simple_xattr_set(&attrs->xattrs, name, value, size, flags, NULL);
}

static int kernfs_vfs_xattr_get(const struct xattr_handler *handler,
				struct dentry *unused, struct inode *inode,
				const char *suffix, void *value, size_t size)
{
	const char *name = xattr_full_name(handler, suffix);
	struct kernfs_node *kn = inode->i_private;

	return kernfs_xattr_get(kn, name, value, size);
}

static int kernfs_vfs_xattr_set(const struct xattr_handler *handler,
				struct user_namespace *mnt_userns,
				struct dentry *unused, struct inode *inode,
				const char *suffix, const void *value,
				size_t size, int flags)
{
	const char *name = xattr_full_name(handler, suffix);
	struct kernfs_node *kn = inode->i_private;

	return kernfs_xattr_set(kn, name, value, size, flags);
}

static int kernfs_vfs_user_xattr_add(struct kernfs_node *kn,
				     const char *full_name,
				     struct simple_xattrs *xattrs,
				     const void *value, size_t size, int flags)
{
	atomic_t *sz = &kn->iattr->user_xattr_size;
	atomic_t *nr = &kn->iattr->nr_user_xattrs;
	ssize_t removed_size;
	int ret;

	if (atomic_inc_return(nr) > KERNFS_MAX_USER_XATTRS) {
		ret = -ENOSPC;
		goto dec_count_out;
	}

	if (atomic_add_return(size, sz) > KERNFS_USER_XATTR_SIZE_LIMIT) {
		ret = -ENOSPC;
		goto dec_size_out;
	}

	ret = simple_xattr_set(xattrs, full_name, value, size, flags,
			       &removed_size);

	if (!ret && removed_size >= 0)
		size = removed_size;
	else if (!ret)
		return 0;
dec_size_out:
	atomic_sub(size, sz);
dec_count_out:
	atomic_dec(nr);
	return ret;
}

static int kernfs_vfs_user_xattr_rm(struct kernfs_node *kn,
				    const char *full_name,
				    struct simple_xattrs *xattrs,
				    const void *value, size_t size, int flags)
{
	atomic_t *sz = &kn->iattr->user_xattr_size;
	atomic_t *nr = &kn->iattr->nr_user_xattrs;
	ssize_t removed_size;
	int ret;

	ret = simple_xattr_set(xattrs, full_name, value, size, flags,
			       &removed_size);

	if (removed_size >= 0) {
		atomic_sub(removed_size, sz);
		atomic_dec(nr);
	}

	return ret;
}

static int kernfs_vfs_user_xattr_set(const struct xattr_handler *handler,
				     struct user_namespace *mnt_userns,
>>>>>>> upstream/android-13
				     struct dentry *unused, struct inode *inode,
				     const char *suffix, const void *value,
				     size_t size, int flags)
{
<<<<<<< HEAD
	struct kernfs_node *kn = inode->i_private;
	struct kernfs_iattrs *attrs;
	void *secdata;
	u32 secdata_len = 0;
	int error;
=======
	const char *full_name = xattr_full_name(handler, suffix);
	struct kernfs_node *kn = inode->i_private;
	struct kernfs_iattrs *attrs;

	if (!(kernfs_root(kn)->flags & KERNFS_ROOT_SUPPORT_USER_XATTR))
		return -EOPNOTSUPP;
>>>>>>> upstream/android-13

	attrs = kernfs_iattrs(kn);
	if (!attrs)
		return -ENOMEM;

<<<<<<< HEAD
	error = security_inode_setsecurity(inode, suffix, value, size, flags);
	if (error)
		return error;
	error = security_inode_getsecctx(inode, &secdata, &secdata_len);
	if (error)
		return error;

	mutex_lock(&kernfs_mutex);
	error = kernfs_node_setsecdata(attrs, &secdata, &secdata_len);
	mutex_unlock(&kernfs_mutex);

	if (secdata)
		security_release_secctx(secdata, secdata_len);
	return error;
}

static const struct xattr_handler kernfs_security_xattr_handler = {
	.prefix = XATTR_SECURITY_PREFIX,
	.get = kernfs_xattr_get,
	.set = kernfs_security_xattr_set,
=======
	if (value)
		return kernfs_vfs_user_xattr_add(kn, full_name, &attrs->xattrs,
						 value, size, flags);
	else
		return kernfs_vfs_user_xattr_rm(kn, full_name, &attrs->xattrs,
						value, size, flags);

}

static const struct xattr_handler kernfs_trusted_xattr_handler = {
	.prefix = XATTR_TRUSTED_PREFIX,
	.get = kernfs_vfs_xattr_get,
	.set = kernfs_vfs_xattr_set,
};

static const struct xattr_handler kernfs_security_xattr_handler = {
	.prefix = XATTR_SECURITY_PREFIX,
	.get = kernfs_vfs_xattr_get,
	.set = kernfs_vfs_xattr_set,
};

static const struct xattr_handler kernfs_user_xattr_handler = {
	.prefix = XATTR_USER_PREFIX,
	.get = kernfs_vfs_xattr_get,
	.set = kernfs_vfs_user_xattr_set,
>>>>>>> upstream/android-13
};

const struct xattr_handler *kernfs_xattr_handlers[] = {
	&kernfs_trusted_xattr_handler,
	&kernfs_security_xattr_handler,
<<<<<<< HEAD
=======
	&kernfs_user_xattr_handler,
>>>>>>> upstream/android-13
	NULL
};
