/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2014 Facebook.  All rights reserved.
 */

#ifndef BTRFS_REF_VERIFY_H
#define BTRFS_REF_VERIFY_H

#ifdef CONFIG_BTRFS_FS_REF_VERIFY
int btrfs_build_ref_tree(struct btrfs_fs_info *fs_info);
void btrfs_free_ref_cache(struct btrfs_fs_info *fs_info);
<<<<<<< HEAD
int btrfs_ref_tree_mod(struct btrfs_root *root, u64 bytenr, u64 num_bytes,
		       u64 parent, u64 ref_root, u64 owner, u64 offset,
		       int action);
=======
int btrfs_ref_tree_mod(struct btrfs_fs_info *fs_info,
		       struct btrfs_ref *generic_ref);
>>>>>>> upstream/android-13
void btrfs_free_ref_tree_range(struct btrfs_fs_info *fs_info, u64 start,
			       u64 len);

static inline void btrfs_init_ref_verify(struct btrfs_fs_info *fs_info)
{
	spin_lock_init(&fs_info->ref_verify_lock);
	fs_info->block_tree = RB_ROOT;
}
#else
static inline int btrfs_build_ref_tree(struct btrfs_fs_info *fs_info)
{
	return 0;
}

static inline void btrfs_free_ref_cache(struct btrfs_fs_info *fs_info)
{
}

<<<<<<< HEAD
static inline int btrfs_ref_tree_mod(struct btrfs_root *root, u64 bytenr,
				     u64 num_bytes, u64 parent, u64 ref_root,
				     u64 owner, u64 offset, int action)
=======
static inline int btrfs_ref_tree_mod(struct btrfs_fs_info *fs_info,
		       struct btrfs_ref *generic_ref)
>>>>>>> upstream/android-13
{
	return 0;
}

static inline void btrfs_free_ref_tree_range(struct btrfs_fs_info *fs_info,
					     u64 start, u64 len)
{
}

static inline void btrfs_init_ref_verify(struct btrfs_fs_info *fs_info)
{
}

#endif /* CONFIG_BTRFS_FS_REF_VERIFY */

#endif
