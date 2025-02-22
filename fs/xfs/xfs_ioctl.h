// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2008 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef __XFS_IOCTL_H__
#define __XFS_IOCTL_H__

<<<<<<< HEAD
extern int
xfs_ioc_space(
	struct file		*filp,
	unsigned int		cmd,
=======
struct xfs_bstat;
struct xfs_ibulk;
struct xfs_inogrp;


extern int
xfs_ioc_space(
	struct file		*filp,
>>>>>>> upstream/android-13
	xfs_flock64_t		*bf);

int
xfs_ioc_swapext(
	xfs_swapext_t	*sxp);

extern int
xfs_find_handle(
	unsigned int		cmd,
	xfs_fsop_handlereq_t	*hreq);

extern int
xfs_open_by_handle(
	struct file		*parfilp,
	xfs_fsop_handlereq_t	*hreq);

extern int
xfs_readlink_by_handle(
	struct file		*parfilp,
	xfs_fsop_handlereq_t	*hreq);

<<<<<<< HEAD
extern int
xfs_attrmulti_attr_get(
	struct inode		*inode,
	unsigned char		*name,
	unsigned char		__user *ubuf,
	uint32_t		*len,
	uint32_t		flags);

extern int
xfs_attrmulti_attr_set(
	struct inode		*inode,
	unsigned char		*name,
	const unsigned char	__user *ubuf,
	uint32_t		len,
	uint32_t		flags);

extern int
xfs_attrmulti_attr_remove(
	struct inode		*inode,
	unsigned char		*name,
	uint32_t		flags);
=======
int xfs_ioc_attrmulti_one(struct file *parfilp, struct inode *inode,
		uint32_t opcode, void __user *uname, void __user *value,
		uint32_t *len, uint32_t flags);
int xfs_ioc_attr_list(struct xfs_inode *dp, void __user *ubuf, int bufsize,
	int flags, struct xfs_attrlist_cursor __user *ucursor);
>>>>>>> upstream/android-13

extern struct dentry *
xfs_handle_to_dentry(
	struct file		*parfilp,
	void __user		*uhandle,
	u32			hlen);

<<<<<<< HEAD
=======
extern int
xfs_fileattr_get(
	struct dentry		*dentry,
	struct fileattr		*fa);

extern int
xfs_fileattr_set(
	struct user_namespace	*mnt_userns,
	struct dentry		*dentry,
	struct fileattr		*fa);

>>>>>>> upstream/android-13
extern long
xfs_file_ioctl(
	struct file		*filp,
	unsigned int		cmd,
	unsigned long		p);

extern long
xfs_file_compat_ioctl(
	struct file		*file,
	unsigned int		cmd,
	unsigned long		arg);

<<<<<<< HEAD
extern int
xfs_set_dmattrs(
	struct xfs_inode	*ip,
	uint			evmask,
	uint16_t		state);
=======
int xfs_fsbulkstat_one_fmt(struct xfs_ibulk *breq,
			   const struct xfs_bulkstat *bstat);
int xfs_fsinumbers_fmt(struct xfs_ibulk *breq, const struct xfs_inumbers *igrp);
>>>>>>> upstream/android-13

#endif
