// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2016 Oracle.  All Rights Reserved.
 * Author: Darrick J. Wong <darrick.wong@oracle.com>
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
<<<<<<< HEAD
#include "xfs_sb.h"
#include "xfs_mount.h"
#include "xfs_btree.h"
#include "xfs_bmap.h"
=======
#include "xfs_mount.h"
#include "xfs_btree.h"
#include "xfs_btree_staging.h"
>>>>>>> upstream/android-13
#include "xfs_refcount_btree.h"
#include "xfs_alloc.h"
#include "xfs_error.h"
#include "xfs_trace.h"
<<<<<<< HEAD
#include "xfs_cksum.h"
#include "xfs_trans.h"
#include "xfs_bit.h"
#include "xfs_rmap.h"
=======
#include "xfs_trans.h"
#include "xfs_bit.h"
#include "xfs_rmap.h"
#include "xfs_ag.h"
>>>>>>> upstream/android-13

static struct xfs_btree_cur *
xfs_refcountbt_dup_cursor(
	struct xfs_btree_cur	*cur)
{
	return xfs_refcountbt_init_cursor(cur->bc_mp, cur->bc_tp,
<<<<<<< HEAD
			cur->bc_private.a.agbp, cur->bc_private.a.agno);
=======
			cur->bc_ag.agbp, cur->bc_ag.pag);
>>>>>>> upstream/android-13
}

STATIC void
xfs_refcountbt_set_root(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_ptr	*ptr,
	int			inc)
{
	struct xfs_buf		*agbp = cur->bc_private.a.agbp;
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(agbp);
	xfs_agnumber_t		seqno = be32_to_cpu(agf->agf_seqno);
	struct xfs_perag	*pag = xfs_perag_get(cur->bc_mp, seqno);
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_ptr	*ptr,
	int				inc)
{
	struct xfs_buf		*agbp = cur->bc_ag.agbp;
	struct xfs_agf		*agf = agbp->b_addr;
	struct xfs_perag	*pag = agbp->b_pag;
>>>>>>> upstream/android-13

	ASSERT(ptr->s != 0);

	agf->agf_refcount_root = ptr->s;
	be32_add_cpu(&agf->agf_refcount_level, inc);
	pag->pagf_refcount_level += inc;
<<<<<<< HEAD
	xfs_perag_put(pag);
=======
>>>>>>> upstream/android-13

	xfs_alloc_log_agf(cur->bc_tp, agbp,
			XFS_AGF_REFCOUNT_ROOT | XFS_AGF_REFCOUNT_LEVEL);
}

STATIC int
xfs_refcountbt_alloc_block(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_ptr	*start,
	union xfs_btree_ptr	*new,
	int			*stat)
{
	struct xfs_buf		*agbp = cur->bc_private.a.agbp;
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(agbp);
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_ptr	*start,
	union xfs_btree_ptr		*new,
	int				*stat)
{
	struct xfs_buf		*agbp = cur->bc_ag.agbp;
	struct xfs_agf		*agf = agbp->b_addr;
>>>>>>> upstream/android-13
	struct xfs_alloc_arg	args;		/* block allocation args */
	int			error;		/* error return value */

	memset(&args, 0, sizeof(args));
	args.tp = cur->bc_tp;
	args.mp = cur->bc_mp;
	args.type = XFS_ALLOCTYPE_NEAR_BNO;
<<<<<<< HEAD
	args.fsbno = XFS_AGB_TO_FSB(cur->bc_mp, cur->bc_private.a.agno,
			xfs_refc_block(args.mp));
	xfs_rmap_ag_owner(&args.oinfo, XFS_RMAP_OWN_REFC);
=======
	args.fsbno = XFS_AGB_TO_FSB(cur->bc_mp, cur->bc_ag.pag->pag_agno,
			xfs_refc_block(args.mp));
	args.oinfo = XFS_RMAP_OINFO_REFC;
>>>>>>> upstream/android-13
	args.minlen = args.maxlen = args.prod = 1;
	args.resv = XFS_AG_RESV_METADATA;

	error = xfs_alloc_vextent(&args);
	if (error)
		goto out_error;
<<<<<<< HEAD
	trace_xfs_refcountbt_alloc_block(cur->bc_mp, cur->bc_private.a.agno,
=======
	trace_xfs_refcountbt_alloc_block(cur->bc_mp, cur->bc_ag.pag->pag_agno,
>>>>>>> upstream/android-13
			args.agbno, 1);
	if (args.fsbno == NULLFSBLOCK) {
		*stat = 0;
		return 0;
	}
<<<<<<< HEAD
	ASSERT(args.agno == cur->bc_private.a.agno);
=======
	ASSERT(args.agno == cur->bc_ag.pag->pag_agno);
>>>>>>> upstream/android-13
	ASSERT(args.len == 1);

	new->s = cpu_to_be32(args.agbno);
	be32_add_cpu(&agf->agf_refcount_blocks, 1);
	xfs_alloc_log_agf(cur->bc_tp, agbp, XFS_AGF_REFCOUNT_BLOCKS);

	*stat = 1;
	return 0;

out_error:
	return error;
}

STATIC int
xfs_refcountbt_free_block(
	struct xfs_btree_cur	*cur,
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = cur->bc_mp;
<<<<<<< HEAD
	struct xfs_buf		*agbp = cur->bc_private.a.agbp;
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(agbp);
	xfs_fsblock_t		fsbno = XFS_DADDR_TO_FSB(mp, XFS_BUF_ADDR(bp));
	struct xfs_owner_info	oinfo;
	int			error;

	trace_xfs_refcountbt_free_block(cur->bc_mp, cur->bc_private.a.agno,
			XFS_FSB_TO_AGBNO(cur->bc_mp, fsbno), 1);
	xfs_rmap_ag_owner(&oinfo, XFS_RMAP_OWN_REFC);
	be32_add_cpu(&agf->agf_refcount_blocks, -1);
	xfs_alloc_log_agf(cur->bc_tp, agbp, XFS_AGF_REFCOUNT_BLOCKS);
	error = xfs_free_extent(cur->bc_tp, fsbno, 1, &oinfo,
=======
	struct xfs_buf		*agbp = cur->bc_ag.agbp;
	struct xfs_agf		*agf = agbp->b_addr;
	xfs_fsblock_t		fsbno = XFS_DADDR_TO_FSB(mp, xfs_buf_daddr(bp));
	int			error;

	trace_xfs_refcountbt_free_block(cur->bc_mp, cur->bc_ag.pag->pag_agno,
			XFS_FSB_TO_AGBNO(cur->bc_mp, fsbno), 1);
	be32_add_cpu(&agf->agf_refcount_blocks, -1);
	xfs_alloc_log_agf(cur->bc_tp, agbp, XFS_AGF_REFCOUNT_BLOCKS);
	error = xfs_free_extent(cur->bc_tp, fsbno, 1, &XFS_RMAP_OINFO_REFC,
>>>>>>> upstream/android-13
			XFS_AG_RESV_METADATA);
	if (error)
		return error;

	return error;
}

STATIC int
xfs_refcountbt_get_minrecs(
	struct xfs_btree_cur	*cur,
	int			level)
{
	return cur->bc_mp->m_refc_mnr[level != 0];
}

STATIC int
xfs_refcountbt_get_maxrecs(
	struct xfs_btree_cur	*cur,
	int			level)
{
	return cur->bc_mp->m_refc_mxr[level != 0];
}

STATIC void
xfs_refcountbt_init_key_from_rec(
<<<<<<< HEAD
	union xfs_btree_key	*key,
	union xfs_btree_rec	*rec)
=======
	union xfs_btree_key		*key,
	const union xfs_btree_rec	*rec)
>>>>>>> upstream/android-13
{
	key->refc.rc_startblock = rec->refc.rc_startblock;
}

STATIC void
xfs_refcountbt_init_high_key_from_rec(
<<<<<<< HEAD
	union xfs_btree_key	*key,
	union xfs_btree_rec	*rec)
{
	__u32			x;
=======
	union xfs_btree_key		*key,
	const union xfs_btree_rec	*rec)
{
	__u32				x;
>>>>>>> upstream/android-13

	x = be32_to_cpu(rec->refc.rc_startblock);
	x += be32_to_cpu(rec->refc.rc_blockcount) - 1;
	key->refc.rc_startblock = cpu_to_be32(x);
}

STATIC void
xfs_refcountbt_init_rec_from_cur(
	struct xfs_btree_cur	*cur,
	union xfs_btree_rec	*rec)
{
	rec->refc.rc_startblock = cpu_to_be32(cur->bc_rec.rc.rc_startblock);
	rec->refc.rc_blockcount = cpu_to_be32(cur->bc_rec.rc.rc_blockcount);
	rec->refc.rc_refcount = cpu_to_be32(cur->bc_rec.rc.rc_refcount);
}

STATIC void
xfs_refcountbt_init_ptr_from_cur(
	struct xfs_btree_cur	*cur,
	union xfs_btree_ptr	*ptr)
{
<<<<<<< HEAD
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(cur->bc_private.a.agbp);

	ASSERT(cur->bc_private.a.agno == be32_to_cpu(agf->agf_seqno));
=======
	struct xfs_agf		*agf = cur->bc_ag.agbp->b_addr;

	ASSERT(cur->bc_ag.pag->pag_agno == be32_to_cpu(agf->agf_seqno));
>>>>>>> upstream/android-13

	ptr->s = agf->agf_refcount_root;
}

STATIC int64_t
xfs_refcountbt_key_diff(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_key	*key)
{
	struct xfs_refcount_irec	*rec = &cur->bc_rec.rc;
	struct xfs_refcount_key		*kp = &key->refc;
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_key	*key)
{
	struct xfs_refcount_irec	*rec = &cur->bc_rec.rc;
	const struct xfs_refcount_key	*kp = &key->refc;
>>>>>>> upstream/android-13

	return (int64_t)be32_to_cpu(kp->rc_startblock) - rec->rc_startblock;
}

STATIC int64_t
xfs_refcountbt_diff_two_keys(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_key	*k1,
	union xfs_btree_key	*k2)
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_key	*k1,
	const union xfs_btree_key	*k2)
>>>>>>> upstream/android-13
{
	return (int64_t)be32_to_cpu(k1->refc.rc_startblock) -
			  be32_to_cpu(k2->refc.rc_startblock);
}

STATIC xfs_failaddr_t
xfs_refcountbt_verify(
	struct xfs_buf		*bp)
{
<<<<<<< HEAD
	struct xfs_mount	*mp = bp->b_target->bt_mount;
=======
	struct xfs_mount	*mp = bp->b_mount;
>>>>>>> upstream/android-13
	struct xfs_btree_block	*block = XFS_BUF_TO_BLOCK(bp);
	struct xfs_perag	*pag = bp->b_pag;
	xfs_failaddr_t		fa;
	unsigned int		level;

<<<<<<< HEAD
	if (block->bb_magic != cpu_to_be32(XFS_REFC_CRC_MAGIC))
		return __this_address;

	if (!xfs_sb_version_hasreflink(&mp->m_sb))
=======
	if (!xfs_verify_magic(bp, block->bb_magic))
		return __this_address;

	if (!xfs_has_reflink(mp))
>>>>>>> upstream/android-13
		return __this_address;
	fa = xfs_btree_sblock_v5hdr_verify(bp);
	if (fa)
		return fa;

	level = be16_to_cpu(block->bb_level);
	if (pag && pag->pagf_init) {
		if (level >= pag->pagf_refcount_level)
			return __this_address;
	} else if (level >= mp->m_refc_maxlevels)
		return __this_address;

	return xfs_btree_sblock_verify(bp, mp->m_refc_mxr[level != 0]);
}

STATIC void
xfs_refcountbt_read_verify(
	struct xfs_buf	*bp)
{
	xfs_failaddr_t	fa;

	if (!xfs_btree_sblock_verify_crc(bp))
		xfs_verifier_error(bp, -EFSBADCRC, __this_address);
	else {
		fa = xfs_refcountbt_verify(bp);
		if (fa)
			xfs_verifier_error(bp, -EFSCORRUPTED, fa);
	}

	if (bp->b_error)
		trace_xfs_btree_corrupt(bp, _RET_IP_);
}

STATIC void
xfs_refcountbt_write_verify(
	struct xfs_buf	*bp)
{
	xfs_failaddr_t	fa;

	fa = xfs_refcountbt_verify(bp);
	if (fa) {
		trace_xfs_btree_corrupt(bp, _RET_IP_);
		xfs_verifier_error(bp, -EFSCORRUPTED, fa);
		return;
	}
	xfs_btree_sblock_calc_crc(bp);

}

const struct xfs_buf_ops xfs_refcountbt_buf_ops = {
	.name			= "xfs_refcountbt",
<<<<<<< HEAD
=======
	.magic			= { 0, cpu_to_be32(XFS_REFC_CRC_MAGIC) },
>>>>>>> upstream/android-13
	.verify_read		= xfs_refcountbt_read_verify,
	.verify_write		= xfs_refcountbt_write_verify,
	.verify_struct		= xfs_refcountbt_verify,
};

STATIC int
xfs_refcountbt_keys_inorder(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_key	*k1,
	union xfs_btree_key	*k2)
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_key	*k1,
	const union xfs_btree_key	*k2)
>>>>>>> upstream/android-13
{
	return be32_to_cpu(k1->refc.rc_startblock) <
	       be32_to_cpu(k2->refc.rc_startblock);
}

STATIC int
xfs_refcountbt_recs_inorder(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_rec	*r1,
	union xfs_btree_rec	*r2)
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_rec	*r1,
	const union xfs_btree_rec	*r2)
>>>>>>> upstream/android-13
{
	return  be32_to_cpu(r1->refc.rc_startblock) +
		be32_to_cpu(r1->refc.rc_blockcount) <=
		be32_to_cpu(r2->refc.rc_startblock);
}

static const struct xfs_btree_ops xfs_refcountbt_ops = {
	.rec_len		= sizeof(struct xfs_refcount_rec),
	.key_len		= sizeof(struct xfs_refcount_key),

	.dup_cursor		= xfs_refcountbt_dup_cursor,
	.set_root		= xfs_refcountbt_set_root,
	.alloc_block		= xfs_refcountbt_alloc_block,
	.free_block		= xfs_refcountbt_free_block,
	.get_minrecs		= xfs_refcountbt_get_minrecs,
	.get_maxrecs		= xfs_refcountbt_get_maxrecs,
	.init_key_from_rec	= xfs_refcountbt_init_key_from_rec,
	.init_high_key_from_rec	= xfs_refcountbt_init_high_key_from_rec,
	.init_rec_from_cur	= xfs_refcountbt_init_rec_from_cur,
	.init_ptr_from_cur	= xfs_refcountbt_init_ptr_from_cur,
	.key_diff		= xfs_refcountbt_key_diff,
	.buf_ops		= &xfs_refcountbt_buf_ops,
	.diff_two_keys		= xfs_refcountbt_diff_two_keys,
	.keys_inorder		= xfs_refcountbt_keys_inorder,
	.recs_inorder		= xfs_refcountbt_recs_inorder,
};

/*
<<<<<<< HEAD
 * Allocate a new refcount btree cursor.
 */
=======
 * Initialize a new refcount btree cursor.
 */
static struct xfs_btree_cur *
xfs_refcountbt_init_common(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	struct xfs_perag	*pag)
{
	struct xfs_btree_cur	*cur;

	ASSERT(pag->pag_agno < mp->m_sb.sb_agcount);

	cur = kmem_cache_zalloc(xfs_btree_cur_zone, GFP_NOFS | __GFP_NOFAIL);
	cur->bc_tp = tp;
	cur->bc_mp = mp;
	cur->bc_btnum = XFS_BTNUM_REFC;
	cur->bc_blocklog = mp->m_sb.sb_blocklog;
	cur->bc_statoff = XFS_STATS_CALC_INDEX(xs_refcbt_2);

	cur->bc_flags |= XFS_BTREE_CRC_BLOCKS;

	/* take a reference for the cursor */
	atomic_inc(&pag->pag_ref);
	cur->bc_ag.pag = pag;

	cur->bc_ag.refc.nr_ops = 0;
	cur->bc_ag.refc.shape_changes = 0;
	cur->bc_ops = &xfs_refcountbt_ops;
	return cur;
}

/* Create a btree cursor. */
>>>>>>> upstream/android-13
struct xfs_btree_cur *
xfs_refcountbt_init_cursor(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	struct xfs_buf		*agbp,
<<<<<<< HEAD
	xfs_agnumber_t		agno)
{
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(agbp);
	struct xfs_btree_cur	*cur;

	ASSERT(agno != NULLAGNUMBER);
	ASSERT(agno < mp->m_sb.sb_agcount);
	cur = kmem_zone_zalloc(xfs_btree_cur_zone, KM_NOFS);

	cur->bc_tp = tp;
	cur->bc_mp = mp;
	cur->bc_btnum = XFS_BTNUM_REFC;
	cur->bc_blocklog = mp->m_sb.sb_blocklog;
	cur->bc_ops = &xfs_refcountbt_ops;
	cur->bc_statoff = XFS_STATS_CALC_INDEX(xs_refcbt_2);

	cur->bc_nlevels = be32_to_cpu(agf->agf_refcount_level);

	cur->bc_private.a.agbp = agbp;
	cur->bc_private.a.agno = agno;
	cur->bc_flags |= XFS_BTREE_CRC_BLOCKS;

	cur->bc_private.a.priv.refc.nr_ops = 0;
	cur->bc_private.a.priv.refc.shape_changes = 0;

	return cur;
}

=======
	struct xfs_perag	*pag)
{
	struct xfs_agf		*agf = agbp->b_addr;
	struct xfs_btree_cur	*cur;

	cur = xfs_refcountbt_init_common(mp, tp, pag);
	cur->bc_nlevels = be32_to_cpu(agf->agf_refcount_level);
	cur->bc_ag.agbp = agbp;
	return cur;
}

/* Create a btree cursor with a fake root for staging. */
struct xfs_btree_cur *
xfs_refcountbt_stage_cursor(
	struct xfs_mount	*mp,
	struct xbtree_afakeroot	*afake,
	struct xfs_perag	*pag)
{
	struct xfs_btree_cur	*cur;

	cur = xfs_refcountbt_init_common(mp, NULL, pag);
	xfs_btree_stage_afakeroot(cur, afake);
	return cur;
}

/*
 * Swap in the new btree root.  Once we pass this point the newly rebuilt btree
 * is in place and we have to kill off all the old btree blocks.
 */
void
xfs_refcountbt_commit_staged_btree(
	struct xfs_btree_cur	*cur,
	struct xfs_trans	*tp,
	struct xfs_buf		*agbp)
{
	struct xfs_agf		*agf = agbp->b_addr;
	struct xbtree_afakeroot	*afake = cur->bc_ag.afake;

	ASSERT(cur->bc_flags & XFS_BTREE_STAGING);

	agf->agf_refcount_root = cpu_to_be32(afake->af_root);
	agf->agf_refcount_level = cpu_to_be32(afake->af_levels);
	agf->agf_refcount_blocks = cpu_to_be32(afake->af_blocks);
	xfs_alloc_log_agf(tp, agbp, XFS_AGF_REFCOUNT_BLOCKS |
				    XFS_AGF_REFCOUNT_ROOT |
				    XFS_AGF_REFCOUNT_LEVEL);
	xfs_btree_commit_afakeroot(cur, tp, agbp, &xfs_refcountbt_ops);
}

>>>>>>> upstream/android-13
/*
 * Calculate the number of records in a refcount btree block.
 */
int
xfs_refcountbt_maxrecs(
	int			blocklen,
	bool			leaf)
{
	blocklen -= XFS_REFCOUNT_BLOCK_LEN;

	if (leaf)
		return blocklen / sizeof(struct xfs_refcount_rec);
	return blocklen / (sizeof(struct xfs_refcount_key) +
			   sizeof(xfs_refcount_ptr_t));
}

/* Compute the maximum height of a refcount btree. */
void
xfs_refcountbt_compute_maxlevels(
	struct xfs_mount		*mp)
{
	mp->m_refc_maxlevels = xfs_btree_compute_maxlevels(
			mp->m_refc_mnr, mp->m_sb.sb_agblocks);
}

/* Calculate the refcount btree size for some records. */
xfs_extlen_t
xfs_refcountbt_calc_size(
	struct xfs_mount	*mp,
	unsigned long long	len)
{
	return xfs_btree_calc_size(mp->m_refc_mnr, len);
}

/*
 * Calculate the maximum refcount btree size.
 */
xfs_extlen_t
xfs_refcountbt_max_size(
	struct xfs_mount	*mp,
	xfs_agblock_t		agblocks)
{
	/* Bail out if we're uninitialized, which can happen in mkfs. */
	if (mp->m_refc_mxr[0] == 0)
		return 0;

	return xfs_refcountbt_calc_size(mp, agblocks);
}

/*
 * Figure out how many blocks to reserve and how many are used by this btree.
 */
int
xfs_refcountbt_calc_reserves(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
<<<<<<< HEAD
	xfs_agnumber_t		agno,
=======
	struct xfs_perag	*pag,
>>>>>>> upstream/android-13
	xfs_extlen_t		*ask,
	xfs_extlen_t		*used)
{
	struct xfs_buf		*agbp;
	struct xfs_agf		*agf;
	xfs_agblock_t		agblocks;
	xfs_extlen_t		tree_len;
	int			error;

<<<<<<< HEAD
	if (!xfs_sb_version_hasreflink(&mp->m_sb))
		return 0;


	error = xfs_alloc_read_agf(mp, tp, agno, 0, &agbp);
	if (error)
		return error;

	agf = XFS_BUF_TO_AGF(agbp);
=======
	if (!xfs_has_reflink(mp))
		return 0;

	error = xfs_alloc_read_agf(mp, tp, pag->pag_agno, 0, &agbp);
	if (error)
		return error;

	agf = agbp->b_addr;
>>>>>>> upstream/android-13
	agblocks = be32_to_cpu(agf->agf_length);
	tree_len = be32_to_cpu(agf->agf_refcount_blocks);
	xfs_trans_brelse(tp, agbp);

<<<<<<< HEAD
=======
	/*
	 * The log is permanently allocated, so the space it occupies will
	 * never be available for the kinds of things that would require btree
	 * expansion.  We therefore can pretend the space isn't there.
	 */
	if (mp->m_sb.sb_logstart &&
	    XFS_FSB_TO_AGNO(mp, mp->m_sb.sb_logstart) == pag->pag_agno)
		agblocks -= mp->m_sb.sb_logblocks;

>>>>>>> upstream/android-13
	*ask += xfs_refcountbt_max_size(mp, agblocks);
	*used += tree_len;

	return error;
}
