<<<<<<< HEAD
/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
 * acl.h
 *
 * Copyright (C) 2004, 2008 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * acl.h
 *
 * Copyright (C) 2004, 2008 Oracle.  All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef OCFS2_ACL_H
#define OCFS2_ACL_H

#include <linux/posix_acl_xattr.h>

struct ocfs2_acl_entry {
	__le16 e_tag;
	__le16 e_perm;
	__le32 e_id;
};

<<<<<<< HEAD
struct posix_acl *ocfs2_iop_get_acl(struct inode *inode, int type);
int ocfs2_iop_set_acl(struct inode *inode, struct posix_acl *acl, int type);
=======
struct posix_acl *ocfs2_iop_get_acl(struct inode *inode, int type, bool rcu);
int ocfs2_iop_set_acl(struct user_namespace *mnt_userns, struct inode *inode,
		      struct posix_acl *acl, int type);
>>>>>>> upstream/android-13
extern int ocfs2_acl_chmod(struct inode *, struct buffer_head *);
extern int ocfs2_init_acl(handle_t *, struct inode *, struct inode *,
			  struct buffer_head *, struct buffer_head *,
			  struct ocfs2_alloc_context *,
			  struct ocfs2_alloc_context *);

#endif /* OCFS2_ACL_H */
