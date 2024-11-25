<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Functions to sequence PREFLUSH and FUA writes.
 *
 * Copyright (C) 2011		Max Planck Institute for Gravitational Physics
 * Copyright (C) 2011		Tejun Heo <tj@kernel.org>
 *
<<<<<<< HEAD
 * This file is released under the GPLv2.
 *
=======
>>>>>>> upstream/android-13
 * REQ_{PREFLUSH|FUA} requests are decomposed to sequences consisted of three
 * optional steps - PREFLUSH, DATA and POSTFLUSH - according to the request
 * properties and hardware capability.
 *
 * If a request doesn't have data, only REQ_PREFLUSH makes sense, which
 * indicates a simple flush request.  If there is data, REQ_PREFLUSH indicates
 * that the device cache should be flushed before the data is executed, and
 * REQ_FUA means that the data must be on non-volatile media on request
 * completion.
 *
 * If the device doesn't have writeback cache, PREFLUSH and FUA don't make any
 * difference.  The requests are either completed immediately if there's no data
 * or executed as normal requests otherwise.
 *
 * If the device has writeback cache and supports FUA, REQ_PREFLUSH is
 * translated to PREFLUSH but REQ_FUA is passed down directly with DATA.
 *
 * If the device has writeback cache and doesn't support FUA, REQ_PREFLUSH
 * is translated to PREFLUSH and REQ_FUA to POSTFLUSH.
 *
 * The actual execution of flush is double buffered.  Whenever a request
 * needs to execute PRE or POSTFLUSH, it queues at
 * fq->flush_queue[fq->flush_pending_idx].  Once certain criteria are met, a
 * REQ_OP_FLUSH is issued and the pending_idx is toggled.  When the flush
 * completes, all the requests which were pending are proceeded to the next
 * step.  This allows arbitrary merging of different types of PREFLUSH/FUA
 * requests.
 *
 * Currently, the following conditions are used to determine when to issue
 * flush.
 *
 * C1. At any given time, only one flush shall be in progress.  This makes
 *     double buffering sufficient.
 *
 * C2. Flush is deferred if any request is executing DATA of its sequence.
 *     This avoids issuing separate POSTFLUSHes for requests which shared
 *     PREFLUSH.
 *
 * C3. The second condition is ignored if there is a request which has
 *     waited longer than FLUSH_PENDING_TIMEOUT.  This is to avoid
 *     starvation in the unlikely case where there are continuous stream of
 *     FUA (without PREFLUSH) requests.
 *
 * For devices which support FUA, it isn't clear whether C2 (and thus C3)
 * is beneficial.
 *
 * Note that a sequenced PREFLUSH/FUA request with DATA is completed twice.
 * Once while executing DATA and again after the whole sequence is
 * complete.  The first completion updates the contained bio but doesn't
 * finish it so that the bio submitter is notified only after the whole
 * sequence is complete.  This is implemented by testing RQF_FLUSH_SEQ in
 * req_bio_endio().
 *
 * The above peculiarity requires that each PREFLUSH/FUA request has only one
 * bio attached to it, which is guaranteed as they aren't allowed to be
 * merged in the usual way.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
#include <linux/gfp.h>
#include <linux/blk-mq.h>

#include "blk.h"
#include "blk-mq.h"
#include "blk-mq-tag.h"
#include "blk-mq-sched.h"

/* PREFLUSH/FUA sequences */
enum {
	REQ_FSEQ_PREFLUSH	= (1 << 0), /* pre-flushing in progress */
	REQ_FSEQ_DATA		= (1 << 1), /* data write in progress */
	REQ_FSEQ_POSTFLUSH	= (1 << 2), /* post-flushing in progress */
	REQ_FSEQ_DONE		= (1 << 3),

	REQ_FSEQ_ACTIONS	= REQ_FSEQ_PREFLUSH | REQ_FSEQ_DATA |
				  REQ_FSEQ_POSTFLUSH,

	/*
	 * If flush has been pending longer than the following timeout,
	 * it's issued even if flush_data requests are still in flight.
	 */
	FLUSH_PENDING_TIMEOUT	= 5 * HZ,
};

<<<<<<< HEAD
static bool blk_kick_flush(struct request_queue *q,
=======
static void blk_kick_flush(struct request_queue *q,
>>>>>>> upstream/android-13
			   struct blk_flush_queue *fq, unsigned int flags);

static unsigned int blk_flush_policy(unsigned long fflags, struct request *rq)
{
	unsigned int policy = 0;

	if (blk_rq_sectors(rq))
		policy |= REQ_FSEQ_DATA;

	if (fflags & (1UL << QUEUE_FLAG_WC)) {
		if (rq->cmd_flags & REQ_PREFLUSH)
			policy |= REQ_FSEQ_PREFLUSH;
		if (!(fflags & (1UL << QUEUE_FLAG_FUA)) &&
		    (rq->cmd_flags & REQ_FUA))
			policy |= REQ_FSEQ_POSTFLUSH;
	}
	return policy;
}

static unsigned int blk_flush_cur_seq(struct request *rq)
{
	return 1 << ffz(rq->flush.seq);
}

static void blk_flush_restore_request(struct request *rq)
{
	/*
	 * After flush data completion, @rq->bio is %NULL but we need to
	 * complete the bio again.  @rq->biotail is guaranteed to equal the
	 * original @rq->bio.  Restore it.
	 */
	rq->bio = rq->biotail;

	/* make @rq a normal request */
	rq->rq_flags &= ~RQF_FLUSH_SEQ;
	rq->end_io = rq->flush.saved_end_io;
}

<<<<<<< HEAD
static bool blk_flush_queue_rq(struct request *rq, bool add_front)
{
	if (rq->q->mq_ops) {
		blk_mq_add_to_requeue_list(rq, add_front, true);
		return false;
	} else {
		if (add_front)
			list_add(&rq->queuelist, &rq->q->queue_head);
		else
			list_add_tail(&rq->queuelist, &rq->q->queue_head);
		return true;
	}
=======
static void blk_flush_queue_rq(struct request *rq, bool add_front)
{
	blk_mq_add_to_requeue_list(rq, add_front, true);
}

static void blk_account_io_flush(struct request *rq)
{
	struct block_device *part = rq->rq_disk->part0;

	part_stat_lock();
	part_stat_inc(part, ios[STAT_FLUSH]);
	part_stat_add(part, nsecs[STAT_FLUSH],
		      ktime_get_ns() - rq->start_time_ns);
	part_stat_unlock();
>>>>>>> upstream/android-13
}

/**
 * blk_flush_complete_seq - complete flush sequence
 * @rq: PREFLUSH/FUA request being sequenced
 * @fq: flush queue
 * @seq: sequences to complete (mask of %REQ_FSEQ_*, can be zero)
 * @error: whether an error occurred
 *
 * @rq just completed @seq part of its flush sequence, record the
 * completion and trigger the next step.
 *
 * CONTEXT:
<<<<<<< HEAD
 * spin_lock_irq(q->queue_lock or fq->mq_flush_lock)
 *
 * RETURNS:
 * %true if requests were added to the dispatch queue, %false otherwise.
 */
static bool blk_flush_complete_seq(struct request *rq,
=======
 * spin_lock_irq(fq->mq_flush_lock)
 */
static void blk_flush_complete_seq(struct request *rq,
>>>>>>> upstream/android-13
				   struct blk_flush_queue *fq,
				   unsigned int seq, blk_status_t error)
{
	struct request_queue *q = rq->q;
	struct list_head *pending = &fq->flush_queue[fq->flush_pending_idx];
<<<<<<< HEAD
	bool queued = false, kicked;
=======
>>>>>>> upstream/android-13
	unsigned int cmd_flags;

	BUG_ON(rq->flush.seq & seq);
	rq->flush.seq |= seq;
	cmd_flags = rq->cmd_flags;

	if (likely(!error))
		seq = blk_flush_cur_seq(rq);
	else
		seq = REQ_FSEQ_DONE;

	switch (seq) {
	case REQ_FSEQ_PREFLUSH:
	case REQ_FSEQ_POSTFLUSH:
		/* queue for flush */
		if (list_empty(pending))
			fq->flush_pending_since = jiffies;
		list_move_tail(&rq->flush.list, pending);
		break;

	case REQ_FSEQ_DATA:
		list_move_tail(&rq->flush.list, &fq->flush_data_in_flight);
<<<<<<< HEAD
		queued = blk_flush_queue_rq(rq, true);
=======
		blk_flush_queue_rq(rq, true);
>>>>>>> upstream/android-13
		break;

	case REQ_FSEQ_DONE:
		/*
<<<<<<< HEAD
		 * @rq was previously adjusted by blk_flush_issue() for
=======
		 * @rq was previously adjusted by blk_insert_flush() for
>>>>>>> upstream/android-13
		 * flush sequencing and may already have gone through the
		 * flush data request completion path.  Restore @rq for
		 * normal completion and end it.
		 */
		BUG_ON(!list_empty(&rq->queuelist));
		list_del_init(&rq->flush.list);
		blk_flush_restore_request(rq);
<<<<<<< HEAD
		if (q->mq_ops)
			blk_mq_end_request(rq, error);
		else
			__blk_end_request_all(rq, error);
=======
		blk_mq_end_request(rq, error);
>>>>>>> upstream/android-13
		break;

	default:
		BUG();
	}

<<<<<<< HEAD
	kicked = blk_kick_flush(q, fq, cmd_flags);
	return kicked | queued;
=======
	blk_kick_flush(q, fq, cmd_flags);
>>>>>>> upstream/android-13
}

static void flush_end_io(struct request *flush_rq, blk_status_t error)
{
	struct request_queue *q = flush_rq->q;
	struct list_head *running;
<<<<<<< HEAD
	bool queued = false;
=======
>>>>>>> upstream/android-13
	struct request *rq, *n;
	unsigned long flags = 0;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, flush_rq->mq_ctx);

<<<<<<< HEAD
	if (q->mq_ops) {
		struct blk_mq_hw_ctx *hctx;

		/* release the tag's ownership to the req cloned from */
		spin_lock_irqsave(&fq->mq_flush_lock, flags);

		if (!refcount_dec_and_test(&flush_rq->ref)) {
			fq->rq_status = error;
			spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
			return;
		}

		if (fq->rq_status != BLK_STS_OK)
			error = fq->rq_status;

		hctx = blk_mq_map_queue(q, flush_rq->mq_ctx->cpu);
		if (!q->elevator) {
			blk_mq_tag_set_rq(hctx, flush_rq->tag, fq->orig_rq);
			flush_rq->tag = -1;
		} else {
			blk_mq_put_driver_tag_hctx(hctx, flush_rq);
			flush_rq->internal_tag = -1;
		}
=======
	/* release the tag's ownership to the req cloned from */
	spin_lock_irqsave(&fq->mq_flush_lock, flags);

	if (!refcount_dec_and_test(&flush_rq->ref)) {
		fq->rq_status = error;
		spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
		return;
	}

	blk_account_io_flush(flush_rq);
	/*
	 * Flush request has to be marked as IDLE when it is really ended
	 * because its .end_io() is called from timeout code path too for
	 * avoiding use-after-free.
	 */
	WRITE_ONCE(flush_rq->state, MQ_RQ_IDLE);
	if (fq->rq_status != BLK_STS_OK) {
		error = fq->rq_status;
		fq->rq_status = BLK_STS_OK;
	}

	if (!q->elevator) {
		flush_rq->tag = BLK_MQ_NO_TAG;
	} else {
		blk_mq_put_driver_tag(flush_rq);
		flush_rq->internal_tag = BLK_MQ_NO_TAG;
>>>>>>> upstream/android-13
	}

	running = &fq->flush_queue[fq->flush_running_idx];
	BUG_ON(fq->flush_pending_idx == fq->flush_running_idx);

	/* account completion of the flush request */
	fq->flush_running_idx ^= 1;

<<<<<<< HEAD
	if (!q->mq_ops)
		elv_completed_request(q, flush_rq);

=======
>>>>>>> upstream/android-13
	/* and push the waiting requests to the next stage */
	list_for_each_entry_safe(rq, n, running, flush.list) {
		unsigned int seq = blk_flush_cur_seq(rq);

		BUG_ON(seq != REQ_FSEQ_PREFLUSH && seq != REQ_FSEQ_POSTFLUSH);
<<<<<<< HEAD
		queued |= blk_flush_complete_seq(rq, fq, seq, error);
	}

	/*
	 * Kick the queue to avoid stall for two cases:
	 * 1. Moving a request silently to empty queue_head may stall the
	 * queue.
	 * 2. When flush request is running in non-queueable queue, the
	 * queue is hold. Restart the queue after flush request is finished
	 * to avoid stall.
	 * This function is called from request completion path and calling
	 * directly into request_fn may confuse the driver.  Always use
	 * kblockd.
	 */
	if (queued || fq->flush_queue_delayed) {
		WARN_ON(q->mq_ops);
		blk_run_queue_async(q);
	}
	fq->flush_queue_delayed = 0;
	if (q->mq_ops)
		spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
=======
		blk_flush_complete_seq(rq, fq, seq, error);
	}

	spin_unlock_irqrestore(&fq->mq_flush_lock, flags);
}

bool is_flush_rq(struct request *rq)
{
	return rq->end_io == flush_end_io;
>>>>>>> upstream/android-13
}

/**
 * blk_kick_flush - consider issuing flush request
 * @q: request_queue being kicked
 * @fq: flush queue
 * @flags: cmd_flags of the original request
 *
 * Flush related states of @q have changed, consider issuing flush request.
 * Please read the comment at the top of this file for more info.
 *
 * CONTEXT:
<<<<<<< HEAD
 * spin_lock_irq(q->queue_lock or fq->mq_flush_lock)
 *
 * RETURNS:
 * %true if flush was issued, %false otherwise.
 */
static bool blk_kick_flush(struct request_queue *q, struct blk_flush_queue *fq,
=======
 * spin_lock_irq(fq->mq_flush_lock)
 *
 */
static void blk_kick_flush(struct request_queue *q, struct blk_flush_queue *fq,
>>>>>>> upstream/android-13
			   unsigned int flags)
{
	struct list_head *pending = &fq->flush_queue[fq->flush_pending_idx];
	struct request *first_rq =
		list_first_entry(pending, struct request, flush.list);
	struct request *flush_rq = fq->flush_rq;

	/* C1 described at the top of this file */
	if (fq->flush_pending_idx != fq->flush_running_idx || list_empty(pending))
<<<<<<< HEAD
		return false;

	/* C2 and C3
	 *
	 * For blk-mq + scheduling, we can risk having all driver tags
	 * assigned to empty flushes, and we deadlock if we are expecting
	 * other requests to make progress. Don't defer for that case.
	 */
	if (!list_empty(&fq->flush_data_in_flight) &&
	    !(q->mq_ops && q->elevator) &&
	    time_before(jiffies,
			fq->flush_pending_since + FLUSH_PENDING_TIMEOUT))
		return false;
=======
		return;

	/* C2 and C3 */
	if (!list_empty(&fq->flush_data_in_flight) &&
	    time_before(jiffies,
			fq->flush_pending_since + FLUSH_PENDING_TIMEOUT))
		return;
>>>>>>> upstream/android-13

	/*
	 * Issue flush and toggle pending_idx.  This makes pending_idx
	 * different from running_idx, which means flush is in flight.
	 */
	fq->flush_pending_idx ^= 1;

	blk_rq_init(q, flush_rq);

	/*
	 * In case of none scheduler, borrow tag from the first request
	 * since they can't be in flight at the same time. And acquire
	 * the tag's ownership for flush req.
	 *
	 * In case of IO scheduler, flush rq need to borrow scheduler tag
	 * just for cheating put/get driver tag.
	 */
<<<<<<< HEAD
	if (q->mq_ops) {
		struct blk_mq_hw_ctx *hctx;

		flush_rq->mq_ctx = first_rq->mq_ctx;

		if (!q->elevator) {
			fq->orig_rq = first_rq;
			flush_rq->tag = first_rq->tag;
			hctx = blk_mq_map_queue(q, first_rq->mq_ctx->cpu);
			blk_mq_tag_set_rq(hctx, first_rq->tag, flush_rq);
		} else {
			flush_rq->internal_tag = first_rq->internal_tag;
		}
	}
=======
	flush_rq->mq_ctx = first_rq->mq_ctx;
	flush_rq->mq_hctx = first_rq->mq_hctx;

	if (!q->elevator) {
		flush_rq->tag = first_rq->tag;

		/*
		 * We borrow data request's driver tag, so have to mark
		 * this flush request as INFLIGHT for avoiding double
		 * account of this driver tag
		 */
		flush_rq->rq_flags |= RQF_MQ_INFLIGHT;
	} else
		flush_rq->internal_tag = first_rq->internal_tag;
>>>>>>> upstream/android-13

	flush_rq->cmd_flags = REQ_OP_FLUSH | REQ_PREFLUSH;
	flush_rq->cmd_flags |= (flags & REQ_DRV) | (flags & REQ_FAILFAST_MASK);
	flush_rq->rq_flags |= RQF_FLUSH_SEQ;
	flush_rq->rq_disk = first_rq->rq_disk;
	flush_rq->end_io = flush_end_io;
<<<<<<< HEAD

	return blk_flush_queue_rq(flush_rq, false);
}

static void flush_data_end_io(struct request *rq, blk_status_t error)
{
	struct request_queue *q = rq->q;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, NULL);

	lockdep_assert_held(q->queue_lock);

	/*
	 * Updating q->in_flight[] here for making this tag usable
	 * early. Because in blk_queue_start_tag(),
	 * q->in_flight[BLK_RW_ASYNC] is used to limit async I/O and
	 * reserve tags for sync I/O.
	 *
	 * More importantly this way can avoid the following I/O
	 * deadlock:
	 *
	 * - suppose there are 40 fua requests comming to flush queue
	 *   and queue depth is 31
	 * - 30 rqs are scheduled then blk_queue_start_tag() can't alloc
	 *   tag for async I/O any more
	 * - all the 30 rqs are completed before FLUSH_PENDING_TIMEOUT
	 *   and flush_data_end_io() is called
	 * - the other rqs still can't go ahead if not updating
	 *   q->in_flight[BLK_RW_ASYNC] here, meantime these rqs
	 *   are held in flush data queue and make no progress of
	 *   handling post flush rq
	 * - only after the post flush rq is handled, all these rqs
	 *   can be completed
	 */

	elv_completed_request(q, rq);

	/* for avoiding double accounting */
	rq->rq_flags &= ~RQF_STARTED;

	/*
	 * After populating an empty queue, kick it to avoid stall.  Read
	 * the comment in flush_end_io().
	 */
	if (blk_flush_complete_seq(rq, fq, REQ_FSEQ_DATA, error))
		blk_run_queue_async(q);
=======
	/*
	 * Order WRITE ->end_io and WRITE rq->ref, and its pair is the one
	 * implied in refcount_inc_not_zero() called from
	 * blk_mq_find_and_get_req(), which orders WRITE/READ flush_rq->ref
	 * and READ flush_rq->end_io
	 */
	smp_wmb();
	refcount_set(&flush_rq->ref, 1);

	blk_flush_queue_rq(flush_rq, false);
>>>>>>> upstream/android-13
}

static void mq_flush_data_end_io(struct request *rq, blk_status_t error)
{
	struct request_queue *q = rq->q;
<<<<<<< HEAD
	struct blk_mq_hw_ctx *hctx;
=======
	struct blk_mq_hw_ctx *hctx = rq->mq_hctx;
>>>>>>> upstream/android-13
	struct blk_mq_ctx *ctx = rq->mq_ctx;
	unsigned long flags;
	struct blk_flush_queue *fq = blk_get_flush_queue(q, ctx);

<<<<<<< HEAD
	hctx = blk_mq_map_queue(q, ctx->cpu);

	if (q->elevator) {
		WARN_ON(rq->tag < 0);
		blk_mq_put_driver_tag_hctx(hctx, rq);
=======
	if (q->elevator) {
		WARN_ON(rq->tag < 0);
		blk_mq_put_driver_tag(rq);
>>>>>>> upstream/android-13
	}

	/*
	 * After populating an empty queue, kick it to avoid stall.  Read
	 * the comment in flush_end_io().
	 */
	spin_lock_irqsave(&fq->mq_flush_lock, flags);
	blk_flush_complete_seq(rq, fq, REQ_FSEQ_DATA, error);
	spin_unlock_irqrestore(&fq->mq_flush_lock, flags);

	blk_mq_sched_restart(hctx);
}

/**
 * blk_insert_flush - insert a new PREFLUSH/FUA request
 * @rq: request to insert
 *
 * To be called from __elv_add_request() for %ELEVATOR_INSERT_FLUSH insertions.
 * or __blk_mq_run_hw_queue() to dispatch request.
 * @rq is being submitted.  Analyze what needs to be done and put it on the
 * right queue.
 */
void blk_insert_flush(struct request *rq)
{
	struct request_queue *q = rq->q;
	unsigned long fflags = q->queue_flags;	/* may change, cache */
	unsigned int policy = blk_flush_policy(fflags, rq);
	struct blk_flush_queue *fq = blk_get_flush_queue(q, rq->mq_ctx);

<<<<<<< HEAD
	if (!q->mq_ops)
		lockdep_assert_held(q->queue_lock);

=======
>>>>>>> upstream/android-13
	/*
	 * @policy now records what operations need to be done.  Adjust
	 * REQ_PREFLUSH and FUA for the driver.
	 */
	rq->cmd_flags &= ~REQ_PREFLUSH;
	if (!(fflags & (1UL << QUEUE_FLAG_FUA)))
		rq->cmd_flags &= ~REQ_FUA;

	/*
	 * REQ_PREFLUSH|REQ_FUA implies REQ_SYNC, so if we clear any
	 * of those flags, we have to set REQ_SYNC to avoid skewing
	 * the request accounting.
	 */
	rq->cmd_flags |= REQ_SYNC;

	/*
	 * An empty flush handed down from a stacking driver may
	 * translate into nothing if the underlying device does not
	 * advertise a write-back cache.  In this case, simply
	 * complete the request.
	 */
	if (!policy) {
<<<<<<< HEAD
		if (q->mq_ops)
			blk_mq_end_request(rq, 0);
		else
			__blk_end_request(rq, 0, 0);
=======
		blk_mq_end_request(rq, 0);
>>>>>>> upstream/android-13
		return;
	}

	BUG_ON(rq->bio != rq->biotail); /*assumes zero or single bio rq */

	/*
	 * If there's data but flush is not necessary, the request can be
	 * processed directly without going through flush machinery.  Queue
	 * for normal execution.
	 */
	if ((policy & REQ_FSEQ_DATA) &&
	    !(policy & (REQ_FSEQ_PREFLUSH | REQ_FSEQ_POSTFLUSH))) {
<<<<<<< HEAD
		if (q->mq_ops)
			blk_mq_request_bypass_insert(rq, false);
		else
			list_add_tail(&rq->queuelist, &q->queue_head);
=======
		blk_mq_request_bypass_insert(rq, false, false);
>>>>>>> upstream/android-13
		return;
	}

	/*
	 * @rq should go through flush machinery.  Mark it part of flush
	 * sequence and submit for further processing.
	 */
	memset(&rq->flush, 0, sizeof(rq->flush));
	INIT_LIST_HEAD(&rq->flush.list);
	rq->rq_flags |= RQF_FLUSH_SEQ;
	rq->flush.saved_end_io = rq->end_io; /* Usually NULL */
<<<<<<< HEAD
	if (q->mq_ops) {
		rq->end_io = mq_flush_data_end_io;

		spin_lock_irq(&fq->mq_flush_lock);
		blk_flush_complete_seq(rq, fq, REQ_FSEQ_ACTIONS & ~policy, 0);
		spin_unlock_irq(&fq->mq_flush_lock);
		return;
	}
	rq->end_io = flush_data_end_io;

	blk_flush_complete_seq(rq, fq, REQ_FSEQ_ACTIONS & ~policy, 0);
=======

	rq->end_io = mq_flush_data_end_io;

	spin_lock_irq(&fq->mq_flush_lock);
	blk_flush_complete_seq(rq, fq, REQ_FSEQ_ACTIONS & ~policy, 0);
	spin_unlock_irq(&fq->mq_flush_lock);
>>>>>>> upstream/android-13
}

/**
 * blkdev_issue_flush - queue a flush
 * @bdev:	blockdev to issue flush for
<<<<<<< HEAD
 * @gfp_mask:	memory allocation flags (for bio_alloc)
 * @error_sector:	error sector
 *
 * Description:
 *    Issue a flush for the block device in question. Caller can supply
 *    room for storing the error offset in case of a flush error, if they
 *    wish to.
 */
int blkdev_issue_flush(struct block_device *bdev, gfp_t gfp_mask,
		sector_t *error_sector)
{
	struct request_queue *q;
	struct bio *bio;
	int ret = 0;

	if (bdev->bd_disk == NULL)
		return -ENXIO;

	q = bdev_get_queue(bdev);
	if (!q)
		return -ENXIO;

	/*
	 * some block devices may not have their queue correctly set up here
	 * (e.g. loop device without a backing file) and so issuing a flush
	 * here will panic. Ensure there is a request function before issuing
	 * the flush.
	 */
	if (!q->make_request_fn)
		return -ENXIO;

	bio = bio_alloc(gfp_mask, 0);
	bio_set_dev(bio, bdev);
	bio->bi_opf = REQ_OP_WRITE | REQ_PREFLUSH;

	ret = submit_bio_wait(bio);

	/*
	 * The driver must store the error location in ->bi_sector, if
	 * it supports it. For non-stacked drivers, this should be
	 * copied from blk_rq_pos(rq).
	 */
	if (error_sector)
		*error_sector = bio->bi_iter.bi_sector;

	bio_put(bio);
	return ret;
}
EXPORT_SYMBOL(blkdev_issue_flush);

struct blk_flush_queue *blk_alloc_flush_queue(struct request_queue *q,
		int node, int cmd_size, gfp_t flags)
=======
 *
 * Description:
 *    Issue a flush for the block device in question.
 */
int blkdev_issue_flush(struct block_device *bdev)
{
	struct bio bio;

	bio_init(&bio, NULL, 0);
	bio_set_dev(&bio, bdev);
	bio.bi_opf = REQ_OP_WRITE | REQ_PREFLUSH;
	return submit_bio_wait(&bio);
}
EXPORT_SYMBOL(blkdev_issue_flush);

struct blk_flush_queue *blk_alloc_flush_queue(int node, int cmd_size,
					      gfp_t flags)
>>>>>>> upstream/android-13
{
	struct blk_flush_queue *fq;
	int rq_sz = sizeof(struct request);

	fq = kzalloc_node(sizeof(*fq), flags, node);
	if (!fq)
		goto fail;

<<<<<<< HEAD
	if (q->mq_ops)
		spin_lock_init(&fq->mq_flush_lock);
=======
	spin_lock_init(&fq->mq_flush_lock);
>>>>>>> upstream/android-13

	rq_sz = round_up(rq_sz + cmd_size, cache_line_size());
	fq->flush_rq = kzalloc_node(rq_sz, flags, node);
	if (!fq->flush_rq)
		goto fail_rq;

	INIT_LIST_HEAD(&fq->flush_queue[0]);
	INIT_LIST_HEAD(&fq->flush_queue[1]);
	INIT_LIST_HEAD(&fq->flush_data_in_flight);

	return fq;

 fail_rq:
	kfree(fq);
 fail:
	return NULL;
}

void blk_free_flush_queue(struct blk_flush_queue *fq)
{
	/* bio based request queue hasn't flush queue */
	if (!fq)
		return;

	kfree(fq->flush_rq);
	kfree(fq);
}
<<<<<<< HEAD
=======

/*
 * Allow driver to set its own lock class to fq->mq_flush_lock for
 * avoiding lockdep complaint.
 *
 * flush_end_io() may be called recursively from some driver, such as
 * nvme-loop, so lockdep may complain 'possible recursive locking' because
 * all 'struct blk_flush_queue' instance share same mq_flush_lock lock class
 * key. We need to assign different lock class for these driver's
 * fq->mq_flush_lock for avoiding the lockdep warning.
 *
 * Use dynamically allocated lock class key for each 'blk_flush_queue'
 * instance is over-kill, and more worse it introduces horrible boot delay
 * issue because synchronize_rcu() is implied in lockdep_unregister_key which
 * is called for each hctx release. SCSI probing may synchronously create and
 * destroy lots of MQ request_queues for non-existent devices, and some robot
 * test kernel always enable lockdep option. It is observed that more than half
 * an hour is taken during SCSI MQ probe with per-fq lock class.
 */
void blk_mq_hctx_set_fq_lock_class(struct blk_mq_hw_ctx *hctx,
		struct lock_class_key *key)
{
	lockdep_set_class(&hctx->fq->mq_flush_lock, key);
}
EXPORT_SYMBOL_GPL(blk_mq_hctx_set_fq_lock_class);
>>>>>>> upstream/android-13
