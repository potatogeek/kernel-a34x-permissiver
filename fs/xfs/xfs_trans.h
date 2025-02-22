// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef	__XFS_TRANS_H__
#define	__XFS_TRANS_H__

/* kernel only transaction subsystem defines */

struct xfs_buf;
struct xfs_buftarg;
struct xfs_efd_log_item;
struct xfs_efi_log_item;
struct xfs_inode;
struct xfs_item_ops;
struct xfs_log_iovec;
struct xfs_mount;
struct xfs_trans;
struct xfs_trans_res;
struct xfs_dquot_acct;
struct xfs_rud_log_item;
struct xfs_rui_log_item;
struct xfs_btree_cur;
struct xfs_cui_log_item;
struct xfs_cud_log_item;
struct xfs_bui_log_item;
struct xfs_bud_log_item;

<<<<<<< HEAD
typedef struct xfs_log_item {
=======
struct xfs_log_item {
>>>>>>> upstream/android-13
	struct list_head		li_ail;		/* AIL pointers */
	struct list_head		li_trans;	/* transaction list */
	xfs_lsn_t			li_lsn;		/* last on-disk lsn */
	struct xfs_mount		*li_mountp;	/* ptr to fs mount */
	struct xfs_ail			*li_ailp;	/* ptr to AIL */
	uint				li_type;	/* item type */
	unsigned long			li_flags;	/* misc flags */
	struct xfs_buf			*li_buf;	/* real buffer pointer */
	struct list_head		li_bio_list;	/* buffer item list */
<<<<<<< HEAD
	void				(*li_cb)(struct xfs_buf *,
						 struct xfs_log_item *);
							/* buffer item iodone */
							/* callback func */
=======
>>>>>>> upstream/android-13
	const struct xfs_item_ops	*li_ops;	/* function list */

	/* delayed logging */
	struct list_head		li_cil;		/* CIL pointers */
	struct xfs_log_vec		*li_lv;		/* active log vector */
	struct xfs_log_vec		*li_lv_shadow;	/* standby vector */
<<<<<<< HEAD
	xfs_lsn_t			li_seq;		/* CIL commit seq */
} xfs_log_item_t;
=======
	xfs_csn_t			li_seq;		/* CIL commit seq */
};
>>>>>>> upstream/android-13

/*
 * li_flags use the (set/test/clear)_bit atomic interfaces because updates can
 * race with each other and we don't want to have to use the AIL lock to
 * serialise all updates.
 */
#define	XFS_LI_IN_AIL	0
#define	XFS_LI_ABORTED	1
#define	XFS_LI_FAILED	2
#define	XFS_LI_DIRTY	3	/* log item dirty in transaction */

#define XFS_LI_FLAGS \
	{ (1 << XFS_LI_IN_AIL),		"IN_AIL" }, \
	{ (1 << XFS_LI_ABORTED),	"ABORTED" }, \
	{ (1 << XFS_LI_FAILED),		"FAILED" }, \
	{ (1 << XFS_LI_DIRTY),		"DIRTY" }

struct xfs_item_ops {
<<<<<<< HEAD
	void (*iop_size)(xfs_log_item_t *, int *, int *);
	void (*iop_format)(xfs_log_item_t *, struct xfs_log_vec *);
	void (*iop_pin)(xfs_log_item_t *);
	void (*iop_unpin)(xfs_log_item_t *, int remove);
	uint (*iop_push)(struct xfs_log_item *, struct list_head *);
	void (*iop_unlock)(xfs_log_item_t *);
	xfs_lsn_t (*iop_committed)(xfs_log_item_t *, xfs_lsn_t);
	void (*iop_committing)(xfs_log_item_t *, xfs_lsn_t);
	void (*iop_error)(xfs_log_item_t *, xfs_buf_t *);
};

=======
	unsigned flags;
	void (*iop_size)(struct xfs_log_item *, int *, int *);
	void (*iop_format)(struct xfs_log_item *, struct xfs_log_vec *);
	void (*iop_pin)(struct xfs_log_item *);
	void (*iop_unpin)(struct xfs_log_item *, int remove);
	uint (*iop_push)(struct xfs_log_item *, struct list_head *);
	void (*iop_committing)(struct xfs_log_item *lip, xfs_csn_t seq);
	void (*iop_release)(struct xfs_log_item *);
	xfs_lsn_t (*iop_committed)(struct xfs_log_item *, xfs_lsn_t);
	int (*iop_recover)(struct xfs_log_item *lip,
			   struct list_head *capture_list);
	bool (*iop_match)(struct xfs_log_item *item, uint64_t id);
	struct xfs_log_item *(*iop_relog)(struct xfs_log_item *intent,
			struct xfs_trans *tp);
};

/* Is this log item a deferred action intent? */
static inline bool
xlog_item_is_intent(struct xfs_log_item *lip)
{
	return lip->li_ops->iop_recover != NULL &&
	       lip->li_ops->iop_match != NULL;
}

/* Is this a log intent-done item? */
static inline bool
xlog_item_is_intent_done(struct xfs_log_item *lip)
{
	return lip->li_ops->iop_unpin == NULL &&
	       lip->li_ops->iop_push == NULL;
}

/*
 * Release the log item as soon as committed.  This is for items just logging
 * intents that never need to be written back in place.
 */
#define XFS_ITEM_RELEASE_WHEN_COMMITTED	(1 << 0)

>>>>>>> upstream/android-13
void	xfs_log_item_init(struct xfs_mount *mp, struct xfs_log_item *item,
			  int type, const struct xfs_item_ops *ops);

/*
 * Return values for the iop_push() routines.
 */
#define XFS_ITEM_SUCCESS	0
#define XFS_ITEM_PINNED		1
#define XFS_ITEM_LOCKED		2
#define XFS_ITEM_FLUSHING	3

/*
 * Deferred operation item relogging limits.
 */
#define XFS_DEFER_OPS_NR_INODES	2	/* join up to two inodes */
#define XFS_DEFER_OPS_NR_BUFS	2	/* join up to two buffers */

/*
 * This is the structure maintained for every active transaction.
 */
typedef struct xfs_trans {
	unsigned int		t_magic;	/* magic number */
	unsigned int		t_log_res;	/* amt of log space resvd */
	unsigned int		t_log_count;	/* count for perm log res */
	unsigned int		t_blk_res;	/* # of blocks resvd */
	unsigned int		t_blk_res_used;	/* # of resvd blocks used */
	unsigned int		t_rtx_res;	/* # of rt extents resvd */
	unsigned int		t_rtx_res_used;	/* # of resvd rt extents used */
	unsigned int		t_flags;	/* misc flags */
	xfs_fsblock_t		t_firstblock;	/* first block allocated */
	struct xlog_ticket	*t_ticket;	/* log mgr ticket */
	struct xfs_mount	*t_mountp;	/* ptr to fs mount struct */
	struct xfs_dquot_acct   *t_dqinfo;	/* acctg info for dquots */
	int64_t			t_icount_delta;	/* superblock icount change */
	int64_t			t_ifree_delta;	/* superblock ifree change */
	int64_t			t_fdblocks_delta; /* superblock fdblocks chg */
	int64_t			t_res_fdblocks_delta; /* on-disk only chg */
	int64_t			t_frextents_delta;/* superblock freextents chg*/
	int64_t			t_res_frextents_delta; /* on-disk only chg */
<<<<<<< HEAD
#if defined(DEBUG) || defined(XFS_WARN)
	int64_t			t_ag_freeblks_delta; /* debugging counter */
	int64_t			t_ag_flist_delta; /* debugging counter */
	int64_t			t_ag_btree_delta; /* debugging counter */
#endif
=======
>>>>>>> upstream/android-13
	int64_t			t_dblocks_delta;/* superblock dblocks change */
	int64_t			t_agcount_delta;/* superblock agcount change */
	int64_t			t_imaxpct_delta;/* superblock imaxpct change */
	int64_t			t_rextsize_delta;/* superblock rextsize chg */
	int64_t			t_rbmblocks_delta;/* superblock rbmblocks chg */
	int64_t			t_rblocks_delta;/* superblock rblocks change */
	int64_t			t_rextents_delta;/* superblocks rextents chg */
	int64_t			t_rextslog_delta;/* superblocks rextslog chg */
	struct list_head	t_items;	/* log item descriptors */
	struct list_head	t_busy;		/* list of busy extents */
	struct list_head	t_dfops;	/* deferred operations */
	unsigned long		t_pflags;	/* saved process flags state */
} xfs_trans_t;

/*
 * XFS transaction mechanism exported interfaces that are
 * actually macros.
 */
#define	xfs_trans_set_sync(tp)		((tp)->t_flags |= XFS_TRANS_SYNC)

<<<<<<< HEAD
#if defined(DEBUG) || defined(XFS_WARN)
#define	xfs_trans_agblocks_delta(tp, d)	((tp)->t_ag_freeblks_delta += (int64_t)d)
#define	xfs_trans_agflist_delta(tp, d)	((tp)->t_ag_flist_delta += (int64_t)d)
#define	xfs_trans_agbtree_delta(tp, d)	((tp)->t_ag_btree_delta += (int64_t)d)
#else
#define	xfs_trans_agblocks_delta(tp, d)
#define	xfs_trans_agflist_delta(tp, d)
#define	xfs_trans_agbtree_delta(tp, d)
#endif

=======
>>>>>>> upstream/android-13
/*
 * XFS transaction mechanism exported interfaces.
 */
int		xfs_trans_alloc(struct xfs_mount *mp, struct xfs_trans_res *resp,
			uint blocks, uint rtextents, uint flags,
			struct xfs_trans **tpp);
int		xfs_trans_alloc_empty(struct xfs_mount *mp,
			struct xfs_trans **tpp);
void		xfs_trans_mod_sb(xfs_trans_t *, uint, int64_t);

<<<<<<< HEAD
struct xfs_buf	*xfs_trans_get_buf_map(struct xfs_trans *tp,
				       struct xfs_buftarg *target,
				       struct xfs_buf_map *map, int nmaps,
				       uint flags);

static inline struct xfs_buf *
=======
int xfs_trans_get_buf_map(struct xfs_trans *tp, struct xfs_buftarg *target,
		struct xfs_buf_map *map, int nmaps, xfs_buf_flags_t flags,
		struct xfs_buf **bpp);

static inline int
>>>>>>> upstream/android-13
xfs_trans_get_buf(
	struct xfs_trans	*tp,
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	int			numblks,
<<<<<<< HEAD
	uint			flags)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);
	return xfs_trans_get_buf_map(tp, target, &map, 1, flags);
=======
	uint			flags,
	struct xfs_buf		**bpp)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);
	return xfs_trans_get_buf_map(tp, target, &map, 1, flags, bpp);
>>>>>>> upstream/android-13
}

int		xfs_trans_read_buf_map(struct xfs_mount *mp,
				       struct xfs_trans *tp,
				       struct xfs_buftarg *target,
				       struct xfs_buf_map *map, int nmaps,
				       xfs_buf_flags_t flags,
				       struct xfs_buf **bpp,
				       const struct xfs_buf_ops *ops);

static inline int
xfs_trans_read_buf(
	struct xfs_mount	*mp,
	struct xfs_trans	*tp,
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	int			numblks,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp,
	const struct xfs_buf_ops *ops)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);
	return xfs_trans_read_buf_map(mp, tp, target, &map, 1,
				      flags, bpp, ops);
}

<<<<<<< HEAD
struct xfs_buf	*xfs_trans_getsb(xfs_trans_t *, struct xfs_mount *, int);
=======
struct xfs_buf	*xfs_trans_getsb(struct xfs_trans *);
>>>>>>> upstream/android-13

void		xfs_trans_brelse(xfs_trans_t *, struct xfs_buf *);
void		xfs_trans_bjoin(xfs_trans_t *, struct xfs_buf *);
void		xfs_trans_bhold(xfs_trans_t *, struct xfs_buf *);
void		xfs_trans_bhold_release(xfs_trans_t *, struct xfs_buf *);
void		xfs_trans_binval(xfs_trans_t *, struct xfs_buf *);
void		xfs_trans_inode_buf(xfs_trans_t *, struct xfs_buf *);
void		xfs_trans_stale_inode_buf(xfs_trans_t *, struct xfs_buf *);
bool		xfs_trans_ordered_buf(xfs_trans_t *, struct xfs_buf *);
void		xfs_trans_dquot_buf(xfs_trans_t *, struct xfs_buf *, uint);
void		xfs_trans_inode_alloc_buf(xfs_trans_t *, struct xfs_buf *);
void		xfs_trans_ichgtime(struct xfs_trans *, struct xfs_inode *, int);
void		xfs_trans_ijoin(struct xfs_trans *, struct xfs_inode *, uint);
void		xfs_trans_log_buf(struct xfs_trans *, struct xfs_buf *, uint,
				  uint);
void		xfs_trans_dirty_buf(struct xfs_trans *, struct xfs_buf *);
<<<<<<< HEAD
void		xfs_trans_log_inode(xfs_trans_t *, struct xfs_inode *, uint);

void		xfs_extent_free_init_defer_op(void);
struct xfs_efd_log_item	*xfs_trans_get_efd(struct xfs_trans *,
				  struct xfs_efi_log_item *,
				  uint);
int		xfs_trans_free_extent(struct xfs_trans *,
				      struct xfs_efd_log_item *, xfs_fsblock_t,
				      xfs_extlen_t, struct xfs_owner_info *,
				      bool);
=======
bool		xfs_trans_buf_is_dirty(struct xfs_buf *bp);
void		xfs_trans_log_inode(xfs_trans_t *, struct xfs_inode *, uint);

>>>>>>> upstream/android-13
int		xfs_trans_commit(struct xfs_trans *);
int		xfs_trans_roll(struct xfs_trans **);
int		xfs_trans_roll_inode(struct xfs_trans **, struct xfs_inode *);
void		xfs_trans_cancel(xfs_trans_t *);
int		xfs_trans_ail_init(struct xfs_mount *);
void		xfs_trans_ail_destroy(struct xfs_mount *);

void		xfs_trans_buf_set_type(struct xfs_trans *, struct xfs_buf *,
				       enum xfs_blft);
void		xfs_trans_buf_copy_type(struct xfs_buf *dst_bp,
					struct xfs_buf *src_bp);

extern kmem_zone_t	*xfs_trans_zone;

<<<<<<< HEAD
/* rmap updates */
enum xfs_rmap_intent_type;

void xfs_rmap_update_init_defer_op(void);
struct xfs_rud_log_item *xfs_trans_get_rud(struct xfs_trans *tp,
		struct xfs_rui_log_item *ruip);
int xfs_trans_log_finish_rmap_update(struct xfs_trans *tp,
		struct xfs_rud_log_item *rudp, enum xfs_rmap_intent_type type,
		uint64_t owner, int whichfork, xfs_fileoff_t startoff,
		xfs_fsblock_t startblock, xfs_filblks_t blockcount,
		xfs_exntst_t state, struct xfs_btree_cur **pcur);

/* refcount updates */
enum xfs_refcount_intent_type;

void xfs_refcount_update_init_defer_op(void);
struct xfs_cud_log_item *xfs_trans_get_cud(struct xfs_trans *tp,
		struct xfs_cui_log_item *cuip);
int xfs_trans_log_finish_refcount_update(struct xfs_trans *tp,
		struct xfs_cud_log_item *cudp,
		enum xfs_refcount_intent_type type, xfs_fsblock_t startblock,
		xfs_extlen_t blockcount, xfs_fsblock_t *new_fsb,
		xfs_extlen_t *new_len, struct xfs_btree_cur **pcur);

/* mapping updates */
enum xfs_bmap_intent_type;

void xfs_bmap_update_init_defer_op(void);
struct xfs_bud_log_item *xfs_trans_get_bud(struct xfs_trans *tp,
		struct xfs_bui_log_item *buip);
int xfs_trans_log_finish_bmap_update(struct xfs_trans *tp,
		struct xfs_bud_log_item *rudp, enum xfs_bmap_intent_type type,
		struct xfs_inode *ip, int whichfork, xfs_fileoff_t startoff,
		xfs_fsblock_t startblock, xfs_filblks_t *blockcount,
		xfs_exntst_t state);
=======
static inline struct xfs_log_item *
xfs_trans_item_relog(
	struct xfs_log_item	*lip,
	struct xfs_trans	*tp)
{
	return lip->li_ops->iop_relog(lip, tp);
}

struct xfs_dquot;

int xfs_trans_alloc_inode(struct xfs_inode *ip, struct xfs_trans_res *resv,
		unsigned int dblocks, unsigned int rblocks, bool force,
		struct xfs_trans **tpp);
int xfs_trans_alloc_icreate(struct xfs_mount *mp, struct xfs_trans_res *resv,
		struct xfs_dquot *udqp, struct xfs_dquot *gdqp,
		struct xfs_dquot *pdqp, unsigned int dblocks,
		struct xfs_trans **tpp);
int xfs_trans_alloc_ichange(struct xfs_inode *ip, struct xfs_dquot *udqp,
		struct xfs_dquot *gdqp, struct xfs_dquot *pdqp, bool force,
		struct xfs_trans **tpp);

static inline void
xfs_trans_set_context(
	struct xfs_trans	*tp)
{
	ASSERT(current->journal_info == NULL);
	tp->t_pflags = memalloc_nofs_save();
	current->journal_info = tp;
}

static inline void
xfs_trans_clear_context(
	struct xfs_trans	*tp)
{
	if (current->journal_info == tp) {
		memalloc_nofs_restore(tp->t_pflags);
		current->journal_info = NULL;
	}
}

static inline void
xfs_trans_switch_context(
	struct xfs_trans	*old_tp,
	struct xfs_trans	*new_tp)
{
	ASSERT(current->journal_info == old_tp);
	new_tp->t_pflags = old_tp->t_pflags;
	old_tp->t_pflags = 0;
	current->journal_info = new_tp;
}
>>>>>>> upstream/android-13

#endif	/* __XFS_TRANS_H__ */
