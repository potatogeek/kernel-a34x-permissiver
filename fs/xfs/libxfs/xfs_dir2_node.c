// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * Copyright (c) 2013 Red Hat, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include "xfs_fs.h"
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
#include "xfs_inode.h"
#include "xfs_bmap.h"
#include "xfs_dir2.h"
#include "xfs_dir2_priv.h"
#include "xfs_error.h"
#include "xfs_trace.h"
#include "xfs_trans.h"
#include "xfs_buf_item.h"
<<<<<<< HEAD
#include "xfs_cksum.h"
=======
>>>>>>> upstream/android-13
#include "xfs_log.h"

/*
 * Function declarations.
 */
static int xfs_dir2_leafn_add(struct xfs_buf *bp, xfs_da_args_t *args,
			      int index);
static void xfs_dir2_leafn_rebalance(xfs_da_state_t *state,
				     xfs_da_state_blk_t *blk1,
				     xfs_da_state_blk_t *blk2);
static int xfs_dir2_leafn_remove(xfs_da_args_t *args, struct xfs_buf *bp,
				 int index, xfs_da_state_blk_t *dblk,
				 int *rval);
<<<<<<< HEAD
static int xfs_dir2_node_addname_int(xfs_da_args_t *args,
				     xfs_da_state_blk_t *fblk);
=======

/*
 * Convert data space db to the corresponding free db.
 */
static xfs_dir2_db_t
xfs_dir2_db_to_fdb(struct xfs_da_geometry *geo, xfs_dir2_db_t db)
{
	return xfs_dir2_byte_to_db(geo, XFS_DIR2_FREE_OFFSET) +
			(db / geo->free_max_bests);
}

/*
 * Convert data space db to the corresponding index in a free db.
 */
static int
xfs_dir2_db_to_fdindex(struct xfs_da_geometry *geo, xfs_dir2_db_t db)
{
	return db % geo->free_max_bests;
}
>>>>>>> upstream/android-13

/*
 * Check internal consistency of a leafn block.
 */
#ifdef DEBUG
static xfs_failaddr_t
xfs_dir3_leafn_check(
	struct xfs_inode	*dp,
	struct xfs_buf		*bp)
{
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	struct xfs_dir3_icleaf_hdr leafhdr;

<<<<<<< HEAD
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);

	if (leafhdr.magic == XFS_DIR3_LEAFN_MAGIC) {
		struct xfs_dir3_leaf_hdr *leaf3 = bp->b_addr;
		if (be64_to_cpu(leaf3->info.blkno) != bp->b_bn)
=======
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &leafhdr, leaf);

	if (leafhdr.magic == XFS_DIR3_LEAFN_MAGIC) {
		struct xfs_dir3_leaf_hdr *leaf3 = bp->b_addr;
		if (be64_to_cpu(leaf3->info.blkno) != xfs_buf_daddr(bp))
>>>>>>> upstream/android-13
			return __this_address;
	} else if (leafhdr.magic != XFS_DIR2_LEAFN_MAGIC)
		return __this_address;

<<<<<<< HEAD
	return xfs_dir3_leaf_check_int(dp->i_mount, dp, &leafhdr, leaf);
=======
	return xfs_dir3_leaf_check_int(dp->i_mount, &leafhdr, leaf, false);
>>>>>>> upstream/android-13
}

static inline void
xfs_dir3_leaf_check(
	struct xfs_inode	*dp,
	struct xfs_buf		*bp)
{
	xfs_failaddr_t		fa;

	fa = xfs_dir3_leafn_check(dp, bp);
	if (!fa)
		return;
	xfs_corruption_error(__func__, XFS_ERRLEVEL_LOW, dp->i_mount,
			bp->b_addr, BBTOB(bp->b_length), __FILE__, __LINE__,
			fa);
	ASSERT(0);
}
#else
#define	xfs_dir3_leaf_check(dp, bp)
#endif

static xfs_failaddr_t
xfs_dir3_free_verify(
	struct xfs_buf		*bp)
{
<<<<<<< HEAD
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	struct xfs_dir2_free_hdr *hdr = bp->b_addr;

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
		struct xfs_dir3_blk_hdr *hdr3 = bp->b_addr;

		if (hdr3->magic != cpu_to_be32(XFS_DIR3_FREE_MAGIC))
			return __this_address;
		if (!uuid_equal(&hdr3->uuid, &mp->m_sb.sb_meta_uuid))
			return __this_address;
		if (be64_to_cpu(hdr3->blkno) != bp->b_bn)
			return __this_address;
		if (!xfs_log_check_lsn(mp, be64_to_cpu(hdr3->lsn)))
			return __this_address;
	} else {
		if (hdr->magic != cpu_to_be32(XFS_DIR2_FREE_MAGIC))
			return __this_address;
=======
	struct xfs_mount	*mp = bp->b_mount;
	struct xfs_dir2_free_hdr *hdr = bp->b_addr;

	if (!xfs_verify_magic(bp, hdr->magic))
		return __this_address;

	if (xfs_has_crc(mp)) {
		struct xfs_dir3_blk_hdr *hdr3 = bp->b_addr;

		if (!uuid_equal(&hdr3->uuid, &mp->m_sb.sb_meta_uuid))
			return __this_address;
		if (be64_to_cpu(hdr3->blkno) != xfs_buf_daddr(bp))
			return __this_address;
		if (!xfs_log_check_lsn(mp, be64_to_cpu(hdr3->lsn)))
			return __this_address;
>>>>>>> upstream/android-13
	}

	/* XXX: should bounds check the xfs_dir3_icfree_hdr here */

	return NULL;
}

static void
xfs_dir3_free_read_verify(
	struct xfs_buf	*bp)
{
<<<<<<< HEAD
	struct xfs_mount	*mp = bp->b_target->bt_mount;
	xfs_failaddr_t		fa;

	if (xfs_sb_version_hascrc(&mp->m_sb) &&
=======
	struct xfs_mount	*mp = bp->b_mount;
	xfs_failaddr_t		fa;

	if (xfs_has_crc(mp) &&
>>>>>>> upstream/android-13
	    !xfs_buf_verify_cksum(bp, XFS_DIR3_FREE_CRC_OFF))
		xfs_verifier_error(bp, -EFSBADCRC, __this_address);
	else {
		fa = xfs_dir3_free_verify(bp);
		if (fa)
			xfs_verifier_error(bp, -EFSCORRUPTED, fa);
	}
}

static void
xfs_dir3_free_write_verify(
	struct xfs_buf	*bp)
{
<<<<<<< HEAD
	struct xfs_mount	*mp = bp->b_target->bt_mount;
=======
	struct xfs_mount	*mp = bp->b_mount;
>>>>>>> upstream/android-13
	struct xfs_buf_log_item	*bip = bp->b_log_item;
	struct xfs_dir3_blk_hdr	*hdr3 = bp->b_addr;
	xfs_failaddr_t		fa;

	fa = xfs_dir3_free_verify(bp);
	if (fa) {
		xfs_verifier_error(bp, -EFSCORRUPTED, fa);
		return;
	}

<<<<<<< HEAD
	if (!xfs_sb_version_hascrc(&mp->m_sb))
=======
	if (!xfs_has_crc(mp))
>>>>>>> upstream/android-13
		return;

	if (bip)
		hdr3->lsn = cpu_to_be64(bip->bli_item.li_lsn);

	xfs_buf_update_cksum(bp, XFS_DIR3_FREE_CRC_OFF);
}

const struct xfs_buf_ops xfs_dir3_free_buf_ops = {
	.name = "xfs_dir3_free",
<<<<<<< HEAD
=======
	.magic = { cpu_to_be32(XFS_DIR2_FREE_MAGIC),
		   cpu_to_be32(XFS_DIR3_FREE_MAGIC) },
>>>>>>> upstream/android-13
	.verify_read = xfs_dir3_free_read_verify,
	.verify_write = xfs_dir3_free_write_verify,
	.verify_struct = xfs_dir3_free_verify,
};

/* Everything ok in the free block header? */
static xfs_failaddr_t
xfs_dir3_free_header_check(
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = dp->i_mount;
<<<<<<< HEAD
	unsigned int		firstdb;
	int			maxbests;

	maxbests = dp->d_ops->free_max_bests(mp->m_dir_geo);
	firstdb = (xfs_dir2_da_to_db(mp->m_dir_geo, fbno) -
		   xfs_dir2_byte_to_db(mp->m_dir_geo, XFS_DIR2_FREE_OFFSET)) *
			maxbests;
	if (xfs_sb_version_hascrc(&mp->m_sb)) {
=======
	int			maxbests = mp->m_dir_geo->free_max_bests;
	unsigned int		firstdb;

	firstdb = (xfs_dir2_da_to_db(mp->m_dir_geo, fbno) -
		   xfs_dir2_byte_to_db(mp->m_dir_geo, XFS_DIR2_FREE_OFFSET)) *
			maxbests;
	if (xfs_has_crc(mp)) {
>>>>>>> upstream/android-13
		struct xfs_dir3_free_hdr *hdr3 = bp->b_addr;

		if (be32_to_cpu(hdr3->firstdb) != firstdb)
			return __this_address;
		if (be32_to_cpu(hdr3->nvalid) > maxbests)
			return __this_address;
		if (be32_to_cpu(hdr3->nvalid) < be32_to_cpu(hdr3->nused))
			return __this_address;
<<<<<<< HEAD
=======
		if (be64_to_cpu(hdr3->hdr.owner) != dp->i_ino)
			return __this_address;
>>>>>>> upstream/android-13
	} else {
		struct xfs_dir2_free_hdr *hdr = bp->b_addr;

		if (be32_to_cpu(hdr->firstdb) != firstdb)
			return __this_address;
		if (be32_to_cpu(hdr->nvalid) > maxbests)
			return __this_address;
		if (be32_to_cpu(hdr->nvalid) < be32_to_cpu(hdr->nused))
			return __this_address;
	}
	return NULL;
}

static int
__xfs_dir3_free_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
<<<<<<< HEAD
	xfs_daddr_t		mappedbno,
=======
	unsigned int		flags,
>>>>>>> upstream/android-13
	struct xfs_buf		**bpp)
{
	xfs_failaddr_t		fa;
	int			err;

<<<<<<< HEAD
	err = xfs_da_read_buf(tp, dp, fbno, mappedbno, bpp,
				XFS_DATA_FORK, &xfs_dir3_free_buf_ops);
=======
	err = xfs_da_read_buf(tp, dp, fbno, flags, bpp, XFS_DATA_FORK,
			&xfs_dir3_free_buf_ops);
>>>>>>> upstream/android-13
	if (err || !*bpp)
		return err;

	/* Check things that we can't do in the verifier. */
	fa = xfs_dir3_free_header_check(dp, fbno, *bpp);
	if (fa) {
<<<<<<< HEAD
		xfs_verifier_error(*bpp, -EFSCORRUPTED, fa);
=======
		__xfs_buf_mark_corrupt(*bpp, fa);
>>>>>>> upstream/android-13
		xfs_trans_brelse(tp, *bpp);
		*bpp = NULL;
		return -EFSCORRUPTED;
	}

	/* try read returns without an error or *bpp if it lands in a hole */
	if (tp)
		xfs_trans_buf_set_type(tp, *bpp, XFS_BLFT_DIR_FREE_BUF);

	return 0;
}

<<<<<<< HEAD
=======
void
xfs_dir2_free_hdr_from_disk(
	struct xfs_mount		*mp,
	struct xfs_dir3_icfree_hdr	*to,
	struct xfs_dir2_free		*from)
{
	if (xfs_has_crc(mp)) {
		struct xfs_dir3_free	*from3 = (struct xfs_dir3_free *)from;

		to->magic = be32_to_cpu(from3->hdr.hdr.magic);
		to->firstdb = be32_to_cpu(from3->hdr.firstdb);
		to->nvalid = be32_to_cpu(from3->hdr.nvalid);
		to->nused = be32_to_cpu(from3->hdr.nused);
		to->bests = from3->bests;

		ASSERT(to->magic == XFS_DIR3_FREE_MAGIC);
	} else {
		to->magic = be32_to_cpu(from->hdr.magic);
		to->firstdb = be32_to_cpu(from->hdr.firstdb);
		to->nvalid = be32_to_cpu(from->hdr.nvalid);
		to->nused = be32_to_cpu(from->hdr.nused);
		to->bests = from->bests;

		ASSERT(to->magic == XFS_DIR2_FREE_MAGIC);
	}
}

static void
xfs_dir2_free_hdr_to_disk(
	struct xfs_mount		*mp,
	struct xfs_dir2_free		*to,
	struct xfs_dir3_icfree_hdr	*from)
{
	if (xfs_has_crc(mp)) {
		struct xfs_dir3_free	*to3 = (struct xfs_dir3_free *)to;

		ASSERT(from->magic == XFS_DIR3_FREE_MAGIC);

		to3->hdr.hdr.magic = cpu_to_be32(from->magic);
		to3->hdr.firstdb = cpu_to_be32(from->firstdb);
		to3->hdr.nvalid = cpu_to_be32(from->nvalid);
		to3->hdr.nused = cpu_to_be32(from->nused);
	} else {
		ASSERT(from->magic == XFS_DIR2_FREE_MAGIC);

		to->hdr.magic = cpu_to_be32(from->magic);
		to->hdr.firstdb = cpu_to_be32(from->firstdb);
		to->hdr.nvalid = cpu_to_be32(from->nvalid);
		to->hdr.nused = cpu_to_be32(from->nused);
	}
}

>>>>>>> upstream/android-13
int
xfs_dir2_free_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
	struct xfs_buf		**bpp)
{
<<<<<<< HEAD
	return __xfs_dir3_free_read(tp, dp, fbno, -1, bpp);
=======
	return __xfs_dir3_free_read(tp, dp, fbno, 0, bpp);
>>>>>>> upstream/android-13
}

static int
xfs_dir2_free_try_read(
	struct xfs_trans	*tp,
	struct xfs_inode	*dp,
	xfs_dablk_t		fbno,
	struct xfs_buf		**bpp)
{
<<<<<<< HEAD
	return __xfs_dir3_free_read(tp, dp, fbno, -2, bpp);
=======
	return __xfs_dir3_free_read(tp, dp, fbno, XFS_DABUF_MAP_HOLE_OK, bpp);
>>>>>>> upstream/android-13
}

static int
xfs_dir3_free_get_buf(
	xfs_da_args_t		*args,
	xfs_dir2_db_t		fbno,
	struct xfs_buf		**bpp)
{
	struct xfs_trans	*tp = args->trans;
	struct xfs_inode	*dp = args->dp;
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_buf		*bp;
	int			error;
	struct xfs_dir3_icfree_hdr hdr;

	error = xfs_da_get_buf(tp, dp, xfs_dir2_db_to_da(args->geo, fbno),
<<<<<<< HEAD
				   -1, &bp, XFS_DATA_FORK);
=======
			&bp, XFS_DATA_FORK);
>>>>>>> upstream/android-13
	if (error)
		return error;

	xfs_trans_buf_set_type(tp, bp, XFS_BLFT_DIR_FREE_BUF);
	bp->b_ops = &xfs_dir3_free_buf_ops;

	/*
	 * Initialize the new block to be empty, and remember
	 * its first slot as our empty slot.
	 */
	memset(bp->b_addr, 0, sizeof(struct xfs_dir3_free_hdr));
	memset(&hdr, 0, sizeof(hdr));

<<<<<<< HEAD
	if (xfs_sb_version_hascrc(&mp->m_sb)) {
=======
	if (xfs_has_crc(mp)) {
>>>>>>> upstream/android-13
		struct xfs_dir3_free_hdr *hdr3 = bp->b_addr;

		hdr.magic = XFS_DIR3_FREE_MAGIC;

<<<<<<< HEAD
		hdr3->hdr.blkno = cpu_to_be64(bp->b_bn);
=======
		hdr3->hdr.blkno = cpu_to_be64(xfs_buf_daddr(bp));
>>>>>>> upstream/android-13
		hdr3->hdr.owner = cpu_to_be64(dp->i_ino);
		uuid_copy(&hdr3->hdr.uuid, &mp->m_sb.sb_meta_uuid);
	} else
		hdr.magic = XFS_DIR2_FREE_MAGIC;
<<<<<<< HEAD
	dp->d_ops->free_hdr_to_disk(bp->b_addr, &hdr);
=======
	xfs_dir2_free_hdr_to_disk(mp, bp->b_addr, &hdr);
>>>>>>> upstream/android-13
	*bpp = bp;
	return 0;
}

/*
 * Log entries from a freespace block.
 */
STATIC void
xfs_dir2_free_log_bests(
	struct xfs_da_args	*args,
<<<<<<< HEAD
=======
	struct xfs_dir3_icfree_hdr *hdr,
>>>>>>> upstream/android-13
	struct xfs_buf		*bp,
	int			first,		/* first entry to log */
	int			last)		/* last entry to log */
{
<<<<<<< HEAD
	xfs_dir2_free_t		*free;		/* freespace structure */
	__be16			*bests;

	free = bp->b_addr;
	bests = args->dp->d_ops->free_bests_p(free);
	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC) ||
	       free->hdr.magic == cpu_to_be32(XFS_DIR3_FREE_MAGIC));
	xfs_trans_log_buf(args->trans, bp,
		(uint)((char *)&bests[first] - (char *)free),
		(uint)((char *)&bests[last] - (char *)free +
		       sizeof(bests[0]) - 1));
=======
	struct xfs_dir2_free	*free = bp->b_addr;

	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC) ||
	       free->hdr.magic == cpu_to_be32(XFS_DIR3_FREE_MAGIC));
	xfs_trans_log_buf(args->trans, bp,
			  (char *)&hdr->bests[first] - (char *)free,
			  (char *)&hdr->bests[last] - (char *)free +
			   sizeof(hdr->bests[0]) - 1);
>>>>>>> upstream/android-13
}

/*
 * Log header from a freespace block.
 */
static void
xfs_dir2_free_log_header(
	struct xfs_da_args	*args,
	struct xfs_buf		*bp)
{
#ifdef DEBUG
	xfs_dir2_free_t		*free;		/* freespace structure */

	free = bp->b_addr;
	ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC) ||
	       free->hdr.magic == cpu_to_be32(XFS_DIR3_FREE_MAGIC));
#endif
	xfs_trans_log_buf(args->trans, bp, 0,
<<<<<<< HEAD
			  args->dp->d_ops->free_hdr_size - 1);
=======
			  args->geo->free_hdr_size - 1);
>>>>>>> upstream/android-13
}

/*
 * Convert a leaf-format directory to a node-format directory.
 * We need to change the magic number of the leaf block, and copy
 * the freespace table out of the leaf block into its own block.
 */
int						/* error */
xfs_dir2_leaf_to_node(
	xfs_da_args_t		*args,		/* operation arguments */
	struct xfs_buf		*lbp)		/* leaf buffer */
{
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return value */
	struct xfs_buf		*fbp;		/* freespace buffer */
	xfs_dir2_db_t		fdb;		/* freespace block number */
<<<<<<< HEAD
	xfs_dir2_free_t		*free;		/* freespace structure */
=======
>>>>>>> upstream/android-13
	__be16			*from;		/* pointer to freespace entry */
	int			i;		/* leaf freespace index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_tail_t	*ltp;		/* leaf tail structure */
	int			n;		/* count of live freespc ents */
	xfs_dir2_data_off_t	off;		/* freespace entry value */
<<<<<<< HEAD
	__be16			*to;		/* pointer to freespace entry */
=======
>>>>>>> upstream/android-13
	xfs_trans_t		*tp;		/* transaction pointer */
	struct xfs_dir3_icfree_hdr freehdr;

	trace_xfs_dir2_leaf_to_node(args);

	dp = args->dp;
	tp = args->trans;
	/*
	 * Add a freespace block to the directory.
	 */
	if ((error = xfs_dir2_grow_inode(args, XFS_DIR2_FREE_SPACE, &fdb))) {
		return error;
	}
	ASSERT(fdb == xfs_dir2_byte_to_db(args->geo, XFS_DIR2_FREE_OFFSET));
	/*
	 * Get the buffer for the new freespace block.
	 */
	error = xfs_dir3_free_get_buf(args, fdb, &fbp);
	if (error)
		return error;

<<<<<<< HEAD
	free = fbp->b_addr;
	dp->d_ops->free_hdr_from_disk(&freehdr, free);
	leaf = lbp->b_addr;
	ltp = xfs_dir2_leaf_tail_p(args->geo, leaf);
	if (be32_to_cpu(ltp->bestcount) >
				(uint)dp->i_d.di_size / args->geo->blksize)
		return -EFSCORRUPTED;
=======
	xfs_dir2_free_hdr_from_disk(dp->i_mount, &freehdr, fbp->b_addr);
	leaf = lbp->b_addr;
	ltp = xfs_dir2_leaf_tail_p(args->geo, leaf);
	if (be32_to_cpu(ltp->bestcount) >
				(uint)dp->i_disk_size / args->geo->blksize) {
		xfs_buf_mark_corrupt(lbp);
		return -EFSCORRUPTED;
	}
>>>>>>> upstream/android-13

	/*
	 * Copy freespace entries from the leaf block to the new block.
	 * Count active entries.
	 */
	from = xfs_dir2_leaf_bests_p(ltp);
<<<<<<< HEAD
	to = dp->d_ops->free_bests_p(free);
	for (i = n = 0; i < be32_to_cpu(ltp->bestcount); i++, from++, to++) {
		if ((off = be16_to_cpu(*from)) != NULLDATAOFF)
			n++;
		*to = cpu_to_be16(off);
=======
	for (i = n = 0; i < be32_to_cpu(ltp->bestcount); i++, from++) {
		off = be16_to_cpu(*from);
		if (off != NULLDATAOFF)
			n++;
		freehdr.bests[i] = cpu_to_be16(off);
>>>>>>> upstream/android-13
	}

	/*
	 * Now initialize the freespace block header.
	 */
	freehdr.nused = n;
	freehdr.nvalid = be32_to_cpu(ltp->bestcount);

<<<<<<< HEAD
	dp->d_ops->free_hdr_to_disk(fbp->b_addr, &freehdr);
	xfs_dir2_free_log_bests(args, fbp, 0, freehdr.nvalid - 1);
=======
	xfs_dir2_free_hdr_to_disk(dp->i_mount, fbp->b_addr, &freehdr);
	xfs_dir2_free_log_bests(args, &freehdr, fbp, 0, freehdr.nvalid - 1);
>>>>>>> upstream/android-13
	xfs_dir2_free_log_header(args, fbp);

	/*
	 * Converting the leaf to a leafnode is just a matter of changing the
	 * magic number and the ops. Do the change directly to the buffer as
	 * it's less work (and less code) than decoding the header to host
	 * format and back again.
	 */
	if (leaf->hdr.info.magic == cpu_to_be16(XFS_DIR2_LEAF1_MAGIC))
		leaf->hdr.info.magic = cpu_to_be16(XFS_DIR2_LEAFN_MAGIC);
	else
		leaf->hdr.info.magic = cpu_to_be16(XFS_DIR3_LEAFN_MAGIC);
	lbp->b_ops = &xfs_dir3_leafn_buf_ops;
	xfs_trans_buf_set_type(tp, lbp, XFS_BLFT_DIR_LEAFN_BUF);
	xfs_dir3_leaf_log_header(args, lbp);
	xfs_dir3_leaf_check(dp, lbp);
	return 0;
}

/*
 * Add a leaf entry to a leaf block in a node-form directory.
 * The other work necessary is done from the caller.
 */
static int					/* error */
xfs_dir2_leafn_add(
	struct xfs_buf		*bp,		/* leaf buffer */
<<<<<<< HEAD
	xfs_da_args_t		*args,		/* operation arguments */
	int			index)		/* insertion pt for new entry */
{
	int			compact;	/* compacting stale leaves */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			highstale;	/* next stale entry */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	int			lfloghigh;	/* high leaf entry logging */
	int			lfloglow;	/* low leaf entry logging */
	int			lowstale;	/* previous stale entry */
	struct xfs_dir3_icleaf_hdr leafhdr;
	struct xfs_dir2_leaf_entry *ents;

	trace_xfs_dir2_leafn_add(args, index);

	dp = args->dp;
	leaf = bp->b_addr;
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
=======
	struct xfs_da_args	*args,		/* operation arguments */
	int			index)		/* insertion pt for new entry */
{
	struct xfs_dir3_icleaf_hdr leafhdr;
	struct xfs_inode	*dp = args->dp;
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	struct xfs_dir2_leaf_entry *lep;
	struct xfs_dir2_leaf_entry *ents;
	int			compact;	/* compacting stale leaves */
	int			highstale = 0;	/* next stale entry */
	int			lfloghigh;	/* high leaf entry logging */
	int			lfloglow;	/* low leaf entry logging */
	int			lowstale = 0;	/* previous stale entry */

	trace_xfs_dir2_leafn_add(args, index);

	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &leafhdr, leaf);
	ents = leafhdr.ents;
>>>>>>> upstream/android-13

	/*
	 * Quick check just to make sure we are not going to index
	 * into other peoples memory
	 */
<<<<<<< HEAD
	if (index < 0)
		return -EFSCORRUPTED;
=======
	if (index < 0) {
		xfs_buf_mark_corrupt(bp);
		return -EFSCORRUPTED;
	}
>>>>>>> upstream/android-13

	/*
	 * If there are already the maximum number of leaf entries in
	 * the block, if there are no stale entries it won't fit.
	 * Caller will do a split.  If there are stale entries we'll do
	 * a compact.
	 */

<<<<<<< HEAD
	if (leafhdr.count == dp->d_ops->leaf_max_ents(args->geo)) {
=======
	if (leafhdr.count == args->geo->leaf_max_ents) {
>>>>>>> upstream/android-13
		if (!leafhdr.stale)
			return -ENOSPC;
		compact = leafhdr.stale > 1;
	} else
		compact = 0;
	ASSERT(index == 0 || be32_to_cpu(ents[index - 1].hashval) <= args->hashval);
	ASSERT(index == leafhdr.count ||
	       be32_to_cpu(ents[index].hashval) >= args->hashval);

	if (args->op_flags & XFS_DA_OP_JUSTCHECK)
		return 0;

	/*
	 * Compact out all but one stale leaf entry.  Leaves behind
	 * the entry closest to index.
	 */
	if (compact)
		xfs_dir3_leaf_compact_x1(&leafhdr, ents, &index, &lowstale,
					 &highstale, &lfloglow, &lfloghigh);
	else if (leafhdr.stale) {
		/*
		 * Set impossible logging indices for this case.
		 */
		lfloglow = leafhdr.count;
		lfloghigh = -1;
	}

	/*
	 * Insert the new entry, log everything.
	 */
	lep = xfs_dir3_leaf_find_entry(&leafhdr, ents, index, compact, lowstale,
				       highstale, &lfloglow, &lfloghigh);

	lep->hashval = cpu_to_be32(args->hashval);
	lep->address = cpu_to_be32(xfs_dir2_db_off_to_dataptr(args->geo,
				args->blkno, args->index));

<<<<<<< HEAD
	dp->d_ops->leaf_hdr_to_disk(leaf, &leafhdr);
	xfs_dir3_leaf_log_header(args, bp);
	xfs_dir3_leaf_log_ents(args, bp, lfloglow, lfloghigh);
=======
	xfs_dir2_leaf_hdr_to_disk(dp->i_mount, leaf, &leafhdr);
	xfs_dir3_leaf_log_header(args, bp);
	xfs_dir3_leaf_log_ents(args, &leafhdr, bp, lfloglow, lfloghigh);
>>>>>>> upstream/android-13
	xfs_dir3_leaf_check(dp, bp);
	return 0;
}

#ifdef DEBUG
static void
xfs_dir2_free_hdr_check(
	struct xfs_inode *dp,
	struct xfs_buf	*bp,
	xfs_dir2_db_t	db)
{
	struct xfs_dir3_icfree_hdr hdr;

<<<<<<< HEAD
	dp->d_ops->free_hdr_from_disk(&hdr, bp->b_addr);

	ASSERT((hdr.firstdb %
		dp->d_ops->free_max_bests(dp->i_mount->m_dir_geo)) == 0);
=======
	xfs_dir2_free_hdr_from_disk(dp->i_mount, &hdr, bp->b_addr);

	ASSERT((hdr.firstdb % dp->i_mount->m_dir_geo->free_max_bests) == 0);
>>>>>>> upstream/android-13
	ASSERT(hdr.firstdb <= db);
	ASSERT(db < hdr.firstdb + hdr.nvalid);
}
#else
#define xfs_dir2_free_hdr_check(dp, bp, db)
#endif	/* DEBUG */

/*
 * Return the last hash value in the leaf.
 * Stale entries are ok.
 */
xfs_dahash_t					/* hash value */
xfs_dir2_leaf_lasthash(
	struct xfs_inode *dp,
	struct xfs_buf	*bp,			/* leaf buffer */
	int		*count)			/* count of entries in leaf */
{
<<<<<<< HEAD
	struct xfs_dir2_leaf	*leaf = bp->b_addr;
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_dir3_icleaf_hdr leafhdr;

	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
=======
	struct xfs_dir3_icleaf_hdr leafhdr;

	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &leafhdr, bp->b_addr);
>>>>>>> upstream/android-13

	ASSERT(leafhdr.magic == XFS_DIR2_LEAFN_MAGIC ||
	       leafhdr.magic == XFS_DIR3_LEAFN_MAGIC ||
	       leafhdr.magic == XFS_DIR2_LEAF1_MAGIC ||
	       leafhdr.magic == XFS_DIR3_LEAF1_MAGIC);

	if (count)
		*count = leafhdr.count;
	if (!leafhdr.count)
		return 0;
<<<<<<< HEAD

	ents = dp->d_ops->leaf_ents_p(leaf);
	return be32_to_cpu(ents[leafhdr.count - 1].hashval);
=======
	return be32_to_cpu(leafhdr.ents[leafhdr.count - 1].hashval);
>>>>>>> upstream/android-13
}

/*
 * Look up a leaf entry for space to add a name in a node-format leaf block.
 * The extrablk in state is a freespace block.
 */
STATIC int
xfs_dir2_leafn_lookup_for_addname(
	struct xfs_buf		*bp,		/* leaf buffer */
	xfs_da_args_t		*args,		/* operation arguments */
	int			*indexp,	/* out: leaf entry index */
	xfs_da_state_t		*state)		/* state to fill in */
{
	struct xfs_buf		*curbp = NULL;	/* current data/free buffer */
	xfs_dir2_db_t		curdb = -1;	/* current data block number */
	xfs_dir2_db_t		curfdb = -1;	/* current free block number */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return value */
	int			fi;		/* free entry index */
	xfs_dir2_free_t		*free = NULL;	/* free block structure */
	int			index;		/* leaf entry index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	int			length;		/* length of new data entry */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_dir2_db_t		newdb;		/* new data block number */
	xfs_dir2_db_t		newfdb;		/* new free block number */
	xfs_trans_t		*tp;		/* transaction pointer */
<<<<<<< HEAD
	struct xfs_dir2_leaf_entry *ents;
=======
>>>>>>> upstream/android-13
	struct xfs_dir3_icleaf_hdr leafhdr;

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
	leaf = bp->b_addr;
<<<<<<< HEAD
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
=======
	xfs_dir2_leaf_hdr_from_disk(mp, &leafhdr, leaf);
>>>>>>> upstream/android-13

	xfs_dir3_leaf_check(dp, bp);
	ASSERT(leafhdr.count > 0);

	/*
	 * Look up the hash value in the leaf entries.
	 */
	index = xfs_dir2_leaf_search_hash(args, bp);
	/*
	 * Do we have a buffer coming in?
	 */
	if (state->extravalid) {
		/* If so, it's a free block buffer, get the block number. */
		curbp = state->extrablk.bp;
		curfdb = state->extrablk.blkno;
		free = curbp->b_addr;
		ASSERT(free->hdr.magic == cpu_to_be32(XFS_DIR2_FREE_MAGIC) ||
		       free->hdr.magic == cpu_to_be32(XFS_DIR3_FREE_MAGIC));
	}
<<<<<<< HEAD
	length = dp->d_ops->data_entsize(args->namelen);
	/*
	 * Loop over leaf entries with the right hash value.
	 */
	for (lep = &ents[index];
=======
	length = xfs_dir2_data_entsize(mp, args->namelen);
	/*
	 * Loop over leaf entries with the right hash value.
	 */
	for (lep = &leafhdr.ents[index];
>>>>>>> upstream/android-13
	     index < leafhdr.count && be32_to_cpu(lep->hashval) == args->hashval;
	     lep++, index++) {
		/*
		 * Skip stale leaf entries.
		 */
		if (be32_to_cpu(lep->address) == XFS_DIR2_NULL_DATAPTR)
			continue;
		/*
		 * Pull the data block number from the entry.
		 */
		newdb = xfs_dir2_dataptr_to_db(args->geo,
					       be32_to_cpu(lep->address));
		/*
		 * For addname, we're looking for a place to put the new entry.
		 * We want to use a data block with an entry of equal
		 * hash value to ours if there is one with room.
		 *
		 * If this block isn't the data block we already have
		 * in hand, take a look at it.
		 */
		if (newdb != curdb) {
<<<<<<< HEAD
			__be16 *bests;
=======
			struct xfs_dir3_icfree_hdr freehdr;
>>>>>>> upstream/android-13

			curdb = newdb;
			/*
			 * Convert the data block to the free block
			 * holding its freespace information.
			 */
<<<<<<< HEAD
			newfdb = dp->d_ops->db_to_fdb(args->geo, newdb);
=======
			newfdb = xfs_dir2_db_to_fdb(args->geo, newdb);
>>>>>>> upstream/android-13
			/*
			 * If it's not the one we have in hand, read it in.
			 */
			if (newfdb != curfdb) {
				/*
				 * If we had one before, drop it.
				 */
				if (curbp)
					xfs_trans_brelse(tp, curbp);

				error = xfs_dir2_free_read(tp, dp,
						xfs_dir2_db_to_da(args->geo,
								  newfdb),
						&curbp);
				if (error)
					return error;
				free = curbp->b_addr;

				xfs_dir2_free_hdr_check(dp, curbp, curdb);
			}
			/*
			 * Get the index for our entry.
			 */
<<<<<<< HEAD
			fi = dp->d_ops->db_to_fdindex(args->geo, curdb);
			/*
			 * If it has room, return it.
			 */
			bests = dp->d_ops->free_bests_p(free);
			if (unlikely(bests[fi] == cpu_to_be16(NULLDATAOFF))) {
				XFS_ERROR_REPORT("xfs_dir2_leafn_lookup_int",
							XFS_ERRLEVEL_LOW, mp);
=======
			fi = xfs_dir2_db_to_fdindex(args->geo, curdb);
			/*
			 * If it has room, return it.
			 */
			xfs_dir2_free_hdr_from_disk(mp, &freehdr, free);
			if (XFS_IS_CORRUPT(mp,
					   freehdr.bests[fi] ==
					   cpu_to_be16(NULLDATAOFF))) {
>>>>>>> upstream/android-13
				if (curfdb != newfdb)
					xfs_trans_brelse(tp, curbp);
				return -EFSCORRUPTED;
			}
			curfdb = newfdb;
<<<<<<< HEAD
			if (be16_to_cpu(bests[fi]) >= length)
=======
			if (be16_to_cpu(freehdr.bests[fi]) >= length)
>>>>>>> upstream/android-13
				goto out;
		}
	}
	/* Didn't find any space */
	fi = -1;
out:
	ASSERT(args->op_flags & XFS_DA_OP_OKNOENT);
	if (curbp) {
		/* Giving back a free block. */
		state->extravalid = 1;
		state->extrablk.bp = curbp;
		state->extrablk.index = fi;
		state->extrablk.blkno = curfdb;

		/*
		 * Important: this magic number is not in the buffer - it's for
		 * buffer type information and therefore only the free/data type
		 * matters here, not whether CRCs are enabled or not.
		 */
		state->extrablk.magic = XFS_DIR2_FREE_MAGIC;
	} else {
		state->extravalid = 0;
	}
	/*
	 * Return the index, that will be the insertion point.
	 */
	*indexp = index;
	return -ENOENT;
}

/*
 * Look up a leaf entry in a node-format leaf block.
 * The extrablk in state a data block.
 */
STATIC int
xfs_dir2_leafn_lookup_for_entry(
	struct xfs_buf		*bp,		/* leaf buffer */
	xfs_da_args_t		*args,		/* operation arguments */
	int			*indexp,	/* out: leaf entry index */
	xfs_da_state_t		*state)		/* state to fill in */
{
	struct xfs_buf		*curbp = NULL;	/* current data/free buffer */
	xfs_dir2_db_t		curdb = -1;	/* current data block number */
	xfs_dir2_data_entry_t	*dep;		/* data block entry */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return value */
	int			index;		/* leaf entry index */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	xfs_mount_t		*mp;		/* filesystem mount point */
	xfs_dir2_db_t		newdb;		/* new data block number */
	xfs_trans_t		*tp;		/* transaction pointer */
	enum xfs_dacmp		cmp;		/* comparison result */
<<<<<<< HEAD
	struct xfs_dir2_leaf_entry *ents;
=======
>>>>>>> upstream/android-13
	struct xfs_dir3_icleaf_hdr leafhdr;

	dp = args->dp;
	tp = args->trans;
	mp = dp->i_mount;
	leaf = bp->b_addr;
<<<<<<< HEAD
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);

	xfs_dir3_leaf_check(dp, bp);
	ASSERT(leafhdr.count > 0);
=======
	xfs_dir2_leaf_hdr_from_disk(mp, &leafhdr, leaf);

	xfs_dir3_leaf_check(dp, bp);
	if (leafhdr.count <= 0) {
		xfs_buf_mark_corrupt(bp);
		return -EFSCORRUPTED;
	}
>>>>>>> upstream/android-13

	/*
	 * Look up the hash value in the leaf entries.
	 */
	index = xfs_dir2_leaf_search_hash(args, bp);
	/*
	 * Do we have a buffer coming in?
	 */
	if (state->extravalid) {
		curbp = state->extrablk.bp;
		curdb = state->extrablk.blkno;
	}
	/*
	 * Loop over leaf entries with the right hash value.
	 */
<<<<<<< HEAD
	for (lep = &ents[index];
=======
	for (lep = &leafhdr.ents[index];
>>>>>>> upstream/android-13
	     index < leafhdr.count && be32_to_cpu(lep->hashval) == args->hashval;
	     lep++, index++) {
		/*
		 * Skip stale leaf entries.
		 */
		if (be32_to_cpu(lep->address) == XFS_DIR2_NULL_DATAPTR)
			continue;
		/*
		 * Pull the data block number from the entry.
		 */
		newdb = xfs_dir2_dataptr_to_db(args->geo,
					       be32_to_cpu(lep->address));
		/*
		 * Not adding a new entry, so we really want to find
		 * the name given to us.
		 *
		 * If it's a different data block, go get it.
		 */
		if (newdb != curdb) {
			/*
			 * If we had a block before that we aren't saving
			 * for a CI name, drop it
			 */
			if (curbp && (args->cmpresult == XFS_CMP_DIFFERENT ||
						curdb != state->extrablk.blkno))
				xfs_trans_brelse(tp, curbp);
			/*
			 * If needing the block that is saved with a CI match,
			 * use it otherwise read in the new data block.
			 */
			if (args->cmpresult != XFS_CMP_DIFFERENT &&
					newdb == state->extrablk.blkno) {
				ASSERT(state->extravalid);
				curbp = state->extrablk.bp;
			} else {
				error = xfs_dir3_data_read(tp, dp,
						xfs_dir2_db_to_da(args->geo,
								  newdb),
<<<<<<< HEAD
						-1, &curbp);
=======
						0, &curbp);
>>>>>>> upstream/android-13
				if (error)
					return error;
			}
			xfs_dir3_data_check(dp, curbp);
			curdb = newdb;
		}
		/*
		 * Point to the data entry.
		 */
		dep = (xfs_dir2_data_entry_t *)((char *)curbp->b_addr +
			xfs_dir2_dataptr_to_off(args->geo,
						be32_to_cpu(lep->address)));
		/*
		 * Compare the entry and if it's an exact match, return
		 * EEXIST immediately. If it's the first case-insensitive
		 * match, store the block & inode number and continue looking.
		 */
<<<<<<< HEAD
		cmp = mp->m_dirnameops->compname(args, dep->name, dep->namelen);
=======
		cmp = xfs_dir2_compname(args, dep->name, dep->namelen);
>>>>>>> upstream/android-13
		if (cmp != XFS_CMP_DIFFERENT && cmp != args->cmpresult) {
			/* If there is a CI match block, drop it */
			if (args->cmpresult != XFS_CMP_DIFFERENT &&
						curdb != state->extrablk.blkno)
				xfs_trans_brelse(tp, state->extrablk.bp);
			args->cmpresult = cmp;
			args->inumber = be64_to_cpu(dep->inumber);
<<<<<<< HEAD
			args->filetype = dp->d_ops->data_get_ftype(dep);
=======
			args->filetype = xfs_dir2_data_get_ftype(mp, dep);
>>>>>>> upstream/android-13
			*indexp = index;
			state->extravalid = 1;
			state->extrablk.bp = curbp;
			state->extrablk.blkno = curdb;
			state->extrablk.index = (int)((char *)dep -
							(char *)curbp->b_addr);
			state->extrablk.magic = XFS_DIR2_DATA_MAGIC;
			curbp->b_ops = &xfs_dir3_data_buf_ops;
			xfs_trans_buf_set_type(tp, curbp, XFS_BLFT_DIR_DATA_BUF);
			if (cmp == XFS_CMP_EXACT)
				return -EEXIST;
		}
	}
	ASSERT(index == leafhdr.count || (args->op_flags & XFS_DA_OP_OKNOENT));
	if (curbp) {
		if (args->cmpresult == XFS_CMP_DIFFERENT) {
			/* Giving back last used data block. */
			state->extravalid = 1;
			state->extrablk.bp = curbp;
			state->extrablk.index = -1;
			state->extrablk.blkno = curdb;
			state->extrablk.magic = XFS_DIR2_DATA_MAGIC;
			curbp->b_ops = &xfs_dir3_data_buf_ops;
			xfs_trans_buf_set_type(tp, curbp, XFS_BLFT_DIR_DATA_BUF);
		} else {
			/* If the curbp is not the CI match block, drop it */
			if (state->extrablk.bp != curbp)
				xfs_trans_brelse(tp, curbp);
		}
	} else {
		state->extravalid = 0;
	}
	*indexp = index;
	return -ENOENT;
}

/*
 * Look up a leaf entry in a node-format leaf block.
 * If this is an addname then the extrablk in state is a freespace block,
 * otherwise it's a data block.
 */
int
xfs_dir2_leafn_lookup_int(
	struct xfs_buf		*bp,		/* leaf buffer */
	xfs_da_args_t		*args,		/* operation arguments */
	int			*indexp,	/* out: leaf entry index */
	xfs_da_state_t		*state)		/* state to fill in */
{
	if (args->op_flags & XFS_DA_OP_ADDNAME)
		return xfs_dir2_leafn_lookup_for_addname(bp, args, indexp,
							state);
	return xfs_dir2_leafn_lookup_for_entry(bp, args, indexp, state);
}

/*
 * Move count leaf entries from source to destination leaf.
 * Log entries and headers.  Stale entries are preserved.
 */
static void
xfs_dir3_leafn_moveents(
	xfs_da_args_t			*args,	/* operation arguments */
	struct xfs_buf			*bp_s,	/* source */
	struct xfs_dir3_icleaf_hdr	*shdr,
	struct xfs_dir2_leaf_entry	*sents,
	int				start_s,/* source leaf index */
	struct xfs_buf			*bp_d,	/* destination */
	struct xfs_dir3_icleaf_hdr	*dhdr,
	struct xfs_dir2_leaf_entry	*dents,
	int				start_d,/* destination leaf index */
	int				count)	/* count of leaves to copy */
{
	int				stale;	/* count stale leaves copied */

	trace_xfs_dir2_leafn_moveents(args, start_s, start_d, count);

	/*
	 * Silently return if nothing to do.
	 */
	if (count == 0)
		return;

	/*
	 * If the destination index is not the end of the current
	 * destination leaf entries, open up a hole in the destination
	 * to hold the new entries.
	 */
	if (start_d < dhdr->count) {
		memmove(&dents[start_d + count], &dents[start_d],
			(dhdr->count - start_d) * sizeof(xfs_dir2_leaf_entry_t));
<<<<<<< HEAD
		xfs_dir3_leaf_log_ents(args, bp_d, start_d + count,
=======
		xfs_dir3_leaf_log_ents(args, dhdr, bp_d, start_d + count,
>>>>>>> upstream/android-13
				       count + dhdr->count - 1);
	}
	/*
	 * If the source has stale leaves, count the ones in the copy range
	 * so we can update the header correctly.
	 */
	if (shdr->stale) {
		int	i;			/* temp leaf index */

		for (i = start_s, stale = 0; i < start_s + count; i++) {
			if (sents[i].address ==
					cpu_to_be32(XFS_DIR2_NULL_DATAPTR))
				stale++;
		}
	} else
		stale = 0;
	/*
	 * Copy the leaf entries from source to destination.
	 */
	memcpy(&dents[start_d], &sents[start_s],
		count * sizeof(xfs_dir2_leaf_entry_t));
<<<<<<< HEAD
	xfs_dir3_leaf_log_ents(args, bp_d, start_d, start_d + count - 1);
=======
	xfs_dir3_leaf_log_ents(args, dhdr, bp_d, start_d, start_d + count - 1);
>>>>>>> upstream/android-13

	/*
	 * If there are source entries after the ones we copied,
	 * delete the ones we copied by sliding the next ones down.
	 */
	if (start_s + count < shdr->count) {
		memmove(&sents[start_s], &sents[start_s + count],
			count * sizeof(xfs_dir2_leaf_entry_t));
<<<<<<< HEAD
		xfs_dir3_leaf_log_ents(args, bp_s, start_s, start_s + count - 1);
=======
		xfs_dir3_leaf_log_ents(args, shdr, bp_s, start_s,
				       start_s + count - 1);
>>>>>>> upstream/android-13
	}

	/*
	 * Update the headers and log them.
	 */
	shdr->count -= count;
	shdr->stale -= stale;
	dhdr->count += count;
	dhdr->stale += stale;
}

/*
 * Determine the sort order of two leaf blocks.
 * Returns 1 if both are valid and leaf2 should be before leaf1, else 0.
 */
int						/* sort order */
xfs_dir2_leafn_order(
	struct xfs_inode	*dp,
	struct xfs_buf		*leaf1_bp,		/* leaf1 buffer */
	struct xfs_buf		*leaf2_bp)		/* leaf2 buffer */
{
	struct xfs_dir2_leaf	*leaf1 = leaf1_bp->b_addr;
	struct xfs_dir2_leaf	*leaf2 = leaf2_bp->b_addr;
	struct xfs_dir2_leaf_entry *ents1;
	struct xfs_dir2_leaf_entry *ents2;
	struct xfs_dir3_icleaf_hdr hdr1;
	struct xfs_dir3_icleaf_hdr hdr2;

<<<<<<< HEAD
	dp->d_ops->leaf_hdr_from_disk(&hdr1, leaf1);
	dp->d_ops->leaf_hdr_from_disk(&hdr2, leaf2);
	ents1 = dp->d_ops->leaf_ents_p(leaf1);
	ents2 = dp->d_ops->leaf_ents_p(leaf2);
=======
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &hdr1, leaf1);
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &hdr2, leaf2);
	ents1 = hdr1.ents;
	ents2 = hdr2.ents;
>>>>>>> upstream/android-13

	if (hdr1.count > 0 && hdr2.count > 0 &&
	    (be32_to_cpu(ents2[0].hashval) < be32_to_cpu(ents1[0].hashval) ||
	     be32_to_cpu(ents2[hdr2.count - 1].hashval) <
				be32_to_cpu(ents1[hdr1.count - 1].hashval)))
		return 1;
	return 0;
}

/*
 * Rebalance leaf entries between two leaf blocks.
 * This is actually only called when the second block is new,
 * though the code deals with the general case.
 * A new entry will be inserted in one of the blocks, and that
 * entry is taken into account when balancing.
 */
static void
xfs_dir2_leafn_rebalance(
	xfs_da_state_t		*state,		/* btree cursor */
	xfs_da_state_blk_t	*blk1,		/* first btree block */
	xfs_da_state_blk_t	*blk2)		/* second btree block */
{
	xfs_da_args_t		*args;		/* operation arguments */
	int			count;		/* count (& direction) leaves */
	int			isleft;		/* new goes in left leaf */
	xfs_dir2_leaf_t		*leaf1;		/* first leaf structure */
	xfs_dir2_leaf_t		*leaf2;		/* second leaf structure */
	int			mid;		/* midpoint leaf index */
#if defined(DEBUG) || defined(XFS_WARN)
	int			oldstale;	/* old count of stale leaves */
#endif
	int			oldsum;		/* old total leaf count */
	int			swap_blocks;	/* swapped leaf blocks */
	struct xfs_dir2_leaf_entry *ents1;
	struct xfs_dir2_leaf_entry *ents2;
	struct xfs_dir3_icleaf_hdr hdr1;
	struct xfs_dir3_icleaf_hdr hdr2;
	struct xfs_inode	*dp = state->args->dp;

	args = state->args;
	/*
	 * If the block order is wrong, swap the arguments.
	 */
	swap_blocks = xfs_dir2_leafn_order(dp, blk1->bp, blk2->bp);
	if (swap_blocks)
		swap(blk1, blk2);

	leaf1 = blk1->bp->b_addr;
	leaf2 = blk2->bp->b_addr;
<<<<<<< HEAD
	dp->d_ops->leaf_hdr_from_disk(&hdr1, leaf1);
	dp->d_ops->leaf_hdr_from_disk(&hdr2, leaf2);
	ents1 = dp->d_ops->leaf_ents_p(leaf1);
	ents2 = dp->d_ops->leaf_ents_p(leaf2);
=======
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &hdr1, leaf1);
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &hdr2, leaf2);
	ents1 = hdr1.ents;
	ents2 = hdr2.ents;
>>>>>>> upstream/android-13

	oldsum = hdr1.count + hdr2.count;
#if defined(DEBUG) || defined(XFS_WARN)
	oldstale = hdr1.stale + hdr2.stale;
#endif
	mid = oldsum >> 1;

	/*
	 * If the old leaf count was odd then the new one will be even,
	 * so we need to divide the new count evenly.
	 */
	if (oldsum & 1) {
		xfs_dahash_t	midhash;	/* middle entry hash value */

		if (mid >= hdr1.count)
			midhash = be32_to_cpu(ents2[mid - hdr1.count].hashval);
		else
			midhash = be32_to_cpu(ents1[mid].hashval);
		isleft = args->hashval <= midhash;
	}
	/*
	 * If the old count is even then the new count is odd, so there's
	 * no preferred side for the new entry.
	 * Pick the left one.
	 */
	else
		isleft = 1;
	/*
	 * Calculate moved entry count.  Positive means left-to-right,
	 * negative means right-to-left.  Then move the entries.
	 */
	count = hdr1.count - mid + (isleft == 0);
	if (count > 0)
		xfs_dir3_leafn_moveents(args, blk1->bp, &hdr1, ents1,
					hdr1.count - count, blk2->bp,
					&hdr2, ents2, 0, count);
	else if (count < 0)
		xfs_dir3_leafn_moveents(args, blk2->bp, &hdr2, ents2, 0,
					blk1->bp, &hdr1, ents1,
					hdr1.count, count);

	ASSERT(hdr1.count + hdr2.count == oldsum);
	ASSERT(hdr1.stale + hdr2.stale == oldstale);

	/* log the changes made when moving the entries */
<<<<<<< HEAD
	dp->d_ops->leaf_hdr_to_disk(leaf1, &hdr1);
	dp->d_ops->leaf_hdr_to_disk(leaf2, &hdr2);
=======
	xfs_dir2_leaf_hdr_to_disk(dp->i_mount, leaf1, &hdr1);
	xfs_dir2_leaf_hdr_to_disk(dp->i_mount, leaf2, &hdr2);
>>>>>>> upstream/android-13
	xfs_dir3_leaf_log_header(args, blk1->bp);
	xfs_dir3_leaf_log_header(args, blk2->bp);

	xfs_dir3_leaf_check(dp, blk1->bp);
	xfs_dir3_leaf_check(dp, blk2->bp);

	/*
	 * Mark whether we're inserting into the old or new leaf.
	 */
	if (hdr1.count < hdr2.count)
		state->inleaf = swap_blocks;
	else if (hdr1.count > hdr2.count)
		state->inleaf = !swap_blocks;
	else
		state->inleaf = swap_blocks ^ (blk1->index <= hdr1.count);
	/*
	 * Adjust the expected index for insertion.
	 */
	if (!state->inleaf)
		blk2->index = blk1->index - hdr1.count;

	/*
	 * Finally sanity check just to make sure we are not returning a
	 * negative index
	 */
	if (blk2->index < 0) {
		state->inleaf = 1;
		blk2->index = 0;
		xfs_alert(dp->i_mount,
	"%s: picked the wrong leaf? reverting original leaf: blk1->index %d",
			__func__, blk1->index);
	}
}

static int
xfs_dir3_data_block_free(
	xfs_da_args_t		*args,
	struct xfs_dir2_data_hdr *hdr,
	struct xfs_dir2_free	*free,
	xfs_dir2_db_t		fdb,
	int			findex,
	struct xfs_buf		*fbp,
	int			longest)
{
	int			logfree = 0;
<<<<<<< HEAD
	__be16			*bests;
	struct xfs_dir3_icfree_hdr freehdr;
	struct xfs_inode	*dp = args->dp;

	dp->d_ops->free_hdr_from_disk(&freehdr, free);
	bests = dp->d_ops->free_bests_p(free);
=======
	struct xfs_dir3_icfree_hdr freehdr;
	struct xfs_inode	*dp = args->dp;

	xfs_dir2_free_hdr_from_disk(dp->i_mount, &freehdr, free);
>>>>>>> upstream/android-13
	if (hdr) {
		/*
		 * Data block is not empty, just set the free entry to the new
		 * value.
		 */
<<<<<<< HEAD
		bests[findex] = cpu_to_be16(longest);
		xfs_dir2_free_log_bests(args, fbp, findex, findex);
=======
		freehdr.bests[findex] = cpu_to_be16(longest);
		xfs_dir2_free_log_bests(args, &freehdr, fbp, findex, findex);
>>>>>>> upstream/android-13
		return 0;
	}

	/* One less used entry in the free table. */
	freehdr.nused--;

	/*
	 * If this was the last entry in the table, we can trim the table size
	 * back.  There might be other entries at the end referring to
	 * non-existent data blocks, get those too.
	 */
	if (findex == freehdr.nvalid - 1) {
		int	i;		/* free entry index */

		for (i = findex - 1; i >= 0; i--) {
<<<<<<< HEAD
			if (bests[i] != cpu_to_be16(NULLDATAOFF))
=======
			if (freehdr.bests[i] != cpu_to_be16(NULLDATAOFF))
>>>>>>> upstream/android-13
				break;
		}
		freehdr.nvalid = i + 1;
		logfree = 0;
	} else {
		/* Not the last entry, just punch it out.  */
<<<<<<< HEAD
		bests[findex] = cpu_to_be16(NULLDATAOFF);
		logfree = 1;
	}

	dp->d_ops->free_hdr_to_disk(free, &freehdr);
=======
		freehdr.bests[findex] = cpu_to_be16(NULLDATAOFF);
		logfree = 1;
	}

	xfs_dir2_free_hdr_to_disk(dp->i_mount, free, &freehdr);
>>>>>>> upstream/android-13
	xfs_dir2_free_log_header(args, fbp);

	/*
	 * If there are no useful entries left in the block, get rid of the
	 * block if we can.
	 */
	if (!freehdr.nused) {
		int error;

		error = xfs_dir2_shrink_inode(args, fdb, fbp);
		if (error == 0) {
			fbp = NULL;
			logfree = 0;
		} else if (error != -ENOSPC || args->total != 0)
			return error;
		/*
		 * It's possible to get ENOSPC if there is no
		 * space reservation.  In this case some one
		 * else will eventually get rid of this block.
		 */
	}

	/* Log the free entry that changed, unless we got rid of it.  */
	if (logfree)
<<<<<<< HEAD
		xfs_dir2_free_log_bests(args, fbp, findex, findex);
=======
		xfs_dir2_free_log_bests(args, &freehdr, fbp, findex, findex);
>>>>>>> upstream/android-13
	return 0;
}

/*
 * Remove an entry from a node directory.
 * This removes the leaf entry and the data entry,
 * and updates the free block if necessary.
 */
static int					/* error */
xfs_dir2_leafn_remove(
	xfs_da_args_t		*args,		/* operation arguments */
	struct xfs_buf		*bp,		/* leaf buffer */
	int			index,		/* leaf entry index */
	xfs_da_state_blk_t	*dblk,		/* data block */
	int			*rval)		/* resulting block needs join */
{
<<<<<<< HEAD
=======
	struct xfs_da_geometry	*geo = args->geo;
>>>>>>> upstream/android-13
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
	xfs_dir2_db_t		db;		/* data block number */
	struct xfs_buf		*dbp;		/* data block buffer */
	xfs_dir2_data_entry_t	*dep;		/* data block entry */
	xfs_inode_t		*dp;		/* incore directory inode */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry */
	int			longest;	/* longest data free entry */
	int			off;		/* data block entry offset */
	int			needlog;	/* need to log data header */
	int			needscan;	/* need to rescan data frees */
	xfs_trans_t		*tp;		/* transaction pointer */
	struct xfs_dir2_data_free *bf;		/* bestfree table */
	struct xfs_dir3_icleaf_hdr leafhdr;
<<<<<<< HEAD
	struct xfs_dir2_leaf_entry *ents;
=======
>>>>>>> upstream/android-13

	trace_xfs_dir2_leafn_remove(args, index);

	dp = args->dp;
	tp = args->trans;
	leaf = bp->b_addr;
<<<<<<< HEAD
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
=======
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &leafhdr, leaf);
>>>>>>> upstream/android-13

	/*
	 * Point to the entry we're removing.
	 */
<<<<<<< HEAD
	lep = &ents[index];
=======
	lep = &leafhdr.ents[index];
>>>>>>> upstream/android-13

	/*
	 * Extract the data block and offset from the entry.
	 */
<<<<<<< HEAD
	db = xfs_dir2_dataptr_to_db(args->geo, be32_to_cpu(lep->address));
	ASSERT(dblk->blkno == db);
	off = xfs_dir2_dataptr_to_off(args->geo, be32_to_cpu(lep->address));
=======
	db = xfs_dir2_dataptr_to_db(geo, be32_to_cpu(lep->address));
	ASSERT(dblk->blkno == db);
	off = xfs_dir2_dataptr_to_off(geo, be32_to_cpu(lep->address));
>>>>>>> upstream/android-13
	ASSERT(dblk->index == off);

	/*
	 * Kill the leaf entry by marking it stale.
	 * Log the leaf block changes.
	 */
	leafhdr.stale++;
<<<<<<< HEAD
	dp->d_ops->leaf_hdr_to_disk(leaf, &leafhdr);
	xfs_dir3_leaf_log_header(args, bp);

	lep->address = cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
	xfs_dir3_leaf_log_ents(args, bp, index, index);
=======
	xfs_dir2_leaf_hdr_to_disk(dp->i_mount, leaf, &leafhdr);
	xfs_dir3_leaf_log_header(args, bp);

	lep->address = cpu_to_be32(XFS_DIR2_NULL_DATAPTR);
	xfs_dir3_leaf_log_ents(args, &leafhdr, bp, index, index);
>>>>>>> upstream/android-13

	/*
	 * Make the data entry free.  Keep track of the longest freespace
	 * in the data block in case it changes.
	 */
	dbp = dblk->bp;
	hdr = dbp->b_addr;
	dep = (xfs_dir2_data_entry_t *)((char *)hdr + off);
<<<<<<< HEAD
	bf = dp->d_ops->data_bestfree_p(hdr);
	longest = be16_to_cpu(bf[0].length);
	needlog = needscan = 0;
	xfs_dir2_data_make_free(args, dbp, off,
		dp->d_ops->data_entsize(dep->namelen), &needlog, &needscan);
=======
	bf = xfs_dir2_data_bestfree_p(dp->i_mount, hdr);
	longest = be16_to_cpu(bf[0].length);
	needlog = needscan = 0;
	xfs_dir2_data_make_free(args, dbp, off,
		xfs_dir2_data_entsize(dp->i_mount, dep->namelen), &needlog,
		&needscan);
>>>>>>> upstream/android-13
	/*
	 * Rescan the data block freespaces for bestfree.
	 * Log the data block header if needed.
	 */
	if (needscan)
<<<<<<< HEAD
		xfs_dir2_data_freescan(dp, hdr, &needlog);
=======
		xfs_dir2_data_freescan(dp->i_mount, hdr, &needlog);
>>>>>>> upstream/android-13
	if (needlog)
		xfs_dir2_data_log_header(args, dbp);
	xfs_dir3_data_check(dp, dbp);
	/*
	 * If the longest data block freespace changes, need to update
	 * the corresponding freeblock entry.
	 */
	if (longest < be16_to_cpu(bf[0].length)) {
		int		error;		/* error return value */
		struct xfs_buf	*fbp;		/* freeblock buffer */
		xfs_dir2_db_t	fdb;		/* freeblock block number */
		int		findex;		/* index in freeblock entries */
		xfs_dir2_free_t	*free;		/* freeblock structure */

		/*
		 * Convert the data block number to a free block,
		 * read in the free block.
		 */
<<<<<<< HEAD
		fdb = dp->d_ops->db_to_fdb(args->geo, db);
		error = xfs_dir2_free_read(tp, dp,
					   xfs_dir2_db_to_da(args->geo, fdb),
=======
		fdb = xfs_dir2_db_to_fdb(geo, db);
		error = xfs_dir2_free_read(tp, dp, xfs_dir2_db_to_da(geo, fdb),
>>>>>>> upstream/android-13
					   &fbp);
		if (error)
			return error;
		free = fbp->b_addr;
#ifdef DEBUG
	{
		struct xfs_dir3_icfree_hdr freehdr;
<<<<<<< HEAD
		dp->d_ops->free_hdr_from_disk(&freehdr, free);
		ASSERT(freehdr.firstdb == dp->d_ops->free_max_bests(args->geo) *
			(fdb - xfs_dir2_byte_to_db(args->geo,
						   XFS_DIR2_FREE_OFFSET)));
=======

		xfs_dir2_free_hdr_from_disk(dp->i_mount, &freehdr, free);
		ASSERT(freehdr.firstdb == geo->free_max_bests *
			(fdb - xfs_dir2_byte_to_db(geo, XFS_DIR2_FREE_OFFSET)));
>>>>>>> upstream/android-13
	}
#endif
		/*
		 * Calculate which entry we need to fix.
		 */
<<<<<<< HEAD
		findex = dp->d_ops->db_to_fdindex(args->geo, db);
=======
		findex = xfs_dir2_db_to_fdindex(geo, db);
>>>>>>> upstream/android-13
		longest = be16_to_cpu(bf[0].length);
		/*
		 * If the data block is now empty we can get rid of it
		 * (usually).
		 */
<<<<<<< HEAD
		if (longest == args->geo->blksize -
			       dp->d_ops->data_entry_offset) {
=======
		if (longest == geo->blksize - geo->data_entry_offset) {
>>>>>>> upstream/android-13
			/*
			 * Try to punch out the data block.
			 */
			error = xfs_dir2_shrink_inode(args, db, dbp);
			if (error == 0) {
				dblk->bp = NULL;
				hdr = NULL;
			}
			/*
			 * We can get ENOSPC if there's no space reservation.
			 * In this case just drop the buffer and some one else
			 * will eventually get rid of the empty block.
			 */
			else if (!(error == -ENOSPC && args->total == 0))
				return error;
		}
		/*
		 * If we got rid of the data block, we can eliminate that entry
		 * in the free block.
		 */
		error = xfs_dir3_data_block_free(args, hdr, free,
						 fdb, findex, fbp, longest);
		if (error)
			return error;
	}

	xfs_dir3_leaf_check(dp, bp);
	/*
	 * Return indication of whether this leaf block is empty enough
	 * to justify trying to join it with a neighbor.
	 */
<<<<<<< HEAD
	*rval = (dp->d_ops->leaf_hdr_size +
		 (uint)sizeof(ents[0]) * (leafhdr.count - leafhdr.stale)) <
		args->geo->magicpct;
=======
	*rval = (geo->leaf_hdr_size +
		 (uint)sizeof(leafhdr.ents) * (leafhdr.count - leafhdr.stale)) <
		geo->magicpct;
>>>>>>> upstream/android-13
	return 0;
}

/*
 * Split the leaf entries in the old block into old and new blocks.
 */
int						/* error */
xfs_dir2_leafn_split(
	xfs_da_state_t		*state,		/* btree cursor */
	xfs_da_state_blk_t	*oldblk,	/* original block */
	xfs_da_state_blk_t	*newblk)	/* newly created block */
{
	xfs_da_args_t		*args;		/* operation arguments */
	xfs_dablk_t		blkno;		/* new leaf block number */
	int			error;		/* error return value */
	struct xfs_inode	*dp;

	/*
	 * Allocate space for a new leaf node.
	 */
	args = state->args;
	dp = args->dp;
	ASSERT(oldblk->magic == XFS_DIR2_LEAFN_MAGIC);
	error = xfs_da_grow_inode(args, &blkno);
	if (error) {
		return error;
	}
	/*
	 * Initialize the new leaf block.
	 */
	error = xfs_dir3_leaf_get_buf(args, xfs_dir2_da_to_db(args->geo, blkno),
				      &newblk->bp, XFS_DIR2_LEAFN_MAGIC);
	if (error)
		return error;

	newblk->blkno = blkno;
	newblk->magic = XFS_DIR2_LEAFN_MAGIC;
	/*
	 * Rebalance the entries across the two leaves, link the new
	 * block into the leaves.
	 */
	xfs_dir2_leafn_rebalance(state, oldblk, newblk);
	error = xfs_da3_blk_link(state, oldblk, newblk);
	if (error) {
		return error;
	}
	/*
	 * Insert the new entry in the correct block.
	 */
	if (state->inleaf)
		error = xfs_dir2_leafn_add(oldblk->bp, args, oldblk->index);
	else
		error = xfs_dir2_leafn_add(newblk->bp, args, newblk->index);
	/*
	 * Update last hashval in each block since we added the name.
	 */
	oldblk->hashval = xfs_dir2_leaf_lasthash(dp, oldblk->bp, NULL);
	newblk->hashval = xfs_dir2_leaf_lasthash(dp, newblk->bp, NULL);
	xfs_dir3_leaf_check(dp, oldblk->bp);
	xfs_dir3_leaf_check(dp, newblk->bp);
	return error;
}

/*
 * Check a leaf block and its neighbors to see if the block should be
 * collapsed into one or the other neighbor.  Always keep the block
 * with the smaller block number.
 * If the current block is over 50% full, don't try to join it, return 0.
 * If the block is empty, fill in the state structure and return 2.
 * If it can be collapsed, fill in the state structure and return 1.
 * If nothing can be done, return 0.
 */
int						/* error */
xfs_dir2_leafn_toosmall(
	xfs_da_state_t		*state,		/* btree cursor */
	int			*action)	/* resulting action to take */
{
	xfs_da_state_blk_t	*blk;		/* leaf block */
	xfs_dablk_t		blkno;		/* leaf block number */
	struct xfs_buf		*bp;		/* leaf buffer */
	int			bytes;		/* bytes in use */
	int			count;		/* leaf live entry count */
	int			error;		/* error return value */
	int			forward;	/* sibling block direction */
	int			i;		/* sibling counter */
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	int			rval;		/* result from path_shift */
	struct xfs_dir3_icleaf_hdr leafhdr;
	struct xfs_dir2_leaf_entry *ents;
	struct xfs_inode	*dp = state->args->dp;

	/*
	 * Check for the degenerate case of the block being over 50% full.
	 * If so, it's not worth even looking to see if we might be able
	 * to coalesce with a sibling.
	 */
	blk = &state->path.blk[state->path.active - 1];
	leaf = blk->bp->b_addr;
<<<<<<< HEAD
	dp->d_ops->leaf_hdr_from_disk(&leafhdr, leaf);
	ents = dp->d_ops->leaf_ents_p(leaf);
	xfs_dir3_leaf_check(dp, blk->bp);

	count = leafhdr.count - leafhdr.stale;
	bytes = dp->d_ops->leaf_hdr_size + count * sizeof(ents[0]);
=======
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &leafhdr, leaf);
	ents = leafhdr.ents;
	xfs_dir3_leaf_check(dp, blk->bp);

	count = leafhdr.count - leafhdr.stale;
	bytes = state->args->geo->leaf_hdr_size + count * sizeof(ents[0]);
>>>>>>> upstream/android-13
	if (bytes > (state->args->geo->blksize >> 1)) {
		/*
		 * Blk over 50%, don't try to join.
		 */
		*action = 0;
		return 0;
	}
	/*
	 * Check for the degenerate case of the block being empty.
	 * If the block is empty, we'll simply delete it, no need to
	 * coalesce it with a sibling block.  We choose (arbitrarily)
	 * to merge with the forward block unless it is NULL.
	 */
	if (count == 0) {
		/*
		 * Make altpath point to the block we want to keep and
		 * path point to the block we want to drop (this one).
		 */
		forward = (leafhdr.forw != 0);
		memcpy(&state->altpath, &state->path, sizeof(state->path));
		error = xfs_da3_path_shift(state, &state->altpath, forward, 0,
			&rval);
		if (error)
			return error;
		*action = rval ? 2 : 0;
		return 0;
	}
	/*
	 * Examine each sibling block to see if we can coalesce with
	 * at least 25% free space to spare.  We need to figure out
	 * whether to merge with the forward or the backward block.
	 * We prefer coalescing with the lower numbered sibling so as
	 * to shrink a directory over time.
	 */
	forward = leafhdr.forw < leafhdr.back;
	for (i = 0, bp = NULL; i < 2; forward = !forward, i++) {
		struct xfs_dir3_icleaf_hdr hdr2;

		blkno = forward ? leafhdr.forw : leafhdr.back;
		if (blkno == 0)
			continue;
		/*
		 * Read the sibling leaf block.
		 */
<<<<<<< HEAD
		error = xfs_dir3_leafn_read(state->args->trans, dp,
					    blkno, -1, &bp);
=======
		error = xfs_dir3_leafn_read(state->args->trans, dp, blkno, &bp);
>>>>>>> upstream/android-13
		if (error)
			return error;

		/*
		 * Count bytes in the two blocks combined.
		 */
		count = leafhdr.count - leafhdr.stale;
		bytes = state->args->geo->blksize -
			(state->args->geo->blksize >> 2);

		leaf = bp->b_addr;
<<<<<<< HEAD
		dp->d_ops->leaf_hdr_from_disk(&hdr2, leaf);
		ents = dp->d_ops->leaf_ents_p(leaf);
=======
		xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &hdr2, leaf);
		ents = hdr2.ents;
>>>>>>> upstream/android-13
		count += hdr2.count - hdr2.stale;
		bytes -= count * sizeof(ents[0]);

		/*
		 * Fits with at least 25% to spare.
		 */
		if (bytes >= 0)
			break;
		xfs_trans_brelse(state->args->trans, bp);
	}
	/*
	 * Didn't like either block, give up.
	 */
	if (i >= 2) {
		*action = 0;
		return 0;
	}

	/*
	 * Make altpath point to the block we want to keep (the lower
	 * numbered block) and path point to the block we want to drop.
	 */
	memcpy(&state->altpath, &state->path, sizeof(state->path));
	if (blkno < blk->blkno)
		error = xfs_da3_path_shift(state, &state->altpath, forward, 0,
			&rval);
	else
		error = xfs_da3_path_shift(state, &state->path, forward, 0,
			&rval);
	if (error) {
		return error;
	}
	*action = rval ? 0 : 1;
	return 0;
}

/*
 * Move all the leaf entries from drop_blk to save_blk.
 * This is done as part of a join operation.
 */
void
xfs_dir2_leafn_unbalance(
	xfs_da_state_t		*state,		/* cursor */
	xfs_da_state_blk_t	*drop_blk,	/* dead block */
	xfs_da_state_blk_t	*save_blk)	/* surviving block */
{
	xfs_da_args_t		*args;		/* operation arguments */
	xfs_dir2_leaf_t		*drop_leaf;	/* dead leaf structure */
	xfs_dir2_leaf_t		*save_leaf;	/* surviving leaf structure */
	struct xfs_dir3_icleaf_hdr savehdr;
	struct xfs_dir3_icleaf_hdr drophdr;
	struct xfs_dir2_leaf_entry *sents;
	struct xfs_dir2_leaf_entry *dents;
	struct xfs_inode	*dp = state->args->dp;

	args = state->args;
	ASSERT(drop_blk->magic == XFS_DIR2_LEAFN_MAGIC);
	ASSERT(save_blk->magic == XFS_DIR2_LEAFN_MAGIC);
	drop_leaf = drop_blk->bp->b_addr;
	save_leaf = save_blk->bp->b_addr;

<<<<<<< HEAD
	dp->d_ops->leaf_hdr_from_disk(&savehdr, save_leaf);
	dp->d_ops->leaf_hdr_from_disk(&drophdr, drop_leaf);
	sents = dp->d_ops->leaf_ents_p(save_leaf);
	dents = dp->d_ops->leaf_ents_p(drop_leaf);
=======
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &savehdr, save_leaf);
	xfs_dir2_leaf_hdr_from_disk(dp->i_mount, &drophdr, drop_leaf);
	sents = savehdr.ents;
	dents = drophdr.ents;
>>>>>>> upstream/android-13

	/*
	 * If there are any stale leaf entries, take this opportunity
	 * to purge them.
	 */
	if (drophdr.stale)
		xfs_dir3_leaf_compact(args, &drophdr, drop_blk->bp);
	if (savehdr.stale)
		xfs_dir3_leaf_compact(args, &savehdr, save_blk->bp);

	/*
	 * Move the entries from drop to the appropriate end of save.
	 */
	drop_blk->hashval = be32_to_cpu(dents[drophdr.count - 1].hashval);
	if (xfs_dir2_leafn_order(dp, save_blk->bp, drop_blk->bp))
		xfs_dir3_leafn_moveents(args, drop_blk->bp, &drophdr, dents, 0,
					save_blk->bp, &savehdr, sents, 0,
					drophdr.count);
	else
		xfs_dir3_leafn_moveents(args, drop_blk->bp, &drophdr, dents, 0,
					save_blk->bp, &savehdr, sents,
					savehdr.count, drophdr.count);
	save_blk->hashval = be32_to_cpu(sents[savehdr.count - 1].hashval);

	/* log the changes made when moving the entries */
<<<<<<< HEAD
	dp->d_ops->leaf_hdr_to_disk(save_leaf, &savehdr);
	dp->d_ops->leaf_hdr_to_disk(drop_leaf, &drophdr);
=======
	xfs_dir2_leaf_hdr_to_disk(dp->i_mount, save_leaf, &savehdr);
	xfs_dir2_leaf_hdr_to_disk(dp->i_mount, drop_leaf, &drophdr);
>>>>>>> upstream/android-13
	xfs_dir3_leaf_log_header(args, save_blk->bp);
	xfs_dir3_leaf_log_header(args, drop_blk->bp);

	xfs_dir3_leaf_check(dp, save_blk->bp);
	xfs_dir3_leaf_check(dp, drop_blk->bp);
}

/*
<<<<<<< HEAD
=======
 * Add a new data block to the directory at the free space index that the caller
 * has specified.
 */
static int
xfs_dir2_node_add_datablk(
	struct xfs_da_args	*args,
	struct xfs_da_state_blk	*fblk,
	xfs_dir2_db_t		*dbno,
	struct xfs_buf		**dbpp,
	struct xfs_buf		**fbpp,
	struct xfs_dir3_icfree_hdr *hdr,
	int			*findex)
{
	struct xfs_inode	*dp = args->dp;
	struct xfs_trans	*tp = args->trans;
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_dir2_data_free *bf;
	xfs_dir2_db_t		fbno;
	struct xfs_buf		*fbp;
	struct xfs_buf		*dbp;
	int			error;

	/* Not allowed to allocate, return failure. */
	if (args->total == 0)
		return -ENOSPC;

	/* Allocate and initialize the new data block.  */
	error = xfs_dir2_grow_inode(args, XFS_DIR2_DATA_SPACE, dbno);
	if (error)
		return error;
	error = xfs_dir3_data_init(args, *dbno, &dbp);
	if (error)
		return error;

	/*
	 * Get the freespace block corresponding to the data block
	 * that was just allocated.
	 */
	fbno = xfs_dir2_db_to_fdb(args->geo, *dbno);
	error = xfs_dir2_free_try_read(tp, dp,
			       xfs_dir2_db_to_da(args->geo, fbno), &fbp);
	if (error)
		return error;

	/*
	 * If there wasn't a freespace block, the read will
	 * return a NULL fbp.  Allocate and initialize a new one.
	 */
	if (!fbp) {
		error = xfs_dir2_grow_inode(args, XFS_DIR2_FREE_SPACE, &fbno);
		if (error)
			return error;

		if (XFS_IS_CORRUPT(mp,
				   xfs_dir2_db_to_fdb(args->geo, *dbno) !=
				   fbno)) {
			xfs_alert(mp,
"%s: dir ino %llu needed freesp block %lld for data block %lld, got %lld",
				__func__, (unsigned long long)dp->i_ino,
				(long long)xfs_dir2_db_to_fdb(args->geo, *dbno),
				(long long)*dbno, (long long)fbno);
			if (fblk) {
				xfs_alert(mp,
			" fblk "PTR_FMT" blkno %llu index %d magic 0x%x",
					fblk, (unsigned long long)fblk->blkno,
					fblk->index, fblk->magic);
			} else {
				xfs_alert(mp, " ... fblk is NULL");
			}
			return -EFSCORRUPTED;
		}

		/* Get a buffer for the new block. */
		error = xfs_dir3_free_get_buf(args, fbno, &fbp);
		if (error)
			return error;
		xfs_dir2_free_hdr_from_disk(mp, hdr, fbp->b_addr);

		/* Remember the first slot as our empty slot. */
		hdr->firstdb = (fbno - xfs_dir2_byte_to_db(args->geo,
							XFS_DIR2_FREE_OFFSET)) *
				args->geo->free_max_bests;
	} else {
		xfs_dir2_free_hdr_from_disk(mp, hdr, fbp->b_addr);
	}

	/* Set the freespace block index from the data block number. */
	*findex = xfs_dir2_db_to_fdindex(args->geo, *dbno);

	/* Extend the freespace table if the new data block is off the end. */
	if (*findex >= hdr->nvalid) {
		ASSERT(*findex < args->geo->free_max_bests);
		hdr->nvalid = *findex + 1;
		hdr->bests[*findex] = cpu_to_be16(NULLDATAOFF);
	}

	/*
	 * If this entry was for an empty data block (this should always be
	 * true) then update the header.
	 */
	if (hdr->bests[*findex] == cpu_to_be16(NULLDATAOFF)) {
		hdr->nused++;
		xfs_dir2_free_hdr_to_disk(mp, fbp->b_addr, hdr);
		xfs_dir2_free_log_header(args, fbp);
	}

	/* Update the freespace value for the new block in the table. */
	bf = xfs_dir2_data_bestfree_p(mp, dbp->b_addr);
	hdr->bests[*findex] = bf[0].length;

	*dbpp = dbp;
	*fbpp = fbp;
	return 0;
}

static int
xfs_dir2_node_find_freeblk(
	struct xfs_da_args	*args,
	struct xfs_da_state_blk	*fblk,
	xfs_dir2_db_t		*dbnop,
	struct xfs_buf		**fbpp,
	struct xfs_dir3_icfree_hdr *hdr,
	int			*findexp,
	int			length)
{
	struct xfs_inode	*dp = args->dp;
	struct xfs_trans	*tp = args->trans;
	struct xfs_buf		*fbp = NULL;
	xfs_dir2_db_t		firstfbno;
	xfs_dir2_db_t		lastfbno;
	xfs_dir2_db_t		ifbno = -1;
	xfs_dir2_db_t		dbno = -1;
	xfs_dir2_db_t		fbno;
	xfs_fileoff_t		fo;
	int			findex = 0;
	int			error;

	/*
	 * If we came in with a freespace block that means that lookup
	 * found an entry with our hash value.  This is the freespace
	 * block for that data entry.
	 */
	if (fblk) {
		fbp = fblk->bp;
		findex = fblk->index;
		xfs_dir2_free_hdr_from_disk(dp->i_mount, hdr, fbp->b_addr);
		if (findex >= 0) {
			/* caller already found the freespace for us. */
			ASSERT(findex < hdr->nvalid);
			ASSERT(be16_to_cpu(hdr->bests[findex]) != NULLDATAOFF);
			ASSERT(be16_to_cpu(hdr->bests[findex]) >= length);
			dbno = hdr->firstdb + findex;
			goto found_block;
		}

		/*
		 * The data block looked at didn't have enough room.
		 * We'll start at the beginning of the freespace entries.
		 */
		ifbno = fblk->blkno;
		xfs_trans_brelse(tp, fbp);
		fbp = NULL;
		fblk->bp = NULL;
	}

	/*
	 * If we don't have a data block yet, we're going to scan the freespace
	 * data for a data block with enough free space in it.
	 */
	error = xfs_bmap_last_offset(dp, &fo, XFS_DATA_FORK);
	if (error)
		return error;
	lastfbno = xfs_dir2_da_to_db(args->geo, (xfs_dablk_t)fo);
	firstfbno = xfs_dir2_byte_to_db(args->geo, XFS_DIR2_FREE_OFFSET);

	for (fbno = lastfbno - 1; fbno >= firstfbno; fbno--) {
		/* If it's ifbno we already looked at it. */
		if (fbno == ifbno)
			continue;

		/*
		 * Read the block.  There can be holes in the freespace blocks,
		 * so this might not succeed.  This should be really rare, so
		 * there's no reason to avoid it.
		 */
		error = xfs_dir2_free_try_read(tp, dp,
				xfs_dir2_db_to_da(args->geo, fbno),
				&fbp);
		if (error)
			return error;
		if (!fbp)
			continue;

		xfs_dir2_free_hdr_from_disk(dp->i_mount, hdr, fbp->b_addr);

		/* Scan the free entry array for a large enough free space. */
		for (findex = hdr->nvalid - 1; findex >= 0; findex--) {
			if (be16_to_cpu(hdr->bests[findex]) != NULLDATAOFF &&
			    be16_to_cpu(hdr->bests[findex]) >= length) {
				dbno = hdr->firstdb + findex;
				goto found_block;
			}
		}

		/* Didn't find free space, go on to next free block */
		xfs_trans_brelse(tp, fbp);
	}

found_block:
	*dbnop = dbno;
	*fbpp = fbp;
	*findexp = findex;
	return 0;
}

/*
 * Add the data entry for a node-format directory name addition.
 * The leaf entry is added in xfs_dir2_leafn_add.
 * We may enter with a freespace block that the lookup found.
 */
static int
xfs_dir2_node_addname_int(
	struct xfs_da_args	*args,		/* operation arguments */
	struct xfs_da_state_blk	*fblk)		/* optional freespace block */
{
	struct xfs_dir2_data_unused *dup;	/* data unused entry pointer */
	struct xfs_dir2_data_entry *dep;	/* data entry pointer */
	struct xfs_dir2_data_hdr *hdr;		/* data block header */
	struct xfs_dir2_data_free *bf;
	struct xfs_trans	*tp = args->trans;
	struct xfs_inode	*dp = args->dp;
	struct xfs_dir3_icfree_hdr freehdr;
	struct xfs_buf		*dbp;		/* data block buffer */
	struct xfs_buf		*fbp;		/* freespace buffer */
	xfs_dir2_data_aoff_t	aoff;
	xfs_dir2_db_t		dbno;		/* data block number */
	int			error;		/* error return value */
	int			findex;		/* freespace entry index */
	int			length;		/* length of the new entry */
	int			logfree = 0;	/* need to log free entry */
	int			needlog = 0;	/* need to log data header */
	int			needscan = 0;	/* need to rescan data frees */
	__be16			*tagp;		/* data entry tag pointer */

	length = xfs_dir2_data_entsize(dp->i_mount, args->namelen);
	error = xfs_dir2_node_find_freeblk(args, fblk, &dbno, &fbp, &freehdr,
					   &findex, length);
	if (error)
		return error;

	/*
	 * Now we know if we must allocate blocks, so if we are checking whether
	 * we can insert without allocation then we can return now.
	 */
	if (args->op_flags & XFS_DA_OP_JUSTCHECK) {
		if (dbno == -1)
			return -ENOSPC;
		return 0;
	}

	/*
	 * If we don't have a data block, we need to allocate one and make
	 * the freespace entries refer to it.
	 */
	if (dbno == -1) {
		/* we're going to have to log the free block index later */
		logfree = 1;
		error = xfs_dir2_node_add_datablk(args, fblk, &dbno, &dbp, &fbp,
						  &freehdr, &findex);
	} else {
		/* Read the data block in. */
		error = xfs_dir3_data_read(tp, dp,
					   xfs_dir2_db_to_da(args->geo, dbno),
					   0, &dbp);
	}
	if (error)
		return error;

	/* setup for data block up now */
	hdr = dbp->b_addr;
	bf = xfs_dir2_data_bestfree_p(dp->i_mount, hdr);
	ASSERT(be16_to_cpu(bf[0].length) >= length);

	/* Point to the existing unused space. */
	dup = (xfs_dir2_data_unused_t *)
	      ((char *)hdr + be16_to_cpu(bf[0].offset));

	/* Mark the first part of the unused space, inuse for us. */
	aoff = (xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr);
	error = xfs_dir2_data_use_free(args, dbp, dup, aoff, length,
			&needlog, &needscan);
	if (error) {
		xfs_trans_brelse(tp, dbp);
		return error;
	}

	/* Fill in the new entry and log it. */
	dep = (xfs_dir2_data_entry_t *)dup;
	dep->inumber = cpu_to_be64(args->inumber);
	dep->namelen = args->namelen;
	memcpy(dep->name, args->name, dep->namelen);
	xfs_dir2_data_put_ftype(dp->i_mount, dep, args->filetype);
	tagp = xfs_dir2_data_entry_tag_p(dp->i_mount, dep);
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
	xfs_dir2_data_log_entry(args, dbp, dep);

	/* Rescan the freespace and log the data block if needed. */
	if (needscan)
		xfs_dir2_data_freescan(dp->i_mount, hdr, &needlog);
	if (needlog)
		xfs_dir2_data_log_header(args, dbp);

	/* If the freespace block entry is now wrong, update it. */
	if (freehdr.bests[findex] != bf[0].length) {
		freehdr.bests[findex] = bf[0].length;
		logfree = 1;
	}

	/* Log the freespace entry if needed. */
	if (logfree)
		xfs_dir2_free_log_bests(args, &freehdr, fbp, findex, findex);

	/* Return the data block and offset in args. */
	args->blkno = (xfs_dablk_t)dbno;
	args->index = be16_to_cpu(*tagp);
	return 0;
}

/*
>>>>>>> upstream/android-13
 * Top-level node form directory addname routine.
 */
int						/* error */
xfs_dir2_node_addname(
	xfs_da_args_t		*args)		/* operation arguments */
{
	xfs_da_state_blk_t	*blk;		/* leaf block for insert */
	int			error;		/* error return value */
	int			rval;		/* sub-return value */
	xfs_da_state_t		*state;		/* btree cursor */

	trace_xfs_dir2_node_addname(args);

	/*
	 * Allocate and initialize the state (btree cursor).
	 */
<<<<<<< HEAD
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;
=======
	state = xfs_da_state_alloc(args);
>>>>>>> upstream/android-13
	/*
	 * Look up the name.  We're not supposed to find it, but
	 * this gives us the insertion point.
	 */
	error = xfs_da3_node_lookup_int(state, &rval);
	if (error)
		rval = error;
	if (rval != -ENOENT) {
		goto done;
	}
	/*
	 * Add the data entry to a data block.
	 * Extravalid is set to a freeblock found by lookup.
	 */
	rval = xfs_dir2_node_addname_int(args,
		state->extravalid ? &state->extrablk : NULL);
	if (rval) {
		goto done;
	}
	blk = &state->path.blk[state->path.active - 1];
	ASSERT(blk->magic == XFS_DIR2_LEAFN_MAGIC);
	/*
	 * Add the new leaf entry.
	 */
	rval = xfs_dir2_leafn_add(blk->bp, args, blk->index);
	if (rval == 0) {
		/*
		 * It worked, fix the hash values up the btree.
		 */
		if (!(args->op_flags & XFS_DA_OP_JUSTCHECK))
			xfs_da3_fixhashpath(state, &state->path);
	} else {
		/*
		 * It didn't work, we need to split the leaf block.
		 */
		if (args->total == 0) {
			ASSERT(rval == -ENOSPC);
			goto done;
		}
		/*
		 * Split the leaf block and insert the new entry.
		 */
		rval = xfs_da3_split(state);
	}
done:
	xfs_da_state_free(state);
	return rval;
}

/*
<<<<<<< HEAD
 * Add the data entry for a node-format directory name addition.
 * The leaf entry is added in xfs_dir2_leafn_add.
 * We may enter with a freespace block that the lookup found.
 */
static int					/* error */
xfs_dir2_node_addname_int(
	xfs_da_args_t		*args,		/* operation arguments */
	xfs_da_state_blk_t	*fblk)		/* optional freespace block */
{
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
	xfs_dir2_db_t		dbno;		/* data block number */
	struct xfs_buf		*dbp;		/* data block buffer */
	xfs_dir2_data_entry_t	*dep;		/* data entry pointer */
	xfs_inode_t		*dp;		/* incore directory inode */
	xfs_dir2_data_unused_t	*dup;		/* data unused entry pointer */
	int			error;		/* error return value */
	xfs_dir2_db_t		fbno;		/* freespace block number */
	struct xfs_buf		*fbp;		/* freespace buffer */
	int			findex;		/* freespace entry index */
	xfs_dir2_free_t		*free=NULL;	/* freespace block structure */
	xfs_dir2_db_t		ifbno;		/* initial freespace block no */
	xfs_dir2_db_t		lastfbno=0;	/* highest freespace block no */
	int			length;		/* length of the new entry */
	int			logfree;	/* need to log free entry */
	xfs_mount_t		*mp;		/* filesystem mount point */
	int			needlog;	/* need to log data header */
	int			needscan;	/* need to rescan data frees */
	__be16			*tagp;		/* data entry tag pointer */
	xfs_trans_t		*tp;		/* transaction pointer */
	__be16			*bests;
	struct xfs_dir3_icfree_hdr freehdr;
	struct xfs_dir2_data_free *bf;
	xfs_dir2_data_aoff_t	aoff;

	dp = args->dp;
	mp = dp->i_mount;
	tp = args->trans;
	length = dp->d_ops->data_entsize(args->namelen);
	/*
	 * If we came in with a freespace block that means that lookup
	 * found an entry with our hash value.  This is the freespace
	 * block for that data entry.
	 */
	if (fblk) {
		fbp = fblk->bp;
		/*
		 * Remember initial freespace block number.
		 */
		ifbno = fblk->blkno;
		free = fbp->b_addr;
		findex = fblk->index;
		bests = dp->d_ops->free_bests_p(free);
		dp->d_ops->free_hdr_from_disk(&freehdr, free);

		/*
		 * This means the free entry showed that the data block had
		 * space for our entry, so we remembered it.
		 * Use that data block.
		 */
		if (findex >= 0) {
			ASSERT(findex < freehdr.nvalid);
			ASSERT(be16_to_cpu(bests[findex]) != NULLDATAOFF);
			ASSERT(be16_to_cpu(bests[findex]) >= length);
			dbno = freehdr.firstdb + findex;
		} else {
			/*
			 * The data block looked at didn't have enough room.
			 * We'll start at the beginning of the freespace entries.
			 */
			dbno = -1;
			findex = 0;
		}
	} else {
		/*
		 * Didn't come in with a freespace block, so no data block.
		 */
		ifbno = dbno = -1;
		fbp = NULL;
		findex = 0;
	}

	/*
	 * If we don't have a data block yet, we're going to scan the
	 * freespace blocks looking for one.  Figure out what the
	 * highest freespace block number is.
	 */
	if (dbno == -1) {
		xfs_fileoff_t	fo;		/* freespace block number */

		if ((error = xfs_bmap_last_offset(dp, &fo, XFS_DATA_FORK)))
			return error;
		lastfbno = xfs_dir2_da_to_db(args->geo, (xfs_dablk_t)fo);
		fbno = ifbno;
	}
	/*
	 * While we haven't identified a data block, search the freeblock
	 * data for a good data block.  If we find a null freeblock entry,
	 * indicating a hole in the data blocks, remember that.
	 */
	while (dbno == -1) {
		/*
		 * If we don't have a freeblock in hand, get the next one.
		 */
		if (fbp == NULL) {
			/*
			 * Happens the first time through unless lookup gave
			 * us a freespace block to start with.
			 */
			if (++fbno == 0)
				fbno = xfs_dir2_byte_to_db(args->geo,
							XFS_DIR2_FREE_OFFSET);
			/*
			 * If it's ifbno we already looked at it.
			 */
			if (fbno == ifbno)
				fbno++;
			/*
			 * If it's off the end we're done.
			 */
			if (fbno >= lastfbno)
				break;
			/*
			 * Read the block.  There can be holes in the
			 * freespace blocks, so this might not succeed.
			 * This should be really rare, so there's no reason
			 * to avoid it.
			 */
			error = xfs_dir2_free_try_read(tp, dp,
					xfs_dir2_db_to_da(args->geo, fbno),
					&fbp);
			if (error)
				return error;
			if (!fbp)
				continue;
			free = fbp->b_addr;
			findex = 0;
		}
		/*
		 * Look at the current free entry.  Is it good enough?
		 *
		 * The bests initialisation should be where the bufer is read in
		 * the above branch. But gcc is too stupid to realise that bests
		 * and the freehdr are actually initialised if they are placed
		 * there, so we have to do it here to avoid warnings. Blech.
		 */
		bests = dp->d_ops->free_bests_p(free);
		dp->d_ops->free_hdr_from_disk(&freehdr, free);
		if (be16_to_cpu(bests[findex]) != NULLDATAOFF &&
		    be16_to_cpu(bests[findex]) >= length)
			dbno = freehdr.firstdb + findex;
		else {
			/*
			 * Are we done with the freeblock?
			 */
			if (++findex == freehdr.nvalid) {
				/*
				 * Drop the block.
				 */
				xfs_trans_brelse(tp, fbp);
				fbp = NULL;
				if (fblk && fblk->bp)
					fblk->bp = NULL;
			}
		}
	}
	/*
	 * If we don't have a data block, we need to allocate one and make
	 * the freespace entries refer to it.
	 */
	if (unlikely(dbno == -1)) {
		/*
		 * Not allowed to allocate, return failure.
		 */
		if ((args->op_flags & XFS_DA_OP_JUSTCHECK) || args->total == 0)
			return -ENOSPC;

		/*
		 * Allocate and initialize the new data block.
		 */
		if (unlikely((error = xfs_dir2_grow_inode(args,
							 XFS_DIR2_DATA_SPACE,
							 &dbno)) ||
		    (error = xfs_dir3_data_init(args, dbno, &dbp))))
			return error;

		/*
		 * If (somehow) we have a freespace block, get rid of it.
		 */
		if (fbp)
			xfs_trans_brelse(tp, fbp);
		if (fblk && fblk->bp)
			fblk->bp = NULL;

		/*
		 * Get the freespace block corresponding to the data block
		 * that was just allocated.
		 */
		fbno = dp->d_ops->db_to_fdb(args->geo, dbno);
		error = xfs_dir2_free_try_read(tp, dp,
				       xfs_dir2_db_to_da(args->geo, fbno),
				       &fbp);
		if (error)
			return error;

		/*
		 * If there wasn't a freespace block, the read will
		 * return a NULL fbp.  Allocate and initialize a new one.
		 */
		if (!fbp) {
			error = xfs_dir2_grow_inode(args, XFS_DIR2_FREE_SPACE,
						    &fbno);
			if (error)
				return error;

			if (dp->d_ops->db_to_fdb(args->geo, dbno) != fbno) {
				xfs_alert(mp,
"%s: dir ino %llu needed freesp block %lld for data block %lld, got %lld ifbno %llu lastfbno %d",
					__func__, (unsigned long long)dp->i_ino,
					(long long)dp->d_ops->db_to_fdb(
								args->geo, dbno),
					(long long)dbno, (long long)fbno,
					(unsigned long long)ifbno, lastfbno);
				if (fblk) {
					xfs_alert(mp,
				" fblk "PTR_FMT" blkno %llu index %d magic 0x%x",
						fblk,
						(unsigned long long)fblk->blkno,
						fblk->index,
						fblk->magic);
				} else {
					xfs_alert(mp, " ... fblk is NULL");
				}
				XFS_ERROR_REPORT("xfs_dir2_node_addname_int",
						 XFS_ERRLEVEL_LOW, mp);
				return -EFSCORRUPTED;
			}

			/*
			 * Get a buffer for the new block.
			 */
			error = xfs_dir3_free_get_buf(args, fbno, &fbp);
			if (error)
				return error;
			free = fbp->b_addr;
			bests = dp->d_ops->free_bests_p(free);
			dp->d_ops->free_hdr_from_disk(&freehdr, free);

			/*
			 * Remember the first slot as our empty slot.
			 */
			freehdr.firstdb =
				(fbno - xfs_dir2_byte_to_db(args->geo,
							XFS_DIR2_FREE_OFFSET)) *
					dp->d_ops->free_max_bests(args->geo);
		} else {
			free = fbp->b_addr;
			bests = dp->d_ops->free_bests_p(free);
			dp->d_ops->free_hdr_from_disk(&freehdr, free);
		}

		/*
		 * Set the freespace block index from the data block number.
		 */
		findex = dp->d_ops->db_to_fdindex(args->geo, dbno);
		/*
		 * If it's after the end of the current entries in the
		 * freespace block, extend that table.
		 */
		if (findex >= freehdr.nvalid) {
			ASSERT(findex < dp->d_ops->free_max_bests(args->geo));
			freehdr.nvalid = findex + 1;
			/*
			 * Tag new entry so nused will go up.
			 */
			bests[findex] = cpu_to_be16(NULLDATAOFF);
		}
		/*
		 * If this entry was for an empty data block
		 * (this should always be true) then update the header.
		 */
		if (bests[findex] == cpu_to_be16(NULLDATAOFF)) {
			freehdr.nused++;
			dp->d_ops->free_hdr_to_disk(fbp->b_addr, &freehdr);
			xfs_dir2_free_log_header(args, fbp);
		}
		/*
		 * Update the real value in the table.
		 * We haven't allocated the data entry yet so this will
		 * change again.
		 */
		hdr = dbp->b_addr;
		bf = dp->d_ops->data_bestfree_p(hdr);
		bests[findex] = bf[0].length;
		logfree = 1;
	}
	/*
	 * We had a data block so we don't have to make a new one.
	 */
	else {
		/*
		 * If just checking, we succeeded.
		 */
		if (args->op_flags & XFS_DA_OP_JUSTCHECK)
			return 0;

		/*
		 * Read the data block in.
		 */
		error = xfs_dir3_data_read(tp, dp,
					   xfs_dir2_db_to_da(args->geo, dbno),
					   -1, &dbp);
		if (error)
			return error;
		hdr = dbp->b_addr;
		bf = dp->d_ops->data_bestfree_p(hdr);
		logfree = 0;
	}
	ASSERT(be16_to_cpu(bf[0].length) >= length);
	/*
	 * Point to the existing unused space.
	 */
	dup = (xfs_dir2_data_unused_t *)
	      ((char *)hdr + be16_to_cpu(bf[0].offset));
	needscan = needlog = 0;
	/*
	 * Mark the first part of the unused space, inuse for us.
	 */
	aoff = (xfs_dir2_data_aoff_t)((char *)dup - (char *)hdr);
	error = xfs_dir2_data_use_free(args, dbp, dup, aoff, length,
			&needlog, &needscan);
	if (error) {
		xfs_trans_brelse(tp, dbp);
		return error;
	}
	/*
	 * Fill in the new entry and log it.
	 */
	dep = (xfs_dir2_data_entry_t *)dup;
	dep->inumber = cpu_to_be64(args->inumber);
	dep->namelen = args->namelen;
	memcpy(dep->name, args->name, dep->namelen);
	dp->d_ops->data_put_ftype(dep, args->filetype);
	tagp = dp->d_ops->data_entry_tag_p(dep);
	*tagp = cpu_to_be16((char *)dep - (char *)hdr);
	xfs_dir2_data_log_entry(args, dbp, dep);
	/*
	 * Rescan the block for bestfree if needed.
	 */
	if (needscan)
		xfs_dir2_data_freescan(dp, hdr, &needlog);
	/*
	 * Log the data block header if needed.
	 */
	if (needlog)
		xfs_dir2_data_log_header(args, dbp);
	/*
	 * If the freespace entry is now wrong, update it.
	 */
	bests = dp->d_ops->free_bests_p(free); /* gcc is so stupid */
	if (be16_to_cpu(bests[findex]) != be16_to_cpu(bf[0].length)) {
		bests[findex] = bf[0].length;
		logfree = 1;
	}
	/*
	 * Log the freespace entry if needed.
	 */
	if (logfree)
		xfs_dir2_free_log_bests(args, fbp, findex, findex);
	/*
	 * Return the data block and offset in args, then drop the data block.
	 */
	args->blkno = (xfs_dablk_t)dbno;
	args->index = be16_to_cpu(*tagp);
	return 0;
}

/*
=======
>>>>>>> upstream/android-13
 * Lookup an entry in a node-format directory.
 * All the real work happens in xfs_da3_node_lookup_int.
 * The only real output is the inode number of the entry.
 */
int						/* error */
xfs_dir2_node_lookup(
	xfs_da_args_t	*args)			/* operation arguments */
{
	int		error;			/* error return value */
	int		i;			/* btree level */
	int		rval;			/* operation return value */
	xfs_da_state_t	*state;			/* btree cursor */

	trace_xfs_dir2_node_lookup(args);

	/*
	 * Allocate and initialize the btree cursor.
	 */
<<<<<<< HEAD
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;
=======
	state = xfs_da_state_alloc(args);

>>>>>>> upstream/android-13
	/*
	 * Fill in the path to the entry in the cursor.
	 */
	error = xfs_da3_node_lookup_int(state, &rval);
	if (error)
		rval = error;
	else if (rval == -ENOENT && args->cmpresult == XFS_CMP_CASE) {
		/* If a CI match, dup the actual name and return -EEXIST */
		xfs_dir2_data_entry_t	*dep;

		dep = (xfs_dir2_data_entry_t *)
			((char *)state->extrablk.bp->b_addr +
						 state->extrablk.index);
		rval = xfs_dir_cilookup_result(args, dep->name, dep->namelen);
	}
	/*
	 * Release the btree blocks and leaf block.
	 */
	for (i = 0; i < state->path.active; i++) {
		xfs_trans_brelse(args->trans, state->path.blk[i].bp);
		state->path.blk[i].bp = NULL;
	}
	/*
	 * Release the data block if we have it.
	 */
	if (state->extravalid && state->extrablk.bp) {
		xfs_trans_brelse(args->trans, state->extrablk.bp);
		state->extrablk.bp = NULL;
	}
	xfs_da_state_free(state);
	return rval;
}

/*
 * Remove an entry from a node-format directory.
 */
int						/* error */
xfs_dir2_node_removename(
	struct xfs_da_args	*args)		/* operation arguments */
{
	struct xfs_da_state_blk	*blk;		/* leaf block */
	int			error;		/* error return value */
	int			rval;		/* operation return value */
	struct xfs_da_state	*state;		/* btree cursor */

	trace_xfs_dir2_node_removename(args);

	/*
	 * Allocate and initialize the btree cursor.
	 */
<<<<<<< HEAD
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;
=======
	state = xfs_da_state_alloc(args);
>>>>>>> upstream/android-13

	/* Look up the entry we're deleting, set up the cursor. */
	error = xfs_da3_node_lookup_int(state, &rval);
	if (error)
		goto out_free;

	/* Didn't find it, upper layer screwed up. */
	if (rval != -EEXIST) {
		error = rval;
		goto out_free;
	}

	blk = &state->path.blk[state->path.active - 1];
	ASSERT(blk->magic == XFS_DIR2_LEAFN_MAGIC);
	ASSERT(state->extravalid);
	/*
	 * Remove the leaf and data entries.
	 * Extrablk refers to the data block.
	 */
	error = xfs_dir2_leafn_remove(args, blk->bp, blk->index,
		&state->extrablk, &rval);
	if (error)
		goto out_free;
	/*
	 * Fix the hash values up the btree.
	 */
	xfs_da3_fixhashpath(state, &state->path);
	/*
	 * If we need to join leaf blocks, do it.
	 */
	if (rval && state->path.active > 1)
		error = xfs_da3_join(state);
	/*
	 * If no errors so far, try conversion to leaf format.
	 */
	if (!error)
		error = xfs_dir2_node_to_leaf(state);
out_free:
	xfs_da_state_free(state);
	return error;
}

/*
 * Replace an entry's inode number in a node-format directory.
 */
int						/* error */
xfs_dir2_node_replace(
	xfs_da_args_t		*args)		/* operation arguments */
{
	xfs_da_state_blk_t	*blk;		/* leaf block */
	xfs_dir2_data_hdr_t	*hdr;		/* data block header */
	xfs_dir2_data_entry_t	*dep;		/* data entry changed */
	int			error;		/* error return value */
	int			i;		/* btree level */
	xfs_ino_t		inum;		/* new inode number */
	int			ftype;		/* new file type */
<<<<<<< HEAD
	xfs_dir2_leaf_t		*leaf;		/* leaf structure */
	xfs_dir2_leaf_entry_t	*lep;		/* leaf entry being changed */
=======
>>>>>>> upstream/android-13
	int			rval;		/* internal return value */
	xfs_da_state_t		*state;		/* btree cursor */

	trace_xfs_dir2_node_replace(args);

	/*
	 * Allocate and initialize the btree cursor.
	 */
<<<<<<< HEAD
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = args->dp->i_mount;
=======
	state = xfs_da_state_alloc(args);
>>>>>>> upstream/android-13

	/*
	 * We have to save new inode number and ftype since
	 * xfs_da3_node_lookup_int() is going to overwrite them
	 */
	inum = args->inumber;
	ftype = args->filetype;

	/*
	 * Lookup the entry to change in the btree.
	 */
	error = xfs_da3_node_lookup_int(state, &rval);
	if (error) {
		rval = error;
	}
	/*
	 * It should be found, since the vnodeops layer has looked it up
	 * and locked it.  But paranoia is good.
	 */
	if (rval == -EEXIST) {
<<<<<<< HEAD
		struct xfs_dir2_leaf_entry *ents;
=======
		struct xfs_dir3_icleaf_hdr	leafhdr;

>>>>>>> upstream/android-13
		/*
		 * Find the leaf entry.
		 */
		blk = &state->path.blk[state->path.active - 1];
		ASSERT(blk->magic == XFS_DIR2_LEAFN_MAGIC);
<<<<<<< HEAD
		leaf = blk->bp->b_addr;
		ents = args->dp->d_ops->leaf_ents_p(leaf);
		lep = &ents[blk->index];
		ASSERT(state->extravalid);
=======
		ASSERT(state->extravalid);

		xfs_dir2_leaf_hdr_from_disk(state->mp, &leafhdr,
					    blk->bp->b_addr);
>>>>>>> upstream/android-13
		/*
		 * Point to the data entry.
		 */
		hdr = state->extrablk.bp->b_addr;
		ASSERT(hdr->magic == cpu_to_be32(XFS_DIR2_DATA_MAGIC) ||
		       hdr->magic == cpu_to_be32(XFS_DIR3_DATA_MAGIC));
		dep = (xfs_dir2_data_entry_t *)
		      ((char *)hdr +
		       xfs_dir2_dataptr_to_off(args->geo,
<<<<<<< HEAD
					       be32_to_cpu(lep->address)));
=======
				be32_to_cpu(leafhdr.ents[blk->index].address)));
>>>>>>> upstream/android-13
		ASSERT(inum != be64_to_cpu(dep->inumber));
		/*
		 * Fill in the new inode number and log the entry.
		 */
		dep->inumber = cpu_to_be64(inum);
<<<<<<< HEAD
		args->dp->d_ops->data_put_ftype(dep, ftype);
=======
		xfs_dir2_data_put_ftype(state->mp, dep, ftype);
>>>>>>> upstream/android-13
		xfs_dir2_data_log_entry(args, state->extrablk.bp, dep);
		rval = 0;
	}
	/*
	 * Didn't find it, and we're holding a data block.  Drop it.
	 */
	else if (state->extravalid) {
		xfs_trans_brelse(args->trans, state->extrablk.bp);
		state->extrablk.bp = NULL;
	}
	/*
	 * Release all the buffers in the cursor.
	 */
	for (i = 0; i < state->path.active; i++) {
		xfs_trans_brelse(args->trans, state->path.blk[i].bp);
		state->path.blk[i].bp = NULL;
	}
	xfs_da_state_free(state);
	return rval;
}

/*
 * Trim off a trailing empty freespace block.
 * Return (in rvalp) 1 if we did it, 0 if not.
 */
int						/* error */
xfs_dir2_node_trim_free(
	xfs_da_args_t		*args,		/* operation arguments */
	xfs_fileoff_t		fo,		/* free block number */
	int			*rvalp)		/* out: did something */
{
	struct xfs_buf		*bp;		/* freespace buffer */
	xfs_inode_t		*dp;		/* incore directory inode */
	int			error;		/* error return code */
	xfs_dir2_free_t		*free;		/* freespace structure */
	xfs_trans_t		*tp;		/* transaction pointer */
	struct xfs_dir3_icfree_hdr freehdr;

	dp = args->dp;
	tp = args->trans;

	*rvalp = 0;

	/*
	 * Read the freespace block.
	 */
	error = xfs_dir2_free_try_read(tp, dp, fo, &bp);
	if (error)
		return error;
	/*
	 * There can be holes in freespace.  If fo is a hole, there's
	 * nothing to do.
	 */
	if (!bp)
		return 0;
	free = bp->b_addr;
<<<<<<< HEAD
	dp->d_ops->free_hdr_from_disk(&freehdr, free);
=======
	xfs_dir2_free_hdr_from_disk(dp->i_mount, &freehdr, free);
>>>>>>> upstream/android-13

	/*
	 * If there are used entries, there's nothing to do.
	 */
	if (freehdr.nused > 0) {
		xfs_trans_brelse(tp, bp);
		return 0;
	}
	/*
	 * Blow the block away.
	 */
	error = xfs_dir2_shrink_inode(args,
			xfs_dir2_da_to_db(args->geo, (xfs_dablk_t)fo), bp);
	if (error) {
		/*
		 * Can't fail with ENOSPC since that only happens with no
		 * space reservation, when breaking up an extent into two
		 * pieces.  This is the last block of an extent.
		 */
		ASSERT(error != -ENOSPC);
		xfs_trans_brelse(tp, bp);
		return error;
	}
	/*
	 * Return that we succeeded.
	 */
	*rvalp = 1;
	return 0;
}
