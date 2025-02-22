// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 */

#include <linux/slab.h>
#include <linux/blkdev.h>
#include <linux/writeback.h>
<<<<<<< HEAD
=======
#include <linux/sched/mm.h>
#include "misc.h"
>>>>>>> upstream/android-13
#include "ctree.h"
#include "transaction.h"
#include "btrfs_inode.h"
#include "extent_io.h"
#include "disk-io.h"
#include "compression.h"
<<<<<<< HEAD
=======
#include "delalloc-space.h"
#include "qgroup.h"
#include "subpage.h"
>>>>>>> upstream/android-13

static struct kmem_cache *btrfs_ordered_extent_cache;

static u64 entry_end(struct btrfs_ordered_extent *entry)
{
<<<<<<< HEAD
	if (entry->file_offset + entry->len < entry->file_offset)
		return (u64)-1;
	return entry->file_offset + entry->len;
=======
	if (entry->file_offset + entry->num_bytes < entry->file_offset)
		return (u64)-1;
	return entry->file_offset + entry->num_bytes;
>>>>>>> upstream/android-13
}

/* returns NULL if the insertion worked, or it returns the node it did find
 * in the tree
 */
static struct rb_node *tree_insert(struct rb_root *root, u64 file_offset,
				   struct rb_node *node)
{
	struct rb_node **p = &root->rb_node;
	struct rb_node *parent = NULL;
	struct btrfs_ordered_extent *entry;

	while (*p) {
		parent = *p;
		entry = rb_entry(parent, struct btrfs_ordered_extent, rb_node);

		if (file_offset < entry->file_offset)
			p = &(*p)->rb_left;
		else if (file_offset >= entry_end(entry))
			p = &(*p)->rb_right;
		else
			return parent;
	}

	rb_link_node(node, parent, p);
	rb_insert_color(node, root);
	return NULL;
}

<<<<<<< HEAD
static void ordered_data_tree_panic(struct inode *inode, int errno,
					       u64 offset)
{
	struct btrfs_fs_info *fs_info = btrfs_sb(inode->i_sb);
	btrfs_panic(fs_info, errno,
		    "Inconsistency in ordered tree at offset %llu", offset);
}

=======
>>>>>>> upstream/android-13
/*
 * look for a given offset in the tree, and if it can't be found return the
 * first lesser offset
 */
static struct rb_node *__tree_search(struct rb_root *root, u64 file_offset,
				     struct rb_node **prev_ret)
{
	struct rb_node *n = root->rb_node;
	struct rb_node *prev = NULL;
	struct rb_node *test;
	struct btrfs_ordered_extent *entry;
	struct btrfs_ordered_extent *prev_entry = NULL;

	while (n) {
		entry = rb_entry(n, struct btrfs_ordered_extent, rb_node);
		prev = n;
		prev_entry = entry;

		if (file_offset < entry->file_offset)
			n = n->rb_left;
		else if (file_offset >= entry_end(entry))
			n = n->rb_right;
		else
			return n;
	}
	if (!prev_ret)
		return NULL;

	while (prev && file_offset >= entry_end(prev_entry)) {
		test = rb_next(prev);
		if (!test)
			break;
		prev_entry = rb_entry(test, struct btrfs_ordered_extent,
				      rb_node);
		if (file_offset < entry_end(prev_entry))
			break;

		prev = test;
	}
	if (prev)
		prev_entry = rb_entry(prev, struct btrfs_ordered_extent,
				      rb_node);
	while (prev && file_offset < entry_end(prev_entry)) {
		test = rb_prev(prev);
		if (!test)
			break;
		prev_entry = rb_entry(test, struct btrfs_ordered_extent,
				      rb_node);
		prev = test;
	}
	*prev_ret = prev;
	return NULL;
}

<<<<<<< HEAD
/*
 * helper to check if a given offset is inside a given entry
 */
static int offset_in_entry(struct btrfs_ordered_extent *entry, u64 file_offset)
{
	if (file_offset < entry->file_offset ||
	    entry->file_offset + entry->len <= file_offset)
		return 0;
	return 1;
}

=======
>>>>>>> upstream/android-13
static int range_overlaps(struct btrfs_ordered_extent *entry, u64 file_offset,
			  u64 len)
{
	if (file_offset + len <= entry->file_offset ||
<<<<<<< HEAD
	    entry->file_offset + entry->len <= file_offset)
=======
	    entry->file_offset + entry->num_bytes <= file_offset)
>>>>>>> upstream/android-13
		return 0;
	return 1;
}

/*
 * look find the first ordered struct that has this offset, otherwise
 * the first one less than this offset
 */
static inline struct rb_node *tree_search(struct btrfs_ordered_inode_tree *tree,
					  u64 file_offset)
{
	struct rb_root *root = &tree->tree;
	struct rb_node *prev = NULL;
	struct rb_node *ret;
	struct btrfs_ordered_extent *entry;

	if (tree->last) {
		entry = rb_entry(tree->last, struct btrfs_ordered_extent,
				 rb_node);
<<<<<<< HEAD
		if (offset_in_entry(entry, file_offset))
=======
		if (in_range(file_offset, entry->file_offset, entry->num_bytes))
>>>>>>> upstream/android-13
			return tree->last;
	}
	ret = __tree_search(root, file_offset, &prev);
	if (!ret)
		ret = prev;
	if (ret)
		tree->last = ret;
	return ret;
}

<<<<<<< HEAD
/* allocate and add a new ordered_extent into the per-inode tree.
 * file_offset is the logical offset in the file
 *
 * start is the disk block number of an extent already reserved in the
 * extent allocation tree
 *
 * len is the length of the extent
=======
/*
 * Allocate and add a new ordered_extent into the per-inode tree.
>>>>>>> upstream/android-13
 *
 * The tree is given a single reference on the ordered extent that was
 * inserted.
 */
<<<<<<< HEAD
static int __btrfs_add_ordered_extent(struct inode *inode, u64 file_offset,
				      u64 start, u64 len, u64 disk_len,
				      int type, int dio, int compress_type)
{
	struct btrfs_fs_info *fs_info = btrfs_sb(inode->i_sb);
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry;

	tree = &BTRFS_I(inode)->ordered_tree;
=======
static int __btrfs_add_ordered_extent(struct btrfs_inode *inode, u64 file_offset,
				      u64 disk_bytenr, u64 num_bytes,
				      u64 disk_num_bytes, int type, int dio,
				      int compress_type)
{
	struct btrfs_root *root = inode->root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct btrfs_ordered_inode_tree *tree = &inode->ordered_tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry;
	int ret;

	if (type == BTRFS_ORDERED_NOCOW || type == BTRFS_ORDERED_PREALLOC) {
		/* For nocow write, we can release the qgroup rsv right now */
		ret = btrfs_qgroup_free_data(inode, NULL, file_offset, num_bytes);
		if (ret < 0)
			return ret;
		ret = 0;
	} else {
		/*
		 * The ordered extent has reserved qgroup space, release now
		 * and pass the reserved number for qgroup_record to free.
		 */
		ret = btrfs_qgroup_release_data(inode, file_offset, num_bytes);
		if (ret < 0)
			return ret;
	}
>>>>>>> upstream/android-13
	entry = kmem_cache_zalloc(btrfs_ordered_extent_cache, GFP_NOFS);
	if (!entry)
		return -ENOMEM;

	entry->file_offset = file_offset;
<<<<<<< HEAD
	entry->start = start;
	entry->len = len;
	entry->disk_len = disk_len;
	entry->bytes_left = len;
	entry->inode = igrab(inode);
	entry->compress_type = compress_type;
	entry->truncated_len = (u64)-1;
	if (type != BTRFS_ORDERED_IO_DONE && type != BTRFS_ORDERED_COMPLETE)
		set_bit(type, &entry->flags);
=======
	entry->disk_bytenr = disk_bytenr;
	entry->num_bytes = num_bytes;
	entry->disk_num_bytes = disk_num_bytes;
	entry->bytes_left = num_bytes;
	entry->inode = igrab(&inode->vfs_inode);
	entry->compress_type = compress_type;
	entry->truncated_len = (u64)-1;
	entry->qgroup_rsv = ret;
	entry->physical = (u64)-1;

	ASSERT(type == BTRFS_ORDERED_REGULAR ||
	       type == BTRFS_ORDERED_NOCOW ||
	       type == BTRFS_ORDERED_PREALLOC ||
	       type == BTRFS_ORDERED_COMPRESSED);
	set_bit(type, &entry->flags);

	percpu_counter_add_batch(&fs_info->ordered_bytes, num_bytes,
				 fs_info->delalloc_batch);
>>>>>>> upstream/android-13

	if (dio)
		set_bit(BTRFS_ORDERED_DIRECT, &entry->flags);

	/* one ref for the tree */
	refcount_set(&entry->refs, 1);
	init_waitqueue_head(&entry->wait);
	INIT_LIST_HEAD(&entry->list);
<<<<<<< HEAD
	INIT_LIST_HEAD(&entry->root_extent_list);
	INIT_LIST_HEAD(&entry->work_list);
	init_completion(&entry->completion);
	INIT_LIST_HEAD(&entry->log_list);
	INIT_LIST_HEAD(&entry->trans_list);
=======
	INIT_LIST_HEAD(&entry->log_list);
	INIT_LIST_HEAD(&entry->root_extent_list);
	INIT_LIST_HEAD(&entry->work_list);
	init_completion(&entry->completion);
>>>>>>> upstream/android-13

	trace_btrfs_ordered_extent_add(inode, entry);

	spin_lock_irq(&tree->lock);
	node = tree_insert(&tree->tree, file_offset,
			   &entry->rb_node);
	if (node)
<<<<<<< HEAD
		ordered_data_tree_panic(inode, -EEXIST, file_offset);
=======
		btrfs_panic(fs_info, -EEXIST,
				"inconsistency in ordered tree at offset %llu",
				file_offset);
>>>>>>> upstream/android-13
	spin_unlock_irq(&tree->lock);

	spin_lock(&root->ordered_extent_lock);
	list_add_tail(&entry->root_extent_list,
		      &root->ordered_extents);
	root->nr_ordered_extents++;
	if (root->nr_ordered_extents == 1) {
		spin_lock(&fs_info->ordered_root_lock);
		BUG_ON(!list_empty(&root->ordered_root));
		list_add_tail(&root->ordered_root, &fs_info->ordered_roots);
		spin_unlock(&fs_info->ordered_root_lock);
	}
	spin_unlock(&root->ordered_extent_lock);

	/*
	 * We don't need the count_max_extents here, we can assume that all of
	 * that work has been done at higher layers, so this is truly the
	 * smallest the extent is going to get.
	 */
<<<<<<< HEAD
	spin_lock(&BTRFS_I(inode)->lock);
	btrfs_mod_outstanding_extents(BTRFS_I(inode), 1);
	spin_unlock(&BTRFS_I(inode)->lock);
=======
	spin_lock(&inode->lock);
	btrfs_mod_outstanding_extents(inode, 1);
	spin_unlock(&inode->lock);
>>>>>>> upstream/android-13

	return 0;
}

<<<<<<< HEAD
int btrfs_add_ordered_extent(struct inode *inode, u64 file_offset,
			     u64 start, u64 len, u64 disk_len, int type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 0,
					  BTRFS_COMPRESS_NONE);
}

int btrfs_add_ordered_extent_dio(struct inode *inode, u64 file_offset,
				 u64 start, u64 len, u64 disk_len, int type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 1,
					  BTRFS_COMPRESS_NONE);
}

int btrfs_add_ordered_extent_compress(struct inode *inode, u64 file_offset,
				      u64 start, u64 len, u64 disk_len,
				      int type, int compress_type)
{
	return __btrfs_add_ordered_extent(inode, file_offset, start, len,
					  disk_len, type, 0,
=======
int btrfs_add_ordered_extent(struct btrfs_inode *inode, u64 file_offset,
			     u64 disk_bytenr, u64 num_bytes, u64 disk_num_bytes,
			     int type)
{
	ASSERT(type == BTRFS_ORDERED_REGULAR ||
	       type == BTRFS_ORDERED_NOCOW ||
	       type == BTRFS_ORDERED_PREALLOC);
	return __btrfs_add_ordered_extent(inode, file_offset, disk_bytenr,
					  num_bytes, disk_num_bytes, type, 0,
					  BTRFS_COMPRESS_NONE);
}

int btrfs_add_ordered_extent_dio(struct btrfs_inode *inode, u64 file_offset,
				 u64 disk_bytenr, u64 num_bytes,
				 u64 disk_num_bytes, int type)
{
	ASSERT(type == BTRFS_ORDERED_REGULAR ||
	       type == BTRFS_ORDERED_NOCOW ||
	       type == BTRFS_ORDERED_PREALLOC);
	return __btrfs_add_ordered_extent(inode, file_offset, disk_bytenr,
					  num_bytes, disk_num_bytes, type, 1,
					  BTRFS_COMPRESS_NONE);
}

int btrfs_add_ordered_extent_compress(struct btrfs_inode *inode, u64 file_offset,
				      u64 disk_bytenr, u64 num_bytes,
				      u64 disk_num_bytes, int compress_type)
{
	ASSERT(compress_type != BTRFS_COMPRESS_NONE);
	return __btrfs_add_ordered_extent(inode, file_offset, disk_bytenr,
					  num_bytes, disk_num_bytes,
					  BTRFS_ORDERED_COMPRESSED, 0,
>>>>>>> upstream/android-13
					  compress_type);
}

/*
 * Add a struct btrfs_ordered_sum into the list of checksums to be inserted
 * when an ordered extent is finished.  If the list covers more than one
 * ordered extent, it is split across multiples.
 */
<<<<<<< HEAD
void btrfs_add_ordered_sum(struct inode *inode,
			   struct btrfs_ordered_extent *entry,
=======
void btrfs_add_ordered_sum(struct btrfs_ordered_extent *entry,
>>>>>>> upstream/android-13
			   struct btrfs_ordered_sum *sum)
{
	struct btrfs_ordered_inode_tree *tree;

<<<<<<< HEAD
	tree = &BTRFS_I(inode)->ordered_tree;
=======
	tree = &BTRFS_I(entry->inode)->ordered_tree;
>>>>>>> upstream/android-13
	spin_lock_irq(&tree->lock);
	list_add_tail(&sum->list, &entry->list);
	spin_unlock_irq(&tree->lock);
}

/*
<<<<<<< HEAD
 * this is used to account for finished IO across a given range
 * of the file.  The IO may span ordered extents.  If
 * a given ordered_extent is completely done, 1 is returned, otherwise
 * 0.
 *
 * test_and_set_bit on a flag in the struct btrfs_ordered_extent is used
 * to make sure this function only returns 1 once for a given ordered extent.
 *
 * file_offset is updated to one byte past the range that is recorded as
 * complete.  This allows you to walk forward in the file.
 */
int btrfs_dec_test_first_ordered_pending(struct inode *inode,
				   struct btrfs_ordered_extent **cached,
				   u64 *file_offset, u64 io_size, int uptodate)
{
	struct btrfs_fs_info *fs_info = btrfs_sb(inode->i_sb);
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	int ret;
	unsigned long flags;
	u64 dec_end;
	u64 dec_start;
	u64 to_dec;

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock_irqsave(&tree->lock, flags);
	node = tree_search(tree, *file_offset);
	if (!node) {
		ret = 1;
		goto out;
	}

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
	if (!offset_in_entry(entry, *file_offset)) {
		ret = 1;
		goto out;
	}

	dec_start = max(*file_offset, entry->file_offset);
	dec_end = min(*file_offset + io_size, entry->file_offset +
		      entry->len);
	*file_offset = dec_end;
	if (dec_start > dec_end) {
		btrfs_crit(fs_info, "bad ordering dec_start %llu end %llu",
			   dec_start, dec_end);
	}
	to_dec = dec_end - dec_start;
	if (to_dec > entry->bytes_left) {
		btrfs_crit(fs_info,
			   "bad ordered accounting left %llu size %llu",
			   entry->bytes_left, to_dec);
	}
	entry->bytes_left -= to_dec;
	if (!uptodate)
		set_bit(BTRFS_ORDERED_IOERR, &entry->flags);

	if (entry->bytes_left == 0) {
		ret = test_and_set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
		/* test_and_set_bit implies a barrier */
		cond_wake_up_nomb(&entry->wait);
	} else {
		ret = 1;
	}
out:
	if (!ret && cached && entry) {
		*cached = entry;
		refcount_inc(&entry->refs);
	}
	spin_unlock_irqrestore(&tree->lock, flags);
	return ret == 0;
}

/*
 * this is used to account for finished IO across a given range
 * of the file.  The IO should not span ordered extents.  If
 * a given ordered_extent is completely done, 1 is returned, otherwise
 * 0.
 *
 * test_and_set_bit on a flag in the struct btrfs_ordered_extent is used
 * to make sure this function only returns 1 once for a given ordered extent.
 */
int btrfs_dec_test_ordered_pending(struct inode *inode,
				   struct btrfs_ordered_extent **cached,
				   u64 file_offset, u64 io_size, int uptodate)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	unsigned long flags;
	int ret;

	tree = &BTRFS_I(inode)->ordered_tree;
=======
 * Mark all ordered extents io inside the specified range finished.
 *
 * @page:	 The invovled page for the opeartion.
 *		 For uncompressed buffered IO, the page status also needs to be
 *		 updated to indicate whether the pending ordered io is finished.
 *		 Can be NULL for direct IO and compressed write.
 *		 For these cases, callers are ensured they won't execute the
 *		 endio function twice.
 * @finish_func: The function to be executed when all the IO of an ordered
 *		 extent are finished.
 *
 * This function is called for endio, thus the range must have ordered
 * extent(s) coveri it.
 */
void btrfs_mark_ordered_io_finished(struct btrfs_inode *inode,
				struct page *page, u64 file_offset,
				u64 num_bytes, btrfs_func_t finish_func,
				bool uptodate)
{
	struct btrfs_ordered_inode_tree *tree = &inode->ordered_tree;
	struct btrfs_fs_info *fs_info = inode->root->fs_info;
	struct btrfs_workqueue *wq;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	unsigned long flags;
	u64 cur = file_offset;

	if (btrfs_is_free_space_inode(inode))
		wq = fs_info->endio_freespace_worker;
	else
		wq = fs_info->endio_write_workers;

	if (page)
		ASSERT(page->mapping && page_offset(page) <= file_offset &&
		       file_offset + num_bytes <= page_offset(page) + PAGE_SIZE);

	spin_lock_irqsave(&tree->lock, flags);
	while (cur < file_offset + num_bytes) {
		u64 entry_end;
		u64 end;
		u32 len;

		node = tree_search(tree, cur);
		/* No ordered extents at all */
		if (!node)
			break;

		entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
		entry_end = entry->file_offset + entry->num_bytes;
		/*
		 * |<-- OE --->|  |
		 *		  cur
		 * Go to next OE.
		 */
		if (cur >= entry_end) {
			node = rb_next(node);
			/* No more ordered extents, exit */
			if (!node)
				break;
			entry = rb_entry(node, struct btrfs_ordered_extent,
					 rb_node);

			/* Go to next ordered extent and continue */
			cur = entry->file_offset;
			continue;
		}
		/*
		 * |	|<--- OE --->|
		 * cur
		 * Go to the start of OE.
		 */
		if (cur < entry->file_offset) {
			cur = entry->file_offset;
			continue;
		}

		/*
		 * Now we are definitely inside one ordered extent.
		 *
		 * |<--- OE --->|
		 *	|
		 *	cur
		 */
		end = min(entry->file_offset + entry->num_bytes,
			  file_offset + num_bytes) - 1;
		ASSERT(end + 1 - cur < U32_MAX);
		len = end + 1 - cur;

		if (page) {
			/*
			 * Ordered (Private2) bit indicates whether we still
			 * have pending io unfinished for the ordered extent.
			 *
			 * If there's no such bit, we need to skip to next range.
			 */
			if (!btrfs_page_test_ordered(fs_info, page, cur, len)) {
				cur += len;
				continue;
			}
			btrfs_page_clear_ordered(fs_info, page, cur, len);
		}

		/* Now we're fine to update the accounting */
		if (unlikely(len > entry->bytes_left)) {
			WARN_ON(1);
			btrfs_crit(fs_info,
"bad ordered extent accounting, root=%llu ino=%llu OE offset=%llu OE len=%llu to_dec=%u left=%llu",
				   inode->root->root_key.objectid,
				   btrfs_ino(inode),
				   entry->file_offset,
				   entry->num_bytes,
				   len, entry->bytes_left);
			entry->bytes_left = 0;
		} else {
			entry->bytes_left -= len;
		}

		if (!uptodate)
			set_bit(BTRFS_ORDERED_IOERR, &entry->flags);

		/*
		 * All the IO of the ordered extent is finished, we need to queue
		 * the finish_func to be executed.
		 */
		if (entry->bytes_left == 0) {
			set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
			cond_wake_up(&entry->wait);
			refcount_inc(&entry->refs);
			spin_unlock_irqrestore(&tree->lock, flags);
			btrfs_init_work(&entry->work, finish_func, NULL, NULL);
			btrfs_queue_work(wq, &entry->work);
			spin_lock_irqsave(&tree->lock, flags);
		}
		cur += len;
	}
	spin_unlock_irqrestore(&tree->lock, flags);
}

/*
 * Finish IO for one ordered extent across a given range.  The range can only
 * contain one ordered extent.
 *
 * @cached:	 The cached ordered extent. If not NULL, we can skip the tree
 *               search and use the ordered extent directly.
 * 		 Will be also used to store the finished ordered extent.
 * @file_offset: File offset for the finished IO
 * @io_size:	 Length of the finish IO range
 *
 * Return true if the ordered extent is finished in the range, and update
 * @cached.
 * Return false otherwise.
 *
 * NOTE: The range can NOT cross multiple ordered extents.
 * Thus caller should ensure the range doesn't cross ordered extents.
 */
bool btrfs_dec_test_ordered_pending(struct btrfs_inode *inode,
				    struct btrfs_ordered_extent **cached,
				    u64 file_offset, u64 io_size)
{
	struct btrfs_ordered_inode_tree *tree = &inode->ordered_tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
	unsigned long flags;
	bool finished = false;

>>>>>>> upstream/android-13
	spin_lock_irqsave(&tree->lock, flags);
	if (cached && *cached) {
		entry = *cached;
		goto have_entry;
	}

	node = tree_search(tree, file_offset);
<<<<<<< HEAD
	if (!node) {
		ret = 1;
		goto out;
	}

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
have_entry:
	if (!offset_in_entry(entry, file_offset)) {
		ret = 1;
		goto out;
	}

	if (io_size > entry->bytes_left) {
		btrfs_crit(BTRFS_I(inode)->root->fs_info,
			   "bad ordered accounting left %llu size %llu",
		       entry->bytes_left, io_size);
	}
	entry->bytes_left -= io_size;
	if (!uptodate)
		set_bit(BTRFS_ORDERED_IOERR, &entry->flags);

	if (entry->bytes_left == 0) {
		ret = test_and_set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
		/* test_and_set_bit implies a barrier */
		cond_wake_up_nomb(&entry->wait);
	} else {
		ret = 1;
	}
out:
	if (!ret && cached && entry) {
=======
	if (!node)
		goto out;

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
have_entry:
	if (!in_range(file_offset, entry->file_offset, entry->num_bytes))
		goto out;

	if (io_size > entry->bytes_left)
		btrfs_crit(inode->root->fs_info,
			   "bad ordered accounting left %llu size %llu",
		       entry->bytes_left, io_size);

	entry->bytes_left -= io_size;

	if (entry->bytes_left == 0) {
		/*
		 * Ensure only one caller can set the flag and finished_ret
		 * accordingly
		 */
		finished = !test_and_set_bit(BTRFS_ORDERED_IO_DONE, &entry->flags);
		/* test_and_set_bit implies a barrier */
		cond_wake_up_nomb(&entry->wait);
	}
out:
	if (finished && cached && entry) {
>>>>>>> upstream/android-13
		*cached = entry;
		refcount_inc(&entry->refs);
	}
	spin_unlock_irqrestore(&tree->lock, flags);
<<<<<<< HEAD
	return ret == 0;
=======
	return finished;
>>>>>>> upstream/android-13
}

/*
 * used to drop a reference on an ordered extent.  This will free
 * the extent if the last reference is dropped
 */
void btrfs_put_ordered_extent(struct btrfs_ordered_extent *entry)
{
	struct list_head *cur;
	struct btrfs_ordered_sum *sum;

<<<<<<< HEAD
	trace_btrfs_ordered_extent_put(entry->inode, entry);

	if (refcount_dec_and_test(&entry->refs)) {
		ASSERT(list_empty(&entry->log_list));
		ASSERT(list_empty(&entry->trans_list));
		ASSERT(list_empty(&entry->root_extent_list));
=======
	trace_btrfs_ordered_extent_put(BTRFS_I(entry->inode), entry);

	if (refcount_dec_and_test(&entry->refs)) {
		ASSERT(list_empty(&entry->root_extent_list));
		ASSERT(list_empty(&entry->log_list));
>>>>>>> upstream/android-13
		ASSERT(RB_EMPTY_NODE(&entry->rb_node));
		if (entry->inode)
			btrfs_add_delayed_iput(entry->inode);
		while (!list_empty(&entry->list)) {
			cur = entry->list.next;
			sum = list_entry(cur, struct btrfs_ordered_sum, list);
			list_del(&sum->list);
<<<<<<< HEAD
			kfree(sum);
=======
			kvfree(sum);
>>>>>>> upstream/android-13
		}
		kmem_cache_free(btrfs_ordered_extent_cache, entry);
	}
}

/*
 * remove an ordered extent from the tree.  No references are dropped
 * and waiters are woken up.
 */
<<<<<<< HEAD
void btrfs_remove_ordered_extent(struct inode *inode,
				 struct btrfs_ordered_extent *entry)
{
	struct btrfs_fs_info *fs_info = btrfs_sb(inode->i_sb);
	struct btrfs_ordered_inode_tree *tree;
	struct btrfs_inode *btrfs_inode = BTRFS_I(inode);
	struct btrfs_root *root = btrfs_inode->root;
	struct rb_node *node;
	bool dec_pending_ordered = false;
=======
void btrfs_remove_ordered_extent(struct btrfs_inode *btrfs_inode,
				 struct btrfs_ordered_extent *entry)
{
	struct btrfs_ordered_inode_tree *tree;
	struct btrfs_root *root = btrfs_inode->root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct rb_node *node;
	bool pending;
>>>>>>> upstream/android-13

	/* This is paired with btrfs_add_ordered_extent. */
	spin_lock(&btrfs_inode->lock);
	btrfs_mod_outstanding_extents(btrfs_inode, -1);
	spin_unlock(&btrfs_inode->lock);
	if (root != fs_info->tree_root)
<<<<<<< HEAD
		btrfs_delalloc_release_metadata(btrfs_inode, entry->len, false);
=======
		btrfs_delalloc_release_metadata(btrfs_inode, entry->num_bytes,
						false);

	percpu_counter_add_batch(&fs_info->ordered_bytes, -entry->num_bytes,
				 fs_info->delalloc_batch);
>>>>>>> upstream/android-13

	tree = &btrfs_inode->ordered_tree;
	spin_lock_irq(&tree->lock);
	node = &entry->rb_node;
	rb_erase(node, &tree->tree);
	RB_CLEAR_NODE(node);
	if (tree->last == node)
		tree->last = NULL;
	set_bit(BTRFS_ORDERED_COMPLETE, &entry->flags);
<<<<<<< HEAD
	if (test_and_clear_bit(BTRFS_ORDERED_PENDING, &entry->flags))
		dec_pending_ordered = true;
=======
	pending = test_and_clear_bit(BTRFS_ORDERED_PENDING, &entry->flags);
>>>>>>> upstream/android-13
	spin_unlock_irq(&tree->lock);

	/*
	 * The current running transaction is waiting on us, we need to let it
	 * know that we're complete and wake it up.
	 */
<<<<<<< HEAD
	if (dec_pending_ordered) {
=======
	if (pending) {
>>>>>>> upstream/android-13
		struct btrfs_transaction *trans;

		/*
		 * The checks for trans are just a formality, it should be set,
		 * but if it isn't we don't want to deref/assert under the spin
		 * lock, so be nice and check if trans is set, but ASSERT() so
		 * if it isn't set a developer will notice.
		 */
		spin_lock(&fs_info->trans_lock);
		trans = fs_info->running_transaction;
		if (trans)
			refcount_inc(&trans->use_count);
		spin_unlock(&fs_info->trans_lock);

		ASSERT(trans);
		if (trans) {
			if (atomic_dec_and_test(&trans->pending_ordered))
				wake_up(&trans->pending_wait);
			btrfs_put_transaction(trans);
		}
	}

	spin_lock(&root->ordered_extent_lock);
	list_del_init(&entry->root_extent_list);
	root->nr_ordered_extents--;

<<<<<<< HEAD
	trace_btrfs_ordered_extent_remove(inode, entry);
=======
	trace_btrfs_ordered_extent_remove(btrfs_inode, entry);
>>>>>>> upstream/android-13

	if (!root->nr_ordered_extents) {
		spin_lock(&fs_info->ordered_root_lock);
		BUG_ON(list_empty(&root->ordered_root));
		list_del_init(&root->ordered_root);
		spin_unlock(&fs_info->ordered_root_lock);
	}
	spin_unlock(&root->ordered_extent_lock);
	wake_up(&entry->wait);
}

static void btrfs_run_ordered_extent_work(struct btrfs_work *work)
{
	struct btrfs_ordered_extent *ordered;

	ordered = container_of(work, struct btrfs_ordered_extent, flush_work);
<<<<<<< HEAD
	btrfs_start_ordered_extent(ordered->inode, ordered, 1);
=======
	btrfs_start_ordered_extent(ordered, 1);
>>>>>>> upstream/android-13
	complete(&ordered->completion);
}

/*
 * wait for all the ordered extents in a root.  This is done when balancing
 * space between drives.
 */
u64 btrfs_wait_ordered_extents(struct btrfs_root *root, u64 nr,
			       const u64 range_start, const u64 range_len)
{
	struct btrfs_fs_info *fs_info = root->fs_info;
	LIST_HEAD(splice);
	LIST_HEAD(skipped);
	LIST_HEAD(works);
	struct btrfs_ordered_extent *ordered, *next;
	u64 count = 0;
	const u64 range_end = range_start + range_len;

	mutex_lock(&root->ordered_extent_mutex);
	spin_lock(&root->ordered_extent_lock);
	list_splice_init(&root->ordered_extents, &splice);
	while (!list_empty(&splice) && nr) {
		ordered = list_first_entry(&splice, struct btrfs_ordered_extent,
					   root_extent_list);

<<<<<<< HEAD
		if (range_end <= ordered->start ||
		    ordered->start + ordered->disk_len <= range_start) {
=======
		if (range_end <= ordered->disk_bytenr ||
		    ordered->disk_bytenr + ordered->disk_num_bytes <= range_start) {
>>>>>>> upstream/android-13
			list_move_tail(&ordered->root_extent_list, &skipped);
			cond_resched_lock(&root->ordered_extent_lock);
			continue;
		}

		list_move_tail(&ordered->root_extent_list,
			       &root->ordered_extents);
		refcount_inc(&ordered->refs);
		spin_unlock(&root->ordered_extent_lock);

		btrfs_init_work(&ordered->flush_work,
<<<<<<< HEAD
				btrfs_flush_delalloc_helper,
=======
>>>>>>> upstream/android-13
				btrfs_run_ordered_extent_work, NULL, NULL);
		list_add_tail(&ordered->work_list, &works);
		btrfs_queue_work(fs_info->flush_workers, &ordered->flush_work);

		cond_resched();
		spin_lock(&root->ordered_extent_lock);
		if (nr != U64_MAX)
			nr--;
		count++;
	}
	list_splice_tail(&skipped, &root->ordered_extents);
	list_splice_tail(&splice, &root->ordered_extents);
	spin_unlock(&root->ordered_extent_lock);

	list_for_each_entry_safe(ordered, next, &works, work_list) {
		list_del_init(&ordered->work_list);
		wait_for_completion(&ordered->completion);
		btrfs_put_ordered_extent(ordered);
		cond_resched();
	}
	mutex_unlock(&root->ordered_extent_mutex);

	return count;
}

<<<<<<< HEAD
u64 btrfs_wait_ordered_roots(struct btrfs_fs_info *fs_info, u64 nr,
=======
void btrfs_wait_ordered_roots(struct btrfs_fs_info *fs_info, u64 nr,
>>>>>>> upstream/android-13
			     const u64 range_start, const u64 range_len)
{
	struct btrfs_root *root;
	struct list_head splice;
<<<<<<< HEAD
	u64 total_done = 0;
=======
>>>>>>> upstream/android-13
	u64 done;

	INIT_LIST_HEAD(&splice);

	mutex_lock(&fs_info->ordered_operations_mutex);
	spin_lock(&fs_info->ordered_root_lock);
	list_splice_init(&fs_info->ordered_roots, &splice);
	while (!list_empty(&splice) && nr) {
		root = list_first_entry(&splice, struct btrfs_root,
					ordered_root);
<<<<<<< HEAD
		root = btrfs_grab_fs_root(root);
=======
		root = btrfs_grab_root(root);
>>>>>>> upstream/android-13
		BUG_ON(!root);
		list_move_tail(&root->ordered_root,
			       &fs_info->ordered_roots);
		spin_unlock(&fs_info->ordered_root_lock);

		done = btrfs_wait_ordered_extents(root, nr,
						  range_start, range_len);
<<<<<<< HEAD
		btrfs_put_fs_root(root);
		total_done += done;
=======
		btrfs_put_root(root);
>>>>>>> upstream/android-13

		spin_lock(&fs_info->ordered_root_lock);
		if (nr != U64_MAX) {
			nr -= done;
		}
	}
	list_splice_tail(&splice, &fs_info->ordered_roots);
	spin_unlock(&fs_info->ordered_root_lock);
	mutex_unlock(&fs_info->ordered_operations_mutex);
<<<<<<< HEAD

	return total_done;
=======
>>>>>>> upstream/android-13
}

/*
 * Used to start IO or wait for a given ordered extent to finish.
 *
 * If wait is one, this effectively waits on page writeback for all the pages
 * in the extent, and it waits on the io completion code to insert
 * metadata into the btree corresponding to the extent
 */
<<<<<<< HEAD
void btrfs_start_ordered_extent(struct inode *inode,
				       struct btrfs_ordered_extent *entry,
				       int wait)
{
	u64 start = entry->file_offset;
	u64 end = start + entry->len - 1;
=======
void btrfs_start_ordered_extent(struct btrfs_ordered_extent *entry, int wait)
{
	u64 start = entry->file_offset;
	u64 end = start + entry->num_bytes - 1;
	struct btrfs_inode *inode = BTRFS_I(entry->inode);
>>>>>>> upstream/android-13

	trace_btrfs_ordered_extent_start(inode, entry);

	/*
	 * pages in the range can be dirty, clean or writeback.  We
	 * start IO on any dirty ones so the wait doesn't stall waiting
	 * for the flusher thread to find them
	 */
	if (!test_bit(BTRFS_ORDERED_DIRECT, &entry->flags))
<<<<<<< HEAD
		filemap_fdatawrite_range(inode->i_mapping, start, end);
=======
		filemap_fdatawrite_range(inode->vfs_inode.i_mapping, start, end);
>>>>>>> upstream/android-13
	if (wait) {
		wait_event(entry->wait, test_bit(BTRFS_ORDERED_COMPLETE,
						 &entry->flags));
	}
}

/*
 * Used to wait on ordered extents across a large range of bytes.
 */
int btrfs_wait_ordered_range(struct inode *inode, u64 start, u64 len)
{
	int ret = 0;
	int ret_wb = 0;
	u64 end;
	u64 orig_end;
	struct btrfs_ordered_extent *ordered;

	if (start + len < start) {
		orig_end = INT_LIMIT(loff_t);
	} else {
		orig_end = start + len - 1;
		if (orig_end > INT_LIMIT(loff_t))
			orig_end = INT_LIMIT(loff_t);
	}

	/* start IO across the range first to instantiate any delalloc
	 * extents
	 */
	ret = btrfs_fdatawrite_range(inode, start, orig_end);
	if (ret)
		return ret;

	/*
	 * If we have a writeback error don't return immediately. Wait first
	 * for any ordered extents that haven't completed yet. This is to make
	 * sure no one can dirty the same page ranges and call writepages()
	 * before the ordered extents complete - to avoid failures (-EEXIST)
	 * when adding the new ordered extents to the ordered tree.
	 */
	ret_wb = filemap_fdatawait_range(inode->i_mapping, start, orig_end);

	end = orig_end;
	while (1) {
<<<<<<< HEAD
		ordered = btrfs_lookup_first_ordered_extent(inode, end);
=======
		ordered = btrfs_lookup_first_ordered_extent(BTRFS_I(inode), end);
>>>>>>> upstream/android-13
		if (!ordered)
			break;
		if (ordered->file_offset > orig_end) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
<<<<<<< HEAD
		if (ordered->file_offset + ordered->len <= start) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
		btrfs_start_ordered_extent(inode, ordered, 1);
=======
		if (ordered->file_offset + ordered->num_bytes <= start) {
			btrfs_put_ordered_extent(ordered);
			break;
		}
		btrfs_start_ordered_extent(ordered, 1);
>>>>>>> upstream/android-13
		end = ordered->file_offset;
		/*
		 * If the ordered extent had an error save the error but don't
		 * exit without waiting first for all other ordered extents in
		 * the range to complete.
		 */
		if (test_bit(BTRFS_ORDERED_IOERR, &ordered->flags))
			ret = -EIO;
		btrfs_put_ordered_extent(ordered);
		if (end == 0 || end == start)
			break;
		end--;
	}
	return ret_wb ? ret_wb : ret;
}

/*
 * find an ordered extent corresponding to file_offset.  return NULL if
 * nothing is found, otherwise take a reference on the extent and return it
 */
<<<<<<< HEAD
struct btrfs_ordered_extent *btrfs_lookup_ordered_extent(struct inode *inode,
=======
struct btrfs_ordered_extent *btrfs_lookup_ordered_extent(struct btrfs_inode *inode,
>>>>>>> upstream/android-13
							 u64 file_offset)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;
<<<<<<< HEAD

	tree = &BTRFS_I(inode)->ordered_tree;
	spin_lock_irq(&tree->lock);
=======
	unsigned long flags;

	tree = &inode->ordered_tree;
	spin_lock_irqsave(&tree->lock, flags);
>>>>>>> upstream/android-13
	node = tree_search(tree, file_offset);
	if (!node)
		goto out;

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
<<<<<<< HEAD
	if (!offset_in_entry(entry, file_offset))
=======
	if (!in_range(file_offset, entry->file_offset, entry->num_bytes))
>>>>>>> upstream/android-13
		entry = NULL;
	if (entry)
		refcount_inc(&entry->refs);
out:
<<<<<<< HEAD
	spin_unlock_irq(&tree->lock);
=======
	spin_unlock_irqrestore(&tree->lock, flags);
>>>>>>> upstream/android-13
	return entry;
}

/* Since the DIO code tries to lock a wide area we need to look for any ordered
 * extents that exist in the range, rather than just the start of the range.
 */
struct btrfs_ordered_extent *btrfs_lookup_ordered_range(
		struct btrfs_inode *inode, u64 file_offset, u64 len)
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

	tree = &inode->ordered_tree;
	spin_lock_irq(&tree->lock);
	node = tree_search(tree, file_offset);
	if (!node) {
		node = tree_search(tree, file_offset + len);
		if (!node)
			goto out;
	}

	while (1) {
		entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
		if (range_overlaps(entry, file_offset, len))
			break;

		if (entry->file_offset >= file_offset + len) {
			entry = NULL;
			break;
		}
		entry = NULL;
		node = rb_next(node);
		if (!node)
			break;
	}
out:
	if (entry)
		refcount_inc(&entry->refs);
	spin_unlock_irq(&tree->lock);
	return entry;
}

/*
<<<<<<< HEAD
=======
 * Adds all ordered extents to the given list. The list ends up sorted by the
 * file_offset of the ordered extents.
 */
void btrfs_get_ordered_extents_for_logging(struct btrfs_inode *inode,
					   struct list_head *list)
{
	struct btrfs_ordered_inode_tree *tree = &inode->ordered_tree;
	struct rb_node *n;

	ASSERT(inode_is_locked(&inode->vfs_inode));

	spin_lock_irq(&tree->lock);
	for (n = rb_first(&tree->tree); n; n = rb_next(n)) {
		struct btrfs_ordered_extent *ordered;

		ordered = rb_entry(n, struct btrfs_ordered_extent, rb_node);

		if (test_bit(BTRFS_ORDERED_LOGGED, &ordered->flags))
			continue;

		ASSERT(list_empty(&ordered->log_list));
		list_add_tail(&ordered->log_list, list);
		refcount_inc(&ordered->refs);
	}
	spin_unlock_irq(&tree->lock);
}

/*
>>>>>>> upstream/android-13
 * lookup and return any extent before 'file_offset'.  NULL is returned
 * if none is found
 */
struct btrfs_ordered_extent *
<<<<<<< HEAD
btrfs_lookup_first_ordered_extent(struct inode *inode, u64 file_offset)
=======
btrfs_lookup_first_ordered_extent(struct btrfs_inode *inode, u64 file_offset)
>>>>>>> upstream/android-13
{
	struct btrfs_ordered_inode_tree *tree;
	struct rb_node *node;
	struct btrfs_ordered_extent *entry = NULL;

<<<<<<< HEAD
	tree = &BTRFS_I(inode)->ordered_tree;
=======
	tree = &inode->ordered_tree;
>>>>>>> upstream/android-13
	spin_lock_irq(&tree->lock);
	node = tree_search(tree, file_offset);
	if (!node)
		goto out;

	entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);
	refcount_inc(&entry->refs);
out:
	spin_unlock_irq(&tree->lock);
	return entry;
}

/*
<<<<<<< HEAD
 * After an extent is done, call this to conditionally update the on disk
 * i_size.  i_size is updated to cover any fully written part of the file.
 */
int btrfs_ordered_update_i_size(struct inode *inode, u64 offset,
				struct btrfs_ordered_extent *ordered)
{
	struct btrfs_ordered_inode_tree *tree = &BTRFS_I(inode)->ordered_tree;
	u64 disk_i_size;
	u64 new_i_size;
	u64 i_size = i_size_read(inode);
	struct rb_node *node;
	struct rb_node *prev = NULL;
	struct btrfs_ordered_extent *test;
	int ret = 1;
	u64 orig_offset = offset;

	spin_lock_irq(&tree->lock);
	if (ordered) {
		offset = entry_end(ordered);
		if (test_bit(BTRFS_ORDERED_TRUNCATED, &ordered->flags))
			offset = min(offset,
				     ordered->file_offset +
				     ordered->truncated_len);
	} else {
		offset = ALIGN(offset, btrfs_inode_sectorsize(inode));
	}
	disk_i_size = BTRFS_I(inode)->disk_i_size;

	/*
	 * truncate file.
	 * If ordered is not NULL, then this is called from endio and
	 * disk_i_size will be updated by either truncate itself or any
	 * in-flight IOs which are inside the disk_i_size.
	 *
	 * Because btrfs_setsize() may set i_size with disk_i_size if truncate
	 * fails somehow, we need to make sure we have a precise disk_i_size by
	 * updating it as usual.
	 *
	 */
	if (!ordered && disk_i_size > i_size) {
		BTRFS_I(inode)->disk_i_size = orig_offset;
		ret = 0;
		goto out;
	}

	/*
	 * if the disk i_size is already at the inode->i_size, or
	 * this ordered extent is inside the disk i_size, we're done
	 */
	if (disk_i_size == i_size)
		goto out;

	/*
	 * We still need to update disk_i_size if outstanding_isize is greater
	 * than disk_i_size.
	 */
	if (offset <= disk_i_size &&
	    (!ordered || ordered->outstanding_isize <= disk_i_size))
		goto out;

	/*
	 * walk backward from this ordered extent to disk_i_size.
	 * if we find an ordered extent then we can't update disk i_size
	 * yet
	 */
	if (ordered) {
		node = rb_prev(&ordered->rb_node);
	} else {
		prev = tree_search(tree, offset);
		/*
		 * we insert file extents without involving ordered struct,
		 * so there should be no ordered struct cover this offset
		 */
		if (prev) {
			test = rb_entry(prev, struct btrfs_ordered_extent,
					rb_node);
			BUG_ON(offset_in_entry(test, offset));
		}
		node = prev;
	}
	for (; node; node = rb_prev(node)) {
		test = rb_entry(node, struct btrfs_ordered_extent, rb_node);

		/* We treat this entry as if it doesn't exist */
		if (test_bit(BTRFS_ORDERED_UPDATED_ISIZE, &test->flags))
			continue;

		if (entry_end(test) <= disk_i_size)
			break;
		if (test->file_offset >= i_size)
			break;

		/*
		 * We don't update disk_i_size now, so record this undealt
		 * i_size. Or we will not know the real i_size.
		 */
		if (test->outstanding_isize < offset)
			test->outstanding_isize = offset;
		if (ordered &&
		    ordered->outstanding_isize > test->outstanding_isize)
			test->outstanding_isize = ordered->outstanding_isize;
		goto out;
	}
	new_i_size = min_t(u64, offset, i_size);

	/*
	 * Some ordered extents may completed before the current one, and
	 * we hold the real i_size in ->outstanding_isize.
	 */
	if (ordered && ordered->outstanding_isize > new_i_size)
		new_i_size = min_t(u64, ordered->outstanding_isize, i_size);
	BTRFS_I(inode)->disk_i_size = new_i_size;
	ret = 0;
out:
	/*
	 * We need to do this because we can't remove ordered extents until
	 * after the i_disk_size has been updated and then the inode has been
	 * updated to reflect the change, so we need to tell anybody who finds
	 * this ordered extent that we've already done all the real work, we
	 * just haven't completed all the other work.
	 */
	if (ordered)
		set_bit(BTRFS_ORDERED_UPDATED_ISIZE, &ordered->flags);
	spin_unlock_irq(&tree->lock);
	return ret;
}

/*
 * search the ordered extents for one corresponding to 'offset' and
 * try to find a checksum.  This is used because we allow pages to
 * be reclaimed before their checksum is actually put into the btree
 */
int btrfs_find_ordered_sum(struct inode *inode, u64 offset, u64 disk_bytenr,
			   u32 *sum, int len)
{
	struct btrfs_ordered_sum *ordered_sum;
	struct btrfs_ordered_extent *ordered;
	struct btrfs_ordered_inode_tree *tree = &BTRFS_I(inode)->ordered_tree;
	unsigned long num_sectors;
	unsigned long i;
	u32 sectorsize = btrfs_inode_sectorsize(inode);
	int index = 0;

	ordered = btrfs_lookup_ordered_extent(inode, offset);
	if (!ordered)
		return 0;

	spin_lock_irq(&tree->lock);
	list_for_each_entry_reverse(ordered_sum, &ordered->list, list) {
		if (disk_bytenr >= ordered_sum->bytenr &&
		    disk_bytenr < ordered_sum->bytenr + ordered_sum->len) {
			i = (disk_bytenr - ordered_sum->bytenr) >>
			    inode->i_sb->s_blocksize_bits;
			num_sectors = ordered_sum->len >>
				      inode->i_sb->s_blocksize_bits;
			num_sectors = min_t(int, len - index, num_sectors - i);
			memcpy(sum + index, ordered_sum->sums + i,
			       num_sectors);

			index += (int)num_sectors;
			if (index == len)
				goto out;
			disk_bytenr += num_sectors * sectorsize;
		}
	}
out:
	spin_unlock_irq(&tree->lock);
	btrfs_put_ordered_extent(ordered);
	return index;
=======
 * Lookup the first ordered extent that overlaps the range
 * [@file_offset, @file_offset + @len).
 *
 * The difference between this and btrfs_lookup_first_ordered_extent() is
 * that this one won't return any ordered extent that does not overlap the range.
 * And the difference against btrfs_lookup_ordered_extent() is, this function
 * ensures the first ordered extent gets returned.
 */
struct btrfs_ordered_extent *btrfs_lookup_first_ordered_range(
			struct btrfs_inode *inode, u64 file_offset, u64 len)
{
	struct btrfs_ordered_inode_tree *tree = &inode->ordered_tree;
	struct rb_node *node;
	struct rb_node *cur;
	struct rb_node *prev;
	struct rb_node *next;
	struct btrfs_ordered_extent *entry = NULL;

	spin_lock_irq(&tree->lock);
	node = tree->tree.rb_node;
	/*
	 * Here we don't want to use tree_search() which will use tree->last
	 * and screw up the search order.
	 * And __tree_search() can't return the adjacent ordered extents
	 * either, thus here we do our own search.
	 */
	while (node) {
		entry = rb_entry(node, struct btrfs_ordered_extent, rb_node);

		if (file_offset < entry->file_offset) {
			node = node->rb_left;
		} else if (file_offset >= entry_end(entry)) {
			node = node->rb_right;
		} else {
			/*
			 * Direct hit, got an ordered extent that starts at
			 * @file_offset
			 */
			goto out;
		}
	}
	if (!entry) {
		/* Empty tree */
		goto out;
	}

	cur = &entry->rb_node;
	/* We got an entry around @file_offset, check adjacent entries */
	if (entry->file_offset < file_offset) {
		prev = cur;
		next = rb_next(cur);
	} else {
		prev = rb_prev(cur);
		next = cur;
	}
	if (prev) {
		entry = rb_entry(prev, struct btrfs_ordered_extent, rb_node);
		if (range_overlaps(entry, file_offset, len))
			goto out;
	}
	if (next) {
		entry = rb_entry(next, struct btrfs_ordered_extent, rb_node);
		if (range_overlaps(entry, file_offset, len))
			goto out;
	}
	/* No ordered extent in the range */
	entry = NULL;
out:
	if (entry)
		refcount_inc(&entry->refs);
	spin_unlock_irq(&tree->lock);
	return entry;
}

/*
 * btrfs_flush_ordered_range - Lock the passed range and ensures all pending
 * ordered extents in it are run to completion.
 *
 * @inode:        Inode whose ordered tree is to be searched
 * @start:        Beginning of range to flush
 * @end:          Last byte of range to lock
 * @cached_state: If passed, will return the extent state responsible for the
 * locked range. It's the caller's responsibility to free the cached state.
 *
 * This function always returns with the given range locked, ensuring after it's
 * called no order extent can be pending.
 */
void btrfs_lock_and_flush_ordered_range(struct btrfs_inode *inode, u64 start,
					u64 end,
					struct extent_state **cached_state)
{
	struct btrfs_ordered_extent *ordered;
	struct extent_state *cache = NULL;
	struct extent_state **cachedp = &cache;

	if (cached_state)
		cachedp = cached_state;

	while (1) {
		lock_extent_bits(&inode->io_tree, start, end, cachedp);
		ordered = btrfs_lookup_ordered_range(inode, start,
						     end - start + 1);
		if (!ordered) {
			/*
			 * If no external cached_state has been passed then
			 * decrement the extra ref taken for cachedp since we
			 * aren't exposing it outside of this function
			 */
			if (!cached_state)
				refcount_dec(&cache->refs);
			break;
		}
		unlock_extent_cached(&inode->io_tree, start, end, cachedp);
		btrfs_start_ordered_extent(ordered, 1);
		btrfs_put_ordered_extent(ordered);
	}
}

static int clone_ordered_extent(struct btrfs_ordered_extent *ordered, u64 pos,
				u64 len)
{
	struct inode *inode = ordered->inode;
	struct btrfs_fs_info *fs_info = BTRFS_I(inode)->root->fs_info;
	u64 file_offset = ordered->file_offset + pos;
	u64 disk_bytenr = ordered->disk_bytenr + pos;
	u64 num_bytes = len;
	u64 disk_num_bytes = len;
	int type;
	unsigned long flags_masked = ordered->flags & ~(1 << BTRFS_ORDERED_DIRECT);
	int compress_type = ordered->compress_type;
	unsigned long weight;
	int ret;

	weight = hweight_long(flags_masked);
	WARN_ON_ONCE(weight > 1);
	if (!weight)
		type = 0;
	else
		type = __ffs(flags_masked);

	/*
	 * The splitting extent is already counted and will be added again
	 * in btrfs_add_ordered_extent_*(). Subtract num_bytes to avoid
	 * double counting.
	 */
	percpu_counter_add_batch(&fs_info->ordered_bytes, -num_bytes,
				 fs_info->delalloc_batch);
	if (test_bit(BTRFS_ORDERED_COMPRESSED, &ordered->flags)) {
		WARN_ON_ONCE(1);
		ret = btrfs_add_ordered_extent_compress(BTRFS_I(inode),
				file_offset, disk_bytenr, num_bytes,
				disk_num_bytes, compress_type);
	} else if (test_bit(BTRFS_ORDERED_DIRECT, &ordered->flags)) {
		ret = btrfs_add_ordered_extent_dio(BTRFS_I(inode), file_offset,
				disk_bytenr, num_bytes, disk_num_bytes, type);
	} else {
		ret = btrfs_add_ordered_extent(BTRFS_I(inode), file_offset,
				disk_bytenr, num_bytes, disk_num_bytes, type);
	}

	return ret;
}

int btrfs_split_ordered_extent(struct btrfs_ordered_extent *ordered, u64 pre,
				u64 post)
{
	struct inode *inode = ordered->inode;
	struct btrfs_ordered_inode_tree *tree = &BTRFS_I(inode)->ordered_tree;
	struct rb_node *node;
	struct btrfs_fs_info *fs_info = btrfs_sb(inode->i_sb);
	int ret = 0;

	spin_lock_irq(&tree->lock);
	/* Remove from tree once */
	node = &ordered->rb_node;
	rb_erase(node, &tree->tree);
	RB_CLEAR_NODE(node);
	if (tree->last == node)
		tree->last = NULL;

	ordered->file_offset += pre;
	ordered->disk_bytenr += pre;
	ordered->num_bytes -= (pre + post);
	ordered->disk_num_bytes -= (pre + post);
	ordered->bytes_left -= (pre + post);

	/* Re-insert the node */
	node = tree_insert(&tree->tree, ordered->file_offset, &ordered->rb_node);
	if (node)
		btrfs_panic(fs_info, -EEXIST,
			"zoned: inconsistency in ordered tree at offset %llu",
			    ordered->file_offset);

	spin_unlock_irq(&tree->lock);

	if (pre)
		ret = clone_ordered_extent(ordered, 0, pre);
	if (ret == 0 && post)
		ret = clone_ordered_extent(ordered, pre + ordered->disk_num_bytes,
					   post);

	return ret;
>>>>>>> upstream/android-13
}

int __init ordered_data_init(void)
{
	btrfs_ordered_extent_cache = kmem_cache_create("btrfs_ordered_extent",
				     sizeof(struct btrfs_ordered_extent), 0,
				     SLAB_MEM_SPREAD,
				     NULL);
	if (!btrfs_ordered_extent_cache)
		return -ENOMEM;

	return 0;
}

void __cold ordered_data_exit(void)
{
	kmem_cache_destroy(btrfs_ordered_extent_cache);
}
