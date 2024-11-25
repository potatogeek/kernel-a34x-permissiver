/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BLKDEV_H
#define _LINUX_BLKDEV_H

#include <linux/sched.h>
#include <linux/sched/clock.h>

#ifdef CONFIG_BLOCK

#include <linux/major.h>
#include <linux/genhd.h>
#include <linux/list.h>
#include <linux/llist.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/pagemap.h>
#include <linux/backing-dev-defs.h>
#include <linux/wait.h>
#include <linux/mempool.h>
#include <linux/pfn.h>
#include <linux/bio.h>
#include <linux/stringify.h>
#include <linux/gfp.h>
#include <linux/bsg.h>
#include <linux/smp.h>
#include <linux/rcupdate.h>
#include <linux/percpu-refcount.h>
#include <linux/scatterlist.h>
#include <linux/blkzoned.h>

struct module;
struct scsi_ioctl_command;

struct request_queue;
struct elevator_queue;
struct blk_trace;
struct request;
struct sg_io_hdr;
struct bsg_job;
struct blkcg_gq;
struct blk_flush_queue;
struct pr_ops;
struct rq_qos;
struct blk_queue_stats;
struct blk_stat_callback;
struct keyslot_manager;

#define BLKDEV_MIN_RQ	4
#ifdef CONFIG_LARGE_DIRTY_BUFFER
#define BLKDEV_MAX_RQ	256
#else
#define BLKDEV_MAX_RQ  128     
#endif


#define BLK_MQ_POLL_STATS_BKTS 16


#define BLKCG_MAX_POLS		5

typedef void (rq_end_io_fn)(struct request *, blk_status_t);

#define BLK_RL_SYNCFULL		(1U << 0)
#define BLK_RL_ASYNCFULL	(1U << 1)

struct request_list {
	struct request_queue	*q;	
#ifdef CONFIG_BLK_CGROUP
	struct blkcg_gq		*blkg;	
#endif
	
	int			count[2];
	int			starved[2];
	mempool_t		*rq_pool;
	wait_queue_head_t	wait[2];
	unsigned int		flags;
};


typedef __u32 __bitwise req_flags_t;


#define RQF_SORTED		((__force req_flags_t)(1 << 0))

#define RQF_STARTED		((__force req_flags_t)(1 << 1))

#define RQF_QUEUED		((__force req_flags_t)(1 << 2))

#define RQF_SOFTBARRIER		((__force req_flags_t)(1 << 3))

#define RQF_FLUSH_SEQ		((__force req_flags_t)(1 << 4))

#define RQF_MIXED_MERGE		((__force req_flags_t)(1 << 5))

#define RQF_MQ_INFLIGHT		((__force req_flags_t)(1 << 6))

#define RQF_DONTPREP		((__force req_flags_t)(1 << 7))

#define RQF_PREEMPT		((__force req_flags_t)(1 << 8))

#define RQF_COPY_USER		((__force req_flags_t)(1 << 9))

#define RQF_FAILED		((__force req_flags_t)(1 << 10))

#define RQF_QUIET		((__force req_flags_t)(1 << 11))

#define RQF_ELVPRIV		((__force req_flags_t)(1 << 12))

#define RQF_IO_STAT		((__force req_flags_t)(1 << 13))

#define RQF_ALLOCED		((__force req_flags_t)(1 << 14))

#define RQF_PM			((__force req_flags_t)(1 << 15))

#define RQF_HASHED		((__force req_flags_t)(1 << 16))

#define RQF_STATS		((__force req_flags_t)(1 << 17))

#define RQF_SPECIAL_PAYLOAD	((__force req_flags_t)(1 << 18))

#define RQF_ZONE_WRITE_LOCKED	((__force req_flags_t)(1 << 19))

#define RQF_MQ_POLL_SLEPT	((__force req_flags_t)(1 << 20))

#define RQF_TIMED_OUT		((__force req_flags_t)(1 << 21))


#define RQF_NOMERGE_FLAGS \
	(RQF_STARTED | RQF_SOFTBARRIER | RQF_FLUSH_SEQ | RQF_SPECIAL_PAYLOAD)


enum mq_rq_state {
	MQ_RQ_IDLE		= 0,
	MQ_RQ_IN_FLIGHT		= 1,
	MQ_RQ_COMPLETE		= 2,
};


struct request {
	struct request_queue *q;
	struct blk_mq_ctx *mq_ctx;

	int cpu;
	unsigned int cmd_flags;		
	req_flags_t rq_flags;

	int internal_tag;

	
	unsigned int __data_len;	
	int tag;
	sector_t __sector;		

	struct bio *bio;
	struct bio *biotail;

	struct list_head queuelist;

	
	union {
		struct hlist_node hash;	
		struct list_head ipi_list;
	};

	
	union {
		struct rb_node rb_node;	
		struct bio_vec special_vec;
		void *completion_data;
		int error_count; 
	};

	
	union {
		struct {
			struct io_cq		*icq;
			void			*priv[2];
		} elv;

		struct {
			unsigned int		seq;
			struct list_head	list;
			rq_end_io_fn		*saved_end_io;
		} flush;
	};

	struct gendisk *rq_disk;
	struct hd_struct *part;
	
	u64 start_time_ns;
	
	u64 io_start_time_ns;

#ifdef CONFIG_BLK_WBT
	unsigned short wbt_flags;
#endif
#ifdef CONFIG_BLK_DEV_THROTTLING_LOW
	unsigned short throtl_size;
#endif

	
	unsigned short nr_phys_segments;

#if defined(CONFIG_BLK_DEV_INTEGRITY)
	unsigned short nr_integrity_segments;
#endif

	unsigned short write_hint;
	unsigned short ioprio;

	void *special;		

	unsigned int extra_len;	

	enum mq_rq_state state;
	refcount_t ref;

	unsigned int timeout;

	
	unsigned long __deadline;

	struct list_head timeout_list;

	union {
		struct __call_single_data csd;
		u64 fifo_time;
	};

	
	rq_end_io_fn *end_io;
	void *end_io_data;

	
	struct request *next_rq;

#ifdef CONFIG_BLK_CGROUP
	struct request_list *rl;		
#endif
};

static inline bool blk_op_is_scsi(unsigned int op)
{
	return op == REQ_OP_SCSI_IN || op == REQ_OP_SCSI_OUT;
}

static inline bool blk_op_is_private(unsigned int op)
{
	return op == REQ_OP_DRV_IN || op == REQ_OP_DRV_OUT;
}

static inline bool blk_rq_is_scsi(struct request *rq)
{
	return blk_op_is_scsi(req_op(rq));
}

static inline bool blk_rq_is_private(struct request *rq)
{
	return blk_op_is_private(req_op(rq));
}

static inline bool blk_rq_is_passthrough(struct request *rq)
{
	return blk_rq_is_scsi(rq) || blk_rq_is_private(rq);
}

static inline bool bio_is_passthrough(struct bio *bio)
{
	unsigned op = bio_op(bio);

	return blk_op_is_scsi(op) || blk_op_is_private(op);
}

static inline unsigned short req_get_ioprio(struct request *req)
{
	return req->ioprio;
}

#include <linux/elevator.h>

struct blk_queue_ctx;

typedef void (request_fn_proc) (struct request_queue *q);
typedef blk_qc_t (make_request_fn) (struct request_queue *q, struct bio *bio);
typedef bool (poll_q_fn) (struct request_queue *q, blk_qc_t);
typedef int (prep_rq_fn) (struct request_queue *, struct request *);
typedef void (unprep_rq_fn) (struct request_queue *, struct request *);

struct bio_vec;
typedef void (softirq_done_fn)(struct request *);
typedef int (dma_drain_needed_fn)(struct request *);
typedef int (lld_busy_fn) (struct request_queue *q);
typedef int (bsg_job_fn) (struct bsg_job *);
typedef int (init_rq_fn)(struct request_queue *, struct request *, gfp_t);
typedef void (exit_rq_fn)(struct request_queue *, struct request *);

enum blk_eh_timer_return {
	BLK_EH_DONE,		
	BLK_EH_RESET_TIMER,	
};

typedef enum blk_eh_timer_return (rq_timed_out_fn)(struct request *);

enum blk_queue_state {
	Queue_down,
	Queue_up,
};

struct blk_queue_tag {
	struct request **tag_index;	
	unsigned long *tag_map;		
	int max_depth;			
	int real_max_depth;		
	atomic_t refcnt;		
	int alloc_policy;		
	int next_tag;			
};
#define BLK_TAG_ALLOC_FIFO 0 
#define BLK_TAG_ALLOC_RR 1 

#define BLK_SCSI_MAX_CMDS	(256)
#define BLK_SCSI_CMD_PER_LONG	(BLK_SCSI_MAX_CMDS / (sizeof(long) * 8))


enum blk_zoned_model {
	BLK_ZONED_NONE,	
	BLK_ZONED_HA,	
	BLK_ZONED_HM,	
};

struct queue_limits {
	unsigned long		bounce_pfn;
	unsigned long		seg_boundary_mask;
	unsigned long		virt_boundary_mask;

	unsigned int		max_hw_sectors;
	unsigned int		max_dev_sectors;
	unsigned int		chunk_sectors;
	unsigned int		max_sectors;
	unsigned int		max_segment_size;
	unsigned int		physical_block_size;
	unsigned int		logical_block_size;
	unsigned int		alignment_offset;
	unsigned int		io_min;
	unsigned int		io_opt;
	unsigned int		max_discard_sectors;
	unsigned int		max_hw_discard_sectors;
	unsigned int		max_write_same_sectors;
	unsigned int		max_write_zeroes_sectors;
	unsigned int		discard_granularity;
	unsigned int		discard_alignment;

	unsigned short		max_segments;
	unsigned short		max_integrity_segments;
	unsigned short		max_discard_segments;

	unsigned char		misaligned;
	unsigned char		discard_misaligned;
	unsigned char		cluster;
	unsigned char		raid_partial_stripes_expensive;
	enum blk_zoned_model	zoned;
};

#ifdef CONFIG_BLK_DEV_ZONED

struct blk_zone_report_hdr {
	unsigned int	nr_zones;
	u8		padding[60];
};

extern int blkdev_report_zones(struct block_device *bdev,
			       sector_t sector, struct blk_zone *zones,
			       unsigned int *nr_zones, gfp_t gfp_mask);
extern int blkdev_reset_zones(struct block_device *bdev, sector_t sectors,
			      sector_t nr_sectors, gfp_t gfp_mask);

extern int blkdev_report_zones_ioctl(struct block_device *bdev, fmode_t mode,
				     unsigned int cmd, unsigned long arg);
extern int blkdev_reset_zones_ioctl(struct block_device *bdev, fmode_t mode,
				    unsigned int cmd, unsigned long arg);

#else 

static inline int blkdev_report_zones_ioctl(struct block_device *bdev,
					    fmode_t mode, unsigned int cmd,
					    unsigned long arg)
{
	return -ENOTTY;
}

static inline int blkdev_reset_zones_ioctl(struct block_device *bdev,
					   fmode_t mode, unsigned int cmd,
					   unsigned long arg)
{
	return -ENOTTY;
}

#endif 

#ifdef CONFIG_BLK_IO_VOLUME
struct block_io_volume {
	int			queuing_rqs;
	long long		queuing_bytes;

	
	unsigned int		peak_rqs;
	unsigned int		peak_rqs_cnt[4];
	unsigned int		peak_bytes;
	unsigned int		peak_bytes_cnt[4];
};

// WRITE : 1, READ : 0
#define BLK_MAX_IO_VOLS	2
#define blk_io_vol_rqs(q, op)		((q)->blk_io_vol[(op)&1].queuing_rqs)
#define blk_io_vol_bytes(q, op)		((q)->blk_io_vol[(op)&1].queuing_bytes)
#else
#define blk_io_vol_rqs(q, op)		do {} while (0)
#define blk_io_vol_bytes(q, op)		do {} while (0)
#endif

#ifdef CONFIG_BLK_TURBO_WRITE
typedef void (blk_tw_try_on_fn) (struct request_queue *q);
typedef void (blk_tw_try_off_fn) (struct request_queue *q);

enum blk_tw_state {
	TW_OFF = 0,
	TW_ON_READY,
	TW_OFF_READY,
	TW_ON,

	NR_TW_STATE
};

struct blk_turbo_write {
	enum blk_tw_state	state;
	unsigned long		state_ts;

	long long		up_threshold_bytes;
	int			up_threshold_rqs;
	long long		down_threshold_bytes;
	int			down_threshold_rqs;

	
	int			on_delay;
	
	int			off_delay;

	
	unsigned long		prev_on_ready_ts;
	
	int			on_interval;

	blk_tw_try_on_fn	*try_on;
	blk_tw_try_off_fn	*try_off;

	
	int			curr_issued_kb;
	
	unsigned int		total_issued_mb;
	
	unsigned int		issued_size_cnt[4];
};

int blk_alloc_turbo_write(struct request_queue *q);
void blk_free_turbo_write(struct request_queue *q);
int blk_register_tw_try_on_fn(struct request_queue *q,
	blk_tw_try_on_fn *fn);
int blk_register_tw_try_off_fn(struct request_queue *q,
	blk_tw_try_off_fn *fn);
int blk_reset_tw_state(struct request_queue *q);
void blk_update_tw_state(struct request_queue *q,
	int write_rqs, long long write_bytes);
void blk_account_tw_io(struct request_queue *q,
	int opf, int bytes);
#else
#define blk_alloc_turbo_write(q)			do {} while (0)
#define blk_free_turbo_write(q)				do {} while (0)
#define blk_register_tw_enable_fn(q, fn)		do {} while (0)
#define blk_register_tw_disable_fn(q, fn)		do {} while (0)
#define blk_reset_tw_state(q)				do {} while (0)
#define blk_update_tw_state(q, write_rqs, write_bytes)	do {} while (0)
#define blk_account_tw_io(q, opf, bytes)		do {} while (0)
#endif

struct request_queue {
	
	struct list_head	queue_head;
	struct request		*last_merge;
	struct elevator_queue	*elevator;
	int			nr_rqs[2];	
	int			nr_rqs_elvpriv;	

	struct blk_queue_stats	*stats;
	struct rq_qos		*rq_qos;

	
	struct request_list	root_rl;

	request_fn_proc		*request_fn;
	make_request_fn		*make_request_fn;
	poll_q_fn		*poll_fn;
	prep_rq_fn		*prep_rq_fn;
	unprep_rq_fn		*unprep_rq_fn;
	softirq_done_fn		*softirq_done_fn;
	rq_timed_out_fn		*rq_timed_out_fn;
	dma_drain_needed_fn	*dma_drain_needed;
	lld_busy_fn		*lld_busy_fn;
	
	init_rq_fn		*init_rq_fn;
	
	exit_rq_fn		*exit_rq_fn;
	
	void (*initialize_rq_fn)(struct request *rq);

	const struct blk_mq_ops	*mq_ops;

	unsigned int		*mq_map;

	
	struct blk_mq_ctx __percpu	*queue_ctx;
	unsigned int		nr_queues;

	unsigned int		queue_depth;

	
	struct blk_mq_hw_ctx	**queue_hw_ctx;
	unsigned int		nr_hw_queues;

	
	sector_t		end_sector;
	struct request		*boundary_rq;

	
	struct delayed_work	delay_work;

	struct backing_dev_info	*backing_dev_info;

	
	void			*queuedata;

	
	unsigned long		queue_flags;
	
	atomic_t		pm_only;

	
	int			id;

	
	gfp_t			bounce_gfp;

	
	spinlock_t		__queue_lock;
	spinlock_t		*queue_lock;

	
	struct kobject kobj;

	
	struct kobject mq_kobj;

#ifdef  CONFIG_BLK_DEV_INTEGRITY
	struct blk_integrity integrity;
#endif	

#ifdef CONFIG_PM
	struct device		*dev;
	int			rpm_status;
	unsigned int		nr_pending;
#endif

	
	unsigned long		nr_requests;	
	unsigned int		nr_congestion_on;
	unsigned int		nr_congestion_off;
	unsigned int		nr_batching;

	unsigned int		dma_drain_size;
	void			*dma_drain_buffer;
	unsigned int		dma_pad_mask;
	unsigned int		dma_alignment;

	struct blk_queue_tag	*queue_tags;

	unsigned int		nr_sorted;
	unsigned int		in_flight[2];
	unsigned long long	in_flight_time;
	ktime_t			in_flight_stamp;

	
	unsigned int		request_fn_active;
#ifdef CONFIG_BLK_INLINE_ENCRYPTION
	
	struct keyslot_manager *ksm;
#endif

	unsigned int		rq_timeout;
	int			poll_nsec;

	struct blk_stat_callback	*poll_cb;
	struct blk_rq_stat	poll_stat[BLK_MQ_POLL_STATS_BKTS];

	struct timer_list	timeout;
	struct work_struct	timeout_work;
	struct list_head	timeout_list;

	struct list_head	icq_list;
#ifdef CONFIG_BLK_CGROUP
	DECLARE_BITMAP		(blkcg_pols, BLKCG_MAX_POLS);
	struct blkcg_gq		*root_blkg;
	struct list_head	blkg_list;
#endif

	struct queue_limits	limits;

#ifdef CONFIG_BLK_DEV_ZONED
	
	unsigned int		nr_zones;
	unsigned long		*seq_zones_bitmap;
	unsigned long		*seq_zones_wlock;
#endif 

	
	unsigned int		sg_timeout;
	unsigned int		sg_reserved_size;
	int			node;
#ifdef CONFIG_BLK_DEV_IO_TRACE
	struct blk_trace __rcu	*blk_trace;
	struct mutex		blk_trace_mutex;
#endif
	
	struct blk_flush_queue	*fq;
	unsigned long		flush_ios;

	struct list_head	requeue_list;
	spinlock_t		requeue_lock;
	struct delayed_work	requeue_work;

	struct mutex		sysfs_lock;

	int			bypass_depth;
	atomic_t		mq_freeze_depth;

	bsg_job_fn		*bsg_job_fn;
	struct bsg_class_device bsg_dev;

#ifdef CONFIG_BLK_DEV_THROTTLING
	
	struct throtl_data *td;
#endif
	struct rcu_head		rcu_head;
	wait_queue_head_t	mq_freeze_wq;
	struct percpu_ref	q_usage_counter;
	struct list_head	all_q_node;

	struct blk_mq_tag_set	*tag_set;
	struct list_head	tag_set_list;
	struct bio_set		bio_split;

#ifdef CONFIG_BLK_DEBUG_FS
	struct dentry		*debugfs_dir;
	struct dentry		*sched_debugfs_dir;
#endif

	bool			mq_sysfs_init_done;

	size_t			cmd_size;
	void			*rq_alloc_data;

	struct work_struct	release_work;

#define BLK_MAX_WRITE_HINTS	5
	u64			write_hints[BLK_MAX_WRITE_HINTS];

#ifdef CONFIG_BLK_IO_VOLUME
        struct block_io_volume  blk_io_vol[BLK_MAX_IO_VOLS];
#endif

#ifdef CONFIG_BLK_TURBO_WRITE
        struct blk_turbo_write  *tw;
#endif

#ifdef CONFIG_SCSI_UFS_TW
	bool			turbo_write_dev;
#endif
};

#define QUEUE_FLAG_QUEUED	0	
#define QUEUE_FLAG_STOPPED	1	
#define QUEUE_FLAG_DYING	2	
#define QUEUE_FLAG_BYPASS	3	
#define QUEUE_FLAG_BIDI		4	
#define QUEUE_FLAG_NOMERGES     5	
#define QUEUE_FLAG_SAME_COMP	6	
#define QUEUE_FLAG_FAIL_IO	7	
#define QUEUE_FLAG_NONROT	9	
#define QUEUE_FLAG_VIRT        QUEUE_FLAG_NONROT 
#define QUEUE_FLAG_IO_STAT     10	
#define QUEUE_FLAG_DISCARD     11	
#define QUEUE_FLAG_NOXMERGES   12	
#define QUEUE_FLAG_ADD_RANDOM  13	
#define QUEUE_FLAG_SECERASE    14	
#define QUEUE_FLAG_SAME_FORCE  15	
#define QUEUE_FLAG_DEAD        16	
#define QUEUE_FLAG_INIT_DONE   17	
#define QUEUE_FLAG_NO_SG_MERGE 18	
#define QUEUE_FLAG_POLL	       19	
#define QUEUE_FLAG_WC	       20	
#define QUEUE_FLAG_FUA	       21	
#define QUEUE_FLAG_FLUSH_NQ    22	
#define QUEUE_FLAG_DAX         23	
#define QUEUE_FLAG_STATS       24	
#define QUEUE_FLAG_POLL_STATS  25	
#define QUEUE_FLAG_REGISTERED  26	
#define QUEUE_FLAG_SCSI_PASSTHROUGH 27	
#define QUEUE_FLAG_QUIESCED    28	

#define QUEUE_FLAG_DEFAULT	((1 << QUEUE_FLAG_IO_STAT) |		\
				 (1 << QUEUE_FLAG_SAME_COMP)	|	\
				 (1 << QUEUE_FLAG_ADD_RANDOM))

#define QUEUE_FLAG_MQ_DEFAULT	((1 << QUEUE_FLAG_IO_STAT) |		\
				 (1 << QUEUE_FLAG_SAME_COMP)	|	\
				 (1 << QUEUE_FLAG_POLL))

void blk_queue_flag_set(unsigned int flag, struct request_queue *q);
void blk_queue_flag_clear(unsigned int flag, struct request_queue *q);
bool blk_queue_flag_test_and_set(unsigned int flag, struct request_queue *q);
bool blk_queue_flag_test_and_clear(unsigned int flag, struct request_queue *q);

#define blk_queue_tagged(q)	test_bit(QUEUE_FLAG_QUEUED, &(q)->queue_flags)
#define blk_queue_stopped(q)	test_bit(QUEUE_FLAG_STOPPED, &(q)->queue_flags)
#define blk_queue_dying(q)	test_bit(QUEUE_FLAG_DYING, &(q)->queue_flags)
#define blk_queue_dead(q)	test_bit(QUEUE_FLAG_DEAD, &(q)->queue_flags)
#define blk_queue_bypass(q)	test_bit(QUEUE_FLAG_BYPASS, &(q)->queue_flags)
#define blk_queue_init_done(q)	test_bit(QUEUE_FLAG_INIT_DONE, &(q)->queue_flags)
#define blk_queue_nomerges(q)	test_bit(QUEUE_FLAG_NOMERGES, &(q)->queue_flags)
#define blk_queue_noxmerges(q)	\
	test_bit(QUEUE_FLAG_NOXMERGES, &(q)->queue_flags)
#define blk_queue_nonrot(q)	test_bit(QUEUE_FLAG_NONROT, &(q)->queue_flags)
#define blk_queue_io_stat(q)	test_bit(QUEUE_FLAG_IO_STAT, &(q)->queue_flags)
#define blk_queue_add_random(q)	test_bit(QUEUE_FLAG_ADD_RANDOM, &(q)->queue_flags)
#define blk_queue_discard(q)	test_bit(QUEUE_FLAG_DISCARD, &(q)->queue_flags)
#define blk_queue_secure_erase(q) \
	(test_bit(QUEUE_FLAG_SECERASE, &(q)->queue_flags))
#define blk_queue_dax(q)	test_bit(QUEUE_FLAG_DAX, &(q)->queue_flags)
#define blk_queue_scsi_passthrough(q)	\
	test_bit(QUEUE_FLAG_SCSI_PASSTHROUGH, &(q)->queue_flags)

#define blk_noretry_request(rq) \
	((rq)->cmd_flags & (REQ_FAILFAST_DEV|REQ_FAILFAST_TRANSPORT| \
			     REQ_FAILFAST_DRIVER))
#define blk_queue_quiesced(q)	test_bit(QUEUE_FLAG_QUIESCED, &(q)->queue_flags)
#define blk_queue_pm_only(q)	atomic_read(&(q)->pm_only)
#define blk_queue_fua(q)	test_bit(QUEUE_FLAG_FUA, &(q)->queue_flags)
#define blk_queue_registered(q)	test_bit(QUEUE_FLAG_REGISTERED, &(q)->queue_flags)

extern void blk_set_pm_only(struct request_queue *q);
extern void blk_clear_pm_only(struct request_queue *q);

static inline int queue_in_flight(struct request_queue *q)
{
	return q->in_flight[0] + q->in_flight[1];
}

static inline bool blk_account_rq(struct request *rq)
{
	return (rq->rq_flags & RQF_STARTED) && !blk_rq_is_passthrough(rq);
}

#define blk_rq_cpu_valid(rq)	((rq)->cpu != -1)
#define blk_bidi_rq(rq)		((rq)->next_rq != NULL)

#define blk_queued_rq(rq)	(!list_empty(&(rq)->queuelist))

#define list_entry_rq(ptr)	list_entry((ptr), struct request, queuelist)

#define rq_data_dir(rq)		(op_is_write(req_op(rq)) ? WRITE : READ)


static inline bool queue_is_rq_based(struct request_queue *q)
{
	return q->request_fn || q->mq_ops;
}

static inline unsigned int blk_queue_cluster(struct request_queue *q)
{
	return q->limits.cluster;
}

static inline enum blk_zoned_model
blk_queue_zoned_model(struct request_queue *q)
{
	return q->limits.zoned;
}

static inline bool blk_queue_is_zoned(struct request_queue *q)
{
	switch (blk_queue_zoned_model(q)) {
	case BLK_ZONED_HA:
	case BLK_ZONED_HM:
		return true;
	default:
		return false;
	}
}

static inline unsigned int blk_queue_zone_sectors(struct request_queue *q)
{
	return blk_queue_is_zoned(q) ? q->limits.chunk_sectors : 0;
}

#ifdef CONFIG_BLK_DEV_ZONED
static inline unsigned int blk_queue_zone_no(struct request_queue *q,
					     sector_t sector)
{
	if (!blk_queue_is_zoned(q))
		return 0;
	return sector >> ilog2(q->limits.chunk_sectors);
}

static inline bool blk_queue_zone_is_seq(struct request_queue *q,
					 sector_t sector)
{
	if (!blk_queue_is_zoned(q) || !q->seq_zones_bitmap)
		return false;
	return test_bit(blk_queue_zone_no(q, sector), q->seq_zones_bitmap);
}
#endif 

static inline bool rq_is_sync(struct request *rq)
{
	return op_is_sync(rq->cmd_flags);
}

static inline bool blk_rl_full(struct request_list *rl, bool sync)
{
	unsigned int flag = sync ? BLK_RL_SYNCFULL : BLK_RL_ASYNCFULL;

	return rl->flags & flag;
}

static inline void blk_set_rl_full(struct request_list *rl, bool sync)
{
	unsigned int flag = sync ? BLK_RL_SYNCFULL : BLK_RL_ASYNCFULL;

	rl->flags |= flag;
}

static inline void blk_clear_rl_full(struct request_list *rl, bool sync)
{
	unsigned int flag = sync ? BLK_RL_SYNCFULL : BLK_RL_ASYNCFULL;

	rl->flags &= ~flag;
}

static inline bool rq_mergeable(struct request *rq)
{
	if (blk_rq_is_passthrough(rq))
		return false;

	if (req_op(rq) == REQ_OP_FLUSH)
		return false;

	if (req_op(rq) == REQ_OP_WRITE_ZEROES)
		return false;

	if (rq->cmd_flags & REQ_NOMERGE_FLAGS)
		return false;
	if (rq->rq_flags & RQF_NOMERGE_FLAGS)
		return false;

	return true;
}

static inline bool blk_write_same_mergeable(struct bio *a, struct bio *b)
{
	if (bio_page(a) == bio_page(b) &&
	    bio_offset(a) == bio_offset(b))
		return true;

	return false;
}

static inline unsigned int blk_queue_depth(struct request_queue *q)
{
	if (q->queue_depth)
		return q->queue_depth;

	return q->nr_requests;
}


enum {
	BLKPREP_OK,		
	BLKPREP_KILL,		
	BLKPREP_DEFER,		
	BLKPREP_INVALID,	
};

extern unsigned long blk_max_low_pfn, blk_max_pfn;



#if BITS_PER_LONG == 32
#define BLK_BOUNCE_HIGH		((u64)blk_max_low_pfn << PAGE_SHIFT)
#else
#define BLK_BOUNCE_HIGH		-1ULL
#endif
#define BLK_BOUNCE_ANY		(-1ULL)
#define BLK_BOUNCE_ISA		(DMA_BIT_MASK(24))


#define BLK_DEFAULT_SG_TIMEOUT	(60 * HZ)
#define BLK_MIN_SG_TIMEOUT	(7 * HZ)

struct rq_map_data {
	struct page **pages;
	int page_order;
	int nr_entries;
	unsigned long offset;
	int null_mapped;
	int from_user;
};

struct req_iterator {
	struct bvec_iter iter;
	struct bio *bio;
};


#define for_each_bio(_bio)		\
	for (; _bio; _bio = _bio->bi_next)
#define __rq_for_each_bio(_bio, rq)	\
	if ((rq->bio))			\
		for (_bio = (rq)->bio; _bio; _bio = _bio->bi_next)

#define rq_for_each_segment(bvl, _rq, _iter)			\
	__rq_for_each_bio(_iter.bio, _rq)			\
		bio_for_each_segment(bvl, _iter.bio, _iter.iter)

#define rq_iter_last(bvec, _iter)				\
		(_iter.bio->bi_next == NULL &&			\
		 bio_iter_last(bvec, _iter.iter))

#ifndef ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE
# error	"You should define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE for your platform"
#endif
#if ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE
extern void rq_flush_dcache_pages(struct request *rq);
#else
static inline void rq_flush_dcache_pages(struct request *rq)
{
}
#endif

extern int blk_register_queue(struct gendisk *disk);
extern void blk_unregister_queue(struct gendisk *disk);
extern blk_qc_t generic_make_request(struct bio *bio);
extern blk_qc_t direct_make_request(struct bio *bio);
extern void blk_rq_init(struct request_queue *q, struct request *rq);
extern void blk_init_request_from_bio(struct request *req, struct bio *bio);
extern void blk_put_request(struct request *);
extern void __blk_put_request(struct request_queue *, struct request *);
extern struct request *blk_get_request(struct request_queue *, unsigned int op,
				       blk_mq_req_flags_t flags);
extern void blk_requeue_request(struct request_queue *, struct request *);
extern int blk_lld_busy(struct request_queue *q);
extern int blk_rq_prep_clone(struct request *rq, struct request *rq_src,
			     struct bio_set *bs, gfp_t gfp_mask,
			     int (*bio_ctr)(struct bio *, struct bio *, void *),
			     void *data);
extern void blk_rq_unprep_clone(struct request *rq);
extern blk_status_t blk_insert_cloned_request(struct request_queue *q,
				     struct request *rq);
extern int blk_rq_append_bio(struct request *rq, struct bio **bio);
extern void blk_delay_queue(struct request_queue *, unsigned long);
extern void blk_queue_split(struct request_queue *, struct bio **);
extern void blk_recount_segments(struct request_queue *, struct bio *);
extern int scsi_verify_blk_ioctl(struct block_device *, unsigned int);
extern int scsi_cmd_blk_ioctl(struct block_device *, fmode_t,
			      unsigned int, void __user *);
extern int scsi_cmd_ioctl(struct request_queue *, struct gendisk *, fmode_t,
			  unsigned int, void __user *);
extern int sg_scsi_ioctl(struct request_queue *, struct gendisk *, fmode_t,
			 struct scsi_ioctl_command __user *);

extern int blk_queue_enter(struct request_queue *q, blk_mq_req_flags_t flags);
extern void blk_queue_exit(struct request_queue *q);
extern void blk_start_queue(struct request_queue *q);
extern void blk_start_queue_async(struct request_queue *q);
extern void blk_stop_queue(struct request_queue *q);
extern void blk_sync_queue(struct request_queue *q);
extern void __blk_stop_queue(struct request_queue *q);
extern void __blk_run_queue(struct request_queue *q);
extern void __blk_run_queue_uncond(struct request_queue *q);
extern void blk_run_queue(struct request_queue *);
extern void blk_run_queue_async(struct request_queue *q);
extern int blk_rq_map_user(struct request_queue *, struct request *,
			   struct rq_map_data *, void __user *, unsigned long,
			   gfp_t);
extern int blk_rq_unmap_user(struct bio *);
extern int blk_rq_map_kern(struct request_queue *, struct request *, void *, unsigned int, gfp_t);
extern int blk_rq_map_user_iov(struct request_queue *, struct request *,
			       struct rq_map_data *, const struct iov_iter *,
			       gfp_t);
extern void blk_execute_rq(struct request_queue *, struct gendisk *,
			  struct request *, int);
extern void blk_execute_rq_nowait(struct request_queue *, struct gendisk *,
				  struct request *, int, rq_end_io_fn *);

int blk_status_to_errno(blk_status_t status);
blk_status_t errno_to_blk_status(int errno);

bool blk_poll(struct request_queue *q, blk_qc_t cookie);

static inline struct request_queue *bdev_get_queue(struct block_device *bdev)
{
	return bdev->bd_disk->queue;	
}


#ifndef SECTOR_SHIFT
#define SECTOR_SHIFT 9
#endif
#ifndef SECTOR_SIZE
#define SECTOR_SIZE (1 << SECTOR_SHIFT)
#endif


static inline sector_t blk_rq_pos(const struct request *rq)
{
	return rq->__sector;
}

static inline unsigned int blk_rq_bytes(const struct request *rq)
{
	return rq->__data_len;
}

static inline int blk_rq_cur_bytes(const struct request *rq)
{
	return rq->bio ? bio_cur_bytes(rq->bio) : 0;
}

extern unsigned int blk_rq_err_bytes(const struct request *rq);

static inline unsigned int blk_rq_sectors(const struct request *rq)
{
	return blk_rq_bytes(rq) >> SECTOR_SHIFT;
}

static inline unsigned int blk_rq_cur_sectors(const struct request *rq)
{
	return blk_rq_cur_bytes(rq) >> SECTOR_SHIFT;
}

#ifdef CONFIG_BLK_DEV_ZONED
static inline unsigned int blk_rq_zone_no(struct request *rq)
{
	return blk_queue_zone_no(rq->q, blk_rq_pos(rq));
}

static inline unsigned int blk_rq_zone_is_seq(struct request *rq)
{
	return blk_queue_zone_is_seq(rq->q, blk_rq_pos(rq));
}
#endif 


static inline unsigned int blk_rq_payload_bytes(struct request *rq)
{
	if (rq->rq_flags & RQF_SPECIAL_PAYLOAD)
		return rq->special_vec.bv_len;
	return blk_rq_bytes(rq);
}

static inline unsigned int blk_queue_get_max_sectors(struct request_queue *q,
						     int op)
{
	if (unlikely(op == REQ_OP_DISCARD || op == REQ_OP_SECURE_ERASE))
		return min(q->limits.max_discard_sectors,
			   UINT_MAX >> SECTOR_SHIFT);

	if (unlikely(op == REQ_OP_WRITE_SAME))
		return q->limits.max_write_same_sectors;

	if (unlikely(op == REQ_OP_WRITE_ZEROES))
		return q->limits.max_write_zeroes_sectors;

	return q->limits.max_sectors;
}


static inline unsigned int blk_max_size_offset(struct request_queue *q,
					       sector_t offset)
{
	if (!q->limits.chunk_sectors)
		return q->limits.max_sectors;

	return min(q->limits.max_sectors, (unsigned int)(q->limits.chunk_sectors -
			(offset & (q->limits.chunk_sectors - 1))));
}

static inline unsigned int blk_rq_get_max_sectors(struct request *rq,
						  sector_t offset)
{
	struct request_queue *q = rq->q;

	if (blk_rq_is_passthrough(rq))
		return q->limits.max_hw_sectors;

	if (!q->limits.chunk_sectors ||
	    req_op(rq) == REQ_OP_DISCARD ||
	    req_op(rq) == REQ_OP_SECURE_ERASE)
		return blk_queue_get_max_sectors(q, req_op(rq));

	return min(blk_max_size_offset(q, offset),
			blk_queue_get_max_sectors(q, req_op(rq)));
}

static inline unsigned int blk_rq_count_bios(struct request *rq)
{
	unsigned int nr_bios = 0;
	struct bio *bio;

	__rq_for_each_bio(bio, rq)
		nr_bios++;

	return nr_bios;
}


extern struct request *blk_peek_request(struct request_queue *q);
extern void blk_start_request(struct request *rq);
extern struct request *blk_fetch_request(struct request_queue *q);

void blk_steal_bios(struct bio_list *list, struct request *rq);


extern bool blk_update_request(struct request *rq, blk_status_t error,
			       unsigned int nr_bytes);
extern void blk_finish_request(struct request *rq, blk_status_t error);
extern bool blk_end_request(struct request *rq, blk_status_t error,
			    unsigned int nr_bytes);
extern void blk_end_request_all(struct request *rq, blk_status_t error);
extern bool __blk_end_request(struct request *rq, blk_status_t error,
			      unsigned int nr_bytes);
extern void __blk_end_request_all(struct request *rq, blk_status_t error);
extern bool __blk_end_request_cur(struct request *rq, blk_status_t error);

extern void blk_complete_request(struct request *);
extern void __blk_complete_request(struct request *);
extern void blk_abort_request(struct request *);
extern void blk_unprep_request(struct request *);


extern struct request_queue *blk_init_queue_node(request_fn_proc *rfn,
					spinlock_t *lock, int node_id);
extern struct request_queue *blk_init_queue(request_fn_proc *, spinlock_t *);
extern int blk_init_allocated_queue(struct request_queue *);
extern void blk_cleanup_queue(struct request_queue *);
extern void blk_queue_make_request(struct request_queue *, make_request_fn *);
extern void blk_queue_bounce_limit(struct request_queue *, u64);
extern void blk_queue_max_hw_sectors(struct request_queue *, unsigned int);
extern void blk_queue_chunk_sectors(struct request_queue *, unsigned int);
extern void blk_queue_max_segments(struct request_queue *, unsigned short);
extern void blk_queue_max_discard_segments(struct request_queue *,
		unsigned short);
extern void blk_queue_max_segment_size(struct request_queue *, unsigned int);
extern void blk_queue_max_discard_sectors(struct request_queue *q,
		unsigned int max_discard_sectors);
extern void blk_queue_max_write_same_sectors(struct request_queue *q,
		unsigned int max_write_same_sectors);
extern void blk_queue_max_write_zeroes_sectors(struct request_queue *q,
		unsigned int max_write_same_sectors);
extern void blk_queue_logical_block_size(struct request_queue *, unsigned int);
extern void blk_queue_physical_block_size(struct request_queue *, unsigned int);
extern void blk_queue_alignment_offset(struct request_queue *q,
				       unsigned int alignment);
extern void blk_limits_io_min(struct queue_limits *limits, unsigned int min);
extern void blk_queue_io_min(struct request_queue *q, unsigned int min);
extern void blk_limits_io_opt(struct queue_limits *limits, unsigned int opt);
extern void blk_queue_io_opt(struct request_queue *q, unsigned int opt);
extern void blk_set_queue_depth(struct request_queue *q, unsigned int depth);
extern void blk_set_default_limits(struct queue_limits *lim);
extern void blk_set_stacking_limits(struct queue_limits *lim);
extern int blk_stack_limits(struct queue_limits *t, struct queue_limits *b,
			    sector_t offset);
extern int bdev_stack_limits(struct queue_limits *t, struct block_device *bdev,
			    sector_t offset);
extern void disk_stack_limits(struct gendisk *disk, struct block_device *bdev,
			      sector_t offset);
extern void blk_queue_stack_limits(struct request_queue *t, struct request_queue *b);
extern void blk_queue_dma_pad(struct request_queue *, unsigned int);
extern void blk_queue_update_dma_pad(struct request_queue *, unsigned int);
extern int blk_queue_dma_drain(struct request_queue *q,
			       dma_drain_needed_fn *dma_drain_needed,
			       void *buf, unsigned int size);
extern void blk_queue_lld_busy(struct request_queue *q, lld_busy_fn *fn);
extern void blk_queue_segment_boundary(struct request_queue *, unsigned long);
extern void blk_queue_virt_boundary(struct request_queue *, unsigned long);
extern void blk_queue_prep_rq(struct request_queue *, prep_rq_fn *pfn);
extern void blk_queue_unprep_rq(struct request_queue *, unprep_rq_fn *ufn);
extern void blk_queue_dma_alignment(struct request_queue *, int);
extern void blk_queue_update_dma_alignment(struct request_queue *, int);
extern void blk_queue_softirq_done(struct request_queue *, softirq_done_fn *);
extern void blk_queue_rq_timed_out(struct request_queue *, rq_timed_out_fn *);
extern void blk_queue_rq_timeout(struct request_queue *, unsigned int);
extern void blk_queue_flush_queueable(struct request_queue *q, bool queueable);
extern void blk_queue_write_cache(struct request_queue *q, bool enabled, bool fua);


static inline unsigned short blk_rq_nr_phys_segments(struct request *rq)
{
	if (rq->rq_flags & RQF_SPECIAL_PAYLOAD)
		return 1;
	return rq->nr_phys_segments;
}


static inline unsigned short blk_rq_nr_discard_segments(struct request *rq)
{
	return max_t(unsigned short, rq->nr_phys_segments, 1);
}

extern int blk_rq_map_sg(struct request_queue *, struct request *, struct scatterlist *);
extern void blk_dump_rq_flags(struct request *, char *);
extern long nr_blockdev_pages(void);

bool __must_check blk_get_queue(struct request_queue *);
struct request_queue *blk_alloc_queue(gfp_t);
struct request_queue *blk_alloc_queue_node(gfp_t gfp_mask, int node_id,
					   spinlock_t *lock);
extern void blk_put_queue(struct request_queue *);
extern void blk_set_queue_dying(struct request_queue *);


#ifdef CONFIG_PM
extern void blk_pm_runtime_init(struct request_queue *q, struct device *dev);
extern int blk_pre_runtime_suspend(struct request_queue *q);
extern void blk_post_runtime_suspend(struct request_queue *q, int err);
extern void blk_pre_runtime_resume(struct request_queue *q);
extern void blk_post_runtime_resume(struct request_queue *q, int err);
extern void blk_set_runtime_active(struct request_queue *q);
#else
static inline void blk_pm_runtime_init(struct request_queue *q,
	struct device *dev) {}
static inline int blk_pre_runtime_suspend(struct request_queue *q)
{
	return -ENOSYS;
}
static inline void blk_post_runtime_suspend(struct request_queue *q, int err) {}
static inline void blk_pre_runtime_resume(struct request_queue *q) {}
static inline void blk_post_runtime_resume(struct request_queue *q, int err) {}
static inline void blk_set_runtime_active(struct request_queue *q) {}
#endif


struct blk_plug {
	struct list_head list; 
	struct list_head mq_list; 
	struct list_head cb_list; 
};
#define BLK_MAX_REQUEST_COUNT 16
#define BLK_PLUG_FLUSH_SIZE (128 * 1024)

struct blk_plug_cb;
typedef void (*blk_plug_cb_fn)(struct blk_plug_cb *, bool);
struct blk_plug_cb {
	struct list_head list;
	blk_plug_cb_fn callback;
	void *data;
};
extern struct blk_plug_cb *blk_check_plugged(blk_plug_cb_fn unplug,
					     void *data, int size);
extern void blk_start_plug(struct blk_plug *);
extern void blk_finish_plug(struct blk_plug *);
extern void blk_flush_plug_list(struct blk_plug *, bool);

static inline void blk_flush_plug(struct task_struct *tsk)
{
	struct blk_plug *plug = tsk->plug;

	if (plug)
		blk_flush_plug_list(plug, false);
}

static inline void blk_schedule_flush_plug(struct task_struct *tsk)
{
	struct blk_plug *plug = tsk->plug;

	if (plug)
		blk_flush_plug_list(plug, true);
}

static inline bool blk_needs_flush_plug(struct task_struct *tsk)
{
	struct blk_plug *plug = tsk->plug;

	return plug &&
		(!list_empty(&plug->list) ||
		 !list_empty(&plug->mq_list) ||
		 !list_empty(&plug->cb_list));
}


extern int blk_queue_start_tag(struct request_queue *, struct request *);
extern struct request *blk_queue_find_tag(struct request_queue *, int);
extern void blk_queue_end_tag(struct request_queue *, struct request *);
extern int blk_queue_init_tags(struct request_queue *, int, struct blk_queue_tag *, int);
extern void blk_queue_free_tags(struct request_queue *);
extern int blk_queue_resize_tags(struct request_queue *, int);
extern struct blk_queue_tag *blk_init_tags(int, int);
extern void blk_free_tags(struct blk_queue_tag *);

static inline struct request *blk_map_queue_find_tag(struct blk_queue_tag *bqt,
						int tag)
{
	if (unlikely(bqt == NULL || tag >= bqt->real_max_depth))
		return NULL;
	return bqt->tag_index[tag];
}

extern int blkdev_issue_flush(struct block_device *, gfp_t, sector_t *);
extern int blkdev_issue_write_same(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, struct page *page);

#define BLKDEV_DISCARD_SECURE	(1 << 0)	

extern int blkdev_issue_discard(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, unsigned long flags);
extern int __blkdev_issue_discard(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, int flags,
		struct bio **biop);

#define BLKDEV_ZERO_NOUNMAP	(1 << 0)  
#define BLKDEV_ZERO_NOFALLBACK	(1 << 1)  

extern int __blkdev_issue_zeroout(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, struct bio **biop,
		unsigned flags);
extern int blkdev_issue_zeroout(struct block_device *bdev, sector_t sector,
		sector_t nr_sects, gfp_t gfp_mask, unsigned flags);

static inline int sb_issue_discard(struct super_block *sb, sector_t block,
		sector_t nr_blocks, gfp_t gfp_mask, unsigned long flags)
{
	return blkdev_issue_discard(sb->s_bdev,
				    block << (sb->s_blocksize_bits -
					      SECTOR_SHIFT),
				    nr_blocks << (sb->s_blocksize_bits -
						  SECTOR_SHIFT),
				    gfp_mask, flags);
}
static inline int sb_issue_zeroout(struct super_block *sb, sector_t block,
		sector_t nr_blocks, gfp_t gfp_mask)
{
	return blkdev_issue_zeroout(sb->s_bdev,
				    block << (sb->s_blocksize_bits -
					      SECTOR_SHIFT),
				    nr_blocks << (sb->s_blocksize_bits -
						  SECTOR_SHIFT),
				    gfp_mask, 0);
}

extern int blk_verify_command(unsigned char *cmd, fmode_t mode);

enum blk_default_limits {
	BLK_MAX_SEGMENTS	= 128,
	BLK_SAFE_MAX_SECTORS	= 255,
	BLK_DEF_MAX_SECTORS	= 1024,
	BLK_MAX_SEGMENT_SIZE	= 65536,
	BLK_SEG_BOUNDARY_MASK	= 0xFFFFFFFFUL,
};

static inline unsigned long queue_segment_boundary(struct request_queue *q)
{
	return q->limits.seg_boundary_mask;
}

static inline unsigned long queue_virt_boundary(struct request_queue *q)
{
	return q->limits.virt_boundary_mask;
}

static inline unsigned int queue_max_sectors(struct request_queue *q)
{
	return q->limits.max_sectors;
}

static inline unsigned int queue_max_hw_sectors(struct request_queue *q)
{
	return q->limits.max_hw_sectors;
}

static inline unsigned short queue_max_segments(struct request_queue *q)
{
	return q->limits.max_segments;
}

static inline unsigned short queue_max_discard_segments(struct request_queue *q)
{
	return q->limits.max_discard_segments;
}

static inline unsigned int queue_max_segment_size(struct request_queue *q)
{
	return q->limits.max_segment_size;
}

static inline unsigned queue_logical_block_size(struct request_queue *q)
{
	int retval = 512;

	if (q && q->limits.logical_block_size)
		retval = q->limits.logical_block_size;

	return retval;
}

static inline unsigned int bdev_logical_block_size(struct block_device *bdev)
{
	return queue_logical_block_size(bdev_get_queue(bdev));
}

static inline unsigned int queue_physical_block_size(struct request_queue *q)
{
	return q->limits.physical_block_size;
}

static inline unsigned int bdev_physical_block_size(struct block_device *bdev)
{
	return queue_physical_block_size(bdev_get_queue(bdev));
}

static inline unsigned int queue_io_min(struct request_queue *q)
{
	return q->limits.io_min;
}

static inline int bdev_io_min(struct block_device *bdev)
{
	return queue_io_min(bdev_get_queue(bdev));
}

static inline unsigned int queue_io_opt(struct request_queue *q)
{
	return q->limits.io_opt;
}

static inline int bdev_io_opt(struct block_device *bdev)
{
	return queue_io_opt(bdev_get_queue(bdev));
}

static inline int queue_alignment_offset(struct request_queue *q)
{
	if (q->limits.misaligned)
		return -1;

	return q->limits.alignment_offset;
}

static inline int queue_limit_alignment_offset(struct queue_limits *lim, sector_t sector)
{
	unsigned int granularity = max(lim->physical_block_size, lim->io_min);
	unsigned int alignment = sector_div(sector, granularity >> SECTOR_SHIFT)
		<< SECTOR_SHIFT;

	return (granularity + lim->alignment_offset - alignment) % granularity;
}

static inline int bdev_alignment_offset(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q->limits.misaligned)
		return -1;

	if (bdev != bdev->bd_contains)
		return bdev->bd_part->alignment_offset;

	return q->limits.alignment_offset;
}

static inline int queue_discard_alignment(struct request_queue *q)
{
	if (q->limits.discard_misaligned)
		return -1;

	return q->limits.discard_alignment;
}

static inline int queue_limit_discard_alignment(struct queue_limits *lim, sector_t sector)
{
	unsigned int alignment, granularity, offset;

	if (!lim->max_discard_sectors)
		return 0;

	
	alignment = lim->discard_alignment >> SECTOR_SHIFT;
	granularity = lim->discard_granularity >> SECTOR_SHIFT;
	if (!granularity)
		return 0;

	
	offset = sector_div(sector, granularity);

	
	offset = (granularity + alignment - offset) % granularity;

	
	return offset << SECTOR_SHIFT;
}

static inline int bdev_discard_alignment(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (bdev != bdev->bd_contains)
		return bdev->bd_part->discard_alignment;

	return q->limits.discard_alignment;
}

static inline unsigned int bdev_write_same(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return q->limits.max_write_same_sectors;

	return 0;
}

static inline unsigned int bdev_write_zeroes_sectors(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return q->limits.max_write_zeroes_sectors;

	return 0;
}

static inline enum blk_zoned_model bdev_zoned_model(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return blk_queue_zoned_model(q);

	return BLK_ZONED_NONE;
}

static inline bool bdev_is_zoned(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return blk_queue_is_zoned(q);

	return false;
}

static inline unsigned int bdev_zone_sectors(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q)
		return blk_queue_zone_sectors(q);
	return 0;
}

static inline int queue_dma_alignment(struct request_queue *q)
{
	return q ? q->dma_alignment : 511;
}

static inline int blk_rq_aligned(struct request_queue *q, unsigned long addr,
				 unsigned int len)
{
	unsigned int alignment = queue_dma_alignment(q) | q->dma_pad_mask;
	return !(addr & alignment) && !(len & alignment);
}


static inline unsigned int blksize_bits(unsigned int size)
{
	unsigned int bits = 8;
	do {
		bits++;
		size >>= 1;
	} while (size > 256);
	return bits;
}

static inline unsigned int block_size(struct block_device *bdev)
{
	return bdev->bd_block_size;
}

static inline bool queue_flush_queueable(struct request_queue *q)
{
	return !test_bit(QUEUE_FLAG_FLUSH_NQ, &q->queue_flags);
}

typedef struct {struct page *v;} Sector;

unsigned char *read_dev_sector(struct block_device *, sector_t, Sector *);

static inline void put_dev_sector(Sector p)
{
	put_page(p.v);
}

static inline bool __bvec_gap_to_prev(struct request_queue *q,
				struct bio_vec *bprv, unsigned int offset)
{
	return offset ||
		((bprv->bv_offset + bprv->bv_len) & queue_virt_boundary(q));
}


static inline bool bvec_gap_to_prev(struct request_queue *q,
				struct bio_vec *bprv, unsigned int offset)
{
	if (!queue_virt_boundary(q))
		return false;
	return __bvec_gap_to_prev(q, bprv, offset);
}


static inline bool bios_segs_mergeable(struct request_queue *q,
		struct bio *prev, struct bio_vec *prev_last_bv,
		struct bio_vec *next_first_bv)
{
	if (!BIOVEC_PHYS_MERGEABLE(prev_last_bv, next_first_bv))
		return false;
	if (!BIOVEC_SEG_BOUNDARY(q, prev_last_bv, next_first_bv))
		return false;
	if (prev->bi_seg_back_size + next_first_bv->bv_len >
			queue_max_segment_size(q))
		return false;
	return true;
}

static inline bool bio_will_gap(struct request_queue *q,
				struct request *prev_rq,
				struct bio *prev,
				struct bio *next)
{
	if (bio_has_data(prev) && queue_virt_boundary(q)) {
		struct bio_vec pb, nb;

		
		if (prev_rq)
			bio_get_first_bvec(prev_rq->bio, &pb);
		else
			bio_get_first_bvec(prev, &pb);
		if (pb.bv_offset)
			return true;

		
		bio_get_last_bvec(prev, &pb);
		bio_get_first_bvec(next, &nb);

		if (!bios_segs_mergeable(q, prev, &pb, &nb))
			return __bvec_gap_to_prev(q, &pb, nb.bv_offset);
	}

	return false;
}

static inline bool req_gap_back_merge(struct request *req, struct bio *bio)
{
	return bio_will_gap(req->q, req, req->biotail, bio);
}

static inline bool req_gap_front_merge(struct request *req, struct bio *bio)
{
	return bio_will_gap(req->q, NULL, bio, req->bio);
}

int kblockd_schedule_work(struct work_struct *work);
int kblockd_schedule_work_on(int cpu, struct work_struct *work);
int kblockd_mod_delayed_work_on(int cpu, struct delayed_work *dwork, unsigned long delay);

#define MODULE_ALIAS_BLOCKDEV(major,minor) \
	MODULE_ALIAS("block-major-" __stringify(major) "-" __stringify(minor))
#define MODULE_ALIAS_BLOCKDEV_MAJOR(major) \
	MODULE_ALIAS("block-major-" __stringify(major) "-*")

#if defined(CONFIG_BLK_DEV_INTEGRITY)

enum blk_integrity_flags {
	BLK_INTEGRITY_VERIFY		= 1 << 0,
	BLK_INTEGRITY_GENERATE		= 1 << 1,
	BLK_INTEGRITY_DEVICE_CAPABLE	= 1 << 2,
	BLK_INTEGRITY_IP_CHECKSUM	= 1 << 3,
};

struct blk_integrity_iter {
	void			*prot_buf;
	void			*data_buf;
	sector_t		seed;
	unsigned int		data_size;
	unsigned short		interval;
	const char		*disk_name;
};

typedef blk_status_t (integrity_processing_fn) (struct blk_integrity_iter *);

struct blk_integrity_profile {
	integrity_processing_fn		*generate_fn;
	integrity_processing_fn		*verify_fn;
	const char			*name;
};

extern void blk_integrity_register(struct gendisk *, struct blk_integrity *);
extern void blk_integrity_unregister(struct gendisk *);
extern int blk_integrity_compare(struct gendisk *, struct gendisk *);
extern int blk_rq_map_integrity_sg(struct request_queue *, struct bio *,
				   struct scatterlist *);
extern int blk_rq_count_integrity_sg(struct request_queue *, struct bio *);
extern bool blk_integrity_merge_rq(struct request_queue *, struct request *,
				   struct request *);
extern bool blk_integrity_merge_bio(struct request_queue *, struct request *,
				    struct bio *);

static inline struct blk_integrity *blk_get_integrity(struct gendisk *disk)
{
	struct blk_integrity *bi = &disk->queue->integrity;

	if (!bi->profile)
		return NULL;

	return bi;
}

static inline
struct blk_integrity *bdev_get_integrity(struct block_device *bdev)
{
	return blk_get_integrity(bdev->bd_disk);
}

static inline bool blk_integrity_rq(struct request *rq)
{
	return rq->cmd_flags & REQ_INTEGRITY;
}

static inline void blk_queue_max_integrity_segments(struct request_queue *q,
						    unsigned int segs)
{
	q->limits.max_integrity_segments = segs;
}

static inline unsigned short
queue_max_integrity_segments(struct request_queue *q)
{
	return q->limits.max_integrity_segments;
}

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


static inline unsigned int bio_integrity_intervals(struct blk_integrity *bi,
						   unsigned int sectors)
{
	return sectors >> (bi->interval_exp - 9);
}

static inline unsigned int bio_integrity_bytes(struct blk_integrity *bi,
					       unsigned int sectors)
{
	return bio_integrity_intervals(bi, sectors) * bi->tuple_size;
}

#else 

struct bio;
struct block_device;
struct gendisk;
struct blk_integrity;

static inline int blk_integrity_rq(struct request *rq)
{
	return 0;
}
static inline int blk_rq_count_integrity_sg(struct request_queue *q,
					    struct bio *b)
{
	return 0;
}
static inline int blk_rq_map_integrity_sg(struct request_queue *q,
					  struct bio *b,
					  struct scatterlist *s)
{
	return 0;
}
static inline struct blk_integrity *bdev_get_integrity(struct block_device *b)
{
	return NULL;
}
static inline struct blk_integrity *blk_get_integrity(struct gendisk *disk)
{
	return NULL;
}
static inline int blk_integrity_compare(struct gendisk *a, struct gendisk *b)
{
	return 0;
}
static inline void blk_integrity_register(struct gendisk *d,
					 struct blk_integrity *b)
{
}
static inline void blk_integrity_unregister(struct gendisk *d)
{
}
static inline void blk_queue_max_integrity_segments(struct request_queue *q,
						    unsigned int segs)
{
}
static inline unsigned short queue_max_integrity_segments(struct request_queue *q)
{
	return 0;
}
static inline bool blk_integrity_merge_rq(struct request_queue *rq,
					  struct request *r1,
					  struct request *r2)
{
	return true;
}
static inline bool blk_integrity_merge_bio(struct request_queue *rq,
					   struct request *r,
					   struct bio *b)
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

static inline unsigned int bio_integrity_intervals(struct blk_integrity *bi,
						   unsigned int sectors)
{
	return 0;
}

static inline unsigned int bio_integrity_bytes(struct blk_integrity *bi,
					       unsigned int sectors)
{
	return 0;
}

#endif 

struct block_device_operations {
	int (*open) (struct block_device *, fmode_t);
	void (*release) (struct gendisk *, fmode_t);
	int (*rw_page)(struct block_device *, sector_t, struct page *, unsigned int);
	int (*ioctl) (struct block_device *, fmode_t, unsigned, unsigned long);
	int (*compat_ioctl) (struct block_device *, fmode_t, unsigned, unsigned long);
	unsigned int (*check_events) (struct gendisk *disk,
				      unsigned int clearing);
	
	int (*media_changed) (struct gendisk *);
	void (*unlock_native_capacity) (struct gendisk *);
	int (*revalidate_disk) (struct gendisk *);
	int (*getgeo)(struct block_device *, struct hd_geometry *);
	
	void (*swap_slot_free_notify) (struct block_device *, unsigned long);
	struct module *owner;
	const struct pr_ops *pr_ops;
};

extern int __blkdev_driver_ioctl(struct block_device *, fmode_t, unsigned int,
				 unsigned long);
extern int bdev_read_page(struct block_device *, sector_t, struct page *);
extern int bdev_write_page(struct block_device *, sector_t, struct page *,
						struct writeback_control *);

#ifdef CONFIG_BLK_DEV_ZONED
bool blk_req_needs_zone_write_lock(struct request *rq);
void __blk_req_zone_write_lock(struct request *rq);
void __blk_req_zone_write_unlock(struct request *rq);

static inline void blk_req_zone_write_lock(struct request *rq)
{
	if (blk_req_needs_zone_write_lock(rq))
		__blk_req_zone_write_lock(rq);
}

static inline void blk_req_zone_write_unlock(struct request *rq)
{
	if (rq->rq_flags & RQF_ZONE_WRITE_LOCKED)
		__blk_req_zone_write_unlock(rq);
}

static inline bool blk_req_zone_is_write_locked(struct request *rq)
{
	return rq->q->seq_zones_wlock &&
		test_bit(blk_rq_zone_no(rq), rq->q->seq_zones_wlock);
}

static inline bool blk_req_can_dispatch_to_zone(struct request *rq)
{
	if (!blk_req_needs_zone_write_lock(rq))
		return true;
	return !blk_req_zone_is_write_locked(rq);
}
#else
static inline bool blk_req_needs_zone_write_lock(struct request *rq)
{
	return false;
}

static inline void blk_req_zone_write_lock(struct request *rq)
{
}

static inline void blk_req_zone_write_unlock(struct request *rq)
{
}
static inline bool blk_req_zone_is_write_locked(struct request *rq)
{
	return false;
}

static inline bool blk_req_can_dispatch_to_zone(struct request *rq)
{
	return true;
}
#endif 

#else 

struct block_device;


#define buffer_heads_over_limit 0

static inline long nr_blockdev_pages(void)
{
	return 0;
}

struct blk_plug {
};

static inline void blk_start_plug(struct blk_plug *plug)
{
}

static inline void blk_finish_plug(struct blk_plug *plug)
{
}

static inline void blk_flush_plug(struct task_struct *task)
{
}

static inline void blk_schedule_flush_plug(struct task_struct *task)
{
}


static inline bool blk_needs_flush_plug(struct task_struct *tsk)
{
	return false;
}

static inline int blkdev_issue_flush(struct block_device *bdev, gfp_t gfp_mask,
				     sector_t *error_sector)
{
	return 0;
}

#endif 

#endif
