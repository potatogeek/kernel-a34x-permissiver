<<<<<<< HEAD
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
>>>>>>> upstream/android-13
 */

#ifndef __ACL_DOT_H__
#define __ACL_DOT_H__

#include "incore.h"

#define GFS2_ACL_MAX_ENTRIES(sdp) ((300 << (sdp)->sd_sb.sb_bsize_shift) >> 12)

<<<<<<< HEAD
extern struct posix_acl *gfs2_get_acl(struct inode *inode, int type);
extern int __gfs2_set_acl(struct inode *inode, struct posix_acl *acl, int type);
extern int gfs2_set_acl(struct inode *inode, struct posix_acl *acl, int type);
=======
extern struct posix_acl *gfs2_get_acl(struct inode *inode, int type, bool rcu);
extern int __gfs2_set_acl(struct inode *inode, struct posix_acl *acl, int type);
extern int gfs2_set_acl(struct user_namespace *mnt_userns, struct inode *inode,
			struct posix_acl *acl, int type);
>>>>>>> upstream/android-13

#endif /* __ACL_DOT_H__ */
