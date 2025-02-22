<<<<<<< HEAD
/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
>>>>>>> upstream/android-13
 * namei.h
 *
 * Function prototypes
 *
 * Copyright (C) 2002, 2004 Oracle.  All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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

#ifndef OCFS2_NAMEI_H
#define OCFS2_NAMEI_H

#define OCFS2_DIO_ORPHAN_PREFIX "dio-"
#define OCFS2_DIO_ORPHAN_PREFIX_LEN 4

extern const struct inode_operations ocfs2_dir_iops;

struct dentry *ocfs2_get_parent(struct dentry *child);

int ocfs2_orphan_del(struct ocfs2_super *osb,
		     handle_t *handle,
		     struct inode *orphan_dir_inode,
		     struct inode *inode,
		     struct buffer_head *orphan_dir_bh,
		     bool dio);
int ocfs2_create_inode_in_orphan(struct inode *dir,
				 int mode,
				 struct inode **new_inode);
int ocfs2_add_inode_to_orphan(struct ocfs2_super *osb,
		struct inode *inode);
int ocfs2_del_inode_from_orphan(struct ocfs2_super *osb,
		struct inode *inode, struct buffer_head *di_bh,
		int update_isize, loff_t end);
int ocfs2_mv_orphaned_inode_to_new(struct inode *dir,
				   struct inode *new_inode,
				   struct dentry *new_dentry);

#endif /* OCFS2_NAMEI_H */
