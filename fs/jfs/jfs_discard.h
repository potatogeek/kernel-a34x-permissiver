<<<<<<< HEAD
/*
 *   Copyright (C) Tino Reichardt, 2012
 *
 *   This program is free software;  you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program;  if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 *   Copyright (C) Tino Reichardt, 2012
>>>>>>> upstream/android-13
 */
#ifndef _H_JFS_DISCARD
#define _H_JFS_DISCARD

struct fstrim_range;

extern void jfs_issue_discard(struct inode *ip, u64 blkno, u64 nblocks);
extern int jfs_ioc_trim(struct inode *ip, struct fstrim_range *range);

#endif /* _H_JFS_DISCARD */
