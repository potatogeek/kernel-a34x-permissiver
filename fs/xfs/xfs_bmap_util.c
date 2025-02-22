// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
 * Copyright (c) 2012 Red Hat, Inc.
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
<<<<<<< HEAD
#include "xfs_da_format.h"
=======
>>>>>>> upstream/android-13
#include "xfs_defer.h"
#include "xfs_inode.h"
#include "xfs_btree.h"
#include "xfs_trans.h"
<<<<<<< HEAD
#include "xfs_extfree_item.h"
=======
>>>>>>> upstream/android-13
#include "xfs_alloc.h"
#include "xfs_bmap.h"
#include "xfs_bmap_util.h"
#include "xfs_bmap_btree.h"
#include "xfs_rtalloc.h"
#include "xfs_error.h"
#include "xfs_quota.h"
#include "xfs_trans_space.h"
#include "xfs_trace.h"
#include "xfs_icache.h"
<<<<<<< HEAD
#include "xfs_log.h"
#include "xfs_rmap_btree.h"
#include "xfs_iomap.h"
#include "xfs_reflink.h"
#include "xfs_refcount.h"
=======
#include "xfs_iomap.h"
#include "xfs_reflink.h"
>>>>>>> upstream/android-13

/* Kernel only BMAP related definitions and functions */

/*
 * Convert the given file system block to a disk block.  We have to treat it
 * differently based on whether the file is a real time file or not, because the
 * bmap code does.
 */
xfs_daddr_t
xfs_fsb_to_db(struct xfs_inode *ip, xfs_fsblock_t fsb)
{
<<<<<<< HEAD
	return (XFS_IS_REALTIME_INODE(ip) ? \
		 (xfs_daddr_t)XFS_FSB_TO_BB((ip)->i_mount, (fsb)) : \
		 XFS_FSB_TO_DADDR((ip)->i_mount, (fsb)));
=======
	if (XFS_IS_REALTIME_INODE(ip))
		return XFS_FSB_TO_BB(ip->i_mount, fsb);
	return XFS_FSB_TO_DADDR(ip->i_mount, fsb);
>>>>>>> upstream/android-13
}

/*
 * Routine to zero an extent on disk allocated to the specific inode.
 *
 * The VFS functions take a linearised filesystem block offset, so we have to
 * convert the sparse xfs fsb to the right format first.
 * VFS types are real funky, too.
 */
int
xfs_zero_extent(
<<<<<<< HEAD
	struct xfs_inode *ip,
	xfs_fsblock_t	start_fsb,
	xfs_off_t	count_fsb)
{
	struct xfs_mount *mp = ip->i_mount;
	xfs_daddr_t	sector = xfs_fsb_to_db(ip, start_fsb);
	sector_t	block = XFS_BB_TO_FSBT(mp, sector);

	return blkdev_issue_zeroout(xfs_find_bdev_for_inode(VFS_I(ip)),
=======
	struct xfs_inode	*ip,
	xfs_fsblock_t		start_fsb,
	xfs_off_t		count_fsb)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_buftarg	*target = xfs_inode_buftarg(ip);
	xfs_daddr_t		sector = xfs_fsb_to_db(ip, start_fsb);
	sector_t		block = XFS_BB_TO_FSBT(mp, sector);

	return blkdev_issue_zeroout(target->bt_bdev,
>>>>>>> upstream/android-13
		block << (mp->m_super->s_blocksize_bits - 9),
		count_fsb << (mp->m_super->s_blocksize_bits - 9),
		GFP_NOFS, 0);
}

#ifdef CONFIG_XFS_RT
int
xfs_bmap_rtalloc(
<<<<<<< HEAD
	struct xfs_bmalloca	*ap)	/* bmap alloc argument struct */
{
	int		error;		/* error return value */
	xfs_mount_t	*mp;		/* mount point structure */
	xfs_extlen_t	prod = 0;	/* product factor for allocators */
	xfs_extlen_t	mod = 0;	/* product factor for allocators */
	xfs_extlen_t	ralen = 0;	/* realtime allocation length */
	xfs_extlen_t	align;		/* minimum allocation alignment */
	xfs_rtblock_t	rtb;

	mp = ap->ip->i_mount;
	align = xfs_get_extsz_hint(ap->ip);
=======
	struct xfs_bmalloca	*ap)
{
	struct xfs_mount	*mp = ap->ip->i_mount;
	xfs_fileoff_t		orig_offset = ap->offset;
	xfs_rtblock_t		rtb;
	xfs_extlen_t		prod = 0;  /* product factor for allocators */
	xfs_extlen_t		mod = 0;   /* product factor for allocators */
	xfs_extlen_t		ralen = 0; /* realtime allocation length */
	xfs_extlen_t		align;     /* minimum allocation alignment */
	xfs_extlen_t		orig_length = ap->length;
	xfs_extlen_t		minlen = mp->m_sb.sb_rextsize;
	xfs_extlen_t		raminlen;
	bool			rtlocked = false;
	bool			ignore_locality = false;
	int			error;

	align = xfs_get_extsz_hint(ap->ip);
retry:
>>>>>>> upstream/android-13
	prod = align / mp->m_sb.sb_rextsize;
	error = xfs_bmap_extsize_align(mp, &ap->got, &ap->prev,
					align, 1, ap->eof, 0,
					ap->conv, &ap->offset, &ap->length);
	if (error)
		return error;
	ASSERT(ap->length);
	ASSERT(ap->length % mp->m_sb.sb_rextsize == 0);

	/*
<<<<<<< HEAD
=======
	 * If we shifted the file offset downward to satisfy an extent size
	 * hint, increase minlen by that amount so that the allocator won't
	 * give us an allocation that's too short to cover at least one of the
	 * blocks that the caller asked for.
	 */
	if (ap->offset != orig_offset)
		minlen += orig_offset - ap->offset;

	/*
>>>>>>> upstream/android-13
	 * If the offset & length are not perfectly aligned
	 * then kill prod, it will just get us in trouble.
	 */
	div_u64_rem(ap->offset, align, &mod);
	if (mod || ap->length % align)
		prod = 1;
	/*
	 * Set ralen to be the actual requested length in rtextents.
	 */
	ralen = ap->length / mp->m_sb.sb_rextsize;
	/*
	 * If the old value was close enough to MAXEXTLEN that
	 * we rounded up to it, cut it back so it's valid again.
	 * Note that if it's a really large request (bigger than
	 * MAXEXTLEN), we don't hear about that number, and can't
	 * adjust the starting point to match it.
	 */
	if (ralen * mp->m_sb.sb_rextsize >= MAXEXTLEN)
		ralen = MAXEXTLEN / mp->m_sb.sb_rextsize;

	/*
	 * Lock out modifications to both the RT bitmap and summary inodes
	 */
<<<<<<< HEAD
	xfs_ilock(mp->m_rbmip, XFS_ILOCK_EXCL|XFS_ILOCK_RTBITMAP);
	xfs_trans_ijoin(ap->tp, mp->m_rbmip, XFS_ILOCK_EXCL);
	xfs_ilock(mp->m_rsumip, XFS_ILOCK_EXCL|XFS_ILOCK_RTSUM);
	xfs_trans_ijoin(ap->tp, mp->m_rsumip, XFS_ILOCK_EXCL);
=======
	if (!rtlocked) {
		xfs_ilock(mp->m_rbmip, XFS_ILOCK_EXCL|XFS_ILOCK_RTBITMAP);
		xfs_trans_ijoin(ap->tp, mp->m_rbmip, XFS_ILOCK_EXCL);
		xfs_ilock(mp->m_rsumip, XFS_ILOCK_EXCL|XFS_ILOCK_RTSUM);
		xfs_trans_ijoin(ap->tp, mp->m_rsumip, XFS_ILOCK_EXCL);
		rtlocked = true;
	}
>>>>>>> upstream/android-13

	/*
	 * If it's an allocation to an empty file at offset 0,
	 * pick an extent that will space things out in the rt area.
	 */
	if (ap->eof && ap->offset == 0) {
<<<<<<< HEAD
		xfs_rtblock_t uninitialized_var(rtx); /* realtime extent no */
=======
		xfs_rtblock_t rtx; /* realtime extent no */
>>>>>>> upstream/android-13

		error = xfs_rtpick_extent(mp, ap->tp, ralen, &rtx);
		if (error)
			return error;
		ap->blkno = rtx * mp->m_sb.sb_rextsize;
	} else {
		ap->blkno = 0;
	}

	xfs_bmap_adjacent(ap);

	/*
	 * Realtime allocation, done through xfs_rtallocate_extent.
	 */
<<<<<<< HEAD
	do_div(ap->blkno, mp->m_sb.sb_rextsize);
	rtb = ap->blkno;
	ap->length = ralen;
	error = xfs_rtallocate_extent(ap->tp, ap->blkno, 1, ap->length,
				&ralen, ap->wasdel, prod, &rtb);
	if (error)
		return error;

	ap->blkno = rtb;
	if (ap->blkno != NULLFSBLOCK) {
		ap->blkno *= mp->m_sb.sb_rextsize;
		ralen *= mp->m_sb.sb_rextsize;
		ap->length = ralen;
		ap->ip->i_d.di_nblocks += ralen;
		xfs_trans_log_inode(ap->tp, ap->ip, XFS_ILOG_CORE);
		if (ap->wasdel)
			ap->ip->i_delayed_blks -= ralen;
=======
	if (ignore_locality)
		ap->blkno = 0;
	else
		do_div(ap->blkno, mp->m_sb.sb_rextsize);
	rtb = ap->blkno;
	ap->length = ralen;
	raminlen = max_t(xfs_extlen_t, 1, minlen / mp->m_sb.sb_rextsize);
	error = xfs_rtallocate_extent(ap->tp, ap->blkno, raminlen, ap->length,
			&ralen, ap->wasdel, prod, &rtb);
	if (error)
		return error;

	if (rtb != NULLRTBLOCK) {
		ap->blkno = rtb * mp->m_sb.sb_rextsize;
		ap->length = ralen * mp->m_sb.sb_rextsize;
		ap->ip->i_nblocks += ap->length;
		xfs_trans_log_inode(ap->tp, ap->ip, XFS_ILOG_CORE);
		if (ap->wasdel)
			ap->ip->i_delayed_blks -= ap->length;
>>>>>>> upstream/android-13
		/*
		 * Adjust the disk quota also. This was reserved
		 * earlier.
		 */
		xfs_trans_mod_dquot_byino(ap->tp, ap->ip,
			ap->wasdel ? XFS_TRANS_DQ_DELRTBCOUNT :
<<<<<<< HEAD
					XFS_TRANS_DQ_RTBCOUNT, (long) ralen);

		/* Zero the extent if we were asked to do so */
		if (ap->datatype & XFS_ALLOC_USERDATA_ZERO) {
			error = xfs_zero_extent(ap->ip, ap->blkno, ap->length);
			if (error)
				return error;
		}
	} else {
		ap->length = 0;
	}
=======
					XFS_TRANS_DQ_RTBCOUNT, ap->length);
		return 0;
	}

	if (align > mp->m_sb.sb_rextsize) {
		/*
		 * We previously enlarged the request length to try to satisfy
		 * an extent size hint.  The allocator didn't return anything,
		 * so reset the parameters to the original values and try again
		 * without alignment criteria.
		 */
		ap->offset = orig_offset;
		ap->length = orig_length;
		minlen = align = mp->m_sb.sb_rextsize;
		goto retry;
	}

	if (!ignore_locality && ap->blkno != 0) {
		/*
		 * If we can't allocate near a specific rt extent, try again
		 * without locality criteria.
		 */
		ignore_locality = true;
		goto retry;
	}

	ap->blkno = NULLFSBLOCK;
	ap->length = 0;
>>>>>>> upstream/android-13
	return 0;
}
#endif /* CONFIG_XFS_RT */

/*
<<<<<<< HEAD
 * Check if the endoff is outside the last extent. If so the caller will grow
 * the allocation to a stripe unit boundary.  All offsets are considered outside
 * the end of file for an empty fork, so 1 is returned in *eof in that case.
 */
int
xfs_bmap_eof(
	struct xfs_inode	*ip,
	xfs_fileoff_t		endoff,
	int			whichfork,
	int			*eof)
{
	struct xfs_bmbt_irec	rec;
	int			error;

	error = xfs_bmap_last_extent(NULL, ip, whichfork, &rec, eof);
	if (error || *eof)
		return error;

	*eof = endoff >= rec.br_startoff + rec.br_blockcount;
	return 0;
}

/*
=======
>>>>>>> upstream/android-13
 * Extent tree block counting routines.
 */

/*
 * Count leaf blocks given a range of extent records.  Delayed allocation
 * extents are not counted towards the totals.
 */
xfs_extnum_t
xfs_bmap_count_leaves(
	struct xfs_ifork	*ifp,
	xfs_filblks_t		*count)
{
	struct xfs_iext_cursor	icur;
	struct xfs_bmbt_irec	got;
	xfs_extnum_t		numrecs = 0;

	for_each_xfs_iext(ifp, &icur, &got) {
		if (!isnullstartblock(got.br_startblock)) {
			*count += got.br_blockcount;
			numrecs++;
		}
	}

	return numrecs;
}

/*
<<<<<<< HEAD
 * Count leaf blocks given a range of extent records originally
 * in btree format.
 */
STATIC void
xfs_bmap_disk_count_leaves(
	struct xfs_mount	*mp,
	struct xfs_btree_block	*block,
	int			numrecs,
	xfs_filblks_t		*count)
{
	int		b;
	xfs_bmbt_rec_t	*frp;

	for (b = 1; b <= numrecs; b++) {
		frp = XFS_BMBT_REC_ADDR(mp, block, b);
		*count += xfs_bmbt_disk_get_blockcount(frp);
	}
}

/*
 * Recursively walks each level of a btree
 * to count total fsblocks in use.
 */
STATIC int
xfs_bmap_count_tree(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	struct xfs_ifork	*ifp,
	xfs_fsblock_t		blockno,
	int			levelin,
	xfs_extnum_t		*nextents,
	xfs_filblks_t		*count)
{
	int			error;
	struct xfs_buf		*bp, *nbp;
	int			level = levelin;
	__be64			*pp;
	xfs_fsblock_t           bno = blockno;
	xfs_fsblock_t		nextbno;
	struct xfs_btree_block	*block, *nextblock;
	int			numrecs;

	error = xfs_btree_read_bufl(mp, tp, bno, 0, &bp, XFS_BMAP_BTREE_REF,
						&xfs_bmbt_buf_ops);
	if (error)
		return error;
	*count += 1;
	block = XFS_BUF_TO_BLOCK(bp);

	if (--level) {
		/* Not at node above leaves, count this level of nodes */
		nextbno = be64_to_cpu(block->bb_u.l.bb_rightsib);
		while (nextbno != NULLFSBLOCK) {
			error = xfs_btree_read_bufl(mp, tp, nextbno, 0, &nbp,
						XFS_BMAP_BTREE_REF,
						&xfs_bmbt_buf_ops);
			if (error)
				return error;
			*count += 1;
			nextblock = XFS_BUF_TO_BLOCK(nbp);
			nextbno = be64_to_cpu(nextblock->bb_u.l.bb_rightsib);
			xfs_trans_brelse(tp, nbp);
		}

		/* Dive to the next level */
		pp = XFS_BMBT_PTR_ADDR(mp, block, 1, mp->m_bmap_dmxr[1]);
		bno = be64_to_cpu(*pp);
		error = xfs_bmap_count_tree(mp, tp, ifp, bno, level, nextents,
				count);
		if (error) {
			xfs_trans_brelse(tp, bp);
			XFS_ERROR_REPORT("xfs_bmap_count_tree(1)",
					 XFS_ERRLEVEL_LOW, mp);
			return -EFSCORRUPTED;
		}
		xfs_trans_brelse(tp, bp);
	} else {
		/* count all level 1 nodes and their leaves */
		for (;;) {
			nextbno = be64_to_cpu(block->bb_u.l.bb_rightsib);
			numrecs = be16_to_cpu(block->bb_numrecs);
			(*nextents) += numrecs;
			xfs_bmap_disk_count_leaves(mp, block, numrecs, count);
			xfs_trans_brelse(tp, bp);
			if (nextbno == NULLFSBLOCK)
				break;
			bno = nextbno;
			error = xfs_btree_read_bufl(mp, tp, bno, 0, &bp,
						XFS_BMAP_BTREE_REF,
						&xfs_bmbt_buf_ops);
			if (error)
				return error;
			*count += 1;
			block = XFS_BUF_TO_BLOCK(bp);
		}
	}
	return 0;
}

/*
=======
>>>>>>> upstream/android-13
 * Count fsblocks of the given fork.  Delayed allocation extents are
 * not counted towards the totals.
 */
int
xfs_bmap_count_blocks(
	struct xfs_trans	*tp,
	struct xfs_inode	*ip,
	int			whichfork,
	xfs_extnum_t		*nextents,
	xfs_filblks_t		*count)
{
<<<<<<< HEAD
	struct xfs_mount	*mp;	/* file system mount structure */
	__be64			*pp;	/* pointer to block address */
	struct xfs_btree_block	*block;	/* current btree block */
	struct xfs_ifork	*ifp;	/* fork structure */
	xfs_fsblock_t		bno;	/* block # of "block" */
	int			level;	/* btree level, for checking */
	int			error;

	bno = NULLFSBLOCK;
	mp = ip->i_mount;
	*nextents = 0;
	*count = 0;
	ifp = XFS_IFORK_PTR(ip, whichfork);
	if (!ifp)
		return 0;

	switch (XFS_IFORK_FORMAT(ip, whichfork)) {
	case XFS_DINODE_FMT_EXTENTS:
		*nextents = xfs_bmap_count_leaves(ifp, count);
		return 0;
	case XFS_DINODE_FMT_BTREE:
		if (!(ifp->if_flags & XFS_IFEXTENTS)) {
			error = xfs_iread_extents(tp, ip, whichfork);
			if (error)
				return error;
		}

		/*
		 * Root level must use BMAP_BROOT_PTR_ADDR macro to get ptr out.
		 */
		block = ifp->if_broot;
		level = be16_to_cpu(block->bb_level);
		ASSERT(level > 0);
		pp = XFS_BMAP_BROOT_PTR_ADDR(mp, block, 1, ifp->if_broot_bytes);
		bno = be64_to_cpu(*pp);
		ASSERT(bno != NULLFSBLOCK);
		ASSERT(XFS_FSB_TO_AGNO(mp, bno) < mp->m_sb.sb_agcount);
		ASSERT(XFS_FSB_TO_AGBNO(mp, bno) < mp->m_sb.sb_agblocks);

		error = xfs_bmap_count_tree(mp, tp, ifp, bno, level,
				nextents, count);
		if (error) {
			XFS_ERROR_REPORT("xfs_bmap_count_blocks(2)",
					XFS_ERRLEVEL_LOW, mp);
			return -EFSCORRUPTED;
		}
		return 0;
=======
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_ifork	*ifp = XFS_IFORK_PTR(ip, whichfork);
	struct xfs_btree_cur	*cur;
	xfs_extlen_t		btblocks = 0;
	int			error;

	*nextents = 0;
	*count = 0;

	if (!ifp)
		return 0;

	switch (ifp->if_format) {
	case XFS_DINODE_FMT_BTREE:
		error = xfs_iread_extents(tp, ip, whichfork);
		if (error)
			return error;

		cur = xfs_bmbt_init_cursor(mp, tp, ip, whichfork);
		error = xfs_btree_count_blocks(cur, &btblocks);
		xfs_btree_del_cursor(cur, error);
		if (error)
			return error;

		/*
		 * xfs_btree_count_blocks includes the root block contained in
		 * the inode fork in @btblocks, so subtract one because we're
		 * only interested in allocated disk blocks.
		 */
		*count += btblocks - 1;

		fallthrough;
	case XFS_DINODE_FMT_EXTENTS:
		*nextents = xfs_bmap_count_leaves(ifp, count);
		break;
>>>>>>> upstream/android-13
	}

	return 0;
}

static int
xfs_getbmap_report_one(
	struct xfs_inode	*ip,
	struct getbmapx		*bmv,
	struct kgetbmap		*out,
	int64_t			bmv_end,
	struct xfs_bmbt_irec	*got)
{
	struct kgetbmap		*p = out + bmv->bmv_entries;
<<<<<<< HEAD
	bool			shared = false, trimmed = false;
	int			error;

	error = xfs_reflink_trim_around_shared(ip, got, &shared, &trimmed);
=======
	bool			shared = false;
	int			error;

	error = xfs_reflink_trim_around_shared(ip, got, &shared);
>>>>>>> upstream/android-13
	if (error)
		return error;

	if (isnullstartblock(got->br_startblock) ||
	    got->br_startblock == DELAYSTARTBLOCK) {
		/*
		 * Delalloc extents that start beyond EOF can occur due to
		 * speculative EOF allocation when the delalloc extent is larger
		 * than the largest freespace extent at conversion time.  These
		 * extents cannot be converted by data writeback, so can exist
		 * here even if we are not supposed to be finding delalloc
		 * extents.
		 */
		if (got->br_startoff < XFS_B_TO_FSB(ip->i_mount, XFS_ISIZE(ip)))
			ASSERT((bmv->bmv_iflags & BMV_IF_DELALLOC) != 0);

		p->bmv_oflags |= BMV_OF_DELALLOC;
		p->bmv_block = -2;
	} else {
		p->bmv_block = xfs_fsb_to_db(ip, got->br_startblock);
	}

	if (got->br_state == XFS_EXT_UNWRITTEN &&
	    (bmv->bmv_iflags & BMV_IF_PREALLOC))
		p->bmv_oflags |= BMV_OF_PREALLOC;

	if (shared)
		p->bmv_oflags |= BMV_OF_SHARED;

	p->bmv_offset = XFS_FSB_TO_BB(ip->i_mount, got->br_startoff);
	p->bmv_length = XFS_FSB_TO_BB(ip->i_mount, got->br_blockcount);

	bmv->bmv_offset = p->bmv_offset + p->bmv_length;
	bmv->bmv_length = max(0LL, bmv_end - bmv->bmv_offset);
	bmv->bmv_entries++;
	return 0;
}

static void
xfs_getbmap_report_hole(
	struct xfs_inode	*ip,
	struct getbmapx		*bmv,
	struct kgetbmap		*out,
	int64_t			bmv_end,
	xfs_fileoff_t		bno,
	xfs_fileoff_t		end)
{
	struct kgetbmap		*p = out + bmv->bmv_entries;

	if (bmv->bmv_iflags & BMV_IF_NO_HOLES)
		return;

	p->bmv_block = -1;
	p->bmv_offset = XFS_FSB_TO_BB(ip->i_mount, bno);
	p->bmv_length = XFS_FSB_TO_BB(ip->i_mount, end - bno);

	bmv->bmv_offset = p->bmv_offset + p->bmv_length;
	bmv->bmv_length = max(0LL, bmv_end - bmv->bmv_offset);
	bmv->bmv_entries++;
}

static inline bool
xfs_getbmap_full(
	struct getbmapx		*bmv)
{
	return bmv->bmv_length == 0 || bmv->bmv_entries >= bmv->bmv_count - 1;
}

static bool
xfs_getbmap_next_rec(
	struct xfs_bmbt_irec	*rec,
	xfs_fileoff_t		total_end)
{
	xfs_fileoff_t		end = rec->br_startoff + rec->br_blockcount;

	if (end == total_end)
		return false;

	rec->br_startoff += rec->br_blockcount;
	if (!isnullstartblock(rec->br_startblock) &&
	    rec->br_startblock != DELAYSTARTBLOCK)
		rec->br_startblock += rec->br_blockcount;
	rec->br_blockcount = total_end - end;
	return true;
}

/*
 * Get inode's extents as described in bmv, and format for output.
 * Calls formatter to fill the user's buffer until all extents
 * are mapped, until the passed-in bmv->bmv_count slots have
 * been filled, or until the formatter short-circuits the loop,
 * if it is tracking filled-in extents on its own.
 */
int						/* error code */
xfs_getbmap(
	struct xfs_inode	*ip,
	struct getbmapx		*bmv,		/* user bmap structure */
	struct kgetbmap		*out)
{
	struct xfs_mount	*mp = ip->i_mount;
	int			iflags = bmv->bmv_iflags;
	int			whichfork, lock, error = 0;
	int64_t			bmv_end, max_len;
	xfs_fileoff_t		bno, first_bno;
	struct xfs_ifork	*ifp;
	struct xfs_bmbt_irec	got, rec;
	xfs_filblks_t		len;
	struct xfs_iext_cursor	icur;

	if (bmv->bmv_iflags & ~BMV_IF_VALID)
		return -EINVAL;
#ifndef DEBUG
	/* Only allow CoW fork queries if we're debugging. */
	if (iflags & BMV_IF_COWFORK)
		return -EINVAL;
#endif
	if ((iflags & BMV_IF_ATTRFORK) && (iflags & BMV_IF_COWFORK))
		return -EINVAL;

	if (bmv->bmv_length < -1)
		return -EINVAL;
	bmv->bmv_entries = 0;
	if (bmv->bmv_length == 0)
		return 0;

	if (iflags & BMV_IF_ATTRFORK)
		whichfork = XFS_ATTR_FORK;
	else if (iflags & BMV_IF_COWFORK)
		whichfork = XFS_COW_FORK;
	else
		whichfork = XFS_DATA_FORK;
	ifp = XFS_IFORK_PTR(ip, whichfork);

	xfs_ilock(ip, XFS_IOLOCK_SHARED);
	switch (whichfork) {
	case XFS_ATTR_FORK:
		if (!XFS_IFORK_Q(ip))
			goto out_unlock_iolock;

		max_len = 1LL << 32;
		lock = xfs_ilock_attr_map_shared(ip);
		break;
	case XFS_COW_FORK:
		/* No CoW fork? Just return */
		if (!ifp)
			goto out_unlock_iolock;

		if (xfs_get_cowextsz_hint(ip))
			max_len = mp->m_super->s_maxbytes;
		else
			max_len = XFS_ISIZE(ip);

		lock = XFS_ILOCK_SHARED;
		xfs_ilock(ip, lock);
		break;
	case XFS_DATA_FORK:
		if (!(iflags & BMV_IF_DELALLOC) &&
<<<<<<< HEAD
		    (ip->i_delayed_blks || XFS_ISIZE(ip) > ip->i_d.di_size)) {
=======
		    (ip->i_delayed_blks || XFS_ISIZE(ip) > ip->i_disk_size)) {
>>>>>>> upstream/android-13
			error = filemap_write_and_wait(VFS_I(ip)->i_mapping);
			if (error)
				goto out_unlock_iolock;

			/*
			 * Even after flushing the inode, there can still be
			 * delalloc blocks on the inode beyond EOF due to
			 * speculative preallocation.  These are not removed
			 * until the release function is called or the inode
			 * is inactivated.  Hence we cannot assert here that
			 * ip->i_delayed_blks == 0.
			 */
		}

		if (xfs_get_extsz_hint(ip) ||
<<<<<<< HEAD
		    (ip->i_d.di_flags &
=======
		    (ip->i_diflags &
>>>>>>> upstream/android-13
		     (XFS_DIFLAG_PREALLOC | XFS_DIFLAG_APPEND)))
			max_len = mp->m_super->s_maxbytes;
		else
			max_len = XFS_ISIZE(ip);

		lock = xfs_ilock_data_map_shared(ip);
		break;
	}

<<<<<<< HEAD
	switch (XFS_IFORK_FORMAT(ip, whichfork)) {
=======
	switch (ifp->if_format) {
>>>>>>> upstream/android-13
	case XFS_DINODE_FMT_EXTENTS:
	case XFS_DINODE_FMT_BTREE:
		break;
	case XFS_DINODE_FMT_LOCAL:
		/* Local format inode forks report no extents. */
		goto out_unlock_ilock;
	default:
		error = -EINVAL;
		goto out_unlock_ilock;
	}

	if (bmv->bmv_length == -1) {
		max_len = XFS_FSB_TO_BB(mp, XFS_B_TO_FSB(mp, max_len));
		bmv->bmv_length = max(0LL, max_len - bmv->bmv_offset);
	}

	bmv_end = bmv->bmv_offset + bmv->bmv_length;

	first_bno = bno = XFS_BB_TO_FSBT(mp, bmv->bmv_offset);
	len = XFS_BB_TO_FSB(mp, bmv->bmv_length);

<<<<<<< HEAD
	if (!(ifp->if_flags & XFS_IFEXTENTS)) {
		error = xfs_iread_extents(NULL, ip, whichfork);
		if (error)
			goto out_unlock_ilock;
	}
=======
	error = xfs_iread_extents(NULL, ip, whichfork);
	if (error)
		goto out_unlock_ilock;
>>>>>>> upstream/android-13

	if (!xfs_iext_lookup_extent(ip, ifp, bno, &icur, &got)) {
		/*
		 * Report a whole-file hole if the delalloc flag is set to
		 * stay compatible with the old implementation.
		 */
		if (iflags & BMV_IF_DELALLOC)
			xfs_getbmap_report_hole(ip, bmv, out, bmv_end, bno,
					XFS_B_TO_FSB(mp, XFS_ISIZE(ip)));
		goto out_unlock_ilock;
	}

	while (!xfs_getbmap_full(bmv)) {
		xfs_trim_extent(&got, first_bno, len);

		/*
		 * Report an entry for a hole if this extent doesn't directly
		 * follow the previous one.
		 */
		if (got.br_startoff > bno) {
			xfs_getbmap_report_hole(ip, bmv, out, bmv_end, bno,
					got.br_startoff);
			if (xfs_getbmap_full(bmv))
				break;
		}

		/*
		 * In order to report shared extents accurately, we report each
		 * distinct shared / unshared part of a single bmbt record with
		 * an individual getbmapx record.
		 */
		bno = got.br_startoff + got.br_blockcount;
		rec = got;
		do {
			error = xfs_getbmap_report_one(ip, bmv, out, bmv_end,
					&rec);
			if (error || xfs_getbmap_full(bmv))
				goto out_unlock_ilock;
		} while (xfs_getbmap_next_rec(&rec, bno));

		if (!xfs_iext_next_extent(ifp, &icur, &got)) {
			xfs_fileoff_t	end = XFS_B_TO_FSB(mp, XFS_ISIZE(ip));

			out[bmv->bmv_entries - 1].bmv_oflags |= BMV_OF_LAST;

			if (whichfork != XFS_ATTR_FORK && bno < end &&
			    !xfs_getbmap_full(bmv)) {
				xfs_getbmap_report_hole(ip, bmv, out, bmv_end,
						bno, end);
			}
			break;
		}

		if (bno >= first_bno + len)
			break;
	}

out_unlock_ilock:
	xfs_iunlock(ip, lock);
out_unlock_iolock:
	xfs_iunlock(ip, XFS_IOLOCK_SHARED);
	return error;
}

/*
 * Dead simple method of punching delalyed allocation blocks from a range in
 * the inode.  This will always punch out both the start and end blocks, even
 * if the ranges only partially overlap them, so it is up to the caller to
 * ensure that partial blocks are not passed in.
 */
int
xfs_bmap_punch_delalloc_range(
	struct xfs_inode	*ip,
	xfs_fileoff_t		start_fsb,
	xfs_fileoff_t		length)
{
	struct xfs_ifork	*ifp = &ip->i_df;
	xfs_fileoff_t		end_fsb = start_fsb + length;
	struct xfs_bmbt_irec	got, del;
	struct xfs_iext_cursor	icur;
	int			error = 0;

<<<<<<< HEAD
	ASSERT(ifp->if_flags & XFS_IFEXTENTS);
=======
	ASSERT(!xfs_need_iread_extents(ifp));
>>>>>>> upstream/android-13

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	if (!xfs_iext_lookup_extent_before(ip, ifp, &end_fsb, &icur, &got))
		goto out_unlock;

	while (got.br_startoff + got.br_blockcount > start_fsb) {
		del = got;
		xfs_trim_extent(&del, start_fsb, length);

		/*
		 * A delete can push the cursor forward. Step back to the
		 * previous extent on non-delalloc or extents outside the
		 * target range.
		 */
		if (!del.br_blockcount ||
		    !isnullstartblock(del.br_startblock)) {
			if (!xfs_iext_prev_extent(ifp, &icur, &got))
				break;
			continue;
		}

		error = xfs_bmap_del_extent_delay(ip, XFS_DATA_FORK, &icur,
						  &got, &del);
		if (error || !xfs_iext_get_extent(ifp, &icur, &got))
			break;
	}

out_unlock:
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
	return error;
}

/*
 * Test whether it is appropriate to check an inode for and free post EOF
 * blocks. The 'force' parameter determines whether we should also consider
 * regular files that are marked preallocated or append-only.
 */
bool
<<<<<<< HEAD
xfs_can_free_eofblocks(struct xfs_inode *ip, bool force)
{
=======
xfs_can_free_eofblocks(
	struct xfs_inode	*ip,
	bool			force)
{
	struct xfs_bmbt_irec	imap;
	struct xfs_mount	*mp = ip->i_mount;
	xfs_fileoff_t		end_fsb;
	xfs_fileoff_t		last_fsb;
	int			nimaps = 1;
	int			error;

	/*
	 * Caller must either hold the exclusive io lock; or be inactivating
	 * the inode, which guarantees there are no other users of the inode.
	 */
	ASSERT(xfs_isilocked(ip, XFS_IOLOCK_EXCL) ||
	       (VFS_I(ip)->i_state & I_FREEING));

>>>>>>> upstream/android-13
	/* prealloc/delalloc exists only on regular files */
	if (!S_ISREG(VFS_I(ip)->i_mode))
		return false;

	/*
	 * Zero sized files with no cached pages and delalloc blocks will not
	 * have speculative prealloc/delalloc blocks to remove.
	 */
	if (VFS_I(ip)->i_size == 0 &&
	    VFS_I(ip)->i_mapping->nrpages == 0 &&
	    ip->i_delayed_blks == 0)
		return false;

	/* If we haven't read in the extent list, then don't do it now. */
<<<<<<< HEAD
	if (!(ip->i_df.if_flags & XFS_IFEXTENTS))
=======
	if (xfs_need_iread_extents(&ip->i_df))
>>>>>>> upstream/android-13
		return false;

	/*
	 * Do not free real preallocated or append-only files unless the file
	 * has delalloc blocks and we are forced to remove them.
	 */
<<<<<<< HEAD
	if (ip->i_d.di_flags & (XFS_DIFLAG_PREALLOC | XFS_DIFLAG_APPEND))
		if (!force || ip->i_delayed_blks == 0)
			return false;

	return true;
=======
	if (ip->i_diflags & (XFS_DIFLAG_PREALLOC | XFS_DIFLAG_APPEND))
		if (!force || ip->i_delayed_blks == 0)
			return false;

	/*
	 * Do not try to free post-EOF blocks if EOF is beyond the end of the
	 * range supported by the page cache, because the truncation will loop
	 * forever.
	 */
	end_fsb = XFS_B_TO_FSB(mp, (xfs_ufsize_t)XFS_ISIZE(ip));
	last_fsb = XFS_B_TO_FSB(mp, mp->m_super->s_maxbytes);
	if (last_fsb <= end_fsb)
		return false;

	/*
	 * Look up the mapping for the first block past EOF.  If we can't find
	 * it, there's nothing to free.
	 */
	xfs_ilock(ip, XFS_ILOCK_SHARED);
	error = xfs_bmapi_read(ip, end_fsb, last_fsb - end_fsb, &imap, &nimaps,
			0);
	xfs_iunlock(ip, XFS_ILOCK_SHARED);
	if (error || nimaps == 0)
		return false;

	/*
	 * If there's a real mapping there or there are delayed allocation
	 * reservations, then we have post-EOF blocks to try to free.
	 */
	return imap.br_startblock != HOLESTARTBLOCK || ip->i_delayed_blks;
>>>>>>> upstream/android-13
}

/*
 * This is called to free any blocks beyond eof. The caller must hold
 * IOLOCK_EXCL unless we are in the inode reclaim path and have the only
 * reference to the inode.
 */
int
xfs_free_eofblocks(
	struct xfs_inode	*ip)
{
	struct xfs_trans	*tp;
<<<<<<< HEAD
	int			error;
	xfs_fileoff_t		end_fsb;
	xfs_fileoff_t		last_fsb;
	xfs_filblks_t		map_len;
	int			nimaps;
	struct xfs_bmbt_irec	imap;
	struct xfs_mount	*mp = ip->i_mount;

	/*
	 * Figure out if there are any blocks beyond the end
	 * of the file.  If not, then there is nothing to do.
	 */
	end_fsb = XFS_B_TO_FSB(mp, (xfs_ufsize_t)XFS_ISIZE(ip));
	last_fsb = XFS_B_TO_FSB(mp, mp->m_super->s_maxbytes);
	if (last_fsb <= end_fsb)
		return 0;
	map_len = last_fsb - end_fsb;

	nimaps = 1;
	xfs_ilock(ip, XFS_ILOCK_SHARED);
	error = xfs_bmapi_read(ip, end_fsb, map_len, &imap, &nimaps, 0);
	xfs_iunlock(ip, XFS_ILOCK_SHARED);

	/*
	 * If there are blocks after the end of file, truncate the file to its
	 * current size to free them up.
	 */
	if (!error && (nimaps != 0) &&
	    (imap.br_startblock != HOLESTARTBLOCK ||
	     ip->i_delayed_blks)) {
		/*
		 * Attach the dquots to the inode up front.
		 */
		error = xfs_qm_dqattach(ip);
		if (error)
			return error;

		/* wait on dio to ensure i_size has settled */
		inode_dio_wait(VFS_I(ip));

		error = xfs_trans_alloc(mp, &M_RES(mp)->tr_itruncate, 0, 0, 0,
				&tp);
		if (error) {
			ASSERT(XFS_FORCED_SHUTDOWN(mp));
			return error;
		}

		xfs_ilock(ip, XFS_ILOCK_EXCL);
		xfs_trans_ijoin(tp, ip, 0);

		/*
		 * Do not update the on-disk file size.  If we update the
		 * on-disk file size and then the system crashes before the
		 * contents of the file are flushed to disk then the files
		 * may be full of holes (ie NULL files bug).
		 */
		error = xfs_itruncate_extents_flags(&tp, ip, XFS_DATA_FORK,
					XFS_ISIZE(ip), XFS_BMAPI_NODISCARD);
		if (error) {
			/*
			 * If we get an error at this point we simply don't
			 * bother truncating the file.
			 */
			xfs_trans_cancel(tp);
		} else {
			error = xfs_trans_commit(tp);
			if (!error)
				xfs_inode_clear_eofblocks_tag(ip);
		}

		xfs_iunlock(ip, XFS_ILOCK_EXCL);
	}
=======
	struct xfs_mount	*mp = ip->i_mount;
	int			error;

	/* Attach the dquots to the inode up front. */
	error = xfs_qm_dqattach(ip);
	if (error)
		return error;

	/* Wait on dio to ensure i_size has settled. */
	inode_dio_wait(VFS_I(ip));

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_itruncate, 0, 0, 0, &tp);
	if (error) {
		ASSERT(xfs_is_shutdown(mp));
		return error;
	}

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, 0);

	/*
	 * Do not update the on-disk file size.  If we update the on-disk file
	 * size and then the system crashes before the contents of the file are
	 * flushed to disk then the files may be full of holes (ie NULL files
	 * bug).
	 */
	error = xfs_itruncate_extents_flags(&tp, ip, XFS_DATA_FORK,
				XFS_ISIZE(ip), XFS_BMAPI_NODISCARD);
	if (error)
		goto err_cancel;

	error = xfs_trans_commit(tp);
	if (error)
		goto out_unlock;

	xfs_inode_clear_eofblocks_tag(ip);
	goto out_unlock;

err_cancel:
	/*
	 * If we get an error at this point we simply don't
	 * bother truncating the file.
	 */
	xfs_trans_cancel(tp);
out_unlock:
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
>>>>>>> upstream/android-13
	return error;
}

int
xfs_alloc_file_space(
	struct xfs_inode	*ip,
	xfs_off_t		offset,
	xfs_off_t		len,
	int			alloc_type)
{
	xfs_mount_t		*mp = ip->i_mount;
	xfs_off_t		count;
	xfs_filblks_t		allocated_fsb;
	xfs_filblks_t		allocatesize_fsb;
	xfs_extlen_t		extsz, temp;
	xfs_fileoff_t		startoffset_fsb;
<<<<<<< HEAD
	int			nimaps;
	int			quota_flag;
	int			rt;
	xfs_trans_t		*tp;
	xfs_bmbt_irec_t		imaps[1], *imapp;
	uint			qblocks, resblks, resrtextents;
=======
	xfs_fileoff_t		endoffset_fsb;
	int			nimaps;
	int			rt;
	xfs_trans_t		*tp;
	xfs_bmbt_irec_t		imaps[1], *imapp;
>>>>>>> upstream/android-13
	int			error;

	trace_xfs_alloc_file_space(ip);

<<<<<<< HEAD
	if (XFS_FORCED_SHUTDOWN(mp))
=======
	if (xfs_is_shutdown(mp))
>>>>>>> upstream/android-13
		return -EIO;

	error = xfs_qm_dqattach(ip);
	if (error)
		return error;

	if (len <= 0)
		return -EINVAL;

	rt = XFS_IS_REALTIME_INODE(ip);
	extsz = xfs_get_extsz_hint(ip);

	count = len;
	imapp = &imaps[0];
	nimaps = 1;
	startoffset_fsb	= XFS_B_TO_FSBT(mp, offset);
<<<<<<< HEAD
	allocatesize_fsb = XFS_B_TO_FSB(mp, count);
=======
	endoffset_fsb = XFS_B_TO_FSB(mp, offset + count);
	allocatesize_fsb = endoffset_fsb - startoffset_fsb;
>>>>>>> upstream/android-13

	/*
	 * Allocate file space until done or until there is an error
	 */
	while (allocatesize_fsb && !error) {
		xfs_fileoff_t	s, e;
<<<<<<< HEAD
=======
		unsigned int	dblocks, rblocks, resblks;
>>>>>>> upstream/android-13

		/*
		 * Determine space reservations for data/realtime.
		 */
		if (unlikely(extsz)) {
			s = startoffset_fsb;
			do_div(s, extsz);
			s *= extsz;
			e = startoffset_fsb + allocatesize_fsb;
			div_u64_rem(startoffset_fsb, extsz, &temp);
			if (temp)
				e += temp;
			div_u64_rem(e, extsz, &temp);
			if (temp)
				e += extsz - temp;
		} else {
			s = 0;
			e = allocatesize_fsb;
		}

		/*
		 * The transaction reservation is limited to a 32-bit block
		 * count, hence we need to limit the number of blocks we are
		 * trying to reserve to avoid an overflow. We can't allocate
		 * more than @nimaps extents, and an extent is limited on disk
		 * to MAXEXTLEN (21 bits), so use that to enforce the limit.
		 */
		resblks = min_t(xfs_fileoff_t, (e - s), (MAXEXTLEN * nimaps));
		if (unlikely(rt)) {
<<<<<<< HEAD
			resrtextents = qblocks = resblks;
			resrtextents /= mp->m_sb.sb_rextsize;
			resblks = XFS_DIOSTRAT_SPACE_RES(mp, 0);
			quota_flag = XFS_QMOPT_RES_RTBLKS;
		} else {
			resrtextents = 0;
			resblks = qblocks = XFS_DIOSTRAT_SPACE_RES(mp, resblks);
			quota_flag = XFS_QMOPT_RES_REGBLKS;
=======
			dblocks = XFS_DIOSTRAT_SPACE_RES(mp, 0);
			rblocks = resblks;
		} else {
			dblocks = XFS_DIOSTRAT_SPACE_RES(mp, resblks);
			rblocks = 0;
>>>>>>> upstream/android-13
		}

		/*
		 * Allocate and setup the transaction.
		 */
<<<<<<< HEAD
		error = xfs_trans_alloc(mp, &M_RES(mp)->tr_write, resblks,
				resrtextents, 0, &tp);

		/*
		 * Check for running out of space
		 */
		if (error) {
			/*
			 * Free the transaction structure.
			 */
			ASSERT(error == -ENOSPC || XFS_FORCED_SHUTDOWN(mp));
			break;
		}
		xfs_ilock(ip, XFS_ILOCK_EXCL);
		error = xfs_trans_reserve_quota_nblks(tp, ip, qblocks,
						      0, quota_flag);
		if (error)
			goto error1;

		xfs_trans_ijoin(tp, ip, 0);

		error = xfs_bmapi_write(tp, ip, startoffset_fsb,
					allocatesize_fsb, alloc_type, resblks,
					imapp, &nimaps);
		if (error)
			goto error0;
=======
		error = xfs_trans_alloc_inode(ip, &M_RES(mp)->tr_write,
				dblocks, rblocks, false, &tp);
		if (error)
			break;

		error = xfs_iext_count_may_overflow(ip, XFS_DATA_FORK,
				XFS_IEXT_ADD_NOSPLIT_CNT);
		if (error)
			goto error;

		error = xfs_bmapi_write(tp, ip, startoffset_fsb,
					allocatesize_fsb, alloc_type, 0, imapp,
					&nimaps);
		if (error)
			goto error;
>>>>>>> upstream/android-13

		/*
		 * Complete the transaction
		 */
		error = xfs_trans_commit(tp);
		xfs_iunlock(ip, XFS_ILOCK_EXCL);
		if (error)
			break;

		allocated_fsb = imapp->br_blockcount;

		if (nimaps == 0) {
			error = -ENOSPC;
			break;
		}

		startoffset_fsb += allocated_fsb;
		allocatesize_fsb -= allocated_fsb;
	}

	return error;

<<<<<<< HEAD
error0:	/* unlock inode, unreserve quota blocks, cancel trans */
	xfs_trans_unreserve_quota_nblks(tp, ip, (long)qblocks, 0, quota_flag);

error1:	/* Just cancel transaction */
=======
error:
>>>>>>> upstream/android-13
	xfs_trans_cancel(tp);
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
	return error;
}

static int
xfs_unmap_extent(
	struct xfs_inode	*ip,
	xfs_fileoff_t		startoffset_fsb,
	xfs_filblks_t		len_fsb,
	int			*done)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_trans	*tp;
	uint			resblks = XFS_DIOSTRAT_SPACE_RES(mp, 0);
	int			error;

<<<<<<< HEAD
	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_write, resblks, 0, 0, &tp);
	if (error) {
		ASSERT(error == -ENOSPC || XFS_FORCED_SHUTDOWN(mp));
		return error;
	}

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	error = xfs_trans_reserve_quota(tp, mp, ip->i_udquot, ip->i_gdquot,
			ip->i_pdquot, resblks, 0, XFS_QMOPT_RES_REGBLKS);
	if (error)
		goto out_trans_cancel;

	xfs_trans_ijoin(tp, ip, 0);

=======
	error = xfs_trans_alloc_inode(ip, &M_RES(mp)->tr_write, resblks, 0,
			false, &tp);
	if (error)
		return error;

	error = xfs_iext_count_may_overflow(ip, XFS_DATA_FORK,
			XFS_IEXT_PUNCH_HOLE_CNT);
	if (error)
		goto out_trans_cancel;

>>>>>>> upstream/android-13
	error = xfs_bunmapi(tp, ip, startoffset_fsb, len_fsb, 0, 2, done);
	if (error)
		goto out_trans_cancel;

	error = xfs_trans_commit(tp);
out_unlock:
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
	return error;

out_trans_cancel:
	xfs_trans_cancel(tp);
	goto out_unlock;
}

<<<<<<< HEAD
static int
xfs_adjust_extent_unmap_boundaries(
	struct xfs_inode	*ip,
	xfs_fileoff_t		*startoffset_fsb,
	xfs_fileoff_t		*endoffset_fsb)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_bmbt_irec	imap;
	int			nimap, error;
	xfs_extlen_t		mod = 0;

	nimap = 1;
	error = xfs_bmapi_read(ip, *startoffset_fsb, 1, &imap, &nimap, 0);
	if (error)
		return error;

	if (nimap && imap.br_startblock != HOLESTARTBLOCK) {
		ASSERT(imap.br_startblock != DELAYSTARTBLOCK);
		div_u64_rem(imap.br_startblock, mp->m_sb.sb_rextsize, &mod);
		if (mod)
			*startoffset_fsb += mp->m_sb.sb_rextsize - mod;
	}

	nimap = 1;
	error = xfs_bmapi_read(ip, *endoffset_fsb - 1, 1, &imap, &nimap, 0);
	if (error)
		return error;

	if (nimap && imap.br_startblock != HOLESTARTBLOCK) {
		ASSERT(imap.br_startblock != DELAYSTARTBLOCK);
		mod++;
		if (mod && mod != mp->m_sb.sb_rextsize)
			*endoffset_fsb -= mod;
	}

	return 0;
}

=======
/* Caller must first wait for the completion of any pending DIOs if required. */
>>>>>>> upstream/android-13
int
xfs_flush_unmap_range(
	struct xfs_inode	*ip,
	xfs_off_t		offset,
	xfs_off_t		len)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct inode		*inode = VFS_I(ip);
	xfs_off_t		rounding, start, end;
	int			error;

<<<<<<< HEAD
	/* wait for the completion of any pending DIOs */
	inode_dio_wait(inode);

	rounding = max_t(xfs_off_t, 1 << mp->m_sb.sb_blocklog, PAGE_SIZE);
=======
	rounding = max_t(xfs_off_t, mp->m_sb.sb_blocksize, PAGE_SIZE);
>>>>>>> upstream/android-13
	start = round_down(offset, rounding);
	end = round_up(offset + len, rounding) - 1;

	error = filemap_write_and_wait_range(inode->i_mapping, start, end);
	if (error)
		return error;
	truncate_pagecache_range(inode, start, end);
	return 0;
}

int
xfs_free_file_space(
	struct xfs_inode	*ip,
	xfs_off_t		offset,
	xfs_off_t		len)
{
	struct xfs_mount	*mp = ip->i_mount;
	xfs_fileoff_t		startoffset_fsb;
	xfs_fileoff_t		endoffset_fsb;
	int			done = 0, error;

	trace_xfs_free_file_space(ip);

	error = xfs_qm_dqattach(ip);
	if (error)
		return error;

	if (len <= 0)	/* if nothing being freed */
		return 0;

<<<<<<< HEAD
	error = xfs_flush_unmap_range(ip, offset, len);
	if (error)
		return error;

	startoffset_fsb = XFS_B_TO_FSB(mp, offset);
	endoffset_fsb = XFS_B_TO_FSBT(mp, offset + len);

	/*
	 * Need to zero the stuff we're not freeing, on disk.  If it's a RT file
	 * and we can't use unwritten extents then we actually need to ensure
	 * to zero the whole extent, otherwise we just need to take of block
	 * boundaries, and xfs_bunmapi will handle the rest.
	 */
	if (XFS_IS_REALTIME_INODE(ip) &&
	    !xfs_sb_version_hasextflgbit(&mp->m_sb)) {
		error = xfs_adjust_extent_unmap_boundaries(ip, &startoffset_fsb,
				&endoffset_fsb);
		if (error)
			return error;
	}

=======
	startoffset_fsb = XFS_B_TO_FSB(mp, offset);
	endoffset_fsb = XFS_B_TO_FSBT(mp, offset + len);

	/* We can only free complete realtime extents. */
	if (XFS_IS_REALTIME_INODE(ip) && mp->m_sb.sb_rextsize > 1) {
		startoffset_fsb = roundup_64(startoffset_fsb,
					     mp->m_sb.sb_rextsize);
		endoffset_fsb = rounddown_64(endoffset_fsb,
					     mp->m_sb.sb_rextsize);
	}

	/*
	 * Need to zero the stuff we're not freeing, on disk.
	 */
>>>>>>> upstream/android-13
	if (endoffset_fsb > startoffset_fsb) {
		while (!done) {
			error = xfs_unmap_extent(ip, startoffset_fsb,
					endoffset_fsb - startoffset_fsb, &done);
			if (error)
				return error;
		}
	}

	/*
	 * Now that we've unmap all full blocks we'll have to zero out any
	 * partial block at the beginning and/or end.  iomap_zero_range is smart
	 * enough to skip any holes, including those we just created, but we
	 * must take care not to zero beyond EOF and enlarge i_size.
	 */
	if (offset >= XFS_ISIZE(ip))
		return 0;
	if (offset + len > XFS_ISIZE(ip))
		len = XFS_ISIZE(ip) - offset;
<<<<<<< HEAD
	error = iomap_zero_range(VFS_I(ip), offset, len, NULL, &xfs_iomap_ops);
=======
	error = iomap_zero_range(VFS_I(ip), offset, len, NULL,
			&xfs_buffered_write_iomap_ops);
>>>>>>> upstream/android-13
	if (error)
		return error;

	/*
	 * If we zeroed right up to EOF and EOF straddles a page boundary we
	 * must make sure that the post-EOF area is also zeroed because the
	 * page could be mmap'd and iomap_zero_range doesn't do that for us.
	 * Writeback of the eof page will do this, albeit clumsily.
	 */
	if (offset + len >= XFS_ISIZE(ip) && offset_in_page(offset + len) > 0) {
		error = filemap_write_and_wait_range(VFS_I(ip)->i_mapping,
				round_down(offset + len, PAGE_SIZE), LLONG_MAX);
	}

	return error;
}

<<<<<<< HEAD
/*
 * Preallocate and zero a range of a file. This mechanism has the allocation
 * semantics of fallocate and in addition converts data in the range to zeroes.
 */
int
xfs_zero_file_space(
	struct xfs_inode	*ip,
	xfs_off_t		offset,
	xfs_off_t		len)
{
	struct xfs_mount	*mp = ip->i_mount;
	uint			blksize;
	int			error;

	trace_xfs_zero_file_space(ip);

	blksize = 1 << mp->m_sb.sb_blocklog;

	/*
	 * Punch a hole and prealloc the range. We use hole punch rather than
	 * unwritten extent conversion for two reasons:
	 *
	 * 1.) Hole punch handles partial block zeroing for us.
	 *
	 * 2.) If prealloc returns ENOSPC, the file range is still zero-valued
	 * by virtue of the hole punch.
	 */
	error = xfs_free_file_space(ip, offset, len);
	if (error)
		goto out;

	error = xfs_alloc_file_space(ip, round_down(offset, blksize),
				     round_up(offset + len, blksize) -
				     round_down(offset, blksize),
				     XFS_BMAPI_PREALLOC);
out:
	return error;

}

=======
>>>>>>> upstream/android-13
static int
xfs_prepare_shift(
	struct xfs_inode	*ip,
	loff_t			offset)
{
<<<<<<< HEAD
=======
	struct xfs_mount	*mp = ip->i_mount;
>>>>>>> upstream/android-13
	int			error;

	/*
	 * Trim eofblocks to avoid shifting uninitialized post-eof preallocation
	 * into the accessible region of the file.
	 */
	if (xfs_can_free_eofblocks(ip, true)) {
		error = xfs_free_eofblocks(ip);
		if (error)
			return error;
	}

	/*
<<<<<<< HEAD
=======
	 * Shift operations must stabilize the start block offset boundary along
	 * with the full range of the operation. If we don't, a COW writeback
	 * completion could race with an insert, front merge with the start
	 * extent (after split) during the shift and corrupt the file. Start
	 * with the block just prior to the start to stabilize the boundary.
	 */
	offset = round_down(offset, mp->m_sb.sb_blocksize);
	if (offset)
		offset -= mp->m_sb.sb_blocksize;

	/*
>>>>>>> upstream/android-13
	 * Writeback and invalidate cache for the remainder of the file as we're
	 * about to shift down every extent from offset to EOF.
	 */
	error = xfs_flush_unmap_range(ip, offset, XFS_ISIZE(ip));
	if (error)
		return error;

	/*
	 * Clean out anything hanging around in the cow fork now that
	 * we've flushed all the dirty data out to disk to avoid having
	 * CoW extents at the wrong offsets.
	 */
	if (xfs_inode_has_cow_data(ip)) {
		error = xfs_reflink_cancel_cow_range(ip, offset, NULLFILEOFF,
				true);
		if (error)
			return error;
	}

	return 0;
}

/*
 * xfs_collapse_file_space()
 *	This routine frees disk space and shift extent for the given file.
 *	The first thing we do is to free data blocks in the specified range
 *	by calling xfs_free_file_space(). It would also sync dirty data
 *	and invalidate page cache over the region on which collapse range
 *	is working. And Shift extent records to the left to cover a hole.
 * RETURNS:
 *	0 on success
 *	errno on error
 *
 */
int
xfs_collapse_file_space(
	struct xfs_inode	*ip,
	xfs_off_t		offset,
	xfs_off_t		len)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_trans	*tp;
	int			error;
	xfs_fileoff_t		next_fsb = XFS_B_TO_FSB(mp, offset + len);
	xfs_fileoff_t		shift_fsb = XFS_B_TO_FSB(mp, len);
<<<<<<< HEAD
	uint			resblks = XFS_DIOSTRAT_SPACE_RES(mp, 0);
=======
>>>>>>> upstream/android-13
	bool			done = false;

	ASSERT(xfs_isilocked(ip, XFS_IOLOCK_EXCL));
	ASSERT(xfs_isilocked(ip, XFS_MMAPLOCK_EXCL));

	trace_xfs_collapse_file_space(ip);

	error = xfs_free_file_space(ip, offset, len);
	if (error)
		return error;

	error = xfs_prepare_shift(ip, offset);
	if (error)
		return error;

<<<<<<< HEAD
	while (!error && !done) {
		error = xfs_trans_alloc(mp, &M_RES(mp)->tr_write, resblks, 0, 0,
					&tp);
		if (error)
			break;

		xfs_ilock(ip, XFS_ILOCK_EXCL);
		error = xfs_trans_reserve_quota(tp, mp, ip->i_udquot,
				ip->i_gdquot, ip->i_pdquot, resblks, 0,
				XFS_QMOPT_RES_REGBLKS);
		if (error)
			goto out_trans_cancel;
		xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);

=======
	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_write, 0, 0, 0, &tp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, 0);

	while (!done) {
>>>>>>> upstream/android-13
		error = xfs_bmap_collapse_extents(tp, ip, &next_fsb, shift_fsb,
				&done);
		if (error)
			goto out_trans_cancel;
<<<<<<< HEAD

		error = xfs_trans_commit(tp);
	}

=======
		if (done)
			break;

		/* finish any deferred frees and roll the transaction */
		error = xfs_defer_finish(&tp);
		if (error)
			goto out_trans_cancel;
	}

	error = xfs_trans_commit(tp);
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
>>>>>>> upstream/android-13
	return error;

out_trans_cancel:
	xfs_trans_cancel(tp);
<<<<<<< HEAD
=======
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
>>>>>>> upstream/android-13
	return error;
}

/*
 * xfs_insert_file_space()
 *	This routine create hole space by shifting extents for the given file.
 *	The first thing we do is to sync dirty data and invalidate page cache
 *	over the region on which insert range is working. And split an extent
 *	to two extents at given offset by calling xfs_bmap_split_extent.
 *	And shift all extent records which are laying between [offset,
 *	last allocated extent] to the right to reserve hole range.
 * RETURNS:
 *	0 on success
 *	errno on error
 */
int
xfs_insert_file_space(
	struct xfs_inode	*ip,
	loff_t			offset,
	loff_t			len)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_trans	*tp;
	int			error;
	xfs_fileoff_t		stop_fsb = XFS_B_TO_FSB(mp, offset);
	xfs_fileoff_t		next_fsb = NULLFSBLOCK;
	xfs_fileoff_t		shift_fsb = XFS_B_TO_FSB(mp, len);
	bool			done = false;

	ASSERT(xfs_isilocked(ip, XFS_IOLOCK_EXCL));
	ASSERT(xfs_isilocked(ip, XFS_MMAPLOCK_EXCL));

	trace_xfs_insert_file_space(ip);

	error = xfs_bmap_can_insert_extents(ip, stop_fsb, shift_fsb);
	if (error)
		return error;

	error = xfs_prepare_shift(ip, offset);
	if (error)
		return error;

<<<<<<< HEAD
=======
	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_write,
			XFS_DIOSTRAT_SPACE_RES(mp, 0), 0, 0, &tp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, 0);

	error = xfs_iext_count_may_overflow(ip, XFS_DATA_FORK,
			XFS_IEXT_PUNCH_HOLE_CNT);
	if (error)
		goto out_trans_cancel;

>>>>>>> upstream/android-13
	/*
	 * The extent shifting code works on extent granularity. So, if stop_fsb
	 * is not the starting block of extent, we need to split the extent at
	 * stop_fsb.
	 */
<<<<<<< HEAD
	error = xfs_bmap_split_extent(ip, stop_fsb);
	if (error)
		return error;

	while (!error && !done) {
		error = xfs_trans_alloc(mp, &M_RES(mp)->tr_write, 0, 0, 0,
					&tp);
		if (error)
			break;

		xfs_ilock(ip, XFS_ILOCK_EXCL);
		xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);
=======
	error = xfs_bmap_split_extent(tp, ip, stop_fsb);
	if (error)
		goto out_trans_cancel;

	do {
		error = xfs_defer_finish(&tp);
		if (error)
			goto out_trans_cancel;

>>>>>>> upstream/android-13
		error = xfs_bmap_insert_extents(tp, ip, &next_fsb, shift_fsb,
				&done, stop_fsb);
		if (error)
			goto out_trans_cancel;
<<<<<<< HEAD

		error = xfs_trans_commit(tp);
	}

=======
	} while (!done);

	error = xfs_trans_commit(tp);
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
>>>>>>> upstream/android-13
	return error;

out_trans_cancel:
	xfs_trans_cancel(tp);
<<<<<<< HEAD
=======
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
>>>>>>> upstream/android-13
	return error;
}

/*
 * We need to check that the format of the data fork in the temporary inode is
 * valid for the target inode before doing the swap. This is not a problem with
 * attr1 because of the fixed fork offset, but attr2 has a dynamically sized
 * data fork depending on the space the attribute fork is taking so we can get
 * invalid formats on the target inode.
 *
 * E.g. target has space for 7 extents in extent format, temp inode only has
 * space for 6.  If we defragment down to 7 extents, then the tmp format is a
 * btree, but when swapped it needs to be in extent format. Hence we can't just
 * blindly swap data forks on attr2 filesystems.
 *
 * Note that we check the swap in both directions so that we don't end up with
 * a corrupt temporary inode, either.
 *
 * Note that fixing the way xfs_fsr sets up the attribute fork in the source
 * inode will prevent this situation from occurring, so all we do here is
 * reject and log the attempt. basically we are putting the responsibility on
 * userspace to get this right.
 */
static int
xfs_swap_extents_check_format(
	struct xfs_inode	*ip,	/* target inode */
	struct xfs_inode	*tip)	/* tmp inode */
{
<<<<<<< HEAD

	/* Should never get a local format */
	if (ip->i_d.di_format == XFS_DINODE_FMT_LOCAL ||
	    tip->i_d.di_format == XFS_DINODE_FMT_LOCAL)
=======
	struct xfs_ifork	*ifp = &ip->i_df;
	struct xfs_ifork	*tifp = &tip->i_df;

	/* User/group/project quota ids must match if quotas are enforced. */
	if (XFS_IS_QUOTA_ON(ip->i_mount) &&
	    (!uid_eq(VFS_I(ip)->i_uid, VFS_I(tip)->i_uid) ||
	     !gid_eq(VFS_I(ip)->i_gid, VFS_I(tip)->i_gid) ||
	     ip->i_projid != tip->i_projid))
		return -EINVAL;

	/* Should never get a local format */
	if (ifp->if_format == XFS_DINODE_FMT_LOCAL ||
	    tifp->if_format == XFS_DINODE_FMT_LOCAL)
>>>>>>> upstream/android-13
		return -EINVAL;

	/*
	 * if the target inode has less extents that then temporary inode then
	 * why did userspace call us?
	 */
<<<<<<< HEAD
	if (ip->i_d.di_nextents < tip->i_d.di_nextents)
=======
	if (ifp->if_nextents < tifp->if_nextents)
>>>>>>> upstream/android-13
		return -EINVAL;

	/*
	 * If we have to use the (expensive) rmap swap method, we can
	 * handle any number of extents and any format.
	 */
<<<<<<< HEAD
	if (xfs_sb_version_hasrmapbt(&ip->i_mount->m_sb))
=======
	if (xfs_has_rmapbt(ip->i_mount))
>>>>>>> upstream/android-13
		return 0;

	/*
	 * if the target inode is in extent form and the temp inode is in btree
	 * form then we will end up with the target inode in the wrong format
	 * as we already know there are less extents in the temp inode.
	 */
<<<<<<< HEAD
	if (ip->i_d.di_format == XFS_DINODE_FMT_EXTENTS &&
	    tip->i_d.di_format == XFS_DINODE_FMT_BTREE)
		return -EINVAL;

	/* Check temp in extent form to max in target */
	if (tip->i_d.di_format == XFS_DINODE_FMT_EXTENTS &&
	    XFS_IFORK_NEXTENTS(tip, XFS_DATA_FORK) >
			XFS_IFORK_MAXEXT(ip, XFS_DATA_FORK))
		return -EINVAL;

	/* Check target in extent form to max in temp */
	if (ip->i_d.di_format == XFS_DINODE_FMT_EXTENTS &&
	    XFS_IFORK_NEXTENTS(ip, XFS_DATA_FORK) >
			XFS_IFORK_MAXEXT(tip, XFS_DATA_FORK))
=======
	if (ifp->if_format == XFS_DINODE_FMT_EXTENTS &&
	    tifp->if_format == XFS_DINODE_FMT_BTREE)
		return -EINVAL;

	/* Check temp in extent form to max in target */
	if (tifp->if_format == XFS_DINODE_FMT_EXTENTS &&
	    tifp->if_nextents > XFS_IFORK_MAXEXT(ip, XFS_DATA_FORK))
		return -EINVAL;

	/* Check target in extent form to max in temp */
	if (ifp->if_format == XFS_DINODE_FMT_EXTENTS &&
	    ifp->if_nextents > XFS_IFORK_MAXEXT(tip, XFS_DATA_FORK))
>>>>>>> upstream/android-13
		return -EINVAL;

	/*
	 * If we are in a btree format, check that the temp root block will fit
	 * in the target and that it has enough extents to be in btree format
	 * in the target.
	 *
	 * Note that we have to be careful to allow btree->extent conversions
	 * (a common defrag case) which will occur when the temp inode is in
	 * extent format...
	 */
<<<<<<< HEAD
	if (tip->i_d.di_format == XFS_DINODE_FMT_BTREE) {
		if (XFS_IFORK_Q(ip) &&
		    XFS_BMAP_BMDR_SPACE(tip->i_df.if_broot) > XFS_IFORK_BOFF(ip))
			return -EINVAL;
		if (XFS_IFORK_NEXTENTS(tip, XFS_DATA_FORK) <=
		    XFS_IFORK_MAXEXT(ip, XFS_DATA_FORK))
=======
	if (tifp->if_format == XFS_DINODE_FMT_BTREE) {
		if (XFS_IFORK_Q(ip) &&
		    XFS_BMAP_BMDR_SPACE(tifp->if_broot) > XFS_IFORK_BOFF(ip))
			return -EINVAL;
		if (tifp->if_nextents <= XFS_IFORK_MAXEXT(ip, XFS_DATA_FORK))
>>>>>>> upstream/android-13
			return -EINVAL;
	}

	/* Reciprocal target->temp btree format checks */
<<<<<<< HEAD
	if (ip->i_d.di_format == XFS_DINODE_FMT_BTREE) {
		if (XFS_IFORK_Q(tip) &&
		    XFS_BMAP_BMDR_SPACE(ip->i_df.if_broot) > XFS_IFORK_BOFF(tip))
			return -EINVAL;
		if (XFS_IFORK_NEXTENTS(ip, XFS_DATA_FORK) <=
		    XFS_IFORK_MAXEXT(tip, XFS_DATA_FORK))
=======
	if (ifp->if_format == XFS_DINODE_FMT_BTREE) {
		if (XFS_IFORK_Q(tip) &&
		    XFS_BMAP_BMDR_SPACE(ip->i_df.if_broot) > XFS_IFORK_BOFF(tip))
			return -EINVAL;
		if (ifp->if_nextents <= XFS_IFORK_MAXEXT(tip, XFS_DATA_FORK))
>>>>>>> upstream/android-13
			return -EINVAL;
	}

	return 0;
}

static int
xfs_swap_extent_flush(
	struct xfs_inode	*ip)
{
	int	error;

	error = filemap_write_and_wait(VFS_I(ip)->i_mapping);
	if (error)
		return error;
	truncate_pagecache_range(VFS_I(ip), 0, -1);

	/* Verify O_DIRECT for ftmp */
	if (VFS_I(ip)->i_mapping->nrpages)
		return -EINVAL;
	return 0;
}

/*
 * Move extents from one file to another, when rmap is enabled.
 */
STATIC int
xfs_swap_extent_rmap(
	struct xfs_trans		**tpp,
	struct xfs_inode		*ip,
	struct xfs_inode		*tip)
{
	struct xfs_trans		*tp = *tpp;
	struct xfs_bmbt_irec		irec;
	struct xfs_bmbt_irec		uirec;
	struct xfs_bmbt_irec		tirec;
	xfs_fileoff_t			offset_fsb;
	xfs_fileoff_t			end_fsb;
	xfs_filblks_t			count_fsb;
	int				error;
	xfs_filblks_t			ilen;
	xfs_filblks_t			rlen;
	int				nimaps;
	uint64_t			tip_flags2;

	/*
	 * If the source file has shared blocks, we must flag the donor
	 * file as having shared blocks so that we get the shared-block
	 * rmap functions when we go to fix up the rmaps.  The flags
	 * will be switch for reals later.
	 */
<<<<<<< HEAD
	tip_flags2 = tip->i_d.di_flags2;
	if (ip->i_d.di_flags2 & XFS_DIFLAG2_REFLINK)
		tip->i_d.di_flags2 |= XFS_DIFLAG2_REFLINK;
=======
	tip_flags2 = tip->i_diflags2;
	if (ip->i_diflags2 & XFS_DIFLAG2_REFLINK)
		tip->i_diflags2 |= XFS_DIFLAG2_REFLINK;
>>>>>>> upstream/android-13

	offset_fsb = 0;
	end_fsb = XFS_B_TO_FSB(ip->i_mount, i_size_read(VFS_I(ip)));
	count_fsb = (xfs_filblks_t)(end_fsb - offset_fsb);

	while (count_fsb) {
		/* Read extent from the donor file */
		nimaps = 1;
		error = xfs_bmapi_read(tip, offset_fsb, count_fsb, &tirec,
				&nimaps, 0);
		if (error)
			goto out;
		ASSERT(nimaps == 1);
		ASSERT(tirec.br_startblock != DELAYSTARTBLOCK);

		trace_xfs_swap_extent_rmap_remap(tip, &tirec);
		ilen = tirec.br_blockcount;

		/* Unmap the old blocks in the source file. */
		while (tirec.br_blockcount) {
			ASSERT(tp->t_firstblock == NULLFSBLOCK);
			trace_xfs_swap_extent_rmap_remap_piece(tip, &tirec);

			/* Read extent from the source file */
			nimaps = 1;
			error = xfs_bmapi_read(ip, tirec.br_startoff,
					tirec.br_blockcount, &irec,
					&nimaps, 0);
			if (error)
				goto out;
			ASSERT(nimaps == 1);
			ASSERT(tirec.br_startoff == irec.br_startoff);
			trace_xfs_swap_extent_rmap_remap_piece(ip, &irec);

			/* Trim the extent. */
			uirec = tirec;
			uirec.br_blockcount = rlen = min_t(xfs_filblks_t,
					tirec.br_blockcount,
					irec.br_blockcount);
			trace_xfs_swap_extent_rmap_remap_piece(tip, &uirec);

<<<<<<< HEAD
			/* Remove the mapping from the donor file. */
			error = xfs_bmap_unmap_extent(tp, tip, &uirec);
			if (error)
				goto out;

			/* Remove the mapping from the source file. */
			error = xfs_bmap_unmap_extent(tp, ip, &irec);
			if (error)
				goto out;

			/* Map the donor file's blocks into the source file. */
			error = xfs_bmap_map_extent(tp, ip, &uirec);
			if (error)
				goto out;

			/* Map the source file's blocks into the donor file. */
			error = xfs_bmap_map_extent(tp, tip, &irec);
			if (error)
				goto out;
=======
			if (xfs_bmap_is_real_extent(&uirec)) {
				error = xfs_iext_count_may_overflow(ip,
						XFS_DATA_FORK,
						XFS_IEXT_SWAP_RMAP_CNT);
				if (error)
					goto out;
			}

			if (xfs_bmap_is_real_extent(&irec)) {
				error = xfs_iext_count_may_overflow(tip,
						XFS_DATA_FORK,
						XFS_IEXT_SWAP_RMAP_CNT);
				if (error)
					goto out;
			}

			/* Remove the mapping from the donor file. */
			xfs_bmap_unmap_extent(tp, tip, &uirec);

			/* Remove the mapping from the source file. */
			xfs_bmap_unmap_extent(tp, ip, &irec);

			/* Map the donor file's blocks into the source file. */
			xfs_bmap_map_extent(tp, ip, &uirec);

			/* Map the source file's blocks into the donor file. */
			xfs_bmap_map_extent(tp, tip, &irec);
>>>>>>> upstream/android-13

			error = xfs_defer_finish(tpp);
			tp = *tpp;
			if (error)
				goto out;

			tirec.br_startoff += rlen;
			if (tirec.br_startblock != HOLESTARTBLOCK &&
			    tirec.br_startblock != DELAYSTARTBLOCK)
				tirec.br_startblock += rlen;
			tirec.br_blockcount -= rlen;
		}

		/* Roll on... */
		count_fsb -= ilen;
		offset_fsb += ilen;
	}

<<<<<<< HEAD
	tip->i_d.di_flags2 = tip_flags2;
=======
	tip->i_diflags2 = tip_flags2;
>>>>>>> upstream/android-13
	return 0;

out:
	trace_xfs_swap_extent_rmap_error(ip, error, _RET_IP_);
<<<<<<< HEAD
	tip->i_d.di_flags2 = tip_flags2;
=======
	tip->i_diflags2 = tip_flags2;
>>>>>>> upstream/android-13
	return error;
}

/* Swap the extents of two files by swapping data forks. */
STATIC int
xfs_swap_extent_forks(
	struct xfs_trans	*tp,
	struct xfs_inode	*ip,
	struct xfs_inode	*tip,
	int			*src_log_flags,
	int			*target_log_flags)
{
	xfs_filblks_t		aforkblks = 0;
	xfs_filblks_t		taforkblks = 0;
	xfs_extnum_t		junk;
	uint64_t		tmp;
	int			error;

	/*
	 * Count the number of extended attribute blocks
	 */
<<<<<<< HEAD
	if ( ((XFS_IFORK_Q(ip) != 0) && (ip->i_d.di_anextents > 0)) &&
	     (ip->i_d.di_aformat != XFS_DINODE_FMT_LOCAL)) {
=======
	if (XFS_IFORK_Q(ip) && ip->i_afp->if_nextents > 0 &&
	    ip->i_afp->if_format != XFS_DINODE_FMT_LOCAL) {
>>>>>>> upstream/android-13
		error = xfs_bmap_count_blocks(tp, ip, XFS_ATTR_FORK, &junk,
				&aforkblks);
		if (error)
			return error;
	}
<<<<<<< HEAD
	if ( ((XFS_IFORK_Q(tip) != 0) && (tip->i_d.di_anextents > 0)) &&
	     (tip->i_d.di_aformat != XFS_DINODE_FMT_LOCAL)) {
=======
	if (XFS_IFORK_Q(tip) && tip->i_afp->if_nextents > 0 &&
	    tip->i_afp->if_format != XFS_DINODE_FMT_LOCAL) {
>>>>>>> upstream/android-13
		error = xfs_bmap_count_blocks(tp, tip, XFS_ATTR_FORK, &junk,
				&taforkblks);
		if (error)
			return error;
	}

	/*
	 * Btree format (v3) inodes have the inode number stamped in the bmbt
	 * block headers. We can't start changing the bmbt blocks until the
	 * inode owner change is logged so recovery does the right thing in the
	 * event of a crash. Set the owner change log flags now and leave the
	 * bmbt scan as the last step.
	 */
<<<<<<< HEAD
	if (ip->i_d.di_version == 3 &&
	    ip->i_d.di_format == XFS_DINODE_FMT_BTREE)
		(*target_log_flags) |= XFS_ILOG_DOWNER;
	if (tip->i_d.di_version == 3 &&
	    tip->i_d.di_format == XFS_DINODE_FMT_BTREE)
		(*src_log_flags) |= XFS_ILOG_DOWNER;
=======
	if (xfs_has_v3inodes(ip->i_mount)) {
		if (ip->i_df.if_format == XFS_DINODE_FMT_BTREE)
			(*target_log_flags) |= XFS_ILOG_DOWNER;
		if (tip->i_df.if_format == XFS_DINODE_FMT_BTREE)
			(*src_log_flags) |= XFS_ILOG_DOWNER;
	}
>>>>>>> upstream/android-13

	/*
	 * Swap the data forks of the inodes
	 */
	swap(ip->i_df, tip->i_df);

	/*
	 * Fix the on-disk inode values
	 */
<<<<<<< HEAD
	tmp = (uint64_t)ip->i_d.di_nblocks;
	ip->i_d.di_nblocks = tip->i_d.di_nblocks - taforkblks + aforkblks;
	tip->i_d.di_nblocks = tmp + taforkblks - aforkblks;

	swap(ip->i_d.di_nextents, tip->i_d.di_nextents);
	swap(ip->i_d.di_format, tip->i_d.di_format);
=======
	tmp = (uint64_t)ip->i_nblocks;
	ip->i_nblocks = tip->i_nblocks - taforkblks + aforkblks;
	tip->i_nblocks = tmp + taforkblks - aforkblks;
>>>>>>> upstream/android-13

	/*
	 * The extents in the source inode could still contain speculative
	 * preallocation beyond EOF (e.g. the file is open but not modified
	 * while defrag is in progress). In that case, we need to copy over the
	 * number of delalloc blocks the data fork in the source inode is
	 * tracking beyond EOF so that when the fork is truncated away when the
	 * temporary inode is unlinked we don't underrun the i_delayed_blks
	 * counter on that inode.
	 */
	ASSERT(tip->i_delayed_blks == 0);
	tip->i_delayed_blks = ip->i_delayed_blks;
	ip->i_delayed_blks = 0;

<<<<<<< HEAD
	switch (ip->i_d.di_format) {
=======
	switch (ip->i_df.if_format) {
>>>>>>> upstream/android-13
	case XFS_DINODE_FMT_EXTENTS:
		(*src_log_flags) |= XFS_ILOG_DEXT;
		break;
	case XFS_DINODE_FMT_BTREE:
<<<<<<< HEAD
		ASSERT(ip->i_d.di_version < 3 ||
=======
		ASSERT(!xfs_has_v3inodes(ip->i_mount) ||
>>>>>>> upstream/android-13
		       (*src_log_flags & XFS_ILOG_DOWNER));
		(*src_log_flags) |= XFS_ILOG_DBROOT;
		break;
	}

<<<<<<< HEAD
	switch (tip->i_d.di_format) {
=======
	switch (tip->i_df.if_format) {
>>>>>>> upstream/android-13
	case XFS_DINODE_FMT_EXTENTS:
		(*target_log_flags) |= XFS_ILOG_DEXT;
		break;
	case XFS_DINODE_FMT_BTREE:
		(*target_log_flags) |= XFS_ILOG_DBROOT;
<<<<<<< HEAD
		ASSERT(tip->i_d.di_version < 3 ||
=======
		ASSERT(!xfs_has_v3inodes(ip->i_mount) ||
>>>>>>> upstream/android-13
		       (*target_log_flags & XFS_ILOG_DOWNER));
		break;
	}

	return 0;
}

/*
 * Fix up the owners of the bmbt blocks to refer to the current inode. The
 * change owner scan attempts to order all modified buffers in the current
 * transaction. In the event of ordered buffer failure, the offending buffer is
 * physically logged as a fallback and the scan returns -EAGAIN. We must roll
 * the transaction in this case to replenish the fallback log reservation and
 * restart the scan. This process repeats until the scan completes.
 */
static int
xfs_swap_change_owner(
	struct xfs_trans	**tpp,
	struct xfs_inode	*ip,
	struct xfs_inode	*tmpip)
{
	int			error;
	struct xfs_trans	*tp = *tpp;

	do {
		error = xfs_bmbt_change_owner(tp, ip, XFS_DATA_FORK, ip->i_ino,
					      NULL);
		/* success or fatal error */
		if (error != -EAGAIN)
			break;

		error = xfs_trans_roll(tpp);
		if (error)
			break;
		tp = *tpp;

		/*
		 * Redirty both inodes so they can relog and keep the log tail
		 * moving forward.
		 */
		xfs_trans_ijoin(tp, ip, 0);
		xfs_trans_ijoin(tp, tmpip, 0);
		xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);
		xfs_trans_log_inode(tp, tmpip, XFS_ILOG_CORE);
	} while (true);

	return error;
}

int
xfs_swap_extents(
	struct xfs_inode	*ip,	/* target inode */
	struct xfs_inode	*tip,	/* tmp inode */
	struct xfs_swapext	*sxp)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_trans	*tp;
	struct xfs_bstat	*sbp = &sxp->sx_stat;
	int			src_log_flags, target_log_flags;
	int			error = 0;
<<<<<<< HEAD
	int			lock_flags;
	uint64_t		f;
	int			resblks = 0;
=======
	uint64_t		f;
	int			resblks = 0;
	unsigned int		flags = 0;
>>>>>>> upstream/android-13

	/*
	 * Lock the inodes against other IO, page faults and truncate to
	 * begin with.  Then we can ensure the inodes are flushed and have no
	 * page cache safely. Once we have done this we can take the ilocks and
	 * do the rest of the checks.
	 */
	lock_two_nondirectories(VFS_I(ip), VFS_I(tip));
<<<<<<< HEAD
	lock_flags = XFS_MMAPLOCK_EXCL;
	xfs_lock_two_inodes(ip, XFS_MMAPLOCK_EXCL, tip, XFS_MMAPLOCK_EXCL);
=======
	filemap_invalidate_lock_two(VFS_I(ip)->i_mapping,
				    VFS_I(tip)->i_mapping);
>>>>>>> upstream/android-13

	/* Verify that both files have the same format */
	if ((VFS_I(ip)->i_mode & S_IFMT) != (VFS_I(tip)->i_mode & S_IFMT)) {
		error = -EINVAL;
		goto out_unlock;
	}

	/* Verify both files are either real-time or non-realtime */
	if (XFS_IS_REALTIME_INODE(ip) != XFS_IS_REALTIME_INODE(tip)) {
		error = -EINVAL;
		goto out_unlock;
	}

<<<<<<< HEAD
=======
	error = xfs_qm_dqattach(ip);
	if (error)
		goto out_unlock;

	error = xfs_qm_dqattach(tip);
	if (error)
		goto out_unlock;

>>>>>>> upstream/android-13
	error = xfs_swap_extent_flush(ip);
	if (error)
		goto out_unlock;
	error = xfs_swap_extent_flush(tip);
	if (error)
		goto out_unlock;

	if (xfs_inode_has_cow_data(tip)) {
		error = xfs_reflink_cancel_cow_range(tip, 0, NULLFILEOFF, true);
		if (error)
			goto out_unlock;
	}

	/*
	 * Extent "swapping" with rmap requires a permanent reservation and
	 * a block reservation because it's really just a remap operation
	 * performed with log redo items!
	 */
<<<<<<< HEAD
	if (xfs_sb_version_hasrmapbt(&mp->m_sb)) {
		int		w	= XFS_DATA_FORK;
		uint32_t	ipnext	= XFS_IFORK_NEXTENTS(ip, w);
		uint32_t	tipnext	= XFS_IFORK_NEXTENTS(tip, w);
=======
	if (xfs_has_rmapbt(mp)) {
		int		w = XFS_DATA_FORK;
		uint32_t	ipnext = ip->i_df.if_nextents;
		uint32_t	tipnext	= tip->i_df.if_nextents;
>>>>>>> upstream/android-13

		/*
		 * Conceptually this shouldn't affect the shape of either bmbt,
		 * but since we atomically move extents one by one, we reserve
		 * enough space to rebuild both trees.
		 */
		resblks = XFS_SWAP_RMAP_SPACE_RES(mp, ipnext, w);
		resblks +=  XFS_SWAP_RMAP_SPACE_RES(mp, tipnext, w);

		/*
<<<<<<< HEAD
		 * Handle the corner case where either inode might straddle the
		 * btree format boundary. If so, the inode could bounce between
		 * btree <-> extent format on unmap -> remap cycles, freeing and
		 * allocating a bmapbt block each time.
		 */
		if (ipnext == (XFS_IFORK_MAXEXT(ip, w) + 1))
			resblks += XFS_IFORK_MAXEXT(ip, w);
		if (tipnext == (XFS_IFORK_MAXEXT(tip, w) + 1))
			resblks += XFS_IFORK_MAXEXT(tip, w);
	}
	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_write, resblks, 0, 0, &tp);
=======
		 * If either inode straddles a bmapbt block allocation boundary,
		 * the rmapbt algorithm triggers repeated allocs and frees as
		 * extents are remapped. This can exhaust the block reservation
		 * prematurely and cause shutdown. Return freed blocks to the
		 * transaction reservation to counter this behavior.
		 */
		flags |= XFS_TRANS_RES_FDBLKS;
	}
	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_write, resblks, 0, flags,
				&tp);
>>>>>>> upstream/android-13
	if (error)
		goto out_unlock;

	/*
	 * Lock and join the inodes to the tansaction so that transaction commit
	 * or cancel will unlock the inodes from this point onwards.
	 */
	xfs_lock_two_inodes(ip, XFS_ILOCK_EXCL, tip, XFS_ILOCK_EXCL);
<<<<<<< HEAD
	lock_flags |= XFS_ILOCK_EXCL;
=======
>>>>>>> upstream/android-13
	xfs_trans_ijoin(tp, ip, 0);
	xfs_trans_ijoin(tp, tip, 0);


	/* Verify all data are being swapped */
	if (sxp->sx_offset != 0 ||
<<<<<<< HEAD
	    sxp->sx_length != ip->i_d.di_size ||
	    sxp->sx_length != tip->i_d.di_size) {
=======
	    sxp->sx_length != ip->i_disk_size ||
	    sxp->sx_length != tip->i_disk_size) {
>>>>>>> upstream/android-13
		error = -EFAULT;
		goto out_trans_cancel;
	}

	trace_xfs_swap_extent_before(ip, 0);
	trace_xfs_swap_extent_before(tip, 1);

	/* check inode formats now that data is flushed */
	error = xfs_swap_extents_check_format(ip, tip);
	if (error) {
		xfs_notice(mp,
		    "%s: inode 0x%llx format is incompatible for exchanging.",
				__func__, ip->i_ino);
		goto out_trans_cancel;
	}

	/*
	 * Compare the current change & modify times with that
	 * passed in.  If they differ, we abort this swap.
	 * This is the mechanism used to ensure the calling
	 * process that the file was not changed out from
	 * under it.
	 */
	if ((sbp->bs_ctime.tv_sec != VFS_I(ip)->i_ctime.tv_sec) ||
	    (sbp->bs_ctime.tv_nsec != VFS_I(ip)->i_ctime.tv_nsec) ||
	    (sbp->bs_mtime.tv_sec != VFS_I(ip)->i_mtime.tv_sec) ||
	    (sbp->bs_mtime.tv_nsec != VFS_I(ip)->i_mtime.tv_nsec)) {
		error = -EBUSY;
		goto out_trans_cancel;
	}

	/*
	 * Note the trickiness in setting the log flags - we set the owner log
	 * flag on the opposite inode (i.e. the inode we are setting the new
	 * owner to be) because once we swap the forks and log that, log
	 * recovery is going to see the fork as owned by the swapped inode,
	 * not the pre-swapped inodes.
	 */
	src_log_flags = XFS_ILOG_CORE;
	target_log_flags = XFS_ILOG_CORE;

<<<<<<< HEAD
	if (xfs_sb_version_hasrmapbt(&mp->m_sb))
=======
	if (xfs_has_rmapbt(mp))
>>>>>>> upstream/android-13
		error = xfs_swap_extent_rmap(&tp, ip, tip);
	else
		error = xfs_swap_extent_forks(tp, ip, tip, &src_log_flags,
				&target_log_flags);
	if (error)
		goto out_trans_cancel;

	/* Do we have to swap reflink flags? */
<<<<<<< HEAD
	if ((ip->i_d.di_flags2 & XFS_DIFLAG2_REFLINK) ^
	    (tip->i_d.di_flags2 & XFS_DIFLAG2_REFLINK)) {
		f = ip->i_d.di_flags2 & XFS_DIFLAG2_REFLINK;
		ip->i_d.di_flags2 &= ~XFS_DIFLAG2_REFLINK;
		ip->i_d.di_flags2 |= tip->i_d.di_flags2 & XFS_DIFLAG2_REFLINK;
		tip->i_d.di_flags2 &= ~XFS_DIFLAG2_REFLINK;
		tip->i_d.di_flags2 |= f & XFS_DIFLAG2_REFLINK;
	}

	/* Swap the cow forks. */
	if (xfs_sb_version_hasreflink(&mp->m_sb)) {
		ASSERT(ip->i_cformat == XFS_DINODE_FMT_EXTENTS);
		ASSERT(tip->i_cformat == XFS_DINODE_FMT_EXTENTS);

		swap(ip->i_cnextents, tip->i_cnextents);
=======
	if ((ip->i_diflags2 & XFS_DIFLAG2_REFLINK) ^
	    (tip->i_diflags2 & XFS_DIFLAG2_REFLINK)) {
		f = ip->i_diflags2 & XFS_DIFLAG2_REFLINK;
		ip->i_diflags2 &= ~XFS_DIFLAG2_REFLINK;
		ip->i_diflags2 |= tip->i_diflags2 & XFS_DIFLAG2_REFLINK;
		tip->i_diflags2 &= ~XFS_DIFLAG2_REFLINK;
		tip->i_diflags2 |= f & XFS_DIFLAG2_REFLINK;
	}

	/* Swap the cow forks. */
	if (xfs_has_reflink(mp)) {
		ASSERT(!ip->i_cowfp ||
		       ip->i_cowfp->if_format == XFS_DINODE_FMT_EXTENTS);
		ASSERT(!tip->i_cowfp ||
		       tip->i_cowfp->if_format == XFS_DINODE_FMT_EXTENTS);

>>>>>>> upstream/android-13
		swap(ip->i_cowfp, tip->i_cowfp);

		if (ip->i_cowfp && ip->i_cowfp->if_bytes)
			xfs_inode_set_cowblocks_tag(ip);
		else
			xfs_inode_clear_cowblocks_tag(ip);
		if (tip->i_cowfp && tip->i_cowfp->if_bytes)
			xfs_inode_set_cowblocks_tag(tip);
		else
			xfs_inode_clear_cowblocks_tag(tip);
	}

	xfs_trans_log_inode(tp, ip,  src_log_flags);
	xfs_trans_log_inode(tp, tip, target_log_flags);

	/*
	 * The extent forks have been swapped, but crc=1,rmapbt=0 filesystems
	 * have inode number owner values in the bmbt blocks that still refer to
	 * the old inode. Scan each bmbt to fix up the owner values with the
	 * inode number of the current inode.
	 */
	if (src_log_flags & XFS_ILOG_DOWNER) {
		error = xfs_swap_change_owner(&tp, ip, tip);
		if (error)
			goto out_trans_cancel;
	}
	if (target_log_flags & XFS_ILOG_DOWNER) {
		error = xfs_swap_change_owner(&tp, tip, ip);
		if (error)
			goto out_trans_cancel;
	}

	/*
	 * If this is a synchronous mount, make sure that the
	 * transaction goes to disk before returning to the user.
	 */
<<<<<<< HEAD
	if (mp->m_flags & XFS_MOUNT_WSYNC)
=======
	if (xfs_has_wsync(mp))
>>>>>>> upstream/android-13
		xfs_trans_set_sync(tp);

	error = xfs_trans_commit(tp);

	trace_xfs_swap_extent_after(ip, 0);
	trace_xfs_swap_extent_after(tip, 1);

<<<<<<< HEAD
out_unlock:
	xfs_iunlock(ip, lock_flags);
	xfs_iunlock(tip, lock_flags);
=======
out_unlock_ilock:
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
	xfs_iunlock(tip, XFS_ILOCK_EXCL);
out_unlock:
	filemap_invalidate_unlock_two(VFS_I(ip)->i_mapping,
				      VFS_I(tip)->i_mapping);
>>>>>>> upstream/android-13
	unlock_two_nondirectories(VFS_I(ip), VFS_I(tip));
	return error;

out_trans_cancel:
	xfs_trans_cancel(tp);
<<<<<<< HEAD
	goto out_unlock;
=======
	goto out_unlock_ilock;
>>>>>>> upstream/android-13
}
