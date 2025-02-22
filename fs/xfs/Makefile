# SPDX-License-Identifier: GPL-2.0
#
# Copyright (c) 2000-2005 Silicon Graphics, Inc.
# All Rights Reserved.
#

<<<<<<< HEAD
ccflags-y += -I$(src)			# needed for trace events
ccflags-y += -I$(src)/libxfs

ccflags-$(CONFIG_XFS_DEBUG) += -g
=======
ccflags-y += -I $(srctree)/$(src)		# needed for trace events
ccflags-y += -I $(srctree)/$(src)/libxfs
>>>>>>> upstream/android-13

obj-$(CONFIG_XFS_FS)		+= xfs.o

# this one should be compiled first, as the tracing macros can easily blow up
xfs-y				+= xfs_trace.o

# build the libxfs code first
xfs-y				+= $(addprefix libxfs/, \
				   xfs_ag.o \
				   xfs_alloc.o \
				   xfs_alloc_btree.o \
				   xfs_attr.o \
				   xfs_attr_leaf.o \
				   xfs_attr_remote.o \
				   xfs_bit.o \
				   xfs_bmap.o \
				   xfs_bmap_btree.o \
				   xfs_btree.o \
<<<<<<< HEAD
				   xfs_da_btree.o \
				   xfs_da_format.o \
=======
				   xfs_btree_staging.o \
				   xfs_da_btree.o \
>>>>>>> upstream/android-13
				   xfs_defer.o \
				   xfs_dir2.o \
				   xfs_dir2_block.o \
				   xfs_dir2_data.o \
				   xfs_dir2_leaf.o \
				   xfs_dir2_node.o \
				   xfs_dir2_sf.o \
				   xfs_dquot_buf.o \
				   xfs_ialloc.o \
				   xfs_ialloc_btree.o \
				   xfs_iext_tree.o \
				   xfs_inode_fork.o \
				   xfs_inode_buf.o \
				   xfs_log_rlimit.o \
				   xfs_ag_resv.o \
				   xfs_rmap.o \
				   xfs_rmap_btree.o \
				   xfs_refcount.o \
				   xfs_refcount_btree.o \
				   xfs_sb.o \
				   xfs_symlink_remote.o \
<<<<<<< HEAD
=======
				   xfs_trans_inode.o \
>>>>>>> upstream/android-13
				   xfs_trans_resv.o \
				   xfs_types.o \
				   )
# xfs_rtbitmap is shared with libxfs
xfs-$(CONFIG_XFS_RT)		+= $(addprefix libxfs/, \
				   xfs_rtbitmap.o \
				   )

# highlevel code
xfs-y				+= xfs_aops.o \
				   xfs_attr_inactive.o \
				   xfs_attr_list.o \
				   xfs_bmap_util.o \
<<<<<<< HEAD
=======
				   xfs_bio_io.o \
>>>>>>> upstream/android-13
				   xfs_buf.o \
				   xfs_dir2_readdir.o \
				   xfs_discard.o \
				   xfs_error.o \
				   xfs_export.o \
				   xfs_extent_busy.o \
				   xfs_file.o \
				   xfs_filestream.o \
				   xfs_fsmap.o \
				   xfs_fsops.o \
				   xfs_globals.o \
<<<<<<< HEAD
=======
				   xfs_health.o \
>>>>>>> upstream/android-13
				   xfs_icache.o \
				   xfs_ioctl.o \
				   xfs_iomap.o \
				   xfs_iops.o \
				   xfs_inode.o \
				   xfs_itable.o \
<<<<<<< HEAD
				   xfs_message.o \
				   xfs_mount.o \
				   xfs_mru_cache.o \
=======
				   xfs_iwalk.o \
				   xfs_message.o \
				   xfs_mount.o \
				   xfs_mru_cache.o \
				   xfs_pwork.o \
>>>>>>> upstream/android-13
				   xfs_reflink.o \
				   xfs_stats.o \
				   xfs_super.o \
				   xfs_symlink.o \
				   xfs_sysfs.o \
				   xfs_trans.o \
				   xfs_xattr.o \
				   kmem.o

# low-level transaction/log code
xfs-y				+= xfs_log.o \
				   xfs_log_cil.o \
				   xfs_bmap_item.o \
				   xfs_buf_item.o \
<<<<<<< HEAD
				   xfs_extfree_item.o \
				   xfs_icreate_item.o \
				   xfs_inode_item.o \
=======
				   xfs_buf_item_recover.o \
				   xfs_dquot_item_recover.o \
				   xfs_extfree_item.o \
				   xfs_icreate_item.o \
				   xfs_inode_item.o \
				   xfs_inode_item_recover.o \
>>>>>>> upstream/android-13
				   xfs_refcount_item.o \
				   xfs_rmap_item.o \
				   xfs_log_recover.o \
				   xfs_trans_ail.o \
<<<<<<< HEAD
				   xfs_trans_bmap.o \
				   xfs_trans_buf.o \
				   xfs_trans_extfree.o \
				   xfs_trans_inode.o \
				   xfs_trans_refcount.o \
				   xfs_trans_rmap.o \
=======
				   xfs_trans_buf.o
>>>>>>> upstream/android-13

# optional features
xfs-$(CONFIG_XFS_QUOTA)		+= xfs_dquot.o \
				   xfs_dquot_item.o \
				   xfs_trans_dquot.o \
				   xfs_qm_syscalls.o \
				   xfs_qm_bhv.o \
				   xfs_qm.o \
				   xfs_quotaops.o

# xfs_rtbitmap is shared with libxfs
xfs-$(CONFIG_XFS_RT)		+= xfs_rtalloc.o

xfs-$(CONFIG_XFS_POSIX_ACL)	+= xfs_acl.o
xfs-$(CONFIG_SYSCTL)		+= xfs_sysctl.o
xfs-$(CONFIG_COMPAT)		+= xfs_ioctl32.o
xfs-$(CONFIG_EXPORTFS_BLOCK_OPS)	+= xfs_pnfs.o

# online scrub/repair
ifeq ($(CONFIG_XFS_ONLINE_SCRUB),y)

# Tracepoints like to blow up, so build that before everything else

xfs-y				+= $(addprefix scrub/, \
				   trace.o \
				   agheader.o \
				   alloc.o \
				   attr.o \
				   bmap.o \
				   btree.o \
				   common.o \
				   dabtree.o \
				   dir.o \
<<<<<<< HEAD
=======
				   fscounters.o \
				   health.o \
>>>>>>> upstream/android-13
				   ialloc.o \
				   inode.o \
				   parent.o \
				   refcount.o \
				   rmap.o \
				   scrub.o \
				   symlink.o \
				   )

xfs-$(CONFIG_XFS_RT)		+= scrub/rtbitmap.o
xfs-$(CONFIG_XFS_QUOTA)		+= scrub/quota.o

# online repair
ifeq ($(CONFIG_XFS_ONLINE_REPAIR),y)
xfs-y				+= $(addprefix scrub/, \
				   agheader_repair.o \
				   bitmap.o \
				   repair.o \
				   )
endif
endif
