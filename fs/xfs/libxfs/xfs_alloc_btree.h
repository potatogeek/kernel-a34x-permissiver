<<<<<<< HEAD
// SPDX-License-Identifier: GPL-2.0
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> upstream/android-13
/*
 * Copyright (c) 2000,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef __XFS_ALLOC_BTREE_H__
#define	__XFS_ALLOC_BTREE_H__

/*
 * Freespace on-disk structures
 */

struct xfs_buf;
struct xfs_btree_cur;
struct xfs_mount;
<<<<<<< HEAD
=======
struct xfs_perag;
struct xbtree_afakeroot;
>>>>>>> upstream/android-13

/*
 * Btree block header size depends on a superblock flag.
 */
#define XFS_ALLOC_BLOCK_LEN(mp) \
<<<<<<< HEAD
	(xfs_sb_version_hascrc(&((mp)->m_sb)) ? \
=======
	(xfs_has_crc(((mp))) ? \
>>>>>>> upstream/android-13
		XFS_BTREE_SBLOCK_CRC_LEN : XFS_BTREE_SBLOCK_LEN)

/*
 * Record, key, and pointer address macros for btree blocks.
 *
 * (note that some of these may appear unused, but they are used in userspace)
 */
#define XFS_ALLOC_REC_ADDR(mp, block, index) \
	((xfs_alloc_rec_t *) \
		((char *)(block) + \
		 XFS_ALLOC_BLOCK_LEN(mp) + \
		 (((index) - 1) * sizeof(xfs_alloc_rec_t))))

#define XFS_ALLOC_KEY_ADDR(mp, block, index) \
	((xfs_alloc_key_t *) \
		((char *)(block) + \
		 XFS_ALLOC_BLOCK_LEN(mp) + \
		 ((index) - 1) * sizeof(xfs_alloc_key_t)))

#define XFS_ALLOC_PTR_ADDR(mp, block, index, maxrecs) \
	((xfs_alloc_ptr_t *) \
		((char *)(block) + \
		 XFS_ALLOC_BLOCK_LEN(mp) + \
		 (maxrecs) * sizeof(xfs_alloc_key_t) + \
		 ((index) - 1) * sizeof(xfs_alloc_ptr_t)))

<<<<<<< HEAD
extern struct xfs_btree_cur *xfs_allocbt_init_cursor(struct xfs_mount *,
		struct xfs_trans *, struct xfs_buf *,
		xfs_agnumber_t, xfs_btnum_t);
=======
extern struct xfs_btree_cur *xfs_allocbt_init_cursor(struct xfs_mount *mp,
		struct xfs_trans *tp, struct xfs_buf *bp,
		struct xfs_perag *pag, xfs_btnum_t btnum);
struct xfs_btree_cur *xfs_allocbt_stage_cursor(struct xfs_mount *mp,
		struct xbtree_afakeroot *afake, struct xfs_perag *pag,
		xfs_btnum_t btnum);
>>>>>>> upstream/android-13
extern int xfs_allocbt_maxrecs(struct xfs_mount *, int, int);
extern xfs_extlen_t xfs_allocbt_calc_size(struct xfs_mount *mp,
		unsigned long long len);

<<<<<<< HEAD
=======
void xfs_allocbt_commit_staged_btree(struct xfs_btree_cur *cur,
		struct xfs_trans *tp, struct xfs_buf *agbp);

>>>>>>> upstream/android-13
#endif	/* __XFS_ALLOC_BTREE_H__ */
