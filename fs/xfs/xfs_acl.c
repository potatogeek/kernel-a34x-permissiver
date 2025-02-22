// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2008, Christoph Hellwig
 * All Rights Reserved.
 */
#include "xfs.h"
<<<<<<< HEAD
=======
#include "xfs_shared.h"
>>>>>>> upstream/android-13
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
<<<<<<< HEAD
#include "xfs_acl.h"
#include "xfs_attr.h"
#include "xfs_trace.h"
#include <linux/slab.h>
#include <linux/xattr.h>
#include <linux/posix_acl_xattr.h>

=======
#include "xfs_attr.h"
#include "xfs_trace.h"
#include "xfs_error.h"
#include "xfs_acl.h"
#include "xfs_da_format.h"
#include "xfs_da_btree.h"
#include "xfs_trans.h"

#include <linux/posix_acl_xattr.h>
>>>>>>> upstream/android-13

/*
 * Locking scheme:
 *  - all ACL updates are protected by inode->i_mutex, which is taken before
 *    calling into this file.
 */

STATIC struct posix_acl *
xfs_acl_from_disk(
<<<<<<< HEAD
=======
	struct xfs_mount	*mp,
>>>>>>> upstream/android-13
	const struct xfs_acl	*aclp,
	int			len,
	int			max_entries)
{
	struct posix_acl_entry *acl_e;
	struct posix_acl *acl;
	const struct xfs_acl_entry *ace;
	unsigned int count, i;

<<<<<<< HEAD
	if (len < sizeof(*aclp))
		return ERR_PTR(-EFSCORRUPTED);
	count = be32_to_cpu(aclp->acl_cnt);
	if (count > max_entries || XFS_ACL_SIZE(count) != len)
		return ERR_PTR(-EFSCORRUPTED);
=======
	if (len < sizeof(*aclp)) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, aclp,
				len);
		return ERR_PTR(-EFSCORRUPTED);
	}

	count = be32_to_cpu(aclp->acl_cnt);
	if (count > max_entries || XFS_ACL_SIZE(count) != len) {
		XFS_CORRUPTION_ERROR(__func__, XFS_ERRLEVEL_LOW, mp, aclp,
				len);
		return ERR_PTR(-EFSCORRUPTED);
	}
>>>>>>> upstream/android-13

	acl = posix_acl_alloc(count, GFP_KERNEL);
	if (!acl)
		return ERR_PTR(-ENOMEM);

	for (i = 0; i < count; i++) {
		acl_e = &acl->a_entries[i];
		ace = &aclp->acl_entry[i];

		/*
		 * The tag is 32 bits on disk and 16 bits in core.
		 *
		 * Because every access to it goes through the core
		 * format first this is not a problem.
		 */
		acl_e->e_tag = be32_to_cpu(ace->ae_tag);
		acl_e->e_perm = be16_to_cpu(ace->ae_perm);

		switch (acl_e->e_tag) {
		case ACL_USER:
<<<<<<< HEAD
			acl_e->e_uid = xfs_uid_to_kuid(be32_to_cpu(ace->ae_id));
			break;
		case ACL_GROUP:
			acl_e->e_gid = xfs_gid_to_kgid(be32_to_cpu(ace->ae_id));
=======
			acl_e->e_uid = make_kuid(&init_user_ns,
						 be32_to_cpu(ace->ae_id));
			break;
		case ACL_GROUP:
			acl_e->e_gid = make_kgid(&init_user_ns,
						 be32_to_cpu(ace->ae_id));
>>>>>>> upstream/android-13
			break;
		case ACL_USER_OBJ:
		case ACL_GROUP_OBJ:
		case ACL_MASK:
		case ACL_OTHER:
			break;
		default:
			goto fail;
		}
	}
	return acl;

fail:
	posix_acl_release(acl);
	return ERR_PTR(-EINVAL);
}

STATIC void
xfs_acl_to_disk(struct xfs_acl *aclp, const struct posix_acl *acl)
{
	const struct posix_acl_entry *acl_e;
	struct xfs_acl_entry *ace;
	int i;

	aclp->acl_cnt = cpu_to_be32(acl->a_count);
	for (i = 0; i < acl->a_count; i++) {
		ace = &aclp->acl_entry[i];
		acl_e = &acl->a_entries[i];

		ace->ae_tag = cpu_to_be32(acl_e->e_tag);
		switch (acl_e->e_tag) {
		case ACL_USER:
<<<<<<< HEAD
			ace->ae_id = cpu_to_be32(xfs_kuid_to_uid(acl_e->e_uid));
			break;
		case ACL_GROUP:
			ace->ae_id = cpu_to_be32(xfs_kgid_to_gid(acl_e->e_gid));
=======
			ace->ae_id = cpu_to_be32(
					from_kuid(&init_user_ns, acl_e->e_uid));
			break;
		case ACL_GROUP:
			ace->ae_id = cpu_to_be32(
					from_kgid(&init_user_ns, acl_e->e_gid));
>>>>>>> upstream/android-13
			break;
		default:
			ace->ae_id = cpu_to_be32(ACL_UNDEFINED_ID);
			break;
		}

		ace->ae_perm = cpu_to_be16(acl_e->e_perm);
	}
}

struct posix_acl *
<<<<<<< HEAD
xfs_get_acl(struct inode *inode, int type)
{
	struct xfs_inode *ip = XFS_I(inode);
	struct posix_acl *acl = NULL;
	struct xfs_acl *xfs_acl;
	unsigned char *ea_name;
	int error;
	int len;
=======
xfs_get_acl(struct inode *inode, int type, bool rcu)
{
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	struct posix_acl	*acl = NULL;
	struct xfs_da_args	args = {
		.dp		= ip,
		.attr_filter	= XFS_ATTR_ROOT,
		.valuelen	= XFS_ACL_MAX_SIZE(mp),
	};
	int			error;

	if (rcu)
		return ERR_PTR(-ECHILD);
>>>>>>> upstream/android-13

	trace_xfs_get_acl(ip);

	switch (type) {
	case ACL_TYPE_ACCESS:
<<<<<<< HEAD
		ea_name = SGI_ACL_FILE;
		break;
	case ACL_TYPE_DEFAULT:
		ea_name = SGI_ACL_DEFAULT;
=======
		args.name = SGI_ACL_FILE;
		break;
	case ACL_TYPE_DEFAULT:
		args.name = SGI_ACL_DEFAULT;
>>>>>>> upstream/android-13
		break;
	default:
		BUG();
	}
<<<<<<< HEAD

	/*
	 * If we have a cached ACLs value just return it, not need to
	 * go out to the disk.
	 */
	len = XFS_ACL_MAX_SIZE(ip->i_mount);
	xfs_acl = kmem_zalloc_large(len, KM_SLEEP);
	if (!xfs_acl)
		return ERR_PTR(-ENOMEM);

	error = xfs_attr_get(ip, ea_name, (unsigned char *)xfs_acl,
							&len, ATTR_ROOT);
	if (error) {
		/*
		 * If the attribute doesn't exist make sure we have a negative
		 * cache entry, for any other error assume it is transient.
		 */
		if (error != -ENOATTR)
			acl = ERR_PTR(error);
	} else  {
		acl = xfs_acl_from_disk(xfs_acl, len,
					XFS_ACL_MAX_ENTRIES(ip->i_mount));
	}
	kmem_free(xfs_acl);
=======
	args.namelen = strlen(args.name);

	/*
	 * If the attribute doesn't exist make sure we have a negative cache
	 * entry, for any other error assume it is transient.
	 */
	error = xfs_attr_get(&args);
	if (!error) {
		acl = xfs_acl_from_disk(mp, args.value, args.valuelen,
					XFS_ACL_MAX_ENTRIES(mp));
	} else if (error != -ENOATTR) {
		acl = ERR_PTR(error);
	}

	kmem_free(args.value);
>>>>>>> upstream/android-13
	return acl;
}

int
__xfs_set_acl(struct inode *inode, struct posix_acl *acl, int type)
{
<<<<<<< HEAD
	struct xfs_inode *ip = XFS_I(inode);
	unsigned char *ea_name;
	int error;

	switch (type) {
	case ACL_TYPE_ACCESS:
		ea_name = SGI_ACL_FILE;
=======
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_da_args	args = {
		.dp		= ip,
		.attr_filter	= XFS_ATTR_ROOT,
	};
	int			error;

	switch (type) {
	case ACL_TYPE_ACCESS:
		args.name = SGI_ACL_FILE;
>>>>>>> upstream/android-13
		break;
	case ACL_TYPE_DEFAULT:
		if (!S_ISDIR(inode->i_mode))
			return acl ? -EACCES : 0;
<<<<<<< HEAD
		ea_name = SGI_ACL_DEFAULT;
=======
		args.name = SGI_ACL_DEFAULT;
>>>>>>> upstream/android-13
		break;
	default:
		return -EINVAL;
	}
<<<<<<< HEAD

	if (acl) {
		struct xfs_acl *xfs_acl;
		int len = XFS_ACL_MAX_SIZE(ip->i_mount);

		xfs_acl = kmem_zalloc_large(len, KM_SLEEP);
		if (!xfs_acl)
			return -ENOMEM;

		xfs_acl_to_disk(xfs_acl, acl);

		/* subtract away the unused acl entries */
		len -= sizeof(struct xfs_acl_entry) *
			 (XFS_ACL_MAX_ENTRIES(ip->i_mount) - acl->a_count);

		error = xfs_attr_set(ip, ea_name, (unsigned char *)xfs_acl,
				len, ATTR_ROOT);

		kmem_free(xfs_acl);
	} else {
		/*
		 * A NULL ACL argument means we want to remove the ACL.
		 */
		error = xfs_attr_remove(ip, ea_name, ATTR_ROOT);

		/*
		 * If the attribute didn't exist to start with that's fine.
		 */
		if (error == -ENOATTR)
			error = 0;
	}

=======
	args.namelen = strlen(args.name);

	if (acl) {
		args.valuelen = XFS_ACL_SIZE(acl->a_count);
		args.value = kvzalloc(args.valuelen, GFP_KERNEL);
		if (!args.value)
			return -ENOMEM;
		xfs_acl_to_disk(args.value, acl);
	}

	error = xfs_attr_set(&args);
	kmem_free(args.value);

	/*
	 * If the attribute didn't exist to start with that's fine.
	 */
	if (!acl && error == -ENOATTR)
		error = 0;
>>>>>>> upstream/android-13
	if (!error)
		set_cached_acl(inode, type, acl);
	return error;
}

static int
<<<<<<< HEAD
xfs_set_mode(struct inode *inode, umode_t mode)
{
	int error = 0;

	if (mode != inode->i_mode) {
		struct iattr iattr;

		iattr.ia_valid = ATTR_MODE | ATTR_CTIME;
		iattr.ia_mode = mode;
		iattr.ia_ctime = current_time(inode);

		error = xfs_setattr_nonsize(XFS_I(inode), &iattr, XFS_ATTR_NOACL);
	}

	return error;
}

int
xfs_set_acl(struct inode *inode, struct posix_acl *acl, int type)
=======
xfs_acl_set_mode(
	struct inode		*inode,
	umode_t			mode)
{
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_trans	*tp;
	int			error;

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_ichange, 0, 0, 0, &tp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
	inode->i_mode = mode;
	inode->i_ctime = current_time(inode);
	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	if (xfs_has_wsync(mp))
		xfs_trans_set_sync(tp);
	return xfs_trans_commit(tp);
}

int
xfs_set_acl(struct user_namespace *mnt_userns, struct inode *inode,
	    struct posix_acl *acl, int type)
>>>>>>> upstream/android-13
{
	umode_t mode;
	bool set_mode = false;
	int error = 0;

	if (!acl)
		goto set_acl;

	error = -E2BIG;
	if (acl->a_count > XFS_ACL_MAX_ENTRIES(XFS_M(inode->i_sb)))
		return error;

	if (type == ACL_TYPE_ACCESS) {
<<<<<<< HEAD
		error = posix_acl_update_mode(inode, &mode, &acl);
=======
		error = posix_acl_update_mode(mnt_userns, inode, &mode, &acl);
>>>>>>> upstream/android-13
		if (error)
			return error;
		set_mode = true;
	}

 set_acl:
<<<<<<< HEAD
	error =  __xfs_set_acl(inode, acl, type);
	if (error)
		return error;

=======
>>>>>>> upstream/android-13
	/*
	 * We set the mode after successfully updating the ACL xattr because the
	 * xattr update can fail at ENOSPC and we don't want to change the mode
	 * if the ACL update hasn't been applied.
	 */
<<<<<<< HEAD
	if (set_mode)
		error = xfs_set_mode(inode, mode);

	return error;
}
=======
	error =  __xfs_set_acl(inode, acl, type);
	if (!error && set_mode && mode != inode->i_mode)
		error = xfs_acl_set_mode(inode, mode);
	return error;
}

/*
 * Invalidate any cached ACLs if the user has bypassed the ACL interface.
 * We don't validate the content whatsoever so it is caller responsibility to
 * provide data in valid format and ensure i_mode is consistent.
 */
void
xfs_forget_acl(
	struct inode		*inode,
	const char		*name)
{
	if (!strcmp(name, SGI_ACL_FILE))
		forget_cached_acl(inode, ACL_TYPE_ACCESS);
	else if (!strcmp(name, SGI_ACL_DEFAULT))
		forget_cached_acl(inode, ACL_TYPE_DEFAULT);
}
>>>>>>> upstream/android-13
