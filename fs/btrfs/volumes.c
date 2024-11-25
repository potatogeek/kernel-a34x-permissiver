// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 */

#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/bio.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/buffer_head.h>
=======
>>>>>>> upstream/android-13
#include <linux/blkdev.h>
#include <linux/ratelimit.h>
#include <linux/kthread.h>
#include <linux/raid/pq.h>
#include <linux/semaphore.h>
#include <linux/uuid.h>
#include <linux/list_sort.h>
<<<<<<< HEAD
=======
#include <linux/namei.h>
#include "misc.h"
>>>>>>> upstream/android-13
#include "ctree.h"
#include "extent_map.h"
#include "disk-io.h"
#include "transaction.h"
#include "print-tree.h"
#include "volumes.h"
#include "raid56.h"
#include "async-thread.h"
#include "check-integrity.h"
#include "rcu-string.h"
<<<<<<< HEAD
#include "math.h"
#include "dev-replace.h"
#include "sysfs.h"
#include "tree-checker.h"
=======
#include "dev-replace.h"
#include "sysfs.h"
#include "tree-checker.h"
#include "space-info.h"
#include "block-group.h"
#include "discard.h"
#include "zoned.h"
>>>>>>> upstream/android-13

const struct btrfs_raid_attr btrfs_raid_array[BTRFS_NR_RAID_TYPES] = {
	[BTRFS_RAID_RAID10] = {
		.sub_stripes	= 2,
		.dev_stripes	= 1,
		.devs_max	= 0,	/* 0 == as many as possible */
<<<<<<< HEAD
		.devs_min	= 4,
		.tolerated_failures = 1,
		.devs_increment	= 2,
		.ncopies	= 2,
=======
		.devs_min	= 2,
		.tolerated_failures = 1,
		.devs_increment	= 2,
		.ncopies	= 2,
		.nparity        = 0,
>>>>>>> upstream/android-13
		.raid_name	= "raid10",
		.bg_flag	= BTRFS_BLOCK_GROUP_RAID10,
		.mindev_error	= BTRFS_ERROR_DEV_RAID10_MIN_NOT_MET,
	},
	[BTRFS_RAID_RAID1] = {
		.sub_stripes	= 1,
		.dev_stripes	= 1,
		.devs_max	= 2,
		.devs_min	= 2,
		.tolerated_failures = 1,
		.devs_increment	= 2,
		.ncopies	= 2,
<<<<<<< HEAD
=======
		.nparity        = 0,
>>>>>>> upstream/android-13
		.raid_name	= "raid1",
		.bg_flag	= BTRFS_BLOCK_GROUP_RAID1,
		.mindev_error	= BTRFS_ERROR_DEV_RAID1_MIN_NOT_MET,
	},
<<<<<<< HEAD
=======
	[BTRFS_RAID_RAID1C3] = {
		.sub_stripes	= 1,
		.dev_stripes	= 1,
		.devs_max	= 3,
		.devs_min	= 3,
		.tolerated_failures = 2,
		.devs_increment	= 3,
		.ncopies	= 3,
		.nparity        = 0,
		.raid_name	= "raid1c3",
		.bg_flag	= BTRFS_BLOCK_GROUP_RAID1C3,
		.mindev_error	= BTRFS_ERROR_DEV_RAID1C3_MIN_NOT_MET,
	},
	[BTRFS_RAID_RAID1C4] = {
		.sub_stripes	= 1,
		.dev_stripes	= 1,
		.devs_max	= 4,
		.devs_min	= 4,
		.tolerated_failures = 3,
		.devs_increment	= 4,
		.ncopies	= 4,
		.nparity        = 0,
		.raid_name	= "raid1c4",
		.bg_flag	= BTRFS_BLOCK_GROUP_RAID1C4,
		.mindev_error	= BTRFS_ERROR_DEV_RAID1C4_MIN_NOT_MET,
	},
>>>>>>> upstream/android-13
	[BTRFS_RAID_DUP] = {
		.sub_stripes	= 1,
		.dev_stripes	= 2,
		.devs_max	= 1,
		.devs_min	= 1,
		.tolerated_failures = 0,
		.devs_increment	= 1,
		.ncopies	= 2,
<<<<<<< HEAD
=======
		.nparity        = 0,
>>>>>>> upstream/android-13
		.raid_name	= "dup",
		.bg_flag	= BTRFS_BLOCK_GROUP_DUP,
		.mindev_error	= 0,
	},
	[BTRFS_RAID_RAID0] = {
		.sub_stripes	= 1,
		.dev_stripes	= 1,
		.devs_max	= 0,
<<<<<<< HEAD
		.devs_min	= 2,
		.tolerated_failures = 0,
		.devs_increment	= 1,
		.ncopies	= 1,
=======
		.devs_min	= 1,
		.tolerated_failures = 0,
		.devs_increment	= 1,
		.ncopies	= 1,
		.nparity        = 0,
>>>>>>> upstream/android-13
		.raid_name	= "raid0",
		.bg_flag	= BTRFS_BLOCK_GROUP_RAID0,
		.mindev_error	= 0,
	},
	[BTRFS_RAID_SINGLE] = {
		.sub_stripes	= 1,
		.dev_stripes	= 1,
		.devs_max	= 1,
		.devs_min	= 1,
		.tolerated_failures = 0,
		.devs_increment	= 1,
		.ncopies	= 1,
<<<<<<< HEAD
=======
		.nparity        = 0,
>>>>>>> upstream/android-13
		.raid_name	= "single",
		.bg_flag	= 0,
		.mindev_error	= 0,
	},
	[BTRFS_RAID_RAID5] = {
		.sub_stripes	= 1,
		.dev_stripes	= 1,
		.devs_max	= 0,
		.devs_min	= 2,
		.tolerated_failures = 1,
		.devs_increment	= 1,
		.ncopies	= 1,
<<<<<<< HEAD
=======
		.nparity        = 1,
>>>>>>> upstream/android-13
		.raid_name	= "raid5",
		.bg_flag	= BTRFS_BLOCK_GROUP_RAID5,
		.mindev_error	= BTRFS_ERROR_DEV_RAID5_MIN_NOT_MET,
	},
	[BTRFS_RAID_RAID6] = {
		.sub_stripes	= 1,
		.dev_stripes	= 1,
		.devs_max	= 0,
		.devs_min	= 3,
		.tolerated_failures = 2,
		.devs_increment	= 1,
		.ncopies	= 1,
<<<<<<< HEAD
=======
		.nparity        = 2,
>>>>>>> upstream/android-13
		.raid_name	= "raid6",
		.bg_flag	= BTRFS_BLOCK_GROUP_RAID6,
		.mindev_error	= BTRFS_ERROR_DEV_RAID6_MIN_NOT_MET,
	},
};

<<<<<<< HEAD
const char *get_raid_name(enum btrfs_raid_types type)
{
	if (type >= BTRFS_NR_RAID_TYPES)
		return NULL;

	return btrfs_raid_array[type].raid_name;
}

static int init_first_rw_device(struct btrfs_trans_handle *trans,
				struct btrfs_fs_info *fs_info);
static int btrfs_relocate_sys_chunks(struct btrfs_fs_info *fs_info);
static void __btrfs_reset_dev_stats(struct btrfs_device *dev);
=======
/*
 * Convert block group flags (BTRFS_BLOCK_GROUP_*) to btrfs_raid_types, which
 * can be used as index to access btrfs_raid_array[].
 */
enum btrfs_raid_types __attribute_const__ btrfs_bg_flags_to_raid_index(u64 flags)
{
	if (flags & BTRFS_BLOCK_GROUP_RAID10)
		return BTRFS_RAID_RAID10;
	else if (flags & BTRFS_BLOCK_GROUP_RAID1)
		return BTRFS_RAID_RAID1;
	else if (flags & BTRFS_BLOCK_GROUP_RAID1C3)
		return BTRFS_RAID_RAID1C3;
	else if (flags & BTRFS_BLOCK_GROUP_RAID1C4)
		return BTRFS_RAID_RAID1C4;
	else if (flags & BTRFS_BLOCK_GROUP_DUP)
		return BTRFS_RAID_DUP;
	else if (flags & BTRFS_BLOCK_GROUP_RAID0)
		return BTRFS_RAID_RAID0;
	else if (flags & BTRFS_BLOCK_GROUP_RAID5)
		return BTRFS_RAID_RAID5;
	else if (flags & BTRFS_BLOCK_GROUP_RAID6)
		return BTRFS_RAID_RAID6;

	return BTRFS_RAID_SINGLE; /* BTRFS_BLOCK_GROUP_SINGLE */
}

const char *btrfs_bg_type_to_raid_name(u64 flags)
{
	const int index = btrfs_bg_flags_to_raid_index(flags);

	if (index >= BTRFS_NR_RAID_TYPES)
		return NULL;

	return btrfs_raid_array[index].raid_name;
}

/*
 * Fill @buf with textual description of @bg_flags, no more than @size_buf
 * bytes including terminating null byte.
 */
void btrfs_describe_block_groups(u64 bg_flags, char *buf, u32 size_buf)
{
	int i;
	int ret;
	char *bp = buf;
	u64 flags = bg_flags;
	u32 size_bp = size_buf;

	if (!flags) {
		strcpy(bp, "NONE");
		return;
	}

#define DESCRIBE_FLAG(flag, desc)						\
	do {								\
		if (flags & (flag)) {					\
			ret = snprintf(bp, size_bp, "%s|", (desc));	\
			if (ret < 0 || ret >= size_bp)			\
				goto out_overflow;			\
			size_bp -= ret;					\
			bp += ret;					\
			flags &= ~(flag);				\
		}							\
	} while (0)

	DESCRIBE_FLAG(BTRFS_BLOCK_GROUP_DATA, "data");
	DESCRIBE_FLAG(BTRFS_BLOCK_GROUP_SYSTEM, "system");
	DESCRIBE_FLAG(BTRFS_BLOCK_GROUP_METADATA, "metadata");

	DESCRIBE_FLAG(BTRFS_AVAIL_ALLOC_BIT_SINGLE, "single");
	for (i = 0; i < BTRFS_NR_RAID_TYPES; i++)
		DESCRIBE_FLAG(btrfs_raid_array[i].bg_flag,
			      btrfs_raid_array[i].raid_name);
#undef DESCRIBE_FLAG

	if (flags) {
		ret = snprintf(bp, size_bp, "0x%llx|", flags);
		size_bp -= ret;
	}

	if (size_bp < size_buf)
		buf[size_buf - size_bp - 1] = '\0'; /* remove last | */

	/*
	 * The text is trimmed, it's up to the caller to provide sufficiently
	 * large buffer
	 */
out_overflow:;
}

static int init_first_rw_device(struct btrfs_trans_handle *trans);
static int btrfs_relocate_sys_chunks(struct btrfs_fs_info *fs_info);
>>>>>>> upstream/android-13
static void btrfs_dev_stat_print_on_error(struct btrfs_device *dev);
static void btrfs_dev_stat_print_on_load(struct btrfs_device *device);
static int __btrfs_map_block(struct btrfs_fs_info *fs_info,
			     enum btrfs_map_op op,
			     u64 logical, u64 *length,
			     struct btrfs_bio **bbio_ret,
			     int mirror_num, int need_raid_map);

/*
 * Device locking
 * ==============
 *
 * There are several mutexes that protect manipulation of devices and low-level
 * structures like chunks but not block groups, extents or files
 *
 * uuid_mutex (global lock)
 * ------------------------
 * protects the fs_uuids list that tracks all per-fs fs_devices, resulting from
 * the SCAN_DEV ioctl registration or from mount either implicitly (the first
 * device) or requested by the device= mount option
 *
 * the mutex can be very coarse and can cover long-running operations
 *
 * protects: updates to fs_devices counters like missing devices, rw devices,
<<<<<<< HEAD
 * seeding, structure cloning, openning/closing devices at mount/umount time
=======
 * seeding, structure cloning, opening/closing devices at mount/umount time
>>>>>>> upstream/android-13
 *
 * global::fs_devs - add, remove, updates to the global list
 *
 * does not protect: manipulation of the fs_devices::devices list in general
 * but in mount context it could be used to exclude list modifications by eg.
 * scan ioctl
 *
 * btrfs_device::name - renames (write side), read is RCU
 *
 * fs_devices::device_list_mutex (per-fs, with RCU)
 * ------------------------------------------------
 * protects updates to fs_devices::devices, ie. adding and deleting
 *
 * simple list traversal with read-only actions can be done with RCU protection
 *
 * may be used to exclude some operations from running concurrently without any
 * modifications to the list (see write_all_supers)
 *
 * Is not required at mount and close times, because our device list is
 * protected by the uuid_mutex at that point.
 *
 * balance_mutex
 * -------------
 * protects balance structures (status, state) and context accessed from
 * several places (internally, ioctl)
 *
 * chunk_mutex
 * -----------
 * protects chunks, adding or removing during allocation, trim or when a new
<<<<<<< HEAD
 * device is added/removed
=======
 * device is added/removed. Additionally it also protects post_commit_list of
 * individual devices, since they can be added to the transaction's
 * post_commit_list only with chunk_mutex held.
>>>>>>> upstream/android-13
 *
 * cleaner_mutex
 * -------------
 * a big lock that is held by the cleaner thread and prevents running subvolume
 * cleaning together with relocation or delayed iputs
 *
 *
 * Lock nesting
 * ============
 *
 * uuid_mutex
<<<<<<< HEAD
 *   volume_mutex
 *     device_list_mutex
 *       chunk_mutex
 *     balance_mutex
 *
 *
 * Exclusive operations, BTRFS_FS_EXCL_OP
 * ======================================
=======
 *   device_list_mutex
 *     chunk_mutex
 *   balance_mutex
 *
 *
 * Exclusive operations
 * ====================
>>>>>>> upstream/android-13
 *
 * Maintains the exclusivity of the following operations that apply to the
 * whole filesystem and cannot run in parallel.
 *
 * - Balance (*)
 * - Device add
 * - Device remove
 * - Device replace (*)
 * - Resize
 *
 * The device operations (as above) can be in one of the following states:
 *
 * - Running state
 * - Paused state
 * - Completed state
 *
 * Only device operations marked with (*) can go into the Paused state for the
 * following reasons:
 *
 * - ioctl (only Balance can be Paused through ioctl)
 * - filesystem remounted as read-only
 * - filesystem unmounted and mounted as read-only
 * - system power-cycle and filesystem mounted as read-only
 * - filesystem or device errors leading to forced read-only
 *
<<<<<<< HEAD
 * BTRFS_FS_EXCL_OP flag is set and cleared using atomic operations.
 * During the course of Paused state, the BTRFS_FS_EXCL_OP remains set.
 * A device operation in Paused or Running state can be canceled or resumed
 * either by ioctl (Balance only) or when remounted as read-write.
 * BTRFS_FS_EXCL_OP flag is cleared when the device operation is canceled or
=======
 * The status of exclusive operation is set and cleared atomically.
 * During the course of Paused state, fs_info::exclusive_operation remains set.
 * A device operation in Paused or Running state can be canceled or resumed
 * either by ioctl (Balance only) or when remounted as read-write.
 * The exclusive status is cleared when the device operation is canceled or
>>>>>>> upstream/android-13
 * completed.
 */

DEFINE_MUTEX(uuid_mutex);
static LIST_HEAD(fs_uuids);
<<<<<<< HEAD
struct list_head *btrfs_get_fs_uuids(void)
=======
struct list_head * __attribute_const__ btrfs_get_fs_uuids(void)
>>>>>>> upstream/android-13
{
	return &fs_uuids;
}

/*
 * alloc_fs_devices - allocate struct btrfs_fs_devices
<<<<<<< HEAD
 * @fsid:	if not NULL, copy the uuid to fs_devices::fsid
=======
 * @fsid:		if not NULL, copy the UUID to fs_devices::fsid
 * @metadata_fsid:	if not NULL, copy the UUID to fs_devices::metadata_fsid
>>>>>>> upstream/android-13
 *
 * Return a pointer to a new struct btrfs_fs_devices on success, or ERR_PTR().
 * The returned struct is not linked onto any lists and can be destroyed with
 * kfree() right away.
 */
<<<<<<< HEAD
static struct btrfs_fs_devices *alloc_fs_devices(const u8 *fsid)
=======
static struct btrfs_fs_devices *alloc_fs_devices(const u8 *fsid,
						 const u8 *metadata_fsid)
>>>>>>> upstream/android-13
{
	struct btrfs_fs_devices *fs_devs;

	fs_devs = kzalloc(sizeof(*fs_devs), GFP_KERNEL);
	if (!fs_devs)
		return ERR_PTR(-ENOMEM);

	mutex_init(&fs_devs->device_list_mutex);

	INIT_LIST_HEAD(&fs_devs->devices);
<<<<<<< HEAD
	INIT_LIST_HEAD(&fs_devs->resized_devices);
	INIT_LIST_HEAD(&fs_devs->alloc_list);
	INIT_LIST_HEAD(&fs_devs->fs_list);
	if (fsid)
		memcpy(fs_devs->fsid, fsid, BTRFS_FSID_SIZE);

=======
	INIT_LIST_HEAD(&fs_devs->alloc_list);
	INIT_LIST_HEAD(&fs_devs->fs_list);
	INIT_LIST_HEAD(&fs_devs->seed_list);
	if (fsid)
		memcpy(fs_devs->fsid, fsid, BTRFS_FSID_SIZE);

	if (metadata_fsid)
		memcpy(fs_devs->metadata_uuid, metadata_fsid, BTRFS_FSID_SIZE);
	else if (fsid)
		memcpy(fs_devs->metadata_uuid, fsid, BTRFS_FSID_SIZE);

>>>>>>> upstream/android-13
	return fs_devs;
}

void btrfs_free_device(struct btrfs_device *device)
{
<<<<<<< HEAD
	rcu_string_free(device->name);
	bio_put(device->flush_bio);
=======
	WARN_ON(!list_empty(&device->post_commit_list));
	rcu_string_free(device->name);
	extent_io_tree_release(&device->alloc_state);
	bio_put(device->flush_bio);
	btrfs_destroy_dev_zone_info(device);
>>>>>>> upstream/android-13
	kfree(device);
}

static void free_fs_devices(struct btrfs_fs_devices *fs_devices)
{
	struct btrfs_device *device;
	WARN_ON(fs_devices->opened);
	while (!list_empty(&fs_devices->devices)) {
		device = list_entry(fs_devices->devices.next,
				    struct btrfs_device, dev_list);
		list_del(&device->dev_list);
		btrfs_free_device(device);
	}
	kfree(fs_devices);
}

<<<<<<< HEAD
static void btrfs_kobject_uevent(struct block_device *bdev,
				 enum kobject_action action)
{
	int ret;

	ret = kobject_uevent(&disk_to_dev(bdev->bd_disk)->kobj, action);
	if (ret)
		pr_warn("BTRFS: Sending event '%d' to kobject: '%s' (%p): failed\n",
			action,
			kobject_name(&disk_to_dev(bdev->bd_disk)->kobj),
			&disk_to_dev(bdev->bd_disk)->kobj);
}

=======
>>>>>>> upstream/android-13
void __exit btrfs_cleanup_fs_uuids(void)
{
	struct btrfs_fs_devices *fs_devices;

	while (!list_empty(&fs_uuids)) {
		fs_devices = list_entry(fs_uuids.next,
					struct btrfs_fs_devices, fs_list);
		list_del(&fs_devices->fs_list);
		free_fs_devices(fs_devices);
	}
}

<<<<<<< HEAD
/*
 * Returns a pointer to a new btrfs_device on success; ERR_PTR() on error.
 * Returned struct is not linked onto any lists and must be destroyed using
 * btrfs_free_device.
 */
static struct btrfs_device *__alloc_device(void)
{
	struct btrfs_device *dev;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return ERR_PTR(-ENOMEM);

	/*
	 * Preallocate a bio that's always going to be used for flushing device
	 * barriers and matches the device lifespan
	 */
	dev->flush_bio = bio_alloc_bioset(GFP_KERNEL, 0, NULL);
	if (!dev->flush_bio) {
		kfree(dev);
		return ERR_PTR(-ENOMEM);
	}

	INIT_LIST_HEAD(&dev->dev_list);
	INIT_LIST_HEAD(&dev->dev_alloc_list);
	INIT_LIST_HEAD(&dev->resized_list);

	spin_lock_init(&dev->io_lock);

	atomic_set(&dev->reada_in_flight, 0);
	atomic_set(&dev->dev_stats_ccnt, 0);
	btrfs_device_data_ordered_init(dev);
	INIT_RADIX_TREE(&dev->reada_zones, GFP_NOFS & ~__GFP_DIRECT_RECLAIM);
	INIT_RADIX_TREE(&dev->reada_extents, GFP_NOFS & ~__GFP_DIRECT_RECLAIM);

	return dev;
}

static noinline struct btrfs_fs_devices *find_fsid(u8 *fsid)
{
	struct btrfs_fs_devices *fs_devices;

	list_for_each_entry(fs_devices, &fs_uuids, fs_list) {
		if (memcmp(fsid, fs_devices->fsid, BTRFS_FSID_SIZE) == 0)
			return fs_devices;
=======
static noinline struct btrfs_fs_devices *find_fsid(
		const u8 *fsid, const u8 *metadata_fsid)
{
	struct btrfs_fs_devices *fs_devices;

	ASSERT(fsid);

	/* Handle non-split brain cases */
	list_for_each_entry(fs_devices, &fs_uuids, fs_list) {
		if (metadata_fsid) {
			if (memcmp(fsid, fs_devices->fsid, BTRFS_FSID_SIZE) == 0
			    && memcmp(metadata_fsid, fs_devices->metadata_uuid,
				      BTRFS_FSID_SIZE) == 0)
				return fs_devices;
		} else {
			if (memcmp(fsid, fs_devices->fsid, BTRFS_FSID_SIZE) == 0)
				return fs_devices;
		}
>>>>>>> upstream/android-13
	}
	return NULL;
}

<<<<<<< HEAD
static int
btrfs_get_bdev_and_sb(const char *device_path, fmode_t flags, void *holder,
		      int flush, struct block_device **bdev,
		      struct buffer_head **bh)
=======
static struct btrfs_fs_devices *find_fsid_with_metadata_uuid(
				struct btrfs_super_block *disk_super)
{

	struct btrfs_fs_devices *fs_devices;

	/*
	 * Handle scanned device having completed its fsid change but
	 * belonging to a fs_devices that was created by first scanning
	 * a device which didn't have its fsid/metadata_uuid changed
	 * at all and the CHANGING_FSID_V2 flag set.
	 */
	list_for_each_entry(fs_devices, &fs_uuids, fs_list) {
		if (fs_devices->fsid_change &&
		    memcmp(disk_super->metadata_uuid, fs_devices->fsid,
			   BTRFS_FSID_SIZE) == 0 &&
		    memcmp(fs_devices->fsid, fs_devices->metadata_uuid,
			   BTRFS_FSID_SIZE) == 0) {
			return fs_devices;
		}
	}
	/*
	 * Handle scanned device having completed its fsid change but
	 * belonging to a fs_devices that was created by a device that
	 * has an outdated pair of fsid/metadata_uuid and
	 * CHANGING_FSID_V2 flag set.
	 */
	list_for_each_entry(fs_devices, &fs_uuids, fs_list) {
		if (fs_devices->fsid_change &&
		    memcmp(fs_devices->metadata_uuid,
			   fs_devices->fsid, BTRFS_FSID_SIZE) != 0 &&
		    memcmp(disk_super->metadata_uuid, fs_devices->metadata_uuid,
			   BTRFS_FSID_SIZE) == 0) {
			return fs_devices;
		}
	}

	return find_fsid(disk_super->fsid, disk_super->metadata_uuid);
}


static int
btrfs_get_bdev_and_sb(const char *device_path, fmode_t flags, void *holder,
		      int flush, struct block_device **bdev,
		      struct btrfs_super_block **disk_super)
>>>>>>> upstream/android-13
{
	int ret;

	*bdev = blkdev_get_by_path(device_path, flags, holder);

	if (IS_ERR(*bdev)) {
		ret = PTR_ERR(*bdev);
		goto error;
	}

	if (flush)
		filemap_write_and_wait((*bdev)->bd_inode->i_mapping);
	ret = set_blocksize(*bdev, BTRFS_BDEV_BLOCKSIZE);
	if (ret) {
		blkdev_put(*bdev, flags);
		goto error;
	}
	invalidate_bdev(*bdev);
<<<<<<< HEAD
	*bh = btrfs_read_dev_super(*bdev);
	if (IS_ERR(*bh)) {
		ret = PTR_ERR(*bh);
=======
	*disk_super = btrfs_read_dev_super(*bdev);
	if (IS_ERR(*disk_super)) {
		ret = PTR_ERR(*disk_super);
>>>>>>> upstream/android-13
		blkdev_put(*bdev, flags);
		goto error;
	}

	return 0;

error:
	*bdev = NULL;
<<<<<<< HEAD
	*bh = NULL;
	return ret;
}

static void requeue_list(struct btrfs_pending_bios *pending_bios,
			struct bio *head, struct bio *tail)
{

	struct bio *old_head;

	old_head = pending_bios->head;
	pending_bios->head = head;
	if (pending_bios->tail)
		tail->bi_next = old_head;
	else
		pending_bios->tail = tail;
}

/*
 * we try to collect pending bios for a device so we don't get a large
 * number of procs sending bios down to the same device.  This greatly
 * improves the schedulers ability to collect and merge the bios.
 *
 * But, it also turns into a long list of bios to process and that is sure
 * to eventually make the worker thread block.  The solution here is to
 * make some progress and then put this work struct back at the end of
 * the list if the block device is congested.  This way, multiple devices
 * can make progress from a single worker thread.
 */
static noinline void run_scheduled_bios(struct btrfs_device *device)
{
	struct btrfs_fs_info *fs_info = device->fs_info;
	struct bio *pending;
	struct backing_dev_info *bdi;
	struct btrfs_pending_bios *pending_bios;
	struct bio *tail;
	struct bio *cur;
	int again = 0;
	unsigned long num_run;
	unsigned long batch_run = 0;
	unsigned long last_waited = 0;
	int force_reg = 0;
	int sync_pending = 0;
	struct blk_plug plug;

	/*
	 * this function runs all the bios we've collected for
	 * a particular device.  We don't want to wander off to
	 * another device without first sending all of these down.
	 * So, setup a plug here and finish it off before we return
	 */
	blk_start_plug(&plug);

	bdi = device->bdev->bd_bdi;

loop:
	spin_lock(&device->io_lock);

loop_lock:
	num_run = 0;

	/* take all the bios off the list at once and process them
	 * later on (without the lock held).  But, remember the
	 * tail and other pointers so the bios can be properly reinserted
	 * into the list if we hit congestion
	 */
	if (!force_reg && device->pending_sync_bios.head) {
		pending_bios = &device->pending_sync_bios;
		force_reg = 1;
	} else {
		pending_bios = &device->pending_bios;
		force_reg = 0;
	}

	pending = pending_bios->head;
	tail = pending_bios->tail;
	WARN_ON(pending && !tail);

	/*
	 * if pending was null this time around, no bios need processing
	 * at all and we can stop.  Otherwise it'll loop back up again
	 * and do an additional check so no bios are missed.
	 *
	 * device->running_pending is used to synchronize with the
	 * schedule_bio code.
	 */
	if (device->pending_sync_bios.head == NULL &&
	    device->pending_bios.head == NULL) {
		again = 0;
		device->running_pending = 0;
	} else {
		again = 1;
		device->running_pending = 1;
	}

	pending_bios->head = NULL;
	pending_bios->tail = NULL;

	spin_unlock(&device->io_lock);

	while (pending) {

		rmb();
		/* we want to work on both lists, but do more bios on the
		 * sync list than the regular list
		 */
		if ((num_run > 32 &&
		    pending_bios != &device->pending_sync_bios &&
		    device->pending_sync_bios.head) ||
		   (num_run > 64 && pending_bios == &device->pending_sync_bios &&
		    device->pending_bios.head)) {
			spin_lock(&device->io_lock);
			requeue_list(pending_bios, pending, tail);
			goto loop_lock;
		}

		cur = pending;
		pending = pending->bi_next;
		cur->bi_next = NULL;

		BUG_ON(atomic_read(&cur->__bi_cnt) == 0);

		/*
		 * if we're doing the sync list, record that our
		 * plug has some sync requests on it
		 *
		 * If we're doing the regular list and there are
		 * sync requests sitting around, unplug before
		 * we add more
		 */
		if (pending_bios == &device->pending_sync_bios) {
			sync_pending = 1;
		} else if (sync_pending) {
			blk_finish_plug(&plug);
			blk_start_plug(&plug);
			sync_pending = 0;
		}

		btrfsic_submit_bio(cur);
		num_run++;
		batch_run++;

		cond_resched();

		/*
		 * we made progress, there is more work to do and the bdi
		 * is now congested.  Back off and let other work structs
		 * run instead
		 */
		if (pending && bdi_write_congested(bdi) && batch_run > 8 &&
		    fs_info->fs_devices->open_devices > 1) {
			struct io_context *ioc;

			ioc = current->io_context;

			/*
			 * the main goal here is that we don't want to
			 * block if we're going to be able to submit
			 * more requests without blocking.
			 *
			 * This code does two great things, it pokes into
			 * the elevator code from a filesystem _and_
			 * it makes assumptions about how batching works.
			 */
			if (ioc && ioc->nr_batch_requests > 0 &&
			    time_before(jiffies, ioc->last_waited + HZ/50UL) &&
			    (last_waited == 0 ||
			     ioc->last_waited == last_waited)) {
				/*
				 * we want to go through our batch of
				 * requests and stop.  So, we copy out
				 * the ioc->last_waited time and test
				 * against it before looping
				 */
				last_waited = ioc->last_waited;
				cond_resched();
				continue;
			}
			spin_lock(&device->io_lock);
			requeue_list(pending_bios, pending, tail);
			device->running_pending = 1;

			spin_unlock(&device->io_lock);
			btrfs_queue_work(fs_info->submit_workers,
					 &device->work);
			goto done;
		}
	}

	cond_resched();
	if (again)
		goto loop;

	spin_lock(&device->io_lock);
	if (device->pending_bios.head || device->pending_sync_bios.head)
		goto loop_lock;
	spin_unlock(&device->io_lock);

done:
	blk_finish_plug(&plug);
}

static void pending_bios_fn(struct btrfs_work *work)
{
	struct btrfs_device *device;

	device = container_of(work, struct btrfs_device, work);
	run_scheduled_bios(device);
=======
	return ret;
}

/*
 * Check if the device in the path matches the device in the given struct device.
 *
 * Returns:
 *   true  If it is the same device.
 *   false If it is not the same device or on error.
 */
static bool device_matched(const struct btrfs_device *device, const char *path)
{
	char *device_name;
	dev_t dev_old;
	dev_t dev_new;
	int ret;

	/*
	 * If we are looking for a device with the matching dev_t, then skip
	 * device without a name (a missing device).
	 */
	if (!device->name)
		return false;

	device_name = kzalloc(BTRFS_PATH_NAME_MAX, GFP_KERNEL);
	if (!device_name)
		return false;

	rcu_read_lock();
	scnprintf(device_name, BTRFS_PATH_NAME_MAX, "%s", rcu_str_deref(device->name));
	rcu_read_unlock();

	ret = lookup_bdev(device_name, &dev_old);
	kfree(device_name);
	if (ret)
		return false;

	ret = lookup_bdev(path, &dev_new);
	if (ret)
		return false;

	if (dev_old == dev_new)
		return true;

	return false;
>>>>>>> upstream/android-13
}

/*
 *  Search and remove all stale (devices which are not mounted) devices.
 *  When both inputs are NULL, it will search and release all stale devices.
 *  path:	Optional. When provided will it release all unmounted devices
 *		matching this path only.
 *  skip_dev:	Optional. Will skip this device when searching for the stale
 *		devices.
<<<<<<< HEAD
 */
static void btrfs_free_stale_devices(const char *path,
=======
 *  Return:	0 for success or if @path is NULL.
 * 		-EBUSY if @path is a mounted device.
 * 		-ENOENT if @path does not match any device in the list.
 */
static int btrfs_free_stale_devices(const char *path,
>>>>>>> upstream/android-13
				     struct btrfs_device *skip_device)
{
	struct btrfs_fs_devices *fs_devices, *tmp_fs_devices;
	struct btrfs_device *device, *tmp_device;
<<<<<<< HEAD

	list_for_each_entry_safe(fs_devices, tmp_fs_devices, &fs_uuids, fs_list) {
		mutex_lock(&fs_devices->device_list_mutex);
		if (fs_devices->opened) {
			mutex_unlock(&fs_devices->device_list_mutex);
			continue;
		}

		list_for_each_entry_safe(device, tmp_device,
					 &fs_devices->devices, dev_list) {
			int not_found = 0;

			if (skip_device && skip_device == device)
				continue;
			if (path && !device->name)
				continue;

			rcu_read_lock();
			if (path)
				not_found = strcmp(rcu_str_deref(device->name),
						   path);
			rcu_read_unlock();
			if (not_found)
				continue;
=======
	int ret = 0;

	lockdep_assert_held(&uuid_mutex);

	if (path)
		ret = -ENOENT;

	list_for_each_entry_safe(fs_devices, tmp_fs_devices, &fs_uuids, fs_list) {

		mutex_lock(&fs_devices->device_list_mutex);
		list_for_each_entry_safe(device, tmp_device,
					 &fs_devices->devices, dev_list) {
			if (skip_device && skip_device == device)
				continue;
			if (path && !device_matched(device, path))
				continue;
			if (fs_devices->opened) {
				/* for an already deleted device return 0 */
				if (path && ret != 0)
					ret = -EBUSY;
				break;
			}
>>>>>>> upstream/android-13

			/* delete the stale device */
			fs_devices->num_devices--;
			list_del(&device->dev_list);
			btrfs_free_device(device);

<<<<<<< HEAD
			if (fs_devices->num_devices == 0)
				break;
		}
		mutex_unlock(&fs_devices->device_list_mutex);
=======
			ret = 0;
		}
		mutex_unlock(&fs_devices->device_list_mutex);

>>>>>>> upstream/android-13
		if (fs_devices->num_devices == 0) {
			btrfs_sysfs_remove_fsid(fs_devices);
			list_del(&fs_devices->fs_list);
			free_fs_devices(fs_devices);
		}
	}
<<<<<<< HEAD
=======

	return ret;
>>>>>>> upstream/android-13
}

/*
 * This is only used on mount, and we are protected from competing things
 * messing with our fs_devices by the uuid_mutex, thus we do not need the
 * fs_devices->device_list_mutex here.
 */
static int btrfs_open_one_device(struct btrfs_fs_devices *fs_devices,
			struct btrfs_device *device, fmode_t flags,
			void *holder)
{
	struct request_queue *q;
	struct block_device *bdev;
<<<<<<< HEAD
	struct buffer_head *bh;
=======
>>>>>>> upstream/android-13
	struct btrfs_super_block *disk_super;
	u64 devid;
	int ret;

	if (device->bdev)
		return -EINVAL;
	if (!device->name)
		return -EINVAL;

	ret = btrfs_get_bdev_and_sb(device->name->str, flags, holder, 1,
<<<<<<< HEAD
				    &bdev, &bh);
	if (ret)
		return ret;

	disk_super = (struct btrfs_super_block *)bh->b_data;
	devid = btrfs_stack_device_id(&disk_super->dev_item);
	if (devid != device->devid)
		goto error_brelse;

	if (memcmp(device->uuid, disk_super->dev_item.uuid, BTRFS_UUID_SIZE))
		goto error_brelse;
=======
				    &bdev, &disk_super);
	if (ret)
		return ret;

	devid = btrfs_stack_device_id(&disk_super->dev_item);
	if (devid != device->devid)
		goto error_free_page;

	if (memcmp(device->uuid, disk_super->dev_item.uuid, BTRFS_UUID_SIZE))
		goto error_free_page;
>>>>>>> upstream/android-13

	device->generation = btrfs_super_generation(disk_super);

	if (btrfs_super_flags(disk_super) & BTRFS_SUPER_FLAG_SEEDING) {
<<<<<<< HEAD
		clear_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state);
		fs_devices->seeding = 1;
=======
		if (btrfs_super_incompat_flags(disk_super) &
		    BTRFS_FEATURE_INCOMPAT_METADATA_UUID) {
			pr_err(
		"BTRFS: Invalid seeding and uuid-changed device detected\n");
			goto error_free_page;
		}

		clear_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state);
		fs_devices->seeding = true;
>>>>>>> upstream/android-13
	} else {
		if (bdev_read_only(bdev))
			clear_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state);
		else
			set_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state);
	}

	q = bdev_get_queue(bdev);
	if (!blk_queue_nonrot(q))
<<<<<<< HEAD
		fs_devices->rotating = 1;
=======
		fs_devices->rotating = true;
>>>>>>> upstream/android-13

	device->bdev = bdev;
	clear_bit(BTRFS_DEV_STATE_IN_FS_METADATA, &device->dev_state);
	device->mode = flags;

	fs_devices->open_devices++;
	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state) &&
	    device->devid != BTRFS_DEV_REPLACE_DEVID) {
		fs_devices->rw_devices++;
		list_add_tail(&device->dev_alloc_list, &fs_devices->alloc_list);
	}
<<<<<<< HEAD
	brelse(bh);

	return 0;

error_brelse:
	brelse(bh);
=======
	btrfs_release_disk_super(disk_super);

	return 0;

error_free_page:
	btrfs_release_disk_super(disk_super);
>>>>>>> upstream/android-13
	blkdev_put(bdev, flags);

	return -EINVAL;
}

/*
<<<<<<< HEAD
=======
 * Handle scanned device having its CHANGING_FSID_V2 flag set and the fs_devices
 * being created with a disk that has already completed its fsid change. Such
 * disk can belong to an fs which has its FSID changed or to one which doesn't.
 * Handle both cases here.
 */
static struct btrfs_fs_devices *find_fsid_inprogress(
					struct btrfs_super_block *disk_super)
{
	struct btrfs_fs_devices *fs_devices;

	list_for_each_entry(fs_devices, &fs_uuids, fs_list) {
		if (memcmp(fs_devices->metadata_uuid, fs_devices->fsid,
			   BTRFS_FSID_SIZE) != 0 &&
		    memcmp(fs_devices->metadata_uuid, disk_super->fsid,
			   BTRFS_FSID_SIZE) == 0 && !fs_devices->fsid_change) {
			return fs_devices;
		}
	}

	return find_fsid(disk_super->fsid, NULL);
}


static struct btrfs_fs_devices *find_fsid_changed(
					struct btrfs_super_block *disk_super)
{
	struct btrfs_fs_devices *fs_devices;

	/*
	 * Handles the case where scanned device is part of an fs that had
	 * multiple successful changes of FSID but currently device didn't
	 * observe it. Meaning our fsid will be different than theirs. We need
	 * to handle two subcases :
	 *  1 - The fs still continues to have different METADATA/FSID uuids.
	 *  2 - The fs is switched back to its original FSID (METADATA/FSID
	 *  are equal).
	 */
	list_for_each_entry(fs_devices, &fs_uuids, fs_list) {
		/* Changed UUIDs */
		if (memcmp(fs_devices->metadata_uuid, fs_devices->fsid,
			   BTRFS_FSID_SIZE) != 0 &&
		    memcmp(fs_devices->metadata_uuid, disk_super->metadata_uuid,
			   BTRFS_FSID_SIZE) == 0 &&
		    memcmp(fs_devices->fsid, disk_super->fsid,
			   BTRFS_FSID_SIZE) != 0)
			return fs_devices;

		/* Unchanged UUIDs */
		if (memcmp(fs_devices->metadata_uuid, fs_devices->fsid,
			   BTRFS_FSID_SIZE) == 0 &&
		    memcmp(fs_devices->fsid, disk_super->metadata_uuid,
			   BTRFS_FSID_SIZE) == 0)
			return fs_devices;
	}

	return NULL;
}

static struct btrfs_fs_devices *find_fsid_reverted_metadata(
				struct btrfs_super_block *disk_super)
{
	struct btrfs_fs_devices *fs_devices;

	/*
	 * Handle the case where the scanned device is part of an fs whose last
	 * metadata UUID change reverted it to the original FSID. At the same
	 * time * fs_devices was first created by another constitutent device
	 * which didn't fully observe the operation. This results in an
	 * btrfs_fs_devices created with metadata/fsid different AND
	 * btrfs_fs_devices::fsid_change set AND the metadata_uuid of the
	 * fs_devices equal to the FSID of the disk.
	 */
	list_for_each_entry(fs_devices, &fs_uuids, fs_list) {
		if (memcmp(fs_devices->fsid, fs_devices->metadata_uuid,
			   BTRFS_FSID_SIZE) != 0 &&
		    memcmp(fs_devices->metadata_uuid, disk_super->fsid,
			   BTRFS_FSID_SIZE) == 0 &&
		    fs_devices->fsid_change)
			return fs_devices;
	}

	return NULL;
}
/*
>>>>>>> upstream/android-13
 * Add new device to list of registered devices
 *
 * Returns:
 * device pointer which was just added or updated when successful
 * error pointer when failed
 */
static noinline struct btrfs_device *device_list_add(const char *path,
			   struct btrfs_super_block *disk_super,
			   bool *new_device_added)
{
	struct btrfs_device *device;
<<<<<<< HEAD
	struct btrfs_fs_devices *fs_devices;
	struct rcu_string *name;
	u64 found_transid = btrfs_super_generation(disk_super);
	u64 devid = btrfs_stack_device_id(&disk_super->dev_item);

	fs_devices = find_fsid(disk_super->fsid);
	if (!fs_devices) {
		fs_devices = alloc_fs_devices(disk_super->fsid);
		if (IS_ERR(fs_devices))
			return ERR_CAST(fs_devices);

=======
	struct btrfs_fs_devices *fs_devices = NULL;
	struct rcu_string *name;
	u64 found_transid = btrfs_super_generation(disk_super);
	u64 devid = btrfs_stack_device_id(&disk_super->dev_item);
	bool has_metadata_uuid = (btrfs_super_incompat_flags(disk_super) &
		BTRFS_FEATURE_INCOMPAT_METADATA_UUID);
	bool fsid_change_in_progress = (btrfs_super_flags(disk_super) &
					BTRFS_SUPER_FLAG_CHANGING_FSID_V2);

	if (fsid_change_in_progress) {
		if (!has_metadata_uuid)
			fs_devices = find_fsid_inprogress(disk_super);
		else
			fs_devices = find_fsid_changed(disk_super);
	} else if (has_metadata_uuid) {
		fs_devices = find_fsid_with_metadata_uuid(disk_super);
	} else {
		fs_devices = find_fsid_reverted_metadata(disk_super);
		if (!fs_devices)
			fs_devices = find_fsid(disk_super->fsid, NULL);
	}


	if (!fs_devices) {
		if (has_metadata_uuid)
			fs_devices = alloc_fs_devices(disk_super->fsid,
						      disk_super->metadata_uuid);
		else
			fs_devices = alloc_fs_devices(disk_super->fsid, NULL);

		if (IS_ERR(fs_devices))
			return ERR_CAST(fs_devices);

		fs_devices->fsid_change = fsid_change_in_progress;

>>>>>>> upstream/android-13
		mutex_lock(&fs_devices->device_list_mutex);
		list_add(&fs_devices->fs_list, &fs_uuids);

		device = NULL;
	} else {
		mutex_lock(&fs_devices->device_list_mutex);
		device = btrfs_find_device(fs_devices, devid,
<<<<<<< HEAD
				disk_super->dev_item.uuid, NULL, false);
=======
				disk_super->dev_item.uuid, NULL);

		/*
		 * If this disk has been pulled into an fs devices created by
		 * a device which had the CHANGING_FSID_V2 flag then replace the
		 * metadata_uuid/fsid values of the fs_devices.
		 */
		if (fs_devices->fsid_change &&
		    found_transid > fs_devices->latest_generation) {
			memcpy(fs_devices->fsid, disk_super->fsid,
					BTRFS_FSID_SIZE);

			if (has_metadata_uuid)
				memcpy(fs_devices->metadata_uuid,
				       disk_super->metadata_uuid,
				       BTRFS_FSID_SIZE);
			else
				memcpy(fs_devices->metadata_uuid,
				       disk_super->fsid, BTRFS_FSID_SIZE);

			fs_devices->fsid_change = false;
		}
>>>>>>> upstream/android-13
	}

	if (!device) {
		if (fs_devices->opened) {
			mutex_unlock(&fs_devices->device_list_mutex);
			return ERR_PTR(-EBUSY);
		}

		device = btrfs_alloc_device(NULL, &devid,
					    disk_super->dev_item.uuid);
		if (IS_ERR(device)) {
			mutex_unlock(&fs_devices->device_list_mutex);
			/* we can safely leave the fs_devices entry around */
			return device;
		}

		name = rcu_string_strdup(path, GFP_NOFS);
		if (!name) {
			btrfs_free_device(device);
			mutex_unlock(&fs_devices->device_list_mutex);
			return ERR_PTR(-ENOMEM);
		}
		rcu_assign_pointer(device->name, name);

		list_add_rcu(&device->dev_list, &fs_devices->devices);
		fs_devices->num_devices++;

		device->fs_devices = fs_devices;
		*new_device_added = true;

		if (disk_super->label[0])
<<<<<<< HEAD
			pr_info("BTRFS: device label %s devid %llu transid %llu %s\n",
				disk_super->label, devid, found_transid, path);
		else
			pr_info("BTRFS: device fsid %pU devid %llu transid %llu %s\n",
				disk_super->fsid, devid, found_transid, path);
=======
			pr_info(
	"BTRFS: device label %s devid %llu transid %llu %s scanned by %s (%d)\n",
				disk_super->label, devid, found_transid, path,
				current->comm, task_pid_nr(current));
		else
			pr_info(
	"BTRFS: device fsid %pU devid %llu transid %llu %s scanned by %s (%d)\n",
				disk_super->fsid, devid, found_transid, path,
				current->comm, task_pid_nr(current));
>>>>>>> upstream/android-13

	} else if (!device->name || strcmp(device->name->str, path)) {
		/*
		 * When FS is already mounted.
		 * 1. If you are here and if the device->name is NULL that
		 *    means this device was missing at time of FS mount.
		 * 2. If you are here and if the device->name is different
		 *    from 'path' that means either
		 *      a. The same device disappeared and reappeared with
		 *         different name. or
		 *      b. The missing-disk-which-was-replaced, has
		 *         reappeared now.
		 *
		 * We must allow 1 and 2a above. But 2b would be a spurious
		 * and unintentional.
		 *
		 * Further in case of 1 and 2a above, the disk at 'path'
		 * would have missed some transaction when it was away and
		 * in case of 2a the stale bdev has to be updated as well.
		 * 2b must not be allowed at all time.
		 */

		/*
		 * For now, we do allow update to btrfs_fs_device through the
		 * btrfs dev scan cli after FS has been mounted.  We're still
		 * tracking a problem where systems fail mount by subvolume id
		 * when we reject replacement on a mounted FS.
		 */
		if (!fs_devices->opened && found_transid < device->generation) {
			/*
			 * That is if the FS is _not_ mounted and if you
			 * are here, that means there is more than one
			 * disk with same uuid and devid.We keep the one
			 * with larger generation number or the last-in if
			 * generation are equal.
			 */
			mutex_unlock(&fs_devices->device_list_mutex);
			return ERR_PTR(-EEXIST);
		}

		/*
		 * We are going to replace the device path for a given devid,
		 * make sure it's the same device if the device is mounted
		 */
		if (device->bdev) {
<<<<<<< HEAD
			struct block_device *path_bdev;

			path_bdev = lookup_bdev(path);
			if (IS_ERR(path_bdev)) {
				mutex_unlock(&fs_devices->device_list_mutex);
				return ERR_CAST(path_bdev);
			}

			if (device->bdev != path_bdev) {
				bdput(path_bdev);
=======
			int error;
			dev_t path_dev;

			error = lookup_bdev(path, &path_dev);
			if (error) {
				mutex_unlock(&fs_devices->device_list_mutex);
				return ERR_PTR(error);
			}

			if (device->bdev->bd_dev != path_dev) {
>>>>>>> upstream/android-13
				mutex_unlock(&fs_devices->device_list_mutex);
				/*
				 * device->fs_info may not be reliable here, so
				 * pass in a NULL instead. This avoids a
				 * possible use-after-free when the fs_info and
				 * fs_info->sb are already torn down.
				 */
				btrfs_warn_in_rcu(NULL,
	"duplicate device %s devid %llu generation %llu scanned by %s (%d)",
						  path, devid, found_transid,
						  current->comm,
						  task_pid_nr(current));
				return ERR_PTR(-EEXIST);
			}
<<<<<<< HEAD
			bdput(path_bdev);
=======
>>>>>>> upstream/android-13
			btrfs_info_in_rcu(device->fs_info,
	"devid %llu device path %s changed to %s scanned by %s (%d)",
					  devid, rcu_str_deref(device->name),
					  path, current->comm,
					  task_pid_nr(current));
		}

		name = rcu_string_strdup(path, GFP_NOFS);
		if (!name) {
			mutex_unlock(&fs_devices->device_list_mutex);
			return ERR_PTR(-ENOMEM);
		}
		rcu_string_free(device->name);
		rcu_assign_pointer(device->name, name);
		if (test_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state)) {
			fs_devices->missing_devices--;
			clear_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state);
		}
	}

	/*
	 * Unmount does not free the btrfs_device struct but would zero
	 * generation along with most of the other members. So just update
	 * it back. We need it to pick the disk with largest generation
	 * (as above).
	 */
<<<<<<< HEAD
	if (!fs_devices->opened)
		device->generation = found_transid;
=======
	if (!fs_devices->opened) {
		device->generation = found_transid;
		fs_devices->latest_generation = max_t(u64, found_transid,
						fs_devices->latest_generation);
	}
>>>>>>> upstream/android-13

	fs_devices->total_devices = btrfs_super_num_devices(disk_super);

	mutex_unlock(&fs_devices->device_list_mutex);
	return device;
}

static struct btrfs_fs_devices *clone_fs_devices(struct btrfs_fs_devices *orig)
{
	struct btrfs_fs_devices *fs_devices;
	struct btrfs_device *device;
	struct btrfs_device *orig_dev;
<<<<<<< HEAD

	fs_devices = alloc_fs_devices(orig->fsid);
	if (IS_ERR(fs_devices))
		return fs_devices;

	mutex_lock(&orig->device_list_mutex);
	fs_devices->total_devices = orig->total_devices;

	/* We have held the volume lock, it is safe to get the devices. */
=======
	int ret = 0;

	lockdep_assert_held(&uuid_mutex);

	fs_devices = alloc_fs_devices(orig->fsid, NULL);
	if (IS_ERR(fs_devices))
		return fs_devices;

	fs_devices->total_devices = orig->total_devices;

>>>>>>> upstream/android-13
	list_for_each_entry(orig_dev, &orig->devices, dev_list) {
		struct rcu_string *name;

		device = btrfs_alloc_device(NULL, &orig_dev->devid,
					    orig_dev->uuid);
<<<<<<< HEAD
		if (IS_ERR(device))
			goto error;
=======
		if (IS_ERR(device)) {
			ret = PTR_ERR(device);
			goto error;
		}
>>>>>>> upstream/android-13

		/*
		 * This is ok to do without rcu read locked because we hold the
		 * uuid mutex so nothing we touch in here is going to disappear.
		 */
		if (orig_dev->name) {
			name = rcu_string_strdup(orig_dev->name->str,
					GFP_KERNEL);
			if (!name) {
				btrfs_free_device(device);
<<<<<<< HEAD
=======
				ret = -ENOMEM;
>>>>>>> upstream/android-13
				goto error;
			}
			rcu_assign_pointer(device->name, name);
		}

		list_add(&device->dev_list, &fs_devices->devices);
		device->fs_devices = fs_devices;
		fs_devices->num_devices++;
	}
<<<<<<< HEAD
	mutex_unlock(&orig->device_list_mutex);
	return fs_devices;
error:
	mutex_unlock(&orig->device_list_mutex);
	free_fs_devices(fs_devices);
	return ERR_PTR(-ENOMEM);
}

/*
 * After we have read the system tree and know devids belonging to
 * this filesystem, remove the device which does not belong there.
 */
void btrfs_free_extra_devids(struct btrfs_fs_devices *fs_devices, int step)
{
	struct btrfs_device *device, *next;
	struct btrfs_device *latest_dev = NULL;

	mutex_lock(&uuid_mutex);
again:
	/* This is the initialized path, it is safe to release the devices. */
	list_for_each_entry_safe(device, next, &fs_devices->devices, dev_list) {
		if (test_bit(BTRFS_DEV_STATE_IN_FS_METADATA,
							&device->dev_state)) {
			if (!test_bit(BTRFS_DEV_STATE_REPLACE_TGT,
			     &device->dev_state) &&
			    !test_bit(BTRFS_DEV_STATE_MISSING,
				      &device->dev_state) &&
			     (!latest_dev ||
			      device->generation > latest_dev->generation)) {
				latest_dev = device;
=======
	return fs_devices;
error:
	free_fs_devices(fs_devices);
	return ERR_PTR(ret);
}

static void __btrfs_free_extra_devids(struct btrfs_fs_devices *fs_devices,
				      struct btrfs_device **latest_dev)
{
	struct btrfs_device *device, *next;

	/* This is the initialized path, it is safe to release the devices. */
	list_for_each_entry_safe(device, next, &fs_devices->devices, dev_list) {
		if (test_bit(BTRFS_DEV_STATE_IN_FS_METADATA, &device->dev_state)) {
			if (!test_bit(BTRFS_DEV_STATE_REPLACE_TGT,
				      &device->dev_state) &&
			    !test_bit(BTRFS_DEV_STATE_MISSING,
				      &device->dev_state) &&
			    (!*latest_dev ||
			     device->generation > (*latest_dev)->generation)) {
				*latest_dev = device;
>>>>>>> upstream/android-13
			}
			continue;
		}

		/*
		 * We have already validated the presence of BTRFS_DEV_REPLACE_DEVID,
		 * in btrfs_init_dev_replace() so just continue.
		 */
		if (device->devid == BTRFS_DEV_REPLACE_DEVID)
			continue;

		if (device->bdev) {
			blkdev_put(device->bdev, device->mode);
			device->bdev = NULL;
			fs_devices->open_devices--;
		}
		if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state)) {
			list_del_init(&device->dev_alloc_list);
			clear_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state);
<<<<<<< HEAD
=======
			fs_devices->rw_devices--;
>>>>>>> upstream/android-13
		}
		list_del_init(&device->dev_list);
		fs_devices->num_devices--;
		btrfs_free_device(device);
	}

<<<<<<< HEAD
	if (fs_devices->seed) {
		fs_devices = fs_devices->seed;
		goto again;
	}

	fs_devices->latest_bdev = latest_dev->bdev;

	mutex_unlock(&uuid_mutex);
}

static void free_device_rcu(struct rcu_head *head)
{
	struct btrfs_device *device;

	device = container_of(head, struct btrfs_device, rcu);
	btrfs_free_device(device);
=======
}

/*
 * After we have read the system tree and know devids belonging to this
 * filesystem, remove the device which does not belong there.
 */
void btrfs_free_extra_devids(struct btrfs_fs_devices *fs_devices)
{
	struct btrfs_device *latest_dev = NULL;
	struct btrfs_fs_devices *seed_dev;

	mutex_lock(&uuid_mutex);
	__btrfs_free_extra_devids(fs_devices, &latest_dev);

	list_for_each_entry(seed_dev, &fs_devices->seed_list, seed_list)
		__btrfs_free_extra_devids(seed_dev, &latest_dev);

	fs_devices->latest_dev = latest_dev;

	mutex_unlock(&uuid_mutex);
>>>>>>> upstream/android-13
}

static void btrfs_close_bdev(struct btrfs_device *device)
{
	if (!device->bdev)
		return;

	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state)) {
		sync_blockdev(device->bdev);
		invalidate_bdev(device->bdev);
	}

	blkdev_put(device->bdev, device->mode);
}

static void btrfs_close_one_device(struct btrfs_device *device)
{
	struct btrfs_fs_devices *fs_devices = device->fs_devices;
<<<<<<< HEAD
	struct btrfs_device *new_device;
	struct rcu_string *name;

	if (device->bdev)
		fs_devices->open_devices--;
=======
>>>>>>> upstream/android-13

	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state) &&
	    device->devid != BTRFS_DEV_REPLACE_DEVID) {
		list_del_init(&device->dev_alloc_list);
		fs_devices->rw_devices--;
	}

<<<<<<< HEAD
	if (test_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state))
		fs_devices->missing_devices--;

	btrfs_close_bdev(device);

	new_device = btrfs_alloc_device(NULL, &device->devid,
					device->uuid);
	BUG_ON(IS_ERR(new_device)); /* -ENOMEM */

	/* Safe because we are under uuid_mutex */
	if (device->name) {
		name = rcu_string_strdup(device->name->str, GFP_NOFS);
		BUG_ON(!name); /* -ENOMEM */
		rcu_assign_pointer(new_device->name, name);
	}

	list_replace_rcu(&device->dev_list, &new_device->dev_list);
	new_device->fs_devices = device->fs_devices;

	call_rcu(&device->rcu, free_device_rcu);
}

static int close_fs_devices(struct btrfs_fs_devices *fs_devices)
{
	struct btrfs_device *device, *tmp;

	if (--fs_devices->opened > 0)
		return 0;

	mutex_lock(&fs_devices->device_list_mutex);
	list_for_each_entry_safe(device, tmp, &fs_devices->devices, dev_list) {
		btrfs_close_one_device(device);
	}
	mutex_unlock(&fs_devices->device_list_mutex);
=======
	if (device->devid == BTRFS_DEV_REPLACE_DEVID)
		clear_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state);

	if (test_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state)) {
		clear_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state);
		fs_devices->missing_devices--;
	}

	btrfs_close_bdev(device);
	if (device->bdev) {
		fs_devices->open_devices--;
		device->bdev = NULL;
	}
	clear_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state);
	btrfs_destroy_dev_zone_info(device);

	device->fs_info = NULL;
	atomic_set(&device->dev_stats_ccnt, 0);
	extent_io_tree_release(&device->alloc_state);

	/*
	 * Reset the flush error record. We might have a transient flush error
	 * in this mount, and if so we aborted the current transaction and set
	 * the fs to an error state, guaranteeing no super blocks can be further
	 * committed. However that error might be transient and if we unmount the
	 * filesystem and mount it again, we should allow the mount to succeed
	 * (btrfs_check_rw_degradable() should not fail) - if after mounting the
	 * filesystem again we still get flush errors, then we will again abort
	 * any transaction and set the error state, guaranteeing no commits of
	 * unsafe super blocks.
	 */
	device->last_flush_error = 0;

	/* Verify the device is back in a pristine state  */
	ASSERT(!test_bit(BTRFS_DEV_STATE_FLUSH_SENT, &device->dev_state));
	ASSERT(!test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state));
	ASSERT(list_empty(&device->dev_alloc_list));
	ASSERT(list_empty(&device->post_commit_list));
	ASSERT(atomic_read(&device->reada_in_flight) == 0);
}

static void close_fs_devices(struct btrfs_fs_devices *fs_devices)
{
	struct btrfs_device *device, *tmp;

	lockdep_assert_held(&uuid_mutex);

	if (--fs_devices->opened > 0)
		return;

	list_for_each_entry_safe(device, tmp, &fs_devices->devices, dev_list)
		btrfs_close_one_device(device);
>>>>>>> upstream/android-13

	WARN_ON(fs_devices->open_devices);
	WARN_ON(fs_devices->rw_devices);
	fs_devices->opened = 0;
<<<<<<< HEAD
	fs_devices->seeding = 0;

	return 0;
}

int btrfs_close_devices(struct btrfs_fs_devices *fs_devices)
{
	struct btrfs_fs_devices *seed_devices = NULL;
	int ret;

	mutex_lock(&uuid_mutex);
	ret = close_fs_devices(fs_devices);
	if (!fs_devices->opened) {
		seed_devices = fs_devices->seed;
		fs_devices->seed = NULL;
	}
	mutex_unlock(&uuid_mutex);

	while (seed_devices) {
		fs_devices = seed_devices;
		seed_devices = fs_devices->seed;
		close_fs_devices(fs_devices);
		free_fs_devices(fs_devices);
	}
	return ret;
=======
	fs_devices->seeding = false;
	fs_devices->fs_info = NULL;
}

void btrfs_close_devices(struct btrfs_fs_devices *fs_devices)
{
	LIST_HEAD(list);
	struct btrfs_fs_devices *tmp;

	mutex_lock(&uuid_mutex);
	close_fs_devices(fs_devices);
	if (!fs_devices->opened)
		list_splice_init(&fs_devices->seed_list, &list);

	list_for_each_entry_safe(fs_devices, tmp, &list, seed_list) {
		close_fs_devices(fs_devices);
		list_del(&fs_devices->seed_list);
		free_fs_devices(fs_devices);
	}
	mutex_unlock(&uuid_mutex);
>>>>>>> upstream/android-13
}

static int open_fs_devices(struct btrfs_fs_devices *fs_devices,
				fmode_t flags, void *holder)
{
	struct btrfs_device *device;
	struct btrfs_device *latest_dev = NULL;
<<<<<<< HEAD
	int ret = 0;

	flags |= FMODE_EXCL;

	list_for_each_entry(device, &fs_devices->devices, dev_list) {
		/* Just open everything we can; ignore failures here */
		if (btrfs_open_one_device(fs_devices, device, flags, holder))
			continue;

		if (!latest_dev ||
		    device->generation > latest_dev->generation)
			latest_dev = device;
	}
	if (fs_devices->open_devices == 0) {
		ret = -EINVAL;
		goto out;
	}
	fs_devices->opened = 1;
	fs_devices->latest_bdev = latest_dev->bdev;
	fs_devices->total_rw_bytes = 0;
out:
	return ret;
}

static int devid_cmp(void *priv, struct list_head *a, struct list_head *b)
{
	struct btrfs_device *dev1, *dev2;
=======
	struct btrfs_device *tmp_device;

	flags |= FMODE_EXCL;

	list_for_each_entry_safe(device, tmp_device, &fs_devices->devices,
				 dev_list) {
		int ret;

		ret = btrfs_open_one_device(fs_devices, device, flags, holder);
		if (ret == 0 &&
		    (!latest_dev || device->generation > latest_dev->generation)) {
			latest_dev = device;
		} else if (ret == -ENODATA) {
			fs_devices->num_devices--;
			list_del(&device->dev_list);
			btrfs_free_device(device);
		}
	}
	if (fs_devices->open_devices == 0)
		return -EINVAL;

	fs_devices->opened = 1;
	fs_devices->latest_dev = latest_dev;
	fs_devices->total_rw_bytes = 0;
	fs_devices->chunk_alloc_policy = BTRFS_CHUNK_ALLOC_REGULAR;
	fs_devices->read_policy = BTRFS_READ_POLICY_PID;

	return 0;
}

static int devid_cmp(void *priv, const struct list_head *a,
		     const struct list_head *b)
{
	const struct btrfs_device *dev1, *dev2;
>>>>>>> upstream/android-13

	dev1 = list_entry(a, struct btrfs_device, dev_list);
	dev2 = list_entry(b, struct btrfs_device, dev_list);

	if (dev1->devid < dev2->devid)
		return -1;
	else if (dev1->devid > dev2->devid)
		return 1;
	return 0;
}

int btrfs_open_devices(struct btrfs_fs_devices *fs_devices,
		       fmode_t flags, void *holder)
{
	int ret;

	lockdep_assert_held(&uuid_mutex);
	/*
	 * The device_list_mutex cannot be taken here in case opening the
<<<<<<< HEAD
	 * underlying device takes further locks like bd_mutex.
=======
	 * underlying device takes further locks like open_mutex.
>>>>>>> upstream/android-13
	 *
	 * We also don't need the lock here as this is called during mount and
	 * exclusion is provided by uuid_mutex
	 */

	if (fs_devices->opened) {
		fs_devices->opened++;
		ret = 0;
	} else {
		list_sort(NULL, &fs_devices->devices, devid_cmp);
		ret = open_fs_devices(fs_devices, flags, holder);
	}

	return ret;
}

<<<<<<< HEAD
static void btrfs_release_disk_super(struct page *page)
{
	kunmap(page);
	put_page(page);
}

static int btrfs_read_disk_super(struct block_device *bdev, u64 bytenr,
				 struct page **page,
				 struct btrfs_super_block **disk_super)
{
=======
void btrfs_release_disk_super(struct btrfs_super_block *super)
{
	struct page *page = virt_to_page(super);

	put_page(page);
}

static struct btrfs_super_block *btrfs_read_disk_super(struct block_device *bdev,
						       u64 bytenr, u64 bytenr_orig)
{
	struct btrfs_super_block *disk_super;
	struct page *page;
>>>>>>> upstream/android-13
	void *p;
	pgoff_t index;

	/* make sure our super fits in the device */
	if (bytenr + PAGE_SIZE >= i_size_read(bdev->bd_inode))
<<<<<<< HEAD
		return 1;

	/* make sure our super fits in the page */
	if (sizeof(**disk_super) > PAGE_SIZE)
		return 1;

	/* make sure our super doesn't straddle pages on disk */
	index = bytenr >> PAGE_SHIFT;
	if ((bytenr + sizeof(**disk_super) - 1) >> PAGE_SHIFT != index)
		return 1;

	/* pull in the page with our super */
	*page = read_cache_page_gfp(bdev->bd_inode->i_mapping,
				   index, GFP_KERNEL);

	if (IS_ERR_OR_NULL(*page))
		return 1;

	p = kmap(*page);

	/* align our pointer to the offset of the super block */
	*disk_super = p + (bytenr & ~PAGE_MASK);

	if (btrfs_super_bytenr(*disk_super) != bytenr ||
	    btrfs_super_magic(*disk_super) != BTRFS_MAGIC) {
		btrfs_release_disk_super(*page);
		return 1;
	}

	if ((*disk_super)->label[0] &&
		(*disk_super)->label[BTRFS_LABEL_SIZE - 1])
		(*disk_super)->label[BTRFS_LABEL_SIZE - 1] = '\0';

	return 0;
=======
		return ERR_PTR(-EINVAL);

	/* make sure our super fits in the page */
	if (sizeof(*disk_super) > PAGE_SIZE)
		return ERR_PTR(-EINVAL);

	/* make sure our super doesn't straddle pages on disk */
	index = bytenr >> PAGE_SHIFT;
	if ((bytenr + sizeof(*disk_super) - 1) >> PAGE_SHIFT != index)
		return ERR_PTR(-EINVAL);

	/* pull in the page with our super */
	page = read_cache_page_gfp(bdev->bd_inode->i_mapping, index, GFP_KERNEL);

	if (IS_ERR(page))
		return ERR_CAST(page);

	p = page_address(page);

	/* align our pointer to the offset of the super block */
	disk_super = p + offset_in_page(bytenr);

	if (btrfs_super_bytenr(disk_super) != bytenr_orig ||
	    btrfs_super_magic(disk_super) != BTRFS_MAGIC) {
		btrfs_release_disk_super(p);
		return ERR_PTR(-EINVAL);
	}

	if (disk_super->label[0] && disk_super->label[BTRFS_LABEL_SIZE - 1])
		disk_super->label[BTRFS_LABEL_SIZE - 1] = 0;

	return disk_super;
}

int btrfs_forget_devices(const char *path)
{
	int ret;

	mutex_lock(&uuid_mutex);
	ret = btrfs_free_stale_devices(strlen(path) ? path : NULL, NULL);
	mutex_unlock(&uuid_mutex);

	return ret;
>>>>>>> upstream/android-13
}

/*
 * Look for a btrfs signature on a device. This may be called out of the mount path
 * and we are not allowed to call set_blocksize during the scan. The superblock
 * is read via pagecache
 */
struct btrfs_device *btrfs_scan_one_device(const char *path, fmode_t flags,
					   void *holder)
{
	struct btrfs_super_block *disk_super;
	bool new_device_added = false;
	struct btrfs_device *device = NULL;
	struct block_device *bdev;
<<<<<<< HEAD
	struct page *page;
	u64 bytenr;
=======
	u64 bytenr, bytenr_orig;
	int ret;
>>>>>>> upstream/android-13

	lockdep_assert_held(&uuid_mutex);

	/*
	 * we would like to check all the supers, but that would make
	 * a btrfs mount succeed after a mkfs from a different FS.
	 * So, we need to add a special mount option to scan for
	 * later supers, using BTRFS_SUPER_MIRROR_MAX instead
	 */
<<<<<<< HEAD
	bytenr = btrfs_sb_offset(0);
=======
>>>>>>> upstream/android-13
	flags |= FMODE_EXCL;

	bdev = blkdev_get_by_path(path, flags, holder);
	if (IS_ERR(bdev))
		return ERR_CAST(bdev);

<<<<<<< HEAD
	if (btrfs_read_disk_super(bdev, bytenr, &page, &disk_super)) {
		device = ERR_PTR(-EINVAL);
=======
	bytenr_orig = btrfs_sb_offset(0);
	ret = btrfs_sb_log_location_bdev(bdev, 0, READ, &bytenr);
	if (ret) {
		device = ERR_PTR(ret);
		goto error_bdev_put;
	}

	disk_super = btrfs_read_disk_super(bdev, bytenr, bytenr_orig);
	if (IS_ERR(disk_super)) {
		device = ERR_CAST(disk_super);
>>>>>>> upstream/android-13
		goto error_bdev_put;
	}

	device = device_list_add(path, disk_super, &new_device_added);
	if (!IS_ERR(device)) {
		if (new_device_added)
			btrfs_free_stale_devices(path, device);
	}

<<<<<<< HEAD
	btrfs_release_disk_super(page);
=======
	btrfs_release_disk_super(disk_super);
>>>>>>> upstream/android-13

error_bdev_put:
	blkdev_put(bdev, flags);

	return device;
}

<<<<<<< HEAD
static int contains_pending_extent(struct btrfs_transaction *transaction,
				   struct btrfs_device *device,
				   u64 *start, u64 len)
{
	struct btrfs_fs_info *fs_info = device->fs_info;
	struct extent_map *em;
	struct list_head *search_list = &fs_info->pinned_chunks;
	int ret = 0;
	u64 physical_start = *start;

	if (transaction)
		search_list = &transaction->pending_chunks;
again:
	list_for_each_entry(em, search_list, list) {
		struct map_lookup *map;
		int i;

		map = em->map_lookup;
		for (i = 0; i < map->num_stripes; i++) {
			u64 end;

			if (map->stripes[i].dev != device)
				continue;
			if (map->stripes[i].physical >= physical_start + len ||
			    map->stripes[i].physical + em->orig_block_len <=
			    physical_start)
				continue;
			/*
			 * Make sure that while processing the pinned list we do
			 * not override our *start with a lower value, because
			 * we can have pinned chunks that fall within this
			 * device hole and that have lower physical addresses
			 * than the pending chunks we processed before. If we
			 * do not take this special care we can end up getting
			 * 2 pending chunks that start at the same physical
			 * device offsets because the end offset of a pinned
			 * chunk can be equal to the start offset of some
			 * pending chunk.
			 */
			end = map->stripes[i].physical + em->orig_block_len;
			if (end > *start) {
				*start = end;
				ret = 1;
			}
		}
	}
	if (search_list != &fs_info->pinned_chunks) {
		search_list = &fs_info->pinned_chunks;
		goto again;
	}

	return ret;
}

=======
/*
 * Try to find a chunk that intersects [start, start + len] range and when one
 * such is found, record the end of it in *start
 */
static bool contains_pending_extent(struct btrfs_device *device, u64 *start,
				    u64 len)
{
	u64 physical_start, physical_end;

	lockdep_assert_held(&device->fs_info->chunk_mutex);

	if (!find_first_extent_bit(&device->alloc_state, *start,
				   &physical_start, &physical_end,
				   CHUNK_ALLOCATED, NULL)) {

		if (in_range(physical_start, *start, len) ||
		    in_range(*start, physical_start,
			     physical_end - physical_start)) {
			*start = physical_end + 1;
			return true;
		}
	}
	return false;
}

static u64 dev_extent_search_start(struct btrfs_device *device, u64 start)
{
	switch (device->fs_devices->chunk_alloc_policy) {
	case BTRFS_CHUNK_ALLOC_REGULAR:
		/*
		 * We don't want to overwrite the superblock on the drive nor
		 * any area used by the boot loader (grub for example), so we
		 * make sure to start at an offset of at least 1MB.
		 */
		return max_t(u64, start, SZ_1M);
	case BTRFS_CHUNK_ALLOC_ZONED:
		/*
		 * We don't care about the starting region like regular
		 * allocator, because we anyway use/reserve the first two zones
		 * for superblock logging.
		 */
		return ALIGN(start, device->zone_info->zone_size);
	default:
		BUG();
	}
}

static bool dev_extent_hole_check_zoned(struct btrfs_device *device,
					u64 *hole_start, u64 *hole_size,
					u64 num_bytes)
{
	u64 zone_size = device->zone_info->zone_size;
	u64 pos;
	int ret;
	bool changed = false;

	ASSERT(IS_ALIGNED(*hole_start, zone_size));

	while (*hole_size > 0) {
		pos = btrfs_find_allocatable_zones(device, *hole_start,
						   *hole_start + *hole_size,
						   num_bytes);
		if (pos != *hole_start) {
			*hole_size = *hole_start + *hole_size - pos;
			*hole_start = pos;
			changed = true;
			if (*hole_size < num_bytes)
				break;
		}

		ret = btrfs_ensure_empty_zones(device, pos, num_bytes);

		/* Range is ensured to be empty */
		if (!ret)
			return changed;

		/* Given hole range was invalid (outside of device) */
		if (ret == -ERANGE) {
			*hole_start += *hole_size;
			*hole_size = 0;
			return true;
		}

		*hole_start += zone_size;
		*hole_size -= zone_size;
		changed = true;
	}

	return changed;
}

/**
 * dev_extent_hole_check - check if specified hole is suitable for allocation
 * @device:	the device which we have the hole
 * @hole_start: starting position of the hole
 * @hole_size:	the size of the hole
 * @num_bytes:	the size of the free space that we need
 *
 * This function may modify @hole_start and @hole_size to reflect the suitable
 * position for allocation. Returns 1 if hole position is updated, 0 otherwise.
 */
static bool dev_extent_hole_check(struct btrfs_device *device, u64 *hole_start,
				  u64 *hole_size, u64 num_bytes)
{
	bool changed = false;
	u64 hole_end = *hole_start + *hole_size;

	for (;;) {
		/*
		 * Check before we set max_hole_start, otherwise we could end up
		 * sending back this offset anyway.
		 */
		if (contains_pending_extent(device, hole_start, *hole_size)) {
			if (hole_end >= *hole_start)
				*hole_size = hole_end - *hole_start;
			else
				*hole_size = 0;
			changed = true;
		}

		switch (device->fs_devices->chunk_alloc_policy) {
		case BTRFS_CHUNK_ALLOC_REGULAR:
			/* No extra check */
			break;
		case BTRFS_CHUNK_ALLOC_ZONED:
			if (dev_extent_hole_check_zoned(device, hole_start,
							hole_size, num_bytes)) {
				changed = true;
				/*
				 * The changed hole can contain pending extent.
				 * Loop again to check that.
				 */
				continue;
			}
			break;
		default:
			BUG();
		}

		break;
	}

	return changed;
}
>>>>>>> upstream/android-13

/*
 * find_free_dev_extent_start - find free space in the specified device
 * @device:	  the device which we search the free space in
 * @num_bytes:	  the size of the free space that we need
 * @search_start: the position from which to begin the search
 * @start:	  store the start of the free space.
 * @len:	  the size of the free space. that we find, or the size
 *		  of the max free space if we don't find suitable free space
 *
 * this uses a pretty simple search, the expectation is that it is
 * called very infrequently and that a given device has a small number
 * of extents
 *
 * @start is used to store the start of the free space if we find. But if we
 * don't find suitable free space, it will be used to store the start position
 * of the max free space.
 *
 * @len is used to store the size of the free space that we find.
 * But if we don't find suitable free space, it is used to store the size of
 * the max free space.
<<<<<<< HEAD
 */
int find_free_dev_extent_start(struct btrfs_transaction *transaction,
			       struct btrfs_device *device, u64 num_bytes,
			       u64 search_start, u64 *start, u64 *len)
=======
 *
 * NOTE: This function will search *commit* root of device tree, and does extra
 * check to ensure dev extents are not double allocated.
 * This makes the function safe to allocate dev extents but may not report
 * correct usable device space, as device extent freed in current transaction
 * is not reported as available.
 */
static int find_free_dev_extent_start(struct btrfs_device *device,
				u64 num_bytes, u64 search_start, u64 *start,
				u64 *len)
>>>>>>> upstream/android-13
{
	struct btrfs_fs_info *fs_info = device->fs_info;
	struct btrfs_root *root = fs_info->dev_root;
	struct btrfs_key key;
	struct btrfs_dev_extent *dev_extent;
	struct btrfs_path *path;
	u64 hole_size;
	u64 max_hole_start;
	u64 max_hole_size;
	u64 extent_end;
	u64 search_end = device->total_bytes;
	int ret;
	int slot;
	struct extent_buffer *l;

<<<<<<< HEAD
	/*
	 * We don't want to overwrite the superblock on the drive nor any area
	 * used by the boot loader (grub for example), so we make sure to start
	 * at an offset of at least 1MB.
	 */
	search_start = max_t(u64, search_start, SZ_1M);
=======
	search_start = dev_extent_search_start(device, search_start);

	WARN_ON(device->zone_info &&
		!IS_ALIGNED(num_bytes, device->zone_info->zone_size));
>>>>>>> upstream/android-13

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	max_hole_start = search_start;
	max_hole_size = 0;

again:
	if (search_start >= search_end ||
		test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state)) {
		ret = -ENOSPC;
		goto out;
	}

	path->reada = READA_FORWARD;
	path->search_commit_root = 1;
	path->skip_locking = 1;

	key.objectid = device->devid;
	key.offset = search_start;
	key.type = BTRFS_DEV_EXTENT_KEY;

<<<<<<< HEAD
	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		goto out;
	if (ret > 0) {
		ret = btrfs_previous_item(root, path, key.objectid, key.type);
		if (ret < 0)
			goto out;
	}
=======
	ret = btrfs_search_backwards(root, &key, path);
	if (ret < 0)
		goto out;
>>>>>>> upstream/android-13

	while (1) {
		l = path->nodes[0];
		slot = path->slots[0];
		if (slot >= btrfs_header_nritems(l)) {
			ret = btrfs_next_leaf(root, path);
			if (ret == 0)
				continue;
			if (ret < 0)
				goto out;

			break;
		}
		btrfs_item_key_to_cpu(l, &key, slot);

		if (key.objectid < device->devid)
			goto next;

		if (key.objectid > device->devid)
			break;

		if (key.type != BTRFS_DEV_EXTENT_KEY)
			goto next;

		if (key.offset > search_start) {
			hole_size = key.offset - search_start;
<<<<<<< HEAD

			/*
			 * Have to check before we set max_hole_start, otherwise
			 * we could end up sending back this offset anyway.
			 */
			if (contains_pending_extent(transaction, device,
						    &search_start,
						    hole_size)) {
				if (key.offset >= search_start) {
					hole_size = key.offset - search_start;
				} else {
					WARN_ON_ONCE(1);
					hole_size = 0;
				}
			}
=======
			dev_extent_hole_check(device, &search_start, &hole_size,
					      num_bytes);
>>>>>>> upstream/android-13

			if (hole_size > max_hole_size) {
				max_hole_start = search_start;
				max_hole_size = hole_size;
			}

			/*
			 * If this free space is greater than which we need,
			 * it must be the max free space that we have found
			 * until now, so max_hole_start must point to the start
			 * of this free space and the length of this free space
			 * is stored in max_hole_size. Thus, we return
			 * max_hole_start and max_hole_size and go back to the
			 * caller.
			 */
			if (hole_size >= num_bytes) {
				ret = 0;
				goto out;
			}
		}

		dev_extent = btrfs_item_ptr(l, slot, struct btrfs_dev_extent);
		extent_end = key.offset + btrfs_dev_extent_length(l,
								  dev_extent);
		if (extent_end > search_start)
			search_start = extent_end;
next:
		path->slots[0]++;
		cond_resched();
	}

	/*
	 * At this point, search_start should be the end of
	 * allocated dev extents, and when shrinking the device,
	 * search_end may be smaller than search_start.
	 */
	if (search_end > search_start) {
		hole_size = search_end - search_start;
<<<<<<< HEAD

		if (contains_pending_extent(transaction, device, &search_start,
					    hole_size)) {
=======
		if (dev_extent_hole_check(device, &search_start, &hole_size,
					  num_bytes)) {
>>>>>>> upstream/android-13
			btrfs_release_path(path);
			goto again;
		}

		if (hole_size > max_hole_size) {
			max_hole_start = search_start;
			max_hole_size = hole_size;
		}
	}

	/* See above. */
	if (max_hole_size < num_bytes)
		ret = -ENOSPC;
	else
		ret = 0;

out:
	btrfs_free_path(path);
	*start = max_hole_start;
	if (len)
		*len = max_hole_size;
	return ret;
}

<<<<<<< HEAD
int find_free_dev_extent(struct btrfs_trans_handle *trans,
			 struct btrfs_device *device, u64 num_bytes,
			 u64 *start, u64 *len)
{
	/* FIXME use last free of some kind */
	return find_free_dev_extent_start(trans->transaction, device,
					  num_bytes, 0, start, len);
=======
int find_free_dev_extent(struct btrfs_device *device, u64 num_bytes,
			 u64 *start, u64 *len)
{
	/* FIXME use last free of some kind */
	return find_free_dev_extent_start(device, num_bytes, 0, start, len);
>>>>>>> upstream/android-13
}

static int btrfs_free_dev_extent(struct btrfs_trans_handle *trans,
			  struct btrfs_device *device,
			  u64 start, u64 *dev_extent_len)
{
	struct btrfs_fs_info *fs_info = device->fs_info;
	struct btrfs_root *root = fs_info->dev_root;
	int ret;
	struct btrfs_path *path;
	struct btrfs_key key;
	struct btrfs_key found_key;
	struct extent_buffer *leaf = NULL;
	struct btrfs_dev_extent *extent = NULL;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = device->devid;
	key.offset = start;
	key.type = BTRFS_DEV_EXTENT_KEY;
again:
	ret = btrfs_search_slot(trans, root, &key, path, -1, 1);
	if (ret > 0) {
		ret = btrfs_previous_item(root, path, key.objectid,
					  BTRFS_DEV_EXTENT_KEY);
		if (ret)
			goto out;
		leaf = path->nodes[0];
		btrfs_item_key_to_cpu(leaf, &found_key, path->slots[0]);
		extent = btrfs_item_ptr(leaf, path->slots[0],
					struct btrfs_dev_extent);
		BUG_ON(found_key.offset > start || found_key.offset +
		       btrfs_dev_extent_length(leaf, extent) < start);
		key = found_key;
		btrfs_release_path(path);
		goto again;
	} else if (ret == 0) {
		leaf = path->nodes[0];
		extent = btrfs_item_ptr(leaf, path->slots[0],
					struct btrfs_dev_extent);
	} else {
<<<<<<< HEAD
		btrfs_handle_fs_error(fs_info, ret, "Slot search failed");
=======
>>>>>>> upstream/android-13
		goto out;
	}

	*dev_extent_len = btrfs_dev_extent_length(leaf, extent);

	ret = btrfs_del_item(trans, root, path);
<<<<<<< HEAD
	if (ret) {
		btrfs_handle_fs_error(fs_info, ret,
				      "Failed to remove dev extent item");
	} else {
		set_bit(BTRFS_TRANS_HAVE_FREE_BGS, &trans->transaction->flags);
	}
out:
	btrfs_free_path(path);
	return ret;
}

static int btrfs_alloc_dev_extent(struct btrfs_trans_handle *trans,
				  struct btrfs_device *device,
				  u64 chunk_offset, u64 start, u64 num_bytes)
{
	int ret;
	struct btrfs_path *path;
	struct btrfs_fs_info *fs_info = device->fs_info;
	struct btrfs_root *root = fs_info->dev_root;
	struct btrfs_dev_extent *extent;
	struct extent_buffer *leaf;
	struct btrfs_key key;

	WARN_ON(!test_bit(BTRFS_DEV_STATE_IN_FS_METADATA, &device->dev_state));
	WARN_ON(test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state));
	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = device->devid;
	key.offset = start;
	key.type = BTRFS_DEV_EXTENT_KEY;
	ret = btrfs_insert_empty_item(trans, root, path, &key,
				      sizeof(*extent));
	if (ret)
		goto out;

	leaf = path->nodes[0];
	extent = btrfs_item_ptr(leaf, path->slots[0],
				struct btrfs_dev_extent);
	btrfs_set_dev_extent_chunk_tree(leaf, extent,
					BTRFS_CHUNK_TREE_OBJECTID);
	btrfs_set_dev_extent_chunk_objectid(leaf, extent,
					    BTRFS_FIRST_CHUNK_TREE_OBJECTID);
	btrfs_set_dev_extent_chunk_offset(leaf, extent, chunk_offset);

	btrfs_set_dev_extent_length(leaf, extent, num_bytes);
	btrfs_mark_buffer_dirty(leaf);
=======
	if (ret == 0)
		set_bit(BTRFS_TRANS_HAVE_FREE_BGS, &trans->transaction->flags);
>>>>>>> upstream/android-13
out:
	btrfs_free_path(path);
	return ret;
}

static u64 find_next_chunk(struct btrfs_fs_info *fs_info)
{
	struct extent_map_tree *em_tree;
	struct extent_map *em;
	struct rb_node *n;
	u64 ret = 0;

<<<<<<< HEAD
	em_tree = &fs_info->mapping_tree.map_tree;
	read_lock(&em_tree->lock);
	n = rb_last(&em_tree->map);
=======
	em_tree = &fs_info->mapping_tree;
	read_lock(&em_tree->lock);
	n = rb_last(&em_tree->map.rb_root);
>>>>>>> upstream/android-13
	if (n) {
		em = rb_entry(n, struct extent_map, rb_node);
		ret = em->start + em->len;
	}
	read_unlock(&em_tree->lock);

	return ret;
}

static noinline int find_next_devid(struct btrfs_fs_info *fs_info,
				    u64 *devid_ret)
{
	int ret;
	struct btrfs_key key;
	struct btrfs_key found_key;
	struct btrfs_path *path;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = BTRFS_DEV_ITEMS_OBJECTID;
	key.type = BTRFS_DEV_ITEM_KEY;
	key.offset = (u64)-1;

	ret = btrfs_search_slot(NULL, fs_info->chunk_root, &key, path, 0, 0);
	if (ret < 0)
		goto error;

<<<<<<< HEAD
	BUG_ON(ret == 0); /* Corruption */
=======
	if (ret == 0) {
		/* Corruption */
		btrfs_err(fs_info, "corrupted chunk tree devid -1 matched");
		ret = -EUCLEAN;
		goto error;
	}
>>>>>>> upstream/android-13

	ret = btrfs_previous_item(fs_info->chunk_root, path,
				  BTRFS_DEV_ITEMS_OBJECTID,
				  BTRFS_DEV_ITEM_KEY);
	if (ret) {
		*devid_ret = 1;
	} else {
		btrfs_item_key_to_cpu(path->nodes[0], &found_key,
				      path->slots[0]);
		*devid_ret = found_key.offset + 1;
	}
	ret = 0;
error:
	btrfs_free_path(path);
	return ret;
}

/*
 * the device information is stored in the chunk root
 * the btrfs_device struct should be fully filled in
 */
static int btrfs_add_dev_item(struct btrfs_trans_handle *trans,
			    struct btrfs_device *device)
{
	int ret;
	struct btrfs_path *path;
	struct btrfs_dev_item *dev_item;
	struct extent_buffer *leaf;
	struct btrfs_key key;
	unsigned long ptr;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = BTRFS_DEV_ITEMS_OBJECTID;
	key.type = BTRFS_DEV_ITEM_KEY;
	key.offset = device->devid;

	ret = btrfs_insert_empty_item(trans, trans->fs_info->chunk_root, path,
				      &key, sizeof(*dev_item));
	if (ret)
		goto out;

	leaf = path->nodes[0];
	dev_item = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_dev_item);

	btrfs_set_device_id(leaf, dev_item, device->devid);
	btrfs_set_device_generation(leaf, dev_item, 0);
	btrfs_set_device_type(leaf, dev_item, device->type);
	btrfs_set_device_io_align(leaf, dev_item, device->io_align);
	btrfs_set_device_io_width(leaf, dev_item, device->io_width);
	btrfs_set_device_sector_size(leaf, dev_item, device->sector_size);
	btrfs_set_device_total_bytes(leaf, dev_item,
				     btrfs_device_get_disk_total_bytes(device));
	btrfs_set_device_bytes_used(leaf, dev_item,
				    btrfs_device_get_bytes_used(device));
	btrfs_set_device_group(leaf, dev_item, 0);
	btrfs_set_device_seek_speed(leaf, dev_item, 0);
	btrfs_set_device_bandwidth(leaf, dev_item, 0);
	btrfs_set_device_start_offset(leaf, dev_item, 0);

	ptr = btrfs_device_uuid(dev_item);
	write_extent_buffer(leaf, device->uuid, ptr, BTRFS_UUID_SIZE);
	ptr = btrfs_device_fsid(dev_item);
<<<<<<< HEAD
	write_extent_buffer(leaf, trans->fs_info->fsid, ptr, BTRFS_FSID_SIZE);
=======
	write_extent_buffer(leaf, trans->fs_info->fs_devices->metadata_uuid,
			    ptr, BTRFS_FSID_SIZE);
>>>>>>> upstream/android-13
	btrfs_mark_buffer_dirty(leaf);

	ret = 0;
out:
	btrfs_free_path(path);
	return ret;
}

/*
 * Function to update ctime/mtime for a given device path.
 * Mainly used for ctime/mtime based probe like libblkid.
<<<<<<< HEAD
 */
static void update_dev_time(const char *path_name)
{
	struct file *filp;

	filp = filp_open(path_name, O_RDWR, 0);
	if (IS_ERR(filp))
		return;
	file_update_time(filp);
	filp_close(filp, NULL);
}

static int btrfs_rm_dev_item(struct btrfs_fs_info *fs_info,
			     struct btrfs_device *device)
{
	struct btrfs_root *root = fs_info->chunk_root;
=======
 *
 * We don't care about errors here, this is just to be kind to userspace.
 */
static void update_dev_time(const char *device_path)
{
	struct path path;
	struct timespec64 now;
	int ret;

	ret = kern_path(device_path, LOOKUP_FOLLOW, &path);
	if (ret)
		return;

	now = current_time(d_inode(path.dentry));
	inode_update_time(d_inode(path.dentry), &now, S_MTIME | S_CTIME);
	path_put(&path);
}

static int btrfs_rm_dev_item(struct btrfs_device *device)
{
	struct btrfs_root *root = device->fs_info->chunk_root;
>>>>>>> upstream/android-13
	int ret;
	struct btrfs_path *path;
	struct btrfs_key key;
	struct btrfs_trans_handle *trans;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans)) {
		btrfs_free_path(path);
		return PTR_ERR(trans);
	}
	key.objectid = BTRFS_DEV_ITEMS_OBJECTID;
	key.type = BTRFS_DEV_ITEM_KEY;
	key.offset = device->devid;

	ret = btrfs_search_slot(trans, root, &key, path, -1, 1);
	if (ret) {
		if (ret > 0)
			ret = -ENOENT;
		btrfs_abort_transaction(trans, ret);
		btrfs_end_transaction(trans);
		goto out;
	}

	ret = btrfs_del_item(trans, root, path);
	if (ret) {
		btrfs_abort_transaction(trans, ret);
		btrfs_end_transaction(trans);
	}

out:
	btrfs_free_path(path);
	if (!ret)
		ret = btrfs_commit_transaction(trans);
	return ret;
}

/*
 * Verify that @num_devices satisfies the RAID profile constraints in the whole
 * filesystem. It's up to the caller to adjust that number regarding eg. device
 * replace.
 */
static int btrfs_check_raid_min_devices(struct btrfs_fs_info *fs_info,
		u64 num_devices)
{
	u64 all_avail;
	unsigned seq;
	int i;

	do {
		seq = read_seqbegin(&fs_info->profiles_lock);

		all_avail = fs_info->avail_data_alloc_bits |
			    fs_info->avail_system_alloc_bits |
			    fs_info->avail_metadata_alloc_bits;
	} while (read_seqretry(&fs_info->profiles_lock, seq));

	for (i = 0; i < BTRFS_NR_RAID_TYPES; i++) {
		if (!(all_avail & btrfs_raid_array[i].bg_flag))
			continue;

<<<<<<< HEAD
		if (num_devices < btrfs_raid_array[i].devs_min) {
			int ret = btrfs_raid_array[i].mindev_error;

			if (ret)
				return ret;
		}
=======
		if (num_devices < btrfs_raid_array[i].devs_min)
			return btrfs_raid_array[i].mindev_error;
>>>>>>> upstream/android-13
	}

	return 0;
}

static struct btrfs_device * btrfs_find_next_active_device(
		struct btrfs_fs_devices *fs_devs, struct btrfs_device *device)
{
	struct btrfs_device *next_device;

	list_for_each_entry(next_device, &fs_devs->devices, dev_list) {
		if (next_device != device &&
		    !test_bit(BTRFS_DEV_STATE_MISSING, &next_device->dev_state)
		    && next_device->bdev)
			return next_device;
	}

	return NULL;
}

/*
<<<<<<< HEAD
 * Helper function to check if the given device is part of s_bdev / latest_bdev
=======
 * Helper function to check if the given device is part of s_bdev / latest_dev
>>>>>>> upstream/android-13
 * and replace it with the provided or the next active device, in the context
 * where this function called, there should be always be another device (or
 * this_dev) which is active.
 */
<<<<<<< HEAD
void btrfs_assign_next_active_device(struct btrfs_device *device,
				     struct btrfs_device *this_dev)
{
	struct btrfs_fs_info *fs_info = device->fs_info;
	struct btrfs_device *next_device;

	if (this_dev)
		next_device = this_dev;
	else
		next_device = btrfs_find_next_active_device(fs_info->fs_devices,
								device);
=======
void __cold btrfs_assign_next_active_device(struct btrfs_device *device,
					    struct btrfs_device *next_device)
{
	struct btrfs_fs_info *fs_info = device->fs_info;

	if (!next_device)
		next_device = btrfs_find_next_active_device(fs_info->fs_devices,
							    device);
>>>>>>> upstream/android-13
	ASSERT(next_device);

	if (fs_info->sb->s_bdev &&
			(fs_info->sb->s_bdev == device->bdev))
		fs_info->sb->s_bdev = next_device->bdev;

<<<<<<< HEAD
	if (fs_info->fs_devices->latest_bdev == device->bdev)
		fs_info->fs_devices->latest_bdev = next_device->bdev;
}

int btrfs_rm_device(struct btrfs_fs_info *fs_info, const char *device_path,
		u64 devid)
=======
	if (fs_info->fs_devices->latest_dev->bdev == device->bdev)
		fs_info->fs_devices->latest_dev = next_device;
}

/*
 * Return btrfs_fs_devices::num_devices excluding the device that's being
 * currently replaced.
 */
static u64 btrfs_num_devices(struct btrfs_fs_info *fs_info)
{
	u64 num_devices = fs_info->fs_devices->num_devices;

	down_read(&fs_info->dev_replace.rwsem);
	if (btrfs_dev_replace_is_ongoing(&fs_info->dev_replace)) {
		ASSERT(num_devices > 1);
		num_devices--;
	}
	up_read(&fs_info->dev_replace.rwsem);

	return num_devices;
}

void btrfs_scratch_superblocks(struct btrfs_fs_info *fs_info,
			       struct block_device *bdev,
			       const char *device_path)
{
	struct btrfs_super_block *disk_super;
	int copy_num;

	if (!bdev)
		return;

	for (copy_num = 0; copy_num < BTRFS_SUPER_MIRROR_MAX; copy_num++) {
		struct page *page;
		int ret;

		disk_super = btrfs_read_dev_one_super(bdev, copy_num);
		if (IS_ERR(disk_super))
			continue;

		if (bdev_is_zoned(bdev)) {
			btrfs_reset_sb_log_zones(bdev, copy_num);
			continue;
		}

		memset(&disk_super->magic, 0, sizeof(disk_super->magic));

		page = virt_to_page(disk_super);
		set_page_dirty(page);
		lock_page(page);
		/* write_on_page() unlocks the page */
		ret = write_one_page(page);
		if (ret)
			btrfs_warn(fs_info,
				"error clearing superblock number %d (%d)",
				copy_num, ret);
		btrfs_release_disk_super(disk_super);

	}

	/* Notify udev that device has changed */
	btrfs_kobject_uevent(bdev, KOBJ_CHANGE);

	/* Update ctime/mtime for device path for libblkid */
	update_dev_time(device_path);
}

int btrfs_rm_device(struct btrfs_fs_info *fs_info, const char *device_path,
		    u64 devid, struct block_device **bdev, fmode_t *mode)
>>>>>>> upstream/android-13
{
	struct btrfs_device *device;
	struct btrfs_fs_devices *cur_devices;
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	u64 num_devices;
	int ret = 0;

<<<<<<< HEAD
	mutex_lock(&uuid_mutex);

	num_devices = fs_devices->num_devices;
	btrfs_dev_replace_read_lock(&fs_info->dev_replace);
	if (btrfs_dev_replace_is_ongoing(&fs_info->dev_replace)) {
		WARN_ON(num_devices < 1);
		num_devices--;
	}
	btrfs_dev_replace_read_unlock(&fs_info->dev_replace);
=======
	/*
	 * The device list in fs_devices is accessed without locks (neither
	 * uuid_mutex nor device_list_mutex) as it won't change on a mounted
	 * filesystem and another device rm cannot run.
	 */
	num_devices = btrfs_num_devices(fs_info);
>>>>>>> upstream/android-13

	ret = btrfs_check_raid_min_devices(fs_info, num_devices - 1);
	if (ret)
		goto out;

<<<<<<< HEAD
	ret = btrfs_find_device_by_devspec(fs_info, devid, device_path,
					   &device);
	if (ret)
		goto out;
=======
	device = btrfs_find_device_by_devspec(fs_info, devid, device_path);

	if (IS_ERR(device)) {
		if (PTR_ERR(device) == -ENOENT &&
		    device_path && strcmp(device_path, "missing") == 0)
			ret = BTRFS_ERROR_DEV_MISSING_NOT_FOUND;
		else
			ret = PTR_ERR(device);
		goto out;
	}

	if (btrfs_pinned_by_swapfile(fs_info, device)) {
		btrfs_warn_in_rcu(fs_info,
		  "cannot remove device %s (devid %llu) due to active swapfile",
				  rcu_str_deref(device->name), device->devid);
		ret = -ETXTBSY;
		goto out;
	}
>>>>>>> upstream/android-13

	if (test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state)) {
		ret = BTRFS_ERROR_DEV_TGT_REPLACE;
		goto out;
	}

	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state) &&
	    fs_info->fs_devices->rw_devices == 1) {
		ret = BTRFS_ERROR_DEV_ONLY_WRITABLE;
		goto out;
	}

	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state)) {
		mutex_lock(&fs_info->chunk_mutex);
		list_del_init(&device->dev_alloc_list);
		device->fs_devices->rw_devices--;
		mutex_unlock(&fs_info->chunk_mutex);
	}

<<<<<<< HEAD
	mutex_unlock(&uuid_mutex);
	ret = btrfs_shrink_device(device, 0);
	mutex_lock(&uuid_mutex);
=======
	ret = btrfs_shrink_device(device, 0);
	if (!ret)
		btrfs_reada_remove_dev(device);
>>>>>>> upstream/android-13
	if (ret)
		goto error_undo;

	/*
	 * TODO: the superblock still includes this device in its num_devices
	 * counter although write_all_supers() is not locked out. This
	 * could give a filesystem state which requires a degraded mount.
	 */
<<<<<<< HEAD
	ret = btrfs_rm_dev_item(fs_info, device);
=======
	ret = btrfs_rm_dev_item(device);
>>>>>>> upstream/android-13
	if (ret)
		goto error_undo;

	clear_bit(BTRFS_DEV_STATE_IN_FS_METADATA, &device->dev_state);
<<<<<<< HEAD
	btrfs_scrub_cancel_dev(fs_info, device);
=======
	btrfs_scrub_cancel_dev(device);
>>>>>>> upstream/android-13

	/*
	 * the device list mutex makes sure that we don't change
	 * the device list while someone else is writing out all
	 * the device supers. Whoever is writing all supers, should
	 * lock the device list mutex before getting the number of
	 * devices in the super block (super_copy). Conversely,
	 * whoever updates the number of devices in the super block
	 * (super_copy) should hold the device list mutex.
	 */

	/*
	 * In normal cases the cur_devices == fs_devices. But in case
	 * of deleting a seed device, the cur_devices should point to
	 * its own fs_devices listed under the fs_devices->seed.
	 */
	cur_devices = device->fs_devices;
	mutex_lock(&fs_devices->device_list_mutex);
	list_del_rcu(&device->dev_list);

	cur_devices->num_devices--;
	cur_devices->total_devices--;
	/* Update total_devices of the parent fs_devices if it's seed */
	if (cur_devices != fs_devices)
		fs_devices->total_devices--;

	if (test_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state))
		cur_devices->missing_devices--;

	btrfs_assign_next_active_device(device, NULL);

	if (device->bdev) {
		cur_devices->open_devices--;
		/* remove sysfs entry */
<<<<<<< HEAD
		btrfs_sysfs_rm_device_link(fs_devices, device);
=======
		btrfs_sysfs_remove_device(device);
>>>>>>> upstream/android-13
	}

	num_devices = btrfs_super_num_devices(fs_info->super_copy) - 1;
	btrfs_set_super_num_devices(fs_info->super_copy, num_devices);
	mutex_unlock(&fs_devices->device_list_mutex);

	/*
<<<<<<< HEAD
	 * at this point, the device is zero sized and detached from
	 * the devices list.  All that's left is to zero out the old
	 * supers and free the device.
	 */
	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state))
		btrfs_scratch_superblocks(device->bdev, device->name->str);

	btrfs_close_bdev(device);
	call_rcu(&device->rcu, free_device_rcu);

	if (cur_devices->open_devices == 0) {
		while (fs_devices) {
			if (fs_devices->seed == cur_devices) {
				fs_devices->seed = cur_devices->seed;
				break;
			}
			fs_devices = fs_devices->seed;
		}
		cur_devices->seed = NULL;
=======
	 * At this point, the device is zero sized and detached from the
	 * devices list.  All that's left is to zero out the old supers and
	 * free the device.
	 *
	 * We cannot call btrfs_close_bdev() here because we're holding the sb
	 * write lock, and blkdev_put() will pull in the ->open_mutex on the
	 * block device and it's dependencies.  Instead just flush the device
	 * and let the caller do the final blkdev_put.
	 */
	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state)) {
		btrfs_scratch_superblocks(fs_info, device->bdev,
					  device->name->str);
		if (device->bdev) {
			sync_blockdev(device->bdev);
			invalidate_bdev(device->bdev);
		}
	}

	*bdev = device->bdev;
	*mode = device->mode;
	synchronize_rcu();
	btrfs_free_device(device);

	if (cur_devices->open_devices == 0) {
		list_del_init(&cur_devices->seed_list);
>>>>>>> upstream/android-13
		close_fs_devices(cur_devices);
		free_fs_devices(cur_devices);
	}

out:
<<<<<<< HEAD
	mutex_unlock(&uuid_mutex);
	return ret;

error_undo:
=======
	return ret;

error_undo:
	btrfs_reada_undo_remove_dev(device);
>>>>>>> upstream/android-13
	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state)) {
		mutex_lock(&fs_info->chunk_mutex);
		list_add(&device->dev_alloc_list,
			 &fs_devices->alloc_list);
		device->fs_devices->rw_devices++;
		mutex_unlock(&fs_info->chunk_mutex);
	}
	goto out;
}

void btrfs_rm_dev_replace_remove_srcdev(struct btrfs_device *srcdev)
{
	struct btrfs_fs_devices *fs_devices;

	lockdep_assert_held(&srcdev->fs_info->fs_devices->device_list_mutex);

	/*
	 * in case of fs with no seed, srcdev->fs_devices will point
	 * to fs_devices of fs_info. However when the dev being replaced is
	 * a seed dev it will point to the seed's local fs_devices. In short
	 * srcdev will have its correct fs_devices in both the cases.
	 */
	fs_devices = srcdev->fs_devices;

	list_del_rcu(&srcdev->dev_list);
	list_del(&srcdev->dev_alloc_list);
	fs_devices->num_devices--;
	if (test_bit(BTRFS_DEV_STATE_MISSING, &srcdev->dev_state))
		fs_devices->missing_devices--;

	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &srcdev->dev_state))
		fs_devices->rw_devices--;

	if (srcdev->bdev)
		fs_devices->open_devices--;
}

<<<<<<< HEAD
void btrfs_rm_dev_replace_free_srcdev(struct btrfs_fs_info *fs_info,
				      struct btrfs_device *srcdev)
{
	struct btrfs_fs_devices *fs_devices = srcdev->fs_devices;

	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &srcdev->dev_state)) {
		/* zero out the old super if it is writable */
		btrfs_scratch_superblocks(srcdev->bdev, srcdev->name->str);
	}

	btrfs_close_bdev(srcdev);
	call_rcu(&srcdev->rcu, free_device_rcu);

	/* if this is no devs we rather delete the fs_devices */
	if (!fs_devices->num_devices) {
		struct btrfs_fs_devices *tmp_fs_devices;

=======
void btrfs_rm_dev_replace_free_srcdev(struct btrfs_device *srcdev)
{
	struct btrfs_fs_devices *fs_devices = srcdev->fs_devices;

	mutex_lock(&uuid_mutex);

	btrfs_close_bdev(srcdev);
	synchronize_rcu();
	btrfs_free_device(srcdev);

	/* if this is no devs we rather delete the fs_devices */
	if (!fs_devices->num_devices) {
>>>>>>> upstream/android-13
		/*
		 * On a mounted FS, num_devices can't be zero unless it's a
		 * seed. In case of a seed device being replaced, the replace
		 * target added to the sprout FS, so there will be no more
		 * device left under the seed FS.
		 */
		ASSERT(fs_devices->seeding);

<<<<<<< HEAD
		tmp_fs_devices = fs_info->fs_devices;
		while (tmp_fs_devices) {
			if (tmp_fs_devices->seed == fs_devices) {
				tmp_fs_devices->seed = fs_devices->seed;
				break;
			}
			tmp_fs_devices = tmp_fs_devices->seed;
		}
		fs_devices->seed = NULL;
		close_fs_devices(fs_devices);
		free_fs_devices(fs_devices);
	}
=======
		list_del_init(&fs_devices->seed_list);
		close_fs_devices(fs_devices);
		free_fs_devices(fs_devices);
	}
	mutex_unlock(&uuid_mutex);
>>>>>>> upstream/android-13
}

void btrfs_destroy_dev_replace_tgtdev(struct btrfs_device *tgtdev)
{
	struct btrfs_fs_devices *fs_devices = tgtdev->fs_info->fs_devices;

<<<<<<< HEAD
	WARN_ON(!tgtdev);
	mutex_lock(&fs_devices->device_list_mutex);

	btrfs_sysfs_rm_device_link(fs_devices, tgtdev);
=======
	mutex_lock(&fs_devices->device_list_mutex);

	btrfs_sysfs_remove_device(tgtdev);
>>>>>>> upstream/android-13

	if (tgtdev->bdev)
		fs_devices->open_devices--;

	fs_devices->num_devices--;

	btrfs_assign_next_active_device(tgtdev, NULL);

	list_del_rcu(&tgtdev->dev_list);

	mutex_unlock(&fs_devices->device_list_mutex);

<<<<<<< HEAD
	/*
	 * The update_dev_time() with in btrfs_scratch_superblocks()
	 * may lead to a call to btrfs_show_devname() which will try
	 * to hold device_list_mutex. And here this device
	 * is already out of device list, so we don't have to hold
	 * the device_list_mutex lock.
	 */
	btrfs_scratch_superblocks(tgtdev->bdev, tgtdev->name->str);

	btrfs_close_bdev(tgtdev);
	call_rcu(&tgtdev->rcu, free_device_rcu);
}

static int btrfs_find_device_by_path(struct btrfs_fs_info *fs_info,
				     const char *device_path,
				     struct btrfs_device **device)
=======
	btrfs_scratch_superblocks(tgtdev->fs_info, tgtdev->bdev,
				  tgtdev->name->str);

	btrfs_close_bdev(tgtdev);
	synchronize_rcu();
	btrfs_free_device(tgtdev);
}

static struct btrfs_device *btrfs_find_device_by_path(
		struct btrfs_fs_info *fs_info, const char *device_path)
>>>>>>> upstream/android-13
{
	int ret = 0;
	struct btrfs_super_block *disk_super;
	u64 devid;
	u8 *dev_uuid;
	struct block_device *bdev;
<<<<<<< HEAD
	struct buffer_head *bh;

	*device = NULL;
	ret = btrfs_get_bdev_and_sb(device_path, FMODE_READ,
				    fs_info->bdev_holder, 0, &bdev, &bh);
	if (ret)
		return ret;
	disk_super = (struct btrfs_super_block *)bh->b_data;
	devid = btrfs_stack_device_id(&disk_super->dev_item);
	dev_uuid = disk_super->dev_item.uuid;
	*device = btrfs_find_device(fs_info->fs_devices, devid, dev_uuid,
				    disk_super->fsid, true);
	brelse(bh);
	if (!*device)
		ret = -ENOENT;
	blkdev_put(bdev, FMODE_READ);
	return ret;
}

int btrfs_find_device_missing_or_by_path(struct btrfs_fs_info *fs_info,
					 const char *device_path,
					 struct btrfs_device **device)
{
	*device = NULL;
	if (strcmp(device_path, "missing") == 0) {
		struct list_head *devices;
		struct btrfs_device *tmp;

		devices = &fs_info->fs_devices->devices;
		list_for_each_entry(tmp, devices, dev_list) {
			if (test_bit(BTRFS_DEV_STATE_IN_FS_METADATA,
					&tmp->dev_state) && !tmp->bdev) {
				*device = tmp;
				break;
			}
		}

		if (!*device)
			return BTRFS_ERROR_DEV_MISSING_NOT_FOUND;

		return 0;
	} else {
		return btrfs_find_device_by_path(fs_info, device_path, device);
	}
=======
	struct btrfs_device *device;

	ret = btrfs_get_bdev_and_sb(device_path, FMODE_READ,
				    fs_info->bdev_holder, 0, &bdev, &disk_super);
	if (ret)
		return ERR_PTR(ret);

	devid = btrfs_stack_device_id(&disk_super->dev_item);
	dev_uuid = disk_super->dev_item.uuid;
	if (btrfs_fs_incompat(fs_info, METADATA_UUID))
		device = btrfs_find_device(fs_info->fs_devices, devid, dev_uuid,
					   disk_super->metadata_uuid);
	else
		device = btrfs_find_device(fs_info->fs_devices, devid, dev_uuid,
					   disk_super->fsid);

	btrfs_release_disk_super(disk_super);
	if (!device)
		device = ERR_PTR(-ENOENT);
	blkdev_put(bdev, FMODE_READ);
	return device;
>>>>>>> upstream/android-13
}

/*
 * Lookup a device given by device id, or the path if the id is 0.
 */
<<<<<<< HEAD
int btrfs_find_device_by_devspec(struct btrfs_fs_info *fs_info, u64 devid,
				 const char *devpath,
				 struct btrfs_device **device)
{
	int ret;

	if (devid) {
		ret = 0;
		*device = btrfs_find_device(fs_info->fs_devices, devid,
					    NULL, NULL, true);
		if (!*device)
			ret = -ENOENT;
	} else {
		if (!devpath || !devpath[0])
			return -EINVAL;

		ret = btrfs_find_device_missing_or_by_path(fs_info, devpath,
							   device);
	}
	return ret;
=======
struct btrfs_device *btrfs_find_device_by_devspec(
		struct btrfs_fs_info *fs_info, u64 devid,
		const char *device_path)
{
	struct btrfs_device *device;

	if (devid) {
		device = btrfs_find_device(fs_info->fs_devices, devid, NULL,
					   NULL);
		if (!device)
			return ERR_PTR(-ENOENT);
		return device;
	}

	if (!device_path || !device_path[0])
		return ERR_PTR(-EINVAL);

	if (strcmp(device_path, "missing") == 0) {
		/* Find first missing device */
		list_for_each_entry(device, &fs_info->fs_devices->devices,
				    dev_list) {
			if (test_bit(BTRFS_DEV_STATE_IN_FS_METADATA,
				     &device->dev_state) && !device->bdev)
				return device;
		}
		return ERR_PTR(-ENOENT);
	}

	return btrfs_find_device_by_path(fs_info, device_path);
>>>>>>> upstream/android-13
}

/*
 * does all the dirty work required for changing file system's UUID.
 */
static int btrfs_prepare_sprout(struct btrfs_fs_info *fs_info)
{
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	struct btrfs_fs_devices *old_devices;
	struct btrfs_fs_devices *seed_devices;
	struct btrfs_super_block *disk_super = fs_info->super_copy;
	struct btrfs_device *device;
	u64 super_flags;

	lockdep_assert_held(&uuid_mutex);
	if (!fs_devices->seeding)
		return -EINVAL;

<<<<<<< HEAD
	seed_devices = alloc_fs_devices(NULL);
	if (IS_ERR(seed_devices))
		return PTR_ERR(seed_devices);

=======
	/*
	 * Private copy of the seed devices, anchored at
	 * fs_info->fs_devices->seed_list
	 */
	seed_devices = alloc_fs_devices(NULL, NULL);
	if (IS_ERR(seed_devices))
		return PTR_ERR(seed_devices);

	/*
	 * It's necessary to retain a copy of the original seed fs_devices in
	 * fs_uuids so that filesystems which have been seeded can successfully
	 * reference the seed device from open_seed_devices. This also supports
	 * multiple fs seed.
	 */
>>>>>>> upstream/android-13
	old_devices = clone_fs_devices(fs_devices);
	if (IS_ERR(old_devices)) {
		kfree(seed_devices);
		return PTR_ERR(old_devices);
	}

	list_add(&old_devices->fs_list, &fs_uuids);

	memcpy(seed_devices, fs_devices, sizeof(*seed_devices));
	seed_devices->opened = 1;
	INIT_LIST_HEAD(&seed_devices->devices);
	INIT_LIST_HEAD(&seed_devices->alloc_list);
	mutex_init(&seed_devices->device_list_mutex);

	mutex_lock(&fs_devices->device_list_mutex);
	list_splice_init_rcu(&fs_devices->devices, &seed_devices->devices,
			      synchronize_rcu);
	list_for_each_entry(device, &seed_devices->devices, dev_list)
		device->fs_devices = seed_devices;

<<<<<<< HEAD
	mutex_lock(&fs_info->chunk_mutex);
	list_splice_init(&fs_devices->alloc_list, &seed_devices->alloc_list);
	mutex_unlock(&fs_info->chunk_mutex);

	fs_devices->seeding = 0;
	fs_devices->num_devices = 0;
	fs_devices->open_devices = 0;
	fs_devices->missing_devices = 0;
	fs_devices->rotating = 0;
	fs_devices->seed = seed_devices;

	generate_random_uuid(fs_devices->fsid);
	memcpy(fs_info->fsid, fs_devices->fsid, BTRFS_FSID_SIZE);
=======
	fs_devices->seeding = false;
	fs_devices->num_devices = 0;
	fs_devices->open_devices = 0;
	fs_devices->missing_devices = 0;
	fs_devices->rotating = false;
	list_add(&seed_devices->seed_list, &fs_devices->seed_list);

	generate_random_uuid(fs_devices->fsid);
	memcpy(fs_devices->metadata_uuid, fs_devices->fsid, BTRFS_FSID_SIZE);
>>>>>>> upstream/android-13
	memcpy(disk_super->fsid, fs_devices->fsid, BTRFS_FSID_SIZE);
	mutex_unlock(&fs_devices->device_list_mutex);

	super_flags = btrfs_super_flags(disk_super) &
		      ~BTRFS_SUPER_FLAG_SEEDING;
	btrfs_set_super_flags(disk_super, super_flags);

	return 0;
}

/*
 * Store the expected generation for seed devices in device items.
 */
<<<<<<< HEAD
static int btrfs_finish_sprout(struct btrfs_trans_handle *trans,
			       struct btrfs_fs_info *fs_info)
{
=======
static int btrfs_finish_sprout(struct btrfs_trans_handle *trans)
{
	struct btrfs_fs_info *fs_info = trans->fs_info;
>>>>>>> upstream/android-13
	struct btrfs_root *root = fs_info->chunk_root;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_dev_item *dev_item;
	struct btrfs_device *device;
	struct btrfs_key key;
	u8 fs_uuid[BTRFS_FSID_SIZE];
	u8 dev_uuid[BTRFS_UUID_SIZE];
	u64 devid;
	int ret;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = BTRFS_DEV_ITEMS_OBJECTID;
	key.offset = 0;
	key.type = BTRFS_DEV_ITEM_KEY;

	while (1) {
		ret = btrfs_search_slot(trans, root, &key, path, 0, 1);
		if (ret < 0)
			goto error;

		leaf = path->nodes[0];
next_slot:
		if (path->slots[0] >= btrfs_header_nritems(leaf)) {
			ret = btrfs_next_leaf(root, path);
			if (ret > 0)
				break;
			if (ret < 0)
				goto error;
			leaf = path->nodes[0];
			btrfs_item_key_to_cpu(leaf, &key, path->slots[0]);
			btrfs_release_path(path);
			continue;
		}

		btrfs_item_key_to_cpu(leaf, &key, path->slots[0]);
		if (key.objectid != BTRFS_DEV_ITEMS_OBJECTID ||
		    key.type != BTRFS_DEV_ITEM_KEY)
			break;

		dev_item = btrfs_item_ptr(leaf, path->slots[0],
					  struct btrfs_dev_item);
		devid = btrfs_device_id(leaf, dev_item);
		read_extent_buffer(leaf, dev_uuid, btrfs_device_uuid(dev_item),
				   BTRFS_UUID_SIZE);
		read_extent_buffer(leaf, fs_uuid, btrfs_device_fsid(dev_item),
				   BTRFS_FSID_SIZE);
		device = btrfs_find_device(fs_info->fs_devices, devid, dev_uuid,
<<<<<<< HEAD
					   fs_uuid, true);
=======
					   fs_uuid);
>>>>>>> upstream/android-13
		BUG_ON(!device); /* Logic error */

		if (device->fs_devices->seeding) {
			btrfs_set_device_generation(leaf, dev_item,
						    device->generation);
			btrfs_mark_buffer_dirty(leaf);
		}

		path->slots[0]++;
		goto next_slot;
	}
	ret = 0;
error:
	btrfs_free_path(path);
	return ret;
}

int btrfs_init_new_device(struct btrfs_fs_info *fs_info, const char *device_path)
{
	struct btrfs_root *root = fs_info->dev_root;
	struct request_queue *q;
	struct btrfs_trans_handle *trans;
	struct btrfs_device *device;
	struct block_device *bdev;
	struct super_block *sb = fs_info->sb;
	struct rcu_string *name;
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	u64 orig_super_total_bytes;
	u64 orig_super_num_devices;
	int seeding_dev = 0;
	int ret = 0;
<<<<<<< HEAD
	bool unlocked = false;
=======
	bool locked = false;
>>>>>>> upstream/android-13

	if (sb_rdonly(sb) && !fs_devices->seeding)
		return -EROFS;

	bdev = blkdev_get_by_path(device_path, FMODE_WRITE | FMODE_EXCL,
				  fs_info->bdev_holder);
	if (IS_ERR(bdev))
		return PTR_ERR(bdev);

<<<<<<< HEAD
=======
	if (!btrfs_check_device_zone_type(fs_info, bdev)) {
		ret = -EINVAL;
		goto error;
	}

>>>>>>> upstream/android-13
	if (fs_devices->seeding) {
		seeding_dev = 1;
		down_write(&sb->s_umount);
		mutex_lock(&uuid_mutex);
<<<<<<< HEAD
	}

	filemap_write_and_wait(bdev->bd_inode->i_mapping);

	mutex_lock(&fs_devices->device_list_mutex);
	list_for_each_entry(device, &fs_devices->devices, dev_list) {
		if (device->bdev == bdev) {
			ret = -EEXIST;
			mutex_unlock(
				&fs_devices->device_list_mutex);
			goto error;
		}
	}
	mutex_unlock(&fs_devices->device_list_mutex);
=======
		locked = true;
	}

	sync_blockdev(bdev);

	rcu_read_lock();
	list_for_each_entry_rcu(device, &fs_devices->devices, dev_list) {
		if (device->bdev == bdev) {
			ret = -EEXIST;
			rcu_read_unlock();
			goto error;
		}
	}
	rcu_read_unlock();
>>>>>>> upstream/android-13

	device = btrfs_alloc_device(fs_info, NULL, NULL);
	if (IS_ERR(device)) {
		/* we can safely leave the fs_devices entry around */
		ret = PTR_ERR(device);
		goto error;
	}

	name = rcu_string_strdup(device_path, GFP_KERNEL);
	if (!name) {
		ret = -ENOMEM;
		goto error_free_device;
	}
	rcu_assign_pointer(device->name, name);

<<<<<<< HEAD
	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans)) {
		ret = PTR_ERR(trans);
		goto error_free_device;
=======
	device->fs_info = fs_info;
	device->bdev = bdev;

	ret = btrfs_get_dev_zone_info(device, false);
	if (ret)
		goto error_free_device;

	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans)) {
		ret = PTR_ERR(trans);
		goto error_free_zone;
>>>>>>> upstream/android-13
	}

	q = bdev_get_queue(bdev);
	set_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state);
	device->generation = trans->transid;
	device->io_width = fs_info->sectorsize;
	device->io_align = fs_info->sectorsize;
	device->sector_size = fs_info->sectorsize;
	device->total_bytes = round_down(i_size_read(bdev->bd_inode),
					 fs_info->sectorsize);
	device->disk_total_bytes = device->total_bytes;
	device->commit_total_bytes = device->total_bytes;
<<<<<<< HEAD
	device->fs_info = fs_info;
	device->bdev = bdev;
=======
>>>>>>> upstream/android-13
	set_bit(BTRFS_DEV_STATE_IN_FS_METADATA, &device->dev_state);
	clear_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state);
	device->mode = FMODE_EXCL;
	device->dev_stats_valid = 1;
	set_blocksize(device->bdev, BTRFS_BDEV_BLOCKSIZE);

	if (seeding_dev) {
<<<<<<< HEAD
		sb->s_flags &= ~SB_RDONLY;
=======
		btrfs_clear_sb_rdonly(sb);
>>>>>>> upstream/android-13
		ret = btrfs_prepare_sprout(fs_info);
		if (ret) {
			btrfs_abort_transaction(trans, ret);
			goto error_trans;
		}
<<<<<<< HEAD
=======
		btrfs_assign_next_active_device(fs_info->fs_devices->latest_dev,
						device);
>>>>>>> upstream/android-13
	}

	device->fs_devices = fs_devices;

	mutex_lock(&fs_devices->device_list_mutex);
	mutex_lock(&fs_info->chunk_mutex);
	list_add_rcu(&device->dev_list, &fs_devices->devices);
	list_add(&device->dev_alloc_list, &fs_devices->alloc_list);
	fs_devices->num_devices++;
	fs_devices->open_devices++;
	fs_devices->rw_devices++;
	fs_devices->total_devices++;
	fs_devices->total_rw_bytes += device->total_bytes;

	atomic64_add(device->total_bytes, &fs_info->free_chunk_space);

	if (!blk_queue_nonrot(q))
<<<<<<< HEAD
		fs_devices->rotating = 1;
=======
		fs_devices->rotating = true;
>>>>>>> upstream/android-13

	orig_super_total_bytes = btrfs_super_total_bytes(fs_info->super_copy);
	btrfs_set_super_total_bytes(fs_info->super_copy,
		round_down(orig_super_total_bytes + device->total_bytes,
			   fs_info->sectorsize));

	orig_super_num_devices = btrfs_super_num_devices(fs_info->super_copy);
	btrfs_set_super_num_devices(fs_info->super_copy,
				    orig_super_num_devices + 1);

	/*
	 * we've got more storage, clear any full flags on the space
	 * infos
	 */
	btrfs_clear_space_info_full(fs_info);

	mutex_unlock(&fs_info->chunk_mutex);

	/* Add sysfs device entry */
<<<<<<< HEAD
	btrfs_sysfs_add_device_link(fs_devices, device);
=======
	btrfs_sysfs_add_device(device);
>>>>>>> upstream/android-13

	mutex_unlock(&fs_devices->device_list_mutex);

	if (seeding_dev) {
		mutex_lock(&fs_info->chunk_mutex);
<<<<<<< HEAD
		ret = init_first_rw_device(trans, fs_info);
=======
		ret = init_first_rw_device(trans);
>>>>>>> upstream/android-13
		mutex_unlock(&fs_info->chunk_mutex);
		if (ret) {
			btrfs_abort_transaction(trans, ret);
			goto error_sysfs;
		}
	}

	ret = btrfs_add_dev_item(trans, device);
	if (ret) {
		btrfs_abort_transaction(trans, ret);
		goto error_sysfs;
	}

	if (seeding_dev) {
<<<<<<< HEAD
		char fsid_buf[BTRFS_UUID_UNPARSED_SIZE];

		ret = btrfs_finish_sprout(trans, fs_info);
=======
		ret = btrfs_finish_sprout(trans);
>>>>>>> upstream/android-13
		if (ret) {
			btrfs_abort_transaction(trans, ret);
			goto error_sysfs;
		}

<<<<<<< HEAD
		/* Sprouting would change fsid of the mounted root,
		 * so rename the fsid on the sysfs
		 */
		snprintf(fsid_buf, BTRFS_UUID_UNPARSED_SIZE, "%pU",
						fs_info->fsid);
		if (kobject_rename(&fs_devices->fsid_kobj, fsid_buf))
			btrfs_warn(fs_info,
				   "sysfs: failed to create fsid for sprout");
=======
		/*
		 * fs_devices now represents the newly sprouted filesystem and
		 * its fsid has been changed by btrfs_prepare_sprout
		 */
		btrfs_sysfs_update_sprout_fsid(fs_devices);
>>>>>>> upstream/android-13
	}

	ret = btrfs_commit_transaction(trans);

	if (seeding_dev) {
		mutex_unlock(&uuid_mutex);
		up_write(&sb->s_umount);
<<<<<<< HEAD
		unlocked = true;
=======
		locked = false;
>>>>>>> upstream/android-13

		if (ret) /* transaction commit */
			return ret;

		ret = btrfs_relocate_sys_chunks(fs_info);
		if (ret < 0)
			btrfs_handle_fs_error(fs_info, ret,
				    "Failed to relocate sys chunks after device initialization. This can be fixed using the \"btrfs balance\" command.");
		trans = btrfs_attach_transaction(root);
		if (IS_ERR(trans)) {
			if (PTR_ERR(trans) == -ENOENT)
				return 0;
			ret = PTR_ERR(trans);
			trans = NULL;
			goto error_sysfs;
		}
		ret = btrfs_commit_transaction(trans);
	}

<<<<<<< HEAD
	/* Update ctime/mtime for libblkid */
	update_dev_time(device_path);
	return ret;

error_sysfs:
	btrfs_sysfs_rm_device_link(fs_devices, device);
=======
	/*
	 * Now that we have written a new super block to this device, check all
	 * other fs_devices list if device_path alienates any other scanned
	 * device.
	 * We can ignore the return value as it typically returns -EINVAL and
	 * only succeeds if the device was an alien.
	 */
	btrfs_forget_devices(device_path);

	/* Update ctime/mtime for blkid or udev */
	update_dev_time(device_path);

	return ret;

error_sysfs:
	btrfs_sysfs_remove_device(device);
>>>>>>> upstream/android-13
	mutex_lock(&fs_info->fs_devices->device_list_mutex);
	mutex_lock(&fs_info->chunk_mutex);
	list_del_rcu(&device->dev_list);
	list_del(&device->dev_alloc_list);
	fs_info->fs_devices->num_devices--;
	fs_info->fs_devices->open_devices--;
	fs_info->fs_devices->rw_devices--;
	fs_info->fs_devices->total_devices--;
	fs_info->fs_devices->total_rw_bytes -= device->total_bytes;
	atomic64_sub(device->total_bytes, &fs_info->free_chunk_space);
	btrfs_set_super_total_bytes(fs_info->super_copy,
				    orig_super_total_bytes);
	btrfs_set_super_num_devices(fs_info->super_copy,
				    orig_super_num_devices);
	mutex_unlock(&fs_info->chunk_mutex);
	mutex_unlock(&fs_info->fs_devices->device_list_mutex);
error_trans:
	if (seeding_dev)
<<<<<<< HEAD
		sb->s_flags |= SB_RDONLY;
	if (trans)
		btrfs_end_transaction(trans);
=======
		btrfs_set_sb_rdonly(sb);
	if (trans)
		btrfs_end_transaction(trans);
error_free_zone:
	btrfs_destroy_dev_zone_info(device);
>>>>>>> upstream/android-13
error_free_device:
	btrfs_free_device(device);
error:
	blkdev_put(bdev, FMODE_EXCL);
<<<<<<< HEAD
	if (seeding_dev && !unlocked) {
=======
	if (locked) {
>>>>>>> upstream/android-13
		mutex_unlock(&uuid_mutex);
		up_write(&sb->s_umount);
	}
	return ret;
}

static noinline int btrfs_update_device(struct btrfs_trans_handle *trans,
					struct btrfs_device *device)
{
	int ret;
	struct btrfs_path *path;
	struct btrfs_root *root = device->fs_info->chunk_root;
	struct btrfs_dev_item *dev_item;
	struct extent_buffer *leaf;
	struct btrfs_key key;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = BTRFS_DEV_ITEMS_OBJECTID;
	key.type = BTRFS_DEV_ITEM_KEY;
	key.offset = device->devid;

	ret = btrfs_search_slot(trans, root, &key, path, 0, 1);
	if (ret < 0)
		goto out;

	if (ret > 0) {
		ret = -ENOENT;
		goto out;
	}

	leaf = path->nodes[0];
	dev_item = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_dev_item);

	btrfs_set_device_id(leaf, dev_item, device->devid);
	btrfs_set_device_type(leaf, dev_item, device->type);
	btrfs_set_device_io_align(leaf, dev_item, device->io_align);
	btrfs_set_device_io_width(leaf, dev_item, device->io_width);
	btrfs_set_device_sector_size(leaf, dev_item, device->sector_size);
	btrfs_set_device_total_bytes(leaf, dev_item,
				     btrfs_device_get_disk_total_bytes(device));
	btrfs_set_device_bytes_used(leaf, dev_item,
				    btrfs_device_get_bytes_used(device));
	btrfs_mark_buffer_dirty(leaf);

out:
	btrfs_free_path(path);
	return ret;
}

int btrfs_grow_device(struct btrfs_trans_handle *trans,
		      struct btrfs_device *device, u64 new_size)
{
	struct btrfs_fs_info *fs_info = device->fs_info;
	struct btrfs_super_block *super_copy = fs_info->super_copy;
<<<<<<< HEAD
	struct btrfs_fs_devices *fs_devices;
=======
>>>>>>> upstream/android-13
	u64 old_total;
	u64 diff;

	if (!test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state))
		return -EACCES;

	new_size = round_down(new_size, fs_info->sectorsize);

	mutex_lock(&fs_info->chunk_mutex);
	old_total = btrfs_super_total_bytes(super_copy);
	diff = round_down(new_size - device->total_bytes, fs_info->sectorsize);

	if (new_size <= device->total_bytes ||
	    test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state)) {
		mutex_unlock(&fs_info->chunk_mutex);
		return -EINVAL;
	}

<<<<<<< HEAD
	fs_devices = fs_info->fs_devices;

=======
>>>>>>> upstream/android-13
	btrfs_set_super_total_bytes(super_copy,
			round_down(old_total + diff, fs_info->sectorsize));
	device->fs_devices->total_rw_bytes += diff;

	btrfs_device_set_total_bytes(device, new_size);
	btrfs_device_set_disk_total_bytes(device, new_size);
	btrfs_clear_space_info_full(device->fs_info);
<<<<<<< HEAD
	if (list_empty(&device->resized_list))
		list_add_tail(&device->resized_list,
			      &fs_devices->resized_devices);
=======
	if (list_empty(&device->post_commit_list))
		list_add_tail(&device->post_commit_list,
			      &trans->transaction->dev_update_list);
>>>>>>> upstream/android-13
	mutex_unlock(&fs_info->chunk_mutex);

	return btrfs_update_device(trans, device);
}

static int btrfs_free_chunk(struct btrfs_trans_handle *trans, u64 chunk_offset)
{
	struct btrfs_fs_info *fs_info = trans->fs_info;
	struct btrfs_root *root = fs_info->chunk_root;
	int ret;
	struct btrfs_path *path;
	struct btrfs_key key;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = BTRFS_FIRST_CHUNK_TREE_OBJECTID;
	key.offset = chunk_offset;
	key.type = BTRFS_CHUNK_ITEM_KEY;

	ret = btrfs_search_slot(trans, root, &key, path, -1, 1);
	if (ret < 0)
		goto out;
	else if (ret > 0) { /* Logic error or corruption */
		btrfs_handle_fs_error(fs_info, -ENOENT,
				      "Failed lookup while freeing chunk.");
		ret = -ENOENT;
		goto out;
	}

	ret = btrfs_del_item(trans, root, path);
	if (ret < 0)
		btrfs_handle_fs_error(fs_info, ret,
				      "Failed to delete chunk item.");
out:
	btrfs_free_path(path);
	return ret;
}

static int btrfs_del_sys_chunk(struct btrfs_fs_info *fs_info, u64 chunk_offset)
{
	struct btrfs_super_block *super_copy = fs_info->super_copy;
	struct btrfs_disk_key *disk_key;
	struct btrfs_chunk *chunk;
	u8 *ptr;
	int ret = 0;
	u32 num_stripes;
	u32 array_size;
	u32 len = 0;
	u32 cur;
	struct btrfs_key key;

<<<<<<< HEAD
	mutex_lock(&fs_info->chunk_mutex);
=======
	lockdep_assert_held(&fs_info->chunk_mutex);
>>>>>>> upstream/android-13
	array_size = btrfs_super_sys_array_size(super_copy);

	ptr = super_copy->sys_chunk_array;
	cur = 0;

	while (cur < array_size) {
		disk_key = (struct btrfs_disk_key *)ptr;
		btrfs_disk_key_to_cpu(&key, disk_key);

		len = sizeof(*disk_key);

		if (key.type == BTRFS_CHUNK_ITEM_KEY) {
			chunk = (struct btrfs_chunk *)(ptr + len);
			num_stripes = btrfs_stack_chunk_num_stripes(chunk);
			len += btrfs_chunk_item_size(num_stripes);
		} else {
			ret = -EIO;
			break;
		}
		if (key.objectid == BTRFS_FIRST_CHUNK_TREE_OBJECTID &&
		    key.offset == chunk_offset) {
			memmove(ptr, ptr + len, array_size - (cur + len));
			array_size -= len;
			btrfs_set_super_sys_array_size(super_copy, array_size);
		} else {
			ptr += len;
			cur += len;
		}
	}
<<<<<<< HEAD
	mutex_unlock(&fs_info->chunk_mutex);
	return ret;
}

static struct extent_map *get_chunk_map(struct btrfs_fs_info *fs_info,
					u64 logical, u64 length)
=======
	return ret;
}

/*
 * btrfs_get_chunk_map() - Find the mapping containing the given logical extent.
 * @logical: Logical block offset in bytes.
 * @length: Length of extent in bytes.
 *
 * Return: Chunk mapping or ERR_PTR.
 */
struct extent_map *btrfs_get_chunk_map(struct btrfs_fs_info *fs_info,
				       u64 logical, u64 length)
>>>>>>> upstream/android-13
{
	struct extent_map_tree *em_tree;
	struct extent_map *em;

<<<<<<< HEAD
	em_tree = &fs_info->mapping_tree.map_tree;
=======
	em_tree = &fs_info->mapping_tree;
>>>>>>> upstream/android-13
	read_lock(&em_tree->lock);
	em = lookup_extent_mapping(em_tree, logical, length);
	read_unlock(&em_tree->lock);

	if (!em) {
		btrfs_crit(fs_info, "unable to find logical %llu length %llu",
			   logical, length);
		return ERR_PTR(-EINVAL);
	}

	if (em->start > logical || em->start + em->len < logical) {
		btrfs_crit(fs_info,
			   "found a bad mapping, wanted %llu-%llu, found %llu-%llu",
			   logical, length, em->start, em->start + em->len);
		free_extent_map(em);
		return ERR_PTR(-EINVAL);
	}

	/* callers are responsible for dropping em's ref. */
	return em;
}

<<<<<<< HEAD
=======
static int remove_chunk_item(struct btrfs_trans_handle *trans,
			     struct map_lookup *map, u64 chunk_offset)
{
	int i;

	/*
	 * Removing chunk items and updating the device items in the chunks btree
	 * requires holding the chunk_mutex.
	 * See the comment at btrfs_chunk_alloc() for the details.
	 */
	lockdep_assert_held(&trans->fs_info->chunk_mutex);

	for (i = 0; i < map->num_stripes; i++) {
		int ret;

		ret = btrfs_update_device(trans, map->stripes[i].dev);
		if (ret)
			return ret;
	}

	return btrfs_free_chunk(trans, chunk_offset);
}

>>>>>>> upstream/android-13
int btrfs_remove_chunk(struct btrfs_trans_handle *trans, u64 chunk_offset)
{
	struct btrfs_fs_info *fs_info = trans->fs_info;
	struct extent_map *em;
	struct map_lookup *map;
	u64 dev_extent_len = 0;
	int i, ret = 0;
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;

<<<<<<< HEAD
	em = get_chunk_map(fs_info, chunk_offset, 1);
=======
	em = btrfs_get_chunk_map(fs_info, chunk_offset, 1);
>>>>>>> upstream/android-13
	if (IS_ERR(em)) {
		/*
		 * This is a logic error, but we don't want to just rely on the
		 * user having built with ASSERT enabled, so if ASSERT doesn't
		 * do anything we still error out.
		 */
		ASSERT(0);
		return PTR_ERR(em);
	}
	map = em->map_lookup;
<<<<<<< HEAD
	mutex_lock(&fs_info->chunk_mutex);
	check_system_chunk(trans, map->type);
	mutex_unlock(&fs_info->chunk_mutex);

	/*
	 * Take the device list mutex to prevent races with the final phase of
	 * a device replace operation that replaces the device object associated
	 * with map stripes (dev-replace.c:btrfs_dev_replace_finishing()).
=======

	/*
	 * First delete the device extent items from the devices btree.
	 * We take the device_list_mutex to avoid racing with the finishing phase
	 * of a device replace operation. See the comment below before acquiring
	 * fs_info->chunk_mutex. Note that here we do not acquire the chunk_mutex
	 * because that can result in a deadlock when deleting the device extent
	 * items from the devices btree - COWing an extent buffer from the btree
	 * may result in allocating a new metadata chunk, which would attempt to
	 * lock again fs_info->chunk_mutex.
>>>>>>> upstream/android-13
	 */
	mutex_lock(&fs_devices->device_list_mutex);
	for (i = 0; i < map->num_stripes; i++) {
		struct btrfs_device *device = map->stripes[i].dev;
		ret = btrfs_free_dev_extent(trans, device,
					    map->stripes[i].physical,
					    &dev_extent_len);
		if (ret) {
			mutex_unlock(&fs_devices->device_list_mutex);
			btrfs_abort_transaction(trans, ret);
			goto out;
		}

		if (device->bytes_used > 0) {
			mutex_lock(&fs_info->chunk_mutex);
			btrfs_device_set_bytes_used(device,
					device->bytes_used - dev_extent_len);
			atomic64_add(dev_extent_len, &fs_info->free_chunk_space);
			btrfs_clear_space_info_full(fs_info);
			mutex_unlock(&fs_info->chunk_mutex);
		}
<<<<<<< HEAD

		if (map->stripes[i].dev) {
			ret = btrfs_update_device(trans, map->stripes[i].dev);
			if (ret) {
				mutex_unlock(&fs_devices->device_list_mutex);
				btrfs_abort_transaction(trans, ret);
				goto out;
			}
		}
	}
	mutex_unlock(&fs_devices->device_list_mutex);

	ret = btrfs_free_chunk(trans, chunk_offset);
	if (ret) {
=======
	}
	mutex_unlock(&fs_devices->device_list_mutex);

	/*
	 * We acquire fs_info->chunk_mutex for 2 reasons:
	 *
	 * 1) Just like with the first phase of the chunk allocation, we must
	 *    reserve system space, do all chunk btree updates and deletions, and
	 *    update the system chunk array in the superblock while holding this
	 *    mutex. This is for similar reasons as explained on the comment at
	 *    the top of btrfs_chunk_alloc();
	 *
	 * 2) Prevent races with the final phase of a device replace operation
	 *    that replaces the device object associated with the map's stripes,
	 *    because the device object's id can change at any time during that
	 *    final phase of the device replace operation
	 *    (dev-replace.c:btrfs_dev_replace_finishing()), so we could grab the
	 *    replaced device and then see it with an ID of
	 *    BTRFS_DEV_REPLACE_DEVID, which would cause a failure when updating
	 *    the device item, which does not exists on the chunk btree.
	 *    The finishing phase of device replace acquires both the
	 *    device_list_mutex and the chunk_mutex, in that order, so we are
	 *    safe by just acquiring the chunk_mutex.
	 */
	trans->removing_chunk = true;
	mutex_lock(&fs_info->chunk_mutex);

	check_system_chunk(trans, map->type);

	ret = remove_chunk_item(trans, map, chunk_offset);
	/*
	 * Normally we should not get -ENOSPC since we reserved space before
	 * through the call to check_system_chunk().
	 *
	 * Despite our system space_info having enough free space, we may not
	 * be able to allocate extents from its block groups, because all have
	 * an incompatible profile, which will force us to allocate a new system
	 * block group with the right profile, or right after we called
	 * check_system_space() above, a scrub turned the only system block group
	 * with enough free space into RO mode.
	 * This is explained with more detail at do_chunk_alloc().
	 *
	 * So if we get -ENOSPC, allocate a new system chunk and retry once.
	 */
	if (ret == -ENOSPC) {
		const u64 sys_flags = btrfs_system_alloc_profile(fs_info);
		struct btrfs_block_group *sys_bg;

		sys_bg = btrfs_alloc_chunk(trans, sys_flags);
		if (IS_ERR(sys_bg)) {
			ret = PTR_ERR(sys_bg);
			btrfs_abort_transaction(trans, ret);
			goto out;
		}

		ret = btrfs_chunk_alloc_add_chunk_item(trans, sys_bg);
		if (ret) {
			btrfs_abort_transaction(trans, ret);
			goto out;
		}

		ret = remove_chunk_item(trans, map, chunk_offset);
		if (ret) {
			btrfs_abort_transaction(trans, ret);
			goto out;
		}
	} else if (ret) {
>>>>>>> upstream/android-13
		btrfs_abort_transaction(trans, ret);
		goto out;
	}

	trace_btrfs_chunk_free(fs_info, map, chunk_offset, em->len);

	if (map->type & BTRFS_BLOCK_GROUP_SYSTEM) {
		ret = btrfs_del_sys_chunk(fs_info, chunk_offset);
		if (ret) {
			btrfs_abort_transaction(trans, ret);
			goto out;
		}
	}

<<<<<<< HEAD
=======
	mutex_unlock(&fs_info->chunk_mutex);
	trans->removing_chunk = false;

	/*
	 * We are done with chunk btree updates and deletions, so release the
	 * system space we previously reserved (with check_system_chunk()).
	 */
	btrfs_trans_release_chunk_metadata(trans);

>>>>>>> upstream/android-13
	ret = btrfs_remove_block_group(trans, chunk_offset, em);
	if (ret) {
		btrfs_abort_transaction(trans, ret);
		goto out;
	}

out:
<<<<<<< HEAD
=======
	if (trans->removing_chunk) {
		mutex_unlock(&fs_info->chunk_mutex);
		trans->removing_chunk = false;
	}
>>>>>>> upstream/android-13
	/* once for us */
	free_extent_map(em);
	return ret;
}

<<<<<<< HEAD
static int btrfs_relocate_chunk(struct btrfs_fs_info *fs_info, u64 chunk_offset)
{
	struct btrfs_root *root = fs_info->chunk_root;
	struct btrfs_trans_handle *trans;
=======
int btrfs_relocate_chunk(struct btrfs_fs_info *fs_info, u64 chunk_offset)
{
	struct btrfs_root *root = fs_info->chunk_root;
	struct btrfs_trans_handle *trans;
	struct btrfs_block_group *block_group;
	u64 length;
>>>>>>> upstream/android-13
	int ret;

	/*
	 * Prevent races with automatic removal of unused block groups.
	 * After we relocate and before we remove the chunk with offset
	 * chunk_offset, automatic removal of the block group can kick in,
	 * resulting in a failure when calling btrfs_remove_chunk() below.
	 *
	 * Make sure to acquire this mutex before doing a tree search (dev
	 * or chunk trees) to find chunks. Otherwise the cleaner kthread might
	 * call btrfs_remove_chunk() (through btrfs_delete_unused_bgs()) after
	 * we release the path used to search the chunk/dev tree and before
	 * the current task acquires this mutex and calls us.
	 */
<<<<<<< HEAD
	lockdep_assert_held(&fs_info->delete_unused_bgs_mutex);

	ret = btrfs_can_relocate(fs_info, chunk_offset);
	if (ret)
		return -ENOSPC;
=======
	lockdep_assert_held(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13

	/* step one, relocate all the extents inside this chunk */
	btrfs_scrub_pause(fs_info);
	ret = btrfs_relocate_block_group(fs_info, chunk_offset);
	btrfs_scrub_continue(fs_info);
	if (ret)
		return ret;

<<<<<<< HEAD
	/*
	 * We add the kobjects here (and after forcing data chunk creation)
	 * since relocation is the only place we'll create chunks of a new
	 * type at runtime.  The only place where we'll remove the last
	 * chunk of a type is the call immediately below this one.  Even
	 * so, we're protected against races with the cleaner thread since
	 * we're covered by the delete_unused_bgs_mutex.
	 */
	btrfs_add_raid_kobjects(fs_info);
=======
	block_group = btrfs_lookup_block_group(fs_info, chunk_offset);
	if (!block_group)
		return -ENOENT;
	btrfs_discard_cancel_work(&fs_info->discard_ctl, block_group);
	length = block_group->length;
	btrfs_put_block_group(block_group);

	/*
	 * On a zoned file system, discard the whole block group, this will
	 * trigger a REQ_OP_ZONE_RESET operation on the device zone. If
	 * resetting the zone fails, don't treat it as a fatal problem from the
	 * filesystem's point of view.
	 */
	if (btrfs_is_zoned(fs_info)) {
		ret = btrfs_discard_extent(fs_info, chunk_offset, length, NULL);
		if (ret)
			btrfs_info(fs_info,
				"failed to reset zone %llu after relocation",
				chunk_offset);
	}
>>>>>>> upstream/android-13

	trans = btrfs_start_trans_remove_block_group(root->fs_info,
						     chunk_offset);
	if (IS_ERR(trans)) {
		ret = PTR_ERR(trans);
		btrfs_handle_fs_error(root->fs_info, ret, NULL);
		return ret;
	}

	/*
	 * step two, delete the device extents and the
	 * chunk tree entries
	 */
	ret = btrfs_remove_chunk(trans, chunk_offset);
	btrfs_end_transaction(trans);
	return ret;
}

static int btrfs_relocate_sys_chunks(struct btrfs_fs_info *fs_info)
{
	struct btrfs_root *chunk_root = fs_info->chunk_root;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_chunk *chunk;
	struct btrfs_key key;
	struct btrfs_key found_key;
	u64 chunk_type;
	bool retried = false;
	int failed = 0;
	int ret;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

again:
	key.objectid = BTRFS_FIRST_CHUNK_TREE_OBJECTID;
	key.offset = (u64)-1;
	key.type = BTRFS_CHUNK_ITEM_KEY;

	while (1) {
<<<<<<< HEAD
		mutex_lock(&fs_info->delete_unused_bgs_mutex);
		ret = btrfs_search_slot(NULL, chunk_root, &key, path, 0, 0);
		if (ret < 0) {
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
		mutex_lock(&fs_info->reclaim_bgs_lock);
		ret = btrfs_search_slot(NULL, chunk_root, &key, path, 0, 0);
		if (ret < 0) {
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			goto error;
		}
		BUG_ON(ret == 0); /* Corruption */

		ret = btrfs_previous_item(chunk_root, path, key.objectid,
					  key.type);
		if (ret)
<<<<<<< HEAD
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
		if (ret < 0)
			goto error;
		if (ret > 0)
			break;

		leaf = path->nodes[0];
		btrfs_item_key_to_cpu(leaf, &found_key, path->slots[0]);

		chunk = btrfs_item_ptr(leaf, path->slots[0],
				       struct btrfs_chunk);
		chunk_type = btrfs_chunk_type(leaf, chunk);
		btrfs_release_path(path);

		if (chunk_type & BTRFS_BLOCK_GROUP_SYSTEM) {
			ret = btrfs_relocate_chunk(fs_info, found_key.offset);
			if (ret == -ENOSPC)
				failed++;
			else
				BUG_ON(ret);
		}
<<<<<<< HEAD
		mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
		mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13

		if (found_key.offset == 0)
			break;
		key.offset = found_key.offset - 1;
	}
	ret = 0;
	if (failed && !retried) {
		failed = 0;
		retried = true;
		goto again;
	} else if (WARN_ON(failed && retried)) {
		ret = -ENOSPC;
	}
error:
	btrfs_free_path(path);
	return ret;
}

/*
 * return 1 : allocate a data chunk successfully,
 * return <0: errors during allocating a data chunk,
 * return 0 : no need to allocate a data chunk.
 */
static int btrfs_may_alloc_data_chunk(struct btrfs_fs_info *fs_info,
				      u64 chunk_offset)
{
<<<<<<< HEAD
	struct btrfs_block_group_cache *cache;
=======
	struct btrfs_block_group *cache;
>>>>>>> upstream/android-13
	u64 bytes_used;
	u64 chunk_type;

	cache = btrfs_lookup_block_group(fs_info, chunk_offset);
	ASSERT(cache);
	chunk_type = cache->flags;
	btrfs_put_block_group(cache);

<<<<<<< HEAD
	if (chunk_type & BTRFS_BLOCK_GROUP_DATA) {
		spin_lock(&fs_info->data_sinfo->lock);
		bytes_used = fs_info->data_sinfo->bytes_used;
		spin_unlock(&fs_info->data_sinfo->lock);

		if (!bytes_used) {
			struct btrfs_trans_handle *trans;
			int ret;

			trans =	btrfs_join_transaction(fs_info->tree_root);
			if (IS_ERR(trans))
				return PTR_ERR(trans);

			ret = btrfs_force_chunk_alloc(trans,
						      BTRFS_BLOCK_GROUP_DATA);
			btrfs_end_transaction(trans);
			if (ret < 0)
				return ret;

			btrfs_add_raid_kobjects(fs_info);

			return 1;
		}
	}
=======
	if (!(chunk_type & BTRFS_BLOCK_GROUP_DATA))
		return 0;

	spin_lock(&fs_info->data_sinfo->lock);
	bytes_used = fs_info->data_sinfo->bytes_used;
	spin_unlock(&fs_info->data_sinfo->lock);

	if (!bytes_used) {
		struct btrfs_trans_handle *trans;
		int ret;

		trans =	btrfs_join_transaction(fs_info->tree_root);
		if (IS_ERR(trans))
			return PTR_ERR(trans);

		ret = btrfs_force_chunk_alloc(trans, BTRFS_BLOCK_GROUP_DATA);
		btrfs_end_transaction(trans);
		if (ret < 0)
			return ret;
		return 1;
	}

>>>>>>> upstream/android-13
	return 0;
}

static int insert_balance_item(struct btrfs_fs_info *fs_info,
			       struct btrfs_balance_control *bctl)
{
	struct btrfs_root *root = fs_info->tree_root;
	struct btrfs_trans_handle *trans;
	struct btrfs_balance_item *item;
	struct btrfs_disk_balance_args disk_bargs;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_key key;
	int ret, err;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans)) {
		btrfs_free_path(path);
		return PTR_ERR(trans);
	}

	key.objectid = BTRFS_BALANCE_OBJECTID;
	key.type = BTRFS_TEMPORARY_ITEM_KEY;
	key.offset = 0;

	ret = btrfs_insert_empty_item(trans, root, path, &key,
				      sizeof(*item));
	if (ret)
		goto out;

	leaf = path->nodes[0];
	item = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_balance_item);

	memzero_extent_buffer(leaf, (unsigned long)item, sizeof(*item));

	btrfs_cpu_balance_args_to_disk(&disk_bargs, &bctl->data);
	btrfs_set_balance_data(leaf, item, &disk_bargs);
	btrfs_cpu_balance_args_to_disk(&disk_bargs, &bctl->meta);
	btrfs_set_balance_meta(leaf, item, &disk_bargs);
	btrfs_cpu_balance_args_to_disk(&disk_bargs, &bctl->sys);
	btrfs_set_balance_sys(leaf, item, &disk_bargs);

	btrfs_set_balance_flags(leaf, item, bctl->flags);

	btrfs_mark_buffer_dirty(leaf);
out:
	btrfs_free_path(path);
	err = btrfs_commit_transaction(trans);
	if (err && !ret)
		ret = err;
	return ret;
}

static int del_balance_item(struct btrfs_fs_info *fs_info)
{
	struct btrfs_root *root = fs_info->tree_root;
	struct btrfs_trans_handle *trans;
	struct btrfs_path *path;
	struct btrfs_key key;
	int ret, err;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

<<<<<<< HEAD
	trans = btrfs_start_transaction(root, 0);
=======
	trans = btrfs_start_transaction_fallback_global_rsv(root, 0);
>>>>>>> upstream/android-13
	if (IS_ERR(trans)) {
		btrfs_free_path(path);
		return PTR_ERR(trans);
	}

	key.objectid = BTRFS_BALANCE_OBJECTID;
	key.type = BTRFS_TEMPORARY_ITEM_KEY;
	key.offset = 0;

	ret = btrfs_search_slot(trans, root, &key, path, -1, 1);
	if (ret < 0)
		goto out;
	if (ret > 0) {
		ret = -ENOENT;
		goto out;
	}

	ret = btrfs_del_item(trans, root, path);
out:
	btrfs_free_path(path);
	err = btrfs_commit_transaction(trans);
	if (err && !ret)
		ret = err;
	return ret;
}

/*
 * This is a heuristic used to reduce the number of chunks balanced on
 * resume after balance was interrupted.
 */
static void update_balance_args(struct btrfs_balance_control *bctl)
{
	/*
	 * Turn on soft mode for chunk types that were being converted.
	 */
	if (bctl->data.flags & BTRFS_BALANCE_ARGS_CONVERT)
		bctl->data.flags |= BTRFS_BALANCE_ARGS_SOFT;
	if (bctl->sys.flags & BTRFS_BALANCE_ARGS_CONVERT)
		bctl->sys.flags |= BTRFS_BALANCE_ARGS_SOFT;
	if (bctl->meta.flags & BTRFS_BALANCE_ARGS_CONVERT)
		bctl->meta.flags |= BTRFS_BALANCE_ARGS_SOFT;

	/*
	 * Turn on usage filter if is not already used.  The idea is
	 * that chunks that we have already balanced should be
	 * reasonably full.  Don't do it for chunks that are being
	 * converted - that will keep us from relocating unconverted
	 * (albeit full) chunks.
	 */
	if (!(bctl->data.flags & BTRFS_BALANCE_ARGS_USAGE) &&
	    !(bctl->data.flags & BTRFS_BALANCE_ARGS_USAGE_RANGE) &&
	    !(bctl->data.flags & BTRFS_BALANCE_ARGS_CONVERT)) {
		bctl->data.flags |= BTRFS_BALANCE_ARGS_USAGE;
		bctl->data.usage = 90;
	}
	if (!(bctl->sys.flags & BTRFS_BALANCE_ARGS_USAGE) &&
	    !(bctl->sys.flags & BTRFS_BALANCE_ARGS_USAGE_RANGE) &&
	    !(bctl->sys.flags & BTRFS_BALANCE_ARGS_CONVERT)) {
		bctl->sys.flags |= BTRFS_BALANCE_ARGS_USAGE;
		bctl->sys.usage = 90;
	}
	if (!(bctl->meta.flags & BTRFS_BALANCE_ARGS_USAGE) &&
	    !(bctl->meta.flags & BTRFS_BALANCE_ARGS_USAGE_RANGE) &&
	    !(bctl->meta.flags & BTRFS_BALANCE_ARGS_CONVERT)) {
		bctl->meta.flags |= BTRFS_BALANCE_ARGS_USAGE;
		bctl->meta.usage = 90;
	}
}

/*
 * Clear the balance status in fs_info and delete the balance item from disk.
 */
static void reset_balance_state(struct btrfs_fs_info *fs_info)
{
	struct btrfs_balance_control *bctl = fs_info->balance_ctl;
	int ret;

	BUG_ON(!fs_info->balance_ctl);

	spin_lock(&fs_info->balance_lock);
	fs_info->balance_ctl = NULL;
	spin_unlock(&fs_info->balance_lock);

	kfree(bctl);
	ret = del_balance_item(fs_info);
	if (ret)
		btrfs_handle_fs_error(fs_info, ret, NULL);
}

/*
 * Balance filters.  Return 1 if chunk should be filtered out
 * (should not be balanced).
 */
static int chunk_profiles_filter(u64 chunk_type,
				 struct btrfs_balance_args *bargs)
{
	chunk_type = chunk_to_extended(chunk_type) &
				BTRFS_EXTENDED_PROFILE_MASK;

	if (bargs->profiles & chunk_type)
		return 0;

	return 1;
}

static int chunk_usage_range_filter(struct btrfs_fs_info *fs_info, u64 chunk_offset,
			      struct btrfs_balance_args *bargs)
{
<<<<<<< HEAD
	struct btrfs_block_group_cache *cache;
=======
	struct btrfs_block_group *cache;
>>>>>>> upstream/android-13
	u64 chunk_used;
	u64 user_thresh_min;
	u64 user_thresh_max;
	int ret = 1;

	cache = btrfs_lookup_block_group(fs_info, chunk_offset);
<<<<<<< HEAD
	chunk_used = btrfs_block_group_used(&cache->item);
=======
	chunk_used = cache->used;
>>>>>>> upstream/android-13

	if (bargs->usage_min == 0)
		user_thresh_min = 0;
	else
<<<<<<< HEAD
		user_thresh_min = div_factor_fine(cache->key.offset,
					bargs->usage_min);
=======
		user_thresh_min = div_factor_fine(cache->length,
						  bargs->usage_min);
>>>>>>> upstream/android-13

	if (bargs->usage_max == 0)
		user_thresh_max = 1;
	else if (bargs->usage_max > 100)
<<<<<<< HEAD
		user_thresh_max = cache->key.offset;
	else
		user_thresh_max = div_factor_fine(cache->key.offset,
					bargs->usage_max);
=======
		user_thresh_max = cache->length;
	else
		user_thresh_max = div_factor_fine(cache->length,
						  bargs->usage_max);
>>>>>>> upstream/android-13

	if (user_thresh_min <= chunk_used && chunk_used < user_thresh_max)
		ret = 0;

	btrfs_put_block_group(cache);
	return ret;
}

static int chunk_usage_filter(struct btrfs_fs_info *fs_info,
		u64 chunk_offset, struct btrfs_balance_args *bargs)
{
<<<<<<< HEAD
	struct btrfs_block_group_cache *cache;
=======
	struct btrfs_block_group *cache;
>>>>>>> upstream/android-13
	u64 chunk_used, user_thresh;
	int ret = 1;

	cache = btrfs_lookup_block_group(fs_info, chunk_offset);
<<<<<<< HEAD
	chunk_used = btrfs_block_group_used(&cache->item);
=======
	chunk_used = cache->used;
>>>>>>> upstream/android-13

	if (bargs->usage_min == 0)
		user_thresh = 1;
	else if (bargs->usage > 100)
<<<<<<< HEAD
		user_thresh = cache->key.offset;
	else
		user_thresh = div_factor_fine(cache->key.offset,
					      bargs->usage);
=======
		user_thresh = cache->length;
	else
		user_thresh = div_factor_fine(cache->length, bargs->usage);
>>>>>>> upstream/android-13

	if (chunk_used < user_thresh)
		ret = 0;

	btrfs_put_block_group(cache);
	return ret;
}

static int chunk_devid_filter(struct extent_buffer *leaf,
			      struct btrfs_chunk *chunk,
			      struct btrfs_balance_args *bargs)
{
	struct btrfs_stripe *stripe;
	int num_stripes = btrfs_chunk_num_stripes(leaf, chunk);
	int i;

	for (i = 0; i < num_stripes; i++) {
		stripe = btrfs_stripe_nr(chunk, i);
		if (btrfs_stripe_devid(leaf, stripe) == bargs->devid)
			return 0;
	}

	return 1;
}

<<<<<<< HEAD
=======
static u64 calc_data_stripes(u64 type, int num_stripes)
{
	const int index = btrfs_bg_flags_to_raid_index(type);
	const int ncopies = btrfs_raid_array[index].ncopies;
	const int nparity = btrfs_raid_array[index].nparity;

	return (num_stripes - nparity) / ncopies;
}

>>>>>>> upstream/android-13
/* [pstart, pend) */
static int chunk_drange_filter(struct extent_buffer *leaf,
			       struct btrfs_chunk *chunk,
			       struct btrfs_balance_args *bargs)
{
	struct btrfs_stripe *stripe;
	int num_stripes = btrfs_chunk_num_stripes(leaf, chunk);
	u64 stripe_offset;
	u64 stripe_length;
<<<<<<< HEAD
=======
	u64 type;
>>>>>>> upstream/android-13
	int factor;
	int i;

	if (!(bargs->flags & BTRFS_BALANCE_ARGS_DEVID))
		return 0;

<<<<<<< HEAD
	if (btrfs_chunk_type(leaf, chunk) & (BTRFS_BLOCK_GROUP_DUP |
	     BTRFS_BLOCK_GROUP_RAID1 | BTRFS_BLOCK_GROUP_RAID10)) {
		factor = num_stripes / 2;
	} else if (btrfs_chunk_type(leaf, chunk) & BTRFS_BLOCK_GROUP_RAID5) {
		factor = num_stripes - 1;
	} else if (btrfs_chunk_type(leaf, chunk) & BTRFS_BLOCK_GROUP_RAID6) {
		factor = num_stripes - 2;
	} else {
		factor = num_stripes;
	}
=======
	type = btrfs_chunk_type(leaf, chunk);
	factor = calc_data_stripes(type, num_stripes);
>>>>>>> upstream/android-13

	for (i = 0; i < num_stripes; i++) {
		stripe = btrfs_stripe_nr(chunk, i);
		if (btrfs_stripe_devid(leaf, stripe) != bargs->devid)
			continue;

		stripe_offset = btrfs_stripe_offset(leaf, stripe);
		stripe_length = btrfs_chunk_length(leaf, chunk);
		stripe_length = div_u64(stripe_length, factor);

		if (stripe_offset < bargs->pend &&
		    stripe_offset + stripe_length > bargs->pstart)
			return 0;
	}

	return 1;
}

/* [vstart, vend) */
static int chunk_vrange_filter(struct extent_buffer *leaf,
			       struct btrfs_chunk *chunk,
			       u64 chunk_offset,
			       struct btrfs_balance_args *bargs)
{
	if (chunk_offset < bargs->vend &&
	    chunk_offset + btrfs_chunk_length(leaf, chunk) > bargs->vstart)
		/* at least part of the chunk is inside this vrange */
		return 0;

	return 1;
}

static int chunk_stripes_range_filter(struct extent_buffer *leaf,
			       struct btrfs_chunk *chunk,
			       struct btrfs_balance_args *bargs)
{
	int num_stripes = btrfs_chunk_num_stripes(leaf, chunk);

	if (bargs->stripes_min <= num_stripes
			&& num_stripes <= bargs->stripes_max)
		return 0;

	return 1;
}

static int chunk_soft_convert_filter(u64 chunk_type,
				     struct btrfs_balance_args *bargs)
{
	if (!(bargs->flags & BTRFS_BALANCE_ARGS_CONVERT))
		return 0;

	chunk_type = chunk_to_extended(chunk_type) &
				BTRFS_EXTENDED_PROFILE_MASK;

	if (bargs->target == chunk_type)
		return 1;

	return 0;
}

<<<<<<< HEAD
static int should_balance_chunk(struct btrfs_fs_info *fs_info,
				struct extent_buffer *leaf,
				struct btrfs_chunk *chunk, u64 chunk_offset)
{
=======
static int should_balance_chunk(struct extent_buffer *leaf,
				struct btrfs_chunk *chunk, u64 chunk_offset)
{
	struct btrfs_fs_info *fs_info = leaf->fs_info;
>>>>>>> upstream/android-13
	struct btrfs_balance_control *bctl = fs_info->balance_ctl;
	struct btrfs_balance_args *bargs = NULL;
	u64 chunk_type = btrfs_chunk_type(leaf, chunk);

	/* type filter */
	if (!((chunk_type & BTRFS_BLOCK_GROUP_TYPE_MASK) &
	      (bctl->flags & BTRFS_BALANCE_TYPE_MASK))) {
		return 0;
	}

	if (chunk_type & BTRFS_BLOCK_GROUP_DATA)
		bargs = &bctl->data;
	else if (chunk_type & BTRFS_BLOCK_GROUP_SYSTEM)
		bargs = &bctl->sys;
	else if (chunk_type & BTRFS_BLOCK_GROUP_METADATA)
		bargs = &bctl->meta;

	/* profiles filter */
	if ((bargs->flags & BTRFS_BALANCE_ARGS_PROFILES) &&
	    chunk_profiles_filter(chunk_type, bargs)) {
		return 0;
	}

	/* usage filter */
	if ((bargs->flags & BTRFS_BALANCE_ARGS_USAGE) &&
	    chunk_usage_filter(fs_info, chunk_offset, bargs)) {
		return 0;
	} else if ((bargs->flags & BTRFS_BALANCE_ARGS_USAGE_RANGE) &&
	    chunk_usage_range_filter(fs_info, chunk_offset, bargs)) {
		return 0;
	}

	/* devid filter */
	if ((bargs->flags & BTRFS_BALANCE_ARGS_DEVID) &&
	    chunk_devid_filter(leaf, chunk, bargs)) {
		return 0;
	}

	/* drange filter, makes sense only with devid filter */
	if ((bargs->flags & BTRFS_BALANCE_ARGS_DRANGE) &&
	    chunk_drange_filter(leaf, chunk, bargs)) {
		return 0;
	}

	/* vrange filter */
	if ((bargs->flags & BTRFS_BALANCE_ARGS_VRANGE) &&
	    chunk_vrange_filter(leaf, chunk, chunk_offset, bargs)) {
		return 0;
	}

	/* stripes filter */
	if ((bargs->flags & BTRFS_BALANCE_ARGS_STRIPES_RANGE) &&
	    chunk_stripes_range_filter(leaf, chunk, bargs)) {
		return 0;
	}

	/* soft profile changing mode */
	if ((bargs->flags & BTRFS_BALANCE_ARGS_SOFT) &&
	    chunk_soft_convert_filter(chunk_type, bargs)) {
		return 0;
	}

	/*
	 * limited by count, must be the last filter
	 */
	if ((bargs->flags & BTRFS_BALANCE_ARGS_LIMIT)) {
		if (bargs->limit == 0)
			return 0;
		else
			bargs->limit--;
	} else if ((bargs->flags & BTRFS_BALANCE_ARGS_LIMIT_RANGE)) {
		/*
		 * Same logic as the 'limit' filter; the minimum cannot be
		 * determined here because we do not have the global information
		 * about the count of all chunks that satisfy the filters.
		 */
		if (bargs->limit_max == 0)
			return 0;
		else
			bargs->limit_max--;
	}

	return 1;
}

static int __btrfs_balance(struct btrfs_fs_info *fs_info)
{
	struct btrfs_balance_control *bctl = fs_info->balance_ctl;
	struct btrfs_root *chunk_root = fs_info->chunk_root;
<<<<<<< HEAD
	struct btrfs_root *dev_root = fs_info->dev_root;
	struct list_head *devices;
	struct btrfs_device *device;
	u64 old_size;
	u64 size_to_free;
=======
>>>>>>> upstream/android-13
	u64 chunk_type;
	struct btrfs_chunk *chunk;
	struct btrfs_path *path = NULL;
	struct btrfs_key key;
	struct btrfs_key found_key;
<<<<<<< HEAD
	struct btrfs_trans_handle *trans;
=======
>>>>>>> upstream/android-13
	struct extent_buffer *leaf;
	int slot;
	int ret;
	int enospc_errors = 0;
	bool counting = true;
	/* The single value limit and min/max limits use the same bytes in the */
	u64 limit_data = bctl->data.limit;
	u64 limit_meta = bctl->meta.limit;
	u64 limit_sys = bctl->sys.limit;
	u32 count_data = 0;
	u32 count_meta = 0;
	u32 count_sys = 0;
	int chunk_reserved = 0;

<<<<<<< HEAD
	/* step one make some room on all the devices */
	devices = &fs_info->fs_devices->devices;
	list_for_each_entry(device, devices, dev_list) {
		old_size = btrfs_device_get_total_bytes(device);
		size_to_free = div_factor(old_size, 1);
		size_to_free = min_t(u64, size_to_free, SZ_1M);
		if (!test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state) ||
		    btrfs_device_get_total_bytes(device) -
		    btrfs_device_get_bytes_used(device) > size_to_free ||
		    test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state))
			continue;

		ret = btrfs_shrink_device(device, old_size - size_to_free);
		if (ret == -ENOSPC)
			break;
		if (ret) {
			/* btrfs_shrink_device never returns ret > 0 */
			WARN_ON(ret > 0);
			goto error;
		}

		trans = btrfs_start_transaction(dev_root, 0);
		if (IS_ERR(trans)) {
			ret = PTR_ERR(trans);
			btrfs_info_in_rcu(fs_info,
		 "resize: unable to start transaction after shrinking device %s (error %d), old size %llu, new size %llu",
					  rcu_str_deref(device->name), ret,
					  old_size, old_size - size_to_free);
			goto error;
		}

		ret = btrfs_grow_device(trans, device, old_size);
		if (ret) {
			btrfs_end_transaction(trans);
			/* btrfs_grow_device never returns ret > 0 */
			WARN_ON(ret > 0);
			btrfs_info_in_rcu(fs_info,
		 "resize: unable to grow device after shrinking device %s (error %d), old size %llu, new size %llu",
					  rcu_str_deref(device->name), ret,
					  old_size, old_size - size_to_free);
			goto error;
		}

		btrfs_end_transaction(trans);
	}

	/* step two, relocate all the chunks */
=======
>>>>>>> upstream/android-13
	path = btrfs_alloc_path();
	if (!path) {
		ret = -ENOMEM;
		goto error;
	}

	/* zero out stat counters */
	spin_lock(&fs_info->balance_lock);
	memset(&bctl->stat, 0, sizeof(bctl->stat));
	spin_unlock(&fs_info->balance_lock);
again:
	if (!counting) {
		/*
		 * The single value limit and min/max limits use the same bytes
		 * in the
		 */
		bctl->data.limit = limit_data;
		bctl->meta.limit = limit_meta;
		bctl->sys.limit = limit_sys;
	}
	key.objectid = BTRFS_FIRST_CHUNK_TREE_OBJECTID;
	key.offset = (u64)-1;
	key.type = BTRFS_CHUNK_ITEM_KEY;

	while (1) {
		if ((!counting && atomic_read(&fs_info->balance_pause_req)) ||
		    atomic_read(&fs_info->balance_cancel_req)) {
			ret = -ECANCELED;
			goto error;
		}

<<<<<<< HEAD
		mutex_lock(&fs_info->delete_unused_bgs_mutex);
		ret = btrfs_search_slot(NULL, chunk_root, &key, path, 0, 0);
		if (ret < 0) {
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
		mutex_lock(&fs_info->reclaim_bgs_lock);
		ret = btrfs_search_slot(NULL, chunk_root, &key, path, 0, 0);
		if (ret < 0) {
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			goto error;
		}

		/*
		 * this shouldn't happen, it means the last relocate
		 * failed
		 */
		if (ret == 0)
			BUG(); /* FIXME break ? */

		ret = btrfs_previous_item(chunk_root, path, 0,
					  BTRFS_CHUNK_ITEM_KEY);
		if (ret) {
<<<<<<< HEAD
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			ret = 0;
			break;
		}

		leaf = path->nodes[0];
		slot = path->slots[0];
		btrfs_item_key_to_cpu(leaf, &found_key, slot);

		if (found_key.objectid != key.objectid) {
<<<<<<< HEAD
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			break;
		}

		chunk = btrfs_item_ptr(leaf, slot, struct btrfs_chunk);
		chunk_type = btrfs_chunk_type(leaf, chunk);

		if (!counting) {
			spin_lock(&fs_info->balance_lock);
			bctl->stat.considered++;
			spin_unlock(&fs_info->balance_lock);
		}

<<<<<<< HEAD
		ret = should_balance_chunk(fs_info, leaf, chunk,
					   found_key.offset);

		btrfs_release_path(path);
		if (!ret) {
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
		ret = should_balance_chunk(leaf, chunk, found_key.offset);

		btrfs_release_path(path);
		if (!ret) {
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			goto loop;
		}

		if (counting) {
<<<<<<< HEAD
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			spin_lock(&fs_info->balance_lock);
			bctl->stat.expected++;
			spin_unlock(&fs_info->balance_lock);

			if (chunk_type & BTRFS_BLOCK_GROUP_DATA)
				count_data++;
			else if (chunk_type & BTRFS_BLOCK_GROUP_SYSTEM)
				count_sys++;
			else if (chunk_type & BTRFS_BLOCK_GROUP_METADATA)
				count_meta++;

			goto loop;
		}

		/*
		 * Apply limit_min filter, no need to check if the LIMITS
		 * filter is used, limit_min is 0 by default
		 */
		if (((chunk_type & BTRFS_BLOCK_GROUP_DATA) &&
					count_data < bctl->data.limit_min)
				|| ((chunk_type & BTRFS_BLOCK_GROUP_METADATA) &&
					count_meta < bctl->meta.limit_min)
				|| ((chunk_type & BTRFS_BLOCK_GROUP_SYSTEM) &&
					count_sys < bctl->sys.limit_min)) {
<<<<<<< HEAD
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			goto loop;
		}

		if (!chunk_reserved) {
			/*
			 * We may be relocating the only data chunk we have,
			 * which could potentially end up with losing data's
			 * raid profile, so lets allocate an empty one in
			 * advance.
			 */
			ret = btrfs_may_alloc_data_chunk(fs_info,
							 found_key.offset);
			if (ret < 0) {
<<<<<<< HEAD
				mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
				mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
				goto error;
			} else if (ret == 1) {
				chunk_reserved = 1;
			}
		}

		ret = btrfs_relocate_chunk(fs_info, found_key.offset);
<<<<<<< HEAD
		mutex_unlock(&fs_info->delete_unused_bgs_mutex);
		if (ret && ret != -ENOSPC)
			goto error;
		if (ret == -ENOSPC) {
			enospc_errors++;
=======
		mutex_unlock(&fs_info->reclaim_bgs_lock);
		if (ret == -ENOSPC) {
			enospc_errors++;
		} else if (ret == -ETXTBSY) {
			btrfs_info(fs_info,
	   "skipping relocation of block group %llu due to active swapfile",
				   found_key.offset);
			ret = 0;
		} else if (ret) {
			goto error;
>>>>>>> upstream/android-13
		} else {
			spin_lock(&fs_info->balance_lock);
			bctl->stat.completed++;
			spin_unlock(&fs_info->balance_lock);
		}
loop:
		if (found_key.offset == 0)
			break;
		key.offset = found_key.offset - 1;
	}

	if (counting) {
		btrfs_release_path(path);
		counting = false;
		goto again;
	}
error:
	btrfs_free_path(path);
	if (enospc_errors) {
		btrfs_info(fs_info, "%d enospc errors during balance",
			   enospc_errors);
		if (!ret)
			ret = -ENOSPC;
	}

	return ret;
}

/**
 * alloc_profile_is_valid - see if a given profile is valid and reduced
 * @flags: profile to validate
 * @extended: if true @flags is treated as an extended profile
 */
static int alloc_profile_is_valid(u64 flags, int extended)
{
	u64 mask = (extended ? BTRFS_EXTENDED_PROFILE_MASK :
			       BTRFS_BLOCK_GROUP_PROFILE_MASK);

	flags &= ~BTRFS_BLOCK_GROUP_TYPE_MASK;

	/* 1) check that all other bits are zeroed */
	if (flags & ~mask)
		return 0;

	/* 2) see if profile is reduced */
	if (flags == 0)
		return !extended; /* "0" is valid for usual profiles */

<<<<<<< HEAD
	/* true if exactly one bit set */
	return (flags & (flags - 1)) == 0;
=======
	return has_single_bit_set(flags);
>>>>>>> upstream/android-13
}

static inline int balance_need_close(struct btrfs_fs_info *fs_info)
{
	/* cancel requested || normal exit path */
	return atomic_read(&fs_info->balance_cancel_req) ||
		(atomic_read(&fs_info->balance_pause_req) == 0 &&
		 atomic_read(&fs_info->balance_cancel_req) == 0);
}

<<<<<<< HEAD
/* Non-zero return value signifies invalidity */
static inline int validate_convert_profile(struct btrfs_balance_args *bctl_arg,
		u64 allowed)
{
	return ((bctl_arg->flags & BTRFS_BALANCE_ARGS_CONVERT) &&
		(!alloc_profile_is_valid(bctl_arg->target, 1) ||
		 (bctl_arg->target & ~allowed)));
=======
/*
 * Validate target profile against allowed profiles and return true if it's OK.
 * Otherwise print the error message and return false.
 */
static inline int validate_convert_profile(struct btrfs_fs_info *fs_info,
		const struct btrfs_balance_args *bargs,
		u64 allowed, const char *type)
{
	if (!(bargs->flags & BTRFS_BALANCE_ARGS_CONVERT))
		return true;

	if (fs_info->sectorsize < PAGE_SIZE &&
		bargs->target & BTRFS_BLOCK_GROUP_RAID56_MASK) {
		btrfs_err(fs_info,
		"RAID56 is not yet supported for sectorsize %u with page size %lu",
			  fs_info->sectorsize, PAGE_SIZE);
		return false;
	}
	/* Profile is valid and does not have bits outside of the allowed set */
	if (alloc_profile_is_valid(bargs->target, 1) &&
	    (bargs->target & ~allowed) == 0)
		return true;

	btrfs_err(fs_info, "balance: invalid convert %s profile %s",
			type, btrfs_bg_type_to_raid_name(bargs->target));
	return false;
}

/*
 * Fill @buf with textual description of balance filter flags @bargs, up to
 * @size_buf including the terminating null. The output may be trimmed if it
 * does not fit into the provided buffer.
 */
static void describe_balance_args(struct btrfs_balance_args *bargs, char *buf,
				 u32 size_buf)
{
	int ret;
	u32 size_bp = size_buf;
	char *bp = buf;
	u64 flags = bargs->flags;
	char tmp_buf[128] = {'\0'};

	if (!flags)
		return;

#define CHECK_APPEND_NOARG(a)						\
	do {								\
		ret = snprintf(bp, size_bp, (a));			\
		if (ret < 0 || ret >= size_bp)				\
			goto out_overflow;				\
		size_bp -= ret;						\
		bp += ret;						\
	} while (0)

#define CHECK_APPEND_1ARG(a, v1)					\
	do {								\
		ret = snprintf(bp, size_bp, (a), (v1));			\
		if (ret < 0 || ret >= size_bp)				\
			goto out_overflow;				\
		size_bp -= ret;						\
		bp += ret;						\
	} while (0)

#define CHECK_APPEND_2ARG(a, v1, v2)					\
	do {								\
		ret = snprintf(bp, size_bp, (a), (v1), (v2));		\
		if (ret < 0 || ret >= size_bp)				\
			goto out_overflow;				\
		size_bp -= ret;						\
		bp += ret;						\
	} while (0)

	if (flags & BTRFS_BALANCE_ARGS_CONVERT)
		CHECK_APPEND_1ARG("convert=%s,",
				  btrfs_bg_type_to_raid_name(bargs->target));

	if (flags & BTRFS_BALANCE_ARGS_SOFT)
		CHECK_APPEND_NOARG("soft,");

	if (flags & BTRFS_BALANCE_ARGS_PROFILES) {
		btrfs_describe_block_groups(bargs->profiles, tmp_buf,
					    sizeof(tmp_buf));
		CHECK_APPEND_1ARG("profiles=%s,", tmp_buf);
	}

	if (flags & BTRFS_BALANCE_ARGS_USAGE)
		CHECK_APPEND_1ARG("usage=%llu,", bargs->usage);

	if (flags & BTRFS_BALANCE_ARGS_USAGE_RANGE)
		CHECK_APPEND_2ARG("usage=%u..%u,",
				  bargs->usage_min, bargs->usage_max);

	if (flags & BTRFS_BALANCE_ARGS_DEVID)
		CHECK_APPEND_1ARG("devid=%llu,", bargs->devid);

	if (flags & BTRFS_BALANCE_ARGS_DRANGE)
		CHECK_APPEND_2ARG("drange=%llu..%llu,",
				  bargs->pstart, bargs->pend);

	if (flags & BTRFS_BALANCE_ARGS_VRANGE)
		CHECK_APPEND_2ARG("vrange=%llu..%llu,",
				  bargs->vstart, bargs->vend);

	if (flags & BTRFS_BALANCE_ARGS_LIMIT)
		CHECK_APPEND_1ARG("limit=%llu,", bargs->limit);

	if (flags & BTRFS_BALANCE_ARGS_LIMIT_RANGE)
		CHECK_APPEND_2ARG("limit=%u..%u,",
				bargs->limit_min, bargs->limit_max);

	if (flags & BTRFS_BALANCE_ARGS_STRIPES_RANGE)
		CHECK_APPEND_2ARG("stripes=%u..%u,",
				  bargs->stripes_min, bargs->stripes_max);

#undef CHECK_APPEND_2ARG
#undef CHECK_APPEND_1ARG
#undef CHECK_APPEND_NOARG

out_overflow:

	if (size_bp < size_buf)
		buf[size_buf - size_bp - 1] = '\0'; /* remove last , */
	else
		buf[0] = '\0';
}

static void describe_balance_start_or_resume(struct btrfs_fs_info *fs_info)
{
	u32 size_buf = 1024;
	char tmp_buf[192] = {'\0'};
	char *buf;
	char *bp;
	u32 size_bp = size_buf;
	int ret;
	struct btrfs_balance_control *bctl = fs_info->balance_ctl;

	buf = kzalloc(size_buf, GFP_KERNEL);
	if (!buf)
		return;

	bp = buf;

#define CHECK_APPEND_1ARG(a, v1)					\
	do {								\
		ret = snprintf(bp, size_bp, (a), (v1));			\
		if (ret < 0 || ret >= size_bp)				\
			goto out_overflow;				\
		size_bp -= ret;						\
		bp += ret;						\
	} while (0)

	if (bctl->flags & BTRFS_BALANCE_FORCE)
		CHECK_APPEND_1ARG("%s", "-f ");

	if (bctl->flags & BTRFS_BALANCE_DATA) {
		describe_balance_args(&bctl->data, tmp_buf, sizeof(tmp_buf));
		CHECK_APPEND_1ARG("-d%s ", tmp_buf);
	}

	if (bctl->flags & BTRFS_BALANCE_METADATA) {
		describe_balance_args(&bctl->meta, tmp_buf, sizeof(tmp_buf));
		CHECK_APPEND_1ARG("-m%s ", tmp_buf);
	}

	if (bctl->flags & BTRFS_BALANCE_SYSTEM) {
		describe_balance_args(&bctl->sys, tmp_buf, sizeof(tmp_buf));
		CHECK_APPEND_1ARG("-s%s ", tmp_buf);
	}

#undef CHECK_APPEND_1ARG

out_overflow:

	if (size_bp < size_buf)
		buf[size_buf - size_bp - 1] = '\0'; /* remove last " " */
	btrfs_info(fs_info, "balance: %s %s",
		   (bctl->flags & BTRFS_BALANCE_RESUME) ?
		   "resume" : "start", buf);

	kfree(buf);
>>>>>>> upstream/android-13
}

/*
 * Should be called with balance mutexe held
 */
int btrfs_balance(struct btrfs_fs_info *fs_info,
		  struct btrfs_balance_control *bctl,
		  struct btrfs_ioctl_balance_args *bargs)
{
	u64 meta_target, data_target;
	u64 allowed;
	int mixed = 0;
	int ret;
	u64 num_devices;
	unsigned seq;
<<<<<<< HEAD
	bool reducing_integrity;

	if (btrfs_fs_closing(fs_info) ||
	    atomic_read(&fs_info->balance_pause_req) ||
	    atomic_read(&fs_info->balance_cancel_req)) {
=======
	bool reducing_redundancy;
	int i;

	if (btrfs_fs_closing(fs_info) ||
	    atomic_read(&fs_info->balance_pause_req) ||
	    btrfs_should_cancel_balance(fs_info)) {
>>>>>>> upstream/android-13
		ret = -EINVAL;
		goto out;
	}

	allowed = btrfs_super_incompat_flags(fs_info->super_copy);
	if (allowed & BTRFS_FEATURE_INCOMPAT_MIXED_GROUPS)
		mixed = 1;

	/*
	 * In case of mixed groups both data and meta should be picked,
	 * and identical options should be given for both of them.
	 */
	allowed = BTRFS_BALANCE_DATA | BTRFS_BALANCE_METADATA;
	if (mixed && (bctl->flags & allowed)) {
		if (!(bctl->flags & BTRFS_BALANCE_DATA) ||
		    !(bctl->flags & BTRFS_BALANCE_METADATA) ||
		    memcmp(&bctl->data, &bctl->meta, sizeof(bctl->data))) {
			btrfs_err(fs_info,
	  "balance: mixed groups data and metadata options must be the same");
			ret = -EINVAL;
			goto out;
		}
	}

<<<<<<< HEAD
	num_devices = fs_info->fs_devices->num_devices;
	btrfs_dev_replace_read_lock(&fs_info->dev_replace);
	if (btrfs_dev_replace_is_ongoing(&fs_info->dev_replace)) {
		BUG_ON(num_devices < 1);
		num_devices--;
	}
	btrfs_dev_replace_read_unlock(&fs_info->dev_replace);
	allowed = BTRFS_AVAIL_ALLOC_BIT_SINGLE | BTRFS_BLOCK_GROUP_DUP;
	if (num_devices > 1)
		allowed |= (BTRFS_BLOCK_GROUP_RAID0 | BTRFS_BLOCK_GROUP_RAID1);
	if (num_devices > 2)
		allowed |= BTRFS_BLOCK_GROUP_RAID5;
	if (num_devices > 3)
		allowed |= (BTRFS_BLOCK_GROUP_RAID10 |
			    BTRFS_BLOCK_GROUP_RAID6);
	if (validate_convert_profile(&bctl->data, allowed)) {
		int index = btrfs_bg_flags_to_raid_index(bctl->data.target);

		btrfs_err(fs_info,
			  "balance: invalid convert data profile %s",
			  get_raid_name(index));
		ret = -EINVAL;
		goto out;
	}
	if (validate_convert_profile(&bctl->meta, allowed)) {
		int index = btrfs_bg_flags_to_raid_index(bctl->meta.target);

		btrfs_err(fs_info,
			  "balance: invalid convert metadata profile %s",
			  get_raid_name(index));
		ret = -EINVAL;
		goto out;
	}
	if (validate_convert_profile(&bctl->sys, allowed)) {
		int index = btrfs_bg_flags_to_raid_index(bctl->sys.target);

		btrfs_err(fs_info,
			  "balance: invalid convert system profile %s",
			  get_raid_name(index));
=======
	/*
	 * rw_devices will not change at the moment, device add/delete/replace
	 * are exclusive
	 */
	num_devices = fs_info->fs_devices->rw_devices;

	/*
	 * SINGLE profile on-disk has no profile bit, but in-memory we have a
	 * special bit for it, to make it easier to distinguish.  Thus we need
	 * to set it manually, or balance would refuse the profile.
	 */
	allowed = BTRFS_AVAIL_ALLOC_BIT_SINGLE;
	for (i = 0; i < ARRAY_SIZE(btrfs_raid_array); i++)
		if (num_devices >= btrfs_raid_array[i].devs_min)
			allowed |= btrfs_raid_array[i].bg_flag;

	if (!validate_convert_profile(fs_info, &bctl->data, allowed, "data") ||
	    !validate_convert_profile(fs_info, &bctl->meta, allowed, "metadata") ||
	    !validate_convert_profile(fs_info, &bctl->sys,  allowed, "system")) {
>>>>>>> upstream/android-13
		ret = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	/* allow to reduce meta or sys integrity only if force set */
	allowed = BTRFS_BLOCK_GROUP_DUP | BTRFS_BLOCK_GROUP_RAID1 |
			BTRFS_BLOCK_GROUP_RAID10 |
			BTRFS_BLOCK_GROUP_RAID5 |
			BTRFS_BLOCK_GROUP_RAID6;
=======
	/*
	 * Allow to reduce metadata or system integrity only if force set for
	 * profiles with redundancy (copies, parity)
	 */
	allowed = 0;
	for (i = 0; i < ARRAY_SIZE(btrfs_raid_array); i++) {
		if (btrfs_raid_array[i].ncopies >= 2 ||
		    btrfs_raid_array[i].tolerated_failures >= 1)
			allowed |= btrfs_raid_array[i].bg_flag;
	}
>>>>>>> upstream/android-13
	do {
		seq = read_seqbegin(&fs_info->profiles_lock);

		if (((bctl->sys.flags & BTRFS_BALANCE_ARGS_CONVERT) &&
		     (fs_info->avail_system_alloc_bits & allowed) &&
		     !(bctl->sys.target & allowed)) ||
		    ((bctl->meta.flags & BTRFS_BALANCE_ARGS_CONVERT) &&
		     (fs_info->avail_metadata_alloc_bits & allowed) &&
		     !(bctl->meta.target & allowed)))
<<<<<<< HEAD
			reducing_integrity = true;
		else
			reducing_integrity = false;
=======
			reducing_redundancy = true;
		else
			reducing_redundancy = false;
>>>>>>> upstream/android-13

		/* if we're not converting, the target field is uninitialized */
		meta_target = (bctl->meta.flags & BTRFS_BALANCE_ARGS_CONVERT) ?
			bctl->meta.target : fs_info->avail_metadata_alloc_bits;
		data_target = (bctl->data.flags & BTRFS_BALANCE_ARGS_CONVERT) ?
			bctl->data.target : fs_info->avail_data_alloc_bits;
	} while (read_seqretry(&fs_info->profiles_lock, seq));

<<<<<<< HEAD
	if (reducing_integrity) {
		if (bctl->flags & BTRFS_BALANCE_FORCE) {
			btrfs_info(fs_info,
				   "balance: force reducing metadata integrity");
		} else {
			btrfs_err(fs_info,
	  "balance: reduces metadata integrity, use --force if you want this");
=======
	if (reducing_redundancy) {
		if (bctl->flags & BTRFS_BALANCE_FORCE) {
			btrfs_info(fs_info,
			   "balance: force reducing metadata redundancy");
		} else {
			btrfs_err(fs_info,
	"balance: reduces metadata redundancy, use --force if you want this");
>>>>>>> upstream/android-13
			ret = -EINVAL;
			goto out;
		}
	}

	if (btrfs_get_num_tolerated_disk_barrier_failures(meta_target) <
		btrfs_get_num_tolerated_disk_barrier_failures(data_target)) {
<<<<<<< HEAD
		int meta_index = btrfs_bg_flags_to_raid_index(meta_target);
		int data_index = btrfs_bg_flags_to_raid_index(data_target);

		btrfs_warn(fs_info,
	"balance: metadata profile %s has lower redundancy than data profile %s",
			   get_raid_name(meta_index), get_raid_name(data_index));
=======
		btrfs_warn(fs_info,
	"balance: metadata profile %s has lower redundancy than data profile %s",
				btrfs_bg_type_to_raid_name(meta_target),
				btrfs_bg_type_to_raid_name(data_target));
>>>>>>> upstream/android-13
	}

	ret = insert_balance_item(fs_info, bctl);
	if (ret && ret != -EEXIST)
		goto out;

	if (!(bctl->flags & BTRFS_BALANCE_RESUME)) {
		BUG_ON(ret == -EEXIST);
		BUG_ON(fs_info->balance_ctl);
		spin_lock(&fs_info->balance_lock);
		fs_info->balance_ctl = bctl;
		spin_unlock(&fs_info->balance_lock);
	} else {
		BUG_ON(ret != -EEXIST);
		spin_lock(&fs_info->balance_lock);
		update_balance_args(bctl);
		spin_unlock(&fs_info->balance_lock);
	}

	ASSERT(!test_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags));
	set_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags);
<<<<<<< HEAD
=======
	describe_balance_start_or_resume(fs_info);
>>>>>>> upstream/android-13
	mutex_unlock(&fs_info->balance_mutex);

	ret = __btrfs_balance(fs_info);

	mutex_lock(&fs_info->balance_mutex);
<<<<<<< HEAD
=======
	if (ret == -ECANCELED && atomic_read(&fs_info->balance_pause_req))
		btrfs_info(fs_info, "balance: paused");
	/*
	 * Balance can be canceled by:
	 *
	 * - Regular cancel request
	 *   Then ret == -ECANCELED and balance_cancel_req > 0
	 *
	 * - Fatal signal to "btrfs" process
	 *   Either the signal caught by wait_reserve_ticket() and callers
	 *   got -EINTR, or caught by btrfs_should_cancel_balance() and
	 *   got -ECANCELED.
	 *   Either way, in this case balance_cancel_req = 0, and
	 *   ret == -EINTR or ret == -ECANCELED.
	 *
	 * So here we only check the return value to catch canceled balance.
	 */
	else if (ret == -ECANCELED || ret == -EINTR)
		btrfs_info(fs_info, "balance: canceled");
	else
		btrfs_info(fs_info, "balance: ended with status: %d", ret);

>>>>>>> upstream/android-13
	clear_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags);

	if (bargs) {
		memset(bargs, 0, sizeof(*bargs));
		btrfs_update_ioctl_balance_args(fs_info, bargs);
	}

	if ((ret && ret != -ECANCELED && ret != -ENOSPC) ||
	    balance_need_close(fs_info)) {
		reset_balance_state(fs_info);
<<<<<<< HEAD
		clear_bit(BTRFS_FS_EXCL_OP, &fs_info->flags);
=======
		btrfs_exclop_finish(fs_info);
>>>>>>> upstream/android-13
	}

	wake_up(&fs_info->balance_wait_q);

	return ret;
out:
	if (bctl->flags & BTRFS_BALANCE_RESUME)
		reset_balance_state(fs_info);
	else
		kfree(bctl);
<<<<<<< HEAD
	clear_bit(BTRFS_FS_EXCL_OP, &fs_info->flags);
=======
	btrfs_exclop_finish(fs_info);
>>>>>>> upstream/android-13

	return ret;
}

static int balance_kthread(void *data)
{
	struct btrfs_fs_info *fs_info = data;
	int ret = 0;

<<<<<<< HEAD
	mutex_lock(&fs_info->balance_mutex);
	if (fs_info->balance_ctl) {
		btrfs_info(fs_info, "balance: resuming");
		ret = btrfs_balance(fs_info, fs_info->balance_ctl, NULL);
	}
	mutex_unlock(&fs_info->balance_mutex);
=======
	sb_start_write(fs_info->sb);
	mutex_lock(&fs_info->balance_mutex);
	if (fs_info->balance_ctl)
		ret = btrfs_balance(fs_info, fs_info->balance_ctl, NULL);
	mutex_unlock(&fs_info->balance_mutex);
	sb_end_write(fs_info->sb);
>>>>>>> upstream/android-13

	return ret;
}

int btrfs_resume_balance_async(struct btrfs_fs_info *fs_info)
{
	struct task_struct *tsk;

	mutex_lock(&fs_info->balance_mutex);
	if (!fs_info->balance_ctl) {
		mutex_unlock(&fs_info->balance_mutex);
		return 0;
	}
	mutex_unlock(&fs_info->balance_mutex);

	if (btrfs_test_opt(fs_info, SKIP_BALANCE)) {
		btrfs_info(fs_info, "balance: resume skipped");
		return 0;
	}

	/*
	 * A ro->rw remount sequence should continue with the paused balance
	 * regardless of who pauses it, system or the user as of now, so set
	 * the resume flag.
	 */
	spin_lock(&fs_info->balance_lock);
	fs_info->balance_ctl->flags |= BTRFS_BALANCE_RESUME;
	spin_unlock(&fs_info->balance_lock);

	tsk = kthread_run(balance_kthread, fs_info, "btrfs-balance");
	return PTR_ERR_OR_ZERO(tsk);
}

int btrfs_recover_balance(struct btrfs_fs_info *fs_info)
{
	struct btrfs_balance_control *bctl;
	struct btrfs_balance_item *item;
	struct btrfs_disk_balance_args disk_bargs;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_key key;
	int ret;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	key.objectid = BTRFS_BALANCE_OBJECTID;
	key.type = BTRFS_TEMPORARY_ITEM_KEY;
	key.offset = 0;

	ret = btrfs_search_slot(NULL, fs_info->tree_root, &key, path, 0, 0);
	if (ret < 0)
		goto out;
	if (ret > 0) { /* ret = -ENOENT; */
		ret = 0;
		goto out;
	}

	bctl = kzalloc(sizeof(*bctl), GFP_NOFS);
	if (!bctl) {
		ret = -ENOMEM;
		goto out;
	}

	leaf = path->nodes[0];
	item = btrfs_item_ptr(leaf, path->slots[0], struct btrfs_balance_item);

	bctl->flags = btrfs_balance_flags(leaf, item);
	bctl->flags |= BTRFS_BALANCE_RESUME;

	btrfs_balance_data(leaf, item, &disk_bargs);
	btrfs_disk_balance_args_to_cpu(&bctl->data, &disk_bargs);
	btrfs_balance_meta(leaf, item, &disk_bargs);
	btrfs_disk_balance_args_to_cpu(&bctl->meta, &disk_bargs);
	btrfs_balance_sys(leaf, item, &disk_bargs);
	btrfs_disk_balance_args_to_cpu(&bctl->sys, &disk_bargs);

	/*
	 * This should never happen, as the paused balance state is recovered
	 * during mount without any chance of other exclusive ops to collide.
	 *
	 * This gives the exclusive op status to balance and keeps in paused
	 * state until user intervention (cancel or umount). If the ownership
	 * cannot be assigned, show a message but do not fail. The balance
	 * is in a paused state and must have fs_info::balance_ctl properly
	 * set up.
	 */
<<<<<<< HEAD
	if (test_and_set_bit(BTRFS_FS_EXCL_OP, &fs_info->flags))
=======
	if (!btrfs_exclop_start(fs_info, BTRFS_EXCLOP_BALANCE))
>>>>>>> upstream/android-13
		btrfs_warn(fs_info,
	"balance: cannot set exclusive op status, resume manually");

	btrfs_release_path(path);

	mutex_lock(&fs_info->balance_mutex);
	BUG_ON(fs_info->balance_ctl);
	spin_lock(&fs_info->balance_lock);
	fs_info->balance_ctl = bctl;
	spin_unlock(&fs_info->balance_lock);
	mutex_unlock(&fs_info->balance_mutex);
out:
	btrfs_free_path(path);
	return ret;
}

int btrfs_pause_balance(struct btrfs_fs_info *fs_info)
{
	int ret = 0;

	mutex_lock(&fs_info->balance_mutex);
	if (!fs_info->balance_ctl) {
		mutex_unlock(&fs_info->balance_mutex);
		return -ENOTCONN;
	}

	if (test_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags)) {
		atomic_inc(&fs_info->balance_pause_req);
		mutex_unlock(&fs_info->balance_mutex);

		wait_event(fs_info->balance_wait_q,
			   !test_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags));

		mutex_lock(&fs_info->balance_mutex);
		/* we are good with balance_ctl ripped off from under us */
		BUG_ON(test_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags));
		atomic_dec(&fs_info->balance_pause_req);
	} else {
		ret = -ENOTCONN;
	}

	mutex_unlock(&fs_info->balance_mutex);
	return ret;
}

int btrfs_cancel_balance(struct btrfs_fs_info *fs_info)
{
	mutex_lock(&fs_info->balance_mutex);
	if (!fs_info->balance_ctl) {
		mutex_unlock(&fs_info->balance_mutex);
		return -ENOTCONN;
	}

	/*
	 * A paused balance with the item stored on disk can be resumed at
	 * mount time if the mount is read-write. Otherwise it's still paused
	 * and we must not allow cancelling as it deletes the item.
	 */
	if (sb_rdonly(fs_info->sb)) {
		mutex_unlock(&fs_info->balance_mutex);
		return -EROFS;
	}

	atomic_inc(&fs_info->balance_cancel_req);
	/*
	 * if we are running just wait and return, balance item is
	 * deleted in btrfs_balance in this case
	 */
	if (test_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags)) {
		mutex_unlock(&fs_info->balance_mutex);
		wait_event(fs_info->balance_wait_q,
			   !test_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags));
		mutex_lock(&fs_info->balance_mutex);
	} else {
		mutex_unlock(&fs_info->balance_mutex);
		/*
		 * Lock released to allow other waiters to continue, we'll
		 * reexamine the status again.
		 */
		mutex_lock(&fs_info->balance_mutex);

		if (fs_info->balance_ctl) {
			reset_balance_state(fs_info);
<<<<<<< HEAD
			clear_bit(BTRFS_FS_EXCL_OP, &fs_info->flags);
=======
			btrfs_exclop_finish(fs_info);
>>>>>>> upstream/android-13
			btrfs_info(fs_info, "balance: canceled");
		}
	}

	BUG_ON(fs_info->balance_ctl ||
		test_bit(BTRFS_FS_BALANCE_RUNNING, &fs_info->flags));
	atomic_dec(&fs_info->balance_cancel_req);
	mutex_unlock(&fs_info->balance_mutex);
	return 0;
}

<<<<<<< HEAD
static int btrfs_uuid_scan_kthread(void *data)
=======
int btrfs_uuid_scan_kthread(void *data)
>>>>>>> upstream/android-13
{
	struct btrfs_fs_info *fs_info = data;
	struct btrfs_root *root = fs_info->tree_root;
	struct btrfs_key key;
	struct btrfs_path *path = NULL;
	int ret = 0;
	struct extent_buffer *eb;
	int slot;
	struct btrfs_root_item root_item;
	u32 item_size;
	struct btrfs_trans_handle *trans = NULL;
<<<<<<< HEAD
=======
	bool closing = false;
>>>>>>> upstream/android-13

	path = btrfs_alloc_path();
	if (!path) {
		ret = -ENOMEM;
		goto out;
	}

	key.objectid = 0;
	key.type = BTRFS_ROOT_ITEM_KEY;
	key.offset = 0;

	while (1) {
<<<<<<< HEAD
=======
		if (btrfs_fs_closing(fs_info)) {
			closing = true;
			break;
		}
>>>>>>> upstream/android-13
		ret = btrfs_search_forward(root, &key, path,
				BTRFS_OLDEST_GENERATION);
		if (ret) {
			if (ret > 0)
				ret = 0;
			break;
		}

		if (key.type != BTRFS_ROOT_ITEM_KEY ||
		    (key.objectid < BTRFS_FIRST_FREE_OBJECTID &&
		     key.objectid != BTRFS_FS_TREE_OBJECTID) ||
		    key.objectid > BTRFS_LAST_FREE_OBJECTID)
			goto skip;

		eb = path->nodes[0];
		slot = path->slots[0];
		item_size = btrfs_item_size_nr(eb, slot);
		if (item_size < sizeof(root_item))
			goto skip;

		read_extent_buffer(eb, &root_item,
				   btrfs_item_ptr_offset(eb, slot),
				   (int)sizeof(root_item));
		if (btrfs_root_refs(&root_item) == 0)
			goto skip;

		if (!btrfs_is_empty_uuid(root_item.uuid) ||
		    !btrfs_is_empty_uuid(root_item.received_uuid)) {
			if (trans)
				goto update_tree;

			btrfs_release_path(path);
			/*
			 * 1 - subvol uuid item
			 * 1 - received_subvol uuid item
			 */
			trans = btrfs_start_transaction(fs_info->uuid_root, 2);
			if (IS_ERR(trans)) {
				ret = PTR_ERR(trans);
				break;
			}
			continue;
		} else {
			goto skip;
		}
update_tree:
		btrfs_release_path(path);
		if (!btrfs_is_empty_uuid(root_item.uuid)) {
			ret = btrfs_uuid_tree_add(trans, root_item.uuid,
						  BTRFS_UUID_KEY_SUBVOL,
						  key.objectid);
			if (ret < 0) {
				btrfs_warn(fs_info, "uuid_tree_add failed %d",
					ret);
				break;
			}
		}

		if (!btrfs_is_empty_uuid(root_item.received_uuid)) {
			ret = btrfs_uuid_tree_add(trans,
						  root_item.received_uuid,
						 BTRFS_UUID_KEY_RECEIVED_SUBVOL,
						  key.objectid);
			if (ret < 0) {
				btrfs_warn(fs_info, "uuid_tree_add failed %d",
					ret);
				break;
			}
		}

skip:
		btrfs_release_path(path);
		if (trans) {
			ret = btrfs_end_transaction(trans);
			trans = NULL;
			if (ret)
				break;
		}

		if (key.offset < (u64)-1) {
			key.offset++;
		} else if (key.type < BTRFS_ROOT_ITEM_KEY) {
			key.offset = 0;
			key.type = BTRFS_ROOT_ITEM_KEY;
		} else if (key.objectid < (u64)-1) {
			key.offset = 0;
			key.type = BTRFS_ROOT_ITEM_KEY;
			key.objectid++;
		} else {
			break;
		}
		cond_resched();
	}

out:
	btrfs_free_path(path);
	if (trans && !IS_ERR(trans))
		btrfs_end_transaction(trans);
	if (ret)
		btrfs_warn(fs_info, "btrfs_uuid_scan_kthread failed %d", ret);
<<<<<<< HEAD
	else
=======
	else if (!closing)
>>>>>>> upstream/android-13
		set_bit(BTRFS_FS_UPDATE_UUID_TREE_GEN, &fs_info->flags);
	up(&fs_info->uuid_tree_rescan_sem);
	return 0;
}

<<<<<<< HEAD
/*
 * Callback for btrfs_uuid_tree_iterate().
 * returns:
 * 0	check succeeded, the entry is not outdated.
 * < 0	if an error occurred.
 * > 0	if the check failed, which means the caller shall remove the entry.
 */
static int btrfs_check_uuid_tree_entry(struct btrfs_fs_info *fs_info,
				       u8 *uuid, u8 type, u64 subid)
{
	struct btrfs_key key;
	int ret = 0;
	struct btrfs_root *subvol_root;

	if (type != BTRFS_UUID_KEY_SUBVOL &&
	    type != BTRFS_UUID_KEY_RECEIVED_SUBVOL)
		goto out;

	key.objectid = subid;
	key.type = BTRFS_ROOT_ITEM_KEY;
	key.offset = (u64)-1;
	subvol_root = btrfs_read_fs_root_no_name(fs_info, &key);
	if (IS_ERR(subvol_root)) {
		ret = PTR_ERR(subvol_root);
		if (ret == -ENOENT)
			ret = 1;
		goto out;
	}

	switch (type) {
	case BTRFS_UUID_KEY_SUBVOL:
		if (memcmp(uuid, subvol_root->root_item.uuid, BTRFS_UUID_SIZE))
			ret = 1;
		break;
	case BTRFS_UUID_KEY_RECEIVED_SUBVOL:
		if (memcmp(uuid, subvol_root->root_item.received_uuid,
			   BTRFS_UUID_SIZE))
			ret = 1;
		break;
	}

out:
	return ret;
}

static int btrfs_uuid_rescan_kthread(void *data)
{
	struct btrfs_fs_info *fs_info = (struct btrfs_fs_info *)data;
	int ret;

	/*
	 * 1st step is to iterate through the existing UUID tree and
	 * to delete all entries that contain outdated data.
	 * 2nd step is to add all missing entries to the UUID tree.
	 */
	ret = btrfs_uuid_tree_iterate(fs_info, btrfs_check_uuid_tree_entry);
	if (ret < 0) {
		btrfs_warn(fs_info, "iterating uuid_tree failed %d", ret);
		up(&fs_info->uuid_tree_rescan_sem);
		return ret;
	}
	return btrfs_uuid_scan_kthread(data);
}

=======
>>>>>>> upstream/android-13
int btrfs_create_uuid_tree(struct btrfs_fs_info *fs_info)
{
	struct btrfs_trans_handle *trans;
	struct btrfs_root *tree_root = fs_info->tree_root;
	struct btrfs_root *uuid_root;
	struct task_struct *task;
	int ret;

	/*
	 * 1 - root node
	 * 1 - root item
	 */
	trans = btrfs_start_transaction(tree_root, 2);
	if (IS_ERR(trans))
		return PTR_ERR(trans);

<<<<<<< HEAD
	uuid_root = btrfs_create_tree(trans, fs_info,
				      BTRFS_UUID_TREE_OBJECTID);
=======
	uuid_root = btrfs_create_tree(trans, BTRFS_UUID_TREE_OBJECTID);
>>>>>>> upstream/android-13
	if (IS_ERR(uuid_root)) {
		ret = PTR_ERR(uuid_root);
		btrfs_abort_transaction(trans, ret);
		btrfs_end_transaction(trans);
		return ret;
	}

	fs_info->uuid_root = uuid_root;

	ret = btrfs_commit_transaction(trans);
	if (ret)
		return ret;

	down(&fs_info->uuid_tree_rescan_sem);
	task = kthread_run(btrfs_uuid_scan_kthread, fs_info, "btrfs-uuid");
	if (IS_ERR(task)) {
		/* fs_info->update_uuid_tree_gen remains 0 in all error case */
		btrfs_warn(fs_info, "failed to start uuid_scan task");
		up(&fs_info->uuid_tree_rescan_sem);
		return PTR_ERR(task);
	}

	return 0;
}

<<<<<<< HEAD
int btrfs_check_uuid_tree(struct btrfs_fs_info *fs_info)
{
	struct task_struct *task;

	down(&fs_info->uuid_tree_rescan_sem);
	task = kthread_run(btrfs_uuid_rescan_kthread, fs_info, "btrfs-uuid");
	if (IS_ERR(task)) {
		/* fs_info->update_uuid_tree_gen remains 0 in all error case */
		btrfs_warn(fs_info, "failed to start uuid_rescan task");
		up(&fs_info->uuid_tree_rescan_sem);
		return PTR_ERR(task);
	}

	return 0;
}

=======
>>>>>>> upstream/android-13
/*
 * shrinking a device means finding all of the device extents past
 * the new size, and then following the back refs to the chunks.
 * The chunk relocation code actually frees the device extent
 */
int btrfs_shrink_device(struct btrfs_device *device, u64 new_size)
{
	struct btrfs_fs_info *fs_info = device->fs_info;
	struct btrfs_root *root = fs_info->dev_root;
	struct btrfs_trans_handle *trans;
	struct btrfs_dev_extent *dev_extent = NULL;
	struct btrfs_path *path;
	u64 length;
	u64 chunk_offset;
	int ret;
	int slot;
	int failed = 0;
	bool retried = false;
<<<<<<< HEAD
	bool checked_pending_chunks = false;
=======
>>>>>>> upstream/android-13
	struct extent_buffer *l;
	struct btrfs_key key;
	struct btrfs_super_block *super_copy = fs_info->super_copy;
	u64 old_total = btrfs_super_total_bytes(super_copy);
	u64 old_size = btrfs_device_get_total_bytes(device);
	u64 diff;
<<<<<<< HEAD

	new_size = round_down(new_size, fs_info->sectorsize);
=======
	u64 start;

	new_size = round_down(new_size, fs_info->sectorsize);
	start = new_size;
>>>>>>> upstream/android-13
	diff = round_down(old_size - new_size, fs_info->sectorsize);

	if (test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state))
		return -EINVAL;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	path->reada = READA_BACK;

<<<<<<< HEAD
=======
	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans)) {
		btrfs_free_path(path);
		return PTR_ERR(trans);
	}

>>>>>>> upstream/android-13
	mutex_lock(&fs_info->chunk_mutex);

	btrfs_device_set_total_bytes(device, new_size);
	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state)) {
		device->fs_devices->total_rw_bytes -= diff;
		atomic64_sub(diff, &fs_info->free_chunk_space);
	}
<<<<<<< HEAD
	mutex_unlock(&fs_info->chunk_mutex);
=======

	/*
	 * Once the device's size has been set to the new size, ensure all
	 * in-memory chunks are synced to disk so that the loop below sees them
	 * and relocates them accordingly.
	 */
	if (contains_pending_extent(device, &start, diff)) {
		mutex_unlock(&fs_info->chunk_mutex);
		ret = btrfs_commit_transaction(trans);
		if (ret)
			goto done;
	} else {
		mutex_unlock(&fs_info->chunk_mutex);
		btrfs_end_transaction(trans);
	}
>>>>>>> upstream/android-13

again:
	key.objectid = device->devid;
	key.offset = (u64)-1;
	key.type = BTRFS_DEV_EXTENT_KEY;

	do {
<<<<<<< HEAD
		mutex_lock(&fs_info->delete_unused_bgs_mutex);
		ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
		if (ret < 0) {
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
		mutex_lock(&fs_info->reclaim_bgs_lock);
		ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
		if (ret < 0) {
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			goto done;
		}

		ret = btrfs_previous_item(root, path, 0, key.type);
<<<<<<< HEAD
		if (ret)
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
		if (ret < 0)
			goto done;
		if (ret) {
=======
		if (ret) {
			mutex_unlock(&fs_info->reclaim_bgs_lock);
			if (ret < 0)
				goto done;
>>>>>>> upstream/android-13
			ret = 0;
			btrfs_release_path(path);
			break;
		}

		l = path->nodes[0];
		slot = path->slots[0];
		btrfs_item_key_to_cpu(l, &key, path->slots[0]);

		if (key.objectid != device->devid) {
<<<<<<< HEAD
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			btrfs_release_path(path);
			break;
		}

		dev_extent = btrfs_item_ptr(l, slot, struct btrfs_dev_extent);
		length = btrfs_dev_extent_length(l, dev_extent);

		if (key.offset + length <= new_size) {
<<<<<<< HEAD
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			btrfs_release_path(path);
			break;
		}

		chunk_offset = btrfs_dev_extent_chunk_offset(l, dev_extent);
		btrfs_release_path(path);

		/*
		 * We may be relocating the only data chunk we have,
		 * which could potentially end up with losing data's
		 * raid profile, so lets allocate an empty one in
		 * advance.
		 */
		ret = btrfs_may_alloc_data_chunk(fs_info, chunk_offset);
		if (ret < 0) {
<<<<<<< HEAD
			mutex_unlock(&fs_info->delete_unused_bgs_mutex);
=======
			mutex_unlock(&fs_info->reclaim_bgs_lock);
>>>>>>> upstream/android-13
			goto done;
		}

		ret = btrfs_relocate_chunk(fs_info, chunk_offset);
<<<<<<< HEAD
		mutex_unlock(&fs_info->delete_unused_bgs_mutex);
		if (ret && ret != -ENOSPC)
			goto done;
		if (ret == -ENOSPC)
			failed++;
=======
		mutex_unlock(&fs_info->reclaim_bgs_lock);
		if (ret == -ENOSPC) {
			failed++;
		} else if (ret) {
			if (ret == -ETXTBSY) {
				btrfs_warn(fs_info,
		   "could not shrink block group %llu due to active swapfile",
					   chunk_offset);
			}
			goto done;
		}
>>>>>>> upstream/android-13
	} while (key.offset-- > 0);

	if (failed && !retried) {
		failed = 0;
		retried = true;
		goto again;
	} else if (failed && retried) {
		ret = -ENOSPC;
		goto done;
	}

	/* Shrinking succeeded, else we would be at "done". */
	trans = btrfs_start_transaction(root, 0);
	if (IS_ERR(trans)) {
		ret = PTR_ERR(trans);
		goto done;
	}

	mutex_lock(&fs_info->chunk_mutex);
<<<<<<< HEAD

	/*
	 * We checked in the above loop all device extents that were already in
	 * the device tree. However before we have updated the device's
	 * total_bytes to the new size, we might have had chunk allocations that
	 * have not complete yet (new block groups attached to transaction
	 * handles), and therefore their device extents were not yet in the
	 * device tree and we missed them in the loop above. So if we have any
	 * pending chunk using a device extent that overlaps the device range
	 * that we can not use anymore, commit the current transaction and
	 * repeat the search on the device tree - this way we guarantee we will
	 * not have chunks using device extents that end beyond 'new_size'.
	 */
	if (!checked_pending_chunks) {
		u64 start = new_size;
		u64 len = old_size - new_size;

		if (contains_pending_extent(trans->transaction, device,
					    &start, len)) {
			mutex_unlock(&fs_info->chunk_mutex);
			checked_pending_chunks = true;
			failed = 0;
			retried = false;
			ret = btrfs_commit_transaction(trans);
			if (ret)
				goto done;
			goto again;
		}
	}

	btrfs_device_set_disk_total_bytes(device, new_size);
	if (list_empty(&device->resized_list))
		list_add_tail(&device->resized_list,
			      &fs_info->fs_devices->resized_devices);
=======
	/* Clear all state bits beyond the shrunk device size */
	clear_extent_bits(&device->alloc_state, new_size, (u64)-1,
			  CHUNK_STATE_MASK);

	btrfs_device_set_disk_total_bytes(device, new_size);
	if (list_empty(&device->post_commit_list))
		list_add_tail(&device->post_commit_list,
			      &trans->transaction->dev_update_list);
>>>>>>> upstream/android-13

	WARN_ON(diff > old_total);
	btrfs_set_super_total_bytes(super_copy,
			round_down(old_total - diff, fs_info->sectorsize));
	mutex_unlock(&fs_info->chunk_mutex);

	/* Now btrfs_update_device() will change the on-disk size. */
	ret = btrfs_update_device(trans, device);
	if (ret < 0) {
		btrfs_abort_transaction(trans, ret);
		btrfs_end_transaction(trans);
	} else {
		ret = btrfs_commit_transaction(trans);
	}
done:
	btrfs_free_path(path);
	if (ret) {
		mutex_lock(&fs_info->chunk_mutex);
		btrfs_device_set_total_bytes(device, old_size);
		if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state))
			device->fs_devices->total_rw_bytes += diff;
		atomic64_add(diff, &fs_info->free_chunk_space);
		mutex_unlock(&fs_info->chunk_mutex);
	}
	return ret;
}

static int btrfs_add_system_chunk(struct btrfs_fs_info *fs_info,
			   struct btrfs_key *key,
			   struct btrfs_chunk *chunk, int item_size)
{
	struct btrfs_super_block *super_copy = fs_info->super_copy;
	struct btrfs_disk_key disk_key;
	u32 array_size;
	u8 *ptr;

<<<<<<< HEAD
	mutex_lock(&fs_info->chunk_mutex);
	array_size = btrfs_super_sys_array_size(super_copy);
	if (array_size + item_size + sizeof(disk_key)
			> BTRFS_SYSTEM_CHUNK_ARRAY_SIZE) {
		mutex_unlock(&fs_info->chunk_mutex);
		return -EFBIG;
	}
=======
	lockdep_assert_held(&fs_info->chunk_mutex);

	array_size = btrfs_super_sys_array_size(super_copy);
	if (array_size + item_size + sizeof(disk_key)
			> BTRFS_SYSTEM_CHUNK_ARRAY_SIZE)
		return -EFBIG;
>>>>>>> upstream/android-13

	ptr = super_copy->sys_chunk_array + array_size;
	btrfs_cpu_key_to_disk(&disk_key, key);
	memcpy(ptr, &disk_key, sizeof(disk_key));
	ptr += sizeof(disk_key);
	memcpy(ptr, chunk, item_size);
	item_size += sizeof(disk_key);
	btrfs_set_super_sys_array_size(super_copy, array_size + item_size);
<<<<<<< HEAD
	mutex_unlock(&fs_info->chunk_mutex);
=======
>>>>>>> upstream/android-13

	return 0;
}

/*
 * sort the devices in descending order by max_avail, total_avail
 */
static int btrfs_cmp_device_info(const void *a, const void *b)
{
	const struct btrfs_device_info *di_a = a;
	const struct btrfs_device_info *di_b = b;

	if (di_a->max_avail > di_b->max_avail)
		return -1;
	if (di_a->max_avail < di_b->max_avail)
		return 1;
	if (di_a->total_avail > di_b->total_avail)
		return -1;
	if (di_a->total_avail < di_b->total_avail)
		return 1;
	return 0;
}

static void check_raid56_incompat_flag(struct btrfs_fs_info *info, u64 type)
{
	if (!(type & BTRFS_BLOCK_GROUP_RAID56_MASK))
		return;

	btrfs_set_fs_incompat(info, RAID56);
}

<<<<<<< HEAD
static int __btrfs_alloc_chunk(struct btrfs_trans_handle *trans,
			       u64 start, u64 type)
{
	struct btrfs_fs_info *info = trans->fs_info;
	struct btrfs_fs_devices *fs_devices = info->fs_devices;
	struct btrfs_device *device;
	struct map_lookup *map = NULL;
	struct extent_map_tree *em_tree;
	struct extent_map *em;
	struct btrfs_device_info *devices_info = NULL;
	u64 total_avail;
	int num_stripes;	/* total number of stripes to allocate */
	int data_stripes;	/* number of stripes that count for
				   block group size */
	int sub_stripes;	/* sub_stripes info for map */
	int dev_stripes;	/* stripes per dev */
	int devs_max;		/* max devs to use */
	int devs_min;		/* min devs needed */
	int devs_increment;	/* ndevs has to be a multiple of this */
	int ncopies;		/* how many copies to data has */
	int ret;
	u64 max_stripe_size;
	u64 max_chunk_size;
	u64 stripe_size;
	u64 num_bytes;
	int ndevs;
	int i;
	int j;
	int index;

	BUG_ON(!alloc_profile_is_valid(type, 0));

	if (list_empty(&fs_devices->alloc_list)) {
		if (btrfs_test_opt(info, ENOSPC_DEBUG))
			btrfs_debug(info, "%s: no writable device", __func__);
		return -ENOSPC;
	}

	index = btrfs_bg_flags_to_raid_index(type);

	sub_stripes = btrfs_raid_array[index].sub_stripes;
	dev_stripes = btrfs_raid_array[index].dev_stripes;
	devs_max = btrfs_raid_array[index].devs_max;
	devs_min = btrfs_raid_array[index].devs_min;
	devs_increment = btrfs_raid_array[index].devs_increment;
	ncopies = btrfs_raid_array[index].ncopies;

	if (type & BTRFS_BLOCK_GROUP_DATA) {
		max_stripe_size = SZ_1G;
		max_chunk_size = BTRFS_MAX_DATA_CHUNK_SIZE;
		if (!devs_max)
			devs_max = BTRFS_MAX_DEVS(info);
	} else if (type & BTRFS_BLOCK_GROUP_METADATA) {
		/* for larger filesystems, use larger metadata chunks */
		if (fs_devices->total_rw_bytes > 50ULL * SZ_1G)
			max_stripe_size = SZ_1G;
		else
			max_stripe_size = SZ_256M;
		max_chunk_size = max_stripe_size;
		if (!devs_max)
			devs_max = BTRFS_MAX_DEVS(info);
	} else if (type & BTRFS_BLOCK_GROUP_SYSTEM) {
		max_stripe_size = SZ_32M;
		max_chunk_size = 2 * max_stripe_size;
		if (!devs_max)
			devs_max = BTRFS_MAX_DEVS_SYS_CHUNK;
	} else {
		btrfs_err(info, "invalid chunk type 0x%llx requested",
		       type);
		BUG_ON(1);
	}

	/* we don't want a chunk larger than 10% of writeable space */
	max_chunk_size = min(div_factor(fs_devices->total_rw_bytes, 1),
			     max_chunk_size);

	devices_info = kcalloc(fs_devices->rw_devices, sizeof(*devices_info),
			       GFP_NOFS);
	if (!devices_info)
		return -ENOMEM;
=======
static void check_raid1c34_incompat_flag(struct btrfs_fs_info *info, u64 type)
{
	if (!(type & (BTRFS_BLOCK_GROUP_RAID1C3 | BTRFS_BLOCK_GROUP_RAID1C4)))
		return;

	btrfs_set_fs_incompat(info, RAID1C34);
}

/*
 * Structure used internally for __btrfs_alloc_chunk() function.
 * Wraps needed parameters.
 */
struct alloc_chunk_ctl {
	u64 start;
	u64 type;
	/* Total number of stripes to allocate */
	int num_stripes;
	/* sub_stripes info for map */
	int sub_stripes;
	/* Stripes per device */
	int dev_stripes;
	/* Maximum number of devices to use */
	int devs_max;
	/* Minimum number of devices to use */
	int devs_min;
	/* ndevs has to be a multiple of this */
	int devs_increment;
	/* Number of copies */
	int ncopies;
	/* Number of stripes worth of bytes to store parity information */
	int nparity;
	u64 max_stripe_size;
	u64 max_chunk_size;
	u64 dev_extent_min;
	u64 stripe_size;
	u64 chunk_size;
	int ndevs;
};

static void init_alloc_chunk_ctl_policy_regular(
				struct btrfs_fs_devices *fs_devices,
				struct alloc_chunk_ctl *ctl)
{
	u64 type = ctl->type;

	if (type & BTRFS_BLOCK_GROUP_DATA) {
		ctl->max_stripe_size = SZ_1G;
		ctl->max_chunk_size = BTRFS_MAX_DATA_CHUNK_SIZE;
	} else if (type & BTRFS_BLOCK_GROUP_METADATA) {
		/* For larger filesystems, use larger metadata chunks */
		if (fs_devices->total_rw_bytes > 50ULL * SZ_1G)
			ctl->max_stripe_size = SZ_1G;
		else
			ctl->max_stripe_size = SZ_256M;
		ctl->max_chunk_size = ctl->max_stripe_size;
	} else if (type & BTRFS_BLOCK_GROUP_SYSTEM) {
		ctl->max_stripe_size = SZ_32M;
		ctl->max_chunk_size = 2 * ctl->max_stripe_size;
		ctl->devs_max = min_t(int, ctl->devs_max,
				      BTRFS_MAX_DEVS_SYS_CHUNK);
	} else {
		BUG();
	}

	/* We don't want a chunk larger than 10% of writable space */
	ctl->max_chunk_size = min(div_factor(fs_devices->total_rw_bytes, 1),
				  ctl->max_chunk_size);
	ctl->dev_extent_min = BTRFS_STRIPE_LEN * ctl->dev_stripes;
}

static void init_alloc_chunk_ctl_policy_zoned(
				      struct btrfs_fs_devices *fs_devices,
				      struct alloc_chunk_ctl *ctl)
{
	u64 zone_size = fs_devices->fs_info->zone_size;
	u64 limit;
	int min_num_stripes = ctl->devs_min * ctl->dev_stripes;
	int min_data_stripes = (min_num_stripes - ctl->nparity) / ctl->ncopies;
	u64 min_chunk_size = min_data_stripes * zone_size;
	u64 type = ctl->type;

	ctl->max_stripe_size = zone_size;
	if (type & BTRFS_BLOCK_GROUP_DATA) {
		ctl->max_chunk_size = round_down(BTRFS_MAX_DATA_CHUNK_SIZE,
						 zone_size);
	} else if (type & BTRFS_BLOCK_GROUP_METADATA) {
		ctl->max_chunk_size = ctl->max_stripe_size;
	} else if (type & BTRFS_BLOCK_GROUP_SYSTEM) {
		ctl->max_chunk_size = 2 * ctl->max_stripe_size;
		ctl->devs_max = min_t(int, ctl->devs_max,
				      BTRFS_MAX_DEVS_SYS_CHUNK);
	} else {
		BUG();
	}

	/* We don't want a chunk larger than 10% of writable space */
	limit = max(round_down(div_factor(fs_devices->total_rw_bytes, 1),
			       zone_size),
		    min_chunk_size);
	ctl->max_chunk_size = min(limit, ctl->max_chunk_size);
	ctl->dev_extent_min = zone_size * ctl->dev_stripes;
}

static void init_alloc_chunk_ctl(struct btrfs_fs_devices *fs_devices,
				 struct alloc_chunk_ctl *ctl)
{
	int index = btrfs_bg_flags_to_raid_index(ctl->type);

	ctl->sub_stripes = btrfs_raid_array[index].sub_stripes;
	ctl->dev_stripes = btrfs_raid_array[index].dev_stripes;
	ctl->devs_max = btrfs_raid_array[index].devs_max;
	if (!ctl->devs_max)
		ctl->devs_max = BTRFS_MAX_DEVS(fs_devices->fs_info);
	ctl->devs_min = btrfs_raid_array[index].devs_min;
	ctl->devs_increment = btrfs_raid_array[index].devs_increment;
	ctl->ncopies = btrfs_raid_array[index].ncopies;
	ctl->nparity = btrfs_raid_array[index].nparity;
	ctl->ndevs = 0;

	switch (fs_devices->chunk_alloc_policy) {
	case BTRFS_CHUNK_ALLOC_REGULAR:
		init_alloc_chunk_ctl_policy_regular(fs_devices, ctl);
		break;
	case BTRFS_CHUNK_ALLOC_ZONED:
		init_alloc_chunk_ctl_policy_zoned(fs_devices, ctl);
		break;
	default:
		BUG();
	}
}

static int gather_device_info(struct btrfs_fs_devices *fs_devices,
			      struct alloc_chunk_ctl *ctl,
			      struct btrfs_device_info *devices_info)
{
	struct btrfs_fs_info *info = fs_devices->fs_info;
	struct btrfs_device *device;
	u64 total_avail;
	u64 dev_extent_want = ctl->max_stripe_size * ctl->dev_stripes;
	int ret;
	int ndevs = 0;
	u64 max_avail;
	u64 dev_offset;
>>>>>>> upstream/android-13

	/*
	 * in the first pass through the devices list, we gather information
	 * about the available holes on each device.
	 */
<<<<<<< HEAD
	ndevs = 0;
	list_for_each_entry(device, &fs_devices->alloc_list, dev_alloc_list) {
		u64 max_avail;
		u64 dev_offset;

=======
	list_for_each_entry(device, &fs_devices->alloc_list, dev_alloc_list) {
>>>>>>> upstream/android-13
		if (!test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state)) {
			WARN(1, KERN_ERR
			       "BTRFS: read-only device in alloc_list\n");
			continue;
		}

		if (!test_bit(BTRFS_DEV_STATE_IN_FS_METADATA,
					&device->dev_state) ||
		    test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state))
			continue;

		if (device->total_bytes > device->bytes_used)
			total_avail = device->total_bytes - device->bytes_used;
		else
			total_avail = 0;

		/* If there is no space on this device, skip it. */
<<<<<<< HEAD
		if (total_avail == 0)
			continue;

		ret = find_free_dev_extent(trans, device,
					   max_stripe_size * dev_stripes,
					   &dev_offset, &max_avail);
		if (ret && ret != -ENOSPC)
			goto error;

		if (ret == 0)
			max_avail = max_stripe_size * dev_stripes;

		if (max_avail < BTRFS_STRIPE_LEN * dev_stripes) {
			if (btrfs_test_opt(info, ENOSPC_DEBUG))
				btrfs_debug(info,
			"%s: devid %llu has no free space, have=%llu want=%u",
					    __func__, device->devid, max_avail,
					    BTRFS_STRIPE_LEN * dev_stripes);
=======
		if (total_avail < ctl->dev_extent_min)
			continue;

		ret = find_free_dev_extent(device, dev_extent_want, &dev_offset,
					   &max_avail);
		if (ret && ret != -ENOSPC)
			return ret;

		if (ret == 0)
			max_avail = dev_extent_want;

		if (max_avail < ctl->dev_extent_min) {
			if (btrfs_test_opt(info, ENOSPC_DEBUG))
				btrfs_debug(info,
			"%s: devid %llu has no free space, have=%llu want=%llu",
					    __func__, device->devid, max_avail,
					    ctl->dev_extent_min);
>>>>>>> upstream/android-13
			continue;
		}

		if (ndevs == fs_devices->rw_devices) {
			WARN(1, "%s: found more than %llu devices\n",
			     __func__, fs_devices->rw_devices);
			break;
		}
		devices_info[ndevs].dev_offset = dev_offset;
		devices_info[ndevs].max_avail = max_avail;
		devices_info[ndevs].total_avail = total_avail;
		devices_info[ndevs].dev = device;
		++ndevs;
	}
<<<<<<< HEAD
=======
	ctl->ndevs = ndevs;
>>>>>>> upstream/android-13

	/*
	 * now sort the devices by hole size / available space
	 */
	sort(devices_info, ndevs, sizeof(struct btrfs_device_info),
	     btrfs_cmp_device_info, NULL);

<<<<<<< HEAD
	/* round down to number of usable stripes */
	ndevs = round_down(ndevs, devs_increment);

	if (ndevs < devs_min) {
		ret = -ENOSPC;
		if (btrfs_test_opt(info, ENOSPC_DEBUG)) {
			btrfs_debug(info,
	"%s: not enough devices with free space: have=%d minimum required=%d",
				    __func__, ndevs, devs_min);
		}
		goto error;
	}

	ndevs = min(ndevs, devs_max);
=======
	return 0;
}

static int decide_stripe_size_regular(struct alloc_chunk_ctl *ctl,
				      struct btrfs_device_info *devices_info)
{
	/* Number of stripes that count for block group size */
	int data_stripes;
>>>>>>> upstream/android-13

	/*
	 * The primary goal is to maximize the number of stripes, so use as
	 * many devices as possible, even if the stripes are not maximum sized.
	 *
	 * The DUP profile stores more than one stripe per device, the
	 * max_avail is the total size so we have to adjust.
	 */
<<<<<<< HEAD
	stripe_size = div_u64(devices_info[ndevs - 1].max_avail, dev_stripes);
	num_stripes = ndevs * dev_stripes;

	/*
	 * this will have to be fixed for RAID1 and RAID10 over
	 * more drives
	 */
	data_stripes = num_stripes / ncopies;

	if (type & BTRFS_BLOCK_GROUP_RAID5)
		data_stripes = num_stripes - 1;

	if (type & BTRFS_BLOCK_GROUP_RAID6)
		data_stripes = num_stripes - 2;

	/*
	 * Use the number of data stripes to figure out how big this chunk
	 * is really going to be in terms of logical address space,
	 * and compare that answer with the max chunk size. If it's higher,
	 * we try to reduce stripe_size.
	 */
	if (stripe_size * data_stripes > max_chunk_size) {
=======
	ctl->stripe_size = div_u64(devices_info[ctl->ndevs - 1].max_avail,
				   ctl->dev_stripes);
	ctl->num_stripes = ctl->ndevs * ctl->dev_stripes;

	/* This will have to be fixed for RAID1 and RAID10 over more drives */
	data_stripes = (ctl->num_stripes - ctl->nparity) / ctl->ncopies;

	/*
	 * Use the number of data stripes to figure out how big this chunk is
	 * really going to be in terms of logical address space, and compare
	 * that answer with the max chunk size. If it's higher, we try to
	 * reduce stripe_size.
	 */
	if (ctl->stripe_size * data_stripes > ctl->max_chunk_size) {
>>>>>>> upstream/android-13
		/*
		 * Reduce stripe_size, round it up to a 16MB boundary again and
		 * then use it, unless it ends up being even bigger than the
		 * previous value we had already.
		 */
<<<<<<< HEAD
		stripe_size = min(round_up(div_u64(max_chunk_size,
						   data_stripes), SZ_16M),
				  stripe_size);
	}

	/* align to BTRFS_STRIPE_LEN */
	stripe_size = round_down(stripe_size, BTRFS_STRIPE_LEN);

	map = kmalloc(map_lookup_size(num_stripes), GFP_NOFS);
	if (!map) {
		ret = -ENOMEM;
		goto error;
	}
	map->num_stripes = num_stripes;

	for (i = 0; i < ndevs; ++i) {
		for (j = 0; j < dev_stripes; ++j) {
			int s = i * dev_stripes + j;
			map->stripes[s].dev = devices_info[i].dev;
			map->stripes[s].physical = devices_info[i].dev_offset +
						   j * stripe_size;
=======
		ctl->stripe_size = min(round_up(div_u64(ctl->max_chunk_size,
							data_stripes), SZ_16M),
				       ctl->stripe_size);
	}

	/* Align to BTRFS_STRIPE_LEN */
	ctl->stripe_size = round_down(ctl->stripe_size, BTRFS_STRIPE_LEN);
	ctl->chunk_size = ctl->stripe_size * data_stripes;

	return 0;
}

static int decide_stripe_size_zoned(struct alloc_chunk_ctl *ctl,
				    struct btrfs_device_info *devices_info)
{
	u64 zone_size = devices_info[0].dev->zone_info->zone_size;
	/* Number of stripes that count for block group size */
	int data_stripes;

	/*
	 * It should hold because:
	 *    dev_extent_min == dev_extent_want == zone_size * dev_stripes
	 */
	ASSERT(devices_info[ctl->ndevs - 1].max_avail == ctl->dev_extent_min);

	ctl->stripe_size = zone_size;
	ctl->num_stripes = ctl->ndevs * ctl->dev_stripes;
	data_stripes = (ctl->num_stripes - ctl->nparity) / ctl->ncopies;

	/* stripe_size is fixed in zoned filesysmte. Reduce ndevs instead. */
	if (ctl->stripe_size * data_stripes > ctl->max_chunk_size) {
		ctl->ndevs = div_u64(div_u64(ctl->max_chunk_size * ctl->ncopies,
					     ctl->stripe_size) + ctl->nparity,
				     ctl->dev_stripes);
		ctl->num_stripes = ctl->ndevs * ctl->dev_stripes;
		data_stripes = (ctl->num_stripes - ctl->nparity) / ctl->ncopies;
		ASSERT(ctl->stripe_size * data_stripes <= ctl->max_chunk_size);
	}

	ctl->chunk_size = ctl->stripe_size * data_stripes;

	return 0;
}

static int decide_stripe_size(struct btrfs_fs_devices *fs_devices,
			      struct alloc_chunk_ctl *ctl,
			      struct btrfs_device_info *devices_info)
{
	struct btrfs_fs_info *info = fs_devices->fs_info;

	/*
	 * Round down to number of usable stripes, devs_increment can be any
	 * number so we can't use round_down() that requires power of 2, while
	 * rounddown is safe.
	 */
	ctl->ndevs = rounddown(ctl->ndevs, ctl->devs_increment);

	if (ctl->ndevs < ctl->devs_min) {
		if (btrfs_test_opt(info, ENOSPC_DEBUG)) {
			btrfs_debug(info,
	"%s: not enough devices with free space: have=%d minimum required=%d",
				    __func__, ctl->ndevs, ctl->devs_min);
		}
		return -ENOSPC;
	}

	ctl->ndevs = min(ctl->ndevs, ctl->devs_max);

	switch (fs_devices->chunk_alloc_policy) {
	case BTRFS_CHUNK_ALLOC_REGULAR:
		return decide_stripe_size_regular(ctl, devices_info);
	case BTRFS_CHUNK_ALLOC_ZONED:
		return decide_stripe_size_zoned(ctl, devices_info);
	default:
		BUG();
	}
}

static struct btrfs_block_group *create_chunk(struct btrfs_trans_handle *trans,
			struct alloc_chunk_ctl *ctl,
			struct btrfs_device_info *devices_info)
{
	struct btrfs_fs_info *info = trans->fs_info;
	struct map_lookup *map = NULL;
	struct extent_map_tree *em_tree;
	struct btrfs_block_group *block_group;
	struct extent_map *em;
	u64 start = ctl->start;
	u64 type = ctl->type;
	int ret;
	int i;
	int j;

	map = kmalloc(map_lookup_size(ctl->num_stripes), GFP_NOFS);
	if (!map)
		return ERR_PTR(-ENOMEM);
	map->num_stripes = ctl->num_stripes;

	for (i = 0; i < ctl->ndevs; ++i) {
		for (j = 0; j < ctl->dev_stripes; ++j) {
			int s = i * ctl->dev_stripes + j;
			map->stripes[s].dev = devices_info[i].dev;
			map->stripes[s].physical = devices_info[i].dev_offset +
						   j * ctl->stripe_size;
>>>>>>> upstream/android-13
		}
	}
	map->stripe_len = BTRFS_STRIPE_LEN;
	map->io_align = BTRFS_STRIPE_LEN;
	map->io_width = BTRFS_STRIPE_LEN;
	map->type = type;
<<<<<<< HEAD
	map->sub_stripes = sub_stripes;

	num_bytes = stripe_size * data_stripes;

	trace_btrfs_chunk_alloc(info, map, start, num_bytes);
=======
	map->sub_stripes = ctl->sub_stripes;

	trace_btrfs_chunk_alloc(info, map, start, ctl->chunk_size);
>>>>>>> upstream/android-13

	em = alloc_extent_map();
	if (!em) {
		kfree(map);
<<<<<<< HEAD
		ret = -ENOMEM;
		goto error;
=======
		return ERR_PTR(-ENOMEM);
>>>>>>> upstream/android-13
	}
	set_bit(EXTENT_FLAG_FS_MAPPING, &em->flags);
	em->map_lookup = map;
	em->start = start;
<<<<<<< HEAD
	em->len = num_bytes;
	em->block_start = 0;
	em->block_len = em->len;
	em->orig_block_len = stripe_size;

	em_tree = &info->mapping_tree.map_tree;
=======
	em->len = ctl->chunk_size;
	em->block_start = 0;
	em->block_len = em->len;
	em->orig_block_len = ctl->stripe_size;

	em_tree = &info->mapping_tree;
>>>>>>> upstream/android-13
	write_lock(&em_tree->lock);
	ret = add_extent_mapping(em_tree, em, 0);
	if (ret) {
		write_unlock(&em_tree->lock);
		free_extent_map(em);
<<<<<<< HEAD
		goto error;
	}

	list_add_tail(&em->list, &trans->transaction->pending_chunks);
	refcount_inc(&em->refs);
	write_unlock(&em_tree->lock);

	ret = btrfs_make_block_group(trans, 0, type, start, num_bytes);
	if (ret)
		goto error_del_extent;

	for (i = 0; i < map->num_stripes; i++) {
		num_bytes = map->stripes[i].dev->bytes_used + stripe_size;
		btrfs_device_set_bytes_used(map->stripes[i].dev, num_bytes);
		map->stripes[i].dev->has_pending_chunks = true;
	}

	atomic64_sub(stripe_size * map->num_stripes, &info->free_chunk_space);

	free_extent_map(em);
	check_raid56_incompat_flag(info, type);

	kfree(devices_info);
	return 0;
=======
		return ERR_PTR(ret);
	}
	write_unlock(&em_tree->lock);

	block_group = btrfs_make_block_group(trans, 0, type, start, ctl->chunk_size);
	if (IS_ERR(block_group))
		goto error_del_extent;

	for (i = 0; i < map->num_stripes; i++) {
		struct btrfs_device *dev = map->stripes[i].dev;

		btrfs_device_set_bytes_used(dev,
					    dev->bytes_used + ctl->stripe_size);
		if (list_empty(&dev->post_commit_list))
			list_add_tail(&dev->post_commit_list,
				      &trans->transaction->dev_update_list);
	}

	atomic64_sub(ctl->stripe_size * map->num_stripes,
		     &info->free_chunk_space);

	free_extent_map(em);
	check_raid56_incompat_flag(info, type);
	check_raid1c34_incompat_flag(info, type);

	return block_group;
>>>>>>> upstream/android-13

error_del_extent:
	write_lock(&em_tree->lock);
	remove_extent_mapping(em_tree, em);
	write_unlock(&em_tree->lock);

	/* One for our allocation */
	free_extent_map(em);
	/* One for the tree reference */
	free_extent_map(em);
<<<<<<< HEAD
	/* One for the pending_chunks list reference */
	free_extent_map(em);
error:
	kfree(devices_info);
	return ret;
}

int btrfs_finish_chunk_alloc(struct btrfs_trans_handle *trans,
			     u64 chunk_offset, u64 chunk_size)
=======

	return block_group;
}

struct btrfs_block_group *btrfs_alloc_chunk(struct btrfs_trans_handle *trans,
					    u64 type)
{
	struct btrfs_fs_info *info = trans->fs_info;
	struct btrfs_fs_devices *fs_devices = info->fs_devices;
	struct btrfs_device_info *devices_info = NULL;
	struct alloc_chunk_ctl ctl;
	struct btrfs_block_group *block_group;
	int ret;

	lockdep_assert_held(&info->chunk_mutex);

	if (!alloc_profile_is_valid(type, 0)) {
		ASSERT(0);
		return ERR_PTR(-EINVAL);
	}

	if (list_empty(&fs_devices->alloc_list)) {
		if (btrfs_test_opt(info, ENOSPC_DEBUG))
			btrfs_debug(info, "%s: no writable device", __func__);
		return ERR_PTR(-ENOSPC);
	}

	if (!(type & BTRFS_BLOCK_GROUP_TYPE_MASK)) {
		btrfs_err(info, "invalid chunk type 0x%llx requested", type);
		ASSERT(0);
		return ERR_PTR(-EINVAL);
	}

	ctl.start = find_next_chunk(info);
	ctl.type = type;
	init_alloc_chunk_ctl(fs_devices, &ctl);

	devices_info = kcalloc(fs_devices->rw_devices, sizeof(*devices_info),
			       GFP_NOFS);
	if (!devices_info)
		return ERR_PTR(-ENOMEM);

	ret = gather_device_info(fs_devices, &ctl, devices_info);
	if (ret < 0) {
		block_group = ERR_PTR(ret);
		goto out;
	}

	ret = decide_stripe_size(fs_devices, &ctl, devices_info);
	if (ret < 0) {
		block_group = ERR_PTR(ret);
		goto out;
	}

	block_group = create_chunk(trans, &ctl, devices_info);

out:
	kfree(devices_info);
	return block_group;
}

/*
 * This function, btrfs_chunk_alloc_add_chunk_item(), typically belongs to the
 * phase 1 of chunk allocation. It belongs to phase 2 only when allocating system
 * chunks.
 *
 * See the comment at btrfs_chunk_alloc() for details about the chunk allocation
 * phases.
 */
int btrfs_chunk_alloc_add_chunk_item(struct btrfs_trans_handle *trans,
				     struct btrfs_block_group *bg)
>>>>>>> upstream/android-13
{
	struct btrfs_fs_info *fs_info = trans->fs_info;
	struct btrfs_root *extent_root = fs_info->extent_root;
	struct btrfs_root *chunk_root = fs_info->chunk_root;
	struct btrfs_key key;
<<<<<<< HEAD
	struct btrfs_device *device;
=======
>>>>>>> upstream/android-13
	struct btrfs_chunk *chunk;
	struct btrfs_stripe *stripe;
	struct extent_map *em;
	struct map_lookup *map;
	size_t item_size;
<<<<<<< HEAD
	u64 dev_offset;
	u64 stripe_size;
	int i = 0;
	int ret = 0;

	em = get_chunk_map(fs_info, chunk_offset, chunk_size);
	if (IS_ERR(em))
		return PTR_ERR(em);

	map = em->map_lookup;
	item_size = btrfs_chunk_item_size(map->num_stripes);
	stripe_size = em->orig_block_len;
=======
	int i;
	int ret;

	/*
	 * We take the chunk_mutex for 2 reasons:
	 *
	 * 1) Updates and insertions in the chunk btree must be done while holding
	 *    the chunk_mutex, as well as updating the system chunk array in the
	 *    superblock. See the comment on top of btrfs_chunk_alloc() for the
	 *    details;
	 *
	 * 2) To prevent races with the final phase of a device replace operation
	 *    that replaces the device object associated with the map's stripes,
	 *    because the device object's id can change at any time during that
	 *    final phase of the device replace operation
	 *    (dev-replace.c:btrfs_dev_replace_finishing()), so we could grab the
	 *    replaced device and then see it with an ID of BTRFS_DEV_REPLACE_DEVID,
	 *    which would cause a failure when updating the device item, which does
	 *    not exists, or persisting a stripe of the chunk item with such ID.
	 *    Here we can't use the device_list_mutex because our caller already
	 *    has locked the chunk_mutex, and the final phase of device replace
	 *    acquires both mutexes - first the device_list_mutex and then the
	 *    chunk_mutex. Using any of those two mutexes protects us from a
	 *    concurrent device replace.
	 */
	lockdep_assert_held(&fs_info->chunk_mutex);

	em = btrfs_get_chunk_map(fs_info, bg->start, bg->length);
	if (IS_ERR(em)) {
		ret = PTR_ERR(em);
		btrfs_abort_transaction(trans, ret);
		return ret;
	}

	map = em->map_lookup;
	item_size = btrfs_chunk_item_size(map->num_stripes);
>>>>>>> upstream/android-13

	chunk = kzalloc(item_size, GFP_NOFS);
	if (!chunk) {
		ret = -ENOMEM;
<<<<<<< HEAD
		goto out;
	}

	/*
	 * Take the device list mutex to prevent races with the final phase of
	 * a device replace operation that replaces the device object associated
	 * with the map's stripes, because the device object's id can change
	 * at any time during that final phase of the device replace operation
	 * (dev-replace.c:btrfs_dev_replace_finishing()).
	 */
	mutex_lock(&fs_info->fs_devices->device_list_mutex);
	for (i = 0; i < map->num_stripes; i++) {
		device = map->stripes[i].dev;
		dev_offset = map->stripes[i].physical;

		ret = btrfs_update_device(trans, device);
		if (ret)
			break;
		ret = btrfs_alloc_dev_extent(trans, device, chunk_offset,
					     dev_offset, stripe_size);
		if (ret)
			break;
	}
	if (ret) {
		mutex_unlock(&fs_info->fs_devices->device_list_mutex);
		goto out;
=======
		btrfs_abort_transaction(trans, ret);
		goto out;
	}

	for (i = 0; i < map->num_stripes; i++) {
		struct btrfs_device *device = map->stripes[i].dev;

		ret = btrfs_update_device(trans, device);
		if (ret)
			goto out;
>>>>>>> upstream/android-13
	}

	stripe = &chunk->stripe;
	for (i = 0; i < map->num_stripes; i++) {
<<<<<<< HEAD
		device = map->stripes[i].dev;
		dev_offset = map->stripes[i].physical;
=======
		struct btrfs_device *device = map->stripes[i].dev;
		const u64 dev_offset = map->stripes[i].physical;
>>>>>>> upstream/android-13

		btrfs_set_stack_stripe_devid(stripe, device->devid);
		btrfs_set_stack_stripe_offset(stripe, dev_offset);
		memcpy(stripe->dev_uuid, device->uuid, BTRFS_UUID_SIZE);
		stripe++;
	}
<<<<<<< HEAD
	mutex_unlock(&fs_info->fs_devices->device_list_mutex);

	btrfs_set_stack_chunk_length(chunk, chunk_size);
=======

	btrfs_set_stack_chunk_length(chunk, bg->length);
>>>>>>> upstream/android-13
	btrfs_set_stack_chunk_owner(chunk, extent_root->root_key.objectid);
	btrfs_set_stack_chunk_stripe_len(chunk, map->stripe_len);
	btrfs_set_stack_chunk_type(chunk, map->type);
	btrfs_set_stack_chunk_num_stripes(chunk, map->num_stripes);
	btrfs_set_stack_chunk_io_align(chunk, map->stripe_len);
	btrfs_set_stack_chunk_io_width(chunk, map->stripe_len);
	btrfs_set_stack_chunk_sector_size(chunk, fs_info->sectorsize);
	btrfs_set_stack_chunk_sub_stripes(chunk, map->sub_stripes);

	key.objectid = BTRFS_FIRST_CHUNK_TREE_OBJECTID;
	key.type = BTRFS_CHUNK_ITEM_KEY;
<<<<<<< HEAD
	key.offset = chunk_offset;

	ret = btrfs_insert_item(trans, chunk_root, &key, chunk, item_size);
	if (ret == 0 && map->type & BTRFS_BLOCK_GROUP_SYSTEM) {
		/*
		 * TODO: Cleanup of inserted chunk root in case of
		 * failure.
		 */
		ret = btrfs_add_system_chunk(fs_info, &key, chunk, item_size);
=======
	key.offset = bg->start;

	ret = btrfs_insert_item(trans, chunk_root, &key, chunk, item_size);
	if (ret)
		goto out;

	bg->chunk_item_inserted = 1;

	if (map->type & BTRFS_BLOCK_GROUP_SYSTEM) {
		ret = btrfs_add_system_chunk(fs_info, &key, chunk, item_size);
		if (ret)
			goto out;
>>>>>>> upstream/android-13
	}

out:
	kfree(chunk);
	free_extent_map(em);
	return ret;
}

<<<<<<< HEAD
/*
 * Chunk allocation falls into two parts. The first part does works
 * that make the new allocated chunk useable, but not do any operation
 * that modifies the chunk tree. The second part does the works that
 * require modifying the chunk tree. This division is important for the
 * bootstrap process of adding storage to a seed btrfs.
 */
int btrfs_alloc_chunk(struct btrfs_trans_handle *trans, u64 type)
{
	u64 chunk_offset;

	lockdep_assert_held(&trans->fs_info->chunk_mutex);
	chunk_offset = find_next_chunk(trans->fs_info);
	return __btrfs_alloc_chunk(trans, chunk_offset, type);
}

static noinline int init_first_rw_device(struct btrfs_trans_handle *trans,
					 struct btrfs_fs_info *fs_info)
{
	u64 chunk_offset;
	u64 sys_chunk_offset;
	u64 alloc_profile;
	int ret;

	chunk_offset = find_next_chunk(fs_info);
	alloc_profile = btrfs_metadata_alloc_profile(fs_info);
	ret = __btrfs_alloc_chunk(trans, chunk_offset, alloc_profile);
	if (ret)
		return ret;

	sys_chunk_offset = find_next_chunk(fs_info);
	alloc_profile = btrfs_system_alloc_profile(fs_info);
	ret = __btrfs_alloc_chunk(trans, sys_chunk_offset, alloc_profile);
	return ret;
=======
static noinline int init_first_rw_device(struct btrfs_trans_handle *trans)
{
	struct btrfs_fs_info *fs_info = trans->fs_info;
	u64 alloc_profile;
	struct btrfs_block_group *meta_bg;
	struct btrfs_block_group *sys_bg;

	/*
	 * When adding a new device for sprouting, the seed device is read-only
	 * so we must first allocate a metadata and a system chunk. But before
	 * adding the block group items to the extent, device and chunk btrees,
	 * we must first:
	 *
	 * 1) Create both chunks without doing any changes to the btrees, as
	 *    otherwise we would get -ENOSPC since the block groups from the
	 *    seed device are read-only;
	 *
	 * 2) Add the device item for the new sprout device - finishing the setup
	 *    of a new block group requires updating the device item in the chunk
	 *    btree, so it must exist when we attempt to do it. The previous step
	 *    ensures this does not fail with -ENOSPC.
	 *
	 * After that we can add the block group items to their btrees:
	 * update existing device item in the chunk btree, add a new block group
	 * item to the extent btree, add a new chunk item to the chunk btree and
	 * finally add the new device extent items to the devices btree.
	 */

	alloc_profile = btrfs_metadata_alloc_profile(fs_info);
	meta_bg = btrfs_alloc_chunk(trans, alloc_profile);
	if (IS_ERR(meta_bg))
		return PTR_ERR(meta_bg);

	alloc_profile = btrfs_system_alloc_profile(fs_info);
	sys_bg = btrfs_alloc_chunk(trans, alloc_profile);
	if (IS_ERR(sys_bg))
		return PTR_ERR(sys_bg);

	return 0;
>>>>>>> upstream/android-13
}

static inline int btrfs_chunk_max_errors(struct map_lookup *map)
{
<<<<<<< HEAD
	int max_errors;

	if (map->type & (BTRFS_BLOCK_GROUP_RAID1 |
			 BTRFS_BLOCK_GROUP_RAID10 |
			 BTRFS_BLOCK_GROUP_RAID5)) {
		max_errors = 1;
	} else if (map->type & BTRFS_BLOCK_GROUP_RAID6) {
		max_errors = 2;
	} else {
		max_errors = 0;
	}

	return max_errors;
=======
	const int index = btrfs_bg_flags_to_raid_index(map->type);

	return btrfs_raid_array[index].tolerated_failures;
>>>>>>> upstream/android-13
}

int btrfs_chunk_readonly(struct btrfs_fs_info *fs_info, u64 chunk_offset)
{
	struct extent_map *em;
	struct map_lookup *map;
	int readonly = 0;
	int miss_ndevs = 0;
	int i;

<<<<<<< HEAD
	em = get_chunk_map(fs_info, chunk_offset, 1);
=======
	em = btrfs_get_chunk_map(fs_info, chunk_offset, 1);
>>>>>>> upstream/android-13
	if (IS_ERR(em))
		return 1;

	map = em->map_lookup;
	for (i = 0; i < map->num_stripes; i++) {
		if (test_bit(BTRFS_DEV_STATE_MISSING,
					&map->stripes[i].dev->dev_state)) {
			miss_ndevs++;
			continue;
		}
		if (!test_bit(BTRFS_DEV_STATE_WRITEABLE,
					&map->stripes[i].dev->dev_state)) {
			readonly = 1;
			goto end;
		}
	}

	/*
	 * If the number of missing devices is larger than max errors,
	 * we can not write the data into that chunk successfully, so
	 * set it readonly.
	 */
	if (miss_ndevs > btrfs_chunk_max_errors(map))
		readonly = 1;
end:
	free_extent_map(em);
	return readonly;
}

<<<<<<< HEAD
void btrfs_mapping_init(struct btrfs_mapping_tree *tree)
{
	extent_map_tree_init(&tree->map_tree);
}

void btrfs_mapping_tree_free(struct btrfs_mapping_tree *tree)
=======
void btrfs_mapping_tree_free(struct extent_map_tree *tree)
>>>>>>> upstream/android-13
{
	struct extent_map *em;

	while (1) {
<<<<<<< HEAD
		write_lock(&tree->map_tree.lock);
		em = lookup_extent_mapping(&tree->map_tree, 0, (u64)-1);
		if (em)
			remove_extent_mapping(&tree->map_tree, em);
		write_unlock(&tree->map_tree.lock);
=======
		write_lock(&tree->lock);
		em = lookup_extent_mapping(tree, 0, (u64)-1);
		if (em)
			remove_extent_mapping(tree, em);
		write_unlock(&tree->lock);
>>>>>>> upstream/android-13
		if (!em)
			break;
		/* once for us */
		free_extent_map(em);
		/* once for the tree */
		free_extent_map(em);
	}
}

int btrfs_num_copies(struct btrfs_fs_info *fs_info, u64 logical, u64 len)
{
	struct extent_map *em;
	struct map_lookup *map;
	int ret;

<<<<<<< HEAD
	em = get_chunk_map(fs_info, logical, len);
=======
	em = btrfs_get_chunk_map(fs_info, logical, len);
>>>>>>> upstream/android-13
	if (IS_ERR(em))
		/*
		 * We could return errors for these cases, but that could get
		 * ugly and we'd probably do the same thing which is just not do
		 * anything else and exit, so return 1 so the callers don't try
		 * to use other copies.
		 */
		return 1;

	map = em->map_lookup;
<<<<<<< HEAD
	if (map->type & (BTRFS_BLOCK_GROUP_DUP | BTRFS_BLOCK_GROUP_RAID1))
=======
	if (map->type & (BTRFS_BLOCK_GROUP_DUP | BTRFS_BLOCK_GROUP_RAID1_MASK))
>>>>>>> upstream/android-13
		ret = map->num_stripes;
	else if (map->type & BTRFS_BLOCK_GROUP_RAID10)
		ret = map->sub_stripes;
	else if (map->type & BTRFS_BLOCK_GROUP_RAID5)
		ret = 2;
	else if (map->type & BTRFS_BLOCK_GROUP_RAID6)
		/*
		 * There could be two corrupted data stripes, we need
		 * to loop retry in order to rebuild the correct data.
		 *
		 * Fail a stripe at a time on every retry except the
		 * stripe under reconstruction.
		 */
		ret = map->num_stripes;
	else
		ret = 1;
	free_extent_map(em);

<<<<<<< HEAD
	btrfs_dev_replace_read_lock(&fs_info->dev_replace);
	if (btrfs_dev_replace_is_ongoing(&fs_info->dev_replace) &&
	    fs_info->dev_replace.tgtdev)
		ret++;
	btrfs_dev_replace_read_unlock(&fs_info->dev_replace);
=======
	down_read(&fs_info->dev_replace.rwsem);
	if (btrfs_dev_replace_is_ongoing(&fs_info->dev_replace) &&
	    fs_info->dev_replace.tgtdev)
		ret++;
	up_read(&fs_info->dev_replace.rwsem);
>>>>>>> upstream/android-13

	return ret;
}

unsigned long btrfs_full_stripe_len(struct btrfs_fs_info *fs_info,
				    u64 logical)
{
	struct extent_map *em;
	struct map_lookup *map;
	unsigned long len = fs_info->sectorsize;

<<<<<<< HEAD
	em = get_chunk_map(fs_info, logical, len);
=======
	em = btrfs_get_chunk_map(fs_info, logical, len);
>>>>>>> upstream/android-13

	if (!WARN_ON(IS_ERR(em))) {
		map = em->map_lookup;
		if (map->type & BTRFS_BLOCK_GROUP_RAID56_MASK)
			len = map->stripe_len * nr_data_stripes(map);
		free_extent_map(em);
	}
	return len;
}

int btrfs_is_parity_mirror(struct btrfs_fs_info *fs_info, u64 logical, u64 len)
{
	struct extent_map *em;
	struct map_lookup *map;
	int ret = 0;

<<<<<<< HEAD
	em = get_chunk_map(fs_info, logical, len);
=======
	em = btrfs_get_chunk_map(fs_info, logical, len);
>>>>>>> upstream/android-13

	if(!WARN_ON(IS_ERR(em))) {
		map = em->map_lookup;
		if (map->type & BTRFS_BLOCK_GROUP_RAID56_MASK)
			ret = 1;
		free_extent_map(em);
	}
	return ret;
}

static int find_live_mirror(struct btrfs_fs_info *fs_info,
			    struct map_lookup *map, int first,
			    int dev_replace_is_ongoing)
{
	int i;
	int num_stripes;
	int preferred_mirror;
	int tolerance;
	struct btrfs_device *srcdev;

	ASSERT((map->type &
<<<<<<< HEAD
		 (BTRFS_BLOCK_GROUP_RAID1 | BTRFS_BLOCK_GROUP_RAID10)));
=======
		 (BTRFS_BLOCK_GROUP_RAID1_MASK | BTRFS_BLOCK_GROUP_RAID10)));
>>>>>>> upstream/android-13

	if (map->type & BTRFS_BLOCK_GROUP_RAID10)
		num_stripes = map->sub_stripes;
	else
		num_stripes = map->num_stripes;

<<<<<<< HEAD
	preferred_mirror = first + current->pid % num_stripes;
=======
	switch (fs_info->fs_devices->read_policy) {
	default:
		/* Shouldn't happen, just warn and use pid instead of failing */
		btrfs_warn_rl(fs_info,
			      "unknown read_policy type %u, reset to pid",
			      fs_info->fs_devices->read_policy);
		fs_info->fs_devices->read_policy = BTRFS_READ_POLICY_PID;
		fallthrough;
	case BTRFS_READ_POLICY_PID:
		preferred_mirror = first + (current->pid % num_stripes);
		break;
	}
>>>>>>> upstream/android-13

	if (dev_replace_is_ongoing &&
	    fs_info->dev_replace.cont_reading_from_srcdev_mode ==
	     BTRFS_DEV_REPLACE_ITEM_CONT_READING_FROM_SRCDEV_MODE_AVOID)
		srcdev = fs_info->dev_replace.srcdev;
	else
		srcdev = NULL;

	/*
	 * try to avoid the drive that is the source drive for a
	 * dev-replace procedure, only choose it if no other non-missing
	 * mirror is available
	 */
	for (tolerance = 0; tolerance < 2; tolerance++) {
		if (map->stripes[preferred_mirror].dev->bdev &&
		    (tolerance || map->stripes[preferred_mirror].dev != srcdev))
			return preferred_mirror;
		for (i = first; i < first + num_stripes; i++) {
			if (map->stripes[i].dev->bdev &&
			    (tolerance || map->stripes[i].dev != srcdev))
				return i;
		}
	}

	/* we couldn't find one that doesn't fail.  Just return something
	 * and the io error handling code will clean up eventually
	 */
	return preferred_mirror;
}

<<<<<<< HEAD
static inline int parity_smaller(u64 a, u64 b)
{
	return a > b;
}

/* Bubble-sort the stripe set to put the parity/syndrome stripes last */
static void sort_parity_stripes(struct btrfs_bio *bbio, int num_stripes)
{
	struct btrfs_bio_stripe s;
	int i;
	u64 l;
=======
/* Bubble-sort the stripe set to put the parity/syndrome stripes last */
static void sort_parity_stripes(struct btrfs_bio *bbio, int num_stripes)
{
	int i;
>>>>>>> upstream/android-13
	int again = 1;

	while (again) {
		again = 0;
		for (i = 0; i < num_stripes - 1; i++) {
<<<<<<< HEAD
			if (parity_smaller(bbio->raid_map[i],
					   bbio->raid_map[i+1])) {
				s = bbio->stripes[i];
				l = bbio->raid_map[i];
				bbio->stripes[i] = bbio->stripes[i+1];
				bbio->raid_map[i] = bbio->raid_map[i+1];
				bbio->stripes[i+1] = s;
				bbio->raid_map[i+1] = l;

=======
			/* Swap if parity is on a smaller index */
			if (bbio->raid_map[i] > bbio->raid_map[i + 1]) {
				swap(bbio->stripes[i], bbio->stripes[i + 1]);
				swap(bbio->raid_map[i], bbio->raid_map[i + 1]);
>>>>>>> upstream/android-13
				again = 1;
			}
		}
	}
}

static struct btrfs_bio *alloc_btrfs_bio(int total_stripes, int real_stripes)
{
	struct btrfs_bio *bbio = kzalloc(
		 /* the size of the btrfs_bio */
		sizeof(struct btrfs_bio) +
		/* plus the variable array for the stripes */
		sizeof(struct btrfs_bio_stripe) * (total_stripes) +
		/* plus the variable array for the tgt dev */
		sizeof(int) * (real_stripes) +
		/*
		 * plus the raid_map, which includes both the tgt dev
		 * and the stripes
		 */
		sizeof(u64) * (total_stripes),
		GFP_NOFS|__GFP_NOFAIL);

	atomic_set(&bbio->error, 0);
	refcount_set(&bbio->refs, 1);

<<<<<<< HEAD
=======
	bbio->tgtdev_map = (int *)(bbio->stripes + total_stripes);
	bbio->raid_map = (u64 *)(bbio->tgtdev_map + real_stripes);

>>>>>>> upstream/android-13
	return bbio;
}

void btrfs_get_bbio(struct btrfs_bio *bbio)
{
	WARN_ON(!refcount_read(&bbio->refs));
	refcount_inc(&bbio->refs);
}

void btrfs_put_bbio(struct btrfs_bio *bbio)
{
	if (!bbio)
		return;
	if (refcount_dec_and_test(&bbio->refs))
		kfree(bbio);
}

/* can REQ_OP_DISCARD be sent with other REQ like REQ_OP_WRITE? */
/*
 * Please note that, discard won't be sent to target device of device
 * replace.
 */
static int __btrfs_map_block_for_discard(struct btrfs_fs_info *fs_info,
<<<<<<< HEAD
					 u64 logical, u64 length,
=======
					 u64 logical, u64 *length_ret,
>>>>>>> upstream/android-13
					 struct btrfs_bio **bbio_ret)
{
	struct extent_map *em;
	struct map_lookup *map;
	struct btrfs_bio *bbio;
<<<<<<< HEAD
=======
	u64 length = *length_ret;
>>>>>>> upstream/android-13
	u64 offset;
	u64 stripe_nr;
	u64 stripe_nr_end;
	u64 stripe_end_offset;
	u64 stripe_cnt;
	u64 stripe_len;
	u64 stripe_offset;
	u64 num_stripes;
	u32 stripe_index;
	u32 factor = 0;
	u32 sub_stripes = 0;
	u64 stripes_per_dev = 0;
	u32 remaining_stripes = 0;
	u32 last_stripe = 0;
	int ret = 0;
	int i;

	/* discard always return a bbio */
	ASSERT(bbio_ret);

<<<<<<< HEAD
	em = get_chunk_map(fs_info, logical, length);
=======
	em = btrfs_get_chunk_map(fs_info, logical, length);
>>>>>>> upstream/android-13
	if (IS_ERR(em))
		return PTR_ERR(em);

	map = em->map_lookup;
	/* we don't discard raid56 yet */
	if (map->type & BTRFS_BLOCK_GROUP_RAID56_MASK) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	offset = logical - em->start;
<<<<<<< HEAD
	length = min_t(u64, em->len - offset, length);
=======
	length = min_t(u64, em->start + em->len - logical, length);
	*length_ret = length;
>>>>>>> upstream/android-13

	stripe_len = map->stripe_len;
	/*
	 * stripe_nr counts the total number of stripes we have to stride
	 * to get to this block
	 */
	stripe_nr = div64_u64(offset, stripe_len);

	/* stripe_offset is the offset of this block in its stripe */
	stripe_offset = offset - stripe_nr * stripe_len;

	stripe_nr_end = round_up(offset + length, map->stripe_len);
	stripe_nr_end = div64_u64(stripe_nr_end, map->stripe_len);
	stripe_cnt = stripe_nr_end - stripe_nr;
	stripe_end_offset = stripe_nr_end * map->stripe_len -
			    (offset + length);
	/*
	 * after this, stripe_nr is the number of stripes on this
	 * device we have to walk to find the data, and stripe_index is
	 * the number of our device in the stripe array
	 */
	num_stripes = 1;
	stripe_index = 0;
	if (map->type & (BTRFS_BLOCK_GROUP_RAID0 |
			 BTRFS_BLOCK_GROUP_RAID10)) {
		if (map->type & BTRFS_BLOCK_GROUP_RAID0)
			sub_stripes = 1;
		else
			sub_stripes = map->sub_stripes;

		factor = map->num_stripes / sub_stripes;
		num_stripes = min_t(u64, map->num_stripes,
				    sub_stripes * stripe_cnt);
		stripe_nr = div_u64_rem(stripe_nr, factor, &stripe_index);
		stripe_index *= sub_stripes;
		stripes_per_dev = div_u64_rem(stripe_cnt, factor,
					      &remaining_stripes);
		div_u64_rem(stripe_nr_end - 1, factor, &last_stripe);
		last_stripe *= sub_stripes;
<<<<<<< HEAD
	} else if (map->type & (BTRFS_BLOCK_GROUP_RAID1 |
=======
	} else if (map->type & (BTRFS_BLOCK_GROUP_RAID1_MASK |
>>>>>>> upstream/android-13
				BTRFS_BLOCK_GROUP_DUP)) {
		num_stripes = map->num_stripes;
	} else {
		stripe_nr = div_u64_rem(stripe_nr, map->num_stripes,
					&stripe_index);
	}

	bbio = alloc_btrfs_bio(num_stripes, 0);
	if (!bbio) {
		ret = -ENOMEM;
		goto out;
	}

	for (i = 0; i < num_stripes; i++) {
		bbio->stripes[i].physical =
			map->stripes[stripe_index].physical +
			stripe_offset + stripe_nr * map->stripe_len;
		bbio->stripes[i].dev = map->stripes[stripe_index].dev;

		if (map->type & (BTRFS_BLOCK_GROUP_RAID0 |
				 BTRFS_BLOCK_GROUP_RAID10)) {
			bbio->stripes[i].length = stripes_per_dev *
				map->stripe_len;

			if (i / sub_stripes < remaining_stripes)
				bbio->stripes[i].length +=
					map->stripe_len;

			/*
			 * Special for the first stripe and
			 * the last stripe:
			 *
			 * |-------|...|-------|
			 *     |----------|
			 *    off     end_off
			 */
			if (i < sub_stripes)
				bbio->stripes[i].length -=
					stripe_offset;

			if (stripe_index >= last_stripe &&
			    stripe_index <= (last_stripe +
					     sub_stripes - 1))
				bbio->stripes[i].length -=
					stripe_end_offset;

			if (i == sub_stripes - 1)
				stripe_offset = 0;
		} else {
			bbio->stripes[i].length = length;
		}

		stripe_index++;
		if (stripe_index == map->num_stripes) {
			stripe_index = 0;
			stripe_nr++;
		}
	}

	*bbio_ret = bbio;
	bbio->map_type = map->type;
	bbio->num_stripes = num_stripes;
out:
	free_extent_map(em);
	return ret;
}

/*
 * In dev-replace case, for repair case (that's the only case where the mirror
 * is selected explicitly when calling btrfs_map_block), blocks left of the
 * left cursor can also be read from the target drive.
 *
 * For REQ_GET_READ_MIRRORS, the target drive is added as the last one to the
 * array of stripes.
 * For READ, it also needs to be supported using the same mirror number.
 *
 * If the requested block is not left of the left cursor, EIO is returned. This
 * can happen because btrfs_num_copies() returns one more in the dev-replace
 * case.
 */
static int get_extra_mirror_from_replace(struct btrfs_fs_info *fs_info,
					 u64 logical, u64 length,
					 u64 srcdev_devid, int *mirror_num,
					 u64 *physical)
{
	struct btrfs_bio *bbio = NULL;
	int num_stripes;
	int index_srcdev = 0;
	int found = 0;
	u64 physical_of_found = 0;
	int i;
	int ret = 0;

	ret = __btrfs_map_block(fs_info, BTRFS_MAP_GET_READ_MIRRORS,
				logical, &length, &bbio, 0, 0);
	if (ret) {
		ASSERT(bbio == NULL);
		return ret;
	}

	num_stripes = bbio->num_stripes;
	if (*mirror_num > num_stripes) {
		/*
		 * BTRFS_MAP_GET_READ_MIRRORS does not contain this mirror,
		 * that means that the requested area is not left of the left
		 * cursor
		 */
		btrfs_put_bbio(bbio);
		return -EIO;
	}

	/*
	 * process the rest of the function using the mirror_num of the source
	 * drive. Therefore look it up first.  At the end, patch the device
	 * pointer to the one of the target drive.
	 */
	for (i = 0; i < num_stripes; i++) {
		if (bbio->stripes[i].dev->devid != srcdev_devid)
			continue;

		/*
		 * In case of DUP, in order to keep it simple, only add the
		 * mirror with the lowest physical address
		 */
		if (found &&
		    physical_of_found <= bbio->stripes[i].physical)
			continue;

		index_srcdev = i;
		found = 1;
		physical_of_found = bbio->stripes[i].physical;
	}

	btrfs_put_bbio(bbio);

	ASSERT(found);
	if (!found)
		return -EIO;

	*mirror_num = index_srcdev + 1;
	*physical = physical_of_found;
	return ret;
}

<<<<<<< HEAD
static void handle_ops_on_dev_replace(enum btrfs_map_op op,
				      struct btrfs_bio **bbio_ret,
				      struct btrfs_dev_replace *dev_replace,
=======
static bool is_block_group_to_copy(struct btrfs_fs_info *fs_info, u64 logical)
{
	struct btrfs_block_group *cache;
	bool ret;

	/* Non zoned filesystem does not use "to_copy" flag */
	if (!btrfs_is_zoned(fs_info))
		return false;

	cache = btrfs_lookup_block_group(fs_info, logical);

	spin_lock(&cache->lock);
	ret = cache->to_copy;
	spin_unlock(&cache->lock);

	btrfs_put_block_group(cache);
	return ret;
}

static void handle_ops_on_dev_replace(enum btrfs_map_op op,
				      struct btrfs_bio **bbio_ret,
				      struct btrfs_dev_replace *dev_replace,
				      u64 logical,
>>>>>>> upstream/android-13
				      int *num_stripes_ret, int *max_errors_ret)
{
	struct btrfs_bio *bbio = *bbio_ret;
	u64 srcdev_devid = dev_replace->srcdev->devid;
	int tgtdev_indexes = 0;
	int num_stripes = *num_stripes_ret;
	int max_errors = *max_errors_ret;
	int i;

	if (op == BTRFS_MAP_WRITE) {
		int index_where_to_add;

		/*
<<<<<<< HEAD
=======
		 * A block group which have "to_copy" set will eventually
		 * copied by dev-replace process. We can avoid cloning IO here.
		 */
		if (is_block_group_to_copy(dev_replace->srcdev->fs_info, logical))
			return;

		/*
>>>>>>> upstream/android-13
		 * duplicate the write operations while the dev replace
		 * procedure is running. Since the copying of the old disk to
		 * the new disk takes place at run time while the filesystem is
		 * mounted writable, the regular write operations to the old
		 * disk have to be duplicated to go to the new disk as well.
		 *
		 * Note that device->missing is handled by the caller, and that
		 * the write to the old disk is already set up in the stripes
		 * array.
		 */
		index_where_to_add = num_stripes;
		for (i = 0; i < num_stripes; i++) {
			if (bbio->stripes[i].dev->devid == srcdev_devid) {
				/* write to new disk, too */
				struct btrfs_bio_stripe *new =
					bbio->stripes + index_where_to_add;
				struct btrfs_bio_stripe *old =
					bbio->stripes + i;

				new->physical = old->physical;
				new->length = old->length;
				new->dev = dev_replace->tgtdev;
				bbio->tgtdev_map[i] = index_where_to_add;
				index_where_to_add++;
				max_errors++;
				tgtdev_indexes++;
			}
		}
		num_stripes = index_where_to_add;
	} else if (op == BTRFS_MAP_GET_READ_MIRRORS) {
		int index_srcdev = 0;
		int found = 0;
		u64 physical_of_found = 0;

		/*
		 * During the dev-replace procedure, the target drive can also
		 * be used to read data in case it is needed to repair a corrupt
		 * block elsewhere. This is possible if the requested area is
		 * left of the left cursor. In this area, the target drive is a
		 * full copy of the source drive.
		 */
		for (i = 0; i < num_stripes; i++) {
			if (bbio->stripes[i].dev->devid == srcdev_devid) {
				/*
				 * In case of DUP, in order to keep it simple,
				 * only add the mirror with the lowest physical
				 * address
				 */
				if (found &&
				    physical_of_found <=
				     bbio->stripes[i].physical)
					continue;
				index_srcdev = i;
				found = 1;
				physical_of_found = bbio->stripes[i].physical;
			}
		}
		if (found) {
			struct btrfs_bio_stripe *tgtdev_stripe =
				bbio->stripes + num_stripes;

			tgtdev_stripe->physical = physical_of_found;
			tgtdev_stripe->length =
				bbio->stripes[index_srcdev].length;
			tgtdev_stripe->dev = dev_replace->tgtdev;
			bbio->tgtdev_map[index_srcdev] = num_stripes;

			tgtdev_indexes++;
			num_stripes++;
		}
	}

	*num_stripes_ret = num_stripes;
	*max_errors_ret = max_errors;
	bbio->num_tgtdevs = tgtdev_indexes;
	*bbio_ret = bbio;
}

static bool need_full_stripe(enum btrfs_map_op op)
{
	return (op == BTRFS_MAP_WRITE || op == BTRFS_MAP_GET_READ_MIRRORS);
}

<<<<<<< HEAD
=======
/*
 * Calculate the geometry of a particular (address, len) tuple. This
 * information is used to calculate how big a particular bio can get before it
 * straddles a stripe.
 *
 * @fs_info: the filesystem
 * @em:      mapping containing the logical extent
 * @op:      type of operation - write or read
 * @logical: address that we want to figure out the geometry of
 * @io_geom: pointer used to return values
 *
 * Returns < 0 in case a chunk for the given logical address cannot be found,
 * usually shouldn't happen unless @logical is corrupted, 0 otherwise.
 */
int btrfs_get_io_geometry(struct btrfs_fs_info *fs_info, struct extent_map *em,
			  enum btrfs_map_op op, u64 logical,
			  struct btrfs_io_geometry *io_geom)
{
	struct map_lookup *map;
	u64 len;
	u64 offset;
	u64 stripe_offset;
	u64 stripe_nr;
	u64 stripe_len;
	u64 raid56_full_stripe_start = (u64)-1;
	int data_stripes;

	ASSERT(op != BTRFS_MAP_DISCARD);

	map = em->map_lookup;
	/* Offset of this logical address in the chunk */
	offset = logical - em->start;
	/* Len of a stripe in a chunk */
	stripe_len = map->stripe_len;
	/* Stripe where this block falls in */
	stripe_nr = div64_u64(offset, stripe_len);
	/* Offset of stripe in the chunk */
	stripe_offset = stripe_nr * stripe_len;
	if (offset < stripe_offset) {
		btrfs_crit(fs_info,
"stripe math has gone wrong, stripe_offset=%llu offset=%llu start=%llu logical=%llu stripe_len=%llu",
			stripe_offset, offset, em->start, logical, stripe_len);
		return -EINVAL;
	}

	/* stripe_offset is the offset of this block in its stripe */
	stripe_offset = offset - stripe_offset;
	data_stripes = nr_data_stripes(map);

	if (map->type & BTRFS_BLOCK_GROUP_PROFILE_MASK) {
		u64 max_len = stripe_len - stripe_offset;

		/*
		 * In case of raid56, we need to know the stripe aligned start
		 */
		if (map->type & BTRFS_BLOCK_GROUP_RAID56_MASK) {
			unsigned long full_stripe_len = stripe_len * data_stripes;
			raid56_full_stripe_start = offset;

			/*
			 * Allow a write of a full stripe, but make sure we
			 * don't allow straddling of stripes
			 */
			raid56_full_stripe_start = div64_u64(raid56_full_stripe_start,
					full_stripe_len);
			raid56_full_stripe_start *= full_stripe_len;

			/*
			 * For writes to RAID[56], allow a full stripeset across
			 * all disks. For other RAID types and for RAID[56]
			 * reads, just allow a single stripe (on a single disk).
			 */
			if (op == BTRFS_MAP_WRITE) {
				max_len = stripe_len * data_stripes -
					  (offset - raid56_full_stripe_start);
			}
		}
		len = min_t(u64, em->len - offset, max_len);
	} else {
		len = em->len - offset;
	}

	io_geom->len = len;
	io_geom->offset = offset;
	io_geom->stripe_len = stripe_len;
	io_geom->stripe_nr = stripe_nr;
	io_geom->stripe_offset = stripe_offset;
	io_geom->raid56_stripe_offset = raid56_full_stripe_start;

	return 0;
}

>>>>>>> upstream/android-13
static int __btrfs_map_block(struct btrfs_fs_info *fs_info,
			     enum btrfs_map_op op,
			     u64 logical, u64 *length,
			     struct btrfs_bio **bbio_ret,
			     int mirror_num, int need_raid_map)
{
	struct extent_map *em;
	struct map_lookup *map;
<<<<<<< HEAD
	u64 offset;
=======
>>>>>>> upstream/android-13
	u64 stripe_offset;
	u64 stripe_nr;
	u64 stripe_len;
	u32 stripe_index;
<<<<<<< HEAD
=======
	int data_stripes;
>>>>>>> upstream/android-13
	int i;
	int ret = 0;
	int num_stripes;
	int max_errors = 0;
	int tgtdev_indexes = 0;
	struct btrfs_bio *bbio = NULL;
	struct btrfs_dev_replace *dev_replace = &fs_info->dev_replace;
	int dev_replace_is_ongoing = 0;
	int num_alloc_stripes;
	int patch_the_first_stripe_for_dev_replace = 0;
	u64 physical_to_patch_in_first_stripe = 0;
	u64 raid56_full_stripe_start = (u64)-1;
<<<<<<< HEAD

	if (op == BTRFS_MAP_DISCARD)
		return __btrfs_map_block_for_discard(fs_info, logical,
						     *length, bbio_ret);

	em = get_chunk_map(fs_info, logical, *length);
	if (IS_ERR(em))
		return PTR_ERR(em);

	map = em->map_lookup;
	offset = logical - em->start;

	stripe_len = map->stripe_len;
	stripe_nr = offset;
	/*
	 * stripe_nr counts the total number of stripes we have to stride
	 * to get to this block
	 */
	stripe_nr = div64_u64(stripe_nr, stripe_len);

	stripe_offset = stripe_nr * stripe_len;
	if (offset < stripe_offset) {
		btrfs_crit(fs_info,
			   "stripe math has gone wrong, stripe_offset=%llu, offset=%llu, start=%llu, logical=%llu, stripe_len=%llu",
			   stripe_offset, offset, em->start, logical,
			   stripe_len);
		free_extent_map(em);
		return -EINVAL;
	}

	/* stripe_offset is the offset of this block in its stripe*/
	stripe_offset = offset - stripe_offset;

	/* if we're here for raid56, we need to know the stripe aligned start */
	if (map->type & BTRFS_BLOCK_GROUP_RAID56_MASK) {
		unsigned long full_stripe_len = stripe_len * nr_data_stripes(map);
		raid56_full_stripe_start = offset;

		/* allow a write of a full stripe, but make sure we don't
		 * allow straddling of stripes
		 */
		raid56_full_stripe_start = div64_u64(raid56_full_stripe_start,
				full_stripe_len);
		raid56_full_stripe_start *= full_stripe_len;
	}

	if (map->type & BTRFS_BLOCK_GROUP_PROFILE_MASK) {
		u64 max_len;
		/* For writes to RAID[56], allow a full stripeset across all disks.
		   For other RAID types and for RAID[56] reads, just allow a single
		   stripe (on a single disk). */
		if ((map->type & BTRFS_BLOCK_GROUP_RAID56_MASK) &&
		    (op == BTRFS_MAP_WRITE)) {
			max_len = stripe_len * nr_data_stripes(map) -
				(offset - raid56_full_stripe_start);
		} else {
			/* we limit the length of each bio to what fits in a stripe */
			max_len = stripe_len - stripe_offset;
		}
		*length = min_t(u64, em->len - offset, max_len);
	} else {
		*length = em->len - offset;
	}

	/* This is for when we're called from btrfs_merge_bio_hook() and all
	   it cares about is the length */
	if (!bbio_ret)
		goto out;

	btrfs_dev_replace_read_lock(dev_replace);
	dev_replace_is_ongoing = btrfs_dev_replace_is_ongoing(dev_replace);
	if (!dev_replace_is_ongoing)
		btrfs_dev_replace_read_unlock(dev_replace);
	else
		btrfs_dev_replace_set_lock_blocking(dev_replace);
=======
	struct btrfs_io_geometry geom;

	ASSERT(bbio_ret);
	ASSERT(op != BTRFS_MAP_DISCARD);

	em = btrfs_get_chunk_map(fs_info, logical, *length);
	ASSERT(!IS_ERR(em));

	ret = btrfs_get_io_geometry(fs_info, em, op, logical, &geom);
	if (ret < 0)
		return ret;

	map = em->map_lookup;

	*length = geom.len;
	stripe_len = geom.stripe_len;
	stripe_nr = geom.stripe_nr;
	stripe_offset = geom.stripe_offset;
	raid56_full_stripe_start = geom.raid56_stripe_offset;
	data_stripes = nr_data_stripes(map);

	down_read(&dev_replace->rwsem);
	dev_replace_is_ongoing = btrfs_dev_replace_is_ongoing(dev_replace);
	/*
	 * Hold the semaphore for read during the whole operation, write is
	 * requested at commit time but must wait.
	 */
	if (!dev_replace_is_ongoing)
		up_read(&dev_replace->rwsem);
>>>>>>> upstream/android-13

	if (dev_replace_is_ongoing && mirror_num == map->num_stripes + 1 &&
	    !need_full_stripe(op) && dev_replace->tgtdev != NULL) {
		ret = get_extra_mirror_from_replace(fs_info, logical, *length,
						    dev_replace->srcdev->devid,
						    &mirror_num,
					    &physical_to_patch_in_first_stripe);
		if (ret)
			goto out;
		else
			patch_the_first_stripe_for_dev_replace = 1;
	} else if (mirror_num > map->num_stripes) {
		mirror_num = 0;
	}

	num_stripes = 1;
	stripe_index = 0;
	if (map->type & BTRFS_BLOCK_GROUP_RAID0) {
		stripe_nr = div_u64_rem(stripe_nr, map->num_stripes,
				&stripe_index);
		if (!need_full_stripe(op))
			mirror_num = 1;
<<<<<<< HEAD
	} else if (map->type & BTRFS_BLOCK_GROUP_RAID1) {
=======
	} else if (map->type & BTRFS_BLOCK_GROUP_RAID1_MASK) {
>>>>>>> upstream/android-13
		if (need_full_stripe(op))
			num_stripes = map->num_stripes;
		else if (mirror_num)
			stripe_index = mirror_num - 1;
		else {
			stripe_index = find_live_mirror(fs_info, map, 0,
					    dev_replace_is_ongoing);
			mirror_num = stripe_index + 1;
		}

	} else if (map->type & BTRFS_BLOCK_GROUP_DUP) {
		if (need_full_stripe(op)) {
			num_stripes = map->num_stripes;
		} else if (mirror_num) {
			stripe_index = mirror_num - 1;
		} else {
			mirror_num = 1;
		}

	} else if (map->type & BTRFS_BLOCK_GROUP_RAID10) {
		u32 factor = map->num_stripes / map->sub_stripes;

		stripe_nr = div_u64_rem(stripe_nr, factor, &stripe_index);
		stripe_index *= map->sub_stripes;

		if (need_full_stripe(op))
			num_stripes = map->sub_stripes;
		else if (mirror_num)
			stripe_index += mirror_num - 1;
		else {
			int old_stripe_index = stripe_index;
			stripe_index = find_live_mirror(fs_info, map,
					      stripe_index,
					      dev_replace_is_ongoing);
			mirror_num = stripe_index - old_stripe_index + 1;
		}

	} else if (map->type & BTRFS_BLOCK_GROUP_RAID56_MASK) {
		if (need_raid_map && (need_full_stripe(op) || mirror_num > 1)) {
			/* push stripe_nr back to the start of the full stripe */
			stripe_nr = div64_u64(raid56_full_stripe_start,
<<<<<<< HEAD
					stripe_len * nr_data_stripes(map));
=======
					stripe_len * data_stripes);
>>>>>>> upstream/android-13

			/* RAID[56] write or recovery. Return all stripes */
			num_stripes = map->num_stripes;
			max_errors = nr_parity_stripes(map);

			*length = map->stripe_len;
			stripe_index = 0;
			stripe_offset = 0;
		} else {
			/*
			 * Mirror #0 or #1 means the original data block.
			 * Mirror #2 is RAID5 parity block.
			 * Mirror #3 is RAID6 Q block.
			 */
			stripe_nr = div_u64_rem(stripe_nr,
<<<<<<< HEAD
					nr_data_stripes(map), &stripe_index);
			if (mirror_num > 1)
				stripe_index = nr_data_stripes(map) +
						mirror_num - 2;
=======
					data_stripes, &stripe_index);
			if (mirror_num > 1)
				stripe_index = data_stripes + mirror_num - 2;
>>>>>>> upstream/android-13

			/* We distribute the parity blocks across stripes */
			div_u64_rem(stripe_nr + stripe_index, map->num_stripes,
					&stripe_index);
			if (!need_full_stripe(op) && mirror_num <= 1)
				mirror_num = 1;
		}
	} else {
		/*
		 * after this, stripe_nr is the number of stripes on this
		 * device we have to walk to find the data, and stripe_index is
		 * the number of our device in the stripe array
		 */
		stripe_nr = div_u64_rem(stripe_nr, map->num_stripes,
				&stripe_index);
		mirror_num = stripe_index + 1;
	}
	if (stripe_index >= map->num_stripes) {
		btrfs_crit(fs_info,
			   "stripe index math went horribly wrong, got stripe_index=%u, num_stripes=%u",
			   stripe_index, map->num_stripes);
		ret = -EINVAL;
		goto out;
	}

	num_alloc_stripes = num_stripes;
	if (dev_replace_is_ongoing && dev_replace->tgtdev != NULL) {
		if (op == BTRFS_MAP_WRITE)
			num_alloc_stripes <<= 1;
		if (op == BTRFS_MAP_GET_READ_MIRRORS)
			num_alloc_stripes++;
		tgtdev_indexes = num_stripes;
	}

	bbio = alloc_btrfs_bio(num_alloc_stripes, tgtdev_indexes);
	if (!bbio) {
		ret = -ENOMEM;
		goto out;
	}
<<<<<<< HEAD
	if (dev_replace_is_ongoing && dev_replace->tgtdev != NULL)
		bbio->tgtdev_map = (int *)(bbio->stripes + num_alloc_stripes);
=======

	for (i = 0; i < num_stripes; i++) {
		bbio->stripes[i].physical = map->stripes[stripe_index].physical +
			stripe_offset + stripe_nr * map->stripe_len;
		bbio->stripes[i].dev = map->stripes[stripe_index].dev;
		stripe_index++;
	}
>>>>>>> upstream/android-13

	/* build raid_map */
	if (map->type & BTRFS_BLOCK_GROUP_RAID56_MASK && need_raid_map &&
	    (need_full_stripe(op) || mirror_num > 1)) {
		u64 tmp;
		unsigned rot;

<<<<<<< HEAD
		bbio->raid_map = (u64 *)((void *)bbio->stripes +
				 sizeof(struct btrfs_bio_stripe) *
				 num_alloc_stripes +
				 sizeof(int) * tgtdev_indexes);

=======
>>>>>>> upstream/android-13
		/* Work out the disk rotation on this stripe-set */
		div_u64_rem(stripe_nr, num_stripes, &rot);

		/* Fill in the logical address of each stripe */
<<<<<<< HEAD
		tmp = stripe_nr * nr_data_stripes(map);
		for (i = 0; i < nr_data_stripes(map); i++)
=======
		tmp = stripe_nr * data_stripes;
		for (i = 0; i < data_stripes; i++)
>>>>>>> upstream/android-13
			bbio->raid_map[(i+rot) % num_stripes] =
				em->start + (tmp + i) * map->stripe_len;

		bbio->raid_map[(i+rot) % map->num_stripes] = RAID5_P_STRIPE;
		if (map->type & BTRFS_BLOCK_GROUP_RAID6)
			bbio->raid_map[(i+rot+1) % num_stripes] =
				RAID6_Q_STRIPE;
<<<<<<< HEAD
	}


	for (i = 0; i < num_stripes; i++) {
		bbio->stripes[i].physical =
			map->stripes[stripe_index].physical +
			stripe_offset +
			stripe_nr * map->stripe_len;
		bbio->stripes[i].dev =
			map->stripes[stripe_index].dev;
		stripe_index++;
=======

		sort_parity_stripes(bbio, num_stripes);
>>>>>>> upstream/android-13
	}

	if (need_full_stripe(op))
		max_errors = btrfs_chunk_max_errors(map);

<<<<<<< HEAD
	if (bbio->raid_map)
		sort_parity_stripes(bbio, num_stripes);

	if (dev_replace_is_ongoing && dev_replace->tgtdev != NULL &&
	    need_full_stripe(op)) {
		handle_ops_on_dev_replace(op, &bbio, dev_replace, &num_stripes,
					  &max_errors);
=======
	if (dev_replace_is_ongoing && dev_replace->tgtdev != NULL &&
	    need_full_stripe(op)) {
		handle_ops_on_dev_replace(op, &bbio, dev_replace, logical,
					  &num_stripes, &max_errors);
>>>>>>> upstream/android-13
	}

	*bbio_ret = bbio;
	bbio->map_type = map->type;
	bbio->num_stripes = num_stripes;
	bbio->max_errors = max_errors;
	bbio->mirror_num = mirror_num;

	/*
	 * this is the case that REQ_READ && dev_replace_is_ongoing &&
	 * mirror_num == num_stripes + 1 && dev_replace target drive is
	 * available as a mirror
	 */
	if (patch_the_first_stripe_for_dev_replace && num_stripes > 0) {
		WARN_ON(num_stripes > 1);
		bbio->stripes[0].dev = dev_replace->tgtdev;
		bbio->stripes[0].physical = physical_to_patch_in_first_stripe;
		bbio->mirror_num = map->num_stripes + 1;
	}
out:
	if (dev_replace_is_ongoing) {
<<<<<<< HEAD
		btrfs_dev_replace_clear_lock_blocking(dev_replace);
		btrfs_dev_replace_read_unlock(dev_replace);
=======
		lockdep_assert_held(&dev_replace->rwsem);
		/* Unlock and let waiting writers proceed */
		up_read(&dev_replace->rwsem);
>>>>>>> upstream/android-13
	}
	free_extent_map(em);
	return ret;
}

int btrfs_map_block(struct btrfs_fs_info *fs_info, enum btrfs_map_op op,
		      u64 logical, u64 *length,
		      struct btrfs_bio **bbio_ret, int mirror_num)
{
<<<<<<< HEAD
=======
	if (op == BTRFS_MAP_DISCARD)
		return __btrfs_map_block_for_discard(fs_info, logical,
						     length, bbio_ret);

>>>>>>> upstream/android-13
	return __btrfs_map_block(fs_info, op, logical, length, bbio_ret,
				 mirror_num, 0);
}

/* For Scrub/replace */
int btrfs_map_sblock(struct btrfs_fs_info *fs_info, enum btrfs_map_op op,
		     u64 logical, u64 *length,
		     struct btrfs_bio **bbio_ret)
{
	return __btrfs_map_block(fs_info, op, logical, length, bbio_ret, 0, 1);
}

<<<<<<< HEAD
int btrfs_rmap_block(struct btrfs_fs_info *fs_info, u64 chunk_start,
		     u64 physical, u64 **logical, int *naddrs, int *stripe_len)
{
	struct extent_map *em;
	struct map_lookup *map;
	u64 *buf;
	u64 bytenr;
	u64 length;
	u64 stripe_nr;
	u64 rmap_len;
	int i, j, nr = 0;

	em = get_chunk_map(fs_info, chunk_start, 1);
	if (IS_ERR(em))
		return -EIO;

	map = em->map_lookup;
	length = em->len;
	rmap_len = map->stripe_len;

	if (map->type & BTRFS_BLOCK_GROUP_RAID10)
		length = div_u64(length, map->num_stripes / map->sub_stripes);
	else if (map->type & BTRFS_BLOCK_GROUP_RAID0)
		length = div_u64(length, map->num_stripes);
	else if (map->type & BTRFS_BLOCK_GROUP_RAID56_MASK) {
		length = div_u64(length, nr_data_stripes(map));
		rmap_len = map->stripe_len * nr_data_stripes(map);
	}

	buf = kcalloc(map->num_stripes, sizeof(u64), GFP_NOFS);
	BUG_ON(!buf); /* -ENOMEM */

	for (i = 0; i < map->num_stripes; i++) {
		if (map->stripes[i].physical > physical ||
		    map->stripes[i].physical + length <= physical)
			continue;

		stripe_nr = physical - map->stripes[i].physical;
		stripe_nr = div64_u64(stripe_nr, map->stripe_len);

		if (map->type & BTRFS_BLOCK_GROUP_RAID10) {
			stripe_nr = stripe_nr * map->num_stripes + i;
			stripe_nr = div_u64(stripe_nr, map->sub_stripes);
		} else if (map->type & BTRFS_BLOCK_GROUP_RAID0) {
			stripe_nr = stripe_nr * map->num_stripes + i;
		} /* else if RAID[56], multiply by nr_data_stripes().
		   * Alternatively, just use rmap_len below instead of
		   * map->stripe_len */

		bytenr = chunk_start + stripe_nr * rmap_len;
		WARN_ON(nr >= map->num_stripes);
		for (j = 0; j < nr; j++) {
			if (buf[j] == bytenr)
				break;
		}
		if (j == nr) {
			WARN_ON(nr >= map->num_stripes);
			buf[nr++] = bytenr;
		}
	}

	*logical = buf;
	*naddrs = nr;
	*stripe_len = rmap_len;

	free_extent_map(em);
	return 0;
}

=======
>>>>>>> upstream/android-13
static inline void btrfs_end_bbio(struct btrfs_bio *bbio, struct bio *bio)
{
	bio->bi_private = bbio->private;
	bio->bi_end_io = bbio->end_io;
	bio_endio(bio);

	btrfs_put_bbio(bbio);
}

static void btrfs_end_bio(struct bio *bio)
{
	struct btrfs_bio *bbio = bio->bi_private;
	int is_orig_bio = 0;

	if (bio->bi_status) {
		atomic_inc(&bbio->error);
		if (bio->bi_status == BLK_STS_IOERR ||
		    bio->bi_status == BLK_STS_TARGET) {
<<<<<<< HEAD
			unsigned int stripe_index =
				btrfs_io_bio(bio)->stripe_index;
			struct btrfs_device *dev;

			BUG_ON(stripe_index >= bbio->num_stripes);
			dev = bbio->stripes[stripe_index].dev;
			if (dev->bdev) {
				if (bio_op(bio) == REQ_OP_WRITE)
					btrfs_dev_stat_inc_and_print(dev,
						BTRFS_DEV_STAT_WRITE_ERRS);
				else if (!(bio->bi_opf & REQ_RAHEAD))
					btrfs_dev_stat_inc_and_print(dev,
						BTRFS_DEV_STAT_READ_ERRS);
				if (bio->bi_opf & REQ_PREFLUSH)
					btrfs_dev_stat_inc_and_print(dev,
						BTRFS_DEV_STAT_FLUSH_ERRS);
			}
=======
			struct btrfs_device *dev = btrfs_io_bio(bio)->device;

			ASSERT(dev->bdev);
			if (btrfs_op(bio) == BTRFS_MAP_WRITE)
				btrfs_dev_stat_inc_and_print(dev,
						BTRFS_DEV_STAT_WRITE_ERRS);
			else if (!(bio->bi_opf & REQ_RAHEAD))
				btrfs_dev_stat_inc_and_print(dev,
						BTRFS_DEV_STAT_READ_ERRS);
			if (bio->bi_opf & REQ_PREFLUSH)
				btrfs_dev_stat_inc_and_print(dev,
						BTRFS_DEV_STAT_FLUSH_ERRS);
>>>>>>> upstream/android-13
		}
	}

	if (bio == bbio->orig_bio)
		is_orig_bio = 1;

	btrfs_bio_counter_dec(bbio->fs_info);

	if (atomic_dec_and_test(&bbio->stripes_pending)) {
		if (!is_orig_bio) {
			bio_put(bio);
			bio = bbio->orig_bio;
		}

		btrfs_io_bio(bio)->mirror_num = bbio->mirror_num;
		/* only send an error to the higher layers if it is
		 * beyond the tolerance of the btrfs bio
		 */
		if (atomic_read(&bbio->error) > bbio->max_errors) {
			bio->bi_status = BLK_STS_IOERR;
		} else {
			/*
			 * this bio is actually up to date, we didn't
			 * go over the max number of errors
			 */
			bio->bi_status = BLK_STS_OK;
		}

		btrfs_end_bbio(bbio, bio);
	} else if (!is_orig_bio) {
		bio_put(bio);
	}
}

<<<<<<< HEAD
/*
 * see run_scheduled_bios for a description of why bios are collected for
 * async submit.
 *
 * This will add one bio to the pending list for a device and make sure
 * the work struct is scheduled.
 */
static noinline void btrfs_schedule_bio(struct btrfs_device *device,
					struct bio *bio)
{
	struct btrfs_fs_info *fs_info = device->fs_info;
	int should_queue = 1;
	struct btrfs_pending_bios *pending_bios;

	/* don't bother with additional async steps for reads, right now */
	if (bio_op(bio) == REQ_OP_READ) {
		btrfsic_submit_bio(bio);
		return;
	}

	WARN_ON(bio->bi_next);
	bio->bi_next = NULL;

	spin_lock(&device->io_lock);
	if (op_is_sync(bio->bi_opf))
		pending_bios = &device->pending_sync_bios;
	else
		pending_bios = &device->pending_bios;

	if (pending_bios->tail)
		pending_bios->tail->bi_next = bio;

	pending_bios->tail = bio;
	if (!pending_bios->head)
		pending_bios->head = bio;
	if (device->running_pending)
		should_queue = 0;

	spin_unlock(&device->io_lock);

	if (should_queue)
		btrfs_queue_work(fs_info->submit_workers, &device->work);
}

static void submit_stripe_bio(struct btrfs_bio *bbio, struct bio *bio,
			      u64 physical, int dev_nr, int async)
{
	struct btrfs_device *dev = bbio->stripes[dev_nr].dev;
	struct btrfs_fs_info *fs_info = bbio->fs_info;

	bio->bi_private = bbio;
	btrfs_io_bio(bio)->stripe_index = dev_nr;
	bio->bi_end_io = btrfs_end_bio;
	bio->bi_iter.bi_sector = physical >> 9;
	btrfs_debug_in_rcu(fs_info,
	"btrfs_map_bio: rw %d 0x%x, sector=%llu, dev=%lu (%s id %llu), size=%u",
		bio_op(bio), bio->bi_opf, (u64)bio->bi_iter.bi_sector,
		(u_long)dev->bdev->bd_dev, rcu_str_deref(dev->name), dev->devid,
		bio->bi_iter.bi_size);
=======
static void submit_stripe_bio(struct btrfs_bio *bbio, struct bio *bio,
			      u64 physical, struct btrfs_device *dev)
{
	struct btrfs_fs_info *fs_info = bbio->fs_info;

	bio->bi_private = bbio;
	btrfs_io_bio(bio)->device = dev;
	bio->bi_end_io = btrfs_end_bio;
	bio->bi_iter.bi_sector = physical >> 9;
	/*
	 * For zone append writing, bi_sector must point the beginning of the
	 * zone
	 */
	if (bio_op(bio) == REQ_OP_ZONE_APPEND) {
		if (btrfs_dev_is_sequential(dev, physical)) {
			u64 zone_start = round_down(physical, fs_info->zone_size);

			bio->bi_iter.bi_sector = zone_start >> SECTOR_SHIFT;
		} else {
			bio->bi_opf &= ~REQ_OP_ZONE_APPEND;
			bio->bi_opf |= REQ_OP_WRITE;
		}
	}
	btrfs_debug_in_rcu(fs_info,
	"btrfs_map_bio: rw %d 0x%x, sector=%llu, dev=%lu (%s id %llu), size=%u",
		bio_op(bio), bio->bi_opf, bio->bi_iter.bi_sector,
		(unsigned long)dev->bdev->bd_dev, rcu_str_deref(dev->name),
		dev->devid, bio->bi_iter.bi_size);
>>>>>>> upstream/android-13
	bio_set_dev(bio, dev->bdev);

	btrfs_bio_counter_inc_noblocked(fs_info);

<<<<<<< HEAD
	if (async)
		btrfs_schedule_bio(dev, bio);
	else
		btrfsic_submit_bio(bio);
=======
	btrfsic_submit_bio(bio);
>>>>>>> upstream/android-13
}

static void bbio_error(struct btrfs_bio *bbio, struct bio *bio, u64 logical)
{
	atomic_inc(&bbio->error);
	if (atomic_dec_and_test(&bbio->stripes_pending)) {
		/* Should be the original bio. */
		WARN_ON(bio != bbio->orig_bio);

		btrfs_io_bio(bio)->mirror_num = bbio->mirror_num;
		bio->bi_iter.bi_sector = logical >> 9;
		if (atomic_read(&bbio->error) > bbio->max_errors)
			bio->bi_status = BLK_STS_IOERR;
		else
			bio->bi_status = BLK_STS_OK;
		btrfs_end_bbio(bbio, bio);
	}
}

blk_status_t btrfs_map_bio(struct btrfs_fs_info *fs_info, struct bio *bio,
<<<<<<< HEAD
			   int mirror_num, int async_submit)
{
	struct btrfs_device *dev;
	struct bio *first_bio = bio;
	u64 logical = (u64)bio->bi_iter.bi_sector << 9;
=======
			   int mirror_num)
{
	struct btrfs_device *dev;
	struct bio *first_bio = bio;
	u64 logical = bio->bi_iter.bi_sector << 9;
>>>>>>> upstream/android-13
	u64 length = 0;
	u64 map_length;
	int ret;
	int dev_nr;
	int total_devs;
	struct btrfs_bio *bbio = NULL;

	length = bio->bi_iter.bi_size;
	map_length = length;

	btrfs_bio_counter_inc_blocked(fs_info);
	ret = __btrfs_map_block(fs_info, btrfs_op(bio), logical,
				&map_length, &bbio, mirror_num, 1);
	if (ret) {
		btrfs_bio_counter_dec(fs_info);
		return errno_to_blk_status(ret);
	}

	total_devs = bbio->num_stripes;
	bbio->orig_bio = first_bio;
	bbio->private = first_bio->bi_private;
	bbio->end_io = first_bio->bi_end_io;
	bbio->fs_info = fs_info;
	atomic_set(&bbio->stripes_pending, bbio->num_stripes);

	if ((bbio->map_type & BTRFS_BLOCK_GROUP_RAID56_MASK) &&
<<<<<<< HEAD
	    ((bio_op(bio) == REQ_OP_WRITE) || (mirror_num > 1))) {
		/* In this case, map_length has been set to the length of
		   a single stripe; not the whole write */
		if (bio_op(bio) == REQ_OP_WRITE) {
=======
	    ((btrfs_op(bio) == BTRFS_MAP_WRITE) || (mirror_num > 1))) {
		/* In this case, map_length has been set to the length of
		   a single stripe; not the whole write */
		if (btrfs_op(bio) == BTRFS_MAP_WRITE) {
>>>>>>> upstream/android-13
			ret = raid56_parity_write(fs_info, bio, bbio,
						  map_length);
		} else {
			ret = raid56_parity_recover(fs_info, bio, bbio,
						    map_length, mirror_num, 1);
		}

		btrfs_bio_counter_dec(fs_info);
		return errno_to_blk_status(ret);
	}

	if (map_length < length) {
		btrfs_crit(fs_info,
			   "mapping failed logical %llu bio len %llu len %llu",
			   logical, length, map_length);
		BUG();
	}

	for (dev_nr = 0; dev_nr < total_devs; dev_nr++) {
		dev = bbio->stripes[dev_nr].dev;
		if (!dev || !dev->bdev || test_bit(BTRFS_DEV_STATE_MISSING,
						   &dev->dev_state) ||
<<<<<<< HEAD
		    (bio_op(first_bio) == REQ_OP_WRITE &&
=======
		    (btrfs_op(first_bio) == BTRFS_MAP_WRITE &&
>>>>>>> upstream/android-13
		    !test_bit(BTRFS_DEV_STATE_WRITEABLE, &dev->dev_state))) {
			bbio_error(bbio, first_bio, logical);
			continue;
		}

		if (dev_nr < total_devs - 1)
			bio = btrfs_bio_clone(first_bio);
		else
			bio = first_bio;

<<<<<<< HEAD
		submit_stripe_bio(bbio, bio, bbio->stripes[dev_nr].physical,
				  dev_nr, async_submit);
=======
		submit_stripe_bio(bbio, bio, bbio->stripes[dev_nr].physical, dev);
>>>>>>> upstream/android-13
	}
	btrfs_bio_counter_dec(fs_info);
	return BLK_STS_OK;
}

/*
 * Find a device specified by @devid or @uuid in the list of @fs_devices, or
 * return NULL.
 *
 * If devid and uuid are both specified, the match must be exact, otherwise
 * only devid is used.
<<<<<<< HEAD
 *
 * If @seed is true, traverse through the seed devices.
 */
struct btrfs_device *btrfs_find_device(struct btrfs_fs_devices *fs_devices,
					u64 devid, u8 *uuid, u8 *fsid,
					bool seed)
{
	struct btrfs_device *device;

	while (fs_devices) {
		if (!fsid ||
		    !memcmp(fs_devices->fsid, fsid, BTRFS_FSID_SIZE)) {
			list_for_each_entry(device, &fs_devices->devices,
=======
 */
struct btrfs_device *btrfs_find_device(struct btrfs_fs_devices *fs_devices,
				       u64 devid, u8 *uuid, u8 *fsid)
{
	struct btrfs_device *device;
	struct btrfs_fs_devices *seed_devs;

	if (!fsid || !memcmp(fs_devices->metadata_uuid, fsid, BTRFS_FSID_SIZE)) {
		list_for_each_entry(device, &fs_devices->devices, dev_list) {
			if (device->devid == devid &&
			    (!uuid || memcmp(device->uuid, uuid,
					     BTRFS_UUID_SIZE) == 0))
				return device;
		}
	}

	list_for_each_entry(seed_devs, &fs_devices->seed_list, seed_list) {
		if (!fsid ||
		    !memcmp(seed_devs->metadata_uuid, fsid, BTRFS_FSID_SIZE)) {
			list_for_each_entry(device, &seed_devs->devices,
>>>>>>> upstream/android-13
					    dev_list) {
				if (device->devid == devid &&
				    (!uuid || memcmp(device->uuid, uuid,
						     BTRFS_UUID_SIZE) == 0))
					return device;
			}
		}
<<<<<<< HEAD
		if (seed)
			fs_devices = fs_devices->seed;
		else
			return NULL;
	}
=======
	}

>>>>>>> upstream/android-13
	return NULL;
}

static struct btrfs_device *add_missing_dev(struct btrfs_fs_devices *fs_devices,
					    u64 devid, u8 *dev_uuid)
{
	struct btrfs_device *device;
	unsigned int nofs_flag;

	/*
	 * We call this under the chunk_mutex, so we want to use NOFS for this
	 * allocation, however we don't want to change btrfs_alloc_device() to
	 * always do NOFS because we use it in a lot of other GFP_KERNEL safe
	 * places.
	 */
	nofs_flag = memalloc_nofs_save();
	device = btrfs_alloc_device(NULL, &devid, dev_uuid);
	memalloc_nofs_restore(nofs_flag);
	if (IS_ERR(device))
		return device;

	list_add(&device->dev_list, &fs_devices->devices);
	device->fs_devices = fs_devices;
	fs_devices->num_devices++;

	set_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state);
	fs_devices->missing_devices++;

	return device;
}

/**
 * btrfs_alloc_device - allocate struct btrfs_device
 * @fs_info:	used only for generating a new devid, can be NULL if
 *		devid is provided (i.e. @devid != NULL).
 * @devid:	a pointer to devid for this device.  If NULL a new devid
 *		is generated.
 * @uuid:	a pointer to UUID for this device.  If NULL a new UUID
 *		is generated.
 *
 * Return: a pointer to a new &struct btrfs_device on success; ERR_PTR()
 * on error.  Returned struct is not linked onto any lists and must be
 * destroyed with btrfs_free_device.
 */
struct btrfs_device *btrfs_alloc_device(struct btrfs_fs_info *fs_info,
					const u64 *devid,
					const u8 *uuid)
{
	struct btrfs_device *dev;
	u64 tmp;

	if (WARN_ON(!devid && !fs_info))
		return ERR_PTR(-EINVAL);

<<<<<<< HEAD
	dev = __alloc_device();
	if (IS_ERR(dev))
		return dev;
=======
	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	if (!dev)
		return ERR_PTR(-ENOMEM);

	/*
	 * Preallocate a bio that's always going to be used for flushing device
	 * barriers and matches the device lifespan
	 */
	dev->flush_bio = bio_kmalloc(GFP_KERNEL, 0);
	if (!dev->flush_bio) {
		kfree(dev);
		return ERR_PTR(-ENOMEM);
	}

	INIT_LIST_HEAD(&dev->dev_list);
	INIT_LIST_HEAD(&dev->dev_alloc_list);
	INIT_LIST_HEAD(&dev->post_commit_list);

	atomic_set(&dev->reada_in_flight, 0);
	atomic_set(&dev->dev_stats_ccnt, 0);
	btrfs_device_data_ordered_init(dev);
	INIT_RADIX_TREE(&dev->reada_zones, GFP_NOFS & ~__GFP_DIRECT_RECLAIM);
	INIT_RADIX_TREE(&dev->reada_extents, GFP_NOFS & ~__GFP_DIRECT_RECLAIM);
	extent_io_tree_init(fs_info, &dev->alloc_state,
			    IO_TREE_DEVICE_ALLOC_STATE, NULL);
>>>>>>> upstream/android-13

	if (devid)
		tmp = *devid;
	else {
		int ret;

		ret = find_next_devid(fs_info, &tmp);
		if (ret) {
			btrfs_free_device(dev);
			return ERR_PTR(ret);
		}
	}
	dev->devid = tmp;

	if (uuid)
		memcpy(dev->uuid, uuid, BTRFS_UUID_SIZE);
	else
		generate_random_uuid(dev->uuid);

<<<<<<< HEAD
	btrfs_init_work(&dev->work, btrfs_submit_helper,
			pending_bios_fn, NULL, NULL);

=======
>>>>>>> upstream/android-13
	return dev;
}

static void btrfs_report_missing_device(struct btrfs_fs_info *fs_info,
					u64 devid, u8 *uuid, bool error)
{
	if (error)
		btrfs_err_rl(fs_info, "devid %llu uuid %pU is missing",
			      devid, uuid);
	else
		btrfs_warn_rl(fs_info, "devid %llu uuid %pU is missing",
			      devid, uuid);
}

<<<<<<< HEAD
static int read_one_chunk(struct btrfs_fs_info *fs_info, struct btrfs_key *key,
			  struct extent_buffer *leaf,
			  struct btrfs_chunk *chunk)
{
	struct btrfs_mapping_tree *map_tree = &fs_info->mapping_tree;
=======
static u64 calc_stripe_length(u64 type, u64 chunk_len, int num_stripes)
{
	const int data_stripes = calc_data_stripes(type, num_stripes);

	return div_u64(chunk_len, data_stripes);
}

#if BITS_PER_LONG == 32
/*
 * Due to page cache limit, metadata beyond BTRFS_32BIT_MAX_FILE_SIZE
 * can't be accessed on 32bit systems.
 *
 * This function do mount time check to reject the fs if it already has
 * metadata chunk beyond that limit.
 */
static int check_32bit_meta_chunk(struct btrfs_fs_info *fs_info,
				  u64 logical, u64 length, u64 type)
{
	if (!(type & BTRFS_BLOCK_GROUP_METADATA))
		return 0;

	if (logical + length < MAX_LFS_FILESIZE)
		return 0;

	btrfs_err_32bit_limit(fs_info);
	return -EOVERFLOW;
}

/*
 * This is to give early warning for any metadata chunk reaching
 * BTRFS_32BIT_EARLY_WARN_THRESHOLD.
 * Although we can still access the metadata, it's not going to be possible
 * once the limit is reached.
 */
static void warn_32bit_meta_chunk(struct btrfs_fs_info *fs_info,
				  u64 logical, u64 length, u64 type)
{
	if (!(type & BTRFS_BLOCK_GROUP_METADATA))
		return;

	if (logical + length < BTRFS_32BIT_EARLY_WARN_THRESHOLD)
		return;

	btrfs_warn_32bit_limit(fs_info);
}
#endif

static int read_one_chunk(struct btrfs_key *key, struct extent_buffer *leaf,
			  struct btrfs_chunk *chunk)
{
	struct btrfs_fs_info *fs_info = leaf->fs_info;
	struct extent_map_tree *map_tree = &fs_info->mapping_tree;
>>>>>>> upstream/android-13
	struct map_lookup *map;
	struct extent_map *em;
	u64 logical;
	u64 length;
	u64 devid;
<<<<<<< HEAD
=======
	u64 type;
>>>>>>> upstream/android-13
	u8 uuid[BTRFS_UUID_SIZE];
	int num_stripes;
	int ret;
	int i;

	logical = key->offset;
	length = btrfs_chunk_length(leaf, chunk);
<<<<<<< HEAD
	num_stripes = btrfs_chunk_num_stripes(leaf, chunk);

=======
	type = btrfs_chunk_type(leaf, chunk);
	num_stripes = btrfs_chunk_num_stripes(leaf, chunk);

#if BITS_PER_LONG == 32
	ret = check_32bit_meta_chunk(fs_info, logical, length, type);
	if (ret < 0)
		return ret;
	warn_32bit_meta_chunk(fs_info, logical, length, type);
#endif

>>>>>>> upstream/android-13
	/*
	 * Only need to verify chunk item if we're reading from sys chunk array,
	 * as chunk item in tree block is already verified by tree-checker.
	 */
	if (leaf->start == BTRFS_SUPER_INFO_OFFSET) {
<<<<<<< HEAD
		ret = btrfs_check_chunk_valid(fs_info, leaf, chunk, logical);
=======
		ret = btrfs_check_chunk_valid(leaf, chunk, logical);
>>>>>>> upstream/android-13
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	read_lock(&map_tree->map_tree.lock);
	em = lookup_extent_mapping(&map_tree->map_tree, logical, 1);
	read_unlock(&map_tree->map_tree.lock);
=======
	read_lock(&map_tree->lock);
	em = lookup_extent_mapping(map_tree, logical, 1);
	read_unlock(&map_tree->lock);
>>>>>>> upstream/android-13

	/* already mapped? */
	if (em && em->start <= logical && em->start + em->len > logical) {
		free_extent_map(em);
		return 0;
	} else if (em) {
		free_extent_map(em);
	}

	em = alloc_extent_map();
	if (!em)
		return -ENOMEM;
	map = kmalloc(map_lookup_size(num_stripes), GFP_NOFS);
	if (!map) {
		free_extent_map(em);
		return -ENOMEM;
	}

	set_bit(EXTENT_FLAG_FS_MAPPING, &em->flags);
	em->map_lookup = map;
	em->start = logical;
	em->len = length;
	em->orig_start = 0;
	em->block_start = 0;
	em->block_len = em->len;

	map->num_stripes = num_stripes;
	map->io_width = btrfs_chunk_io_width(leaf, chunk);
	map->io_align = btrfs_chunk_io_align(leaf, chunk);
	map->stripe_len = btrfs_chunk_stripe_len(leaf, chunk);
<<<<<<< HEAD
	map->type = btrfs_chunk_type(leaf, chunk);
	map->sub_stripes = btrfs_chunk_sub_stripes(leaf, chunk);
	map->verified_stripes = 0;
=======
	map->type = type;
	map->sub_stripes = btrfs_chunk_sub_stripes(leaf, chunk);
	map->verified_stripes = 0;
	em->orig_block_len = calc_stripe_length(type, em->len,
						map->num_stripes);
>>>>>>> upstream/android-13
	for (i = 0; i < num_stripes; i++) {
		map->stripes[i].physical =
			btrfs_stripe_offset_nr(leaf, chunk, i);
		devid = btrfs_stripe_devid_nr(leaf, chunk, i);
		read_extent_buffer(leaf, uuid, (unsigned long)
				   btrfs_stripe_dev_uuid_nr(chunk, i),
				   BTRFS_UUID_SIZE);
		map->stripes[i].dev = btrfs_find_device(fs_info->fs_devices,
<<<<<<< HEAD
						devid, uuid, NULL, true);
=======
							devid, uuid, NULL);
>>>>>>> upstream/android-13
		if (!map->stripes[i].dev &&
		    !btrfs_test_opt(fs_info, DEGRADED)) {
			free_extent_map(em);
			btrfs_report_missing_device(fs_info, devid, uuid, true);
			return -ENOENT;
		}
		if (!map->stripes[i].dev) {
			map->stripes[i].dev =
				add_missing_dev(fs_info->fs_devices, devid,
						uuid);
			if (IS_ERR(map->stripes[i].dev)) {
				free_extent_map(em);
				btrfs_err(fs_info,
					"failed to init missing dev %llu: %ld",
					devid, PTR_ERR(map->stripes[i].dev));
				return PTR_ERR(map->stripes[i].dev);
			}
			btrfs_report_missing_device(fs_info, devid, uuid, false);
		}
		set_bit(BTRFS_DEV_STATE_IN_FS_METADATA,
				&(map->stripes[i].dev->dev_state));

	}

<<<<<<< HEAD
	write_lock(&map_tree->map_tree.lock);
	ret = add_extent_mapping(&map_tree->map_tree, em, 0);
	write_unlock(&map_tree->map_tree.lock);
=======
	write_lock(&map_tree->lock);
	ret = add_extent_mapping(map_tree, em, 0);
	write_unlock(&map_tree->lock);
>>>>>>> upstream/android-13
	if (ret < 0) {
		btrfs_err(fs_info,
			  "failed to add chunk map, start=%llu len=%llu: %d",
			  em->start, em->len, ret);
	}
	free_extent_map(em);

	return ret;
}

static void fill_device_from_item(struct extent_buffer *leaf,
				 struct btrfs_dev_item *dev_item,
				 struct btrfs_device *device)
{
	unsigned long ptr;

	device->devid = btrfs_device_id(leaf, dev_item);
	device->disk_total_bytes = btrfs_device_total_bytes(leaf, dev_item);
	device->total_bytes = device->disk_total_bytes;
	device->commit_total_bytes = device->disk_total_bytes;
	device->bytes_used = btrfs_device_bytes_used(leaf, dev_item);
	device->commit_bytes_used = device->bytes_used;
	device->type = btrfs_device_type(leaf, dev_item);
	device->io_align = btrfs_device_io_align(leaf, dev_item);
	device->io_width = btrfs_device_io_width(leaf, dev_item);
	device->sector_size = btrfs_device_sector_size(leaf, dev_item);
	WARN_ON(device->devid == BTRFS_DEV_REPLACE_DEVID);
	clear_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state);

	ptr = btrfs_device_uuid(dev_item);
	read_extent_buffer(leaf, device->uuid, ptr, BTRFS_UUID_SIZE);
}

static struct btrfs_fs_devices *open_seed_devices(struct btrfs_fs_info *fs_info,
						  u8 *fsid)
{
	struct btrfs_fs_devices *fs_devices;
	int ret;

	lockdep_assert_held(&uuid_mutex);
	ASSERT(fsid);

<<<<<<< HEAD
	fs_devices = fs_info->fs_devices->seed;
	while (fs_devices) {
		if (!memcmp(fs_devices->fsid, fsid, BTRFS_FSID_SIZE))
			return fs_devices;

		fs_devices = fs_devices->seed;
	}

	fs_devices = find_fsid(fsid);
=======
	/* This will match only for multi-device seed fs */
	list_for_each_entry(fs_devices, &fs_info->fs_devices->seed_list, seed_list)
		if (!memcmp(fs_devices->fsid, fsid, BTRFS_FSID_SIZE))
			return fs_devices;


	fs_devices = find_fsid(fsid, NULL);
>>>>>>> upstream/android-13
	if (!fs_devices) {
		if (!btrfs_test_opt(fs_info, DEGRADED))
			return ERR_PTR(-ENOENT);

<<<<<<< HEAD
		fs_devices = alloc_fs_devices(fsid);
		if (IS_ERR(fs_devices))
			return fs_devices;

		fs_devices->seeding = 1;
=======
		fs_devices = alloc_fs_devices(fsid, NULL);
		if (IS_ERR(fs_devices))
			return fs_devices;

		fs_devices->seeding = true;
>>>>>>> upstream/android-13
		fs_devices->opened = 1;
		return fs_devices;
	}

<<<<<<< HEAD
=======
	/*
	 * Upon first call for a seed fs fsid, just create a private copy of the
	 * respective fs_devices and anchor it at fs_info->fs_devices->seed_list
	 */
>>>>>>> upstream/android-13
	fs_devices = clone_fs_devices(fs_devices);
	if (IS_ERR(fs_devices))
		return fs_devices;

	ret = open_fs_devices(fs_devices, FMODE_READ, fs_info->bdev_holder);
	if (ret) {
		free_fs_devices(fs_devices);
<<<<<<< HEAD
		fs_devices = ERR_PTR(ret);
		goto out;
=======
		return ERR_PTR(ret);
>>>>>>> upstream/android-13
	}

	if (!fs_devices->seeding) {
		close_fs_devices(fs_devices);
		free_fs_devices(fs_devices);
<<<<<<< HEAD
		fs_devices = ERR_PTR(-EINVAL);
		goto out;
	}

	fs_devices->seed = fs_info->fs_devices->seed;
	fs_info->fs_devices->seed = fs_devices;
out:
	return fs_devices;
}

static int read_one_dev(struct btrfs_fs_info *fs_info,
			struct extent_buffer *leaf,
			struct btrfs_dev_item *dev_item)
{
=======
		return ERR_PTR(-EINVAL);
	}

	list_add(&fs_devices->seed_list, &fs_info->fs_devices->seed_list);

	return fs_devices;
}

static int read_one_dev(struct extent_buffer *leaf,
			struct btrfs_dev_item *dev_item)
{
	struct btrfs_fs_info *fs_info = leaf->fs_info;
>>>>>>> upstream/android-13
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	struct btrfs_device *device;
	u64 devid;
	int ret;
	u8 fs_uuid[BTRFS_FSID_SIZE];
	u8 dev_uuid[BTRFS_UUID_SIZE];

	devid = btrfs_device_id(leaf, dev_item);
	read_extent_buffer(leaf, dev_uuid, btrfs_device_uuid(dev_item),
			   BTRFS_UUID_SIZE);
	read_extent_buffer(leaf, fs_uuid, btrfs_device_fsid(dev_item),
			   BTRFS_FSID_SIZE);

<<<<<<< HEAD
	if (memcmp(fs_uuid, fs_info->fsid, BTRFS_FSID_SIZE)) {
=======
	if (memcmp(fs_uuid, fs_devices->metadata_uuid, BTRFS_FSID_SIZE)) {
>>>>>>> upstream/android-13
		fs_devices = open_seed_devices(fs_info, fs_uuid);
		if (IS_ERR(fs_devices))
			return PTR_ERR(fs_devices);
	}

	device = btrfs_find_device(fs_info->fs_devices, devid, dev_uuid,
<<<<<<< HEAD
				   fs_uuid, true);
=======
				   fs_uuid);
>>>>>>> upstream/android-13
	if (!device) {
		if (!btrfs_test_opt(fs_info, DEGRADED)) {
			btrfs_report_missing_device(fs_info, devid,
							dev_uuid, true);
			return -ENOENT;
		}

		device = add_missing_dev(fs_devices, devid, dev_uuid);
		if (IS_ERR(device)) {
			btrfs_err(fs_info,
				"failed to add missing dev %llu: %ld",
				devid, PTR_ERR(device));
			return PTR_ERR(device);
		}
		btrfs_report_missing_device(fs_info, devid, dev_uuid, false);
	} else {
		if (!device->bdev) {
			if (!btrfs_test_opt(fs_info, DEGRADED)) {
				btrfs_report_missing_device(fs_info,
						devid, dev_uuid, true);
				return -ENOENT;
			}
			btrfs_report_missing_device(fs_info, devid,
							dev_uuid, false);
		}

		if (!device->bdev &&
		    !test_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state)) {
			/*
			 * this happens when a device that was properly setup
			 * in the device info lists suddenly goes bad.
			 * device->bdev is NULL, and so we have to set
			 * device->missing to one here
			 */
			device->fs_devices->missing_devices++;
			set_bit(BTRFS_DEV_STATE_MISSING, &device->dev_state);
		}

		/* Move the device to its own fs_devices */
		if (device->fs_devices != fs_devices) {
			ASSERT(test_bit(BTRFS_DEV_STATE_MISSING,
							&device->dev_state));

			list_move(&device->dev_list, &fs_devices->devices);
			device->fs_devices->num_devices--;
			fs_devices->num_devices++;

			device->fs_devices->missing_devices--;
			fs_devices->missing_devices++;

			device->fs_devices = fs_devices;
		}
	}

	if (device->fs_devices != fs_info->fs_devices) {
		BUG_ON(test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state));
		if (device->generation !=
		    btrfs_device_generation(leaf, dev_item))
			return -EINVAL;
	}

	fill_device_from_item(leaf, dev_item, device);
<<<<<<< HEAD
=======
	if (device->bdev) {
		u64 max_total_bytes = i_size_read(device->bdev->bd_inode);

		if (device->total_bytes > max_total_bytes) {
			btrfs_err(fs_info,
			"device total_bytes should be at most %llu but found %llu",
				  max_total_bytes, device->total_bytes);
			return -EINVAL;
		}
	}
>>>>>>> upstream/android-13
	set_bit(BTRFS_DEV_STATE_IN_FS_METADATA, &device->dev_state);
	if (test_bit(BTRFS_DEV_STATE_WRITEABLE, &device->dev_state) &&
	   !test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &device->dev_state)) {
		device->fs_devices->total_rw_bytes += device->total_bytes;
		atomic64_add(device->total_bytes - device->bytes_used,
				&fs_info->free_chunk_space);
	}
	ret = 0;
	return ret;
}

int btrfs_read_sys_array(struct btrfs_fs_info *fs_info)
{
	struct btrfs_root *root = fs_info->tree_root;
	struct btrfs_super_block *super_copy = fs_info->super_copy;
	struct extent_buffer *sb;
	struct btrfs_disk_key *disk_key;
	struct btrfs_chunk *chunk;
	u8 *array_ptr;
	unsigned long sb_array_offset;
	int ret = 0;
	u32 num_stripes;
	u32 array_size;
	u32 len = 0;
	u32 cur_offset;
	u64 type;
	struct btrfs_key key;

	ASSERT(BTRFS_SUPER_INFO_SIZE <= fs_info->nodesize);
	/*
	 * This will create extent buffer of nodesize, superblock size is
	 * fixed to BTRFS_SUPER_INFO_SIZE. If nodesize > sb size, this will
	 * overallocate but we can keep it as-is, only the first page is used.
	 */
<<<<<<< HEAD
	sb = btrfs_find_create_tree_block(fs_info, BTRFS_SUPER_INFO_OFFSET);
	if (IS_ERR(sb))
		return PTR_ERR(sb);
	set_extent_buffer_uptodate(sb);
	btrfs_set_buffer_lockdep_class(root->root_key.objectid, sb, 0);
=======
	sb = btrfs_find_create_tree_block(fs_info, BTRFS_SUPER_INFO_OFFSET,
					  root->root_key.objectid, 0);
	if (IS_ERR(sb))
		return PTR_ERR(sb);
	set_extent_buffer_uptodate(sb);
>>>>>>> upstream/android-13
	/*
	 * The sb extent buffer is artificial and just used to read the system array.
	 * set_extent_buffer_uptodate() call does not properly mark all it's
	 * pages up-to-date when the page is larger: extent does not cover the
	 * whole page and consequently check_page_uptodate does not find all
	 * the page's extents up-to-date (the hole beyond sb),
	 * write_extent_buffer then triggers a WARN_ON.
	 *
	 * Regular short extents go through mark_extent_buffer_dirty/writeback cycle,
	 * but sb spans only this function. Add an explicit SetPageUptodate call
	 * to silence the warning eg. on PowerPC 64.
	 */
	if (PAGE_SIZE > BTRFS_SUPER_INFO_SIZE)
		SetPageUptodate(sb->pages[0]);

	write_extent_buffer(sb, super_copy, 0, BTRFS_SUPER_INFO_SIZE);
	array_size = btrfs_super_sys_array_size(super_copy);

	array_ptr = super_copy->sys_chunk_array;
	sb_array_offset = offsetof(struct btrfs_super_block, sys_chunk_array);
	cur_offset = 0;

	while (cur_offset < array_size) {
		disk_key = (struct btrfs_disk_key *)array_ptr;
		len = sizeof(*disk_key);
		if (cur_offset + len > array_size)
			goto out_short_read;

		btrfs_disk_key_to_cpu(&key, disk_key);

		array_ptr += len;
		sb_array_offset += len;
		cur_offset += len;

<<<<<<< HEAD
		if (key.type == BTRFS_CHUNK_ITEM_KEY) {
			chunk = (struct btrfs_chunk *)sb_array_offset;
			/*
			 * At least one btrfs_chunk with one stripe must be
			 * present, exact stripe count check comes afterwards
			 */
			len = btrfs_chunk_item_size(1);
			if (cur_offset + len > array_size)
				goto out_short_read;

			num_stripes = btrfs_chunk_num_stripes(sb, chunk);
			if (!num_stripes) {
				btrfs_err(fs_info,
					"invalid number of stripes %u in sys_array at offset %u",
					num_stripes, cur_offset);
				ret = -EIO;
				break;
			}

			type = btrfs_chunk_type(sb, chunk);
			if ((type & BTRFS_BLOCK_GROUP_SYSTEM) == 0) {
				btrfs_err(fs_info,
			    "invalid chunk type %llu in sys_array at offset %u",
					type, cur_offset);
				ret = -EIO;
				break;
			}

			len = btrfs_chunk_item_size(num_stripes);
			if (cur_offset + len > array_size)
				goto out_short_read;

			ret = read_one_chunk(fs_info, &key, sb, chunk);
			if (ret)
				break;
		} else {
=======
		if (key.type != BTRFS_CHUNK_ITEM_KEY) {
>>>>>>> upstream/android-13
			btrfs_err(fs_info,
			    "unexpected item type %u in sys_array at offset %u",
				  (u32)key.type, cur_offset);
			ret = -EIO;
			break;
		}
<<<<<<< HEAD
=======

		chunk = (struct btrfs_chunk *)sb_array_offset;
		/*
		 * At least one btrfs_chunk with one stripe must be present,
		 * exact stripe count check comes afterwards
		 */
		len = btrfs_chunk_item_size(1);
		if (cur_offset + len > array_size)
			goto out_short_read;

		num_stripes = btrfs_chunk_num_stripes(sb, chunk);
		if (!num_stripes) {
			btrfs_err(fs_info,
			"invalid number of stripes %u in sys_array at offset %u",
				  num_stripes, cur_offset);
			ret = -EIO;
			break;
		}

		type = btrfs_chunk_type(sb, chunk);
		if ((type & BTRFS_BLOCK_GROUP_SYSTEM) == 0) {
			btrfs_err(fs_info,
			"invalid chunk type %llu in sys_array at offset %u",
				  type, cur_offset);
			ret = -EIO;
			break;
		}

		len = btrfs_chunk_item_size(num_stripes);
		if (cur_offset + len > array_size)
			goto out_short_read;

		ret = read_one_chunk(&key, sb, chunk);
		if (ret)
			break;

>>>>>>> upstream/android-13
		array_ptr += len;
		sb_array_offset += len;
		cur_offset += len;
	}
	clear_extent_buffer_uptodate(sb);
	free_extent_buffer_stale(sb);
	return ret;

out_short_read:
	btrfs_err(fs_info, "sys_array too short to read %u bytes at offset %u",
			len, cur_offset);
	clear_extent_buffer_uptodate(sb);
	free_extent_buffer_stale(sb);
	return -EIO;
}

/*
 * Check if all chunks in the fs are OK for read-write degraded mount
 *
 * If the @failing_dev is specified, it's accounted as missing.
 *
 * Return true if all chunks meet the minimal RW mount requirements.
 * Return false if any chunk doesn't meet the minimal RW mount requirements.
 */
bool btrfs_check_rw_degradable(struct btrfs_fs_info *fs_info,
					struct btrfs_device *failing_dev)
{
<<<<<<< HEAD
	struct btrfs_mapping_tree *map_tree = &fs_info->mapping_tree;
=======
	struct extent_map_tree *map_tree = &fs_info->mapping_tree;
>>>>>>> upstream/android-13
	struct extent_map *em;
	u64 next_start = 0;
	bool ret = true;

<<<<<<< HEAD
	read_lock(&map_tree->map_tree.lock);
	em = lookup_extent_mapping(&map_tree->map_tree, 0, (u64)-1);
	read_unlock(&map_tree->map_tree.lock);
=======
	read_lock(&map_tree->lock);
	em = lookup_extent_mapping(map_tree, 0, (u64)-1);
	read_unlock(&map_tree->lock);
>>>>>>> upstream/android-13
	/* No chunk at all? Return false anyway */
	if (!em) {
		ret = false;
		goto out;
	}
	while (em) {
		struct map_lookup *map;
		int missing = 0;
		int max_tolerated;
		int i;

		map = em->map_lookup;
		max_tolerated =
			btrfs_get_num_tolerated_disk_barrier_failures(
					map->type);
		for (i = 0; i < map->num_stripes; i++) {
			struct btrfs_device *dev = map->stripes[i].dev;

			if (!dev || !dev->bdev ||
			    test_bit(BTRFS_DEV_STATE_MISSING, &dev->dev_state) ||
			    dev->last_flush_error)
				missing++;
			else if (failing_dev && failing_dev == dev)
				missing++;
		}
		if (missing > max_tolerated) {
			if (!failing_dev)
				btrfs_warn(fs_info,
<<<<<<< HEAD
	"chunk %llu missing %d devices, max tolerance is %d for writeable mount",
=======
	"chunk %llu missing %d devices, max tolerance is %d for writable mount",
>>>>>>> upstream/android-13
				   em->start, missing, max_tolerated);
			free_extent_map(em);
			ret = false;
			goto out;
		}
		next_start = extent_map_end(em);
		free_extent_map(em);

<<<<<<< HEAD
		read_lock(&map_tree->map_tree.lock);
		em = lookup_extent_mapping(&map_tree->map_tree, next_start,
					   (u64)(-1) - next_start);
		read_unlock(&map_tree->map_tree.lock);
=======
		read_lock(&map_tree->lock);
		em = lookup_extent_mapping(map_tree, next_start,
					   (u64)(-1) - next_start);
		read_unlock(&map_tree->lock);
>>>>>>> upstream/android-13
	}
out:
	return ret;
}

<<<<<<< HEAD
=======
static void readahead_tree_node_children(struct extent_buffer *node)
{
	int i;
	const int nr_items = btrfs_header_nritems(node);

	for (i = 0; i < nr_items; i++)
		btrfs_readahead_node_child(node, i);
}

>>>>>>> upstream/android-13
int btrfs_read_chunk_tree(struct btrfs_fs_info *fs_info)
{
	struct btrfs_root *root = fs_info->chunk_root;
	struct btrfs_path *path;
	struct extent_buffer *leaf;
	struct btrfs_key key;
	struct btrfs_key found_key;
	int ret;
	int slot;
	u64 total_dev = 0;
<<<<<<< HEAD
=======
	u64 last_ra_node = 0;
>>>>>>> upstream/android-13

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	/*
	 * uuid_mutex is needed only if we are mounting a sprout FS
	 * otherwise we don't need it.
	 */
	mutex_lock(&uuid_mutex);
<<<<<<< HEAD
	mutex_lock(&fs_info->chunk_mutex);
=======
>>>>>>> upstream/android-13

	/*
	 * It is possible for mount and umount to race in such a way that
	 * we execute this code path, but open_fs_devices failed to clear
	 * total_rw_bytes. We certainly want it cleared before reading the
	 * device items, so clear it here.
	 */
	fs_info->fs_devices->total_rw_bytes = 0;

	/*
<<<<<<< HEAD
=======
	 * Lockdep complains about possible circular locking dependency between
	 * a disk's open_mutex (struct gendisk.open_mutex), the rw semaphores
	 * used for freeze procection of a fs (struct super_block.s_writers),
	 * which we take when starting a transaction, and extent buffers of the
	 * chunk tree if we call read_one_dev() while holding a lock on an
	 * extent buffer of the chunk tree. Since we are mounting the filesystem
	 * and at this point there can't be any concurrent task modifying the
	 * chunk tree, to keep it simple, just skip locking on the chunk tree.
	 */
	ASSERT(!test_bit(BTRFS_FS_OPEN, &fs_info->flags));
	path->skip_locking = 1;

	/*
>>>>>>> upstream/android-13
	 * Read all device items, and then all the chunk items. All
	 * device items are found before any chunk item (their object id
	 * is smaller than the lowest possible object id for a chunk
	 * item - BTRFS_FIRST_CHUNK_TREE_OBJECTID).
	 */
	key.objectid = BTRFS_DEV_ITEMS_OBJECTID;
	key.offset = 0;
	key.type = 0;
	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		goto error;
	while (1) {
<<<<<<< HEAD
=======
		struct extent_buffer *node;

>>>>>>> upstream/android-13
		leaf = path->nodes[0];
		slot = path->slots[0];
		if (slot >= btrfs_header_nritems(leaf)) {
			ret = btrfs_next_leaf(root, path);
			if (ret == 0)
				continue;
			if (ret < 0)
				goto error;
			break;
		}
<<<<<<< HEAD
=======
		node = path->nodes[1];
		if (node) {
			if (last_ra_node != node->start) {
				readahead_tree_node_children(node);
				last_ra_node = node->start;
			}
		}
>>>>>>> upstream/android-13
		btrfs_item_key_to_cpu(leaf, &found_key, slot);
		if (found_key.type == BTRFS_DEV_ITEM_KEY) {
			struct btrfs_dev_item *dev_item;
			dev_item = btrfs_item_ptr(leaf, slot,
						  struct btrfs_dev_item);
<<<<<<< HEAD
			ret = read_one_dev(fs_info, leaf, dev_item);
=======
			ret = read_one_dev(leaf, dev_item);
>>>>>>> upstream/android-13
			if (ret)
				goto error;
			total_dev++;
		} else if (found_key.type == BTRFS_CHUNK_ITEM_KEY) {
			struct btrfs_chunk *chunk;
<<<<<<< HEAD
			chunk = btrfs_item_ptr(leaf, slot, struct btrfs_chunk);
			ret = read_one_chunk(fs_info, &found_key, leaf, chunk);
=======

			/*
			 * We are only called at mount time, so no need to take
			 * fs_info->chunk_mutex. Plus, to avoid lockdep warnings,
			 * we always lock first fs_info->chunk_mutex before
			 * acquiring any locks on the chunk tree. This is a
			 * requirement for chunk allocation, see the comment on
			 * top of btrfs_chunk_alloc() for details.
			 */
			chunk = btrfs_item_ptr(leaf, slot, struct btrfs_chunk);
			ret = read_one_chunk(&found_key, leaf, chunk);
>>>>>>> upstream/android-13
			if (ret)
				goto error;
		}
		path->slots[0]++;
	}

	/*
	 * After loading chunk tree, we've got all device information,
	 * do another round of validation checks.
	 */
	if (total_dev != fs_info->fs_devices->total_devices) {
		btrfs_err(fs_info,
	   "super_num_devices %llu mismatch with num_devices %llu found here",
			  btrfs_super_num_devices(fs_info->super_copy),
			  total_dev);
		ret = -EINVAL;
		goto error;
	}
	if (btrfs_super_total_bytes(fs_info->super_copy) <
	    fs_info->fs_devices->total_rw_bytes) {
		btrfs_err(fs_info,
	"super_total_bytes %llu mismatch with fs_devices total_rw_bytes %llu",
			  btrfs_super_total_bytes(fs_info->super_copy),
			  fs_info->fs_devices->total_rw_bytes);
		ret = -EINVAL;
		goto error;
	}
	ret = 0;
error:
<<<<<<< HEAD
	mutex_unlock(&fs_info->chunk_mutex);
=======
>>>>>>> upstream/android-13
	mutex_unlock(&uuid_mutex);

	btrfs_free_path(path);
	return ret;
}

void btrfs_init_devices_late(struct btrfs_fs_info *fs_info)
{
<<<<<<< HEAD
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	struct btrfs_device *device;

	while (fs_devices) {
		mutex_lock(&fs_devices->device_list_mutex);
		list_for_each_entry(device, &fs_devices->devices, dev_list)
			device->fs_info = fs_info;
		mutex_unlock(&fs_devices->device_list_mutex);

		fs_devices = fs_devices->seed;
	}
}

static void __btrfs_reset_dev_stats(struct btrfs_device *dev)
{
	int i;

	for (i = 0; i < BTRFS_DEV_STAT_VALUES_MAX; i++)
		btrfs_dev_stat_reset(dev, i);
=======
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices, *seed_devs;
	struct btrfs_device *device;

	fs_devices->fs_info = fs_info;

	mutex_lock(&fs_devices->device_list_mutex);
	list_for_each_entry(device, &fs_devices->devices, dev_list)
		device->fs_info = fs_info;

	list_for_each_entry(seed_devs, &fs_devices->seed_list, seed_list) {
		list_for_each_entry(device, &seed_devs->devices, dev_list)
			device->fs_info = fs_info;

		seed_devs->fs_info = fs_info;
	}
	mutex_unlock(&fs_devices->device_list_mutex);
}

static u64 btrfs_dev_stats_value(const struct extent_buffer *eb,
				 const struct btrfs_dev_stats_item *ptr,
				 int index)
{
	u64 val;

	read_extent_buffer(eb, &val,
			   offsetof(struct btrfs_dev_stats_item, values) +
			    ((unsigned long)ptr) + (index * sizeof(u64)),
			   sizeof(val));
	return val;
}

static void btrfs_set_dev_stats_value(struct extent_buffer *eb,
				      struct btrfs_dev_stats_item *ptr,
				      int index, u64 val)
{
	write_extent_buffer(eb, &val,
			    offsetof(struct btrfs_dev_stats_item, values) +
			     ((unsigned long)ptr) + (index * sizeof(u64)),
			    sizeof(val));
}

static int btrfs_device_init_dev_stats(struct btrfs_device *device,
				       struct btrfs_path *path)
{
	struct btrfs_dev_stats_item *ptr;
	struct extent_buffer *eb;
	struct btrfs_key key;
	int item_size;
	int i, ret, slot;

	if (!device->fs_info->dev_root)
		return 0;

	key.objectid = BTRFS_DEV_STATS_OBJECTID;
	key.type = BTRFS_PERSISTENT_ITEM_KEY;
	key.offset = device->devid;
	ret = btrfs_search_slot(NULL, device->fs_info->dev_root, &key, path, 0, 0);
	if (ret) {
		for (i = 0; i < BTRFS_DEV_STAT_VALUES_MAX; i++)
			btrfs_dev_stat_set(device, i, 0);
		device->dev_stats_valid = 1;
		btrfs_release_path(path);
		return ret < 0 ? ret : 0;
	}
	slot = path->slots[0];
	eb = path->nodes[0];
	item_size = btrfs_item_size_nr(eb, slot);

	ptr = btrfs_item_ptr(eb, slot, struct btrfs_dev_stats_item);

	for (i = 0; i < BTRFS_DEV_STAT_VALUES_MAX; i++) {
		if (item_size >= (1 + i) * sizeof(__le64))
			btrfs_dev_stat_set(device, i,
					   btrfs_dev_stats_value(eb, ptr, i));
		else
			btrfs_dev_stat_set(device, i, 0);
	}

	device->dev_stats_valid = 1;
	btrfs_dev_stat_print_on_load(device);
	btrfs_release_path(path);

	return 0;
>>>>>>> upstream/android-13
}

int btrfs_init_dev_stats(struct btrfs_fs_info *fs_info)
{
<<<<<<< HEAD
	struct btrfs_key key;
	struct btrfs_key found_key;
	struct btrfs_root *dev_root = fs_info->dev_root;
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	struct extent_buffer *eb;
	int slot;
	int ret = 0;
	struct btrfs_device *device;
	struct btrfs_path *path = NULL;
	int i;

	path = btrfs_alloc_path();
	if (!path) {
		ret = -ENOMEM;
		goto out;
	}

	mutex_lock(&fs_devices->device_list_mutex);
	list_for_each_entry(device, &fs_devices->devices, dev_list) {
		int item_size;
		struct btrfs_dev_stats_item *ptr;

		key.objectid = BTRFS_DEV_STATS_OBJECTID;
		key.type = BTRFS_PERSISTENT_ITEM_KEY;
		key.offset = device->devid;
		ret = btrfs_search_slot(NULL, dev_root, &key, path, 0, 0);
		if (ret) {
			__btrfs_reset_dev_stats(device);
			device->dev_stats_valid = 1;
			btrfs_release_path(path);
			continue;
		}
		slot = path->slots[0];
		eb = path->nodes[0];
		btrfs_item_key_to_cpu(eb, &found_key, slot);
		item_size = btrfs_item_size_nr(eb, slot);

		ptr = btrfs_item_ptr(eb, slot,
				     struct btrfs_dev_stats_item);

		for (i = 0; i < BTRFS_DEV_STAT_VALUES_MAX; i++) {
			if (item_size >= (1 + i) * sizeof(__le64))
				btrfs_dev_stat_set(device, i,
					btrfs_dev_stats_value(eb, ptr, i));
			else
				btrfs_dev_stat_reset(device, i);
		}

		device->dev_stats_valid = 1;
		btrfs_dev_stat_print_on_load(device);
		btrfs_release_path(path);
	}
	mutex_unlock(&fs_devices->device_list_mutex);

out:
	btrfs_free_path(path);
	return ret < 0 ? ret : 0;
=======
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices, *seed_devs;
	struct btrfs_device *device;
	struct btrfs_path *path = NULL;
	int ret = 0;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	mutex_lock(&fs_devices->device_list_mutex);
	list_for_each_entry(device, &fs_devices->devices, dev_list) {
		ret = btrfs_device_init_dev_stats(device, path);
		if (ret)
			goto out;
	}
	list_for_each_entry(seed_devs, &fs_devices->seed_list, seed_list) {
		list_for_each_entry(device, &seed_devs->devices, dev_list) {
			ret = btrfs_device_init_dev_stats(device, path);
			if (ret)
				goto out;
		}
	}
out:
	mutex_unlock(&fs_devices->device_list_mutex);

	btrfs_free_path(path);
	return ret;
>>>>>>> upstream/android-13
}

static int update_dev_stat_item(struct btrfs_trans_handle *trans,
				struct btrfs_device *device)
{
	struct btrfs_fs_info *fs_info = trans->fs_info;
	struct btrfs_root *dev_root = fs_info->dev_root;
	struct btrfs_path *path;
	struct btrfs_key key;
	struct extent_buffer *eb;
	struct btrfs_dev_stats_item *ptr;
	int ret;
	int i;

	key.objectid = BTRFS_DEV_STATS_OBJECTID;
	key.type = BTRFS_PERSISTENT_ITEM_KEY;
	key.offset = device->devid;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;
	ret = btrfs_search_slot(trans, dev_root, &key, path, -1, 1);
	if (ret < 0) {
		btrfs_warn_in_rcu(fs_info,
			"error %d while searching for dev_stats item for device %s",
			      ret, rcu_str_deref(device->name));
		goto out;
	}

	if (ret == 0 &&
	    btrfs_item_size_nr(path->nodes[0], path->slots[0]) < sizeof(*ptr)) {
		/* need to delete old one and insert a new one */
		ret = btrfs_del_item(trans, dev_root, path);
		if (ret != 0) {
			btrfs_warn_in_rcu(fs_info,
				"delete too small dev_stats item for device %s failed %d",
				      rcu_str_deref(device->name), ret);
			goto out;
		}
		ret = 1;
	}

	if (ret == 1) {
		/* need to insert a new item */
		btrfs_release_path(path);
		ret = btrfs_insert_empty_item(trans, dev_root, path,
					      &key, sizeof(*ptr));
		if (ret < 0) {
			btrfs_warn_in_rcu(fs_info,
				"insert dev_stats item for device %s failed %d",
				rcu_str_deref(device->name), ret);
			goto out;
		}
	}

	eb = path->nodes[0];
	ptr = btrfs_item_ptr(eb, path->slots[0], struct btrfs_dev_stats_item);
	for (i = 0; i < BTRFS_DEV_STAT_VALUES_MAX; i++)
		btrfs_set_dev_stats_value(eb, ptr, i,
					  btrfs_dev_stat_read(device, i));
	btrfs_mark_buffer_dirty(eb);

out:
	btrfs_free_path(path);
	return ret;
}

/*
 * called from commit_transaction. Writes all changed device stats to disk.
 */
<<<<<<< HEAD
int btrfs_run_dev_stats(struct btrfs_trans_handle *trans,
			struct btrfs_fs_info *fs_info)
{
=======
int btrfs_run_dev_stats(struct btrfs_trans_handle *trans)
{
	struct btrfs_fs_info *fs_info = trans->fs_info;
>>>>>>> upstream/android-13
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	struct btrfs_device *device;
	int stats_cnt;
	int ret = 0;

	mutex_lock(&fs_devices->device_list_mutex);
	list_for_each_entry(device, &fs_devices->devices, dev_list) {
		stats_cnt = atomic_read(&device->dev_stats_ccnt);
		if (!device->dev_stats_valid || stats_cnt == 0)
			continue;


		/*
		 * There is a LOAD-LOAD control dependency between the value of
		 * dev_stats_ccnt and updating the on-disk values which requires
		 * reading the in-memory counters. Such control dependencies
		 * require explicit read memory barriers.
		 *
		 * This memory barriers pairs with smp_mb__before_atomic in
		 * btrfs_dev_stat_inc/btrfs_dev_stat_set and with the full
		 * barrier implied by atomic_xchg in
		 * btrfs_dev_stats_read_and_reset
		 */
		smp_rmb();

		ret = update_dev_stat_item(trans, device);
		if (!ret)
			atomic_sub(stats_cnt, &device->dev_stats_ccnt);
	}
	mutex_unlock(&fs_devices->device_list_mutex);

	return ret;
}

void btrfs_dev_stat_inc_and_print(struct btrfs_device *dev, int index)
{
	btrfs_dev_stat_inc(dev, index);
	btrfs_dev_stat_print_on_error(dev);
}

static void btrfs_dev_stat_print_on_error(struct btrfs_device *dev)
{
	if (!dev->dev_stats_valid)
		return;
	btrfs_err_rl_in_rcu(dev->fs_info,
		"bdev %s errs: wr %u, rd %u, flush %u, corrupt %u, gen %u",
			   rcu_str_deref(dev->name),
			   btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_WRITE_ERRS),
			   btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_READ_ERRS),
			   btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_FLUSH_ERRS),
			   btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_CORRUPTION_ERRS),
			   btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_GENERATION_ERRS));
}

static void btrfs_dev_stat_print_on_load(struct btrfs_device *dev)
{
	int i;

	for (i = 0; i < BTRFS_DEV_STAT_VALUES_MAX; i++)
		if (btrfs_dev_stat_read(dev, i) != 0)
			break;
	if (i == BTRFS_DEV_STAT_VALUES_MAX)
		return; /* all values == 0, suppress message */

	btrfs_info_in_rcu(dev->fs_info,
		"bdev %s errs: wr %u, rd %u, flush %u, corrupt %u, gen %u",
	       rcu_str_deref(dev->name),
	       btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_WRITE_ERRS),
	       btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_READ_ERRS),
	       btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_FLUSH_ERRS),
	       btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_CORRUPTION_ERRS),
	       btrfs_dev_stat_read(dev, BTRFS_DEV_STAT_GENERATION_ERRS));
}

int btrfs_get_dev_stats(struct btrfs_fs_info *fs_info,
			struct btrfs_ioctl_get_dev_stats *stats)
{
	struct btrfs_device *dev;
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	int i;

	mutex_lock(&fs_devices->device_list_mutex);
<<<<<<< HEAD
	dev = btrfs_find_device(fs_info->fs_devices, stats->devid,
				NULL, NULL, true);
=======
	dev = btrfs_find_device(fs_info->fs_devices, stats->devid, NULL, NULL);
>>>>>>> upstream/android-13
	mutex_unlock(&fs_devices->device_list_mutex);

	if (!dev) {
		btrfs_warn(fs_info, "get dev_stats failed, device not found");
		return -ENODEV;
	} else if (!dev->dev_stats_valid) {
		btrfs_warn(fs_info, "get dev_stats failed, not yet valid");
		return -ENODEV;
	} else if (stats->flags & BTRFS_DEV_STATS_RESET) {
		for (i = 0; i < BTRFS_DEV_STAT_VALUES_MAX; i++) {
			if (stats->nr_items > i)
				stats->values[i] =
					btrfs_dev_stat_read_and_reset(dev, i);
			else
<<<<<<< HEAD
				btrfs_dev_stat_reset(dev, i);
=======
				btrfs_dev_stat_set(dev, i, 0);
>>>>>>> upstream/android-13
		}
		btrfs_info(fs_info, "device stats zeroed by %s (%d)",
			   current->comm, task_pid_nr(current));
	} else {
		for (i = 0; i < BTRFS_DEV_STAT_VALUES_MAX; i++)
			if (stats->nr_items > i)
				stats->values[i] = btrfs_dev_stat_read(dev, i);
	}
	if (stats->nr_items > BTRFS_DEV_STAT_VALUES_MAX)
		stats->nr_items = BTRFS_DEV_STAT_VALUES_MAX;
	return 0;
}

<<<<<<< HEAD
void btrfs_scratch_superblocks(struct block_device *bdev, const char *device_path)
{
	struct buffer_head *bh;
	struct btrfs_super_block *disk_super;
	int copy_num;

	if (!bdev)
		return;

	for (copy_num = 0; copy_num < BTRFS_SUPER_MIRROR_MAX;
		copy_num++) {

		if (btrfs_read_dev_one_super(bdev, copy_num, &bh))
			continue;

		disk_super = (struct btrfs_super_block *)bh->b_data;

		memset(&disk_super->magic, 0, sizeof(disk_super->magic));
		set_buffer_dirty(bh);
		sync_dirty_buffer(bh);
		brelse(bh);
	}

	/* Notify udev that device has changed */
	btrfs_kobject_uevent(bdev, KOBJ_CHANGE);

	/* Update ctime/mtime for device path for libblkid */
	update_dev_time(device_path);
}

/*
 * Update the size of all devices, which is used for writing out the
 * super blocks.
 */
void btrfs_update_commit_device_size(struct btrfs_fs_info *fs_info)
{
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	struct btrfs_device *curr, *next;

	if (list_empty(&fs_devices->resized_devices))
		return;

	mutex_lock(&fs_devices->device_list_mutex);
	mutex_lock(&fs_info->chunk_mutex);
	list_for_each_entry_safe(curr, next, &fs_devices->resized_devices,
				 resized_list) {
		list_del_init(&curr->resized_list);
		curr->commit_total_bytes = curr->disk_total_bytes;
	}
	mutex_unlock(&fs_info->chunk_mutex);
	mutex_unlock(&fs_devices->device_list_mutex);
}

/* Must be invoked during the transaction commit */
void btrfs_update_commit_device_bytes_used(struct btrfs_transaction *trans)
{
	struct btrfs_fs_info *fs_info = trans->fs_info;
	struct extent_map *em;
	struct map_lookup *map;
	struct btrfs_device *dev;
	int i;

	if (list_empty(&trans->pending_chunks))
		return;

	/* In order to kick the device replace finish process */
	mutex_lock(&fs_info->chunk_mutex);
	list_for_each_entry(em, &trans->pending_chunks, list) {
		map = em->map_lookup;

		for (i = 0; i < map->num_stripes; i++) {
			dev = map->stripes[i].dev;
			dev->commit_bytes_used = dev->bytes_used;
			dev->has_pending_chunks = false;
		}
	}
	mutex_unlock(&fs_info->chunk_mutex);
}

void btrfs_set_fs_info_ptr(struct btrfs_fs_info *fs_info)
{
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	while (fs_devices) {
		fs_devices->fs_info = fs_info;
		fs_devices = fs_devices->seed;
	}
}

void btrfs_reset_fs_info_ptr(struct btrfs_fs_info *fs_info)
{
	struct btrfs_fs_devices *fs_devices = fs_info->fs_devices;
	while (fs_devices) {
		fs_devices->fs_info = NULL;
		fs_devices = fs_devices->seed;
	}
=======
/*
 * Update the size and bytes used for each device where it changed.  This is
 * delayed since we would otherwise get errors while writing out the
 * superblocks.
 *
 * Must be invoked during transaction commit.
 */
void btrfs_commit_device_sizes(struct btrfs_transaction *trans)
{
	struct btrfs_device *curr, *next;

	ASSERT(trans->state == TRANS_STATE_COMMIT_DOING);

	if (list_empty(&trans->dev_update_list))
		return;

	/*
	 * We don't need the device_list_mutex here.  This list is owned by the
	 * transaction and the transaction must complete before the device is
	 * released.
	 */
	mutex_lock(&trans->fs_info->chunk_mutex);
	list_for_each_entry_safe(curr, next, &trans->dev_update_list,
				 post_commit_list) {
		list_del_init(&curr->post_commit_list);
		curr->commit_total_bytes = curr->disk_total_bytes;
		curr->commit_bytes_used = curr->bytes_used;
	}
	mutex_unlock(&trans->fs_info->chunk_mutex);
>>>>>>> upstream/android-13
}

/*
 * Multiplicity factor for simple profiles: DUP, RAID1-like and RAID10.
 */
int btrfs_bg_type_to_factor(u64 flags)
{
<<<<<<< HEAD
	if (flags & (BTRFS_BLOCK_GROUP_DUP | BTRFS_BLOCK_GROUP_RAID1 |
		     BTRFS_BLOCK_GROUP_RAID10))
		return 2;
	return 1;
}


static u64 calc_stripe_length(u64 type, u64 chunk_len, int num_stripes)
{
	int index = btrfs_bg_flags_to_raid_index(type);
	int ncopies = btrfs_raid_array[index].ncopies;
	int data_stripes;

	switch (type & BTRFS_BLOCK_GROUP_PROFILE_MASK) {
	case BTRFS_BLOCK_GROUP_RAID5:
		data_stripes = num_stripes - 1;
		break;
	case BTRFS_BLOCK_GROUP_RAID6:
		data_stripes = num_stripes - 2;
		break;
	default:
		data_stripes = num_stripes / ncopies;
		break;
	}
	return div_u64(chunk_len, data_stripes);
}
=======
	const int index = btrfs_bg_flags_to_raid_index(flags);

	return btrfs_raid_array[index].ncopies;
}


>>>>>>> upstream/android-13

static int verify_one_dev_extent(struct btrfs_fs_info *fs_info,
				 u64 chunk_offset, u64 devid,
				 u64 physical_offset, u64 physical_len)
{
<<<<<<< HEAD
	struct extent_map_tree *em_tree = &fs_info->mapping_tree.map_tree;
=======
	struct extent_map_tree *em_tree = &fs_info->mapping_tree;
>>>>>>> upstream/android-13
	struct extent_map *em;
	struct map_lookup *map;
	struct btrfs_device *dev;
	u64 stripe_len;
	bool found = false;
	int ret = 0;
	int i;

	read_lock(&em_tree->lock);
	em = lookup_extent_mapping(em_tree, chunk_offset, 1);
	read_unlock(&em_tree->lock);

	if (!em) {
		btrfs_err(fs_info,
"dev extent physical offset %llu on devid %llu doesn't have corresponding chunk",
			  physical_offset, devid);
		ret = -EUCLEAN;
		goto out;
	}

	map = em->map_lookup;
	stripe_len = calc_stripe_length(map->type, em->len, map->num_stripes);
	if (physical_len != stripe_len) {
		btrfs_err(fs_info,
"dev extent physical offset %llu on devid %llu length doesn't match chunk %llu, have %llu expect %llu",
			  physical_offset, devid, em->start, physical_len,
			  stripe_len);
		ret = -EUCLEAN;
		goto out;
	}

	for (i = 0; i < map->num_stripes; i++) {
		if (map->stripes[i].dev->devid == devid &&
		    map->stripes[i].physical == physical_offset) {
			found = true;
			if (map->verified_stripes >= map->num_stripes) {
				btrfs_err(fs_info,
				"too many dev extents for chunk %llu found",
					  em->start);
				ret = -EUCLEAN;
				goto out;
			}
			map->verified_stripes++;
			break;
		}
	}
	if (!found) {
		btrfs_err(fs_info,
	"dev extent physical offset %llu devid %llu has no corresponding chunk",
			physical_offset, devid);
		ret = -EUCLEAN;
	}

<<<<<<< HEAD
	/* Make sure no dev extent is beyond device bondary */
	dev = btrfs_find_device(fs_info->fs_devices, devid, NULL, NULL, true);
=======
	/* Make sure no dev extent is beyond device boundary */
	dev = btrfs_find_device(fs_info->fs_devices, devid, NULL, NULL);
>>>>>>> upstream/android-13
	if (!dev) {
		btrfs_err(fs_info, "failed to find devid %llu", devid);
		ret = -EUCLEAN;
		goto out;
	}

<<<<<<< HEAD
	/* It's possible this device is a dummy for seed device */
	if (dev->disk_total_bytes == 0) {
		dev = btrfs_find_device(fs_info->fs_devices->seed, devid,
					NULL, NULL, false);
		if (!dev) {
			btrfs_err(fs_info, "failed to find seed devid %llu",
				  devid);
			ret = -EUCLEAN;
			goto out;
		}
	}

=======
>>>>>>> upstream/android-13
	if (physical_offset + physical_len > dev->disk_total_bytes) {
		btrfs_err(fs_info,
"dev extent devid %llu physical offset %llu len %llu is beyond device boundary %llu",
			  devid, physical_offset, physical_len,
			  dev->disk_total_bytes);
		ret = -EUCLEAN;
		goto out;
	}
<<<<<<< HEAD
=======

	if (dev->zone_info) {
		u64 zone_size = dev->zone_info->zone_size;

		if (!IS_ALIGNED(physical_offset, zone_size) ||
		    !IS_ALIGNED(physical_len, zone_size)) {
			btrfs_err(fs_info,
"zoned: dev extent devid %llu physical offset %llu len %llu is not aligned to device zone",
				  devid, physical_offset, physical_len);
			ret = -EUCLEAN;
			goto out;
		}
	}

>>>>>>> upstream/android-13
out:
	free_extent_map(em);
	return ret;
}

static int verify_chunk_dev_extent_mapping(struct btrfs_fs_info *fs_info)
{
<<<<<<< HEAD
	struct extent_map_tree *em_tree = &fs_info->mapping_tree.map_tree;
=======
	struct extent_map_tree *em_tree = &fs_info->mapping_tree;
>>>>>>> upstream/android-13
	struct extent_map *em;
	struct rb_node *node;
	int ret = 0;

	read_lock(&em_tree->lock);
<<<<<<< HEAD
	for (node = rb_first(&em_tree->map); node; node = rb_next(node)) {
=======
	for (node = rb_first_cached(&em_tree->map); node; node = rb_next(node)) {
>>>>>>> upstream/android-13
		em = rb_entry(node, struct extent_map, rb_node);
		if (em->map_lookup->num_stripes !=
		    em->map_lookup->verified_stripes) {
			btrfs_err(fs_info,
			"chunk %llu has missing dev extent, have %d expect %d",
				  em->start, em->map_lookup->verified_stripes,
				  em->map_lookup->num_stripes);
			ret = -EUCLEAN;
			goto out;
		}
	}
out:
	read_unlock(&em_tree->lock);
	return ret;
}

/*
 * Ensure that all dev extents are mapped to correct chunk, otherwise
 * later chunk allocation/free would cause unexpected behavior.
 *
 * NOTE: This will iterate through the whole device tree, which should be of
 * the same size level as the chunk tree.  This slightly increases mount time.
 */
int btrfs_verify_dev_extents(struct btrfs_fs_info *fs_info)
{
	struct btrfs_path *path;
	struct btrfs_root *root = fs_info->dev_root;
	struct btrfs_key key;
	u64 prev_devid = 0;
	u64 prev_dev_ext_end = 0;
	int ret = 0;

<<<<<<< HEAD
=======
	/*
	 * We don't have a dev_root because we mounted with ignorebadroots and
	 * failed to load the root, so we want to skip the verification in this
	 * case for sure.
	 *
	 * However if the dev root is fine, but the tree itself is corrupted
	 * we'd still fail to mount.  This verification is only to make sure
	 * writes can happen safely, so instead just bypass this check
	 * completely in the case of IGNOREBADROOTS.
	 */
	if (btrfs_test_opt(fs_info, IGNOREBADROOTS))
		return 0;

>>>>>>> upstream/android-13
	key.objectid = 1;
	key.type = BTRFS_DEV_EXTENT_KEY;
	key.offset = 0;

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	path->reada = READA_FORWARD;
	ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
	if (ret < 0)
		goto out;

	if (path->slots[0] >= btrfs_header_nritems(path->nodes[0])) {
<<<<<<< HEAD
		ret = btrfs_next_item(root, path);
=======
		ret = btrfs_next_leaf(root, path);
>>>>>>> upstream/android-13
		if (ret < 0)
			goto out;
		/* No dev extents at all? Not good */
		if (ret > 0) {
			ret = -EUCLEAN;
			goto out;
		}
	}
	while (1) {
		struct extent_buffer *leaf = path->nodes[0];
		struct btrfs_dev_extent *dext;
		int slot = path->slots[0];
		u64 chunk_offset;
		u64 physical_offset;
		u64 physical_len;
		u64 devid;

		btrfs_item_key_to_cpu(leaf, &key, slot);
		if (key.type != BTRFS_DEV_EXTENT_KEY)
			break;
		devid = key.objectid;
		physical_offset = key.offset;

		dext = btrfs_item_ptr(leaf, slot, struct btrfs_dev_extent);
		chunk_offset = btrfs_dev_extent_chunk_offset(leaf, dext);
		physical_len = btrfs_dev_extent_length(leaf, dext);

		/* Check if this dev extent overlaps with the previous one */
		if (devid == prev_devid && physical_offset < prev_dev_ext_end) {
			btrfs_err(fs_info,
"dev extent devid %llu physical offset %llu overlap with previous dev extent end %llu",
				  devid, physical_offset, prev_dev_ext_end);
			ret = -EUCLEAN;
			goto out;
		}

		ret = verify_one_dev_extent(fs_info, chunk_offset, devid,
					    physical_offset, physical_len);
		if (ret < 0)
			goto out;
		prev_devid = devid;
		prev_dev_ext_end = physical_offset + physical_len;

		ret = btrfs_next_item(root, path);
		if (ret < 0)
			goto out;
		if (ret > 0) {
			ret = 0;
			break;
		}
	}

	/* Ensure all chunks have corresponding dev extents */
	ret = verify_chunk_dev_extent_mapping(fs_info);
out:
	btrfs_free_path(path);
	return ret;
}
<<<<<<< HEAD
=======

/*
 * Check whether the given block group or device is pinned by any inode being
 * used as a swapfile.
 */
bool btrfs_pinned_by_swapfile(struct btrfs_fs_info *fs_info, void *ptr)
{
	struct btrfs_swapfile_pin *sp;
	struct rb_node *node;

	spin_lock(&fs_info->swapfile_pins_lock);
	node = fs_info->swapfile_pins.rb_node;
	while (node) {
		sp = rb_entry(node, struct btrfs_swapfile_pin, node);
		if (ptr < sp->ptr)
			node = node->rb_left;
		else if (ptr > sp->ptr)
			node = node->rb_right;
		else
			break;
	}
	spin_unlock(&fs_info->swapfile_pins_lock);
	return node != NULL;
}

static int relocating_repair_kthread(void *data)
{
	struct btrfs_block_group *cache = (struct btrfs_block_group *)data;
	struct btrfs_fs_info *fs_info = cache->fs_info;
	u64 target;
	int ret = 0;

	target = cache->start;
	btrfs_put_block_group(cache);

	sb_start_write(fs_info->sb);
	if (!btrfs_exclop_start(fs_info, BTRFS_EXCLOP_BALANCE)) {
		btrfs_info(fs_info,
			   "zoned: skip relocating block group %llu to repair: EBUSY",
			   target);
		sb_end_write(fs_info->sb);
		return -EBUSY;
	}

	mutex_lock(&fs_info->reclaim_bgs_lock);

	/* Ensure block group still exists */
	cache = btrfs_lookup_block_group(fs_info, target);
	if (!cache)
		goto out;

	if (!cache->relocating_repair)
		goto out;

	ret = btrfs_may_alloc_data_chunk(fs_info, target);
	if (ret < 0)
		goto out;

	btrfs_info(fs_info,
		   "zoned: relocating block group %llu to repair IO failure",
		   target);
	ret = btrfs_relocate_chunk(fs_info, target);

out:
	if (cache)
		btrfs_put_block_group(cache);
	mutex_unlock(&fs_info->reclaim_bgs_lock);
	btrfs_exclop_finish(fs_info);
	sb_end_write(fs_info->sb);

	return ret;
}

int btrfs_repair_one_zone(struct btrfs_fs_info *fs_info, u64 logical)
{
	struct btrfs_block_group *cache;

	/* Do not attempt to repair in degraded state */
	if (btrfs_test_opt(fs_info, DEGRADED))
		return 0;

	cache = btrfs_lookup_block_group(fs_info, logical);
	if (!cache)
		return 0;

	spin_lock(&cache->lock);
	if (cache->relocating_repair) {
		spin_unlock(&cache->lock);
		btrfs_put_block_group(cache);
		return 0;
	}
	cache->relocating_repair = 1;
	spin_unlock(&cache->lock);

	kthread_run(relocating_repair_kthread, cache,
		    "btrfs-relocating-repair");

	return 0;
}
>>>>>>> upstream/android-13
