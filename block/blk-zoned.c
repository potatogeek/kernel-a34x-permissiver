<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> upstream/android-13
/*
 * Zoned block device handling
 *
 * Copyright (c) 2015, Hannes Reinecke
 * Copyright (c) 2015, SUSE Linux GmbH
 *
 * Copyright (c) 2016, Damien Le Moal
 * Copyright (c) 2016, Western Digital
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/rbtree.h>
#include <linux/blkdev.h>
<<<<<<< HEAD

static inline sector_t blk_zone_start(struct request_queue *q,
				      sector_t sector)
{
	sector_t zone_mask = blk_queue_zone_sectors(q) - 1;

	return sector & ~zone_mask;
}
=======
#include <linux/blk-mq.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/sched/mm.h>

#include "blk.h"

#define ZONE_COND_NAME(name) [BLK_ZONE_COND_##name] = #name
static const char *const zone_cond_name[] = {
	ZONE_COND_NAME(NOT_WP),
	ZONE_COND_NAME(EMPTY),
	ZONE_COND_NAME(IMP_OPEN),
	ZONE_COND_NAME(EXP_OPEN),
	ZONE_COND_NAME(CLOSED),
	ZONE_COND_NAME(READONLY),
	ZONE_COND_NAME(FULL),
	ZONE_COND_NAME(OFFLINE),
};
#undef ZONE_COND_NAME

/**
 * blk_zone_cond_str - Return string XXX in BLK_ZONE_COND_XXX.
 * @zone_cond: BLK_ZONE_COND_XXX.
 *
 * Description: Centralize block layer function to convert BLK_ZONE_COND_XXX
 * into string format. Useful in the debugging and tracing zone conditions. For
 * invalid BLK_ZONE_COND_XXX it returns string "UNKNOWN".
 */
const char *blk_zone_cond_str(enum blk_zone_cond zone_cond)
{
	static const char *zone_cond_str = "UNKNOWN";

	if (zone_cond < ARRAY_SIZE(zone_cond_name) && zone_cond_name[zone_cond])
		zone_cond_str = zone_cond_name[zone_cond];

	return zone_cond_str;
}
EXPORT_SYMBOL_GPL(blk_zone_cond_str);
>>>>>>> upstream/android-13

/*
 * Return true if a request is a write requests that needs zone write locking.
 */
bool blk_req_needs_zone_write_lock(struct request *rq)
{
	if (!rq->q->seq_zones_wlock)
		return false;

	if (blk_rq_is_passthrough(rq))
		return false;

	switch (req_op(rq)) {
	case REQ_OP_WRITE_ZEROES:
	case REQ_OP_WRITE_SAME:
	case REQ_OP_WRITE:
		return blk_rq_zone_is_seq(rq);
	default:
		return false;
	}
}
EXPORT_SYMBOL_GPL(blk_req_needs_zone_write_lock);

<<<<<<< HEAD
=======
bool blk_req_zone_write_trylock(struct request *rq)
{
	unsigned int zno = blk_rq_zone_no(rq);

	if (test_and_set_bit(zno, rq->q->seq_zones_wlock))
		return false;

	WARN_ON_ONCE(rq->rq_flags & RQF_ZONE_WRITE_LOCKED);
	rq->rq_flags |= RQF_ZONE_WRITE_LOCKED;

	return true;
}
EXPORT_SYMBOL_GPL(blk_req_zone_write_trylock);

>>>>>>> upstream/android-13
void __blk_req_zone_write_lock(struct request *rq)
{
	if (WARN_ON_ONCE(test_and_set_bit(blk_rq_zone_no(rq),
					  rq->q->seq_zones_wlock)))
		return;

	WARN_ON_ONCE(rq->rq_flags & RQF_ZONE_WRITE_LOCKED);
	rq->rq_flags |= RQF_ZONE_WRITE_LOCKED;
}
EXPORT_SYMBOL_GPL(__blk_req_zone_write_lock);

void __blk_req_zone_write_unlock(struct request *rq)
{
	rq->rq_flags &= ~RQF_ZONE_WRITE_LOCKED;
	if (rq->q->seq_zones_wlock)
		WARN_ON_ONCE(!test_and_clear_bit(blk_rq_zone_no(rq),
						 rq->q->seq_zones_wlock));
}
EXPORT_SYMBOL_GPL(__blk_req_zone_write_unlock);

<<<<<<< HEAD
/*
 * Check that a zone report belongs to the partition.
 * If yes, fix its start sector and write pointer, copy it in the
 * zone information array and return true. Return false otherwise.
 */
static bool blkdev_report_zone(struct block_device *bdev,
			       struct blk_zone *rep,
			       struct blk_zone *zone)
{
	sector_t offset = get_start_sect(bdev);

	if (rep->start < offset)
		return false;

	rep->start -= offset;
	if (rep->start + rep->len > bdev->bd_part->nr_sects)
		return false;

	if (rep->type == BLK_ZONE_TYPE_CONVENTIONAL)
		rep->wp = rep->start + rep->len;
	else
		rep->wp -= offset;
	memcpy(zone, rep, sizeof(struct blk_zone));

	return true;
}
=======
/**
 * blkdev_nr_zones - Get number of zones
 * @disk:	Target gendisk
 *
 * Return the total number of zones of a zoned block device.  For a block
 * device without zone capabilities, the number of zones is always 0.
 */
unsigned int blkdev_nr_zones(struct gendisk *disk)
{
	sector_t zone_sectors = blk_queue_zone_sectors(disk->queue);

	if (!blk_queue_is_zoned(disk->queue))
		return 0;
	return (get_capacity(disk) + zone_sectors - 1) >> ilog2(zone_sectors);
}
EXPORT_SYMBOL_GPL(blkdev_nr_zones);
>>>>>>> upstream/android-13

/**
 * blkdev_report_zones - Get zones information
 * @bdev:	Target block device
 * @sector:	Sector from which to report zones
<<<<<<< HEAD
 * @zones:	Array of zone structures where to return the zones information
 * @nr_zones:	Number of zone structures in the zone array
 * @gfp_mask:	Memory allocation flags (for bio_alloc)
 *
 * Description:
 *    Get zone information starting from the zone containing @sector.
 *    The number of zone information reported may be less than the number
 *    requested by @nr_zones. The number of zones actually reported is
 *    returned in @nr_zones.
 */
int blkdev_report_zones(struct block_device *bdev,
			sector_t sector,
			struct blk_zone *zones,
			unsigned int *nr_zones,
			gfp_t gfp_mask)
{
	struct request_queue *q = bdev_get_queue(bdev);
	struct blk_zone_report_hdr *hdr;
	unsigned int nrz = *nr_zones;
	struct page *page;
	unsigned int nr_rep;
	size_t rep_bytes;
	unsigned int nr_pages;
	struct bio *bio;
	struct bio_vec *bv;
	unsigned int i, n, nz;
	unsigned int ofst;
	void *addr;
	int ret;

	if (!q)
		return -ENXIO;

	if (!blk_queue_is_zoned(q))
		return -EOPNOTSUPP;

	if (!nrz)
		return 0;

	if (sector > bdev->bd_part->nr_sects) {
		*nr_zones = 0;
		return 0;
	}

	/*
	 * The zone report has a header. So make room for it in the
	 * payload. Also make sure that the report fits in a single BIO
	 * that will not be split down the stack.
	 */
	rep_bytes = sizeof(struct blk_zone_report_hdr) +
		sizeof(struct blk_zone) * nrz;
	rep_bytes = (rep_bytes + PAGE_SIZE - 1) & PAGE_MASK;
	if (rep_bytes > (queue_max_sectors(q) << 9))
		rep_bytes = queue_max_sectors(q) << 9;

	nr_pages = min_t(unsigned int, BIO_MAX_PAGES,
			 rep_bytes >> PAGE_SHIFT);
	nr_pages = min_t(unsigned int, nr_pages,
			 queue_max_segments(q));

	bio = bio_alloc(gfp_mask, nr_pages);
	if (!bio)
		return -ENOMEM;

	bio_set_dev(bio, bdev);
	bio->bi_iter.bi_sector = blk_zone_start(q, sector);
	bio_set_op_attrs(bio, REQ_OP_ZONE_REPORT, 0);

	for (i = 0; i < nr_pages; i++) {
		page = alloc_page(gfp_mask);
		if (!page) {
			ret = -ENOMEM;
			goto out;
		}
		if (!bio_add_page(bio, page, PAGE_SIZE, 0)) {
			__free_page(page);
			break;
		}
	}

	if (i == 0)
		ret = -ENOMEM;
	else
		ret = submit_bio_wait(bio);
	if (ret)
		goto out;

	/*
	 * Process the report result: skip the header and go through the
	 * reported zones to fixup and fixup the zone information for
	 * partitions. At the same time, return the zone information into
	 * the zone array.
	 */
	n = 0;
	nz = 0;
	nr_rep = 0;
	bio_for_each_segment_all(bv, bio, i) {

		if (!bv->bv_page)
			break;

		addr = kmap_atomic(bv->bv_page);

		/* Get header in the first page */
		ofst = 0;
		if (!nr_rep) {
			hdr = addr;
			nr_rep = hdr->nr_zones;
			ofst = sizeof(struct blk_zone_report_hdr);
		}

		/* Fixup and report zones */
		while (ofst < bv->bv_len &&
		       n < nr_rep && nz < nrz) {
			if (blkdev_report_zone(bdev, addr + ofst, &zones[nz]))
				nz++;
			ofst += sizeof(struct blk_zone);
			n++;
		}

		kunmap_atomic(addr);

		if (n >= nr_rep || nz >= nrz)
			break;

	}

	*nr_zones = nz;
out:
	bio_for_each_segment_all(bv, bio, i)
		__free_page(bv->bv_page);
	bio_put(bio);

	return ret;
}
EXPORT_SYMBOL_GPL(blkdev_report_zones);

/**
 * blkdev_reset_zones - Reset zones write pointer
 * @bdev:	Target block device
 * @sector:	Start sector of the first zone to reset
 * @nr_sectors:	Number of sectors, at least the length of one zone
 * @gfp_mask:	Memory allocation flags (for bio_alloc)
 *
 * Description:
 *    Reset the write pointer of the zones contained in the range
 *    @sector..@sector+@nr_sectors. Specifying the entire disk sector range
 *    is valid, but the specified range should not contain conventional zones.
 */
int blkdev_reset_zones(struct block_device *bdev,
		       sector_t sector, sector_t nr_sectors,
		       gfp_t gfp_mask)
{
	struct request_queue *q = bdev_get_queue(bdev);
	sector_t zone_sectors;
	sector_t end_sector = sector + nr_sectors;
	struct bio *bio;
	int ret;

	if (!q)
		return -ENXIO;

	if (!blk_queue_is_zoned(q))
		return -EOPNOTSUPP;

	if (end_sector > bdev->bd_part->nr_sects)
		/* Out of range */
		return -EINVAL;

	/* Check alignment (handle eventual smaller last zone) */
	zone_sectors = blk_queue_zone_sectors(q);
	if (sector & (zone_sectors - 1))
		return -EINVAL;

	if ((nr_sectors & (zone_sectors - 1)) &&
	    end_sector != bdev->bd_part->nr_sects)
		return -EINVAL;

	while (sector < end_sector) {

		bio = bio_alloc(gfp_mask, 0);
		bio->bi_iter.bi_sector = sector;
		bio_set_dev(bio, bdev);
		bio_set_op_attrs(bio, REQ_OP_ZONE_RESET, 0);

		ret = submit_bio_wait(bio);
		bio_put(bio);

		if (ret)
			return ret;

=======
 * @nr_zones:	Maximum number of zones to report
 * @cb:		Callback function called for each reported zone
 * @data:	Private data for the callback
 *
 * Description:
 *    Get zone information starting from the zone containing @sector for at most
 *    @nr_zones, and call @cb for each zone reported by the device.
 *    To report all zones in a device starting from @sector, the BLK_ALL_ZONES
 *    constant can be passed to @nr_zones.
 *    Returns the number of zones reported by the device, or a negative errno
 *    value in case of failure.
 *
 *    Note: The caller must use memalloc_noXX_save/restore() calls to control
 *    memory allocations done within this function.
 */
int blkdev_report_zones(struct block_device *bdev, sector_t sector,
			unsigned int nr_zones, report_zones_cb cb, void *data)
{
	struct gendisk *disk = bdev->bd_disk;
	sector_t capacity = get_capacity(disk);

	if (!blk_queue_is_zoned(bdev_get_queue(bdev)) ||
	    WARN_ON_ONCE(!disk->fops->report_zones))
		return -EOPNOTSUPP;

	if (!nr_zones || sector >= capacity)
		return 0;

	return disk->fops->report_zones(disk, sector, nr_zones, cb, data);
}
EXPORT_SYMBOL_GPL(blkdev_report_zones);

static inline unsigned long *blk_alloc_zone_bitmap(int node,
						   unsigned int nr_zones)
{
	return kcalloc_node(BITS_TO_LONGS(nr_zones), sizeof(unsigned long),
			    GFP_NOIO, node);
}

static int blk_zone_need_reset_cb(struct blk_zone *zone, unsigned int idx,
				  void *data)
{
	/*
	 * For an all-zones reset, ignore conventional, empty, read-only
	 * and offline zones.
	 */
	switch (zone->cond) {
	case BLK_ZONE_COND_NOT_WP:
	case BLK_ZONE_COND_EMPTY:
	case BLK_ZONE_COND_READONLY:
	case BLK_ZONE_COND_OFFLINE:
		return 0;
	default:
		set_bit(idx, (unsigned long *)data);
		return 0;
	}
}

static int blkdev_zone_reset_all_emulated(struct block_device *bdev,
					  gfp_t gfp_mask)
{
	struct request_queue *q = bdev_get_queue(bdev);
	sector_t capacity = get_capacity(bdev->bd_disk);
	sector_t zone_sectors = blk_queue_zone_sectors(q);
	unsigned long *need_reset;
	struct bio *bio = NULL;
	sector_t sector = 0;
	int ret;

	need_reset = blk_alloc_zone_bitmap(q->node, q->nr_zones);
	if (!need_reset)
		return -ENOMEM;

	ret = bdev->bd_disk->fops->report_zones(bdev->bd_disk, 0,
				q->nr_zones, blk_zone_need_reset_cb,
				need_reset);
	if (ret < 0)
		goto out_free_need_reset;

	ret = 0;
	while (sector < capacity) {
		if (!test_bit(blk_queue_zone_no(q, sector), need_reset)) {
			sector += zone_sectors;
			continue;
		}

		bio = blk_next_bio(bio, 0, gfp_mask);
		bio_set_dev(bio, bdev);
		bio->bi_opf = REQ_OP_ZONE_RESET | REQ_SYNC;
		bio->bi_iter.bi_sector = sector;
>>>>>>> upstream/android-13
		sector += zone_sectors;

		/* This may take a while, so be nice to others */
		cond_resched();
<<<<<<< HEAD

	}

	return 0;
}
EXPORT_SYMBOL_GPL(blkdev_reset_zones);
=======
	}

	if (bio) {
		ret = submit_bio_wait(bio);
		bio_put(bio);
	}

out_free_need_reset:
	kfree(need_reset);
	return ret;
}

static int blkdev_zone_reset_all(struct block_device *bdev, gfp_t gfp_mask)
{
	struct bio bio;

	bio_init(&bio, NULL, 0);
	bio_set_dev(&bio, bdev);
	bio.bi_opf = REQ_OP_ZONE_RESET_ALL | REQ_SYNC;

	return submit_bio_wait(&bio);
}

/**
 * blkdev_zone_mgmt - Execute a zone management operation on a range of zones
 * @bdev:	Target block device
 * @op:		Operation to be performed on the zones
 * @sector:	Start sector of the first zone to operate on
 * @nr_sectors:	Number of sectors, should be at least the length of one zone and
 *		must be zone size aligned.
 * @gfp_mask:	Memory allocation flags (for bio_alloc)
 *
 * Description:
 *    Perform the specified operation on the range of zones specified by
 *    @sector..@sector+@nr_sectors. Specifying the entire disk sector range
 *    is valid, but the specified range should not contain conventional zones.
 *    The operation to execute on each zone can be a zone reset, open, close
 *    or finish request.
 */
int blkdev_zone_mgmt(struct block_device *bdev, enum req_opf op,
		     sector_t sector, sector_t nr_sectors,
		     gfp_t gfp_mask)
{
	struct request_queue *q = bdev_get_queue(bdev);
	sector_t zone_sectors = blk_queue_zone_sectors(q);
	sector_t capacity = get_capacity(bdev->bd_disk);
	sector_t end_sector = sector + nr_sectors;
	struct bio *bio = NULL;
	int ret = 0;

	if (!blk_queue_is_zoned(q))
		return -EOPNOTSUPP;

	if (bdev_read_only(bdev))
		return -EPERM;

	if (!op_is_zone_mgmt(op))
		return -EOPNOTSUPP;

	if (end_sector <= sector || end_sector > capacity)
		/* Out of range */
		return -EINVAL;

	/* Check alignment (handle eventual smaller last zone) */
	if (sector & (zone_sectors - 1))
		return -EINVAL;

	if ((nr_sectors & (zone_sectors - 1)) && end_sector != capacity)
		return -EINVAL;

	/*
	 * In the case of a zone reset operation over all zones,
	 * REQ_OP_ZONE_RESET_ALL can be used with devices supporting this
	 * command. For other devices, we emulate this command behavior by
	 * identifying the zones needing a reset.
	 */
	if (op == REQ_OP_ZONE_RESET && sector == 0 && nr_sectors == capacity) {
		if (!blk_queue_zone_resetall(q))
			return blkdev_zone_reset_all_emulated(bdev, gfp_mask);
		return blkdev_zone_reset_all(bdev, gfp_mask);
	}

	while (sector < end_sector) {
		bio = blk_next_bio(bio, 0, gfp_mask);
		bio_set_dev(bio, bdev);
		bio->bi_opf = op | REQ_SYNC;
		bio->bi_iter.bi_sector = sector;
		sector += zone_sectors;

		/* This may take a while, so be nice to others */
		cond_resched();
	}

	ret = submit_bio_wait(bio);
	bio_put(bio);

	return ret;
}
EXPORT_SYMBOL_GPL(blkdev_zone_mgmt);

struct zone_report_args {
	struct blk_zone __user *zones;
};

static int blkdev_copy_zone_to_user(struct blk_zone *zone, unsigned int idx,
				    void *data)
{
	struct zone_report_args *args = data;

	if (copy_to_user(&args->zones[idx], zone, sizeof(struct blk_zone)))
		return -EFAULT;
	return 0;
}
>>>>>>> upstream/android-13

/*
 * BLKREPORTZONE ioctl processing.
 * Called from blkdev_ioctl.
 */
int blkdev_report_zones_ioctl(struct block_device *bdev, fmode_t mode,
			      unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
<<<<<<< HEAD
	struct request_queue *q;
	struct blk_zone_report rep;
	struct blk_zone *zones;
=======
	struct zone_report_args args;
	struct request_queue *q;
	struct blk_zone_report rep;
>>>>>>> upstream/android-13
	int ret;

	if (!argp)
		return -EINVAL;

	q = bdev_get_queue(bdev);
	if (!q)
		return -ENXIO;

	if (!blk_queue_is_zoned(q))
		return -ENOTTY;

<<<<<<< HEAD
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;

=======
>>>>>>> upstream/android-13
	if (copy_from_user(&rep, argp, sizeof(struct blk_zone_report)))
		return -EFAULT;

	if (!rep.nr_zones)
		return -EINVAL;

<<<<<<< HEAD
	if (rep.nr_zones > INT_MAX / sizeof(struct blk_zone))
		return -ERANGE;

	zones = kvmalloc_array(rep.nr_zones, sizeof(struct blk_zone),
			       GFP_KERNEL | __GFP_ZERO);
	if (!zones)
		return -ENOMEM;

	ret = blkdev_report_zones(bdev, rep.sector,
				  zones, &rep.nr_zones,
				  GFP_KERNEL);
	if (ret)
		goto out;

	if (copy_to_user(argp, &rep, sizeof(struct blk_zone_report))) {
		ret = -EFAULT;
		goto out;
	}

	if (rep.nr_zones) {
		if (copy_to_user(argp + sizeof(struct blk_zone_report), zones,
				 sizeof(struct blk_zone) * rep.nr_zones))
			ret = -EFAULT;
	}

 out:
	kvfree(zones);

	return ret;
}

/*
 * BLKRESETZONE ioctl processing.
 * Called from blkdev_ioctl.
 */
int blkdev_reset_zones_ioctl(struct block_device *bdev, fmode_t mode,
			     unsigned int cmd, unsigned long arg)
=======
	args.zones = argp + sizeof(struct blk_zone_report);
	ret = blkdev_report_zones(bdev, rep.sector, rep.nr_zones,
				  blkdev_copy_zone_to_user, &args);
	if (ret < 0)
		return ret;

	rep.nr_zones = ret;
	rep.flags = BLK_ZONE_REP_CAPACITY;
	if (copy_to_user(argp, &rep, sizeof(struct blk_zone_report)))
		return -EFAULT;
	return 0;
}

static int blkdev_truncate_zone_range(struct block_device *bdev, fmode_t mode,
				      const struct blk_zone_range *zrange)
{
	loff_t start, end;

	if (zrange->sector + zrange->nr_sectors <= zrange->sector ||
	    zrange->sector + zrange->nr_sectors > get_capacity(bdev->bd_disk))
		/* Out of range */
		return -EINVAL;

	start = zrange->sector << SECTOR_SHIFT;
	end = ((zrange->sector + zrange->nr_sectors) << SECTOR_SHIFT) - 1;

	return truncate_bdev_range(bdev, mode, start, end);
}

/*
 * BLKRESETZONE, BLKOPENZONE, BLKCLOSEZONE and BLKFINISHZONE ioctl processing.
 * Called from blkdev_ioctl.
 */
int blkdev_zone_mgmt_ioctl(struct block_device *bdev, fmode_t mode,
			   unsigned int cmd, unsigned long arg)
>>>>>>> upstream/android-13
{
	void __user *argp = (void __user *)arg;
	struct request_queue *q;
	struct blk_zone_range zrange;
<<<<<<< HEAD
=======
	enum req_opf op;
	int ret;
>>>>>>> upstream/android-13

	if (!argp)
		return -EINVAL;

	q = bdev_get_queue(bdev);
	if (!q)
		return -ENXIO;

	if (!blk_queue_is_zoned(q))
		return -ENOTTY;

<<<<<<< HEAD
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;

=======
>>>>>>> upstream/android-13
	if (!(mode & FMODE_WRITE))
		return -EBADF;

	if (copy_from_user(&zrange, argp, sizeof(struct blk_zone_range)))
		return -EFAULT;

<<<<<<< HEAD
	return blkdev_reset_zones(bdev, zrange.sector, zrange.nr_sectors,
				  GFP_KERNEL);
=======
	switch (cmd) {
	case BLKRESETZONE:
		op = REQ_OP_ZONE_RESET;

		/* Invalidate the page cache, including dirty pages. */
		filemap_invalidate_lock(bdev->bd_inode->i_mapping);
		ret = blkdev_truncate_zone_range(bdev, mode, &zrange);
		if (ret)
			goto fail;
		break;
	case BLKOPENZONE:
		op = REQ_OP_ZONE_OPEN;
		break;
	case BLKCLOSEZONE:
		op = REQ_OP_ZONE_CLOSE;
		break;
	case BLKFINISHZONE:
		op = REQ_OP_ZONE_FINISH;
		break;
	default:
		return -ENOTTY;
	}

	ret = blkdev_zone_mgmt(bdev, op, zrange.sector, zrange.nr_sectors,
			       GFP_KERNEL);

fail:
	if (cmd == BLKRESETZONE)
		filemap_invalidate_unlock(bdev->bd_inode->i_mapping);

	return ret;
}

void blk_queue_free_zone_bitmaps(struct request_queue *q)
{
	kfree(q->conv_zones_bitmap);
	q->conv_zones_bitmap = NULL;
	kfree(q->seq_zones_wlock);
	q->seq_zones_wlock = NULL;
}

struct blk_revalidate_zone_args {
	struct gendisk	*disk;
	unsigned long	*conv_zones_bitmap;
	unsigned long	*seq_zones_wlock;
	unsigned int	nr_zones;
	sector_t	zone_sectors;
	sector_t	sector;
};

/*
 * Helper function to check the validity of zones of a zoned block device.
 */
static int blk_revalidate_zone_cb(struct blk_zone *zone, unsigned int idx,
				  void *data)
{
	struct blk_revalidate_zone_args *args = data;
	struct gendisk *disk = args->disk;
	struct request_queue *q = disk->queue;
	sector_t capacity = get_capacity(disk);

	/*
	 * All zones must have the same size, with the exception on an eventual
	 * smaller last zone.
	 */
	if (zone->start == 0) {
		if (zone->len == 0 || !is_power_of_2(zone->len)) {
			pr_warn("%s: Invalid zoned device with non power of two zone size (%llu)\n",
				disk->disk_name, zone->len);
			return -ENODEV;
		}

		args->zone_sectors = zone->len;
		args->nr_zones = (capacity + zone->len - 1) >> ilog2(zone->len);
	} else if (zone->start + args->zone_sectors < capacity) {
		if (zone->len != args->zone_sectors) {
			pr_warn("%s: Invalid zoned device with non constant zone size\n",
				disk->disk_name);
			return -ENODEV;
		}
	} else {
		if (zone->len > args->zone_sectors) {
			pr_warn("%s: Invalid zoned device with larger last zone size\n",
				disk->disk_name);
			return -ENODEV;
		}
	}

	/* Check for holes in the zone report */
	if (zone->start != args->sector) {
		pr_warn("%s: Zone gap at sectors %llu..%llu\n",
			disk->disk_name, args->sector, zone->start);
		return -ENODEV;
	}

	/* Check zone type */
	switch (zone->type) {
	case BLK_ZONE_TYPE_CONVENTIONAL:
		if (!args->conv_zones_bitmap) {
			args->conv_zones_bitmap =
				blk_alloc_zone_bitmap(q->node, args->nr_zones);
			if (!args->conv_zones_bitmap)
				return -ENOMEM;
		}
		set_bit(idx, args->conv_zones_bitmap);
		break;
	case BLK_ZONE_TYPE_SEQWRITE_REQ:
	case BLK_ZONE_TYPE_SEQWRITE_PREF:
		if (!args->seq_zones_wlock) {
			args->seq_zones_wlock =
				blk_alloc_zone_bitmap(q->node, args->nr_zones);
			if (!args->seq_zones_wlock)
				return -ENOMEM;
		}
		break;
	default:
		pr_warn("%s: Invalid zone type 0x%x at sectors %llu\n",
			disk->disk_name, (int)zone->type, zone->start);
		return -ENODEV;
	}

	args->sector += zone->len;
	return 0;
}

/**
 * blk_revalidate_disk_zones - (re)allocate and initialize zone bitmaps
 * @disk:	Target disk
 * @update_driver_data:	Callback to update driver data on the frozen disk
 *
 * Helper function for low-level device drivers to (re) allocate and initialize
 * a disk request queue zone bitmaps. This functions should normally be called
 * within the disk ->revalidate method for blk-mq based drivers.  For BIO based
 * drivers only q->nr_zones needs to be updated so that the sysfs exposed value
 * is correct.
 * If the @update_driver_data callback function is not NULL, the callback is
 * executed with the device request queue frozen after all zones have been
 * checked.
 */
int blk_revalidate_disk_zones(struct gendisk *disk,
			      void (*update_driver_data)(struct gendisk *disk))
{
	struct request_queue *q = disk->queue;
	struct blk_revalidate_zone_args args = {
		.disk		= disk,
	};
	unsigned int noio_flag;
	int ret;

	if (WARN_ON_ONCE(!blk_queue_is_zoned(q)))
		return -EIO;
	if (WARN_ON_ONCE(!queue_is_mq(q)))
		return -EIO;

	if (!get_capacity(disk))
		return -EIO;

	/*
	 * Ensure that all memory allocations in this context are done as if
	 * GFP_NOIO was specified.
	 */
	noio_flag = memalloc_noio_save();
	ret = disk->fops->report_zones(disk, 0, UINT_MAX,
				       blk_revalidate_zone_cb, &args);
	if (!ret) {
		pr_warn("%s: No zones reported\n", disk->disk_name);
		ret = -ENODEV;
	}
	memalloc_noio_restore(noio_flag);

	/*
	 * If zones where reported, make sure that the entire disk capacity
	 * has been checked.
	 */
	if (ret > 0 && args.sector != get_capacity(disk)) {
		pr_warn("%s: Missing zones from sector %llu\n",
			disk->disk_name, args.sector);
		ret = -ENODEV;
	}

	/*
	 * Install the new bitmaps and update nr_zones only once the queue is
	 * stopped and all I/Os are completed (i.e. a scheduler is not
	 * referencing the bitmaps).
	 */
	blk_mq_freeze_queue(q);
	if (ret > 0) {
		blk_queue_chunk_sectors(q, args.zone_sectors);
		q->nr_zones = args.nr_zones;
		swap(q->seq_zones_wlock, args.seq_zones_wlock);
		swap(q->conv_zones_bitmap, args.conv_zones_bitmap);
		if (update_driver_data)
			update_driver_data(disk);
		ret = 0;
	} else {
		pr_warn("%s: failed to revalidate zones\n", disk->disk_name);
		blk_queue_free_zone_bitmaps(q);
	}
	blk_mq_unfreeze_queue(q);

	kfree(args.seq_zones_wlock);
	kfree(args.conv_zones_bitmap);
	return ret;
}
EXPORT_SYMBOL_GPL(blk_revalidate_disk_zones);

void blk_queue_clear_zone_settings(struct request_queue *q)
{
	blk_mq_freeze_queue(q);

	blk_queue_free_zone_bitmaps(q);
	blk_queue_flag_clear(QUEUE_FLAG_ZONE_RESETALL, q);
	q->required_elevator_features &= ~ELEVATOR_F_ZBD_SEQ_WRITE;
	q->nr_zones = 0;
	q->max_open_zones = 0;
	q->max_active_zones = 0;
	q->limits.chunk_sectors = 0;
	q->limits.zone_write_granularity = 0;
	q->limits.max_zone_append_sectors = 0;

	blk_mq_unfreeze_queue(q);
>>>>>>> upstream/android-13
}
