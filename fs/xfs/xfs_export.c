// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2004-2005 Silicon Graphics, Inc.
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
<<<<<<< HEAD
#include "xfs_da_format.h"
#include "xfs_da_btree.h"
=======
>>>>>>> upstream/android-13
#include "xfs_dir2.h"
#include "xfs_export.h"
#include "xfs_inode.h"
#include "xfs_trans.h"
#include "xfs_inode_item.h"
<<<<<<< HEAD
#include "xfs_trace.h"
#include "xfs_icache.h"
#include "xfs_log.h"
=======
#include "xfs_icache.h"
>>>>>>> upstream/android-13
#include "xfs_pnfs.h"

/*
 * Note that we only accept fileids which are long enough rather than allow
 * the parent generation number to default to zero.  XFS considers zero a
 * valid generation number not an invalid/wildcard value.
 */
static int xfs_fileid_length(int fileid_type)
{
	switch (fileid_type) {
	case FILEID_INO32_GEN:
		return 2;
	case FILEID_INO32_GEN_PARENT:
		return 4;
	case FILEID_INO32_GEN | XFS_FILEID_TYPE_64FLAG:
		return 3;
	case FILEID_INO32_GEN_PARENT | XFS_FILEID_TYPE_64FLAG:
		return 6;
	}
	return FILEID_INVALID;
}

STATIC int
xfs_fs_encode_fh(
	struct inode	*inode,
	__u32		*fh,
	int		*max_len,
	struct inode	*parent)
{
<<<<<<< HEAD
=======
	struct xfs_mount	*mp = XFS_M(inode->i_sb);
>>>>>>> upstream/android-13
	struct fid		*fid = (struct fid *)fh;
	struct xfs_fid64	*fid64 = (struct xfs_fid64 *)fh;
	int			fileid_type;
	int			len;

	/* Directories don't need their parent encoded, they have ".." */
	if (!parent)
		fileid_type = FILEID_INO32_GEN;
	else
		fileid_type = FILEID_INO32_GEN_PARENT;

	/*
<<<<<<< HEAD
	 * If the the filesystem may contain 64bit inode numbers, we need
=======
	 * If the filesystem may contain 64bit inode numbers, we need
>>>>>>> upstream/android-13
	 * to use larger file handles that can represent them.
	 *
	 * While we only allocate inodes that do not fit into 32 bits any
	 * large enough filesystem may contain them, thus the slightly
	 * confusing looking conditional below.
	 */
<<<<<<< HEAD
	if (!(XFS_M(inode->i_sb)->m_flags & XFS_MOUNT_SMALL_INUMS) ||
	    (XFS_M(inode->i_sb)->m_flags & XFS_MOUNT_32BITINODES))
=======
	if (!xfs_has_small_inums(mp) || xfs_is_inode32(mp))
>>>>>>> upstream/android-13
		fileid_type |= XFS_FILEID_TYPE_64FLAG;

	/*
	 * Only encode if there is enough space given.  In practice
	 * this means we can't export a filesystem with 64bit inodes
	 * over NFSv2 with the subtree_check export option; the other
	 * seven combinations work.  The real answer is "don't use v2".
	 */
	len = xfs_fileid_length(fileid_type);
	if (*max_len < len) {
		*max_len = len;
		return FILEID_INVALID;
	}
	*max_len = len;

	switch (fileid_type) {
	case FILEID_INO32_GEN_PARENT:
		fid->i32.parent_ino = XFS_I(parent)->i_ino;
		fid->i32.parent_gen = parent->i_generation;
<<<<<<< HEAD
		/*FALLTHRU*/
=======
		fallthrough;
>>>>>>> upstream/android-13
	case FILEID_INO32_GEN:
		fid->i32.ino = XFS_I(inode)->i_ino;
		fid->i32.gen = inode->i_generation;
		break;
	case FILEID_INO32_GEN_PARENT | XFS_FILEID_TYPE_64FLAG:
		fid64->parent_ino = XFS_I(parent)->i_ino;
		fid64->parent_gen = parent->i_generation;
<<<<<<< HEAD
		/*FALLTHRU*/
=======
		fallthrough;
>>>>>>> upstream/android-13
	case FILEID_INO32_GEN | XFS_FILEID_TYPE_64FLAG:
		fid64->ino = XFS_I(inode)->i_ino;
		fid64->gen = inode->i_generation;
		break;
	}

	return fileid_type;
}

STATIC struct inode *
xfs_nfs_get_inode(
	struct super_block	*sb,
	u64			ino,
	u32			generation)
{
 	xfs_mount_t		*mp = XFS_M(sb);
	xfs_inode_t		*ip;
	int			error;

	/*
	 * NFS can sometimes send requests for ino 0.  Fail them gracefully.
	 */
	if (ino == 0)
		return ERR_PTR(-ESTALE);

	/*
	 * The XFS_IGET_UNTRUSTED means that an invalid inode number is just
	 * fine and not an indication of a corrupted filesystem as clients can
	 * send invalid file handles and we have to handle it gracefully..
	 */
	error = xfs_iget(mp, NULL, ino, XFS_IGET_UNTRUSTED, 0, &ip);
	if (error) {

		/*
		 * EINVAL means the inode cluster doesn't exist anymore.
		 * EFSCORRUPTED means the metadata pointing to the inode cluster
		 * or the inode cluster itself is corrupt.  This implies the
		 * filehandle is stale, so we should translate it here.
		 * We don't use ESTALE directly down the chain to not
		 * confuse applications using bulkstat that expect EINVAL.
		 */
		switch (error) {
		case -EINVAL:
		case -ENOENT:
		case -EFSCORRUPTED:
			error = -ESTALE;
			break;
		default:
			break;
		}
		return ERR_PTR(error);
	}

	if (VFS_I(ip)->i_generation != generation) {
		xfs_irele(ip);
		return ERR_PTR(-ESTALE);
	}

	return VFS_I(ip);
}

STATIC struct dentry *
xfs_fs_fh_to_dentry(struct super_block *sb, struct fid *fid,
		 int fh_len, int fileid_type)
{
	struct xfs_fid64	*fid64 = (struct xfs_fid64 *)fid;
	struct inode		*inode = NULL;

	if (fh_len < xfs_fileid_length(fileid_type))
		return NULL;

	switch (fileid_type) {
	case FILEID_INO32_GEN_PARENT:
	case FILEID_INO32_GEN:
		inode = xfs_nfs_get_inode(sb, fid->i32.ino, fid->i32.gen);
		break;
	case FILEID_INO32_GEN_PARENT | XFS_FILEID_TYPE_64FLAG:
	case FILEID_INO32_GEN | XFS_FILEID_TYPE_64FLAG:
		inode = xfs_nfs_get_inode(sb, fid64->ino, fid64->gen);
		break;
	}

	return d_obtain_alias(inode);
}

STATIC struct dentry *
xfs_fs_fh_to_parent(struct super_block *sb, struct fid *fid,
		 int fh_len, int fileid_type)
{
	struct xfs_fid64	*fid64 = (struct xfs_fid64 *)fid;
	struct inode		*inode = NULL;

	if (fh_len < xfs_fileid_length(fileid_type))
		return NULL;

	switch (fileid_type) {
	case FILEID_INO32_GEN_PARENT:
		inode = xfs_nfs_get_inode(sb, fid->i32.parent_ino,
					      fid->i32.parent_gen);
		break;
	case FILEID_INO32_GEN_PARENT | XFS_FILEID_TYPE_64FLAG:
		inode = xfs_nfs_get_inode(sb, fid64->parent_ino,
					      fid64->parent_gen);
		break;
	}

	return d_obtain_alias(inode);
}

STATIC struct dentry *
xfs_fs_get_parent(
	struct dentry		*child)
{
	int			error;
	struct xfs_inode	*cip;

	error = xfs_lookup(XFS_I(d_inode(child)), &xfs_name_dotdot, &cip, NULL);
	if (unlikely(error))
		return ERR_PTR(error);

	return d_obtain_alias(VFS_I(cip));
}

STATIC int
xfs_fs_nfs_commit_metadata(
	struct inode		*inode)
{
<<<<<<< HEAD
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_mount	*mp = ip->i_mount;
	xfs_lsn_t		lsn = 0;

	xfs_ilock(ip, XFS_ILOCK_SHARED);
	if (xfs_ipincount(ip))
		lsn = ip->i_itemp->ili_last_lsn;
	xfs_iunlock(ip, XFS_ILOCK_SHARED);

	if (!lsn)
		return 0;
	return xfs_log_force_lsn(mp, lsn, XFS_LOG_SYNC, NULL);
=======
	return xfs_log_force_inode(XFS_I(inode));
>>>>>>> upstream/android-13
}

const struct export_operations xfs_export_operations = {
	.encode_fh		= xfs_fs_encode_fh,
	.fh_to_dentry		= xfs_fs_fh_to_dentry,
	.fh_to_parent		= xfs_fs_fh_to_parent,
	.get_parent		= xfs_fs_get_parent,
	.commit_metadata	= xfs_fs_nfs_commit_metadata,
#ifdef CONFIG_EXPORTFS_BLOCK_OPS
	.get_uuid		= xfs_fs_get_uuid,
	.map_blocks		= xfs_fs_map_blocks,
	.commit_blocks		= xfs_fs_commit_blocks,
#endif
};
