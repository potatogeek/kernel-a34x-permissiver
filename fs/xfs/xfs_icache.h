// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef XFS_SYNC_H
#define XFS_SYNC_H 1

struct xfs_mount;
struct xfs_perag;

<<<<<<< HEAD
struct xfs_eofblocks {
	__u32		eof_flags;
	kuid_t		eof_uid;
	kgid_t		eof_gid;
	prid_t		eof_prid;
	__u64		eof_min_file_size;
};

#define SYNC_WAIT		0x0001	/* wait for i/o to complete */
#define SYNC_TRYLOCK		0x0002  /* only try to lock inodes */

/*
 * tags for inode radix tree
 */
#define XFS_ICI_NO_TAG		(-1)	/* special flag for an untagged lookup
					   in xfs_inode_ag_iterator */
#define XFS_ICI_RECLAIM_TAG	0	/* inode is to be reclaimed */
#define XFS_ICI_EOFBLOCKS_TAG	1	/* inode has blocks beyond EOF */
#define XFS_ICI_COWBLOCKS_TAG	2	/* inode can have cow blocks to gc */
=======
struct xfs_icwalk {
	__u32		icw_flags;
	kuid_t		icw_uid;
	kgid_t		icw_gid;
	prid_t		icw_prid;
	__u64		icw_min_file_size;
	long		icw_scan_limit;
};

/* Flags that reflect xfs_fs_eofblocks functionality. */
#define XFS_ICWALK_FLAG_SYNC		(1U << 0) /* sync/wait mode scan */
#define XFS_ICWALK_FLAG_UID		(1U << 1) /* filter by uid */
#define XFS_ICWALK_FLAG_GID		(1U << 2) /* filter by gid */
#define XFS_ICWALK_FLAG_PRID		(1U << 3) /* filter by project id */
#define XFS_ICWALK_FLAG_MINFILESIZE	(1U << 4) /* filter by min file size */

#define XFS_ICWALK_FLAGS_VALID		(XFS_ICWALK_FLAG_SYNC | \
					 XFS_ICWALK_FLAG_UID | \
					 XFS_ICWALK_FLAG_GID | \
					 XFS_ICWALK_FLAG_PRID | \
					 XFS_ICWALK_FLAG_MINFILESIZE)
>>>>>>> upstream/android-13

/*
 * Flags for xfs_iget()
 */
#define XFS_IGET_CREATE		0x1
#define XFS_IGET_UNTRUSTED	0x2
#define XFS_IGET_DONTCACHE	0x4
#define XFS_IGET_INCORE		0x8	/* don't read from disk or reinit */

<<<<<<< HEAD
/*
 * flags for AG inode iterator
 */
#define XFS_AGITER_INEW_WAIT	0x1	/* wait on new inodes */

=======
>>>>>>> upstream/android-13
int xfs_iget(struct xfs_mount *mp, struct xfs_trans *tp, xfs_ino_t ino,
	     uint flags, uint lock_flags, xfs_inode_t **ipp);

/* recovery needs direct inode allocation capability */
struct xfs_inode * xfs_inode_alloc(struct xfs_mount *mp, xfs_ino_t ino);
void xfs_inode_free(struct xfs_inode *ip);

void xfs_reclaim_worker(struct work_struct *work);

<<<<<<< HEAD
int xfs_reclaim_inodes(struct xfs_mount *mp, int mode);
int xfs_reclaim_inodes_count(struct xfs_mount *mp);
long xfs_reclaim_inodes_nr(struct xfs_mount *mp, int nr_to_scan);

void xfs_inode_set_reclaim_tag(struct xfs_inode *ip);

void xfs_inode_set_eofblocks_tag(struct xfs_inode *ip);
void xfs_inode_clear_eofblocks_tag(struct xfs_inode *ip);
int xfs_icache_free_eofblocks(struct xfs_mount *, struct xfs_eofblocks *);
int xfs_inode_free_quota_eofblocks(struct xfs_inode *ip);
void xfs_eofblocks_worker(struct work_struct *);
void xfs_queue_eofblocks(struct xfs_mount *);

void xfs_inode_set_cowblocks_tag(struct xfs_inode *ip);
void xfs_inode_clear_cowblocks_tag(struct xfs_inode *ip);
int xfs_icache_free_cowblocks(struct xfs_mount *, struct xfs_eofblocks *);
int xfs_inode_free_quota_cowblocks(struct xfs_inode *ip);
void xfs_cowblocks_worker(struct work_struct *);
void xfs_queue_cowblocks(struct xfs_mount *);

int xfs_inode_ag_iterator(struct xfs_mount *mp,
	int (*execute)(struct xfs_inode *ip, int flags, void *args),
	int flags, void *args);
int xfs_inode_ag_iterator_flags(struct xfs_mount *mp,
	int (*execute)(struct xfs_inode *ip, int flags, void *args),
	int flags, void *args, int iter_flags);
int xfs_inode_ag_iterator_tag(struct xfs_mount *mp,
	int (*execute)(struct xfs_inode *ip, int flags, void *args),
	int flags, void *args, int tag);

static inline int
xfs_fs_eofblocks_from_user(
	struct xfs_fs_eofblocks		*src,
	struct xfs_eofblocks		*dst)
{
	if (src->eof_version != XFS_EOFBLOCKS_VERSION)
		return -EINVAL;

	if (src->eof_flags & ~XFS_EOF_FLAGS_VALID)
		return -EINVAL;

	if (memchr_inv(&src->pad32, 0, sizeof(src->pad32)) ||
	    memchr_inv(src->pad64, 0, sizeof(src->pad64)))
		return -EINVAL;

	dst->eof_flags = src->eof_flags;
	dst->eof_prid = src->eof_prid;
	dst->eof_min_file_size = src->eof_min_file_size;

	dst->eof_uid = INVALID_UID;
	if (src->eof_flags & XFS_EOF_FLAGS_UID) {
		dst->eof_uid = make_kuid(current_user_ns(), src->eof_uid);
		if (!uid_valid(dst->eof_uid))
			return -EINVAL;
	}

	dst->eof_gid = INVALID_GID;
	if (src->eof_flags & XFS_EOF_FLAGS_GID) {
		dst->eof_gid = make_kgid(current_user_ns(), src->eof_gid);
		if (!gid_valid(dst->eof_gid))
			return -EINVAL;
	}
	return 0;
}
=======
void xfs_reclaim_inodes(struct xfs_mount *mp);
long xfs_reclaim_inodes_count(struct xfs_mount *mp);
long xfs_reclaim_inodes_nr(struct xfs_mount *mp, unsigned long nr_to_scan);

void xfs_inode_mark_reclaimable(struct xfs_inode *ip);

int xfs_blockgc_free_dquots(struct xfs_mount *mp, struct xfs_dquot *udqp,
		struct xfs_dquot *gdqp, struct xfs_dquot *pdqp,
		unsigned int iwalk_flags);
int xfs_blockgc_free_quota(struct xfs_inode *ip, unsigned int iwalk_flags);
int xfs_blockgc_free_space(struct xfs_mount *mp, struct xfs_icwalk *icm);
void xfs_blockgc_flush_all(struct xfs_mount *mp);

void xfs_inode_set_eofblocks_tag(struct xfs_inode *ip);
void xfs_inode_clear_eofblocks_tag(struct xfs_inode *ip);

void xfs_inode_set_cowblocks_tag(struct xfs_inode *ip);
void xfs_inode_clear_cowblocks_tag(struct xfs_inode *ip);

void xfs_blockgc_worker(struct work_struct *work);
>>>>>>> upstream/android-13

int xfs_icache_inode_is_allocated(struct xfs_mount *mp, struct xfs_trans *tp,
				  xfs_ino_t ino, bool *inuse);

<<<<<<< HEAD
void xfs_icache_disable_reclaim(struct xfs_mount *mp);
void xfs_icache_enable_reclaim(struct xfs_mount *mp);
=======
void xfs_blockgc_stop(struct xfs_mount *mp);
void xfs_blockgc_start(struct xfs_mount *mp);

void xfs_inodegc_worker(struct work_struct *work);
void xfs_inodegc_flush(struct xfs_mount *mp);
void xfs_inodegc_stop(struct xfs_mount *mp);
void xfs_inodegc_start(struct xfs_mount *mp);
void xfs_inodegc_cpu_dead(struct xfs_mount *mp, unsigned int cpu);
int xfs_inodegc_register_shrinker(struct xfs_mount *mp);
>>>>>>> upstream/android-13

#endif
