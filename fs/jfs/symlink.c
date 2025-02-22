<<<<<<< HEAD
/*
 *   Copyright (C) Christoph Hellwig, 2001-2002
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
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *   Copyright (C) Christoph Hellwig, 2001-2002
>>>>>>> upstream/android-13
 */

#include <linux/fs.h>
#include "jfs_incore.h"
#include "jfs_inode.h"
#include "jfs_xattr.h"

const struct inode_operations jfs_fast_symlink_inode_operations = {
	.get_link	= simple_get_link,
	.setattr	= jfs_setattr,
	.listxattr	= jfs_listxattr,
};

const struct inode_operations jfs_symlink_inode_operations = {
	.get_link	= page_get_link,
	.setattr	= jfs_setattr,
	.listxattr	= jfs_listxattr,
};

