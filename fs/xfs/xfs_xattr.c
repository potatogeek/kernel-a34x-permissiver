// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2008 Christoph Hellwig.
 * Portions Copyright (C) 2000-2008 Silicon Graphics, Inc.
 */

#include "xfs.h"
<<<<<<< HEAD
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_da_format.h"
#include "xfs_inode.h"
#include "xfs_attr.h"
#include "xfs_attr_leaf.h"
#include "xfs_acl.h"

#include <linux/posix_acl_xattr.h>
#include <linux/xattr.h>
=======
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_da_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
#include "xfs_attr.h"
#include "xfs_acl.h"
#include "xfs_da_btree.h"

#include <linux/posix_acl_xattr.h>
>>>>>>> upstream/android-13


static int
xfs_xattr_get(const struct xattr_handler *handler, struct dentry *unused,
		struct inode *inode, const char *name, void *value, size_t size)
{
<<<<<<< HEAD
	int xflags = handler->flags;
	struct xfs_inode *ip = XFS_I(inode);
	int error, asize = size;

	/* Convert Linux syscall to XFS internal ATTR flags */
	if (!size) {
		xflags |= ATTR_KERNOVAL;
		value = NULL;
	}

	error = xfs_attr_get(ip, (unsigned char *)name, value, &asize, xflags);
	if (error)
		return error;
	return asize;
}

void
xfs_forget_acl(
	struct inode		*inode,
	const char		*name,
	int			xflags)
{
	/*
	 * Invalidate any cached ACLs if the user has bypassed the ACL
	 * interface. We don't validate the content whatsoever so it is caller
	 * responsibility to provide data in valid format and ensure i_mode is
	 * consistent.
	 */
	if (xflags & ATTR_ROOT) {
#ifdef CONFIG_XFS_POSIX_ACL
		if (!strcmp(name, SGI_ACL_FILE))
			forget_cached_acl(inode, ACL_TYPE_ACCESS);
		else if (!strcmp(name, SGI_ACL_DEFAULT))
			forget_cached_acl(inode, ACL_TYPE_DEFAULT);
#endif
	}
}

static int
xfs_xattr_set(const struct xattr_handler *handler, struct dentry *unused,
		struct inode *inode, const char *name, const void *value,
		size_t size, int flags)
{
	int			xflags = handler->flags;
	struct xfs_inode	*ip = XFS_I(inode);
	int			error;

	/* Convert Linux syscall to XFS internal ATTR flags */
	if (flags & XATTR_CREATE)
		xflags |= ATTR_CREATE;
	if (flags & XATTR_REPLACE)
		xflags |= ATTR_REPLACE;

	if (!value)
		return xfs_attr_remove(ip, (unsigned char *)name, xflags);
	error = xfs_attr_set(ip, (unsigned char *)name,
				(void *)value, size, xflags);
	if (!error)
		xfs_forget_acl(inode, name, xflags);

=======
	struct xfs_da_args	args = {
		.dp		= XFS_I(inode),
		.attr_filter	= handler->flags,
		.name		= name,
		.namelen	= strlen(name),
		.value		= value,
		.valuelen	= size,
	};
	int			error;

	error = xfs_attr_get(&args);
	if (error)
		return error;
	return args.valuelen;
}

static int
xfs_xattr_set(const struct xattr_handler *handler,
	      struct user_namespace *mnt_userns, struct dentry *unused,
	      struct inode *inode, const char *name, const void *value,
	      size_t size, int flags)
{
	struct xfs_da_args	args = {
		.dp		= XFS_I(inode),
		.attr_filter	= handler->flags,
		.attr_flags	= flags,
		.name		= name,
		.namelen	= strlen(name),
		.value		= (void *)value,
		.valuelen	= size,
	};
	int			error;

	error = xfs_attr_set(&args);
	if (!error && (handler->flags & XFS_ATTR_ROOT))
		xfs_forget_acl(inode, name);
>>>>>>> upstream/android-13
	return error;
}

static const struct xattr_handler xfs_xattr_user_handler = {
	.prefix	= XATTR_USER_PREFIX,
	.flags	= 0, /* no flags implies user namespace */
	.get	= xfs_xattr_get,
	.set	= xfs_xattr_set,
};

static const struct xattr_handler xfs_xattr_trusted_handler = {
	.prefix	= XATTR_TRUSTED_PREFIX,
<<<<<<< HEAD
	.flags	= ATTR_ROOT,
=======
	.flags	= XFS_ATTR_ROOT,
>>>>>>> upstream/android-13
	.get	= xfs_xattr_get,
	.set	= xfs_xattr_set,
};

static const struct xattr_handler xfs_xattr_security_handler = {
	.prefix	= XATTR_SECURITY_PREFIX,
<<<<<<< HEAD
	.flags	= ATTR_SECURE,
=======
	.flags	= XFS_ATTR_SECURE,
>>>>>>> upstream/android-13
	.get	= xfs_xattr_get,
	.set	= xfs_xattr_set,
};

const struct xattr_handler *xfs_xattr_handlers[] = {
	&xfs_xattr_user_handler,
	&xfs_xattr_trusted_handler,
	&xfs_xattr_security_handler,
#ifdef CONFIG_XFS_POSIX_ACL
	&posix_acl_access_xattr_handler,
	&posix_acl_default_xattr_handler,
#endif
	NULL
};

static void
__xfs_xattr_put_listent(
	struct xfs_attr_list_context *context,
	char *prefix,
	int prefix_len,
	unsigned char *name,
	int namelen)
{
	char *offset;
	int arraytop;

	if (context->count < 0 || context->seen_enough)
		return;

<<<<<<< HEAD
	if (!context->alist)
=======
	if (!context->buffer)
>>>>>>> upstream/android-13
		goto compute_size;

	arraytop = context->count + prefix_len + namelen + 1;
	if (arraytop > context->firstu) {
		context->count = -1;	/* insufficient space */
		context->seen_enough = 1;
		return;
	}
<<<<<<< HEAD
	offset = (char *)context->alist + context->count;
=======
	offset = context->buffer + context->count;
>>>>>>> upstream/android-13
	strncpy(offset, prefix, prefix_len);
	offset += prefix_len;
	strncpy(offset, (char *)name, namelen);			/* real name */
	offset += namelen;
	*offset = '\0';

compute_size:
	context->count += prefix_len + namelen + 1;
	return;
}

static void
xfs_xattr_put_listent(
	struct xfs_attr_list_context *context,
	int		flags,
	unsigned char	*name,
	int		namelen,
	int		valuelen)
{
	char *prefix;
	int prefix_len;

	ASSERT(context->count >= 0);

	if (flags & XFS_ATTR_ROOT) {
#ifdef CONFIG_XFS_POSIX_ACL
		if (namelen == SGI_ACL_FILE_SIZE &&
		    strncmp(name, SGI_ACL_FILE,
			    SGI_ACL_FILE_SIZE) == 0) {
			__xfs_xattr_put_listent(
					context, XATTR_SYSTEM_PREFIX,
					XATTR_SYSTEM_PREFIX_LEN,
					XATTR_POSIX_ACL_ACCESS,
					strlen(XATTR_POSIX_ACL_ACCESS));
		} else if (namelen == SGI_ACL_DEFAULT_SIZE &&
			 strncmp(name, SGI_ACL_DEFAULT,
				 SGI_ACL_DEFAULT_SIZE) == 0) {
			__xfs_xattr_put_listent(
					context, XATTR_SYSTEM_PREFIX,
					XATTR_SYSTEM_PREFIX_LEN,
					XATTR_POSIX_ACL_DEFAULT,
					strlen(XATTR_POSIX_ACL_DEFAULT));
		}
#endif

		/*
		 * Only show root namespace entries if we are actually allowed to
		 * see them.
		 */
		if (!capable(CAP_SYS_ADMIN))
			return;

		prefix = XATTR_TRUSTED_PREFIX;
		prefix_len = XATTR_TRUSTED_PREFIX_LEN;
	} else if (flags & XFS_ATTR_SECURE) {
		prefix = XATTR_SECURITY_PREFIX;
		prefix_len = XATTR_SECURITY_PREFIX_LEN;
	} else {
		prefix = XATTR_USER_PREFIX;
		prefix_len = XATTR_USER_PREFIX_LEN;
	}

	__xfs_xattr_put_listent(context, prefix, prefix_len, name,
				namelen);
	return;
}

ssize_t
xfs_vn_listxattr(
	struct dentry	*dentry,
	char		*data,
	size_t		size)
{
	struct xfs_attr_list_context context;
<<<<<<< HEAD
	struct attrlist_cursor_kern cursor = { 0 };
=======
>>>>>>> upstream/android-13
	struct inode	*inode = d_inode(dentry);
	int		error;

	/*
	 * First read the regular on-disk attributes.
	 */
	memset(&context, 0, sizeof(context));
	context.dp = XFS_I(inode);
<<<<<<< HEAD
	context.cursor = &cursor;
	context.resynch = 1;
	context.alist = size ? data : NULL;
=======
	context.resynch = 1;
	context.buffer = size ? data : NULL;
>>>>>>> upstream/android-13
	context.bufsize = size;
	context.firstu = context.bufsize;
	context.put_listent = xfs_xattr_put_listent;

<<<<<<< HEAD
	error = xfs_attr_list_int(&context);
=======
	error = xfs_attr_list(&context);
>>>>>>> upstream/android-13
	if (error)
		return error;
	if (context.count < 0)
		return -ERANGE;

	return context.count;
}
