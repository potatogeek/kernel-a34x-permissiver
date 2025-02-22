/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 */

#ifndef BTRFS_INODE_H
#define BTRFS_INODE_H

#include <linux/hash.h>
<<<<<<< HEAD
=======
#include <linux/refcount.h>
>>>>>>> upstream/android-13
#include "extent_map.h"
#include "extent_io.h"
#include "ordered-data.h"
#include "delayed-inode.h"

/*
 * ordered_data_close is set by truncate when a file that used
 * to have good data has been truncated to zero.  When it is set
 * the btrfs file release call will add this inode to the
 * ordered operations list so that we make sure to flush out any
 * new data the application may have written before commit.
 */
enum {
<<<<<<< HEAD
	BTRFS_INODE_ORDERED_DATA_CLOSE = 0,
	BTRFS_INODE_DUMMY,
	BTRFS_INODE_IN_DEFRAG,
	BTRFS_INODE_HAS_ASYNC_EXTENT,
	BTRFS_INODE_NEEDS_FULL_SYNC,
	BTRFS_INODE_COPY_EVERYTHING,
	BTRFS_INODE_IN_DELALLOC_LIST,
	BTRFS_INODE_READDIO_NEED_LOCK,
	BTRFS_INODE_HAS_PROPS,
	BTRFS_INODE_SNAPSHOT_FLUSH,
=======
	BTRFS_INODE_FLUSH_ON_CLOSE,
	BTRFS_INODE_DUMMY,
	BTRFS_INODE_IN_DEFRAG,
	BTRFS_INODE_HAS_ASYNC_EXTENT,
	 /*
	  * Always set under the VFS' inode lock, otherwise it can cause races
	  * during fsync (we start as a fast fsync and then end up in a full
	  * fsync racing with ordered extent completion).
	  */
	BTRFS_INODE_NEEDS_FULL_SYNC,
	BTRFS_INODE_COPY_EVERYTHING,
	BTRFS_INODE_IN_DELALLOC_LIST,
	BTRFS_INODE_HAS_PROPS,
	BTRFS_INODE_SNAPSHOT_FLUSH,
	/*
	 * Set and used when logging an inode and it serves to signal that an
	 * inode does not have xattrs, so subsequent fsyncs can avoid searching
	 * for xattrs to log. This bit must be cleared whenever a xattr is added
	 * to an inode.
	 */
	BTRFS_INODE_NO_XATTRS,
	/*
	 * Set when we are in a context where we need to start a transaction and
	 * have dirty pages with the respective file range locked. This is to
	 * ensure that when reserving space for the transaction, if we are low
	 * on available space and need to flush delalloc, we will not flush
	 * delalloc for this inode, because that could result in a deadlock (on
	 * the file range, inode's io_tree).
	 */
	BTRFS_INODE_NO_DELALLOC_FLUSH,
	/*
	 * Set when we are working on enabling verity for a file. Computing and
	 * writing the whole Merkle tree can take a while so we want to prevent
	 * races where two separate tasks attempt to simultaneously start verity
	 * on the same file.
	 */
	BTRFS_INODE_VERITY_IN_PROGRESS,
>>>>>>> upstream/android-13
};

/* in memory btrfs inode */
struct btrfs_inode {
	/* which subvolume this inode belongs to */
	struct btrfs_root *root;

	/* key used to find this inode on disk.  This is used by the code
	 * to read in roots of subvolumes
	 */
	struct btrfs_key location;

	/*
	 * Lock for counters and all fields used to determine if the inode is in
	 * the log or not (last_trans, last_sub_trans, last_log_commit,
<<<<<<< HEAD
	 * logged_trans).
=======
	 * logged_trans), to access/update new_delalloc_bytes and to update the
	 * VFS' inode number of bytes used.
>>>>>>> upstream/android-13
	 */
	spinlock_t lock;

	/* the extent_tree has caches of all the extent mappings to disk */
	struct extent_map_tree extent_tree;

	/* the io_tree does range state (DIRTY, LOCKED etc) */
	struct extent_io_tree io_tree;

	/* special utility tree used to record which mirrors have already been
	 * tried when checksums fail for a given block
	 */
	struct extent_io_tree io_failure_tree;

<<<<<<< HEAD
	/* held while logging the inode in tree-log.c */
	struct mutex log_mutex;

	/* held while doing delalloc reservations */
	struct mutex delalloc_mutex;

=======
	/*
	 * Keep track of where the inode has extent items mapped in order to
	 * make sure the i_size adjustments are accurate
	 */
	struct extent_io_tree file_extent_tree;

	/* held while logging the inode in tree-log.c */
	struct mutex log_mutex;

>>>>>>> upstream/android-13
	/* used to order data wrt metadata */
	struct btrfs_ordered_inode_tree ordered_tree;

	/* list of all the delalloc inodes in the FS.  There are times we need
	 * to write all the delalloc pages to disk, and this list is used
	 * to walk them all.
	 */
	struct list_head delalloc_inodes;

	/* node for the red-black tree that links inodes in subvolume root */
	struct rb_node rb_node;

	unsigned long runtime_flags;

	/* Keep track of who's O_SYNC/fsyncing currently */
	atomic_t sync_writers;

	/* full 64 bit generation number, struct vfs_inode doesn't have a big
	 * enough field for this.
	 */
	u64 generation;

	/*
	 * transid of the trans_handle that last modified this inode
	 */
	u64 last_trans;

	/*
	 * transid that last logged this inode
	 */
	u64 logged_trans;

	/*
	 * log transid when this inode was last modified
	 */
	int last_sub_trans;

	/* a local copy of root's last_log_commit */
	int last_log_commit;

	/* total number of bytes pending delalloc, used by stat to calc the
	 * real block usage of the file
	 */
	u64 delalloc_bytes;

	/*
	 * Total number of bytes pending delalloc that fall within a file
	 * range that is either a hole or beyond EOF (and no prealloc extent
	 * exists in the range). This is always <= delalloc_bytes.
	 */
	u64 new_delalloc_bytes;

	/*
	 * total number of bytes pending defrag, used by stat to check whether
	 * it needs COW.
	 */
	u64 defrag_bytes;

	/*
	 * the size of the file stored in the metadata on disk.  data=ordered
	 * means the in-memory i_size might be larger than the size on disk
	 * because not all the blocks are written yet.
	 */
	u64 disk_i_size;

	/*
	 * if this is a directory then index_cnt is the counter for the index
	 * number for new files that are created
	 */
	u64 index_cnt;

	/* Cache the directory index number to speed the dir/file remove */
	u64 dir_index;

	/* the fsync log has some corner cases that mean we have to check
	 * directories to see if any unlinks have been done before
	 * the directory was logged.  See tree-log.c for all the
	 * details
	 */
	u64 last_unlink_trans;

	/*
<<<<<<< HEAD
	 * Track the transaction id of the last transaction used to create a
	 * hard link for the inode. This is used by the log tree (fsync).
	 */
	u64 last_link_trans;
=======
	 * The id/generation of the last transaction where this inode was
	 * either the source or the destination of a clone/dedupe operation.
	 * Used when logging an inode to know if there are shared extents that
	 * need special care when logging checksum items, to avoid duplicate
	 * checksum items in a log (which can lead to a corruption where we end
	 * up with missing checksum ranges after log replay).
	 * Protected by the vfs inode lock.
	 */
	u64 last_reflink_trans;
>>>>>>> upstream/android-13

	/*
	 * Number of bytes outstanding that are going to need csums.  This is
	 * used in ENOSPC accounting.
	 */
	u64 csum_bytes;

<<<<<<< HEAD
	/* flags field from the on disk inode */
	u32 flags;
=======
	/* Backwards incompatible flags, lower half of inode_item::flags  */
	u32 flags;
	/* Read-only compatibility flags, upper half of inode_item::flags */
	u32 ro_flags;
>>>>>>> upstream/android-13

	/*
	 * Counters to keep track of the number of extent item's we may use due
	 * to delalloc and such.  outstanding_extents is the number of extent
	 * items we think we'll end up using, and reserved_extents is the number
	 * of extent items we've reserved metadata for.
	 */
	unsigned outstanding_extents;

	struct btrfs_block_rsv block_rsv;

	/*
	 * Cached values of inode properties
	 */
	unsigned prop_compress;		/* per-file compression algorithm */
	/*
	 * Force compression on the file using the defrag ioctl, could be
	 * different from prop_compress and takes precedence if set
	 */
	unsigned defrag_compress;

	struct btrfs_delayed_node *delayed_node;

	/* File creation time. */
	struct timespec64 i_otime;

	/* Hook into fs_info->delayed_iputs */
	struct list_head delayed_iput;

<<<<<<< HEAD
	/*
	 * To avoid races between lockless (i_mutex not held) direct IO writes
	 * and concurrent fsync requests. Direct IO writes must acquire read
	 * access on this semaphore for creating an extent map and its
	 * corresponding ordered extent. The fast fsync path must acquire write
	 * access on this semaphore before it collects ordered extents and
	 * extent maps.
	 */
	struct rw_semaphore dio_sem;

	struct inode vfs_inode;
};

extern unsigned char btrfs_filetype_table[];
=======
	struct rw_semaphore i_mmap_lock;
	struct inode vfs_inode;
};

static inline u32 btrfs_inode_sectorsize(const struct btrfs_inode *inode)
{
	return inode->root->fs_info->sectorsize;
}
>>>>>>> upstream/android-13

static inline struct btrfs_inode *BTRFS_I(const struct inode *inode)
{
	return container_of(inode, struct btrfs_inode, vfs_inode);
}

static inline unsigned long btrfs_inode_hash(u64 objectid,
					     const struct btrfs_root *root)
{
<<<<<<< HEAD
	u64 h = objectid ^ (root->objectid * GOLDEN_RATIO_PRIME);
=======
	u64 h = objectid ^ (root->root_key.objectid * GOLDEN_RATIO_PRIME);
>>>>>>> upstream/android-13

#if BITS_PER_LONG == 32
	h = (h >> 32) ^ (h & 0xffffffff);
#endif

	return (unsigned long)h;
}

static inline void btrfs_insert_inode_hash(struct inode *inode)
{
	unsigned long h = btrfs_inode_hash(inode->i_ino, BTRFS_I(inode)->root);

	__insert_inode_hash(inode, h);
}

static inline u64 btrfs_ino(const struct btrfs_inode *inode)
{
	u64 ino = inode->location.objectid;

	/*
	 * !ino: btree_inode
	 * type == BTRFS_ROOT_ITEM_KEY: subvol dir
	 */
	if (!ino || inode->location.type == BTRFS_ROOT_ITEM_KEY)
		ino = inode->vfs_inode.i_ino;
	return ino;
}

static inline void btrfs_i_size_write(struct btrfs_inode *inode, u64 size)
{
	i_size_write(&inode->vfs_inode, size);
	inode->disk_i_size = size;
}

static inline bool btrfs_is_free_space_inode(struct btrfs_inode *inode)
{
	struct btrfs_root *root = inode->root;

	if (root == root->fs_info->tree_root &&
	    btrfs_ino(inode) != BTRFS_BTREE_INODE_OBJECTID)
		return true;
	if (inode->location.objectid == BTRFS_FREE_INO_OBJECTID)
		return true;
	return false;
}

<<<<<<< HEAD
=======
static inline bool is_data_inode(struct inode *inode)
{
	return btrfs_ino(BTRFS_I(inode)) != BTRFS_BTREE_INODE_OBJECTID;
}

>>>>>>> upstream/android-13
static inline void btrfs_mod_outstanding_extents(struct btrfs_inode *inode,
						 int mod)
{
	lockdep_assert_held(&inode->lock);
	inode->outstanding_extents += mod;
	if (btrfs_is_free_space_inode(inode))
		return;
	trace_btrfs_inode_mod_outstanding_extents(inode->root, btrfs_ino(inode),
						  mod);
}

<<<<<<< HEAD
static inline int btrfs_inode_in_log(struct btrfs_inode *inode, u64 generation)
{
	int ret = 0;
=======
/*
 * Called every time after doing a buffered, direct IO or memory mapped write.
 *
 * This is to ensure that if we write to a file that was previously fsynced in
 * the current transaction, then try to fsync it again in the same transaction,
 * we will know that there were changes in the file and that it needs to be
 * logged.
 */
static inline void btrfs_set_inode_last_sub_trans(struct btrfs_inode *inode)
{
	spin_lock(&inode->lock);
	inode->last_sub_trans = inode->root->log_transid;
	spin_unlock(&inode->lock);
}

static inline bool btrfs_inode_in_log(struct btrfs_inode *inode, u64 generation)
{
	bool ret = false;
>>>>>>> upstream/android-13

	spin_lock(&inode->lock);
	if (inode->logged_trans == generation &&
	    inode->last_sub_trans <= inode->last_log_commit &&
<<<<<<< HEAD
	    inode->last_sub_trans <= inode->root->last_log_commit) {
		/*
		 * After a ranged fsync we might have left some extent maps
		 * (that fall outside the fsync's range). So return false
		 * here if the list isn't empty, to make sure btrfs_log_inode()
		 * will be called and process those extent maps.
		 */
		smp_mb();
		if (list_empty(&inode->extent_tree.modified_extents))
			ret = 1;
	}
=======
	    inode->last_sub_trans <= inode->root->last_log_commit)
		ret = true;
>>>>>>> upstream/android-13
	spin_unlock(&inode->lock);
	return ret;
}

<<<<<<< HEAD
#define BTRFS_DIO_ORIG_BIO_SUBMITTED	0x1

struct btrfs_dio_private {
	struct inode *inode;
	unsigned long flags;
	u64 logical_offset;
	u64 disk_bytenr;
	u64 bytes;
	void *private;

	/* number of bios pending for this dio */
	atomic_t pending_bios;

	/* IO errors */
	int errors;

	/* orig_bio is our btrfs_io_bio */
	struct bio *orig_bio;
=======
struct btrfs_dio_private {
	struct inode *inode;
	u64 logical_offset;
	u64 disk_bytenr;
	/* Used for bio::bi_size */
	u32 bytes;

	/*
	 * References to this structure. There is one reference per in-flight
	 * bio plus one while we're still setting up.
	 */
	refcount_t refs;
>>>>>>> upstream/android-13

	/* dio_bio came from fs/direct-io.c */
	struct bio *dio_bio;

<<<<<<< HEAD
	/*
	 * The original bio may be split to several sub-bios, this is
	 * done during endio of sub-bios
	 */
	blk_status_t (*subio_endio)(struct inode *, struct btrfs_io_bio *,
			blk_status_t);
};

/*
 * Disable DIO read nolock optimization, so new dio readers will be forced
 * to grab i_mutex. It is used to avoid the endless truncate due to
 * nonlocked dio read.
 */
static inline void btrfs_inode_block_unlocked_dio(struct btrfs_inode *inode)
{
	set_bit(BTRFS_INODE_READDIO_NEED_LOCK, &inode->runtime_flags);
	smp_mb();
}

static inline void btrfs_inode_resume_unlocked_dio(struct btrfs_inode *inode)
{
	smp_mb__before_atomic();
	clear_bit(BTRFS_INODE_READDIO_NEED_LOCK, &inode->runtime_flags);
}

static inline void btrfs_print_data_csum_error(struct btrfs_inode *inode,
		u64 logical_start, u32 csum, u32 csum_expected, int mirror_num)
{
	struct btrfs_root *root = inode->root;

	/* Output minus objectid, which is more meaningful */
	if (root->objectid >= BTRFS_LAST_FREE_OBJECTID)
		btrfs_warn_rl(root->fs_info,
	"csum failed root %lld ino %lld off %llu csum 0x%08x expected csum 0x%08x mirror %d",
			root->objectid, btrfs_ino(inode),
			logical_start, csum, csum_expected, mirror_num);
	else
		btrfs_warn_rl(root->fs_info,
	"csum failed root %llu ino %llu off %llu csum 0x%08x expected csum 0x%08x mirror %d",
			root->objectid, btrfs_ino(inode),
			logical_start, csum, csum_expected, mirror_num);
=======
	/* Array of checksums */
	u8 csums[];
};

/*
 * btrfs_inode_item stores flags in a u64, btrfs_inode stores them in two
 * separate u32s. These two functions convert between the two representations.
 */
static inline u64 btrfs_inode_combine_flags(u32 flags, u32 ro_flags)
{
	return (flags | ((u64)ro_flags << 32));
}

static inline void btrfs_inode_split_flags(u64 inode_item_flags,
					   u32 *flags, u32 *ro_flags)
{
	*flags = (u32)inode_item_flags;
	*ro_flags = (u32)(inode_item_flags >> 32);
}

/* Array of bytes with variable length, hexadecimal format 0x1234 */
#define CSUM_FMT				"0x%*phN"
#define CSUM_FMT_VALUE(size, bytes)		size, bytes

static inline void btrfs_print_data_csum_error(struct btrfs_inode *inode,
		u64 logical_start, u8 *csum, u8 *csum_expected, int mirror_num)
{
	struct btrfs_root *root = inode->root;
	const u32 csum_size = root->fs_info->csum_size;

	/* Output minus objectid, which is more meaningful */
	if (root->root_key.objectid >= BTRFS_LAST_FREE_OBJECTID)
		btrfs_warn_rl(root->fs_info,
"csum failed root %lld ino %lld off %llu csum " CSUM_FMT " expected csum " CSUM_FMT " mirror %d",
			root->root_key.objectid, btrfs_ino(inode),
			logical_start,
			CSUM_FMT_VALUE(csum_size, csum),
			CSUM_FMT_VALUE(csum_size, csum_expected),
			mirror_num);
	else
		btrfs_warn_rl(root->fs_info,
"csum failed root %llu ino %llu off %llu csum " CSUM_FMT " expected csum " CSUM_FMT " mirror %d",
			root->root_key.objectid, btrfs_ino(inode),
			logical_start,
			CSUM_FMT_VALUE(csum_size, csum),
			CSUM_FMT_VALUE(csum_size, csum_expected),
			mirror_num);
>>>>>>> upstream/android-13
}

#endif
