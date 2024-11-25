/* SPDX-License-Identifier: GPL-2.0 */
#ifndef INT_BLK_MQ_TAG_H
#define INT_BLK_MQ_TAG_H

<<<<<<< HEAD
#include "blk-mq.h"

=======
>>>>>>> upstream/android-13
/*
 * Tag address space map.
 */
struct blk_mq_tags {
	unsigned int nr_tags;
	unsigned int nr_reserved_tags;

	atomic_t active_queues;

<<<<<<< HEAD
	struct sbitmap_queue bitmap_tags;
	struct sbitmap_queue breserved_tags;
=======
	struct sbitmap_queue *bitmap_tags;
	struct sbitmap_queue *breserved_tags;

	struct sbitmap_queue __bitmap_tags;
	struct sbitmap_queue __breserved_tags;
>>>>>>> upstream/android-13

	struct request **rqs;
	struct request **static_rqs;
	struct list_head page_list;
<<<<<<< HEAD
};


extern struct blk_mq_tags *blk_mq_init_tags(unsigned int nr_tags, unsigned int reserved_tags, int node, int alloc_policy);
extern void blk_mq_free_tags(struct blk_mq_tags *tags);

extern unsigned int blk_mq_get_tag(struct blk_mq_alloc_data *data);
extern void blk_mq_put_tag(struct blk_mq_hw_ctx *hctx, struct blk_mq_tags *tags,
			   struct blk_mq_ctx *ctx, unsigned int tag);
extern bool blk_mq_has_free_tags(struct blk_mq_tags *tags);
extern int blk_mq_tag_update_depth(struct blk_mq_hw_ctx *hctx,
					struct blk_mq_tags **tags,
					unsigned int depth, bool can_grow);
extern void blk_mq_tag_wakeup_all(struct blk_mq_tags *tags, bool);
void blk_mq_queue_tag_busy_iter(struct request_queue *q, busy_iter_fn *fn,
		void *priv);
=======

	/*
	 * used to clear request reference in rqs[] before freeing one
	 * request pool
	 */
	spinlock_t lock;

	ANDROID_OEM_DATA(1);
};

extern struct blk_mq_tags *blk_mq_init_tags(unsigned int nr_tags,
					unsigned int reserved_tags,
					int node, unsigned int flags);
extern void blk_mq_free_tags(struct blk_mq_tags *tags, unsigned int flags);
extern int blk_mq_init_bitmaps(struct sbitmap_queue *bitmap_tags,
			       struct sbitmap_queue *breserved_tags,
			       unsigned int queue_depth,
			       unsigned int reserved,
			       int node, int alloc_policy);

extern int blk_mq_init_shared_sbitmap(struct blk_mq_tag_set *set);
extern void blk_mq_exit_shared_sbitmap(struct blk_mq_tag_set *set);
extern unsigned int blk_mq_get_tag(struct blk_mq_alloc_data *data);
extern void blk_mq_put_tag(struct blk_mq_tags *tags, struct blk_mq_ctx *ctx,
			   unsigned int tag);
extern int blk_mq_tag_update_depth(struct blk_mq_hw_ctx *hctx,
					struct blk_mq_tags **tags,
					unsigned int depth, bool can_grow);
extern void blk_mq_tag_resize_shared_sbitmap(struct blk_mq_tag_set *set,
					     unsigned int size);

extern void blk_mq_tag_wakeup_all(struct blk_mq_tags *tags, bool);
void blk_mq_queue_tag_busy_iter(struct request_queue *q, busy_iter_fn *fn,
		void *priv);
void blk_mq_all_tag_iter(struct blk_mq_tags *tags, busy_tag_iter_fn *fn,
		void *priv);
>>>>>>> upstream/android-13

static inline struct sbq_wait_state *bt_wait_ptr(struct sbitmap_queue *bt,
						 struct blk_mq_hw_ctx *hctx)
{
	if (!hctx)
		return &bt->ws[0];
	return sbq_wait_ptr(bt, &hctx->wait_index);
}

enum {
<<<<<<< HEAD
	BLK_MQ_TAG_FAIL		= -1U,
	BLK_MQ_TAG_MIN		= 1,
	BLK_MQ_TAG_MAX		= BLK_MQ_TAG_FAIL - 1,
=======
	BLK_MQ_NO_TAG		= -1U,
	BLK_MQ_TAG_MIN		= 1,
	BLK_MQ_TAG_MAX		= BLK_MQ_NO_TAG - 1,
>>>>>>> upstream/android-13
};

extern bool __blk_mq_tag_busy(struct blk_mq_hw_ctx *);
extern void __blk_mq_tag_idle(struct blk_mq_hw_ctx *);

static inline bool blk_mq_tag_busy(struct blk_mq_hw_ctx *hctx)
{
<<<<<<< HEAD
	if (!(hctx->flags & BLK_MQ_F_TAG_SHARED))
=======
	if (!(hctx->flags & BLK_MQ_F_TAG_QUEUE_SHARED))
>>>>>>> upstream/android-13
		return false;

	return __blk_mq_tag_busy(hctx);
}

static inline void blk_mq_tag_idle(struct blk_mq_hw_ctx *hctx)
{
<<<<<<< HEAD
	if (!(hctx->flags & BLK_MQ_F_TAG_SHARED))
=======
	if (!(hctx->flags & BLK_MQ_F_TAG_QUEUE_SHARED))
>>>>>>> upstream/android-13
		return;

	__blk_mq_tag_idle(hctx);
}

<<<<<<< HEAD
/*
 * This helper should only be used for flush request to share tag
 * with the request cloned from, and both the two requests can't be
 * in flight at the same time. The caller has to make sure the tag
 * can't be freed.
 */
static inline void blk_mq_tag_set_rq(struct blk_mq_hw_ctx *hctx,
		unsigned int tag, struct request *rq)
{
	hctx->tags->rqs[tag] = rq;
}

=======
>>>>>>> upstream/android-13
static inline bool blk_mq_tag_is_reserved(struct blk_mq_tags *tags,
					  unsigned int tag)
{
	return tag < tags->nr_reserved_tags;
}

#endif
