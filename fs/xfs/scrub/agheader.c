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
#include "xfs_alloc.h"
#include "xfs_ialloc.h"
#include "xfs_rmap.h"
#include "scrub/xfs_scrub.h"
#include "scrub/scrub.h"
#include "scrub/common.h"
#include "scrub/trace.h"
=======
#include "xfs_btree.h"
#include "xfs_sb.h"
#include "xfs_alloc.h"
#include "xfs_ialloc.h"
#include "xfs_rmap.h"
#include "xfs_ag.h"
#include "scrub/scrub.h"
#include "scrub/common.h"
>>>>>>> upstream/android-13

/* Superblock */

/* Cross-reference with the other btrees. */
STATIC void
xchk_superblock_xref(
	struct xfs_scrub	*sc,
	struct xfs_buf		*bp)
{
<<<<<<< HEAD
	struct xfs_owner_info	oinfo;
=======
>>>>>>> upstream/android-13
	struct xfs_mount	*mp = sc->mp;
	xfs_agnumber_t		agno = sc->sm->sm_agno;
	xfs_agblock_t		agbno;
	int			error;

	if (sc->sm->sm_flags & XFS_SCRUB_OFLAG_CORRUPT)
		return;

	agbno = XFS_SB_BLOCK(mp);

<<<<<<< HEAD
	error = xchk_ag_init(sc, agno, &sc->sa);
=======
	error = xchk_ag_init_existing(sc, agno, &sc->sa);
>>>>>>> upstream/android-13
	if (!xchk_xref_process_error(sc, agno, agbno, &error))
		return;

	xchk_xref_is_used_space(sc, agbno, 1);
	xchk_xref_is_not_inode_chunk(sc, agbno, 1);
<<<<<<< HEAD
	xfs_rmap_ag_owner(&oinfo, XFS_RMAP_OWN_FS);
	xchk_xref_is_owned_by(sc, agbno, 1, &oinfo);
=======
	xchk_xref_is_owned_by(sc, agbno, 1, &XFS_RMAP_OINFO_FS);
>>>>>>> upstream/android-13
	xchk_xref_is_not_shared(sc, agbno, 1);

	/* scrub teardown will take care of sc->sa for us */
}

/*
 * Scrub the filesystem superblock.
 *
 * Note: We do /not/ attempt to check AG 0's superblock.  Mount is
 * responsible for validating all the geometry information in sb 0, so
 * if the filesystem is capable of initiating online scrub, then clearly
 * sb 0 is ok and we can use its information to check everything else.
 */
int
xchk_superblock(
	struct xfs_scrub	*sc)
{
	struct xfs_mount	*mp = sc->mp;
	struct xfs_buf		*bp;
	struct xfs_dsb		*sb;
<<<<<<< HEAD
=======
	struct xfs_perag	*pag;
>>>>>>> upstream/android-13
	xfs_agnumber_t		agno;
	uint32_t		v2_ok;
	__be32			features_mask;
	int			error;
	__be16			vernum_mask;

	agno = sc->sm->sm_agno;
	if (agno == 0)
		return 0;

<<<<<<< HEAD
=======
	/*
	 * Grab an active reference to the perag structure.  If we can't get
	 * it, we're racing with something that's tearing down the AG, so
	 * signal that the AG no longer exists.
	 */
	pag = xfs_perag_get(mp, agno);
	if (!pag)
		return -ENOENT;

>>>>>>> upstream/android-13
	error = xfs_sb_read_secondary(mp, sc->tp, agno, &bp);
	/*
	 * The superblock verifier can return several different error codes
	 * if it thinks the superblock doesn't look right.  For a mount these
	 * would all get bounced back to userspace, but if we're here then the
	 * fs mounted successfully, which means that this secondary superblock
	 * is simply incorrect.  Treat all these codes the same way we treat
	 * any corruption.
	 */
	switch (error) {
	case -EINVAL:	/* also -EWRONGFS */
	case -ENOSYS:
	case -EFBIG:
		error = -EFSCORRUPTED;
<<<<<<< HEAD
=======
		fallthrough;
>>>>>>> upstream/android-13
	default:
		break;
	}
	if (!xchk_process_error(sc, agno, XFS_SB_BLOCK(mp), &error))
<<<<<<< HEAD
		return error;

	sb = XFS_BUF_TO_SBP(bp);
=======
		goto out_pag;

	sb = bp->b_addr;
>>>>>>> upstream/android-13

	/*
	 * Verify the geometries match.  Fields that are permanently
	 * set by mkfs are checked; fields that can be updated later
	 * (and are not propagated to backup superblocks) are preen
	 * checked.
	 */
	if (sb->sb_blocksize != cpu_to_be32(mp->m_sb.sb_blocksize))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_dblocks != cpu_to_be64(mp->m_sb.sb_dblocks))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_rblocks != cpu_to_be64(mp->m_sb.sb_rblocks))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_rextents != cpu_to_be64(mp->m_sb.sb_rextents))
		xchk_block_set_corrupt(sc, bp);

	if (!uuid_equal(&sb->sb_uuid, &mp->m_sb.sb_uuid))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_logstart != cpu_to_be64(mp->m_sb.sb_logstart))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_rootino != cpu_to_be64(mp->m_sb.sb_rootino))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_rbmino != cpu_to_be64(mp->m_sb.sb_rbmino))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_rsumino != cpu_to_be64(mp->m_sb.sb_rsumino))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_rextsize != cpu_to_be32(mp->m_sb.sb_rextsize))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_agblocks != cpu_to_be32(mp->m_sb.sb_agblocks))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_agcount != cpu_to_be32(mp->m_sb.sb_agcount))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_rbmblocks != cpu_to_be32(mp->m_sb.sb_rbmblocks))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_logblocks != cpu_to_be32(mp->m_sb.sb_logblocks))
		xchk_block_set_corrupt(sc, bp);

	/* Check sb_versionnum bits that are set at mkfs time. */
	vernum_mask = cpu_to_be16(~XFS_SB_VERSION_OKBITS |
				  XFS_SB_VERSION_NUMBITS |
				  XFS_SB_VERSION_ALIGNBIT |
				  XFS_SB_VERSION_DALIGNBIT |
				  XFS_SB_VERSION_SHAREDBIT |
				  XFS_SB_VERSION_LOGV2BIT |
				  XFS_SB_VERSION_SECTORBIT |
				  XFS_SB_VERSION_EXTFLGBIT |
				  XFS_SB_VERSION_DIRV2BIT);
	if ((sb->sb_versionnum & vernum_mask) !=
	    (cpu_to_be16(mp->m_sb.sb_versionnum) & vernum_mask))
		xchk_block_set_corrupt(sc, bp);

	/* Check sb_versionnum bits that can be set after mkfs time. */
	vernum_mask = cpu_to_be16(XFS_SB_VERSION_ATTRBIT |
				  XFS_SB_VERSION_NLINKBIT |
				  XFS_SB_VERSION_QUOTABIT);
	if ((sb->sb_versionnum & vernum_mask) !=
	    (cpu_to_be16(mp->m_sb.sb_versionnum) & vernum_mask))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_sectsize != cpu_to_be16(mp->m_sb.sb_sectsize))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_inodesize != cpu_to_be16(mp->m_sb.sb_inodesize))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_inopblock != cpu_to_be16(mp->m_sb.sb_inopblock))
		xchk_block_set_corrupt(sc, bp);

	if (memcmp(sb->sb_fname, mp->m_sb.sb_fname, sizeof(sb->sb_fname)))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_blocklog != mp->m_sb.sb_blocklog)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_sectlog != mp->m_sb.sb_sectlog)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_inodelog != mp->m_sb.sb_inodelog)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_inopblog != mp->m_sb.sb_inopblog)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_agblklog != mp->m_sb.sb_agblklog)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_rextslog != mp->m_sb.sb_rextslog)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_imax_pct != mp->m_sb.sb_imax_pct)
		xchk_block_set_preen(sc, bp);

	/*
	 * Skip the summary counters since we track them in memory anyway.
	 * sb_icount, sb_ifree, sb_fdblocks, sb_frexents
	 */

	if (sb->sb_uquotino != cpu_to_be64(mp->m_sb.sb_uquotino))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_gquotino != cpu_to_be64(mp->m_sb.sb_gquotino))
		xchk_block_set_preen(sc, bp);

	/*
	 * Skip the quota flags since repair will force quotacheck.
	 * sb_qflags
	 */

	if (sb->sb_flags != mp->m_sb.sb_flags)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_shared_vn != mp->m_sb.sb_shared_vn)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_inoalignmt != cpu_to_be32(mp->m_sb.sb_inoalignmt))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_unit != cpu_to_be32(mp->m_sb.sb_unit))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_width != cpu_to_be32(mp->m_sb.sb_width))
		xchk_block_set_preen(sc, bp);

	if (sb->sb_dirblklog != mp->m_sb.sb_dirblklog)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_logsectlog != mp->m_sb.sb_logsectlog)
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_logsectsize != cpu_to_be16(mp->m_sb.sb_logsectsize))
		xchk_block_set_corrupt(sc, bp);

	if (sb->sb_logsunit != cpu_to_be32(mp->m_sb.sb_logsunit))
		xchk_block_set_corrupt(sc, bp);

	/* Do we see any invalid bits in sb_features2? */
	if (!xfs_sb_version_hasmorebits(&mp->m_sb)) {
		if (sb->sb_features2 != 0)
			xchk_block_set_corrupt(sc, bp);
	} else {
		v2_ok = XFS_SB_VERSION2_OKBITS;
<<<<<<< HEAD
		if (XFS_SB_VERSION_NUM(&mp->m_sb) >= XFS_SB_VERSION_5)
=======
		if (xfs_sb_is_v5(&mp->m_sb))
>>>>>>> upstream/android-13
			v2_ok |= XFS_SB_VERSION2_CRCBIT;

		if (!!(sb->sb_features2 & cpu_to_be32(~v2_ok)))
			xchk_block_set_corrupt(sc, bp);

		if (sb->sb_features2 != sb->sb_bad_features2)
			xchk_block_set_preen(sc, bp);
	}

	/* Check sb_features2 flags that are set at mkfs time. */
	features_mask = cpu_to_be32(XFS_SB_VERSION2_LAZYSBCOUNTBIT |
				    XFS_SB_VERSION2_PROJID32BIT |
				    XFS_SB_VERSION2_CRCBIT |
				    XFS_SB_VERSION2_FTYPE);
	if ((sb->sb_features2 & features_mask) !=
	    (cpu_to_be32(mp->m_sb.sb_features2) & features_mask))
		xchk_block_set_corrupt(sc, bp);

	/* Check sb_features2 flags that can be set after mkfs time. */
	features_mask = cpu_to_be32(XFS_SB_VERSION2_ATTR2BIT);
	if ((sb->sb_features2 & features_mask) !=
	    (cpu_to_be32(mp->m_sb.sb_features2) & features_mask))
		xchk_block_set_corrupt(sc, bp);

<<<<<<< HEAD
	if (!xfs_sb_version_hascrc(&mp->m_sb)) {
=======
	if (!xfs_has_crc(mp)) {
>>>>>>> upstream/android-13
		/* all v5 fields must be zero */
		if (memchr_inv(&sb->sb_features_compat, 0,
				sizeof(struct xfs_dsb) -
				offsetof(struct xfs_dsb, sb_features_compat)))
			xchk_block_set_corrupt(sc, bp);
	} else {
		/* Check compat flags; all are set at mkfs time. */
		features_mask = cpu_to_be32(XFS_SB_FEAT_COMPAT_UNKNOWN);
		if ((sb->sb_features_compat & features_mask) !=
		    (cpu_to_be32(mp->m_sb.sb_features_compat) & features_mask))
			xchk_block_set_corrupt(sc, bp);

		/* Check ro compat flags; all are set at mkfs time. */
		features_mask = cpu_to_be32(XFS_SB_FEAT_RO_COMPAT_UNKNOWN |
					    XFS_SB_FEAT_RO_COMPAT_FINOBT |
					    XFS_SB_FEAT_RO_COMPAT_RMAPBT |
					    XFS_SB_FEAT_RO_COMPAT_REFLINK);
		if ((sb->sb_features_ro_compat & features_mask) !=
		    (cpu_to_be32(mp->m_sb.sb_features_ro_compat) &
		     features_mask))
			xchk_block_set_corrupt(sc, bp);

		/* Check incompat flags; all are set at mkfs time. */
		features_mask = cpu_to_be32(XFS_SB_FEAT_INCOMPAT_UNKNOWN |
					    XFS_SB_FEAT_INCOMPAT_FTYPE |
					    XFS_SB_FEAT_INCOMPAT_SPINODES |
					    XFS_SB_FEAT_INCOMPAT_META_UUID);
		if ((sb->sb_features_incompat & features_mask) !=
		    (cpu_to_be32(mp->m_sb.sb_features_incompat) &
		     features_mask))
			xchk_block_set_corrupt(sc, bp);

		/* Check log incompat flags; all are set at mkfs time. */
		features_mask = cpu_to_be32(XFS_SB_FEAT_INCOMPAT_LOG_UNKNOWN);
		if ((sb->sb_features_log_incompat & features_mask) !=
		    (cpu_to_be32(mp->m_sb.sb_features_log_incompat) &
		     features_mask))
			xchk_block_set_corrupt(sc, bp);

		/* Don't care about sb_crc */

		if (sb->sb_spino_align != cpu_to_be32(mp->m_sb.sb_spino_align))
			xchk_block_set_corrupt(sc, bp);

		if (sb->sb_pquotino != cpu_to_be64(mp->m_sb.sb_pquotino))
			xchk_block_set_preen(sc, bp);

		/* Don't care about sb_lsn */
	}

<<<<<<< HEAD
	if (xfs_sb_version_hasmetauuid(&mp->m_sb)) {
=======
	if (xfs_has_metauuid(mp)) {
>>>>>>> upstream/android-13
		/* The metadata UUID must be the same for all supers */
		if (!uuid_equal(&sb->sb_meta_uuid, &mp->m_sb.sb_meta_uuid))
			xchk_block_set_corrupt(sc, bp);
	}

	/* Everything else must be zero. */
	if (memchr_inv(sb + 1, 0,
			BBTOB(bp->b_length) - sizeof(struct xfs_dsb)))
		xchk_block_set_corrupt(sc, bp);

	xchk_superblock_xref(sc, bp);
<<<<<<< HEAD

=======
out_pag:
	xfs_perag_put(pag);
>>>>>>> upstream/android-13
	return error;
}

/* AGF */

/* Tally freespace record lengths. */
STATIC int
xchk_agf_record_bno_lengths(
	struct xfs_btree_cur		*cur,
<<<<<<< HEAD
	struct xfs_alloc_rec_incore	*rec,
=======
	const struct xfs_alloc_rec_incore *rec,
>>>>>>> upstream/android-13
	void				*priv)
{
	xfs_extlen_t			*blocks = priv;

	(*blocks) += rec->ar_blockcount;
	return 0;
}

/* Check agf_freeblks */
static inline void
xchk_agf_xref_freeblks(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(sc->sa.agf_bp);
=======
	struct xfs_agf		*agf = sc->sa.agf_bp->b_addr;
>>>>>>> upstream/android-13
	xfs_extlen_t		blocks = 0;
	int			error;

	if (!sc->sa.bno_cur)
		return;

	error = xfs_alloc_query_all(sc->sa.bno_cur,
			xchk_agf_record_bno_lengths, &blocks);
	if (!xchk_should_check_xref(sc, &error, &sc->sa.bno_cur))
		return;
	if (blocks != be32_to_cpu(agf->agf_freeblks))
		xchk_block_xref_set_corrupt(sc, sc->sa.agf_bp);
}

/* Cross reference the AGF with the cntbt (freespace by length btree) */
static inline void
xchk_agf_xref_cntbt(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(sc->sa.agf_bp);
=======
	struct xfs_agf		*agf = sc->sa.agf_bp->b_addr;
>>>>>>> upstream/android-13
	xfs_agblock_t		agbno;
	xfs_extlen_t		blocks;
	int			have;
	int			error;

	if (!sc->sa.cnt_cur)
		return;

	/* Any freespace at all? */
	error = xfs_alloc_lookup_le(sc->sa.cnt_cur, 0, -1U, &have);
	if (!xchk_should_check_xref(sc, &error, &sc->sa.cnt_cur))
		return;
	if (!have) {
<<<<<<< HEAD
		if (agf->agf_freeblks != be32_to_cpu(0))
=======
		if (agf->agf_freeblks != cpu_to_be32(0))
>>>>>>> upstream/android-13
			xchk_block_xref_set_corrupt(sc, sc->sa.agf_bp);
		return;
	}

	/* Check agf_longest */
	error = xfs_alloc_get_rec(sc->sa.cnt_cur, &agbno, &blocks, &have);
	if (!xchk_should_check_xref(sc, &error, &sc->sa.cnt_cur))
		return;
	if (!have || blocks != be32_to_cpu(agf->agf_longest))
		xchk_block_xref_set_corrupt(sc, sc->sa.agf_bp);
}

/* Check the btree block counts in the AGF against the btrees. */
STATIC void
xchk_agf_xref_btreeblks(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(sc->sa.agf_bp);
=======
	struct xfs_agf		*agf = sc->sa.agf_bp->b_addr;
>>>>>>> upstream/android-13
	struct xfs_mount	*mp = sc->mp;
	xfs_agblock_t		blocks;
	xfs_agblock_t		btreeblks;
	int			error;

<<<<<<< HEAD
=======
	/* agf_btreeblks didn't exist before lazysbcount */
	if (!xfs_has_lazysbcount(sc->mp))
		return;

>>>>>>> upstream/android-13
	/* Check agf_rmap_blocks; set up for agf_btreeblks check */
	if (sc->sa.rmap_cur) {
		error = xfs_btree_count_blocks(sc->sa.rmap_cur, &blocks);
		if (!xchk_should_check_xref(sc, &error, &sc->sa.rmap_cur))
			return;
		btreeblks = blocks - 1;
		if (blocks != be32_to_cpu(agf->agf_rmap_blocks))
			xchk_block_xref_set_corrupt(sc, sc->sa.agf_bp);
	} else {
		btreeblks = 0;
	}

	/*
	 * No rmap cursor; we can't xref if we have the rmapbt feature.
	 * We also can't do it if we're missing the free space btree cursors.
	 */
<<<<<<< HEAD
	if ((xfs_sb_version_hasrmapbt(&mp->m_sb) && !sc->sa.rmap_cur) ||
=======
	if ((xfs_has_rmapbt(mp) && !sc->sa.rmap_cur) ||
>>>>>>> upstream/android-13
	    !sc->sa.bno_cur || !sc->sa.cnt_cur)
		return;

	/* Check agf_btreeblks */
	error = xfs_btree_count_blocks(sc->sa.bno_cur, &blocks);
	if (!xchk_should_check_xref(sc, &error, &sc->sa.bno_cur))
		return;
	btreeblks += blocks - 1;

	error = xfs_btree_count_blocks(sc->sa.cnt_cur, &blocks);
	if (!xchk_should_check_xref(sc, &error, &sc->sa.cnt_cur))
		return;
	btreeblks += blocks - 1;

	if (btreeblks != be32_to_cpu(agf->agf_btreeblks))
		xchk_block_xref_set_corrupt(sc, sc->sa.agf_bp);
}

/* Check agf_refcount_blocks against tree size */
static inline void
xchk_agf_xref_refcblks(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	struct xfs_agf		*agf = XFS_BUF_TO_AGF(sc->sa.agf_bp);
=======
	struct xfs_agf		*agf = sc->sa.agf_bp->b_addr;
>>>>>>> upstream/android-13
	xfs_agblock_t		blocks;
	int			error;

	if (!sc->sa.refc_cur)
		return;

	error = xfs_btree_count_blocks(sc->sa.refc_cur, &blocks);
	if (!xchk_should_check_xref(sc, &error, &sc->sa.refc_cur))
		return;
	if (blocks != be32_to_cpu(agf->agf_refcount_blocks))
		xchk_block_xref_set_corrupt(sc, sc->sa.agf_bp);
}

/* Cross-reference with the other btrees. */
STATIC void
xchk_agf_xref(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	struct xfs_owner_info	oinfo;
	struct xfs_mount	*mp = sc->mp;
	xfs_agblock_t		agbno;
	int			error;
=======
	struct xfs_mount	*mp = sc->mp;
	xfs_agblock_t		agbno;
>>>>>>> upstream/android-13

	if (sc->sm->sm_flags & XFS_SCRUB_OFLAG_CORRUPT)
		return;

	agbno = XFS_AGF_BLOCK(mp);

<<<<<<< HEAD
	error = xchk_ag_btcur_init(sc, &sc->sa);
	if (error)
		return;
=======
	xchk_ag_btcur_init(sc, &sc->sa);
>>>>>>> upstream/android-13

	xchk_xref_is_used_space(sc, agbno, 1);
	xchk_agf_xref_freeblks(sc);
	xchk_agf_xref_cntbt(sc);
	xchk_xref_is_not_inode_chunk(sc, agbno, 1);
<<<<<<< HEAD
	xfs_rmap_ag_owner(&oinfo, XFS_RMAP_OWN_FS);
	xchk_xref_is_owned_by(sc, agbno, 1, &oinfo);
=======
	xchk_xref_is_owned_by(sc, agbno, 1, &XFS_RMAP_OINFO_FS);
>>>>>>> upstream/android-13
	xchk_agf_xref_btreeblks(sc);
	xchk_xref_is_not_shared(sc, agbno, 1);
	xchk_agf_xref_refcblks(sc);

	/* scrub teardown will take care of sc->sa for us */
}

/* Scrub the AGF. */
int
xchk_agf(
	struct xfs_scrub	*sc)
{
	struct xfs_mount	*mp = sc->mp;
	struct xfs_agf		*agf;
<<<<<<< HEAD
	xfs_agnumber_t		agno;
=======
	struct xfs_perag	*pag;
	xfs_agnumber_t		agno = sc->sm->sm_agno;
>>>>>>> upstream/android-13
	xfs_agblock_t		agbno;
	xfs_agblock_t		eoag;
	xfs_agblock_t		agfl_first;
	xfs_agblock_t		agfl_last;
	xfs_agblock_t		agfl_count;
	xfs_agblock_t		fl_count;
	int			level;
	int			error = 0;

<<<<<<< HEAD
	agno = sc->sa.agno = sc->sm->sm_agno;
	error = xchk_ag_read_headers(sc, agno, &sc->sa.agi_bp,
			&sc->sa.agf_bp, &sc->sa.agfl_bp);
=======
	error = xchk_ag_read_headers(sc, agno, &sc->sa);
>>>>>>> upstream/android-13
	if (!xchk_process_error(sc, agno, XFS_AGF_BLOCK(sc->mp), &error))
		goto out;
	xchk_buffer_recheck(sc, sc->sa.agf_bp);

<<<<<<< HEAD
	agf = XFS_BUF_TO_AGF(sc->sa.agf_bp);
=======
	agf = sc->sa.agf_bp->b_addr;
	pag = sc->sa.pag;
>>>>>>> upstream/android-13

	/* Check the AG length */
	eoag = be32_to_cpu(agf->agf_length);
	if (eoag != xfs_ag_block_count(mp, agno))
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);

	/* Check the AGF btree roots and levels */
	agbno = be32_to_cpu(agf->agf_roots[XFS_BTNUM_BNO]);
	if (!xfs_verify_agbno(mp, agno, agbno))
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);

	agbno = be32_to_cpu(agf->agf_roots[XFS_BTNUM_CNT]);
	if (!xfs_verify_agbno(mp, agno, agbno))
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);

	level = be32_to_cpu(agf->agf_levels[XFS_BTNUM_BNO]);
	if (level <= 0 || level > XFS_BTREE_MAXLEVELS)
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);

	level = be32_to_cpu(agf->agf_levels[XFS_BTNUM_CNT]);
	if (level <= 0 || level > XFS_BTREE_MAXLEVELS)
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);

<<<<<<< HEAD
	if (xfs_sb_version_hasrmapbt(&mp->m_sb)) {
=======
	if (xfs_has_rmapbt(mp)) {
>>>>>>> upstream/android-13
		agbno = be32_to_cpu(agf->agf_roots[XFS_BTNUM_RMAP]);
		if (!xfs_verify_agbno(mp, agno, agbno))
			xchk_block_set_corrupt(sc, sc->sa.agf_bp);

		level = be32_to_cpu(agf->agf_levels[XFS_BTNUM_RMAP]);
		if (level <= 0 || level > XFS_BTREE_MAXLEVELS)
			xchk_block_set_corrupt(sc, sc->sa.agf_bp);
	}

<<<<<<< HEAD
	if (xfs_sb_version_hasreflink(&mp->m_sb)) {
=======
	if (xfs_has_reflink(mp)) {
>>>>>>> upstream/android-13
		agbno = be32_to_cpu(agf->agf_refcount_root);
		if (!xfs_verify_agbno(mp, agno, agbno))
			xchk_block_set_corrupt(sc, sc->sa.agf_bp);

		level = be32_to_cpu(agf->agf_refcount_level);
		if (level <= 0 || level > XFS_BTREE_MAXLEVELS)
			xchk_block_set_corrupt(sc, sc->sa.agf_bp);
	}

	/* Check the AGFL counters */
	agfl_first = be32_to_cpu(agf->agf_flfirst);
	agfl_last = be32_to_cpu(agf->agf_fllast);
	agfl_count = be32_to_cpu(agf->agf_flcount);
	if (agfl_last > agfl_first)
		fl_count = agfl_last - agfl_first + 1;
	else
		fl_count = xfs_agfl_size(mp) - agfl_first + agfl_last + 1;
	if (agfl_count != 0 && fl_count != agfl_count)
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);

<<<<<<< HEAD
=======
	/* Do the incore counters match? */
	if (pag->pagf_freeblks != be32_to_cpu(agf->agf_freeblks))
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);
	if (pag->pagf_flcount != be32_to_cpu(agf->agf_flcount))
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);
	if (xfs_has_lazysbcount(sc->mp) &&
	    pag->pagf_btreeblks != be32_to_cpu(agf->agf_btreeblks))
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);

>>>>>>> upstream/android-13
	xchk_agf_xref(sc);
out:
	return error;
}

/* AGFL */

struct xchk_agfl_info {
<<<<<<< HEAD
	struct xfs_owner_info	oinfo;
=======
>>>>>>> upstream/android-13
	unsigned int		sz_entries;
	unsigned int		nr_entries;
	xfs_agblock_t		*entries;
	struct xfs_scrub	*sc;
};

/* Cross-reference with the other btrees. */
STATIC void
xchk_agfl_block_xref(
	struct xfs_scrub	*sc,
<<<<<<< HEAD
	xfs_agblock_t		agbno,
	struct xfs_owner_info	*oinfo)
=======
	xfs_agblock_t		agbno)
>>>>>>> upstream/android-13
{
	if (sc->sm->sm_flags & XFS_SCRUB_OFLAG_CORRUPT)
		return;

	xchk_xref_is_used_space(sc, agbno, 1);
	xchk_xref_is_not_inode_chunk(sc, agbno, 1);
<<<<<<< HEAD
	xchk_xref_is_owned_by(sc, agbno, 1, oinfo);
=======
	xchk_xref_is_owned_by(sc, agbno, 1, &XFS_RMAP_OINFO_AG);
>>>>>>> upstream/android-13
	xchk_xref_is_not_shared(sc, agbno, 1);
}

/* Scrub an AGFL block. */
STATIC int
xchk_agfl_block(
	struct xfs_mount	*mp,
	xfs_agblock_t		agbno,
	void			*priv)
{
	struct xchk_agfl_info	*sai = priv;
	struct xfs_scrub	*sc = sai->sc;
<<<<<<< HEAD
	xfs_agnumber_t		agno = sc->sa.agno;
=======
	xfs_agnumber_t		agno = sc->sa.pag->pag_agno;
>>>>>>> upstream/android-13

	if (xfs_verify_agbno(mp, agno, agbno) &&
	    sai->nr_entries < sai->sz_entries)
		sai->entries[sai->nr_entries++] = agbno;
	else
		xchk_block_set_corrupt(sc, sc->sa.agfl_bp);

<<<<<<< HEAD
	xchk_agfl_block_xref(sc, agbno, priv);

	if (sc->sm->sm_flags & XFS_SCRUB_OFLAG_CORRUPT)
		return XFS_BTREE_QUERY_RANGE_ABORT;
=======
	xchk_agfl_block_xref(sc, agbno);

	if (sc->sm->sm_flags & XFS_SCRUB_OFLAG_CORRUPT)
		return -ECANCELED;
>>>>>>> upstream/android-13

	return 0;
}

static int
xchk_agblock_cmp(
	const void		*pa,
	const void		*pb)
{
	const xfs_agblock_t	*a = pa;
	const xfs_agblock_t	*b = pb;

	return (int)*a - (int)*b;
}

/* Cross-reference with the other btrees. */
STATIC void
xchk_agfl_xref(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	struct xfs_owner_info	oinfo;
	struct xfs_mount	*mp = sc->mp;
	xfs_agblock_t		agbno;
	int			error;
=======
	struct xfs_mount	*mp = sc->mp;
	xfs_agblock_t		agbno;
>>>>>>> upstream/android-13

	if (sc->sm->sm_flags & XFS_SCRUB_OFLAG_CORRUPT)
		return;

	agbno = XFS_AGFL_BLOCK(mp);

<<<<<<< HEAD
	error = xchk_ag_btcur_init(sc, &sc->sa);
	if (error)
		return;

	xchk_xref_is_used_space(sc, agbno, 1);
	xchk_xref_is_not_inode_chunk(sc, agbno, 1);
	xfs_rmap_ag_owner(&oinfo, XFS_RMAP_OWN_FS);
	xchk_xref_is_owned_by(sc, agbno, 1, &oinfo);
=======
	xchk_ag_btcur_init(sc, &sc->sa);

	xchk_xref_is_used_space(sc, agbno, 1);
	xchk_xref_is_not_inode_chunk(sc, agbno, 1);
	xchk_xref_is_owned_by(sc, agbno, 1, &XFS_RMAP_OINFO_FS);
>>>>>>> upstream/android-13
	xchk_xref_is_not_shared(sc, agbno, 1);

	/*
	 * Scrub teardown will take care of sc->sa for us.  Leave sc->sa
	 * active so that the agfl block xref can use it too.
	 */
}

/* Scrub the AGFL. */
int
xchk_agfl(
	struct xfs_scrub	*sc)
{
	struct xchk_agfl_info	sai;
	struct xfs_agf		*agf;
<<<<<<< HEAD
	xfs_agnumber_t		agno;
=======
	xfs_agnumber_t		agno = sc->sm->sm_agno;
>>>>>>> upstream/android-13
	unsigned int		agflcount;
	unsigned int		i;
	int			error;

<<<<<<< HEAD
	agno = sc->sa.agno = sc->sm->sm_agno;
	error = xchk_ag_read_headers(sc, agno, &sc->sa.agi_bp,
			&sc->sa.agf_bp, &sc->sa.agfl_bp);
=======
	error = xchk_ag_read_headers(sc, agno, &sc->sa);
>>>>>>> upstream/android-13
	if (!xchk_process_error(sc, agno, XFS_AGFL_BLOCK(sc->mp), &error))
		goto out;
	if (!sc->sa.agf_bp)
		return -EFSCORRUPTED;
	xchk_buffer_recheck(sc, sc->sa.agfl_bp);

	xchk_agfl_xref(sc);

	if (sc->sm->sm_flags & XFS_SCRUB_OFLAG_CORRUPT)
		goto out;

	/* Allocate buffer to ensure uniqueness of AGFL entries. */
<<<<<<< HEAD
	agf = XFS_BUF_TO_AGF(sc->sa.agf_bp);
=======
	agf = sc->sa.agf_bp->b_addr;
>>>>>>> upstream/android-13
	agflcount = be32_to_cpu(agf->agf_flcount);
	if (agflcount > xfs_agfl_size(sc->mp)) {
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);
		goto out;
	}
	memset(&sai, 0, sizeof(sai));
	sai.sc = sc;
	sai.sz_entries = agflcount;
	sai.entries = kmem_zalloc(sizeof(xfs_agblock_t) * agflcount,
			KM_MAYFAIL);
	if (!sai.entries) {
		error = -ENOMEM;
		goto out;
	}

	/* Check the blocks in the AGFL. */
<<<<<<< HEAD
	xfs_rmap_ag_owner(&sai.oinfo, XFS_RMAP_OWN_AG);
	error = xfs_agfl_walk(sc->mp, XFS_BUF_TO_AGF(sc->sa.agf_bp),
			sc->sa.agfl_bp, xchk_agfl_block, &sai);
	if (error == XFS_BTREE_QUERY_RANGE_ABORT) {
=======
	error = xfs_agfl_walk(sc->mp, sc->sa.agf_bp->b_addr,
			sc->sa.agfl_bp, xchk_agfl_block, &sai);
	if (error == -ECANCELED) {
>>>>>>> upstream/android-13
		error = 0;
		goto out_free;
	}
	if (error)
		goto out_free;

	if (agflcount != sai.nr_entries) {
		xchk_block_set_corrupt(sc, sc->sa.agf_bp);
		goto out_free;
	}

	/* Sort entries, check for duplicates. */
	sort(sai.entries, sai.nr_entries, sizeof(sai.entries[0]),
			xchk_agblock_cmp, NULL);
	for (i = 1; i < sai.nr_entries; i++) {
		if (sai.entries[i] == sai.entries[i - 1]) {
			xchk_block_set_corrupt(sc, sc->sa.agf_bp);
			break;
		}
	}

out_free:
	kmem_free(sai.entries);
out:
	return error;
}

/* AGI */

/* Check agi_count/agi_freecount */
static inline void
xchk_agi_xref_icounts(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	struct xfs_agi		*agi = XFS_BUF_TO_AGI(sc->sa.agi_bp);
=======
	struct xfs_agi		*agi = sc->sa.agi_bp->b_addr;
>>>>>>> upstream/android-13
	xfs_agino_t		icount;
	xfs_agino_t		freecount;
	int			error;

	if (!sc->sa.ino_cur)
		return;

	error = xfs_ialloc_count_inodes(sc->sa.ino_cur, &icount, &freecount);
	if (!xchk_should_check_xref(sc, &error, &sc->sa.ino_cur))
		return;
	if (be32_to_cpu(agi->agi_count) != icount ||
	    be32_to_cpu(agi->agi_freecount) != freecount)
		xchk_block_xref_set_corrupt(sc, sc->sa.agi_bp);
}

<<<<<<< HEAD
=======
/* Check agi_[fi]blocks against tree size */
static inline void
xchk_agi_xref_fiblocks(
	struct xfs_scrub	*sc)
{
	struct xfs_agi		*agi = sc->sa.agi_bp->b_addr;
	xfs_agblock_t		blocks;
	int			error = 0;

	if (!xfs_has_inobtcounts(sc->mp))
		return;

	if (sc->sa.ino_cur) {
		error = xfs_btree_count_blocks(sc->sa.ino_cur, &blocks);
		if (!xchk_should_check_xref(sc, &error, &sc->sa.ino_cur))
			return;
		if (blocks != be32_to_cpu(agi->agi_iblocks))
			xchk_block_xref_set_corrupt(sc, sc->sa.agi_bp);
	}

	if (sc->sa.fino_cur) {
		error = xfs_btree_count_blocks(sc->sa.fino_cur, &blocks);
		if (!xchk_should_check_xref(sc, &error, &sc->sa.fino_cur))
			return;
		if (blocks != be32_to_cpu(agi->agi_fblocks))
			xchk_block_xref_set_corrupt(sc, sc->sa.agi_bp);
	}
}

>>>>>>> upstream/android-13
/* Cross-reference with the other btrees. */
STATIC void
xchk_agi_xref(
	struct xfs_scrub	*sc)
{
<<<<<<< HEAD
	struct xfs_owner_info	oinfo;
	struct xfs_mount	*mp = sc->mp;
	xfs_agblock_t		agbno;
	int			error;
=======
	struct xfs_mount	*mp = sc->mp;
	xfs_agblock_t		agbno;
>>>>>>> upstream/android-13

	if (sc->sm->sm_flags & XFS_SCRUB_OFLAG_CORRUPT)
		return;

	agbno = XFS_AGI_BLOCK(mp);

<<<<<<< HEAD
	error = xchk_ag_btcur_init(sc, &sc->sa);
	if (error)
		return;
=======
	xchk_ag_btcur_init(sc, &sc->sa);
>>>>>>> upstream/android-13

	xchk_xref_is_used_space(sc, agbno, 1);
	xchk_xref_is_not_inode_chunk(sc, agbno, 1);
	xchk_agi_xref_icounts(sc);
<<<<<<< HEAD
	xfs_rmap_ag_owner(&oinfo, XFS_RMAP_OWN_FS);
	xchk_xref_is_owned_by(sc, agbno, 1, &oinfo);
	xchk_xref_is_not_shared(sc, agbno, 1);
=======
	xchk_xref_is_owned_by(sc, agbno, 1, &XFS_RMAP_OINFO_FS);
	xchk_xref_is_not_shared(sc, agbno, 1);
	xchk_agi_xref_fiblocks(sc);
>>>>>>> upstream/android-13

	/* scrub teardown will take care of sc->sa for us */
}

/* Scrub the AGI. */
int
xchk_agi(
	struct xfs_scrub	*sc)
{
	struct xfs_mount	*mp = sc->mp;
	struct xfs_agi		*agi;
<<<<<<< HEAD
	xfs_agnumber_t		agno;
=======
	struct xfs_perag	*pag;
	xfs_agnumber_t		agno = sc->sm->sm_agno;
>>>>>>> upstream/android-13
	xfs_agblock_t		agbno;
	xfs_agblock_t		eoag;
	xfs_agino_t		agino;
	xfs_agino_t		first_agino;
	xfs_agino_t		last_agino;
	xfs_agino_t		icount;
	int			i;
	int			level;
	int			error = 0;

<<<<<<< HEAD
	agno = sc->sa.agno = sc->sm->sm_agno;
	error = xchk_ag_read_headers(sc, agno, &sc->sa.agi_bp,
			&sc->sa.agf_bp, &sc->sa.agfl_bp);
=======
	error = xchk_ag_read_headers(sc, agno, &sc->sa);
>>>>>>> upstream/android-13
	if (!xchk_process_error(sc, agno, XFS_AGI_BLOCK(sc->mp), &error))
		goto out;
	xchk_buffer_recheck(sc, sc->sa.agi_bp);

<<<<<<< HEAD
	agi = XFS_BUF_TO_AGI(sc->sa.agi_bp);
=======
	agi = sc->sa.agi_bp->b_addr;
	pag = sc->sa.pag;
>>>>>>> upstream/android-13

	/* Check the AG length */
	eoag = be32_to_cpu(agi->agi_length);
	if (eoag != xfs_ag_block_count(mp, agno))
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

	/* Check btree roots and levels */
	agbno = be32_to_cpu(agi->agi_root);
	if (!xfs_verify_agbno(mp, agno, agbno))
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

	level = be32_to_cpu(agi->agi_level);
	if (level <= 0 || level > XFS_BTREE_MAXLEVELS)
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

<<<<<<< HEAD
	if (xfs_sb_version_hasfinobt(&mp->m_sb)) {
=======
	if (xfs_has_finobt(mp)) {
>>>>>>> upstream/android-13
		agbno = be32_to_cpu(agi->agi_free_root);
		if (!xfs_verify_agbno(mp, agno, agbno))
			xchk_block_set_corrupt(sc, sc->sa.agi_bp);

		level = be32_to_cpu(agi->agi_free_level);
		if (level <= 0 || level > XFS_BTREE_MAXLEVELS)
			xchk_block_set_corrupt(sc, sc->sa.agi_bp);
	}

	/* Check inode counters */
	xfs_agino_range(mp, agno, &first_agino, &last_agino);
	icount = be32_to_cpu(agi->agi_count);
	if (icount > last_agino - first_agino + 1 ||
	    icount < be32_to_cpu(agi->agi_freecount))
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

	/* Check inode pointers */
	agino = be32_to_cpu(agi->agi_newino);
<<<<<<< HEAD
	if (agino != NULLAGINO && !xfs_verify_agino(mp, agno, agino))
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

	agino = be32_to_cpu(agi->agi_dirino);
	if (agino != NULLAGINO && !xfs_verify_agino(mp, agno, agino))
=======
	if (!xfs_verify_agino_or_null(mp, agno, agino))
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

	agino = be32_to_cpu(agi->agi_dirino);
	if (!xfs_verify_agino_or_null(mp, agno, agino))
>>>>>>> upstream/android-13
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

	/* Check unlinked inode buckets */
	for (i = 0; i < XFS_AGI_UNLINKED_BUCKETS; i++) {
		agino = be32_to_cpu(agi->agi_unlinked[i]);
<<<<<<< HEAD
		if (agino == NULLAGINO)
			continue;
		if (!xfs_verify_agino(mp, agno, agino))
=======
		if (!xfs_verify_agino_or_null(mp, agno, agino))
>>>>>>> upstream/android-13
			xchk_block_set_corrupt(sc, sc->sa.agi_bp);
	}

	if (agi->agi_pad32 != cpu_to_be32(0))
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

<<<<<<< HEAD
=======
	/* Do the incore counters match? */
	if (pag->pagi_count != be32_to_cpu(agi->agi_count))
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);
	if (pag->pagi_freecount != be32_to_cpu(agi->agi_freecount))
		xchk_block_set_corrupt(sc, sc->sa.agi_bp);

>>>>>>> upstream/android-13
	xchk_agi_xref(sc);
out:
	return error;
}
