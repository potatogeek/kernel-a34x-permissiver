// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
 * Copyright (c) 2013 Red Hat, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include "xfs_fs.h"
#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_inode.h"
#include "xfs_quota.h"
#include "xfs_trans.h"
#include "xfs_qm.h"
#include "xfs_error.h"
<<<<<<< HEAD
#include "xfs_cksum.h"
#include "xfs_trace.h"
=======
>>>>>>> upstream/android-13

int
xfs_calc_dquots_per_chunk(
	unsigned int		nbblks)	/* basic block units */
{
	ASSERT(nbblks > 0);
	return BBTOB(nbblks) / sizeof(xfs_dqblk_t);
}

/*
 * Do some primitive error checking on ondisk dquot data structures.
 *
 * The xfs_dqblk structure /contains/ the xfs_disk_dquot structure;
 * we verify them separately because at some points we have only the
 * smaller xfs_disk_dquot structure available.
 */

xfs_failaddr_t
xfs_dquot_verify(
<<<<<<< HEAD
	struct xfs_mount *mp,
	xfs_disk_dquot_t *ddq,
	xfs_dqid_t	 id,
	uint		 type)	  /* used only during quotacheck */
{
=======
	struct xfs_mount	*mp,
	struct xfs_disk_dquot	*ddq,
	xfs_dqid_t		id)	/* used only during quotacheck */
{
	__u8			ddq_type;

>>>>>>> upstream/android-13
	/*
	 * We can encounter an uninitialized dquot buffer for 2 reasons:
	 * 1. If we crash while deleting the quotainode(s), and those blks got
	 *    used for user data. This is because we take the path of regular
	 *    file deletion; however, the size field of quotainodes is never
	 *    updated, so all the tricks that we play in itruncate_finish
	 *    don't quite matter.
	 *
	 * 2. We don't play the quota buffers when there's a quotaoff logitem.
	 *    But the allocation will be replayed so we'll end up with an
	 *    uninitialized quota block.
	 *
	 * This is all fine; things are still consistent, and we haven't lost
	 * any quota information. Just don't complain about bad dquot blks.
	 */
	if (ddq->d_magic != cpu_to_be16(XFS_DQUOT_MAGIC))
		return __this_address;
	if (ddq->d_version != XFS_DQUOT_VERSION)
		return __this_address;

<<<<<<< HEAD
	if (type && ddq->d_flags != type)
		return __this_address;
	if (ddq->d_flags != XFS_DQ_USER &&
	    ddq->d_flags != XFS_DQ_PROJ &&
	    ddq->d_flags != XFS_DQ_GROUP)
=======
	if (ddq->d_type & ~XFS_DQTYPE_ANY)
		return __this_address;
	ddq_type = ddq->d_type & XFS_DQTYPE_REC_MASK;
	if (ddq_type != XFS_DQTYPE_USER &&
	    ddq_type != XFS_DQTYPE_PROJ &&
	    ddq_type != XFS_DQTYPE_GROUP)
		return __this_address;

	if ((ddq->d_type & XFS_DQTYPE_BIGTIME) &&
	    !xfs_has_bigtime(mp))
		return __this_address;

	if ((ddq->d_type & XFS_DQTYPE_BIGTIME) && !ddq->d_id)
>>>>>>> upstream/android-13
		return __this_address;

	if (id != -1 && id != be32_to_cpu(ddq->d_id))
		return __this_address;

	if (!ddq->d_id)
		return NULL;

	if (ddq->d_blk_softlimit &&
	    be64_to_cpu(ddq->d_bcount) > be64_to_cpu(ddq->d_blk_softlimit) &&
	    !ddq->d_btimer)
		return __this_address;

	if (ddq->d_ino_softlimit &&
	    be64_to_cpu(ddq->d_icount) > be64_to_cpu(ddq->d_ino_softlimit) &&
	    !ddq->d_itimer)
		return __this_address;

	if (ddq->d_rtb_softlimit &&
	    be64_to_cpu(ddq->d_rtbcount) > be64_to_cpu(ddq->d_rtb_softlimit) &&
	    !ddq->d_rtbtimer)
		return __this_address;

	return NULL;
}

xfs_failaddr_t
xfs_dqblk_verify(
	struct xfs_mount	*mp,
	struct xfs_dqblk	*dqb,
<<<<<<< HEAD
	xfs_dqid_t	 	id,
	uint		 	type)	/* used only during quotacheck */
{
	if (xfs_sb_version_hascrc(&mp->m_sb) &&
	    !uuid_equal(&dqb->dd_uuid, &mp->m_sb.sb_meta_uuid))
		return __this_address;

	return xfs_dquot_verify(mp, &dqb->dd_diskdq, id, type);
=======
	xfs_dqid_t		id)	/* used only during quotacheck */
{
	if (xfs_has_crc(mp) &&
	    !uuid_equal(&dqb->dd_uuid, &mp->m_sb.sb_meta_uuid))
		return __this_address;

	return xfs_dquot_verify(mp, &dqb->dd_diskdq, id);
>>>>>>> upstream/android-13
}

/*
 * Do some primitive error checking on ondisk dquot data structures.
 */
<<<<<<< HEAD
int
=======
void
>>>>>>> upstream/android-13
xfs_dqblk_repair(
	struct xfs_mount	*mp,
	struct xfs_dqblk	*dqb,
	xfs_dqid_t		id,
<<<<<<< HEAD
	uint			type)
=======
	xfs_dqtype_t		type)
>>>>>>> upstream/android-13
{
	/*
	 * Typically, a repair is only requested by quotacheck.
	 */
	ASSERT(id != -1);
	memset(dqb, 0, sizeof(xfs_dqblk_t));

	dqb->dd_diskdq.d_magic = cpu_to_be16(XFS_DQUOT_MAGIC);
	dqb->dd_diskdq.d_version = XFS_DQUOT_VERSION;
<<<<<<< HEAD
	dqb->dd_diskdq.d_flags = type;
	dqb->dd_diskdq.d_id = cpu_to_be32(id);

	if (xfs_sb_version_hascrc(&mp->m_sb)) {
=======
	dqb->dd_diskdq.d_type = type;
	dqb->dd_diskdq.d_id = cpu_to_be32(id);

	if (xfs_has_crc(mp)) {
>>>>>>> upstream/android-13
		uuid_copy(&dqb->dd_uuid, &mp->m_sb.sb_meta_uuid);
		xfs_update_cksum((char *)dqb, sizeof(struct xfs_dqblk),
				 XFS_DQUOT_CRC_OFF);
	}
<<<<<<< HEAD

	return 0;
=======
>>>>>>> upstream/android-13
}

STATIC bool
xfs_dquot_buf_verify_crc(
	struct xfs_mount	*mp,
	struct xfs_buf		*bp,
	bool			readahead)
{
	struct xfs_dqblk	*d = (struct xfs_dqblk *)bp->b_addr;
	int			ndquots;
	int			i;

<<<<<<< HEAD
	if (!xfs_sb_version_hascrc(&mp->m_sb))
=======
	if (!xfs_has_crc(mp))
>>>>>>> upstream/android-13
		return true;

	/*
	 * if we are in log recovery, the quota subsystem has not been
	 * initialised so we have no quotainfo structure. In that case, we need
	 * to manually calculate the number of dquots in the buffer.
	 */
	if (mp->m_quotainfo)
		ndquots = mp->m_quotainfo->qi_dqperchunk;
	else
		ndquots = xfs_calc_dquots_per_chunk(bp->b_length);

	for (i = 0; i < ndquots; i++, d++) {
		if (!xfs_verify_cksum((char *)d, sizeof(struct xfs_dqblk),
				 XFS_DQUOT_CRC_OFF)) {
			if (!readahead)
				xfs_buf_verifier_error(bp, -EFSBADCRC, __func__,
					d, sizeof(*d), __this_address);
			return false;
		}
	}
	return true;
}

STATIC xfs_failaddr_t
xfs_dquot_buf_verify(
	struct xfs_mount	*mp,
	struct xfs_buf		*bp,
	bool			readahead)
{
	struct xfs_dqblk	*dqb = bp->b_addr;
	xfs_failaddr_t		fa;
	xfs_dqid_t		id = 0;
	int			ndquots;
	int			i;

	/*
	 * if we are in log recovery, the quota subsystem has not been
	 * initialised so we have no quotainfo structure. In that case, we need
	 * to manually calculate the number of dquots in the buffer.
	 */
	if (mp->m_quotainfo)
		ndquots = mp->m_quotainfo->qi_dqperchunk;
	else
		ndquots = xfs_calc_dquots_per_chunk(bp->b_length);

	/*
	 * On the first read of the buffer, verify that each dquot is valid.
	 * We don't know what the id of the dquot is supposed to be, just that
	 * they should be increasing monotonically within the buffer. If the
	 * first id is corrupt, then it will fail on the second dquot in the
	 * buffer so corruptions could point to the wrong dquot in this case.
	 */
	for (i = 0; i < ndquots; i++) {
		struct xfs_disk_dquot	*ddq;

		ddq = &dqb[i].dd_diskdq;

		if (i == 0)
			id = be32_to_cpu(ddq->d_id);

<<<<<<< HEAD
		fa = xfs_dqblk_verify(mp, &dqb[i], id + i, 0);
=======
		fa = xfs_dqblk_verify(mp, &dqb[i], id + i);
>>>>>>> upstream/android-13
		if (fa) {
			if (!readahead)
				xfs_buf_verifier_error(bp, -EFSCORRUPTED,
					__func__, &dqb[i],
					sizeof(struct xfs_dqblk), fa);
			return fa;
		}
	}

	return NULL;
}

static xfs_failaddr_t
xfs_dquot_buf_verify_struct(
	struct xfs_buf		*bp)
{
<<<<<<< HEAD
	struct xfs_mount	*mp = bp->b_target->bt_mount;
=======
	struct xfs_mount	*mp = bp->b_mount;
>>>>>>> upstream/android-13

	return xfs_dquot_buf_verify(mp, bp, false);
}

static void
xfs_dquot_buf_read_verify(
	struct xfs_buf		*bp)
{
<<<<<<< HEAD
	struct xfs_mount	*mp = bp->b_target->bt_mount;
=======
	struct xfs_mount	*mp = bp->b_mount;
>>>>>>> upstream/android-13

	if (!xfs_dquot_buf_verify_crc(mp, bp, false))
		return;
	xfs_dquot_buf_verify(mp, bp, false);
}

/*
 * readahead errors are silent and simply leave the buffer as !done so a real
 * read will then be run with the xfs_dquot_buf_ops verifier. See
 * xfs_inode_buf_verify() for why we use EIO and ~XBF_DONE here rather than
 * reporting the failure.
 */
static void
xfs_dquot_buf_readahead_verify(
	struct xfs_buf	*bp)
{
<<<<<<< HEAD
	struct xfs_mount	*mp = bp->b_target->bt_mount;
=======
	struct xfs_mount	*mp = bp->b_mount;
>>>>>>> upstream/android-13

	if (!xfs_dquot_buf_verify_crc(mp, bp, true) ||
	    xfs_dquot_buf_verify(mp, bp, true) != NULL) {
		xfs_buf_ioerror(bp, -EIO);
		bp->b_flags &= ~XBF_DONE;
	}
}

/*
 * we don't calculate the CRC here as that is done when the dquot is flushed to
 * the buffer after the update is done. This ensures that the dquot in the
 * buffer always has an up-to-date CRC value.
 */
static void
xfs_dquot_buf_write_verify(
	struct xfs_buf		*bp)
{
<<<<<<< HEAD
	struct xfs_mount	*mp = bp->b_target->bt_mount;
=======
	struct xfs_mount	*mp = bp->b_mount;
>>>>>>> upstream/android-13

	xfs_dquot_buf_verify(mp, bp, false);
}

const struct xfs_buf_ops xfs_dquot_buf_ops = {
	.name = "xfs_dquot",
<<<<<<< HEAD
=======
	.magic16 = { cpu_to_be16(XFS_DQUOT_MAGIC),
		     cpu_to_be16(XFS_DQUOT_MAGIC) },
>>>>>>> upstream/android-13
	.verify_read = xfs_dquot_buf_read_verify,
	.verify_write = xfs_dquot_buf_write_verify,
	.verify_struct = xfs_dquot_buf_verify_struct,
};

const struct xfs_buf_ops xfs_dquot_buf_ra_ops = {
	.name = "xfs_dquot_ra",
<<<<<<< HEAD
	.verify_read = xfs_dquot_buf_readahead_verify,
	.verify_write = xfs_dquot_buf_write_verify,
};
=======
	.magic16 = { cpu_to_be16(XFS_DQUOT_MAGIC),
		     cpu_to_be16(XFS_DQUOT_MAGIC) },
	.verify_read = xfs_dquot_buf_readahead_verify,
	.verify_write = xfs_dquot_buf_write_verify,
};

/* Convert an on-disk timer value into an incore timer value. */
time64_t
xfs_dquot_from_disk_ts(
	struct xfs_disk_dquot	*ddq,
	__be32			dtimer)
{
	uint32_t		t = be32_to_cpu(dtimer);

	if (t != 0 && (ddq->d_type & XFS_DQTYPE_BIGTIME))
		return xfs_dq_bigtime_to_unix(t);

	return t;
}

/* Convert an incore timer value into an on-disk timer value. */
__be32
xfs_dquot_to_disk_ts(
	struct xfs_dquot	*dqp,
	time64_t		timer)
{
	uint32_t		t = timer;

	if (timer != 0 && (dqp->q_type & XFS_DQTYPE_BIGTIME))
		t = xfs_dq_unix_to_bigtime(timer);

	return cpu_to_be32(t);
}
>>>>>>> upstream/android-13
