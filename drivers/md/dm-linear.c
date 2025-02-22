/*
 * Copyright (C) 2001-2003 Sistina Software (UK) Limited.
 *
 * This file is released under the GPL.
 */

#include "dm.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/dax.h>
#include <linux/slab.h>
#include <linux/device-mapper.h>

#define DM_MSG_PREFIX "linear"

/*
 * Linear: maps a linear range of a device.
 */
struct linear_c {
	struct dm_dev *dev;
	sector_t start;
};

/*
 * Construct a linear mapping: <dev_path> <offset>
 */
static int linear_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	struct linear_c *lc;
	unsigned long long tmp;
	char dummy;
	int ret;

	if (argc != 2) {
		ti->error = "Invalid argument count";
		return -EINVAL;
	}

	lc = kmalloc(sizeof(*lc), GFP_KERNEL);
	if (lc == NULL) {
		ti->error = "Cannot allocate linear context";
		return -ENOMEM;
	}

	ret = -EINVAL;
	if (sscanf(argv[1], "%llu%c", &tmp, &dummy) != 1 || tmp != (sector_t)tmp) {
		ti->error = "Invalid device sector";
		goto bad;
	}
	lc->start = tmp;

	ret = dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &lc->dev);
	if (ret) {
		ti->error = "Device lookup failed";
		goto bad;
	}

	ti->num_flush_bios = 1;
	ti->num_discard_bios = 1;
	ti->num_secure_erase_bios = 1;
	ti->num_write_same_bios = 1;
	ti->num_write_zeroes_bios = 1;
<<<<<<< HEAD
	ti->may_passthrough_inline_crypto = true;
=======
>>>>>>> upstream/android-13
	ti->private = lc;
	return 0;

      bad:
	kfree(lc);
	return ret;
}

static void linear_dtr(struct dm_target *ti)
{
	struct linear_c *lc = (struct linear_c *) ti->private;

	dm_put_device(ti, lc->dev);
	kfree(lc);
}

static sector_t linear_map_sector(struct dm_target *ti, sector_t bi_sector)
{
	struct linear_c *lc = ti->private;

	return lc->start + dm_target_offset(ti, bi_sector);
}

static void linear_map_bio(struct dm_target *ti, struct bio *bio)
{
	struct linear_c *lc = ti->private;

	bio_set_dev(bio, lc->dev->bdev);
<<<<<<< HEAD
	if (bio_sectors(bio) || bio_op(bio) == REQ_OP_ZONE_RESET)
=======
	if (bio_sectors(bio) || op_is_zone_mgmt(bio_op(bio)))
>>>>>>> upstream/android-13
		bio->bi_iter.bi_sector =
			linear_map_sector(ti, bio->bi_iter.bi_sector);
}

static int linear_map(struct dm_target *ti, struct bio *bio)
{
	linear_map_bio(ti, bio);

	return DM_MAPIO_REMAPPED;
}

<<<<<<< HEAD
#ifdef CONFIG_BLK_DEV_ZONED
static int linear_end_io(struct dm_target *ti, struct bio *bio,
			 blk_status_t *error)
{
	struct linear_c *lc = ti->private;

	if (!*error && bio_op(bio) == REQ_OP_ZONE_REPORT)
		dm_remap_zone_report(ti, bio, lc->start);

	return DM_ENDIO_DONE;
}
#endif

=======
>>>>>>> upstream/android-13
static void linear_status(struct dm_target *ti, status_type_t type,
			  unsigned status_flags, char *result, unsigned maxlen)
{
	struct linear_c *lc = (struct linear_c *) ti->private;
<<<<<<< HEAD
=======
	size_t sz = 0;
>>>>>>> upstream/android-13

	switch (type) {
	case STATUSTYPE_INFO:
		result[0] = '\0';
		break;

	case STATUSTYPE_TABLE:
<<<<<<< HEAD
		snprintf(result, maxlen, "%s %llu", lc->dev->name,
				(unsigned long long)lc->start);
=======
		DMEMIT("%s %llu", lc->dev->name, (unsigned long long)lc->start);
		break;

	case STATUSTYPE_IMA:
		DMEMIT_TARGET_NAME_VERSION(ti->type);
		DMEMIT(",device_name=%s,start=%llu;", lc->dev->name,
		       (unsigned long long)lc->start);
>>>>>>> upstream/android-13
		break;
	}
}

static int linear_prepare_ioctl(struct dm_target *ti, struct block_device **bdev)
{
	struct linear_c *lc = (struct linear_c *) ti->private;
	struct dm_dev *dev = lc->dev;

	*bdev = dev->bdev;

	/*
	 * Only pass ioctls through if the device sizes match exactly.
	 */
	if (lc->start ||
	    ti->len != i_size_read(dev->bdev->bd_inode) >> SECTOR_SHIFT)
		return 1;
	return 0;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_BLK_DEV_ZONED
static int linear_report_zones(struct dm_target *ti,
		struct dm_report_zones_args *args, unsigned int nr_zones)
{
	struct linear_c *lc = ti->private;

	return dm_report_zones(lc->dev->bdev, lc->start,
			       linear_map_sector(ti, args->next_sector),
			       args, nr_zones);
}
#else
#define linear_report_zones NULL
#endif

>>>>>>> upstream/android-13
static int linear_iterate_devices(struct dm_target *ti,
				  iterate_devices_callout_fn fn, void *data)
{
	struct linear_c *lc = ti->private;

	return fn(ti, lc->dev, lc->start, ti->len, data);
}

#if IS_ENABLED(CONFIG_DAX_DRIVER)
static long linear_dax_direct_access(struct dm_target *ti, pgoff_t pgoff,
		long nr_pages, void **kaddr, pfn_t *pfn)
{
	long ret;
	struct linear_c *lc = ti->private;
	struct block_device *bdev = lc->dev->bdev;
	struct dax_device *dax_dev = lc->dev->dax_dev;
	sector_t dev_sector, sector = pgoff * PAGE_SECTORS;

	dev_sector = linear_map_sector(ti, sector);
	ret = bdev_dax_pgoff(bdev, dev_sector, nr_pages * PAGE_SIZE, &pgoff);
	if (ret)
		return ret;
	return dax_direct_access(dax_dev, pgoff, nr_pages, kaddr, pfn);
}

static size_t linear_dax_copy_from_iter(struct dm_target *ti, pgoff_t pgoff,
		void *addr, size_t bytes, struct iov_iter *i)
{
	struct linear_c *lc = ti->private;
	struct block_device *bdev = lc->dev->bdev;
	struct dax_device *dax_dev = lc->dev->dax_dev;
	sector_t dev_sector, sector = pgoff * PAGE_SECTORS;

	dev_sector = linear_map_sector(ti, sector);
	if (bdev_dax_pgoff(bdev, dev_sector, ALIGN(bytes, PAGE_SIZE), &pgoff))
		return 0;
	return dax_copy_from_iter(dax_dev, pgoff, addr, bytes, i);
}

static size_t linear_dax_copy_to_iter(struct dm_target *ti, pgoff_t pgoff,
		void *addr, size_t bytes, struct iov_iter *i)
{
	struct linear_c *lc = ti->private;
	struct block_device *bdev = lc->dev->bdev;
	struct dax_device *dax_dev = lc->dev->dax_dev;
	sector_t dev_sector, sector = pgoff * PAGE_SECTORS;

	dev_sector = linear_map_sector(ti, sector);
	if (bdev_dax_pgoff(bdev, dev_sector, ALIGN(bytes, PAGE_SIZE), &pgoff))
		return 0;
	return dax_copy_to_iter(dax_dev, pgoff, addr, bytes, i);
}

<<<<<<< HEAD
=======
static int linear_dax_zero_page_range(struct dm_target *ti, pgoff_t pgoff,
				      size_t nr_pages)
{
	int ret;
	struct linear_c *lc = ti->private;
	struct block_device *bdev = lc->dev->bdev;
	struct dax_device *dax_dev = lc->dev->dax_dev;
	sector_t dev_sector, sector = pgoff * PAGE_SECTORS;

	dev_sector = linear_map_sector(ti, sector);
	ret = bdev_dax_pgoff(bdev, dev_sector, nr_pages << PAGE_SHIFT, &pgoff);
	if (ret)
		return ret;
	return dax_zero_page_range(dax_dev, pgoff, nr_pages);
}

>>>>>>> upstream/android-13
#else
#define linear_dax_direct_access NULL
#define linear_dax_copy_from_iter NULL
#define linear_dax_copy_to_iter NULL
<<<<<<< HEAD
=======
#define linear_dax_zero_page_range NULL
>>>>>>> upstream/android-13
#endif

static struct target_type linear_target = {
	.name   = "linear",
	.version = {1, 4, 0},
<<<<<<< HEAD
#ifdef CONFIG_BLK_DEV_ZONED
	.end_io = linear_end_io,
	.features = DM_TARGET_PASSES_INTEGRITY | DM_TARGET_ZONED_HM,
#else
	.features = DM_TARGET_PASSES_INTEGRITY,
#endif
=======
	.features = DM_TARGET_PASSES_INTEGRITY | DM_TARGET_NOWAIT |
		    DM_TARGET_ZONED_HM | DM_TARGET_PASSES_CRYPTO,
	.report_zones = linear_report_zones,
>>>>>>> upstream/android-13
	.module = THIS_MODULE,
	.ctr    = linear_ctr,
	.dtr    = linear_dtr,
	.map    = linear_map,
	.status = linear_status,
	.prepare_ioctl = linear_prepare_ioctl,
	.iterate_devices = linear_iterate_devices,
	.direct_access = linear_dax_direct_access,
	.dax_copy_from_iter = linear_dax_copy_from_iter,
	.dax_copy_to_iter = linear_dax_copy_to_iter,
<<<<<<< HEAD
=======
	.dax_zero_page_range = linear_dax_zero_page_range,
>>>>>>> upstream/android-13
};

int __init dm_linear_init(void)
{
	int r = dm_register_target(&linear_target);

	if (r < 0)
		DMERR("register failed %d", r);

	return r;
}

void dm_linear_exit(void)
{
	dm_unregister_target(&linear_target);
}
