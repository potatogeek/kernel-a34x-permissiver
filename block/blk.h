/* SPDX-License-Identifier: GPL-2.0 */
#ifndef BLK_INTERNAL_H
#define BLK_INTERNAL_H

#include <linux/idr.h>
#include <linux/blk-mq.h>
<<<<<<< HEAD
#include "blk-mq.h"

/* Amount of time in which a process may batch requests */
#define BLK_BATCH_TIME	(HZ/50UL)

/* Number of requests a "batching" process may submit */
#define BLK_BATCH_REQ	32
=======
#include <linux/part_stat.h>
#include <linux/blk-crypto.h>
#include <linux/memblock.h>	/* for max_pfn/max_low_pfn */
#include <xen/xen.h>
#include "blk-crypto-internal.h"
#include "blk-mq.h"
#include "blk-mq-sched.h"
>>>>>>> upstream/android-13

/* Max future timer expiry for timeouts */
#define BLK_MAX_TIMEOUT		(5 * HZ)

<<<<<<< HEAD
#ifdef CONFIG_DEBUG_FS
extern struct dentry *blk_debugfs_root;
#endif

struct blk_flush_queue {
	unsigned int		flush_queue_delayed:1;
=======
extern struct dentry *blk_debugfs_root;

struct blk_flush_queue {
>>>>>>> upstream/android-13
	unsigned int		flush_pending_idx:1;
	unsigned int		flush_running_idx:1;
	blk_status_t 		rq_status;
	unsigned long		flush_pending_since;
	struct list_head	flush_queue[2];
	struct list_head	flush_data_in_flight;
	struct request		*flush_rq;

<<<<<<< HEAD
	/*
	 * flush_rq shares tag with this rq, both can't be active
	 * at the same time
	 */
	struct request		*orig_rq;
=======
>>>>>>> upstream/android-13
	spinlock_t		mq_flush_lock;
};

extern struct kmem_cache *blk_requestq_cachep;
<<<<<<< HEAD
extern struct kmem_cache *request_cachep;
extern struct kobj_type blk_queue_ktype;
extern struct ida blk_queue_ida;

/*
 * @q->queue_lock is set while a queue is being initialized. Since we know
 * that no other threads access the queue object before @q->queue_lock has
 * been set, it is safe to manipulate queue flags without holding the
 * queue_lock if @q->queue_lock == NULL. See also blk_alloc_queue_node() and
 * blk_init_allocated_queue().
 */
static inline void queue_lockdep_assert_held(struct request_queue *q)
{
	if (q->queue_lock)
		lockdep_assert_held(q->queue_lock);
}

static inline void queue_flag_set_unlocked(unsigned int flag,
					   struct request_queue *q)
{
	if (test_bit(QUEUE_FLAG_INIT_DONE, &q->queue_flags) &&
	    kref_read(&q->kobj.kref))
		lockdep_assert_held(q->queue_lock);
	__set_bit(flag, &q->queue_flags);
}

static inline void queue_flag_clear_unlocked(unsigned int flag,
					     struct request_queue *q)
{
	if (test_bit(QUEUE_FLAG_INIT_DONE, &q->queue_flags) &&
	    kref_read(&q->kobj.kref))
		lockdep_assert_held(q->queue_lock);
	__clear_bit(flag, &q->queue_flags);
}

static inline int queue_flag_test_and_clear(unsigned int flag,
					    struct request_queue *q)
{
	queue_lockdep_assert_held(q);

	if (test_bit(flag, &q->queue_flags)) {
		__clear_bit(flag, &q->queue_flags);
		return 1;
	}

	return 0;
}

static inline int queue_flag_test_and_set(unsigned int flag,
					  struct request_queue *q)
{
	queue_lockdep_assert_held(q);

	if (!test_bit(flag, &q->queue_flags)) {
		__set_bit(flag, &q->queue_flags);
		return 0;
	}

	return 1;
}

static inline void queue_flag_set(unsigned int flag, struct request_queue *q)
{
	queue_lockdep_assert_held(q);
	__set_bit(flag, &q->queue_flags);
}

static inline void queue_flag_clear(unsigned int flag, struct request_queue *q)
{
	queue_lockdep_assert_held(q);
	__clear_bit(flag, &q->queue_flags);
}

static inline struct blk_flush_queue *blk_get_flush_queue(
		struct request_queue *q, struct blk_mq_ctx *ctx)
{
	if (q->mq_ops)
		return blk_mq_map_queue(q, ctx->cpu)->fq;
	return q->fq;
=======
extern struct kobj_type blk_queue_ktype;
extern struct ida blk_queue_ida;

static inline struct blk_flush_queue *
blk_get_flush_queue(struct request_queue *q, struct blk_mq_ctx *ctx)
{
	return blk_mq_map_queue(q, REQ_OP_FLUSH, ctx)->fq;
>>>>>>> upstream/android-13
}

static inline void __blk_get_queue(struct request_queue *q)
{
	kobject_get(&q->kobj);
}

<<<<<<< HEAD
static inline bool
is_flush_rq(struct request *req, struct blk_mq_hw_ctx *hctx)
{
	return hctx->fq->flush_rq == req;
}

struct blk_flush_queue *blk_alloc_flush_queue(struct request_queue *q,
		int node, int cmd_size, gfp_t flags);
void blk_free_flush_queue(struct blk_flush_queue *q);

int blk_init_rl(struct request_list *rl, struct request_queue *q,
		gfp_t gfp_mask);
void blk_exit_rl(struct request_queue *q, struct request_list *rl);
void blk_exit_queue(struct request_queue *q);
void blk_rq_bio_prep(struct request_queue *q, struct request *rq,
			struct bio *bio);
void blk_queue_bypass_start(struct request_queue *q);
void blk_queue_bypass_end(struct request_queue *q);
void __blk_queue_free_tags(struct request_queue *q);
void blk_freeze_queue(struct request_queue *q);

static inline void blk_queue_enter_live(struct request_queue *q)
{
	/*
	 * Given that running in generic_make_request() context
	 * guarantees that a live reference against q_usage_counter has
	 * been established, further references under that same context
	 * need not check that the queue has been frozen (marked dead).
	 */
	percpu_ref_get(&q->q_usage_counter);
=======
bool is_flush_rq(struct request *req);

struct blk_flush_queue *blk_alloc_flush_queue(int node, int cmd_size,
					      gfp_t flags);
void blk_free_flush_queue(struct blk_flush_queue *q);

void blk_freeze_queue(struct request_queue *q);
void __blk_mq_unfreeze_queue(struct request_queue *q, bool force_atomic);
void blk_queue_start_drain(struct request_queue *q);

#define BIO_INLINE_VECS 4
struct bio_vec *bvec_alloc(mempool_t *pool, unsigned short *nr_vecs,
		gfp_t gfp_mask);
void bvec_free(mempool_t *pool, struct bio_vec *bv, unsigned short nr_vecs);

static inline bool biovec_phys_mergeable(struct request_queue *q,
		struct bio_vec *vec1, struct bio_vec *vec2)
{
	unsigned long mask = queue_segment_boundary(q);
	phys_addr_t addr1 = page_to_phys(vec1->bv_page) + vec1->bv_offset;
	phys_addr_t addr2 = page_to_phys(vec2->bv_page) + vec2->bv_offset;

	if (addr1 + vec1->bv_len != addr2)
		return false;
	if (xen_domain() && !xen_biovec_phys_mergeable(vec1, vec2->bv_page))
		return false;
	if ((addr1 | mask) != ((addr2 + vec2->bv_len - 1) | mask))
		return false;
	return true;
}

static inline bool __bvec_gap_to_prev(struct request_queue *q,
		struct bio_vec *bprv, unsigned int offset)
{
	return (offset & queue_virt_boundary(q)) ||
		((bprv->bv_offset + bprv->bv_len) & queue_virt_boundary(q));
}

/*
 * Check if adding a bio_vec after bprv with offset would create a gap in
 * the SG list. Most drivers don't care about this, but some do.
 */
static inline bool bvec_gap_to_prev(struct request_queue *q,
		struct bio_vec *bprv, unsigned int offset)
{
	if (!queue_virt_boundary(q))
		return false;
	return __bvec_gap_to_prev(q, bprv, offset);
>>>>>>> upstream/android-13
}

#ifdef CONFIG_BLK_DEV_INTEGRITY
void blk_flush_integrity(void);
bool __bio_integrity_endio(struct bio *);
<<<<<<< HEAD
=======
void bio_integrity_free(struct bio *bio);
>>>>>>> upstream/android-13
static inline bool bio_integrity_endio(struct bio *bio)
{
	if (bio_integrity(bio))
		return __bio_integrity_endio(bio);
	return true;
}
<<<<<<< HEAD
#else
=======

bool blk_integrity_merge_rq(struct request_queue *, struct request *,
		struct request *);
bool blk_integrity_merge_bio(struct request_queue *, struct request *,
		struct bio *);

static inline bool integrity_req_gap_back_merge(struct request *req,
		struct bio *next)
{
	struct bio_integrity_payload *bip = bio_integrity(req->bio);
	struct bio_integrity_payload *bip_next = bio_integrity(next);

	return bvec_gap_to_prev(req->q, &bip->bip_vec[bip->bip_vcnt - 1],
				bip_next->bip_vec[0].bv_offset);
}

static inline bool integrity_req_gap_front_merge(struct request *req,
		struct bio *bio)
{
	struct bio_integrity_payload *bip = bio_integrity(bio);
	struct bio_integrity_payload *bip_next = bio_integrity(req->bio);

	return bvec_gap_to_prev(req->q, &bip->bip_vec[bip->bip_vcnt - 1],
				bip_next->bip_vec[0].bv_offset);
}

int blk_integrity_add(struct gendisk *disk);
void blk_integrity_del(struct gendisk *);
#else /* CONFIG_BLK_DEV_INTEGRITY */
static inline bool blk_integrity_merge_rq(struct request_queue *rq,
		struct request *r1, struct request *r2)
{
	return true;
}
static inline bool blk_integrity_merge_bio(struct request_queue *rq,
		struct request *r, struct bio *b)
{
	return true;
}
static inline bool integrity_req_gap_back_merge(struct request *req,
		struct bio *next)
{
	return false;
}
static inline bool integrity_req_gap_front_merge(struct request *req,
		struct bio *bio)
{
	return false;
}

>>>>>>> upstream/android-13
static inline void blk_flush_integrity(void)
{
}
static inline bool bio_integrity_endio(struct bio *bio)
{
	return true;
}
<<<<<<< HEAD
#endif

void blk_timeout_work(struct work_struct *work);
unsigned long blk_rq_timeout(unsigned long timeout);
void blk_add_timer(struct request *req);
void blk_delete_timer(struct request *);


bool bio_attempt_front_merge(struct request_queue *q, struct request *req,
			     struct bio *bio);
bool bio_attempt_back_merge(struct request_queue *q, struct request *req,
			    struct bio *bio);
bool bio_attempt_discard_merge(struct request_queue *q, struct request *req,
		struct bio *bio);
bool blk_attempt_plug_merge(struct request_queue *q, struct bio *bio,
			    unsigned int *request_count,
			    struct request **same_queue_rq);
unsigned int blk_plug_queued_count(struct request_queue *q);

void blk_account_io_start(struct request *req, bool new_io);
void blk_account_io_completion(struct request *req, unsigned int bytes);
void blk_account_io_done(struct request *req, u64 now);

/*
 * EH timer and IO completion will both attempt to 'grab' the request, make
 * sure that only one of them succeeds. Steal the bottom bit of the
 * __deadline field for this.
 */
static inline int blk_mark_rq_complete(struct request *rq)
{
	return test_and_set_bit(0, &rq->__deadline);
}

static inline void blk_clear_rq_complete(struct request *rq)
{
	clear_bit(0, &rq->__deadline);
}

static inline bool blk_rq_is_complete(struct request *rq)
{
	return test_bit(0, &rq->__deadline);
}

#ifdef CONFIG_BLK_IO_VOLUME
void blk_queue_reset_io_vol(struct request_queue *q);
void blk_queue_io_vol_add(struct request_queue *q,
	int opf, long long bytes);
void blk_queue_io_vol_del(struct request_queue *q,
	int opf, long long bytes);
void blk_queue_io_vol_merge(struct request_queue *q,
	int opf, int rqs, long long bytes);
#else
#define blk_queue_reset_io_vol(q)			do {} while (0)
#define blk_queue_io_vol_add(q, opf, bytes)		do {} while (0)
#define blk_queue_io_vol_del(q, opf, bytes)		do {} while (0)
#define blk_queue_io_vol_merge(q, opf, rqs, bytes)	do {} while (0)
#endif
=======
static inline void bio_integrity_free(struct bio *bio)
{
}
static inline int blk_integrity_add(struct gendisk *disk)
{
	return 0;
}
static inline void blk_integrity_del(struct gendisk *disk)
{
}
#endif /* CONFIG_BLK_DEV_INTEGRITY */

unsigned long blk_rq_timeout(unsigned long timeout);
void blk_add_timer(struct request *req);

bool blk_attempt_plug_merge(struct request_queue *q, struct bio *bio,
		unsigned int nr_segs, struct request **same_queue_rq);
bool blk_bio_list_merge(struct request_queue *q, struct list_head *list,
			struct bio *bio, unsigned int nr_segs);

void blk_account_io_start(struct request *req);
void blk_account_io_done(struct request *req, u64 now);

/*
 * Plug flush limits
 */
#define BLK_MAX_REQUEST_COUNT	32
#define BLK_PLUG_FLUSH_SIZE	(128 * 1024)
>>>>>>> upstream/android-13

/*
 * Internal elevator interface
 */
#define ELV_ON_HASH(rq) ((rq)->rq_flags & RQF_HASHED)

void blk_insert_flush(struct request *rq);

<<<<<<< HEAD
static inline void elv_activate_rq(struct request_queue *q, struct request *rq)
{
	struct elevator_queue *e = q->elevator;

	if (e->type->ops.sq.elevator_activate_req_fn)
		e->type->ops.sq.elevator_activate_req_fn(q, rq);
}

static inline void elv_deactivate_rq(struct request_queue *q, struct request *rq)
{
	struct elevator_queue *e = q->elevator;

	if (e->type->ops.sq.elevator_deactivate_req_fn)
		e->type->ops.sq.elevator_deactivate_req_fn(q, rq);
}

int elevator_init(struct request_queue *);
int elevator_init_mq(struct request_queue *q);
int elevator_switch_mq(struct request_queue *q,
			      struct elevator_type *new_e);
void elevator_exit(struct request_queue *, struct elevator_queue *);
int elv_register_queue(struct request_queue *q);
void elv_unregister_queue(struct request_queue *q);

struct hd_struct *__disk_get_part(struct gendisk *disk, int partno);

#ifdef CONFIG_FAIL_IO_TIMEOUT
int blk_should_fake_timeout(struct request_queue *);
ssize_t part_timeout_show(struct device *, struct device_attribute *, char *);
ssize_t part_timeout_store(struct device *, struct device_attribute *,
				const char *, size_t);
#else
static inline int blk_should_fake_timeout(struct request_queue *q)
{
	return 0;
}
#endif

int ll_back_merge_fn(struct request_queue *q, struct request *req,
		     struct bio *bio);
int ll_front_merge_fn(struct request_queue *q, struct request *req, 
		      struct bio *bio);
struct request *attempt_back_merge(struct request_queue *q, struct request *rq);
struct request *attempt_front_merge(struct request_queue *q, struct request *rq);
int blk_attempt_req_merge(struct request_queue *q, struct request *rq,
				struct request *next);
void blk_recalc_rq_segments(struct request *rq);
=======
int elevator_switch_mq(struct request_queue *q,
			      struct elevator_type *new_e);
void __elevator_exit(struct request_queue *, struct elevator_queue *);
int elv_register_queue(struct request_queue *q, bool uevent);
void elv_unregister_queue(struct request_queue *q);

static inline void elevator_exit(struct request_queue *q,
		struct elevator_queue *e)
{
	lockdep_assert_held(&q->sysfs_lock);

	blk_mq_sched_free_requests(q);
	__elevator_exit(q, e);
}

ssize_t part_size_show(struct device *dev, struct device_attribute *attr,
		char *buf);
ssize_t part_stat_show(struct device *dev, struct device_attribute *attr,
		char *buf);
ssize_t part_inflight_show(struct device *dev, struct device_attribute *attr,
		char *buf);
ssize_t part_fail_show(struct device *dev, struct device_attribute *attr,
		char *buf);
ssize_t part_fail_store(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count);
ssize_t part_timeout_show(struct device *, struct device_attribute *, char *);
ssize_t part_timeout_store(struct device *, struct device_attribute *,
				const char *, size_t);

void __blk_queue_split(struct bio **bio, unsigned int *nr_segs);
int ll_back_merge_fn(struct request *req, struct bio *bio,
		unsigned int nr_segs);
bool blk_attempt_req_merge(struct request_queue *q, struct request *rq,
				struct request *next);
unsigned int blk_recalc_rq_segments(struct request *rq);
>>>>>>> upstream/android-13
void blk_rq_set_mixed_merge(struct request *rq);
bool blk_rq_merge_ok(struct request *rq, struct bio *bio);
enum elv_merge blk_try_merge(struct request *rq, struct bio *bio);

<<<<<<< HEAD
void blk_queue_congestion_threshold(struct request_queue *q);

int blk_dev_init(void);


/*
 * Return the threshold (number of used requests) at which the queue is
 * considered to be congested.  It include a little hysteresis to keep the
 * context switch rate down.
 */
static inline int queue_congestion_on_threshold(struct request_queue *q)
{
	return q->nr_congestion_on;
}

/*
 * The threshold at which a queue is considered to be uncongested
 */
static inline int queue_congestion_off_threshold(struct request_queue *q)
{
	return q->nr_congestion_off;
}

extern int blk_update_nr_requests(struct request_queue *, unsigned int);

=======
int blk_dev_init(void);

>>>>>>> upstream/android-13
/*
 * Contribute to IO statistics IFF:
 *
 *	a) it's attached to a gendisk, and
<<<<<<< HEAD
 *	b) the queue had IO stats enabled when this request was started, and
 *	c) it's a file system request
 */
static inline bool blk_do_io_stat(struct request *rq)
{
	return rq->rq_disk &&
	       (rq->rq_flags & RQF_IO_STAT) &&
		!blk_rq_is_passthrough(rq);
=======
 *	b) the queue had IO stats enabled when this request was started
 */
static inline bool blk_do_io_stat(struct request *rq)
{
	return rq->rq_disk && (rq->rq_flags & RQF_IO_STAT);
>>>>>>> upstream/android-13
}

static inline void req_set_nomerge(struct request_queue *q, struct request *req)
{
	req->cmd_flags |= REQ_NOMERGE;
	if (req == q->last_merge)
		q->last_merge = NULL;
}

/*
<<<<<<< HEAD
 * Steal a bit from this field for legacy IO path atomic IO marking. Note that
 * setting the deadline clears the bottom bit, potentially clearing the
 * completed bit. The user has to be OK with this (current ones are fine).
 */
static inline void blk_rq_set_deadline(struct request *rq, unsigned long time)
{
	rq->__deadline = time & ~0x1UL;
}

static inline unsigned long blk_rq_deadline(struct request *rq)
{
	return rq->__deadline & ~0x1UL;
}

/*
=======
>>>>>>> upstream/android-13
 * The max size one bio can handle is UINT_MAX becasue bvec_iter.bi_size
 * is defined as 'unsigned int', meantime it has to aligned to with logical
 * block size which is the minimum accepted unit by hardware.
 */
static inline unsigned int bio_allowed_max_sectors(struct request_queue *q)
{
	return round_down(UINT_MAX, queue_logical_block_size(q)) >> 9;
}

/*
<<<<<<< HEAD
=======
 * The max bio size which is aligned to q->limits.discard_granularity. This
 * is a hint to split large discard bio in generic block layer, then if device
 * driver needs to split the discard bio into smaller ones, their bi_size can
 * be very probably and easily aligned to discard_granularity of the device's
 * queue.
 */
static inline unsigned int bio_aligned_discard_max_sectors(
					struct request_queue *q)
{
	return round_down(UINT_MAX, q->limits.discard_granularity) >>
			SECTOR_SHIFT;
}

/*
>>>>>>> upstream/android-13
 * Internal io_context interface
 */
void get_io_context(struct io_context *ioc);
struct io_cq *ioc_lookup_icq(struct io_context *ioc, struct request_queue *q);
struct io_cq *ioc_create_icq(struct io_context *ioc, struct request_queue *q,
			     gfp_t gfp_mask);
void ioc_clear_queue(struct request_queue *q);

int create_task_io_context(struct task_struct *task, gfp_t gfp_mask, int node);

<<<<<<< HEAD
/**
 * rq_ioc - determine io_context for request allocation
 * @bio: request being allocated is for this bio (can be %NULL)
 *
 * Determine io_context to use for request allocation for @bio.  May return
 * %NULL if %current->io_context doesn't exist.
 */
static inline struct io_context *rq_ioc(struct bio *bio)
{
#ifdef CONFIG_BLK_CGROUP
	if (bio && bio->bi_ioc)
		return bio->bi_ioc;
#endif
	return current->io_context;
}

/**
 * create_io_context - try to create task->io_context
 * @gfp_mask: allocation mask
 * @node: allocation node
 *
 * If %current->io_context is %NULL, allocate a new io_context and install
 * it.  Returns the current %current->io_context which may be %NULL if
 * allocation failed.
 *
 * Note that this function can't be called with IRQ disabled because
 * task_lock which protects %current->io_context is IRQ-unsafe.
 */
static inline struct io_context *create_io_context(gfp_t gfp_mask, int node)
{
	WARN_ON_ONCE(irqs_disabled());
	if (unlikely(!current->io_context))
		create_task_io_context(current, gfp_mask, node);
	return current->io_context;
}

=======
>>>>>>> upstream/android-13
/*
 * Internal throttling interface
 */
#ifdef CONFIG_BLK_DEV_THROTTLING
<<<<<<< HEAD
extern void blk_throtl_drain(struct request_queue *q);
extern int blk_throtl_init(struct request_queue *q);
extern void blk_throtl_exit(struct request_queue *q);
extern void blk_throtl_register_queue(struct request_queue *q);
#else /* CONFIG_BLK_DEV_THROTTLING */
static inline void blk_throtl_drain(struct request_queue *q) { }
static inline int blk_throtl_init(struct request_queue *q) { return 0; }
static inline void blk_throtl_exit(struct request_queue *q) { }
static inline void blk_throtl_register_queue(struct request_queue *q) { }
=======
extern int blk_throtl_init(struct request_queue *q);
extern void blk_throtl_exit(struct request_queue *q);
extern void blk_throtl_register_queue(struct request_queue *q);
extern void blk_throtl_charge_bio_split(struct bio *bio);
bool blk_throtl_bio(struct bio *bio);
#else /* CONFIG_BLK_DEV_THROTTLING */
static inline int blk_throtl_init(struct request_queue *q) { return 0; }
static inline void blk_throtl_exit(struct request_queue *q) { }
static inline void blk_throtl_register_queue(struct request_queue *q) { }
static inline void blk_throtl_charge_bio_split(struct bio *bio) { }
static inline bool blk_throtl_bio(struct bio *bio) { return false; }
>>>>>>> upstream/android-13
#endif /* CONFIG_BLK_DEV_THROTTLING */
#ifdef CONFIG_BLK_DEV_THROTTLING_LOW
extern ssize_t blk_throtl_sample_time_show(struct request_queue *q, char *page);
extern ssize_t blk_throtl_sample_time_store(struct request_queue *q,
	const char *page, size_t count);
extern void blk_throtl_bio_endio(struct bio *bio);
extern void blk_throtl_stat_add(struct request *rq, u64 time);
#else
static inline void blk_throtl_bio_endio(struct bio *bio) { }
static inline void blk_throtl_stat_add(struct request *rq, u64 time) { }
#endif

<<<<<<< HEAD
#ifdef CONFIG_BOUNCE
extern int init_emergency_isa_pool(void);
extern void blk_queue_bounce(struct request_queue *q, struct bio **bio);
#else
static inline int init_emergency_isa_pool(void)
{
	return 0;
}
static inline void blk_queue_bounce(struct request_queue *q, struct bio **bio)
{
}
#endif /* CONFIG_BOUNCE */

extern void blk_drain_queue(struct request_queue *q);
=======
void __blk_queue_bounce(struct request_queue *q, struct bio **bio);

static inline bool blk_queue_may_bounce(struct request_queue *q)
{
	return IS_ENABLED(CONFIG_BOUNCE) &&
		q->limits.bounce == BLK_BOUNCE_HIGH &&
		max_low_pfn >= max_pfn;
}

static inline void blk_queue_bounce(struct request_queue *q, struct bio **bio)
{
	if (unlikely(blk_queue_may_bounce(q) && bio_has_data(*bio)))
		__blk_queue_bounce(q, bio);	
}
>>>>>>> upstream/android-13

#ifdef CONFIG_BLK_CGROUP_IOLATENCY
extern int blk_iolatency_init(struct request_queue *q);
#else
static inline int blk_iolatency_init(struct request_queue *q) { return 0; }
#endif

<<<<<<< HEAD
=======
struct bio *blk_next_bio(struct bio *bio, unsigned int nr_pages, gfp_t gfp);

#ifdef CONFIG_BLK_DEV_ZONED
void blk_queue_free_zone_bitmaps(struct request_queue *q);
void blk_queue_clear_zone_settings(struct request_queue *q);
#else
static inline void blk_queue_free_zone_bitmaps(struct request_queue *q) {}
static inline void blk_queue_clear_zone_settings(struct request_queue *q) {}
#endif

int blk_alloc_ext_minor(void);
void blk_free_ext_minor(unsigned int minor);
#define ADDPART_FLAG_NONE	0
#define ADDPART_FLAG_RAID	1
#define ADDPART_FLAG_WHOLEDISK	2
int bdev_add_partition(struct gendisk *disk, int partno, sector_t start,
		sector_t length);
int bdev_del_partition(struct gendisk *disk, int partno);
int bdev_resize_partition(struct gendisk *disk, int partno, sector_t start,
		sector_t length);

int bio_add_hw_page(struct request_queue *q, struct bio *bio,
		struct page *page, unsigned int len, unsigned int offset,
		unsigned int max_sectors, bool *same_page);

struct request_queue *blk_alloc_queue(int node_id);

int disk_alloc_events(struct gendisk *disk);
void disk_add_events(struct gendisk *disk);
void disk_del_events(struct gendisk *disk);
void disk_release_events(struct gendisk *disk);
extern struct device_attribute dev_attr_events;
extern struct device_attribute dev_attr_events_async;
extern struct device_attribute dev_attr_events_poll_msecs;

static inline void bio_clear_hipri(struct bio *bio)
{
	/* can't support alloc cache if we turn off polling */
	bio_clear_flag(bio, BIO_PERCPU_CACHE);
	bio->bi_opf &= ~REQ_HIPRI;
}

extern const struct address_space_operations def_blk_aops;

>>>>>>> upstream/android-13
#endif /* BLK_INTERNAL_H */
