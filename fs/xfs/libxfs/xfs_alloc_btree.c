// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2001,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
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
=======
#include "xfs_mount.h"
#include "xfs_btree.h"
#include "xfs_btree_staging.h"
>>>>>>> upstream/android-13
#include "xfs_alloc_btree.h"
#include "xfs_alloc.h"
#include "xfs_extent_busy.h"
#include "xfs_error.h"
#include "xfs_trace.h"
<<<<<<< HEAD
#include "xfs_cksum.h"
#include "xfs_trans.h"
=======
#include "xfs_trans.h"
#include "xfs_ag.h"
>>>>>>> upstream/android-13


STATIC struct xfs_btree_cur *
xfs_allocbt_dup_cursor(
	struct xfs_btree_cur	*cur)
{
	return xfs_allocbt_init_cursor(cur->bc_mp, cur->bc_tp,
<<<<<<< HEAD
			cur->bc_private.a.agbp, cur->bc_private.a.agno,
			cur->bc_btnum);
=======
			cur->bc_ag.agbp, cur->bc_ag.pag, cur->bc_btnum);
>>>>>>> upstream/android-13
}

STATIC void
xfs_allocbt_set_root(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_ptr	*ptr,
	int			inc)
{
	struct xfs_buf		*agbp = cur->bc_private.a.agbp;
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(agbp);
	xfs_agnumber_t		seqno = be32_to_cpu(agf->agf_seqno);
	int			btnum = cur->bc_btnum;
	struct xfs_perag	*pag = xfs_perag_get(cur->bc_mp, seqno);
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_ptr	*ptr,
	int				inc)
{
	struct xfs_buf		*agbp = cur->bc_ag.agbp;
	struct xfs_agf		*agf = agbp->b_addr;
	int			btnum = cur->bc_btnum;
>>>>>>> upstream/android-13

	ASSERT(ptr->s != 0);

	agf->agf_roots[btnum] = ptr->s;
	be32_add_cpu(&agf->agf_levels[btnum], inc);
<<<<<<< HEAD
	pag->pagf_levels[btnum] += inc;
	xfs_perag_put(pag);
=======
	cur->bc_ag.pag->pagf_levels[btnum] += inc;
>>>>>>> upstream/android-13

	xfs_alloc_log_agf(cur->bc_tp, agbp, XFS_AGF_ROOTS | XFS_AGF_LEVELS);
}

STATIC int
xfs_allocbt_alloc_block(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_ptr	*start,
	union xfs_btree_ptr	*new,
	int			*stat)
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_ptr	*start,
	union xfs_btree_ptr		*new,
	int				*stat)
>>>>>>> upstream/android-13
{
	int			error;
	xfs_agblock_t		bno;

	/* Allocate the new block from the freelist. If we can't, give up.  */
<<<<<<< HEAD
	error = xfs_alloc_get_freelist(cur->bc_tp, cur->bc_private.a.agbp,
=======
	error = xfs_alloc_get_freelist(cur->bc_tp, cur->bc_ag.agbp,
>>>>>>> upstream/android-13
				       &bno, 1);
	if (error)
		return error;

	if (bno == NULLAGBLOCK) {
		*stat = 0;
		return 0;
	}

<<<<<<< HEAD
	xfs_extent_busy_reuse(cur->bc_mp, cur->bc_private.a.agno, bno, 1, false);

	xfs_trans_agbtree_delta(cur->bc_tp, 1);
=======
	atomic64_inc(&cur->bc_mp->m_allocbt_blks);
	xfs_extent_busy_reuse(cur->bc_mp, cur->bc_ag.agbp->b_pag, bno, 1, false);

>>>>>>> upstream/android-13
	new->s = cpu_to_be32(bno);

	*stat = 1;
	return 0;
}

STATIC int
xfs_allocbt_free_block(
	struct xfs_btree_cur	*cur,
	struct xfs_buf		*bp)
{
<<<<<<< HEAD
	struct xfs_buf		*agbp = cur->bc_private.a.agbp;
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(agbp);
	xfs_agblock_t		bno;
	int			error;

	bno = xfs_daddr_to_agbno(cur->bc_mp, XFS_BUF_ADDR(bp));
=======
	struct xfs_buf		*agbp = cur->bc_ag.agbp;
	xfs_agblock_t		bno;
	int			error;

	bno = xfs_daddr_to_agbno(cur->bc_mp, xfs_buf_daddr(bp));
>>>>>>> upstream/android-13
	error = xfs_alloc_put_freelist(cur->bc_tp, agbp, NULL, bno, 1);
	if (error)
		return error;

<<<<<<< HEAD
	xfs_extent_busy_insert(cur->bc_tp, be32_to_cpu(agf->agf_seqno), bno, 1,
			      XFS_EXTENT_BUSY_SKIP_DISCARD);
	xfs_trans_agbtree_delta(cur->bc_tp, -1);
=======
	atomic64_dec(&cur->bc_mp->m_allocbt_blks);
	xfs_extent_busy_insert(cur->bc_tp, agbp->b_pag, bno, 1,
			      XFS_EXTENT_BUSY_SKIP_DISCARD);
>>>>>>> upstream/android-13
	return 0;
}

/*
 * Update the longest extent in the AGF
 */
STATIC void
xfs_allocbt_update_lastrec(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	struct xfs_btree_block	*block,
	union xfs_btree_rec	*rec,
	int			ptr,
	int			reason)
{
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(cur->bc_private.a.agbp);
	xfs_agnumber_t		seqno = be32_to_cpu(agf->agf_seqno);
=======
	struct xfs_btree_cur		*cur,
	const struct xfs_btree_block	*block,
	const union xfs_btree_rec	*rec,
	int				ptr,
	int				reason)
{
	struct xfs_agf		*agf = cur->bc_ag.agbp->b_addr;
>>>>>>> upstream/android-13
	struct xfs_perag	*pag;
	__be32			len;
	int			numrecs;

	ASSERT(cur->bc_btnum == XFS_BTNUM_CNT);

	switch (reason) {
	case LASTREC_UPDATE:
		/*
		 * If this is the last leaf block and it's the last record,
		 * then update the size of the longest extent in the AG.
		 */
		if (ptr != xfs_btree_get_numrecs(block))
			return;
		len = rec->alloc.ar_blockcount;
		break;
	case LASTREC_INSREC:
		if (be32_to_cpu(rec->alloc.ar_blockcount) <=
		    be32_to_cpu(agf->agf_longest))
			return;
		len = rec->alloc.ar_blockcount;
		break;
	case LASTREC_DELREC:
		numrecs = xfs_btree_get_numrecs(block);
		if (ptr <= numrecs)
			return;
		ASSERT(ptr == numrecs + 1);

		if (numrecs) {
			xfs_alloc_rec_t *rrp;

			rrp = XFS_ALLOC_REC_ADDR(cur->bc_mp, block, numrecs);
			len = rrp->ar_blockcount;
		} else {
			len = 0;
		}

		break;
	default:
		ASSERT(0);
		return;
	}

	agf->agf_longest = len;
<<<<<<< HEAD
	pag = xfs_perag_get(cur->bc_mp, seqno);
	pag->pagf_longest = be32_to_cpu(len);
	xfs_perag_put(pag);
	xfs_alloc_log_agf(cur->bc_tp, cur->bc_private.a.agbp, XFS_AGF_LONGEST);
=======
	pag = cur->bc_ag.agbp->b_pag;
	pag->pagf_longest = be32_to_cpu(len);
	xfs_alloc_log_agf(cur->bc_tp, cur->bc_ag.agbp, XFS_AGF_LONGEST);
>>>>>>> upstream/android-13
}

STATIC int
xfs_allocbt_get_minrecs(
	struct xfs_btree_cur	*cur,
	int			level)
{
	return cur->bc_mp->m_alloc_mnr[level != 0];
}

STATIC int
xfs_allocbt_get_maxrecs(
	struct xfs_btree_cur	*cur,
	int			level)
{
	return cur->bc_mp->m_alloc_mxr[level != 0];
}

STATIC void
xfs_allocbt_init_key_from_rec(
<<<<<<< HEAD
	union xfs_btree_key	*key,
	union xfs_btree_rec	*rec)
=======
	union xfs_btree_key		*key,
	const union xfs_btree_rec	*rec)
>>>>>>> upstream/android-13
{
	key->alloc.ar_startblock = rec->alloc.ar_startblock;
	key->alloc.ar_blockcount = rec->alloc.ar_blockcount;
}

STATIC void
xfs_bnobt_init_high_key_from_rec(
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

	x = be32_to_cpu(rec->alloc.ar_startblock);
	x += be32_to_cpu(rec->alloc.ar_blockcount) - 1;
	key->alloc.ar_startblock = cpu_to_be32(x);
	key->alloc.ar_blockcount = 0;
}

STATIC void
xfs_cntbt_init_high_key_from_rec(
<<<<<<< HEAD
	union xfs_btree_key	*key,
	union xfs_btree_rec	*rec)
=======
	union xfs_btree_key		*key,
	const union xfs_btree_rec	*rec)
>>>>>>> upstream/android-13
{
	key->alloc.ar_blockcount = rec->alloc.ar_blockcount;
	key->alloc.ar_startblock = 0;
}

STATIC void
xfs_allocbt_init_rec_from_cur(
	struct xfs_btree_cur	*cur,
	union xfs_btree_rec	*rec)
{
	rec->alloc.ar_startblock = cpu_to_be32(cur->bc_rec.a.ar_startblock);
	rec->alloc.ar_blockcount = cpu_to_be32(cur->bc_rec.a.ar_blockcount);
}

STATIC void
xfs_allocbt_init_ptr_from_cur(
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

	ptr->s = agf->agf_roots[cur->bc_btnum];
}

STATIC int64_t
xfs_bnobt_key_diff(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_key	*key)
{
	xfs_alloc_rec_incore_t	*rec = &cur->bc_rec.a;
	xfs_alloc_key_t		*kp = &key->alloc;
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_key	*key)
{
	struct xfs_alloc_rec_incore	*rec = &cur->bc_rec.a;
	const struct xfs_alloc_rec	*kp = &key->alloc;
>>>>>>> upstream/android-13

	return (int64_t)be32_to_cpu(kp->ar_startblock) - rec->ar_startblock;
}

STATIC int64_t
xfs_cntbt_key_diff(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_key	*key)
{
	xfs_alloc_rec_incore_t	*rec = &cur->bc_rec.a;
	xfs_alloc_key_t		*kp = &key->alloc;
	int64_t			diff;
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_key	*key)
{
	struct xfs_alloc_rec_incore	*rec = &cur->bc_rec.a;
	const struct xfs_alloc_rec	*kp = &key->alloc;
	int64_t				diff;
>>>>>>> upstream/android-13

	diff = (int64_t)be32_to_cpu(kp->ar_blockcount) - rec->ar_blockcount;
	if (diff)
		return diff;

	return (int64_t)be32_to_cpu(kp->ar_startblock) - rec->ar_startblock;
}

STATIC int64_t
xfs_bnobt_diff_two_keys(
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
	return (int64_t)be32_to_cpu(k1->alloc.ar_startblock) -
			  be32_to_cpu(k2->alloc.ar_startblock);
}

STATIC int64_t
xfs_cntbt_diff_two_keys(
<<<<<<< HEAD
	struct xfs_btree_cur	*cur,
	union xfs_btree_key	*k1,
	union xfs_btree_key	*k2)
{
	int64_t			diff;
=======
	struct xfs_btree_cur		*cur,
	const union xfs_btree_key	*k1,
	const union xfs_btree_key	*k2)
{
	int64_t				diff;
>>>>>>> upstream/android-13

	diff =  be32_to_cpu(k1->alloc.ar_blockcount) -
		be32_to_cpu(k2->alloc.ar_blockcount);
	if (diff)
		return diff;

	return  be32_to_cpu(k1->alloc.ar_startblock) -
		be32_to_cpu(k2->alloc.ar_startblock);
}

static xfs_failaddr_t
xfs_allocbt_verify(
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

	/*
	 * magic number and level verification
	 *
	 * During growfs operations, we can't verify the exact level or owner as
	 * the perag is not fully initialised and hence not attached to the
	 * buffer.  In this case, check against the maximum tree depth.
	 *
	 * Similarly, during log recovery we will have a perag structure
	 * attached, but the agf information will not yet have been initialised
	 * from the on disk AGF. Again, we can only check against maximum limits
	 * in this case.
	 */
	level = be16_to_cpu(block->bb_level);
	switch (block->bb_magic) {
	case cpu_to_be32(XFS_ABTB_CRC_MAGIC):
		fa = xfs_btree_sblock_v5hdr_verify(bp);
		if (fa)
			return fa;
		/* fall through */
	case cpu_to_be32(XFS_ABTB_MAGIC):
		if (pag && pag->pagf_init) {
			if (level >= pag->pagf_levels[XFS_BTNUM_BNOi])
				return __this_address;
		} else if (level >= mp->m_ag_maxlevels)
			return __this_address;
		break;
	case cpu_to_be32(XFS_ABTC_CRC_MAGIC):
		fa = xfs_btree_sblock_v5hdr_verify(bp);
		if (fa)
			return fa;
		/* fall through */
	case cpu_to_be32(XFS_ABTC_MAGIC):
		if (pag && pag->pagf_init) {
			if (level >= pag->pagf_levels[XFS_BTNUM_CNTi])
				return __this_address;
		} else if (level >= mp->m_ag_maxlevels)
			return __this_address;
		break;
	default:
		return __this_address;
	}
=======
	xfs_btnum_t		btnum = XFS_BTNUM_BNOi;

	if (!xfs_verify_magic(bp, block->bb_magic))
		return __this_address;

	if (xfs_has_crc(mp)) {
		fa = xfs_btree_sblock_v5hdr_verify(bp);
		if (fa)
			return fa;
	}

	/*
	 * The perag may not be attached during grow operations or fully
	 * initialized from the AGF during log recovery. Therefore we can only
	 * check against maximum tree depth from those contexts.
	 *
	 * Otherwise check against the per-tree limit. Peek at one of the
	 * verifier magic values to determine the type of tree we're verifying
	 * against.
	 */
	level = be16_to_cpu(block->bb_level);
	if (bp->b_ops->magic[0] == cpu_to_be32(XFS_ABTC_MAGIC))
		btnum = XFS_BTNUM_CNTi;
	if (pag && pag->pagf_init) {
		if (level >= pag->pagf_levels[btnum])
			return __this_address;
	} else if (level >= mp->m_ag_maxlevels)
		return __this_address;
>>>>>>> upstream/android-13

	return xfs_btree_sblock_verify(bp, mp->m_alloc_mxr[level != 0]);
}

static void
xfs_allocbt_read_verify(
	struct xfs_buf	*bp)
{
	xfs_failaddr_t	fa;

	if (!xfs_btree_sblock_verify_crc(bp))
		xfs_verifier_error(bp, -EFSBADCRC, __this_address);
	else {
		fa = xfs_allocbt_verify(bp);
		if (fa)
			xfs_verifier_error(bp, -EFSCORRUPTED, fa);
	}

	if (bp->b_error)
		trace_xfs_btree_corrupt(bp, _RET_IP_);
}

static void
xfs_allocbt_write_verify(
	struct xfs_buf	*bp)
{
	xfs_failaddr_t	fa;

	fa = xfs_allocbt_verify(bp);
	if (fa) {
		trace_xfs_btree_corrupt(bp, _RET_IP_);
		xfs_verifier_error(bp, -EFSCORRUPTED, fa);
		return;
	}
	xfs_btree_sblock_calc_crc(bp);

}

<<<<<<< HEAD
const struct xfs_buf_ops xfs_allocbt_buf_ops = {
	.name = "xfs_allocbt",
=======
const struct xfs_buf_ops xfs_bnobt_buf_ops = {
	.name = "xfs_bnobt",
	.magic = { cpu_to_be32(XFS_ABTB_MAGIC),
		   cpu_to_be32(XFS_ABTB_CRC_MAGIC) },
>>>>>>> upstream/android-13
	.verify_read = xfs_allocbt_read_verify,
	.verify_write = xfs_allocbt_write_verify,
	.verify_struct = xfs_allocbt_verify,
};

<<<<<<< HEAD

STATIC int
xfs_bnobt_keys_inorder(
	struct xfs_btree_cur	*cur,
	union xfs_btree_key	*k1,
	union xfs_btree_key	*k2)
=======
const struct xfs_buf_ops xfs_cntbt_buf_ops = {
	.name = "xfs_cntbt",
	.magic = { cpu_to_be32(XFS_ABTC_MAGIC),
		   cpu_to_be32(XFS_ABTC_CRC_MAGIC) },
	.verify_read = xfs_allocbt_read_verify,
	.verify_write = xfs_allocbt_write_verify,
	.verify_struct = xfs_allocbt_verify,
};

STATIC int
xfs_bnobt_keys_inorder(
	struct xfs_btree_cur		*cur,
	const union xfs_btree_key	*k1,
	const union xfs_btree_key	*k2)
>>>>>>> upstream/android-13
{
	return be32_to_cpu(k1->alloc.ar_startblock) <
	       be32_to_cpu(k2->alloc.ar_startblock);
}

STATIC int
xfs_bnobt_recs_inorder(
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
	return be32_to_cpu(r1->alloc.ar_startblock) +
		be32_to_cpu(r1->alloc.ar_blockcount) <=
		be32_to_cpu(r2->alloc.ar_startblock);
}

STATIC int
xfs_cntbt_keys_inorder(
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
	return be32_to_cpu(k1->alloc.ar_blockcount) <
		be32_to_cpu(k2->alloc.ar_blockcount) ||
		(k1->alloc.ar_blockcount == k2->alloc.ar_blockcount &&
		 be32_to_cpu(k1->alloc.ar_startblock) <
		 be32_to_cpu(k2->alloc.ar_startblock));
}

STATIC int
xfs_cntbt_recs_inorder(
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
	return be32_to_cpu(r1->alloc.ar_blockcount) <
		be32_to_cpu(r2->alloc.ar_blockcount) ||
		(r1->alloc.ar_blockcount == r2->alloc.ar_blockcount &&
		 be32_to_cpu(r1->alloc.ar_startblock) <
		 be32_to_cpu(r2->alloc.ar_startblock));
}

static const struct xfs_btree_ops xfs_bnobt_ops = {
	.rec_len		= sizeof(xfs_alloc_rec_t),
	.key_len		= sizeof(xfs_alloc_key_t),

	.dup_cursor		= xfs_allocbt_dup_cursor,
	.set_root		= xfs_allocbt_set_root,
	.alloc_block		= xfs_allocbt_alloc_block,
	.free_block		= xfs_allocbt_free_block,
	.update_lastrec		= xfs_allocbt_update_lastrec,
	.get_minrecs		= xfs_allocbt_get_minrecs,
	.get_maxrecs		= xfs_allocbt_get_maxrecs,
	.init_key_from_rec	= xfs_allocbt_init_key_from_rec,
	.init_high_key_from_rec	= xfs_bnobt_init_high_key_from_rec,
	.init_rec_from_cur	= xfs_allocbt_init_rec_from_cur,
	.init_ptr_from_cur	= xfs_allocbt_init_ptr_from_cur,
	.key_diff		= xfs_bnobt_key_diff,
<<<<<<< HEAD
	.buf_ops		= &xfs_allocbt_buf_ops,
=======
	.buf_ops		= &xfs_bnobt_buf_ops,
>>>>>>> upstream/android-13
	.diff_two_keys		= xfs_bnobt_diff_two_keys,
	.keys_inorder		= xfs_bnobt_keys_inorder,
	.recs_inorder		= xfs_bnobt_recs_inorder,
};

static const struct xfs_btree_ops xfs_cntbt_ops = {
	.rec_len		= sizeof(xfs_alloc_rec_t),
	.key_len		= sizeof(xfs_alloc_key_t),

	.dup_cursor		= xfs_allocbt_dup_cursor,
	.set_root		= xfs_allocbt_set_root,
	.alloc_block		= xfs_allocbt_alloc_block,
	.free_block		= xfs_allocbt_free_block,
	.update_lastrec		= xfs_allocbt_update_lastrec,
	.get_minrecs		= xfs_allocbt_get_minrecs,
	.get_maxrecs		= xfs_allocbt_get_maxrecs,
	.init_key_from_rec	= xfs_allocbt_init_key_from_rec,
	.init_high_key_from_rec	= xfs_cntbt_init_high_key_from_rec,
	.init_rec_from_cur	= xfs_allocbt_init_rec_from_cur,
	.init_ptr_from_cur	= xfs_allocbt_init_ptr_from_cur,
	.key_diff		= xfs_cntbt_key_diff,
<<<<<<< HEAD
	.buf_ops		= &xfs_allocbt_buf_ops,
=======
	.buf_ops		= &xfs_cntbt_buf_ops,
>>>>>>> upstream/android-13
	.diff_two_keys		= xfs_cntbt_diff_two_keys,
	.keys_inorder		= xfs_cntbt_keys_inorder,
	.recs_inorder		= xfs_cntbt_recs_inorder,
};

<<<<<<< HEAD
=======
/* Allocate most of a new allocation btree cursor. */
STATIC struct xfs_btree_cur *
xfs_allocbt_init_common(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	struct xfs_perag	*pag,
	xfs_btnum_t		btnum)
{
	struct xfs_btree_cur	*cur;

	ASSERT(btnum == XFS_BTNUM_BNO || btnum == XFS_BTNUM_CNT);

	cur = kmem_cache_zalloc(xfs_btree_cur_zone, GFP_NOFS | __GFP_NOFAIL);

	cur->bc_tp = tp;
	cur->bc_mp = mp;
	cur->bc_btnum = btnum;
	cur->bc_blocklog = mp->m_sb.sb_blocklog;
	cur->bc_ag.abt.active = false;

	if (btnum == XFS_BTNUM_CNT) {
		cur->bc_ops = &xfs_cntbt_ops;
		cur->bc_statoff = XFS_STATS_CALC_INDEX(xs_abtc_2);
		cur->bc_flags = XFS_BTREE_LASTREC_UPDATE;
	} else {
		cur->bc_ops = &xfs_bnobt_ops;
		cur->bc_statoff = XFS_STATS_CALC_INDEX(xs_abtb_2);
	}

	/* take a reference for the cursor */
	atomic_inc(&pag->pag_ref);
	cur->bc_ag.pag = pag;

	if (xfs_has_crc(mp))
		cur->bc_flags |= XFS_BTREE_CRC_BLOCKS;

	return cur;
}

>>>>>>> upstream/android-13
/*
 * Allocate a new allocation btree cursor.
 */
struct xfs_btree_cur *			/* new alloc btree cursor */
xfs_allocbt_init_cursor(
	struct xfs_mount	*mp,		/* file system mount point */
	struct xfs_trans	*tp,		/* transaction pointer */
	struct xfs_buf		*agbp,		/* buffer for agf structure */
<<<<<<< HEAD
	xfs_agnumber_t		agno,		/* allocation group number */
	xfs_btnum_t		btnum)		/* btree identifier */
{
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(agbp);
	struct xfs_btree_cur	*cur;

	ASSERT(btnum == XFS_BTNUM_BNO || btnum == XFS_BTNUM_CNT);

	cur = kmem_zone_zalloc(xfs_btree_cur_zone, KM_NOFS);

	cur->bc_tp = tp;
	cur->bc_mp = mp;
	cur->bc_btnum = btnum;
	cur->bc_blocklog = mp->m_sb.sb_blocklog;

	if (btnum == XFS_BTNUM_CNT) {
		cur->bc_statoff = XFS_STATS_CALC_INDEX(xs_abtc_2);
		cur->bc_ops = &xfs_cntbt_ops;
		cur->bc_nlevels = be32_to_cpu(agf->agf_levels[XFS_BTNUM_CNT]);
		cur->bc_flags = XFS_BTREE_LASTREC_UPDATE;
	} else {
		cur->bc_statoff = XFS_STATS_CALC_INDEX(xs_abtb_2);
		cur->bc_ops = &xfs_bnobt_ops;
		cur->bc_nlevels = be32_to_cpu(agf->agf_levels[XFS_BTNUM_BNO]);
	}

	cur->bc_private.a.agbp = agbp;
	cur->bc_private.a.agno = agno;

	if (xfs_sb_version_hascrc(&mp->m_sb))
		cur->bc_flags |= XFS_BTREE_CRC_BLOCKS;
=======
	struct xfs_perag	*pag,
	xfs_btnum_t		btnum)		/* btree identifier */
{
	struct xfs_agf		*agf = agbp->b_addr;
	struct xfs_btree_cur	*cur;

	cur = xfs_allocbt_init_common(mp, tp, pag, btnum);
	if (btnum == XFS_BTNUM_CNT)
		cur->bc_nlevels = be32_to_cpu(agf->agf_levels[XFS_BTNUM_CNT]);
	else
		cur->bc_nlevels = be32_to_cpu(agf->agf_levels[XFS_BTNUM_BNO]);

	cur->bc_ag.agbp = agbp;
>>>>>>> upstream/android-13

	return cur;
}

<<<<<<< HEAD
=======
/* Create a free space btree cursor with a fake root for staging. */
struct xfs_btree_cur *
xfs_allocbt_stage_cursor(
	struct xfs_mount	*mp,
	struct xbtree_afakeroot	*afake,
	struct xfs_perag	*pag,
	xfs_btnum_t		btnum)
{
	struct xfs_btree_cur	*cur;

	cur = xfs_allocbt_init_common(mp, NULL, pag, btnum);
	xfs_btree_stage_afakeroot(cur, afake);
	return cur;
}

/*
 * Install a new free space btree root.  Caller is responsible for invalidating
 * and freeing the old btree blocks.
 */
void
xfs_allocbt_commit_staged_btree(
	struct xfs_btree_cur	*cur,
	struct xfs_trans	*tp,
	struct xfs_buf		*agbp)
{
	struct xfs_agf		*agf = agbp->b_addr;
	struct xbtree_afakeroot	*afake = cur->bc_ag.afake;

	ASSERT(cur->bc_flags & XFS_BTREE_STAGING);

	agf->agf_roots[cur->bc_btnum] = cpu_to_be32(afake->af_root);
	agf->agf_levels[cur->bc_btnum] = cpu_to_be32(afake->af_levels);
	xfs_alloc_log_agf(tp, agbp, XFS_AGF_ROOTS | XFS_AGF_LEVELS);

	if (cur->bc_btnum == XFS_BTNUM_BNO) {
		xfs_btree_commit_afakeroot(cur, tp, agbp, &xfs_bnobt_ops);
	} else {
		cur->bc_flags |= XFS_BTREE_LASTREC_UPDATE;
		xfs_btree_commit_afakeroot(cur, tp, agbp, &xfs_cntbt_ops);
	}
}

>>>>>>> upstream/android-13
/*
 * Calculate number of records in an alloc btree block.
 */
int
xfs_allocbt_maxrecs(
	struct xfs_mount	*mp,
	int			blocklen,
	int			leaf)
{
	blocklen -= XFS_ALLOC_BLOCK_LEN(mp);

	if (leaf)
		return blocklen / sizeof(xfs_alloc_rec_t);
	return blocklen / (sizeof(xfs_alloc_key_t) + sizeof(xfs_alloc_ptr_t));
}

/* Calculate the freespace btree size for some records. */
xfs_extlen_t
xfs_allocbt_calc_size(
	struct xfs_mount	*mp,
	unsigned long long	len)
{
	return xfs_btree_calc_size(mp->m_alloc_mnr, len);
}
