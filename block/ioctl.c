<<<<<<< HEAD
#include <linux/capability.h>
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/capability.h>
#include <linux/compat.h>
>>>>>>> upstream/android-13
#include <linux/blkdev.h>
#include <linux/export.h>
#include <linux/gfp.h>
#include <linux/blkpg.h>
#include <linux/hdreg.h>
#include <linux/backing-dev.h>
#include <linux/fs.h>
#include <linux/blktrace_api.h>
#include <linux/pr.h>
#include <linux/uaccess.h>
<<<<<<< HEAD

static int blkpg_ioctl(struct block_device *bdev, struct blkpg_ioctl_arg __user *arg)
{
	struct block_device *bdevp;
	struct gendisk *disk;
	struct hd_struct *part, *lpart;
	struct blkpg_ioctl_arg a;
	struct blkpg_partition p;
	struct disk_part_iter piter;
	long long start, length;
	int partno;

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
	if (copy_from_user(&a, arg, sizeof(struct blkpg_ioctl_arg)))
		return -EFAULT;
	if (copy_from_user(&p, a.data, sizeof(struct blkpg_partition)))
		return -EFAULT;
	disk = bdev->bd_disk;
	if (bdev != bdev->bd_contains)
		return -EINVAL;
	partno = p.pno;
	if (partno <= 0)
		return -EINVAL;
	switch (a.op) {
		case BLKPG_ADD_PARTITION:
			start = p.start >> 9;
			length = p.length >> 9;
			/* check for fit in a hd_struct */
			if (sizeof(sector_t) == sizeof(long) &&
			    sizeof(long long) > sizeof(long)) {
				long pstart = start, plength = length;
				if (pstart != start || plength != length
				    || pstart < 0 || plength < 0 || partno > 65535)
					return -EINVAL;
			}
			/* check if partition is aligned to blocksize */
			if (p.start & (bdev_logical_block_size(bdev) - 1))
				return -EINVAL;

			mutex_lock(&bdev->bd_mutex);

			/* overlap? */
			disk_part_iter_init(&piter, disk,
					    DISK_PITER_INCL_EMPTY);
			while ((part = disk_part_iter_next(&piter))) {
				if (!(start + length <= part->start_sect ||
				      start >= part->start_sect + part->nr_sects)) {
					disk_part_iter_exit(&piter);
					mutex_unlock(&bdev->bd_mutex);
					return -EBUSY;
				}
			}
			disk_part_iter_exit(&piter);

			/* all seems OK */
			part = add_partition(disk, partno, start, length,
					     ADDPART_FLAG_NONE, NULL);
			mutex_unlock(&bdev->bd_mutex);
			return PTR_ERR_OR_ZERO(part);
		case BLKPG_DEL_PARTITION:
			part = disk_get_part(disk, partno);
			if (!part)
				return -ENXIO;

			bdevp = bdget(part_devt(part));
			disk_put_part(part);
			if (!bdevp)
				return -ENOMEM;

			mutex_lock(&bdevp->bd_mutex);
			if (bdevp->bd_openers) {
				mutex_unlock(&bdevp->bd_mutex);
				bdput(bdevp);
				return -EBUSY;
			}
			/* all seems OK */
			fsync_bdev(bdevp);
			invalidate_bdev(bdevp);

			mutex_lock_nested(&bdev->bd_mutex, 1);
			delete_partition(disk, partno);
			mutex_unlock(&bdev->bd_mutex);
			mutex_unlock(&bdevp->bd_mutex);
			bdput(bdevp);

			return 0;
		case BLKPG_RESIZE_PARTITION:
			start = p.start >> 9;
			/* new length of partition in bytes */
			length = p.length >> 9;
			/* check for fit in a hd_struct */
			if (sizeof(sector_t) == sizeof(long) &&
			    sizeof(long long) > sizeof(long)) {
				long pstart = start, plength = length;
				if (pstart != start || plength != length
				    || pstart < 0 || plength < 0)
					return -EINVAL;
			}
			part = disk_get_part(disk, partno);
			if (!part)
				return -ENXIO;
			bdevp = bdget(part_devt(part));
			if (!bdevp) {
				disk_put_part(part);
				return -ENOMEM;
			}
			mutex_lock(&bdevp->bd_mutex);
			mutex_lock_nested(&bdev->bd_mutex, 1);
			if (start != part->start_sect) {
				mutex_unlock(&bdevp->bd_mutex);
				mutex_unlock(&bdev->bd_mutex);
				bdput(bdevp);
				disk_put_part(part);
				return -EINVAL;
			}
			/* overlap? */
			disk_part_iter_init(&piter, disk,
					    DISK_PITER_INCL_EMPTY);
			while ((lpart = disk_part_iter_next(&piter))) {
				if (lpart->partno != partno &&
				   !(start + length <= lpart->start_sect ||
				   start >= lpart->start_sect + lpart->nr_sects)
				   ) {
					disk_part_iter_exit(&piter);
					mutex_unlock(&bdevp->bd_mutex);
					mutex_unlock(&bdev->bd_mutex);
					bdput(bdevp);
					disk_put_part(part);
					return -EBUSY;
				}
			}
			disk_part_iter_exit(&piter);
			part_nr_sects_write(part, (sector_t)length);
			i_size_write(bdevp->bd_inode, p.length);
			mutex_unlock(&bdevp->bd_mutex);
			mutex_unlock(&bdev->bd_mutex);
			bdput(bdevp);
			disk_put_part(part);
			return 0;
		default:
			return -EINVAL;
	}
}

/*
 * This is an exported API for the block driver, and will not
 * acquire bd_mutex. This API should be used in case that
 * caller has held bd_mutex already.
 */
int __blkdev_reread_part(struct block_device *bdev)
{
	struct gendisk *disk = bdev->bd_disk;

	if (!disk_part_scan_enabled(disk) || bdev != bdev->bd_contains)
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;

	lockdep_assert_held(&bdev->bd_mutex);

	return rescan_partitions(disk, bdev);
}
EXPORT_SYMBOL(__blkdev_reread_part);

/*
 * This is an exported API for the block driver, and will
 * try to acquire bd_mutex. If bd_mutex has been held already
 * in current context, please call __blkdev_reread_part().
 *
 * Make sure the held locks in current context aren't required
 * in open()/close() handler and I/O path for avoiding ABBA deadlock:
 * - bd_mutex is held before calling block driver's open/close
 *   handler
 * - reading partition table may submit I/O to the block device
 */
int blkdev_reread_part(struct block_device *bdev)
{
	int res;

	mutex_lock(&bdev->bd_mutex);
	res = __blkdev_reread_part(bdev);
	mutex_unlock(&bdev->bd_mutex);

	return res;
}
EXPORT_SYMBOL(blkdev_reread_part);
=======
#include "blk.h"

static int blkpg_do_ioctl(struct block_device *bdev,
			  struct blkpg_partition __user *upart, int op)
{
	struct gendisk *disk = bdev->bd_disk;
	struct blkpg_partition p;
	long long start, length;

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
	if (copy_from_user(&p, upart, sizeof(struct blkpg_partition)))
		return -EFAULT;
	if (bdev_is_partition(bdev))
		return -EINVAL;

	if (p.pno <= 0)
		return -EINVAL;

	if (op == BLKPG_DEL_PARTITION)
		return bdev_del_partition(disk, p.pno);

	start = p.start >> SECTOR_SHIFT;
	length = p.length >> SECTOR_SHIFT;

	switch (op) {
	case BLKPG_ADD_PARTITION:
		/* check if partition is aligned to blocksize */
		if (p.start & (bdev_logical_block_size(bdev) - 1))
			return -EINVAL;
		return bdev_add_partition(disk, p.pno, start, length);
	case BLKPG_RESIZE_PARTITION:
		return bdev_resize_partition(disk, p.pno, start, length);
	default:
		return -EINVAL;
	}
}

static int blkpg_ioctl(struct block_device *bdev,
		       struct blkpg_ioctl_arg __user *arg)
{
	struct blkpg_partition __user *udata;
	int op;

	if (get_user(op, &arg->op) || get_user(udata, &arg->data))
		return -EFAULT;

	return blkpg_do_ioctl(bdev, udata, op);
}

#ifdef CONFIG_COMPAT
struct compat_blkpg_ioctl_arg {
	compat_int_t op;
	compat_int_t flags;
	compat_int_t datalen;
	compat_caddr_t data;
};

static int compat_blkpg_ioctl(struct block_device *bdev,
			      struct compat_blkpg_ioctl_arg __user *arg)
{
	compat_caddr_t udata;
	int op;

	if (get_user(op, &arg->op) || get_user(udata, &arg->data))
		return -EFAULT;

	return blkpg_do_ioctl(bdev, compat_ptr(udata), op);
}
#endif

static int blkdev_reread_part(struct block_device *bdev, fmode_t mode)
{
	struct block_device *tmp;

	if (!disk_part_scan_enabled(bdev->bd_disk) || bdev_is_partition(bdev))
		return -EINVAL;
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
	if (bdev->bd_disk->open_partitions)
		return -EBUSY;

	/*
	 * Reopen the device to revalidate the driver state and force a
	 * partition rescan.
	 */
	mode &= ~FMODE_EXCL;
	set_bit(GD_NEED_PART_SCAN, &bdev->bd_disk->state);

	tmp = blkdev_get_by_dev(bdev->bd_dev, mode, NULL);
	if (IS_ERR(tmp))
		return PTR_ERR(tmp);
	blkdev_put(tmp, mode);
	return 0;
}
>>>>>>> upstream/android-13

static int blk_ioctl_discard(struct block_device *bdev, fmode_t mode,
		unsigned long arg, unsigned long flags)
{
	uint64_t range[2];
	uint64_t start, len;
	struct request_queue *q = bdev_get_queue(bdev);
<<<<<<< HEAD
	struct address_space *mapping = bdev->bd_inode->i_mapping;

=======
	struct inode *inode = bdev->bd_inode;
	int err;
>>>>>>> upstream/android-13

	if (!(mode & FMODE_WRITE))
		return -EBADF;

	if (!blk_queue_discard(q))
		return -EOPNOTSUPP;

	if (copy_from_user(range, (void __user *)arg, sizeof(range)))
		return -EFAULT;

	start = range[0];
	len = range[1];

	if (start & 511)
		return -EINVAL;
	if (len & 511)
		return -EINVAL;

	if (start + len > i_size_read(bdev->bd_inode))
		return -EINVAL;
<<<<<<< HEAD
=======

>>>>>>> upstream/android-13
	printk("%s %d:%d %llu %llu",
		(flags & BLKDEV_DISCARD_SECURE) ? "SECDIS" : "DIS",
		MAJOR(bdev->bd_dev), MINOR(bdev->bd_dev),
		(unsigned long long)start, (unsigned long long)len);
<<<<<<< HEAD
	truncate_inode_pages_range(mapping, start, start + len - 1);
	return blkdev_issue_discard(bdev, start >> 9, len >> 9,
				    GFP_KERNEL, flags);
=======

	filemap_invalidate_lock(inode->i_mapping);
	err = truncate_bdev_range(bdev, mode, start, start + len - 1);
	if (err)
		goto fail;

	err = blkdev_issue_discard(bdev, start >> 9, len >> 9,
				   GFP_KERNEL, flags);

fail:
	filemap_invalidate_unlock(inode->i_mapping);
	return err;
>>>>>>> upstream/android-13
}

static int blk_ioctl_zeroout(struct block_device *bdev, fmode_t mode,
		unsigned long arg)
{
	uint64_t range[2];
<<<<<<< HEAD
	struct address_space *mapping;
	uint64_t start, end, len;
=======
	uint64_t start, end, len;
	struct inode *inode = bdev->bd_inode;
	int err;
>>>>>>> upstream/android-13

	if (!(mode & FMODE_WRITE))
		return -EBADF;

	if (copy_from_user(range, (void __user *)arg, sizeof(range)))
		return -EFAULT;

	start = range[0];
	len = range[1];
	end = start + len - 1;

	if (start & 511)
		return -EINVAL;
	if (len & 511)
		return -EINVAL;
	if (end >= (uint64_t)i_size_read(bdev->bd_inode))
		return -EINVAL;
	if (end < start)
		return -EINVAL;

	/* Invalidate the page cache, including dirty pages */
<<<<<<< HEAD
	mapping = bdev->bd_inode->i_mapping;
	truncate_inode_pages_range(mapping, start, end);

	return blkdev_issue_zeroout(bdev, start >> 9, len >> 9, GFP_KERNEL,
			BLKDEV_ZERO_NOUNMAP);
}

static int put_ushort(unsigned long arg, unsigned short val)
{
	return put_user(val, (unsigned short __user *)arg);
}

static int put_int(unsigned long arg, int val)
{
	return put_user(val, (int __user *)arg);
}

static int put_uint(unsigned long arg, unsigned int val)
{
	return put_user(val, (unsigned int __user *)arg);
}

static int put_long(unsigned long arg, long val)
{
	return put_user(val, (long __user *)arg);
}

static int put_ulong(unsigned long arg, unsigned long val)
{
	return put_user(val, (unsigned long __user *)arg);
}

static int put_u64(unsigned long arg, u64 val)
{
	return put_user(val, (u64 __user *)arg);
}

int __blkdev_driver_ioctl(struct block_device *bdev, fmode_t mode,
=======
	filemap_invalidate_lock(inode->i_mapping);
	err = truncate_bdev_range(bdev, mode, start, end);
	if (err)
		goto fail;

	err = blkdev_issue_zeroout(bdev, start >> 9, len >> 9, GFP_KERNEL,
				   BLKDEV_ZERO_NOUNMAP);

fail:
	filemap_invalidate_unlock(inode->i_mapping);
	return err;
}

static int put_ushort(unsigned short __user *argp, unsigned short val)
{
	return put_user(val, argp);
}

static int put_int(int __user *argp, int val)
{
	return put_user(val, argp);
}

static int put_uint(unsigned int __user *argp, unsigned int val)
{
	return put_user(val, argp);
}

static int put_long(long __user *argp, long val)
{
	return put_user(val, argp);
}

static int put_ulong(unsigned long __user *argp, unsigned long val)
{
	return put_user(val, argp);
}

static int put_u64(u64 __user *argp, u64 val)
{
	return put_user(val, argp);
}

#ifdef CONFIG_COMPAT
static int compat_put_long(compat_long_t __user *argp, long val)
{
	return put_user(val, argp);
}

static int compat_put_ulong(compat_ulong_t __user *argp, compat_ulong_t val)
{
	return put_user(val, argp);
}
#endif

#ifdef CONFIG_COMPAT
/*
 * This is the equivalent of compat_ptr_ioctl(), to be used by block
 * drivers that implement only commands that are completely compatible
 * between 32-bit and 64-bit user space
 */
int blkdev_compat_ptr_ioctl(struct block_device *bdev, fmode_t mode,
>>>>>>> upstream/android-13
			unsigned cmd, unsigned long arg)
{
	struct gendisk *disk = bdev->bd_disk;

	if (disk->fops->ioctl)
<<<<<<< HEAD
		return disk->fops->ioctl(bdev, mode, cmd, arg);

	return -ENOTTY;
}
/*
 * For the record: _GPL here is only because somebody decided to slap it
 * on the previous export.  Sheer idiocy, since it wasn't copyrightable
 * at all and could be open-coded without any exports by anybody who cares.
 */
EXPORT_SYMBOL_GPL(__blkdev_driver_ioctl);
=======
		return disk->fops->ioctl(bdev, mode, cmd,
					 (unsigned long)compat_ptr(arg));

	return -ENOIOCTLCMD;
}
EXPORT_SYMBOL(blkdev_compat_ptr_ioctl);
#endif
>>>>>>> upstream/android-13

static int blkdev_pr_register(struct block_device *bdev,
		struct pr_registration __user *arg)
{
	const struct pr_ops *ops = bdev->bd_disk->fops->pr_ops;
	struct pr_registration reg;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (!ops || !ops->pr_register)
		return -EOPNOTSUPP;
	if (copy_from_user(&reg, arg, sizeof(reg)))
		return -EFAULT;

	if (reg.flags & ~PR_FL_IGNORE_KEY)
		return -EOPNOTSUPP;
	return ops->pr_register(bdev, reg.old_key, reg.new_key, reg.flags);
}

static int blkdev_pr_reserve(struct block_device *bdev,
		struct pr_reservation __user *arg)
{
	const struct pr_ops *ops = bdev->bd_disk->fops->pr_ops;
	struct pr_reservation rsv;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (!ops || !ops->pr_reserve)
		return -EOPNOTSUPP;
	if (copy_from_user(&rsv, arg, sizeof(rsv)))
		return -EFAULT;

	if (rsv.flags & ~PR_FL_IGNORE_KEY)
		return -EOPNOTSUPP;
	return ops->pr_reserve(bdev, rsv.key, rsv.type, rsv.flags);
}

static int blkdev_pr_release(struct block_device *bdev,
		struct pr_reservation __user *arg)
{
	const struct pr_ops *ops = bdev->bd_disk->fops->pr_ops;
	struct pr_reservation rsv;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (!ops || !ops->pr_release)
		return -EOPNOTSUPP;
	if (copy_from_user(&rsv, arg, sizeof(rsv)))
		return -EFAULT;

	if (rsv.flags)
		return -EOPNOTSUPP;
	return ops->pr_release(bdev, rsv.key, rsv.type);
}

static int blkdev_pr_preempt(struct block_device *bdev,
		struct pr_preempt __user *arg, bool abort)
{
	const struct pr_ops *ops = bdev->bd_disk->fops->pr_ops;
	struct pr_preempt p;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (!ops || !ops->pr_preempt)
		return -EOPNOTSUPP;
	if (copy_from_user(&p, arg, sizeof(p)))
		return -EFAULT;

	if (p.flags)
		return -EOPNOTSUPP;
	return ops->pr_preempt(bdev, p.old_key, p.new_key, p.type, abort);
}

static int blkdev_pr_clear(struct block_device *bdev,
		struct pr_clear __user *arg)
{
	const struct pr_ops *ops = bdev->bd_disk->fops->pr_ops;
	struct pr_clear c;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	if (!ops || !ops->pr_clear)
		return -EOPNOTSUPP;
	if (copy_from_user(&c, arg, sizeof(c)))
		return -EFAULT;

	if (c.flags)
		return -EOPNOTSUPP;
	return ops->pr_clear(bdev, c.key);
}

<<<<<<< HEAD
/*
 * Is it an unrecognized ioctl? The correct returns are either
 * ENOTTY (final) or ENOIOCTLCMD ("I don't know this one, try a
 * fallback"). ENOIOCTLCMD gets turned into ENOTTY by the ioctl
 * code before returning.
 *
 * Confused drivers sometimes return EINVAL, which is wrong. It
 * means "I understood the ioctl command, but the parameters to
 * it were wrong".
 *
 * We should aim to just fix the broken drivers, the EINVAL case
 * should go away.
 */
static inline int is_unrecognized_ioctl(int ret)
{
	return	ret == -EINVAL ||
		ret == -ENOTTY ||
		ret == -ENOIOCTLCMD;
}

static int blkdev_flushbuf(struct block_device *bdev, fmode_t mode,
		unsigned cmd, unsigned long arg)
{
	int ret;

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;

	ret = __blkdev_driver_ioctl(bdev, mode, cmd, arg);
	if (!is_unrecognized_ioctl(ret))
		return ret;

=======
static int blkdev_flushbuf(struct block_device *bdev, fmode_t mode,
		unsigned cmd, unsigned long arg)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
>>>>>>> upstream/android-13
	fsync_bdev(bdev);
	invalidate_bdev(bdev);
	return 0;
}

static int blkdev_roset(struct block_device *bdev, fmode_t mode,
		unsigned cmd, unsigned long arg)
{
	int ret, n;

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;

<<<<<<< HEAD
	ret = __blkdev_driver_ioctl(bdev, mode, cmd, arg);
	if (!is_unrecognized_ioctl(ret))
		return ret;
	if (get_user(n, (int __user *)arg))
		return -EFAULT;
	set_device_ro(bdev, n);
=======
	if (get_user(n, (int __user *)arg))
		return -EFAULT;
	if (bdev->bd_disk->fops->set_read_only) {
		ret = bdev->bd_disk->fops->set_read_only(bdev, n);
		if (ret)
			return ret;
	}
	bdev->bd_read_only = n;
>>>>>>> upstream/android-13
	return 0;
}

static int blkdev_getgeo(struct block_device *bdev,
		struct hd_geometry __user *argp)
{
	struct gendisk *disk = bdev->bd_disk;
	struct hd_geometry geo;
	int ret;

	if (!argp)
		return -EINVAL;
	if (!disk->fops->getgeo)
		return -ENOTTY;

	/*
	 * We need to set the startsect first, the driver may
	 * want to override it.
	 */
	memset(&geo, 0, sizeof(geo));
	geo.start = get_start_sect(bdev);
	ret = disk->fops->getgeo(bdev, &geo);
	if (ret)
		return ret;
	if (copy_to_user(argp, &geo, sizeof(geo)))
		return -EFAULT;
	return 0;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_COMPAT
struct compat_hd_geometry {
	unsigned char heads;
	unsigned char sectors;
	unsigned short cylinders;
	u32 start;
};

static int compat_hdio_getgeo(struct block_device *bdev,
			      struct compat_hd_geometry __user *ugeo)
{
	struct gendisk *disk = bdev->bd_disk;
	struct hd_geometry geo;
	int ret;

	if (!ugeo)
		return -EINVAL;
	if (!disk->fops->getgeo)
		return -ENOTTY;

	memset(&geo, 0, sizeof(geo));
	/*
	 * We need to set the startsect first, the driver may
	 * want to override it.
	 */
	geo.start = get_start_sect(bdev);
	ret = disk->fops->getgeo(bdev, &geo);
	if (ret)
		return ret;

	ret = copy_to_user(ugeo, &geo, 4);
	ret |= put_user(geo.start, &ugeo->start);
	if (ret)
		ret = -EFAULT;

	return ret;
}
#endif

>>>>>>> upstream/android-13
/* set the logical block size */
static int blkdev_bszset(struct block_device *bdev, fmode_t mode,
		int __user *argp)
{
	int ret, n;

	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
	if (!argp)
		return -EINVAL;
	if (get_user(n, argp))
		return -EFAULT;

<<<<<<< HEAD
	if (!(mode & FMODE_EXCL)) {
		bdgrab(bdev);
		if (blkdev_get(bdev, mode | FMODE_EXCL, &bdev) < 0)
			return -EBUSY;
	}

	ret = set_blocksize(bdev, n);
	if (!(mode & FMODE_EXCL))
		blkdev_put(bdev, mode | FMODE_EXCL);
=======
	if (mode & FMODE_EXCL)
		return set_blocksize(bdev, n);

	if (IS_ERR(blkdev_get_by_dev(bdev->bd_dev, mode | FMODE_EXCL, &bdev)))
		return -EBUSY;
	ret = set_blocksize(bdev, n);
	blkdev_put(bdev, mode | FMODE_EXCL);

>>>>>>> upstream/android-13
	return ret;
}

/*
<<<<<<< HEAD
 * always keep this in sync with compat_blkdev_ioctl()
 */
int blkdev_ioctl(struct block_device *bdev, fmode_t mode, unsigned cmd,
			unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	loff_t size;
=======
 * Common commands that are handled the same way on native and compat
 * user space. Note the separate arg/argp parameters that are needed
 * to deal with the compat_ptr() conversion.
 */
static int blkdev_common_ioctl(struct block_device *bdev, fmode_t mode,
				unsigned cmd, unsigned long arg, void __user *argp)
{
>>>>>>> upstream/android-13
	unsigned int max_sectors;

	switch (cmd) {
	case BLKFLSBUF:
		return blkdev_flushbuf(bdev, mode, cmd, arg);
	case BLKROSET:
		return blkdev_roset(bdev, mode, cmd, arg);
	case BLKDISCARD:
		return blk_ioctl_discard(bdev, mode, arg, 0);
	case BLKSECDISCARD:
		return blk_ioctl_discard(bdev, mode, arg,
				BLKDEV_DISCARD_SECURE);
	case BLKZEROOUT:
		return blk_ioctl_zeroout(bdev, mode, arg);
<<<<<<< HEAD
	case BLKREPORTZONE:
		return blkdev_report_zones_ioctl(bdev, mode, cmd, arg);
	case BLKRESETZONE:
		return blkdev_reset_zones_ioctl(bdev, mode, cmd, arg);
	case HDIO_GETGEO:
		return blkdev_getgeo(bdev, argp);
	case BLKRAGET:
	case BLKFRAGET:
		if (!arg)
			return -EINVAL;
		return put_long(arg, (bdev->bd_bdi->ra_pages*PAGE_SIZE) / 512);
	case BLKROGET:
		return put_int(arg, bdev_read_only(bdev) != 0);
	case BLKBSZGET: /* get block device soft block size (cf. BLKSSZGET) */
		return put_int(arg, block_size(bdev));
	case BLKSSZGET: /* get block device logical block size */
		return put_int(arg, bdev_logical_block_size(bdev));
	case BLKPBSZGET: /* get block device physical block size */
		return put_uint(arg, bdev_physical_block_size(bdev));
	case BLKIOMIN:
		return put_uint(arg, bdev_io_min(bdev));
	case BLKIOOPT:
		return put_uint(arg, bdev_io_opt(bdev));
	case BLKALIGNOFF:
		return put_int(arg, bdev_alignment_offset(bdev));
	case BLKDISCARDZEROES:
		return put_uint(arg, 0);
	case BLKSECTGET:
		max_sectors = min_t(unsigned int, USHRT_MAX,
				    queue_max_sectors(bdev_get_queue(bdev)));
		return put_ushort(arg, max_sectors);
	case BLKROTATIONAL:
		return put_ushort(arg, !blk_queue_nonrot(bdev_get_queue(bdev)));
=======
	case BLKGETDISKSEQ:
		return put_u64(argp, bdev->bd_disk->diskseq);
	case BLKREPORTZONE:
		return blkdev_report_zones_ioctl(bdev, mode, cmd, arg);
	case BLKRESETZONE:
	case BLKOPENZONE:
	case BLKCLOSEZONE:
	case BLKFINISHZONE:
		return blkdev_zone_mgmt_ioctl(bdev, mode, cmd, arg);
	case BLKGETZONESZ:
		return put_uint(argp, bdev_zone_sectors(bdev));
	case BLKGETNRZONES:
		return put_uint(argp, blkdev_nr_zones(bdev->bd_disk));
	case BLKROGET:
		return put_int(argp, bdev_read_only(bdev) != 0);
	case BLKSSZGET: /* get block device logical block size */
		return put_int(argp, bdev_logical_block_size(bdev));
	case BLKPBSZGET: /* get block device physical block size */
		return put_uint(argp, bdev_physical_block_size(bdev));
	case BLKIOMIN:
		return put_uint(argp, bdev_io_min(bdev));
	case BLKIOOPT:
		return put_uint(argp, bdev_io_opt(bdev));
	case BLKALIGNOFF:
		return put_int(argp, bdev_alignment_offset(bdev));
	case BLKDISCARDZEROES:
		return put_uint(argp, 0);
	case BLKSECTGET:
		max_sectors = min_t(unsigned int, USHRT_MAX,
				    queue_max_sectors(bdev_get_queue(bdev)));
		return put_ushort(argp, max_sectors);
	case BLKROTATIONAL:
		return put_ushort(argp, !blk_queue_nonrot(bdev_get_queue(bdev)));
>>>>>>> upstream/android-13
	case BLKRASET:
	case BLKFRASET:
		if(!capable(CAP_SYS_ADMIN))
			return -EACCES;
<<<<<<< HEAD
		bdev->bd_bdi->ra_pages = (arg * 512) / PAGE_SIZE;
		return 0;
	case BLKBSZSET:
		return blkdev_bszset(bdev, mode, argp);
	case BLKPG:
		return blkpg_ioctl(bdev, argp);
	case BLKRRPART:
		return blkdev_reread_part(bdev);
	case BLKGETSIZE:
		size = i_size_read(bdev->bd_inode);
		if ((size >> 9) > ~0UL)
			return -EFBIG;
		return put_ulong(arg, size >> 9);
	case BLKGETSIZE64:
		return put_u64(arg, i_size_read(bdev->bd_inode));
	case BLKTRACESTART:
	case BLKTRACESTOP:
	case BLKTRACESETUP:
=======
		bdev->bd_disk->bdi->ra_pages = (arg * 512) / PAGE_SIZE;
		return 0;
	case BLKRRPART:
		return blkdev_reread_part(bdev, mode);
	case BLKTRACESTART:
	case BLKTRACESTOP:
>>>>>>> upstream/android-13
	case BLKTRACETEARDOWN:
		return blk_trace_ioctl(bdev, cmd, argp);
	case IOC_PR_REGISTER:
		return blkdev_pr_register(bdev, argp);
	case IOC_PR_RESERVE:
		return blkdev_pr_reserve(bdev, argp);
	case IOC_PR_RELEASE:
		return blkdev_pr_release(bdev, argp);
	case IOC_PR_PREEMPT:
		return blkdev_pr_preempt(bdev, argp, false);
	case IOC_PR_PREEMPT_ABORT:
		return blkdev_pr_preempt(bdev, argp, true);
	case IOC_PR_CLEAR:
		return blkdev_pr_clear(bdev, argp);
	default:
<<<<<<< HEAD
		return __blkdev_driver_ioctl(bdev, mode, cmd, arg);
	}
}
EXPORT_SYMBOL_GPL(blkdev_ioctl);
=======
		return -ENOIOCTLCMD;
	}
}

/*
 * Always keep this in sync with compat_blkdev_ioctl()
 * to handle all incompatible commands in both functions.
 *
 * New commands must be compatible and go into blkdev_common_ioctl
 */
int blkdev_ioctl(struct block_device *bdev, fmode_t mode, unsigned cmd,
			unsigned long arg)
{
	int ret;
	loff_t size;
	void __user *argp = (void __user *)arg;

	switch (cmd) {
	/* These need separate implementations for the data structure */
	case HDIO_GETGEO:
		return blkdev_getgeo(bdev, argp);
	case BLKPG:
		return blkpg_ioctl(bdev, argp);

	/* Compat mode returns 32-bit data instead of 'long' */
	case BLKRAGET:
	case BLKFRAGET:
		if (!argp)
			return -EINVAL;
		return put_long(argp,
			(bdev->bd_disk->bdi->ra_pages * PAGE_SIZE) / 512);
	case BLKGETSIZE:
		size = i_size_read(bdev->bd_inode);
		if ((size >> 9) > ~0UL)
			return -EFBIG;
		return put_ulong(argp, size >> 9);

	/* The data is compatible, but the command number is different */
	case BLKBSZGET: /* get block device soft block size (cf. BLKSSZGET) */
		return put_int(argp, block_size(bdev));
	case BLKBSZSET:
		return blkdev_bszset(bdev, mode, argp);
	case BLKGETSIZE64:
		return put_u64(argp, i_size_read(bdev->bd_inode));

	/* Incompatible alignment on i386 */
	case BLKTRACESETUP:
		return blk_trace_ioctl(bdev, cmd, argp);
	default:
		break;
	}

	ret = blkdev_common_ioctl(bdev, mode, cmd, arg, argp);
	if (ret != -ENOIOCTLCMD)
		return ret;

	if (!bdev->bd_disk->fops->ioctl)
		return -ENOTTY;
	return bdev->bd_disk->fops->ioctl(bdev, mode, cmd, arg);
}
EXPORT_SYMBOL_GPL(blkdev_ioctl); /* for /dev/raw */

#ifdef CONFIG_COMPAT

#define BLKBSZGET_32		_IOR(0x12, 112, int)
#define BLKBSZSET_32		_IOW(0x12, 113, int)
#define BLKGETSIZE64_32		_IOR(0x12, 114, int)

/* Most of the generic ioctls are handled in the normal fallback path.
   This assumes the blkdev's low level compat_ioctl always returns
   ENOIOCTLCMD for unknown ioctls. */
long compat_blkdev_ioctl(struct file *file, unsigned cmd, unsigned long arg)
{
	int ret;
	void __user *argp = compat_ptr(arg);
	struct block_device *bdev = I_BDEV(file->f_mapping->host);
	struct gendisk *disk = bdev->bd_disk;
	fmode_t mode = file->f_mode;
	loff_t size;

	/*
	 * O_NDELAY can be altered using fcntl(.., F_SETFL, ..), so we have
	 * to updated it before every ioctl.
	 */
	if (file->f_flags & O_NDELAY)
		mode |= FMODE_NDELAY;
	else
		mode &= ~FMODE_NDELAY;

	switch (cmd) {
	/* These need separate implementations for the data structure */
	case HDIO_GETGEO:
		return compat_hdio_getgeo(bdev, argp);
	case BLKPG:
		return compat_blkpg_ioctl(bdev, argp);

	/* Compat mode returns 32-bit data instead of 'long' */
	case BLKRAGET:
	case BLKFRAGET:
		if (!argp)
			return -EINVAL;
		return compat_put_long(argp,
			(bdev->bd_disk->bdi->ra_pages * PAGE_SIZE) / 512);
	case BLKGETSIZE:
		size = i_size_read(bdev->bd_inode);
		if ((size >> 9) > ~(compat_ulong_t)0)
			return -EFBIG;
		return compat_put_ulong(argp, size >> 9);

	/* The data is compatible, but the command number is different */
	case BLKBSZGET_32: /* get the logical block size (cf. BLKSSZGET) */
		return put_int(argp, bdev_logical_block_size(bdev));
	case BLKBSZSET_32:
		return blkdev_bszset(bdev, mode, argp);
	case BLKGETSIZE64_32:
		return put_u64(argp, i_size_read(bdev->bd_inode));

	/* Incompatible alignment on i386 */
	case BLKTRACESETUP32:
		return blk_trace_ioctl(bdev, cmd, argp);
	default:
		break;
	}

	ret = blkdev_common_ioctl(bdev, mode, cmd, arg, argp);
	if (ret == -ENOIOCTLCMD && disk->fops->compat_ioctl)
		ret = disk->fops->compat_ioctl(bdev, mode, cmd, arg);

	return ret;
}
#endif
>>>>>>> upstream/android-13
