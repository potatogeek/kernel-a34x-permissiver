/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TARGET_CORE_IBLOCK_H
#define TARGET_CORE_IBLOCK_H

#include <linux/atomic.h>
#include <linux/refcount.h>
<<<<<<< HEAD
=======
#include <linux/blkdev.h>
>>>>>>> upstream/android-13
#include <target/target_core_base.h>

#define IBLOCK_VERSION		"4.0"

#define IBLOCK_MAX_CDBS		16

struct iblock_req {
	refcount_t pending;
	atomic_t ib_bio_err_cnt;
} ____cacheline_aligned;

#define IBDF_HAS_UDEV_PATH		0x01

<<<<<<< HEAD
=======
#define IBD_PLUGF_PLUGGED		0x01

struct iblock_dev_plug {
	struct se_dev_plug se_plug;
	struct blk_plug blk_plug;
	unsigned long flags;
};

>>>>>>> upstream/android-13
struct iblock_dev {
	struct se_device dev;
	unsigned char ibd_udev_path[SE_UDEV_PATH_LEN];
	u32	ibd_flags;
	struct bio_set	ibd_bio_set;
	struct block_device *ibd_bd;
	bool ibd_readonly;
<<<<<<< HEAD
=======
	struct iblock_dev_plug *ibd_plug;
>>>>>>> upstream/android-13
} ____cacheline_aligned;

#endif /* TARGET_CORE_IBLOCK_H */
