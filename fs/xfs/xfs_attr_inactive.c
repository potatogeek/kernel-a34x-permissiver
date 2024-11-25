// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * Copyright (c) 2013 Red Hat, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_bit.h"
#include "xfs_mount.h"
#include "xfs_da_format.h"
#include "xfs_da_btree.h"
#include "xfs_inode.h"
<<<<<<< HEAD
#include "xfs_alloc.h"
#include "xfs_attr_remote.h"
#include "xfs_trans.h"
#include "xfs_inode_item.h"
#include "xfs_bmap.h"
#include "xfs_attr.h"
#include "xfs_attr_leaf.h"
#include "xfs_error.h"
#include "xfs_quota.h"
#include "xfs_trace.h"
#include "xfs_dir2.h"
#include "xfs_defer.h"

/*
 * Look at all the extents for this logical region,
 * invalidate any buffers that are incore/in transactions.
 */
STATIC int
xfs_attr3_leaf_freextent(
	struct xfs_trans	**trans,
=======
#include "xfs_attr.h"
#include "xfs_attr_remote.h"
#include "xfs_trans.h"
#include "xfs_bmap.h"
#include "xfs_attr_leaf.h"
#include "xfs_quota.h"
#include "xfs_dir2.h"
#include "xfs_error.h"

/*
 * Invalidate any incore buffers associated with this remote attribute value
 * extent.   We never log remote attribute value buffers, which means that they
 * won't be attached to a transaction and are therefore safe to mark stale.
 * The actual bunmapi will be taken care of later.
 */
STATIC int
xfs_attr3_rmt_stale(
>>>>>>> upstream/android-13
	struct xfs_inode	*dp,
	xfs_dablk_t		blkno,
	int			blkcnt)
{
	struct xfs_bmbt_irec	map;
<<<<<<< HEAD
	struct xfs_buf		*bp;
	xfs_dablk_t		tblkno;
	xfs_daddr_t		dblkno;
	int			tblkcnt;
	int			dblkcnt;
=======
>>>>>>> upstream/android-13
	int			nmap;
	int			error;

	/*
	 * Roll through the "value", invalidating the attribute value's
	 * blocks.
	 */
<<<<<<< HEAD
	tblkno = blkno;
	tblkcnt = blkcnt;
	while (tblkcnt > 0) {
=======
	while (blkcnt > 0) {
>>>>>>> upstream/android-13
		/*
		 * Try to remember where we decided to put the value.
		 */
		nmap = 1;
<<<<<<< HEAD
		error = xfs_bmapi_read(dp, (xfs_fileoff_t)tblkno, tblkcnt,
				       &map, &nmap, XFS_BMAPI_ATTRFORK);
		if (error) {
			return error;
		}
		ASSERT(nmap == 1);
		ASSERT(map.br_startblock != DELAYSTARTBLOCK);

		/*
		 * If it's a hole, these are already unmapped
		 * so there's nothing to invalidate.
		 */
		if (map.br_startblock != HOLESTARTBLOCK) {

			dblkno = XFS_FSB_TO_DADDR(dp->i_mount,
						  map.br_startblock);
			dblkcnt = XFS_FSB_TO_BB(dp->i_mount,
						map.br_blockcount);
			bp = xfs_trans_get_buf(*trans,
					dp->i_mount->m_ddev_targp,
					dblkno, dblkcnt, 0);
			if (!bp)
				return -ENOMEM;
			xfs_trans_binval(*trans, bp);
			/*
			 * Roll to next transaction.
			 */
			error = xfs_trans_roll_inode(trans, dp);
			if (error)
				return error;
		}

		tblkno += map.br_blockcount;
		tblkcnt -= map.br_blockcount;
=======
		error = xfs_bmapi_read(dp, (xfs_fileoff_t)blkno, blkcnt,
				       &map, &nmap, XFS_BMAPI_ATTRFORK);
		if (error)
			return error;
		if (XFS_IS_CORRUPT(dp->i_mount, nmap != 1))
			return -EFSCORRUPTED;

		/*
		 * Mark any incore buffers for the remote value as stale.  We
		 * never log remote attr value buffers, so the buffer should be
		 * easy to kill.
		 */
		error = xfs_attr_rmtval_stale(dp, &map, 0);
		if (error)
			return error;

		blkno += map.br_blockcount;
		blkcnt -= map.br_blockcount;
>>>>>>> upstream/android-13
	}

	return 0;
}

/*
 * Invalidate all of the "remote" value regions pointed to by a particular
 * leaf block.
 * Note that we must release the lock on the buffer so that we are not
 * caught holding something that the logging code wants to flush to disk.
 */
STATIC int
xfs_attr3_leaf_inactive(
<<<<<<< HEAD
	struct xfs_trans	**trans,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_name_remote *name_rmt;
	struct xfs_attr_inactive_list *list;
	struct xfs_attr_inactive_list *lp;
	int			error;
	int			count;
	int			size;
	int			tmp;
	int			i;
	struct xfs_mount	*mp = bp->b_target->bt_mount;

	leaf = bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(mp->m_attr_geo, &ichdr, leaf);

	/*
	 * Count the number of "remote" value extents.
	 */
	count = 0;
	entry = xfs_attr3_leaf_entryp(leaf);
	for (i = 0; i < ichdr.count; entry++, i++) {
		if (be16_to_cpu(entry->nameidx) &&
		    ((entry->flags & XFS_ATTR_LOCAL) == 0)) {
			name_rmt = xfs_attr3_leaf_name_remote(leaf, i);
			if (name_rmt->valueblk)
				count++;
		}
	}

	/*
	 * If there are no "remote" values, we're done.
	 */
	if (count == 0) {
		xfs_trans_brelse(*trans, bp);
		return 0;
	}

	/*
	 * Allocate storage for a list of all the "remote" value extents.
	 */
	size = count * sizeof(xfs_attr_inactive_list_t);
	list = kmem_alloc(size, KM_SLEEP);

	/*
	 * Identify each of the "remote" value extents.
	 */
	lp = list;
	entry = xfs_attr3_leaf_entryp(leaf);
	for (i = 0; i < ichdr.count; entry++, i++) {
		if (be16_to_cpu(entry->nameidx) &&
		    ((entry->flags & XFS_ATTR_LOCAL) == 0)) {
			name_rmt = xfs_attr3_leaf_name_remote(leaf, i);
			if (name_rmt->valueblk) {
				lp->valueblk = be32_to_cpu(name_rmt->valueblk);
				lp->valuelen = xfs_attr3_rmt_blocks(dp->i_mount,
						    be32_to_cpu(name_rmt->valuelen));
				lp++;
			}
		}
	}
	xfs_trans_brelse(*trans, bp);	/* unlock for trans. in freextent() */

	/*
	 * Invalidate each of the "remote" value extents.
	 */
	error = 0;
	for (lp = list, i = 0; i < count; i++, lp++) {
		tmp = xfs_attr3_leaf_freextent(trans, dp,
				lp->valueblk, lp->valuelen);

		if (error == 0)
			error = tmp;	/* save only the 1st errno */
	}

	kmem_free(list);
=======
	struct xfs_trans		**trans,
	struct xfs_inode		*dp,
	struct xfs_buf			*bp)
{
	struct xfs_attr3_icleaf_hdr	ichdr;
	struct xfs_mount		*mp = bp->b_mount;
	struct xfs_attr_leafblock	*leaf = bp->b_addr;
	struct xfs_attr_leaf_entry	*entry;
	struct xfs_attr_leaf_name_remote *name_rmt;
	int				error = 0;
	int				i;

	xfs_attr3_leaf_hdr_from_disk(mp->m_attr_geo, &ichdr, leaf);

	/*
	 * Find the remote value extents for this leaf and invalidate their
	 * incore buffers.
	 */
	entry = xfs_attr3_leaf_entryp(leaf);
	for (i = 0; i < ichdr.count; entry++, i++) {
		int		blkcnt;

		if (!entry->nameidx || (entry->flags & XFS_ATTR_LOCAL))
			continue;

		name_rmt = xfs_attr3_leaf_name_remote(leaf, i);
		if (!name_rmt->valueblk)
			continue;

		blkcnt = xfs_attr3_rmt_blocks(dp->i_mount,
				be32_to_cpu(name_rmt->valuelen));
		error = xfs_attr3_rmt_stale(dp,
				be32_to_cpu(name_rmt->valueblk), blkcnt);
		if (error)
			goto err;
	}

	xfs_trans_brelse(*trans, bp);
err:
>>>>>>> upstream/android-13
	return error;
}

/*
 * Recurse (gasp!) through the attribute nodes until we find leaves.
 * We're doing a depth-first traversal in order to invalidate everything.
 */
STATIC int
xfs_attr3_node_inactive(
<<<<<<< HEAD
	struct xfs_trans **trans,
	struct xfs_inode *dp,
	struct xfs_buf	*bp,
	int		level)
{
	xfs_da_blkinfo_t *info;
	xfs_da_intnode_t *node;
	xfs_dablk_t child_fsb;
	xfs_daddr_t parent_blkno, child_blkno;
	int error, i;
	struct xfs_buf *child_bp;
	struct xfs_da_node_entry *btree;
	struct xfs_da3_icnode_hdr ichdr;
=======
	struct xfs_trans	**trans,
	struct xfs_inode	*dp,
	struct xfs_buf		*bp,
	int			level)
{
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_da_blkinfo	*info;
	xfs_dablk_t		child_fsb;
	xfs_daddr_t		parent_blkno, child_blkno;
	struct xfs_buf		*child_bp;
	struct xfs_da3_icnode_hdr ichdr;
	int			error, i;
>>>>>>> upstream/android-13

	/*
	 * Since this code is recursive (gasp!) we must protect ourselves.
	 */
	if (level > XFS_DA_NODE_MAXDEPTH) {
<<<<<<< HEAD
		xfs_trans_brelse(*trans, bp);	/* no locks for later trans */
		return -EIO;
	}

	node = bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&ichdr, node);
	parent_blkno = bp->b_bn;
=======
		xfs_buf_mark_corrupt(bp);
		xfs_trans_brelse(*trans, bp);	/* no locks for later trans */
		return -EFSCORRUPTED;
	}

	xfs_da3_node_hdr_from_disk(dp->i_mount, &ichdr, bp->b_addr);
	parent_blkno = xfs_buf_daddr(bp);
>>>>>>> upstream/android-13
	if (!ichdr.count) {
		xfs_trans_brelse(*trans, bp);
		return 0;
	}
<<<<<<< HEAD
	btree = dp->d_ops->node_tree_p(node);
	child_fsb = be32_to_cpu(btree[0].before);
=======
	child_fsb = be32_to_cpu(ichdr.btree[0].before);
>>>>>>> upstream/android-13
	xfs_trans_brelse(*trans, bp);	/* no locks for later trans */

	/*
	 * If this is the node level just above the leaves, simply loop
	 * over the leaves removing all of them.  If this is higher up
	 * in the tree, recurse downward.
	 */
	for (i = 0; i < ichdr.count; i++) {
		/*
		 * Read the subsidiary block to see what we have to work with.
		 * Don't do this in a transaction.  This is a depth-first
		 * traversal of the tree so we may deal with many blocks
		 * before we come back to this one.
		 */
<<<<<<< HEAD
		error = xfs_da3_node_read(*trans, dp, child_fsb, -1, &child_bp,
=======
		error = xfs_da3_node_read(*trans, dp, child_fsb, &child_bp,
>>>>>>> upstream/android-13
					  XFS_ATTR_FORK);
		if (error)
			return error;

		/* save for re-read later */
<<<<<<< HEAD
		child_blkno = XFS_BUF_ADDR(child_bp);
=======
		child_blkno = xfs_buf_daddr(child_bp);
>>>>>>> upstream/android-13

		/*
		 * Invalidate the subtree, however we have to.
		 */
		info = child_bp->b_addr;
		switch (info->magic) {
		case cpu_to_be16(XFS_DA_NODE_MAGIC):
		case cpu_to_be16(XFS_DA3_NODE_MAGIC):
			error = xfs_attr3_node_inactive(trans, dp, child_bp,
							level + 1);
			break;
		case cpu_to_be16(XFS_ATTR_LEAF_MAGIC):
		case cpu_to_be16(XFS_ATTR3_LEAF_MAGIC):
			error = xfs_attr3_leaf_inactive(trans, dp, child_bp);
			break;
		default:
<<<<<<< HEAD
			error = -EIO;
			xfs_trans_brelse(*trans, child_bp);
=======
			xfs_buf_mark_corrupt(child_bp);
			xfs_trans_brelse(*trans, child_bp);
			error = -EFSCORRUPTED;
>>>>>>> upstream/android-13
			break;
		}
		if (error)
			return error;

		/*
		 * Remove the subsidiary block from the cache and from the log.
		 */
<<<<<<< HEAD
		error = xfs_da_get_buf(*trans, dp, 0, child_blkno, &child_bp,
				       XFS_ATTR_FORK);
		if (error)
			return error;
=======
		error = xfs_trans_get_buf(*trans, mp->m_ddev_targp,
				child_blkno,
				XFS_FSB_TO_BB(mp, mp->m_attr_geo->fsbcount), 0,
				&child_bp);
		if (error)
			return error;
		error = bp->b_error;
		if (error) {
			xfs_trans_brelse(*trans, child_bp);
			return error;
		}
>>>>>>> upstream/android-13
		xfs_trans_binval(*trans, child_bp);

		/*
		 * If we're not done, re-read the parent to get the next
		 * child block number.
		 */
		if (i + 1 < ichdr.count) {
<<<<<<< HEAD
			error = xfs_da3_node_read(*trans, dp, 0, parent_blkno,
						 &bp, XFS_ATTR_FORK);
			if (error)
				return error;
			node = bp->b_addr;
			btree = dp->d_ops->node_tree_p(node);
			child_fsb = be32_to_cpu(btree[i + 1].before);
=======
			struct xfs_da3_icnode_hdr phdr;

			error = xfs_da3_node_read_mapped(*trans, dp,
					parent_blkno, &bp, XFS_ATTR_FORK);
			if (error)
				return error;
			xfs_da3_node_hdr_from_disk(dp->i_mount, &phdr,
						  bp->b_addr);
			child_fsb = be32_to_cpu(phdr.btree[i + 1].before);
>>>>>>> upstream/android-13
			xfs_trans_brelse(*trans, bp);
		}
		/*
		 * Atomically commit the whole invalidate stuff.
		 */
		error = xfs_trans_roll_inode(trans, dp);
		if (error)
			return  error;
	}

	return 0;
}

/*
 * Indiscriminately delete the entire attribute fork
 *
 * Recurse (gasp!) through the attribute nodes until we find leaves.
 * We're doing a depth-first traversal in order to invalidate everything.
 */
static int
xfs_attr3_root_inactive(
	struct xfs_trans	**trans,
	struct xfs_inode	*dp)
{
<<<<<<< HEAD
=======
	struct xfs_mount	*mp = dp->i_mount;
>>>>>>> upstream/android-13
	struct xfs_da_blkinfo	*info;
	struct xfs_buf		*bp;
	xfs_daddr_t		blkno;
	int			error;

	/*
	 * Read block 0 to see what we have to work with.
	 * We only get here if we have extents, since we remove
	 * the extents in reverse order the extent containing
	 * block 0 must still be there.
	 */
<<<<<<< HEAD
	error = xfs_da3_node_read(*trans, dp, 0, -1, &bp, XFS_ATTR_FORK);
	if (error)
		return error;
	blkno = bp->b_bn;
=======
	error = xfs_da3_node_read(*trans, dp, 0, &bp, XFS_ATTR_FORK);
	if (error)
		return error;
	blkno = xfs_buf_daddr(bp);
>>>>>>> upstream/android-13

	/*
	 * Invalidate the tree, even if the "tree" is only a single leaf block.
	 * This is a depth-first traversal!
	 */
	info = bp->b_addr;
	switch (info->magic) {
	case cpu_to_be16(XFS_DA_NODE_MAGIC):
	case cpu_to_be16(XFS_DA3_NODE_MAGIC):
		error = xfs_attr3_node_inactive(trans, dp, bp, 1);
		break;
	case cpu_to_be16(XFS_ATTR_LEAF_MAGIC):
	case cpu_to_be16(XFS_ATTR3_LEAF_MAGIC):
		error = xfs_attr3_leaf_inactive(trans, dp, bp);
		break;
	default:
<<<<<<< HEAD
		error = -EIO;
=======
		error = -EFSCORRUPTED;
		xfs_buf_mark_corrupt(bp);
>>>>>>> upstream/android-13
		xfs_trans_brelse(*trans, bp);
		break;
	}
	if (error)
		return error;

	/*
	 * Invalidate the incore copy of the root block.
	 */
<<<<<<< HEAD
	error = xfs_da_get_buf(*trans, dp, 0, blkno, &bp, XFS_ATTR_FORK);
	if (error)
		return error;
=======
	error = xfs_trans_get_buf(*trans, mp->m_ddev_targp, blkno,
			XFS_FSB_TO_BB(mp, mp->m_attr_geo->fsbcount), 0, &bp);
	if (error)
		return error;
	error = bp->b_error;
	if (error) {
		xfs_trans_brelse(*trans, bp);
		return error;
	}
>>>>>>> upstream/android-13
	xfs_trans_binval(*trans, bp);	/* remove from cache */
	/*
	 * Commit the invalidate and start the next transaction.
	 */
	error = xfs_trans_roll_inode(trans, dp);

	return error;
}

/*
 * xfs_attr_inactive kills all traces of an attribute fork on an inode. It
 * removes both the on-disk and in-memory inode fork. Note that this also has to
 * handle the condition of inodes without attributes but with an attribute fork
 * configured, so we can't use xfs_inode_hasattr() here.
 *
 * The in-memory attribute fork is removed even on error.
 */
int
xfs_attr_inactive(
	struct xfs_inode	*dp)
{
	struct xfs_trans	*trans;
	struct xfs_mount	*mp;
	int			lock_mode = XFS_ILOCK_SHARED;
	int			error = 0;

	mp = dp->i_mount;
	ASSERT(! XFS_NOT_DQATTACHED(mp, dp));

	xfs_ilock(dp, lock_mode);
	if (!XFS_IFORK_Q(dp))
		goto out_destroy_fork;
	xfs_iunlock(dp, lock_mode);

	lock_mode = 0;

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_attrinval, 0, 0, 0, &trans);
	if (error)
		goto out_destroy_fork;

	lock_mode = XFS_ILOCK_EXCL;
	xfs_ilock(dp, lock_mode);

	if (!XFS_IFORK_Q(dp))
		goto out_cancel;

	/*
	 * No need to make quota reservations here. We expect to release some
	 * blocks, not allocate, in the common case.
	 */
	xfs_trans_ijoin(trans, dp, 0);

	/*
	 * Invalidate and truncate the attribute fork extents. Make sure the
	 * fork actually has attributes as otherwise the invalidation has no
	 * blocks to read and returns an error. In this case, just do the fork
	 * removal below.
	 */
	if (xfs_inode_hasattr(dp) &&
<<<<<<< HEAD
	    dp->i_d.di_aformat != XFS_DINODE_FMT_LOCAL) {
=======
	    dp->i_afp->if_format != XFS_DINODE_FMT_LOCAL) {
>>>>>>> upstream/android-13
		error = xfs_attr3_root_inactive(&trans, dp);
		if (error)
			goto out_cancel;

		error = xfs_itruncate_extents(&trans, dp, XFS_ATTR_FORK, 0);
		if (error)
			goto out_cancel;
	}

	/* Reset the attribute fork - this also destroys the in-core fork */
	xfs_attr_fork_remove(dp, trans);

	error = xfs_trans_commit(trans);
	xfs_iunlock(dp, lock_mode);
	return error;

out_cancel:
	xfs_trans_cancel(trans);
out_destroy_fork:
	/* kill the in-core attr fork before we drop the inode lock */
<<<<<<< HEAD
	if (dp->i_afp)
		xfs_idestroy_fork(dp, XFS_ATTR_FORK);
=======
	if (dp->i_afp) {
		xfs_idestroy_fork(dp->i_afp);
		kmem_cache_free(xfs_ifork_zone, dp->i_afp);
		dp->i_afp = NULL;
	}
>>>>>>> upstream/android-13
	if (lock_mode)
		xfs_iunlock(dp, lock_mode);
	return error;
}
