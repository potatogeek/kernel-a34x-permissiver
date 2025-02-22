/* SPDX-License-Identifier: GPL-2.0 */
#ifndef BLK_MQ_SCHED_H
#define BLK_MQ_SCHED_H

#include "blk-mq.h"
#include "blk-mq-tag.h"

<<<<<<< HEAD
void blk_mq_sched_free_hctx_data(struct request_queue *q,
				 void (*exit)(struct blk_mq_hw_ctx *));

void blk_mq_sched_assign_ioc(struct request *rq, struct bio *bio);

void blk_mq_sched_request_inserted(struct request *rq);
bool blk_mq_sched_try_merge(struct request_queue *q, struct bio *bio,
				struct request **merged_request);
bool __blk_mq_sched_bio_merge(struct request_queue *q, struct bio *bio);
bool blk_mq_sched_try_insert_merge(struct request_queue *q, struct request *rq);
=======
#define MAX_SCHED_RQ (16 * BLKDEV_MAX_RQ)

void blk_mq_sched_assign_ioc(struct request *rq);

bool blk_mq_sched_try_merge(struct request_queue *q, struct bio *bio,
		unsigned int nr_segs, struct request **merged_request);
bool __blk_mq_sched_bio_merge(struct request_queue *q, struct bio *bio,
		unsigned int nr_segs);
bool blk_mq_sched_try_insert_merge(struct request_queue *q, struct request *rq,
				   struct list_head *free);
>>>>>>> upstream/android-13
void blk_mq_sched_mark_restart_hctx(struct blk_mq_hw_ctx *hctx);
void blk_mq_sched_restart(struct blk_mq_hw_ctx *hctx);

void blk_mq_sched_insert_request(struct request *rq, bool at_head,
				 bool run_queue, bool async);
<<<<<<< HEAD
void blk_mq_sched_insert_requests(struct request_queue *q,
=======
void blk_mq_sched_insert_requests(struct blk_mq_hw_ctx *hctx,
>>>>>>> upstream/android-13
				  struct blk_mq_ctx *ctx,
				  struct list_head *list, bool run_queue_async);

void blk_mq_sched_dispatch_requests(struct blk_mq_hw_ctx *hctx);

int blk_mq_init_sched(struct request_queue *q, struct elevator_type *e);
void blk_mq_exit_sched(struct request_queue *q, struct elevator_queue *e);
<<<<<<< HEAD

static inline bool
blk_mq_sched_bio_merge(struct request_queue *q, struct bio *bio)
=======
void blk_mq_sched_free_requests(struct request_queue *q);

static inline bool
blk_mq_sched_bio_merge(struct request_queue *q, struct bio *bio,
		unsigned int nr_segs)
>>>>>>> upstream/android-13
{
	if (blk_queue_nomerges(q) || !bio_mergeable(bio))
		return false;

<<<<<<< HEAD
	return __blk_mq_sched_bio_merge(q, bio);
=======
	return __blk_mq_sched_bio_merge(q, bio, nr_segs);
>>>>>>> upstream/android-13
}

static inline bool
blk_mq_sched_allow_merge(struct request_queue *q, struct request *rq,
			 struct bio *bio)
{
	struct elevator_queue *e = q->elevator;

<<<<<<< HEAD
	if (e && e->type->ops.mq.allow_merge)
		return e->type->ops.mq.allow_merge(q, rq, bio);
=======
	if (e && e->type->ops.allow_merge)
		return e->type->ops.allow_merge(q, rq, bio);
>>>>>>> upstream/android-13

	return true;
}

<<<<<<< HEAD
static inline void blk_mq_sched_completed_request(struct request *rq)
{
	struct elevator_queue *e = rq->q->elevator;

	if (e && e->type->ops.mq.completed_request)
		e->type->ops.mq.completed_request(rq);
}

static inline void blk_mq_sched_started_request(struct request *rq)
{
	struct request_queue *q = rq->q;
	struct elevator_queue *e = q->elevator;

	if (e && e->type->ops.mq.started_request)
		e->type->ops.mq.started_request(rq);
=======
static inline void blk_mq_sched_completed_request(struct request *rq, u64 now)
{
	struct elevator_queue *e = rq->q->elevator;

	if (e && e->type->ops.completed_request)
		e->type->ops.completed_request(rq, now);
>>>>>>> upstream/android-13
}

static inline void blk_mq_sched_requeue_request(struct request *rq)
{
	struct request_queue *q = rq->q;
	struct elevator_queue *e = q->elevator;

<<<<<<< HEAD
	if (e && e->type->ops.mq.requeue_request)
		e->type->ops.mq.requeue_request(rq);
=======
	if ((rq->rq_flags & RQF_ELVPRIV) && e && e->type->ops.requeue_request)
		e->type->ops.requeue_request(rq);
>>>>>>> upstream/android-13
}

static inline bool blk_mq_sched_has_work(struct blk_mq_hw_ctx *hctx)
{
	struct elevator_queue *e = hctx->queue->elevator;

<<<<<<< HEAD
	if (e && e->type->ops.mq.has_work)
		return e->type->ops.mq.has_work(hctx);
=======
	if (e && e->type->ops.has_work)
		return e->type->ops.has_work(hctx);
>>>>>>> upstream/android-13

	return false;
}

static inline bool blk_mq_sched_needs_restart(struct blk_mq_hw_ctx *hctx)
{
	return test_bit(BLK_MQ_S_SCHED_RESTART, &hctx->state);
}

#endif
