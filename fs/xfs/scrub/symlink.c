// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2017 Oracle.  All Rights Reserved.
 * Author: Darrick J. Wong <darrick.wong@oracle.com>
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
<<<<<<< HEAD
#include "xfs_defer.h"
#include "xfs_btree.h"
#include "xfs_bit.h"
#include "xfs_log_format.h"
#include "xfs_trans.h"
#include "xfs_sb.h"
#include "xfs_inode.h"
#include "xfs_inode_fork.h"
#include "xfs_symlink.h"
#include "scrub/xfs_scrub.h"
#include "scrub/scrub.h"
#include "scrub/common.h"
#include "scrub/trace.h"
=======
#include "xfs_log_format.h"
#include "xfs_inode.h"
#include "xfs_symlink.h"
#include "scrub/scrub.h"
#include "scrub/common.h"
>>>>>>> upstream/android-13

/* Set us up to scrub a symbolic link. */
int
xchk_setup_symlink(
<<<<<<< HEAD
	struct xfs_scrub	*sc,
	struct xfs_inode	*ip)
{
	/* Allocate the buffer without the inode lock held. */
	sc->buf = kmem_zalloc_large(XFS_SYMLINK_MAXLEN + 1, KM_SLEEP);
	if (!sc->buf)
		return -ENOMEM;

	return xchk_setup_inode_contents(sc, ip, 0);
=======
	struct xfs_scrub	*sc)
{
	/* Allocate the buffer without the inode lock held. */
	sc->buf = kvzalloc(XFS_SYMLINK_MAXLEN + 1, GFP_KERNEL);
	if (!sc->buf)
		return -ENOMEM;

	return xchk_setup_inode_contents(sc, 0);
>>>>>>> upstream/android-13
}

/* Symbolic links. */

int
xchk_symlink(
	struct xfs_scrub	*sc)
{
	struct xfs_inode	*ip = sc->ip;
	struct xfs_ifork	*ifp;
	loff_t			len;
	int			error = 0;

	if (!S_ISLNK(VFS_I(ip)->i_mode))
		return -ENOENT;
	ifp = XFS_IFORK_PTR(ip, XFS_DATA_FORK);
<<<<<<< HEAD
	len = ip->i_d.di_size;
=======
	len = ip->i_disk_size;
>>>>>>> upstream/android-13

	/* Plausible size? */
	if (len > XFS_SYMLINK_MAXLEN || len <= 0) {
		xchk_fblock_set_corrupt(sc, XFS_DATA_FORK, 0);
		goto out;
	}

	/* Inline symlink? */
<<<<<<< HEAD
	if (ifp->if_flags & XFS_IFINLINE) {
=======
	if (ifp->if_format == XFS_DINODE_FMT_LOCAL) {
>>>>>>> upstream/android-13
		if (len > XFS_IFORK_DSIZE(ip) ||
		    len > strnlen(ifp->if_u1.if_data, XFS_IFORK_DSIZE(ip)))
			xchk_fblock_set_corrupt(sc, XFS_DATA_FORK, 0);
		goto out;
	}

	/* Remote symlink; must read the contents. */
	error = xfs_readlink_bmap_ilocked(sc->ip, sc->buf);
	if (!xchk_fblock_process_error(sc, XFS_DATA_FORK, 0, &error))
		goto out;
	if (strnlen(sc->buf, XFS_SYMLINK_MAXLEN) < len)
		xchk_fblock_set_corrupt(sc, XFS_DATA_FORK, 0);
out:
	return error;
}
