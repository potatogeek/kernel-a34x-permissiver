// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
<<<<<<< HEAD
#include "xfs_bit.h"
=======
>>>>>>> upstream/android-13
#include "xfs_mount.h"
#include "xfs_defer.h"
#include "xfs_da_format.h"
#include "xfs_da_btree.h"
#include "xfs_attr_sf.h"
#include "xfs_inode.h"
<<<<<<< HEAD
#include "xfs_alloc.h"
#include "xfs_trans.h"
#include "xfs_inode_item.h"
#include "xfs_bmap.h"
#include "xfs_bmap_util.h"
=======
#include "xfs_trans.h"
#include "xfs_bmap.h"
>>>>>>> upstream/android-13
#include "xfs_bmap_btree.h"
#include "xfs_attr.h"
#include "xfs_attr_leaf.h"
#include "xfs_attr_remote.h"
<<<<<<< HEAD
#include "xfs_error.h"
=======
>>>>>>> upstream/android-13
#include "xfs_quota.h"
#include "xfs_trans_space.h"
#include "xfs_trace.h"

/*
 * xfs_attr.c
 *
 * Provide the external interfaces to manage attribute lists.
 */

/*========================================================================
 * Function prototypes for the kernel.
 *========================================================================*/

/*
 * Internal routines when attribute list fits inside the inode.
 */
STATIC int xfs_attr_shortform_addname(xfs_da_args_t *args);

/*
 * Internal routines when attribute list is one block.
 */
STATIC int xfs_attr_leaf_get(xfs_da_args_t *args);
<<<<<<< HEAD
STATIC int xfs_attr_leaf_addname(xfs_da_args_t *args);
STATIC int xfs_attr_leaf_removename(xfs_da_args_t *args);
=======
STATIC int xfs_attr_leaf_removename(xfs_da_args_t *args);
STATIC int xfs_attr_leaf_hasname(struct xfs_da_args *args, struct xfs_buf **bp);
STATIC int xfs_attr_leaf_try_add(struct xfs_da_args *args, struct xfs_buf *bp);
>>>>>>> upstream/android-13

/*
 * Internal routines when attribute list is more than one block.
 */
STATIC int xfs_attr_node_get(xfs_da_args_t *args);
<<<<<<< HEAD
STATIC int xfs_attr_node_addname(xfs_da_args_t *args);
STATIC int xfs_attr_node_removename(xfs_da_args_t *args);
STATIC int xfs_attr_fillstate(xfs_da_state_t *state);
STATIC int xfs_attr_refillstate(xfs_da_state_t *state);


STATIC int
xfs_attr_args_init(
	struct xfs_da_args	*args,
	struct xfs_inode	*dp,
	const unsigned char	*name,
	int			flags)
{

	if (!name)
		return -EINVAL;

	memset(args, 0, sizeof(*args));
	args->geo = dp->i_mount->m_attr_geo;
	args->whichfork = XFS_ATTR_FORK;
	args->dp = dp;
	args->flags = flags;
	args->name = name;
	args->namelen = strlen((const char *)name);
	if (args->namelen >= MAXNAMELEN)
		return -EFAULT;		/* match IRIX behaviour */

	args->hashval = xfs_da_hashname(args->name, args->namelen);
	return 0;
}
=======
STATIC void xfs_attr_restore_rmt_blk(struct xfs_da_args *args);
STATIC int xfs_attr_node_addname(struct xfs_delattr_context *dac);
STATIC int xfs_attr_node_addname_find_attr(struct xfs_delattr_context *dac);
STATIC int xfs_attr_node_addname_clear_incomplete(
				struct xfs_delattr_context *dac);
STATIC int xfs_attr_node_hasname(xfs_da_args_t *args,
				 struct xfs_da_state **state);
STATIC int xfs_attr_fillstate(xfs_da_state_t *state);
STATIC int xfs_attr_refillstate(xfs_da_state_t *state);
STATIC int xfs_attr_set_iter(struct xfs_delattr_context *dac,
			     struct xfs_buf **leaf_bp);
STATIC int xfs_attr_node_removename(struct xfs_da_args *args,
				    struct xfs_da_state *state);
>>>>>>> upstream/android-13

int
xfs_inode_hasattr(
	struct xfs_inode	*ip)
{
	if (!XFS_IFORK_Q(ip) ||
<<<<<<< HEAD
	    (ip->i_d.di_aformat == XFS_DINODE_FMT_EXTENTS &&
	     ip->i_d.di_anextents == 0))
=======
	    (ip->i_afp->if_format == XFS_DINODE_FMT_EXTENTS &&
	     ip->i_afp->if_nextents == 0))
>>>>>>> upstream/android-13
		return 0;
	return 1;
}

<<<<<<< HEAD
=======
/*
 * Returns true if the there is exactly only block in the attr fork, in which
 * case the attribute fork consists of a single leaf block entry.
 */
bool
xfs_attr_is_leaf(
	struct xfs_inode	*ip)
{
	struct xfs_ifork	*ifp = ip->i_afp;
	struct xfs_iext_cursor	icur;
	struct xfs_bmbt_irec	imap;

	if (ifp->if_nextents != 1 || ifp->if_format != XFS_DINODE_FMT_EXTENTS)
		return false;

	xfs_iext_first(ifp, &icur);
	xfs_iext_get_extent(ifp, &icur, &imap);
	return imap.br_startoff == 0 && imap.br_blockcount == 1;
}

>>>>>>> upstream/android-13
/*========================================================================
 * Overall external interface routines.
 *========================================================================*/

<<<<<<< HEAD
/* Retrieve an extended attribute and its value.  Must have ilock. */
int
xfs_attr_get_ilocked(
	struct xfs_inode	*ip,
	struct xfs_da_args	*args)
{
	ASSERT(xfs_isilocked(ip, XFS_ILOCK_SHARED | XFS_ILOCK_EXCL));

	if (!xfs_inode_hasattr(ip))
		return -ENOATTR;
	else if (ip->i_d.di_aformat == XFS_DINODE_FMT_LOCAL)
		return xfs_attr_shortform_getvalue(args);
	else if (xfs_bmap_one_block(ip, XFS_ATTR_FORK))
		return xfs_attr_leaf_get(args);
	else
		return xfs_attr_node_get(args);
}

/* Retrieve an extended attribute by name, and its value. */
int
xfs_attr_get(
	struct xfs_inode	*ip,
	const unsigned char	*name,
	unsigned char		*value,
	int			*valuelenp,
	int			flags)
{
	struct xfs_da_args	args;
	uint			lock_mode;
	int			error;

	XFS_STATS_INC(ip->i_mount, xs_attr_get);

	if (XFS_FORCED_SHUTDOWN(ip->i_mount))
		return -EIO;

	error = xfs_attr_args_init(&args, ip, name, flags);
	if (error)
		return error;

	args.value = value;
	args.valuelen = *valuelenp;
	/* Entirely possible to look up a name which doesn't exist */
	args.op_flags = XFS_DA_OP_OKNOENT;

	lock_mode = xfs_ilock_attr_map_shared(ip);
	error = xfs_attr_get_ilocked(ip, &args);
	xfs_iunlock(ip, lock_mode);

	*valuelenp = args.valuelen;
	return error == -EEXIST ? 0 : error;
=======
/*
 * Retrieve an extended attribute and its value.  Must have ilock.
 * Returns 0 on successful retrieval, otherwise an error.
 */
int
xfs_attr_get_ilocked(
	struct xfs_da_args	*args)
{
	ASSERT(xfs_isilocked(args->dp, XFS_ILOCK_SHARED | XFS_ILOCK_EXCL));

	if (!xfs_inode_hasattr(args->dp))
		return -ENOATTR;

	if (args->dp->i_afp->if_format == XFS_DINODE_FMT_LOCAL)
		return xfs_attr_shortform_getvalue(args);
	if (xfs_attr_is_leaf(args->dp))
		return xfs_attr_leaf_get(args);
	return xfs_attr_node_get(args);
}

/*
 * Retrieve an extended attribute by name, and its value if requested.
 *
 * If args->valuelen is zero, then the caller does not want the value, just an
 * indication whether the attribute exists and the size of the value if it
 * exists. The size is returned in args.valuelen.
 *
 * If args->value is NULL but args->valuelen is non-zero, allocate the buffer
 * for the value after existence of the attribute has been determined. The
 * caller always has to free args->value if it is set, no matter if this
 * function was successful or not.
 *
 * If the attribute is found, but exceeds the size limit set by the caller in
 * args->valuelen, return -ERANGE with the size of the attribute that was found
 * in args->valuelen.
 */
int
xfs_attr_get(
	struct xfs_da_args	*args)
{
	uint			lock_mode;
	int			error;

	XFS_STATS_INC(args->dp->i_mount, xs_attr_get);

	if (xfs_is_shutdown(args->dp->i_mount))
		return -EIO;

	args->geo = args->dp->i_mount->m_attr_geo;
	args->whichfork = XFS_ATTR_FORK;
	args->hashval = xfs_da_hashname(args->name, args->namelen);

	/* Entirely possible to look up a name which doesn't exist */
	args->op_flags = XFS_DA_OP_OKNOENT;

	lock_mode = xfs_ilock_attr_map_shared(args->dp);
	error = xfs_attr_get_ilocked(args);
	xfs_iunlock(args->dp, lock_mode);

	return error;
>>>>>>> upstream/android-13
}

/*
 * Calculate how many blocks we need for the new attribute,
 */
STATIC int
xfs_attr_calc_size(
	struct xfs_da_args	*args,
	int			*local)
{
	struct xfs_mount	*mp = args->dp->i_mount;
	int			size;
	int			nblks;

	/*
	 * Determine space new attribute will use, and if it would be
	 * "local" or "remote" (note: local != inline).
	 */
	size = xfs_attr_leaf_newentsize(args, local);
	nblks = XFS_DAENTER_SPACE_RES(mp, XFS_ATTR_FORK);
	if (*local) {
		if (size > (args->geo->blksize / 2)) {
			/* Double split possible */
			nblks *= 2;
		}
	} else {
		/*
		 * Out of line attribute, cannot double split, but
		 * make room for the attribute value itself.
		 */
		uint	dblocks = xfs_attr3_rmt_blocks(mp, args->valuelen);
		nblks += dblocks;
		nblks += XFS_NEXTENTADD_SPACE_RES(mp, dblocks, XFS_ATTR_FORK);
	}

	return nblks;
}

STATIC int
xfs_attr_try_sf_addname(
	struct xfs_inode	*dp,
	struct xfs_da_args	*args)
{

<<<<<<< HEAD
	struct xfs_mount	*mp = dp->i_mount;
	int			error, error2;
=======
	int			error;

	/*
	 * Build initial attribute list (if required).
	 */
	if (dp->i_afp->if_format == XFS_DINODE_FMT_EXTENTS)
		xfs_attr_shortform_create(args);
>>>>>>> upstream/android-13

	error = xfs_attr_shortform_addname(args);
	if (error == -ENOSPC)
		return error;

	/*
	 * Commit the shortform mods, and we're done.
	 * NOTE: this is also the error path (EEXIST, etc).
	 */
<<<<<<< HEAD
	if (!error && (args->flags & ATTR_KERNOTIME) == 0)
		xfs_trans_ichgtime(args->trans, dp, XFS_ICHGTIME_CHG);

	if (mp->m_flags & XFS_MOUNT_WSYNC)
		xfs_trans_set_sync(args->trans);

	error2 = xfs_trans_commit(args->trans);
	args->trans = NULL;
	return error ? error : error2;
=======
	if (!error && !(args->op_flags & XFS_DA_OP_NOTIME))
		xfs_trans_ichgtime(args->trans, dp, XFS_ICHGTIME_CHG);

	if (xfs_has_wsync(dp->i_mount))
		xfs_trans_set_sync(args->trans);

	return error;
}

/*
 * Check to see if the attr should be upgraded from non-existent or shortform to
 * single-leaf-block attribute list.
 */
static inline bool
xfs_attr_is_shortform(
	struct xfs_inode    *ip)
{
	return ip->i_afp->if_format == XFS_DINODE_FMT_LOCAL ||
	       (ip->i_afp->if_format == XFS_DINODE_FMT_EXTENTS &&
		ip->i_afp->if_nextents == 0);
}

/*
 * Checks to see if a delayed attribute transaction should be rolled.  If so,
 * transaction is finished or rolled as needed.
 */
STATIC int
xfs_attr_trans_roll(
	struct xfs_delattr_context	*dac)
{
	struct xfs_da_args		*args = dac->da_args;
	int				error;

	if (dac->flags & XFS_DAC_DEFER_FINISH) {
		/*
		 * The caller wants us to finish all the deferred ops so that we
		 * avoid pinning the log tail with a large number of deferred
		 * ops.
		 */
		dac->flags &= ~XFS_DAC_DEFER_FINISH;
		error = xfs_defer_finish(&args->trans);
	} else
		error = xfs_trans_roll_inode(&args->trans, args->dp);

	return error;
>>>>>>> upstream/android-13
}

/*
 * Set the attribute specified in @args.
 */
int
xfs_attr_set_args(
<<<<<<< HEAD
	struct xfs_da_args	*args)
{
	struct xfs_inode	*dp = args->dp;
	struct xfs_buf          *leaf_bp = NULL;
	int			error;

	/*
	 * If the attribute list is non-existent or a shortform list,
	 * upgrade it to a single-leaf-block attribute list.
	 */
	if (dp->i_d.di_aformat == XFS_DINODE_FMT_LOCAL ||
	    (dp->i_d.di_aformat == XFS_DINODE_FMT_EXTENTS &&
	     dp->i_d.di_anextents == 0)) {

		/*
		 * Build initial attribute list (if required).
		 */
		if (dp->i_d.di_aformat == XFS_DINODE_FMT_EXTENTS)
			xfs_attr_shortform_create(args);

		/*
		 * Try to add the attr to the attribute list in the inode.
		 */
		error = xfs_attr_try_sf_addname(dp, args);
		if (error != -ENOSPC)
			return error;

		/*
		 * It won't fit in the shortform, transform to a leaf block.
		 * GROT: another possible req'mt for a double-split btree op.
		 */
		error = xfs_attr_shortform_to_leaf(args, &leaf_bp);
=======
	struct xfs_da_args		*args)
{
	struct xfs_buf			*leaf_bp = NULL;
	int				error = 0;
	struct xfs_delattr_context	dac = {
		.da_args	= args,
	};

	do {
		error = xfs_attr_set_iter(&dac, &leaf_bp);
		if (error != -EAGAIN)
			break;

		error = xfs_attr_trans_roll(&dac);
		if (error) {
			if (leaf_bp)
				xfs_trans_brelse(args->trans, leaf_bp);
			return error;
		}
	} while (true);

	return error;
}

STATIC int
xfs_attr_sf_addname(
	struct xfs_delattr_context	*dac,
	struct xfs_buf			**leaf_bp)
{
	struct xfs_da_args		*args = dac->da_args;
	struct xfs_inode		*dp = args->dp;
	int				error = 0;

	/*
	 * Try to add the attr to the attribute list in the inode.
	 */
	error = xfs_attr_try_sf_addname(dp, args);

	/* Should only be 0, -EEXIST or -ENOSPC */
	if (error != -ENOSPC)
		return error;

	/*
	 * It won't fit in the shortform, transform to a leaf block.  GROT:
	 * another possible req'mt for a double-split btree op.
	 */
	error = xfs_attr_shortform_to_leaf(args, leaf_bp);
	if (error)
		return error;

	/*
	 * Prevent the leaf buffer from being unlocked so that a concurrent AIL
	 * push cannot grab the half-baked leaf buffer and run into problems
	 * with the write verifier.
	 */
	xfs_trans_bhold(args->trans, *leaf_bp);

	/*
	 * We're still in XFS_DAS_UNINIT state here.  We've converted
	 * the attr fork to leaf format and will restart with the leaf
	 * add.
	 */
	trace_xfs_attr_sf_addname_return(XFS_DAS_UNINIT, args->dp);
	dac->flags |= XFS_DAC_DEFER_FINISH;
	return -EAGAIN;
}

/*
 * Set the attribute specified in @args.
 * This routine is meant to function as a delayed operation, and may return
 * -EAGAIN when the transaction needs to be rolled.  Calling functions will need
 * to handle this, and recall the function until a successful error code is
 * returned.
 */
int
xfs_attr_set_iter(
	struct xfs_delattr_context	*dac,
	struct xfs_buf			**leaf_bp)
{
	struct xfs_da_args              *args = dac->da_args;
	struct xfs_inode		*dp = args->dp;
	struct xfs_buf			*bp = NULL;
	int				forkoff, error = 0;

	/* State machine switch */
	switch (dac->dela_state) {
	case XFS_DAS_UNINIT:
		/*
		 * If the fork is shortform, attempt to add the attr. If there
		 * is no space, this converts to leaf format and returns
		 * -EAGAIN with the leaf buffer held across the roll. The caller
		 * will deal with a transaction roll error, but otherwise
		 * release the hold once we return with a clean transaction.
		 */
		if (xfs_attr_is_shortform(dp))
			return xfs_attr_sf_addname(dac, leaf_bp);
		if (*leaf_bp != NULL) {
			xfs_trans_bhold_release(args->trans, *leaf_bp);
			*leaf_bp = NULL;
		}

		if (xfs_attr_is_leaf(dp)) {
			error = xfs_attr_leaf_try_add(args, *leaf_bp);
			if (error == -ENOSPC) {
				error = xfs_attr3_leaf_to_node(args);
				if (error)
					return error;

				/*
				 * Finish any deferred work items and roll the
				 * transaction once more.  The goal here is to
				 * call node_addname with the inode and
				 * transaction in the same state (inode locked
				 * and joined, transaction clean) no matter how
				 * we got to this step.
				 *
				 * At this point, we are still in
				 * XFS_DAS_UNINIT, but when we come back, we'll
				 * be a node, so we'll fall down into the node
				 * handling code below
				 */
				dac->flags |= XFS_DAC_DEFER_FINISH;
				trace_xfs_attr_set_iter_return(
					dac->dela_state, args->dp);
				return -EAGAIN;
			} else if (error) {
				return error;
			}

			dac->dela_state = XFS_DAS_FOUND_LBLK;
		} else {
			error = xfs_attr_node_addname_find_attr(dac);
			if (error)
				return error;

			error = xfs_attr_node_addname(dac);
			if (error)
				return error;

			dac->dela_state = XFS_DAS_FOUND_NBLK;
		}
		trace_xfs_attr_set_iter_return(dac->dela_state,	args->dp);
		return -EAGAIN;
	case XFS_DAS_FOUND_LBLK:
		/*
		 * If there was an out-of-line value, allocate the blocks we
		 * identified for its storage and copy the value.  This is done
		 * after we create the attribute so that we don't overflow the
		 * maximum size of a transaction and/or hit a deadlock.
		 */

		/* Open coded xfs_attr_rmtval_set without trans handling */
		if ((dac->flags & XFS_DAC_LEAF_ADDNAME_INIT) == 0) {
			dac->flags |= XFS_DAC_LEAF_ADDNAME_INIT;
			if (args->rmtblkno > 0) {
				error = xfs_attr_rmtval_find_space(dac);
				if (error)
					return error;
			}
		}

		/*
		 * Repeat allocating remote blocks for the attr value until
		 * blkcnt drops to zero.
		 */
		if (dac->blkcnt > 0) {
			error = xfs_attr_rmtval_set_blk(dac);
			if (error)
				return error;
			trace_xfs_attr_set_iter_return(dac->dela_state,
						       args->dp);
			return -EAGAIN;
		}

		error = xfs_attr_rmtval_set_value(args);
>>>>>>> upstream/android-13
		if (error)
			return error;

		/*
<<<<<<< HEAD
		 * Prevent the leaf buffer from being unlocked so that a
		 * concurrent AIL push cannot grab the half-baked leaf
		 * buffer and run into problems with the write verifier.
		 * Once we're done rolling the transaction we can release
		 * the hold and add the attr to the leaf.
		 */
		xfs_trans_bhold(args->trans, leaf_bp);
		error = xfs_defer_finish(&args->trans);
		xfs_trans_bhold_release(args->trans, leaf_bp);
		if (error) {
			xfs_trans_brelse(args->trans, leaf_bp);
			return error;
		}
	}

	if (xfs_bmap_one_block(dp, XFS_ATTR_FORK))
		error = xfs_attr_leaf_addname(args);
	else
		error = xfs_attr_node_addname(args);
	return error;
=======
		 * If this is not a rename, clear the incomplete flag and we're
		 * done.
		 */
		if (!(args->op_flags & XFS_DA_OP_RENAME)) {
			if (args->rmtblkno > 0)
				error = xfs_attr3_leaf_clearflag(args);
			return error;
		}

		/*
		 * If this is an atomic rename operation, we must "flip" the
		 * incomplete flags on the "new" and "old" attribute/value pairs
		 * so that one disappears and one appears atomically.  Then we
		 * must remove the "old" attribute/value pair.
		 *
		 * In a separate transaction, set the incomplete flag on the
		 * "old" attr and clear the incomplete flag on the "new" attr.
		 */
		error = xfs_attr3_leaf_flipflags(args);
		if (error)
			return error;
		/*
		 * Commit the flag value change and start the next trans in
		 * series.
		 */
		dac->dela_state = XFS_DAS_FLIP_LFLAG;
		trace_xfs_attr_set_iter_return(dac->dela_state, args->dp);
		return -EAGAIN;
	case XFS_DAS_FLIP_LFLAG:
		/*
		 * Dismantle the "old" attribute/value pair by removing a
		 * "remote" value (if it exists).
		 */
		xfs_attr_restore_rmt_blk(args);
		error = xfs_attr_rmtval_invalidate(args);
		if (error)
			return error;

		fallthrough;
	case XFS_DAS_RM_LBLK:
		/* Set state in case xfs_attr_rmtval_remove returns -EAGAIN */
		dac->dela_state = XFS_DAS_RM_LBLK;
		if (args->rmtblkno) {
			error = xfs_attr_rmtval_remove(dac);
			if (error == -EAGAIN)
				trace_xfs_attr_set_iter_return(
					dac->dela_state, args->dp);
			if (error)
				return error;

			dac->dela_state = XFS_DAS_RD_LEAF;
			trace_xfs_attr_set_iter_return(dac->dela_state, args->dp);
			return -EAGAIN;
		}

		fallthrough;
	case XFS_DAS_RD_LEAF:
		/*
		 * This is the last step for leaf format. Read the block with
		 * the old attr, remove the old attr, check for shortform
		 * conversion and return.
		 */
		error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno,
					   &bp);
		if (error)
			return error;

		xfs_attr3_leaf_remove(bp, args);

		forkoff = xfs_attr_shortform_allfit(bp, dp);
		if (forkoff)
			error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
			/* bp is gone due to xfs_da_shrink_inode */

		return error;

	case XFS_DAS_FOUND_NBLK:
		/*
		 * Find space for remote blocks and fall into the allocation
		 * state.
		 */
		if (args->rmtblkno > 0) {
			error = xfs_attr_rmtval_find_space(dac);
			if (error)
				return error;
		}

		fallthrough;
	case XFS_DAS_ALLOC_NODE:
		/*
		 * If there was an out-of-line value, allocate the blocks we
		 * identified for its storage and copy the value.  This is done
		 * after we create the attribute so that we don't overflow the
		 * maximum size of a transaction and/or hit a deadlock.
		 */
		dac->dela_state = XFS_DAS_ALLOC_NODE;
		if (args->rmtblkno > 0) {
			if (dac->blkcnt > 0) {
				error = xfs_attr_rmtval_set_blk(dac);
				if (error)
					return error;
				trace_xfs_attr_set_iter_return(
					dac->dela_state, args->dp);
				return -EAGAIN;
			}

			error = xfs_attr_rmtval_set_value(args);
			if (error)
				return error;
		}

		/*
		 * If this was not a rename, clear the incomplete flag and we're
		 * done.
		 */
		if (!(args->op_flags & XFS_DA_OP_RENAME)) {
			if (args->rmtblkno > 0)
				error = xfs_attr3_leaf_clearflag(args);
			goto out;
		}

		/*
		 * If this is an atomic rename operation, we must "flip" the
		 * incomplete flags on the "new" and "old" attribute/value pairs
		 * so that one disappears and one appears atomically.  Then we
		 * must remove the "old" attribute/value pair.
		 *
		 * In a separate transaction, set the incomplete flag on the
		 * "old" attr and clear the incomplete flag on the "new" attr.
		 */
		error = xfs_attr3_leaf_flipflags(args);
		if (error)
			goto out;
		/*
		 * Commit the flag value change and start the next trans in
		 * series
		 */
		dac->dela_state = XFS_DAS_FLIP_NFLAG;
		trace_xfs_attr_set_iter_return(dac->dela_state, args->dp);
		return -EAGAIN;

	case XFS_DAS_FLIP_NFLAG:
		/*
		 * Dismantle the "old" attribute/value pair by removing a
		 * "remote" value (if it exists).
		 */
		xfs_attr_restore_rmt_blk(args);

		error = xfs_attr_rmtval_invalidate(args);
		if (error)
			return error;

		fallthrough;
	case XFS_DAS_RM_NBLK:
		/* Set state in case xfs_attr_rmtval_remove returns -EAGAIN */
		dac->dela_state = XFS_DAS_RM_NBLK;
		if (args->rmtblkno) {
			error = xfs_attr_rmtval_remove(dac);
			if (error == -EAGAIN)
				trace_xfs_attr_set_iter_return(
					dac->dela_state, args->dp);

			if (error)
				return error;

			dac->dela_state = XFS_DAS_CLR_FLAG;
			trace_xfs_attr_set_iter_return(dac->dela_state, args->dp);
			return -EAGAIN;
		}

		fallthrough;
	case XFS_DAS_CLR_FLAG:
		/*
		 * The last state for node format. Look up the old attr and
		 * remove it.
		 */
		error = xfs_attr_node_addname_clear_incomplete(dac);
		break;
	default:
		ASSERT(0);
		break;
	}
out:
	return error;
}


/*
 * Return EEXIST if attr is found, or ENOATTR if not
 */
static int
xfs_attr_lookup(
	struct xfs_da_args	*args)
{
	struct xfs_inode	*dp = args->dp;
	struct xfs_buf		*bp = NULL;
	int			error;

	if (!xfs_inode_hasattr(dp))
		return -ENOATTR;

	if (dp->i_afp->if_format == XFS_DINODE_FMT_LOCAL)
		return xfs_attr_sf_findname(args, NULL, NULL);

	if (xfs_attr_is_leaf(dp)) {
		error = xfs_attr_leaf_hasname(args, &bp);

		if (bp)
			xfs_trans_brelse(args->trans, bp);

		return error;
	}

	return xfs_attr_node_hasname(args, NULL);
>>>>>>> upstream/android-13
}

/*
 * Remove the attribute specified in @args.
 */
int
xfs_attr_remove_args(
<<<<<<< HEAD
	struct xfs_da_args      *args)
{
	struct xfs_inode	*dp = args->dp;
	int			error;

	if (!xfs_inode_hasattr(dp)) {
		error = -ENOATTR;
	} else if (dp->i_d.di_aformat == XFS_DINODE_FMT_LOCAL) {
		ASSERT(dp->i_afp->if_flags & XFS_IFINLINE);
		error = xfs_attr_shortform_remove(args);
	} else if (xfs_bmap_one_block(dp, XFS_ATTR_FORK)) {
		error = xfs_attr_leaf_removename(args);
	} else {
		error = xfs_attr_node_removename(args);
	}
=======
	struct xfs_da_args	*args)
{
	int				error;
	struct xfs_delattr_context	dac = {
		.da_args	= args,
	};

	do {
		error = xfs_attr_remove_iter(&dac);
		if (error != -EAGAIN)
			break;

		error = xfs_attr_trans_roll(&dac);
		if (error)
			return error;

	} while (true);
>>>>>>> upstream/android-13

	return error;
}

<<<<<<< HEAD
int
xfs_attr_set(
	struct xfs_inode	*dp,
	const unsigned char	*name,
	unsigned char		*value,
	int			valuelen,
	int			flags)
{
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_da_args	args;
	struct xfs_trans_res	tres;
	int			rsvd = (flags & ATTR_ROOT) != 0;
	int			error, local;

	XFS_STATS_INC(mp, xs_attr_set);

	if (XFS_FORCED_SHUTDOWN(dp->i_mount))
		return -EIO;

	error = xfs_attr_args_init(&args, dp, name, flags);
	if (error)
		return error;

	args.value = value;
	args.valuelen = valuelen;
	args.op_flags = XFS_DA_OP_ADDNAME | XFS_DA_OP_OKNOENT;
	args.total = xfs_attr_calc_size(&args, &local);

=======
/*
 * Note: If args->value is NULL the attribute will be removed, just like the
 * Linux ->setattr API.
 */
int
xfs_attr_set(
	struct xfs_da_args	*args)
{
	struct xfs_inode	*dp = args->dp;
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_trans_res	tres;
	bool			rsvd = (args->attr_filter & XFS_ATTR_ROOT);
	int			error, local;
	int			rmt_blks = 0;
	unsigned int		total;

	if (xfs_is_shutdown(dp->i_mount))
		return -EIO;

>>>>>>> upstream/android-13
	error = xfs_qm_dqattach(dp);
	if (error)
		return error;

<<<<<<< HEAD
	/*
	 * If the inode doesn't have an attribute fork, add one.
	 * (inode must not be locked when we call this routine)
	 */
	if (XFS_IFORK_Q(dp) == 0) {
		int sf_size = sizeof(xfs_attr_sf_hdr_t) +
			XFS_ATTR_SF_ENTSIZE_BYNAME(args.namelen, valuelen);

		error = xfs_bmap_add_attrfork(dp, sf_size, rsvd);
		if (error)
			return error;
	}

	tres.tr_logres = M_RES(mp)->tr_attrsetm.tr_logres +
			 M_RES(mp)->tr_attrsetrt.tr_logres * args.total;
	tres.tr_logcount = XFS_ATTRSET_LOG_COUNT;
	tres.tr_logflags = XFS_TRANS_PERM_LOG_RES;

=======
	args->geo = mp->m_attr_geo;
	args->whichfork = XFS_ATTR_FORK;
	args->hashval = xfs_da_hashname(args->name, args->namelen);

	/*
	 * We have no control over the attribute names that userspace passes us
	 * to remove, so we have to allow the name lookup prior to attribute
	 * removal to fail as well.
	 */
	args->op_flags = XFS_DA_OP_OKNOENT;

	if (args->value) {
		XFS_STATS_INC(mp, xs_attr_set);

		args->op_flags |= XFS_DA_OP_ADDNAME;
		args->total = xfs_attr_calc_size(args, &local);

		/*
		 * If the inode doesn't have an attribute fork, add one.
		 * (inode must not be locked when we call this routine)
		 */
		if (XFS_IFORK_Q(dp) == 0) {
			int sf_size = sizeof(struct xfs_attr_sf_hdr) +
				xfs_attr_sf_entsize_byname(args->namelen,
						args->valuelen);

			error = xfs_bmap_add_attrfork(dp, sf_size, rsvd);
			if (error)
				return error;
		}

		tres.tr_logres = M_RES(mp)->tr_attrsetm.tr_logres +
				 M_RES(mp)->tr_attrsetrt.tr_logres *
					args->total;
		tres.tr_logcount = XFS_ATTRSET_LOG_COUNT;
		tres.tr_logflags = XFS_TRANS_PERM_LOG_RES;
		total = args->total;

		if (!local)
			rmt_blks = xfs_attr3_rmt_blocks(mp, args->valuelen);
	} else {
		XFS_STATS_INC(mp, xs_attr_remove);

		tres = M_RES(mp)->tr_attrrm;
		total = XFS_ATTRRM_SPACE_RES(mp);
		rmt_blks = xfs_attr3_rmt_blocks(mp, XFS_XATTR_SIZE_MAX);
	}

>>>>>>> upstream/android-13
	/*
	 * Root fork attributes can use reserved data blocks for this
	 * operation if necessary
	 */
<<<<<<< HEAD
	error = xfs_trans_alloc(mp, &tres, args.total, 0,
			rsvd ? XFS_TRANS_RESERVE : 0, &args.trans);
	if (error)
		return error;

	xfs_ilock(dp, XFS_ILOCK_EXCL);
	error = xfs_trans_reserve_quota_nblks(args.trans, dp, args.total, 0,
				rsvd ? XFS_QMOPT_RES_REGBLKS | XFS_QMOPT_FORCE_RES :
				       XFS_QMOPT_RES_REGBLKS);
	if (error)
		goto out_trans_cancel;

	xfs_trans_ijoin(args.trans, dp, 0);
	error = xfs_attr_set_args(&args);
	if (error)
		goto out_trans_cancel;
	if (!args.trans) {
		/* shortform attribute has already been committed */
		goto out_unlock;
=======
	error = xfs_trans_alloc_inode(dp, &tres, total, 0, rsvd, &args->trans);
	if (error)
		return error;

	if (args->value || xfs_inode_hasattr(dp)) {
		error = xfs_iext_count_may_overflow(dp, XFS_ATTR_FORK,
				XFS_IEXT_ATTR_MANIP_CNT(rmt_blks));
		if (error)
			goto out_trans_cancel;
	}

	error = xfs_attr_lookup(args);
	if (args->value) {
		if (error == -EEXIST && (args->attr_flags & XATTR_CREATE))
			goto out_trans_cancel;
		if (error == -ENOATTR && (args->attr_flags & XATTR_REPLACE))
			goto out_trans_cancel;
		if (error != -ENOATTR && error != -EEXIST)
			goto out_trans_cancel;

		error = xfs_attr_set_args(args);
		if (error)
			goto out_trans_cancel;
		/* shortform attribute has already been committed */
		if (!args->trans)
			goto out_unlock;
	} else {
		if (error != -EEXIST)
			goto out_trans_cancel;

		error = xfs_attr_remove_args(args);
		if (error)
			goto out_trans_cancel;
>>>>>>> upstream/android-13
	}

	/*
	 * If this is a synchronous mount, make sure that the
	 * transaction goes to disk before returning to the user.
	 */
<<<<<<< HEAD
	if (mp->m_flags & XFS_MOUNT_WSYNC)
		xfs_trans_set_sync(args.trans);

	if ((flags & ATTR_KERNOTIME) == 0)
		xfs_trans_ichgtime(args.trans, dp, XFS_ICHGTIME_CHG);
=======
	if (xfs_has_wsync(mp))
		xfs_trans_set_sync(args->trans);

	if (!(args->op_flags & XFS_DA_OP_NOTIME))
		xfs_trans_ichgtime(args->trans, dp, XFS_ICHGTIME_CHG);
>>>>>>> upstream/android-13

	/*
	 * Commit the last in the sequence of transactions.
	 */
<<<<<<< HEAD
	xfs_trans_log_inode(args.trans, dp, XFS_ILOG_CORE);
	error = xfs_trans_commit(args.trans);
=======
	xfs_trans_log_inode(args->trans, dp, XFS_ILOG_CORE);
	error = xfs_trans_commit(args->trans);
>>>>>>> upstream/android-13
out_unlock:
	xfs_iunlock(dp, XFS_ILOCK_EXCL);
	return error;

out_trans_cancel:
<<<<<<< HEAD
	if (args.trans)
		xfs_trans_cancel(args.trans);
	goto out_unlock;
}

/*
 * Generic handler routine to remove a name from an attribute list.
 * Transitions attribute list from Btree to shortform as necessary.
 */
int
xfs_attr_remove(
	struct xfs_inode	*dp,
	const unsigned char	*name,
	int			flags)
{
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_da_args	args;
	int			error;

	XFS_STATS_INC(mp, xs_attr_remove);

	if (XFS_FORCED_SHUTDOWN(dp->i_mount))
		return -EIO;

	error = xfs_attr_args_init(&args, dp, name, flags);
	if (error)
		return error;

	/*
	 * we have no control over the attribute names that userspace passes us
	 * to remove, so we have to allow the name lookup prior to attribute
	 * removal to fail.
	 */
	args.op_flags = XFS_DA_OP_OKNOENT;

	error = xfs_qm_dqattach(dp);
	if (error)
		return error;

	/*
	 * Root fork attributes can use reserved data blocks for this
	 * operation if necessary
	 */
	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_attrrm,
			XFS_ATTRRM_SPACE_RES(mp), 0,
			(flags & ATTR_ROOT) ? XFS_TRANS_RESERVE : 0,
			&args.trans);
	if (error)
		return error;

	xfs_ilock(dp, XFS_ILOCK_EXCL);
	/*
	 * No need to make quota reservations here. We expect to release some
	 * blocks not allocate in the common case.
	 */
	xfs_trans_ijoin(args.trans, dp, 0);

	error = xfs_attr_remove_args(&args);
	if (error)
		goto out;

	/*
	 * If this is a synchronous mount, make sure that the
	 * transaction goes to disk before returning to the user.
	 */
	if (mp->m_flags & XFS_MOUNT_WSYNC)
		xfs_trans_set_sync(args.trans);

	if ((flags & ATTR_KERNOTIME) == 0)
		xfs_trans_ichgtime(args.trans, dp, XFS_ICHGTIME_CHG);

	/*
	 * Commit the last in the sequence of transactions.
	 */
	xfs_trans_log_inode(args.trans, dp, XFS_ILOG_CORE);
	error = xfs_trans_commit(args.trans);
	xfs_iunlock(dp, XFS_ILOCK_EXCL);

	return error;

out:
	if (args.trans)
		xfs_trans_cancel(args.trans);
	xfs_iunlock(dp, XFS_ILOCK_EXCL);
	return error;
}

=======
	if (args->trans)
		xfs_trans_cancel(args->trans);
	goto out_unlock;
}

>>>>>>> upstream/android-13
/*========================================================================
 * External routines when attribute list is inside the inode
 *========================================================================*/

<<<<<<< HEAD
=======
static inline int xfs_attr_sf_totsize(struct xfs_inode *dp)
{
	struct xfs_attr_shortform *sf;

	sf = (struct xfs_attr_shortform *)dp->i_afp->if_u1.if_data;
	return be16_to_cpu(sf->hdr.totsize);
}

>>>>>>> upstream/android-13
/*
 * Add a name to the shortform attribute list structure
 * This is the external routine.
 */
STATIC int
xfs_attr_shortform_addname(xfs_da_args_t *args)
{
	int newsize, forkoff, retval;

	trace_xfs_attr_sf_addname(args);

	retval = xfs_attr_shortform_lookup(args);
<<<<<<< HEAD
	if ((args->flags & ATTR_REPLACE) && (retval == -ENOATTR)) {
		return retval;
	} else if (retval == -EEXIST) {
		if (args->flags & ATTR_CREATE)
			return retval;
		retval = xfs_attr_shortform_remove(args);
=======
	if (retval == -ENOATTR && (args->attr_flags & XATTR_REPLACE))
		return retval;
	if (retval == -EEXIST) {
		if (args->attr_flags & XATTR_CREATE)
			return retval;
		retval = xfs_attr_sf_removename(args);
>>>>>>> upstream/android-13
		if (retval)
			return retval;
		/*
		 * Since we have removed the old attr, clear ATTR_REPLACE so
		 * that the leaf format add routine won't trip over the attr
		 * not being around.
		 */
<<<<<<< HEAD
		args->flags &= ~ATTR_REPLACE;
=======
		args->attr_flags &= ~XATTR_REPLACE;
>>>>>>> upstream/android-13
	}

	if (args->namelen >= XFS_ATTR_SF_ENTSIZE_MAX ||
	    args->valuelen >= XFS_ATTR_SF_ENTSIZE_MAX)
		return -ENOSPC;

<<<<<<< HEAD
	newsize = XFS_ATTR_SF_TOTSIZE(args->dp);
	newsize += XFS_ATTR_SF_ENTSIZE_BYNAME(args->namelen, args->valuelen);
=======
	newsize = xfs_attr_sf_totsize(args->dp);
	newsize += xfs_attr_sf_entsize_byname(args->namelen, args->valuelen);
>>>>>>> upstream/android-13

	forkoff = xfs_attr_shortform_bytesfit(args->dp, newsize);
	if (!forkoff)
		return -ENOSPC;

	xfs_attr_shortform_add(args, forkoff);
	return 0;
}


/*========================================================================
 * External routines when attribute list is one block
 *========================================================================*/

<<<<<<< HEAD
/*
 * Add a name to the leaf attribute list structure
 *
 * This leaf block cannot have a "remote" value, we only call this routine
 * if bmap_one_block() says there is only one block (ie: no remote blks).
 */
STATIC int
xfs_attr_leaf_addname(
	struct xfs_da_args	*args)
{
	struct xfs_inode	*dp;
	struct xfs_buf		*bp;
	int			retval, error, forkoff;

	trace_xfs_attr_leaf_addname(args);

	/*
	 * Read the (only) block in the attribute list in.
	 */
	dp = args->dp;
	args->blkno = 0;
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp);
	if (error)
		return error;
=======
/* Store info about a remote block */
STATIC void
xfs_attr_save_rmt_blk(
	struct xfs_da_args	*args)
{
	args->blkno2 = args->blkno;
	args->index2 = args->index;
	args->rmtblkno2 = args->rmtblkno;
	args->rmtblkcnt2 = args->rmtblkcnt;
	args->rmtvaluelen2 = args->rmtvaluelen;
}

/* Set stored info about a remote block */
STATIC void
xfs_attr_restore_rmt_blk(
	struct xfs_da_args	*args)
{
	args->blkno = args->blkno2;
	args->index = args->index2;
	args->rmtblkno = args->rmtblkno2;
	args->rmtblkcnt = args->rmtblkcnt2;
	args->rmtvaluelen = args->rmtvaluelen2;
}

/*
 * Tries to add an attribute to an inode in leaf form
 *
 * This function is meant to execute as part of a delayed operation and leaves
 * the transaction handling to the caller.  On success the attribute is added
 * and the inode and transaction are left dirty.  If there is not enough space,
 * the attr data is converted to node format and -ENOSPC is returned. Caller is
 * responsible for handling the dirty inode and transaction or adding the attr
 * in node format.
 */
STATIC int
xfs_attr_leaf_try_add(
	struct xfs_da_args	*args,
	struct xfs_buf		*bp)
{
	int			retval;
>>>>>>> upstream/android-13

	/*
	 * Look up the given attribute in the leaf block.  Figure out if
	 * the given flags produce an error or call for an atomic rename.
	 */
<<<<<<< HEAD
	retval = xfs_attr3_leaf_lookup_int(bp, args);
	if ((args->flags & ATTR_REPLACE) && (retval == -ENOATTR)) {
		xfs_trans_brelse(args->trans, bp);
		return retval;
	} else if (retval == -EEXIST) {
		if (args->flags & ATTR_CREATE) {	/* pure create op */
			xfs_trans_brelse(args->trans, bp);
			return retval;
		}
=======
	retval = xfs_attr_leaf_hasname(args, &bp);
	if (retval != -ENOATTR && retval != -EEXIST)
		return retval;
	if (retval == -ENOATTR && (args->attr_flags & XATTR_REPLACE))
		goto out_brelse;
	if (retval == -EEXIST) {
		if (args->attr_flags & XATTR_CREATE)
			goto out_brelse;
>>>>>>> upstream/android-13

		trace_xfs_attr_leaf_replace(args);

		/* save the attribute state for later removal*/
		args->op_flags |= XFS_DA_OP_RENAME;	/* an atomic rename */
<<<<<<< HEAD
		args->blkno2 = args->blkno;		/* set 2nd entry info*/
		args->index2 = args->index;
		args->rmtblkno2 = args->rmtblkno;
		args->rmtblkcnt2 = args->rmtblkcnt;
		args->rmtvaluelen2 = args->rmtvaluelen;
=======
		xfs_attr_save_rmt_blk(args);
>>>>>>> upstream/android-13

		/*
		 * clear the remote attr state now that it is saved so that the
		 * values reflect the state of the attribute we are about to
		 * add, not the attribute we just found and will remove later.
		 */
		args->rmtblkno = 0;
		args->rmtblkcnt = 0;
		args->rmtvaluelen = 0;
	}

	/*
<<<<<<< HEAD
	 * Add the attribute to the leaf block, transitioning to a Btree
	 * if required.
	 */
	retval = xfs_attr3_leaf_add(bp, args);
	if (retval == -ENOSPC) {
		/*
		 * Promote the attribute list to the Btree format, then
		 * Commit that transaction so that the node_addname() call
		 * can manage its own transactions.
		 */
		error = xfs_attr3_leaf_to_node(args);
		if (error)
			return error;
		error = xfs_defer_finish(&args->trans);
		if (error)
			return error;

		/*
		 * Commit the current trans (including the inode) and start
		 * a new one.
		 */
		error = xfs_trans_roll_inode(&args->trans, dp);
		if (error)
			return error;

		/*
		 * Fob the whole rest of the problem off on the Btree code.
		 */
		error = xfs_attr_node_addname(args);
		return error;
	}

	/*
	 * Commit the transaction that added the attr name so that
	 * later routines can manage their own transactions.
	 */
	error = xfs_trans_roll_inode(&args->trans, dp);
	if (error)
		return error;

	/*
	 * If there was an out-of-line value, allocate the blocks we
	 * identified for its storage and copy the value.  This is done
	 * after we create the attribute so that we don't overflow the
	 * maximum size of a transaction and/or hit a deadlock.
	 */
	if (args->rmtblkno > 0) {
		error = xfs_attr_rmtval_set(args);
		if (error)
			return error;
	}

	/*
	 * If this is an atomic rename operation, we must "flip" the
	 * incomplete flags on the "new" and "old" attribute/value pairs
	 * so that one disappears and one appears atomically.  Then we
	 * must remove the "old" attribute/value pair.
	 */
	if (args->op_flags & XFS_DA_OP_RENAME) {
		/*
		 * In a separate transaction, set the incomplete flag on the
		 * "old" attr and clear the incomplete flag on the "new" attr.
		 */
		error = xfs_attr3_leaf_flipflags(args);
		if (error)
			return error;

		/*
		 * Dismantle the "old" attribute/value pair by removing
		 * a "remote" value (if it exists).
		 */
		args->index = args->index2;
		args->blkno = args->blkno2;
		args->rmtblkno = args->rmtblkno2;
		args->rmtblkcnt = args->rmtblkcnt2;
		args->rmtvaluelen = args->rmtvaluelen2;
		if (args->rmtblkno) {
			error = xfs_attr_rmtval_remove(args);
			if (error)
				return error;
		}

		/*
		 * Read in the block containing the "old" attr, then
		 * remove the "old" attr from that block (neat, huh!)
		 */
		error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno,
					   -1, &bp);
		if (error)
			return error;

		xfs_attr3_leaf_remove(bp, args);

		/*
		 * If the result is small enough, shrink it all into the inode.
		 */
		if ((forkoff = xfs_attr_shortform_allfit(bp, dp))) {
			error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
			/* bp is gone due to xfs_da_shrink_inode */
			if (error)
				return error;
			error = xfs_defer_finish(&args->trans);
			if (error)
				return error;
		}

		/*
		 * Commit the remove and start the next trans in series.
		 */
		error = xfs_trans_roll_inode(&args->trans, dp);

	} else if (args->rmtblkno > 0) {
		/*
		 * Added a "remote" value, just clear the incomplete flag.
		 */
		error = xfs_attr3_leaf_clearflag(args);
	}
=======
	 * Add the attribute to the leaf block
	 */
	return xfs_attr3_leaf_add(bp, args);

out_brelse:
	xfs_trans_brelse(args->trans, bp);
	return retval;
}

/*
 * Return EEXIST if attr is found, or ENOATTR if not
 */
STATIC int
xfs_attr_leaf_hasname(
	struct xfs_da_args	*args,
	struct xfs_buf		**bp)
{
	int                     error = 0;

	error = xfs_attr3_leaf_read(args->trans, args->dp, 0, bp);
	if (error)
		return error;

	error = xfs_attr3_leaf_lookup_int(*bp, args);
	if (error != -ENOATTR && error != -EEXIST)
		xfs_trans_brelse(args->trans, *bp);

>>>>>>> upstream/android-13
	return error;
}

/*
 * Remove a name from the leaf attribute list structure
 *
 * This leaf block cannot have a "remote" value, we only call this routine
 * if bmap_one_block() says there is only one block (ie: no remote blks).
 */
STATIC int
xfs_attr_leaf_removename(
	struct xfs_da_args	*args)
{
	struct xfs_inode	*dp;
	struct xfs_buf		*bp;
	int			error, forkoff;

	trace_xfs_attr_leaf_removename(args);

	/*
	 * Remove the attribute.
	 */
	dp = args->dp;
<<<<<<< HEAD
	args->blkno = 0;
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp);
	if (error)
		return error;

	error = xfs_attr3_leaf_lookup_int(bp, args);
	if (error == -ENOATTR) {
		xfs_trans_brelse(args->trans, bp);
		return error;
	}
=======

	error = xfs_attr_leaf_hasname(args, &bp);

	if (error == -ENOATTR) {
		xfs_trans_brelse(args->trans, bp);
		return error;
	} else if (error != -EEXIST)
		return error;
>>>>>>> upstream/android-13

	xfs_attr3_leaf_remove(bp, args);

	/*
	 * If the result is small enough, shrink it all into the inode.
	 */
<<<<<<< HEAD
	if ((forkoff = xfs_attr_shortform_allfit(bp, dp))) {
		error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
		/* bp is gone due to xfs_da_shrink_inode */
		if (error)
			return error;
		error = xfs_defer_finish(&args->trans);
		if (error)
			return error;
	}
=======
	forkoff = xfs_attr_shortform_allfit(bp, dp);
	if (forkoff)
		return xfs_attr3_leaf_to_shortform(bp, args, forkoff);
		/* bp is gone due to xfs_da_shrink_inode */

>>>>>>> upstream/android-13
	return 0;
}

/*
 * Look up a name in a leaf attribute list structure.
 *
 * This leaf block cannot have a "remote" value, we only call this routine
 * if bmap_one_block() says there is only one block (ie: no remote blks).
<<<<<<< HEAD
=======
 *
 * Returns 0 on successful retrieval, otherwise an error.
>>>>>>> upstream/android-13
 */
STATIC int
xfs_attr_leaf_get(xfs_da_args_t *args)
{
	struct xfs_buf *bp;
	int error;

	trace_xfs_attr_leaf_get(args);

<<<<<<< HEAD
	args->blkno = 0;
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp);
	if (error)
		return error;

	error = xfs_attr3_leaf_lookup_int(bp, args);
	if (error != -EEXIST)  {
		xfs_trans_brelse(args->trans, bp);
		return error;
	}
	error = xfs_attr3_leaf_getvalue(bp, args);
	xfs_trans_brelse(args->trans, bp);
	if (!error && (args->rmtblkno > 0) && !(args->flags & ATTR_KERNOVAL)) {
		error = xfs_attr_rmtval_get(args);
	}
	return error;
}

=======
	error = xfs_attr_leaf_hasname(args, &bp);

	if (error == -ENOATTR)  {
		xfs_trans_brelse(args->trans, bp);
		return error;
	} else if (error != -EEXIST)
		return error;


	error = xfs_attr3_leaf_getvalue(bp, args);
	xfs_trans_brelse(args->trans, bp);
	return error;
}

/*
 * Return EEXIST if attr is found, or ENOATTR if not
 * statep: If not null is set to point at the found state.  Caller will
 *         be responsible for freeing the state in this case.
 */
STATIC int
xfs_attr_node_hasname(
	struct xfs_da_args	*args,
	struct xfs_da_state	**statep)
{
	struct xfs_da_state	*state;
	int			retval, error;

	state = xfs_da_state_alloc(args);
	if (statep != NULL)
		*statep = NULL;

	/*
	 * Search to see if name exists, and get back a pointer to it.
	 */
	error = xfs_da3_node_lookup_int(state, &retval);
	if (error) {
		xfs_da_state_free(state);
		return error;
	}

	if (statep != NULL)
		*statep = state;
	else
		xfs_da_state_free(state);
	return retval;
}

>>>>>>> upstream/android-13
/*========================================================================
 * External routines when attribute list size > geo->blksize
 *========================================================================*/

<<<<<<< HEAD
/*
 * Add a name to a Btree-format attribute list.
 *
 * This will involve walking down the Btree, and may involve splitting
 * leaf nodes and even splitting intermediate nodes up to and including
 * the root node (a special case of an intermediate node).
 *
 * "Remote" attribute values confuse the issue and atomic rename operations
 * add a whole extra layer of confusion on top of that.
 */
STATIC int
xfs_attr_node_addname(
	struct xfs_da_args	*args)
{
	struct xfs_da_state	*state;
	struct xfs_da_state_blk	*blk;
	struct xfs_inode	*dp;
	struct xfs_mount	*mp;
	int			retval, error;

	trace_xfs_attr_node_addname(args);

	/*
	 * Fill in bucket of arguments/results/context to carry around.
	 */
	dp = args->dp;
	mp = dp->i_mount;
restart:
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = mp;
=======
STATIC int
xfs_attr_node_addname_find_attr(
	struct xfs_delattr_context	*dac)
{
	struct xfs_da_args		*args = dac->da_args;
	int				retval;
>>>>>>> upstream/android-13

	/*
	 * Search to see if name already exists, and get back a pointer
	 * to where it should go.
	 */
<<<<<<< HEAD
	error = xfs_da3_node_lookup_int(state, &retval);
	if (error)
		goto out;
	blk = &state->path.blk[ state->path.active-1 ];
	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
	if ((args->flags & ATTR_REPLACE) && (retval == -ENOATTR)) {
		goto out;
	} else if (retval == -EEXIST) {
		if (args->flags & ATTR_CREATE)
			goto out;
=======
	retval = xfs_attr_node_hasname(args, &dac->da_state);
	if (retval != -ENOATTR && retval != -EEXIST)
		return retval;

	if (retval == -ENOATTR && (args->attr_flags & XATTR_REPLACE))
		goto error;
	if (retval == -EEXIST) {
		if (args->attr_flags & XATTR_CREATE)
			goto error;
>>>>>>> upstream/android-13

		trace_xfs_attr_node_replace(args);

		/* save the attribute state for later removal*/
		args->op_flags |= XFS_DA_OP_RENAME;	/* atomic rename op */
<<<<<<< HEAD
		args->blkno2 = args->blkno;		/* set 2nd entry info*/
		args->index2 = args->index;
		args->rmtblkno2 = args->rmtblkno;
		args->rmtblkcnt2 = args->rmtblkcnt;
		args->rmtvaluelen2 = args->rmtvaluelen;
=======
		xfs_attr_save_rmt_blk(args);
>>>>>>> upstream/android-13

		/*
		 * clear the remote attr state now that it is saved so that the
		 * values reflect the state of the attribute we are about to
		 * add, not the attribute we just found and will remove later.
		 */
		args->rmtblkno = 0;
		args->rmtblkcnt = 0;
		args->rmtvaluelen = 0;
	}

<<<<<<< HEAD
	retval = xfs_attr3_leaf_add(blk->bp, state->args);
	if (retval == -ENOSPC) {
=======
	return 0;
error:
	if (dac->da_state)
		xfs_da_state_free(dac->da_state);
	return retval;
}

/*
 * Add a name to a Btree-format attribute list.
 *
 * This will involve walking down the Btree, and may involve splitting
 * leaf nodes and even splitting intermediate nodes up to and including
 * the root node (a special case of an intermediate node).
 *
 * "Remote" attribute values confuse the issue and atomic rename operations
 * add a whole extra layer of confusion on top of that.
 *
 * This routine is meant to function as a delayed operation, and may return
 * -EAGAIN when the transaction needs to be rolled.  Calling functions will need
 * to handle this, and recall the function until a successful error code is
 *returned.
 */
STATIC int
xfs_attr_node_addname(
	struct xfs_delattr_context	*dac)
{
	struct xfs_da_args		*args = dac->da_args;
	struct xfs_da_state		*state = dac->da_state;
	struct xfs_da_state_blk		*blk;
	int				error;

	trace_xfs_attr_node_addname(args);

	blk = &state->path.blk[state->path.active-1];
	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);

	error = xfs_attr3_leaf_add(blk->bp, state->args);
	if (error == -ENOSPC) {
>>>>>>> upstream/android-13
		if (state->path.active == 1) {
			/*
			 * Its really a single leaf node, but it had
			 * out-of-line values so it looked like it *might*
			 * have been a b-tree.
			 */
			xfs_da_state_free(state);
			state = NULL;
			error = xfs_attr3_leaf_to_node(args);
			if (error)
				goto out;
<<<<<<< HEAD
			error = xfs_defer_finish(&args->trans);
			if (error)
				goto out;

			/*
			 * Commit the node conversion and start the next
			 * trans in the chain.
			 */
			error = xfs_trans_roll_inode(&args->trans, dp);
			if (error)
				goto out;

			goto restart;
=======

			/*
			 * Now that we have converted the leaf to a node, we can
			 * roll the transaction, and try xfs_attr3_leaf_add
			 * again on re-entry.  No need to set dela_state to do
			 * this. dela_state is still unset by this function at
			 * this point.
			 */
			dac->flags |= XFS_DAC_DEFER_FINISH;
			trace_xfs_attr_node_addname_return(
					dac->dela_state, args->dp);
			return -EAGAIN;
>>>>>>> upstream/android-13
		}

		/*
		 * Split as many Btree elements as required.
		 * This code tracks the new and old attr's location
		 * in the index/blkno/rmtblkno/rmtblkcnt fields and
		 * in the index2/blkno2/rmtblkno2/rmtblkcnt2 fields.
		 */
		error = xfs_da3_split(state);
		if (error)
			goto out;
<<<<<<< HEAD
		error = xfs_defer_finish(&args->trans);
		if (error)
			goto out;
=======
		dac->flags |= XFS_DAC_DEFER_FINISH;
>>>>>>> upstream/android-13
	} else {
		/*
		 * Addition succeeded, update Btree hashvals.
		 */
		xfs_da3_fixhashpath(state, &state->path);
	}

<<<<<<< HEAD
	/*
	 * Kill the state structure, we're done with it and need to
	 * allow the buffers to come back later.
	 */
	xfs_da_state_free(state);
	state = NULL;

	/*
	 * Commit the leaf addition or btree split and start the next
	 * trans in the chain.
	 */
	error = xfs_trans_roll_inode(&args->trans, dp);
	if (error)
		goto out;

	/*
	 * If there was an out-of-line value, allocate the blocks we
	 * identified for its storage and copy the value.  This is done
	 * after we create the attribute so that we don't overflow the
	 * maximum size of a transaction and/or hit a deadlock.
	 */
	if (args->rmtblkno > 0) {
		error = xfs_attr_rmtval_set(args);
		if (error)
			return error;
	}

	/*
	 * If this is an atomic rename operation, we must "flip" the
	 * incomplete flags on the "new" and "old" attribute/value pairs
	 * so that one disappears and one appears atomically.  Then we
	 * must remove the "old" attribute/value pair.
	 */
	if (args->op_flags & XFS_DA_OP_RENAME) {
		/*
		 * In a separate transaction, set the incomplete flag on the
		 * "old" attr and clear the incomplete flag on the "new" attr.
		 */
		error = xfs_attr3_leaf_flipflags(args);
		if (error)
			goto out;

		/*
		 * Dismantle the "old" attribute/value pair by removing
		 * a "remote" value (if it exists).
		 */
		args->index = args->index2;
		args->blkno = args->blkno2;
		args->rmtblkno = args->rmtblkno2;
		args->rmtblkcnt = args->rmtblkcnt2;
		args->rmtvaluelen = args->rmtvaluelen2;
		if (args->rmtblkno) {
			error = xfs_attr_rmtval_remove(args);
			if (error)
				return error;
		}

		/*
		 * Re-find the "old" attribute entry after any split ops.
		 * The INCOMPLETE flag means that we will find the "old"
		 * attr, not the "new" one.
		 */
		args->flags |= XFS_ATTR_INCOMPLETE;
		state = xfs_da_state_alloc();
		state->args = args;
		state->mp = mp;
		state->inleaf = 0;
		error = xfs_da3_node_lookup_int(state, &retval);
		if (error)
			goto out;

		/*
		 * Remove the name and update the hashvals in the tree.
		 */
		blk = &state->path.blk[ state->path.active-1 ];
		ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
		error = xfs_attr3_leaf_remove(blk->bp, args);
		xfs_da3_fixhashpath(state, &state->path);

		/*
		 * Check to see if the tree needs to be collapsed.
		 */
		if (retval && (state->path.active > 1)) {
			error = xfs_da3_join(state);
			if (error)
				goto out;
			error = xfs_defer_finish(&args->trans);
			if (error)
				goto out;
		}

		/*
		 * Commit and start the next trans in the chain.
		 */
		error = xfs_trans_roll_inode(&args->trans, dp);
		if (error)
			goto out;

	} else if (args->rmtblkno > 0) {
		/*
		 * Added a "remote" value, just clear the incomplete flag.
		 */
		error = xfs_attr3_leaf_clearflag(args);
=======
out:
	if (state)
		xfs_da_state_free(state);
	return error;
}


STATIC int
xfs_attr_node_addname_clear_incomplete(
	struct xfs_delattr_context	*dac)
{
	struct xfs_da_args		*args = dac->da_args;
	struct xfs_da_state		*state = NULL;
	int				retval = 0;
	int				error = 0;

	/*
	 * Re-find the "old" attribute entry after any split ops. The INCOMPLETE
	 * flag means that we will find the "old" attr, not the "new" one.
	 */
	args->attr_filter |= XFS_ATTR_INCOMPLETE;
	state = xfs_da_state_alloc(args);
	state->inleaf = 0;
	error = xfs_da3_node_lookup_int(state, &retval);
	if (error)
		goto out;

	error = xfs_attr_node_removename(args, state);

	/*
	 * Check to see if the tree needs to be collapsed.
	 */
	if (retval && (state->path.active > 1)) {
		error = xfs_da3_join(state);
>>>>>>> upstream/android-13
		if (error)
			goto out;
	}
	retval = error = 0;

out:
	if (state)
		xfs_da_state_free(state);
	if (error)
		return error;
	return retval;
}

/*
<<<<<<< HEAD
 * Remove a name from a B-tree attribute list.
 *
 * This will involve walking down the Btree, and may involve joining
 * leaf nodes and even joining intermediate nodes up to and including
 * the root node (a special case of an intermediate node).
 */
STATIC int
xfs_attr_node_removename(
	struct xfs_da_args	*args)
{
	struct xfs_da_state	*state;
	struct xfs_da_state_blk	*blk;
	struct xfs_inode	*dp;
	struct xfs_buf		*bp;
	int			retval, error, forkoff;

	trace_xfs_attr_node_removename(args);

	/*
	 * Tie a string around our finger to remind us where we are.
	 */
	dp = args->dp;
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = dp->i_mount;

	/*
	 * Search to see if name exists, and get back a pointer to it.
	 */
	error = xfs_da3_node_lookup_int(state, &retval);
	if (error || (retval != -EEXIST)) {
		if (error == 0)
			error = retval;
		goto out;
	}

	/*
	 * If there is an out-of-line value, de-allocate the blocks.
	 * This is done before we remove the attribute so that we don't
	 * overflow the maximum size of a transaction and/or hit a deadlock.
	 */
	blk = &state->path.blk[ state->path.active-1 ];
	ASSERT(blk->bp != NULL);
	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
	if (args->rmtblkno > 0) {
		/*
		 * Fill in disk block numbers in the state structure
		 * so that we can get the buffers back after we commit
		 * several transactions in the following calls.
		 */
		error = xfs_attr_fillstate(state);
		if (error)
			goto out;

		/*
		 * Mark the attribute as INCOMPLETE, then bunmapi() the
		 * remote value.
		 */
		error = xfs_attr3_leaf_setflag(args);
		if (error)
			goto out;
		error = xfs_attr_rmtval_remove(args);
		if (error)
			goto out;

		/*
		 * Refill the state structure with buffers, the prior calls
		 * released our buffers.
		 */
		error = xfs_attr_refillstate(state);
		if (error)
			goto out;
	}
=======
 * Shrink an attribute from leaf to shortform
 */
STATIC int
xfs_attr_node_shrink(
	struct xfs_da_args	*args,
	struct xfs_da_state     *state)
{
	struct xfs_inode	*dp = args->dp;
	int			error, forkoff;
	struct xfs_buf		*bp;

	/*
	 * Have to get rid of the copy of this dabuf in the state.
	 */
	ASSERT(state->path.active == 1);
	ASSERT(state->path.blk[0].bp);
	state->path.blk[0].bp = NULL;

	error = xfs_attr3_leaf_read(args->trans, args->dp, 0, &bp);
	if (error)
		return error;

	forkoff = xfs_attr_shortform_allfit(bp, dp);
	if (forkoff) {
		error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
		/* bp is gone due to xfs_da_shrink_inode */
	} else
		xfs_trans_brelse(args->trans, bp);

	return error;
}

/*
 * Mark an attribute entry INCOMPLETE and save pointers to the relevant buffers
 * for later deletion of the entry.
 */
STATIC int
xfs_attr_leaf_mark_incomplete(
	struct xfs_da_args	*args,
	struct xfs_da_state	*state)
{
	int			error;

	/*
	 * Fill in disk block numbers in the state structure
	 * so that we can get the buffers back after we commit
	 * several transactions in the following calls.
	 */
	error = xfs_attr_fillstate(state);
	if (error)
		return error;

	/*
	 * Mark the attribute as INCOMPLETE
	 */
	return xfs_attr3_leaf_setflag(args);
}

/*
 * Initial setup for xfs_attr_node_removename.  Make sure the attr is there and
 * the blocks are valid.  Attr keys with remote blocks will be marked
 * incomplete.
 */
STATIC
int xfs_attr_node_removename_setup(
	struct xfs_delattr_context	*dac)
{
	struct xfs_da_args		*args = dac->da_args;
	struct xfs_da_state		**state = &dac->da_state;
	int				error;

	error = xfs_attr_node_hasname(args, state);
	if (error != -EEXIST)
		return error;
	error = 0;

	ASSERT((*state)->path.blk[(*state)->path.active - 1].bp != NULL);
	ASSERT((*state)->path.blk[(*state)->path.active - 1].magic ==
		XFS_ATTR_LEAF_MAGIC);

	if (args->rmtblkno > 0) {
		error = xfs_attr_leaf_mark_incomplete(args, *state);
		if (error)
			goto out;

		error = xfs_attr_rmtval_invalidate(args);
	}
out:
	if (error)
		xfs_da_state_free(*state);

	return error;
}

STATIC int
xfs_attr_node_removename(
	struct xfs_da_args	*args,
	struct xfs_da_state	*state)
{
	struct xfs_da_state_blk	*blk;
	int			retval;
>>>>>>> upstream/android-13

	/*
	 * Remove the name and update the hashvals in the tree.
	 */
<<<<<<< HEAD
	blk = &state->path.blk[ state->path.active-1 ];
=======
	blk = &state->path.blk[state->path.active-1];
>>>>>>> upstream/android-13
	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
	retval = xfs_attr3_leaf_remove(blk->bp, args);
	xfs_da3_fixhashpath(state, &state->path);

<<<<<<< HEAD
	/*
	 * Check to see if the tree needs to be collapsed.
	 */
	if (retval && (state->path.active > 1)) {
		error = xfs_da3_join(state);
		if (error)
			goto out;
		error = xfs_defer_finish(&args->trans);
		if (error)
			goto out;
		/*
		 * Commit the Btree join operation and start a new trans.
		 */
		error = xfs_trans_roll_inode(&args->trans, dp);
		if (error)
			goto out;
	}

	/*
	 * If the result is small enough, push it all into the inode.
	 */
	if (xfs_bmap_one_block(dp, XFS_ATTR_FORK)) {
		/*
		 * Have to get rid of the copy of this dabuf in the state.
		 */
		ASSERT(state->path.active == 1);
		ASSERT(state->path.blk[0].bp);
		state->path.blk[0].bp = NULL;

		error = xfs_attr3_leaf_read(args->trans, args->dp, 0, -1, &bp);
		if (error)
			goto out;

		if ((forkoff = xfs_attr_shortform_allfit(bp, dp))) {
			error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
			/* bp is gone due to xfs_da_shrink_inode */
			if (error)
				goto out;
			error = xfs_defer_finish(&args->trans);
			if (error)
				goto out;
		} else
			xfs_trans_brelse(args->trans, bp);
	}
	error = 0;

out:
	xfs_da_state_free(state);
=======
	return retval;
}

/*
 * Remove the attribute specified in @args.
 *
 * This will involve walking down the Btree, and may involve joining
 * leaf nodes and even joining intermediate nodes up to and including
 * the root node (a special case of an intermediate node).
 *
 * This routine is meant to function as either an in-line or delayed operation,
 * and may return -EAGAIN when the transaction needs to be rolled.  Calling
 * functions will need to handle this, and call the function until a
 * successful error code is returned.
 */
int
xfs_attr_remove_iter(
	struct xfs_delattr_context	*dac)
{
	struct xfs_da_args		*args = dac->da_args;
	struct xfs_da_state		*state = dac->da_state;
	int				retval, error = 0;
	struct xfs_inode		*dp = args->dp;

	trace_xfs_attr_node_removename(args);

	switch (dac->dela_state) {
	case XFS_DAS_UNINIT:
		if (!xfs_inode_hasattr(dp))
			return -ENOATTR;

		/*
		 * Shortform or leaf formats don't require transaction rolls and
		 * thus state transitions. Call the right helper and return.
		 */
		if (dp->i_afp->if_format == XFS_DINODE_FMT_LOCAL)
			return xfs_attr_sf_removename(args);

		if (xfs_attr_is_leaf(dp))
			return xfs_attr_leaf_removename(args);

		/*
		 * Node format may require transaction rolls. Set up the
		 * state context and fall into the state machine.
		 */
		if (!dac->da_state) {
			error = xfs_attr_node_removename_setup(dac);
			if (error)
				return error;
			state = dac->da_state;
		}

		fallthrough;
	case XFS_DAS_RMTBLK:
		dac->dela_state = XFS_DAS_RMTBLK;

		/*
		 * If there is an out-of-line value, de-allocate the blocks.
		 * This is done before we remove the attribute so that we don't
		 * overflow the maximum size of a transaction and/or hit a
		 * deadlock.
		 */
		if (args->rmtblkno > 0) {
			/*
			 * May return -EAGAIN. Roll and repeat until all remote
			 * blocks are removed.
			 */
			error = xfs_attr_rmtval_remove(dac);
			if (error == -EAGAIN) {
				trace_xfs_attr_remove_iter_return(
						dac->dela_state, args->dp);
				return error;
			} else if (error) {
				goto out;
			}

			/*
			 * Refill the state structure with buffers (the prior
			 * calls released our buffers) and close out this
			 * transaction before proceeding.
			 */
			ASSERT(args->rmtblkno == 0);
			error = xfs_attr_refillstate(state);
			if (error)
				goto out;
			dac->dela_state = XFS_DAS_RM_NAME;
			dac->flags |= XFS_DAC_DEFER_FINISH;
			trace_xfs_attr_remove_iter_return(dac->dela_state, args->dp);
			return -EAGAIN;
		}

		fallthrough;
	case XFS_DAS_RM_NAME:
		/*
		 * If we came here fresh from a transaction roll, reattach all
		 * the buffers to the current transaction.
		 */
		if (dac->dela_state == XFS_DAS_RM_NAME) {
			error = xfs_attr_refillstate(state);
			if (error)
				goto out;
		}

		retval = xfs_attr_node_removename(args, state);

		/*
		 * Check to see if the tree needs to be collapsed. If so, roll
		 * the transacton and fall into the shrink state.
		 */
		if (retval && (state->path.active > 1)) {
			error = xfs_da3_join(state);
			if (error)
				goto out;

			dac->flags |= XFS_DAC_DEFER_FINISH;
			dac->dela_state = XFS_DAS_RM_SHRINK;
			trace_xfs_attr_remove_iter_return(
					dac->dela_state, args->dp);
			return -EAGAIN;
		}

		fallthrough;
	case XFS_DAS_RM_SHRINK:
		/*
		 * If the result is small enough, push it all into the inode.
		 * This is our final state so it's safe to return a dirty
		 * transaction.
		 */
		if (xfs_attr_is_leaf(dp))
			error = xfs_attr_node_shrink(args, state);
		ASSERT(error != -EAGAIN);
		break;
	default:
		ASSERT(0);
		error = -EINVAL;
		goto out;
	}
out:
	if (state)
		xfs_da_state_free(state);
>>>>>>> upstream/android-13
	return error;
}

/*
 * Fill in the disk block numbers in the state structure for the buffers
 * that are attached to the state structure.
 * This is done so that we can quickly reattach ourselves to those buffers
 * after some set of transaction commits have released these buffers.
 */
STATIC int
xfs_attr_fillstate(xfs_da_state_t *state)
{
	xfs_da_state_path_t *path;
	xfs_da_state_blk_t *blk;
	int level;

	trace_xfs_attr_fillstate(state->args);

	/*
	 * Roll down the "path" in the state structure, storing the on-disk
	 * block number for those buffers in the "path".
	 */
	path = &state->path;
	ASSERT((path->active >= 0) && (path->active < XFS_DA_NODE_MAXDEPTH));
	for (blk = path->blk, level = 0; level < path->active; blk++, level++) {
		if (blk->bp) {
<<<<<<< HEAD
			blk->disk_blkno = XFS_BUF_ADDR(blk->bp);
=======
			blk->disk_blkno = xfs_buf_daddr(blk->bp);
>>>>>>> upstream/android-13
			blk->bp = NULL;
		} else {
			blk->disk_blkno = 0;
		}
	}

	/*
	 * Roll down the "altpath" in the state structure, storing the on-disk
	 * block number for those buffers in the "altpath".
	 */
	path = &state->altpath;
	ASSERT((path->active >= 0) && (path->active < XFS_DA_NODE_MAXDEPTH));
	for (blk = path->blk, level = 0; level < path->active; blk++, level++) {
		if (blk->bp) {
<<<<<<< HEAD
			blk->disk_blkno = XFS_BUF_ADDR(blk->bp);
=======
			blk->disk_blkno = xfs_buf_daddr(blk->bp);
>>>>>>> upstream/android-13
			blk->bp = NULL;
		} else {
			blk->disk_blkno = 0;
		}
	}

	return 0;
}

/*
 * Reattach the buffers to the state structure based on the disk block
 * numbers stored in the state structure.
 * This is done after some set of transaction commits have released those
 * buffers from our grip.
 */
STATIC int
xfs_attr_refillstate(xfs_da_state_t *state)
{
	xfs_da_state_path_t *path;
	xfs_da_state_blk_t *blk;
	int level, error;

	trace_xfs_attr_refillstate(state->args);

	/*
	 * Roll down the "path" in the state structure, storing the on-disk
	 * block number for those buffers in the "path".
	 */
	path = &state->path;
	ASSERT((path->active >= 0) && (path->active < XFS_DA_NODE_MAXDEPTH));
	for (blk = path->blk, level = 0; level < path->active; blk++, level++) {
		if (blk->disk_blkno) {
<<<<<<< HEAD
			error = xfs_da3_node_read(state->args->trans,
						state->args->dp,
						blk->blkno, blk->disk_blkno,
						&blk->bp, XFS_ATTR_FORK);
=======
			error = xfs_da3_node_read_mapped(state->args->trans,
					state->args->dp, blk->disk_blkno,
					&blk->bp, XFS_ATTR_FORK);
>>>>>>> upstream/android-13
			if (error)
				return error;
		} else {
			blk->bp = NULL;
		}
	}

	/*
	 * Roll down the "altpath" in the state structure, storing the on-disk
	 * block number for those buffers in the "altpath".
	 */
	path = &state->altpath;
	ASSERT((path->active >= 0) && (path->active < XFS_DA_NODE_MAXDEPTH));
	for (blk = path->blk, level = 0; level < path->active; blk++, level++) {
		if (blk->disk_blkno) {
<<<<<<< HEAD
			error = xfs_da3_node_read(state->args->trans,
						state->args->dp,
						blk->blkno, blk->disk_blkno,
						&blk->bp, XFS_ATTR_FORK);
=======
			error = xfs_da3_node_read_mapped(state->args->trans,
					state->args->dp, blk->disk_blkno,
					&blk->bp, XFS_ATTR_FORK);
>>>>>>> upstream/android-13
			if (error)
				return error;
		} else {
			blk->bp = NULL;
		}
	}

	return 0;
}

/*
<<<<<<< HEAD
 * Look up a filename in a node attribute list.
=======
 * Retrieve the attribute data from a node attribute list.
>>>>>>> upstream/android-13
 *
 * This routine gets called for any attribute fork that has more than one
 * block, ie: both true Btree attr lists and for single-leaf-blocks with
 * "remote" values taking up more blocks.
<<<<<<< HEAD
 */
STATIC int
xfs_attr_node_get(xfs_da_args_t *args)
{
	xfs_da_state_t *state;
	xfs_da_state_blk_t *blk;
	int error, retval;
	int i;

	trace_xfs_attr_node_get(args);

	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;

	/*
	 * Search to see if name exists, and get back a pointer to it.
	 */
	error = xfs_da3_node_lookup_int(state, &retval);
	if (error) {
		retval = error;
	} else if (retval == -EEXIST) {
		blk = &state->path.blk[ state->path.active-1 ];
		ASSERT(blk->bp != NULL);
		ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);

		/*
		 * Get the value, local or "remote"
		 */
		retval = xfs_attr3_leaf_getvalue(blk->bp, args);
		if (!retval && (args->rmtblkno > 0)
		    && !(args->flags & ATTR_KERNOVAL)) {
			retval = xfs_attr_rmtval_get(args);
		}
	}
=======
 *
 * Returns 0 on successful retrieval, otherwise an error.
 */
STATIC int
xfs_attr_node_get(
	struct xfs_da_args	*args)
{
	struct xfs_da_state	*state;
	struct xfs_da_state_blk	*blk;
	int			i;
	int			error;

	trace_xfs_attr_node_get(args);

	/*
	 * Search to see if name exists, and get back a pointer to it.
	 */
	error = xfs_attr_node_hasname(args, &state);
	if (error != -EEXIST)
		goto out_release;

	/*
	 * Get the value, local or "remote"
	 */
	blk = &state->path.blk[state->path.active - 1];
	error = xfs_attr3_leaf_getvalue(blk->bp, args);
>>>>>>> upstream/android-13

	/*
	 * If not in a transaction, we have to release all the buffers.
	 */
<<<<<<< HEAD
	for (i = 0; i < state->path.active; i++) {
=======
out_release:
	for (i = 0; state != NULL && i < state->path.active; i++) {
>>>>>>> upstream/android-13
		xfs_trans_brelse(args->trans, state->path.blk[i].bp);
		state->path.blk[i].bp = NULL;
	}

<<<<<<< HEAD
	xfs_da_state_free(state);
	return retval;
=======
	if (state)
		xfs_da_state_free(state);
	return error;
}

/* Returns true if the attribute entry name is valid. */
bool
xfs_attr_namecheck(
	const void	*name,
	size_t		length)
{
	/*
	 * MAXNAMELEN includes the trailing null, but (name/length) leave it
	 * out, so use >= for the length check.
	 */
	if (length >= MAXNAMELEN)
		return false;

	/* There shouldn't be any nulls here */
	return !memchr(name, 0, length);
>>>>>>> upstream/android-13
}
