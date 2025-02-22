// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2002 Silicon Graphics, Inc.
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
<<<<<<< HEAD
#include "xfs_error.h"
=======
>>>>>>> upstream/android-13
#include "xfs_trans.h"
#include "xfs_trans_priv.h"
#include "xfs_quota.h"
#include "xfs_qm.h"
<<<<<<< HEAD
=======
#include "xfs_trace.h"
#include "xfs_error.h"
>>>>>>> upstream/android-13

STATIC void	xfs_trans_alloc_dqinfo(xfs_trans_t *);

/*
 * Add the locked dquot to the transaction.
 * The dquot must be locked, and it cannot be associated with any
 * transaction.
 */
void
xfs_trans_dqjoin(
<<<<<<< HEAD
	xfs_trans_t	*tp,
	xfs_dquot_t	*dqp)
{
	ASSERT(dqp->q_transp != tp);
=======
	struct xfs_trans	*tp,
	struct xfs_dquot	*dqp)
{
>>>>>>> upstream/android-13
	ASSERT(XFS_DQ_IS_LOCKED(dqp));
	ASSERT(dqp->q_logitem.qli_dquot == dqp);

	/*
	 * Get a log_item_desc to point at the new item.
	 */
	xfs_trans_add_item(tp, &dqp->q_logitem.qli_item);
<<<<<<< HEAD

	/*
	 * Initialize d_transp so we can later determine if this dquot is
	 * associated with this transaction.
	 */
	dqp->q_transp = tp;
}


=======
}

>>>>>>> upstream/android-13
/*
 * This is called to mark the dquot as needing
 * to be logged when the transaction is committed.  The dquot must
 * already be associated with the given transaction.
 * Note that it marks the entire transaction as dirty. In the ordinary
 * case, this gets called via xfs_trans_commit, after the transaction
 * is already dirty. However, there's nothing stop this from getting
 * called directly, as done by xfs_qm_scall_setqlim. Hence, the TRANS_DIRTY
 * flag.
 */
void
xfs_trans_log_dquot(
<<<<<<< HEAD
	xfs_trans_t	*tp,
	xfs_dquot_t	*dqp)
{
	ASSERT(dqp->q_transp == tp);
	ASSERT(XFS_DQ_IS_LOCKED(dqp));

=======
	struct xfs_trans	*tp,
	struct xfs_dquot	*dqp)
{
	ASSERT(XFS_DQ_IS_LOCKED(dqp));

	/* Upgrade the dquot to bigtime format if possible. */
	if (dqp->q_id != 0 &&
	    xfs_has_bigtime(tp->t_mountp) &&
	    !(dqp->q_type & XFS_DQTYPE_BIGTIME))
		dqp->q_type |= XFS_DQTYPE_BIGTIME;

>>>>>>> upstream/android-13
	tp->t_flags |= XFS_TRANS_DIRTY;
	set_bit(XFS_LI_DIRTY, &dqp->q_logitem.qli_item.li_flags);
}

/*
 * Carry forward whatever is left of the quota blk reservation to
 * the spanky new transaction
 */
void
xfs_trans_dup_dqinfo(
<<<<<<< HEAD
	xfs_trans_t	*otp,
	xfs_trans_t	*ntp)
{
	xfs_dqtrx_t	*oq, *nq;
	int		i, j;
	xfs_dqtrx_t	*oqa, *nqa;
	ulong		blk_res_used;
=======
	struct xfs_trans	*otp,
	struct xfs_trans	*ntp)
{
	struct xfs_dqtrx	*oq, *nq;
	int			i, j;
	struct xfs_dqtrx	*oqa, *nqa;
	uint64_t		blk_res_used;
>>>>>>> upstream/android-13

	if (!otp->t_dqinfo)
		return;

	xfs_trans_alloc_dqinfo(ntp);

<<<<<<< HEAD
	/*
	 * Because the quota blk reservation is carried forward,
	 * it is also necessary to carry forward the DQ_DIRTY flag.
	 */
	if (otp->t_flags & XFS_TRANS_DQ_DIRTY)
		ntp->t_flags |= XFS_TRANS_DQ_DIRTY;

=======
>>>>>>> upstream/android-13
	for (j = 0; j < XFS_QM_TRANS_DQTYPES; j++) {
		oqa = otp->t_dqinfo->dqs[j];
		nqa = ntp->t_dqinfo->dqs[j];
		for (i = 0; i < XFS_QM_TRANS_MAXDQS; i++) {
			blk_res_used = 0;

			if (oqa[i].qt_dquot == NULL)
				break;
			oq = &oqa[i];
			nq = &nqa[i];

			if (oq->qt_blk_res && oq->qt_bcount_delta > 0)
				blk_res_used = oq->qt_bcount_delta;

			nq->qt_dquot = oq->qt_dquot;
			nq->qt_bcount_delta = nq->qt_icount_delta = 0;
			nq->qt_rtbcount_delta = 0;

			/*
			 * Transfer whatever is left of the reservations.
			 */
			nq->qt_blk_res = oq->qt_blk_res - blk_res_used;
			oq->qt_blk_res = blk_res_used;

			nq->qt_rtblk_res = oq->qt_rtblk_res -
				oq->qt_rtblk_res_used;
			oq->qt_rtblk_res = oq->qt_rtblk_res_used;

			nq->qt_ino_res = oq->qt_ino_res - oq->qt_ino_res_used;
			oq->qt_ino_res = oq->qt_ino_res_used;

		}
	}
}

/*
 * Wrap around mod_dquot to account for both user and group quotas.
 */
void
xfs_trans_mod_dquot_byino(
	xfs_trans_t	*tp,
	xfs_inode_t	*ip,
	uint		field,
<<<<<<< HEAD
	long		delta)
{
	xfs_mount_t	*mp = tp->t_mountp;

	if (!XFS_IS_QUOTA_RUNNING(mp) ||
	    !XFS_IS_QUOTA_ON(mp) ||
	    xfs_is_quota_inode(&mp->m_sb, ip->i_ino))
		return;

	if (tp->t_dqinfo == NULL)
		xfs_trans_alloc_dqinfo(tp);

=======
	int64_t		delta)
{
	xfs_mount_t	*mp = tp->t_mountp;

	if (!XFS_IS_QUOTA_ON(mp) ||
	    xfs_is_quota_inode(&mp->m_sb, ip->i_ino))
		return;

>>>>>>> upstream/android-13
	if (XFS_IS_UQUOTA_ON(mp) && ip->i_udquot)
		(void) xfs_trans_mod_dquot(tp, ip->i_udquot, field, delta);
	if (XFS_IS_GQUOTA_ON(mp) && ip->i_gdquot)
		(void) xfs_trans_mod_dquot(tp, ip->i_gdquot, field, delta);
	if (XFS_IS_PQUOTA_ON(mp) && ip->i_pdquot)
		(void) xfs_trans_mod_dquot(tp, ip->i_pdquot, field, delta);
}

STATIC struct xfs_dqtrx *
xfs_trans_get_dqtrx(
	struct xfs_trans	*tp,
	struct xfs_dquot	*dqp)
{
	int			i;
	struct xfs_dqtrx	*qa;

<<<<<<< HEAD
	if (XFS_QM_ISUDQ(dqp))
		qa = tp->t_dqinfo->dqs[XFS_QM_TRANS_USR];
	else if (XFS_QM_ISGDQ(dqp))
		qa = tp->t_dqinfo->dqs[XFS_QM_TRANS_GRP];
	else if (XFS_QM_ISPDQ(dqp))
		qa = tp->t_dqinfo->dqs[XFS_QM_TRANS_PRJ];
	else
		return NULL;
=======
	switch (xfs_dquot_type(dqp)) {
	case XFS_DQTYPE_USER:
		qa = tp->t_dqinfo->dqs[XFS_QM_TRANS_USR];
		break;
	case XFS_DQTYPE_GROUP:
		qa = tp->t_dqinfo->dqs[XFS_QM_TRANS_GRP];
		break;
	case XFS_DQTYPE_PROJ:
		qa = tp->t_dqinfo->dqs[XFS_QM_TRANS_PRJ];
		break;
	default:
		return NULL;
	}
>>>>>>> upstream/android-13

	for (i = 0; i < XFS_QM_TRANS_MAXDQS; i++) {
		if (qa[i].qt_dquot == NULL ||
		    qa[i].qt_dquot == dqp)
			return &qa[i];
	}

	return NULL;
}

/*
 * Make the changes in the transaction structure.
 * The moral equivalent to xfs_trans_mod_sb().
 * We don't touch any fields in the dquot, so we don't care
 * if it's locked or not (most of the time it won't be).
 */
void
xfs_trans_mod_dquot(
<<<<<<< HEAD
	xfs_trans_t	*tp,
	xfs_dquot_t	*dqp,
	uint		field,
	long		delta)
{
	xfs_dqtrx_t	*qtrx;

	ASSERT(tp);
	ASSERT(XFS_IS_QUOTA_RUNNING(tp->t_mountp));
	qtrx = NULL;

=======
	struct xfs_trans	*tp,
	struct xfs_dquot	*dqp,
	uint			field,
	int64_t			delta)
{
	struct xfs_dqtrx	*qtrx;

	ASSERT(tp);
	ASSERT(XFS_IS_QUOTA_ON(tp->t_mountp));
	qtrx = NULL;

	if (!delta)
		return;

>>>>>>> upstream/android-13
	if (tp->t_dqinfo == NULL)
		xfs_trans_alloc_dqinfo(tp);
	/*
	 * Find either the first free slot or the slot that belongs
	 * to this dquot.
	 */
	qtrx = xfs_trans_get_dqtrx(tp, dqp);
	ASSERT(qtrx);
	if (qtrx->qt_dquot == NULL)
		qtrx->qt_dquot = dqp;

<<<<<<< HEAD
	switch (field) {

		/*
		 * regular disk blk reservation
		 */
	      case XFS_TRANS_DQ_RES_BLKS:
		qtrx->qt_blk_res += (ulong)delta;
		break;

		/*
		 * inode reservation
		 */
	      case XFS_TRANS_DQ_RES_INOS:
		qtrx->qt_ino_res += (ulong)delta;
		break;

		/*
		 * disk blocks used.
		 */
	      case XFS_TRANS_DQ_BCOUNT:
		qtrx->qt_bcount_delta += delta;
		break;

	      case XFS_TRANS_DQ_DELBCOUNT:
		qtrx->qt_delbcnt_delta += delta;
		break;

		/*
		 * Inode Count
		 */
	      case XFS_TRANS_DQ_ICOUNT:
		if (qtrx->qt_ino_res && delta > 0) {
			qtrx->qt_ino_res_used += (ulong)delta;
=======
	trace_xfs_trans_mod_dquot_before(qtrx);
	trace_xfs_trans_mod_dquot(tp, dqp, field, delta);

	switch (field) {
	/* regular disk blk reservation */
	case XFS_TRANS_DQ_RES_BLKS:
		qtrx->qt_blk_res += delta;
		break;

	/* inode reservation */
	case XFS_TRANS_DQ_RES_INOS:
		qtrx->qt_ino_res += delta;
		break;

	/* disk blocks used. */
	case XFS_TRANS_DQ_BCOUNT:
		qtrx->qt_bcount_delta += delta;
		break;

	case XFS_TRANS_DQ_DELBCOUNT:
		qtrx->qt_delbcnt_delta += delta;
		break;

	/* Inode Count */
	case XFS_TRANS_DQ_ICOUNT:
		if (qtrx->qt_ino_res && delta > 0) {
			qtrx->qt_ino_res_used += delta;
>>>>>>> upstream/android-13
			ASSERT(qtrx->qt_ino_res >= qtrx->qt_ino_res_used);
		}
		qtrx->qt_icount_delta += delta;
		break;

<<<<<<< HEAD
		/*
		 * rtblk reservation
		 */
	      case XFS_TRANS_DQ_RES_RTBLKS:
		qtrx->qt_rtblk_res += (ulong)delta;
		break;

		/*
		 * rtblk count
		 */
	      case XFS_TRANS_DQ_RTBCOUNT:
		if (qtrx->qt_rtblk_res && delta > 0) {
			qtrx->qt_rtblk_res_used += (ulong)delta;
=======
	/* rtblk reservation */
	case XFS_TRANS_DQ_RES_RTBLKS:
		qtrx->qt_rtblk_res += delta;
		break;

	/* rtblk count */
	case XFS_TRANS_DQ_RTBCOUNT:
		if (qtrx->qt_rtblk_res && delta > 0) {
			qtrx->qt_rtblk_res_used += delta;
>>>>>>> upstream/android-13
			ASSERT(qtrx->qt_rtblk_res >= qtrx->qt_rtblk_res_used);
		}
		qtrx->qt_rtbcount_delta += delta;
		break;

<<<<<<< HEAD
	      case XFS_TRANS_DQ_DELRTBCOUNT:
		qtrx->qt_delrtb_delta += delta;
		break;

	      default:
		ASSERT(0);
	}
	tp->t_flags |= XFS_TRANS_DQ_DIRTY;
=======
	case XFS_TRANS_DQ_DELRTBCOUNT:
		qtrx->qt_delrtb_delta += delta;
		break;

	default:
		ASSERT(0);
	}

	trace_xfs_trans_mod_dquot_after(qtrx);
>>>>>>> upstream/android-13
}


/*
 * Given an array of dqtrx structures, lock all the dquots associated and join
 * them to the transaction, provided they have been modified.  We know that the
 * highest number of dquots of one type - usr, grp and prj - involved in a
 * transaction is 3 so we don't need to make this very generic.
 */
STATIC void
xfs_trans_dqlockedjoin(
<<<<<<< HEAD
	xfs_trans_t	*tp,
	xfs_dqtrx_t	*q)
=======
	struct xfs_trans	*tp,
	struct xfs_dqtrx	*q)
>>>>>>> upstream/android-13
{
	ASSERT(q[0].qt_dquot != NULL);
	if (q[1].qt_dquot == NULL) {
		xfs_dqlock(q[0].qt_dquot);
		xfs_trans_dqjoin(tp, q[0].qt_dquot);
	} else {
		ASSERT(XFS_QM_TRANS_MAXDQS == 2);
		xfs_dqlock2(q[0].qt_dquot, q[1].qt_dquot);
		xfs_trans_dqjoin(tp, q[0].qt_dquot);
		xfs_trans_dqjoin(tp, q[1].qt_dquot);
	}
}

<<<<<<< HEAD
=======
/* Apply dqtrx changes to the quota reservation counters. */
static inline void
xfs_apply_quota_reservation_deltas(
	struct xfs_dquot_res	*res,
	uint64_t		reserved,
	int64_t			res_used,
	int64_t			count_delta)
{
	if (reserved != 0) {
		/*
		 * Subtle math here: If reserved > res_used (the normal case),
		 * we're simply subtracting the unused transaction quota
		 * reservation from the dquot reservation.
		 *
		 * If, however, res_used > reserved, then we have allocated
		 * more quota blocks than were reserved for the transaction.
		 * We must add that excess to the dquot reservation since it
		 * tracks (usage + resv) and by definition we didn't reserve
		 * that excess.
		 */
		res->reserved -= abs(reserved - res_used);
	} else if (count_delta != 0) {
		/*
		 * These blks were never reserved, either inside a transaction
		 * or outside one (in a delayed allocation). Also, this isn't
		 * always a negative number since we sometimes deliberately
		 * skip quota reservations.
		 */
		res->reserved += count_delta;
	}
}
>>>>>>> upstream/android-13

/*
 * Called by xfs_trans_commit() and similar in spirit to
 * xfs_trans_apply_sb_deltas().
 * Go thru all the dquots belonging to this transaction and modify the
 * INCORE dquot to reflect the actual usages.
 * Unreserve just the reservations done by this transaction.
 * dquot is still left locked at exit.
 */
void
xfs_trans_apply_dquot_deltas(
	struct xfs_trans	*tp)
{
	int			i, j;
	struct xfs_dquot	*dqp;
	struct xfs_dqtrx	*qtrx, *qa;
<<<<<<< HEAD
	struct xfs_disk_dquot	*d;
	long			totalbdelta;
	long			totalrtbdelta;

	if (!(tp->t_flags & XFS_TRANS_DQ_DIRTY))
=======
	int64_t			totalbdelta;
	int64_t			totalrtbdelta;

	if (!tp->t_dqinfo)
>>>>>>> upstream/android-13
		return;

	ASSERT(tp->t_dqinfo);
	for (j = 0; j < XFS_QM_TRANS_DQTYPES; j++) {
		qa = tp->t_dqinfo->dqs[j];
		if (qa[0].qt_dquot == NULL)
			continue;

		/*
		 * Lock all of the dquots and join them to the transaction.
		 */
		xfs_trans_dqlockedjoin(tp, qa);

		for (i = 0; i < XFS_QM_TRANS_MAXDQS; i++) {
<<<<<<< HEAD
=======
			uint64_t	blk_res_used;

>>>>>>> upstream/android-13
			qtrx = &qa[i];
			/*
			 * The array of dquots is filled
			 * sequentially, not sparsely.
			 */
			if ((dqp = qtrx->qt_dquot) == NULL)
				break;

			ASSERT(XFS_DQ_IS_LOCKED(dqp));
<<<<<<< HEAD
			ASSERT(dqp->q_transp == tp);
=======
>>>>>>> upstream/android-13

			/*
			 * adjust the actual number of blocks used
			 */
<<<<<<< HEAD
			d = &dqp->q_core;
=======
>>>>>>> upstream/android-13

			/*
			 * The issue here is - sometimes we don't make a blkquota
			 * reservation intentionally to be fair to users
			 * (when the amount is small). On the other hand,
			 * delayed allocs do make reservations, but that's
			 * outside of a transaction, so we have no
			 * idea how much was really reserved.
			 * So, here we've accumulated delayed allocation blks and
			 * non-delay blks. The assumption is that the
			 * delayed ones are always reserved (outside of a
			 * transaction), and the others may or may not have
			 * quota reservations.
			 */
			totalbdelta = qtrx->qt_bcount_delta +
				qtrx->qt_delbcnt_delta;
			totalrtbdelta = qtrx->qt_rtbcount_delta +
				qtrx->qt_delrtb_delta;
<<<<<<< HEAD
#ifdef DEBUG
			if (totalbdelta < 0)
				ASSERT(be64_to_cpu(d->d_bcount) >=
				       -totalbdelta);

			if (totalrtbdelta < 0)
				ASSERT(be64_to_cpu(d->d_rtbcount) >=
				       -totalrtbdelta);

			if (qtrx->qt_icount_delta < 0)
				ASSERT(be64_to_cpu(d->d_icount) >=
				       -qtrx->qt_icount_delta);
#endif
			if (totalbdelta)
				be64_add_cpu(&d->d_bcount, (xfs_qcnt_t)totalbdelta);

			if (qtrx->qt_icount_delta)
				be64_add_cpu(&d->d_icount, (xfs_qcnt_t)qtrx->qt_icount_delta);

			if (totalrtbdelta)
				be64_add_cpu(&d->d_rtbcount, (xfs_qcnt_t)totalrtbdelta);
=======

			if (totalbdelta != 0 || totalrtbdelta != 0 ||
			    qtrx->qt_icount_delta != 0) {
				trace_xfs_trans_apply_dquot_deltas_before(dqp);
				trace_xfs_trans_apply_dquot_deltas(qtrx);
			}

#ifdef DEBUG
			if (totalbdelta < 0)
				ASSERT(dqp->q_blk.count >= -totalbdelta);

			if (totalrtbdelta < 0)
				ASSERT(dqp->q_rtb.count >= -totalrtbdelta);

			if (qtrx->qt_icount_delta < 0)
				ASSERT(dqp->q_ino.count >= -qtrx->qt_icount_delta);
#endif
			if (totalbdelta)
				dqp->q_blk.count += totalbdelta;

			if (qtrx->qt_icount_delta)
				dqp->q_ino.count += qtrx->qt_icount_delta;

			if (totalrtbdelta)
				dqp->q_rtb.count += totalrtbdelta;

			if (totalbdelta != 0 || totalrtbdelta != 0 ||
			    qtrx->qt_icount_delta != 0)
				trace_xfs_trans_apply_dquot_deltas_after(dqp);
>>>>>>> upstream/android-13

			/*
			 * Get any default limits in use.
			 * Start/reset the timer(s) if needed.
			 */
<<<<<<< HEAD
			if (d->d_id) {
				xfs_qm_adjust_dqlimits(tp->t_mountp, dqp);
				xfs_qm_adjust_dqtimers(tp->t_mountp, d);
			}

			dqp->dq_flags |= XFS_DQ_DIRTY;
=======
			if (dqp->q_id) {
				xfs_qm_adjust_dqlimits(dqp);
				xfs_qm_adjust_dqtimers(dqp);
			}

			dqp->q_flags |= XFS_DQFLAG_DIRTY;
>>>>>>> upstream/android-13
			/*
			 * add this to the list of items to get logged
			 */
			xfs_trans_log_dquot(tp, dqp);
			/*
			 * Take off what's left of the original reservation.
			 * In case of delayed allocations, there's no
			 * reservation that a transaction structure knows of.
			 */
<<<<<<< HEAD
			if (qtrx->qt_blk_res != 0) {
				ulong blk_res_used = 0;

				if (qtrx->qt_bcount_delta > 0)
					blk_res_used = qtrx->qt_bcount_delta;

				if (qtrx->qt_blk_res != blk_res_used) {
					if (qtrx->qt_blk_res > blk_res_used)
						dqp->q_res_bcount -= (xfs_qcnt_t)
							(qtrx->qt_blk_res -
							 blk_res_used);
					else
						dqp->q_res_bcount -= (xfs_qcnt_t)
							(blk_res_used -
							 qtrx->qt_blk_res);
				}
			} else {
				/*
				 * These blks were never reserved, either inside
				 * a transaction or outside one (in a delayed
				 * allocation). Also, this isn't always a
				 * negative number since we sometimes
				 * deliberately skip quota reservations.
				 */
				if (qtrx->qt_bcount_delta) {
					dqp->q_res_bcount +=
					      (xfs_qcnt_t)qtrx->qt_bcount_delta;
				}
			}
			/*
			 * Adjust the RT reservation.
			 */
			if (qtrx->qt_rtblk_res != 0) {
				if (qtrx->qt_rtblk_res != qtrx->qt_rtblk_res_used) {
					if (qtrx->qt_rtblk_res >
					    qtrx->qt_rtblk_res_used)
					       dqp->q_res_rtbcount -= (xfs_qcnt_t)
						       (qtrx->qt_rtblk_res -
							qtrx->qt_rtblk_res_used);
					else
					       dqp->q_res_rtbcount -= (xfs_qcnt_t)
						       (qtrx->qt_rtblk_res_used -
							qtrx->qt_rtblk_res);
				}
			} else {
				if (qtrx->qt_rtbcount_delta)
					dqp->q_res_rtbcount +=
					    (xfs_qcnt_t)qtrx->qt_rtbcount_delta;
			}
=======
			blk_res_used = max_t(int64_t, 0, qtrx->qt_bcount_delta);
			xfs_apply_quota_reservation_deltas(&dqp->q_blk,
					qtrx->qt_blk_res, blk_res_used,
					qtrx->qt_bcount_delta);

			/*
			 * Adjust the RT reservation.
			 */
			xfs_apply_quota_reservation_deltas(&dqp->q_rtb,
					qtrx->qt_rtblk_res,
					qtrx->qt_rtblk_res_used,
					qtrx->qt_rtbcount_delta);
>>>>>>> upstream/android-13

			/*
			 * Adjust the inode reservation.
			 */
<<<<<<< HEAD
			if (qtrx->qt_ino_res != 0) {
				ASSERT(qtrx->qt_ino_res >=
				       qtrx->qt_ino_res_used);
				if (qtrx->qt_ino_res > qtrx->qt_ino_res_used)
					dqp->q_res_icount -= (xfs_qcnt_t)
						(qtrx->qt_ino_res -
						 qtrx->qt_ino_res_used);
			} else {
				if (qtrx->qt_icount_delta)
					dqp->q_res_icount +=
					    (xfs_qcnt_t)qtrx->qt_icount_delta;
			}

			ASSERT(dqp->q_res_bcount >=
				be64_to_cpu(dqp->q_core.d_bcount));
			ASSERT(dqp->q_res_icount >=
				be64_to_cpu(dqp->q_core.d_icount));
			ASSERT(dqp->q_res_rtbcount >=
				be64_to_cpu(dqp->q_core.d_rtbcount));
=======
			ASSERT(qtrx->qt_ino_res >= qtrx->qt_ino_res_used);
			xfs_apply_quota_reservation_deltas(&dqp->q_ino,
					qtrx->qt_ino_res,
					qtrx->qt_ino_res_used,
					qtrx->qt_icount_delta);

			ASSERT(dqp->q_blk.reserved >= dqp->q_blk.count);
			ASSERT(dqp->q_ino.reserved >= dqp->q_ino.count);
			ASSERT(dqp->q_rtb.reserved >= dqp->q_rtb.count);
>>>>>>> upstream/android-13
		}
	}
}

/*
 * Release the reservations, and adjust the dquots accordingly.
 * This is called only when the transaction is being aborted. If by
 * any chance we have done dquot modifications incore (ie. deltas) already,
 * we simply throw those away, since that's the expected behavior
 * when a transaction is curtailed without a commit.
 */
void
xfs_trans_unreserve_and_mod_dquots(
<<<<<<< HEAD
	xfs_trans_t		*tp)
{
	int			i, j;
	xfs_dquot_t		*dqp;
	xfs_dqtrx_t		*qtrx, *qa;
	bool                    locked;

	if (!tp->t_dqinfo || !(tp->t_flags & XFS_TRANS_DQ_DIRTY))
=======
	struct xfs_trans	*tp)
{
	int			i, j;
	struct xfs_dquot	*dqp;
	struct xfs_dqtrx	*qtrx, *qa;
	bool			locked;

	if (!tp->t_dqinfo)
>>>>>>> upstream/android-13
		return;

	for (j = 0; j < XFS_QM_TRANS_DQTYPES; j++) {
		qa = tp->t_dqinfo->dqs[j];

		for (i = 0; i < XFS_QM_TRANS_MAXDQS; i++) {
			qtrx = &qa[i];
			/*
			 * We assume that the array of dquots is filled
			 * sequentially, not sparsely.
			 */
			if ((dqp = qtrx->qt_dquot) == NULL)
				break;
			/*
			 * Unreserve the original reservation. We don't care
			 * about the number of blocks used field, or deltas.
			 * Also we don't bother to zero the fields.
			 */
			locked = false;
			if (qtrx->qt_blk_res) {
				xfs_dqlock(dqp);
				locked = true;
<<<<<<< HEAD
				dqp->q_res_bcount -=
=======
				dqp->q_blk.reserved -=
>>>>>>> upstream/android-13
					(xfs_qcnt_t)qtrx->qt_blk_res;
			}
			if (qtrx->qt_ino_res) {
				if (!locked) {
					xfs_dqlock(dqp);
					locked = true;
				}
<<<<<<< HEAD
				dqp->q_res_icount -=
=======
				dqp->q_ino.reserved -=
>>>>>>> upstream/android-13
					(xfs_qcnt_t)qtrx->qt_ino_res;
			}

			if (qtrx->qt_rtblk_res) {
				if (!locked) {
					xfs_dqlock(dqp);
					locked = true;
				}
<<<<<<< HEAD
				dqp->q_res_rtbcount -=
=======
				dqp->q_rtb.reserved -=
>>>>>>> upstream/android-13
					(xfs_qcnt_t)qtrx->qt_rtblk_res;
			}
			if (locked)
				xfs_dqunlock(dqp);

		}
	}
}

STATIC void
xfs_quota_warn(
	struct xfs_mount	*mp,
	struct xfs_dquot	*dqp,
	int			type)
{
<<<<<<< HEAD
	enum quota_type qtype;

	if (dqp->dq_flags & XFS_DQ_PROJ)
		qtype = PRJQUOTA;
	else if (dqp->dq_flags & XFS_DQ_USER)
		qtype = USRQUOTA;
	else
		qtype = GRPQUOTA;

	quota_send_warning(make_kqid(&init_user_ns, qtype,
				     be32_to_cpu(dqp->q_core.d_id)),
=======
	enum quota_type		qtype;

	switch (xfs_dquot_type(dqp)) {
	case XFS_DQTYPE_PROJ:
		qtype = PRJQUOTA;
		break;
	case XFS_DQTYPE_USER:
		qtype = USRQUOTA;
		break;
	case XFS_DQTYPE_GROUP:
		qtype = GRPQUOTA;
		break;
	default:
		return;
	}

	quota_send_warning(make_kqid(&init_user_ns, qtype, dqp->q_id),
>>>>>>> upstream/android-13
			   mp->m_super->s_dev, type);
}

/*
<<<<<<< HEAD
=======
 * Decide if we can make an additional reservation against a quota resource.
 * Returns an inode QUOTA_NL_ warning code and whether or not it's fatal.
 *
 * Note that we assume that the numeric difference between the inode and block
 * warning codes will always be 3 since it's userspace ABI now, and will never
 * decrease the quota reservation, so the *BELOW messages are irrelevant.
 */
static inline int
xfs_dqresv_check(
	struct xfs_dquot_res	*res,
	struct xfs_quota_limits	*qlim,
	int64_t			delta,
	bool			*fatal)
{
	xfs_qcnt_t		hardlimit = res->hardlimit;
	xfs_qcnt_t		softlimit = res->softlimit;
	xfs_qcnt_t		total_count = res->reserved + delta;

	BUILD_BUG_ON(QUOTA_NL_BHARDWARN     != QUOTA_NL_IHARDWARN + 3);
	BUILD_BUG_ON(QUOTA_NL_BSOFTLONGWARN != QUOTA_NL_ISOFTLONGWARN + 3);
	BUILD_BUG_ON(QUOTA_NL_BSOFTWARN     != QUOTA_NL_ISOFTWARN + 3);

	*fatal = false;
	if (delta <= 0)
		return QUOTA_NL_NOWARN;

	if (!hardlimit)
		hardlimit = qlim->hard;
	if (!softlimit)
		softlimit = qlim->soft;

	if (hardlimit && total_count > hardlimit) {
		*fatal = true;
		return QUOTA_NL_IHARDWARN;
	}

	if (softlimit && total_count > softlimit) {
		time64_t	now = ktime_get_real_seconds();

		if ((res->timer != 0 && now > res->timer) ||
		    (res->warnings != 0 && res->warnings >= qlim->warn)) {
			*fatal = true;
			return QUOTA_NL_ISOFTLONGWARN;
		}

		res->warnings++;
		return QUOTA_NL_ISOFTWARN;
	}

	return QUOTA_NL_NOWARN;
}

/*
>>>>>>> upstream/android-13
 * This reserves disk blocks and inodes against a dquot.
 * Flags indicate if the dquot is to be locked here and also
 * if the blk reservation is for RT or regular blocks.
 * Sending in XFS_QMOPT_FORCE_RES flag skips the quota check.
 */
STATIC int
xfs_trans_dqresv(
<<<<<<< HEAD
	xfs_trans_t	*tp,
	xfs_mount_t	*mp,
	xfs_dquot_t	*dqp,
	long		nblks,
	long		ninos,
	uint		flags)
{
	xfs_qcnt_t	hardlimit;
	xfs_qcnt_t	softlimit;
	time_t		timer;
	xfs_qwarncnt_t	warns;
	xfs_qwarncnt_t	warnlimit;
	xfs_qcnt_t	total_count;
	xfs_qcnt_t	*resbcountp;
	xfs_quotainfo_t	*q = mp->m_quotainfo;
	struct xfs_def_quota	*defq;


	xfs_dqlock(dqp);

	defq = xfs_get_defquota(dqp, q);

	if (flags & XFS_TRANS_DQ_RES_BLKS) {
		hardlimit = be64_to_cpu(dqp->q_core.d_blk_hardlimit);
		if (!hardlimit)
			hardlimit = defq->bhardlimit;
		softlimit = be64_to_cpu(dqp->q_core.d_blk_softlimit);
		if (!softlimit)
			softlimit = defq->bsoftlimit;
		timer = be32_to_cpu(dqp->q_core.d_btimer);
		warns = be16_to_cpu(dqp->q_core.d_bwarns);
		warnlimit = dqp->q_mount->m_quotainfo->qi_bwarnlimit;
		resbcountp = &dqp->q_res_bcount;
	} else {
		ASSERT(flags & XFS_TRANS_DQ_RES_RTBLKS);
		hardlimit = be64_to_cpu(dqp->q_core.d_rtb_hardlimit);
		if (!hardlimit)
			hardlimit = defq->rtbhardlimit;
		softlimit = be64_to_cpu(dqp->q_core.d_rtb_softlimit);
		if (!softlimit)
			softlimit = defq->rtbsoftlimit;
		timer = be32_to_cpu(dqp->q_core.d_rtbtimer);
		warns = be16_to_cpu(dqp->q_core.d_rtbwarns);
		warnlimit = dqp->q_mount->m_quotainfo->qi_rtbwarnlimit;
		resbcountp = &dqp->q_res_rtbcount;
	}

	if ((flags & XFS_QMOPT_FORCE_RES) == 0 &&
	    dqp->q_core.d_id &&
	    ((XFS_IS_UQUOTA_ENFORCED(dqp->q_mount) && XFS_QM_ISUDQ(dqp)) ||
	     (XFS_IS_GQUOTA_ENFORCED(dqp->q_mount) && XFS_QM_ISGDQ(dqp)) ||
	     (XFS_IS_PQUOTA_ENFORCED(dqp->q_mount) && XFS_QM_ISPDQ(dqp)))) {
		if (nblks > 0) {
			/*
			 * dquot is locked already. See if we'd go over the
			 * hardlimit or exceed the timelimit if we allocate
			 * nblks.
			 */
			total_count = *resbcountp + nblks;
			if (hardlimit && total_count > hardlimit) {
				xfs_quota_warn(mp, dqp, QUOTA_NL_BHARDWARN);
				goto error_return;
			}
			if (softlimit && total_count > softlimit) {
				if ((timer != 0 && get_seconds() > timer) ||
				    (warns != 0 && warns >= warnlimit)) {
					xfs_quota_warn(mp, dqp,
						       QUOTA_NL_BSOFTLONGWARN);
					goto error_return;
				}

				xfs_quota_warn(mp, dqp, QUOTA_NL_BSOFTWARN);
			}
		}
		if (ninos > 0) {
			total_count = dqp->q_res_icount + ninos;
			timer = be32_to_cpu(dqp->q_core.d_itimer);
			warns = be16_to_cpu(dqp->q_core.d_iwarns);
			warnlimit = dqp->q_mount->m_quotainfo->qi_iwarnlimit;
			hardlimit = be64_to_cpu(dqp->q_core.d_ino_hardlimit);
			if (!hardlimit)
				hardlimit = defq->ihardlimit;
			softlimit = be64_to_cpu(dqp->q_core.d_ino_softlimit);
			if (!softlimit)
				softlimit = defq->isoftlimit;

			if (hardlimit && total_count > hardlimit) {
				xfs_quota_warn(mp, dqp, QUOTA_NL_IHARDWARN);
				goto error_return;
			}
			if (softlimit && total_count > softlimit) {
				if  ((timer != 0 && get_seconds() > timer) ||
				     (warns != 0 && warns >= warnlimit)) {
					xfs_quota_warn(mp, dqp,
						       QUOTA_NL_ISOFTLONGWARN);
					goto error_return;
				}
				xfs_quota_warn(mp, dqp, QUOTA_NL_ISOFTWARN);
			}
=======
	struct xfs_trans	*tp,
	struct xfs_mount	*mp,
	struct xfs_dquot	*dqp,
	int64_t			nblks,
	long			ninos,
	uint			flags)
{
	struct xfs_quotainfo	*q = mp->m_quotainfo;
	struct xfs_def_quota	*defq;
	struct xfs_dquot_res	*blkres;
	struct xfs_quota_limits	*qlim;

	xfs_dqlock(dqp);

	defq = xfs_get_defquota(q, xfs_dquot_type(dqp));

	if (flags & XFS_TRANS_DQ_RES_BLKS) {
		blkres = &dqp->q_blk;
		qlim = &defq->blk;
	} else {
		blkres = &dqp->q_rtb;
		qlim = &defq->rtb;
	}

	if ((flags & XFS_QMOPT_FORCE_RES) == 0 && dqp->q_id &&
	    xfs_dquot_is_enforced(dqp)) {
		int		quota_nl;
		bool		fatal;

		/*
		 * dquot is locked already. See if we'd go over the hardlimit
		 * or exceed the timelimit if we'd reserve resources.
		 */
		quota_nl = xfs_dqresv_check(blkres, qlim, nblks, &fatal);
		if (quota_nl != QUOTA_NL_NOWARN) {
			/*
			 * Quota block warning codes are 3 more than the inode
			 * codes, which we check above.
			 */
			xfs_quota_warn(mp, dqp, quota_nl + 3);
			if (fatal)
				goto error_return;
		}

		quota_nl = xfs_dqresv_check(&dqp->q_ino, &defq->ino, ninos,
				&fatal);
		if (quota_nl != QUOTA_NL_NOWARN) {
			xfs_quota_warn(mp, dqp, quota_nl);
			if (fatal)
				goto error_return;
>>>>>>> upstream/android-13
		}
	}

	/*
	 * Change the reservation, but not the actual usage.
<<<<<<< HEAD
	 * Note that q_res_bcount = q_core.d_bcount + resv
	 */
	(*resbcountp) += (xfs_qcnt_t)nblks;
	if (ninos != 0)
		dqp->q_res_icount += (xfs_qcnt_t)ninos;
=======
	 * Note that q_blk.reserved = q_blk.count + resv
	 */
	blkres->reserved += (xfs_qcnt_t)nblks;
	dqp->q_ino.reserved += (xfs_qcnt_t)ninos;
>>>>>>> upstream/android-13

	/*
	 * note the reservation amt in the trans struct too,
	 * so that the transaction knows how much was reserved by
	 * it against this particular dquot.
	 * We don't do this when we are reserving for a delayed allocation,
	 * because we don't have the luxury of a transaction envelope then.
	 */
	if (tp) {
<<<<<<< HEAD
		ASSERT(tp->t_dqinfo);
		ASSERT(flags & XFS_QMOPT_RESBLK_MASK);
		if (nblks != 0)
			xfs_trans_mod_dquot(tp, dqp,
					    flags & XFS_QMOPT_RESBLK_MASK,
					    nblks);
		if (ninos != 0)
			xfs_trans_mod_dquot(tp, dqp,
					    XFS_TRANS_DQ_RES_INOS,
					    ninos);
	}
	ASSERT(dqp->q_res_bcount >= be64_to_cpu(dqp->q_core.d_bcount));
	ASSERT(dqp->q_res_rtbcount >= be64_to_cpu(dqp->q_core.d_rtbcount));
	ASSERT(dqp->q_res_icount >= be64_to_cpu(dqp->q_core.d_icount));
=======
		ASSERT(flags & XFS_QMOPT_RESBLK_MASK);
		xfs_trans_mod_dquot(tp, dqp, flags & XFS_QMOPT_RESBLK_MASK,
				    nblks);
		xfs_trans_mod_dquot(tp, dqp, XFS_TRANS_DQ_RES_INOS, ninos);
	}

	if (XFS_IS_CORRUPT(mp, dqp->q_blk.reserved < dqp->q_blk.count) ||
	    XFS_IS_CORRUPT(mp, dqp->q_rtb.reserved < dqp->q_rtb.count) ||
	    XFS_IS_CORRUPT(mp, dqp->q_ino.reserved < dqp->q_ino.count))
		goto error_corrupt;
>>>>>>> upstream/android-13

	xfs_dqunlock(dqp);
	return 0;

error_return:
	xfs_dqunlock(dqp);
<<<<<<< HEAD
	if (flags & XFS_QMOPT_ENOSPC)
		return -ENOSPC;
	return -EDQUOT;
=======
	if (xfs_dquot_type(dqp) == XFS_DQTYPE_PROJ)
		return -ENOSPC;
	return -EDQUOT;
error_corrupt:
	xfs_dqunlock(dqp);
	xfs_force_shutdown(mp, SHUTDOWN_CORRUPT_INCORE);
	return -EFSCORRUPTED;
>>>>>>> upstream/android-13
}


/*
 * Given dquot(s), make disk block and/or inode reservations against them.
 * The fact that this does the reservation against user, group and
 * project quotas is important, because this follows a all-or-nothing
 * approach.
 *
 * flags = XFS_QMOPT_FORCE_RES evades limit enforcement. Used by chown.
 *	   XFS_QMOPT_ENOSPC returns ENOSPC not EDQUOT.  Used by pquota.
 *	   XFS_TRANS_DQ_RES_BLKS reserves regular disk blocks
 *	   XFS_TRANS_DQ_RES_RTBLKS reserves realtime disk blocks
 * dquots are unlocked on return, if they were not locked by caller.
 */
int
xfs_trans_reserve_quota_bydquots(
	struct xfs_trans	*tp,
	struct xfs_mount	*mp,
	struct xfs_dquot	*udqp,
	struct xfs_dquot	*gdqp,
	struct xfs_dquot	*pdqp,
<<<<<<< HEAD
	long			nblks,
=======
	int64_t			nblks,
>>>>>>> upstream/android-13
	long			ninos,
	uint			flags)
{
	int		error;

<<<<<<< HEAD
	if (!XFS_IS_QUOTA_RUNNING(mp) || !XFS_IS_QUOTA_ON(mp))
		return 0;

	if (tp && tp->t_dqinfo == NULL)
		xfs_trans_alloc_dqinfo(tp);

	ASSERT(flags & XFS_QMOPT_RESBLK_MASK);

	if (udqp) {
		error = xfs_trans_dqresv(tp, mp, udqp, nblks, ninos,
					(flags & ~XFS_QMOPT_ENOSPC));
=======
	if (!XFS_IS_QUOTA_ON(mp))
		return 0;

	ASSERT(flags & XFS_QMOPT_RESBLK_MASK);

	if (udqp) {
		error = xfs_trans_dqresv(tp, mp, udqp, nblks, ninos, flags);
>>>>>>> upstream/android-13
		if (error)
			return error;
	}

	if (gdqp) {
		error = xfs_trans_dqresv(tp, mp, gdqp, nblks, ninos, flags);
		if (error)
			goto unwind_usr;
	}

	if (pdqp) {
		error = xfs_trans_dqresv(tp, mp, pdqp, nblks, ninos, flags);
		if (error)
			goto unwind_grp;
	}

	/*
	 * Didn't change anything critical, so, no need to log
	 */
	return 0;

unwind_grp:
	flags |= XFS_QMOPT_FORCE_RES;
	if (gdqp)
		xfs_trans_dqresv(tp, mp, gdqp, -nblks, -ninos, flags);
unwind_usr:
	flags |= XFS_QMOPT_FORCE_RES;
	if (udqp)
		xfs_trans_dqresv(tp, mp, udqp, -nblks, -ninos, flags);
	return error;
}


/*
 * Lock the dquot and change the reservation if we can.
 * This doesn't change the actual usage, just the reservation.
 * The inode sent in is locked.
 */
int
xfs_trans_reserve_quota_nblks(
	struct xfs_trans	*tp,
	struct xfs_inode	*ip,
<<<<<<< HEAD
	long			nblks,
	long			ninos,
	uint			flags)
{
	struct xfs_mount	*mp = ip->i_mount;

	if (!XFS_IS_QUOTA_RUNNING(mp) || !XFS_IS_QUOTA_ON(mp))
		return 0;
	if (XFS_IS_PQUOTA_ON(mp))
		flags |= XFS_QMOPT_ENOSPC;

	ASSERT(!xfs_is_quota_inode(&mp->m_sb, ip->i_ino));

	ASSERT(xfs_isilocked(ip, XFS_ILOCK_EXCL));
	ASSERT((flags & ~(XFS_QMOPT_FORCE_RES | XFS_QMOPT_ENOSPC)) ==
				XFS_TRANS_DQ_RES_RTBLKS ||
	       (flags & ~(XFS_QMOPT_FORCE_RES | XFS_QMOPT_ENOSPC)) ==
				XFS_TRANS_DQ_RES_BLKS);

	/*
	 * Reserve nblks against these dquots, with trans as the mediator.
	 */
	return xfs_trans_reserve_quota_bydquots(tp, mp,
						ip->i_udquot, ip->i_gdquot,
						ip->i_pdquot,
						nblks, ninos, flags);
}

/*
 * This routine is called to allocate a quotaoff log item.
 */
xfs_qoff_logitem_t *
xfs_trans_get_qoff_item(
	xfs_trans_t		*tp,
	xfs_qoff_logitem_t	*startqoff,
	uint			flags)
{
	xfs_qoff_logitem_t	*q;

	ASSERT(tp != NULL);

	q = xfs_qm_qoff_logitem_init(tp->t_mountp, startqoff, flags);
	ASSERT(q != NULL);

	/*
	 * Get a log_item_desc to point at the new item.
	 */
	xfs_trans_add_item(tp, &q->qql_item);
	return q;
}


/*
 * This is called to mark the quotaoff logitem as needing
 * to be logged when the transaction is committed.  The logitem must
 * already be associated with the given transaction.
 */
void
xfs_trans_log_quotaoff_item(
	xfs_trans_t		*tp,
	xfs_qoff_logitem_t	*qlp)
{
	tp->t_flags |= XFS_TRANS_DIRTY;
	set_bit(XFS_LI_DIRTY, &qlp->qql_item.li_flags);
=======
	int64_t			dblocks,
	int64_t			rblocks,
	bool			force)
{
	struct xfs_mount	*mp = ip->i_mount;
	unsigned int		qflags = 0;
	int			error;

	if (!XFS_IS_QUOTA_ON(mp))
		return 0;

	ASSERT(!xfs_is_quota_inode(&mp->m_sb, ip->i_ino));
	ASSERT(xfs_isilocked(ip, XFS_ILOCK_EXCL));

	if (force)
		qflags |= XFS_QMOPT_FORCE_RES;

	/* Reserve data device quota against the inode's dquots. */
	error = xfs_trans_reserve_quota_bydquots(tp, mp, ip->i_udquot,
			ip->i_gdquot, ip->i_pdquot, dblocks, 0,
			XFS_QMOPT_RES_REGBLKS | qflags);
	if (error)
		return error;

	/* Do the same but for realtime blocks. */
	error = xfs_trans_reserve_quota_bydquots(tp, mp, ip->i_udquot,
			ip->i_gdquot, ip->i_pdquot, rblocks, 0,
			XFS_QMOPT_RES_RTBLKS | qflags);
	if (error) {
		xfs_trans_reserve_quota_bydquots(tp, mp, ip->i_udquot,
				ip->i_gdquot, ip->i_pdquot, -dblocks, 0,
				XFS_QMOPT_RES_REGBLKS);
		return error;
	}

	return 0;
}

/* Change the quota reservations for an inode creation activity. */
int
xfs_trans_reserve_quota_icreate(
	struct xfs_trans	*tp,
	struct xfs_dquot	*udqp,
	struct xfs_dquot	*gdqp,
	struct xfs_dquot	*pdqp,
	int64_t			dblocks)
{
	struct xfs_mount	*mp = tp->t_mountp;

	if (!XFS_IS_QUOTA_ON(mp))
		return 0;

	return xfs_trans_reserve_quota_bydquots(tp, mp, udqp, gdqp, pdqp,
			dblocks, 1, XFS_QMOPT_RES_REGBLKS);
>>>>>>> upstream/android-13
}

STATIC void
xfs_trans_alloc_dqinfo(
	xfs_trans_t	*tp)
{
<<<<<<< HEAD
	tp->t_dqinfo = kmem_zone_zalloc(xfs_qm_dqtrxzone, KM_SLEEP);
=======
	tp->t_dqinfo = kmem_cache_zalloc(xfs_qm_dqtrxzone,
					 GFP_KERNEL | __GFP_NOFAIL);
>>>>>>> upstream/android-13
}

void
xfs_trans_free_dqinfo(
	xfs_trans_t	*tp)
{
	if (!tp->t_dqinfo)
		return;
<<<<<<< HEAD
	kmem_zone_free(xfs_qm_dqtrxzone, tp->t_dqinfo);
=======
	kmem_cache_free(xfs_qm_dqtrxzone, tp->t_dqinfo);
>>>>>>> upstream/android-13
	tp->t_dqinfo = NULL;
}
