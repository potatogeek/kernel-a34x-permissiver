// SPDX-License-Identifier: GPL-2.0
/*
 * Author(s)......: Holger Smolinski <Holger.Smolinski@de.ibm.com>
 *		    Horst Hummel <Horst.Hummel@de.ibm.com>
 *		    Carsten Otte <Cotte@de.ibm.com>
 *		    Martin Schwidefsky <schwidefsky@de.ibm.com>
 * Bugreports.to..: <Linux390@de.ibm.com>
 * Copyright IBM Corp. 1999, 2001
 *
 * gendisk related functions for the dasd driver.
 *
 */

#define KMSG_COMPONENT "dasd"

#include <linux/interrupt.h>
<<<<<<< HEAD
=======
#include <linux/major.h>
>>>>>>> upstream/android-13
#include <linux/fs.h>
#include <linux/blkpg.h>

#include <linux/uaccess.h>

/* This is ugly... */
#define PRINTK_HEADER "dasd_gendisk:"

#include "dasd_int.h"

<<<<<<< HEAD
=======
static struct lock_class_key dasd_bio_compl_lkclass;

>>>>>>> upstream/android-13
/*
 * Allocate and register gendisk structure for device.
 */
int dasd_gendisk_alloc(struct dasd_block *block)
{
	struct gendisk *gdp;
	struct dasd_device *base;
	int len;

	/* Make sure the minor for this device exists. */
	base = block->base;
	if (base->devindex >= DASD_PER_MAJOR)
		return -EBUSY;

<<<<<<< HEAD
	gdp = alloc_disk(1 << DASD_PARTN_BITS);
=======
	gdp = __alloc_disk_node(block->request_queue, NUMA_NO_NODE,
				&dasd_bio_compl_lkclass);
>>>>>>> upstream/android-13
	if (!gdp)
		return -ENOMEM;

	/* Initialize gendisk structure. */
	gdp->major = DASD_MAJOR;
	gdp->first_minor = base->devindex << DASD_PARTN_BITS;
<<<<<<< HEAD
=======
	gdp->minors = 1 << DASD_PARTN_BITS;
>>>>>>> upstream/android-13
	gdp->fops = &dasd_device_operations;

	/*
	 * Set device name.
	 *   dasda - dasdz : 26 devices
	 *   dasdaa - dasdzz : 676 devices, added up = 702
	 *   dasdaaa - dasdzzz : 17576 devices, added up = 18278
	 *   dasdaaaa - dasdzzzz : 456976 devices, added up = 475252
	 */
	len = sprintf(gdp->disk_name, "dasd");
	if (base->devindex > 25) {
		if (base->devindex > 701) {
			if (base->devindex > 18277)
			        len += sprintf(gdp->disk_name + len, "%c",
					       'a'+(((base->devindex-18278)
						     /17576)%26));
			len += sprintf(gdp->disk_name + len, "%c",
				       'a'+(((base->devindex-702)/676)%26));
		}
		len += sprintf(gdp->disk_name + len, "%c",
			       'a'+(((base->devindex-26)/26)%26));
	}
	len += sprintf(gdp->disk_name + len, "%c", 'a'+(base->devindex%26));

	if (base->features & DASD_FEATURE_READONLY ||
	    test_bit(DASD_FLAG_DEVICE_RO, &base->flags))
		set_disk_ro(gdp, 1);
	dasd_add_link_to_gendisk(gdp, base);
<<<<<<< HEAD
	gdp->queue = block->request_queue;
	block->gdp = gdp;
	set_capacity(block->gdp, 0);
	device_add_disk(&base->cdev->dev, block->gdp);
=======
	block->gdp = gdp;
	set_capacity(block->gdp, 0);
	device_add_disk(&base->cdev->dev, block->gdp, NULL);
>>>>>>> upstream/android-13
	return 0;
}

/*
 * Unregister and free gendisk structure for device.
 */
void dasd_gendisk_free(struct dasd_block *block)
{
	if (block->gdp) {
		del_gendisk(block->gdp);
		block->gdp->private_data = NULL;
		put_disk(block->gdp);
		block->gdp = NULL;
	}
}

/*
 * Trigger a partition detection.
 */
int dasd_scan_partitions(struct dasd_block *block)
{
	struct block_device *bdev;
	int rc;

<<<<<<< HEAD
	bdev = bdget_disk(block->gdp, 0);
	if (!bdev) {
		DBF_DEV_EVENT(DBF_ERR, block->base, "%s",
			      "scan partitions error, bdget returned NULL");
		return -ENODEV;
	}

	rc = blkdev_get(bdev, FMODE_READ, NULL);
	if (rc < 0) {
		DBF_DEV_EVENT(DBF_ERR, block->base,
			      "scan partitions error, blkdev_get returned %d",
			      rc);
		return -ENODEV;
	}

	rc = blkdev_reread_part(bdev);
=======
	bdev = blkdev_get_by_dev(disk_devt(block->gdp), FMODE_READ, NULL);
	if (IS_ERR(bdev)) {
		DBF_DEV_EVENT(DBF_ERR, block->base,
			      "scan partitions error, blkdev_get returned %ld",
			      PTR_ERR(bdev));
		return -ENODEV;
	}

	mutex_lock(&block->gdp->open_mutex);
	rc = bdev_disk_changed(block->gdp, false);
	mutex_unlock(&block->gdp->open_mutex);
>>>>>>> upstream/android-13
	if (rc)
		DBF_DEV_EVENT(DBF_ERR, block->base,
				"scan partitions error, rc %d", rc);

	/*
	 * Since the matching blkdev_put call to the blkdev_get in
	 * this function is not called before dasd_destroy_partitions
	 * the offline open_count limit needs to be increased from
	 * 0 to 1. This is done by setting device->bdev (see
	 * dasd_generic_set_offline). As long as the partition
	 * detection is running no offline should be allowed. That
	 * is why the assignment to device->bdev is done AFTER
	 * the BLKRRPART ioctl.
	 */
	block->bdev = bdev;
	return 0;
}

/*
 * Remove all inodes in the system for a device, delete the
 * partitions and make device unusable by setting its size to zero.
 */
void dasd_destroy_partitions(struct dasd_block *block)
{
<<<<<<< HEAD
	/* The two structs have 168/176 byte on 31/64 bit. */
	struct blkpg_partition bpart;
	struct blkpg_ioctl_arg barg;
=======
>>>>>>> upstream/android-13
	struct block_device *bdev;

	/*
	 * Get the bdev pointer from the device structure and clear
	 * device->bdev to lower the offline open_count limit again.
	 */
	bdev = block->bdev;
	block->bdev = NULL;

<<<<<<< HEAD
	/*
	 * See fs/partition/check.c:delete_partition
	 * Can't call delete_partitions directly. Use ioctl.
	 * The ioctl also does locking and invalidation.
	 */
	memset(&bpart, 0, sizeof(struct blkpg_partition));
	memset(&barg, 0, sizeof(struct blkpg_ioctl_arg));
	barg.data = (void __force __user *) &bpart;
	barg.op = BLKPG_DEL_PARTITION;
	for (bpart.pno = block->gdp->minors - 1; bpart.pno > 0; bpart.pno--)
		ioctl_by_bdev(bdev, BLKPG, (unsigned long) &barg);

	invalidate_partition(block->gdp, 0);
	/* Matching blkdev_put to the blkdev_get in dasd_scan_partitions. */
	blkdev_put(bdev, FMODE_READ);
	set_capacity(block->gdp, 0);
=======
	mutex_lock(&bdev->bd_disk->open_mutex);
	bdev_disk_changed(bdev->bd_disk, true);
	mutex_unlock(&bdev->bd_disk->open_mutex);

	/* Matching blkdev_put to the blkdev_get in dasd_scan_partitions. */
	blkdev_put(bdev, FMODE_READ);
>>>>>>> upstream/android-13
}

int dasd_gendisk_init(void)
{
	int rc;

	/* Register to static dasd major 94 */
	rc = register_blkdev(DASD_MAJOR, "dasd");
	if (rc != 0) {
		pr_warn("Registering the device driver with major number %d failed\n",
			DASD_MAJOR);
		return rc;
	}
	return 0;
}

void dasd_gendisk_exit(void)
{
	unregister_blkdev(DASD_MAJOR, "dasd");
}
