// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef __XFS_BUF_H__
#define __XFS_BUF_H__

#include <linux/list.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/dax.h>
#include <linux/uio.h>
#include <linux/list_lru.h>

/*
 *	Base types
 */
<<<<<<< HEAD

#define XFS_BUF_DADDR_NULL	((xfs_daddr_t) (-1LL))

typedef enum {
	XBRW_READ = 1,			/* transfer into target memory */
	XBRW_WRITE = 2,			/* transfer from target memory */
	XBRW_ZERO = 3,			/* Zero target memory */
} xfs_buf_rw_t;

=======
struct xfs_buf;

#define XFS_BUF_DADDR_NULL	((xfs_daddr_t) (-1LL))

>>>>>>> upstream/android-13
#define XBF_READ	 (1 << 0) /* buffer intended for reading from device */
#define XBF_WRITE	 (1 << 1) /* buffer intended for writing to device */
#define XBF_READ_AHEAD	 (1 << 2) /* asynchronous read-ahead */
#define XBF_NO_IOACCT	 (1 << 3) /* bypass I/O accounting (non-LRU bufs) */
#define XBF_ASYNC	 (1 << 4) /* initiator will not wait for completion */
#define XBF_DONE	 (1 << 5) /* all pages in the buffer uptodate */
#define XBF_STALE	 (1 << 6) /* buffer has been staled, do not find it */
<<<<<<< HEAD
#define XBF_WRITE_FAIL	 (1 << 24)/* async writes have failed on this buffer */

/* I/O hints for the BIO layer */
#define XBF_SYNCIO	 (1 << 10)/* treat this buffer as synchronous I/O */
#define XBF_FUA		 (1 << 11)/* force cache write through mode */
#define XBF_FLUSH	 (1 << 12)/* flush the disk cache before a write */

/* flags used only as arguments to access routines */
#define XBF_TRYLOCK	 (1 << 16)/* lock requested, but do not wait */
#define XBF_UNMAPPED	 (1 << 17)/* do not map the buffer */
=======
#define XBF_WRITE_FAIL	 (1 << 7) /* async writes have failed on this buffer */

/* buffer type flags for write callbacks */
#define _XBF_INODES	 (1 << 16)/* inode buffer */
#define _XBF_DQUOTS	 (1 << 17)/* dquot buffer */
#define _XBF_LOGRECOVERY	 (1 << 18)/* log recovery buffer */
>>>>>>> upstream/android-13

/* flags used only internally */
#define _XBF_PAGES	 (1 << 20)/* backed by refcounted pages */
#define _XBF_KMEM	 (1 << 21)/* backed by heap memory */
#define _XBF_DELWRI_Q	 (1 << 22)/* buffer on a delwri queue */
<<<<<<< HEAD
#define _XBF_COMPOUND	 (1 << 23)/* compound buffer */
=======

/* flags used only as arguments to access routines */
#define XBF_TRYLOCK	 (1 << 30)/* lock requested, but do not wait */
#define XBF_UNMAPPED	 (1 << 31)/* do not map the buffer */
>>>>>>> upstream/android-13

typedef unsigned int xfs_buf_flags_t;

#define XFS_BUF_FLAGS \
	{ XBF_READ,		"READ" }, \
	{ XBF_WRITE,		"WRITE" }, \
	{ XBF_READ_AHEAD,	"READ_AHEAD" }, \
	{ XBF_NO_IOACCT,	"NO_IOACCT" }, \
	{ XBF_ASYNC,		"ASYNC" }, \
	{ XBF_DONE,		"DONE" }, \
	{ XBF_STALE,		"STALE" }, \
	{ XBF_WRITE_FAIL,	"WRITE_FAIL" }, \
<<<<<<< HEAD
	{ XBF_SYNCIO,		"SYNCIO" }, \
	{ XBF_FUA,		"FUA" }, \
	{ XBF_FLUSH,		"FLUSH" }, \
	{ XBF_TRYLOCK,		"TRYLOCK" },	/* should never be set */\
	{ XBF_UNMAPPED,		"UNMAPPED" },	/* ditto */\
	{ _XBF_PAGES,		"PAGES" }, \
	{ _XBF_KMEM,		"KMEM" }, \
	{ _XBF_DELWRI_Q,	"DELWRI_Q" }, \
	{ _XBF_COMPOUND,	"COMPOUND" }

=======
	{ _XBF_INODES,		"INODES" }, \
	{ _XBF_DQUOTS,		"DQUOTS" }, \
	{ _XBF_LOGRECOVERY,		"LOG_RECOVERY" }, \
	{ _XBF_PAGES,		"PAGES" }, \
	{ _XBF_KMEM,		"KMEM" }, \
	{ _XBF_DELWRI_Q,	"DELWRI_Q" }, \
	/* The following interface flags should never be set */ \
	{ XBF_TRYLOCK,		"TRYLOCK" }, \
	{ XBF_UNMAPPED,		"UNMAPPED" }
>>>>>>> upstream/android-13

/*
 * Internal state flags.
 */
#define XFS_BSTATE_DISPOSE	 (1 << 0)	/* buffer being discarded */
#define XFS_BSTATE_IN_FLIGHT	 (1 << 1)	/* I/O in flight */

/*
 * The xfs_buftarg contains 2 notions of "sector size" -
 *
 * 1) The metadata sector size, which is the minimum unit and
 *    alignment of IO which will be performed by metadata operations.
 * 2) The device logical sector size
 *
 * The first is specified at mkfs time, and is stored on-disk in the
 * superblock's sb_sectsize.
 *
 * The latter is derived from the underlying device, and controls direct IO
 * alignment constraints.
 */
typedef struct xfs_buftarg {
	dev_t			bt_dev;
	struct block_device	*bt_bdev;
	struct dax_device	*bt_daxdev;
	struct xfs_mount	*bt_mount;
	unsigned int		bt_meta_sectorsize;
	size_t			bt_meta_sectormask;
	size_t			bt_logical_sectorsize;
	size_t			bt_logical_sectormask;

	/* LRU control structures */
	struct shrinker		bt_shrinker;
	struct list_lru		bt_lru;

	struct percpu_counter	bt_io_count;
<<<<<<< HEAD
} xfs_buftarg_t;

struct xfs_buf;
typedef void (*xfs_buf_iodone_t)(struct xfs_buf *);


=======
	struct ratelimit_state	bt_ioerror_rl;
} xfs_buftarg_t;

>>>>>>> upstream/android-13
#define XB_PAGES	2

struct xfs_buf_map {
	xfs_daddr_t		bm_bn;	/* block number for I/O */
	int			bm_len;	/* size of I/O */
};

#define DEFINE_SINGLE_BUF_MAP(map, blkno, numblk) \
	struct xfs_buf_map (map) = { .bm_bn = (blkno), .bm_len = (numblk) };

struct xfs_buf_ops {
	char *name;
<<<<<<< HEAD
=======
	union {
		__be32 magic[2];	/* v4 and v5 on disk magic values */
		__be16 magic16[2];	/* v4 and v5 on disk magic values */
	};
>>>>>>> upstream/android-13
	void (*verify_read)(struct xfs_buf *);
	void (*verify_write)(struct xfs_buf *);
	xfs_failaddr_t (*verify_struct)(struct xfs_buf *bp);
};

<<<<<<< HEAD
typedef struct xfs_buf {
=======
struct xfs_buf {
>>>>>>> upstream/android-13
	/*
	 * first cacheline holds all the fields needed for an uncontended cache
	 * hit to be fully processed. The semaphore straddles the cacheline
	 * boundary, but the counter and lock sits on the first cacheline,
	 * which is the only bit that is touched if we hit the semaphore
	 * fast-path on locking.
	 */
	struct rhash_head	b_rhash_head;	/* pag buffer hash node */
<<<<<<< HEAD
	xfs_daddr_t		b_bn;		/* block number of buffer */
=======

	xfs_daddr_t		b_rhash_key;	/* buffer cache index */
>>>>>>> upstream/android-13
	int			b_length;	/* size of buffer in BBs */
	atomic_t		b_hold;		/* reference count */
	atomic_t		b_lru_ref;	/* lru reclaim ref count */
	xfs_buf_flags_t		b_flags;	/* status flags */
	struct semaphore	b_sema;		/* semaphore for lockables */

	/*
	 * concurrent access to b_lru and b_lru_flags are protected by
	 * bt_lru_lock and not by b_sema
	 */
	struct list_head	b_lru;		/* lru list */
	spinlock_t		b_lock;		/* internal state lock */
	unsigned int		b_state;	/* internal state flags */
	int			b_io_error;	/* internal IO error state */
	wait_queue_head_t	b_waiters;	/* unpin waiters */
	struct list_head	b_list;
	struct xfs_perag	*b_pag;		/* contains rbtree root */
<<<<<<< HEAD
	xfs_buftarg_t		*b_target;	/* buffer target (device) */
	void			*b_addr;	/* virtual address of buffer */
	struct work_struct	b_ioend_work;
	struct workqueue_struct	*b_ioend_wq;	/* I/O completion wq */
	xfs_buf_iodone_t	b_iodone;	/* I/O completion function */
	struct completion	b_iowait;	/* queue for I/O waiters */
	void			*b_log_item;
=======
	struct xfs_mount	*b_mount;
	struct xfs_buftarg	*b_target;	/* buffer target (device) */
	void			*b_addr;	/* virtual address of buffer */
	struct work_struct	b_ioend_work;
	struct completion	b_iowait;	/* queue for I/O waiters */
	struct xfs_buf_log_item	*b_log_item;
>>>>>>> upstream/android-13
	struct list_head	b_li_list;	/* Log items list head */
	struct xfs_trans	*b_transp;
	struct page		**b_pages;	/* array of page pointers */
	struct page		*b_page_array[XB_PAGES]; /* inline pages */
	struct xfs_buf_map	*b_maps;	/* compound buffer map */
	struct xfs_buf_map	__b_map;	/* inline compound buffer map */
	int			b_map_count;
<<<<<<< HEAD
	int			b_io_length;	/* IO size in BBs */
	atomic_t		b_pin_count;	/* pin count */
	atomic_t		b_io_remaining;	/* #outstanding I/O requests */
	unsigned int		b_page_count;	/* size of page array */
	unsigned int		b_offset;	/* page offset in first page */
=======
	atomic_t		b_pin_count;	/* pin count */
	atomic_t		b_io_remaining;	/* #outstanding I/O requests */
	unsigned int		b_page_count;	/* size of page array */
	unsigned int		b_offset;	/* page offset of b_addr,
						   only for _XBF_KMEM buffers */
>>>>>>> upstream/android-13
	int			b_error;	/* error code on I/O */

	/*
	 * async write failure retry count. Initialised to zero on the first
	 * failure, then when it exceeds the maximum configured without a
	 * success the write is considered to be failed permanently and the
	 * iodone handler will take appropriate action.
	 *
	 * For retry timeouts, we record the jiffie of the first failure. This
	 * means that we can change the retry timeout for buffers already under
	 * I/O and thus avoid getting stuck in a retry loop with a long timeout.
	 *
	 * last_error is used to ensure that we are getting repeated errors, not
	 * different errors. e.g. a block device might change ENOSPC to EIO when
	 * a failure timeout occurs, so we want to re-initialise the error
	 * retry behaviour appropriately when that happens.
	 */
	int			b_retries;
	unsigned long		b_first_retry_time; /* in jiffies */
	int			b_last_error;

	const struct xfs_buf_ops	*b_ops;
<<<<<<< HEAD
} xfs_buf_t;
=======
};
>>>>>>> upstream/android-13

/* Finding and Reading Buffers */
struct xfs_buf *xfs_buf_incore(struct xfs_buftarg *target,
			   xfs_daddr_t blkno, size_t numblks,
			   xfs_buf_flags_t flags);

<<<<<<< HEAD
struct xfs_buf *_xfs_buf_alloc(struct xfs_buftarg *target,
			       struct xfs_buf_map *map, int nmaps,
			       xfs_buf_flags_t flags);

static inline struct xfs_buf *
xfs_buf_alloc(
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	size_t			numblks,
	xfs_buf_flags_t		flags)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);
	return _xfs_buf_alloc(target, &map, 1, flags);
}

struct xfs_buf *xfs_buf_get_map(struct xfs_buftarg *target,
			       struct xfs_buf_map *map, int nmaps,
			       xfs_buf_flags_t flags);
struct xfs_buf *xfs_buf_read_map(struct xfs_buftarg *target,
			       struct xfs_buf_map *map, int nmaps,
			       xfs_buf_flags_t flags,
			       const struct xfs_buf_ops *ops);
=======
int xfs_buf_get_map(struct xfs_buftarg *target, struct xfs_buf_map *map,
		int nmaps, xfs_buf_flags_t flags, struct xfs_buf **bpp);
int xfs_buf_read_map(struct xfs_buftarg *target, struct xfs_buf_map *map,
		int nmaps, xfs_buf_flags_t flags, struct xfs_buf **bpp,
		const struct xfs_buf_ops *ops, xfs_failaddr_t fa);
>>>>>>> upstream/android-13
void xfs_buf_readahead_map(struct xfs_buftarg *target,
			       struct xfs_buf_map *map, int nmaps,
			       const struct xfs_buf_ops *ops);

<<<<<<< HEAD
static inline struct xfs_buf *
=======
static inline int
>>>>>>> upstream/android-13
xfs_buf_get(
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	size_t			numblks,
<<<<<<< HEAD
	xfs_buf_flags_t		flags)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);
	return xfs_buf_get_map(target, &map, 1, flags);
}

static inline struct xfs_buf *
=======
	struct xfs_buf		**bpp)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);

	return xfs_buf_get_map(target, &map, 1, 0, bpp);
}

static inline int
>>>>>>> upstream/android-13
xfs_buf_read(
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	size_t			numblks,
	xfs_buf_flags_t		flags,
<<<<<<< HEAD
	const struct xfs_buf_ops *ops)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);
	return xfs_buf_read_map(target, &map, 1, flags, ops);
=======
	struct xfs_buf		**bpp,
	const struct xfs_buf_ops *ops)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);

	return xfs_buf_read_map(target, &map, 1, flags, bpp, ops,
			__builtin_return_address(0));
>>>>>>> upstream/android-13
}

static inline void
xfs_buf_readahead(
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	size_t			numblks,
	const struct xfs_buf_ops *ops)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);
	return xfs_buf_readahead_map(target, &map, 1, ops);
}

<<<<<<< HEAD
void xfs_buf_set_empty(struct xfs_buf *bp, size_t numblks);
int xfs_buf_associate_memory(struct xfs_buf *bp, void *mem, size_t length);

struct xfs_buf *xfs_buf_get_uncached(struct xfs_buftarg *target, size_t numblks,
				int flags);
int xfs_buf_read_uncached(struct xfs_buftarg *target, xfs_daddr_t daddr,
			  size_t numblks, int flags, struct xfs_buf **bpp,
			  const struct xfs_buf_ops *ops);
void xfs_buf_hold(struct xfs_buf *bp);

/* Releasing Buffers */
extern void xfs_buf_free(xfs_buf_t *);
extern void xfs_buf_rele(xfs_buf_t *);

/* Locking and Unlocking Buffers */
extern int xfs_buf_trylock(xfs_buf_t *);
extern void xfs_buf_lock(xfs_buf_t *);
extern void xfs_buf_unlock(xfs_buf_t *);
#define xfs_buf_islocked(bp) \
	((bp)->b_sema.count <= 0)

/* Buffer Read and Write Routines */
extern int xfs_bwrite(struct xfs_buf *bp);
extern void xfs_buf_ioend(struct xfs_buf *bp);
extern void __xfs_buf_ioerror(struct xfs_buf *bp, int error,
		xfs_failaddr_t failaddr);
#define xfs_buf_ioerror(bp, err) __xfs_buf_ioerror((bp), (err), __this_address)
extern void xfs_buf_ioerror_alert(struct xfs_buf *, const char *func);

extern int __xfs_buf_submit(struct xfs_buf *bp, bool);
static inline int xfs_buf_submit(struct xfs_buf *bp)
{
	bool wait = bp->b_flags & XBF_ASYNC ? false : true;
	return __xfs_buf_submit(bp, wait);
}

extern void xfs_buf_iomove(xfs_buf_t *, size_t, size_t, void *,
				xfs_buf_rw_t);
#define xfs_buf_zero(bp, off, len) \
	    xfs_buf_iomove((bp), (off), (len), NULL, XBRW_ZERO)
=======
int xfs_buf_get_uncached(struct xfs_buftarg *target, size_t numblks, int flags,
		struct xfs_buf **bpp);
int xfs_buf_read_uncached(struct xfs_buftarg *target, xfs_daddr_t daddr,
			  size_t numblks, int flags, struct xfs_buf **bpp,
			  const struct xfs_buf_ops *ops);
int _xfs_buf_read(struct xfs_buf *bp, xfs_buf_flags_t flags);
void xfs_buf_hold(struct xfs_buf *bp);

/* Releasing Buffers */
extern void xfs_buf_rele(struct xfs_buf *);

/* Locking and Unlocking Buffers */
extern int xfs_buf_trylock(struct xfs_buf *);
extern void xfs_buf_lock(struct xfs_buf *);
extern void xfs_buf_unlock(struct xfs_buf *);
#define xfs_buf_islocked(bp) \
	((bp)->b_sema.count <= 0)

static inline void xfs_buf_relse(struct xfs_buf *bp)
{
	xfs_buf_unlock(bp);
	xfs_buf_rele(bp);
}

/* Buffer Read and Write Routines */
extern int xfs_bwrite(struct xfs_buf *bp);

extern void __xfs_buf_ioerror(struct xfs_buf *bp, int error,
		xfs_failaddr_t failaddr);
#define xfs_buf_ioerror(bp, err) __xfs_buf_ioerror((bp), (err), __this_address)
extern void xfs_buf_ioerror_alert(struct xfs_buf *bp, xfs_failaddr_t fa);
void xfs_buf_ioend_fail(struct xfs_buf *);
void xfs_buf_zero(struct xfs_buf *bp, size_t boff, size_t bsize);
void __xfs_buf_mark_corrupt(struct xfs_buf *bp, xfs_failaddr_t fa);
#define xfs_buf_mark_corrupt(bp) __xfs_buf_mark_corrupt((bp), __this_address)
>>>>>>> upstream/android-13

/* Buffer Utility Routines */
extern void *xfs_buf_offset(struct xfs_buf *, size_t);
extern void xfs_buf_stale(struct xfs_buf *bp);

/* Delayed Write Buffer Routines */
extern void xfs_buf_delwri_cancel(struct list_head *);
extern bool xfs_buf_delwri_queue(struct xfs_buf *, struct list_head *);
extern int xfs_buf_delwri_submit(struct list_head *);
extern int xfs_buf_delwri_submit_nowait(struct list_head *);
extern int xfs_buf_delwri_pushbuf(struct xfs_buf *, struct list_head *);

/* Buffer Daemon Setup Routines */
extern int xfs_buf_init(void);
extern void xfs_buf_terminate(void);

<<<<<<< HEAD
/*
 * These macros use the IO block map rather than b_bn. b_bn is now really
 * just for the buffer cache index for cached buffers. As IO does not use b_bn
 * anymore, uncached buffers do not use b_bn at all and hence must modify the IO
 * map directly. Uncached buffers are not allowed to be discontiguous, so this
 * is safe to do.
 *
 * In future, uncached buffers will pass the block number directly to the io
 * request function and hence these macros will go away at that point.
 */
#define XFS_BUF_ADDR(bp)		((bp)->b_maps[0].bm_bn)
#define XFS_BUF_SET_ADDR(bp, bno)	((bp)->b_maps[0].bm_bn = (xfs_daddr_t)(bno))
=======
static inline xfs_daddr_t xfs_buf_daddr(struct xfs_buf *bp)
{
	return bp->b_maps[0].bm_bn;
}
>>>>>>> upstream/android-13

void xfs_buf_set_ref(struct xfs_buf *bp, int lru_ref);

/*
 * If the buffer is already on the LRU, do nothing. Otherwise set the buffer
 * up with a reference count of 0 so it will be tossed from the cache when
 * released.
 */
static inline void xfs_buf_oneshot(struct xfs_buf *bp)
{
	if (!list_empty(&bp->b_lru) || atomic_read(&bp->b_lru_ref) > 1)
		return;
	atomic_set(&bp->b_lru_ref, 0);
}

static inline int xfs_buf_ispinned(struct xfs_buf *bp)
{
	return atomic_read(&bp->b_pin_count);
}

<<<<<<< HEAD
static inline void xfs_buf_relse(xfs_buf_t *bp)
{
	xfs_buf_unlock(bp);
	xfs_buf_rele(bp);
}

=======
>>>>>>> upstream/android-13
static inline int
xfs_buf_verify_cksum(struct xfs_buf *bp, unsigned long cksum_offset)
{
	return xfs_verify_cksum(bp->b_addr, BBTOB(bp->b_length),
				cksum_offset);
}

static inline void
xfs_buf_update_cksum(struct xfs_buf *bp, unsigned long cksum_offset)
{
	xfs_update_cksum(bp->b_addr, BBTOB(bp->b_length),
			 cksum_offset);
}

/*
 *	Handling of buftargs.
 */
<<<<<<< HEAD
extern xfs_buftarg_t *xfs_alloc_buftarg(struct xfs_mount *,
			struct block_device *, struct dax_device *);
extern void xfs_free_buftarg(struct xfs_buftarg *);
extern void xfs_wait_buftarg(xfs_buftarg_t *);
extern int xfs_setsize_buftarg(xfs_buftarg_t *, unsigned int);
=======
extern struct xfs_buftarg *xfs_alloc_buftarg(struct xfs_mount *,
		struct block_device *, struct dax_device *);
extern void xfs_free_buftarg(struct xfs_buftarg *);
extern void xfs_buftarg_wait(struct xfs_buftarg *);
extern void xfs_buftarg_drain(struct xfs_buftarg *);
extern int xfs_setsize_buftarg(struct xfs_buftarg *, unsigned int);
>>>>>>> upstream/android-13

#define xfs_getsize_buftarg(buftarg)	block_size((buftarg)->bt_bdev)
#define xfs_readonly_buftarg(buftarg)	bdev_read_only((buftarg)->bt_bdev)

<<<<<<< HEAD
=======
int xfs_buf_reverify(struct xfs_buf *bp, const struct xfs_buf_ops *ops);
bool xfs_verify_magic(struct xfs_buf *bp, __be32 dmagic);
bool xfs_verify_magic16(struct xfs_buf *bp, __be16 dmagic);

>>>>>>> upstream/android-13
#endif	/* __XFS_BUF_H__ */
