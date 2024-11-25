// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2018 Oracle.  All Rights Reserved.
 * Author: Darrick J. Wong <darrick.wong@oracle.com>
 */
#ifndef __XFS_SCRUB_BITMAP_H__
#define __XFS_SCRUB_BITMAP_H__

<<<<<<< HEAD
struct xfs_bitmap_range {
=======
struct xbitmap_range {
>>>>>>> upstream/android-13
	struct list_head	list;
	uint64_t		start;
	uint64_t		len;
};

<<<<<<< HEAD
struct xfs_bitmap {
	struct list_head	list;
};

void xfs_bitmap_init(struct xfs_bitmap *bitmap);
void xfs_bitmap_destroy(struct xfs_bitmap *bitmap);

#define for_each_xfs_bitmap_extent(bex, n, bitmap) \
	list_for_each_entry_safe((bex), (n), &(bitmap)->list, list)

#define for_each_xfs_bitmap_block(b, bex, n, bitmap) \
	list_for_each_entry_safe((bex), (n), &(bitmap)->list, list) \
		for ((b) = bex->start; (b) < bex->start + bex->len; (b)++)

int xfs_bitmap_set(struct xfs_bitmap *bitmap, uint64_t start, uint64_t len);
int xfs_bitmap_disunion(struct xfs_bitmap *bitmap, struct xfs_bitmap *sub);
int xfs_bitmap_set_btcur_path(struct xfs_bitmap *bitmap,
		struct xfs_btree_cur *cur);
int xfs_bitmap_set_btblocks(struct xfs_bitmap *bitmap,
		struct xfs_btree_cur *cur);
=======
struct xbitmap {
	struct list_head	list;
};

void xbitmap_init(struct xbitmap *bitmap);
void xbitmap_destroy(struct xbitmap *bitmap);

#define for_each_xbitmap_extent(bex, n, bitmap) \
	list_for_each_entry_safe((bex), (n), &(bitmap)->list, list)

#define for_each_xbitmap_block(b, bex, n, bitmap) \
	list_for_each_entry_safe((bex), (n), &(bitmap)->list, list) \
		for ((b) = (bex)->start; (b) < (bex)->start + (bex)->len; (b)++)

int xbitmap_set(struct xbitmap *bitmap, uint64_t start, uint64_t len);
int xbitmap_disunion(struct xbitmap *bitmap, struct xbitmap *sub);
int xbitmap_set_btcur_path(struct xbitmap *bitmap,
		struct xfs_btree_cur *cur);
int xbitmap_set_btblocks(struct xbitmap *bitmap,
		struct xfs_btree_cur *cur);
uint64_t xbitmap_hweight(struct xbitmap *bitmap);
>>>>>>> upstream/android-13

#endif	/* __XFS_SCRUB_BITMAP_H__ */
